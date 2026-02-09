#include "AppUI.h"

#include "LVGL_Driver.h"

namespace {
lv_obj_t * labelWifi = nullptr;
lv_obj_t * labelMqtt = nullptr;
lv_obj_t * labelButton = nullptr;
lv_obj_t * labelPublish = nullptr;
lv_obj_t * labelSub = nullptr;
lv_obj_t * labelPress = nullptr;
lv_obj_t * labelAck = nullptr;

char wifiText[24] = "";
char mqttText[24] = "";
char buttonText[24] = "";
char publishText[24] = "";
char subText[24] = "";
char pressText[24] = "";
char ackText[24] = "";

void Create_Status_UI() {
  lv_obj_t * screen = lv_scr_act();

  // Root layout
  lv_obj_set_style_pad_all(screen, 0, 0);
  lv_obj_set_style_bg_color(screen, lv_color_black(), 0);
  lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

  // Header
  lv_obj_t * header = lv_obj_create(screen);
  lv_obj_set_size(header, LV_PCT(100), 32);
  lv_obj_align(header, LV_ALIGN_TOP_MID, 0, 0);
  lv_obj_set_style_bg_color(header, lv_color_hex(0x1E1E1E), 0);
  lv_obj_set_style_bg_opa(header, LV_OPA_COVER, 0);
  lv_obj_set_style_border_width(header, 0, 0);
  lv_obj_set_style_pad_all(header, 6, 0);

  lv_obj_t * title = lv_label_create(header);
  lv_label_set_text(title, "MQTT Status");
  lv_obj_set_style_text_color(title, lv_color_white(), 0);
  lv_obj_align(title, LV_ALIGN_LEFT_MID, 0, 0);

  // Main content area
  lv_obj_t * content = lv_obj_create(screen);
  lv_obj_set_size(content, LV_PCT(100), LV_PCT(100));
  lv_obj_align(content, LV_ALIGN_TOP_MID, 0, 32);
  lv_obj_set_style_bg_color(content, lv_color_black(), 0);
  lv_obj_set_style_bg_opa(content, LV_OPA_TRANSP, 0);
  lv_obj_set_style_border_width(content, 0, 0);
  lv_obj_set_style_pad_all(content, 12, 0);

  lv_obj_set_style_text_color(content, lv_color_white(), 0);

  labelWifi = lv_label_create(content);
  labelMqtt = lv_label_create(content);
  labelButton = lv_label_create(content);
  labelPublish = lv_label_create(content);
  labelSub = lv_label_create(content);
  labelPress = lv_label_create(content);
  labelAck = lv_label_create(content);

  lv_obj_align(labelWifi, LV_ALIGN_TOP_LEFT, 0, 0);
  lv_obj_align(labelMqtt, LV_ALIGN_TOP_LEFT, 0, 16);
  lv_obj_align(labelButton, LV_ALIGN_TOP_LEFT, 0, 32);
  lv_obj_align(labelPublish, LV_ALIGN_TOP_LEFT, 0, 48);
  lv_obj_align(labelSub, LV_ALIGN_TOP_LEFT, 0, 64);
  lv_obj_align(labelPress, LV_ALIGN_TOP_LEFT, 0, 80);
  lv_obj_align(labelAck, LV_ALIGN_TOP_LEFT, 0, 96);

  lv_label_set_text(labelWifi, "wifi: --");
  lv_label_set_text(labelMqtt, "mqtt: --");
  lv_label_set_text(labelButton, "button: --");
  lv_label_set_text(labelPublish, "publish: --");
  lv_label_set_text(labelSub, "sub: --");
  lv_label_set_text(labelPress, "press: --");
  lv_label_set_text(labelAck, "ack: --");

  // Footer
  lv_obj_t * footer = lv_obj_create(screen);
  lv_obj_set_size(footer, LV_PCT(100), 28);
  lv_obj_align(footer, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_obj_set_style_bg_color(footer, lv_color_hex(0x1E1E1E), 0);
  lv_obj_set_style_bg_opa(footer, LV_OPA_COVER, 0);
  lv_obj_set_style_border_width(footer, 0, 0);
  lv_obj_set_style_pad_all(footer, 6, 0);

  lv_obj_t * hint = lv_label_create(footer);
  lv_label_set_text(hint, "BOOT -> MQTT");
  lv_obj_set_style_text_color(hint, lv_color_white(), 0);
  lv_obj_align(hint, LV_ALIGN_CENTER, 0, 0);
}
}

void AppUI::Init() {
  Create_Status_UI();
}

void AppUI::UpdateStatus(bool wifiConnected,
                         const char * wifiIp,
                         bool mqttConnected,
                         int mqttState,
                         bool buttonPressed,
                         bool publishPending,
                         bool lastPublishOk,
                         const char * subscriberId,
                         uint32_t nowMs,
                         uint32_t lastPressMs,
                         uint32_t lastAckMs) {
  if (wifiConnected && wifiIp != nullptr && wifiIp[0] != '\0') {
    snprintf(wifiText, sizeof(wifiText), "wifi: %s", wifiIp);
  } else {
    snprintf(wifiText, sizeof(wifiText), "wifi: off");
  }
  lv_label_set_text(labelWifi, wifiText);

  if (mqttConnected) {
    snprintf(mqttText, sizeof(mqttText), "mqtt: ok");
  } else {
    snprintf(mqttText, sizeof(mqttText), "mqtt: off(%d)", mqttState);
  }
  lv_label_set_text(labelMqtt, mqttText);

  snprintf(buttonText, sizeof(buttonText), "button: %s", buttonPressed ? "down" : "idle");
  lv_label_set_text(labelButton, buttonText);

  const char * publishState = publishPending ? "pending" : (lastPublishOk ? "ok" : "fail");
  snprintf(publishText, sizeof(publishText), "publish: %s", publishState);
  lv_label_set_text(labelPublish, publishText);

  if (subscriberId == nullptr || subscriberId[0] == '\0') {
    snprintf(subText, sizeof(subText), "sub: --");
  } else {
    snprintf(subText, sizeof(subText), "sub: %s", subscriberId);
  }
  lv_label_set_text(labelSub, subText);

  uint32_t pressSeconds = 0;
  if (lastPressMs > 0 && nowMs >= lastPressMs) {
    pressSeconds = (nowMs - lastPressMs) / 1000U;
  }
  if (lastPressMs == 0) {
    snprintf(pressText, sizeof(pressText), "press: --");
  } else {
    snprintf(pressText, sizeof(pressText), "press: %lus", static_cast<unsigned long>(pressSeconds));
  }
  lv_label_set_text(labelPress, pressText);

  uint32_t ackSeconds = 0;
  if (lastAckMs > 0 && nowMs >= lastAckMs) {
    ackSeconds = (nowMs - lastAckMs) / 1000U;
  }
  if (lastAckMs == 0) {
    snprintf(ackText, sizeof(ackText), "ack: --");
  } else {
    snprintf(ackText, sizeof(ackText), "ack: %lus", static_cast<unsigned long>(ackSeconds));
  }
  lv_label_set_text(labelAck, ackText);
}
