#include <stdio.h>

#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#include "dht20.h"
#include "i2c_bus.h"
#include "lcd.h"

#include "esp_system.h"

void reset_like_button(void)
{
    esp_restart();
}


static void delay_us(uint32_t us)
{
    esp_rom_delay_us(us);
}

int peripheral_states[] = {
	0, 0, 0}; // O=erreur de capteur ,1=pas de erreur de capteur

lcd_t lcd = {
	    .pins = {
	        .rs = 18,
	        .e  = 19,
	        .d4 = 27,
	        .d5 = 26,
	        .d6 = 25,
	        .d7 = 33,
	    }
	};


void app_main(void) {
	float temperature;
	float humidity;
	char buffer_temperature[32];
	char buffer_humidity[32];

	int inverseur = 0;
	int counter =0;

	i2c_bus_init();

	i2c_scan();

	esp_err_t err;

	err = dht20_init();
	printf("DHT20 init: %s\n", esp_err_to_name(err));
	peripheral_states[0] = (err == ESP_OK);
	
	err= lcd_init(&lcd);
	//lcd_clear(&lcd);
	printf("LCD init: %s\n", esp_err_to_name(err));
	peripheral_states[2] = (err == ESP_OK);
	
	(peripheral_states[2] == 1)?printf("module LCD OK"):printf("error on module LCD");


	while (1) {
		inverseur ^= 1;
		counter++;
		if (counter%8==0){
			delay_us(1000);
			lcd_hard_init(&lcd);
			delay_us(1000);
	
		}
		esp_err_t error_dht20 = dht20_read(&temperature, &humidity);
		if (error_dht20 == ESP_OK) {
			printf("Temperature : %.2f °C\n", temperature);
			printf("Humidite    : %.2f %%\n", humidity);
		} else {
			printf("Erreur de lecture du capteur  DHT20");
		}


		if (peripheral_states[0] == 1) {
			snprintf(buffer_temperature, sizeof(buffer_temperature),
					 "Temperat:%.2fC", temperature);
			snprintf(buffer_humidity, sizeof(buffer_humidity),
					 "Humidity:%.2f%%", humidity);
		}


		// Verifier si LCD est disponible
		if (peripheral_states[2] == 1) {
			printf("OK LCD disponible\n");
			lcd_light_clear(&lcd);
			lcd_set_cursor(&lcd,0, 0);
			(peripheral_states[0] == 1) ? lcd_print(&lcd,buffer_temperature)
										: lcd_print(&lcd,"dht20 temp ERROR");
			
			lcd_set_cursor(&lcd,0, 1);
			(peripheral_states[0] == 1) ? lcd_print(&lcd,buffer_humidity)
										: lcd_print(&lcd,"dht20 hum ERR");
	
		} else {
			printf("Erreur "
				   "Peripheral error\n");
		}
		
		delay_us(1000000);
	}
}