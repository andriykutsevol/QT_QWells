/* *
 * Copyright 1993-2012 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


#include "monte_cuda_lib.h"

int test_lib_func(int a, int b){

	std::cout << "test_lib_func" << std::endl;
	return f_cuda_add_vec(a,b);
}

//int f_monte_cudev(int a, int b){
//
//	std::cout << "test_lib_func" << std::endl;
//	return f_cuda_add_vec(a,b);
//}
