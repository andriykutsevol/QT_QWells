#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/generate.h>
#include <thrust/sort.h>
#include <thrust/copy.h>
#include <cstdlib>

#include "common/book.h"
#include "header.h"
#include <cuda.h>
#include <cuda_runtime.h>

// http://stackoverflow.com/questions/13245258/handle-error-not-found-error-in-cuda
//static void HandleError( cudaError_t err,
//                         const char *file,
//                         int line ) {
//    if (err != cudaSuccess) {
//        printf( "%s in %s at line %d\n", cudaGetErrorString( err ),
//                file, line );
//        exit( EXIT_FAILURE );
//    }
//}
//#define HANDLE_ERROR( err ) (HandleError( err, __FILE__, __LINE__ ))


__global__ void add(int a, int b, int *c){
    *c = a + b;
}

extern "C"
void cuda_main()
{
        int c;
        int *dev_c;
        HANDLE_ERROR( cudaMalloc( (void**)&dev_c, sizeof(int) ) );

        add<<<1,1>>>( 2, 7, dev_c );

        HANDLE_ERROR( cudaMemcpy( &c, dev_c, sizeof(int),
                                  cudaMemcpyDeviceToHost ) );
        printf( "2 + 7 = %d\n", c );
        HANDLE_ERROR( cudaFree( dev_c ) );


   // return cudaGetLastError();
}



//====================================================================


// //*/ generate 32M random numbers on the host
//    thrust::host_vector<int> h_vec(32 << 20);
//    thrust::generate(h_vec.begin(), h_vec.end(), rand);

//    // transfer data to the device
//    thrust::device_vector<int> d_vec = h_vec;

//    // sort data on the device (846M keys per second on GeForce GTX 480)
//    thrust::sort(d_vec.begin(), d_vec.end());

//    // transfer data back to host
//    thrust::copy(d_vec.begin(), d_vec
