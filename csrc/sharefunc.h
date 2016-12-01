#pragma once

#include <stdio.h>
#include <stdarg.h>
#include "foo.h"
//#include "fft.h"
#include "typedefine.h"

extern cint16 ZEROCINT16;
extern cint32 ZEROCINT32;
extern cfloat ZEROCFLOAT;

extern void C2R(cfloat *cin, int size, float* Iout, float* Qout);
extern void R2C(float* Iin, float* Qin, int size, cfloat *cout);

int abs1(cint32 a);
int abs2(cint16 a);
float abs3(cfloat a);

int abs4(cint16 a);
float abs5(cfloat a);

cfloat setcfloat(float re, float im);
cint16 setcint16(int16 re, int16 im);

cfloat add(cfloat a, cfloat b);

cfloat opposite(cfloat a);
cfloat cfloatsubcfloat(cfloat a, cfloat b);

cfloat cfloatmultiplycfloat(cfloat a, cfloat b);
cfloat cfloatmultiplyfloat(cfloat a, float b);
cfloat cfloatmultiplyint(cfloat a, int b);
cint16 cint16multiplycint16(cint16 a, cint16 b);
cint32 cint32multiplycint32(cint32 a, cint32 b);

cfloat cfloatdividecfloat(cfloat *a, cfloat *b);
cfloat cfloatdividefloat(cfloat *a, float *b);

void cfloataddequalcfloat(cfloat *a, cfloat b);
void cint32addequalcint32(cint32 *a, cint32 b);
void cint16addequalcint16(cint16 *a, cint16 b);

void cfloatsubequalcfloat(cfloat *a, cfloat b);

void cfloatmultiplyequalfloat(cfloat *a, float b);
void cfloatmultiplyequalcfloat(cfloat *a, cfloat b);
void cint16multiplyequalfloat(cint16 *a, float b);

void cfloatdivideequalfloat(cfloat *a, float b);
void cfloatdivideequalcfloat(cfloat *a,  cfloat *b);

cfloat conj1(cfloat a);
cint32 conj2(cint32 a);
cint16 conj3(cint16 a);

float AMP(cfloat *a);
float FI(cfloat *a);
void AMPFI(cfloat *a);
void ZERO(cfloat *a);
int convolution(cfloat *inbuf, int inlen, cfloat *pfactor, cfloat *pstat, int statlen, cfloat *outbuf);
int RmRipple(cfloat *inbuf, int inlen, cfloat *outbuf, float *maxvalue, float ratio);
int Normalize(cfloat *inbuf, int inlen, cfloat *outbuf, float *maxvalue);
int convert2scompli(uint d, int width);

float getgauss(float mean, float sigma);

int	float_to_fix(float x, int frac_length, int bit_width);
int	float_to_fix_round(float x, int frac_length, int bit_width);
float fix_to_float(int x, int frac_length);
int fix_add(int x, int Q1, int y, int Q2, int bit_width, int *result_Q);
int fix_minus(int x, int Q1, int y, int Q2, int bit_width, int *result_Q);
int fix_multiplex(int x, int Q1, int y, int Q2, int bit_width, int *result_Q);  
int fix_divise(int x, int Q1, int y, int Q2);
int fix_opposite(int x, int bit_width);

int downsample(cfloat *inbuf, int inlen, int N_downsample, cfloat *outbuf);
