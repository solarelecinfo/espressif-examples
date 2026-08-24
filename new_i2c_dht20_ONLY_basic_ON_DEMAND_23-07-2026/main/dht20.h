/*
 * dht20.h
 *
 *  Created on: 17 juin 2026
 *      Author: jctor
 */

 #ifndef DHT20_H
 #define DHT20_H

 #include "esp_err.h"

 esp_err_t dht20_init(void);
 esp_err_t dht20_read(float *temp, float *hum);

 #endif