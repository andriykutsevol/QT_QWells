
///////////////////////////////////////////////////////////////////////////////
//! Simple kernel to modify vertex positions in sine wave pattern
//! @param data  data in global memory
///////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>

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


#include "phontan_cuda_lib.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cuda_gl_interop.h>

#include <time.h>

const unsigned int window_width = 512;
const unsigned int window_height = 512;

const unsigned int mesh_width = 256; //256;
const unsigned int mesh_height = 256; //256;

//andy
const unsigned int max_age = 10000;

// vbo variables
GLuint vbo;
GLfloat vert_data[] = {-1.0f, 1.0f, -1.0f,
1.0f, 1.0f, -1.0f,
1.0f, -1.0f, -1.0f};

float anim = 0.0;

// mouse controls
int mouse_old_x, mouse_old_y;
int mouse_buttons = 0;
float rotate_x = 0.0, rotate_y = 0.0;
float translate_z = -3.0;

//andy
float * d_particleData;
float * h_particleData;

float targetX, targetY, targetZ;



///////////////////////////////////////////////////////////////////////////////
//! Simple kernel to modify vertex positions in sine wave pattern
//! @param data  data in global memory
///////////////////////////////////////////////////////////////////////////////
__global__ void simple_vbo_kernel(float4 *pos, unsigned int width, unsigned int height, float time)
{
    unsigned int x = blockIdx.x*blockDim.x + threadIdx.x;
    unsigned int y = blockIdx.y*blockDim.y + threadIdx.y;

    // calculate uv coordinates
    float u = x / (float) width;
    float v = y / (float) height;
    u = u*2.0f - 1.0f;
    v = v*2.0f - 1.0f;

    // calculate simple sine wave pattern
    float freq = 4.0f;
    float w = sinf(u*freq + time) * sinf(v*freq + time) * 0.5f;

    // write output vertex
    pos[y*width+x] = make_float4(u, w, v, 1.0f);
}



void cudaUpdateBuff(struct cudaGraphicsResource **vbo_resource, unsigned int vbo_res_flags,
		GLuint *vbo,
		const unsigned int mesh_width,
		const unsigned int mesh_height, float g_fAnim){

	 // register this buffer object with CUDA
	    cudaGraphicsGLRegisterBuffer(vbo_resource, *vbo, vbo_res_flags);

	 // map OpenGL buffer object for writing from CUDA
	   	float4 *dptr;
	   	cudaGraphicsMapResources(1, vbo_resource, 0);
	   	size_t num_bytes;
	   	cudaGraphicsResourceGetMappedPointer((void **)&dptr, &num_bytes,
	   	                                                         *vbo_resource);


	   // launch_kernel(dptr, mesh_width, mesh_height, g_fAnim);
	    // execute the kernel
	    dim3 block(8, 8, 1);
	    dim3 grid(mesh_width / block.x, mesh_height / block.y, 1);
	    simple_vbo_kernel<<< grid, block>>>(dptr, mesh_width, mesh_height, g_fAnim);

	    // unmap buffer object
	    cudaGraphicsUnmapResources(1, vbo_resource, 0);
	    cudaGraphicsUnregisterResource(*vbo_resource);

	    //std::cout<<"end cudaUpdateBuff." << std::endl;

}












































void runCuda(GLuint vbo){
//	       glBegin(GL_TRIANGLES);
//	           glVertex3f(0.0f,1.0f,0.0f);
//	           glVertex3f(-1.0f,-1.0f,0.0f);
//	           glVertex3f(1.0f,-1.0f,0.0f);
//	       glEnd();


	float4 *dptr;

	cudaGLRegisterBufferObject(vbo);
	cudaGLMapBufferObject( (void**)&dptr, vbo);

	//cudaMemcpy(dptr,vert_data,9*sizeof(float),cudaMemcpyHostToDevice);
	dim3 block(8,8,1);
	dim3 grid(256/block.x, 256/block.y, 1);
	fill_particles_kernel<<<1,1>>>(dptr, 256, 256, 1.0f );
	cudaGLUnmapBufferObject(vbo);
	std::cout << "VBO: " << vbo << std::endl;
}



void cuda_unreg_vbo(GLuint vbo)
{

    cudaGLUnregisterBufferObject(vbo);
    vbo = 0;
}

__global__ void fill_particles_kernel(float4 *pos, unsigned int width, unsigned int height, float time){


	unsigned int x = blockIdx.x*blockDim.x + threadIdx.x;
	unsigned int y = blockIdx.y*blockDim.y + threadIdx.y;

	    // calculate uv coordinates
	    float u = x / (float) width;
	    float v = y / (float) height;
	    u = u*2.0f - 1.0f;
	    v = v*2.0f - 1.0f;

	    // calculate simple sine wave pattern
	    float freq = 4.0f;
	    float w = sinf(u*freq + time) * cosf(v*freq + time) * 0.5f;
//
//	    pos[y*width+x].x = u;
//	    pos[y*width+x].y = w;
//	    pos[y*width+x].z = v;
//	    pos[y*width+x].w = 1.0f;

	pos[0].x = 0.0f;
	pos[0].y = 1.0f;
	pos[0].z = 0.0f;
	pos[0].w = 1.0f;

	pos[1].x = -1.0f;
	pos[1].y = -1.0f;
	pos[1].z = 0.0f;
	pos[1].w = 1.0f;

	pos[2].x = 1.0f;
	pos[2].y = -1.0f;
	pos[2].z = 0.0f;
	pos[2].w = 1.0f;

//		pos[0] = 0.0f;
//		pos[1] = 0.0f;
//		pos[2] = 0.0f;
//
//		pos[3] = 0.0f;
//		pos[4] = 0.0f;
//		pos[5] = 0.0f;
//
//		pos[6] = 0.0f;
//		pos[7] = 0.0f;
//		pos[8]= 0.0f;

	printf("hello from kernel\n");

}








__global__ void kernel(float4* pos, float * pdata, unsigned int width,
unsigned int height, int max_age, float time,
	 float randy1, float randy2, float randy3,
	 float tx, float ty, float tz)
{
    unsigned int x = blockIdx.x*blockDim.x + threadIdx.x;
    unsigned int y = blockIdx.y*blockDim.y + threadIdx.y;

    unsigned int arrayLoc = y*width*4 + x*4;
    unsigned int posLoc = y*width+x;

    float rx, ry, rz;
    float vx, vy, vz;
float dx, dy, dz, sum;

    // swarms dont have an age - can tweak this later
    // should initialize in main program
    // for gravity we could initially randomly scatter the particles
       // and let them coallese
       // similar to random swarm placement but no initial V for gravity
       // for gravity need to compute affect of all other particles
       // should try to do an M x M problem and see how fast it is
    // should also try a spring mesh - only need to talk to your neighbours

    if (pdata[arrayLoc] >= max_age)
       {
       rx = (randy1 - 0.5);
       ry = (randy2 - 0.5);
       rz = (randy3 - 0.5);

       pdata[arrayLoc] = 0; // age
       pdata[arrayLoc+1] = 0.001 * rx * rx * rx;
       pdata[arrayLoc+2] = 0.001 * ry * ry * ry;
       pdata[arrayLoc+3] = 0.001 * rz * rz * rz;

       // any new ones spawn near the target

// this gives some interesting effects
//tx = ty = tz = 0.0;
//pdata[arrayLoc+2] = 0.0;

       pos[posLoc].x = tx + 2.0 * rx * rx * rx;
       pos[posLoc].y = ty + 2.0 * ry * ry * ry;
       pos[posLoc].z = tz + 2.0 * rz * rz * rz;
       }

     pdata[arrayLoc] += 1;	    // increase age


     dx = (tx - pos[posLoc].x);
     dy = (ty - pos[posLoc].y);
     dz = (tz - pos[posLoc].z);
     sum = sqrt(dx*dx + dy*dy + dz*dz);

    // update the velocity
// I should give them a constant velocity but use this to update direction
    vx = 0.000005 * dx/sum;
    vy = 0.000005 * dy/sum;
    vz = 0.000005 * dz/sum;

    pdata[arrayLoc+1] = pdata[arrayLoc+1] + vx;
    pdata[arrayLoc+2] = pdata[arrayLoc+2] + vy;
    pdata[arrayLoc+3] = pdata[arrayLoc+3] + vz;

    float newX = pos[posLoc].x + pdata[arrayLoc+1];
    float newY = pos[posLoc].y + pdata[arrayLoc+2];
    float newZ = pos[posLoc].z + pdata[arrayLoc+3];


     // now need to modify the color info in the array

     pos[width*height + posLoc].x = 1.0;
     pos[width*height + posLoc].y = 1.0 - 0.5 * pdata[arrayLoc]/max_age;
     pos[width*height + posLoc].z = 1.0 - pdata[arrayLoc]/max_age;

    // write output vertex
     pos[posLoc] = make_float4(newX, newY, newZ, 1.0f);
}















