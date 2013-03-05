################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mem/datastringMem.c \
../mem/instlineMem.c \
../mem/symbolMem.c 

OBJS += \
./mem/datastringMem.o \
./mem/instlineMem.o \
./mem/symbolMem.o 

C_DEPS += \
./mem/datastringMem.d \
./mem/instlineMem.d \
./mem/symbolMem.d 


# Each subdirectory must supply rules for building sources it contributes
mem/%.o: ../mem/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


