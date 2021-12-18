#include <Arduino.h>
#include <SSD1306Wire.h>  // I2C Oled Display with SSD1306 Controller

#define COMPORT_SPEED   115200
#define COMPORT1_SPEED  9600

// I2C-address and pins of OLED
#define I2C_ADDR  0x3c
#define PIN_SDA   5
#define PIN_SCL   4

// Rx/Tx pins Hardware Serial_1 for GPS
#define PIN_RX1  17  // connected to Tx of GPS
#define PIN_TX1  16  // connected to Rx of GPS

typedef struct {
  double time;    // 01 time [UTC]
  char status;    // 02 A = ok, P = precise, V = warning
  double lat;     // 03 latitude [decimal degrees]
  char ns;        // 04 north or south
  double lon;     // 05 longitude [decimal degrees]
  char ew;        // 06 east or west
  float speed;    // 07 speed over ground [km]
  float track;    // 08 - track angle [degrees]
  long date;      // 09 date without century
  float mv;       // 10 - magnetic variation [degrees]
  char mew;       // 11 - east or west magnetic variation
  char integrity; // 12 signal integrity A = autonomous 
                  //                  D = differential 
                  //                  E = estimated 
                  //                  M = manual 
                  //                  S = simulated 
                  //                  N = data not valid
  int dd, mo, yy; // day, month, year+2000
  int hh, mm, ss; // hours, minutes, seconds [UTC]
} rmc_t;

typedef struct {
  double time;    // 01 time [UTC]
  double lat;     // 02 latitude [decimal degrees]
  char ns;        // 03 north or south
  double lon;     // 04 longitude [decimal degrees]
  char ew;        // 05 east or west
  int quality;    // 06 quality of signal
  int  nbrSat;    // 07 nbr of satellites
  float hdp;      // 08 horizontal dillution of precision
  float alt;      // 09 altitude [m]
  float hae;      // 11 height above WGS84 ellipsoid [m]
  int hh, mm, ss; // hours, minutes, seconds [UTC]
} gga_t;

String s;
char buf[128];
char * pch;
char * pToken[64];
rmc_t rmc;
gga_t gga;
SSD1306Wire display(I2C_ADDR, PIN_SDA, PIN_SCL);