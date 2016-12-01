#pragma once



	
	int uerx_pseudo_rand(int cinit, int *outbuf, int outlen);

	int uerx_isDL(int subfrm_idx, int uldl_switch);
	
	int uerx_isSpecialSubframe(int subfrm_idx, int uldl_switch);
	
	int uerx_isUL(int subfrm_idx, int uldl_switch);
	
	int specsubframlen(int cp_cfg, int spcal_subfrm_cfg);

	int get_num_phich_group(int num_rb_dl, int ng_phich, int cp_cfg);

	int uerx_get_mi_phich(int subfrm_idx, int uldl_swtich);


//extern uerx_sharefunc uerx_sharefunc_u;
