#pragma once

#include "bstx_define.h"
#include "bstx_dcimap.h"



	int bstx_pseudo_rand(int cinit, int sequence_len, int *outbuf);



	int sequence_pss(int num_ID_cell, cfloat *outbuf);

	int sequence_sss(int num_ID_cell, int subfrm_idx, cfloat *outbuf);
	
	int sequence_rs(int num_ID_cell, int cp_cfg, int subfrm_idx, cfloat *outbuf);

	int input_pbch(int16* outbuf);

	int input_pdcch(int dci_format, int num_rb_dl, int num_rb_ul, int num_bstx_ant, int uldl_switch_cfg
							  , bstx_ctrl_pdsch *ctrl_pdsch_u, int16 *outbuf);

	int input_pcfich(int num_ofdm_pdcch);

	int input_pdsch(int transblock_size, int16 *outbuf);



