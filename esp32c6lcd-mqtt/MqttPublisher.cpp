#include "MqttPublisher.h"

#include <Arduino.h>
#include <cstring>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

namespace {
WiFiClient mqttWiFiClient;
WiFiClientSecure mqttWiFiClientSecure;
PubSubClient mqttClient;
MqttPublisher * activePublisher = nullptr;

void OnMqttMessage(char * topic, uint8_t * payload, unsigned int length) {
  if (activePublisher == nullptr) {
    return;
  }
  activePublisher->handleMessage(topic, payload, length);
}
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
                             const char * ackTopic,
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
      ackTopic(ackTopic),
      reconnectIntervalMs(reconnectIntervalMs),
      lastConnectAttemptMs(0),
      lastState(-1),
      lastAckMs(0),
      subscribed(false),
      hasAck(false) {
  lastAck[0] = '\0';
}

void MqttPublisher::begin() {
  if (useTls) {
    mqttClient.setClient(mqttWiFiClientSecure);
  } else {
    mqttClient.setClient(mqttWiFiClient);
  }
  mqttClient.setServer(host, port);
  activePublisher = this;
  mqttClient.setCallback(OnMqttMessage);
  if (useTls && hasTlsConfig()) {
    mqttWiFiClientSecure.setCACert(caCert);
    mqttWiFiClientSecure.setCertificate(clientCert);
    mqttWiFiClientSecure.setPrivateKey(clientKey);
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

bool MqttPublisher::isConnected() const {
  return mqttClient.connected();
}

int MqttPublisher::lastConnectState() const {
  return lastState;
}

const char * MqttPublisher::lastAckPayload() const {
  return hasAck ? lastAck : "";
}

uint32_t MqttPublisher::lastAckTimestampMs() const {
  return lastAckMs;
}

bool MqttPublisher::ensureConnected() {
  if (mqttClient.connected()) {
    return true;
  }

  int stateBefore = mqttClient.state();

  subscribed = false;
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
    if (!mqttClient.connect(clientId, username, password)) {
      lastState = mqttClient.state();
      if (lastState != stateBefore) {
        Serial.printf("mqtt connect failed: %d\n", lastState);
      }
      return false;
    }
  } else if (!mqttClient.connect(clientId)) {
    lastState = mqttClient.state();
    if (lastState != stateBefore) {
      Serial.printf("mqtt connect failed: %d\n", lastState);
    }
    return false;
  }

  lastState = 0;
  if (lastState != stateBefore) {
    Serial.println("mqtt connected");
  }

  if (ackTopic != nullptr && ackTopic[0] != '\0') {
    subscribed = mqttClient.subscribe(ackTopic, 1);
  }
  return true;
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

void MqttPublisher::handleMessage(const char * topic, const uint8_t * payload, unsigned int length) {
  if (ackTopic == nullptr || ackTopic[0] == '\0') {
    return;
  }

  if (strcmp(topic, ackTopic) != 0) {
    return;
  }

  unsigned int copyLen = length;
  if (copyLen >= sizeof(lastAck)) {
    copyLen = sizeof(lastAck) - 1;
  }
  memcpy(lastAck, payload, copyLen);
  lastAck[copyLen] = '\0';
  lastAckMs = millis();
  hasAck = true;
}
