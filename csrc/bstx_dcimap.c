#include "foo.h"
#include "bstx_define.h"
#include "bstx_dcimap.h"
#include "bstx_parse.h"

int bstx_GetDCIlen(int dci_format, int num_rb_ul, int num_rb_dl, int num_bstx_ant)
{
	int i;
    int outlen;
	int num_bits_realloc_type01;
	int num_bits_realloc_type2_dl;
	int num_bits_realloc_type2_ul;
	int len_DCI_0, len_DCI_1, len_DCI_1A, len_DCI_1B, len_DCI_2, len_DCI_2A;

	int size[10] = {12, 14, 16, 20, 24, 26, 32, 40, 44, 56};

	if(num_rb_dl == 6)			num_bits_realloc_type2_dl = 5;
	else if(num_rb_dl == 15)	num_bits_realloc_type2_dl = 7;
	else if(num_rb_dl == 25)	num_bits_realloc_type2_dl = 9;
	else if(num_rb_dl == 50)	num_bits_realloc_type2_dl = 11;
	else if(num_rb_dl == 75)	num_bits_realloc_type2_dl = 12;
	else if(num_rb_dl == 100)	num_bits_realloc_type2_dl = 13;
	else assert(0);

	if(num_rb_ul == 6)			num_bits_realloc_type2_ul = 5;
	else if(num_rb_ul == 15)	num_bits_realloc_type2_ul = 7;
	else if(num_rb_ul == 25)	num_bits_realloc_type2_ul = 9;
	else if(num_rb_ul == 50)	num_bits_realloc_type2_ul = 11;
	else if(num_rb_ul == 75)	num_bits_realloc_type2_ul = 12;
	else if(num_rb_ul == 100)	num_bits_realloc_type2_ul = 13;
	else assert(0);
	
	if(num_rb_dl == 6)			num_bits_realloc_type01 = 6;
	else if(num_rb_dl == 15)	num_bits_realloc_type01 = 8;
	else if(num_rb_dl == 25)	num_bits_realloc_type01 = 13;
	else if(num_rb_dl == 50)	num_bits_realloc_type01 = 17;
	else if(num_rb_dl == 75)	num_bits_realloc_type01 = 19;
	else if(num_rb_dl == 100)	num_bits_realloc_type01 = 25;
	else assert(0);

	len_DCI_0  = 1 + 1 + num_bits_realloc_type2_ul + 5 + 1 + 2 + 3 + 2 + 1;
	len_DCI_1  = num_bits_realloc_type01 + 5 + 4 + 1 + 2 + 2 + 2;
	len_DCI_1A = 1 + 1 + num_bits_realloc_type2_dl + 5 + 4 + 1 + 2 + 2 + 2;
	
	if(num_bstx_ant == 2)
	{
		len_DCI_1B = 1 + num_bits_realloc_type2_dl + 5 + 4 + 1 + 2 + 2 + 1 + 2;
		len_DCI_2  = num_bits_realloc_type01 + 2 + 2+ 4 + 1 + 5 + 1 + 2 + 5 + 1 + 2 + 3;//if rb <10 no Type bit caohuan
    	len_DCI_2A = num_bits_realloc_type01 + 2 + 2+ 4 + 1 + 5 + 1 + 2 + 5 + 1 + 2 + 0;
	}
	else if(num_bstx_ant == 4)
	{
		len_DCI_1B = 1 + num_bits_realloc_type2_dl + 5 + 4 + 1 + 2 + 2 + 1 + 4;
		len_DCI_2  = num_bits_realloc_type01 + 2 + 2+ 4 + 1 + 5 + 1 + 2 + 5 + 1 + 2 + 6;
		len_DCI_2A = num_bits_realloc_type01 + 2 + 2+ 4 + 1 + 5 + 1 + 2 + 5 + 1 + 2 + 2;
	}
	else
	{
		len_DCI_1B = 0;
		len_DCI_2 = 0;
		len_DCI_2A = 0;
	}
	
	if (num_rb_dl > 10)
	{
		len_DCI_1++;
		len_DCI_2++;
		len_DCI_2A++;
	}
	
	if (len_DCI_1A < len_DCI_0)
	{
		len_DCI_1A = len_DCI_0;
	}
	
	for(i=0; i<10; i++)
	{
		if(len_DCI_1A == size[i])
		{
			len_DCI_1A++;
			break;
		}
	}
	
	if(len_DCI_0 < len_DCI_1A)
	{
		len_DCI_0 = len_DCI_1A;
	}
	
	if (len_DCI_1 == len_DCI_0)
	{
		len_DCI_1++;
	}
	
	for(i=0; i<10; i++)
	{
		if(len_DCI_1 == size[i])
		{
			len_DCI_1++;

			break;
		}
	}
	
	if(dci_format == BSTX_DCI_0)			outlen = len_DCI_0;
	else if(dci_format == BSTX_DCI_1)		outlen = len_DCI_1;
	else if(dci_format == BSTX_DCI_1A)		outlen = len_DCI_1A;
	else if(dci_format == BSTX_DCI_1B)		outlen = len_DCI_1B;
	else if(dci_format == BSTX_DCI_2)		outlen = len_DCI_2;
	else if(dci_format == BSTX_DCI_2A)		outlen = len_DCI_2A;

   	return outlen;
}

int dci_write(int dci_format, int num_rb_dl, int num_rb_ul, int num_bstx_ant, int uldl_switch_cfg
						   , bstx_ctrl_pdsch *ptr_ctrl_pdsch, int16 *outbuf)
{	
	int ret_len;
	int dci_len;
		
    dci_len = bstx_GetDCIlen(dci_format, num_rb_ul, num_rb_dl, num_bstx_ant);
	
	if(dci_format == BSTX_DCI_0)
		ret_len = DCI_0_write(num_rb_ul, uldl_switch_cfg, ptr_ctrl_pdsch, outbuf);
	else if(dci_format == BSTX_DCI_1)
		ret_len = DCI_1_write(num_rb_dl, num_bstx_ant, ptr_ctrl_pdsch, outbuf);	
	else if(dci_format == BSTX_DCI_1A)
		ret_len = DCI_1A_write(num_rb_dl, num_bstx_ant, ptr_ctrl_pdsch, outbuf);	
	else if(dci_format == BSTX_DCI_2)
		ret_len = DCI_2_write(num_rb_dl, num_bstx_ant, ptr_ctrl_pdsch, outbuf);	
	else if(dci_format == BSTX_DCI_2A)
		ret_len = DCI_2A_write(num_rb_dl, num_bstx_ant, ptr_ctrl_pdsch, outbuf);
	else
		assert(0);

	memset(&outbuf[ret_len], 0, (dci_len-ret_len)*sizeof(int16));

	return dci_len;
}

int DCI_0_write(int num_rb_ul, int uldl_switch, bstx_ctrl_pdsch *ptr_ctrl_pdsch, int16 *outbuf)
{
	int i;
	int cur_idx;	
	int num_bits_realloc_type2_ul;

	int pusch_hopping_flag	= ptr_ctrl_pdsch->pusch_hopping_flag;
	int tpc_pusch			= ptr_ctrl_pdsch->tpc_pusch;
	int cyclicshift_dmrs	= ptr_ctrl_pdsch->cyclicshift_dmrs;
	int ul_index			= ptr_ctrl_pdsch->ul_index;
	int dai					= ptr_ctrl_pdsch->dai;
	int cqi_request			= ptr_ctrl_pdsch->cqi_request;

	int *mcs_cw = ptr_ctrl_pdsch->mcs_cw;
	int *ndi_cw = ptr_ctrl_pdsch->ndi_cw;
	
	// function body	
	if(num_rb_ul == 6)			num_bits_realloc_type2_ul = 5;
	else if(num_rb_ul == 15)	num_bits_realloc_type2_ul = 7;
	else if(num_rb_ul == 25)	num_bits_realloc_type2_ul = 9;
	else if(num_rb_ul == 50)	num_bits_realloc_type2_ul = 11;
	else if(num_rb_ul == 75)	num_bits_realloc_type2_ul = 12;
	else if(num_rb_ul == 100)	num_bits_realloc_type2_ul = 13;
	else assert(0);

	cur_idx = 0;

	// 1 bit for DCI 0 indication
	outbuf[cur_idx] = 0;
	cur_idx++;

	// 1 bit for pusch hopping flag
	outbuf[cur_idx] = pusch_hopping_flag; 
	cur_idx++;

	for (i=0; i<num_bits_realloc_type2_ul; i++)
	{
		outbuf[cur_idx] = 1;
		cur_idx++;
	}
	
	dec2bin(mcs_cw[0], 5, &outbuf[cur_idx]);	
	cur_idx += 5;
	
	outbuf[cur_idx] = ndi_cw[0];
	cur_idx++;
	
	dec2bin(tpc_pusch, 2, &outbuf[cur_idx]);	
	cur_idx += 2;	
	
	dec2bin(cyclicshift_dmrs, 3, &outbuf[cur_idx]);
	cur_idx	+= 3;
	
	if (uldl_switch == 0)
	{
		dec2bin(ul_index, 2, &outbuf[cur_idx]);
		cur_idx	+= 2;
	}
	else
	{
		dec2bin(dai, 2, &outbuf[cur_idx]);
		cur_idx	+= 2;
	}
	
	outbuf[cur_idx] = cqi_request;
	cur_idx++;
	
	return cur_idx;
}


int DCI_1_write(int num_rb_dl, int num_bstx_ant, bstx_ctrl_pdsch *ptr_ctrl_pdsch, int16 *outbuf)
{
	int i;
	int cur_idx;
	int num_bits_realloc_type01;

	// function body	
	if(num_rb_dl == 6)			num_bits_realloc_type01 = 6;
	else if(num_rb_dl == 15)	num_bits_realloc_type01 = 8;
	else if(num_rb_dl == 25)	num_bits_realloc_type01 = 13;
	else if(num_rb_dl == 50)	num_bits_realloc_type01 = 17;
	else if(num_rb_dl == 75)	num_bits_realloc_type01 = 19;
	else if(num_rb_dl == 100)	num_bits_realloc_type01 = 25;
	else assert(0);
	
	cur_idx = 0;

	if (num_rb_dl > 10)
	{	// 1 bit for re_allocation_type
		outbuf[cur_idx] = ptr_ctrl_pdsch->re_alloc_type;
		cur_idx++;
	}	

	for(i=0; i<num_bits_realloc_type01; i++)
	{	// resource elements allocation assignment
		outbuf[cur_idx] = 1;
		cur_idx++;
	}

	// bit18 ~ 22:MCS(QAM and tbs_cw)
	dec2bin(ptr_ctrl_pdsch->mcs_cw[0], 5, &outbuf[cur_idx]);      
	cur_idx += 5;
	
	// bit23 ~ 26:HARQ_process_number
    dec2bin(ptr_ctrl_pdsch->harqprocess_idx, 4, &outbuf[cur_idx]); 	
	cur_idx += 4;
	
	// bit27: NDI	
	outbuf[cur_idx] = ptr_ctrl_pdsch->ndi_cw[0];
	cur_idx += 1;
	
	// bit28~29 RV
    dec2bin(ptr_ctrl_pdsch->rv_cw[0], 2, &outbuf[cur_idx]);
	cur_idx += 2;
	
	// bit30~31 TPC FOR PUCCH
    dec2bin(ptr_ctrl_pdsch->tpc_pucch, 2, &outbuf[cur_idx]);
	cur_idx += 2;	
	
	// bit32~33 downlink assignmengt index
    dec2bin(ptr_ctrl_pdsch->dai, 2, &outbuf[cur_idx]);
	cur_idx += 2;	
	
	return cur_idx;
}

int DCI_1A_write(int num_rb_dl, int num_bstx_ant, bstx_ctrl_pdsch *ptr_ctrl_pdsch, int16 *outbuf)
{
	int i;
	int cur_idx;
	int num_bits_realloc_type2_dl;
	
	// function body
	if(num_rb_dl == 6)			num_bits_realloc_type2_dl = 5;
	else if(num_rb_dl == 15)	num_bits_realloc_type2_dl = 7;
	else if(num_rb_dl == 25)	num_bits_realloc_type2_dl = 9;
	else if(num_rb_dl == 50)	num_bits_realloc_type2_dl = 11;
	else if(num_rb_dl == 75)	num_bits_realloc_type2_dl = 12;
	else if(num_rb_dl == 100)	num_bits_realloc_type2_dl = 13;
	else assert(0);
	
	cur_idx = 0;
	
	// 1 bit for DCI 1A
	outbuf[cur_idx] = 1;
	cur_idx++;	

	// DCI 1A used for random access process
	if (ptr_ctrl_pdsch->rnti_type == BSTX_C_RNTI)
	{	// RE allocation type 2, 0:local, 1:distributed
		outbuf[cur_idx] = ptr_ctrl_pdsch->local_distrib_vrb_flg; 
		cur_idx++;		
		
		for (i=0; i<num_bits_realloc_type2_dl; i++)
		{	// RE mapping information, we assume it for 1
			outbuf[cur_idx] = 1;	
			cur_idx++;
		}
		
		dec2bin(ptr_ctrl_pdsch->preamble_idx, 6, &outbuf[cur_idx]);
		cur_idx += 6;
		
		dec2bin(ptr_ctrl_pdsch->prach_mask_idx, 4, &outbuf[cur_idx]);		
		cur_idx += 4;
	}
	else
	{	// re_allocation_type 2: local or distributed
		outbuf[cur_idx] = ptr_ctrl_pdsch->local_distrib_vrb_flg;
		cur_idx++;

		for (i=0; i<num_bits_realloc_type2_dl; i++)
		{
			outbuf[cur_idx] = 1;
			cur_idx++;
		}
		
		// bit18 ~ 22:MCS(QAM and tbs_cw)
		dec2bin(ptr_ctrl_pdsch->mcs_cw[0], 5, &outbuf[cur_idx]); 		
		cur_idx += 5;	
		
		// bit23~26:HARQ_process_number
		dec2bin(ptr_ctrl_pdsch->harqprocess_idx, 4, &outbuf[cur_idx]);		
		cur_idx += 4;		
		
		// bit27: NDI
		outbuf[cur_idx] = ptr_ctrl_pdsch->ndi_cw[0];
		cur_idx++;
		
		// 2bit for RV
		dec2bin(ptr_ctrl_pdsch->rv_cw[0], 2, &outbuf[cur_idx]); 
		cur_idx += 2;
		
		dec2bin(ptr_ctrl_pdsch->tpc_pucch, 2, &outbuf[cur_idx]);
		cur_idx += 2;
		
		dec2bin(ptr_ctrl_pdsch->dai, 2, &outbuf[cur_idx]);		
		cur_idx += 2;
	}
	
	return cur_idx; 
}

int DCI_1C_write(int num_rb_dl, int num_bstx_ant, int enable_num_gap2, bstx_ctrl_pdsch *ptr_ctrl_pdsch, int16 *outbuf)
{
	int i;
	int cur_idx;
	int num_bits_realloc_type01;

	// function body
	cur_idx	= 0;

	if(num_rb_dl == 6)	
	{
		num_bits_realloc_type01 = 3;
	}
	else if(num_rb_dl == 15)
	{
		num_bits_realloc_type01 = 5;
	}
	else if(num_rb_dl == 25)
	{
		num_bits_realloc_type01 = 7;
	}
	else if(num_rb_dl == 50)
	{
		if(enable_num_gap2)
		{
			outbuf[cur_idx] = 1;
			cur_idx++;

			num_bits_realloc_type01 = 8;
		}
		else
		{
			outbuf[cur_idx] = 0;
			cur_idx++;

			num_bits_realloc_type01 = 7;
		}
	}
	else if(num_rb_dl == 75)
	{
		if(enable_num_gap2)
		{
			outbuf[cur_idx] = 1;
			cur_idx++;

			num_bits_realloc_type01 = 9;
		}
		else
		{
			outbuf[cur_idx] = 0;
			cur_idx++;

			num_bits_realloc_type01 = 8;
		}
	}
	else if(num_rb_dl == 100)	
	{
		if(enable_num_gap2)
		{
			outbuf[cur_idx] = 1;
			cur_idx++;

			num_bits_realloc_type01 = 10;
		}
		else
		{
			outbuf[cur_idx] = 0;
			cur_idx++;

			num_bits_realloc_type01 = 9;
		}
	}
	else
	{
		assert(0);
	}
	
	for(i=0; i<num_bits_realloc_type01; i++)
	{
		outbuf[cur_idx] = 1;
		cur_idx++;
	}
	
	dec2bin(ptr_ctrl_pdsch->mcs_cw[0], 5, &outbuf[cur_idx]);
	cur_idx += 5;
	
	return cur_idx;	
}

int DCI_2_write(int num_rb_dl, int num_bstx_ant, bstx_ctrl_pdsch *ptr_ctrl_pdsch, int16 *outbuf)
{
	int i;
	int cur_idx;
	int num_bits_resys_type01;
	int num_bits_realloc_type01;
	int precod_inf_bit_num;
	int P;
		
	int re_alloc_type	= ptr_ctrl_pdsch->re_alloc_type;
	int tpc_pucch		= ptr_ctrl_pdsch->tpc_pucch;
	int dai				= ptr_ctrl_pdsch->dai;
	int harqprocess_idx	= ptr_ctrl_pdsch->harqprocess_idx;
	int transblock_codeword_swap= ptr_ctrl_pdsch->transblock_codeword_swap;
	int precode_idx		= ptr_ctrl_pdsch->precode_idx;

	int *mcs_cw			= ptr_ctrl_pdsch->mcs_cw;
	int *rv_cw			= ptr_ctrl_pdsch->rv_cw;
	int *ndi_cw			= ptr_ctrl_pdsch->ndi_cw;	

	int PRBnum			= NUM_RB_ALLOC_UE;
	
	// function body
	if(num_rb_dl == 6)			
	{num_bits_resys_type01 = 6; P = 1;}
	else if(num_rb_dl == 15)	
	{num_bits_resys_type01 = 8; P = 2;}
	else if(num_rb_dl == 25)	
	{num_bits_resys_type01 = 13; P = 2;}
	else if(num_rb_dl == 50)	
	{num_bits_resys_type01 = 17; P = 3;}
	else if(num_rb_dl == 75)	
	{num_bits_resys_type01 = 19; P = 4;}
	else if(num_rb_dl == 100)	
	{num_bits_resys_type01 = 25; P = 4;}
	else assert(0);

//	double a=1.5;
//	double b=0.3;
//	double c=0;
//	c=a/b;
//	c = c+1;

	num_bits_realloc_type01 = ceil((PRBnum+0.0)/(P+0.0)); // orig
	//num_bits_realloc_type01 = ceil((PRBnum + P-1)/P);

//	if((PRBnum % P) != 0){
//	num_bits_realloc_type01 = PRBnum /P + 1;
//	}else
//	{
//		num_bits_realloc_type01 = PRBnum /P;
//	}
	if(num_bstx_ant == 2)
		precod_inf_bit_num = 3;
	else if(num_bstx_ant == 4)
		precod_inf_bit_num = 6;
	else
		assert(0);
	
	cur_idx = 0;

	if (num_rb_dl > 10)
	{	// resoure element allocation type
		outbuf[cur_idx] = re_alloc_type;	
		cur_idx++;
	}
	
	// resource element allocation assignment
	for(i=0; i<num_bits_resys_type01; i++)
	{
		if (i < num_bits_realloc_type01)
		{
			outbuf[cur_idx] = 1;
		}
		else
		{
			outbuf[cur_idx] = 0;
		}
		cur_idx++;
	}

	// TPC FOR PUCCH
	dec2bin(tpc_pucch, 2, &outbuf[cur_idx]);	
	cur_idx += 2;

	// downlink assignmengt index
    dec2bin(dai, 2, &outbuf[cur_idx]);	
	cur_idx += 2;	
	
	// HARQ process number
	dec2bin(harqprocess_idx, 4, &outbuf[cur_idx]);	
	cur_idx += 4;
	
	outbuf[cur_idx] = transblock_codeword_swap;
	cur_idx++;
	
	dec2bin(mcs_cw[0], 5, &outbuf[cur_idx]);
	cur_idx += 5;
	
	outbuf[cur_idx] = ndi_cw[0];
	cur_idx++;
	
	dec2bin(rv_cw[0], 2, &outbuf[cur_idx]);
	cur_idx += 2;
	
	dec2bin(mcs_cw[1], 5, &outbuf[cur_idx]);
	cur_idx += 5;
	
	outbuf[cur_idx] = ndi_cw[1];
	cur_idx++;
	
	dec2bin(rv_cw[1], 2, &outbuf[cur_idx]);
	cur_idx += 2;
	
	dec2bin(precode_idx, precod_inf_bit_num, &outbuf[cur_idx]);
	cur_idx += precod_inf_bit_num;
	
	return cur_idx; 	
}


int DCI_2A_write(int num_rb_dl, int num_bstx_ant, bstx_ctrl_pdsch *ptr_ctrl_pdsch, int16 *outbuf)
{	
	int i;
	int cur_idx;
	int num_bits_realloc_type01;
	int precod_inf_bit_num;
	
	int *rv_cw	= ptr_ctrl_pdsch->rv_cw;
	int *mcs_cw	= ptr_ctrl_pdsch->mcs_cw;
	int *ndi_cw	= ptr_ctrl_pdsch->ndi_cw;	
	
	// function body
	if(num_rb_dl == 6)			num_bits_realloc_type01 = 6;
	else if(num_rb_dl == 15)	num_bits_realloc_type01 = 8;
	else if(num_rb_dl == 25)	num_bits_realloc_type01 = 13;
	else if(num_rb_dl == 50)	num_bits_realloc_type01 = 17;
	else if(num_rb_dl == 75)	num_bits_realloc_type01 = 19;
	else if(num_rb_dl == 100)	num_bits_realloc_type01 = 25;
	else assert(0);

	if(num_bstx_ant == 2)
		precod_inf_bit_num = 0; // 20141128 linjiangnan
	else if(num_bstx_ant == 4)
		precod_inf_bit_num = 2; // 20141128 linjiangnan
	else
		assert(0);
	
	cur_idx = 0;

	if (num_rb_dl > 10)
	{
		outbuf[cur_idx] = ptr_ctrl_pdsch->re_alloc_type;
		cur_idx++;
	}	

	// resource element allocation assignment
	for(i=0; i<num_bits_realloc_type01; i++)
	{
		outbuf[cur_idx] = 1;
		cur_idx++;
	}
	
	// TPC FOR PUCCH
	dec2bin(ptr_ctrl_pdsch->tpc_pucch, 2, &outbuf[cur_idx]);
	cur_idx += 2;	

	// downlink assignmengt index
    dec2bin(ptr_ctrl_pdsch->dai, 2, &outbuf[cur_idx]);
	cur_idx += 2;	

	//  HARQ process number
	dec2bin(ptr_ctrl_pdsch->harqprocess_idx, 4, &outbuf[cur_idx]);
	cur_idx += 4;
	
	outbuf[cur_idx] = ptr_ctrl_pdsch->transblock_codeword_swap;
	cur_idx++;	
	
	dec2bin(mcs_cw[0], 5, &outbuf[cur_idx]);
	cur_idx += 5;
	
	outbuf[cur_idx] = ndi_cw[0];
	cur_idx++;
	
	dec2bin(rv_cw[0], 2, &outbuf[cur_idx]);
	cur_idx += 2;
	
	dec2bin(mcs_cw[1], 5, &outbuf[cur_idx]);
	cur_idx += 5;
	
	outbuf[cur_idx] = ndi_cw[1];
	cur_idx++;
	
	dec2bin(rv_cw[1], 2, &outbuf[cur_idx]);
	cur_idx += 2;

	dec2bin(ptr_ctrl_pdsch->precode_idx, precod_inf_bit_num, &outbuf[cur_idx]);
	cur_idx += precod_inf_bit_num;	
	
	return cur_idx; 	
}

int dec2bin(int dec, int inlen, int16 *outbuf)
{
	int i;

	for(i=0; i<inlen; i++)
	{
        outbuf[i] = (dec >> (inlen-1-i)) & 1 ;
	}		
	
	return 0;
}
