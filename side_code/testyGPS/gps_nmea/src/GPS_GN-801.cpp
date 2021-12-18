/** 
 * Program      GPS_GN-801.cpp
 * Author       2020-01-23 Ch. Geiser
 *    
 * Purpose      Reads the NMEA Sentences sent by the GPS module via the 
 *              hardware serial interface 1 of the ESP32 module and displays 
 *              them on the integrated OLED:
 *              .----------------------.
 *              |   GGA Data GN-801    |
 *              | hh:mm:ss UTC nn x.xx |    nn   = number of satellites in use
 *              | N47.477794 E8.316084 |    x.xx = horizontal dilution of precision
 *              |       408.1 müM      |
 *              `----------------------´   
 * 
 * Board        ESP32 Board WiFi / BlueTooth / OLED / 18650 Lithium Battery
 * 
 *                    .-------------.
 *                    |             |
 *                    |.-----------.|
 *                    ||    OLED   ||
 *                    || 128 x 64  ||
 *                    |.___________.|
 *                    |             |
 *                    |[O]       [O]|         
 *                    |EN       Boot|
 *                    |             |
 *               Vcc -| .---------. |- GND
 *                   -| |         | |-   
 *                   -| |  ESP32  | |- 
 *                   -| |         | |- 
 *                   -| |         | |-                        .-----------------.
 *                   -| |         | |-                GND <== | GND             |
 *                   -| ._________|.|-                Vcc <== | Vcc             |
 *                   -|             |-                        |       GPS       |
 *                   -|             |-                        |     GN-801      | 
 *                   -|             |-                        |                 |
 *                   -|         RX1 |- 17 --------------------| TX              |
 *                   -|         TX1 |- 16 --------------------| RX              |
 *                   -|             |-                        ._________________.
 *                   -|             |-
 *                   -|             |-
 *                   -|             |-
 *                   -|             |-
 *                   -|             |-
 *                   -|             |-
 *                   -|             |-
 *                    |             |
 *                    ]¨|USB     on][
 *                    ]_|          ][o
 *                    |         off][
 *                    ._____________. 
 *
 *  
 * Reference    https://www.youtube.com/watch?v=T6V_nD4D8fg
 *              https://www.u-blox.com
 *              http://lefebure.com/articles/nmea-gga/
 *              https://www.sparkfun.com/datasheets/GPS/NMEA%20Reference%20Manual1.pdf
 * 
 * Remarks      Only the $GNGGA sentence is evaluated in this example code 
 * 
 * Shop         GPS:   https://www.aliexpress.com/item/32951163811.html?spm=a2g0s.9042311.0.0.27424c4dhstVIx
 *              ESP32: https://www.aliexpress.com/item/32905330340.html?spm=a2g0s.9042311.0.0.27424c4dEBuB1V
 * 
 */

#include "GPS_GN-801.h"

bool NmeaCheckSumIsOk(char * nmeaSentence)
{
  uint8_t chksumComputed = 0;
  unsigned int chksumSentence = 0;
  char *p = nmeaSentence + 1;     // Skip leading '$'
  char chks[3];
  // Serial.println(nmeaSentence);
  while (*p != '*' && *p != '\n') // Stop before '*' or end of sentence
  {
    chksumComputed ^= *p;
    p++;
  }

  p++;  // Skip '*'
  chks[0] = *p;
  chks[1] = *(p+1);
  chks[2] = '\0';
  sscanf(chks, "%x", &chksumSentence); 
  // Serial.printf("chksumSentence: %0X, chksumComputed: %0X\r\n", chksumComputed, chksumSentence);
  return (chksumComputed == chksumSentence);
}

double NmeaStrDegMinToDeg(char * nmeaStr)
{
  double nmeaDblDegMin = atof(nmeaStr);
  int deg = int(nmeaDblDegMin / 100);
  double degFrac = (nmeaDblDegMin - 100 * deg) /60;
  return (deg + degFrac);
}

/**
 * Convert NMEA string time to hh/mm/ss
 */
void NmeaStrTimeToHHMMSS(char * nmeaStr, int &hh, int &mm, int &ss)
{
  double nmeaDblTime = atof(nmeaStr);
  hh = int(nmeaDblTime / 10000);
  mm = int((nmeaDblTime - 10000 * hh) / 100);
  ss = int(nmeaDblTime - 10000 * hh - 100 * mm);
}

/**
 * Convert NMEA string date to dd/mm/yy
 */
void NmeaStrDateToDDMOYY(char * nmeaStr, int &dd, int &mo, int &yy)
{
  int nmeaDate = atoi(nmeaStr);
  dd = nmeaDate / 10000;
  mo = (nmeaDate - 10000 * dd) / 100;
  yy = 2000 + nmeaDate - 10000 * dd - 100 * mo;
}

/**
 * Parse RMC - Recommended Minimum sentence C
 */
void parseRMC(char *buf, rmc_t &rmc)
{
  int i = 0;
  //Serial.println(buf);

  if (! NmeaCheckSumIsOk(buf))
  {
    Serial.println("parseRMC: Checksum error");
    return;
  }

  pch = strsep (&buf, ",*");
  
  while (pch != NULL)
  {
    pToken[i] = pch;
    //Serial.printf ("%s\n", pch);
    pch = strsep (&buf, ",*");
    i++;
  }
  // Serial.printf("Nbr of tokens: %d\r\n", i);
  rmc.lat = NmeaStrDegMinToDeg(pToken[3]);
  rmc.ns  = *pToken[4];
  rmc.lon = NmeaStrDegMinToDeg(pToken[5]);
  rmc.ew  = *pToken[6];
  sscanf(pToken[7], "%f", &rmc.speed);
  rmc.speed *= 1.852; // Convert knots to km/h
  if (! *pToken[8])
  {
    //Serial.printf("Token 8: %x\r\n",*pToken[8]);
    rmc.track = 0.0;
  }
  if (! *pToken[10])
  {
    rmc.mv = 0.0;
  }
  if (! *pToken[11])
  {
    rmc.mew = 'X';
  } 
  //Serial.printf("Speed: %f km/h\r\n", rmc.speed);
  NmeaStrTimeToHHMMSS(pToken[1], rmc.hh, rmc.mm, rmc.ss);
  NmeaStrDateToDDMOYY(pToken[9], rmc.dd, rmc.mo, rmc.yy);
  //Serial.printf("%d-%02d-%02d %02d:%02d:%02d UTC\r\n", rmc.yy, rmc.mo, rmc.dd, rmc.hh, rmc.mm, rmc.ss);
  //Serial.printf("%c%f %c%f\r\n\n", rmc.ns, rmc.lat, rmc.ew, rmc.lon);  
}

/**
 * Show RMC data on OLED
 */
void displayRMC(rmc_t &rmc)
{
  display.clear();
  display.drawString(64, 0, "RMC Data GN-801"); 
  sprintf(buf, "%4d-%02d-%02d %02d:%02d:%02d UTC", rmc.yy, rmc.mo, rmc.dd, rmc.hh, rmc.mm, rmc.ss);
  display.drawString(64, 16, buf);
  sprintf(buf, "%c%f %c%f", rmc.ns, rmc.lat, rmc.ew, rmc.lon); 
  display.drawString(64, 32, buf);
  sprintf(buf,"%6.2f km/h %5.1f°", rmc.speed, rmc.track);
  display.drawString(64,48, buf);
  display.display();   
}

void parseVTG(char *buf)
{
  //Serial.println(buf);
}

/**
 * Parse GGA fix information
 */
void parseGGA(char *buf, gga_t &gga)
{
  int i = 0;
  
  Serial.println(buf);

  if (! NmeaCheckSumIsOk(buf))
  {
    Serial.println("parseGGA: Checksum error");
    return;
  }

  pch = strsep (&buf, ",*");

  while (pch != NULL)
  {
    pToken[i] = pch;
    //Serial.printf ("%s\n", pch);
    pch = strsep (&buf, ",*");
    i++;
  }
  // Serial.printf("Nbr of tokens: %d\r\n", i);
  gga.lat = NmeaStrDegMinToDeg(pToken[2]);
  gga.ns  = *pToken[3];
  gga.lon = NmeaStrDegMinToDeg(pToken[4]);
  gga.ew  = *pToken[5];
  sscanf(pToken[6], "%d", &gga.quality);
  sscanf(pToken[7], "%d", &gga.nbrSat);
  sscanf(pToken[8], "%f", &gga.hdp);
  sscanf(pToken[9], "%f", &gga.alt);
  NmeaStrTimeToHHMMSS(pToken[1], gga.hh, gga.mm, gga.ss);
}

/**
 * Show GGA data on OLED
 */
void displayGGA(gga_t &gga)
{
  display.clear();
  display.drawString(64, 0, "GGA Data GN-801"); 
  sprintf(buf, "%02d:%02d:%02d UTC %02d %5.2f", gga.hh, gga.mm, gga.ss, gga.nbrSat, gga.hdp);
  display.drawString(64, 16, buf);
  sprintf(buf, "%c%f %c%f", gga.ns, gga.lat, gga.ew, gga.lon); 
  display.drawString(64, 32, buf);
  sprintf(buf,"%6.1f müM", gga.alt);
  display.drawString(64,48, buf);
  display.display(); 
}

void parseGSA(char *buf)
{
  //Serial.println(buf);
}

void parseGSV(char *buf)
{
  //Serial.println(buf);
}

/**
* Parse GLL - Geographic Longitude and Latitude
*/
void parseGLL(char *buf)
{
  int i = 0;
  int hh, mm, ss;
  double lat, lon;

  Serial.println(buf);
  pch = strtok (buf, ",*");
  
  while (pch != NULL)
  {
    pToken[i] = pch;
    //Serial.printf ("%s\n", pch);
    pch = strtok (NULL, ",*");
    i++;
  }
  lat = NmeaStrDegMinToDeg(pToken[1]);
  lon = NmeaStrDegMinToDeg(pToken[3]);
  NmeaStrTimeToHHMMSS(pToken[5], hh, mm, ss);
  Serial.printf("%s%f %s%f %d:%d:%d UTC\n", pToken[2], lat, pToken[4], lon, hh, mm, ss);
}

void setup() 
{
  Serial.begin(COMPORT_SPEED);
  Serial1.begin(COMPORT1_SPEED, SERIAL_8N1, PIN_RX1, PIN_TX1);
  log_i("GPS-Test");

  display.init();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  log_i("===> done");
}

void loop() 
{
  while (Serial1.available())
  {
    s = Serial1.readStringUntil('\n');
    strcpy(buf, s.c_str());

    //Serial.println(buf);
    
    //if (strstr(buf, "RMC")) { parseRMC(buf, rmc); displayRMC(rmc); }
    //if (strstr(buf, "VTG")) { parseVTG(buf); }
    if (strstr(buf, "GGA")) { parseGGA(buf, gga); displayGGA(gga); }
    //if (strstr(buf, "GSA")) { parseGSA(buf); }
    //if (strstr(buf, "GSV")) { parseGSV(buf); }
    //if (strstr(buf, "GLL")) { parseGLL(buf); }
  }
}

/* NMEA sentence 
$GNGGA,084353.00,4728.65955,N,00818.96752,E,2,12,0.71,391.8,M,47.3,M,,0000*43
*/

/*
$GNRMC,091509.00,A,4728.66095,N,00818.97058,E,0.080,,100420,,,A*62
$GNVTG,,T,,M,0.080,N,0.147,K,A*37
$GNGGA,091509.00,4728.66095,N,00818.97058,E,1,12,0.74,381.3,M,47.3,M,,*48
$GNGSA,A,3,24,15,19,12,13,10,28,20,25,,,,1.32,0.74,1.09*12
$GNGSA,A,3,73,71,8$GNRMC,091519.00,A,4728.66031,N,00818.97018,E,0.140,,100420,,,A*64
$GNVTG,,T,,M,0.140,N,0.259,K,A*36
$GNGGA,091519.00,4728.66031,N,00818.97018,E,1,12,0.74,380.1,M,47.3,M,,*40
$GNGSA,A,3,24,15,19,12,13,10,28,20,25,,,,1.32,0.74,1.09*12
$GNGSA,A,3,73,71,82,81,,,,,,,,,1.32,0.74,1.09*16
$GPGSV,4,1,14,01,02,015,,10,20,297,11,12,42,230,34,13,25,152,17*77
$GPGSV,4,2,14,15,45,183,33,17,37,063,08,19,43,095,26,20,14,264,11*7A
$GPGSV,4,3,14,24,72,304,15,25,10,235,25,28,14,053,24,32,03,321,*73
$GPGSV,4,4,14,36,31,150,,49,35,185,*78
$GLGSV,3,1,10,65,04,343,,70,16,193,,71,49,242,28,72,36,315,*6C
$GLGSV,3,2,10,73,63,305,22,74,27,272,17,80,37,060,,81,12,017,26*67
$GLGSV,3,3,10,82,30,066,27,83,19,118,*63
$GNGLL,4728.66031,N,00818.97018,E,091519.00,A,A*7F
*/

