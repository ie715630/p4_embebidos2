/*
 * filters.h
 *
 *  Created on: Nov 30, 2020
 *      Author: sergio_mndz
 */

#ifndef P4_EMBEBIDOS2_FILTERS_H_
#define P4_EMBEBIDOS2_FILTERS_H_

#include "arm_math.h"
#include "stdio.h"
#include "stdint.h"

#define LENGTH_SAMPLES		1040

#define BLOCK_SIZE    		104
#define NUM_TAPS         	101

typedef enum {LOW_PASS = 1, HIGH_PASS, BAND_PASS,DISABLE} filters_t;

void initialize_fir_lowpass(arm_fir_instance_f32* S, float32_t* state);
void initialize_fir_bandpass(arm_fir_instance_f32* S, float32_t* state);
void initialize_fir_highpass(arm_fir_instance_f32* S, float32_t* state);

void call_fir_lowpass(arm_fir_instance_f32* S, float32_t* pts_src, float32_t* ptr_dst);
void call_fir_bandpass(arm_fir_instance_f32* S, float32_t* pts_src, float32_t* ptr_dst);
void call_fir_highpass(arm_fir_instance_f32* S, float32_t* pts_src, float32_t* ptr_dst);

void init_filters(uint32_t *buffer);

void apply_filter(uint8_t select);

#endif /* P4_EMBEBIDOS2_FILTERS_H_ */
