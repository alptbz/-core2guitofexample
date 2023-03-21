#include "networking.h"

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  Serial.println("Connecting to " + String(ssid));
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, passphrase);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }
  Serial.println("\nSuccess\n");
}

void mqtt_loop() {
    if (!client.connected()) {
        mqtt_re_connect();
  }
  client.loop();
}

void mqtt_init(MQTT_CALLBACK_SIGNATURE) {
    client.setServer(mqtt_server, 1883); 
    client.setCallback(callback); 
    mqtt_re_connect();
}

void mqtt_publish(const char* topic, const char* payload) {
    client.publish(topic, payload);
}

void mqtt_re_connect() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (client.connect(clientId)) {
        // Define subscriptions here
      client.subscribe("lvgl/#");
    } else {
      Serial.println("failed, rc=");
      Serial.println(client.state());
      Serial.println("try again in 5 seconds");
    }
  }
}
