#include "WiFi.h"
#include "heltec.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "ESPAsyncWebServer.h"
#include "SoftwareSerial.h"
#include "SPIFFS.h"
#include "Wire.h"
#include "string.h"
#include "stdio.h"
#include "WiFi.h"
#include "ctime"

#define PASMO 434E6 //pasmo pro Evropu
#define OLED_RST 16
#define OLED_SDA 4
#define OLED_SCL 15
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

const char *ssid = "MK 2.4GHz"; //nazev wifi, na kterou se zařízení připojí
char *password = "MK12345678"; //defaultní heslo
char VyslednyText[2];
String VypisovanyText;
String VypisovanyText2;
const long intervalDisplay = 2000;
const long intervalLoRa = 500;
unsigned long DisplayMillis, LoRaMillis = 0;

AsyncWebServer server(80);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

String Parser(String gps, int x)
{
  switch (x)
  {
  case 1: //longitude
    return (gps.substring(0, 10));
    break;
  case 2: //latitude
    return (gps.substring(10, 20));
    break;
  case 3: //altitude
    return (gps.substring(20, 26));
    break;
  case 4: //cas
    return (gps.substring(26, 34));
    break;
  case 5: //datum
    return (gps.substring(34, 44));
    break;
  case 6: //pocet satelitu
    return (gps.substring(44, 45));
    break;
  }
}

String processor(const String &var)
{
  Serial.println(var);
  if (var == "GPS_LON")
  { //pokud se webserver zeptá na identifikátor GPS, bude mu poslán VypisovanyText
    Serial.println(Parser(VypisovanyText2, 1));
    return (Parser(VypisovanyText2, 1));
  }
  else if (var == "GPS_LAT")
  {
    Serial.println(Parser(VypisovanyText2, 2));
    return (Parser(VypisovanyText2, 2));
  }
  else if (var == "GPS_ALT")
  {
    Serial.println(Parser(VypisovanyText2, 3));
    return (Parser(VypisovanyText2, 3));
  }
  else if (var == "GPS_TIME")
  {
    Serial.println(Parser(VypisovanyText2, 4));
    return (Parser(VypisovanyText2, 4));
  }
  else if (var == "GPS_DATE")
  {
    Serial.println(Parser(VypisovanyText2, 5));
    return (Parser(VypisovanyText2, 5));
  }
  else if (var == "GPS_SAT")
  {
    Serial.println(Parser(VypisovanyText2, 6));
    return (Parser(VypisovanyText2, 6));
  }
  return String();
}
void setup()
{
  Serial.begin(115200);
  Heltec.begin(false /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, PASMO /*long BAND*/); //nastaveni hardwaru
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  //SetPassword();
  ////////////////display///////////////////////////////////////////////
  //funkce pro nastavení displaye
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  Wire.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false))
  { // Adresa 0x3C pro 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Nepůjde do nekonečna
  }

  display.setCursor(0, 0);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.clearDisplay();
  display.print("Nacitani.");
  display.display();
  delay(1500);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Nacitani..");
  display.display();
  delay(1500);

  //////////////////pripojeni spiffsu////////////////////////////

  if (!SPIFFS.begin(true))
  { //inicializace SPIFFsu
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  ////////////////////////////////////////zapnuti serveru a pripojeni cest /////////////////////////

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) { //cesta pro root / webové stránky
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) { //cesta pro .css soubor
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/jquery-3.6.0.min.js", HTTP_GET, [](AsyncWebServerRequest *request) { //cesta pro jquery
    request->send(SPIFFS, "/jquery-3.6.0.min.js", "text/javascript");
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) { //cesta pro jquery
    request->send(SPIFFS, "/script.js", "text/javascript");
  });

  server.begin(); //zapne server

  /////////////////////////////////////////////////////////////////////////////////////////////////
}
void loop()
{
  unsigned long TedMillis = millis();
  VypisovanyText == "";
  int velikostPaketu = LoRa.parsePacket();

  if (TedMillis - LoRaMillis >= intervalLoRa)
  {
    if (velikostPaketu)
    { //pokud se nejaký packet přijme spustí se funkce
      while (LoRa.available())
      { //prevedeni textu do jednoho stringu
        VypisovanyText += (VyslednyText + strlen(VyslednyText), "%c", (char)LoRa.read());
      }
      VypisovanyText2 = VypisovanyText; //vypsany text se uloží do druhé proměnné, aby se mohl načíst nový a s proměnnou stále pracovat
      LoRaMillis = TedMillis;
    }
    if (TedMillis - DisplayMillis >= intervalDisplay)
    {

      Serial.println(Parser(VypisovanyText2, 1));
      Serial.println(Parser(VypisovanyText2, 2));
      Serial.println(Parser(VypisovanyText2, 3));
      Serial.println(Parser(VypisovanyText2, 4));
      Serial.println(Parser(VypisovanyText2, 5));
      Serial.println(Parser(VypisovanyText2, 6));
      Serial.println(password);
      Serial.println(ssid);

      display.setCursor(0, 0);
      display.clearDisplay();
      display.setTextColor(WHITE);
      display.setTextSize(1);
      display.println("----GPS sledovac----");
      display.setCursor(13, 10);
      display.print("Sila signalu: ");
      display.println(LoRa.packetRssi() * (-1));
      display.setCursor(13, 20);
      display.print("IP: ");
      display.println(WiFi.softAPIP());
      display.setCursor(13, 30);
      display.print("Wifi: ");
      display.println(ssid);
      display.setCursor(13, 40);
      display.print("Heslo: ");
      display.println(password);
      display.setCursor(13, 50);
      display.print("Pocet satelitu: ");
      display.println(Parser(VypisovanyText2, 6));
      display.setCursor(0, 60);
      display.println("--------------------");

      display.display();

      DisplayMillis = TedMillis;
    }
  }
  VypisovanyText = ""; //vynuluje vypisovaný text aby se nestakoval
}