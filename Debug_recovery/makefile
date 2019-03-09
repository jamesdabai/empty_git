################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/sysdrv/timer/subdir.mk
-include src/sysdrv/rtc/subdir.mk
-include src/sysdrv/flash/subdir.mk
-include src/sysdrv/SysTick/subdir.mk
-include src/sysdrv/ADC/subdir.mk
-include src/sysdev/wdt/subdir.mk
-include src/sysdev/usb/subdir.mk
-include src/sysdev/uart/subdir.mk
-include src/sysdev/time/subdir.mk
-include src/sysdev/spi/subdir.mk
-include src/sysdev/rtc/subdir.mk
-include src/sysdev/gpio/subdir.mk
-include src/sysdev/flash/subdir.mk
-include src/sysdev/capture/subdir.mk
-include src/sysdev/adc/subdir.mk
-include src/sysbus/usb/subdir.mk
-include src/sysbus/uart/subdir.mk
-include src/sysbus/spi/subdir.mk
-include src/sysbus/i2c/subdir.mk
-include src/sysbus/gpio/subdir.mk
-include src/sysbus/capture/subdir.mk
-include src/security/security_common/subdir.mk
-include src/security/secmgr/subdir.mk
-include src/security/key/subdir.mk
-include src/security/digitalsign/subdir.mk
-include src/security/app/subdir.mk
-include src/security/api/subdir.mk
-include src/security/algorithm/test/subdir.mk
-include src/moddrv/power/subdir.mk
-include src/moddrv/magcard/subdir.mk
-include src/moddrv/lcd/subdir.mk
-include src/moddrv/keyboard/subdir.mk
-include src/moddrv/beep/subdir.mk
-include src/moddev/power/subdir.mk
-include src/moddev/magcard/subdir.mk
-include src/moddev/led/subdir.mk
-include src/moddev/lcd/subdir.mk
-include src/moddev/keyboard/subdir.mk
-include src/moddev/bt_update/subdir.mk
-include src/moddev/beep/subdir.mk
-include src/main/subdir.mk
-include src/fun/time/subdir.mk
-include src/fun/subdir.mk
-include src/fs/xgdfs/subdir.mk
-include src/fs/subdir.mk
-include src/drv/subdir.mk
-include src/common/subdir.mk
-include src/ccore/subdir.mk
-include src/api/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: core.elf

# Tool invocations
core.elf: $(OBJS) $(USER_OBJS) ../linkmap
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	mcore-elf-ld -EL --gc-sections -L"D:\software_xqy\CCore_IDE/tool-chain/ccore-elf/mcore-elf/lib/c0_div" -L"D:\work\k205_gx\K205_360S\CORE\src\ccore" -L"D:\work\k205_gx\K205_360S\CORE\src\drv" -L"D:\software_xqy\CCore_IDE/tool-chain/ccore-elf/lib/gcc/mcore-elf/4.6.0/c0_div" -L"/core/src/app" -L"D:\work\k205_gx\K205_360S\CORE\src\security\algorithm" -L"D:\work\k205_gx\K205_360S\CORE\src\security\hal" -L"D:\work\k205_gx\K205_360S\CORE\src\sysdev\capture" -L"D:\work\k205_gx\K205_360S\CORE\src\security\pinpad" -L"D:\work\k205_gx\K205_360S\CORE\src\moddev\rfcard_mh1608" -L"D:\work\k205_gx\K205_360S\CORE\src\moddev\iccard" -L"D:\work\k205_gx\K205_360S\CORE\src\security\sec_datastore" -L"D:\work\k205_gx\K205_360S\CORE\src\moddev\bluetooth" -e __start -N -t -warn-common -Map core.map -T../linkmap -o "core.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(C++_DEPS)$(OBJS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) core.elf *.bin *.s *.obj *.map *.elf *.a
	-@echo ' '

post-build:
	-mcore-elf-objcopy  -O binary core.elf core.bin& ..\Debug\HTOB.bat
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
