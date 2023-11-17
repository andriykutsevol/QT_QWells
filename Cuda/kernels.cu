/* *
 * Copyright 1993-2012 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 */
#include "../src/header.h"

#include <stdio.h>
#include <stdlib.h>

#include <cuda.h>
#include <curand.h>
#include <curand_kernel.h>


__global__ void add_ker( int a, int b, int *c ) {
	printf("hello from kernel\n");
    *c = a + b;
}


__global__ void setup_kernel(curandState *state){
	int id = threadIdx.x + blockIdx.x * blockDim.x;    // 64 = blockDim.x
	printf("KER_SET:  id, blockIdx.x, threadIdx.x = %d, %d, %d\n", id, blockIdx.x, threadIdx.x);
	curand_init(1, id, 0, &state[id]);   // (seed, sequence, offset, curandState *state)

}

__global__ void generate_kernel(curandState *state, int *result){
	int id = threadIdx.x + blockIdx.x * blockDim.x;
	int count = 0;
	unsigned int x;
	curandState localState = state[id];
	x = curand(&localState);
	printf("KER_GEN: blockIdx.x, threadIdx.x, x = %d,%d, %d\n",blockIdx.x, threadIdx.x, x);
	// Check if odd;
	if(x & 1){
		count ++;
	}
	state[id] = localState;
	result[id] += count;
}
