#pragma once




	
	int multiplex_pdcch(int16 **inbuf, int *pdcch_format_list, int num_pdcch
					, int num_grp_phich, int mi_phich, int fdd_tdd_mode, int num_ofdm_pdcch, int num_rb_dl
					, int num_bstx_ant, int cp_cfg, int16 *outbuf);

	int interleaver_pdcch(cfloat *inbuf, int inlen, int num_id_cell, cfloat *outbuf);				 


