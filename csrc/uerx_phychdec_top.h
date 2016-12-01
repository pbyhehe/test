#pragma once

#include "uerx_deratematch.h"
#include "uerx_deblockseg.h"
#include "uerx_decrcattach.h"
#include "uerx_viterbi_dec.h"

#include "uerx_turboDec.h"


//  uerx_deratematch              uerx_deratematch_u;
//	uerx_deblockseg               uerx_deblockseg_u;
//	uerx_decrcattach              uerx_decrcattach_u;
//	uerx_viterbi_dec              uerx_viterbi_dec_u;


	
	int decode_pdsch(float *inbuf, int inlen, int transblock_size, int catgry_ue, int num_harqprocess
									, int Qm, int rv_idx, int NL, int k_mimo, int NDI, int harqprocess_idx
									, int repet_times, int codeword_idx, int16 *outbuf);

	int decode_pdcch(float *inbuf, int inlen, int transblock_size, int port_ue, int enable_port_select
									, int16 *x_rnti, int16 *outbuf);

	int decode_pbch(float *inbuf, int inlen, int transblock_size, int num_bstx_ant, int16 *outbuf);

