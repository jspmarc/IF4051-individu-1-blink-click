//
// Created by josep on 21/02/23.
//

#ifndef UTILS_H
#define UTILS_H

#include <PubSubClient.h>

void wifi_setup();

void mqtt_setup(PubSubClient &client);
void mqtt_reconnect(PubSubClient &client);
bool mqtt_publish(PubSubClient &client, uint16_t led_frequency);
void __mqtt_callback(char *topic, uint8_t *payload, unsigned int length);

#endif//UTILS_H
