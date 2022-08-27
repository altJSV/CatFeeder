// Загрузка данных сохраненных в файл  config.json
bool loadConfig() {
  // Открываем файл для чтения
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
  // если файл не найден  
    Serial.println("Failed to open config file");
  //  Создаем файл запиав в него аные по умолчанию
    saveConfig();
    configFile.close();
    return false;
  }
  // Проверяем размер файла, будем использовать файл размером меньше 1024 байта
  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    configFile.close();
    return false;
  }

// загружаем файл конфигурации в глобальную переменную
  jsonConfig = configFile.readString();
  configFile.close();
  // Резервируем память для json обекта буфер может рости по мере необходимти предпочтительно для ESP8266 
    DynamicJsonBuffer jsonBuffer;
  //  вызовите парсер JSON через экземпляр jsonBuffer
  //  строку возьмем из глобальной переменной String jsonConfig
    JsonObject& root = jsonBuffer.parseObject(jsonConfig);
  // Теперь можно получить значения из root  
    ssidAP = root["ssidAPName"].as<String>(); // Так получаем строку
    passwordAP = root["ssidAPPassword"].as<String>();
    timezone = root["timezone"];               // Так получаем число
    SSDP_Name = root["SSDPName"].as<String>();
    ssid = root["ssidName"].as<String>();
    password = root["ssidPassword"].as<String>();
    feedAmount = root["feedAmount"];
    for (byte j=0; j<4; j++){
    feedTime[j][0]=root["ah"+String(j)];
     feedTime[j][1]=root["am"+String(j)];
      feedTime[j][2]=root["ac"+String(j)];
  }
    return true;
}

// Запись данных в файл config.json
bool saveConfig() {
  
  // Резервируем памяь для json обекта буфер может рости по мере необходимти предпочтительно для ESP8266 
  DynamicJsonBuffer jsonBuffer;
  //  вызовите парсер JSON через экземпляр jsonBuffer
  JsonObject& json = jsonBuffer.parseObject(jsonConfig);
  // Заполняем поля json 
  json["SSDPName"] = SSDP_Name;
  json["ssidAPName"] = ssidAP;
  json["ssidAPPassword"] = passwordAP;
  json["ssidName"] = ssid;
  json["ssidPassword"] = password;
  json["timezone"] = timezone;
  json["feedAmount"] = feedAmount;
  for (byte j=0; j<4; j++){
  json["ah" + String(j)] =  feedTime[j][0];
  json["am" + String(j)] =  feedTime[j][1];
  json["ac"+ String(j)] =  feedTime[j][2];
  }
  // Помещаем созданный json в глобальную переменную json.printTo(jsonConfig);
  json.printTo(jsonConfig);
  // Открываем файл для записи
  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    //Serial.println("Failed to open config file for writing");
    configFile.close();
    return false;
  }
  // Записываем строку json в файл 
  json.printTo(configFile);
  configFile.close();
  return true;
  }
