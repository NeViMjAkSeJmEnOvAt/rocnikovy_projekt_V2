#include "heltec.h"
#include "TinyGPS++.h"
#include "HardwareSerial.h"
#define PASMO 434E6                      //LoRa pasmo pro Evropu
unsigned long GPSMillis, LoRaMillis = 0; //uloží čas pro GPS a LoRA
const long intervalGPS = 2000;           //interval ve kterém se bude funkce spouštět pro GPS
const long intervalLoRa = 3000;          //interval ve kterém se bude funkce spouštět pro odesílání LoRa

TinyGPSPlus gps;
HardwareSerial SerialGPS(1);

void setup()
{
  Serial.begin(115200);
  SerialGPS.begin(9600, SERIAL_8N1, 16, 17);                                                                                                        //nastaveni portu RX-17 TX-16
  Heltec.begin(false /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, PASMO /*long BAND*/); //nastaveni hardwaru
}
void loop()
{
  unsigned long TedMillis = millis(); //uloží momentální čas

  if (TedMillis - GPSMillis >= intervalGPS) //pokud už uběhl čas od posledního spuštění GPS
  {
    while (SerialGPS.available() > 0) //pokud je GPS signál dostupný
    {
      gps.encode(SerialGPS.read()); //přelož GPS signál
    }
    GPSMillis = TedMillis; //nastaví se momentální čas, aby se mohlo určit poslední spuštění funkce
  }
  if (TedMillis - LoRaMillis >= intervalLoRa)
  {
    Serial.print("LAT=");
    Serial.println(gps.location.lat(), 7); //vypise se 7 pozic cisla
    Serial.print("LONG=");
    Serial.println(gps.location.lng(), 7);
    Serial.print("ALT=");
    Serial.println(gps.altitude.meters());
    Serial.print("SAT=");
    Serial.println(gps.satellites.value());

    //duvod, proc je to v podminkach je, protože potřebuju aby výsledek byl ve dvojciferném výsledku př: 12:42:33
    //když je číslo menší než 1, je výpis: 9:5:3, po úpravě to vyjde 09:05:03
    LoRa.beginPacket(); //začíná tvořit jeden packet, který pak bude odeslán pomocí LoRy
    if (gps.location.lng() < 1.00)
    {
      LoRa.print(00.0000000); //zapíše informaci do packetu
    }
    else
    {
      LoRa.print(gps.location.lng(), 7);
    }

    if (gps.location.lat() < 1.00)
    {
      LoRa.print(00.0000000);
    }
    else
    {
      LoRa.print(gps.location.lat(), 7);
    }
    if (gps.altitude.meters() < 1.00)
    {
      LoRa.print(100.00);
    }
    else
    {
      LoRa.print(gps.altitude.meters());
    }

    if (gps.time.hour() < 10)
    {
      LoRa.print("0");
      LoRa.print(gps.time.hour() + 1);
      LoRa.print(":");
    }
    else
    {
      LoRa.print(gps.time.hour() + 1);
      LoRa.print(":");
    }
    if (gps.time.minute() < 10)
    {
      LoRa.print("0");
      LoRa.print(gps.time.minute());
      LoRa.print(":");
    }
    else
    {
      LoRa.print(gps.time.minute());
      LoRa.print(":");
    }
    if (gps.time.second() < 10)
    {
      LoRa.print("0");
      LoRa.print(gps.time.second());
    }
    else
    {
      LoRa.print(gps.time.second());
    }
    if (gps.date.day() < 10)
    {
      LoRa.print("0");
      LoRa.print(gps.date.day());
      LoRa.print(".");
    }
    else
    {
      LoRa.print(gps.date.day());
      LoRa.print(".");
    }
    if (gps.date.month() < 10)
    {
      LoRa.print("0");
      LoRa.print(gps.date.month());
      LoRa.print(".");
    }
    else
    {
      LoRa.print(gps.date.month());
      LoRa.print(".");
    }
    LoRa.print(gps.date.year());
    LoRa.println(gps.satellites.value());
    LoRa.endPacket(); //ukončí a odešle packet
    LoRaMillis = TedMillis;
  }
}
