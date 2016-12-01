################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../csrc/1fft.cpp \
../csrc/1ifft.cpp 

C_SRCS += \
../csrc/bstx_blockconcate.c \
../csrc/bstx_blockseg.c \
../csrc/bstx_crcattach.c \
../csrc/bstx_dcimap.c \
../csrc/bstx_encode.c \
../csrc/bstx_layermap.c \
../csrc/bstx_mod_pdcch.c \
../csrc/bstx_mod_phich.c \
../csrc/bstx_ofdm.c \
../csrc/bstx_payload.c \
../csrc/bstx_phyprc.c \
../csrc/bstx_precode.c \
../csrc/bstx_qammap.c \
../csrc/bstx_ratematch.c \
../csrc/bstx_remap.c \
../csrc/bstx_scramble.c \
../csrc/bstx_sharefunc.c \
../csrc/bstx_top.c \
../csrc/corr.c \
../csrc/fft.c \
../csrc/fifo.c \
../csrc/fir.c \
../csrc/ifft.c \
../csrc/main.c \
../csrc/sharefunc.c \
../csrc/uerx_agc.c \
../csrc/uerx_ce.c \
../csrc/uerx_dci_parse.c \
../csrc/uerx_deblockseg.c \
../csrc/uerx_decode_irx.c \
../csrc/uerx_decrcattach.c \
../csrc/uerx_deframe.c \
../csrc/uerx_deqam.c \
../csrc/uerx_deratematch.c \
../csrc/uerx_deremap.c \
../csrc/uerx_descram.c \
../csrc/uerx_desymbintelv_pdcch.c \
../csrc/uerx_equ.c \
../csrc/uerx_phychdec_top.c \
../csrc/uerx_physig.c \
../csrc/uerx_sharefunc.c \
../csrc/uerx_synch.c \
../csrc/uerx_top.c \
../csrc/uerx_turbo.c \
../csrc/uerx_turboDebug.c \
../csrc/uerx_turboDec.c \
../csrc/uerx_turbo_fixedPoint.c \
../csrc/uerx_viterbi_dec.c 

OBJS += \
./csrc/1fft.obj \
./csrc/1ifft.obj \
./csrc/bstx_blockconcate.obj \
./csrc/bstx_blockseg.obj \
./csrc/bstx_crcattach.obj \
./csrc/bstx_dcimap.obj \
./csrc/bstx_encode.obj \
./csrc/bstx_layermap.obj \
./csrc/bstx_mod_pdcch.obj \
./csrc/bstx_mod_phich.obj \
./csrc/bstx_ofdm.obj \
./csrc/bstx_payload.obj \
./csrc/bstx_phyprc.obj \
./csrc/bstx_precode.obj \
./csrc/bstx_qammap.obj \
./csrc/bstx_ratematch.obj \
./csrc/bstx_remap.obj \
./csrc/bstx_scramble.obj \
./csrc/bstx_sharefunc.obj \
./csrc/bstx_top.obj \
./csrc/corr.obj \
./csrc/fft.obj \
./csrc/fifo.obj \
./csrc/fir.obj \
./csrc/ifft.obj \
./csrc/main.obj \
./csrc/sharefunc.obj \
./csrc/uerx_agc.obj \
./csrc/uerx_ce.obj \
./csrc/uerx_dci_parse.obj \
./csrc/uerx_deblockseg.obj \
./csrc/uerx_decode_irx.obj \
./csrc/uerx_decrcattach.obj \
./csrc/uerx_deframe.obj \
./csrc/uerx_deqam.obj \
./csrc/uerx_deratematch.obj \
./csrc/uerx_deremap.obj \
./csrc/uerx_descram.obj \
./csrc/uerx_desymbintelv_pdcch.obj \
./csrc/uerx_equ.obj \
./csrc/uerx_phychdec_top.obj \
./csrc/uerx_physig.obj \
./csrc/uerx_sharefunc.obj \
./csrc/uerx_synch.obj \
./csrc/uerx_top.obj \
./csrc/uerx_turbo.obj \
./csrc/uerx_turboDebug.obj \
./csrc/uerx_turboDec.obj \
./csrc/uerx_turbo_fixedPoint.obj \
./csrc/uerx_viterbi_dec.obj 

CPP_DEPS += \
./csrc/1fft.pp \
./csrc/1ifft.pp 

C_DEPS += \
./csrc/bstx_blockconcate.pp \
./csrc/bstx_blockseg.pp \
./csrc/bstx_crcattach.pp \
./csrc/bstx_dcimap.pp \
./csrc/bstx_encode.pp \
./csrc/bstx_layermap.pp \
./csrc/bstx_mod_pdcch.pp \
./csrc/bstx_mod_phich.pp \
./csrc/bstx_ofdm.pp \
./csrc/bstx_payload.pp \
./csrc/bstx_phyprc.pp \
./csrc/bstx_precode.pp \
./csrc/bstx_qammap.pp \
./csrc/bstx_ratematch.pp \
./csrc/bstx_remap.pp \
./csrc/bstx_scramble.pp \
./csrc/bstx_sharefunc.pp \
./csrc/bstx_top.pp \
./csrc/corr.pp \
./csrc/fft.pp \
./csrc/fifo.pp \
./csrc/fir.pp \
./csrc/ifft.pp \
./csrc/main.pp \
./csrc/sharefunc.pp \
./csrc/uerx_agc.pp \
./csrc/uerx_ce.pp \
./csrc/uerx_dci_parse.pp \
./csrc/uerx_deblockseg.pp \
./csrc/uerx_decode_irx.pp \
./csrc/uerx_decrcattach.pp \
./csrc/uerx_deframe.pp \
./csrc/uerx_deqam.pp \
./csrc/uerx_deratematch.pp \
./csrc/uerx_deremap.pp \
./csrc/uerx_descram.pp \
./csrc/uerx_desymbintelv_pdcch.pp \
./csrc/uerx_equ.pp \
./csrc/uerx_phychdec_top.pp \
./csrc/uerx_physig.pp \
./csrc/uerx_sharefunc.pp \
./csrc/uerx_synch.pp \
./csrc/uerx_top.pp \
./csrc/uerx_turbo.pp \
./csrc/uerx_turboDebug.pp \
./csrc/uerx_turboDec.pp \
./csrc/uerx_turbo_fixedPoint.pp \
./csrc/uerx_viterbi_dec.pp 

CPP_DEPS__QUOTED += \
"csrc\1fft.pp" \
"csrc\1ifft.pp" 

C_DEPS__QUOTED += \
"csrc\bstx_blockconcate.pp" \
"csrc\bstx_blockseg.pp" \
"csrc\bstx_crcattach.pp" \
"csrc\bstx_dcimap.pp" \
"csrc\bstx_encode.pp" \
"csrc\bstx_layermap.pp" \
"csrc\bstx_mod_pdcch.pp" \
"csrc\bstx_mod_phich.pp" \
"csrc\bstx_ofdm.pp" \
"csrc\bstx_payload.pp" \
"csrc\bstx_phyprc.pp" \
"csrc\bstx_precode.pp" \
"csrc\bstx_qammap.pp" \
"csrc\bstx_ratematch.pp" \
"csrc\bstx_remap.pp" \
"csrc\bstx_scramble.pp" \
"csrc\bstx_sharefunc.pp" \
"csrc\bstx_top.pp" \
"csrc\corr.pp" \
"csrc\fft.pp" \
"csrc\fifo.pp" \
"csrc\fir.pp" \
"csrc\ifft.pp" \
"csrc\main.pp" \
"csrc\sharefunc.pp" \
"csrc\uerx_agc.pp" \
"csrc\uerx_ce.pp" \
"csrc\uerx_dci_parse.pp" \
"csrc\uerx_deblockseg.pp" \
"csrc\uerx_decode_irx.pp" \
"csrc\uerx_decrcattach.pp" \
"csrc\uerx_deframe.pp" \
"csrc\uerx_deqam.pp" \
"csrc\uerx_deratematch.pp" \
"csrc\uerx_deremap.pp" \
"csrc\uerx_descram.pp" \
"csrc\uerx_desymbintelv_pdcch.pp" \
"csrc\uerx_equ.pp" \
"csrc\uerx_phychdec_top.pp" \
"csrc\uerx_physig.pp" \
"csrc\uerx_sharefunc.pp" \
"csrc\uerx_synch.pp" \
"csrc\uerx_top.pp" \
"csrc\uerx_turbo.pp" \
"csrc\uerx_turboDebug.pp" \
"csrc\uerx_turboDec.pp" \
"csrc\uerx_turbo_fixedPoint.pp" \
"csrc\uerx_viterbi_dec.pp" 

OBJS__QUOTED += \
"csrc\1fft.obj" \
"csrc\1ifft.obj" \
"csrc\bstx_blockconcate.obj" \
"csrc\bstx_blockseg.obj" \
"csrc\bstx_crcattach.obj" \
"csrc\bstx_dcimap.obj" \
"csrc\bstx_encode.obj" \
"csrc\bstx_layermap.obj" \
"csrc\bstx_mod_pdcch.obj" \
"csrc\bstx_mod_phich.obj" \
"csrc\bstx_ofdm.obj" \
"csrc\bstx_payload.obj" \
"csrc\bstx_phyprc.obj" \
"csrc\bstx_precode.obj" \
"csrc\bstx_qammap.obj" \
"csrc\bstx_ratematch.obj" \
"csrc\bstx_remap.obj" \
"csrc\bstx_scramble.obj" \
"csrc\bstx_sharefunc.obj" \
"csrc\bstx_top.obj" \
"csrc\corr.obj" \
"csrc\fft.obj" \
"csrc\fifo.obj" \
"csrc\fir.obj" \
"csrc\ifft.obj" \
"csrc\main.obj" \
"csrc\sharefunc.obj" \
"csrc\uerx_agc.obj" \
"csrc\uerx_ce.obj" \
"csrc\uerx_dci_parse.obj" \
"csrc\uerx_deblockseg.obj" \
"csrc\uerx_decode_irx.obj" \
"csrc\uerx_decrcattach.obj" \
"csrc\uerx_deframe.obj" \
"csrc\uerx_deqam.obj" \
"csrc\uerx_deratematch.obj" \
"csrc\uerx_deremap.obj" \
"csrc\uerx_descram.obj" \
"csrc\uerx_desymbintelv_pdcch.obj" \
"csrc\uerx_equ.obj" \
"csrc\uerx_phychdec_top.obj" \
"csrc\uerx_physig.obj" \
"csrc\uerx_sharefunc.obj" \
"csrc\uerx_synch.obj" \
"csrc\uerx_top.obj" \
"csrc\uerx_turbo.obj" \
"csrc\uerx_turboDebug.obj" \
"csrc\uerx_turboDec.obj" \
"csrc\uerx_turbo_fixedPoint.obj" \
"csrc\uerx_viterbi_dec.obj" 

CPP_SRCS__QUOTED += \
"../csrc/1fft.cpp" \
"../csrc/1ifft.cpp" 

C_SRCS__QUOTED += \
"../csrc/bstx_blockconcate.c" \
"../csrc/bstx_blockseg.c" \
"../csrc/bstx_crcattach.c" \
"../csrc/bstx_dcimap.c" \
"../csrc/bstx_encode.c" \
"../csrc/bstx_layermap.c" \
"../csrc/bstx_mod_pdcch.c" \
"../csrc/bstx_mod_phich.c" \
"../csrc/bstx_ofdm.c" \
"../csrc/bstx_payload.c" \
"../csrc/bstx_phyprc.c" \
"../csrc/bstx_precode.c" \
"../csrc/bstx_qammap.c" \
"../csrc/bstx_ratematch.c" \
"../csrc/bstx_remap.c" \
"../csrc/bstx_scramble.c" \
"../csrc/bstx_sharefunc.c" \
"../csrc/bstx_top.c" \
"../csrc/corr.c" \
"../csrc/fft.c" \
"../csrc/fifo.c" \
"../csrc/fir.c" \
"../csrc/ifft.c" \
"../csrc/main.c" \
"../csrc/sharefunc.c" \
"../csrc/uerx_agc.c" \
"../csrc/uerx_ce.c" \
"../csrc/uerx_dci_parse.c" \
"../csrc/uerx_deblockseg.c" \
"../csrc/uerx_decode_irx.c" \
"../csrc/uerx_decrcattach.c" \
"../csrc/uerx_deframe.c" \
"../csrc/uerx_deqam.c" \
"../csrc/uerx_deratematch.c" \
"../csrc/uerx_deremap.c" \
"../csrc/uerx_descram.c" \
"../csrc/uerx_desymbintelv_pdcch.c" \
"../csrc/uerx_equ.c" \
"../csrc/uerx_phychdec_top.c" \
"../csrc/uerx_physig.c" \
"../csrc/uerx_sharefunc.c" \
"../csrc/uerx_synch.c" \
"../csrc/uerx_top.c" \
"../csrc/uerx_turbo.c" \
"../csrc/uerx_turboDebug.c" \
"../csrc/uerx_turboDec.c" \
"../csrc/uerx_turbo_fixedPoint.c" \
"../csrc/uerx_viterbi_dec.c" 


