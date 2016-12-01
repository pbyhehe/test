#pragma once





	int get_RS(int num_id_cell, int num_bstx_ant, int num_rb_dl, int cp_cfg, int subfrm_idx, int ofdm_idx
					   , cfloat *rs_buf, int *rs_start);

	int ce(cfloat **inbuf, int num_subcarrier, cfloat rs_buf[220], int rs_start[4], int num_rs
				, int num_bstx_ant, int num_uerx_ant, cfloat **outH);

