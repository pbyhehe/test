#pragma once

#include "foo.h"
#include "fir.h"
#include "uerx_agc.h"
#include "uerx_synch.h"
#include "uerx_deframe.h"
#include "uerx_ce.h"
#include "uerx_deremap.h"
#include "uerx_equ.h"
#include "uerx_deqam.h"
#include "uerx_descram.h"
#include "uerx_dci_parse.h"
#include "uerx_decode_irx.h"
#include "uerx_phychdec_top.h"
#include "uerx_desymbintelv_pdcch.h"
#include "uerx_sharefunc.h"



//#pragma DATA_SECTION(uerx_dciparse_u, "uerx_data")
//    uerx_dci_parse          uerx_dciparse_u;
//#pragma DATA_SECTION(uerx_synch_u, "uerx_data")
//    synch1				    uerx_synch_u;
//#pragma DATA_SECTION(uerx_lowpass_u, "uerx_data")
//    fir1				    uerx_lowpass_u[4];
//#pragma DATA_SECTION(Uerx_inf_u, "uerx_data")
//    uerx_inf                Uerx_inf_u;

	int top_Init(float snr_dB);
	int uerx_top_Clear();
	int uerx_top_InputOutput(int frm_idx, int subfrm_idx, cfloat **inbuf, int inlen);

