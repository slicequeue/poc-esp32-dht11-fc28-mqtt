#ifndef WIFIHANDLER_H
#define WIFIHANDLER_H

#include <WiFi.h>

void setup_wifi(const char* ssid, const char* password);
String get_current_datetime_iso8601();

#endif