################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../middlewares_ndef/Src/poller/ndef_poller.c \
../middlewares_ndef/Src/poller/ndef_t2t.c \
../middlewares_ndef/Src/poller/ndef_t3t.c \
../middlewares_ndef/Src/poller/ndef_t4t.c \
../middlewares_ndef/Src/poller/ndef_t5t.c 

OBJS += \
./middlewares_ndef/Src/poller/ndef_poller.o \
./middlewares_ndef/Src/poller/ndef_t2t.o \
./middlewares_ndef/Src/poller/ndef_t3t.o \
./middlewares_ndef/Src/poller/ndef_t4t.o \
./middlewares_ndef/Src/poller/ndef_t5t.o 

C_DEPS += \
./middlewares_ndef/Src/poller/ndef_poller.d \
./middlewares_ndef/Src/poller/ndef_t2t.d \
./middlewares_ndef/Src/poller/ndef_t3t.d \
./middlewares_ndef/Src/poller/ndef_t4t.d \
./middlewares_ndef/Src/poller/ndef_t5t.d 


# Each subdirectory must supply rules for building sources it contributes
middlewares_ndef/Src/poller/ndef_poller.o: ../middlewares_ndef/Src/poller/ndef_poller.c middlewares_ndef/Src/poller/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"middlewares_ndef/Src/poller/ndef_poller.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
middlewares_ndef/Src/poller/ndef_t2t.o: ../middlewares_ndef/Src/poller/ndef_t2t.c middlewares_ndef/Src/poller/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"middlewares_ndef/Src/poller/ndef_t2t.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
middlewares_ndef/Src/poller/ndef_t3t.o: ../middlewares_ndef/Src/poller/ndef_t3t.c middlewares_ndef/Src/poller/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"middlewares_ndef/Src/poller/ndef_t3t.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
middlewares_ndef/Src/poller/ndef_t4t.o: ../middlewares_ndef/Src/poller/ndef_t4t.c middlewares_ndef/Src/poller/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"middlewares_ndef/Src/poller/ndef_t4t.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
middlewares_ndef/Src/poller/ndef_t5t.o: ../middlewares_ndef/Src/poller/ndef_t5t.c middlewares_ndef/Src/poller/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"middlewares_ndef/Src/poller/ndef_t5t.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

