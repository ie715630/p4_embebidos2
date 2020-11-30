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

#define LENGTH_SAMPLES		320

#define BLOCK_SIZE    		32
#define NUM_TAPS         	29

const float32_t firCoeffs[NUM_TAPS] = {0};

void initialize_fir(arm_fir_instance_f32* S, float32_t* coeffs, float32_t* state);

void call_fir(arm_fir_instance_f32* S, float32_t* pts_src, float32_t* ptr_dst);

#endif /* P4_EMBEBIDOS2_FILTERS_H_ */
