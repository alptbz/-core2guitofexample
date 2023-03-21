#ifndef NETWORKING_H_
#define NETWORKING_H_

#include <M5Core2.h>
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"

void setup_wifi();

void mqtt_loop();

void mqtt_init(MQTT_CALLBACK_SIGNATURE);

void mqtt_publish(const char* topic, const char* payload);

void mqtt_re_connect();

#endif // NETWORKING_H_