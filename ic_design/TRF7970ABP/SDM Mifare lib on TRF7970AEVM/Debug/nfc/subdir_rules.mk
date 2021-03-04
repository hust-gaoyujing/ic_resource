################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
nfc/iso14443a.obj: ../nfc/iso14443a.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/TI/ccsv6/ccsv6/tools/compiler/msp430_4.3.3/bin/cl430" -vmsp --abi=eabi -O0 --include_path="C:/TI/ccsv6/ccsv6/ccs_base/msp430/include" --include_path="C:/Users/a0273119/CCS_5.2_workspaces/embedded-rf/Radios/TRF7970A_64_A/MSP430F2370/MIFARE_Classic/_SDM Source Projects/SDM Mifare lib on MSP430G2553 TRF7970A/nfc" --include_path="C:/TI/ccsv6/ccsv6/tools/compiler/msp430_4.3.3/include" --include_path="C:/Users/a0273119/CCS_5.2_workspaces/embedded-rf/Radios/TRF7970A_64_A/MSP430F2370/MIFARE_Classic/_SDM Source Projects/SDM Mifare lib on MSP430G2553 TRF7970A" --include_path="C:/Users/a0273119/CCS_5.2_workspaces/embedded-rf/Radios/TRF7970A_64_A/MSP430F2370/MIFARE_Classic/_SDM Source Projects/SDM Mifare lib on MSP430G2553 TRF7970A/Headers" --include_path="C:/Users/a0273119/CCS_5.2_workspaces/embedded-rf/Radios/TRF7970A_64_A/MSP430F2370/MIFARE_Classic/_SDM Source Projects/SDM Mifare lib on MSP430G2553 TRF7970A/MIFARE Classic" -g --define=__MSP430F2370__ --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="nfc/iso14443a.pp" --obj_directory="nfc" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

nfc/mcu.obj: ../nfc/mcu.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/TI/ccsv6/ccsv6/tools/compiler/msp430_4.3.3/bin/cl430" -vmsp --abi=eabi -O0 --include_path="C:/TI/ccsv6/ccsv6/ccs_base/msp430/include" --include_path="C:/Users/a0273119/CCS_5.2_workspaces/embedded-rf/Radios/TRF7970A_64_A/MSP430F2370/MIFARE_Classic/_SDM Source Projects/SDM Mifare lib on MSP430G2553 TRF7970A/nfc" --include_path="C:/TI/ccsv6/ccsv6/tools/compiler/msp430_4.3.3/include" --include_path="C:/Users/a0273119/CCS_5.2_workspaces/embedded-rf/Radios/TRF7970A_64_A/MSP430F2370/MIFARE_Classic/_SDM Source Projects/SDM Mifare lib on MSP430G2553 TRF7970A" --include_path="C:/Users/a0273119/CCS_5.2_workspaces/embedded-rf/Radios/TRF7970A_64_A/MSP430F2370/MIFARE_Classic/_SDM Source Projects/SDM Mifare lib on MSP430G2553 TRF7970A/Headers" --include_path="C:/Users/a0273119/CCS_5.2_workspaces/embedded-rf/Radios/TRF7970A_64_A/MSP430F2370/MIFARE_Classic/_SDM Source Projects/SDM Mifare lib on MSP430G2553 TRF7970A/MIFARE Classic" -g --define=__MSP430F2370__ --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="nfc/mcu.pp" --obj_directory="nfc" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

nfc/nfc.obj: ../nfc/nfc.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/TI/ccsv6/ccsv6/tools/compiler/msp430_4.3.3/bin/cl430" -vmsp --abi=eabi -O0 --include_path="C:/TI/ccsv6/ccsv6/ccs_base/msp430/include" --include_path="C:/Users/a0273119/CCS_5.2_workspaces/embedded-rf/Radios/TRF7970A_64_A/MSP430F2370/MIFARE_Classic/_SDM Source Projects/SDM Mifare lib on MSP430G2553 TRF7970A/nfc" --include_path="C:/TI/ccsv6/ccsv6/tools/compiler/msp430_4.3.3/include" --include_path="C:/Users/a0273119/CCS_5.2_workspaces/embedded-rf/Radios/TRF7970A_64_A/MSP430F2370/MIFARE_Classic/_SDM Source Projects/SDM Mifare lib on MSP430G2553 TRF7970A" --include_path="C:/Users/a0273119/CCS_5.2_workspaces/embedded-rf/Radios/TRF7970A_64_A/MSP430F2370/MIFARE_Classic/_SDM Source Projects/SDM Mifare lib on MSP430G2553 TRF7970A/Headers" --include_path="C:/Users/a0273119/CCS_5.2_workspaces/embedded-rf/Radios/TRF7970A_64_A/MSP430F2370/MIFARE_Classic/_SDM Source Projects/SDM Mifare lib on MSP430G2553 TRF7970A/MIFARE Classic" -g --define=__MSP430F2370__ --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="nfc/nfc.pp" --obj_directory="nfc" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

nfc/spi.obj: ../nfc/spi.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/TI/ccsv6/ccsv6/tools/compiler/msp430_4.3.3/bin/cl430" -vmsp --abi=eabi -O0 --include_path="C:/TI/ccsv6/ccsv6/ccs_base/msp430/include" --include_path="C:/Users/a0273119/CCS_5.2_workspaces/embedded-rf/Radios/TRF7970A_64_A/MSP430F2370/MIFARE_Classic/_SDM Source Projects/SDM Mifare lib on MSP430G2553 TRF7970A/nfc" --include_path="C:/TI/ccsv6/ccsv6/tools/compiler/msp430_4.3.3/include" --include_path="C:/Users/a0273119/CCS_5.2_workspaces/embedded-rf/Radios/TRF7970A_64_A/MSP430F2370/MIFARE_Classic/_SDM Source Projects/SDM Mifare lib on MSP430G2553 TRF7970A" --include_path="C:/Users/a0273119/CCS_5.2_workspaces/embedded-rf/Radios/TRF7970A_64_A/MSP430F2370/MIFARE_Classic/_SDM Source Projects/SDM Mifare lib on MSP430G2553 TRF7970A/Headers" --include_path="C:/Users/a0273119/CCS_5.2_workspaces/embedded-rf/Radios/TRF7970A_64_A/MSP430F2370/MIFARE_Classic/_SDM Source Projects/SDM Mifare lib on MSP430G2553 TRF7970A/MIFARE Classic" -g --define=__MSP430F2370__ --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="nfc/spi.pp" --obj_directory="nfc" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

nfc/trf797x.obj: ../nfc/trf797x.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/TI/ccsv6/ccsv6/tools/compiler/msp430_4.3.3/bin/cl430" -vmsp --abi=eabi -O0 --include_path="C:/TI/ccsv6/ccsv6/ccs_base/msp430/include" --include_path="C:/Users/a0273119/CCS_5.2_workspaces/embedded-rf/Radios/TRF7970A_64_A/MSP430F2370/MIFARE_Classic/_SDM Source Projects/SDM Mifare lib on MSP430G2553 TRF7970A/nfc" --include_path="C:/TI/ccsv6/ccsv6/tools/compiler/msp430_4.3.3/include" --include_path="C:/Users/a0273119/CCS_5.2_workspaces/embedded-rf/Radios/TRF7970A_64_A/MSP430F2370/MIFARE_Classic/_SDM Source Projects/SDM Mifare lib on MSP430G2553 TRF7970A" --include_path="C:/Users/a0273119/CCS_5.2_workspaces/embedded-rf/Radios/TRF7970A_64_A/MSP430F2370/MIFARE_Classic/_SDM Source Projects/SDM Mifare lib on MSP430G2553 TRF7970A/Headers" --include_path="C:/Users/a0273119/CCS_5.2_workspaces/embedded-rf/Radios/TRF7970A_64_A/MSP430F2370/MIFARE_Classic/_SDM Source Projects/SDM Mifare lib on MSP430G2553 TRF7970A/MIFARE Classic" -g --define=__MSP430F2370__ --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="nfc/trf797x.pp" --obj_directory="nfc" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

nfc/uart.obj: ../nfc/uart.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/TI/ccsv6/ccsv6/tools/compiler/msp430_4.3.3/bin/cl430" -vmsp --abi=eabi -O0 --include_path="C:/TI/ccsv6/ccsv6/ccs_base/msp430/include" --include_path="C:/Users/a0273119/CCS_5.2_workspaces/embedded-rf/Radios/TRF7970A_64_A/MSP430F2370/MIFARE_Classic/_SDM Source Projects/SDM Mifare lib on MSP430G2553 TRF7970A/nfc" --include_path="C:/TI/ccsv6/ccsv6/tools/compiler/msp430_4.3.3/include" --include_path="C:/Users/a0273119/CCS_5.2_workspaces/embedded-rf/Radios/TRF7970A_64_A/MSP430F2370/MIFARE_Classic/_SDM Source Projects/SDM Mifare lib on MSP430G2553 TRF7970A" --include_path="C:/Users/a0273119/CCS_5.2_workspaces/embedded-rf/Radios/TRF7970A_64_A/MSP430F2370/MIFARE_Classic/_SDM Source Projects/SDM Mifare lib on MSP430G2553 TRF7970A/Headers" --include_path="C:/Users/a0273119/CCS_5.2_workspaces/embedded-rf/Radios/TRF7970A_64_A/MSP430F2370/MIFARE_Classic/_SDM Source Projects/SDM Mifare lib on MSP430G2553 TRF7970A/MIFARE Classic" -g --define=__MSP430F2370__ --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="nfc/uart.pp" --obj_directory="nfc" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


