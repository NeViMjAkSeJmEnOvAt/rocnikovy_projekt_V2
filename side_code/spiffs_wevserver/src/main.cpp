#include "WiFi.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "ESPAsyncWebServer.h"
#include "SoftwareSerial.h"
#include "SPIFFS.h"
#include "Wire.h"

#define OLED_RST 16
#define OLED_SDA 4
#define OLED_SCL 15
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64

const char* ssid = "Čau"; //nazev wifi, na kterou se zařízení připojí
const char* password = "zabijse1"; //heslo k wifi
int promenna = 0;
int VypisCislo;
int PIN1 = 3;
int PIN2 = 1;  
String Slovo = "Test";
int LedPin = 2; //vystup ledky na desce
String LedStav; //stav ledky 

SoftwareSerial Test(PIN1, PIN2);

AsyncWebServer server(80);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if(digitalRead(LedPin)){
      LedStav = "ON";
    }
    else{
      LedStav = "OFF";
    }
    Serial.print(LedStav);
    return LedStav;
  }
  if(var == "NUMB"){
    return String(VypisCislo);
  }
  return String();
}

void setup(){
  Serial.begin(115200);
  Test.begin(9600);
  pinMode(LedPin, OUTPUT);

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

  if(!SPIFFS.begin(true)){ //inicializace SPIFFsu
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  WiFi.begin(ssid, password); //připojení k wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP()); //vypíše IP adresu zařízení

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){ //cesta pro root / webové stránky
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){ //cesta pro .css soubor
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){ //cesta pro zapnutí ledky
    digitalWrite(LedPin, HIGH);
    promenna = 1;    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){ //cesta pro vypnutí ledky
    digitalWrite(LedPin, LOW);
    promenna = 0;    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/plus", HTTP_GET, [](AsyncWebServerRequest *request){ //cesta pro přidání čísla
    VypisCislo ++;    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/minus", HTTP_GET, [](AsyncWebServerRequest *request){ //cesta pro přidání čísla
    VypisCislo --;    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  server.begin(); //zapne server
}
void loop(){

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0,0);
  display.print("LED IS:");
  if(promenna == 1){
    display.println("ON");
  }
  else{
    display.println("OFF");
  }
  display.print("CISLO: ");
  display.println(VypisCislo);
  display.setTextSize(1);
  display.println(WiFi.localIP());
  display.display();

   delay(1000);
}