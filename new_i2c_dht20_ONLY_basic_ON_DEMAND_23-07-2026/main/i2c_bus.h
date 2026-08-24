/*
 * i2c_bus.h
 *
 *  Created on: 17 juin 2026
 *      Author: jctor
 */

 #ifndef I2C_BUS_H
 #define I2C_BUS_H


#include "driver/i2c_master.h"

#define I2C_SDA 21
#define I2C_SCL 22

extern i2c_master_bus_handle_t bus_handle;

void i2c_bus_init(void);
void i2c_scan(void);



	
#endif 
