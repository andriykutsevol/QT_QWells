################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../DOC/dev_sobol/7_CUDALibraries/MC_EstimatePiInlineQ/src/main.cpp \
../DOC/dev_sobol/7_CUDALibraries/MC_EstimatePiInlineQ/src/test.cpp 

CU_SRCS += \
../DOC/dev_sobol/7_CUDALibraries/MC_EstimatePiInlineQ/src/piestimator.cu \
../DOC/dev_sobol/7_CUDALibraries/MC_EstimatePiInlineQ/src/tmp.cu 

CU_DEPS += \
./DOC/dev_sobol/7_CUDALibraries/MC_EstimatePiInlineQ/src/piestimator.d \
./DOC/dev_sobol/7_CUDALibraries/MC_EstimatePiInlineQ/src/tmp.d 

OBJS += \
./DOC/dev_sobol/7_CUDALibraries/MC_EstimatePiInlineQ/src/main.o \
./DOC/dev_sobol/7_CUDALibraries/MC_EstimatePiInlineQ/src/piestimator.o \
./DOC/dev_sobol/7_CUDALibraries/MC_EstimatePiInlineQ/src/test.o \
./DOC/dev_sobol/7_CUDALibraries/MC_EstimatePiInlineQ/src/tmp.o 

CPP_DEPS += \
./DOC/dev_sobol/7_CUDALibraries/MC_EstimatePiInlineQ/src/main.d \
./DOC/dev_sobol/7_CUDALibraries/MC_EstimatePiInlineQ/src/test.d 


# Each subdirectory must supply rules for building sources it contributes
DOC/dev_sobol/7_CUDALibraries/MC_EstimatePiInlineQ/src/%.o: ../DOC/dev_sobol/7_CUDALibraries/MC_EstimatePiInlineQ/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	nvcc -I/usr/local/cula/include -G -g -O0 -Xcompiler -fPIC -gencode arch=compute_20,code=sm_20 -gencode arch=compute_20,code=sm_21 -odir "DOC/dev_sobol/7_CUDALibraries/MC_EstimatePiInlineQ/src" -M -o "$(@:%.o=%.d)" "$<"
	nvcc -I/usr/local/cula/include -G -g -O0 -Xcompiler -fPIC --compile  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

DOC/dev_sobol/7_CUDALibraries/MC_EstimatePiInlineQ/src/%.o: ../DOC/dev_sobol/7_CUDALibraries/MC_EstimatePiInlineQ/src/%.cu
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	nvcc -I/usr/local/cula/include -G -g -O0 -Xcompiler -fPIC -gencode arch=compute_20,code=sm_20 -gencode arch=compute_20,code=sm_21 -odir "DOC/dev_sobol/7_CUDALibraries/MC_EstimatePiInlineQ/src" -M -o "$(@:%.o=%.d)" "$<"
	nvcc --compile -G -I/usr/local/cula/include -O0 -Xcompiler -fPIC -g -gencode arch=compute_20,code=compute_20 -gencode arch=compute_20,code=sm_21  -x cu -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


