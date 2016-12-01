#pragma once

#include "foo.h"



	int precode_siso(cfloat *inbuf, int inlen, cfloat *outbuf);

	int precode_diversity(cfloat **inbuf, int inlen, int inlen_layer_cw0, int num_layers, cfloat **outbuf);

	int precode_spatial(cfloat **inbuf, int inlen, int num_layers, int num_bstx_ant, int cdd_cfg, int codebook_idx,  cfloat **outbuf);



	int matrix_mult(cfloat **inbuf0, int num_row0, int num_col0, cfloat **inbuf1, int num_row1, int num_col1, cfloat **outbuf);

	int precode_matrix(cfloat **outbuf, int matrix_type);




	int precode_pcfich(cfloat **inbuf, int inlen, int num_bstx_ant, cfloat **outbuf);

	int precode_phich(cfloat **inbuf, int inlen, int num_bstx_ant, int group_idx, int cp_cfg, cfloat **outbuf);

	int precode_pdcch(cfloat **inbuf, int inlen, int inlen_layer_cw0, int num_bstx_ant, cfloat **outbuf);

	int precode_pbch(cfloat **inbuf, int inlen, int inlen_layer_cw0, int num_bstx_ant, cfloat **outbuf);

	int precode_pdsch(cfloat **inbuf, int inlen, int inlen_layer_cw0, int num_layers, int num_bstx_ant
								 , int transmit_scheme, int cdd_cfg, int codebook_idx, cfloat **outbuf);




enum PRECODE_MATRIX
{
	U_LAY2_ID,
	U_LAY3_ID,	
	U_LAY4_ID,

	W_ANT2_LAY1_CODIDX0_ID,
	W_ANT2_LAY1_CODIDX1_ID,
	W_ANT2_LAY1_CODIDX2_ID,
	W_ANT2_LAY1_CODIDX3_ID,

	W_ANT2_LAY2_CODIDX0_ID,
	W_ANT2_LAY2_CODIDX1_ID,
	W_ANT2_LAY2_CODIDX2_ID,
	
	U0_ID,
	U1_ID,
	U2_ID,
	U3_ID,
	U4_ID,
	U5_ID,
	U6_ID,
	U7_ID,
	U8_ID,
	U9_ID,
	U10_ID,
	U11_ID,
	U12_ID,
	U13_ID,
	U14_ID,
	U15_ID,

	A_ANT2_ID,
	A_ANT4_ID,
	
	A1_PHICH_ID,	
	A2_PHICH_ID
};
