#include "foo.h"
#include "bstx_remap.h"
#include "bstx_sharefunc.h"

#define DUMP_REMAP 0

int remap_pss(cfloat *inbuf, int inlen, int subfrm_idx, int num_rb_dl, float gain_pss_RA, cfloat **outbuf)
{
	int i, j;
	int num_offset;

	num_offset = num_rb_dl*12/2;

	if(subfrm_idx == 1 || subfrm_idx == 6)
	{
		for(i=num_offset-36; i<num_offset-31; i++)
		{
			outbuf[0][2*1320+i].re = 0;
			outbuf[0][2*1320+i].im = 0;
		}

		for(i=num_offset-31,j=0; i<num_offset+31; i++,j++)
		{
			outbuf[0][2*1320+i].re = inbuf[j].re * gain_pss_RA;
			outbuf[0][2*1320+i].im = inbuf[j].im * gain_pss_RA;
		}		

		for(i=num_offset+31; i<num_offset+36; i++)
		{
			outbuf[0][2*1320+i].re = 0;
			outbuf[0][2*1320+i].im = 0;
		}
	}

	return 62;
}

int remap_sss(cfloat *inbuf, int inlen, int subfrm_idx, int num_rb_dl, int cp_cfg, float gain_sss_RA, cfloat **outbuf)
{
	int i, j;
	int num_offset;
	int idx_ofdm;

	num_offset = num_rb_dl*12/2;

	if(cp_cfg == BSTX_NORMAL_CP)		idx_ofdm = 13;
	else						idx_ofdm = 11;

	if(subfrm_idx == 0 || subfrm_idx == 5)
	{
		for(i=num_offset-36; i<num_offset-31; i++)
		{
			outbuf[0][idx_ofdm*1320+i].re = 0;
			outbuf[0][idx_ofdm*1320+i].im = 0;
		}

		for(i=num_offset-31,j=0; i<num_offset+31; i++,j++)
		{
			outbuf[0][idx_ofdm*1320+i].re = inbuf[j].re * gain_sss_RA;
			outbuf[0][idx_ofdm*1320+i].im = inbuf[j].im * gain_sss_RA;
		}		

		for(i=num_offset+31; i<num_offset+36; i++)
		{
			outbuf[0][idx_ofdm*1320+i].re = 0;
			outbuf[0][idx_ofdm*1320+i].im = 0;
		}
	}

	return 62;
}

int remap_rs(cfloat *inbuf, int num_rb_dl, int subfrm_idx, int num_bstx_ant, int num_ID_cell
						 , int cp_cfg, int spcal_subfrm_cfg, int uldl_switch_cfg, cfloat **outbuf)
{
	int i;
	int rs_start0;
	int rs_start3;
	int num_symb_DL;
	int num_ofdm_dl;
	int base_addr0_inbuf;
	int base_addr1_inbuf;
	int base_addr2_inbuf;
	int base_addr3_inbuf;
	int base_addr4_inbuf;
	int base_addr5_inbuf;

	if(cp_cfg == BSTX_NORMAL_CP)		
		num_symb_DL = 7;
	else
		num_symb_DL = 6;

	if(bstx_isSpecialSubframe(subfrm_idx, uldl_switch_cfg))
	{
		num_ofdm_dl = getLen_specsubfram(cp_cfg, spcal_subfrm_cfg);
	}
	else if(bstx_isDL(subfrm_idx,uldl_switch_cfg))
	{
		num_ofdm_dl = 2 * num_symb_DL;
	}
	else
	{
		num_ofdm_dl = 0;
	}

	rs_start0 = (0+num_ID_cell)%6;
	rs_start3 = (3+num_ID_cell)%6;

	base_addr0_inbuf = 110-num_rb_dl + 220*0;
	base_addr1_inbuf = 110-num_rb_dl + 220*1;
	base_addr2_inbuf = 110-num_rb_dl + 220*2;
	base_addr3_inbuf = 110-num_rb_dl + 220*3;
	base_addr4_inbuf = 110-num_rb_dl + 220*4;
	base_addr5_inbuf = 110-num_rb_dl + 220*5;

	if(num_bstx_ant==1 || num_bstx_ant==2 || num_bstx_ant==4) // RS for antenna 0
	{
		if(num_ofdm_dl > 0)
		{
			for(i=0; i<2*num_rb_dl; i++)
			{
				outbuf[0][0*1320 + 6*i+rs_start0] = inbuf[i+base_addr0_inbuf];
			}
		}

		if(num_ofdm_dl > num_symb_DL-3)
		{
			for(i=0; i<2*num_rb_dl; i++)
			{
				outbuf[0][(num_symb_DL-3)*1320 + 6*i+rs_start3] = inbuf[i+base_addr2_inbuf];
			}
		}

		if(num_ofdm_dl > num_symb_DL)
		{
			for(i=0; i<2*num_rb_dl; i++)
			{
				outbuf[0][num_symb_DL*1320 + 6*i+rs_start0] = inbuf[i+base_addr3_inbuf];
			}
		}

		if(num_ofdm_dl > 2*num_symb_DL-3)
		{
			for(i=0; i<2*num_rb_dl; i++)
			{
				outbuf[0][(2*num_symb_DL-3)*1320 + 6*i+rs_start3] = inbuf[i+base_addr5_inbuf];
			}
		}
	} // end if(num_bstx_ant > 0)
	
	if(num_bstx_ant==2 || num_bstx_ant==4) // RS for antenna 1
	{
		if(num_ofdm_dl > 0)
		{
			for(i=0; i<2*num_rb_dl; i++)
			{
				outbuf[1][0*1320 + 6*i+rs_start3] = inbuf[i+base_addr0_inbuf];
			}
		}

		if(num_ofdm_dl > num_symb_DL-3)
		{
			for(i=0; i<2*num_rb_dl; i++)
			{
				outbuf[1][(num_symb_DL-3)*1320 + 6*i+rs_start0] = inbuf[i+base_addr2_inbuf];
			}
		}

		if(num_ofdm_dl > num_symb_DL)
		{
			for(i=0; i<2*num_rb_dl; i++)
			{
				outbuf[1][num_symb_DL*1320 + 6*i+rs_start3] = inbuf[i+base_addr3_inbuf];
			}
		}

		if(num_ofdm_dl > 2*num_symb_DL-3)
		{
			for(i=0; i<2*num_rb_dl; i++)
			{
				outbuf[1][(2*num_symb_DL-3)*1320 + 6*i+rs_start0] = inbuf[i+base_addr5_inbuf];
			}
		}
	} // end if(num_bstx_ant > 1)
	
	if(num_bstx_ant==4) // RS for antenna 2
	{
		if(num_ofdm_dl > 1)
		{
			for(i=0; i<2*num_rb_dl; i++)
			{
				outbuf[2][1*1320 + 6*i+rs_start0] = inbuf[i+base_addr1_inbuf];
			}
		}

		if(num_ofdm_dl > num_symb_DL+1)
		{
			for(i=0; i<2*num_rb_dl; i++)
			{
				outbuf[2][(num_rb_dl+1)*1320 + 6*i+rs_start3] = inbuf[i+base_addr4_inbuf];
			}
		}
	} 
	
	if(num_bstx_ant==4)// RS for antenna 3
	{
		if(num_ofdm_dl > 1)
		{
			for(i=0; i<2*num_rb_dl; i++)
			{
				outbuf[3][1*1320 + 6*i+rs_start3] = inbuf[i+base_addr1_inbuf];
			}
		}

		if(num_ofdm_dl > num_symb_DL+1)
		{
			for(i=0; i<2*num_rb_dl; i++)
			{
				outbuf[3][(num_rb_dl+1)*1320 + 6*i+rs_start0] = inbuf[i+base_addr4_inbuf];
			}
		}
	}

	return 0;
}

int remap_pcfich(cfloat **inbuf, int inlen, int num_ID_cell, int num_bstx_ant, int num_rb_dl
							 , float gain_pcfich_RB, cfloat **outbuf)
{
	int i, p;
	int k[4];
	int rs_start;

	assert(inlen == 16);

	k[0]  = (num_ID_cell)				% (2*num_rb_dl);    
    k[1]  = (num_ID_cell+num_rb_dl/2)	% (2*num_rb_dl);
	k[2]  = (num_ID_cell+num_rb_dl)		% (2*num_rb_dl);
	k[3]  = (num_ID_cell+num_rb_dl*3/2)	% (2*num_rb_dl);

	rs_start = num_ID_cell%3;

	if(rs_start == 0)
	{		
		for(i=0; i<4; i++)
		{
			for(p=0; p<num_bstx_ant; p++)
			{
				outbuf[p][0*1320 + 1+6*k[i]] = cfloatmultiplyfloat(inbuf[p][4*i+0],gain_pcfich_RB);
				outbuf[p][0*1320 + 2+6*k[i]] = cfloatmultiplyfloat(inbuf[p][4*i+1],gain_pcfich_RB);
				outbuf[p][0*1320 + 4+6*k[i]] = cfloatmultiplyfloat(inbuf[p][4*i+2],gain_pcfich_RB);
				outbuf[p][0*1320 + 5+6*k[i]] = cfloatmultiplyfloat(inbuf[p][4*i+3],gain_pcfich_RB);
			}
		}
	}
	else if(rs_start == 1)
	{
		for(i=0; i<4; i++)
		{
			for(p=0; p<num_bstx_ant; p++)
			{
				outbuf[p][0*1320 + 0+6*k[i]] = cfloatmultiplyfloat(inbuf[p][4*i+0],gain_pcfich_RB);
				outbuf[p][0*1320 + 2+6*k[i]] = cfloatmultiplyfloat(inbuf[p][4*i+1],gain_pcfich_RB);
				outbuf[p][0*1320 + 3+6*k[i]] = cfloatmultiplyfloat(inbuf[p][4*i+2],gain_pcfich_RB);
				outbuf[p][0*1320 + 5+6*k[i]] = cfloatmultiplyfloat(inbuf[p][4*i+3],gain_pcfich_RB);
			}
		}
	}
	else
	{		
		for(i=0; i<4; i++)
		{
			for(p=0; p<num_bstx_ant; p++)
			{
				outbuf[p][0*1320 + 0+6*k[i]] = cfloatmultiplyfloat(inbuf[p][4*i+0],gain_pcfich_RB);
				outbuf[p][0*1320 + 1+6*k[i]] = cfloatmultiplyfloat(inbuf[p][4*i+1],gain_pcfich_RB);
				outbuf[p][0*1320 + 3+6*k[i]] = cfloatmultiplyfloat(inbuf[p][4*i+2],gain_pcfich_RB);
				outbuf[p][0*1320 + 4+6*k[i]] = cfloatmultiplyfloat(inbuf[p][4*i+3],gain_pcfich_RB);
			}
		}
	}

	return 16;
}


int remap_phich(cfloat **inbuf, int num_rb_dl, int num_grp_phich, int mi_phich, int fdd_tdd_mode
							, int subfrm_idx, int num_ID_cell, int num_bstx_ant, int cp_cfg, int duration_ih_cfg
							, float gain_phich_RA, float gain_phich_RB, cfloat **outbuf)
{
	int i, j, p;
	int k0, k1, k2, k3;			// k0 : k'0
	int mm;
	int idx;
	int regrps[3];
	int lli;                    // lli:l'(i)
	int ni;                     // ni:n(i)
	int rs_start;
	int num_map_unit_phich;
	int regrp_pcfich[4];

// 	assert(num_bstx_ant != 4); // linjiangnan

	// build PCFICH
	k0  = (num_ID_cell)				  % (2*num_rb_dl);
    k1  = (num_ID_cell+num_rb_dl/2)	  % (2*num_rb_dl);
	k2  = (num_ID_cell+num_rb_dl)	  % (2*num_rb_dl);
	k3  = (num_ID_cell+num_rb_dl*3/2) % (2*num_rb_dl);

	if (k1 < k0)
	{
		regrp_pcfich[0] = k1;
		regrp_pcfich[1] = k2;
		regrp_pcfich[2] = k3;
		regrp_pcfich[3] = k0;
	}
	else if (k2 < k1)
	{
		regrp_pcfich[0] = k2;
		regrp_pcfich[1] = k3;
		regrp_pcfich[2] = k0;
		regrp_pcfich[3] = k1;
	}
	else if (k3 < k2)
	{
		regrp_pcfich[0] = k3;
		regrp_pcfich[1] = k0;
		regrp_pcfich[2] = k1;
		regrp_pcfich[3] = k2;
	}
	else if (k0 < k3)
	{
		regrp_pcfich[0] = k0;
		regrp_pcfich[1] = k1;
		regrp_pcfich[2] = k2;
		regrp_pcfich[3] = k3;
	}                                                          
	
	if(cp_cfg == BSTX_NORMAL_CP)	
	{
		if(fdd_tdd_mode == BSTX_TDD_MUXMODE)			num_map_unit_phich = mi_phich * num_grp_phich;
		else if(fdd_tdd_mode == BSTX_FDD_MUXMODE)	num_map_unit_phich = num_grp_phich;
		else								assert(0);
	}
	else
	{
		if(fdd_tdd_mode == BSTX_TDD_MUXMODE)			num_map_unit_phich = mi_phich * num_grp_phich / 2;
		else if(fdd_tdd_mode == BSTX_FDD_MUXMODE)	num_map_unit_phich = num_grp_phich / 2;
		else								assert(0);
	}

	// PHICH deremap
	regrps[0] = 2*num_rb_dl - 4;
	regrps[1] = 3*num_rb_dl;
	regrps[2] = 3*num_rb_dl;

	rs_start = num_ID_cell%3;

	idx = 0;

	for(mm=0; mm<num_map_unit_phich; mm++)
	{
		for(i=0; i<3; i++)
		{
			if(duration_ih_cfg==0)					lli = 0;
			else if(subfrm_idx==1 || subfrm_idx==6)	lli = ((mm/2+i+1)%2);
			else									lli = i;

			if(duration_ih_cfg==1 && (subfrm_idx == 1 || subfrm_idx == 6))
				ni = ((num_ID_cell*regrps[lli]/regrps[1])+mm+(i*regrps[lli]/3))%regrps[lli];
			else
				ni = ((num_ID_cell*regrps[lli]/regrps[0])+mm+(i*regrps[lli]/3))%regrps[lli];

//			printf("unit_idx(m')=%d; quad_idx(i)=%d; ofdm_idx(l')=%d; REG_idx(n_bar)=%d; ",mm, i, lli, ni);// zuohuanhuan


			if(lli == 0)
			{
				for(j=0; j<4; j++)
				{
					if(ni >= regrp_pcfich[j])	ni++;
				}
			}

//			printf("REG_idx_PHICH=%d \n", ni);// zuohuanhuan


			if(    lli == 0 
				|| (lli==1 && num_bstx_ant==4))
			{
				if(rs_start == 0)
				{
					for(p=0; p<num_bstx_ant; p++)
					{
						outbuf[p][lli*1320 + 1+6*ni] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_phich_RB);
						outbuf[p][lli*1320 + 2+6*ni] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_phich_RB);
						outbuf[p][lli*1320 + 4+6*ni] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_phich_RB);
						outbuf[p][lli*1320 + 5+6*ni] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_phich_RB);
					}
				}
				else if(rs_start == 1)
				{
					for(p=0; p<num_bstx_ant; p++)
					{
						outbuf[p][lli*1320 + 0+6*ni] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_phich_RB);
						outbuf[p][lli*1320 + 2+6*ni] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_phich_RB);
						outbuf[p][lli*1320 + 3+6*ni] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_phich_RB);
						outbuf[p][lli*1320 + 5+6*ni] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_phich_RB);
					}
				}
				else
				{
					for(p=0; p<num_bstx_ant; p++)
					{
						outbuf[p][lli*1320 + 0+6*ni] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_phich_RB);
						outbuf[p][lli*1320 + 1+6*ni] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_phich_RB);
						outbuf[p][lli*1320 + 3+6*ni] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_phich_RB);
						outbuf[p][lli*1320 + 4+6*ni] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_phich_RB);
					}
				}
			}
			else
			{
				for(p=0; p<num_bstx_ant; p++)
				{
					outbuf[p][lli*1320 + 0+4*ni] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_phich_RA);
					outbuf[p][lli*1320 + 1+4*ni] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_phich_RA);
					outbuf[p][lli*1320 + 2+4*ni] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_phich_RA);
					outbuf[p][lli*1320 + 3+4*ni] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_phich_RA);
				}
			}
			
			idx += 4;
		}
	}
	
	return idx;
}


int remap_pdcch(cfloat **inbuf, int num_rb_dl, int num_grp_phich, int mi_phich, int fdd_tdd_mode, int cp_cfg
							, int subfrm_idx, int num_ofdm_pdcch, int num_ID_cell, int num_bstx_ant
							, int duration_phich_cfg, float gain_pdcch_RA, float gain_pdcch_RB, cfloat **outbuf)
{
	int i, j, p;
	int k0, k1, k2, k3;      // KK:k'
	int mm;
	int regrps_each_ofdm[3];
	int lli;                     // lli:l'(i)
	int ni;                      // ni:n(i)
	int idx;
	int rs_start;
	int num_map_unit_phich;
	int regrp_tab[3][300];//one rb can be divided into 3 reg,100 rb =300 reg caohuan
	int regrp_pcfich[4];

	for (i=0; i<3*num_rb_dl; i++)
	{
		regrp_tab[0][i] = 0;
		regrp_tab[1][i] = 0;
		regrp_tab[2][i] = 0;
	}

// 	assert(num_bstx_ant != 4); // linjiangnan

	// build PCFICH
	k0  = (num_ID_cell)				  % (2*num_rb_dl);    //value of REG caohuan,k0,k1,k2,k3 means the position of PCFICH
    k1  = (num_ID_cell+num_rb_dl/2)	  % (2*num_rb_dl);
	k2  = (num_ID_cell+num_rb_dl)	  % (2*num_rb_dl);
	k3  = (num_ID_cell+num_rb_dl*3/2) % (2*num_rb_dl);

	regrp_tab[0][k0] = BSTX_INDICATION_PCFICH;
	regrp_tab[0][k1] = BSTX_INDICATION_PCFICH;
	regrp_tab[0][k2] = BSTX_INDICATION_PCFICH;
	regrp_tab[0][k3] = BSTX_INDICATION_PCFICH;

	if (k1 < k0)
	{
		regrp_pcfich[0] = k1;
		regrp_pcfich[1] = k2;
		regrp_pcfich[2] = k3;
		regrp_pcfich[3] = k0;
	}
	else if (k2 < k1)
	{
		regrp_pcfich[0] = k2;
		regrp_pcfich[1] = k3;
		regrp_pcfich[2] = k0;
		regrp_pcfich[3] = k1;
	}
	else if (k3 < k2)
	{
		regrp_pcfich[0] = k3;
		regrp_pcfich[1] = k0;
		regrp_pcfich[2] = k1;
		regrp_pcfich[3] = k2;
	}
	else if (k0 < k3)
	{
		regrp_pcfich[0] = k0;
		regrp_pcfich[1] = k1;
		regrp_pcfich[2] = k2;
		regrp_pcfich[3] = k3;
	}

	// PHICH TABLE
	regrps_each_ofdm[0] = 2*num_rb_dl - 4;
	regrps_each_ofdm[1] = 3*num_rb_dl;
	regrps_each_ofdm[2] = 3*num_rb_dl;

	if(cp_cfg == BSTX_NORMAL_CP)	
	{
		if(fdd_tdd_mode == BSTX_TDD_MUXMODE)			num_map_unit_phich = mi_phich * num_grp_phich;
		else if(fdd_tdd_mode == BSTX_FDD_MUXMODE)	num_map_unit_phich = num_grp_phich;
		else								assert(0);
	}
	else
	{
		if(fdd_tdd_mode == BSTX_TDD_MUXMODE)			num_map_unit_phich = mi_phich * num_grp_phich / 2;
		else if(fdd_tdd_mode == BSTX_FDD_MUXMODE)	num_map_unit_phich = num_grp_phich / 2;
		else								assert(0);
	}

	for(mm=0; mm<num_map_unit_phich; mm++)
	{
		for(i = 0; i < 3; i++)
		{
			if(duration_phich_cfg==0)				lli = 0;
            else if(subfrm_idx==1 || subfrm_idx==6)	lli = ((mm/2+i+1)%2);
            else									lli = i;

			if(duration_phich_cfg==1 && (subfrm_idx == 1 || subfrm_idx == 6))
				ni = ((num_ID_cell*regrps_each_ofdm[lli]/regrps_each_ofdm[1])+mm+(i*regrps_each_ofdm[lli]/3))%regrps_each_ofdm[lli];
			else
				ni = ((num_ID_cell*regrps_each_ofdm[lli]/regrps_each_ofdm[0])+mm+(i*regrps_each_ofdm[lli]/3))%regrps_each_ofdm[lli];

			if(lli == 0)
			{
				for(j = 0; j < 4; j++)
				{
					if(ni >= regrp_pcfich[j])	ni++;
				}
			}

			regrp_tab[lli][ni] = BSTX_INDICATION_PHICH;
		}
	}


	// DeREmap for PDCCH
	rs_start = num_ID_cell % 3;
	
	idx = 0;

	for (i=0; i<num_rb_dl; i++)
	{
		if (num_ofdm_pdcch>0 && regrp_tab[0][2*i] == 0)
		{
			if(rs_start == 0)
			{
				for(p=0; p<num_bstx_ant; p++)
				{
					outbuf[p][0*1320 + 1+12*i] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdcch_RB);
					outbuf[p][0*1320 + 2+12*i] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdcch_RB);
					outbuf[p][0*1320 + 4+12*i] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdcch_RB);
					outbuf[p][0*1320 + 5+12*i] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdcch_RB);
				}
			}
			else if(rs_start == 1)
			{
				for(p=0; p<num_bstx_ant; p++)
				{
					outbuf[p][0*1320 + 0+12*i] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdcch_RB);
					outbuf[p][0*1320 + 2+12*i] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdcch_RB);
					outbuf[p][0*1320 + 3+12*i] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdcch_RB);
					outbuf[p][0*1320 + 5+12*i] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdcch_RB);
				}
			}
			else if(rs_start == 2)
			{
				for(p=0; p<num_bstx_ant; p++)
				{
					outbuf[p][0*1320 + 0+12*i] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdcch_RB);
					outbuf[p][0*1320 + 1+12*i] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdcch_RB);
					outbuf[p][0*1320 + 3+12*i] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdcch_RB);
					outbuf[p][0*1320 + 4+12*i] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdcch_RB);
				}
			}

			idx += 4;
		}
		
		if (num_ofdm_pdcch>1 && regrp_tab[1][3*i] == 0)
		{
			for(p=0; p<num_bstx_ant; p++)
			{
				outbuf[p][1*1320 + 0+12*i] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdcch_RA);
				outbuf[p][1*1320 + 1+12*i] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdcch_RA);
				outbuf[p][1*1320 + 2+12*i] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdcch_RA);
				outbuf[p][1*1320 + 3+12*i] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdcch_RA);
			}			
			
			idx += 4;
		}
		
		if (num_ofdm_pdcch>2 && regrp_tab[2][3*i] == 0)
		{
			for(p=0; p<num_bstx_ant; p++)
			{
				outbuf[p][2*1320 + 0+12*i] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdcch_RA);
				outbuf[p][2*1320 + 1+12*i] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdcch_RA);
				outbuf[p][2*1320 + 2+12*i] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdcch_RA);
				outbuf[p][2*1320 + 3+12*i] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdcch_RA);
			}	
			
			idx += 4;
		}

		if (num_ofdm_pdcch>1 && regrp_tab[1][3*i+1] == 0)
		{
			for(p=0; p<num_bstx_ant; p++)
			{
				outbuf[p][1*1320 + 4+12*i] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdcch_RA);
				outbuf[p][1*1320 + 5+12*i] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdcch_RA);
				outbuf[p][1*1320 + 6+12*i] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdcch_RA);
				outbuf[p][1*1320 + 7+12*i] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdcch_RA);
			}			
			
			idx += 4;
		}

		if (num_ofdm_pdcch>2 && regrp_tab[2][3*i+1] == 0)
		{
			for(p=0; p<num_bstx_ant; p++)
			{
				outbuf[p][2*1320 + 4+12*i] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdcch_RA);
				outbuf[p][2*1320 + 5+12*i] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdcch_RA);
				outbuf[p][2*1320 + 6+12*i] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdcch_RA);
				outbuf[p][2*1320 + 7+12*i] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdcch_RA);
			}		
			
			idx += 4;
		}
		
		if (num_ofdm_pdcch>0 && regrp_tab[0][2*i+1] == 0)
		{			
			if(rs_start == 0)
			{
				for(p=0; p<num_bstx_ant; p++)
				{
					outbuf[p][0*1320 + 7+12*i] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdcch_RB);
					outbuf[p][0*1320 + 8+12*i] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdcch_RB);
					outbuf[p][0*1320 + 10+12*i] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdcch_RB);
					outbuf[p][0*1320 + 11+12*i] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdcch_RB);
				}
			}
			else if(rs_start == 1)
			{
				for(p=0; p<num_bstx_ant; p++)
				{
					outbuf[p][0*1320 + 6+12*i] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdcch_RB);
					outbuf[p][0*1320 + 8+12*i] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdcch_RB);
					outbuf[p][0*1320 + 9+12*i] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdcch_RB);
					outbuf[p][0*1320 + 11+12*i] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdcch_RB);
				}
			}
			else if(rs_start == 2)
			{
				for(p=0; p<num_bstx_ant; p++)
				{
					outbuf[p][0*1320 + 6+12*i] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdcch_RB);
					outbuf[p][0*1320 + 7+12*i] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdcch_RB);
					outbuf[p][0*1320 + 9+12*i] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdcch_RB);
					outbuf[p][0*1320 + 10+12*i] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdcch_RB);
				}
			}

			idx += 4;
		}
		
		if (num_ofdm_pdcch>1 && regrp_tab[1][3*i+2] == 0)
		{
			for(p=0; p<num_bstx_ant; p++)
			{
				outbuf[p][1*1320 + 8+12*i] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdcch_RA);
				outbuf[p][1*1320 + 9+12*i] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdcch_RA);
				outbuf[p][1*1320 + 10+12*i] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdcch_RA);
				outbuf[p][1*1320 + 11+12*i] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdcch_RA);
			}			
			
			idx += 4;
		}	
		
		if (num_ofdm_pdcch>2 && regrp_tab[2][3*i+2] == 0)
		{
			for(p=0; p<num_bstx_ant; p++)
			{
				outbuf[p][2*1320 + 8+12*i] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdcch_RA);
				outbuf[p][2*1320 + 9+12*i] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdcch_RA);
				outbuf[p][2*1320 + 10+12*i] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdcch_RA);
				outbuf[p][2*1320 + 11+12*i] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdcch_RA);
			}			
			
			idx += 4;
		}
	} // end for (i=0; i<num_rb_dl; i++)
	
	return idx;
}

int remap_pdsch(cfloat **inbuf, int num_rb_dl, int16 **rb_alloc_pdsch, int subfrm_idx, int num_ofdm_pdcch
			, int num_ID_cell, int num_bstx_ant, int cp_cfg, int uldl_switch_cfg, int spec_subfrm_cfg, float gain_pdsch_RA
			, float gain_pdsch_RB, cfloat **outbuf)
{
	int i, l, p;
	int slot, rs_start;
	int idx;
	int num_symb_DL;
	int num_ofdm_dl;

	if(cp_cfg == BSTX_NORMAL_CP)		num_symb_DL = 7;
	else						num_symb_DL = 6;

	if(bstx_isSpecialSubframe(subfrm_idx, uldl_switch_cfg))
	{
		num_ofdm_dl = getLen_specsubfram(cp_cfg, spec_subfrm_cfg);
	}
	else if(bstx_isDL(subfrm_idx,uldl_switch_cfg))
	{
		num_ofdm_dl = 2 * num_symb_DL;
	}
	else
	{
		num_ofdm_dl = 0;
	}
	
	idx = 0;

	for(l=num_ofdm_pdcch; l<num_ofdm_dl; l++)
	{		
		if(l < num_symb_DL)	slot = 0;
		else				slot = 1;

		if (    ((subfrm_idx==1 || subfrm_idx==6) && l==2)	// with PSS
			||	((subfrm_idx==0 || subfrm_idx==5) && l==13)	// with SSS
			||	(subfrm_idx==0 && l>=7 && l<11))			// with PBCH
		{
			if (subfrm_idx==0 && l==7) // with RS
			{
				if (num_bstx_ant == 1)
				{
					rs_start = num_ID_cell%6;

					for (i=0; i<2*num_rb_dl; i++) // resource element group as a unit
					{
						if (rb_alloc_pdsch[slot][i/2] && (i<(num_rb_dl-6) || i>=(num_rb_dl+6)))
						{
							if (rs_start == 0)
							{
								for(p=0; p<num_bstx_ant; p++)
								{
									outbuf[p][l*1320 + 6*i+1] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+2] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+3] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+4] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+5] = cfloatmultiplyfloat(inbuf[p][idx+4],gain_pdsch_RB);
								}							
								
								idx += 5;
							}
							else if (rs_start == 1)
							{
								for(p=0; p<num_bstx_ant; p++)
								{
									outbuf[p][l*1320 + 6*i+0] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+2] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+3] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+4] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+5] = cfloatmultiplyfloat(inbuf[p][idx+4],gain_pdsch_RB);
								}							
								
								idx += 5;
							}
							else if (rs_start == 2)
							{
								for(p=0; p<num_bstx_ant; p++)
								{
									outbuf[p][l*1320 + 6*i+0] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+1] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+3] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+4] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+5] = cfloatmultiplyfloat(inbuf[p][idx+4],gain_pdsch_RB);
								}							
								
								idx += 5;
							}
							else if (rs_start == 3)
							{
								for(p=0; p<num_bstx_ant; p++)
								{
									outbuf[p][l*1320 + 6*i+0] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+1] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+2] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+4] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+5] = cfloatmultiplyfloat(inbuf[p][idx+4],gain_pdsch_RB);
								}							
								
								idx += 5;
							}
							else if (rs_start == 4)
							{
								for(p=0; p<num_bstx_ant; p++)
								{
									outbuf[p][l*1320 + 6*i+0] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+1] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+2] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+3] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+5] = cfloatmultiplyfloat(inbuf[p][idx+4],gain_pdsch_RB);
								}							
								
								idx += 5;
							}
							else if (rs_start == 5)
							{
								for(p=0; p<num_bstx_ant; p++)
								{
									outbuf[p][l*1320 + 6*i+0] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+1] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+2] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+3] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+4] = cfloatmultiplyfloat(inbuf[p][idx+4],gain_pdsch_RB);
								}							

								idx += 5;
							} // end if (rs_start == 0)
						} // if (rb_alloc_pdsch[slot][i/2] && (i<(num_rb_dl-6) || i>=(num_rb_dl+6)))
					} // end for (i=0; i<2*num_rb_dl; i++)
				} 
				else if(num_bstx_ant == 2)
				{
					rs_start = num_ID_cell%3;

					for (i=0; i<2*num_rb_dl; i++) // resource element group as a unit
					{
						if (rb_alloc_pdsch[slot][i/2] && (i<(num_rb_dl-6) || i>=(num_rb_dl+6)))
						{
							if (rs_start == 0)
							{
								for(p=0; p<num_bstx_ant; p++)
								{
									outbuf[p][l*1320 + 6*i+1] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+2] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+4] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+5] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdsch_RB);
								}							
								
								idx += 4;
							}
							else if (rs_start == 1)
							{
								for(p=0; p<num_bstx_ant; p++)
								{
									outbuf[p][l*1320 + 6*i+0] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+2] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+3] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+5] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdsch_RB);
								}
															
								idx += 4;
							}
							else if (rs_start == 2)
							{
								for(p=0; p<num_bstx_ant; p++)
								{
									outbuf[p][l*1320 + 6*i+0] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+1] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+3] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdsch_RB);
									outbuf[p][l*1320 + 6*i+4] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdsch_RB);
								}							
								
								idx += 4;
							}
						} 
					} // end for (i=0; i<2*num_rb_dl; i++)
				}
			}
			else // without RS
			{
				for (i=0; i<2*num_rb_dl; i++) // resource element group as a unit
				{
					if (rb_alloc_pdsch[slot][i/2] && (i<(num_rb_dl-6) || i>=(num_rb_dl+6)))
					{
						for(p=0; p<num_bstx_ant; p++)
						{
							outbuf[p][l*1320 + 6*i+0] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdsch_RA);
							outbuf[p][l*1320 + 6*i+1] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdsch_RA);
							outbuf[p][l*1320 + 6*i+2] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdsch_RA);
							outbuf[p][l*1320 + 6*i+3] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdsch_RA);
							outbuf[p][l*1320 + 6*i+4] = cfloatmultiplyfloat(inbuf[p][idx+4],gain_pdsch_RA);
							outbuf[p][l*1320 + 6*i+5] = cfloatmultiplyfloat(inbuf[p][idx+5],gain_pdsch_RA);
						}					

						idx += 6;
					}
				}
			}
		}
		else
		{
			if (l==4 || l==7 || l==11) // with RS
			{
				if (num_bstx_ant == 1)
				{
					if (l==7)	rs_start = num_ID_cell%6;
					else		rs_start = (num_ID_cell+3)%6;
					
					for (i=0; i<num_rb_dl; i++) // RB as a unit
					{
						if (rb_alloc_pdsch[slot][i])
						{
							if (rs_start == 0)
							{
								for(p=0; p<num_bstx_ant; p++)
								{
									outbuf[p][l*1320 + 12*i+1] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+2] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+3] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+4] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+5] = cfloatmultiplyfloat(inbuf[p][idx+4],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+7] = cfloatmultiplyfloat(inbuf[p][idx+5],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+8] = cfloatmultiplyfloat(inbuf[p][idx+6],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+9] = cfloatmultiplyfloat(inbuf[p][idx+7],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+10] = cfloatmultiplyfloat(inbuf[p][idx+8],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+11] = cfloatmultiplyfloat(inbuf[p][idx+9],gain_pdsch_RB);
								}						
								
								idx += 10;
							}
							else if (rs_start == 1)
							{
								for(p=0; p<num_bstx_ant; p++)
								{
									outbuf[p][l*1320 + 12*i+0] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+2] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+3] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+4] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+5] = cfloatmultiplyfloat(inbuf[p][idx+4],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+6] = cfloatmultiplyfloat(inbuf[p][idx+5],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+8] = cfloatmultiplyfloat(inbuf[p][idx+6],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+9] = cfloatmultiplyfloat(inbuf[p][idx+7],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+10] = cfloatmultiplyfloat(inbuf[p][idx+8],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+11] = cfloatmultiplyfloat(inbuf[p][idx+9],gain_pdsch_RB);
								}							
							
								idx += 10;
							}
							else if (rs_start == 2)
							{
								for(p=0; p<num_bstx_ant; p++)
								{
									outbuf[p][l*1320 + 12*i+0] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+1] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+3] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+4] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+5] = cfloatmultiplyfloat(inbuf[p][idx+4],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+6] = cfloatmultiplyfloat(inbuf[p][idx+5],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+7] = cfloatmultiplyfloat(inbuf[p][idx+6],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+9] = cfloatmultiplyfloat(inbuf[p][idx+7],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+10] = cfloatmultiplyfloat(inbuf[p][idx+8],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+11] = cfloatmultiplyfloat(inbuf[p][idx+9],gain_pdsch_RB);
								}							
								
								idx += 10;
							}
							else if (rs_start == 3)
							{
								for(p=0; p<num_bstx_ant; p++)
								{
									outbuf[p][l*1320 + 12*i+0] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+1] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+2] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+4] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+5] = cfloatmultiplyfloat(inbuf[p][idx+4],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+6] = cfloatmultiplyfloat(inbuf[p][idx+5],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+7] = cfloatmultiplyfloat(inbuf[p][idx+6],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+8] = cfloatmultiplyfloat(inbuf[p][idx+7],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+10] = cfloatmultiplyfloat(inbuf[p][idx+8],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+11] = cfloatmultiplyfloat(inbuf[p][idx+9],gain_pdsch_RB);
								}							
								
								idx += 10;
							}
							else if (rs_start == 4)
							{
								for(p=0; p<num_bstx_ant; p++)
								{
									outbuf[p][l*1320 + 12*i+0] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+1] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+2] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+3] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+5] = cfloatmultiplyfloat(inbuf[p][idx+4],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+6] = cfloatmultiplyfloat(inbuf[p][idx+5],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+7] = cfloatmultiplyfloat(inbuf[p][idx+6],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+8] = cfloatmultiplyfloat(inbuf[p][idx+7],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+9] = cfloatmultiplyfloat(inbuf[p][idx+8],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+11] = cfloatmultiplyfloat(inbuf[p][idx+9],gain_pdsch_RB);;
								}
								
								idx += 10;
							}
							else if (rs_start == 5)
							{
								for(p=0; p<num_bstx_ant; p++)
								{
									outbuf[p][l*1320 + 12*i+0] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+1] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+2] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+3] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+4] = cfloatmultiplyfloat(inbuf[p][idx+4],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+6] = cfloatmultiplyfloat(inbuf[p][idx+5],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+7] = cfloatmultiplyfloat(inbuf[p][idx+6],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+8] = cfloatmultiplyfloat(inbuf[p][idx+7],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+9] = cfloatmultiplyfloat(inbuf[p][idx+8],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+10] = cfloatmultiplyfloat(inbuf[p][idx+9],gain_pdsch_RB);
								}							
								
								idx += 10;
							} // end if (rs_start == 0)
						} // if (rb_alloc_pdsch[slot][i])
					} // end for (i=0; i<num_rb_dl; i++)
				} 
				else if(num_bstx_ant == 2)
				{
					rs_start = num_ID_cell%3;

					for (i=0; i<num_rb_dl; i++) // RB as a unit
					{
						if (rb_alloc_pdsch[slot][i])
						{
							if (rs_start == 0)
							{
								for(p=0; p<num_bstx_ant; p++)
								{
									outbuf[p][l*1320 + 12*i+1] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+2] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+4] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+5] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+7] = cfloatmultiplyfloat(inbuf[p][idx+4],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+8] = cfloatmultiplyfloat(inbuf[p][idx+5],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+10] = cfloatmultiplyfloat(inbuf[p][idx+6],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+11] = cfloatmultiplyfloat(inbuf[p][idx+7],gain_pdsch_RB);
								}						
								
								idx += 8;
							}
							else if (rs_start == 1)
							{
								for(p=0; p<num_bstx_ant; p++)
								{
									outbuf[p][l*1320 + 12*i+0] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+2] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+3] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+5] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+6] = cfloatmultiplyfloat(inbuf[p][idx+4],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+8] = cfloatmultiplyfloat(inbuf[p][idx+5],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+9] = cfloatmultiplyfloat(inbuf[p][idx+6],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+11] = cfloatmultiplyfloat(inbuf[p][idx+7],gain_pdsch_RB);
								}							
								
								idx += 8;
							}
							else if (rs_start == 2)
							{
								for(p=0; p<num_bstx_ant; p++)
								{
									outbuf[p][l*1320 + 12*i+0] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+1] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+3] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+4] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+6] = cfloatmultiplyfloat(inbuf[p][idx+4],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+7] = cfloatmultiplyfloat(inbuf[p][idx+5],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+9] = cfloatmultiplyfloat(inbuf[p][idx+6],gain_pdsch_RB);
									outbuf[p][l*1320 + 12*i+10] = cfloatmultiplyfloat(inbuf[p][idx+7],gain_pdsch_RB);
								}							
								
								idx += 8;
							}
						
						} // if (rb_alloc_pdsch[slot][i])
					} // end for (i=0; i<num_rb_dl; i++)
				}
			}
			else  // without RS
			{
				for (i=0; i<num_rb_dl; i++) // RB as a unit
				{
					if (rb_alloc_pdsch[slot][i])
					{
						for(p=0; p<num_bstx_ant; p++)
						{
							outbuf[p][l*1320 + 12*i+0] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pdsch_RA);
							outbuf[p][l*1320 + 12*i+1] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pdsch_RA);
							outbuf[p][l*1320 + 12*i+2] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pdsch_RA);
							outbuf[p][l*1320 + 12*i+3] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pdsch_RA);
							outbuf[p][l*1320 + 12*i+4] = cfloatmultiplyfloat(inbuf[p][idx+4],gain_pdsch_RA);
							outbuf[p][l*1320 + 12*i+5] = cfloatmultiplyfloat(inbuf[p][idx+5],gain_pdsch_RA);
							outbuf[p][l*1320 + 12*i+6] = cfloatmultiplyfloat(inbuf[p][idx+6],gain_pdsch_RA);
							outbuf[p][l*1320 + 12*i+7] = cfloatmultiplyfloat(inbuf[p][idx+7],gain_pdsch_RA);
							outbuf[p][l*1320 + 12*i+8] = cfloatmultiplyfloat(inbuf[p][idx+8],gain_pdsch_RA);
							outbuf[p][l*1320 + 12*i+9] = cfloatmultiplyfloat(inbuf[p][idx+9],gain_pdsch_RA);
							outbuf[p][l*1320 + 12*i+10] = cfloatmultiplyfloat(inbuf[p][idx+10],gain_pdsch_RA);
							outbuf[p][l*1320 + 12*i+11] = cfloatmultiplyfloat(inbuf[p][idx+11],gain_pdsch_RA);
						}					
						
						idx += 12;					
					}
				} 
			}
		}
	} // end for(l=num_ofdm_pdcch; l<2*num_symb_DL; l++)

	return idx;
}


int remap_pbch(cfloat **inbuf, int num_rb_dl, int subfrm_idx, int num_ID_cell, int cp_cfg, int num_bstx_ant
								, float gain_pbch_RA, float gain_pbch_RB, cfloat **outbuf)
{
	int i, p, l;
	int idx;
	int num_symb_DL;
	int rs_start;
	
	assert(subfrm_idx == 0);	

	if(cp_cfg == BSTX_NORMAL_CP)	num_symb_DL = 7;
	else							num_symb_DL = 6;

	rs_start = num_ID_cell%3;
	
	idx = 0;

	for(l=num_symb_DL; l<num_symb_DL+4; l++)
	{
		if (l==num_symb_DL || l==num_symb_DL+1 || l==2*num_symb_DL-3)
		{			
			for (i=num_rb_dl-6; i<num_rb_dl+6; i++)		           
			{
				if (rs_start == 0)
				{
					for(p=0; p<num_bstx_ant; p++)
					{
						outbuf[p][l*1320 + 6*i+1] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pbch_RB);
						outbuf[p][l*1320 + 6*i+2] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pbch_RB);
						outbuf[p][l*1320 + 6*i+4] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pbch_RB);
						outbuf[p][l*1320 + 6*i+5] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pbch_RB);
					}						
					
					idx += 4;
				}
				else if (rs_start == 1)
				{
					for(p=0; p<num_bstx_ant; p++)
					{
						outbuf[p][l*1320 + 6*i+0] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pbch_RB);
						outbuf[p][l*1320 + 6*i+2] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pbch_RB);
						outbuf[p][l*1320 + 6*i+3] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pbch_RB);
						outbuf[p][l*1320 + 6*i+5] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pbch_RB);
					}					
					
					idx += 4;
				}
				else if (rs_start == 2)
				{
					for(p=0; p<num_bstx_ant; p++)
					{
						outbuf[p][l*1320 + 6*i+0] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pbch_RB);
						outbuf[p][l*1320 + 6*i+1] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pbch_RB);
						outbuf[p][l*1320 + 6*i+3] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pbch_RB);
						outbuf[p][l*1320 + 6*i+4] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pbch_RB);
					}					
					
					idx += 4;
				}
			}
		} 
		else
		{
			for (i=num_rb_dl-6; i<num_rb_dl+6; i++)		           
			{
				for(p=0; p<num_bstx_ant; p++)
				{
					outbuf[p][l*1320 + 6*i+0] = cfloatmultiplyfloat(inbuf[p][idx+0],gain_pbch_RA);
					outbuf[p][l*1320 + 6*i+1] = cfloatmultiplyfloat(inbuf[p][idx+1],gain_pbch_RA);
					outbuf[p][l*1320 + 6*i+2] = cfloatmultiplyfloat(inbuf[p][idx+2],gain_pbch_RA);
					outbuf[p][l*1320 + 6*i+3] = cfloatmultiplyfloat(inbuf[p][idx+3],gain_pbch_RA);
					outbuf[p][l*1320 + 6*i+4] = cfloatmultiplyfloat(inbuf[p][idx+4],gain_pbch_RA);
					outbuf[p][l*1320 + 6*i+5] = cfloatmultiplyfloat(inbuf[p][idx+5],gain_pbch_RA);
				}				
					
				idx += 6;
			}
		}
	}
	
	return idx;
}
