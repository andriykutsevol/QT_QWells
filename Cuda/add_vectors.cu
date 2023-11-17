/*
 * Copyright 1993-2010 NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property and
 * proprietary rights in and to this software and related documentation.
 * Any use, reproduction, disclosure, or distribution of this software
 * and related documentation without an express license agreement from
 * NVIDIA Corporation is strictly prohibited.
 *
 * Please refer to the applicable NVIDIA end user license agreement (EULA)
 * associated with this source code for terms and conditions that govern
 * your use of this NVIDIA software.
 *
 */
#include "../src/header.h"
#include "../../common/book.h"

#include <iostream>

#include <cuda.h>
#include <curand.h>
#include <curand_kernel.h>

#include <cula.h>


int f_cuda_add_vec( void ) {

    int c;
    int *dev_c;
    HANDLE_ERROR( cudaMalloc( (void**)&dev_c, sizeof(int) ) );

    add_ker<<<1,1>>>( 2, 7, dev_c );

    HANDLE_ERROR( cudaMemcpy( &c, dev_c, sizeof(int),
                              cudaMemcpyDeviceToHost ) );
    printf( "2 + 7 = %d\n", c );
    HANDLE_ERROR( cudaFree( dev_c ) );

    printf( "Hello from CUDA!\n" );
    return 0;
}


void f_curand_host_api(void){

	size_t n = 10;
	size_t i;

	curandGenerator_t gen;

	float *devData, *hostData;

	hostData = (float *) calloc(n, sizeof(float));

	cudaMalloc((void **)&devData, n*sizeof(float));

	curandCreateGenerator(&gen, CURAND_RNG_PSEUDO_MTGP32);

	curandSetPseudoRandomGeneratorSeed(gen, 123ULL);

	curandGenerateUniform(gen, devData, n);

	cudaMemcpy(hostData, devData, n*sizeof(float), cudaMemcpyDeviceToHost);

	printf("Random Unif(0,1) draws:\n");
	for(i=0; i<n; i++){
		printf("%1.4f\n", hostData[i]);
	}
	printf("\n");

	curandDestroyGenerator(gen);
	cudaFree(devData);
	free(hostData);
}


void f_curand_dev_api(){
	const int N = 20;
	int i, total;

	int *devResults, *hostResults;
	curandState *devStates;

	hostResults = (int *) calloc(N*N, sizeof(int));

	cudaMalloc((void **)&devResults, N*N*sizeof(int));

	cudaMemset(devResults, 0, N*N*sizeof(int));

	cudaMalloc((void**)&devStates, N*N*sizeof(curandState));

	setup_kernel<<<N,N>>>(devStates);

	//генерировать и использовать псевдослучайные числа.
	//for(i = 0; i<10; i++){
	//	std::cout << "from for " << i << std::endl;
	generate_kernel <<< N,N >>>(devStates, devResults);
	//}
	cudaThreadSynchronize();

	cudaMemcpy(hostResults, devResults, N*N*sizeof(int), cudaMemcpyDeviceToHost);
	// показать резы.
	total = 0;
	for (i=0; i<3*3; i++){
		printf("hostResults\n");
		total += hostResults[i];
	}
	printf("Fraction odd was%10.13f\n", (float) total /(3.0f * 3.0f * 10.0f * 10.0f));

	cudaFree(devStates);
	cudaFree(devResults);
	free(hostResults);

// The END;
}


void f_cula_dev(void){

	// кроме настроек в eclipse надо еще создасть ссылки
	// из /usr/local/cula/lib64/ в /usr/lib/

	culaStatus s;

	s = culaInitialize();
	if(s != culaNoError)
	{
		printf("%s\n", culaGetStatusString(s));
    // /* ... Error Handling ... */
	}

	/* ... Your code ... */

	culaShutdown();

}
















