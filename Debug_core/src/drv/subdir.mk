################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/drv/adc_dma.c \
../src/drv/adc_drv.c \
../src/drv/alg_drv.c \
../src/drv/cache_drv.c \
../src/drv/ccm_drv.c \
../src/drv/cpm_drv.c \
../src/drv/crc_drv.c \
../src/drv/dmac_drv.c \
../src/drv/edma_drv.c \
../src/drv/eflash_drv.c \
../src/drv/eport_drv.c \
../src/drv/ept_drv.c \
../src/drv/i2c_drv.c \
../src/drv/pit32_drv.c \
../src/drv/pit_drv.c \
../src/drv/pwm_drv.c \
../src/drv/reset_drv.c \
../src/drv/rtc_drv.c \
../src/drv/spi_drv.c \
../src/drv/tc_drv.c \
../src/drv/uart_drv.c \
../src/drv/usb_drv.c \
../src/drv/usi_drv.c \
../src/drv/wdt_drv.c 

OBJS += \
./src/drv/adc_dma.o \
./src/drv/adc_drv.o \
./src/drv/alg_drv.o \
./src/drv/cache_drv.o \
./src/drv/ccm_drv.o \
./src/drv/cpm_drv.o \
./src/drv/crc_drv.o \
./src/drv/dmac_drv.o \
./src/drv/edma_drv.o \
./src/drv/eflash_drv.o \
./src/drv/eport_drv.o \
./src/drv/ept_drv.o \
./src/drv/i2c_drv.o \
./src/drv/pit32_drv.o \
./src/drv/pit_drv.o \
./src/drv/pwm_drv.o \
./src/drv/reset_drv.o \
./src/drv/rtc_drv.o \
./src/drv/spi_drv.o \
./src/drv/tc_drv.o \
./src/drv/uart_drv.o \
./src/drv/usb_drv.o \
./src/drv/usi_drv.o \
./src/drv/wdt_drv.o 

C_DEPS += \
./src/drv/adc_dma.d \
./src/drv/adc_drv.d \
./src/drv/alg_drv.d \
./src/drv/cache_drv.d \
./src/drv/ccm_drv.d \
./src/drv/cpm_drv.d \
./src/drv/crc_drv.d \
./src/drv/dmac_drv.d \
./src/drv/edma_drv.d \
./src/drv/eflash_drv.d \
./src/drv/eport_drv.d \
./src/drv/ept_drv.d \
./src/drv/i2c_drv.d \
./src/drv/pit32_drv.d \
./src/drv/pit_drv.d \
./src/drv/pwm_drv.d \
./src/drv/reset_drv.d \
./src/drv/rtc_drv.d \
./src/drv/spi_drv.d \
./src/drv/tc_drv.d \
./src/drv/uart_drv.d \
./src/drv/usb_drv.d \
./src/drv/usi_drv.d \
./src/drv/wdt_drv.d 


# Each subdirectory must supply rules for building sources it contributes
src/drv/%.o: ../src/drv/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	mcore-elf-gcc -I"D:\work\k205_gx\K205_360S\CORE\include" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdrv\timer" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdrv\rtc" -I"D:\work\k205_gx\K205_360S\CORE\src\moddrv\lcd" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdev\spi" -I"D:\work\k205_gx\K205_360S\CORE\src\fun\time" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdev\wdt" -I"D:\work\k205_gx\K205_360S\CORE\src\fun" -I"D:\work\k205_gx\K205_360S\CORE\src\sysbus\i2c" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdev\rtc" -I"D:\work\k205_gx\K205_360S\CORE\src\sysbus\spi" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdrv\SysTick" -I"D:\work\k205_gx\K205_360S\CORE\src\api" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdev\adc" -I"D:\work\k205_gx\K205_360S\CORE\src\drv\inc" -I"D:\work\k205_gx\K205_360S\CORE\src\moddrv\magcard" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdev\capture" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdev\time" -I"D:\work\k205_gx\K205_360S\CORE\src\fs" -I"D:\work\k205_gx\K205_360S\CORE\src\fs\xgdfs" -I"D:\work\k205_gx\K205_360S\CORE\src\moddrv\beep" -I"D:\work\k205_gx\K205_360S\CORE\src\moddev\rfcard_mh1608" -I"D:\work\k205_gx\K205_360S\CORE\src\security\inc" -I"D:\work\k205_gx\K205_360S\CORE\src\security\algorithm\test" -I"D:\work\k205_gx\K205_360S\CORE\src\security\algorithm" -I"D:\work\k205_gx\K205_360S\CORE\src\security\hal" -I"D:\work\k205_gx\K205_360S\CORE\src\security\secmgr" -I"D:\work\k205_gx\K205_360S\CORE\src\security\security_common" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdrv\flash" -I"D:\work\k205_gx\K205_360S\CORE\src\moddrv\keyboard" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdev\uart" -I"D:\work\k205_gx\K205_360S\CORE\src\ccore" -I"D:\work\k205_gx\K205_360S\CORE\src\common" -I"D:\work\k205_gx\K205_360S\CORE\src\moddev\lcd" -I"D:\work\k205_gx\K205_360S\CORE\src\moddev\beep" -I"D:\work\k205_gx\K205_360S\CORE\src\moddev\power" -I"D:\work\k205_gx\K205_360S\CORE\src\moddev\bt_update" -I"D:\work\k205_gx\K205_360S\CORE\src\moddev\download" -I"D:\work\k205_gx\K205_360S\CORE\src\moddev\magcard" -I"D:\work\k205_gx\K205_360S\CORE\src\moddrv\power" -I"D:\work\k205_gx\K205_360S\CORE\src\moddev\keyboard" -I"D:\work\k205_gx\K205_360S\CORE\src\main" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdrv\ADC" -I"D:\work\k205_gx\K205_360S\CORE\src\moddev\bluetooth" -I"D:\work\k205_gx\K205_360S\CORE\src\sysbus\usb" -I"D:\work\k205_gx\K205_360S\CORE\src\sysbus\gpio" -I"D:\work\k205_gx\K205_360S\CORE\src\sysbus\uart" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdev\usb" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdev\gpio" -I"D:\work\k205_gx\K205_360S\CORE\src\sysbus\capture" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdev\flash" -I"D:\software_xqy\CCore_IDE/tool-chain/ccore-elf/lib/gcc/mcore-elf/4.6.0/include" -I"D:\software_xqy\CCore_IDE/tool-chain/ccore-elf/mcore-elf/include" -I"D:\work\k205_gx\K205_360S\CORE\src\moddev\iccard" -I"D:\work\k205_gx\K205_360S\CORE\src\moddev\led" -I"D:\work\k205_gx\K205_360S\CORE\src\security\key" -I"D:\work\k205_gx\K205_360S\CORE\src\security\api" -I"D:\work\k205_gx\K205_360S\CORE\src\security\sec_datastore" -I"D:\work\k205_gx\K205_360S\CORE\src\security\pinpad" -I"D:\work\k205_gx\K205_360S\CORE\src\security\app" -Os -fno-common -ffunction-sections -g -mlittle-endian -Wall -c  -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


