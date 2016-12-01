#include <string.h>
#include "fifo.h"


int fifo_SetParameter(fifo1 *fifo,int pow, int tp)
{
	(*fifo).powsize = pow;
	(*fifo).type = tp;
	(*fifo).size = 2 << ((*fifo).powsize - 1);
	(*fifo).mask = (2 << ((*fifo).powsize - 1)) - 1;
	return 0; 
}

int fifo_Init(fifo1 *fifo)
{
	switch((*fifo).type)
	{
	case CFLOAT_TYPE:
		(*fifo).cfltbuf =(cfloat *) malloc((*fifo).size*sizeof(cfloat));
		memset((*fifo).cfltbuf, 0, sizeof(cfloat)*(*fifo).size);
		break;
	case CINT32_TYPE:
		(*fifo).cint32buf =(cint32*) malloc((*fifo).size*sizeof(cint32));
		memset((*fifo).cint32buf, 0, sizeof(cint32)*(*fifo).size);
		break;
	case CINT16_TYPE:
		(*fifo).cint16buf =(cint16*) malloc((*fifo).size*sizeof(cint16));
		memset((*fifo).cint16buf, 0, sizeof(cint16)*(*fifo).size);
		break;
	case FLOAT_TYPE:
		(*fifo).fltbuf = (float*)malloc((*fifo).size*sizeof(float));
		memset((*fifo).fltbuf, 0, sizeof(float)*(*fifo).size);
		break;
	case INT32_TYPE:
		(*fifo).int32buf = (int32*)malloc((*fifo).size*sizeof(int32));
		memset((*fifo).int32buf, 0, sizeof(int32)*(*fifo).size);
		break;
	case INT16_TYPE:
		(*fifo).int16buf = (int16*)malloc((*fifo).size*sizeof(int16));
		memset((*fifo).int16buf, 0, sizeof(int16)*(*fifo).size);
		break;
	default:
		;
	}

	(*fifo).writeptr = 0;
	(*fifo).readptr = 0;
	(*fifo).ptrdiff = 0;

	return 0;
}

int fifo_Clear(fifo1 *fifo)
{
	switch((*fifo).type)
	{
	case CFLOAT_TYPE:
		free((*fifo).cfltbuf);
		break;
	case CINT32_TYPE:
		free((*fifo).cint32buf);
		break;
	case CINT16_TYPE:
		free((*fifo).cint16buf);
		break;
	case FLOAT_TYPE:
		free((*fifo).fltbuf);
		break;
	case INT32_TYPE:
		free((*fifo).int32buf);
		break;
	case INT16_TYPE:
		free((*fifo).int16buf);
		break;
	default:
		;
	}
	return 0;
}

int pop1(fifo1 *fifo,cfloat *outbuf, int buflen)
{
	int i;
	for(i = 0 ; i < buflen; i++)
	{
		outbuf[i] = (*fifo).cfltbuf[(*fifo).readptr];
		(*fifo).readptr = ((*fifo).readptr + (*fifo).size + 1) & (*fifo).mask;
	}
	(*fifo).ptrdiff = ((*fifo).writeptr + (*fifo).size - (*fifo).readptr) & (*fifo).mask;
	return (*fifo).ptrdiff;
}

int getfirst1(fifo1 *fifo,cfloat *outbuf, int buflen)
{
	int i;
	int rdpt = (*fifo).readptr;
	for(i = 0 ; i < buflen; i++)
	{
		outbuf[i] = (*fifo).cfltbuf[rdpt];
		rdpt = (rdpt + (*fifo).size + 1) & (*fifo).mask;
	}
	return buflen;
}

int getlast1(fifo1 *fifo,cfloat *outbuf, int buflen)
{
	int i;
	int rdpt = ((*fifo).writeptr + (*fifo).size - 1) & (*fifo).mask;
	for(i = buflen - 1; i >= 0; i--)
	{
		outbuf[i] = (*fifo).cfltbuf[rdpt];
		rdpt = (rdpt + (*fifo).size - 1) & (*fifo).mask;
	}
	return buflen;
}

int pop5(fifo1 *fifo,cint32 *outbuf, int buflen)
{
	int i;
	for(i = 0 ; i < buflen; i++)
	{
		outbuf[i] = (*fifo).cint32buf[(*fifo).readptr];
		(*fifo).readptr = ((*fifo).readptr + (*fifo).size + 1) & (*fifo).mask;
	}
	(*fifo).ptrdiff = ((*fifo).writeptr + (*fifo).size - (*fifo).readptr) & (*fifo).mask;
	return (*fifo).ptrdiff;
}

int getfirst5(fifo1 *fifo,cint32 *outbuf, int buflen)
{
	int i;
	int rdpt = (*fifo).readptr;
	for(i = 0 ; i < buflen; i++)
	{
		outbuf[i] = (*fifo).cint32buf[rdpt];
		rdpt = (rdpt + (*fifo).size + 1) & (*fifo).mask;
	}
	return buflen;
}

int getlast5(fifo1 *fifo,cint32 *outbuf, int buflen)
{
	int i;
	int rdpt = ((*fifo).writeptr + (*fifo).size - 1) & (*fifo).mask;
	for(i = buflen - 1; i >= 0; i--)
	{
		outbuf[i] = (*fifo).cint32buf[rdpt];
		rdpt = (rdpt + (*fifo).size - 1) & (*fifo).mask;
	}
	return buflen;
}

int pop6(fifo1 *fifo,cint16 *outbuf, int buflen)
{
	int i;
	for(i = 0 ; i < buflen; i++)
	{
		outbuf[i] = (*fifo).cint16buf[(*fifo).readptr];
		(*fifo).readptr = ((*fifo).readptr + (*fifo).size + 1) & (*fifo).mask;
	}
	(*fifo).ptrdiff = ((*fifo).writeptr + (*fifo).size - (*fifo).readptr) & (*fifo).mask;
	return (*fifo).ptrdiff;
}

int getfirst6(fifo1 *fifo,cint16 *outbuf, int buflen)
{
	int i;
	int rdpt = (*fifo).readptr;
	for(i = 0 ; i < buflen; i++)
	{
		outbuf[i] = (*fifo).cint16buf[rdpt];
		rdpt = (rdpt + (*fifo).size + 1) & (*fifo).mask;
	}
	return buflen;
}

int getlast6(fifo1 *fifo,cint16 *outbuf, int buflen)
{
	int i;
	int rdpt = ((*fifo).writeptr + (*fifo).size - 1) & (*fifo).mask;
	for(i = buflen - 1; i >= 0; i--)
	{
		outbuf[i] = (*fifo).cint16buf[rdpt];
		rdpt = (rdpt + (*fifo).size - 1) & (*fifo).mask;
	}
	return buflen;
}

int pop2(fifo1 *fifo,float *outbuf, int buflen)
{
	int i;
	for(i = 0 ; i < buflen; i++)
	{
		outbuf[i] = (*fifo).fltbuf[(*fifo).readptr];
		(*fifo).readptr = ((*fifo).readptr + (*fifo).size + 1) & (*fifo).mask;
	}
	(*fifo).ptrdiff = ((*fifo).writeptr + (*fifo).size - (*fifo).readptr) & (*fifo).mask;
	return (*fifo).ptrdiff;
}

int getfirst2(fifo1 *fifo,float *outbuf, int buflen)
{
	int i;
	int rdpt = (*fifo).readptr;
	for(i = 0 ; i < buflen; i++)
	{
		outbuf[i] = (*fifo).fltbuf[rdpt];
		rdpt = (rdpt + (*fifo).size + 1) & (*fifo).mask;
	}
	return buflen;
}

int getlast2(fifo1 *fifo,float *outbuf, int buflen)
{
	int i;
	int rdpt = ((*fifo).writeptr + (*fifo).size - 1) & (*fifo).mask;
	for(i = buflen - 1; i >= 0; i--)
	{
		outbuf[i] = (*fifo).fltbuf[rdpt];
		rdpt = (rdpt + (*fifo).size - 1) & (*fifo).mask;
	}
	return buflen;
}

int pop4(fifo1 *fifo,int32 *outbuf, int buflen)
{
	int i;
	for(i = 0 ; i < buflen; i++)
	{
		outbuf[i] = (*fifo).int32buf[(*fifo).readptr];
		(*fifo).readptr = ((*fifo).readptr + (*fifo).size + 1) & (*fifo).mask;
	}
	(*fifo).ptrdiff = ((*fifo).writeptr + (*fifo).size - (*fifo).readptr) & (*fifo).mask;
	return (*fifo).ptrdiff;
}

int getfirst4(fifo1 *fifo,int32 *outbuf, int buflen)
{
	int i;
	int rdpt = (*fifo).readptr;
	for(i = 0 ; i < buflen; i++)
	{
		outbuf[i] = (*fifo).int32buf[rdpt];
		rdpt = (rdpt + (*fifo).size + 1) & (*fifo).mask;
	}
	return buflen;
}

int getlast4(fifo1 *fifo,int32 *outbuf, int buflen)
{
	int i;
	int rdpt = ((*fifo).writeptr + (*fifo).size - 1) & (*fifo).mask;
	for(i = buflen - 1; i >= 0; i--)
	{
		outbuf[i] = (*fifo).int32buf[rdpt];
		rdpt = (rdpt + (*fifo).size - 1) & (*fifo).mask;
	}
	return buflen;
}

int pop3(fifo1 *fifo,int16 *outbuf, int buflen)
{
	int i;
	for(i = 0 ; i < buflen; i++)
	{
		outbuf[i] = (*fifo).int16buf[(*fifo).readptr];
		(*fifo).readptr = ((*fifo).readptr + (*fifo).size + 1) & (*fifo).mask;
	}
	(*fifo).ptrdiff = ((*fifo).writeptr + (*fifo).size - (*fifo).readptr) & (*fifo).mask;
	return (*fifo).ptrdiff;
}

int getfirst3(fifo1 *fifo,int16 *outbuf, int buflen)
{
	int i;
	int rdpt = (*fifo).readptr;
	for(i = 0 ; i < buflen; i++)
	{
		outbuf[i] = (*fifo).int16buf[rdpt];
		rdpt = (rdpt + (*fifo).size + 1) & (*fifo).mask;
	}
	return buflen;
}

int getlast3(fifo1 *fifo,int16 *outbuf, int buflen)
{
	int i;
	int rdpt = ((*fifo).writeptr + (*fifo).size - 1) & (*fifo).mask;
	for(i = buflen - 1; i >= 0; i--)
	{
		outbuf[i] = (*fifo).int16buf[rdpt];
		rdpt = (rdpt + (*fifo).size - 1) & (*fifo).mask;
	}
	return buflen;
}

int push1(fifo1 *fifo,cfloat data)
{
	(*fifo).cfltbuf[(*fifo).writeptr] = data;
	(*fifo).writeptr = (++(*fifo).writeptr) & (*fifo).mask;
	(*fifo).readptr = ((*fifo).readptr == (*fifo).writeptr) ? ((*fifo).readptr + (*fifo).size - 1) & (*fifo).mask : (*fifo).readptr;
	(*fifo).ptrdiff = ((*fifo).writeptr + (*fifo).size - (*fifo).readptr) & (*fifo).mask;
	return (*fifo).ptrdiff;
}

int push7(fifo1 *fifo,cint32 data)
{
	(*fifo).cint32buf[(*fifo).writeptr] = data;
	(*fifo).writeptr = (++(*fifo).writeptr) & (*fifo).mask;
	(*fifo).readptr = ((*fifo).readptr == (*fifo).writeptr) ? ((*fifo).readptr + (*fifo).size - 1) & (*fifo).mask : (*fifo).readptr;
	(*fifo).ptrdiff = ((*fifo).writeptr + (*fifo).size - (*fifo).readptr) & (*fifo).mask;
	return (*fifo).ptrdiff;
}

int push9(fifo1 *fifo,cint16 data)
{
	(*fifo).cint16buf[(*fifo).writeptr] = data;
	(*fifo).writeptr = (++(*fifo).writeptr) & (*fifo).mask;
	(*fifo).readptr = ((*fifo).readptr == (*fifo).writeptr) ? ((*fifo).readptr + (*fifo).size - 1) & (*fifo).mask : (*fifo).readptr;
	(*fifo).ptrdiff = ((*fifo).writeptr + (*fifo).size - (*fifo).readptr) & (*fifo).mask;
	return (*fifo).ptrdiff;
}

int push3(fifo1 *fifo,float data)
{
	(*fifo).fltbuf[(*fifo).writeptr] = data;
	(*fifo).writeptr = (++(*fifo).writeptr) & (*fifo).mask;
	(*fifo).readptr = ((*fifo).readptr == (*fifo).writeptr) ? ((*fifo).readptr + (*fifo).size - 1) & (*fifo).mask : (*fifo).readptr;
	(*fifo).ptrdiff = ((*fifo).writeptr + (*fifo).size - (*fifo).readptr) & (*fifo).mask;
	return (*fifo).ptrdiff;
}

int push5(fifo1 *fifo,int32 data)
{
	(*fifo).int32buf[(*fifo).writeptr] = data;
	(*fifo).writeptr = (++(*fifo).writeptr) & (*fifo).mask;
	(*fifo).readptr = ((*fifo).readptr == (*fifo).writeptr) ? ((*fifo).readptr + (*fifo).size - 1) & (*fifo).mask : (*fifo).readptr;
	(*fifo).ptrdiff = ((*fifo).writeptr + (*fifo).size - (*fifo).readptr) & (*fifo).mask;
	return (*fifo).ptrdiff;
}

int push4(fifo1 *fifo,int16 data)
{
	(*fifo).int16buf[(*fifo).writeptr] = data;
	(*fifo).writeptr = (++(*fifo).writeptr) & (*fifo).mask;
	(*fifo).readptr = ((*fifo).readptr == (*fifo).writeptr) ? ((*fifo).readptr + (*fifo).size - 1) & (*fifo).mask : (*fifo).readptr;
	(*fifo).ptrdiff = ((*fifo).writeptr + (*fifo).size - (*fifo).readptr) & (*fifo).mask;
	return (*fifo).ptrdiff;
}

int push6(fifo1 *fifo,cint32 *buf, int buflen)
{
	int i;
	
	for(i = 0; i < buflen; i++)
	{
		(*fifo).cint32buf[(*fifo).writeptr] = buf[i];
		(*fifo).writeptr = (++(*fifo).writeptr) & (*fifo).mask;
		(*fifo).readptr = ((*fifo).readptr == (*fifo).writeptr) ? ((*fifo).readptr + (*fifo).size - 1) & (*fifo).mask : (*fifo).readptr;
	}
	(*fifo).ptrdiff = ((*fifo).writeptr + (*fifo).size - (*fifo).readptr) & (*fifo).mask;
	return (*fifo).ptrdiff;
}

int push8(fifo1 *fifo,cint16 *buf, int buflen)
{
	int i;
	
	for(i = 0; i < buflen; i++)
	{
		(*fifo).cint16buf[(*fifo).writeptr] = buf[i];
		(*fifo).writeptr = (++(*fifo).writeptr) & (*fifo).mask;
		(*fifo).readptr = ((*fifo).readptr == (*fifo).writeptr) ? ((*fifo).readptr + (*fifo).size - 1) & (*fifo).mask : (*fifo).readptr;
	}
	(*fifo).ptrdiff = ((*fifo).writeptr + (*fifo).size - (*fifo).readptr) & (*fifo).mask;
	return (*fifo).ptrdiff;
}

int push2(fifo1 *fifo,cfloat *buf, int buflen)
{
	int i;
	
	for(i = 0; i < buflen; i++)
	{
		(*fifo).cfltbuf[(*fifo).writeptr] = buf[i];
		(*fifo).writeptr = (++(*fifo).writeptr) & (*fifo).mask;
		(*fifo).readptr = ((*fifo).readptr == (*fifo).writeptr) ? ((*fifo).readptr + (*fifo).size - 1) & (*fifo).mask : (*fifo).readptr;
	}
	(*fifo).ptrdiff = ((*fifo).writeptr + (*fifo).size - (*fifo).readptr) & (*fifo).mask;
	return (*fifo).ptrdiff;
}

int setptrdiff(fifo1 *fifo,int ptrdiff)
{
//	this->ptrdiff = ptrdiff;
	ptrdiff=(*fifo).ptrdiff;    //2015-12-3 add
	(*fifo).readptr = ((*fifo).writeptr + (*fifo).size - ptrdiff) & (*fifo).mask;

	return ptrdiff;
}

//int setptrdiff(int ptrdiff1)
//{
//	ptrdiff1 = ptrdiff;
//	readptr = (writeptr + size - ptrdiff1) & mask;
//
//	return ptrdiff1;
//}

int getptrdiff(fifo1 *fifo)
{
	return (*fifo).ptrdiff;
}

int resetbuf(fifo1 *fifo)
{
	switch((*fifo).type)
	{
	case CFLOAT_TYPE:
		memset((*fifo).cfltbuf, 0, sizeof(cfloat)*(*fifo).size);
		break;
	case CINT32_TYPE:
		memset((*fifo).cint32buf, 0, sizeof(cint16)*(*fifo).size);
		break;
	case CINT16_TYPE:
		memset((*fifo).cint16buf, 0, sizeof(cint16)*(*fifo).size);
		break;
	case FLOAT_TYPE:
		memset((*fifo).fltbuf, 0, sizeof(float)*(*fifo).size);
		break;
	case INT32_TYPE:
		memset((*fifo).int32buf, 0, sizeof(int16)*(*fifo).size);
		break;
	case INT16_TYPE:
		memset((*fifo).int16buf, 0, sizeof(int16)*(*fifo).size);
		break;
	default:
		;
	}
	(*fifo).writeptr = 0;
	(*fifo).readptr = 0;
	(*fifo).ptrdiff = 0;
	return (*fifo).ptrdiff;
}
