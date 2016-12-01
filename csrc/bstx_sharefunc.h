#pragma once



	int bstx_isDL(int subfrm_idx, int UL_DL_cfg);

	int bstx_isSpecialSubframe(int subfrm_idx, int UL_DL_cfg);

	int bstx_isUL(int subfrm_idx, int UL_DL_cfg);

	int bstx_get_mi_phich(int subfrm_idx, int uldl_swtich);

	int get_num_group_phich(int num_rb_dl, int ng_phich, int cp_cfg);

	int getLen_specsubfram(int CP_cfg, int spcal_subfrm_cfg);

	int get_REnum_pdsch(int num_RB_DL, int16 **rb_alloc_pdsch, int subfrm_idx, int num_ofdm_pdcch, int num_bstx_ant					
		, int cp_cfg, int uldl_switch_cfg, int spec_subfrm_cfg);


//extern bstx_sharefunc bstx_sharefunc_u;
