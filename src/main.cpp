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
	if (now - prev_time < 100)
	{
		return;
	}
	prev_time = now;

	int button_state = digitalRead(PIN_BUTTON);
	if (button_state == LOW)
	{
		handle_button_pressed(&led_frequency);
		Serial.printf("LED freq: %d\r\n", led_frequency);
	}

	if ((now / 1000) % 5 == 0)
	{
		Serial.printf("%d    |    ", now);
		if (mqtt_publish(client, led_frequency))
		{
			Serial.println("Published data");
		}
		else
		{
			Serial.println("Can't publish data");
		}
	}
}
