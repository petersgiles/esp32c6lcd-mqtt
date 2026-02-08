# ESP32-C6 LCD 1.47" Starter (LVGL)

A clean starting point for the Waveshare ESP32-C6 1.47" LCD. This repo gives you a minimal LVGL setup and direct access to the LCD hardware so you can build your own UI from scratch.

## Intended workflow

1. Have an idea.
2. Clone this repo.
3. Open esp32/esp32c6lcd-mqtt.ino (rename it if you want).
4. Write your code.

## Quick start (Arduino IDE)

1. Install the ESP32 board package in Arduino IDE.
2. Install the LVGL library (Library Manager).
3. Open esp32/esp32c6lcd-mqtt.ino (rename it if you want).
4. Select the ESP32-C6 board.
5. Build and flash.

If everything is working, you should see a simple “Hello, ESP32-C6!” label on the LCD.

## Hello world entry point

The initial UI is created in [esp32/esp32c6lcd-mqtt.ino](esp32/esp32c6lcd-mqtt.ino):

- `setup()` initializes the LCD + LVGL and calls `Create_Hello_UI()`.
- `loop()` calls `Timer_Loop()` to keep LVGL running.

You can replace `Create_Hello_UI()` with your own UI code.

## File map (what to edit)

- [esp32/esp32c6lcd-mqtt.ino](esp32/esp32c6lcd-mqtt.ino) — main sketch, your UI entry point.
- [esp32/LVGL_Driver.cpp](esp32/LVGL_Driver.cpp) / [esp32/LVGL_Driver.h](esp32/LVGL_Driver.h) — LVGL glue code and tick timer.
- [esp32/Display_ST7789.cpp](esp32/Display_ST7789.cpp) / [esp32/Display_ST7789.h](esp32/Display_ST7789.h) — LCD hardware driver.
- [esp32/lv_conf.h](esp32/lv_conf.h) — LVGL configuration.

## Notes

- The driver is set up for LVGL v9 APIs.
- Touch input is disabled by default.

When you are ready, replace the “Hello” label with your own screens and widgets.
