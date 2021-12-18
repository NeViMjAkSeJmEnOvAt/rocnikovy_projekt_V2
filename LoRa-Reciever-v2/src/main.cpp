#include "heltec.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "Wire.h"
#include "string.h"
#include "stdio.h"

#define PASMO 434E6 //pasmo pro Evropu
#define OLED_RST 16
#define OLED_SDA 4
#define OLED_SCL 15
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);
char VyslednyText[100];

void setup() {
  Serial.begin(115200);
  Heltec.begin(false /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, PASMO /*long BAND*/); //nastaveni hardwaru
    
   ////////////////display////////////////////
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);
  ////////////////////////////////////////
  Wire.begin(OLED_SDA, OLED_SCL);
      if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Adresa 0x3C pro 128x32
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Nepůjde do nekonečna
    }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0,0);
  display.println("LoRa Device");
  display.print("ON");
  display.display();

}

void loop() {

  int velikostPaketu = LoRa.parsePacket();
  if (velikostPaketu) { //pokud se nejaký packet přijme spustí se funkce
    Serial.print("Packet prijat: ");
    while (LoRa.available()) { //prevedeni textu
      sprintf(VyslednyText+strlen(VyslednyText),"%c",(char)LoRa.read());
    }  
    Serial.print(VyslednyText);
    Serial.print("o sile (RSSI): ");
    Serial.println(LoRa.packetRssi());

    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0,0);
    display.println("Prijata Zprava: ");
    display.println(VyslednyText);
    display.print("o sile (RSSI): ");
    display.println(LoRa.packetRssi());
    display.display();
  }
  memset(VyslednyText,0,100); //vycisteni charu s textem
}