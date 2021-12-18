#include <Adafruit_GPS.h>
#define GPSSerial Serial2
Adafruit_GPS GPS(&GPSSerial); //RX 17 TX 16
#define GPSECHO true

void setup()
{
  GPSSerial.begin(9600);
  Serial.begin(115200);
  GPS.begin(9600);
  Serial.println("----------------------------------------------------");
}
uint32_t updateTime = 100;

void loop()
{
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_ALLDATA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.read();
  if (GPS.newNMEAreceived())
  {
    GPS.parse(GPS.lastNMEA());
    Serial.print("Den: ");
    Serial.println(GPS.day);
    Serial.print("Hodina: ");
    Serial.println(int(GPS.hour + 1));
    Serial.print("Pocet Satelitu: ");
    Serial.println(GPS.satellites);
    Serial.print("Latitude: ");
    Serial.print(GPS.latitude);
    Serial.println(GPS.lat);
    Serial.print("Lognitude: ");
    Serial.print(GPS.longitude);
    Serial.println(GPS.lon);
    Serial.print("Latitude2: ");
    Serial.println(GPS.latitude_fixed);
    Serial.print("Lognitude2: ");
    Serial.println(GPS.longitude_fixed);
  }
}