/*
 * i2c_bus.c
 *
 *  Created on: 17 juin 2026
 *      Author: jctor
 */




 #include <stdio.h>

 #include "i2c_bus.h"
 #include "esp_log.h"

 i2c_master_bus_handle_t bus_handle = NULL;

 void i2c_bus_init(void)
 {
     i2c_master_bus_config_t bus_cfg = {
         .i2c_port = I2C_NUM_0,
         .sda_io_num = I2C_SDA,
         .scl_io_num = I2C_SCL,
         .clk_source = I2C_CLK_SRC_DEFAULT,
         .glitch_ignore_cnt = 7,
         .flags.enable_internal_pullup = true,
     };

     ESP_ERROR_CHECK(
         i2c_new_master_bus(
             &bus_cfg,
             &bus_handle
         )
     );
 }

 void i2c_scan(void)
 {
     printf("\nScan I2C :\n");

     for (uint8_t addr = 1; addr < 127; addr++)
     {
         if (i2c_master_probe(
                 bus_handle,
                 addr,
                 50) == ESP_OK)
         {
             printf("Device trouvé : 0x%02X\n", addr);
         }
     }

     printf("Fin scan\n\n");
 }