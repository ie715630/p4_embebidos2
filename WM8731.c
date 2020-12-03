/*
 * WM8731.c
 *
 *  Created on: Nov 12, 2020
 *      Author: kevin
 */

#include "WM8731.h"

#define DEVICE_ADDR 0x1A
#define RESET_ADDR 0x0F
#define LEFT_LINE_IN 0x00
#define RIGHT_LINE_IN 0x01
#define ANALOGUE_AUDIO 0x04
#define POWER_DOWN_CTRL 0x06
#define IFACE_FORMAT 0x07
#define SAMPLING_CONTROL 0x08
#define ACTIVE_CONTROL 0x09

uint8_t init_codec_i2s() {

	uint8_t i2c_data[2];
	freertos_i2c_flag_t transfer_result;

    i2c_data[0] = WM8731_REG_ACTIVE_CTRL;
    i2c_data[1] = WM8731_DEACTIVATE;
    transfer_result = i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);
    if (transfer_result == freertos_i2c_fail) {
    	return 0;
    }
    vTaskDelay(pdMS_TO_TICKS(10));

    i2c_data[0] = WM8731_REG_RESET << 1;
    i2c_data[1] = WM8731_RESET;
    transfer_result = i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);
    if (transfer_result == freertos_i2c_fail) {
       return 0;
    }
    vTaskDelay(pdMS_TO_TICKS(10));

    i2c_data[0] = 1;
    i2c_data[1] = WM8731_LINE_IN_LEFT;
    transfer_result = i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);
    if (transfer_result == freertos_i2c_fail) {
    	return 0;
    }
    vTaskDelay(pdMS_TO_TICKS(10));

    i2c_data[0] = 3;
    i2c_data[1] = WM8731_LINE_IN_RIGHT;
    transfer_result = i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);
    if (transfer_result == freertos_i2c_fail) {
    	return 0;
    }
    vTaskDelay(pdMS_TO_TICKS(10));

    i2c_data[0] = 5;
    i2c_data[1] = WM8731_HP_LEFT;
    transfer_result = i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);
    if (transfer_result == freertos_i2c_fail) {
    	return 0;
    }
    vTaskDelay(pdMS_TO_TICKS(10));

    i2c_data[0] = 7;
    i2c_data[1] = WM8731_HP_RIGHT;
    transfer_result = i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);
    if (transfer_result == freertos_i2c_fail) {
    	return 0;
    }
    vTaskDelay(pdMS_TO_TICKS(10));

    i2c_data[0] = WM8731_REG_ANALOG_PATH << 1;
    i2c_data[1] = WM8731_ANALOG_AUDIO_BYPASS;
    transfer_result = i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);
    if (transfer_result == freertos_i2c_fail) {
    	return 0;
    }
    vTaskDelay(pdMS_TO_TICKS(10));

    i2c_data[0] = WM8731_REG_DIGITAL_PATH << 1;
    i2c_data[1] = WM8731_DIGITAL_AUDIO;
    transfer_result = i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);
    if (transfer_result == freertos_i2c_fail) {
    	return 0;
    }
    vTaskDelay(pdMS_TO_TICKS(10));

    i2c_data[0] = 0xA;
    i2c_data[1] = WM8731_POWER_MODE;
    transfer_result = i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);
    if (transfer_result == freertos_i2c_fail) {
    	return 0;
    }
    vTaskDelay(pdMS_TO_TICKS(10));

    i2c_data[0] = 0xC;
    i2c_data[1] = WM8731_SAMPLING;
    transfer_result = i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);
    if (transfer_result == freertos_i2c_fail) {
    	return 0;
    }
    vTaskDelay(pdMS_TO_TICKS(10));

    i2c_data[0] = 0xE;
    i2c_data[1] = WM8731_DA_INTERFACE_I2S;

    transfer_result = i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);
    if (transfer_result == freertos_i2c_fail) {
    	return 0;
    }
    vTaskDelay(pdMS_TO_TICKS(10));

    i2c_data[0] = 0x10;
    i2c_data[1] = 0;
    transfer_result = i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);
    if (transfer_result == freertos_i2c_fail) {
    	return 0;
    }
    vTaskDelay(pdMS_TO_TICKS(10));

    i2c_data[0] = 0x12;
    i2c_data[1] = WM8731_ACTIVATE;
    transfer_result = i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);
    if (transfer_result == freertos_i2c_fail) {
    	return 0;
    }
    vTaskDelay(pdMS_TO_TICKS(10));

    return 1;
}

uint8_t init_codec_bypass()
{
	// slew rate
	volatile freertos_i2c_flag_t transfer_result;
    uint8_t return_value = 1;

    uint8_t i2c_data[2] = {RESET_ADDR, 0x00};
    transfer_result = i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);
    if (transfer_result == freertos_i2c_fail) {
       return_value = 0;
    }

    uint8_t db = 0; //0x00 (-34.5) - 0x1F (12)
    db = (db+34.5)/1.5;
    i2c_data[0] = LEFT_LINE_IN;
    i2c_data[1] = db;
    transfer_result = i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);
    if (transfer_result == freertos_i2c_fail) {
       return_value = 0;
    }

    i2c_data[0] = RIGHT_LINE_IN;
    i2c_data[1] = db;
    transfer_result = i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);
    if (transfer_result == freertos_i2c_fail) {
       return_value = 0;
    }

    i2c_data[0] = 0xC;
	i2c_data[1] = 0x00;
	transfer_result = i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);
	if (transfer_result == freertos_i2c_fail) {
	   return_value = 0;
	}

    return return_value;
}
