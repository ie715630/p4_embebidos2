/*
 * filters.c
 *
 *  Created on: Nov 30, 2020
 *      Author: sergio_mndz
 */

#include "filters.h"
#include "arm_math.h"

uint32_t numBlocks = LENGTH_SAMPLES/BLOCK_SIZE;
uint32_t blockSize = BLOCK_SIZE;
static float32_t sysOut[LENGTH_SAMPLES];

arm_fir_instance_f32 S[3];
static float32_t firState[3][BLOCK_SIZE + NUM_TAPS - 1];

const float64_t FIR_LPass[NUM_TAPS] = {
		0.0008,
		0.0008,
		0.0009,
		0.0009,
		0.0010,
		0.0012,
		0.0013,
		0.0015,
		0.0017,
		0.0019,
		0.0022,
		0.0025,
		0.0028,
		0.0031,
		0.0035,
		0.0039,
		0.0044,
		0.0048,
		0.0053,
		0.0058,
		0.0064,
		0.0069,
		0.0075,
		0.0081,
		0.0087,
		0.0094,
		0.0100,
		0.0106,
		0.0113,
		0.0119,
		0.0126,
		0.0132,
		0.0138,
		0.0145,
		0.0151,
		0.0156,
		0.0162,
		0.0167,
		0.0173,
		0.0177,
		0.0182,
		0.0186,
		0.0190,
		0.0193,
		0.0196,
		0.0199,
		0.0201,
		0.0202,
		0.0204,
		0.0204,
		0.0205,
		0.0204,
		0.0204,
		0.0202,
		0.0201,
		0.0199,
		0.0196,
		0.0193,
		0.0190,
		0.0186,
		0.0182,
		0.0177,
		0.0173,
		0.0167,
		0.0162,
		0.0156,
		0.0151,
		0.0145,
		0.0138,
		0.0132,
		0.0126,
		0.0119,
		0.0113,
		0.0106,
		0.0100,
		0.0094,
		0.0087,
		0.0081,
		0.0075,
		0.0069,
		0.0064,
		0.0058,
		0.0053,
		0.0048,
		0.0044,
		0.0039,
		0.0035,
		0.0031,
		0.0028,
		0.0025,
		0.0022,
		0.0019,
		0.0017,
		0.0015,
		0.0013,
		0.0012,
		0.0010,
		0.0009,
		0.0009,
		0.0008,
		0.0008,
};

const float64_t FIR_BPass[NUM_TAPS] = {
		0.0000,
		-0.0002,
		-0.0005,
		-0.0009,
		-0.0013,
		-0.0014,
		-0.0012,
		-0.0008,
		-0.0003,
		-0.0001,
		-0.0002,
		-0.0009,
		-0.0021,
		-0.0034,
		-0.0043,
		-0.0043,
		-0.0034,
		-0.0018,
		-0.0003,
		0.0002,
		-0.0009,
		-0.0036,
		-0.0072,
		-0.0102,
		-0.0113,
		-0.0099,
		-0.0061,
		-0.0015,
		0.0019,
		0.0019,
		-0.0022,
		-0.0096,
		-0.0176,
		-0.0229,
		-0.0227,
		-0.0161,
		-0.0050,
		0.0064,
		0.0126,
		0.0097,
		-0.0036,
		-0.0239,
		-0.0440,
		-0.0548,
		-0.0481,
		-0.0202,
		0.0269,
		0.0846,
		0.1403,
		0.1806,
		0.1953,
		0.1806,
		0.1403,
		0.0846,
		0.0269,
		-0.0202,
		-0.0481,
		-0.0548,
		-0.0440,
		-0.0239,
		-0.0036,
		0.0097,
		0.0126,
		0.0064,
		-0.0050,
		-0.0161,
		-0.0227,
		-0.0229,
		-0.0176,
		-0.0096,
		-0.0022,
		0.0019,
		0.0019,
		-0.0015,
		-0.0061,
		-0.0099,
		-0.0113,
		-0.0102,
		-0.0072,
		-0.0036,
		-0.0009,
		0.0002,
		-0.0003,
		-0.0018,
		-0.0034,
		-0.0043,
		-0.0043,
		-0.0034,
		-0.0021,
		-0.0009,
		-0.0002,
		-0.0001,
		-0.0003,
		-0.0008,
		-0.0012,
		-0.0014,
		-0.0013,
		-0.0009,
		-0.0005,
		-0.0002,
		0.0002
};

const float64_t FIR_HPass[NUM_TAPS] = {
		-0.0005,
		-0.0003,
		-0.0003,
		0.0004,
		0.0006,
		0.0007,
		0.0004,
		-0.0001,
		-0.0007,
		-0.0012,
		-0.0012,
		-0.0006,
		0.0004,
		0.0015,
		0.0022,
		0.0019,
		0.0007,
		-0.0012,
		-0.0030,
		-0.0038,
		-0.0030,
		-0.0006,
		0.0026,
		0.0053,
		0.0060,
		0.0042,
		-0.0027,
		-0.0050,
		-0.0088,
		-0.0092,
		-0.0054,
		0.0016,
		0.0092,
		0.0141,
		0.0136,
		0.0066,
		-0.0049,
		-0.0166,
		-0.0232,
		-0.0205,
		-0.0075,
		0.0126,
		0.0325,
		0.0431,
		0.0363,
		0.0081,
		-0.0392,
		-0.0973,
		-0.1533,
		-0.1937,
		0.7921,
		-0.1937,
		-0.1533,
		-0.0973,
		-0.0392,
		0.0081,
		0.0363,
		0.0431,
		0.0325,
		0.0126,
		-0.0075,
		-0.0205,
		-0.0232,
		-0.0166,
		-0.0049,
		0.0066,
		0.0136,
		0.0141,
		0.0092,
		0.0016,
		-0.0054,
		-0.0092,
		-0.0088,
		-0.0050,
		-0.0027,
		0.0042,
		0.0060,
		0.0053,
		0.0026,
		-0.0006,
		-0.0030,
		-0.0038,
		-0.0030,
		-0.0012,
		0.0007,
		0.0019,
		0.0022,
		0.0015,
		0.0004,
		-0.0006,
		-0.0012,
		-0.0012,
		-0.0007,
		-0.0001,
		0.0004,
		0.0007,
		0.0006,
		0.0004,
		-0.0003,
		-0.0003,
		-0.0005,
};

float32_t *input, *output;

void initialize_fir_lowpass(arm_fir_instance_f32* S, float32_t* state)
{
	arm_fir_init_f32(S, NUM_TAPS, (float32_t *)&FIR_LPass[0], state, blockSize);
}

void initialize_fir_bandpass(arm_fir_instance_f32* S, float32_t* state)
{
	arm_fir_init_f32(S, NUM_TAPS, (float32_t *)&FIR_BPass[0], state, blockSize);
}

void initialize_fir_highpass(arm_fir_instance_f32* S, float32_t* state)
{
	arm_fir_init_f32(S, NUM_TAPS, (float32_t *)&FIR_HPass[0], state, blockSize);
}

void call_fir_lowpass(arm_fir_instance_f32* S, float32_t* ptr_src, float32_t* ptr_dst)
{
	arm_fir_f32(S, ptr_src, ptr_dst, blockSize);
}

void call_fir_bandpass(arm_fir_instance_f32* S, float32_t* ptr_src, float32_t* ptr_dst)
{
	arm_fir_f32(S, ptr_src, ptr_dst, blockSize);
}

void call_fir_highpass(arm_fir_instance_f32* S, float32_t* ptr_src, float32_t* ptr_dst)
{
	arm_fir_f32(S, ptr_src, ptr_dst, blockSize);
}


void init_filters(uint32_t *buffer)
{
	input = (float32_t*)buffer;
	output = sysOut;
	initialize_fir_lowpass(&S[1], &firState[0][0]);
	initialize_fir_highpass(&S[2], &firState[1][0]);
	initialize_fir_bandpass(&S[3], &firState[2][0]);
}

void apply_filter(uint8_t select)
{
    switch(select){
        case 1:
        	  for(int i=0; i < numBlocks; i++)
        	  {
        		  call_fir_lowpass(&S[0], input + (i+BLOCK_SIZE), output + (i+BLOCK_SIZE));
        	  }
            break;
        case 2:
			  for(int i=0; i < numBlocks; i++)
			  {
				  call_fir_highpass(&S[1], input + (i+BLOCK_SIZE), output + (i+BLOCK_SIZE));
			  }
            break;
        case 3:
			  for(int i=0; i < numBlocks; i++)
			  {
				  call_fir_bandpass(&S[2], input + (i+BLOCK_SIZE), output + (i+BLOCK_SIZE));
			  }
            break;
        case 4:
			  for(int i=0; i < numBlocks; i++)
			  {
				  output[i] = input[i];
			  }
			break;
        default:
            break;
    }
}
