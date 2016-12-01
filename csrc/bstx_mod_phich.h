#pragma once





	
	int spread_phich(cfloat *inbuf, int inlen, int cp_cfg, int sequence_idx, cfloat *outbuf);	
	
	int grpallign_phich(cfloat *inbuf, int inlen, int cp_cfg, int group_idx, cfloat *outbuf);
	
	int mapunit_phich(cfloat **inbuf, int inlen, int num_bstx_ant, int cp_cfg, int num_grp_phich
						, int mi_phich, int mux_mode, int *idx_group_phich_list, cfloat **outbuf);

