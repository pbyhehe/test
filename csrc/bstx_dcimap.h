#pragma once

#include "bstx_define.h"



	int dec2bin(int dec, int inlen, int16 *outbuf);
	
	int DCI_0_write(int num_RB_UL, int uldl_switch_cfg, bstx_ctrl_pdsch *ptr_ctrl_pdsch, int16 *outbuf);
	int DCI_1_write(int num_RB_DL, int num_bstx_ant, bstx_ctrl_pdsch *ptr_ctrl_pdsch, int16 *outbuf);
	int DCI_1A_write(int num_RB_DL, int num_bstx_ant, bstx_ctrl_pdsch *ptr_ctrl_pdsch, int16 *outbuf);
	int DCI_1C_write(int num_RB_DL, int num_bstx_ant, int enable_num_gap2, bstx_ctrl_pdsch *ptr_ctrl_pdsch, int16 *outbuf);
	int DCI_2_write(int num_RB_DL, int num_bstx_ant, bstx_ctrl_pdsch *ptr_ctrl_pdsch, int16 *outbuf);
	int DCI_2A_write(int num_RB_DL, int num_bstx_ant, bstx_ctrl_pdsch *ptr_ctrl_pdsch, int16 *outbuf);



	int dci_write(int dci_format, int num_rb_dl, int num_rb_ul, int num_bstx_ant, int uldl_switch_cfg
						   , bstx_ctrl_pdsch *ptr_ctrl_pdsch, int16 *outbuf);	

	int bstx_GetDCIlen(int dci_format, int num_RB_UL, int num_RB_DL, int num_bstx_ant);

