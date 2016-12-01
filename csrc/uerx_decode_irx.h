#pragma once

#include "uerx_sharefunc.h"
#include "typedefine.h"




	int decode_Init();
	int decode_Clear();
	
	int decode_pcfich(float *inbuf, int inlen, int num_rb_dl);

	int decode_phich(cfloat *x_inbuf, cfloat *csi_inbuf, int inlen, int subfrm_idx, int num_id_cell
								   , int cp_cfg, int idx_seque_phich);


