#include <Arduino.h>
#include "WiFiHandler.h"
#include "MQTTHandler.h"
#include "DHT.h"
#include "key.h"

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long previousMillis = 0;
const unsigned long interval = 15 * 1000;// 10 * 60 * 1000; // 주기 : 10분에 한번씩

#define DHT_PIN 15     // 온습도 센서가 연결된 핀 번호
#define DHT_TYPE DHT11 // 온습도 센서 타입
#define FC28_PIN 32    // 토양 습윤 센서가 연결된 핀 번호

DHT dht(DHT_PIN, DHT_TYPE);


void setup()
{
  Serial.begin(115200);
  Serial.println("[DHT11/FC28 MQTT Test Start]");
  setup_wifi(ssid, password);
  setup_mqtt(client, mqtt_server, 1883); // MQTT 초기화

  dht.begin();
} // end of setup

void loop()
{
  if (!client.connected())
  {
    reconnect_mqtt();
  }
  client.loop();

  // 주기 루프 수행 - interval 초과에 1번씩 수행
  if ((millis() - previousMillis) > interval)
  {
    previousMillis = millis();

    // 시간 기준
    String datetime = get_current_datetime_iso8601();

    // 습도 단위: %
    float humidityPercent = dht.readHumidity();
    String jsonHumidityPercent = "{\"type\": \"humidity\", \"value\": " + String(humidityPercent) + ", \"unit\": \"%\", \"at\": \"" + datetime +"\"}";
    // 온도 단위: 도씨
    float temperatureCelsius = dht.readTemperature();
    String jsonTemperatureCelsius = "{\"type\": \"temperature\", \"value\": " + String(temperatureCelsius) + ", \"unit\": \"'C\", \"at\": \"" + datetime + "\"}";
    // 토양 습윤 단위: %
    float soilMoisturePercent = map(analogRead(FC28_PIN), 4095, 0, 0, 100);
    String jsonSoilMoisturePercent = "{\"type\": \"soil-moisture\", \"value\": " + String(soilMoisturePercent) + ", \"unit\": \"%\", \"at\": \"" + datetime + "\"}";

    String jsonTotal = "[" + jsonHumidityPercent + "," + jsonTemperatureCelsius + "," + jsonSoilMoisturePercent + "]";
    Serial.println(jsonTotal);

    client.publish(mqtt_topic_output, jsonTotal.c_str());
  }

} // end of loop