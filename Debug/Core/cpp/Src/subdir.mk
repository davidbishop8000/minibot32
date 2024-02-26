################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/cpp/Src/canbus.cpp \
../Core/cpp/Src/drivers_control.cpp \
../Core/cpp/Src/flash_data.cpp \
../Core/cpp/Src/inputs.cpp \
../Core/cpp/Src/mission.cpp \
../Core/cpp/Src/outputs.cpp \
../Core/cpp/Src/servo_control.cpp \
../Core/cpp/Src/uart_wifi.cpp 

OBJS += \
./Core/cpp/Src/canbus.o \
./Core/cpp/Src/drivers_control.o \
./Core/cpp/Src/flash_data.o \
./Core/cpp/Src/inputs.o \
./Core/cpp/Src/mission.o \
./Core/cpp/Src/outputs.o \
./Core/cpp/Src/servo_control.o \
./Core/cpp/Src/uart_wifi.o 

CPP_DEPS += \
./Core/cpp/Src/canbus.d \
./Core/cpp/Src/drivers_control.d \
./Core/cpp/Src/flash_data.d \
./Core/cpp/Src/inputs.d \
./Core/cpp/Src/mission.d \
./Core/cpp/Src/outputs.d \
./Core/cpp/Src/servo_control.d \
./Core/cpp/Src/uart_wifi.d 


# Each subdirectory must supply rules for building sources it contributes
Core/cpp/Src/%.o Core/cpp/Src/%.su: ../Core/cpp/Src/%.cpp Core/cpp/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"C:/ST_proj/minibot32/Core/cpp/Inc" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-cpp-2f-Src

clean-Core-2f-cpp-2f-Src:
	-$(RM) ./Core/cpp/Src/canbus.d ./Core/cpp/Src/canbus.o ./Core/cpp/Src/canbus.su ./Core/cpp/Src/drivers_control.d ./Core/cpp/Src/drivers_control.o ./Core/cpp/Src/drivers_control.su ./Core/cpp/Src/flash_data.d ./Core/cpp/Src/flash_data.o ./Core/cpp/Src/flash_data.su ./Core/cpp/Src/inputs.d ./Core/cpp/Src/inputs.o ./Core/cpp/Src/inputs.su ./Core/cpp/Src/mission.d ./Core/cpp/Src/mission.o ./Core/cpp/Src/mission.su ./Core/cpp/Src/outputs.d ./Core/cpp/Src/outputs.o ./Core/cpp/Src/outputs.su ./Core/cpp/Src/servo_control.d ./Core/cpp/Src/servo_control.o ./Core/cpp/Src/servo_control.su ./Core/cpp/Src/uart_wifi.d ./Core/cpp/Src/uart_wifi.o ./Core/cpp/Src/uart_wifi.su

.PHONY: clean-Core-2f-cpp-2f-Src

