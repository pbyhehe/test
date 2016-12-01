#include "bstx_phyprc.h"


int init_sys_parameter(bstx_inf *bstx_inf_u, int subfrm_idx)
{
	int i, j, c;
	//int tmp;
	
	int num_dl_proc_harq_list[7] = { 4, 7, 10, 9, 12, 15, 6}; // find by uplink and downlink switch config
	
	//char str[200];

	// init system parameter
	bstx_inf_u->cp_cfg				 = CP_CONFIG;
	bstx_inf_u->num_rb_dl			 = NUM_RB_DL;
	bstx_inf_u->num_rb_ul			 = NUM_RB_UL;
	bstx_inf_u->N_fft				 = N_FFT ;
	bstx_inf_u->num_ofdm_pdcch		 = L_PDCCH;
	bstx_inf_u->num_id_cell			 = N_ID_CELL;
	bstx_inf_u->num_bstx_ant		 = NUM_BSTX_ANTENNAS;
	bstx_inf_u->uldl_switch_cfg		 = ULDL_SWITCH_CONFIG;
	bstx_inf_u->spec_subfrm_cfg		 = SPECIAL_SUBFRAME_CONFIG;
	bstx_inf_u->duration_phich_cfg	 = DURATION_CONFIG_PHICH;
	bstx_inf_u->ng_phich_cfg		 = NG_CONFIG_PHICH;
	bstx_inf_u->space_subcarrier_cfg = SPACE_SUBCARRIERS_CONFIG;
    bstx_inf_u->fdd_tdd_mode		 = FDD_TDD_MODE;
	
	// power assignment
	bstx_inf_u->pss_RA		= RA_PSS;
	bstx_inf_u->sss_RA		= RA_SSS;
	bstx_inf_u->pbch_RA		= RA_PBCH;
	bstx_inf_u->pbch_RB		= RB_PBCH
	bstx_inf_u->phich_RA	= RA_PHICH;
	bstx_inf_u->phich_RB	= RB_PHICH;
	bstx_inf_u->pdcch_RA	= RA_PDCCH;
	bstx_inf_u->pdcch_RB	= RB_PDCCH;
	bstx_inf_u->pdsch_RA	= RA_PDSCH;
	bstx_inf_u->pdsch_RB	= RB_PDSCH;
	bstx_inf_u->pcfich_RB	= RB_PCFICH;

	// the number of pdcch, pdsch and phich
    bstx_inf_u->num_pdcch = NUM_PDCCH;
	bstx_inf_u->num_pdsch = NUM_PDSCH;
	bstx_inf_u->num_phich = NUM_PHICH;

	// phich parameters written by mac
	for (i=0; i<bstx_inf_u->num_phich; i++)
	{		
		
		bstx_inf_u->ctrl_arr_phich[i].idx_group_phich = 0;


		bstx_inf_u->ctrl_arr_phich[i].idx_seque_phich = i;
	}	

	// pdcch parameters written by mac
	for (i=0; i<bstx_inf_u->num_pdcch; i++)
	{

		bstx_inf_u->ctrl_arr_pdcch[i].dci_format = 5;


		bstx_inf_u->ctrl_arr_pdcch[i].pdcch_format	= 1;


		bstx_inf_u->ctrl_arr_pdcch[i].num_rnti = 0;


		bstx_inf_u->ctrl_arr_pdcch[i].ue_txant_selec = 0;


		bstx_inf_u->ctrl_arr_pdcch[i].ue_port = 0;
	}

	// pdsch parameters written by mac
	for (i=0; i<bstx_inf_u->num_pdsch; i++)
	{	
		
		bstx_inf_u->ctrl_arr_pdsch[i].transmit_mode = 4;


		bstx_inf_u->ctrl_arr_pdsch[i].ue_rs_cfg	= 0;


		bstx_inf_u->ctrl_arr_pdsch[i].ue_category = 5;
		
		
		bstx_inf_u->ctrl_arr_pdsch[i].num_rnti = 0;

		
		bstx_inf_u->ctrl_arr_pdsch[i].rnti_type = 3;


		bstx_inf_u->ctrl_arr_pdsch[i].rb_start_pdsch = 0;


		bstx_inf_u->ctrl_arr_pdsch[i].num_alloc_rb = ((i==0) ? 100:0);

		
		bstx_inf_u->ctrl_arr_pdsch[i].pusch_hopping_flag = 0;
		
		
		bstx_inf_u->ctrl_arr_pdsch[i].num_codeword = 2;


		bstx_inf_u->ctrl_arr_pdsch[i].mcs_cw[0] = 17;


		bstx_inf_u->ctrl_arr_pdsch[i].mcs_cw[1] = 17;


		bstx_inf_u->ctrl_arr_pdsch[i].tpc_pusch = 0;


		bstx_inf_u->ctrl_arr_pdsch[i].dai = 0;


		bstx_inf_u->ctrl_arr_pdsch[i].cyclicshift_dmrs = 7;


		bstx_inf_u->ctrl_arr_pdsch[i].ul_index = 1;


		bstx_inf_u->ctrl_arr_pdsch[i].cqi_request = 0;
		
		
		bstx_inf_u->ctrl_arr_pdsch[i].re_alloc_type = 0;
		
		
		bstx_inf_u->ctrl_arr_pdsch[i].tpc_pucch = 0;
		
		
		bstx_inf_u->ctrl_arr_pdsch[i].local_distrib_vrb_flg = 0;
		
		
		bstx_inf_u->ctrl_arr_pdsch[i].preamble_idx = 0;
		
		
		bstx_inf_u->ctrl_arr_pdsch[i].prach_mask_idx = 0;
		
		
		bstx_inf_u->ctrl_arr_pdsch[i].transblock_codeword_swap = 0;
	}

	// caculate the system parameters from the exist parameters
	bstx_inf_u->num_harq_process = num_dl_proc_harq_list[bstx_inf_u->uldl_switch_cfg];

	// caculate the phich parameters
	bstx_inf_u->mi_phich = bstx_get_mi_phich(subfrm_idx, bstx_inf_u->uldl_switch_cfg);

	bstx_inf_u->num_group_phich = get_num_group_phich(bstx_inf_u->num_rb_dl, bstx_inf_u->ng_phich_cfg
	, bstx_inf_u->cp_cfg);

	for(i=0; i<10; i++)
	{
		bstx_inf_u->harqidx_rv_ndi_precodeidx[0][i] = i; // process number index
		bstx_inf_u->harqidx_rv_ndi_precodeidx[1][i] = 0; // codeword 0 : RV
		bstx_inf_u->harqidx_rv_ndi_precodeidx[2][i] = 1; // codeword 0 : NDI
		bstx_inf_u->harqidx_rv_ndi_precodeidx[3][i] = 0; // codeword 1 : RV
		bstx_inf_u->harqidx_rv_ndi_precodeidx[4][i] = 1; // codeword 1 : NDI
		bstx_inf_u->harqidx_rv_ndi_precodeidx[5][i] = PRECODING_CODEBOOK_INDEX; // precoding information in DCI 2
	}

	// caculate the pdsch parameters
	for (i=0; i<bstx_inf_u->num_pdsch; i++)
	{
		bstx_inf_u->ctrl_arr_pdsch[i].harqprocess_idx	= bstx_inf_u->harqidx_rv_ndi_precodeidx[0][subfrm_idx];
		bstx_inf_u->ctrl_arr_pdsch[i].ndi_cw[0]			= bstx_inf_u->harqidx_rv_ndi_precodeidx[2][subfrm_idx];
		bstx_inf_u->ctrl_arr_pdsch[i].ndi_cw[1]			= bstx_inf_u->harqidx_rv_ndi_precodeidx[4][subfrm_idx];
		bstx_inf_u->ctrl_arr_pdsch[i].precode_idx		= bstx_inf_u->harqidx_rv_ndi_precodeidx[5][subfrm_idx];

		for(j=0; j<110; j++)
		{
			if(j >= bstx_inf_u->ctrl_arr_pdsch[i].rb_start_pdsch
				&& j < (bstx_inf_u->ctrl_arr_pdsch[i].rb_start_pdsch+bstx_inf_u->ctrl_arr_pdsch[i].num_alloc_rb))
			{
				bstx_inf_u->ctrl_arr_pdsch[i].rb_alloc_bitmap[0][j] = 1;
				bstx_inf_u->ctrl_arr_pdsch[i].rb_alloc_bitmap[1][j] = 1;
			}
			else
			{
				bstx_inf_u->ctrl_arr_pdsch[i].rb_alloc_bitmap[0][j] = 0;
				bstx_inf_u->ctrl_arr_pdsch[i].rb_alloc_bitmap[1][j] = 0;
			}
		}

		if(bstx_inf_u->ctrl_arr_pdcch[i].dci_format == BSTX_DCI_0)
		{
			assert(bstx_inf_u->ctrl_arr_pdsch[i].re_alloc_type == 2);
		}
		else if(bstx_inf_u->ctrl_arr_pdcch[i].dci_format == BSTX_DCI_1)
		{
			if (bstx_inf_u->num_rb_dl <= 10)
			{	
				assert(bstx_inf_u->ctrl_arr_pdsch[i].re_alloc_type == 0);
			}

			assert(bstx_inf_u->ctrl_arr_pdsch[i].num_codeword == 1);
			
			bstx_inf_u->ctrl_arr_pdsch[i].num_layer_spatial_cw[0] = 1;
			bstx_inf_u->ctrl_arr_pdsch[i].cdd_cfg = BSTX_NO_CDD;
			bstx_inf_u->ctrl_arr_pdsch[i].codebook_idx = 0;
			
			if(bstx_inf_u->num_bstx_ant == 1)
			{
				bstx_inf_u->ctrl_arr_pdsch[i].transmit_scheme = BSTX_SISO;
				bstx_inf_u->ctrl_arr_pdsch[i].num_layer_total = 1;
			}
			else
			{
				bstx_inf_u->ctrl_arr_pdsch[i].transmit_scheme = BSTX_DIVERSITY;
				bstx_inf_u->ctrl_arr_pdsch[i].num_layer_total = bstx_inf_u->num_bstx_ant;
			}						
			
			get_tbs_qam(bstx_inf_u->ctrl_arr_pdsch[i].mcs_cw, 1, bstx_inf_u->ctrl_arr_pdsch[i].rnti_type
					, bstx_inf_u->ctrl_arr_pdcch[i].dci_format, bstx_inf_u->ctrl_arr_pdsch[i].num_alloc_rb
					, bstx_inf_u->ctrl_arr_pdsch[i].num_layer_spatial_cw, bstx_inf_u->ctrl_arr_pdsch[i].transmit_scheme
					, bstx_inf_u->ctrl_arr_pdsch[i].tbs_cw, bstx_inf_u->ctrl_arr_pdsch[i].qm_cw);	
		}
		else if(bstx_inf_u->ctrl_arr_pdcch[i].dci_format == BSTX_DCI_1A)
		{
			assert(bstx_inf_u->ctrl_arr_pdsch[i].re_alloc_type == 2);
			assert(bstx_inf_u->ctrl_arr_pdsch[i].num_codeword == 1);
			
			bstx_inf_u->ctrl_arr_pdsch[i].num_layer_spatial_cw[0] = 1;
			bstx_inf_u->ctrl_arr_pdsch[i].cdd_cfg = BSTX_NO_CDD;
			bstx_inf_u->ctrl_arr_pdsch[i].codebook_idx = 0;	

			if(bstx_inf_u->num_bstx_ant == 1)
			{
				bstx_inf_u->ctrl_arr_pdsch[i].transmit_scheme = BSTX_SISO;
				bstx_inf_u->ctrl_arr_pdsch[i].num_layer_total = 1;
			}
			else
			{
				bstx_inf_u->ctrl_arr_pdsch[i].transmit_scheme = BSTX_DIVERSITY;
				bstx_inf_u->ctrl_arr_pdsch[i].num_layer_total = bstx_inf_u->num_bstx_ant;
			}			
			
			if (bstx_inf_u->ctrl_arr_pdsch[i].rnti_type != BSTX_C_RNTI)
			{
				get_tbs_qam(bstx_inf_u->ctrl_arr_pdsch[i].mcs_cw, 1, bstx_inf_u->ctrl_arr_pdsch[i].rnti_type
						, bstx_inf_u->ctrl_arr_pdcch[i].dci_format, bstx_inf_u->ctrl_arr_pdsch[i].num_alloc_rb
						, bstx_inf_u->ctrl_arr_pdsch[i].num_layer_spatial_cw, bstx_inf_u->ctrl_arr_pdsch[i].transmit_scheme
						, bstx_inf_u->ctrl_arr_pdsch[i].tbs_cw, bstx_inf_u->ctrl_arr_pdsch[i].qm_cw);
			}
		}
		else if(bstx_inf_u->ctrl_arr_pdcch[i].dci_format == BSTX_DCI_1C)
		{
			assert(bstx_inf_u->ctrl_arr_pdsch[i].rnti_type == BSTX_SI_RNTI 
				|| bstx_inf_u->ctrl_arr_pdsch[i].rnti_type == BSTX_P_RNTI
				|| bstx_inf_u->ctrl_arr_pdsch[i].rnti_type == BSTX_RA_RNTI);

			if (bstx_inf_u->num_bstx_ant == 1)
			{
				bstx_inf_u->ctrl_arr_pdsch[i].transmit_scheme = BSTX_SISO;
			}
			else
			{
				bstx_inf_u->ctrl_arr_pdsch[i].transmit_scheme = BSTX_DIVERSITY;
			}

			get_tbs_qam(bstx_inf_u->ctrl_arr_pdsch[i].mcs_cw, 1, bstx_inf_u->ctrl_arr_pdsch[i].rnti_type
					, bstx_inf_u->ctrl_arr_pdcch[i].dci_format, bstx_inf_u->ctrl_arr_pdsch[i].num_alloc_rb
					, bstx_inf_u->ctrl_arr_pdsch[i].num_layer_spatial_cw, bstx_inf_u->ctrl_arr_pdsch[i].transmit_scheme
					, bstx_inf_u->ctrl_arr_pdsch[i].tbs_cw, bstx_inf_u->ctrl_arr_pdsch[i].qm_cw);			
		}
		else if(bstx_inf_u->ctrl_arr_pdcch[i].dci_format == BSTX_DCI_2)
		{
			if (bstx_inf_u->num_rb_dl <= 10)
			{		
				assert(bstx_inf_u->ctrl_arr_pdsch[i].re_alloc_type == 0);
			}	

			assert(bstx_inf_u->ctrl_arr_pdsch[i].rnti_type==BSTX_C_RNTI || bstx_inf_u->ctrl_arr_pdsch[i].rnti_type==BSTX_SPSC_RNTI);
			
			assert(bstx_inf_u->ctrl_arr_pdsch[i].transmit_mode == BSTX_TM_MODE4);

			get_tbs_qam(bstx_inf_u->ctrl_arr_pdsch[i].mcs_cw, bstx_inf_u->ctrl_arr_pdsch[i].num_codeword
				, bstx_inf_u->ctrl_arr_pdsch[i].rnti_type, bstx_inf_u->ctrl_arr_pdcch[i].dci_format
				, bstx_inf_u->ctrl_arr_pdsch[i].num_alloc_rb, bstx_inf_u->ctrl_arr_pdsch[i].num_layer_spatial_cw
				, bstx_inf_u->ctrl_arr_pdsch[i].transmit_scheme, bstx_inf_u->ctrl_arr_pdsch[i].tbs_cw
				, bstx_inf_u->ctrl_arr_pdsch[i].qm_cw);

			if (bstx_inf_u->num_bstx_ant == 2)
			{
				bstx_inf_u->ctrl_arr_pdsch[i].cdd_cfg = BSTX_NO_CDD;
				
				if (bstx_inf_u->ctrl_arr_pdsch[i].num_codeword == 1)
				{
					switch (bstx_inf_u->ctrl_arr_pdsch[i].precode_idx)
					{
					case 0:
						bstx_inf_u->ctrl_arr_pdsch[i].num_layer_total = 2;
						bstx_inf_u->ctrl_arr_pdsch[i].transmit_scheme = BSTX_DIVERSITY;
						bstx_inf_u->ctrl_arr_pdsch[i].codebook_idx	  = 1; // zuohuanhuan
//						bstx_inf_u->ctrl_arr_pdsch[i].codebook_idx	  = -1; // huangshoujun_server

						break;
					case 1:
						bstx_inf_u->ctrl_arr_pdsch[i].num_layer_total = 1;
						bstx_inf_u->ctrl_arr_pdsch[i].transmit_scheme = BSTX_SPATIAL;
						bstx_inf_u->ctrl_arr_pdsch[i].codebook_idx	  = 0;
						break;
					case 2:
						bstx_inf_u->ctrl_arr_pdsch[i].num_layer_total = 1;
						bstx_inf_u->ctrl_arr_pdsch[i].transmit_scheme = BSTX_SPATIAL;
						bstx_inf_u->ctrl_arr_pdsch[i].codebook_idx	  = 1;
						break;
					case 3:
						bstx_inf_u->ctrl_arr_pdsch[i].num_layer_total = 1;
						bstx_inf_u->ctrl_arr_pdsch[i].transmit_scheme = BSTX_SPATIAL;
						bstx_inf_u->ctrl_arr_pdsch[i].codebook_idx	  = 2;
						break;
					case 4:
						bstx_inf_u->ctrl_arr_pdsch[i].num_layer_total = 1;
						bstx_inf_u->ctrl_arr_pdsch[i].transmit_scheme = BSTX_SPATIAL;
						bstx_inf_u->ctrl_arr_pdsch[i].codebook_idx	  = 3;
						break;
					case 5:
						bstx_inf_u->ctrl_arr_pdsch[i].num_layer_total = 1;
						bstx_inf_u->ctrl_arr_pdsch[i].transmit_scheme = BSTX_SPATIAL;
						bstx_inf_u->ctrl_arr_pdsch[i].codebook_idx	  = bstx_inf_u->ctrl_arr_pdsch[i].last_codebook_idx;
						
						if (bstx_inf_u->ctrl_arr_pdsch[i].last_ri == 2)	
							bstx_inf_u->ctrl_arr_pdsch[i].codebook_idx = 0;
						
						break;
					case 6:
						bstx_inf_u->ctrl_arr_pdsch[i].num_layer_total = 1;
						bstx_inf_u->ctrl_arr_pdsch[i].transmit_scheme = BSTX_SPATIAL;
						bstx_inf_u->ctrl_arr_pdsch[i].codebook_idx	  = bstx_inf_u->ctrl_arr_pdsch[i].last_codebook_idx;
						
						if (bstx_inf_u->ctrl_arr_pdsch[i].last_ri == 2)		
							bstx_inf_u->ctrl_arr_pdsch[i].codebook_idx = 0;
						
						break;
					case 7:
						assert(0);
						
					default:
						break;				
					}	
				}
				else
				{	 
					switch (bstx_inf_u->ctrl_arr_pdsch[i].precode_idx)
					{
					case 0:
						bstx_inf_u->ctrl_arr_pdsch[i].num_layer_total = 2;
						bstx_inf_u->ctrl_arr_pdsch[i].transmit_scheme = BSTX_SPATIAL;
						bstx_inf_u->ctrl_arr_pdsch[i].codebook_idx	= 1;
						
						break;
					case 1:
						bstx_inf_u->ctrl_arr_pdsch[i].num_layer_total = 2;
						bstx_inf_u->ctrl_arr_pdsch[i].transmit_scheme = BSTX_SPATIAL;
						bstx_inf_u->ctrl_arr_pdsch[i].codebook_idx	= 2;
						break;
					case 2:
						bstx_inf_u->ctrl_arr_pdsch[i].num_layer_total = 1;
						bstx_inf_u->ctrl_arr_pdsch[i].transmit_scheme = BSTX_SPATIAL;
						bstx_inf_u->ctrl_arr_pdsch[i].codebook_idx	= bstx_inf_u->ctrl_arr_pdsch[i].last_codebook_idx;
						break;
					case 3:
					case 4:
					case 5:
					case 6:
					case 7:
						assert(0); 
					default:
						break;
					}
				}
			}
			else if(bstx_inf_u->num_bstx_ant == 4)
			{
				bstx_inf_u->ctrl_arr_pdsch[i].cdd_cfg = BSTX_NO_CDD;
				
				if (bstx_inf_u->ctrl_arr_pdsch[i].num_codeword == 1)
				{
					assert (bstx_inf_u->ctrl_arr_pdsch[i].precode_idx < 35);
					
					// get total number of layers
					if(bstx_inf_u->ctrl_arr_pdsch[i].precode_idx == 0)
						bstx_inf_u->ctrl_arr_pdsch[i].num_layer_total = 4;
					else if(bstx_inf_u->ctrl_arr_pdsch[i].precode_idx >= 1 && bstx_inf_u->ctrl_arr_pdsch[i].precode_idx <= 17)
						bstx_inf_u->ctrl_arr_pdsch[i].num_layer_total = 1;
					else if(bstx_inf_u->ctrl_arr_pdsch[i].precode_idx >= 18 && bstx_inf_u->ctrl_arr_pdsch[i].precode_idx <=34)
						bstx_inf_u->ctrl_arr_pdsch[i].num_layer_total = 2;
					else 
						bstx_inf_u->ctrl_arr_pdsch[i].num_layer_total = 0;
					
					// get transmit scheme
					if(bstx_inf_u->ctrl_arr_pdsch[i].precode_idx == 0)
						bstx_inf_u->ctrl_arr_pdsch[i].transmit_scheme = BSTX_DIVERSITY;
					else
						bstx_inf_u->ctrl_arr_pdsch[i].transmit_scheme = BSTX_SPATIAL;
					
					// get codebook index
					if(bstx_inf_u->ctrl_arr_pdsch[i].precode_idx >= 1 && bstx_inf_u->ctrl_arr_pdsch[i].precode_idx <= 16)
						bstx_inf_u->ctrl_arr_pdsch[i].codebook_idx = bstx_inf_u->ctrl_arr_pdsch[i].precode_idx - 1;
					else if(bstx_inf_u->ctrl_arr_pdsch[i].precode_idx == 17 || bstx_inf_u->ctrl_arr_pdsch[i].precode_idx == 34)
						bstx_inf_u->ctrl_arr_pdsch[i].codebook_idx = bstx_inf_u->ctrl_arr_pdsch[i].last_codebook_idx;
					else if(bstx_inf_u->ctrl_arr_pdsch[i].precode_idx >= 18 && bstx_inf_u->ctrl_arr_pdsch[i].precode_idx <=33)
						bstx_inf_u->ctrl_arr_pdsch[i].codebook_idx = bstx_inf_u->ctrl_arr_pdsch[i].precode_idx - 18;
					else 
						bstx_inf_u->ctrl_arr_pdsch[i].codebook_idx = 0;
				}
				else
				{			
					assert (bstx_inf_u->ctrl_arr_pdsch[i].precode_idx < 51);
					
					// get total number of layers
					if(bstx_inf_u->ctrl_arr_pdsch[i].precode_idx >= 0  && bstx_inf_u->ctrl_arr_pdsch[i].precode_idx <= 16)
						bstx_inf_u->ctrl_arr_pdsch[i].num_layer_total = 2;
					else if(bstx_inf_u->ctrl_arr_pdsch[i].precode_idx >= 17 && bstx_inf_u->ctrl_arr_pdsch[i].precode_idx <= 33)
						bstx_inf_u->ctrl_arr_pdsch[i].num_layer_total = 3;
					else if(bstx_inf_u->ctrl_arr_pdsch[i].precode_idx >= 34 && bstx_inf_u->ctrl_arr_pdsch[i].precode_idx <= 50)
						bstx_inf_u->ctrl_arr_pdsch[i].num_layer_total = 4;
					else 
						bstx_inf_u->ctrl_arr_pdsch[i].num_layer_total = 0;
					
					// get transmit scheme
					bstx_inf_u->ctrl_arr_pdsch[i].transmit_scheme = BSTX_SPATIAL;
					
					// get codebook index
					if(bstx_inf_u->ctrl_arr_pdsch[i].precode_idx >= 0  && bstx_inf_u->ctrl_arr_pdsch[i].precode_idx <= 15)
						bstx_inf_u->ctrl_arr_pdsch[i].codebook_idx = bstx_inf_u->ctrl_arr_pdsch[i].precode_idx;
					else if(bstx_inf_u->ctrl_arr_pdsch[i].precode_idx == 16 || bstx_inf_u->ctrl_arr_pdsch[i].precode_idx == 33)
						bstx_inf_u->ctrl_arr_pdsch[i].codebook_idx = bstx_inf_u->ctrl_arr_pdsch[i].last_codebook_idx;
					else if(bstx_inf_u->ctrl_arr_pdsch[i].precode_idx >= 17 && bstx_inf_u->ctrl_arr_pdsch[i].precode_idx <= 32)
						bstx_inf_u->ctrl_arr_pdsch[i].codebook_idx = bstx_inf_u->ctrl_arr_pdsch[i].precode_idx - 17;
					else 
						bstx_inf_u->ctrl_arr_pdsch[i].codebook_idx = 0;
				}		
			}
			
			// get layer of each codeword
			switch (bstx_inf_u->ctrl_arr_pdsch[i].num_layer_total)                           
			{
			case 1: 
				bstx_inf_u->ctrl_arr_pdsch[i].num_layer_spatial_cw[0] = 1;
				bstx_inf_u->ctrl_arr_pdsch[i].num_layer_spatial_cw[1] = 0;
				break;
			case 2:
				if (bstx_inf_u->ctrl_arr_pdsch[i].num_codeword == 2)
				{
					bstx_inf_u->ctrl_arr_pdsch[i].num_layer_spatial_cw[0] = 1;
					bstx_inf_u->ctrl_arr_pdsch[i].num_layer_spatial_cw[1] = 1;
				}
				else
				{
					bstx_inf_u->ctrl_arr_pdsch[i].num_layer_spatial_cw[0] = 2;
					bstx_inf_u->ctrl_arr_pdsch[i].num_layer_spatial_cw[1] = 0;
				}
				break;
			case 3:
				bstx_inf_u->ctrl_arr_pdsch[i].num_layer_spatial_cw[0] = 1;
				bstx_inf_u->ctrl_arr_pdsch[i].num_layer_spatial_cw[1] = 2;
				break;
			case 4:
				bstx_inf_u->ctrl_arr_pdsch[i].num_layer_spatial_cw[0] = 2;
				bstx_inf_u->ctrl_arr_pdsch[i].num_layer_spatial_cw[1] = 2;
				break;
			default:
				break;
			}
		}
		else if(bstx_inf_u->ctrl_arr_pdcch[i].dci_format == BSTX_DCI_2A)
		{
			if (bstx_inf_u->num_rb_dl <= 10)
			{		
				bstx_inf_u->ctrl_arr_pdsch[i].re_alloc_type = 0;
			}

			assert(bstx_inf_u->ctrl_arr_pdsch[i].rnti_type==BSTX_C_RNTI || bstx_inf_u->ctrl_arr_pdsch[i].rnti_type==BSTX_SPSC_RNTI);
			
			assert(bstx_inf_u->ctrl_arr_pdsch[i].transmit_mode == BSTX_TM_MODE3);

			get_tbs_qam(bstx_inf_u->ctrl_arr_pdsch[i].mcs_cw, bstx_inf_u->ctrl_arr_pdsch[i].num_codeword
				, bstx_inf_u->ctrl_arr_pdsch[i].rnti_type, bstx_inf_u->ctrl_arr_pdcch[i].dci_format
				, bstx_inf_u->ctrl_arr_pdsch[i].num_alloc_rb, bstx_inf_u->ctrl_arr_pdsch[i].num_layer_spatial_cw
				, bstx_inf_u->ctrl_arr_pdsch[i].transmit_scheme, bstx_inf_u->ctrl_arr_pdsch[i].tbs_cw
				, bstx_inf_u->ctrl_arr_pdsch[i].qm_cw);			
			
			if (bstx_inf_u->num_bstx_ant == 2)
			{
				if (bstx_inf_u->ctrl_arr_pdsch[i].num_codeword == 2)
				{
					bstx_inf_u->ctrl_arr_pdsch[i].num_layer_total = 2;
					bstx_inf_u->ctrl_arr_pdsch[i].transmit_scheme = BSTX_SPATIAL;
					bstx_inf_u->ctrl_arr_pdsch[i].cdd_cfg		  = BSTX_LARGE_CDD;
					bstx_inf_u->ctrl_arr_pdsch[i].codebook_idx	  = 0; // linjiangnan 20141218
				}
				else if (bstx_inf_u->ctrl_arr_pdsch[i].num_codeword == 1)
				{
					bstx_inf_u->ctrl_arr_pdsch[i].num_layer_total = 2;
					bstx_inf_u->ctrl_arr_pdsch[i].transmit_scheme = BSTX_DIVERSITY;
					bstx_inf_u->ctrl_arr_pdsch[i].cdd_cfg		  = BSTX_NO_CDD;
				}
			}				
			
			// get layer of each codeword
			switch (bstx_inf_u->ctrl_arr_pdsch[i].num_layer_total)
			{
			case 1: 
				bstx_inf_u->ctrl_arr_pdsch[i].num_layer_spatial_cw[0] = 1;
				bstx_inf_u->ctrl_arr_pdsch[i].num_layer_spatial_cw[1] = 0;
				break;
			case 2:
				if (bstx_inf_u->ctrl_arr_pdsch[i].num_codeword == 2)
				{
					bstx_inf_u->ctrl_arr_pdsch[i].num_layer_spatial_cw[0] = 1;
					bstx_inf_u->ctrl_arr_pdsch[i].num_layer_spatial_cw[1] = 1;
				}
				else
				{
					bstx_inf_u->ctrl_arr_pdsch[i].num_layer_spatial_cw[0] = 2;
					bstx_inf_u->ctrl_arr_pdsch[i].num_layer_spatial_cw[1] = 0;
				}
				break;
			case 3:
				bstx_inf_u->ctrl_arr_pdsch[i].num_layer_spatial_cw[0] = 1;
				bstx_inf_u->ctrl_arr_pdsch[i].num_layer_spatial_cw[1] = 2;
				break;
			case 4:
				bstx_inf_u->ctrl_arr_pdsch[i].num_layer_spatial_cw[0] = 2;
				bstx_inf_u->ctrl_arr_pdsch[i].num_layer_spatial_cw[1] = 2;
				break;
			default:
				break;
			}

		}
		else
		{
			assert(0);
		}

		// update the revision number
		if (bstx_inf_u->ctrl_arr_pdsch[i].num_codeword == 1)
		{
			bstx_inf_u->ctrl_arr_pdsch[i].rv_cw[1]  = 1;
			bstx_inf_u->ctrl_arr_pdsch[i].mcs_cw[1] = 0;
		}

        for (c=0; c<bstx_inf_u->ctrl_arr_pdsch[i].num_codeword; c++)
		{
			if ((bstx_inf_u->ctrl_arr_pdsch[i].qm_cw[c] == 1) 
				|| (bstx_inf_u->ctrl_arr_pdsch[i].qm_cw[c] == 2)
				|| (bstx_inf_u->ctrl_arr_pdsch[i].qm_cw[c] == 4))
			{
				assert(bstx_inf_u->harqidx_rv_ndi_precodeidx[1+c*2][subfrm_idx] >= 0);
				assert(bstx_inf_u->harqidx_rv_ndi_precodeidx[1+c*2][subfrm_idx] <= 3);

				bstx_inf_u->ctrl_arr_pdsch[i].rv_cw[c] = bstx_inf_u->harqidx_rv_ndi_precodeidx[1+c*2][subfrm_idx];
			}
			else if(bstx_inf_u->ctrl_arr_pdsch[i].qm_cw[c] == 6)
			{				
				if(bstx_inf_u->harqidx_rv_ndi_precodeidx[1+c*2][subfrm_idx] == 2)
					bstx_inf_u->ctrl_arr_pdsch[i].rv_cw[c] = 1;
				else if(bstx_inf_u->harqidx_rv_ndi_precodeidx[1+c*2][subfrm_idx] == 3)
					bstx_inf_u->ctrl_arr_pdsch[i].rv_cw[c] = 2;
				else
					bstx_inf_u->ctrl_arr_pdsch[i].rv_cw[c] = 0;
			}
			else
			{
				assert(0);
			}
        }
	}
	
	return 0;
}




int get_tbs_qam(int mcs_idx_cw[2], int num_codeword, int rnti_type, int dci_format, int num_rb_alloc
							 , int num_layer_spatial_cw[2], int transmit_scheme, int tbs_cw[2], int qm_cw[2])
{
	int i, j, c;
	int mcs_idx;
	int tbs_idx_cw[2];
	int tbs_idx;
	int tbs_L1;

	static int last_tbs_cw[2] = {-1, -1};
	
	// get qm and the transblock size idx
	if (rnti_type == BSTX_P_RNTI || rnti_type == BSTX_RA_RNTI || rnti_type == BSTX_SI_RNTI)
	{
		assert(dci_format == BSTX_DCI_1A || dci_format == BSTX_DCI_1C);
		
		for(c=0; c<num_codeword; c++)
		{
			qm_cw[c] = 2;
			tbs_idx_cw[c] = mcs_idx_cw[c];		
		}		
	}
	else
	{
		for(c=0; c<num_codeword; c++)
		{
			mcs_idx = mcs_idx_cw[c];

			if(mcs_idx < 29)
			{			
				qm_cw[c] = BSTX_QAM_TBS_IDX_TABLE[mcs_idx][0];		

				tbs_idx_cw[c] = BSTX_QAM_TBS_IDX_TABLE[mcs_idx][1];
			}
		}
	}
	
	// get the transblock size
	if (dci_format == BSTX_DCI_1C)
	{
		for(c=0; c<num_codeword; c++)
		{
			tbs_idx = tbs_idx_cw[c];

			tbs_cw[c] = BSTX_TBS_DCI_1C[tbs_idx];
			
			last_tbs_cw[c] = tbs_cw[c];
		}
	}
	else
	{
		for(c=0; c<num_codeword; c++)
		{
			mcs_idx = mcs_idx_cw[c];
			tbs_idx = tbs_idx_cw[c];

			if (mcs_idx < 29)
			{		
				if (num_layer_spatial_cw[c] == 2 && transmit_scheme == BSTX_SPATIAL)
				{			
					if (num_rb_alloc <= 55)
					{
						tbs_cw[c] = BSTX_TBS_TABLE1[tbs_idx][2*num_rb_alloc-1];
					}
					else
					{
						tbs_L1	= BSTX_TBS_TABLE1[tbs_idx][num_rb_alloc-1];			
						
						if(tbs_L1 <= 3624)			j = 0;
						else if(tbs_L1 <= 9912)		j = 2;
						else if(tbs_L1 <= 27376)	j = 4;
						else						j = 6;
						
						for (i=0; i<28; i++)
						{
							if (BSTX_TBS_TABLE2[i][j] == tbs_L1)
							{
								tbs_cw[c] = BSTX_TBS_TABLE2[i][j+1];

								break;
							}
						}					
					}
				}
				else
				{
					tbs_cw[c] = BSTX_TBS_TABLE1[tbs_idx][num_rb_alloc-1];
				}
				
				last_tbs_cw[c] = tbs_cw[c];
			}
			else
			{
				assert(last_tbs_cw[c] != -1);

				tbs_cw[c] = last_tbs_cw[c];
			}
		}
	}	

	return 0;	
}
