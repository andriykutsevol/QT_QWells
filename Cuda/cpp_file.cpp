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

void test_cpp_func(){
	printf("Hello from cpp_file.cpp\n");
}



void test_arithm(void){

std::cout << " 5 % 2 = " << 5%2 << std::endl;
std::cout << " 5 / 2 = " << 5/2 << std::endl;
std::cout << " 5 / 4 = " << 5/4 << std::endl;
std::cout << " 43 % 10 = " << 43%10 << std::endl;
std::cout << " 8 / 7 = " << 8/7 << std::endl;
std::cout << " 8 % 7 = " << 8%7 << std::endl;


int x;
    for (x = 0; x < 10; x++)
        if (x & 1)							// если x & 1 не равны ноль тогда печать.
        									// т.к если равны то печати не будет.
            printf("%d is odd\n", x);

    for (x = 0; x < 10; x++)
    	if((x & 1) == 0)					// так понятнее.
    	    printf("%d is EVEN!\n", x);
    	else
    	    printf("%d is ODD!\n", x);

}
