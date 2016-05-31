################################################################################
# Automatically-generated file. Do not edit!
################################################################################

#Progress monitor hint: 49
first : all
-include ../makefile.init

# This file contains definitions of environment variables used in the makefiles and .args files if exist.
-include makefile.local

RM := "$(GNU_Make_Install_DirEnv)/rm" -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include Sources/subdir.mk
-include Project_Settings/Startup_Code/subdir.mk
-include Generated_Code/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(SX_DEPS)),)
-include $(SX_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
FSL_LineFollower.elf \

EXECUTABLES_QUOTED += \
"FSL_LineFollower.elf" \

EXECUTABLES_OS_FORMAT += \
FSL_LineFollower.elf \

SECONDARY_FLASH += \
FSL_LineFollower.hex \

SECONDARY_FLASH_QUOTED += \
"FSL_LineFollower.hex" \

SECONDARY_FLASH_OS_FORMAT += \
FSL_LineFollower.hex \

SECONDARY_LIST += \
FSL_LineFollower.lst \

SECONDARY_LIST_QUOTED += \
"FSL_LineFollower.lst" \

SECONDARY_LIST_OS_FORMAT += \
FSL_LineFollower.lst \

SECONDARY_SIZE += \
FSL_LineFollower.siz \

SECONDARY_SIZE_QUOTED += \
"FSL_LineFollower.siz" \

SECONDARY_SIZE_OS_FORMAT += \
FSL_LineFollower.siz \


# All Target
all: warning-messages explicit-dependencies FSL_LineFollower.elf secondary-outputs

# Tool invocations
FSL_LineFollower.elf: $(OBJS) $(USER_OBJS) ../Project_Settings/Linker_Files/ProcessorExpert.ld
	@echo 'Building target: $@'
	@echo 'Executing target #50 $@'
	@echo 'Invoking: ARM Ltd Windows GCC C Linker'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc"  $(USER_OBJS_QUOTED) $(LIBS_QUOTED) @"FSL_LineFollower.args" -o"FSL_LineFollower.elf"
	@echo 'Finished building target: $@'
	@echo ' '

FSL_LineFollower.hex: FSL_LineFollower.elf
	@echo 'Executing target #51 $@'
	@echo 'Invoking: ARM Ltd Windows GNU Create Flash Image'
	"$(ARMSourceryDirEnv)/arm-none-eabi-objcopy"  -O srec FSL_LineFollower.elf "FSL_LineFollower.hex"
	@echo 'Finished building: $@'
	@echo ' '

FSL_LineFollower.lst: FSL_LineFollower.elf
	@echo 'Executing target #52 $@'
	@echo 'Invoking: ARM Ltd Windows GNU Create Listing'
	"$(ARMSourceryDirEnv)/arm-none-eabi-objdump"  -h -S FSL_LineFollower.elf >"FSL_LineFollower.lst"
	@echo 'Finished building: $@'
	@echo ' '

FSL_LineFollower.siz: FSL_LineFollower.elf
	@echo 'Executing target #53 $@'
	@echo 'Invoking: ARM Ltd Windows GNU Print Size'
	"$(ARMSourceryDirEnv)/arm-none-eabi-size"  --format=berkeley FSL_LineFollower.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(S_DEPS_QUOTED) $(SECONDARY_LIST_QUOTED) $(SECONDARY_FLASH_QUOTED) "./*/*.o" "./*/*/*.o"  $(S_UPPER_DEPS_QUOTED) $(ASM_DEPS_QUOTED) $(SECONDARY_SIZE_QUOTED) $(ASM_UPPER_DEPS_QUOTED) $(SX_DEPS_QUOTED) $(EXECUTABLES_QUOTED) "./*/*.d" "./*/*/*.d"  $(ProjDirPath)/FLASH/FSL_LineFollower.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents explicit-dependencies warning-messages
.SECONDARY:

-include ../makefile.targets

explicit-dependencies: ../Project_Settings/Linker_Files/ProcessorExpert.ld FSL_LineFollower.elf FSL_LineFollower.elf FSL_LineFollower.elf 

warning-messages: 

