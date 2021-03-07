################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../st25r3916_bsp/Src/st25r3916.c \
../st25r3916_bsp/Src/st25r3916_com.c \
../st25r3916_bsp/Src/st25r3916_irq.c \
../st25r3916_bsp/Src/st25r3916_led.c \
../st25r3916_bsp/Src/timer.c 

OBJS += \
./st25r3916_bsp/Src/st25r3916.o \
./st25r3916_bsp/Src/st25r3916_com.o \
./st25r3916_bsp/Src/st25r3916_irq.o \
./st25r3916_bsp/Src/st25r3916_led.o \
./st25r3916_bsp/Src/timer.o 

C_DEPS += \
./st25r3916_bsp/Src/st25r3916.d \
./st25r3916_bsp/Src/st25r3916_com.d \
./st25r3916_bsp/Src/st25r3916_irq.d \
./st25r3916_bsp/Src/st25r3916_led.d \
./st25r3916_bsp/Src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
st25r3916_bsp/Src/st25r3916.o: ../st25r3916_bsp/Src/st25r3916.c st25r3916_bsp/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"st25r3916_bsp/Src/st25r3916.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
st25r3916_bsp/Src/st25r3916_com.o: ../st25r3916_bsp/Src/st25r3916_com.c st25r3916_bsp/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"st25r3916_bsp/Src/st25r3916_com.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
st25r3916_bsp/Src/st25r3916_irq.o: ../st25r3916_bsp/Src/st25r3916_irq.c st25r3916_bsp/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"st25r3916_bsp/Src/st25r3916_irq.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
st25r3916_bsp/Src/st25r3916_led.o: ../st25r3916_bsp/Src/st25r3916_led.c st25r3916_bsp/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"st25r3916_bsp/Src/st25r3916_led.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
st25r3916_bsp/Src/timer.o: ../st25r3916_bsp/Src/timer.c st25r3916_bsp/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"st25r3916_bsp/Src/timer.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

