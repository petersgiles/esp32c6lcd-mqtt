#pragma once

#include <stdint.h>

#include "AppSecrets.h"

namespace AppConfig {
constexpr const char * WifiSsid = AppSecrets::WifiSsid;
constexpr const char * WifiPassword = AppSecrets::WifiPassword;
constexpr uint32_t WifiConnectTimeoutMs = 15000;
constexpr uint32_t WifiReconnectIntervalMs = 10000;

constexpr const char * MqttBrokerHost = "192.168.2.23";
constexpr uint16_t MqttBrokerPort = 1884;
constexpr const char * MqttClientId = "esp32c6lcd-74";
constexpr const char * MqttUsername = AppSecrets::MqttUsername;
constexpr const char * MqttPassword = AppSecrets::MqttPassword;
constexpr const char * MqttCaCert = AppSecrets::MqttCaCert;
constexpr const char * MqttClientCert = AppSecrets::MqttClientCert;
constexpr const char * MqttClientKey = AppSecrets::MqttClientKey;
constexpr bool MqttUseTls = false;
constexpr const char * MqttBootTopic = "esp32c6lcd/boot_button";
constexpr const char * MqttAckTopic = "esp32c6lcd/boot_button/ack";
constexpr const char * MqttBootPayload = "{\"specversion\":\"1.0\",\"id\":\"esp32-boot-1\",\"source\":\"esp32c6lcd\",\"type\":\"boot_button\",\"datacontenttype\":\"text/plain\",\"data\":\"hello\"}";
constexpr uint32_t MqttReconnectIntervalMs = 5000;

constexpr uint8_t BootButtonPin = 9;
constexpr bool BootButtonActiveLow = true;
constexpr uint32_t BootButtonDebounceMs = 50;
}
