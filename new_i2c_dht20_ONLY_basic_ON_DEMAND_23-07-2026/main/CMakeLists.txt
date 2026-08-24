idf_component_register(SRCS 
						"hello_world_main.c"
        				"i2c_bus.c"
        				"dht20.c"
        				"lcd.c"
    					
                       PRIV_REQUIRES spi_flash
                       REQUIRES driver esp_wifi esp_event esp_netif mqtt nvs_flash 
                       INCLUDE_DIRS ".")