################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv5/tools/compiler/c6000_7.4.1

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./csrc/uerx_viterbi_dec.obj" \
"./csrc/uerx_turbo_fixedPoint.obj" \
"./csrc/uerx_turboDec.obj" \
"./csrc/uerx_turboDebug.obj" \
"./csrc/uerx_turbo.obj" \
"./csrc/uerx_top.obj" \
"./csrc/uerx_synch.obj" \
"./csrc/uerx_sharefunc.obj" \
"./csrc/uerx_physig.obj" \
"./csrc/uerx_phychdec_top.obj" \
"./csrc/uerx_equ.obj" \
"./csrc/uerx_desymbintelv_pdcch.obj" \
"./csrc/uerx_descram.obj" \
"./csrc/uerx_deremap.obj" \
"./csrc/uerx_deratematch.obj" \
"./csrc/uerx_deqam.obj" \
"./csrc/uerx_deframe.obj" \
"./csrc/uerx_decrcattach.obj" \
"./csrc/uerx_decode_irx.obj" \
"./csrc/uerx_deblockseg.obj" \
"./csrc/uerx_dci_parse.obj" \
"./csrc/uerx_ce.obj" \
"./csrc/uerx_agc.obj" \
"./csrc/sharefunc.obj" \
"./csrc/main.obj" \
"./csrc/ifft.obj" \
"./csrc/fir.obj" \
"./csrc/fifo.obj" \
"./csrc/fft.obj" \
"./csrc/corr.obj" \
"./csrc/bstx_top.obj" \
"./csrc/bstx_sharefunc.obj" \
"./csrc/bstx_scramble.obj" \
"./csrc/bstx_remap.obj" \
"./csrc/bstx_ratematch.obj" \
"./csrc/bstx_qammap.obj" \
"./csrc/bstx_precode.obj" \
"./csrc/bstx_phyprc.obj" \
"./csrc/bstx_payload.obj" \
"./csrc/bstx_ofdm.obj" \
"./csrc/bstx_mod_phich.obj" \
"./csrc/bstx_mod_pdcch.obj" \
"./csrc/bstx_layermap.obj" \
"./csrc/bstx_encode.obj" \
"./csrc/bstx_dcimap.obj" \
"./csrc/bstx_crcattach.obj" \
"./csrc/bstx_blockseg.obj" \
"./csrc/bstx_blockconcate.obj" \
"./csrc/1ifft.obj" \
"./csrc/1fft.obj" \
-l"libc.a" \
"../lib/dsplib.lib" \
"../lib/dsplib_cn.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include lib/subdir_vars.mk
-include csrc/subdir_vars.mk
-include subdir_rules.mk
-include lib/subdir_rules.mk
-include csrc/subdir_rules.mk
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

# All Target
all: LTE_DL_GJ.out

# Tool invocations
LTE_DL_GJ.out: $(OBJS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C6000 Linker'
	"C:/ti/ccsv5/tools/compiler/c6000_7.4.1/bin/cl6x" -mv6600 --abi=eabi -g --display_error_number --diag_warning=225 --diag_wrap=off -z -m"LTE_DL_GJ.map" -i"C:/ti/ccsv5/tools/compiler/c6000_7.4.1/lib" -i"C:/ti/ccsv5/tools/compiler/c6000_7.4.1/include" --reread_libs --warn_sections --display_error_number --diag_wrap=off --rom_model -o "LTE_DL_GJ.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(GEN_SRCS__QUOTED)$(GEN_MISC_FILES__QUOTED)$(C6000_EXECUTABLE_OUTPUTS__QUOTED)$(GEN_OPTS__QUOTED)$(GEN_CMDS__QUOTED) "LTE_DL_GJ.out"
	-$(RM) "csrc\1fft.pp" "csrc\1ifft.pp" 
	-$(RMDIR) $(GEN_MISC_DIRS__QUOTED)
	-$(RM) "csrc\bstx_blockconcate.pp" "csrc\bstx_blockseg.pp" "csrc\bstx_crcattach.pp" "csrc\bstx_dcimap.pp" "csrc\bstx_encode.pp" "csrc\bstx_layermap.pp" "csrc\bstx_mod_pdcch.pp" "csrc\bstx_mod_phich.pp" "csrc\bstx_ofdm.pp" "csrc\bstx_payload.pp" "csrc\bstx_phyprc.pp" "csrc\bstx_precode.pp" "csrc\bstx_qammap.pp" "csrc\bstx_ratematch.pp" "csrc\bstx_remap.pp" "csrc\bstx_scramble.pp" "csrc\bstx_sharefunc.pp" "csrc\bstx_top.pp" "csrc\corr.pp" "csrc\fft.pp" "csrc\fifo.pp" "csrc\fir.pp" "csrc\ifft.pp" "csrc\main.pp" "csrc\sharefunc.pp" "csrc\uerx_agc.pp" "csrc\uerx_ce.pp" "csrc\uerx_dci_parse.pp" "csrc\uerx_deblockseg.pp" "csrc\uerx_decode_irx.pp" "csrc\uerx_decrcattach.pp" "csrc\uerx_deframe.pp" "csrc\uerx_deqam.pp" "csrc\uerx_deratematch.pp" "csrc\uerx_deremap.pp" "csrc\uerx_descram.pp" "csrc\uerx_desymbintelv_pdcch.pp" "csrc\uerx_equ.pp" "csrc\uerx_phychdec_top.pp" "csrc\uerx_physig.pp" "csrc\uerx_sharefunc.pp" "csrc\uerx_synch.pp" "csrc\uerx_top.pp" "csrc\uerx_turbo.pp" "csrc\uerx_turboDebug.pp" "csrc\uerx_turboDec.pp" 
	-$(RM) "csrc\uerx_turbo_fixedPoint.pp" "csrc\uerx_viterbi_dec.pp" 
	-$(RM) "csrc\1fft.obj" "csrc\1ifft.obj" "csrc\bstx_blockconcate.obj" "csrc\bstx_blockseg.obj" "csrc\bstx_crcattach.obj" "csrc\bstx_dcimap.obj" "csrc\bstx_encode.obj" "csrc\bstx_layermap.obj" "csrc\bstx_mod_pdcch.obj" "csrc\bstx_mod_phich.obj" "csrc\bstx_ofdm.obj" "csrc\bstx_payload.obj" "csrc\bstx_phyprc.obj" "csrc\bstx_precode.obj" "csrc\bstx_qammap.obj" "csrc\bstx_ratematch.obj" "csrc\bstx_remap.obj" "csrc\bstx_scramble.obj" "csrc\bstx_sharefunc.obj" "csrc\bstx_top.obj" "csrc\corr.obj" "csrc\fft.obj" "csrc\fifo.obj" "csrc\fir.obj" "csrc\ifft.obj" "csrc\main.obj" "csrc\sharefunc.obj" "csrc\uerx_agc.obj" "csrc\uerx_ce.obj" "csrc\uerx_dci_parse.obj" "csrc\uerx_deblockseg.obj" "csrc\uerx_decode_irx.obj" "csrc\uerx_decrcattach.obj" "csrc\uerx_deframe.obj" "csrc\uerx_deqam.obj" "csrc\uerx_deratematch.obj" "csrc\uerx_deremap.obj" "csrc\uerx_descram.obj" "csrc\uerx_desymbintelv_pdcch.obj" "csrc\uerx_equ.obj" "csrc\uerx_phychdec_top.obj" "csrc\uerx_physig.obj" "csrc\uerx_sharefunc.obj" "csrc\uerx_synch.obj" 
	-$(RM) "csrc\uerx_top.obj" "csrc\uerx_turbo.obj" "csrc\uerx_turboDebug.obj" "csrc\uerx_turboDec.obj" "csrc\uerx_turbo_fixedPoint.obj" "csrc\uerx_viterbi_dec.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
