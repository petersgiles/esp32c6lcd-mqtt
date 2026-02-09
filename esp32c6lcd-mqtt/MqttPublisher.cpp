#include "MqttPublisher.h"

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

namespace {
WiFiClientSecure mqttWiFiClient;
PubSubClient mqttClient(mqttWiFiClient);
}

MqttPublisher::MqttPublisher(const char * host,
                             uint16_t port,
                             const char * clientId,
                             const char * username,
                             const char * password,
                             const char * caCert,
                             const char * clientCert,
                             const char * clientKey,
                             bool useTls,
                             const char * bootTopic,
                             uint32_t reconnectIntervalMs)
    : host(host),
      port(port),
      clientId(clientId),
      username(username),
      password(password),
      caCert(caCert),
      clientCert(clientCert),
      clientKey(clientKey),
      useTls(useTls),
      bootTopic(bootTopic),
      reconnectIntervalMs(reconnectIntervalMs),
      lastConnectAttemptMs(0) {}

void MqttPublisher::begin() {
  mqttClient.setServer(host, port);
  if (useTls && hasTlsConfig()) {
    mqttWiFiClient.setCACert(caCert);
    mqttWiFiClient.setCertificate(clientCert);
    mqttWiFiClient.setPrivateKey(clientKey);
  }
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

  if (useTls && !hasTlsConfig()) {
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

bool MqttPublisher::hasTlsConfig() const {
  if (!useTls) {
    return true;
  }

  if (caCert == nullptr || caCert[0] == '\0') {
    return false;
  }

  if (clientCert == nullptr || clientCert[0] == '\0') {
    return false;
  }

  if (clientKey == nullptr || clientKey[0] == '\0') {
    return false;
  }

  return true;
}
