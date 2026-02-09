#include "Display_ST7789.h"
#include "LVGL_Driver.h"

#include "AppConfig.h"
#include "AppUI.h"
#include "BootButton.h"
#include "MqttPublisher.h"
#include "WifiConnector.h"

static WifiConnector wifi(
  AppConfig::WifiSsid,
  AppConfig::WifiPassword,
  AppConfig::WifiReconnectIntervalMs,
  AppConfig::WifiConnectTimeoutMs);
static MqttPublisher mqtt(
  AppConfig::MqttBrokerHost,
  AppConfig::MqttBrokerPort,
  AppConfig::MqttClientId,
  AppConfig::MqttUsername,
  AppConfig::MqttPassword,
  AppConfig::MqttCaCert,
  AppConfig::MqttClientCert,
  AppConfig::MqttClientKey,
  AppConfig::MqttUseTls,
  AppConfig::MqttBootTopic,
  AppConfig::MqttAckTopic,
  AppConfig::MqttReconnectIntervalMs);
static BootButton bootButton(
  AppConfig::BootButtonPin,
  AppConfig::BootButtonActiveLow,
  AppConfig::BootButtonDebounceMs);

static bool publishPending = false;
static bool lastPublishOk = false;
static bool buttonPressed = false;
static uint32_t lastPressMs = 0;
static uint32_t lastPublishAttemptMs = 0;
static const uint32_t publishRetryIntervalMs = 1000;
static char wifiIp[20] = "";

void setup() {
  Serial.begin(115200);
  LCD_Init();
  Lvgl_Init();
  AppUI::Init();
  bootButton.begin();
  wifi.begin();
  mqtt.begin();
}

void loop() {
  Timer_Loop();
  wifi.ensureConnected();
  mqtt.loop();
  uint32_t nowMs = millis();
  buttonPressed = bootButton.update();
  if (buttonPressed) {
    lastPressMs = nowMs;
    publishPending = true;
    Serial.println("button press");
  }
  if (publishPending && (nowMs - lastPublishAttemptMs) >= publishRetryIntervalMs) {
    lastPublishAttemptMs = nowMs;
    lastPublishOk = mqtt.publishBootPress(AppConfig::MqttBootPayload);
    Serial.printf("publish %s\n", lastPublishOk ? "ok" : "fail");
    if (lastPublishOk) {
      publishPending = false;
    }
  }
  AppUI::UpdateStatus(
    wifi.isConnected(),
    (wifi.getIp(wifiIp, sizeof(wifiIp)) ? wifiIp : ""),
    mqtt.isConnected(),
    mqtt.lastConnectState(),
    buttonPressed,
    publishPending,
    lastPublishOk,
    mqtt.lastAckPayload(),
    nowMs,
    lastPressMs,
    mqtt.lastAckTimestampMs());
  delay(5);
}
