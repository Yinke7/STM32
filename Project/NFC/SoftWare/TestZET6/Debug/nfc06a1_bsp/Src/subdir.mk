################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../nfc06a1_bsp/Src/nfc06a1.c 

OBJS += \
./nfc06a1_bsp/Src/nfc06a1.o 

C_DEPS += \
./nfc06a1_bsp/Src/nfc06a1.d 


# Each subdirectory must supply rules for building sources it contributes
nfc06a1_bsp/Src/nfc06a1.o: ../nfc06a1_bsp/Src/nfc06a1.c nfc06a1_bsp/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"nfc06a1_bsp/Src/nfc06a1.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

