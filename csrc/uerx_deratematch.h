#pragma once

#include "uerx_define.h"


	int blockseg(int transblock_size, int *size_each_codseg, int *num_codeseg, int *num_nullbit);

	// turbo code
	int intlv_turbo(int16 *inbuf, int inlen, int16 *outbuf);

	int intlv_sub_1th_2th_turbo(int16 **inbuf, int *size_each_codeseg, int num_codeseg
												   , int16 **outbuf, int *size_each_intlvseg);

	int intlv_sub_3th_turbo(int16 **inbuf, int *size_each_codeseg, int num_codeseg
												   , int16 **outbuf, int *size_each_intlvseg);

	int uerx_bit_collect_turbo(int16 **inbuf0, int16 **inbuf1, int16 **inbuf2, int *size_each_intlvseg
										, int num_codeseg, int16 **outbuf);

	int select_turbo(int16 **inbuf, int *len_intlvseg, int num_codseg, int rv_idx, int qm
				, int num_layer, int num_harq_process, int G_pdsch, int k_mimo, int catgry_ue
				, int16 **outbuf, int *len_selctseg);

	int deselect_turbo(int16 **inbuf_flag, int *size_each_intlvseg, int num_codeseg, int rv_idx
										   , int qm, int NL, int num_harqprocess, int G_pdsch, int k_mimo
										   , int catgry_ue, float *inbuf, float **outbuf);

	int decollect_turbo(float **inbuf, int *size_each_intlvseg, int num_codeseg
											, float **outbuf0, float **outbuf1, float **outbuf2);

	int deintlv_sub_1th_2th_turbo(float **inbuf, int *size_each_codeseg, int num_codeseg, float **outbuf);

	int deintlv_sub_3th_turbo(float **inbuf, int *size_each_codeseg, int num_codeseg, float **outbuf);

	// convlution code
	int intlv_sub_conv(int16 *inbuf, int inlen, int16 *outbuf);

	int collect_conv(int16 *inbuf0, int16 *inbuf1, int16 *inbuf2, int inlen, int16 *outbuf);

	int deselect_conv(int16 *inbuf_flag, int inlen_flag, float *inbuf, int inlen, float *outbuf);

	int decollect_conv(float *inbuf, int inlen, float *outbuf0, float *outbuf1, float *outbuf2);

	int deintlv_sub_conv(float *inbuf, int sizeof_tbs_add_crc, float *outbuf);
	


	int deratematch_pdsch(float *inbuf, int inlen, int transblock_size, int catgry_ue
							, int num_harqprocess, int qm, int rv_idx, int NL, int k_mimo
							, float **outbuf0, float **outbuf1, float **outbuf2
							, int *size_each_codeseg, int *num_codeseg, int *num_nullbit); 

	int deratematch_pdcch(float *inbuf, int inlen, int transblock_size, float *outbuf0
										, float *outbuf1, float *outbuf2);

	int deratematch_pbch(float *inbuf, int inlen, int transblock_size, float *outbuf0
										, float *outbuf1, float *outbuf2);

