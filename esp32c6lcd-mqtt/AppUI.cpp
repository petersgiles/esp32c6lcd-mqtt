#include "AppUI.h"

#include "LVGL_Driver.h"

namespace {
void Create_Hello_UI() {
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
  lv_label_set_text(title, "Starter UI");
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

  lv_obj_t * label = lv_label_create(content);
  lv_label_set_text(label, "Hello, ESP32-C6!\nLVGL is running.");
  lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_set_style_text_color(label, lv_color_white(), 0);
  lv_obj_align(label, LV_ALIGN_CENTER, 0, -10);

  // Footer
  lv_obj_t * footer = lv_obj_create(screen);
  lv_obj_set_size(footer, LV_PCT(100), 28);
  lv_obj_align(footer, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_obj_set_style_bg_color(footer, lv_color_hex(0x1E1E1E), 0);
  lv_obj_set_style_bg_opa(footer, LV_OPA_COVER, 0);
  lv_obj_set_style_border_width(footer, 0, 0);
  lv_obj_set_style_pad_all(footer, 6, 0);

  lv_obj_t * hint = lv_label_create(footer);
  lv_label_set_text(hint, "Edit Create_Hello_UI() to start");
  lv_obj_set_style_text_color(hint, lv_color_white(), 0);
  lv_obj_align(hint, LV_ALIGN_CENTER, 0, 0);
}
}

void AppUI::Init() {
  Create_Hello_UI();
}
