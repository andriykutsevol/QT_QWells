/*
 * monte_cuda_lib.h
 *
 *  Created on: 14.02.2014
 *      Author: andrey
 */

#ifndef MONTE_CUDA_LIB_H_
#define MONTE_CUDA_LIB_H_

#include <cuda.h>
#include <cuda_runtime.h>

#include <fstream>

#include <curand.h>
#include <curand_kernel.h>

int test_lib_func(int a, int b);

__global__ void add_ker( int a, int b, int *c );

int f_cuda_add_vec( int a, int b );

//==============================



int f_monte_cudev();
//
//__global__ void setup_sobols_kernel(curandStateSobol64_t *d_sobolStates,
//		curandDirectionVectors64_t *d_rngDirections, int numSims);

//===============================




#endif /* MONTE_CUDA_LIB_H_ */
