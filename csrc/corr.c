#include <stdlib.h>
#include <string.h>
#include "sharefunc.h"
#include "typedefine.h"
#include "corr.h"

//corr1 corr;
int corr_Init(corr1 *corr,cfloat *coefbuf, int len)
{
	int i;

	(*corr).M_statptr = 0;

	(*corr).M_coeflen = len;
	(*corr).M_coef    = (cfloat*)malloc((*corr).M_coeflen*sizeof(cfloat));
	(*corr).M_stat    = (cfloat*)malloc((*corr).M_coeflen*sizeof(cfloat));
	
	for(i = 0; i < len; i++)
	{
		(*corr).M_coef[i] = coefbuf[i];
		(*corr).M_stat[i] = ZEROCFLOAT;
	}

	return 0;
}

int corr_Clear(corr1 *corr)
{
	free((*corr).M_stat);
	free((*corr).M_coef);

	return 0;
}

cfloat corr_InputOutput1(corr1 *corr,cfloat in)
{
	cfloat out;
	float sum;
	int i, j;

	out.re        = 0.0;
	out.im        = 0.0;	
	(*corr).M_stat[(*corr).M_statptr] = in;
	sum           = 0.0;

	for(i = 0; i < (*corr).M_coeflen; i++)
	{
		j    = ((*corr).M_coeflen + (*corr).M_statptr + 1 + i) % (*corr).M_coeflen;
		cfloataddequalcfloat(&out,cfloatmultiplycfloat(conj1((*corr).M_coef[i]), (*corr).M_stat[j]));
		sum += abs5((*corr).M_stat[j]);
	}

	(*corr).M_statptr = ((*corr).M_statptr+1) % (*corr).M_coeflen;

	return out;
}

int corr_InputOutput2(corr1 *corr,cfloat *inbuf, int inlen, cfloat *outbuf)
{
	int i;
	
	for(i = 0; i < inlen; i++)
	{
		outbuf[i] = corr_InputOutput1(corr,inbuf[i]);
	}

	return inlen;
}

int corr_InputOutput3(corr1 *corr,cfloat *inbuf, int inlen, float *outbuf)
{
	int i;
	
	for(i = 0; i < inlen; i++)
	{
		outbuf[i] = abs5(corr_InputOutput1(corr,inbuf[i]));
	}

	return inlen;
}

int corr_InputOutput5(corr1 *corr,cfloat *inbuf, int inlen, int32 *outbuf)
{
	int i;
 	
	for(i = 0; i < inlen; i++)
	{
		outbuf[i] = corr_InputOutput4(corr,inbuf[i]);
	}

	return inlen;
}

int32 corr_InputOutput4(corr1 *corr,cfloat in)
{
	cint32 out;
	int out_pow;
	cint32 d1, d2;
	int    i, j;

	out.re        = 0;
	out.im        = 0;	
	(*corr).M_stat[(*corr).M_statptr] = in;

	for(i = 0; i < (*corr).M_coeflen; i++)
	{
		j = ((*corr).M_coeflen + (*corr).M_statptr + 1 + i) % (*corr).M_coeflen;

		d1.re = (*corr).M_coef[i].re >= 0 ? 1 : -1;
		d1.im = (*corr).M_coef[i].im >= 0 ? 1 : -1;
		d2.re = (*corr).M_stat[j].re >= 0 ? 1 : -1;
		d2.im = (*corr).M_stat[j].im >= 0 ? 1 : -1;

		cint32addequalcint32(&out,cint32multiplycint32(conj2(d1),d2));
	}

	(*corr).M_statptr = ((*corr).M_statptr+1) % (*corr).M_coeflen;

	out_pow = out.re*out.re+out.im*out.im;

	return out_pow;
}




// int corr::SetParameter(cfloat *coefbuf, int len)
// {
// 	int i;
// 
// 	M_coeflen = len;
// 	M_coef    = new cfloat[M_coeflen];
// 	M_stat    = new cfloat[M_coeflen];
// 
// 	for(i = 0; i < len; i++)
// 	{
// 		M_coef[i] = coefbuf[i];
// 		M_stat[i] = ZEROCFLOAT;
// 	}
// 
// 	return 0;
// }

// int corr::SetParameter()
// {	
// 	return 0;
// }
