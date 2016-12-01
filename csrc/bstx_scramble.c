#include "foo.h"
#include "bstx_define.h"
#include "bstx_scramble.h"

#define DUMP_SCRAM 0

int scramble_core(int16 *inbuf, int inlen, int cinit, int Nc, int16 *outbuf)
{
	//int tmp; //for test

	int i, j, len;
	int x2_t[32];
	int *x1_init;
	
	int x1[32], x2[32];

	int x1_1600_push[32] = {0,0,0,1,0,0,1,0,0,1,1,0,1,1,0,0,0,0,1,0,0,1,0,1,1,0,0,1,0,1,0,1}; // push 1600
	int x1_2080_push[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0}; // push to 2080
	int x1_2560_push[32] = {0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,1,0,0,0,0,0,0,0,1,0,1,0,0,1,1,0,0}; // push to 2560
	int x1_3040_push[32] = {0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,1,0,0,1,0,1,1,0,0,1,0,1,1}; // push to 3040
	int x1_2032_push[32] = {0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1}; // push to 2032
	int x1_2464_push[32] = {0,0,0,1,0,1,1,1,0,1,0,0,1,1,0,1,0,1,1,1,0,1,0,0,1,1,0,0,0,0,0,1}; // push to 2464
	int x1_2896_push[32] = {0,0,1,1,0,0,0,1,0,1,1,1,0,1,1,1,0,0,0,0,0,1,1,0,0,1,1,1,1,1,0,1}; // push to 2896	

	if(Nc == 1600)			x1_init = x1_1600_push;
	else if(Nc == 2080)		x1_init = x1_2080_push;
	else if(Nc == 2560)		x1_init = x1_2560_push;
	else if(Nc == 3040)		x1_init = x1_3040_push;
	else if(Nc == 2032)		x1_init = x1_2032_push;
	else if(Nc == 2464)		x1_init = x1_2464_push;
	else if(Nc == 2896)		x1_init = x1_2896_push;
	else					assert(0);

	// push x1 to Nc
	for (i=1; i<32; i++)
	{
		x1[i] = x1_init[i];
	}		
	
	// push x2 to Nc
	for (i=0; i<31; i++)
	{
		x2[i] = cinit%2;
		cinit /= 2;
	}

	x2[31] = x2[3] ^ x2[2] ^ x2[1] ^ x2[0];

	len = Nc - 32;

	for(i=0; i<len; i++)
	{
		x2[i%32] = x2[(i+4)%32] ^ x2[(i+3)%32] ^ x2[(i+2)%32] ^ x2[(i+1)%32];
	}

	// shuffle x2 for next push
	j = len%32;		
	if(j != 0)
	{
		for (i=0; i<32; i++)
		{
			x2_t[i] = x2[(i+j)%32];
		}

		for (i=0; i<32; i++)
		{
			x2[i] = x2_t[i];
		}
	}

	// scrambling
	for (i=0; i<inlen; i++)
	{
		x1[i%32] = x1[(i+4)%32] ^ x1[(i+1)%32];
		x2[i%32] = x2[(i+4)%32] ^ x2[(i+3)%32] ^ x2[(i+2)%32] ^ x2[(i+1)%32];
		
		outbuf[i] = inbuf[i] ^ (x1[i%32]^x2[i%32]);
		
// 		tmp = x1[i%32] ^ x2[i%32]; // zuohuanhuan
// 		if(DUMP_SCRAM)	save.Run(&tmp, 1, "PRSseq_tx", TO_SAVE); // zuohuanhuan
	}

	return inlen;
}



int scramble_pbch(int16 *inbuf, int inlen,  int num_ID_cell, int16 *outbuf)
{
	int cinit;
    int outlen;

	cinit = num_ID_cell;
	
	outlen = scramble_core(inbuf, inlen, cinit, 1600, outbuf);
	
	return outlen;
}

int scramble_pcfich(int16 *inbuf, int inlen,  int num_ID_cell, int subfrm_idx, int16 *outbuf)
{
	int cinit;
    int outlen;

	cinit = (subfrm_idx+1) * (2 * num_ID_cell + 1) * (1<<9) + num_ID_cell;
	
	outlen = scramble_core(inbuf, inlen, cinit, 1600, outbuf);
	
	return outlen;
}


int scramble_pdcch(int16 *inbuf, int inlen,  int num_ID_cell, int subfrm_idx, int16 *outbuf)
{
	int cinit;
    int outlen;

	cinit = subfrm_idx * (1<<9) + num_ID_cell;

	outlen = scramble_core(inbuf, inlen, cinit, 1600, outbuf);
	
	return outlen;
}

int scramble_pdsch(int16 *inbuf, int inlen, int num_ID_cell, int idx_codeword, int num_rnti
								  , int subfrm_idx, int16 *outbuf)
{
	int cinit;

	cinit = num_rnti * (1<<14) + idx_codeword*(1<<13) + subfrm_idx*(1<<9) + num_ID_cell;
		
	scramble_core(inbuf, inlen, cinit, 1600, outbuf);
		
	return inlen;
}

int scramble_phich(cfloat *inbuf, int inlen, int subfrm_idx, int num_ID_cell, int cp_cfg, cfloat *outbuf)
{
	int i;
	int cinit;
	int scrm_len;
	
	static int16 zero_buf[12] = {0}; // huangshoujun_server
	static int16 scrm_buf[12];

// 	static int16 zero_buf[60] = {0}; // zuohuanhuan
// 	static int16 scrm_buf[60];

	if(cp_cfg == BSTX_NORMAL_CP)		scrm_len = 12;
	else if(cp_cfg == BSTX_EXTEND_CP)	scrm_len = 6;
	else assert(0);

	cinit = (subfrm_idx+1)*(2*num_ID_cell+1)*(1<<9) + num_ID_cell; // it should be (subfrm_idx%10+1)..., linjiangnan
	
	scramble_core(zero_buf, scrm_len, cinit, 1600, scrm_buf);
// 	save.Run(scrm_buf, scrm_len, "bstx_itx_phich_scrambel_code", TO_SAVE);

	for (i=0; i<scrm_len; i++)
	{
		outbuf[i].re = inbuf[i].re * (1-2*scrm_buf[i]);
		outbuf[i].im = inbuf[i].im * (1-2*scrm_buf[i]);
	}

	
	return scrm_len;
}
