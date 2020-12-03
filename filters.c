/*
 * filters.c
 *
 *  Created on: Nov 30, 2020
 *      Author: sergio_mndz
 */

#include "filters.h"

uint32_t numBlocks = LENGTH_SAMPLES/BLOCK_SIZE;
uint32_t blockSize = BLOCK_SIZE;

void initialize_fir(arm_fir_instance_f32* S, float32_t* coeffs, float32_t* state)
{
	arm_fir_init_f32(S, NUM_TAPS, coeffs, state, blockSize);
}

void call_fir(arm_fir_instance_f32* S, float32_t* ptr_src, float32_t* ptr_dst)
{
	arm_fir_f32(S, ptr_src, ptr_dst, blockSize);
}
