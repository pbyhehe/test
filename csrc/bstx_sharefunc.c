#include "foo.h"
#include "bstx_define.h"
#include "bstx_sharefunc.h"

//bstx_sharefunc bstx_sharefunc_u;


int bstx_isDL(int subfrm_idx, int uldl_swtich)
{   
    subfrm_idx = subfrm_idx%10;
	if (
		(uldl_swtich==0 && (subfrm_idx==0 || subfrm_idx==5)) || 
		(uldl_swtich==1 && (subfrm_idx==0 || subfrm_idx==4 || subfrm_idx==5 || subfrm_idx==9)) || 
        (uldl_swtich==2 && (subfrm_idx==0 || subfrm_idx==3 || subfrm_idx==4 || subfrm_idx==5 
		|| subfrm_idx==8 || subfrm_idx==9)) || 
        (uldl_swtich==3 && (subfrm_idx!=1 && subfrm_idx!=2 && subfrm_idx!=3 && subfrm_idx!=4)) || 
        (uldl_swtich==4 && (subfrm_idx!=1 && subfrm_idx!=2 && subfrm_idx!=3)) || 
        (uldl_swtich==5 && (subfrm_idx!=1 && subfrm_idx!=2)) || 
        (uldl_swtich==6 && (subfrm_idx==0 || subfrm_idx==5 || subfrm_idx==9)) 
		)
		return 1;
	else
		return 0;
}




int bstx_isSpecialSubframe(int subfrm_idx, int uldl_swtich)
{
	subfrm_idx = subfrm_idx % 10;
	if ( (uldl_swtich==0 && (subfrm_idx==1 || subfrm_idx==6)) || 
		(uldl_swtich==1 && (subfrm_idx==1 || subfrm_idx==6)) ||
		(uldl_swtich==2 && (subfrm_idx==1 || subfrm_idx==6)) || 
		(uldl_swtich==3 && (subfrm_idx==1)) || 
		(uldl_swtich==4 && (subfrm_idx==1)) || 
		(uldl_swtich==5 && (subfrm_idx==1)) || 
		(uldl_swtich==6 && (subfrm_idx==1 || subfrm_idx==6)) 
		)
		return 1;
	else
		return 0;
}


int bstx_isUL(int subfrm_idx, int uldl_swtich)
{
	if(bstx_isDL(subfrm_idx, uldl_swtich) || bstx_isSpecialSubframe(subfrm_idx, uldl_swtich))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}


int getLen_specsubfram(int cp_cfg, int spcal_subfrm_cfg)
{
	int L_DL_SpecialFrame;
	
	if (cp_cfg == 0)
	{
		switch(spcal_subfrm_cfg)
		{
		case 0:	L_DL_SpecialFrame=3;	break;
		case 1:	L_DL_SpecialFrame=9;	break;
		case 2:	L_DL_SpecialFrame=10;	break;
		case 3:	L_DL_SpecialFrame=11;	break;
		case 4:	L_DL_SpecialFrame=12;	break;
		case 5:	L_DL_SpecialFrame=3;	break;
		case 6:	L_DL_SpecialFrame=9;	break;
		case 7:	L_DL_SpecialFrame=10;	break;
		case 8:	L_DL_SpecialFrame=11;	break; // L_DL_SpecialFrame = 12 is wrong, 20140504
		default:L_DL_SpecialFrame=3;	break;
		}
	}
	else if(cp_cfg == 1)
	{
		switch(spcal_subfrm_cfg)
		{
		case 0:	L_DL_SpecialFrame=3;	break;
		case 1:	L_DL_SpecialFrame=8;	break;
		case 2:	L_DL_SpecialFrame=9;	break;
		case 3:	L_DL_SpecialFrame=10;	break;
		case 4:	L_DL_SpecialFrame=3;	break;
		case 5:	L_DL_SpecialFrame=8;	break;
		case 6:	L_DL_SpecialFrame=9;	break;
		default:L_DL_SpecialFrame=3;	break;
		}
	}
	else
	{
		L_DL_SpecialFrame=3;
	}
	
	return L_DL_SpecialFrame;
}



int get_num_group_phich(int num_rb_dl, int ng_phich, int cp_cfg)
{
	int ng_gain;	
	int num_phich_group;

	switch(ng_phich)
	{
	case 0:			ng_gain = 1;		break;
	case 1:			ng_gain = 3;		break;
	case 2:			ng_gain = 6;		break;
	case 3:			ng_gain = 12;		break;
	default:		assert(0);			break;
	}

	if(cp_cfg==0)
	{
		num_phich_group = (ng_gain*num_rb_dl+47) / 48;		// num_phich_group = ceil(ng_gain*num_rb_dl/48);
	}
	else
	{
		num_phich_group = 2 * ((ng_gain*num_rb_dl+47)/48);	// num_phich_group = 2*ceil(ng_gain*num_rb_dl/48);
	}
	
	return num_phich_group;
}

int bstx_get_mi_phich(int subfrm_idx, int uldl_swtich)
{
	int mi;

	static int mi_tab[7][10] = 
	{                                                      // uplink-downlink configuration
		{ 2, 1, 0, 0, 0, 2, 1, 0, 0, 0 },                  // 0
		{ 0, 1, 0, 0, 1, 0, 1, 0, 0, 1 },                  // 1
		{ 0, 0, 0, 1, 0, 0, 0, 0, 1, 0 },                  // 2
		{ 1, 0, 0, 0, 0, 0, 0, 0, 1, 1 },                  // 3
		{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 },                  // 4
		{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },                  // 5
		{ 1, 1, 0, 0, 0, 1, 1, 0, 0, 1 }                   // 6
	};
	
	if (uldl_swtich >= 0 && uldl_swtich < 7 && subfrm_idx >= 0 && subfrm_idx<10)
	{
		mi = mi_tab[uldl_swtich][subfrm_idx];
	}
	else
	{
		assert(0);
	}
	
	return mi;
}


int get_REnum_pdsch(int num_RB_DL, int16 **rb_alloc_pdsch, int subfrm_idx, int num_ofdm_pdcch, int num_bstx_ant
								 , int cp_cfg, int uldl_switch_cfg, int spec_subfrm_cfg)
{
	int i, l;
	int slot;
	int outlen;
	int num_symb_DL;
	int num_ofdm_dl;
	int num_regrp[2];
	int num_regrp_with_pbch[2];

	if(cp_cfg == BSTX_NORMAL_CP)	num_symb_DL = 7;
	else							num_symb_DL = 6;

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
	
	num_regrp_with_pbch[0] = 0;
	num_regrp_with_pbch[1] = 0;
	num_regrp[0] = 0;
	num_regrp[1] = 0;

	for (i=0; i<2*num_RB_DL; i++) // resource element group as a unit
	{
		if (rb_alloc_pdsch[0][i/2] && (i<(num_RB_DL-6) || i>=(num_RB_DL+6)))
		{
			num_regrp_with_pbch[0]++;
		}

		if (rb_alloc_pdsch[1][i/2] && (i<(num_RB_DL-6) || i>=(num_RB_DL+6)))
		{
			num_regrp_with_pbch[1]++;
		}

		if (rb_alloc_pdsch[0][i/2])
		{
			num_regrp[0]++;
		}

		if (rb_alloc_pdsch[1][i/2])
		{
			num_regrp[1]++;
		}
	}

	outlen = 0;

	for(l=num_ofdm_pdcch; l<num_ofdm_dl; l++)
	{		
		if(l < num_symb_DL)	slot = 0;
		else				slot = 1;

		if (   ((subfrm_idx==1 || subfrm_idx==6) && l==2)	// with PSS
			|| ((subfrm_idx==0 || subfrm_idx==5) && l==13)	// with SSS
			|| (subfrm_idx==0 && l>=7 && l<11))			// with PBCH
		{
			if(subfrm_idx==0 && l==7) // with RS
			{
				if (num_bstx_ant == 1)
				{
					outlen += 5 * num_regrp_with_pbch[slot];
				} 
				else if(num_bstx_ant == 2)
				{
					outlen += 4 * num_regrp_with_pbch[slot];
				}				
			}
			else // without RS
			{
				outlen += 6 * num_regrp_with_pbch[slot];
			}			
		}
		else  // without PSS, without SSS, without PBCH, without RS
		{
			if (l==4 || l==7 || l==11) // with RS
			{			
				if (num_bstx_ant == 1)
				{				
					outlen += 5 * num_regrp[slot];
				} 
				else if(num_bstx_ant == 2)
				{
					outlen += 4 * num_regrp[slot];
				}
			}
			else // without RS
			{
				outlen += 6 * num_regrp[slot];
			}
		}
	} // end for(l=num_ofdm_pdcch; l<2*num_symb_DL; l++)

	return outlen;
}
