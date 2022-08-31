//WiFi Initialisation
void WIFIinit() {
   WiFi.mode(WIFI_STA);
  byte tries = 11;
  Serial.println();
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (--tries && WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Not connected");
    Serial.println("WiFi up Ap");
    StartAPMode();
     } else {
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("Connected. IP: ");
    Serial.println(WiFi.localIP());
    WiFi.persistent(true);
  ElegantOTA.begin(&server);    // Start ElegantOTA update server
     }
}


bool StartAPMode()
{ // Отключаем WIFI
  WiFi.disconnect();
  // Меняем режим на режим точки доступа
  WiFi.mode(WIFI_AP);
  // Задаем настройки сети
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  // Включаем WIFI в режиме точки доступа с именем и паролем
  // хронящихся в переменных _ssidAP _passwordAP
  WiFi.softAP(ssidAP, passwordAP);
  return true;
}

   void MQTT_init(){  
  //MQQT initialize
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(MQTTcallback);
  byte tries=10;
  while (--tries && !client.connected()) 
  {   
     Serial.println("Connecting to MQTT...");
     if (client.connect("CatFeeder", mqtt_login,mqtt_pass))
       {
      Serial.println("connected");
      client.publish("CatFeeder/status","Ok");
      client.subscribe("CatFeeder/feed");
      }
    else
    {
      Serial.print("failed with state ");
      Serial.println(client.state());
      delay(2000);
    }
  }
  
   delay(2000);
   }


void SSDP_init(void) {
  // SSDP дескриптор
  server.on("/description.xml", HTTP_GET, []() {
    SSDP.schema(server.client());
  });
  //Если версия  2.0.0 закаментируйте следующую строчку
  SSDP.setDeviceType("upnp:rootdevice");
  SSDP.setSchemaURL("description.xml");
  SSDP.setHTTPPort(80);
  SSDP.setName(SSDP_Name);
  SSDP.setSerialNumber("001788102201");
  SSDP.setURL("/");
  SSDP.setModelName("CatFeeder");
  SSDP.setModelNumber("000000000001");
  SSDP.setManufacturer("Yarovoy Sergey");
   SSDP.begin();
}

void server_init(void) {
  server.on("/restart", handle_Restart); // Перезагрузка модуля по запросу вида http://192.168.0.101/restart?device=ok
  server.on("/configs.json", handle_ConfigJSON); // формирование configs.json страницы для передачи данных в web интерфейс
  // API для устройства
  server.on("/ssdp", handle_Set_Ssdp);     // Установить имя SSDP устройства по запросу вида /ssdp?ssdp=proba
  server.on("/ssid", handle_Set_Ssid);     // Установить имя и пароль роутера по запросу вида /ssid?ssid=home2&password=12345678
  server.on("/ssidap", handle_Set_Ssidap); // Установить имя и пароль для точки доступа по запросу вида /ssidap?ssidAP=home1&passwordAP=8765439
  server.on("/TimeZone", handle_time_zone);    // Установка времянной зоны по запросу вида http://192.168.0.101/TimeZone?timezone=3
  server.on("/Time", handle_Time);
  server.on("/FeedAmount", handle_feed_amount);    // Установка времянной зоны по запросу вида http://192.168.0.101/TimeZone?timezone=3
  server.on("/Alarms",handle_alarms);
  server.on("/feed",handle_feed);
  server.on("/time_scr",handle_time_scr);
  server.on("/status",handle_status_page);
  server.begin();
  Serial.println("HTTP server started");  
}


// Функции API-Set
// Установка SSDP имени по запросу вида http://192.168.0.101/ssdp?ssdp=proba
void handle_Set_Ssdp() {
  SSDP_Name = server.arg("ssdp"); // Получаем значение ssdp из запроса сохраняем в глобальной переменной
  saveConfig();                 // Функция сохранения данных во Flash пока пустая
  server.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
}

void handle_time_scr() {
  int temp_time_scr;
  temp_time_scr = server.arg("time_scr").toInt(); // Получаем значение time_scr из запроса сохраняем в глобальной переменной
  scr_off_ms=temp_time_scr*1000;
  if (temp_time_scr>0){
  reduceBright.setInterval(scr_off_ms);  
  }
  else reduceBright.stop();
  saveConfig();                 // Функция сохранения данных во Flash пока пустая
  server.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
}

// Установка параметров для подключения к внешней AP по запросу вида http://192.168.0.101/ssid?ssid=home2&password=12345678
void handle_Set_Ssid() {
  ssid = server.arg("ssid");            // Получаем значение ssid из запроса сохраняем в глобальной переменной
  password = server.arg("password");    // Получаем значение password из запроса сохраняем в глобальной переменной
  saveConfig();                        // Функция сохранения данных во Flash пока пустая
  server.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}

void handle_feed() {
  feed();
  server.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}

void handle_status_page (){
  String webpage;
  String tempTime;
  uint32_t sec = millis() / 1000ul;      // полное количество секунд
  int timeHours = (sec / 3600ul);        // часы
  int timeDays = timeHours/24u; //дни
  if (timeHours>23) timeHours= timeHours % 24u;
  int timeMins = (sec % 3600ul) / 60ul;  // минуты
  int timeSecs = (sec % 3600ul) % 60ul;  // секунды
  webpage="<html lang='ru'>";
  webpage+="<head>";
  webpage+="<meta http-equiv='Content-type' content='text/html; charset=utf-8'>";
  webpage+="<link rel='stylesheet' href='/bootstrap.min.css'>";
  webpage+="<link rel='stylesheet' type='text/css' href='/style.css'>";
  webpage+="<script type='text/javascript' src='/function.js'></script>";
  webpage+="<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  webpage+="<title>Информация</title>";
  webpage+="</head>";
  webpage+="<body>";
  webpage+="<div class='container'>";
  webpage+="<div class='row' style='text-align:center;'>";
  webpage+="<h1 style='margin:50px;'>Информация</h1>";
  webpage+="<div class='col-sm-offset-2 col-sm-8 col-md-offset-3 col-md-6'>";
  webpage+="<h2>Информация о сети:</h2>";
  webpage+="<div class='alert alert-dismissible alert-info'>";
  if (WiFi.getMode()!=WIFI_AP and WiFi.status() == WL_CONNECTED) {
  webpage+="<b>WiFi mode:</b> WIFI_STA<br>";
  webpage+="<b>SSID: </b>"+WiFi.SSID()+"<br>";
  webpage+="<b>IP: </b>"+WiFi.localIP().toString()+"<br>";
  webpage+="<b>Маска подсети: </b>"+WiFi.subnetMask().toString()+"<br>";
  webpage+="<b>Шлюз: </b>"+WiFi.gatewayIP().toString()+"<br>";
  webpage+="<b>RSSI: </b>"+String(WiFi.RSSI())+" dBm<br>";
  } else {
  webpage+="<b>WiFi mode: WIFI_AP</b><br>";
  webpage+="<b>Название точки доступа: </b> "+ssidAP+"<br>";
  webpage+="<b>IP: </b>"+apIP.toString()+"<br>";
  }
  webpage+="</div>";
  webpage+="<h2>Время:</h2>";
  webpage+="<div class='alert alert-dismissible alert-warning'>";
  tempTime=String(timeDays)+"дн. ";
  if (timeHours<10) tempTime+="0";
  tempTime+=String(timeHours)+"ч. ";
  if (timeMins<10) tempTime+="0";
  tempTime+=String(timeMins)+"мин. ";
  if (timeSecs<10) tempTime+="0";
  tempTime+=String(timeSecs)+"сек";
  webpage+="<b>Время работы: </b>"+tempTime+"<br>";
  webpage+="<b>Время последнего кормления: </b>";
  webpage+=last_feed+"<br><hr>";
  webpage+="<b>Время NTP:</b> "+timeClient.getFormattedTime()+"<br>";
  tempTime=rtc.gettime("H:i:s");
  webpage+="<b>Время RTC:</b> "+tempTime+"<br>";
  webpage+="</div>";
  webpage+="<h2>Информация о микроконтроллере:</h2>";
  webpage+="<div class='alert alert-dismissible alert-info'>";
  webpage+="<b>Chip ID:</b> "+String(ESP.getChipId())+"<br>"; 
  webpage+="<b>SDK Version:</b> "+String(ESP.getSdkVersion())+"<br>"; 
  webpage+="<b>Boot Version:</b> "+String(ESP.getBootVersion())+"<br>"; 
  webpage+="<b>Flash Chip Size:</b> "+String(ESP.getFlashChipSize())+"<br>"; 
  webpage+="<b>Flash Chip Real Size:</b> "+String(ESP.getFlashChipRealSize())+"<br>"; 
  webpage+="<b>Flash Chip Size by Chip ID:</b> "+String(ESP.getFlashChipSizeByChipId())+"<br>"; 
  webpage+="<b>Flash Chip ID:</b> "+String(ESP.getFlashChipId())+"<br>";
  webpage+="<b>Free Heap:</b> "+String(ESP.getFreeHeap())+"<br>"; 
  webpage+="</div>";
  webpage+="<h2>MQTT информация:</h2>";
  webpage+="<div class='alert alert-dismissible alert-info'>";
  webpage+="<b>Подключение :</b> ";
  if (client.connected()) {
    webpage+="<b>Связь с MQTT брокером установлена</b><br>";
    webpage+="<b>Сервер :</b> "+String(mqtt_server)+"<br>";
    webpage+="<b>Порт :</b> "+String(mqtt_port)+"<br>";
  }
  else webpage+="<b>Связь с MQTT брокером не установлена</b><br>";
  webpage+="<b>Состояние:</b> ";
  switch (client.state()){
    case -4: webpage+="MQTT_CONNECTION_TIMEOUT<br>";
            break;
    case -3: webpage+="MQTT_CONNECTION_LOST<br>";
            break;
    case -2: webpage+="MQTT_CONNECT_FAILED<br>";
            break;
    case -1: webpage+="MQTT_DISCONNECTED<br>";
            break;
    case 0: webpage+="MQTT_CONNECTED<br>";
            break;
    case 1: webpage+="MQTT_CONNECT_BAD_PROTOCOL<br>";
            break;
    case 2: webpage+="MQTT_CONNECT_BAD_CLIENT_ID<br>";
            break;
    case 3: webpage+="MQTT_CONNECT_UNAVAILABLE<br>";
            break;
    case 4: webpage+="MQTT_CONNECT_BAD_CREDENTIALS<br>";
            break;
    case 5: webpage+="MQTT_CONNECT_UNAUTHORIZED<br>";
            break;        
  }
  webpage+="</div>";
  webpage+="<a class='btn btn-block btn-default' href='/index.htm'>Вернуться в настройки</a>";
  webpage+="</div>";
   webpage+="</div>";
  webpage+="</div>";
 webpage+="</body>";
webpage+="</html>";
server.send(200, "text/html", webpage);
}

//Установка параметров внутренней точки доступа по запросу вида http://192.168.0.101/ssidap?ssidAP=home1&passwordAP=8765439 
void handle_Set_Ssidap() {              //
  ssidAP = server.arg("ssidAP");         // Получаем значение ssidAP из запроса сохраняем в глобальной переменной
  passwordAP = server.arg("passwordAP"); // Получаем значение passwordAP из запроса сохраняем в глобальной переменной
  saveConfig();                         // Функция сохранения данных во Flash пока пустая
  server.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}

// Установка параметров времянной зоны по запросу вида http://192.168.0.101/TimeZone?timezone=3
void handle_time_zone() {               
  timezone = server.arg("timezone").toInt(); // Получаем значение timezone из запроса конвертируем в int сохраняем в глобальной переменной
  saveConfig();
  server.send(200, "text/plain", "OK"); 
}

void handle_alarms() { 
  String Time="";              
  for (byte j=0; j<4; j++){
    Time=server.arg("alt"+String (j));
     feedTime[j][0]=Time.substring(0,2).toInt();
     feedTime[j][1]=Time.substring(3,5).toInt();
     feedTime[j][2]=server.arg("ac"+String (j)).toInt(); 
  }
  saveConfig();
  server.send(200, "text/plain", "OK"); 
}

void handle_feed_amount() {               
  feedAmount = server.arg("feedAmount").toInt(); // Получаем значение feedAmount из запроса конвертируем в int сохраняем в глобальной переменной
  saveConfig();
  server.send(200, "text/plain", "OK"); 
}

void handle_Time(){
  SetTime();
  server.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
  }

// Перезагрузка модуля по запросу вида http://192.168.0.101/restart?device=ok
void handle_Restart() {
  String restart = server.arg("device");          // Получаем значение device из запроса
  if (restart == "ok") {                         // Если значение равно Ок
    server.send(200, "text / plain", "Reset OK"); // Oтправляем ответ Reset OK
    ESP.restart();                                // перезагружаем модуль
  }
  else {                                        // иначе
    server.send(200, "text / plain", "No Reset"); // Oтправляем ответ No Reset
  }
}


void handle_ConfigJSON() {
  String output;
  int temp_time_scr;
  String root = "{}";  // Формировать строку для отправки в браузер json формат
  //{"SSDP":"SSDP-test","ssid":"home","password":"i12345678","ssidAP":"WiFi","passwordAP":"","ip":"192.168.0.101"}
  // Резервируем память для json обекта буфер может рости по мере необходимти, предпочтительно для ESP8266
  DynamicJsonBuffer jsonBuffer;
  //  вызовите парсер JSON через экземпляр jsonBuffer
  JsonObject& json = jsonBuffer.parseObject(root);
  // Заполняем поля json
  json["SSDP"] = SSDP_Name;
  json["ssidAP"] = ssidAP;
  json["passwordAP"] = passwordAP;
  json["ssid"] = ssid;
  json["password"] = password;
  json["timezone"] = timezone;
  json["ip"] = WiFi.localIP().toString();
  json["time"] = rtc.gettime("H:i:s");
  json["feedAmount"] = feedAmount;
 for (byte j=0; j<4; j++){
 output="";
 if ((feedTime[j][0])<10) output+="0";
 output+=String(feedTime[j][0])+":";
 if ((feedTime[j][1])<10) output+="0";
 output+=String(feedTime[j][1]);
 Serial.println(output);
  json["alt"+String(j)] = output;
  if (feedTime[j][2]==1) output="checked"; else (output=" ");
  json["ac"+String(j)] = output;
}
  temp_time_scr=scr_off_ms/1000;
  json["time_scr"]=temp_time_scr;
  //json["date"] = rtc.gettime("H:i:s");
  // Помещаем созданный json в переменную root
  root = "";
  json.printTo(root);
  server.send(200, "text/json", root);
}

//MQTT
void MQTTcallback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message received in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  String message;
  String command;
  int param,param2;
  for (int i = 0; i < length; i++) 
  {
    message = message + (char)payload[i];
  }
  Serial.print(message);
  command=message.substring(0,4);
  
 
 if (command=="feed"){ feed();
                  client.publish("CatFeeder/feed","CMD OK");
 }
 if (command=="feam"){  param=message.substring(4).toInt();
                  feedAmount=param;
                  client.publish("CatFeeder/feed","CMD OK");
 }
                  
 if (command=="alsw"){  param=message.substring(5,6).toInt();
                  param2=message.substring(7).toInt();
                  if (param>=0 and param<4 and param2>=0 and param2<2) feedTime[param][2]=param2;
                  client.publish("CatFeeder/feed","CMD OK");
                  }    
 
    


   Serial.println();
  Serial.println("-----------------------");
 
}
