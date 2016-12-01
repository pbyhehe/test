#pragma once


#define PI                  3.1415926

typedef char				int8;
typedef unsigned char		uint8;
typedef short				int16;
typedef unsigned short		uint16;
typedef int					int32;
typedef unsigned int		uint;
typedef unsigned int		uint32;


typedef struct CINT8
{
	int8 re;
	int8 im;
}cint8;

typedef struct CINT16
{
	int16 re;
	int16 im;
}cint16;

typedef struct CINT32
{
	int32 re;
	int32 im;
}cint32;

typedef struct CFLOAT
{
	float re;
	float im;
}cfloat;

typedef struct CFLOATPCSI
{
	float re;
	float im;
	float csi;
}cfloatpcsi;
