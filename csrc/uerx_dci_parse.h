#pragma once

typedef struct dci1
{
	int Isready_uerx_dci_parse;
	int **QAM_TBSidx_table;
	int **TBS_table1;
	int **TBS_table2;
	int DCI_type;
	int TBS_last[2];
	int P;
	int DCI_0_len, DCI_1_len, DCI_1A_len, DCI_1B_len, DCI_1C_len, DCI_1D_len;
	int DCI_2_len, DCI_2A_len, DCI_3_len, DCI_3A_len;

	int first_call_N_gap;
	int N_step_RB, N_gap1,  N_gap2 , N_gap, N_VRB_DL_gap1, N_VRB_DL_gap2, N_VRB_DL_gap;
}uerx_dci_parse;


	int dci_Init(uerx_dci_parse  *uerx_dciparse_u);
	int dci_Clear(uerx_dci_parse  *uerx_dciparse_u);
	int dciparse(uerx_dci_parse  *uerx_dciparse_u,int *DCI_format, int16 N_DL_RB, int16 N_RB_UL
		,  int16 mux_mode, int16 RNTI_type
		, int16 transmission_mode, int16 num_bstx_ant
		, int16 *inbuf, int inlen,  int16 RI_last,	int16 PMI_last
		, int16 *codeword_num, int16 *diversity_spatial
		, int16 *layers_total, int16 layer_per_codeword[2]
		, int16 RB_arr[2][110], int16 *pdsch_rb_alloc
		, int16 *HARQ_process_idx, int16 NDI[2], int16 RV[2]
		, int TBS[2], int16 qm[2], int16 *TPC_PUCCH , int16 *DAI
								 , int16 *codebook_idx, int16 *cobook_column_idx,int Preamble_idx,int PRACH_Mask_idx);
// 	int DCI0_parser(int N_RB_UL, int16 *inbuf, UERX_inf *pinf);

	int DCI1_parser(uerx_dci_parse  *uerx_dciparse_u,int16 *inbuf, int16 N_DL_RB, int16 mux_mode, int16 RNTI_type, int16 transmission_mode, int16 num_bstx_ant,
		int16 * codeword_num, int16 *diversity_spatial, int16 *layers_total, int16 layer_per_codeword[2],
		int16 RB_arr[2][110], int16 *pdsch_rb_alloc, int16 *HARQ_process_idx, int16 NDI[2], int16 RV[2],
		int TBS[2], int16 qm[2], int16 *TPC_PUCCH , int16 *DAI);

//	int DCI1A_parser(int N_RB_DL, int16 *inbuf, UERX_inf *pinf);
	// 	int DCI1B_parser(int N_RB_DL, int16 *inbuf, UERX_inf *pinf);
	// 	int DCI1C_parser(int N_RB_DL, int16 *inbuf, UERX_inf *pinf);
	// 	int DCI1D_parser(int N_RB_DL, int16 *inbuf, UERX_inf *pinf);
// 	int DCI2_2A_parser(int N_DL_RB, int16 *inbuf, UERX_inf *pinf);
	int DCI2_2A_parser(uerx_dci_parse  *uerx_dciparse_u,int16 DCI_format, int16 *inbuf, int16 N_DL_RB, int16 mux_mode, int16 RNTI_type, int16 transmission_mode,
		int16 num_bstx_ant, int16 RI_last,	int16 PMI_last,
		int16  *codeword_num, int16 *diversity_spatial, int16 *layers_total,
		int16 layer_per_codeword[2], int16 RB_arr[2][110], int16 *pdsch_rb_alloc,
		int16 *HARQ_process_idx,  int16 NDI[2], int16 RV[2],
		int TBS[2], int16 qm[2], int16 *TPC_PUCCH , int16 *DAI, int16 *codebook_idx,
									int16 *cobook_column_idx);
	int DCI1A_parser(uerx_dci_parse  *uerx_dciparse_u, int16 *inbuf, int16 N_DL_RB, int16 mux_mode, int16 RNTI_type, int16 transmission_mode,
			int16 num_bstx_ant, int16 RI_last,	int16 PMI_last,
			int16 * codeword_num, int16 *diversity_spatial, int16 *layers_total,
			int16 layer_per_codeword[2], int16 RB_arr[2][110], int16 *pdsch_rb_alloc,
			int16 *HARQ_process_idx,  int16 NDI[2], int16 RV[2],
			int TBS[2], int16 qm[2], int16 *TPC_PUCCH , int16 *DAI, int16 *codebook_idx,
			int16 *cobook_column_idx,int DCI_sourlen,int Preamble_idx,int PRACH_Mask_idx);//14.6.9 add

	int RB_assig_inf_interpret(uerx_dci_parse  *uerx_dciparse_u,int16 inbuf[], int inlen, int16 N_DL_RB, int16 re_allocation_type, int16 Local_Distrib_VRB_flag,
											int16 RB_arr[2][110], int16 *pdsch_rb_alloc);

	int MCS_interpret1(uerx_dci_parse  *uerx_dciparse_u,int16 MCS[], int inlen, int16 transblock_idx, int16 RNTI_type, int16 DCI_type,
		int16 PRB_num, int16 TB2layer[2], int TBS[2], int16 qm[2], int16 diver_spatial);
	
	int MCS_interpret2(uerx_dci_parse  *uerx_dciparse_u,int16 MCS_index, int16 N_RB, int16 NL, int *transport_block, int16 *Qm);

	int RV_interpret(int16 RV_arr[], int16 DCI_type, int16 tbs_idx, int inlen, int16 RNTI_type, int16 RV[2]);
	
	int bit2dec(int16 inbuf[], int inlen, int16 type);

 	int Precoding_Info_interpret(int16 inbuf[], int inlen, int16 codeword_num, int16 RI_last, int16 PMI_last,
											  int16 *layers_total, int16 * diversity_spatial, int16 *cobook_index, int16 *cobook_column_idx);

	int uerx_GetDCIlen(uerx_dci_parse  *uerx_dciparse_u,int16 DCI_format, int16 N_RB_UL, int16 N_RB_DL, int16 N_TX_ant);
	int Get_pdsch_gap( uerx_dci_parse  *uerx_dciparse_u,int16 N_DL_RB);
	
 	int DCI1A_Is_Randacces(int16 * inbuf, int type2_len, int DCI1A_len);


