#include "uerx_define.h"
#include "uerx_deremap.h"
#include "uerx_sharefunc.h"


int deremap_pdcch(cfloat **inbuf, int num_rb_dl, int num_grp_phich, int mi_phich, int mux_mode
								 , int cp_cfg, int subfrm_idx, int num_ofdm_pdcch, int num_id_cell, int num_bstx_ant
								 , int duration_phich, float gain_pdcch_RA, float gain_pdcch_RB, cfloat *outbuf)
{
	int i;
	int k0, k1, k2, k3;
	int mm;	
	int li;
	int ni;
	int outlen;
	int is_exist_ofdm1;
	int is_exist_ofdm2;
	int is_exist_ofdm1_not4ant;
	int rgtab_ctrl[3][300];
	int idx_rg_pcfich[4];
	int num_map_unit_phich;
	int num_regrps_each_ofdm[3];

	// function body	
	for (i=0; i<3*num_rb_dl; i++) // clear the alloc table of resource group
	{
		rgtab_ctrl[0][i] = 0;
		rgtab_ctrl[1][i] = 0;
		rgtab_ctrl[2][i] = 0;
	}

	// PCFICH table
	k0  = (num_id_cell+num_rb_dl*0/2) % (2*num_rb_dl);    
    k1  = (num_id_cell+num_rb_dl*1/2) % (2*num_rb_dl);
	k2  = (num_id_cell+num_rb_dl*2/2) % (2*num_rb_dl);
	k3  = (num_id_cell+num_rb_dl*3/2) % (2*num_rb_dl);

	rgtab_ctrl[0][k0] = UERX_INDICATION_PCFICH;
	rgtab_ctrl[0][k1] = UERX_INDICATION_PCFICH;
	rgtab_ctrl[0][k2] = UERX_INDICATION_PCFICH;
	rgtab_ctrl[0][k3] = UERX_INDICATION_PCFICH;

	if (k1 < k0)
	{
		idx_rg_pcfich[0] = k1;
		idx_rg_pcfich[1] = k2;
		idx_rg_pcfich[2] = k3;
		idx_rg_pcfich[3] = k0;
	}
	else if (k2 < k1)
	{
		idx_rg_pcfich[0] = k2;
		idx_rg_pcfich[1] = k3;
		idx_rg_pcfich[2] = k0;
		idx_rg_pcfich[3] = k1;
	}
	else if (k3 < k2)
	{
		idx_rg_pcfich[0] = k3;
		idx_rg_pcfich[1] = k0;
		idx_rg_pcfich[2] = k1;
		idx_rg_pcfich[3] = k2;
	}
	else if (k0 < k3)
	{
		idx_rg_pcfich[0] = k0;
		idx_rg_pcfich[1] = k1;
		idx_rg_pcfich[2] = k2;
		idx_rg_pcfich[3] = k3;
	}

	// PHICH TABLE
	num_regrps_each_ofdm[0] = 2*num_rb_dl - 4;
	
	if(num_bstx_ant == 4)
		num_regrps_each_ofdm[1] = 2*num_rb_dl;
	else
		num_regrps_each_ofdm[1] = 3*num_rb_dl;
	
	num_regrps_each_ofdm[2] = 3*num_rb_dl;

	if(cp_cfg == UERX_NORMAL_CP)	
	{
		if(mux_mode == UERX_TDD_MUXMODE)			num_map_unit_phich = mi_phich * num_grp_phich;
		else if(mux_mode == UERX_FDD_MUXMODE)	num_map_unit_phich = num_grp_phich;
		else								assert(0);
	}
	else
	{
		if(mux_mode == UERX_TDD_MUXMODE)			num_map_unit_phich = mi_phich * num_grp_phich / 2;
		else if(mux_mode == UERX_FDD_MUXMODE)	num_map_unit_phich = num_grp_phich / 2;
		else								assert(0);
	}

	for(mm=0; mm<num_map_unit_phich; mm++)
	{
		for(i=0; i<3; i++)
		{
			if(duration_phich==0)					li = 0;
            else if(subfrm_idx==1 || subfrm_idx==6)	li = ((mm/2+i+1)%2);
            else									li = i;

			if(duration_phich==1 && (subfrm_idx == 1 || subfrm_idx == 6))
				ni = ((num_id_cell*num_regrps_each_ofdm[li]/num_regrps_each_ofdm[1])+mm+(i*num_regrps_each_ofdm[li]/3))%num_regrps_each_ofdm[li];
			else
				ni = ((num_id_cell*num_regrps_each_ofdm[li]/num_regrps_each_ofdm[0])+mm+(i*num_regrps_each_ofdm[li]/3))%num_regrps_each_ofdm[li];

			if(li == 0)
			{
				if(ni >= idx_rg_pcfich[3]-3)		ni += 4;
				else if(ni >= idx_rg_pcfich[2]-2)	ni += 3;
				else if(ni >= idx_rg_pcfich[1]-1)	ni += 2;
				else if(ni >= idx_rg_pcfich[0]-0)	ni += 1;
			}

			rgtab_ctrl[li][ni] = UERX_INDICATION_PHICH;
		}
	}

	// DeREmap for PDCCH
	is_exist_ofdm1         = num_ofdm_pdcch > 1;
	is_exist_ofdm1_not4ant = num_ofdm_pdcch > 1 && num_bstx_ant!=4;
	is_exist_ofdm2         = num_ofdm_pdcch > 2;
	
	k0 = k1 = k2 = 0;
	outlen = 0;

	for (i=0; i<num_rb_dl; i++)
	{
		if (rgtab_ctrl[0][k0++]==0)
		{
			outbuf[outlen+0] = cfloatmultiplyfloat(inbuf[0][0+8*i],gain_pdcch_RB);
			outbuf[outlen+1] = cfloatmultiplyfloat(inbuf[0][1+8*i],gain_pdcch_RB);
			outbuf[outlen+2] = cfloatmultiplyfloat(inbuf[0][2+8*i],gain_pdcch_RB);
			outbuf[outlen+3] = cfloatmultiplyfloat(inbuf[0][3+8*i],gain_pdcch_RB);
			
			outlen += 4;
		}
		
		if (is_exist_ofdm1 && rgtab_ctrl[1][k1++]==0)
		{
			outbuf[outlen+0] = cfloatmultiplyfloat(inbuf[1][0+12*i],gain_pdcch_RA);
			outbuf[outlen+1] = cfloatmultiplyfloat(inbuf[1][1+12*i],gain_pdcch_RA);
			outbuf[outlen+2] = cfloatmultiplyfloat(inbuf[1][2+12*i],gain_pdcch_RA);
			outbuf[outlen+3] = cfloatmultiplyfloat(inbuf[1][3+12*i],gain_pdcch_RA);
			
			outlen += 4;
		}
		
		if (is_exist_ofdm2 && rgtab_ctrl[2][k2++]==0)
		{
			outbuf[outlen+0] = cfloatmultiplyfloat(inbuf[2][0+12*i],gain_pdcch_RA);
			outbuf[outlen+1] = cfloatmultiplyfloat(inbuf[2][0+12*i],gain_pdcch_RA);
			outbuf[outlen+2] = cfloatmultiplyfloat(inbuf[2][0+12*i],gain_pdcch_RA);
			outbuf[outlen+3] = cfloatmultiplyfloat(inbuf[2][0+12*i],gain_pdcch_RA);
			
			outlen += 4;
		}

		if (is_exist_ofdm1_not4ant && rgtab_ctrl[1][k1++]==0)
		{
			outbuf[outlen+0] = cfloatmultiplyfloat(inbuf[1][4+12*i],gain_pdcch_RA);
			outbuf[outlen+1] = cfloatmultiplyfloat(inbuf[1][5+12*i],gain_pdcch_RA);
			outbuf[outlen+2] = cfloatmultiplyfloat(inbuf[1][6+12*i],gain_pdcch_RA);
			outbuf[outlen+3] = cfloatmultiplyfloat(inbuf[1][7+12*i],gain_pdcch_RA);
			
			outlen += 4;
		}

		if (is_exist_ofdm2 && rgtab_ctrl[2][k2++]==0)
		{
			outbuf[outlen+0] = cfloatmultiplyfloat(inbuf[2][4+12*i],gain_pdcch_RA);
			outbuf[outlen+1] = cfloatmultiplyfloat(inbuf[2][5+12*i],gain_pdcch_RA);
			outbuf[outlen+2] = cfloatmultiplyfloat(inbuf[2][6+12*i],gain_pdcch_RA);
			outbuf[outlen+3] = cfloatmultiplyfloat(inbuf[2][7+12*i],gain_pdcch_RA);
			
			outlen += 4;
		}
		
		if (rgtab_ctrl[0][k0++]==0)
		{
			outbuf[outlen+0] = cfloatmultiplyfloat(inbuf[0][4+8*i],gain_pdcch_RB);
			outbuf[outlen+1] = cfloatmultiplyfloat(inbuf[0][5+8*i],gain_pdcch_RB);
			outbuf[outlen+2] = cfloatmultiplyfloat(inbuf[0][6+8*i],gain_pdcch_RB);
			outbuf[outlen+3] = cfloatmultiplyfloat(inbuf[0][7+8*i],gain_pdcch_RB);
			
			outlen += 4;
		}
		
		if (is_exist_ofdm1 && rgtab_ctrl[1][k1++]==0)
		{
			outbuf[outlen+0] = cfloatmultiplyfloat(inbuf[1][8+12*i],gain_pdcch_RA);
			outbuf[outlen+1] = cfloatmultiplyfloat(inbuf[1][9+12*i],gain_pdcch_RA);
			outbuf[outlen+2] = cfloatmultiplyfloat(inbuf[1][10+12*i],gain_pdcch_RA);
			outbuf[outlen+3] = cfloatmultiplyfloat(inbuf[1][11+12*i],gain_pdcch_RA);
			
			outlen += 4;
		}	
		
		if (is_exist_ofdm2 && rgtab_ctrl[2][k2++]==0)
		{
			outbuf[outlen+0] = cfloatmultiplyfloat(inbuf[2][8+12*i],gain_pdcch_RA);
			outbuf[outlen+1] = cfloatmultiplyfloat(inbuf[2][9+12*i],gain_pdcch_RA);
			outbuf[outlen+2] = cfloatmultiplyfloat(inbuf[2][10+12*i],gain_pdcch_RA);
			outbuf[outlen+3] = cfloatmultiplyfloat(inbuf[2][11+12*i],gain_pdcch_RA);
			
			outlen += 4;
		}
	} // end for (i=0; i<num_rb_dl; i++)

	
	return outlen;
}

int deremap_phich(cfloat **inbuf, int num_rb_dl, int subfrm_idx, int num_id_cell
								, int num_bstx_ant, int duration_phich, int cp_cfg, int idx_group_phich
								, float gain_phich_RA, float gain_phich_RB, cfloat *outbuf)
{
	int i;
	int k0, k1, k2, k3;
	int mm;
	int li;
	int ni;
	int outlen;
	int idx_rg_pcfich[4];
	int num_regrps_each_ofdm[3];

	// PCFICH table
	k0  = (num_id_cell+num_rb_dl*0/2) % (2*num_rb_dl);    
    k1  = (num_id_cell+num_rb_dl*1/2) % (2*num_rb_dl);
	k2  = (num_id_cell+num_rb_dl*2/2) % (2*num_rb_dl);
	k3  = (num_id_cell+num_rb_dl*3/2) % (2*num_rb_dl);

	if (k1 < k0)
	{
		idx_rg_pcfich[0] = k1;
		idx_rg_pcfich[1] = k2;
		idx_rg_pcfich[2] = k3;
		idx_rg_pcfich[3] = k0;
	}
	else if (k2 < k1)
	{
		idx_rg_pcfich[0] = k2;
		idx_rg_pcfich[1] = k3;
		idx_rg_pcfich[2] = k0;
		idx_rg_pcfich[3] = k1;
	}
	else if (k3 < k2)
	{
		idx_rg_pcfich[0] = k3;
		idx_rg_pcfich[1] = k0;
		idx_rg_pcfich[2] = k1;
		idx_rg_pcfich[3] = k2;
	}
	else if (k0 < k3)
	{
		idx_rg_pcfich[0] = k0;
		idx_rg_pcfich[1] = k1;
		idx_rg_pcfich[2] = k2;
		idx_rg_pcfich[3] = k3;
	}

	if(cp_cfg == 0)		mm = idx_group_phich;
	else				mm = idx_group_phich/2;

	// PHICH deremap
	num_regrps_each_ofdm[0] = 2*num_rb_dl - 4;
	
	if(num_bstx_ant == 4)
		num_regrps_each_ofdm[1] = 2*num_rb_dl;
	else
		num_regrps_each_ofdm[1] = 3*num_rb_dl;
	
	num_regrps_each_ofdm[2] = 3*num_rb_dl;

	outlen = 0;

	for(i=0; i<3; i++)
	{
		if(duration_phich==0)					li = 0;
        else if(subfrm_idx==1 || subfrm_idx==6)	li = ((mm/2+i+1)%2);
        else									li = i;

		if(duration_phich==1 && (subfrm_idx == 1 || subfrm_idx == 6))
			ni = ((num_id_cell*num_regrps_each_ofdm[li]/num_regrps_each_ofdm[1])+mm+(i*num_regrps_each_ofdm[li]/3))%num_regrps_each_ofdm[li];
		else
			ni = ((num_id_cell*num_regrps_each_ofdm[li]/num_regrps_each_ofdm[0])+mm+(i*num_regrps_each_ofdm[li]/3))%num_regrps_each_ofdm[li];

		if(li == 0)
		{
			if(ni >= idx_rg_pcfich[3]-3)		ni += 4;
			else if(ni >= idx_rg_pcfich[2]-2)	ni += 3;
			else if(ni >= idx_rg_pcfich[1]-1)	ni += 2;
			else if(ni >= idx_rg_pcfich[0]-0)	ni += 1;
		}

		if(li==0 || (li==1 && num_bstx_ant==4)) // with RS
		{
			outbuf[outlen+0] = cfloatmultiplyfloat(inbuf[li][0+4*ni],gain_phich_RB);
			outbuf[outlen+1] = cfloatmultiplyfloat(inbuf[li][1+4*ni],gain_phich_RB);
			outbuf[outlen+2] = cfloatmultiplyfloat(inbuf[li][2+4*ni],gain_phich_RB);
			outbuf[outlen+3] = cfloatmultiplyfloat(inbuf[li][3+4*ni],gain_phich_RB);
			outlen += 4;
		}
		else									// without RS
		{
			outbuf[outlen+0] = cfloatmultiplyfloat(inbuf[li][0+4*ni],gain_phich_RA);
			outbuf[outlen+1] = cfloatmultiplyfloat(inbuf[li][1+4*ni],gain_phich_RA);
			outbuf[outlen+2] = cfloatmultiplyfloat(inbuf[li][2+4*ni],gain_phich_RA);
			outbuf[outlen+3] = cfloatmultiplyfloat(inbuf[li][3+4*ni],gain_phich_RA);
			outlen += 4;
		}
	}
	
	return outlen;
}

int deremap_pcfich(cfloat *inbuf, int num_rb_dl, int num_id_cell, float gain_pcfich_RB, cfloat *outbuf)
{
	int i, k;
	
	// get pcfich symbol
	for(i=0; i<4; i++)
	{
		k = (num_id_cell + i*num_rb_dl/2) % (2*num_rb_dl);

		outbuf[4*i+0] = cfloatmultiplyfloat(inbuf[4*k+0],gain_pcfich_RB);
		outbuf[4*i+1] = cfloatmultiplyfloat(inbuf[4*k+1],gain_pcfich_RB);
		outbuf[4*i+2] = cfloatmultiplyfloat(inbuf[4*k+2],gain_pcfich_RB);
		outbuf[4*i+3] = cfloatmultiplyfloat(inbuf[4*k+3],gain_pcfich_RB);
	}
	
	return 16;
}


int deremap_pbch(cfloat **inbuf, int num_row, int num_rb_dl, int subfrm_idx, int ofdm_idx
							   , int num_id_cell, int num_bstx_ant, int cp_cfg, float gain_pbch_RA
							   , float gain_pbch_RB, cfloat **outbuf)
{
	int i, r;
	int N_data;
	int outlen;
	int is_withRS;
	int rs_start;
	int num_ofdm_slot;

	float gain_pbch;
	
	// function body
	assert(subfrm_idx == 0);

	N_data = 12*num_rb_dl;

	if(cp_cfg == UERX_NORMAL_CP)
		num_ofdm_slot = 7;
	else
		num_ofdm_slot = 6;
	
	// PBCH deremap	
	if (ofdm_idx==num_ofdm_slot || (ofdm_idx==num_ofdm_slot+1 && num_bstx_ant==4))
		gain_pbch = gain_pbch_RB;
	else
		gain_pbch = gain_pbch_RA;

	if (ofdm_idx==num_ofdm_slot || ofdm_idx==num_ofdm_slot+1)
		is_withRS = 1;
	else
		is_withRS = 0;
	
	// PBCH deremap
	if (is_withRS)	// with RS
	{
		rs_start = num_id_cell%3;		
		
		for(r=0; r<num_row; r++)
		{	
			outlen = 0;
			
			for (i=(N_data/2-36); i<(N_data/2+36); i++)		           
			{
				if ((i%3) != rs_start)
				{					
					outbuf[r][outlen] = cfloatmultiplyfloat(inbuf[r][i],gain_pbch);
					outlen++;
				}				
			}
		}
	} 
	else			// without RS
	{
		for(r=0; r<num_row; r++)
		{
			outlen = 0;
			
			for (i=(N_data/2-36); i<(N_data/2+36); i++)		           
			{			
				outbuf[r][outlen] = cfloatmultiplyfloat(inbuf[r][i],gain_pbch);
				outlen++;
			}			
		}		
	}
	
	return outlen;
}

int deremap_pdsch(cfloat **inbuf, int num_row, int num_rb_dl, int16 rb_alloc_pdsch[2][110]
								, int subfrm_idx, int ofdm_idx, int num_id_cell, int num_bstx_ant, int cp_cfg
								, float gain_pdsch_RA, float gain_pdsch_RB, cfloat **outbuf)
{
	int i;
	int r;
	int ns_mod2; 
	int outlen;
	int rs_start;
	int is_withRS;
	int is_withRS_siso;
	int is_withRS_mimo;
	int is_with_ss_pbch;
	int num_ofdm_slot;
	int k0,k1,k2,k3,k4;
	
	// function body
	if(cp_cfg == UERX_NORMAL_CP)		
		num_ofdm_slot = 7;
	else
		num_ofdm_slot = 6;

	ns_mod2 = ofdm_idx / num_ofdm_slot;

	is_withRS = (ofdm_idx == 0)
			 || (ofdm_idx == 1 && num_bstx_ant == 4)
			 ||	(ofdm_idx == num_ofdm_slot-3 )
			 || (ofdm_idx == num_ofdm_slot)
			 || (ofdm_idx == num_ofdm_slot+1 && num_bstx_ant == 4)
			 ||	(ofdm_idx == 2*num_ofdm_slot-3);

	is_with_ss_pbch = ((subfrm_idx==1 || subfrm_idx==6) && ofdm_idx==2)							// with PSS
				   || ((subfrm_idx==0 || subfrm_idx==5) && ofdm_idx==13)						// with SSS
				   || (subfrm_idx==0 && ofdm_idx>=num_ofdm_slot && ofdm_idx<=num_ofdm_slot+3);	// with PBCH

	is_withRS_siso = is_withRS && num_bstx_ant==1;
	is_withRS_mimo = is_withRS && num_bstx_ant!=1;

	if(is_withRS_siso)
	{
		if (ofdm_idx==0 || ofdm_idx==num_ofdm_slot )	
			rs_start = (num_id_cell+0)%6;
		else
			rs_start = (num_id_cell+3)%6;
		
		if (rs_start == 0)
		{
			k0 = 1;
			k1 = 2;
			k2 = 3;
			k3 = 4;
			k4 = 5;
		}
		else if (rs_start == 1)
		{
			k0 = 0;
			k1 = 2;
			k2 = 3;
			k3 = 4;
			k4 = 5;
		}
		else if (rs_start == 2)
		{
			k0 = 0;
			k1 = 1;
			k2 = 3;
			k3 = 4;
			k4 = 5;
		}
		else if (rs_start == 3)
		{
			k0 = 0;
			k1 = 1;
			k2 = 2;
			k3 = 4;
			k4 = 5;
		}
		else if (rs_start == 4)
		{
			k0 = 0;
			k1 = 1;
			k2 = 2;
			k3 = 3;
			k4 = 5;
		}
		else if (rs_start == 5)
		{
			k0 = 0;
			k1 = 1;
			k2 = 2;
			k3 = 3;
			k4 = 4;
		}
	}
	else if(is_withRS_mimo)
	{
		rs_start = num_id_cell%3;

		if (rs_start == 0)
		{
			k0 = 1;
			k1 = 2;
			k2 = 4;
			k3 = 5;
		}
		else if (rs_start == 1)
		{
			k0 = 0;
			k1 = 2;
			k2 = 3;
			k3 = 5;
		}
		else if (rs_start == 2)
		{
			k0 = 0;
			k1 = 1;
			k2 = 3;
			k3 = 4;
		}
	}

	for(r=0; r<num_row; r++)
	{
		outlen = 0;

		for (i=0; i<2*num_rb_dl; i++)	// resource element group as a unit
		{
			if (rb_alloc_pdsch[ns_mod2][i/2] == 0)		
				continue;				// not alloc for this ue
			
			if (is_with_ss_pbch && i>=num_rb_dl-6 && i<num_rb_dl+6)
				continue;				// not used for pdsch

			if (is_withRS_siso)			// with RS, single transmit antenna
			{				
				outbuf[r][outlen+0] = cfloatmultiplyfloat(inbuf[r][6*i+k0],gain_pdsch_RB);
				outbuf[r][outlen+1] = cfloatmultiplyfloat(inbuf[r][6*i+k1],gain_pdsch_RB);
				outbuf[r][outlen+2] = cfloatmultiplyfloat(inbuf[r][6*i+k2],gain_pdsch_RB);
				outbuf[r][outlen+3] = cfloatmultiplyfloat(inbuf[r][6*i+k3],gain_pdsch_RB);
				outbuf[r][outlen+4] = cfloatmultiplyfloat(inbuf[r][6*i+k4],gain_pdsch_RB);
				outlen += 5;
			}
			else if(is_withRS_mimo)		// with RS, multi transmit antenna
			{	
				outbuf[r][outlen+0] = cfloatmultiplyfloat(inbuf[r][6*i+k0],gain_pdsch_RB);
				outbuf[r][outlen+1] = cfloatmultiplyfloat(inbuf[r][6*i+k1],gain_pdsch_RB);
				outbuf[r][outlen+2] = cfloatmultiplyfloat(inbuf[r][6*i+k2],gain_pdsch_RB);
				outbuf[r][outlen+3] = cfloatmultiplyfloat(inbuf[r][6*i+k3],gain_pdsch_RB);
				outlen += 4;
			}
			else						// without RS
			{
				outbuf[r][outlen+0] = cfloatmultiplyfloat(inbuf[r][6*i+0],gain_pdsch_RA);
				outbuf[r][outlen+1] = cfloatmultiplyfloat(inbuf[r][6*i+1],gain_pdsch_RA);
				outbuf[r][outlen+2] = cfloatmultiplyfloat(inbuf[r][6*i+2],gain_pdsch_RA);
				outbuf[r][outlen+3] = cfloatmultiplyfloat(inbuf[r][6*i+3],gain_pdsch_RA);
				outbuf[r][outlen+4] = cfloatmultiplyfloat(inbuf[r][6*i+4],gain_pdsch_RA);
				outbuf[r][outlen+5] = cfloatmultiplyfloat(inbuf[r][6*i+5],gain_pdsch_RA);
				outlen += 6;
			}
		} // for (i=0; i<2*num_rb_dl; i++)
	} // for(r=0; r<num_row; r++)

	return outlen;
}

int rmvRS_ctrlch(cfloat **inbuf, int num_row, int num_rb_dl, int ofdm_idx, int num_id_cell
							 , int num_bstx_ant, cfloat **outbuf)
{
	int i;
	int r;
	int outlen;
	int rs_start;	
	int is_withRS;
	int k0,k1,k2,k3,k4,k5,k6,k7;

	is_withRS = ofdm_idx==0 || (num_bstx_ant==4 && ofdm_idx==1);

	if(is_withRS)
	{
		rs_start = num_id_cell%3;

		if (rs_start == 0)
		{
			k0 = 1;
			k1 = 2;
			k2 = 4;
			k3 = 5;
			k4 = 7;
			k5 = 8;
			k6 = 10;
			k7 = 11;
		}
		else if (rs_start == 1)
		{
			k0 = 0;
			k1 = 2;
			k2 = 3;
			k3 = 5;
			k4 = 6;
			k5 = 8;
			k6 = 9;
			k7 = 11;
		}
		else if (rs_start == 2)
		{
			k0 = 0;
			k1 = 1;
			k2 = 3;
			k3 = 4;
			k4 = 6;
			k5 = 7;
			k6 = 9;
			k7 = 10;
		}
	}	

	if (is_withRS)
	{
		for(r=0; r<num_row; r++)
		{
			outlen = 0;
			
			for (i=0; i<num_rb_dl; i++)
			{		
				outbuf[r][outlen+0] = inbuf[r][12*i+k0];
				outbuf[r][outlen+1] = inbuf[r][12*i+k1];
				outbuf[r][outlen+2] = inbuf[r][12*i+k2];
				outbuf[r][outlen+3] = inbuf[r][12*i+k3];
				outbuf[r][outlen+4] = inbuf[r][12*i+k4];
				outbuf[r][outlen+5] = inbuf[r][12*i+k5];
				outbuf[r][outlen+6] = inbuf[r][12*i+k6];
				outbuf[r][outlen+7] = inbuf[r][12*i+k7];
				
				outlen += 8;
			}			
		}
	}
	else
	{
		for(r=0; r<num_row; r++)
		{
			for (i=0; i<12*num_rb_dl; i++)
			{			
				outbuf[r][i] = inbuf[r][i];
			}
		}
		
		outlen = 12*num_rb_dl;
	}

	return outlen;
}

int get_subcarrier(cfloat **inbuf, int size_fft, int num_subcarrier, int num_uerx_ant, cfloat **outbuf)
{
	int k, m, p;

	// 2048->1200, 2048->900, 1024->600, 512->300, 256->180, 128->72
	m = size_fft - num_subcarrier/2;

	for(p=0; p<num_uerx_ant; p++)
	{
		for(k=0; k<num_subcarrier/2 ; k++) 
		{	// outY<16,11>
			outbuf[p][k] = inbuf[p][k+m];
			outbuf[p][k+num_subcarrier/2] = inbuf[p][k+1];
		}
	}

	return num_subcarrier;
}
