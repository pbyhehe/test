#include "foo.h"
#include "uerx_define.h"
#include "uerx_sharefunc.h"

#pragma DATA_SECTION("uerx_data")


int uerx_pseudo_rand(int cinit, int *outbuf, int outlen)
{
	int i;
	int x1[32]= {0,0,0,1,0,0,1,0,0,1,1,0,1,1,0,0,0,0,1,0,0,1,0,1,1,0,0,1,0,1,0,1}; // push to 1600
	int x2[32];

	// init x2
	for (i=0; i<31; i++)
	{
		x2[i] = cinit%2;
		cinit /= 2;
	}
	
	// push x2 to 1600
	for(i=31; i<1600; i++)
	{
		x2[i%32] = x2[(i+4)%32] ^ x2[(i+3)%32] ^ x2[(i+2)%32] ^ x2[(i+1)%32];
	}
	
	// get pseudo rand sequence
	for (i=0; i<outlen; i++)
	{
		x1[i%32] = x1[(i+4)%32] ^ x1[(i+1)%32];
		x2[i%32] = x2[(i+4)%32] ^ x2[(i+3)%32] ^ x2[(i+2)%32] ^ x2[(i+1)%32];
		
		outbuf[i] = x1[i%32] ^ x2[i%32];
	}
	
	return outlen;	
}

/************************************************************************/
/*                isDL()                                                */
/************************************************************************/
int uerx_isDL(int subfrm_idx, int uldl_switch)
{   
    subfrm_idx = subfrm_idx%10;
	if (
		(uldl_switch==0 && (subfrm_idx==0 || subfrm_idx==5)) || 
		(uldl_switch==1 && (subfrm_idx==0 || subfrm_idx==4 || subfrm_idx==5 || subfrm_idx==9)) || 
        (uldl_switch==2 && (subfrm_idx==0 || subfrm_idx==3 || subfrm_idx==4 || subfrm_idx==5 
		|| subfrm_idx==8 || subfrm_idx==9)) || 
        (uldl_switch==3 && (subfrm_idx!=1 && subfrm_idx!=2 && subfrm_idx!=3 && subfrm_idx!=4)) || 
        (uldl_switch==4 && (subfrm_idx!=1 && subfrm_idx!=2 && subfrm_idx!=3)) || 
        (uldl_switch==5 && (subfrm_idx!=1 && subfrm_idx!=2)) || 
        (uldl_switch==6 && (subfrm_idx==0 || subfrm_idx==5 || subfrm_idx==9)) 
		)
		return 1;
	else
		return 0;
}



/************************************************************************/
/*                isSpecialSubframe()                                   */
/************************************************************************/
int uerx_isSpecialSubframe(int subfrm_idx, int uldl_switch)
{
	subfrm_idx = subfrm_idx % 10;
	if ( (uldl_switch==0 && (subfrm_idx==1 || subfrm_idx==6)) || 
		(uldl_switch==1 && (subfrm_idx==1 || subfrm_idx==6)) ||
		(uldl_switch==2 && (subfrm_idx==1 || subfrm_idx==6)) || 
		(uldl_switch==3 && (subfrm_idx==1)) || 
		(uldl_switch==4 && (subfrm_idx==1)) || 
		(uldl_switch==5 && (subfrm_idx==1)) || 
		(uldl_switch==6 && (subfrm_idx==1 || subfrm_idx==6)) 
		)
		return 1;
	else
		return 0;
}
/************************************************************************/
/*                isUL()				                                */
/************************************************************************/
int uerx_isUL(int subfrm_idx, int uldl_switch)
{
	if(uerx_isDL(subfrm_idx, uldl_switch) ||
		uerx_isSpecialSubframe(subfrm_idx, uldl_switch))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/************************************************************************/
/*                specsubframlen()					                    */
/************************************************************************/
int specsubframlen(int cp_cfg, int spcal_subfrm_cfg)
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
		case 8:	L_DL_SpecialFrame=12;	break;
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


/************************************************************************/
/*                phichgroups()				    	                    */
/************************************************************************/
int get_num_phich_group(int num_rb_dl, int ng_phich, int cp_cfg)
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

int uerx_get_mi_phich(int subfrm_idx, int uldl_swtich)
{
	int mi;

	#pragma SET_DATA_SECTION("uerx_data")
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
	#pragma SET_DATA_SECTION()
	
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
