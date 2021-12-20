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

#define PASMO 434E6 //LoRa pasmo pro Evropu
#define OLED_RST 16
#define OLED_SDA 4
#define OLED_SCL 15
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

const char *ssid = "MK 2.4GHz"; //nazev wifi, na kterou se zařízení připojí
char *password = "MK12345678";  //heslo
char VyslednyText[2];
String VypisovanyText;
String VypisovanyText2;
const long intervalDisplay = 2000; //interval ve kterém se bude funkce spouštět pro Display
const long intervalLoRa = 500;     //interval ve kterém se bude funkce spouštět pro LoRu
unsigned long DisplayMillis, LoRaMillis = 0;

AsyncWebServer server(80); //vytvoření serveru
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

String Parser(String gps, int x) //slouží k rozložení proudu dat získaných z "senderu"
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

String SilaSignalu(const int x) //funkce pro zjistovani sily signalu
{
  {
    if (x <= 40)
    {
      return "Skvely";
    }
    else if (x > 40 && x <= 80)
    {
      return "Dobry";
    }
    else if (x > 80 && x <= 150)
    {
      return "Horsi";
    }
    else
    {
      return "Spatny";
    }
  }
}

String processor(const String &var) //slouží k odesílání informací na webový server
{
  if (var == "GPS_LON")
  { //pokud se webserver zeptá na identifikátor GPS, bude mu poslán VypisovanyText
    return (Parser(VypisovanyText2, 1));
  }
  else if (var == "GPS_LAT")
  {
    return (Parser(VypisovanyText2, 2));
  }
  else if (var == "GPS_ALT")
  {
    return (Parser(VypisovanyText2, 3));
  }
  else if (var == "GPS_TIME")
  {
    return (Parser(VypisovanyText2, 4));
  }
  else if (var == "GPS_DATE")
  {
    return (Parser(VypisovanyText2, 5));
  }
  else if (var == "GPS_SAT")
  {
    return (Parser(VypisovanyText2, 6));
  }
  return String();
}
void setup()
{
  Serial.begin(115200);
  Heltec.begin(false /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, PASMO /*long BAND*/); //nastaveni hardwaru
  WiFi.begin(ssid, password); //zapnutí wifi (připojení se na wifi pomocí údajů)
  while (WiFi.status() != WL_CONNECTED) //dokud se wifi nepřipojí
  {
    delay(1000);
    Serial.println("Connecting to WiFi.."); //vypiš connecting...
  }
  ////////////////display///////////////////////////////////////////////

  //funkce pro nastavení displaye
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  Wire.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false))
  {
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

  if (!SPIFFS.begin(true)) //pokud se spiffs nezapne, ohlásí se chyba
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    display.setCursor(0, 0);
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.println("Problem se zapinanim SPIFFS");
    display.display();
    return;
  }
  ////////////////////////////zapnuti serveru a pripojeni cest /////////////////////////

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) { //cesta pro root / webové stránky
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) { //cesta pro styly v .css souboru
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/jquery-3.6.0.min.js", HTTP_GET, [](AsyncWebServerRequest *request) { //cesta pro jquery
    request->send(SPIFFS, "/jquery-3.6.0.min.js", "text/javascript");
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) { //cesta pro javascript
    request->send(SPIFFS, "/script.js", "text/javascript");
  });

  server.begin(); //zapne server

  /////////////////////////////////////////////////////////////////////////////////////////////////
}
void loop()
{
  unsigned long TedMillis = millis(); //uloží momentální čas
  VypisovanyText == "";
  int velikostPaketu = LoRa.parsePacket();

  if (TedMillis - LoRaMillis >= intervalLoRa) //pokud už uběhl čas od posledního spuštění GPS
  {
    if (velikostPaketu)
    { //pokud se nejaký packet přijme spustí se funkce
      while (LoRa.available())
      { //prevedeni textu do jednoho stringu
        VypisovanyText += (VyslednyText + strlen(VyslednyText), "%c", (char)LoRa.read());
      }
      VypisovanyText2 = VypisovanyText; //vypsany text se uloží do druhé proměnné, aby se mohl načíst nový a s proměnnou stále pracovat
      LoRaMillis = TedMillis;           //nastaví se momentální čas, aby se mohlo určit poslední spuštění funkce
    }
    if (TedMillis - DisplayMillis >= intervalDisplay) //nahrazuje funkci delay();
    {
      //vypisuje do konzole data získaná z "sender"

      Serial.println(Parser(VypisovanyText2, 1));
      Serial.println(Parser(VypisovanyText2, 2));
      Serial.println(Parser(VypisovanyText2, 3));
      Serial.println(Parser(VypisovanyText2, 4));
      Serial.println(Parser(VypisovanyText2, 5));
      Serial.println(Parser(VypisovanyText2, 6));

      // Vypisovani na display

      display.setCursor(0, 0);
      display.clearDisplay();
      display.setTextColor(WHITE);
      display.setTextSize(1);
      display.println("----GPS sledovac----");
      display.setCursor(13, 10);
      display.print("Signal: ");
      display.println(SilaSignalu(LoRa.packetRssi() * (-1)));
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