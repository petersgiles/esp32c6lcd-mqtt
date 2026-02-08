# ESP32-C6 LCD 1.47" Starter (LVGL)

A clean starting point for the Waveshare ESP32-C6 1.47" LCD. This repo gives you a minimal LVGL setup and direct access to the LCD hardware so you can build your own UI from scratch.

## Intended workflow

1. Have an idea.
2. Clone this repo.
3. Rename esp32c6lcd-starter.ino to your project name.
4. Write your code.

## Quick start (Arduino IDE)

1. Install the ESP32 board package in Arduino IDE.
2. Install the LVGL library (Library Manager).
3. Open esp32c6lcd-starter.ino (rename it if you want).
4. Select the ESP32-C6 board.
5. Build and flash.

If everything is working, you should see a simple “Hello, ESP32-C6!” label on the LCD.

## Hello world entry point

The initial UI is created in [esp32c6lcd-starter.ino](esp32c6lcd-starter.ino):

- `setup()` initializes the LCD + LVGL and calls `Create_Hello_UI()`.
- `loop()` calls `Timer_Loop()` to keep LVGL running.

You can replace `Create_Hello_UI()` with your own UI code.

## File map (what to edit)

- [esp32c6lcd-starter.ino](esp32c6lcd-starter.ino) — main sketch, your UI entry point.
- [LVGL_Driver.cpp](LVGL_Driver.cpp) / [LVGL_Driver.h](LVGL_Driver.h) — LVGL glue code and tick timer.
- [Display_ST7789.cpp](Display_ST7789.cpp) / [Display_ST7789.h](Display_ST7789.h) — LCD hardware driver.
- [lv_conf.h](lv_conf.h) — LVGL configuration.

## Notes

- The driver is set up for LVGL v9 APIs.
- Touch input is disabled by default.

When you are ready, replace the “Hello” label with your own screens and widgets.
# esp32c6lcd-starter
