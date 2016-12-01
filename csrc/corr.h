#pragma once

typedef struct corr1
{
	int M_statptr;
	int M_coeflen;
	cfloat *M_stat;
	cfloat *M_coef;
}corr1;
	int corr_Init(corr1 *corr,cfloat *coefbuf, int len);
	int corr_Clear(corr1 *corr);
	cfloat corr_InputOutput1(corr1 *corr,cfloat in);
	int corr_InputOutput2(corr1 *corr,cfloat *inbuf, int inlen, cfloat *outbuf);
	int corr_InputOutput3(corr1 *corr,cfloat *inbuf, int inlen, float *outbuf);
	int32 corr_InputOutput4(corr1 *corr,cfloat in);
	int corr_InputOutput5(corr1 *corr,cfloat *inbuf, int inlen, int32 *outbuf);
















//	int SetParameter();
// 	corr(){};
// 	virtual ~corr(){};
