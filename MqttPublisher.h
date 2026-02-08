#pragma once

#include <stdint.h>

class MqttPublisher {
 public:
  MqttPublisher(const char * host,
                uint16_t port,
                const char * clientId,
                const char * username,
                const char * password,
                const char * bootTopic,
                uint32_t reconnectIntervalMs);

  void begin();
  void loop();
  bool publishBootPress(const char * payload);

 private:
  bool ensureConnected();

  const char * host;
  uint16_t port;
  const char * clientId;
  const char * username;
  const char * password;
  const char * bootTopic;
  uint32_t reconnectIntervalMs;
  uint32_t lastConnectAttemptMs;
};
