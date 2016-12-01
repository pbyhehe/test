#pragma once

typedef struct fir1
{
	int statptr;
	cfloat *stat;
	cfloat *coef;
	float *stat_f;
	float *coef_f;
	int coeflen;
	char coeffilename[256];
}fir1;

    int fir_SetParameter(fir1 *fir,char * coeffilename);
	int fir_Init(fir1 *fir);
	int fir_Clear(fir1 *fir);
	int Reset(fir1 *fir);
	cfloat fir_InputOutput1(fir1 *fir,cfloat in);
	int fir_InputOutput2(fir1 *fir,cfloat *inbuf, int inlen, cfloat *outbuf);
	float fir_InputOutput3(fir1 *fir,float in);
	int fir_InputOutput4(fir1 *fir,float *inbuf, int inlen, float *outbuf);

