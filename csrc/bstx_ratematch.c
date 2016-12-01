#include "foo.h"
#include "bstx_define.h"
#include "bstx_ratematch.h"

#define min(a, b)				(( (a) < (b) )? (a): (b))

int ratematch_pdsch(int16 **inbuf0, int16 **inbuf1, int16 **inbuf2, int *len_codeseg, int num_codseg
								, int rv_idx, int qm, int catgry_ue, int num_harq_process, int G_pdsch, int num_layer
								, int k_mimo, int16 **outbuf, int *len_ratematch)
{ 
	int i;
	int len_intlvseg[25];

	static int16 outbuf0_intlv_mem[13][7000];
	static int16 outbuf1_intlv_mem[13][7000];
	static int16 outbuf2_intlv_mem[13][7000];
	static int16 outbuf_collect_mem[13][21000];

	static int16 *outbuf0_intlv[13];
	static int16 *outbuf1_intlv[13];	
	static int16 *outbuf2_intlv[13];
	static int16 *outbuf_collect[13];

	for(i=0; i<13; i++)
	{
		outbuf0_intlv[i] = outbuf0_intlv_mem[i];
		outbuf1_intlv[i] = outbuf1_intlv_mem[i];
		outbuf2_intlv[i] = outbuf2_intlv_mem[i];
		outbuf_collect[i] = outbuf_collect_mem[i];
	}
			
	// function body
	subblock_intlv_1th_2th_turbo(inbuf0, len_codeseg, num_codseg, outbuf0_intlv, len_intlvseg);
// 	save.Run(outbuf0_intlv[0], len_intlvseg[0], "ratematch_intlv0_old", TO_SAVE);

	subblock_intlv_1th_2th_turbo(inbuf1, len_codeseg, num_codseg, outbuf1_intlv, len_intlvseg);
// 	save.Run(outbuf1_intlv[0], len_intlvseg[0], "ratematch_intlv1_old", TO_SAVE);

	subblock_intlv_the_3th_turbo(inbuf2, len_codeseg, num_codseg, outbuf2_intlv, len_intlvseg);
// 	save.Run(outbuf2_intlv[0], len_intlvseg[0], "ratematch_intlv2_old", TO_SAVE);

	bstx_bit_collect_turbo(outbuf0_intlv, outbuf1_intlv, outbuf2_intlv, len_intlvseg, num_codseg, outbuf_collect);
// 	save.Run(outbuf_collect[0], len_intlvseg[0] * 3, "ratematch_bit_clct_old", TO_SAVE);

	bit_select_turbo(outbuf_collect, len_intlvseg, num_codseg, rv_idx, qm, num_layer, num_harq_process
		, G_pdsch, k_mimo, catgry_ue, outbuf, len_ratematch); 

	return 0;
}

int ratematch_pdcch(int16 *inbuf0, int16 *inbuf1, int16 *inbuf2, int inlen, int pdcch_format, int16 *outbuf)
{ 
	int outlen;
	int outlen_intlv;
	int outlen_collect;
	
	static int16 outbuf0_intlv[128];
	static int16 outbuf1_intlv[128];
	static int16 outbuf2_intlv[128];
	static int16 outbuf_collect[384];

	outlen_intlv = sub_block_interleaver_convlution(inbuf0, inlen, outbuf0_intlv);
	outlen_intlv = sub_block_interleaver_convlution(inbuf1, inlen, outbuf1_intlv);
	outlen_intlv = sub_block_interleaver_convlution(inbuf2, inlen, outbuf2_intlv);

	outlen_collect = bit_collect_convolution(outbuf0_intlv, outbuf1_intlv, outbuf2_intlv, outlen_intlv, outbuf_collect);
	
	outlen = (1 << pdcch_format) * 72;

    bit_select_convolution(outbuf_collect, outlen_collect, outbuf, outlen);

	return outlen;
}


int ratematch_pbch(int16 *inbuf0, int16 *inbuf1, int16 *inbuf2, int inlen, int cp_cfg, int16 *outbuf)
{ 
	int outlen;
	int outlen_intlv;
	int outlen_collect;
	
	static int16 outbuf0_intlv[64];
	static int16 outbuf1_intlv[64];
	static int16 outbuf2_intlv[64];
	static int16 outbuf_collect[192];

	outlen_intlv = sub_block_interleaver_convlution(inbuf0, inlen, outbuf0_intlv);
	outlen_intlv = sub_block_interleaver_convlution(inbuf1, inlen, outbuf1_intlv);
	outlen_intlv = sub_block_interleaver_convlution(inbuf2, inlen, outbuf2_intlv);

	outlen_collect = bit_collect_convolution(outbuf0_intlv, outbuf1_intlv, outbuf2_intlv, outlen_intlv, outbuf_collect);		

	if(cp_cfg==0)	outlen = 1920;
	else            outlen = 1728;

    bit_select_convolution(outbuf_collect, outlen_collect, outbuf, outlen);

	return outlen;
}

int subblock_intlv_1th_2th_turbo(int16 **inbuf, int *len_codeseg, int num_codseg, int16 **outbuf, int *len_intlvseg)
{
	int i, j, k, m;
	int D;	// the number bit of every input segment
	int D1; // the number of dummy bit
	int num_row; // the row number of interleaver
	int num_matrix; // the size of matrix
	
	int16 buffer[200][32];

	// inter column permutation pattern for sub-block interleaver
	int p[32] = {0,16,8,24,4,20,12,28,2,18,10,26,6,22,14,30,1,17,9,25,5,21,13,29,3,19,11,27,7,23,15,31};

	// function body
	for(k=0; k<num_codseg; k++)
	{
		D = len_codeseg[k]+4;
		num_row = (D+31) / 32;
		num_matrix = num_row * 32;
		D1 = num_row*32 - D;

		// dummy bits are padded
		for(i=0; i<num_row; i++)
		{
			for(j=0; j<32; j++)
			{
				m = i*32+j - D1;
				
				if(m < 0) buffer[i][j] = -1;
				else	  buffer[i][j] = inbuf[k][m];
			}
		}
		
		m = 0; // clear the index for every segment output

		// column exchange and output the interleaver result
		for(j=0; j<32; j++)
		{
			for(i=0; i<num_row; i++)
			{
				outbuf[k][m] = buffer[i][p[j]];
				m++;
			}
		}

		len_intlvseg[k]	 = num_matrix;
	}

	return 0;
}


int subblock_intlv_the_3th_turbo(int16 **inbuf, int *len_codeseg, int num_codseg, int16 **outbuf, int *len_intlvseg)
{
	int i, k, m;
	int D;	// the number bit of every input segment
	int D1; // the number of dummy bit
	int num_row; // the row number of interleaver
	int num_matrix; // the size of matrix

	int16 buffer[6400];

	// inter column permutation pattern for sub-block interleaver
	int p[32] = {0,16,8,24,4,20,12,28,2,18,10,26,6,22,14,30,1,17,9,25,5,21,13,29,3,19,11,27,7,23,15,31};

	// function body
	for(k=0; k<num_codseg; k++)
	{
		D = len_codeseg[k]+4;
		num_row = (D+31) / 32;
		num_matrix = num_row * 32;
		D1 = num_row*32 - D;

		// dummy bit are padded
		for(i=0; i<D1; i++)
		{
			buffer[i] = -1;
		}
			
		for(i=D1; i<num_matrix; i++)
		{
			buffer[i] = inbuf[k][i-D1];
		}

		// get inbuf data
		for(i=0; i<num_matrix; i++)
		{
			m = (p[i/num_row] + 32*(i%num_row) + 1) % num_matrix;
            outbuf[k][i] = buffer[m];
		}

		len_intlvseg[k]	 = num_matrix;
	}

	return 0;
}

int bstx_bit_collect_turbo(int16 **inbuf0, int16 **inbuf1, int16 **inbuf2, int *len_intlvseg, int num_codseg
									  , int16 **outbuf)
{
	int i, j;
	
	// function body
	for(i=0; i<num_codseg; i++)
	{
		for(j=0; j<len_intlvseg[i]; j++)
		{
			outbuf[i][j]                     = inbuf0[i][j];
			outbuf[i][2*j+0+len_intlvseg[i]] = inbuf1[i][j];
			outbuf[i][2*j+1+len_intlvseg[i]] = inbuf2[i][j];
		}			    
	}

	return 0;
}



int bit_select_turbo(int16 **inbuf, int *len_intlvseg, int num_codseg, int rv_idx, int qm
				, int num_layer, int num_harq_process, int G_pdsch, int k_mimo, int catgry_ue
				, int16 **outbuf, int *len_selctseg)
{
	int i, j, k, k0, Er, r;
	int N_soft, M_limit, N_IR;
	int G1_pdsch, Kw, Ncb;
	int num_row;

	// Total number of soft channel bits
	if(catgry_ue==1)		N_soft = 250368;  
	else if(catgry_ue==2)	N_soft = 1237248;
	else if(catgry_ue==3)   N_soft = 1237248;
	else if(catgry_ue==4)	N_soft = 1827072;
	else if(catgry_ue==5)	N_soft = 3667200;
	else					assert(0);

	M_limit = 8;

	N_IR = N_soft / (k_mimo*min(num_harq_process,M_limit));

	G1_pdsch = G_pdsch / (num_layer*qm);

	r = G1_pdsch % num_codseg;

	for(i=0; i<num_codseg; i++)
	{
    	num_row = len_intlvseg[i] / 32;
		Kw = 3*len_intlvseg[i];

		Ncb = min(N_IR/num_codseg, Kw);

		// the rate matching output sequence length for the r-th coded block
		if(i <= (num_codseg-r-1))   Er = G1_pdsch/num_codseg * num_layer*qm;  
		else					    Er = (G1_pdsch+num_codseg-1)/num_codseg * num_layer*qm;

		k0 = num_row * ((Ncb+8*num_row-1)/(8*num_row) *2*rv_idx + 2); 

        k = j = 0;
        while(k < Er)
		{
			if(inbuf[i][(k0+j)%Ncb] != -1)
			{
				outbuf[i][k] = inbuf[i][(k0+j)%Ncb];
				k++;
				j++;
			}
			else
			{
				j++;
			}
		}
		
		len_selctseg[i] = Er;
	}

	return 0;
}

int sub_block_interleaver_convlution(int16 *inbuf, int inlen, int16 *outbuf)
{
	int i, j, m;
	int D;			// the number bit of every input segment
	int D1;			// the number of dummy bit
	int num_row;	// the row number of interleaver
	int16 buffer[200][32];

	// inter column permutation pattern for sub-block interleaver
	int p[32] = {1,17,9,25,5,21,13,29,3,19,11,27,7,23,15,31,0,16,8,24,4,20,12,28,2,18,10,26,6,22,14,30};

	D = inlen;
	num_row = (D+31) / 32;
	D1 = num_row * 32 - D;

	// dummy bits are padded
	for(i=0; i<num_row; i++)
	{
		for(j=0; j<32; j++)
		{
			m = i*32+j - D1;
			
			if(m < 0) buffer[i][j] = -1;
			else	  buffer[i][j] = inbuf[m];
		}
	}
	
	// clear the index for every segment output
	m = 0; 

	// column exchange and output the interleaver result
	for(j=0; j<32; j++)
	{
		for(i=0; i<num_row; i++)
		{
			outbuf[m++] = buffer[i][p[j]];
		}
	}

	return m;
}


int bit_collect_convolution(int16 *inbuf0, int16 *inbuf1, int16 *inbuf2, int inlen, int16 *outbuf)
{
	int i;
	
	for(i=0; i<inlen; i++)
	{
		outbuf[i+0*inlen] = inbuf0[i];
		outbuf[i+1*inlen] = inbuf1[i];
		outbuf[i+2*inlen] = inbuf2[i];
	}

	return (3*inlen);
}


int bit_select_convolution(int16 *inbuf, int inlen, int16 *outbuf, int outlen)
{
	int j, k;

	k = j = 0;
    while(k < outlen)
	{
		if(inbuf[j%inlen] != -1)
		{
			outbuf[k] = inbuf[j%inlen];
			k++;
			j++;
		}
		else
		{
			j++;
		}
	}

    return outlen;
}
