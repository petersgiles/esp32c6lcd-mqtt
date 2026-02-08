# ESP32-C6 LCD 1.47" Starter — Instructions

This is a clean starter project. It boots the LCD, initializes LVGL, and shows a simple “Hello” label so you can begin building your own UI.

## Workflow

1. Have an idea.
2. Clone this repo.
3. Rename esp32c6lcd-starter.ino to your project name.
4. Write your code.

## Hello world steps (Arduino IDE)

1. Install the ESP32 board package in Arduino IDE.
2. Install the LVGL library (Library Manager).
3. Open esp32c6lcd-starter.ino.
4. Select the ESP32-C6 board and flash.

You should see “Hello, ESP32-C6! LVGL is running.” on the LCD.

## Where to start coding

- Edit `Create_Hello_UI()` in [esp32c6lcd-starter.ino](esp32c6lcd-starter.ino) to build your UI.
- Keep `Timer_Loop()` in `loop()` so LVGL continues to render and process events.

## Hardware access

- LCD driver: [Display_ST7789.cpp](Display_ST7789.cpp) and [Display_ST7789.h](Display_ST7789.h)
- LVGL glue: [LVGL_Driver.cpp](LVGL_Driver.cpp) and [LVGL_Driver.h](LVGL_Driver.h)
- LVGL config: [lv_conf.h](lv_conf.h)
