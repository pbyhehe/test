#pragma once
#include "typedefine.h"

#define  CFLOAT_TYPE 0
#define  CINT32_TYPE 1
#define  CINT16_TYPE 2
#define  FLOAT_TYPE	 3
#define  INT32_TYPE  4
#define  INT16_TYPE  5

typedef struct fifo1
{
	int type;
    int powsize, size;
    int mask;

    cfloat *cfltbuf;
    cint32 *cint32buf;
    cint16 *cint16buf;
    float  *fltbuf;
    int32  *int32buf;
    int16  *int16buf;

    int readptr, writeptr, ptrdiff;
}fifo1;

	int resetbuf(fifo1 *fifo);
	int getptrdiff(fifo1 *fifo);
	int setptrdiff(fifo1 *fifo,int ptrdiff);

	int push1(fifo1 *fifo,cfloat data);
	int push2(fifo1 *fifo,cfloat *buf, int buflen);
	int pop1(fifo1 *fifo,cfloat *buf, int buflen);
	int getfirst1(fifo1 *fifo,cfloat *outbuf, int buflen);
	int getlast1(fifo1 *fifo,cfloat *outbuf, int buflen);

	int push3(fifo1 *fifo,float data);
	int pop2(fifo1 *fifo,float *buf, int buflen);
	int getfirst2(fifo1 *fifo,float *buf, int buflen);
	int getlast2(fifo1 *fifo,float *outbuf, int buflen);

	int push4(fifo1 *fifo,int16 data);
	int pop3(fifo1 *fifo,int16 *buf, int buflen);
	int getfirst3(fifo1 *fifo,int16 *buf, int buflen);
	int getlast3(fifo1 *fifo,int16 *outbuf, int buflen);

	int push5(fifo1 *fifo,int32 data);
	int pop4(fifo1 *fifo,int32 *outbuf, int buflen);
	int getfirst4(fifo1 *fifo,int32 *outbuf, int buflen);
	int getlast4(fifo1 *fifo,int32 *outbuf, int buflen);

	int push6(fifo1 *fifo,cint32 *buf, int buflen);
	int push7(fifo1 *fifo,cint32 data);
	int pop5(fifo1 *fifo,cint32 *buf, int buflen);
	int getfirst5(fifo1 *fifo,cint32 *buf, int buflen);
	int getlast5(fifo1 *fifo,cint32 *outbuf, int buflen);

	int push8(fifo1 *fifo,cint16 *buf, int buflen);
	int push9(fifo1 *fifo,cint16 data);
	int pop6(fifo1 *fifo,cint16 *buf, int buflen);
	int getfirst6(fifo1 *fifo,cint16 *buf, int buflen);
	int getlast6(fifo1 *fifo,cint16 *outbuf, int buflen);
	
    int fifo_SetParameter(fifo1 *fifo,int powsize, int type);
	int fifo_Init(fifo1 *fifo);
	int fifo_Clear(fifo1 *fifo);

