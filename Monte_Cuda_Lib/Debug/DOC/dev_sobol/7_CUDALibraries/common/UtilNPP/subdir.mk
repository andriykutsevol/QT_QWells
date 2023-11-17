################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../DOC/dev_sobol/7_CUDALibraries/common/UtilNPP/StopWatchLinux.cpp \
../DOC/dev_sobol/7_CUDALibraries/common/UtilNPP/StopWatchWin.cpp 

OBJS += \
./DOC/dev_sobol/7_CUDALibraries/common/UtilNPP/StopWatchLinux.o \
./DOC/dev_sobol/7_CUDALibraries/common/UtilNPP/StopWatchWin.o 

CPP_DEPS += \
./DOC/dev_sobol/7_CUDALibraries/common/UtilNPP/StopWatchLinux.d \
./DOC/dev_sobol/7_CUDALibraries/common/UtilNPP/StopWatchWin.d 


# Each subdirectory must supply rules for building sources it contributes
DOC/dev_sobol/7_CUDALibraries/common/UtilNPP/%.o: ../DOC/dev_sobol/7_CUDALibraries/common/UtilNPP/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	nvcc -I/usr/local/cula/include -G -g -O0 -Xcompiler -fPIC -gencode arch=compute_20,code=sm_20 -gencode arch=compute_20,code=sm_21 -odir "DOC/dev_sobol/7_CUDALibraries/common/UtilNPP" -M -o "$(@:%.o=%.d)" "$<"
	nvcc -I/usr/local/cula/include -G -g -O0 -Xcompiler -fPIC --compile  -x c++ -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


