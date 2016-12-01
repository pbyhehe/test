#include <assert.h>

#include "uerx_define.h"
#include "uerx_decode_irx.h"
#include "uerx_sharefunc.h"

#pragma SET_DATA_SECTION("uerx_data")
static 	cfloat W_cnj[8][12];
#pragma SET_DATA_SECTION()

int decode_Init()
{
	// W_cnj = conj(W)
	W_cnj[0][0].re=1;  W_cnj[0][0].im=0;
	W_cnj[0][1].re=1;  W_cnj[0][1].im=0;
	W_cnj[0][2].re=1;  W_cnj[0][2].im=0;
	W_cnj[0][3].re=1;  W_cnj[0][3].im=0;
	W_cnj[0][4].re=1;  W_cnj[0][4].im=0;
	W_cnj[0][5].re=1;  W_cnj[0][5].im=0;
	W_cnj[0][6].re=1;  W_cnj[0][6].im=0;
	W_cnj[0][7].re=1;  W_cnj[0][7].im=0;
	W_cnj[0][8].re=1;  W_cnj[0][8].im=0;
	W_cnj[0][9].re=1;  W_cnj[0][9].im=0;
	W_cnj[0][10].re=1; W_cnj[0][10].im=0;
	W_cnj[0][11].re=1; W_cnj[0][11].im=0;

	W_cnj[1][0].re=1;   W_cnj[1][0].im=0;
	W_cnj[1][1].re=-1;  W_cnj[1][1].im=0;
	W_cnj[1][2].re=1;   W_cnj[1][2].im=0;
	W_cnj[1][3].re=-1;  W_cnj[1][3].im=0;
	W_cnj[1][4].re=1;   W_cnj[1][4].im=0;
	W_cnj[1][5].re=-1;  W_cnj[1][5].im=0;
	W_cnj[1][6].re=1;   W_cnj[1][6].im=0;
	W_cnj[1][7].re=-1;  W_cnj[1][7].im=0;
	W_cnj[1][8].re=1;   W_cnj[1][8].im=0;
	W_cnj[1][9].re=-1;  W_cnj[1][9].im=0;
	W_cnj[1][10].re=1;  W_cnj[1][10].im=0;
	W_cnj[1][11].re=-1; W_cnj[1][11].im=0;

	W_cnj[2][0].re=1;   W_cnj[2][0].im=0;
	W_cnj[2][1].re=1;   W_cnj[2][1].im=0;
	W_cnj[2][2].re=-1;  W_cnj[2][2].im=0;
	W_cnj[2][3].re=-1;  W_cnj[2][3].im=0;
	W_cnj[2][4].re=1;   W_cnj[2][4].im=0;
	W_cnj[2][5].re=1;   W_cnj[2][5].im=0;
	W_cnj[2][6].re=-1;  W_cnj[2][6].im=0;
	W_cnj[2][7].re=-1;  W_cnj[2][7].im=0;
	W_cnj[2][8].re=1;   W_cnj[2][8].im=0;
	W_cnj[2][9].re=1;   W_cnj[2][9].im=0;
	W_cnj[2][10].re=-1; W_cnj[2][10].im=0;
	W_cnj[2][11].re=-1; W_cnj[2][11].im=0;

	W_cnj[3][0].re=1;   W_cnj[3][0].im=0;
	W_cnj[3][1].re=-1;  W_cnj[3][1].im=0;
	W_cnj[3][2].re=-1;  W_cnj[3][2].im=0;
	W_cnj[3][3].re=1;   W_cnj[3][3].im=0;
	W_cnj[3][4].re=1;   W_cnj[3][4].im=0;
	W_cnj[3][5].re=-1;  W_cnj[3][5].im=0;
	W_cnj[3][6].re=-1;  W_cnj[3][6].im=0;
	W_cnj[3][7].re=1;   W_cnj[3][7].im=0;
	W_cnj[3][8].re=1;   W_cnj[3][8].im=0;
	W_cnj[3][9].re=-1;  W_cnj[3][9].im=0;
	W_cnj[3][10].re=-1; W_cnj[3][10].im=0;
	W_cnj[3][11].re=1;  W_cnj[3][11].im=0;

	W_cnj[4][0].re=0;  W_cnj[4][0].im=-1;
	W_cnj[4][1].re=0;  W_cnj[4][1].im=-1;
	W_cnj[4][2].re=0;  W_cnj[4][2].im=-1;
	W_cnj[4][3].re=0;  W_cnj[4][3].im=-1;
	W_cnj[4][4].re=0;  W_cnj[4][4].im=-1;
	W_cnj[4][5].re=0;  W_cnj[4][5].im=-1;
	W_cnj[4][6].re=0;  W_cnj[4][6].im=-1;
	W_cnj[4][7].re=0;  W_cnj[4][7].im=-1;
	W_cnj[4][8].re=0;  W_cnj[4][8].im=-1;
	W_cnj[4][9].re=0;  W_cnj[4][9].im=-1;
	W_cnj[4][10].re=0; W_cnj[4][10].im=-1;
	W_cnj[4][11].re=0; W_cnj[4][11].im=-1;

	W_cnj[5][0].re=0;  W_cnj[5][0].im=-1;
	W_cnj[5][1].re=0;  W_cnj[5][1].im=1;
	W_cnj[5][2].re=0;  W_cnj[5][2].im=-1;
	W_cnj[5][3].re=0;  W_cnj[5][3].im=1;
	W_cnj[5][4].re=0;  W_cnj[5][4].im=-1;
	W_cnj[5][5].re=0;  W_cnj[5][5].im=1;
	W_cnj[5][6].re=0;  W_cnj[5][6].im=-1;
	W_cnj[5][7].re=0;  W_cnj[5][7].im=1;
	W_cnj[5][8].re=0;  W_cnj[5][8].im=-1;
	W_cnj[5][9].re=0;  W_cnj[5][9].im=1;
	W_cnj[5][10].re=0; W_cnj[5][10].im=-1;
	W_cnj[5][11].re=0; W_cnj[5][11].im=1;

	W_cnj[6][0].re=0;  W_cnj[6][0].im=-1;
	W_cnj[6][1].re=0;  W_cnj[6][1].im=-1;
	W_cnj[6][2].re=0;  W_cnj[6][2].im=1;
	W_cnj[6][3].re=0;  W_cnj[6][3].im=1;
	W_cnj[6][4].re=0;  W_cnj[6][4].im=-1;
	W_cnj[6][5].re=0;  W_cnj[6][5].im=-1;
	W_cnj[6][6].re=0;  W_cnj[6][6].im=1;
	W_cnj[6][7].re=0;  W_cnj[6][7].im=1;
	W_cnj[6][8].re=0;  W_cnj[6][8].im=-1;
	W_cnj[6][9].re=0;  W_cnj[6][9].im=-1;
	W_cnj[6][10].re=0; W_cnj[6][10].im=1;
	W_cnj[6][11].re=0; W_cnj[6][11].im=1;

	W_cnj[7][0].re=0;  W_cnj[7][0].im=-1;
	W_cnj[7][1].re=0;  W_cnj[7][1].im=1;
	W_cnj[7][2].re=0;  W_cnj[7][2].im=1;
	W_cnj[7][3].re=0;  W_cnj[7][3].im=-1;
	W_cnj[7][4].re=0;  W_cnj[7][4].im=-1;
	W_cnj[7][5].re=0;  W_cnj[7][5].im=1;
	W_cnj[7][6].re=0;  W_cnj[7][6].im=1;
	W_cnj[7][7].re=0;  W_cnj[7][7].im=-1;
	W_cnj[7][8].re=0;  W_cnj[7][8].im=-1;
	W_cnj[7][9].re=0;  W_cnj[7][9].im=1;
	W_cnj[7][10].re=0; W_cnj[7][10].im=1;
	W_cnj[7][11].re=0; W_cnj[7][11].im=-1;
 
	return 0;
}

int decode_Clear()
{
	return 0;
}
	
int decode_pcfich(float *inbuf, int inlen, int num_rb_dl)
{
	int i;
	int cfi;
	int num_ofdm_pdcch;

	float llr[4][32];
	float sum_llr[4];	

	#pragma SET_DATA_SECTION("uerx_data")
	static int cfi1[32] = {1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,1,-1};
	static int cfi2[32] = {-1,1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,1};
	static int cfi3[32] = {-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1};
	static int cfi4[32] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	#pragma SET_DATA_SECTION()

	assert(inlen == 32);

	for (i=0; i<32; i++)
	{
		llr[0][i] = inbuf[i]*cfi1[i];
		llr[1][i] = inbuf[i]*cfi2[i];
		llr[2][i] = inbuf[i]*cfi3[i];
		llr[3][i] = inbuf[i]*cfi4[i];
	}

	sum_llr[0] = 0;
	sum_llr[1] = 0;
	sum_llr[2] = 0;
	sum_llr[3] = 0;

	for(i=0; i<32; i++)
	{
		sum_llr[0] += llr[0][i];
		sum_llr[1] += llr[1][i];
		sum_llr[2] += llr[2][i];
		sum_llr[3] += llr[3][i];
	}
	
	if (sum_llr[0]>=sum_llr[1] && sum_llr[0]>=sum_llr[2] && sum_llr[0]>=sum_llr[3])
	{
		cfi = 1;
	}
	else if (sum_llr[1]>=sum_llr[0] && sum_llr[1]>=sum_llr[2] && sum_llr[1]>=sum_llr[3])
	{
		cfi = 2;
	}
	else if (sum_llr[2]>=sum_llr[0] && sum_llr[2]>=sum_llr[1] && sum_llr[2]>=sum_llr[3])
	{
		cfi = 3;
	}
	else if (sum_llr[3]>=sum_llr[0] && sum_llr[3]>=sum_llr[1] && sum_llr[3]>=sum_llr[2])
	{
		assert(0);
	}

	if(num_rb_dl <= 10)
		num_ofdm_pdcch = cfi + 1;
	else
		num_ofdm_pdcch = cfi;

	return num_ofdm_pdcch;
}


int decode_phich(cfloat *x_inbuf, cfloat *csi_inbuf, int inlen, int subfrm_idx, int num_id_cell
								   , int cp_cfg, int idx_seque_phich)
{
	int i;
	int cinit;
	int ret;
	int c_buf[12];	

	float sum_llr;
	
	cfloat Z[12],X[12];
	cfloat llr[12];	
	
	if(cp_cfg == UERX_NORMAL_CP)
	{		
		cinit = (subfrm_idx+1)*(2*num_id_cell+1)*(1<<9) + num_id_cell;		 
		/*uerx_sharefunc_u.*/uerx_pseudo_rand(cinit, c_buf, 12);
		
		for (i=0; i<12; i++)  
		{
			// X<16,3>
			X[i].re = x_inbuf[i].re * (1-2*c_buf[i]); 
			X[i].im = x_inbuf[i].im * (1-2*c_buf[i]); 
			
			Z[i].re = X[i].re*W_cnj[idx_seque_phich][i].re - X[i].im*W_cnj[idx_seque_phich][i].im;
			Z[i].im = X[i].re*W_cnj[idx_seque_phich][i].im + X[i].im*W_cnj[idx_seque_phich][i].re;

			llr[i].re = Z[i].re * csi_inbuf[i].re;
			llr[i].im = Z[i].im * csi_inbuf[i].re;			
		}

		sum_llr = 0;
		
		for (i=0; i<12; i++)
		{
			sum_llr += llr[i].re + llr[i].im;
		}

		ret = sum_llr>0 ? UERX_NACK : UERX_ACK;
	}				
	else // extend CP
	{
		assert(0);
	}
	
	return ret;
}

