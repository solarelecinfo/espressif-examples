#pragma once

#include "esp_err.h"
#include "driver/gpio.h"




typedef struct {
    gpio_num_t rs;
    gpio_num_t e;
    gpio_num_t d4;
    gpio_num_t d5;
    gpio_num_t d6;
    gpio_num_t d7;
} lcd_pins_t;

typedef struct {
    lcd_pins_t pins;
    uint8_t rows;
} lcd_t;

esp_err_t lcd_init(lcd_t *lcd);
esp_err_t lcd_clear(lcd_t *lcd);
esp_err_t lcd_set_cursor(lcd_t *lcd, uint8_t col, uint8_t row);
esp_err_t lcd_print(lcd_t *lcd, const char *str);
esp_err_t lcd_light_clear(lcd_t *lcd);
esp_err_t lcd_hard_init(lcd_t *lcd);