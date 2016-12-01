#include "foo.h"
#include "bstx_define.h"
#include "bstx_encode.h"



int encode_pcfich(int num_ofdm_pdcch, int num_rb_dl, int16 *outbuf)
{
    int i;
	int cfi;
	
	static int16 cfi_buf1[32] = {0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1};
    static int16 cfi_buf2[32] = {1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0};
	static int16 cfi_buf3[32] = {1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1};
	static int16 cfi_buf4[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	if(num_rb_dl <= 10)
		cfi = num_ofdm_pdcch - 1;
	else
		cfi = num_ofdm_pdcch;


	if (cfi == 1)
	{
		for (i=0; i<32; i++)
		{
			outbuf[i] = cfi_buf1[i];
		}
	}
	else if (cfi == 2)
	{
		for (i=0; i<32; i++)
		{
			outbuf[i] = cfi_buf2[i];
		}
	}
	else if (cfi == 3)
	{
		for (i=0; i<32; i++)
		{
			outbuf[i] = cfi_buf3[i];
		}
	}
	else
	{
		for (i=0; i<32; i++)
		{
			outbuf[i] = cfi_buf4[i];
		}
	}
	
	return 32;
}


int encode_phich(int hi, int16 *outbuf)
{    
	outbuf[0] = hi;
	outbuf[1] = hi;
	outbuf[2] = hi;
	
	return 3;
}

int encode_pdcch(int16 *inbuf, int inlen, int16 *d0_outbuf, int16 *d1_outbuf, int16 *d2_outbuf)
{
	int outlen;

	outlen = encode_convo(inbuf, inlen, d0_outbuf, d1_outbuf, d2_outbuf);

	return outlen;
}

int encode_pbch(int16 *inbuf, int inlen, int16 *d0_outbuf, int16 *d1_outbuf, int16 *d2_outbuf)
{
	int outlen;

	outlen = encode_convo(inbuf, inlen, d0_outbuf, d1_outbuf, d2_outbuf);

	return outlen;
}

int encode_pdsch(int16 **inbuf, int *size_each_codseg, int num_codseg, int num_nullbit
								, int16 **d0_outbuf, int16 **d1_outbuf, int16 **d2_outbuf)
{
	int outlen;

	outlen = encode_turbo(inbuf, size_each_codseg, num_codseg, num_nullbit, d0_outbuf, d1_outbuf, d2_outbuf);

	return outlen;
}


int encode_convo(int16 *inbuf, int inlen, int16 *d0_outbuf, int16 *d1_outbuf, int16 *d2_outbuf)
{
    int i;
	int s[6];

	// Init shift register
	s[0] = inbuf[inlen-1];
	s[1] = inbuf[inlen-2];
	s[2] = inbuf[inlen-3];
	s[3] = inbuf[inlen-4];
	s[4] = inbuf[inlen-5];
	s[5] = inbuf[inlen-6];

	// tail biting convolutional coding
	for(i=0; i<inlen; i++)
	{
		d0_outbuf[i] = inbuf[i] ^ s[1] ^ s[2] ^ s[4] ^ s[5];
		d1_outbuf[i] = inbuf[i] ^ s[0] ^ s[1] ^ s[2] ^ s[5];
		d2_outbuf[i] = inbuf[i] ^ s[0] ^ s[1] ^ s[3] ^ s[5];

		s[5] = s[4];
		s[4] = s[3];
		s[3] = s[2];
		s[2] = s[1];
		s[1] = s[0];
		s[0] = inbuf[i];
	}

	return inlen;
}




int encode_turbo(int16 **inbuf, int *size_each_codseg, int num_codseg, int num_nullbit
								, int16 **d0_outbuf, int16 **d1_outbuf, int16 **d2_outbuf)
{
	int i, j;
	int m; // null bit index
	int site_nullbit[1000];

	static int16 x_buf_mem[13][7000];
	static int16 z_buf_mem[13][7000];
	static int16 x1_buf_mem[13][7000];
	static int16 z1_buf_mem[13][7000];
	static int16 intlv_buf_mem[13][7000];

	static int16 *x_buf[13];
	static int16 *z_buf[13];	
	static int16 *x1_buf[13];
	static int16 *z1_buf[13];
	static int16 *intlv_buf[13];

	for(i=0; i<13; i++)
	{
		x_buf[i] = x_buf_mem[i];
		z_buf[i] = z_buf_mem[i];
		x1_buf[i] = x1_buf_mem[i];
		z1_buf[i] = z1_buf_mem[i];
		intlv_buf[i] = intlv_buf_mem[i];
	}

	interleaver_turbo(inbuf, size_each_codseg, num_codseg, intlv_buf, num_nullbit, site_nullbit);

	sub_encoder_turbo(inbuf, size_each_codseg, num_codseg, x_buf, z_buf);

    sub_encoder_turbo(intlv_buf, size_each_codseg, num_codseg, x1_buf, z1_buf);

	// turbo encoding
	for(i=0; i<num_codseg; i++)
	{
		for (j=0; j<size_each_codseg[i]; j++)
		{
			d0_outbuf[i][j] = x_buf[i][j];
		    d1_outbuf[i][j] = z_buf[i][j];
			d2_outbuf[i][j] = z1_buf[i][j];
		}
		
		// j = size_each_codseg[i]
		d0_outbuf[i][j+0] = x_buf[i][j+0];
        d0_outbuf[i][j+1] = z_buf[i][j+1];
        d0_outbuf[i][j+2] = x1_buf[i][j+0];
        d0_outbuf[i][j+3] = z1_buf[i][j+1]; 
                                    
		d1_outbuf[i][j+0] = z_buf[i][j+0];
        d1_outbuf[i][j+1] = x_buf[i][j+2];
        d1_outbuf[i][j+2] = z1_buf[i][j+0];
        d1_outbuf[i][j+3] = x1_buf[i][j+2]; 

		d2_outbuf[i][j+0] = x_buf[i][j+1];
        d2_outbuf[i][j+1] = z_buf[i][j+2];
        d2_outbuf[i][j+2] = x1_buf[i][j+1];
        d2_outbuf[i][j+3] = z1_buf[i][j+2]; 
	}

	// set the first block segment null bit as -1
	for(i=0; i<num_nullbit; i++)
	{
		m = site_nullbit[i];

		d0_outbuf[0][i] = -1;
		d1_outbuf[0][i] = -1;
		d2_outbuf[0][m] = -1;
	}
	
	return 0;	
}




int sub_encoder_turbo(int16 **inbuf, int *size_each_codseg, int num_codseg, int16 **x_outbuf
								   , int16 **z_outbuf)
{
	int i, j; 
    int s0, s1, s2;

	int feekbit;	// feekback bit to state register 0

	for(i=0; i<num_codseg; i++)
	{
		// clear the state register
		s0 = s1 = s2 = 0;

		// sub turbo encode
		for(j=0; j<size_each_codseg[i]; j++)
		{			
			x_outbuf[i][j] = inbuf[i][j];        
			z_outbuf[i][j] = inbuf[i][j] ^ s0 ^ s1;

			// updata state register
			feekbit = inbuf[i][j] ^ s1 ^ s2;
			s2 = s1;
			s1 = s0;
			s0 = feekbit;
		}

		x_outbuf[i][j] = s1 ^ s2;        
		z_outbuf[i][j] = s0 ^ s2;
		j++;

		x_outbuf[i][j] = s0 ^ s1;        
		z_outbuf[i][j] = s1;
		j++;

		x_outbuf[i][j] = s0;        
		z_outbuf[i][j] = s0;
		j++;
	}
    
	return 0;
}


int interleaver_turbo(int16 **inbuf, int *size_each_codseg, int num_codseg, int16 **outbuf
									, int num_nullbit, int *site_nullbit)
{
    int i, j;
	int f1, f1_sub, f1_plus, f2, f2_sub, f2_plus, m;
	
	// get f1 and f2
	for(i=0; i<188; i++)
	{
		if(BSTX_TURBO_INTLV_PARAMETER[i][1] == size_each_codseg[0])
		{
			f1_sub = BSTX_TURBO_INTLV_PARAMETER[i][2];
			f2_sub = BSTX_TURBO_INTLV_PARAMETER[i][3];

			f1_plus = BSTX_TURBO_INTLV_PARAMETER[i+1][2];
			f2_plus = BSTX_TURBO_INTLV_PARAMETER[i+1][3];
			break;
		}
	}

	// interleaver for every code segment
	for(i=0; i<num_codseg; i++)
	{
		if(size_each_codseg[i] == size_each_codseg[0])
		{
			f1 = f1_sub;
			f2 = f2_sub;
		}
		else
		{
			f1 = f1_plus;
			f2 = f2_plus;
		}
	
		for(j=0; j<size_each_codseg[i]; j++)
		{
			m = j*((f1+f2*j)%size_each_codseg[i]) % size_each_codseg[i];

			outbuf[i][j] = inbuf[i][m];

			// first segment perhapes has null bit
			if (i == 0)
			{
				if (j < num_nullbit)
				{
					site_nullbit[j] = m;
				}
			}
		}
	}

	return 0;	
}
