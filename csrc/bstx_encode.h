#pragma once



	int sub_encoder_turbo(int16 **inbuf, int *size_each_codseg, int num_codseg, int16 **x_outbuf, int16 **z_outbuf);

	int interleaver_turbo(int16 **inbuf, int *size_each_codseg, int num_codseg, int16 **outbuf
		, int num_null_bit, int *site_null_bit);

	int encode_turbo(int16 **inbuf, int *size_each_codseg, int num_codseg, int num_nullbit
								, int16 **d0_outbuf, int16 **d1_outbuf, int16 **d2_outbuf);

	int encode_convo(int16 *inbuf, int inlen, int16 *d0_outbuf, int16 *d1_outbuf, int16 *d2_outbuf);


 
	int encode_pcfich(int num_ofdm_pdcch, int num_rb_dl, int16 *outbuf);

	int encode_phich(int hi, int16 *outbuf);

	int encode_pdcch(int16 *inbuf, int inlen, int16 *d0_outbuf, int16 *d1_outbuf, int16 *d2_outbuf);

	int encode_pbch(int16 *inbuf, int inlen, int16 *d0_outbuf, int16 *d1_outbuf, int16 *d2_outbuf);

	int encode_pdsch(int16 **inbuf, int *size_each_codseg, int num_codseg, int num_nullbit
								, int16 **d0_outbuf, int16 **d1_outbuf, int16 **d2_outbuf);

