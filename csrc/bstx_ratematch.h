#pragma once



    int sub_block_interleaver_convlution(int16 *inbuf, int inlen, int16 *outbuf);
	
	int subblock_intlv_1th_2th_turbo(int16 **inbuf, int *len_codeseg, int num_codeseg, int16 **outbuf
		, int *len_intlvseg);

	int subblock_intlv_the_3th_turbo(int16 **inbuf, int *len_codeseg, int num_codeseg, int16 **outbuf
		, int *len_intlvseg);

	int bit_collect_convolution(int16 *inbuf0, int16 *inbuf1, int16 *inbuf2, int inlen, int16 *outbuf);

	int bstx_bit_collect_turbo(int16 **inbuf0, int16 **inbuf1, int16 **inbuf2, int *len_intlvseg
		, int num_codeseg, int16 **outbuf);

	int bit_select_convolution(int16 *inbuf, int inlen, int16 *outbuf, int outlen);

	int bit_select_turbo(int16 **inbuf, int *len_intlvseg, int num_codseg,int rv, int qm
				, int num_layer, int num_harq_process, int G_pdsch, int K_MIMO, int catgry_ue
				, int16 **outbuf, int *len_selct);



	int ratematch_pdsch(int16 **inbuf0, int16 **inbuf1, int16 **inbuf2, int *len_codeseg, int num_codseg
								, int rv, int qm, int catgry_ue, int num_harq_process, int G_pdsch, int num_layer
								, int K_MIMO, int16 **outbuf, int *len_ratematch);

	int ratematch_pdcch(int16 *inbuf0, int16 *inbuf1, int16 *inbuf2, int inlen, int pdcch_format, int16 *outbuf);

	int ratematch_pbch(int16 *inbuf0, int16 *inbuf1, int16 *inbuf2, int inlen, int cp_cfg, int16 *outbuf);

