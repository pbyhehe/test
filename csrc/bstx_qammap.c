#include "foo.h"
#include "bstx_define.h"
#include "bstx_qammap.h"




int qammodu_core(int16 *inbuf, int inlen, int qm, cfloat *outbuf)
{
	int i, j;
	int idx_I, idx_Q;
	int outlen;
	
	int qam64_table[8] = {+3, +1, +5, +7, -3, -1, -5, -7};
	int qam16_table[4] = {+1, +3, -1, -3};
	int qpsk_table[2]  = {+1, -1};

	if(qm == BSTX_QAM64_BITS)
	{
		outlen = inlen / 6;

		for(i=0; i<outlen; i++)
		{
			j = 6*i;

			idx_I = (inbuf[j+0]<<2) | (inbuf[j+2]<<1) | inbuf[j+4];
			idx_Q = (inbuf[j+1]<<2) | (inbuf[j+3]<<1) | inbuf[j+5];

			// outbuf[i].re = 0.1543 * qam64_table[idx_I]; // orig
			// outbuf[i].im = 0.1543 * qam64_table[idx_Q];
			
			outbuf[i].re = (1/sqrt(42)) * qam64_table[idx_I];
			outbuf[i].im = (1/sqrt(42)) * qam64_table[idx_Q];
		}
	}
	else if(qm == BSTX_QAM16_BITS)
	{
		outlen = inlen / 4;

		for(i=0; i<outlen; i++)
		{
			j = 4*i;

			idx_I = (inbuf[j+0]<<1) | inbuf[j+2];
			idx_Q = (inbuf[j+1]<<1) | inbuf[j+3];

			// outbuf[i].re = 0.3162 * qam16_table[idx_I];
			// outbuf[i].im = 0.3162 * qam16_table[idx_Q];
			
			outbuf[i].re = (1/sqrt(10)) * qam16_table[idx_I];
			outbuf[i].im = (1/sqrt(10)) * qam16_table[idx_Q];
		}
	}
	else if(qm == BSTX_QPSK_BITS)
	{
		outlen = inlen / 2;

		for(i=0; i<outlen; i++)
		{
			j = 2*i;
//			outbuf[i].re = 0.7071 * qpsk_table[inbuf[j+0]]; // orig
//			outbuf[i].im = 0.7071 * qpsk_table[inbuf[j+1]];

			outbuf[i].re = (1/sqrt(2.0)) * qpsk_table[inbuf[j+0]];
			outbuf[i].im = (1/sqrt(2.0)) * qpsk_table[inbuf[j+1]];
		}
	}
	else if(qm == BSTX_BPSK_BITS)
	{
		outlen = inlen;

		for(i=0; i<outlen; i++)
		{
			j = i;

//			outbuf[i].re = 0.7071 * qpsk_table[inbuf[j]]; // orig
//			outbuf[i].im = 0.7071 * qpsk_table[inbuf[j]];
			
			outbuf[i].re = (1/sqrt(2)) * qpsk_table[inbuf[j]];
			outbuf[i].im = (1/sqrt(2)) * qpsk_table[inbuf[j]];
		}
	}
	else
	{
		assert(0);
	}	

	return outlen;
}




int qammap_pdsch(int16 *inbuf, int inlen, int qm, cfloat *outbuf)
{	
	int outlen;
	
	outlen = qammodu_core(inbuf, inlen, qm, outbuf);
	
	return outlen;
}

int qammap_pdcch(int16 *inbuf, int inlen, cfloat *outbuf)
{	
 	int outlen;

	outlen = qammodu_core(inbuf, inlen, BSTX_QPSK_BITS, outbuf);

	return outlen;
}

int qammap_pbch(int16 *inbuf, int inlen, cfloat *outbuf)
{	
 	int outlen;

	outlen = qammodu_core(inbuf, inlen, BSTX_QPSK_BITS, outbuf);

	return outlen;
}

int qammap_phich(int16 *inbuf, int inlen, cfloat *outbuf)
{	
 	int outlen;

	outlen = qammodu_core(inbuf, inlen, BSTX_BPSK_BITS, outbuf);

	return outlen;
}

int qammap_pcfich(int16 *inbuf, int inlen, cfloat *outbuf)
{	
 	int outlen;

	outlen = qammodu_core(inbuf, inlen, BSTX_QPSK_BITS, outbuf);

	return outlen;
}
