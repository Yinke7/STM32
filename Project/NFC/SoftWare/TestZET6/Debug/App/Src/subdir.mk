################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Src/analogConfigTbl_NFC06A1.c \
../App/Src/demo_ce.c \
../App/Src/demopolling.c \
../App/Src/logger.c 

OBJS += \
./App/Src/analogConfigTbl_NFC06A1.o \
./App/Src/demo_ce.o \
./App/Src/demopolling.o \
./App/Src/logger.o 

C_DEPS += \
./App/Src/analogConfigTbl_NFC06A1.d \
./App/Src/demo_ce.d \
./App/Src/demopolling.d \
./App/Src/logger.d 


# Each subdirectory must supply rules for building sources it contributes
App/Src/analogConfigTbl_NFC06A1.o: ../App/Src/analogConfigTbl_NFC06A1.c App/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"App/Src/analogConfigTbl_NFC06A1.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
App/Src/demo_ce.o: ../App/Src/demo_ce.c App/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -DST25R3916 -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"App/Src/demo_ce.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
App/Src/demopolling.o: ../App/Src/demopolling.c App/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -DST25R3916 -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"App/Src/demopolling.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
App/Src/logger.o: ../App/Src/logger.c App/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"App/Src/logger.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

