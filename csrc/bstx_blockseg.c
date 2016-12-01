#include "foo.h"
#include "bstx_blockseg.h"


int blockseg_pdsch(int16 *inbuf, int inlen, int16 **outbuf, int *outlen_seg, int *num_seg
								  , int *num_nullbit)
{
	blockseg_core(inbuf, inlen, outbuf, outlen_seg, num_seg, num_nullbit);

	return 0;
}

int blockseg_core(int16 *inbuf, int inlen, int16 **outbuf, int *outlen_seg, int *num_seg
								 , int *num_nullbit)
{
	int B1, K_plus, K_sub, C_all, C_plus, C_sub;
	int i, j;
	int s;	// inbuf index
	int L, K;

	int16 generator_CRC24B[25] = {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1};
	
	if(inlen <= 6144)
	{
		for(i=0; i<188; i++)
		{
			if(BSTX_TURBO_INTLV_PARAMETER[i][1] >= inlen)
			{
				K_plus = BSTX_TURBO_INTLV_PARAMETER[i][1];
				break;
			}
		}

		(*num_nullbit) = K_plus - inlen;

		for(i=0; i<(*num_nullbit); i++)
		{
			outbuf[0][i] = 0;
		}

		for(i=(*num_nullbit); i<K_plus; i++)
		{
			outbuf[0][i] = inbuf[i-(*num_nullbit)];
		}

		// output parameter
		(*num_seg) = 1;
		outlen_seg[0] = K_plus;
	}
	else
	{
		L = 24;
		C_all = (inlen+6144-L-1) / (6144-L);
		B1 = inlen + C_all*L;

		for(i=0; i<188; i++)
		{
			if(C_all*BSTX_TURBO_INTLV_PARAMETER[i][1] >= B1)
			{
				K_plus = BSTX_TURBO_INTLV_PARAMETER[i][1];
				K_sub  = BSTX_TURBO_INTLV_PARAMETER[i-1][1];
				break;
			}
		}

		C_sub = (C_all*K_plus-B1) / (K_plus-K_sub);
		C_plus = C_all - C_sub;
		(*num_nullbit) = C_plus*K_plus + C_sub*K_sub - B1;
	
		s = 0;

		for(i=0; i<C_all; i++)
		{
			if(i < C_sub)		K = K_sub;
			else				K = K_plus;

			if(i == 0)
			{
				for(j=0; j<(*num_nullbit); j++)
				{
					outbuf[0][j] = 0;
				}

				for(j=0; j<K-L-(*num_nullbit); j++)
				{
					outbuf[0][(*num_nullbit)+j] = inbuf[s];
					s++;
				}
				
				crc_attach(outbuf[i], K-L, generator_CRC24B, 25, outbuf[i]);
			}
			else
			{
				crc_attach(&inbuf[s], K-L, generator_CRC24B, 25, outbuf[i]);

				s += K-L;
			}	
		}

		// output parameter
		(*num_seg) = C_all;

		for(i=0; i<C_all; i++)
		{
			if(i<C_sub)		outlen_seg[i] = K_sub;
			else			outlen_seg[i] = K_plus;
		}
	}	
	
	return 0;
}
