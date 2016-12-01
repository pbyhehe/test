#pragma once



	

	
	int descram_core(float *inbuf, int inlen, int reset, int flag, int cinit, int Nc, float *outbuf);

	int descram_pcfich(float *inbuf, int inlen, int num_id_cell, int subfrm_idx, float *outbuf);

	int descram_pdcch(float *inbuf, int inlen, int num_id_cell, int subfrm_idx, float *outbuf);

	int descram_pdsch(float *inbuf, int inlen, int reset, int num_id_cell, int subfrm_idx
								, int idx_codeword, int num_rnti, float *outbuf);

	int descram_pbch(float *inbuf, int inlen, int reset, int num_id_cell, int cp_cfg, int frm_idx, float *outbuf);

