################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../file/entFile.c \
../file/extFile.c \
../file/fileHelper.c \
../file/objFile.c 

OBJS += \
./file/entFile.o \
./file/extFile.o \
./file/fileHelper.o \
./file/objFile.o 

C_DEPS += \
./file/entFile.d \
./file/extFile.d \
./file/fileHelper.d \
./file/objFile.d 


# Each subdirectory must supply rules for building sources it contributes
file/%.o: ../file/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


