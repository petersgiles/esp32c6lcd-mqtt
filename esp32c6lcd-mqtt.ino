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
  AppConfig::MqttBootTopic,
  AppConfig::MqttReconnectIntervalMs);
static BootButton bootButton(
  AppConfig::BootButtonPin,
  AppConfig::BootButtonActiveLow,
  AppConfig::BootButtonDebounceMs);

void setup() {
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
  if (bootButton.update()) {
    mqtt.publishBootPress(AppConfig::MqttBootPayload);
  }
  delay(5);
}
