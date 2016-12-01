#pragma once


#include "foo.h"





	
	int rmvRS_ctrlch(cfloat **inbuf, int num_row, int num_rb_dl, int ofdm_idx, int num_id_cell
							 , int num_bstx_ant, cfloat **outbuf);

	int get_subcarrier(cfloat **inbuf, int size_fft, int num_subcarrier, int num_uerx_ant, cfloat **outbuf);

	int	deremap_pcfich(cfloat *inbuf, int num_rb_dl, int num_id_cell, float gain_pcfich_RouB, cfloat *outbuf);

	int deremap_pdcch(cfloat **inbuf, int num_rb_dl, int num_grp_phich, int mi_phich, int mux_mode
								 , int cp_cfg, int subfrm_idx, int num_ofdm_pdcch, int num_id_cell, int num_bstx_ant
								 , int duration_phich, float gain_pdcch_RA, float gain_pdcch_RB, cfloat *outbuf);

	int deremap_phich(cfloat **inbuf, int num_rb_dl, int subfrm_idx, int num_id_cell
								, int num_bstx_ant, int duration_phich, int cp_cfg, int idx_group_phich
								, float gain_phich_RA, float gain_phich_RB, cfloat *outbuf);

	int deremap_pdsch(cfloat **inbuf, int num_row, int num_rb_dl, int16 rb_alloc_pdsch[2][110]
								, int subfrm_idx, int ofdm_idx, int num_id_cell, int num_bstx_ant, int cp_cfg
								, float gain_pdsch_RA, float gain_pdsch_RB, cfloat **outbuf);

	int deremap_pbch(cfloat **inbuf, int num_row, int num_rb_dl, int subfrm_idx, int ofdm_idx
							   , int num_id_cell, int num_bstx_ant, int cp_cfg, float gain_pbch_RA
							   , float gain_pbch_RB, cfloat **outbuf);





