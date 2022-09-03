/*
  Скетч к проекту "Автокормушка Wi-Fi"
  - Создан на основе оригинального скетча AlexGyver
  - Исходники этого проекта https://github.com/altJSV/CatFeeder/
  - Страница проекта AlexGyver (схемы, описания): https://alexgyver.ru/gyverfeed2/
  - Исходники скетча AlexGyver: https://github.com/AlexGyver/GyverFeed2/
*/

 byte feedTime[4][3] = {
  {6, 0, 1},                  // Часы, минуты, 0/1 выключен или включен
  {12, 0, 1},
  {18, 0, 1},
  {23, 50, 1},      
};
byte a_set=3; //вспомогательная переменная для установки будильников на экране
#define FEED_SPEED 3000     // задержка между шагами мотора  (мс)
#define STEPS_FRW 18        // Шагов вперед
#define STEPS_BKW 10        // Шагов назад
#define EEPROM_SIZE 24

#define SCL 5               // D1
#define SDA 4               // D2

// driver - pins on the board (phaseА1 D5, phaseА2 D6, phaseВ1 D7, phaseВ2 D8)
const byte drvPins[] = {14, 12, 13, 15};


// =========================================================
#include "EncButton2.h"
#include <NTPClient.h>
#include <iarduino_RTC.h> 
iarduino_RTC rtc(RTC_DS3231);
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <SPI.h>  // не используется, но нужен RTCLib для компиляции
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ElegantOTA.h>
#include <WiFiUdp.h>
#include "GyverTimer.h"
#include <PubSubClient.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <ESP8266SSDP.h>

//Настройки OLED дисплея
#define SCREEN_WIDTH 128 // OLED дисплей ширина в пикселях
#define SCREEN_HEIGHT 64 // OLED дисплей высота в пикселях
#define OLED_RESET     -1 
#define SCREEN_ADDRESS 0x3C //I2C адрес дисплея
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //инициализация экрана


//MQTT настройки
const char* mqtt_server = "ip.ad.re.ss"; //ip или http адрес
int mqtt_port = 1883; //порт
const char* mqtt_login="login"; //логин
const char* mqtt_pass="pass"; //пароль

WiFiClient espClient;
PubSubClient client(espClient);


File fsUploadFile; //для файловой системы
int timezone = 3;

//NTP настройка
      WiFiUDP ntpUDP;
      NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600*timezone);

//WiFi настройки
IPAddress apIP(192, 168, 4, 1); //IP точки доступа ESP8266
String ssid = "ssid"; //Название WIFI на роутере
String password = "pass";//Пароль WIFI роутера
String ssidAP="CatFeeder"; //Название точки доступа ESP8266
String passwordAP="12345678";//Пароль точки доступа ESP8266
String SSDP_Name = "CatFeeder"; // Имя SSDP. Под этим именем кормушка будет отображаться в сетевом окружении Windows
String last_feed="Котика никто не кормил"; //время последнего кормления

//Настройки меню
byte WorkMode = 0; //0 - рабочий режим 1-меню настроек
byte menuitem=0; //активный пункт меню
bool alarm_edit=false; //режим редактирования будильников
String jsonConfig = "{}"; //строка конфигурации для WEB морды
ESP8266WebServer server(80); //запускаем WEB сервер на 80 порту
//Установка таймеров
GTimer refscr(MS); //период обновления экрана
GTimer wificheck(MS); //проверка подключения к wifi;
GTimer rtcsync(MS); //синхронизация  RTC c NTP;
GTimer feedCheck(MS);//таймер кормления
GTimer reduceBright(MS);// снижение яркости дисплея при простое

//Объявление кнопок
EncButton2<EB_BTN> btn(INPUT,0); // D3 кнопка В1
EncButton2<EB_BTN> btn2(INPUT,2); //D4 кнопка В2
byte feedAmount = 25; //Размер порции. Нужен для первичной настройки. Далее будет читаться из конфига

//Прочие настройки
bool low_bright=false;
long scr_off_ms=60000;

void setup() {
  Serial.begin(115200);//Открытия COM порта для отладки
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(2000); // Pause for 2 seconds

  // Вывод отладочной информации на OLED
  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(0, 4);

    Wire.begin();                       // I2C
    rtc.begin(); //Старт RTC
    timeClient.begin(); //NTP запуск склиента
    display.println("RTC OK..");
    FS_init();//Запуск файловой системы
    loadConfig(); //Чтение параметров из файла конфигурации
    WIFIinit();                         // Старт Wi-Fi
    display.println(WiFi.localIP());
    server_init(); //Запуск сервера
    if (WiFi.status() == WL_CONNECTED){
    display.print("Connected to ");
    display.println(ssid);  
    SSDP_init();
    MQTT_init();
    if (client.connected()) display.println ("MQTT OK");
    SetTime();
    display.println("RTC sync OK");
    }
    display.display();
refscr.setInterval(1000); //Интервал обновления экрана дисплея 1 секунда
wificheck.setInterval(60000);// Интервал между проверками соединения с wifi 1 минута
rtcsync.setInterval(3600000); //синхронизация  RTC c NTP каждый час
feedCheck.setInterval(500); //проверка таймеров кормления 1 раз в 31 секунду
if (scr_off_ms>0) reduceBright.setInterval(scr_off_ms); //снижение яркости при простое;
for (byte i = 0; i < 4; i++) pinMode(drvPins[i], OUTPUT);   // инициализация пинов драйвера шаговика
delay(5000);

}


void loop() {
   server.handleClient();//Веб сервер обновление страницы
 
    buttons_check(); //проверка нажатия кнопок
    timeClient.update();// синхронизация времени по NTP
  if (refscr.isReady()) { //обновление экрана
    draw_display();
  }
  if (rtcsync.isReady()) {
                          if (WiFi.status() == WL_CONNECTED) SetTime(); //Синхронизация времени rtc c ntp
                          }
                          
  if (wificheck.isReady()) {if (WiFi.getMode() !=WIFI_AP) {//Переподключение к WiFi если недоступен
                          if (WiFi.status() != WL_CONNECTED) {
                            WiFi.reconnect();
                            SSDP_init();
                            
                          }
                           }
                           if (!client.connected()) MQTT_init();
                           }

   if (feedCheck.isReady()){  //сработал будильник. Покормим котика
    static byte prevMin = 0;
    rtc.gettime();
    if (prevMin != rtc.minutes) {
      prevMin = rtc.minutes;
        for (byte i = 0; i < 4; i++)    // for the whole schedule
        if (feedTime[i][0] == rtc.Hours && feedTime[i][1] == rtc.minutes ) {   // время кормления
          feed();
        }
    }
   }


  if (scr_off_ms>0){
  if (reduceBright.isReady()){ //снижение яркости при простое
    if (low_bright==false){
      low_bright=true;
      display.dim(low_bright);
    }
    }
}
  client.loop(); //MQQT читает топики
  


}
