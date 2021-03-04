################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/TI/ccsv6/ccsv6/tools/compiler/msp430_4.3.3

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./main.obj" \
"./nfc/iso14443a.obj" \
"./nfc/mcu.obj" \
"./nfc/nfc.obj" \
"./nfc/spi.obj" \
"./nfc/trf797x.obj" \
"./nfc/uart.obj" \
"./MIFARE Classic/RFid_functions.obj" \
"./MIFARE Classic/crypto1_mifare.obj" \
"./MIFARE Classic/directmode.obj" \
"../targetConfigs/lnk_msp430f2370.cmd" \
"../MIFARE Classic/MifareCryptoLibrary.lib" \
-l"libc.a" \
-l"C:\Users\a0273119\CCS_5.2_workspaces\embedded-rf\Radios\TRF7970A_64_A\MSP430F2370\MIFARE_Classic\_SDM Source Projects\SDM Mifare lib on MSP430G2553 TRF7970A\MIFARE Classic\MifareCryptoLibrary.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include targetConfigs/subdir_vars.mk
-include nfc/subdir_vars.mk
-include MIFARE\ Classic/subdir_vars.mk
-include subdir_rules.mk
-include targetConfigs/subdir_rules.mk
-include nfc/subdir_rules.mk
-include MIFARE\ Classic/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
MSP430G2553_TRF7970A_SDM_Mifare_lib.out \

EXE_OUTPUTS__QUOTED += \
"MSP430G2553_TRF7970A_SDM_Mifare_lib.out" \

BIN_OUTPUTS += \
MSP430G2553_TRF7970A_SDM_Mifare_lib.hex \

BIN_OUTPUTS__QUOTED += \
"MSP430G2553_TRF7970A_SDM_Mifare_lib.hex" \


# All Target
all: MSP430G2553_TRF7970A_SDM_Mifare_lib.out

# Tool invocations
MSP430G2553_TRF7970A_SDM_Mifare_lib.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: MSP430 Linker'
	"C:/TI/ccsv6/ccsv6/tools/compiler/msp430_4.3.3/bin/cl430" -vmsp --abi=eabi -O0 -g --define=__MSP430F2370__ --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal -z -m"MSP430G2553_TRF7970A_SDM_Mifare_lib.map" --heap_size=0 --stack_size=180 --use_hw_mpy=16 -i"C:/TI/ccsv6/ccsv6/ccs_base/msp430/include" -i"C:/TI/ccsv6/ccsv6/tools/compiler/msp430_4.3.3/lib" -i"C:/TI/ccsv6/ccsv6/tools/compiler/msp430_4.3.3/include" --reread_libs --warn_sections --xml_link_info="MSP430G2553_TRF7970A_SDM_Mifare_lib_LinkInfo.xml" --rom_model -o "MSP430G2553_TRF7970A_SDM_Mifare_lib.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

MSP430G2553_TRF7970A_SDM_Mifare_lib.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: MSP430 Hex Utility'
	"C:/TI/ccsv6/ccsv6/tools/compiler/msp430_4.3.3/bin/hex430" --memwidth=8 --romwidth=8 -o "MSP430G2553_TRF7970A_SDM_Mifare_lib.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "main.pp" "nfc\iso14443a.pp" "nfc\mcu.pp" "nfc\nfc.pp" "nfc\spi.pp" "nfc\trf797x.pp" "nfc\uart.pp" "MIFARE Classic\RFid_functions.pp" "MIFARE Classic\crypto1_mifare.pp" "MIFARE Classic\directmode.pp" 
	-$(RM) "main.obj" "nfc\iso14443a.obj" "nfc\mcu.obj" "nfc\nfc.obj" "nfc\spi.obj" "nfc\trf797x.obj" "nfc\uart.obj" "MIFARE Classic\RFid_functions.obj" "MIFARE Classic\crypto1_mifare.obj" "MIFARE Classic\directmode.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

