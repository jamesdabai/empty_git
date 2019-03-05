################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/security/app/admin_manage.c \
../src/security/app/advance.c \
../src/security/app/cmd_proc.c \
../src/security/app/ex_func.c \
../src/security/app/file_auth.c \
../src/security/app/file_opt.c \
../src/security/app/pci_func.c \
../src/security/app/possn.c \
../src/security/app/protocol.c \
../src/security/app/rootkey.c \
../src/security/app/sd_tamper.c \
../src/security/app/sec_test.c 

OBJS += \
./src/security/app/admin_manage.o \
./src/security/app/advance.o \
./src/security/app/cmd_proc.o \
./src/security/app/ex_func.o \
./src/security/app/file_auth.o \
./src/security/app/file_opt.o \
./src/security/app/pci_func.o \
./src/security/app/possn.o \
./src/security/app/protocol.o \
./src/security/app/rootkey.o \
./src/security/app/sd_tamper.o \
./src/security/app/sec_test.o 

C_DEPS += \
./src/security/app/admin_manage.d \
./src/security/app/advance.d \
./src/security/app/cmd_proc.d \
./src/security/app/ex_func.d \
./src/security/app/file_auth.d \
./src/security/app/file_opt.d \
./src/security/app/pci_func.d \
./src/security/app/possn.d \
./src/security/app/protocol.d \
./src/security/app/rootkey.d \
./src/security/app/sd_tamper.d \
./src/security/app/sec_test.d 


# Each subdirectory must supply rules for building sources it contributes
src/security/app/%.o: ../src/security/app/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	mcore-elf-gcc -I"D:\work\k205_gx\K205_360S\CORE\include" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdrv\timer" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdrv\rtc" -I"D:\work\k205_gx\K205_360S\CORE\src\moddrv\lcd" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdev\spi" -I"D:\work\k205_gx\K205_360S\CORE\src\fun\time" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdev\wdt" -I"D:\work\k205_gx\K205_360S\CORE\src\fun" -I"D:\work\k205_gx\K205_360S\CORE\src\sysbus\i2c" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdev\rtc" -I"D:\work\k205_gx\K205_360S\CORE\src\sysbus\spi" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdrv\SysTick" -I"D:\work\k205_gx\K205_360S\CORE\src\api" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdev\adc" -I"D:\work\k205_gx\K205_360S\CORE\src\drv\inc" -I"D:\work\k205_gx\K205_360S\CORE\src\moddrv\magcard" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdev\capture" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdev\time" -I"D:\work\k205_gx\K205_360S\CORE\src\fs" -I"D:\work\k205_gx\K205_360S\CORE\src\fs\xgdfs" -I"D:\work\k205_gx\K205_360S\CORE\src\moddrv\beep" -I"D:\work\k205_gx\K205_360S\CORE\src\moddev\rfcard_mh1608" -I"D:\work\k205_gx\K205_360S\CORE\src\security\inc" -I"D:\work\k205_gx\K205_360S\CORE\src\security\algorithm\test" -I"D:\work\k205_gx\K205_360S\CORE\src\security\algorithm" -I"D:\work\k205_gx\K205_360S\CORE\src\security\hal" -I"D:\work\k205_gx\K205_360S\CORE\src\security\secmgr" -I"D:\work\k205_gx\K205_360S\CORE\src\security\security_common" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdrv\flash" -I"D:\work\k205_gx\K205_360S\CORE\src\moddrv\keyboard" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdev\uart" -I"D:\work\k205_gx\K205_360S\CORE\src\ccore" -I"D:\work\k205_gx\K205_360S\CORE\src\common" -I"D:\work\k205_gx\K205_360S\CORE\src\moddev\lcd" -I"D:\work\k205_gx\K205_360S\CORE\src\moddev\beep" -I"D:\work\k205_gx\K205_360S\CORE\src\moddev\power" -I"D:\work\k205_gx\K205_360S\CORE\src\moddev\bt_update" -I"D:\work\k205_gx\K205_360S\CORE\src\moddev\download" -I"D:\work\k205_gx\K205_360S\CORE\src\moddev\magcard" -I"D:\work\k205_gx\K205_360S\CORE\src\moddrv\power" -I"D:\work\k205_gx\K205_360S\CORE\src\moddev\keyboard" -I"D:\work\k205_gx\K205_360S\CORE\src\main" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdrv\ADC" -I"D:\work\k205_gx\K205_360S\CORE\src\moddev\bluetooth" -I"D:\work\k205_gx\K205_360S\CORE\src\sysbus\usb" -I"D:\work\k205_gx\K205_360S\CORE\src\sysbus\gpio" -I"D:\work\k205_gx\K205_360S\CORE\src\sysbus\uart" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdev\usb" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdev\gpio" -I"D:\work\k205_gx\K205_360S\CORE\src\sysbus\capture" -I"D:\work\k205_gx\K205_360S\CORE\src\sysdev\flash" -I"D:\software_xqy\CCore_IDE/tool-chain/ccore-elf/lib/gcc/mcore-elf/4.6.0/include" -I"D:\software_xqy\CCore_IDE/tool-chain/ccore-elf/mcore-elf/include" -I"D:\work\k205_gx\K205_360S\CORE\src\moddev\iccard" -I"D:\work\k205_gx\K205_360S\CORE\src\moddev\led" -I"D:\work\k205_gx\K205_360S\CORE\src\security\key" -I"D:\work\k205_gx\K205_360S\CORE\src\security\api" -I"D:\work\k205_gx\K205_360S\CORE\src\security\sec_datastore" -I"D:\work\k205_gx\K205_360S\CORE\src\security\pinpad" -I"D:\work\k205_gx\K205_360S\CORE\src\security\app" -Os -fno-common -ffunction-sections -g -mlittle-endian -Wall -c  -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


