################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include SRC/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
Lab3Test.lss \

FLASH_IMAGE += \
Lab3Test.hex \

EEPROM_IMAGE += \
Lab3Test.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: Lab3Test.elf secondary-outputs

# Tool invocations
Lab3Test.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,Lab3Test.map -Wl,-u,vfprintf -lprintf_flt -lm -L"R:\RBE3001workspace\RBELib\Release" -mmcu=atmega644p -o "Lab3Test.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Lab3Test.lss: Lab3Test.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S Lab3Test.elf  >"Lab3Test.lss"
	@echo 'Finished building: $@'
	@echo ' '

Lab3Test.hex: Lab3Test.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex Lab3Test.elf  "Lab3Test.hex"
	@echo 'Finished building: $@'
	@echo ' '

Lab3Test.eep: Lab3Test.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex Lab3Test.elf  "Lab3Test.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: Lab3Test.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega644p Lab3Test.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) Lab3Test.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
