#pragma once
#include "foo.h"
#include "bstx_define.h"
#include "bstx_sharefunc.h"
//#include "bstx_dcimap.h"
#include "bstx_parse.h"





	
	int init_sys_parameter(bstx_inf *bstx_inf_u, int subfrm_idx);

	int get_tbs_qam(int mcs_idx_cw[2], int num_codeword, int rnti_type, int dci_format, int num_rb_alloc
							 , int num_layer_cw[2], int transmit_scheme, int tbs_cw[2], int qm_cw[2]);

