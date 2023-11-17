/*
 * phontan_cuda_lib.h
 *
 *  Created on: 11.03.2014
 *      Author: andrey
 */

#ifndef PHONTAN_CUDA_LIB_H_
#define PHONTAN_CUDA_LIB_H_

#include <cuda.h>
#include <cuda_runtime.h>

#include <curand.h>
#include <curand_kernel.h>

//#include <GL/glew.h>
//#include <GL/glut.h>

#include <GL/gl.h>
#include <GL/glu.h>

int test_lib_func(int a, int b);

void runCuda(GLuint vbo);

//=====================================
//=====================================


void cudaUpdateBuff(struct cudaGraphicsResource **vbo_resource, unsigned int vbo_res_flags,
		GLuint *vbo,
		const unsigned int mesh_width,
		const unsigned int mesh_height, float g_fAnim);

void cudaUnregBuffer(cudaGraphicsResource *vbo_res);

//=====================================
//=====================================


void cuda_unreg_vbo(GLuint vbo);

__global__ void add_ker( int a, int b, int *c );


__global__ void kernel(float4* pos, float * pdata, unsigned int width,
unsigned int height, int max_age, float time,
	 float randy1, float randy2, float randy3,
	 float tx, float ty, float tz);

__global__ void fill_particles_kernel(float4 *pos, unsigned int width, unsigned int height, float time);



int f_cuda_add_vec( int a, int b );


#endif /* PHONTAN_CUDA_LIB_H_ */
