################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../globalHelper.c \
../instructionHandler.c \
../instructionParse.c \
../main.c \
../phase1.c \
../phase2.c 

OBJS += \
./globalHelper.o \
./instructionHandler.o \
./instructionParse.o \
./main.o \
./phase1.o \
./phase2.o 

C_DEPS += \
./globalHelper.d \
./instructionHandler.d \
./instructionParse.d \
./main.d \
./phase1.d \
./phase2.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


