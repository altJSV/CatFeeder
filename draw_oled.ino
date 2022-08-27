//Картинки
static const uint8_t image_data_wifi_icon[18] = {
    // ∙∙∙███∙∙∙
    // ∙██∙∙∙██∙
    // █∙∙∙∙∙∙∙█
    // ∙∙█████∙∙
    // ∙█∙∙∙∙∙█∙
    // ∙∙∙∙█∙∙∙∙
    // ∙∙∙█∙█∙∙∙
    // ∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙█∙∙∙∙
    0x1c, 0x00, 
    0x63, 0x00, 
    0x80, 0x80, 
    0x3e, 0x00, 
    0x41, 0x00, 
    0x08, 0x00, 
    0x14, 0x00, 
    0x00, 0x00, 
    0x08, 0x00
};
static const uint8_t image_data_no_wifi[18] = {
    // ∙∙∙∙∙∙∙∙∙
    // ∙█∙∙∙∙∙█∙
    // ∙∙█∙∙∙█∙∙
    // ∙∙∙█∙█∙∙∙
    // ∙∙∙∙█∙∙∙∙
    // ∙∙∙█∙█∙∙∙
    // ∙∙█∙∙∙█∙∙
    // ∙█∙∙∙∙∙█∙
    // ∙∙∙∙█∙∙∙∙
    0x00, 0x00, 
    0x41, 0x00, 
    0x22, 0x00, 
    0x14, 0x00, 
    0x08, 0x00, 
    0x14, 0x00, 
    0x22, 0x00, 
    0x41, 0x00, 
    0x08, 0x00
};

static const uint8_t image_data_mqtt[18] = {
    // ████∙∙∙∙∙
    // ∙∙∙∙█∙∙∙∙
    // ∙∙∙∙∙█∙∙∙
    // ███∙∙∙█∙∙
    // ∙∙∙█∙∙∙█∙
    // ∙∙∙∙█∙∙∙█
    // ██∙∙∙█∙∙█
    // ∙∙█∙∙█∙∙█
    // ∙∙█∙∙█∙∙█
    0xf0, 0x00, 
    0x08, 0x00, 
    0x04, 0x00, 
    0xe2, 0x00, 
    0x11, 0x00, 
    0x08, 0x80, 
    0xc4, 0x80, 
    0x24, 0x80, 
    0x24, 0x80
};

static const uint8_t image_data_feedAm[60] = {
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙██∙∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙∙████∙█∙∙∙∙∙∙∙
    // ∙∙∙∙∙∙███∙██∙██∙∙∙∙∙
    // ∙∙∙∙∙████∙█∙██∙█∙∙∙∙
    // ∙∙∙∙█∙█∙███∙████∙∙∙∙
    // ∙∙∙∙██∙█∙∙∙██████∙∙∙
    // ∙∙∙∙█∙█∙██∙█∙∙█∙█∙∙∙
    // ∙∙∙█∙█∙█∙█∙█████∙█∙∙
    // ∙∙████∙█∙∙██∙∙████∙∙
    // ∙███∙∙█∙∙█∙███∙█∙██∙
    // ∙█∙███∙∙██∙██∙███∙█∙
    // ∙█∙∙∙∙████████∙∙∙∙█∙
    // ∙█∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙
    // ∙█∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙
    // ∙█∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙
    // ∙█∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙█∙
    // ∙∙██∙∙∙∙∙∙∙∙∙∙∙∙██∙∙
    // ∙∙∙∙████████████∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    0x00, 0x00, 0x00, 
    0x00, 0x30, 0x00, 
    0x01, 0xe8, 0x00, 
    0x03, 0xb6, 0x00, 
    0x07, 0xad, 0x00, 
    0x0a, 0xef, 0x00, 
    0x0d, 0x1f, 0x80, 
    0x0a, 0xd2, 0x80, 
    0x15, 0x5f, 0x40, 
    0x3d, 0x33, 0xc0, 
    0x72, 0x5d, 0x60, 
    0x5c, 0xdb, 0xa0, 
    0x43, 0xfc, 0x20, 
    0x40, 0x00, 0x20, 
    0x40, 0x00, 0x20, 
    0x40, 0x00, 0x20, 
    0x40, 0x00, 0x20, 
    0x30, 0x00, 0xc0, 
    0x0f, 0xff, 0x00, 
    0x00, 0x00, 0x00
};

static const uint8_t image_data_alarm[54] = {
    // ∙∙███∙∙∙∙∙∙∙∙███∙∙
    // ∙█████∙∙∙∙∙∙█████∙
    // ∙████∙∙∙∙∙∙∙∙████∙
    // ∙███∙∙█████∙∙∙███∙
    // ∙█∙∙██████████∙██∙
    // ∙∙∙███∙∙∙∙∙████∙∙∙
    // ∙∙██∙∙∙∙██∙∙∙██∙∙∙
    // ∙∙██∙∙∙∙██∙∙∙███∙∙
    // ∙∙██∙∙∙∙██∙∙∙∙██∙∙
    // ∙∙██∙∙∙∙██∙∙∙∙██∙∙
    // ∙∙██∙∙∙∙██∙∙∙∙██∙∙
    // ∙∙██∙∙∙∙∙∙██∙∙██∙∙
    // ∙∙██∙∙∙∙∙∙∙██∙██∙∙
    // ∙∙███∙∙∙∙∙∙∙∙███∙∙
    // ∙∙∙███∙∙∙∙∙∙███∙∙∙
    // ∙∙∙∙██████████∙∙∙∙
    // ∙∙∙∙∙████████∙∙∙∙∙
    // ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
    0x38, 0x07, 0x00, 
    0x7c, 0x0f, 0x80, 
    0x78, 0x07, 0x80, 
    0x73, 0xe3, 0x80, 
    0x4f, 0xfd, 0x80, 
    0x1c, 0x1e, 0x00, 
    0x30, 0xc6, 0x00, 
    0x30, 0xc7, 0x00, 
    0x30, 0xc3, 0x00, 
    0x30, 0xc3, 0x00, 
    0x30, 0xc3, 0x00, 
    0x30, 0x33, 0x00, 
    0x30, 0x1b, 0x00, 
    0x38, 0x07, 0x00, 
    0x1c, 0x0e, 0x00, 
    0x0f, 0xfc, 0x00, 
    0x07, 0xf8, 0x00, 
    0x00, 0x00, 0x00
};
void draw_display(){
  int ia,ib,ic;
display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(WHITE);
 switch (WorkMode) {
 case 0:  display.setCursor(0, 0);
          display.println(rtc.gettime("H:i:s"));
          //display.println(timeClient.getFormattedTime());
          display.drawFastHLine(0,10,128,WHITE);
          display.drawBitmap(0,16,image_data_feedAm,20,20,1);
          display.setTextSize(2);
          display.setCursor(22, 20);
          display.println(feedAmount);
          display.drawBitmap(48,18,image_data_alarm,18,18,1);
          display.setCursor(68, 20);
          rtc.gettime();
          ib=24;
          ic=5;
          for (byte i=0; i<4; i++){
          if (feedTime[i][2]==1){
            ia=feedTime[i][0]-rtc.Hours;
           if (ia<0) ia=24-rtc.Hours+feedTime[i][0];
          if (ia<=ib) {ic=i;ib=ia;}
          } 
          }
          if (ic!=5){
          if (feedTime[ic][0]<10) display.print("0");
            display.print(feedTime[ic][0]);
            display.print(":");
            if (feedTime[ic][1]<10) display.print("0");
            display.print(feedTime[ic][1]);
          }
          else display.println("--:--");
            display.setTextSize(1); // Draw 2X-scale text
          display.setCursor(0,42);
          display.print("IP:");
          if (WiFi.getMode() != WIFI_AP) display.println(WiFi.localIP()); else display.println(apIP);
          display.setCursor(0,55); 
          display.println("B1-feed, B2-alarms");
          break;

case 1:   display.setTextSize(1);
          display.setCursor(0, 0);
          display.println("Alarms");
          display.drawFastHLine(0,10,128,WHITE);
          display.setCursor(0, 14);
          for (byte i=0; i<4; i++){
            if (i==menuitem and alarm_edit==true) {
              display.setTextColor(BLACK, WHITE);
              }
            else display.setTextColor(WHITE, BLACK);
            if (feedTime[i][0]<10) display.print("0");
            display.print(feedTime[i][0]);
            display.print(":");
            if (feedTime[i][1]<10) display.print("0");
            display.print(feedTime[i][1]);
            if (feedTime [i][2]==0) display.println("  off"); else display.println("  on");
          }
          display.setCursor(0,45);
          display.setTextColor(WHITE, BLACK);
          if (alarm_edit==true){
          switch (a_set){
               case 0: display.println("Set Hours. B1-Down, B1-UP, Hold B1 - Next");
                      break;
               case 1: display.println("Set Mins. B1-Down, B1-UP, Hold B1 - Next");
                      break;
               case 2: display.println("Set on/off. B1-Down, B1-UP, Hold B1 - Next");;
                      break;
               case 3:display.setCursor(0,55); 
                      display.println("Hold B1 to edit");
                      break;              
              }
              }
          else {display.setCursor(0,55);
            display.println("Hold B2 to edit");
         
          }
          break;
          
case 2:   display.setTextSize(1);
          display.setCursor(0, 0);
          display.println("Options");
          display.drawFastHLine(0,10,128,WHITE);
          display.setCursor(0, 14);
          
          if (menuitem==0) display.setTextColor(BLACK, WHITE); else display.setTextColor(WHITE, BLACK);
          display.print("Feed Amount: ");
          display.println(feedAmount);
          
          if (menuitem==1) display.setTextColor(BLACK, WHITE); else display.setTextColor(WHITE, BLACK);
          display.print("Timezone: GMT");
          if (timezone>0) display.print("+");
          display.println(timezone);
          if (menuitem==2) display.setTextColor(BLACK, WHITE); else display.setTextColor(WHITE, BLACK);
          display.print("WiFi mode: ");
          display.println(WiFi.getMode());
          if (menuitem==3) display.setTextColor(BLACK, WHITE); else display.setTextColor(WHITE, BLACK);
          //display.println("Option3");
          display.setCursor(0,55); 
          display.println("B1-set, B2-move");
          break;
          }
          if (WiFi.status() != WL_CONNECTED) display.drawBitmap(119,0,image_data_no_wifi,9,9,1); else display.drawBitmap(119,0,image_data_wifi_icon,9,9,1);
          if (!client.connected()) display.drawBitmap(108,0,image_data_no_wifi,9,9,1); else display.drawBitmap(108,0,image_data_mqtt,9,9,1);
   display.display();
}
