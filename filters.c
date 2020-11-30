/*
 * filters.c
 *
 *  Created on: Nov 30, 2020
 *      Author: sergio_mndz
 */

#include "filters.h"

static float32_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1];

uint32_t numBlocks = LENGTH_SAMPLES/BLOCK_SIZE;
uint32_t blockSize = BLOCK_SIZE;

arm_fir_instance_f32 S;
float32_t *inputF32, *outputF32;
arm_status status;
uint32_t i;

void initialize_fir(arm_fir_instance_f32* S, float32_t* coeffs, float32_t* state)
{
	arm_fir_init_f32(S, NUM_TAPS, coeffs, state, blockSize);
}

void call_fir(arm_fir_instance_f32* S, float32_t* pts_src, float32_t* ptr_dst)
{
	arm_fir_f32(S, ptr_src, ptr_dst, blockSize);
}
