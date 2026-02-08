#include "MqttPublisher.h"

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

namespace {
WiFiClient mqttWiFiClient;
PubSubClient mqttClient(mqttWiFiClient);
}

MqttPublisher::MqttPublisher(const char * host,
                             uint16_t port,
                             const char * clientId,
                             const char * username,
                             const char * password,
                             const char * bootTopic,
                             uint32_t reconnectIntervalMs)
    : host(host),
      port(port),
      clientId(clientId),
      username(username),
      password(password),
      bootTopic(bootTopic),
      reconnectIntervalMs(reconnectIntervalMs),
      lastConnectAttemptMs(0) {}

void MqttPublisher::begin() {
  mqttClient.setServer(host, port);
}

void MqttPublisher::loop() {
  ensureConnected();
  mqttClient.loop();
}

bool MqttPublisher::publishBootPress(const char * payload) {
  if (!ensureConnected()) {
    return false;
  }

  return mqttClient.publish(bootTopic, payload);
}

bool MqttPublisher::ensureConnected() {
  if (mqttClient.connected()) {
    return true;
  }

  if (WiFi.status() != WL_CONNECTED) {
    return false;
  }

  uint32_t now = millis();
  if (now - lastConnectAttemptMs < reconnectIntervalMs) {
    return false;
  }

  lastConnectAttemptMs = now;
  if (username != nullptr && username[0] != '\0') {
    return mqttClient.connect(clientId, username, password);
  }
  return mqttClient.connect(clientId);
}
