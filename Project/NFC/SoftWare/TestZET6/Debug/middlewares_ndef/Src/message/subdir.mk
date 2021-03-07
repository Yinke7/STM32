################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../middlewares_ndef/Src/message/ndef_message.c \
../middlewares_ndef/Src/message/ndef_record.c \
../middlewares_ndef/Src/message/ndef_type_wifi.c \
../middlewares_ndef/Src/message/ndef_types.c \
../middlewares_ndef/Src/message/ndef_types_mime.c \
../middlewares_ndef/Src/message/ndef_types_rtd.c 

OBJS += \
./middlewares_ndef/Src/message/ndef_message.o \
./middlewares_ndef/Src/message/ndef_record.o \
./middlewares_ndef/Src/message/ndef_type_wifi.o \
./middlewares_ndef/Src/message/ndef_types.o \
./middlewares_ndef/Src/message/ndef_types_mime.o \
./middlewares_ndef/Src/message/ndef_types_rtd.o 

C_DEPS += \
./middlewares_ndef/Src/message/ndef_message.d \
./middlewares_ndef/Src/message/ndef_record.d \
./middlewares_ndef/Src/message/ndef_type_wifi.d \
./middlewares_ndef/Src/message/ndef_types.d \
./middlewares_ndef/Src/message/ndef_types_mime.d \
./middlewares_ndef/Src/message/ndef_types_rtd.d 


# Each subdirectory must supply rules for building sources it contributes
middlewares_ndef/Src/message/ndef_message.o: ../middlewares_ndef/Src/message/ndef_message.c middlewares_ndef/Src/message/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"middlewares_ndef/Src/message/ndef_message.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
middlewares_ndef/Src/message/ndef_record.o: ../middlewares_ndef/Src/message/ndef_record.c middlewares_ndef/Src/message/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"middlewares_ndef/Src/message/ndef_record.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
middlewares_ndef/Src/message/ndef_type_wifi.o: ../middlewares_ndef/Src/message/ndef_type_wifi.c middlewares_ndef/Src/message/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"middlewares_ndef/Src/message/ndef_type_wifi.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
middlewares_ndef/Src/message/ndef_types.o: ../middlewares_ndef/Src/message/ndef_types.c middlewares_ndef/Src/message/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"middlewares_ndef/Src/message/ndef_types.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
middlewares_ndef/Src/message/ndef_types_mime.o: ../middlewares_ndef/Src/message/ndef_types_mime.c middlewares_ndef/Src/message/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"middlewares_ndef/Src/message/ndef_types_mime.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
middlewares_ndef/Src/message/ndef_types_rtd.o: ../middlewares_ndef/Src/message/ndef_types_rtd.c middlewares_ndef/Src/message/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"middlewares_ndef/Src/message/ndef_types_rtd.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

