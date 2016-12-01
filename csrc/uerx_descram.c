#include "foo.h"
#include "uerx_define.h"
#include "uerx_descram.h"


#define DESCRAMB_CW0_PDSCH		0
#define DESCRAMB_CW1_PDSCH		1
#define DESCRAMB_PBCH			2
#define DESCRAMB_CTRCH			2
#define DESCRAMBLE_RESET		1


int descram_core(float *inbuf, int inlen, int reset, int flag, int cinit, int Nc, float *outbuf)
{ 
	int i, j, m, len;
	int x1_t[32], x2_t[32];
	int *x1_init_p;
	
	#pragma SET_DATA_SECTION("uerx_data")
	static int x1_1600_push[32] = {0,0,0,1,0,0,1,0,0,1,1,0,1,1,0,0,0,0,1,0,0,1,0,1,1,0,0,1,0,1,0,1}; // push to 1600
	static int x1_2080_push[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0}; // push to 2080
	static int x1_2560_push[32] = {0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,1,0,0,0,0,0,0,0,1,0,1,0,0,1,1,0,0}; // push to 2560
	static int x1_3040_push[32]	= {0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,1,0,0,1,0,1,1,0,0,1,0,1,1}; // push to 3040
	static int x1_2032_push[32]	= {0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1}; // push to 2032
	static int x1_2464_push[32]	= {0,0,0,1,0,1,1,1,0,1,0,0,1,1,0,1,0,1,1,1,0,1,0,0,1,1,0,0,0,0,0,1}; // push to 2464
	static int x1_2896_push[32]	= {0,0,1,1,0,0,0,1,0,1,1,1,0,1,1,1,0,0,0,0,0,1,1,0,0,1,1,1,1,1,0,1}; // push to 2896

	static int x1[3][32], x2[3][32];
	#pragma SET_DATA_SECTION()

	m = flag;

	if (reset)
	{
		if(Nc == 1600)			x1_init_p = x1_1600_push;
		else if(Nc == 2080)		x1_init_p = x1_2080_push;
		else if(Nc == 2560)		x1_init_p = x1_2560_push;
		else if(Nc == 3040)		x1_init_p = x1_3040_push;
		else if(Nc == 2032)		x1_init_p = x1_2032_push;
		else if(Nc == 2464)		x1_init_p = x1_2464_push;
		else if(Nc == 2896)		x1_init_p = x1_2896_push;
		else					assert(0);

		// push x1 to Nc
		for (i=1; i<32; i++)
		{
			x1[m][i] = x1_init_p[i];
		}		
		
		// push x2 to Nc
		for (i=0; i<31; i++)
		{
			x2[m][i] = cinit%2;
			cinit /= 2;
		}

		x2[m][31] = x2[m][3] ^ x2[m][2] ^ x2[m][1] ^ x2[m][0];


		len = Nc - 32;

		for(i=0; i<len; i++)
		{
			x2[m][i%32] = x2[m][(i+4)%32] ^ x2[m][(i+3)%32] ^ x2[m][(i+2)%32] ^ x2[m][(i+1)%32];
		}

		// shuffle x2 for next push
		j = len%32;		
		if(j != 0)
		{
			for (i=0; i<32; i++)
			{
				x2_t[i] = x2[m][(i+j)%32];
			}

			for (i=0; i<32; i++)
			{
				x2[m][i] = x2_t[i];
			}
		}
	}

	// scrambling
	for (i=0; i<inlen; i++)
	{
		x1[m][i%32] = x1[m][(i+4)%32] ^ x1[m][(i+1)%32];
		x2[m][i%32] = x2[m][(i+4)%32] ^ x2[m][(i+3)%32] ^ x2[m][(i+2)%32] ^ x2[m][(i+1)%32];
		
		outbuf[i] = inbuf[i] * (1-2*(x1[m][i%32]^x2[m][i%32]));
	}

	// shuffle x1 and x2 for next scrambling
	j = inlen%32;
	for (i=0; i<32; i++)
	{
		x1_t[i] = x1[m][(i+j)%32];
		x2_t[i] = x2[m][(i+j)%32];
	}

	for (i=0; i<32; i++)
	{
		x1[m][i] = x1_t[i];
		x2[m][i] = x2_t[i];
	}

	return inlen;
}

int descram_pcfich(float *inbuf, int inlen, int num_id_cell, int subfrm_idx, float *outbuf)
{
	int cinit;

	cinit = (subfrm_idx + 1) * (2 * num_id_cell + 1)*(1<<9) + num_id_cell;

	descram_core(inbuf, inlen, DESCRAMBLE_RESET, DESCRAMB_CTRCH, cinit, UERX_NC_DEFAULT, outbuf);

	return inlen;
}

int descram_pdcch(float *inbuf, int inlen, int num_id_cell, int subfrm_idx, float *outbuf)
{
	int cinit;

	cinit = subfrm_idx * (1<<9) + num_id_cell;

	descram_core(inbuf, inlen, DESCRAMBLE_RESET, DESCRAMB_CTRCH, cinit, UERX_NC_DEFAULT, outbuf);
	
	return inlen;
}

int descram_pdsch(float *inbuf, int inlen, int reset, int num_id_cell, int subfrm_idx
								, int idx_codeword, int num_rnti, float *outbuf)
{
	int cinit;	
	int flag;

	if (idx_codeword == 0)
		flag = DESCRAMB_CW0_PDSCH;
	else
		flag = DESCRAMB_CW1_PDSCH;

	cinit = num_rnti * (1<<14) + idx_codeword*(1<<13) + subfrm_idx*(1<<9) + num_id_cell;

	descram_core(inbuf, inlen, reset, flag, cinit, UERX_NC_DEFAULT, outbuf);
	
	return inlen;
}

int descram_pbch(float *inbuf, int inlen, int reset, int num_id_cell, int cp_cfg, int frm_idx
							   , float *outbuf)
{
	int cinit;
	int Nc;

	if(cp_cfg == UERX_NORMAL_CP)		
		Nc = UERX_NC_DEFAULT + 480*(frm_idx%4);
	else
		Nc = UERX_NC_DEFAULT + 432*(frm_idx%4);

	cinit = num_id_cell;

	descram_core(inbuf, inlen, reset, DESCRAMB_PBCH, cinit, Nc, outbuf);
	
	return inlen;
}
