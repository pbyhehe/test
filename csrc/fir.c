#include <string.h>
#include <math.h>
#include "sharefunc.h"
#include "typedefine.h"
#include "fir.h"

int fir_SetParameter( fir1 *fir,char * filename)
{
	strcpy((*fir).coeffilename, filename);
	return 0;
}

int fir_Init(fir1 *fir)
{
	FILE *fp;
	int i;
	float temp;

	if((fp = fopen((*fir).coeffilename, "r")) == NULL)
	{
//		PrintInfo(1, "fir:: error!\n");
		return -1;
	}

	fscanf(fp, "%d", &(*fir).coeflen);

	(*fir).stat= (cfloat*)malloc((*fir).coeflen*sizeof(cfloat));
	(*fir).coef =(cfloat*)malloc((*fir).coeflen*sizeof(cfloat));

	for(i = 0; i < (*fir).coeflen; i++)
	{
		(*fir).stat[i] = ZEROCFLOAT;
		(*fir).coef[i] = ZEROCFLOAT;
	}

	for(i = 0; i < (*fir).coeflen; i++)
	{
		fscanf(fp, "%f", &temp);
		(*fir).coef[i].re = temp;
		(*fir).coef[i].im = 0.0;
	}
	
	fclose(fp);
	
	(*fir).statptr = 0;

	return 0;
}

int fir_Clear(fir1 *fir)
{
	free((*fir).stat);
	free((*fir).coef);

	return 0;
}

int Reset(fir1 *fir)
{
	int i;

	for(i = 0; i < (*fir).coeflen; i++)
	{
		(*fir).stat[i] = ZEROCFLOAT;
	}

	return 0;
}

cfloat fir_InputOutput1(fir1 *fir,cfloat in)
{
	cfloat out;
	int i, j;	
	
	(*fir).stat[(*fir).statptr] = in;

	out.re = 0.0;
	out.im = 0.0;	
	
	for(i = 0; i < (*fir).coeflen; i++)
	{
		j = ((*fir).coeflen + (*fir).statptr - i) % (*fir).coeflen;
		cfloataddequalcfloat(&out,cfloatmultiplycfloat((*fir).coef[i], (*fir).stat[j]));
	}

	(*fir).statptr = ((*fir).statptr+1) % (*fir).coeflen;

	return out;
}

int fir_InputOutput2(fir1 *fir,cfloat *inbuf, int inlen, cfloat *outbuf)
{
	int i;
	
	for(i = 0; i < inlen; i++)
		outbuf[i] = fir_InputOutput1(fir, inbuf[i]);

	return inlen;
}

float fir_InputOutput3(fir1 *fir,float in)
{
	float out;
	int i, j;

	out = 0.0;
	
	(*fir).stat[(*fir).statptr].re = in;

	for(i = 0; i < (*fir).coeflen; i++)
	{
		j = ((*fir).coeflen + (*fir).statptr - i) % (*fir).coeflen;
		out += (*fir).coef[i].re * (*fir).stat[j].re;
	}

	(*fir).statptr = ((*fir).statptr+1) % (*fir).coeflen;

	return out;
}

int fir_InputOutput4(fir1 *fir,float *inbuf, int inlen, float *outbuf)
{
	int i;
	
	for(i = 0; i < inlen; i++)
		outbuf[i] = fir_InputOutput3(fir,inbuf[i]);

	return inlen;
}
