/*
 * header.h
 *
 *  Created on: 08.01.2014
 *      Author: andrey
 */

#ifndef HEADER_H_
#define HEADER_H_

#include <cuda.h>
#include <cuda_runtime.h>

#include <curand.h>
#include <curand_kernel.h>



int f_cuda_add_vec(void);

__global__ void add_ker( int a, int b, int *c );

__global__ void setup_kernel(curandState *state);

__global__  void generate_kernel(curandState *state, int *result);

// ==============================================


void f_curand_host_api(void);

//===============================================

void f_curand_dev_api(void);


//===============================================

void f_cula_dev(void);


//===============================================

void test_arithm(void);


#endif /* HEADER_H_ */
