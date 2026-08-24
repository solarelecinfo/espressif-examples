#include "lcd.h"
#include "esp_err.h"
#include "esp_rom_sys.h"
#include "driver/gpio.h"
#include <string.h>

static void delay_us(uint32_t us)
{
    esp_rom_delay_us(us);
}

static void pulse_enable(lcd_t *lcd)
{
    gpio_set_level(lcd->pins.e, 1);
    delay_us(2);
    gpio_set_level(lcd->pins.e, 0);
    delay_us(50);
}


static void write4(lcd_t *lcd, uint8_t data)
{
    gpio_set_level(lcd->pins.d4, (data >> 0) & 1);
    gpio_set_level(lcd->pins.d5, (data >> 1) & 1);
    gpio_set_level(lcd->pins.d6, (data >> 2) & 1);
    gpio_set_level(lcd->pins.d7, (data >> 3) & 1);
    pulse_enable(lcd);
}

static void write8(lcd_t *lcd, uint8_t data, bool rs)
{
    gpio_set_level(lcd->pins.rs, rs);

    write4(lcd, data >> 4);
    write4(lcd, data & 0x0F);
}

static void cmd(lcd_t *lcd, uint8_t c)
{
    write8(lcd, c, false);

    if (c == 0x01 || c == 0x02)
        delay_us(2000);
    else
        delay_us(50);
}

static void data(lcd_t *lcd, uint8_t d)
{
    write8(lcd, d, 1);
}

esp_err_t lcd_init(lcd_t *lcd)
{
    gpio_config_t io = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask =
            (1ULL << lcd->pins.rs) |
            (1ULL << lcd->pins.e) |
            (1ULL << lcd->pins.d4) |
            (1ULL << lcd->pins.d5) |
            (1ULL << lcd->pins.d6) |
            (1ULL << lcd->pins.d7)
    };

    gpio_config(&io);



    delay_us(50000);

    // init 4-bit mode
    write4(lcd, 0x03); delay_us(4500);
    write4(lcd, 0x03); delay_us(4500);
    write4(lcd, 0x03); delay_us(150);
    write4(lcd, 0x02); delay_us(150);

	//sequence d'initalisation
	cmd(lcd,0x28);    // Function Set
	cmd(lcd,0x08);    // Display OFF
	cmd(lcd,0x01);    // Clear
	cmd(lcd,0x06);    // Entry Mode
	cmd(lcd,0x0C);    // Display ON
	delay_us(2000);

    return ESP_OK;
}

esp_err_t lcd_clear(lcd_t *lcd)
{
	cmd(lcd,0x01);
	return ESP_OK;
}

esp_err_t lcd_set_cursor(lcd_t *lcd, uint8_t col, uint8_t row)
{
    static const uint8_t addr[] = {0x00, 0x40};
    cmd(lcd, 0x80 | (addr[row] + col));
    return ESP_OK;
}

esp_err_t lcd_print(lcd_t *lcd, const char *str)
{
    while (*str)
        data(lcd, *str++);
    return ESP_OK;
}

esp_err_t lcd_light_clear(lcd_t *lcd){
	lcd_set_cursor(lcd,0,0);
	lcd_print(lcd,"                ");

	lcd_set_cursor(lcd,0,1);
	lcd_print(lcd, "                ");
	return ESP_OK;
}


esp_err_t lcd_hard_init(lcd_t *lcd)
{
	gpio_config_t io = {
	    .mode = GPIO_MODE_OUTPUT,
	    .pin_bit_mask =
	        (1ULL << lcd->pins.rs) |
	        (1ULL << lcd->pins.e) |
	        (1ULL << lcd->pins.d4) |
	        (1ULL << lcd->pins.d5) |
	        (1ULL << lcd->pins.d6) |
	        (1ULL << lcd->pins.d7)
	};
	gpio_config(&io);
	delay_us(20000);
	
    gpio_set_level(lcd->pins.rs, 0);
    gpio_set_level(lcd->pins.e, 0);
    gpio_set_level(lcd->pins.d4, 0);
    gpio_set_level(lcd->pins.d5, 0);
    gpio_set_level(lcd->pins.d6, 0);
    gpio_set_level(lcd->pins.d7, 0);



    delay_us(80000);

	// init 4-bit mode
	write4(lcd, 0x03); delay_us(4500);
	write4(lcd, 0x03); delay_us(4500);
	write4(lcd, 0x03); delay_us(150);
	write4(lcd, 0x02); delay_us(150);
	
	//sequence d'initalisation
	cmd(lcd,0x28);    // Function Set
	cmd(lcd,0x08);    // Display OFF
	cmd(lcd,0x01);    // Clear
	cmd(lcd,0x06);    // Entry Mode
	cmd(lcd,0x0C);    // Display ON
    delay_us(2000);


    return ESP_OK;
}