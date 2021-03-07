################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../middlewares_rfal/Src/rfal_analogConfig.c \
../middlewares_rfal/Src/rfal_crc.c \
../middlewares_rfal/Src/rfal_dpo.c \
../middlewares_rfal/Src/rfal_iso15693_2.c \
../middlewares_rfal/Src/rfal_isoDep.c \
../middlewares_rfal/Src/rfal_nfc.c \
../middlewares_rfal/Src/rfal_nfcDep.c \
../middlewares_rfal/Src/rfal_nfca.c \
../middlewares_rfal/Src/rfal_nfcb.c \
../middlewares_rfal/Src/rfal_nfcf.c \
../middlewares_rfal/Src/rfal_nfcv.c \
../middlewares_rfal/Src/rfal_rfst25r3916.c \
../middlewares_rfal/Src/rfal_st25tb.c \
../middlewares_rfal/Src/rfal_st25xv.c \
../middlewares_rfal/Src/rfal_t1t.c \
../middlewares_rfal/Src/rfal_t2t.c \
../middlewares_rfal/Src/rfal_t4t.c 

OBJS += \
./middlewares_rfal/Src/rfal_analogConfig.o \
./middlewares_rfal/Src/rfal_crc.o \
./middlewares_rfal/Src/rfal_dpo.o \
./middlewares_rfal/Src/rfal_iso15693_2.o \
./middlewares_rfal/Src/rfal_isoDep.o \
./middlewares_rfal/Src/rfal_nfc.o \
./middlewares_rfal/Src/rfal_nfcDep.o \
./middlewares_rfal/Src/rfal_nfca.o \
./middlewares_rfal/Src/rfal_nfcb.o \
./middlewares_rfal/Src/rfal_nfcf.o \
./middlewares_rfal/Src/rfal_nfcv.o \
./middlewares_rfal/Src/rfal_rfst25r3916.o \
./middlewares_rfal/Src/rfal_st25tb.o \
./middlewares_rfal/Src/rfal_st25xv.o \
./middlewares_rfal/Src/rfal_t1t.o \
./middlewares_rfal/Src/rfal_t2t.o \
./middlewares_rfal/Src/rfal_t4t.o 

C_DEPS += \
./middlewares_rfal/Src/rfal_analogConfig.d \
./middlewares_rfal/Src/rfal_crc.d \
./middlewares_rfal/Src/rfal_dpo.d \
./middlewares_rfal/Src/rfal_iso15693_2.d \
./middlewares_rfal/Src/rfal_isoDep.d \
./middlewares_rfal/Src/rfal_nfc.d \
./middlewares_rfal/Src/rfal_nfcDep.d \
./middlewares_rfal/Src/rfal_nfca.d \
./middlewares_rfal/Src/rfal_nfcb.d \
./middlewares_rfal/Src/rfal_nfcf.d \
./middlewares_rfal/Src/rfal_nfcv.d \
./middlewares_rfal/Src/rfal_rfst25r3916.d \
./middlewares_rfal/Src/rfal_st25tb.d \
./middlewares_rfal/Src/rfal_st25xv.d \
./middlewares_rfal/Src/rfal_t1t.d \
./middlewares_rfal/Src/rfal_t2t.d \
./middlewares_rfal/Src/rfal_t4t.d 


# Each subdirectory must supply rules for building sources it contributes
middlewares_rfal/Src/rfal_analogConfig.o: ../middlewares_rfal/Src/rfal_analogConfig.c middlewares_rfal/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"middlewares_rfal/Src/rfal_analogConfig.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
middlewares_rfal/Src/rfal_crc.o: ../middlewares_rfal/Src/rfal_crc.c middlewares_rfal/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"middlewares_rfal/Src/rfal_crc.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
middlewares_rfal/Src/rfal_dpo.o: ../middlewares_rfal/Src/rfal_dpo.c middlewares_rfal/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"middlewares_rfal/Src/rfal_dpo.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
middlewares_rfal/Src/rfal_iso15693_2.o: ../middlewares_rfal/Src/rfal_iso15693_2.c middlewares_rfal/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"middlewares_rfal/Src/rfal_iso15693_2.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
middlewares_rfal/Src/rfal_isoDep.o: ../middlewares_rfal/Src/rfal_isoDep.c middlewares_rfal/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"middlewares_rfal/Src/rfal_isoDep.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
middlewares_rfal/Src/rfal_nfc.o: ../middlewares_rfal/Src/rfal_nfc.c middlewares_rfal/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"middlewares_rfal/Src/rfal_nfc.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
middlewares_rfal/Src/rfal_nfcDep.o: ../middlewares_rfal/Src/rfal_nfcDep.c middlewares_rfal/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"middlewares_rfal/Src/rfal_nfcDep.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
middlewares_rfal/Src/rfal_nfca.o: ../middlewares_rfal/Src/rfal_nfca.c middlewares_rfal/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"middlewares_rfal/Src/rfal_nfca.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
middlewares_rfal/Src/rfal_nfcb.o: ../middlewares_rfal/Src/rfal_nfcb.c middlewares_rfal/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"middlewares_rfal/Src/rfal_nfcb.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
middlewares_rfal/Src/rfal_nfcf.o: ../middlewares_rfal/Src/rfal_nfcf.c middlewares_rfal/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"middlewares_rfal/Src/rfal_nfcf.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
middlewares_rfal/Src/rfal_nfcv.o: ../middlewares_rfal/Src/rfal_nfcv.c middlewares_rfal/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"middlewares_rfal/Src/rfal_nfcv.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
middlewares_rfal/Src/rfal_rfst25r3916.o: ../middlewares_rfal/Src/rfal_rfst25r3916.c middlewares_rfal/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"middlewares_rfal/Src/rfal_rfst25r3916.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
middlewares_rfal/Src/rfal_st25tb.o: ../middlewares_rfal/Src/rfal_st25tb.c middlewares_rfal/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"middlewares_rfal/Src/rfal_st25tb.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
middlewares_rfal/Src/rfal_st25xv.o: ../middlewares_rfal/Src/rfal_st25xv.c middlewares_rfal/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"middlewares_rfal/Src/rfal_st25xv.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
middlewares_rfal/Src/rfal_t1t.o: ../middlewares_rfal/Src/rfal_t1t.c middlewares_rfal/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"middlewares_rfal/Src/rfal_t1t.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
middlewares_rfal/Src/rfal_t2t.o: ../middlewares_rfal/Src/rfal_t2t.c middlewares_rfal/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"middlewares_rfal/Src/rfal_t2t.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
middlewares_rfal/Src/rfal_t4t.o: ../middlewares_rfal/Src/rfal_t4t.c middlewares_rfal/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../middlewares_rfal/Inc -I../nfc06a1_bsp/Inc -I../App/Inc -I../st25r3916_bsp/Inc -I"../middlewares_ndef/Inc/poller" -I"../middlewares_ndef/Inc/message" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"middlewares_rfal/Src/rfal_t4t.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

