#include "constants.h"
#include "utils.h"
#include <Arduino.h>
#include <WiFi.h>

uint64_t prev_time = 0;
uint16_t led_frequency = 10;
WiFiClient wifiClient;
PubSubClient client(wifiClient);

void setup()
{
	Serial.begin(115200);

	ledcAttachPin(PIN_LED_BUILTIN, LED_CHANNEL);
	ledcSetup(LED_CHANNEL, 5000, 8);

	delay(500);
	wifi_setup();
	delay(500);
	mqtt_setup(client);
	delay(500);
	ledcWrite(LED_CHANNEL, led_frequency);
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

	if (now_sec % 10 == 0)
	{
		for (int cycle = 0; cycle <= 255; cycle += 5)
		{
			Serial.printf("Increasing LED brightness (%d)...\r\n", cycle);
			ledcWrite(LED_CHANNEL, cycle);
			delay(50);
		}

		if (mqtt_publish(client, led_frequency))
		{
			Serial.println("Published data");
		}
		else
		{
			Serial.println("Can't publish data");
		}
	}
	else if (now_sec % 5 == 0)
	{
		for (int cycle = 255; cycle >= 0; cycle -= 5)
		{
			Serial.printf("Decreasing LED brightness (%d)...\r\n", cycle);
			ledcWrite(LED_CHANNEL, cycle);
			delay(50);
		}
	}
}
