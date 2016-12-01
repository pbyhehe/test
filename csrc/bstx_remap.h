#pragma once

#include "bstx_define.h"



	int remap_pss(cfloat *inbuf, int inlen, int subfrm_idx, int num_rb_dl, float gain_pss_RA, cfloat **outbuf);
	
	int remap_sss(cfloat *inbuf, int inlen, int subfrm_idx, int num_rb_dl, int cp_cfg, float gain_sss_RA, cfloat **outbuf);
	
	int remap_rs(cfloat *inbuf, int num_rb_dl, int subfrm_idx, int num_bstx_ant, int num_ID_cell
						 , int cp_cfg, int spcal_subfrm_cfg, int uldl_switch_cfg, cfloat **outbuf);
	
	int remap_pcfich(cfloat **inbuf, int inlen, int num_ID_cell, int num_bstx_ant, int num_RB_DL
						, float gain_pcfich_RB, cfloat **outbuf);

	int remap_pdcch(cfloat **inbuf, int num_RB_DL, int num_grp_phich, int mi_phich, int mux_mode, int cp_cfg
							, int subfrm_idx, int num_ofdm_pdcch, int num_ID_cell, int num_bstx_ant
							, int duration_phich_cfg, float gain_pdcch_RA, float gain_pdcch_RB, cfloat **outbuf);

	int remap_phich(cfloat **inbuf, int num_RB_DL, int num_grp_phich, int mi_phich, int mux_mode
							, int subfrm_idx, int num_ID_cell, int num_bstx_ant, int cp_cfg, int duration_ih_cfg
							, float gain_phich_RA, float gain_phich_RB, cfloat **outbuf);

	int remap_pdsch(cfloat **inbuf, int num_RB_DL, int16 **rb_alloc_pdsch
								 , int subfrm_idx, int num_ofdm_pdcch, int num_ID_cell, int num_bstx_ant					
								 , int cp_cfg, int uldl_switch_cfg, int spc_subfrm_cfg, float gain_pdsch_RA
								 , float gain_pdsch_RB, cfloat **outbuf);

	int remap_pbch(cfloat **inbuf, int num_RB_DL, int subfrm_idx
								, int num_ID_cell, int cp_cfg, int num_bstx_ant
								, float gain_pbch_RA, float gain_pbch_RB
								, cfloat **outbuf);

