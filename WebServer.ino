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
  for (int i = 0; i < length; i++) 
  {
    message = message + (char)payload[i];
  }
  Serial.print(message);
  if (message == "feed") 
  {
    feed();
    client.publish("CatFeeder/feed","stop");
  }
   Serial.println();
  Serial.println("-----------------------");
 
}
