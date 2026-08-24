/*
 * dht20.c
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

#include "dht20.h"
#include "i2c_bus.h"

#define DHT20_ADDR 0x38

static i2c_master_dev_handle_t dht20_dev = NULL;

esp_err_t dht20_init(void)
{
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = DHT20_ADDR,
        .scl_speed_hz = 100000,
    };

    return i2c_master_bus_add_device(
        bus_handle,
        &dev_cfg,
        &dht20_dev);
}

esp_err_t dht20_read(float *temp, float *hum)
{
    if (dht20_dev == NULL)
    {
        return ESP_ERR_INVALID_STATE;
    }

    uint8_t cmd[3] = {0xAC, 0x33, 0x00};
    uint8_t data[7];

    esp_err_t err;

    // Start measurement
    err = i2c_master_transmit(
        dht20_dev,
        cmd,
        sizeof(cmd),
        pdMS_TO_TICKS(100));

    if (err != ESP_OK)
    {
		printf("DHT20 ABSENT (TX FAIL)\n");
        return err;
    }

    vTaskDelay(pdMS_TO_TICKS(100));

    // Read result
    err = i2c_master_receive(
        dht20_dev,
        data,
        sizeof(data),
        pdMS_TO_TICKS(100));

    if (err != ESP_OK)
    {
		printf("DHT20 ABSENT (RX FAIL)\n");
        return err;
    }
	
	if(data[0] & 0x80)
	{
		printf("DHT20 busy\n");
		return ESP_FAIL;
	}

    // Parsing data
    uint32_t hum_raw =
        ((uint32_t)data[1] << 12) |
        ((uint32_t)data[2] << 4) |
        ((data[3] >> 4) & 0x0F);

    uint32_t temp_raw =
        (((uint32_t)data[3] & 0x0F) << 16) |
        ((uint32_t)data[4] << 8) |
        data[5];

    *hum = (hum_raw * 100.0f) / 1048576.0f;
    *temp = (temp_raw * 200.0f) / 1048576.0f - 50.0f;

    return ESP_OK;
}