#pragma once

#include <stdint.h>

class MqttPublisher {
 public:
  MqttPublisher(const char * host,
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
                uint32_t reconnectIntervalMs);

  void begin();
  void loop();
  bool publishBootPress(const char * payload);
  bool isConnected() const;
  int lastConnectState() const;
  const char * lastAckPayload() const;
  uint32_t lastAckTimestampMs() const;
  void handleMessage(const char * topic, const uint8_t * payload, unsigned int length);

 private:
  bool ensureConnected();
  bool hasTlsConfig() const;

  const char * host;
  uint16_t port;
  const char * clientId;
  const char * username;
  const char * password;
  const char * caCert;
  const char * clientCert;
  const char * clientKey;
  bool useTls;
  const char * bootTopic;
  const char * ackTopic;
  uint32_t reconnectIntervalMs;
  uint32_t lastConnectAttemptMs;
  int lastState;
  uint32_t lastAckMs;
  bool subscribed;
  bool hasAck;
  char lastAck[32];
};
