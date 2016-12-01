#pragma once






	int SISO_decod(cfloat *Y_inbuf, cfloat *H_inbuf, int inlen, float nosieVar
						  , cfloat *X_outbuf, cfloat *csi_outbuf);

	int MRC_decod(cfloat **Y_inbuf, cfloat **H_inbuf, int inlen, float nosieVar, cfloat *X_outbuf, cfloat *csi_outbuf);

	int SFBC_decod(cfloat **Y_inbuf, cfloat **H_inbuf, int inlen, int tx_ant_num, int rx_ant_num, float nosieVar
						  , cfloat *X_outbuf, cfloat *csi_outbuf);	
	
	int CLSM_decod(cfloat **Y_inbuf, cfloat **H_inbuf, int inlen, int num_col0_Nt,int num_row0_Nr, int layers
				, int cod_idx, float noiseVar, cfloat **X_outbuf, cfloat **csi_outbuf, cfloat **ns_outbuf);
	

	int equ_ctrch(cfloat **Y_inbuf, cfloat **H_inbuf, int inlen, float nosieVar, int tx_ant_num, int rx_ant_num
						   , cfloat *X_outbuf, cfloat *csi_outbuf);

	int equ_pdsch(cfloat **Y_inbuf, cfloat **H_inbuf, int inlen, float nosieVar, int num_bstx_ant
						, int num_uerx_ant, int diver_spat, int num_layer_total, int codebook_idx, cfloat **X_outbuf
						, cfloat **csi_outbuf, cfloat **ns_outbuf);
