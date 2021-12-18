#include <Arduino.h>
#include "WiFi.h"
#include "stdlib.h"

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("Station", NULL);
}

void loop() {
}