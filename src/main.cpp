#include "constants.h"
#include "utils.h"
#include <Arduino.h>
#include <WiFi.h>

uint64_t prev_time = 0;
WiFiClient wifiClient;
PubSubClient client(wifiClient);

void setup()
{
	Serial.begin(115200);

	pinMode(PIN_LED_BUILTIN, OUTPUT);

	delay(1000);
	wifi_setup();
	delay(1000);
	mqtt_setup(client);
}

void loop()
{
	if (!client.connected())
	{
		mqtt_reconnect(client);
	}
	client.loop();

	uint64_t now = millis();
	if (now - prev_time < 1000)
	{
		return;
	}

	uint64_t now_sec = now / 1000;
	Serial.printf("Hi %llu\r\n", now_sec);
	prev_time = now;

	if (now_sec % 30 == 0)
	{
		digitalWrite(PIN_LED_BUILTIN, HIGH);
	}
	else
	{
		digitalWrite(PIN_LED_BUILTIN, LOW);
	}
}
