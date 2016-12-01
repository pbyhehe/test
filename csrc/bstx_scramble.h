#pragma once


	int scramble_core(int16 *inbuf, int inlen, int cinit, int Nc, int16 *outbuf);	



	int scramble_pbch(int16 *inbuf, int inlen,  int num_ID_cell, int16 *outbuf);

	int scramble_pdcch(int16 *inbuf, int inlen,  int num_ID_cell, int subfrm_idx, int16 *outbuf);

	int scramble_pdsch(int16 *inbuf, int inlen, int num_ID_cell, int idx_codeword, int num_rnti
								  , int subfrm_idx, int16 *outbuf);

	int scramble_pcfich(int16 *inbuf, int inlen,  int num_ID_cell, int subfrm_idx, int16 *outbuf);

	int scramble_phich(cfloat *inbuf, int inlen, int subfrm_idx, int num_ID_cell, int cp_cfg, cfloat *outbuf);

