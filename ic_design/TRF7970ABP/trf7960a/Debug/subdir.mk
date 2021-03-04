################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../host.c \
../iso14443a.c \
../iso14443b.c \
../iso15693.c \
../main.c \
../mcu.c \
../msp430f23x0.c \
../parallel.c \
../spi.c \
../trf796x.c \
../type2.c \
../uart.c 

CMD_SRCS += \
../lnk_msp430f2370.cmd 

OBJS += \
./host.obj \
./iso14443a.obj \
./iso14443b.obj \
./iso15693.obj \
./main.obj \
./mcu.obj \
./msp430f23x0.obj \
./parallel.obj \
./spi.obj \
./trf796x.obj \
./type2.obj \
./uart.obj 

C_DEPS += \
./host.pp \
./iso14443a.pp \
./iso14443b.pp \
./iso15693.pp \
./main.pp \
./mcu.pp \
./msp430f23x0.pp \
./parallel.pp \
./spi.pp \
./trf796x.pp \
./type2.pp \
./uart.pp 

OBJS__QTD += \
".\host.obj" \
".\iso14443a.obj" \
".\iso14443b.obj" \
".\iso15693.obj" \
".\main.obj" \
".\mcu.obj" \
".\msp430f23x0.obj" \
".\parallel.obj" \
".\spi.obj" \
".\trf796x.obj" \
".\type2.obj" \
".\uart.obj" 

C_DEPS__QTD += \
".\host.pp" \
".\iso14443a.pp" \
".\iso14443b.pp" \
".\iso15693.pp" \
".\main.pp" \
".\mcu.pp" \
".\msp430f23x0.pp" \
".\parallel.pp" \
".\spi.pp" \
".\trf796x.pp" \
".\type2.pp" \
".\uart.pp" 

C_SRCS_QUOTED += \
"../host.c" \
"../iso14443a.c" \
"../iso14443b.c" \
"../iso15693.c" \
"../main.c" \
"../mcu.c" \
"../msp430f23x0.c" \
"../parallel.c" \
"../spi.c" \
"../trf796x.c" \
"../type2.c" \
"../uart.c" 


# Each subdirectory must supply rules for building sources it contributes
host.obj: ../host.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/CCS_V4/ccsv4/tools/compiler/msp430/bin/cl430" -vmsp -g --define=__MSP430F2370__ --include_path="C:/Program Files/Texas Instruments/CCS_V4/ccsv4/msp430/include" --include_path="C:/Program Files/Texas Instruments/CCS_V4/ccsv4/tools/compiler/msp430/include" --diag_warning=225 --printf_support=minimal --preproc_with_compile --preproc_dependency="host.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

iso14443a.obj: ../iso14443a.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/CCS_V4/ccsv4/tools/compiler/msp430/bin/cl430" -vmsp -g --define=__MSP430F2370__ --include_path="C:/Program Files/Texas Instruments/CCS_V4/ccsv4/msp430/include" --include_path="C:/Program Files/Texas Instruments/CCS_V4/ccsv4/tools/compiler/msp430/include" --diag_warning=225 --printf_support=minimal --preproc_with_compile --preproc_dependency="iso14443a.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

iso14443b.obj: ../iso14443b.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/CCS_V4/ccsv4/tools/compiler/msp430/bin/cl430" -vmsp -g --define=__MSP430F2370__ --include_path="C:/Program Files/Texas Instruments/CCS_V4/ccsv4/msp430/include" --include_path="C:/Program Files/Texas Instruments/CCS_V4/ccsv4/tools/compiler/msp430/include" --diag_warning=225 --printf_support=minimal --preproc_with_compile --preproc_dependency="iso14443b.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

iso15693.obj: ../iso15693.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/CCS_V4/ccsv4/tools/compiler/msp430/bin/cl430" -vmsp -g --define=__MSP430F2370__ --include_path="C:/Program Files/Texas Instruments/CCS_V4/ccsv4/msp430/include" --include_path="C:/Program Files/Texas Instruments/CCS_V4/ccsv4/tools/compiler/msp430/include" --diag_warning=225 --printf_support=minimal --preproc_with_compile --preproc_dependency="iso15693.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

lnk_msp430f2370.out: ../lnk_msp430f2370.cmd $(GEN_CMDS)
	@echo 'Building file: $<'
	@echo 'Invoking: Linker'
	"C:/Program Files/Texas Instruments/CCS_V4/ccsv4/tools/compiler/msp430/bin/cl430" -vmsp -g --define=__MSP430F2370__ --diag_warning=225 --printf_support=minimal -z -m"TRF7960A_06_2011.map" --stack_size=100 --heap_size=100 --use_hw_mpy=16 --warn_sections -i"C:/Program Files/Texas Instruments/CCS_V4/ccsv4/msp430/include" -i"C:/Program Files/Texas Instruments/CCS_V4/ccsv4/tools/compiler/msp430/lib" -i"C:/Program Files/Texas Instruments/CCS_V4/ccsv4/tools/compiler/msp430/include" --reread_libs --rom_model -o "$@" "$<" "../lnk_msp430f2370.cmd"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/CCS_V4/ccsv4/tools/compiler/msp430/bin/cl430" -vmsp -g --define=__MSP430F2370__ --include_path="C:/Program Files/Texas Instruments/CCS_V4/ccsv4/msp430/include" --include_path="C:/Program Files/Texas Instruments/CCS_V4/ccsv4/tools/compiler/msp430/include" --diag_warning=225 --printf_support=minimal --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

mcu.obj: ../mcu.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/CCS_V4/ccsv4/tools/compiler/msp430/bin/cl430" -vmsp -g --define=__MSP430F2370__ --include_path="C:/Program Files/Texas Instruments/CCS_V4/ccsv4/msp430/include" --include_path="C:/Program Files/Texas Instruments/CCS_V4/ccsv4/tools/compiler/msp430/include" --diag_warning=225 --printf_support=minimal --preproc_with_compile --preproc_dependency="mcu.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

msp430f23x0.obj: ../msp430f23x0.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/CCS_V4/ccsv4/tools/compiler/msp430/bin/cl430" -vmsp -g --define=__MSP430F2370__ --include_path="C:/Program Files/Texas Instruments/CCS_V4/ccsv4/msp430/include" --include_path="C:/Program Files/Texas Instruments/CCS_V4/ccsv4/tools/compiler/msp430/include" --diag_warning=225 --printf_support=minimal --preproc_with_compile --preproc_dependency="msp430f23x0.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

parallel.obj: ../parallel.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/CCS_V4/ccsv4/tools/compiler/msp430/bin/cl430" -vmsp -g --define=__MSP430F2370__ --include_path="C:/Program Files/Texas Instruments/CCS_V4/ccsv4/msp430/include" --include_path="C:/Program Files/Texas Instruments/CCS_V4/ccsv4/tools/compiler/msp430/include" --diag_warning=225 --printf_support=minimal --preproc_with_compile --preproc_dependency="parallel.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

spi.obj: ../spi.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/CCS_V4/ccsv4/tools/compiler/msp430/bin/cl430" -vmsp -g --define=__MSP430F2370__ --include_path="C:/Program Files/Texas Instruments/CCS_V4/ccsv4/msp430/include" --include_path="C:/Program Files/Texas Instruments/CCS_V4/ccsv4/tools/compiler/msp430/include" --diag_warning=225 --printf_support=minimal --preproc_with_compile --preproc_dependency="spi.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

trf796x.obj: ../trf796x.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/CCS_V4/ccsv4/tools/compiler/msp430/bin/cl430" -vmsp -g --define=__MSP430F2370__ --include_path="C:/Program Files/Texas Instruments/CCS_V4/ccsv4/msp430/include" --include_path="C:/Program Files/Texas Instruments/CCS_V4/ccsv4/tools/compiler/msp430/include" --diag_warning=225 --printf_support=minimal --preproc_with_compile --preproc_dependency="trf796x.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

type2.obj: ../type2.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/CCS_V4/ccsv4/tools/compiler/msp430/bin/cl430" -vmsp -g --define=__MSP430F2370__ --include_path="C:/Program Files/Texas Instruments/CCS_V4/ccsv4/msp430/include" --include_path="C:/Program Files/Texas Instruments/CCS_V4/ccsv4/tools/compiler/msp430/include" --diag_warning=225 --printf_support=minimal --preproc_with_compile --preproc_dependency="type2.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

uart.obj: ../uart.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/CCS_V4/ccsv4/tools/compiler/msp430/bin/cl430" -vmsp -g --define=__MSP430F2370__ --include_path="C:/Program Files/Texas Instruments/CCS_V4/ccsv4/msp430/include" --include_path="C:/Program Files/Texas Instruments/CCS_V4/ccsv4/tools/compiler/msp430/include" --diag_warning=225 --printf_support=minimal --preproc_with_compile --preproc_dependency="uart.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '


