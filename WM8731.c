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

uint8_t init_codec_i2s_2() {
	volatile freertos_i2c_flag_t transfer_result;
    uint8_t return_value = 1;

    uint8_t i2c_data[2] = {RESET_ADDR, 0x00};
    transfer_result = i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);
    if (transfer_result == freertos_i2c_fail) {
       return_value = 0;
    }

    i2c_data[0] = ANALOGUE_AUDIO;
    i2c_data[1] = (1<<1) | (1<<3); // DACSEL
    i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);

    uint8_t db = 0; //0x00 (-34.5) - 0x1F (12)
    db = (db+34.5)/1.5;
    i2c_data[0] = LEFT_LINE_IN;
    i2c_data[1] = db; 
    i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);

    i2c_data[0] = RIGHT_LINE_IN;
    i2c_data[1] = db; 
    i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);

    i2c_data[0] = 0x2;
    i2c_data[1] = 0xF0; 
    i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);

    i2c_data[0] = 0x3;
    i2c_data[1] = 0xF0; 
    i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);

    i2c_data[0] = 0x4;
    // i2c_data[1] = 0xD0;
    i2c_data[1] = 0x12;
    i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);

    i2c_data[0] = 0x5;
    i2c_data[1] = 0x06; 
    i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);

    i2c_data[0] = 0x6;
    i2c_data[1] = 0x00; 
    i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);

    i2c_data[0] = 0x7;
    i2c_data[1] = 0x2; 
    i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);

    // i2c_data[0] = 0x8;
    // i2c_data[1] = 0x23;
    // i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);

    i2c_data[0] = 0x9;
	i2c_data[1] = 0x01;
	i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);

    i2c_data[0] = 0xC;
	i2c_data[1] = 0x00;
	i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);

	return return_value;
}

uint8_t init_codec_i2s() {
	volatile freertos_i2c_flag_t transfer_result;
    uint8_t return_value = 1;

    uint8_t i2c_data[2];

//    i2c_data[2] = {RESET_ADDR, 0x00};
//    transfer_result = i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);
//    if (transfer_result == freertos_i2c_fail) {
//       return_value = 0;
//    }

    i2c_data[0] = ANALOGUE_AUDIO;
    i2c_data[1] = (1<<1) | (1<<4); // DACSEL
    i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);

    uint8_t db = 0; //0x00 (-34.5) - 0x1F (12)
    db = (db+34.5)/1.5;
    i2c_data[0] = LEFT_LINE_IN;
    i2c_data[1] = db;
    i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);

    i2c_data[0] = RIGHT_LINE_IN;
    i2c_data[1] = db;
    i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);

    i2c_data[0] = IFACE_FORMAT;
    // (I2S format) (16 bits) (righ on clk low) (slv mode)
    i2c_data[1] = (0x2) | (0x0<<2) | (0x0<<4) | (0x0<<6);
	i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);

    i2c_data[0] = POWER_DOWN_CTRL;
	i2c_data[1] = 0x00;
	i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);

    i2c_data[0] = ACTIVE_CONTROL;
	i2c_data[1] = 0x01;
	i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);

    i2c_data[0] = 0xC;
	i2c_data[1] = 0x00;
	i2c_multiple_write(DEVICE_ADDR, i2c_data, 2);

	return return_value;
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
