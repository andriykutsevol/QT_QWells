#include <stdio.h>
#include <stdlib.h>
#include <iostream>


#include <cuda.h>
#include <cuda_runtime.h>

#include <curand.h>
#include <curand_kernel.h>

#include <GL/glew.h>
#include <GL/glut.h>


#include "phontan_cuda_lib.h"

int test_lib_func(int a, int b){

	std::cout << "test_lib_func" << std::endl;
	//return f_cuda_add_vec(a,b);
	return 0;
}



