#include "AppUI.h"

#include "LVGL_Driver.h"

namespace {
// ============ LCARS Color Palette ============
// Classic Star Trek LCARS colors
const lv_color_t LCARS_ORANGE = lv_color_hex(0xFF9966);  // Primary accent
const lv_color_t LCARS_PEACH = lv_color_hex(0xFFCC99);   // Secondary accent
const lv_color_t LCARS_BLUE = lv_color_hex(0x9999FF);    // Information
const lv_color_t LCARS_PURPLE = lv_color_hex(0xCC99CC);  // Secondary info
const lv_color_t LCARS_RED = lv_color_hex(0xFF6666);     // Alert
const lv_color_t LCARS_GOLD = lv_color_hex(0xFFCC66);    // Highlight
const lv_color_t LCARS_BG = lv_color_hex(0x000000);      // Background
const lv_color_t LCARS_PANEL = lv_color_hex(0x111122);   // Panel background
const lv_color_t LCARS_TEXT = lv_color_hex(0xFFFFFF);    // Text color

// UI Elements
lv_obj_t * labelWifi = nullptr;
lv_obj_t * labelMqtt = nullptr;
lv_obj_t * labelButton = nullptr;
lv_obj_t * labelPublish = nullptr;
lv_obj_t * labelSub = nullptr;
lv_obj_t * labelPress = nullptr;
lv_obj_t * labelAck = nullptr;

// LCARS decorative elements
lv_obj_t * wifiIndicator = nullptr;
lv_obj_t * mqttIndicator = nullptr;
lv_obj_t * pulseBar = nullptr;
lv_obj_t * stardate = nullptr;

// Animation state
uint8_t animPhase = 0;
bool pulseDirection = true;

char wifiText[32] = "";
char mqttText[32] = "";
char buttonText[32] = "";
char publishText[32] = "";
char subText[32] = "";
char pressText[32] = "";
char ackText[32] = "";
char stardateText[32] = "";

// Helper: Create LCARS-style rounded bar
lv_obj_t * CreateLCARSBar(lv_obj_t * parent, int16_t x, int16_t y, int16_t w, int16_t h, lv_color_t color) {
  lv_obj_t * bar = lv_obj_create(parent);
  lv_obj_set_size(bar, w, h);
  lv_obj_set_pos(bar, x, y);
  lv_obj_set_style_bg_color(bar, color, 0);
  lv_obj_set_style_bg_opa(bar, LV_OPA_COVER, 0);
  lv_obj_set_style_border_width(bar, 0, 0);
  lv_obj_set_style_radius(bar, h / 2, 0);  // Rounded ends
  lv_obj_set_style_pad_all(bar, 0, 0);
  return bar;
}

// Helper: Create LCARS-style corner element
lv_obj_t * CreateLCARSCorner(lv_obj_t * parent, int16_t x, int16_t y, int16_t size, lv_color_t color) {
  lv_obj_t * corner = lv_obj_create(parent);
  lv_obj_set_size(corner, size, size);
  lv_obj_set_pos(corner, x, y);
  lv_obj_set_style_bg_color(corner, color, 0);
  lv_obj_set_style_bg_opa(corner, LV_OPA_COVER, 0);
  lv_obj_set_style_border_width(corner, 0, 0);
  lv_obj_set_style_radius(corner, size / 2, 0);
  lv_obj_set_style_pad_all(corner, 0, 0);
  return corner;
}

// Helper: Create LCARS data panel
lv_obj_t * CreateDataPanel(lv_obj_t * parent, int16_t y, int16_t h, lv_color_t accentColor) {
  lv_obj_t * panel = lv_obj_create(parent);
  lv_obj_set_size(panel, LV_PCT(100), h);
  lv_obj_set_pos(panel, 0, y);
  lv_obj_set_style_bg_color(panel, LCARS_PANEL, 0);
  lv_obj_set_style_bg_opa(panel, LV_OPA_20, 0);
  lv_obj_set_style_border_color(panel, accentColor, 0);
  lv_obj_set_style_border_width(panel, 2, 0);
  lv_obj_set_style_border_side(panel, LV_BORDER_SIDE_LEFT, 0);
  lv_obj_set_style_radius(panel, 0, 0);
  lv_obj_set_style_pad_all(panel, 6, 0);
  return panel;
}

void Create_Status_UI() {
  lv_obj_t * screen = lv_scr_act();

  // Root layout - LCARS black background
  lv_obj_set_style_pad_all(screen, 0, 0);
  lv_obj_set_style_bg_color(screen, LCARS_BG, 0);
  lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

  // ============ LCARS Header Section ============
  lv_obj_t * header = lv_obj_create(screen);
  lv_obj_set_size(header, LV_PCT(100), 50);
  lv_obj_align(header, LV_ALIGN_TOP_MID, 0, 0);
  lv_obj_set_style_bg_color(header, LCARS_BG, 0);
  lv_obj_set_style_bg_opa(header, LV_OPA_COVER, 0);
  lv_obj_set_style_border_width(header, 0, 0);
  lv_obj_set_style_pad_all(header, 0, 0);

  // Top LCARS bar (characteristic orange)
  CreateLCARSBar(header, 0, 0, 120, 20, LCARS_ORANGE);
  CreateLCARSBar(header, 125, 0, 40, 20, LCARS_PEACH);
  
  // Title text on dark background panel
  lv_obj_t * titlePanel = lv_obj_create(header);
  lv_obj_set_size(titlePanel, 170, 26);
  lv_obj_set_pos(titlePanel, 0, 22);
  lv_obj_set_style_bg_color(titlePanel, LCARS_PANEL, 0);
  lv_obj_set_style_bg_opa(titlePanel, LV_OPA_60, 0);
  lv_obj_set_style_border_width(titlePanel, 0, 0);
  lv_obj_set_style_radius(titlePanel, 4, 0);
  lv_obj_set_style_pad_all(titlePanel, 4, 0);

  lv_obj_t * title = lv_label_create(titlePanel);
  lv_label_set_text(title, "LCARS COMM CONSOLE");
  lv_obj_set_style_text_color(title, LCARS_BLUE, 0);
  lv_obj_align(title, LV_ALIGN_LEFT_MID, 4, 0);

  // Stardate display
  stardate = lv_label_create(header);
  lv_label_set_text(stardate, "SD: 00000.0");
  lv_obj_set_style_text_color(stardate, LCARS_GOLD, 0);
  lv_obj_set_pos(stardate, 4, 2);

  // ============ Status Indicators ============
  // WiFi status indicator (top right)
  wifiIndicator = CreateLCARSCorner(screen, 150, 54, 16, LCARS_RED);
  
  // MQTT status indicator
  mqttIndicator = CreateLCARSCorner(screen, 150, 74, 16, LCARS_RED);

  // ============ Main Content Area ============
  lv_obj_t * content = lv_obj_create(screen);
  lv_obj_set_size(content, LV_PCT(100), 190);
  lv_obj_align(content, LV_ALIGN_TOP_MID, 0, 96);
  lv_obj_set_style_bg_color(content, LCARS_BG, 0);
  lv_obj_set_style_bg_opa(content, LV_OPA_TRANSP, 0);
  lv_obj_set_style_border_width(content, 0, 0);
  lv_obj_set_style_pad_all(content, 4, 0);

  // Data panels with LCARS styling
  lv_obj_t * panelWifi = CreateDataPanel(content, 0, 28, LCARS_ORANGE);
  labelWifi = lv_label_create(panelWifi);
  lv_obj_set_style_text_color(labelWifi, LCARS_TEXT, 0);
  lv_obj_align(labelWifi, LV_ALIGN_LEFT_MID, 8, 0);

  lv_obj_t * panelMqtt = CreateDataPanel(content, 32, 28, LCARS_BLUE);
  labelMqtt = lv_label_create(panelMqtt);
  lv_obj_set_style_text_color(labelMqtt, LCARS_TEXT, 0);
  lv_obj_align(labelMqtt, LV_ALIGN_LEFT_MID, 8, 0);

  lv_obj_t * panelButton = CreateDataPanel(content, 64, 28, LCARS_PURPLE);
  labelButton = lv_label_create(panelButton);
  lv_obj_set_style_text_color(labelButton, LCARS_TEXT, 0);
  lv_obj_align(labelButton, LV_ALIGN_LEFT_MID, 8, 0);

  lv_obj_t * panelPublish = CreateDataPanel(content, 96, 28, LCARS_GOLD);
  labelPublish = lv_label_create(panelPublish);
  lv_obj_set_style_text_color(labelPublish, LCARS_TEXT, 0);
  lv_obj_align(labelPublish, LV_ALIGN_LEFT_MID, 8, 0);

  lv_obj_t * panelSub = CreateDataPanel(content, 128, 28, LCARS_PEACH);
  labelSub = lv_label_create(panelSub);
  lv_obj_set_style_text_color(labelSub, LCARS_TEXT, 0);
  lv_obj_align(labelSub, LV_ALIGN_LEFT_MID, 8, 0);

  // Timing info in compact panels
  lv_obj_t * timingPanel = lv_obj_create(content);
  lv_obj_set_size(timingPanel, LV_PCT(100), 26);
  lv_obj_set_pos(timingPanel, 0, 160);
  lv_obj_set_style_bg_color(timingPanel, LCARS_BG, 0);
  lv_obj_set_style_bg_opa(timingPanel, LV_OPA_TRANSP, 0);
  lv_obj_set_style_border_width(timingPanel, 0, 0);
  lv_obj_set_style_pad_all(timingPanel, 2, 0);

  labelPress = lv_label_create(timingPanel);
  lv_obj_set_style_text_color(labelPress, LCARS_BLUE, 0);
  lv_obj_set_pos(labelPress, 4, 0);

  labelAck = lv_label_create(timingPanel);
  lv_obj_set_style_text_color(labelAck, LCARS_PURPLE, 0);
  lv_obj_set_pos(labelAck, 4, 12);

  // ============ Footer LCARS Elements ============
  lv_obj_t * footer = lv_obj_create(screen);
  lv_obj_set_size(footer, LV_PCT(100), 30);
  lv_obj_align(footer, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_obj_set_style_bg_color(footer, LCARS_BG, 0);
  lv_obj_set_style_bg_opa(footer, LV_OPA_COVER, 0);
  lv_obj_set_style_border_width(footer, 0, 0);
  lv_obj_set_style_pad_all(footer, 0, 0);

  // Bottom decorative bars
  CreateLCARSBar(footer, 0, 8, 60, 18, LCARS_PURPLE);
  CreateLCARSBar(footer, 65, 8, 50, 18, LCARS_BLUE);
  pulseBar = CreateLCARSBar(footer, 120, 8, 40, 18, LCARS_ORANGE);

  lv_obj_t * hint = lv_label_create(footer);
  lv_label_set_text(hint, "COMMS ACTIVE");
  lv_obj_set_style_text_color(hint, LCARS_GOLD, 0);
  lv_obj_align(hint, LV_ALIGN_CENTER, 0, -4);

  // Initial text
  lv_label_set_text(labelWifi, "NETWORK: OFFLINE");
  lv_label_set_text(labelMqtt, "COMM: OFFLINE");
  lv_label_set_text(labelButton, "INPUT: STANDBY");
  lv_label_set_text(labelPublish, "TX: IDLE");
  lv_label_set_text(labelSub, "RX: ---");
  lv_label_set_text(labelPress, "LAST: --");
  lv_label_set_text(labelAck, "ACK: --");
}
}

void AppUI::Init() {
  Create_Status_UI();
  
  // Create LVGL timer for LCARS animations (100ms period for smooth pulsing)
  lv_timer_create([](lv_timer_t * timer) {
    AppUI::AnimationTask(timer);
  }, 100, nullptr);
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
  // Update WiFi status with LCARS styling
  if (wifiConnected && wifiIp != nullptr && wifiIp[0] != '\0') {
    snprintf(wifiText, sizeof(wifiText), "NET: %s", wifiIp);
    lv_obj_set_style_bg_color(wifiIndicator, LCARS_BLUE, 0);
  } else {
    snprintf(wifiText, sizeof(wifiText), "NET: OFFLINE");
    lv_obj_set_style_bg_color(wifiIndicator, LCARS_RED, 0);
  }
  lv_label_set_text(labelWifi, wifiText);

  // Update MQTT status
  if (mqttConnected) {
    snprintf(mqttText, sizeof(mqttText), "COMM: ONLINE");
    lv_obj_set_style_bg_color(mqttIndicator, LCARS_BLUE, 0);
  } else {
    snprintf(mqttText, sizeof(mqttText), "COMM: OFFLINE [%d]", mqttState);
    lv_obj_set_style_bg_color(mqttIndicator, LCARS_RED, 0);
  }
  lv_label_set_text(labelMqtt, mqttText);

  // Button status
  snprintf(buttonText, sizeof(buttonText), "INPUT: %s", buttonPressed ? "ACTIVE" : "STANDBY");
  lv_label_set_text(labelButton, buttonText);

  // Publish status with visual indicator
  const char * publishState = publishPending ? "SENDING" : (lastPublishOk ? "SENT" : "ERROR");
  snprintf(publishText, sizeof(publishText), "TX: %s", publishState);
  lv_label_set_text(labelPublish, publishText);

  // Subscriber ID
  if (subscriberId == nullptr || subscriberId[0] == '\0') {
    snprintf(subText, sizeof(subText), "RX: WAITING");
  } else {
    snprintf(subText, sizeof(subText), "RX: %s", subscriberId);
  }
  lv_label_set_text(labelSub, subText);

  // Press timing
  uint32_t pressSeconds = 0;
  if (lastPressMs > 0 && nowMs >= lastPressMs) {
    pressSeconds = (nowMs - lastPressMs) / 1000U;
  }
  if (lastPressMs == 0) {
    snprintf(pressText, sizeof(pressText), "LAST: --");
  } else {
    snprintf(pressText, sizeof(pressText), "LAST: T-%lus", static_cast<unsigned long>(pressSeconds));
  }
  lv_label_set_text(labelPress, pressText);

  // Ack timing
  uint32_t ackSeconds = 0;
  if (lastAckMs > 0 && nowMs >= lastAckMs) {
    ackSeconds = (nowMs - lastAckMs) / 1000U;
  }
  if (lastAckMs == 0) {
    snprintf(ackText, sizeof(ackText), "ACK: --");
  } else {
    snprintf(ackText, sizeof(ackText), "ACK: T-%lus", static_cast<unsigned long>(ackSeconds));
  }
  lv_label_set_text(labelAck, ackText);

  // Update stardate (using millis as basis)
  uint32_t starDateValue = (nowMs / 100) % 100000;
  snprintf(stardateText, sizeof(stardateText), "SD: %05lu.%lu", 
           static_cast<unsigned long>(starDateValue / 10),
           static_cast<unsigned long>(starDateValue % 10));
  lv_label_set_text(stardate, stardateText);
}

// Animation task for LCARS pulsing effects
void AppUI::AnimationTask(void * timer) {
  if (pulseBar == nullptr) return;
  
  animPhase++;
  
  // Pulse the orange bar in footer
  uint8_t opacity = 180;
  if (pulseDirection) {
    opacity = 180 + (animPhase % 76);
    if (animPhase % 76 == 75) pulseDirection = false;
  } else {
    opacity = 255 - (animPhase % 76);
    if (animPhase % 76 == 75) pulseDirection = true;
  }
  
  lv_obj_set_style_bg_opa(pulseBar, opacity, 0);
}
