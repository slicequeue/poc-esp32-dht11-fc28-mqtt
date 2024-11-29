#include <WiFi.h>
#include <time.h>
#include "WiFiHandler.h"

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;    // 시간차
const int daylightOffset_sec = 0;   // 써머타임


// ISO 8601 형식으로 시간 반환
String get_current_datetime_iso8601()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return "null"; // 시간을 얻을 수 없을 때 null 반환
  }
  char timeStr[25];
  strftime(timeStr, sizeof(timeStr), "%Y-%m-%dT%H:%M:%S%z", &timeinfo);
  return String(timeStr);
}

void setup_wifi(const char* ssid, const char* password) {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println(get_current_datetime_iso8601());
}
