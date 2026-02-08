#include "WifiConnector.h"

#include <Arduino.h>
#include <WiFi.h>

WifiConnector::WifiConnector(const char * ssid,
                             const char * password,
                             uint32_t reconnectIntervalMs,
                             uint32_t connectTimeoutMs)
    : ssid(ssid),
      password(password),
      reconnectIntervalMs(reconnectIntervalMs),
      connectTimeoutMs(connectTimeoutMs),
      lastConnectAttemptMs(0) {}

void WifiConnector::begin() {
  WiFi.mode(WIFI_STA);
  connect();
}

void WifiConnector::ensureConnected() {
  if (isConnected()) {
    return;
  }

  uint32_t now = millis();
  if (now - lastConnectAttemptMs < reconnectIntervalMs) {
    return;
  }

  connect();
}

bool WifiConnector::isConnected() const {
  return WiFi.status() == WL_CONNECTED;
}

void WifiConnector::connect() {
  if (ssid == nullptr || ssid[0] == '\0') {
    return;
  }

  lastConnectAttemptMs = millis();
  WiFi.begin(ssid, password);

  uint32_t startMs = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - startMs) < connectTimeoutMs) {
    delay(250);
  }
}
