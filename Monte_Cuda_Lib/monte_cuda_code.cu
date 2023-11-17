#include <iostream>
#include <fstream>

#include <cuda.h>
#include <curand.h>
#include <curand_kernel.h>
#include <cuda_runtime.h>
#include <math.h>

#include <vector_types.h>
#include <typeinfo>

#include <string>
#include <vector>
#include <numeric>
#include <stdexcept>


#include <cula.h>

const int N = 33*1024;
const int numSims = 400000;
// для успешной редукции threadsPerBlock должно быть степерью 2
// 2^9 = 512;
const int threadsPerBlock = 512;
const int blocksPerGrid = 16;

//==============================


__global__ void add_ker( int a, int b, int *c ) {
	printf("hello from kernel\n");
    *c = a + b;
}

int f_cuda_add_vec( int a, int b) {

    int c;
    int *dev_c;
    cudaMalloc( (void**)&dev_c, sizeof(int) );

    add_ker<<<1,1>>>( a, b, dev_c );

    cudaMemcpy( &c, dev_c, sizeof(int),
                              cudaMemcpyDeviceToHost );
    printf( "2 + 7 = %d\n", c );
    cudaFree( dev_c );

   // curandDirectionVectors64_t *rngDirections;
   // curandGetDirectionVectors64(&rngDirections, CURAND_DIRECTION_VECTORS_64_JOEKUO6);

    printf( "Hello from CUDA!\n" );
    return c;
}


//======================================
//http://kvm.gubkin.ru/vip3p2/g4.pdf
//======================================

// это из самой cuda.
//void curand_init(curandDirectionVectors64_t direction_vectors,
//                                           unsigned long long offset,
//                                           curandStateSobol64_t *state)

__global__ void setup_sobols_kernel(curandStateSobol64_t *d_sobolStates,
		int dim_xn, curandDirectionVectors64_t *d_rngDirections, int numSims){
	unsigned int tid = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned int step = gridDim.x * blockDim.x;

	while(tid < numSims){
		for(int ldim=0; ldim < dim_xn; ldim++){
			curand_init(d_rngDirections[ldim], tid, &d_sobolStates[tid + ldim*numSims]);
		}
		tid += step;
	}

}

//================================

__device__ void getPoints(double *xn, curandStateSobol64_t *d_sobolStates,
		double *low_lims, double *top_lims, int tid, int dim_xn, int numSims){

	for(int d=0; d<dim_xn; d++){
		xn[d] = (curand_uniform_double(&d_sobolStates[tid + numSims*d]) *
				(top_lims[d] - low_lims[d])) + low_lims[d];
	}
}

//================================

//__global__ void get_sums_kernel(curandStateSobol64_t *d_sobolStates,
//		double *d_results, int numSims){
//
//	unsigned int tid = blockIdx.x * blockDim.x + threadIdx.x;
//	unsigned int step = gridDim.x * blockDim.x;
//
//
//	const int dim_xn = 4;
//	double xn_s[dim_xn];
//	double low_lims[dim_xn];
//	double top_lims[dim_xn];
//
//	low_lims[0] = 1; low_lims[1] = 1;  low_lims[2] = 0; low_lims[3] = 0; 
//	top_lims[0] = 3; top_lims[1] = 27; top_lims[2] = 90; top_lims[3] = 15;
//
//	while(tid < numSims){
//		getPoints(&xn_s[0], d_sobolStates, &low_lims[0], &top_lims[0], tid, dim_xn, numSims);
//		if(  (xn_s[1] > xn_s[0]) && ( xn_s[1] < pow(xn_s[0],3) )     ){
//			if(   (xn_s[2] > 0) && ( xn_s[2] < (pow(xn_s[0],2) + xn_s[0] * xn_s[1]) )    ){
//				//d_results[tid] = 1;
//				if( (xn_s[3] > 0) && (xn_s[3] < (xn_s[2] / 6.0)) ) {  
//					d_results[tid] = 1;
//				}else{
//					d_results[tid] = 0;
//				}
//			}else{
//				d_results[tid] = 0;
//			}
//		}else{
//			d_results[tid] = 0;
//		}
//
//		tid += step;
//	}
//
//}

__global__ void get_sums_kernel(curandStateSobol64_t *d_sobolStates,
		double *d_results, int numSims){

	unsigned int tid = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned int step = gridDim.x * blockDim.x;

	int cacheIndex = threadIdx.x;
	__shared__ float cache[threadsPerBlock];
	// локальная копия cache создается в каждом блоке.
	// поэтому и размер cache = колличеству нитей в одном лишь блоке.

	const int dim_xn = 4;
	double xn_s[dim_xn];
	double low_lims[dim_xn];
	double top_lims[dim_xn];

	low_lims[0] = 1; low_lims[1] = 1;  low_lims[2] = 0; low_lims[3] = 0; 
	top_lims[0] = 3; top_lims[1] = 27; top_lims[2] = 90; top_lims[3] = 15;

	float temp = 0;
	// это линейное вычисление внутри самой нити.
	// т.е мы досчитываем в нити то,
	// на что не хватило размерностей сетки. Поэтому и temp не векторный.
	while(tid < numSims){
		getPoints(&xn_s[0], d_sobolStates, &low_lims[0], &top_lims[0], tid, dim_xn, numSims);
		if(  (xn_s[1] > xn_s[0]) && ( xn_s[1] < pow(xn_s[0],3) )     )
			if(   (xn_s[2] > 0) && ( xn_s[2] < (pow(xn_s[0],2) + xn_s[0] * xn_s[1]) )    )
				if( (xn_s[3] > 0) && (xn_s[3] < (xn_s[2] / 6.0)) ) 
					temp ++;

		tid += step;
	}
	// cacheIndex = threadIdx.x ;
	// теперь уже для каждой нити мы записываем результат.
	cache[cacheIndex] = temp;
	__syncthreads();
	// все нити отработали
	// теперь получим сумму для каждого блока.

	// стр.75. Каждая нить объединяет два элемента в один. По завершении
	// одного шага кол-во элементов в массиве уменьшится вдвое.
	// Потом __syncthreads(), потом опять уменьшится вдвое.
	// И так за log2(threadsPerBlock) шагов вычислим сумму
	// всех элементов массива cache[].
	int i = blockDim.x / 2;
	while(i != 0){
		if(cacheIndex < i)
			cache[cacheIndex] += cache[cacheIndex + i];
		__syncthreads();
		i /= 2;
	}

	if(cacheIndex == 0)
		d_results[blockIdx.x] = cache[0];

}

//===============================
// Интеграл с отрицательными значениями.
// Найти площадь плоской фигуры, ограниченной линиями
// y = 2x - x^2, y = -x;
//===============================

__global__ void get_sums_kernel_2(curandStateSobol64_t *d_sobolStates,
		double *d_results, int numSims){

	unsigned int tid = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned int step = gridDim.x * blockDim.x;

	int cacheIndex = threadIdx.x;
	__shared__ float cache[threadsPerBlock];
	// локальная копия cache создается в каждом блоке.
	// поэтому и размер cache = колличеству нитей в одном лишь блоке.

	const int dim_xn = 2;
	double xn_s[dim_xn];
	double low_lims[dim_xn];
	double top_lims[dim_xn];

	low_lims[0] = 0; low_lims[1] = 1;
	top_lims[0] = 3; top_lims[1] = -3;

	float temp = 0;
	// это линейное вычисление внутри самой нити.
	// т.е мы досчитываем в нити то,
	// на что не хватило размерностей сетки. Поэтому и temp не векторный.
	while(tid < numSims){

		getPoints(&xn_s[0], d_sobolStates, &low_lims[0], &top_lims[0], tid, dim_xn, numSims);
		if(xn_s[1] > 0){
			if((xn_s[1] > (-xn_s[0])) && (xn_s[1] < (2 * xn_s[0] - xn_s[0] * xn_s[0]))){
				temp ++;
				printf("%f %f \n", xn_s[0], xn_s[1]);
			}else{
				temp --;
			}
		}
		tid += step;
	}
	// cacheIndex = threadIdx.x ;
	// теперь уже для каждой нити мы записываем результат.
	cache[cacheIndex] = temp;
	__syncthreads();
	// все нити отработали
	// теперь получим сумму для каждого блока.

	// стр.75. Каждая нить объединяет два элемента в один. По завершении
	// одного шага кол-во элементов в массиве уменьшится вдвое.
	// Потом __syncthreads(), потом опять уменьшится вдвое.
	// И так за log2(threadsPerBlock) шагов вычислим сумму
	// всех элементов массива cache[].
	int i = blockDim.x / 2;
	while(i != 0){
		if(cacheIndex < i)
			cache[cacheIndex] += cache[cacheIndex + i];
		__syncthreads();
		i /= 2;
	}

	if(cacheIndex == 0)
		d_results[blockIdx.x] = cache[0];

}

//================================

//===============================
// Интеграл с отрицательными значениями.
// Вычислить именно интеграл
// int_0^3pi/2 cos(x) dx.
//===============================

__global__ void get_sums_kernel_3(curandStateSobol64_t *d_sobolStates,
		double *d_results, int numSims){

	unsigned int tid = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned int step = gridDim.x * blockDim.x;

	int cacheIndex = threadIdx.x;
	__shared__ float cache[threadsPerBlock];
	// локальная копия cache создается в каждом блоке.
	// поэтому и размер cache = колличеству нитей в одном лишь блоке.

	const int dim_xn = 2;		 
	double xn_s[dim_xn];
	double low_lims[dim_xn];
	double top_lims[dim_xn];

	low_lims[0] = 0;              low_lims[1] = -1; 
	top_lims[0] = (3 * 3.14 / 2); top_lims[1] =  1;

	float temp = 0;
	// это линейное вычисление внутри самой нити.
	// т.е мы досчитываем в нити то,
	// на ++ сетки. Поэтому и temp не векторный.
	while(tid < numSims){

		getPoints(&xn_s[0], d_sobolStates, &low_lims[0], &top_lims[0], tid, dim_xn, numSims);
		if((xn_s[1] > 0) && (xn_s[1] < (cos(xn_s[0])))) {
			//printf("%f %f \n", xn_s[0], xn_s[1]);
			temp ++;
		}else if((xn_s[1] < 0) && (xn_s[1] > cos(xn_s[0]))){
			temp --;
			//printf("%f %f \n", xn_s[0], xn_s[1]);
		}

		tid += step;
	}
	// cacheIndex = threadIdx.x ;
	// теперь уже для каждой нити мы записываем результат.
	cache[cacheIndex] = temp;
	__syncthreads();
	// все нити отработали
	// теперь получим сумму для каждого блока.

	// стр.75. Каждая нить объединяет два элемента в один. По завершении
	// одного шага кол-во элементов в массиве уменьшится вдвое.
	// Потом __syncthreads(), потом опять уменьшится вдвое.
	// И так за log2(threadsPerBlock) шагов вычислим сумму
	// всех элементов массива cache[].
	int i = blockDim.x / 2;
	while(i != 0){
		if(cacheIndex < i)
			cache[cacheIndex] += cache[cacheIndex + i];
		__syncthreads();
		i /= 2;
	}

	if(cacheIndex == 0)
		d_results[blockIdx.x] = cache[0];

}


int f_monte_cudev(){



	// Размерность устанавливается тут и в get_sums_kernel
	// в виде const int dim_xn = 3;
	//int dim_xn = 4;			// !! для get_sums_kernel
	int dim_xn = 2;			// !! для get_sums_kernel_2
	dim3 block;
	block.x = threadsPerBlock;
	dim3 grid;
	grid.x = blocksPerGrid;


	curandStateSobol64_t *d_sobolStates = 0;
	// dimmensional - dim_xn*numSims
	// состояний столько сколько симуляций * размерность.
	cudaMalloc((void **)&d_sobolStates, dim_xn*numSims * sizeof(curandStateSobol64_t));

	curandDirectionVectors64_t *d_rngDirections = 0;
	// dimmensional - dim_xn
	// векторов направлений столько, сколько размерностей.
	cudaMalloc((void **)&d_rngDirections, dim_xn * sizeof(curandDirectionVectors64_t));

	curandDirectionVectors64_t *rngDirections;
	curandGetDirectionVectors64(&rngDirections, CURAND_DIRECTION_VECTORS_64_JOEKUO6);

	cudaMemcpy(d_rngDirections, rngDirections, dim_xn * sizeof(curandDirectionVectors64_t), cudaMemcpyHostToDevice);

	//setup_sobol_kernel<<<grid, block>>>(d_sobolStates, d_rngDirections, numSims);
	setup_sobols_kernel<<<grid, block>>>(d_sobolStates, dim_xn, d_rngDirections, numSims);


	double *d_results;
	cudaMalloc((void **)&d_results, numSims * sizeof(double));

	//get_sums_kernel<<<grid, block>>>(d_sobolStates, d_results, numSims);
	//get_sums_kernel_2<<<grid, block>>>(d_sobolStates, d_results, numSims);
	get_sums_kernel_3<<<grid, block>>>(d_sobolStates, d_results, numSims);

	cudaThreadSynchronize();

	double *host_results;
	host_results = (double*)calloc( numSims, sizeof(double) );
	cudaMemcpy(host_results, d_results, numSims * sizeof(double), cudaMemcpyDeviceToHost );
	double sum = 0;

	for(int k = 0; k < blocksPerGrid; k++){
		sum += host_results[k];
	}

//	for(int k=0; k<(numSims); k++){
//		// то есть учитываем только точки, которые попали в область определения.
//		sum += host_results[k];
//	}
	//std::cout << "host_sum: " << sum << "; integral is: " << (2*26*90*15* sum / numSims) << std::endl;		 
	std::cout << "host_sum: " << sum << "; integral is: " << (2*(3*3.14/2) * sum / numSims) << std::endl;		 

	return 0;
}
