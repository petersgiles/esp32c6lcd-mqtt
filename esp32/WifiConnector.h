#pragma once

#include <stdint.h>

class WifiConnector {
 public:
  WifiConnector(const char * ssid,
                const char * password,
                uint32_t reconnectIntervalMs,
                uint32_t connectTimeoutMs);

  void begin();
  void ensureConnected();
  bool isConnected() const;

 private:
  void connect();

  const char * ssid;
  const char * password;
  uint32_t reconnectIntervalMs;
  uint32_t connectTimeoutMs;
  uint32_t lastConnectAttemptMs;
};
