#include "foo.h"
#include "ifft.h"
//#include "fft.h"

//ifft ifft_u;
float *Ibuf1, *Qbuf1, *Ibuf2, *Qbuf2, *Iout1, *Qout1;
extern float *Iout;
extern float *Qout;
extern int fft_Init();
extern int fft_Clear();
extern int fft_InputOutput5(float *Iin, float *Qin, int size, float *Iout, float *Qout);
int ifft_IDFTInputOutput(float *Iin, float *Qin, int size, float *Iout, float *Qout)
{
	int n, k;
	float re, im;
	double omega, omegan, sqrtn;
	double cosarg, sinarg;

	sqrtn = sqrt(size + 0.0);
	sqrtn = 1.0 / sqrtn;

	for(k = 0; k < size; k++)
	{
		re = 0.0;
		im = 0.0;
		omega = 2 * PI * (k + 0.0) / (size + 0.0);
		for(n = 0; n < size; n++)
		{
			omegan = omega * n;
			cosarg = cos(omegan);
			sinarg = sin(omegan);

			re += (float)(Iin[n] * cosarg - Qin[n] * sinarg);
			im += (float)(Iin[n] * sinarg + Qin[n] * cosarg);
		}
		Iout[k] = (float)(re * sqrtn);
		Qout[k] = (float)(im * sqrtn);
	}
	return size;
}


int ifft_Init()
{
	/*fft_u.*/fft_Init();
	Ibuf1 = malloc(16384*sizeof(float));
	Qbuf1 = malloc(16384*sizeof(float));
	Ibuf2 = malloc(16384*sizeof(float));
	Qbuf2 = malloc(16384*sizeof(float));
	Iout1 = malloc(16384*sizeof(float));
	Qout1 = malloc(16384*sizeof(float));

	return 0;
}

int ifft_Clear()
{
	/*fft_u.*/fft_Clear();

	free (Ibuf1);
	free (Qbuf1);
	free (Ibuf2);
	free (Qbuf2);
	free (Iout1);
	free (Qout1);
	return 0;
}

int ifft_InputOutput5(float * Iin, float *Qin, int size, float *Iout, float *Qout)
{
	int N  = size;
	if(N!=4096 && N!=2048 && N!=1536 && N!=1024 && N!=512 && N!=256 && N!=128)
		{
		    printf("ifft doesn't support size = %d\n", size);
		    assert(0);
		}
//	int logN =(int)(log10(size)/log10(2));
//
//
//	if(pow(2.0, logN) != N || N > 4096)
//	{
//		printf("ifft doesn't support size = %d\n", size);
//		return -1;
//	}

	int i;
	double sqrtn;

	sqrtn = sqrt((double)size);
	sqrtn = 1.0 / sqrtn;

	for(i = 0; i < size; i++)
	{
		Ibuf1[i] = Iin[i];
		Qbuf1[i] = 0;
	}

	/*fft_u.*/fft_InputOutput5(Ibuf1, Qbuf1, size, Iout, Qout);

	for(i = 0; i < size; i++)
	{
		Ibuf1[i] = Qin[i];
		Qbuf1[i] = 0;
	}

	/*fft_u.*/fft_InputOutput5(Ibuf1, Qbuf1, size, Ibuf2, Qbuf2);

	for(i = 0; i < size; i++)
	{
		Iout[i] += Qbuf2[i];
		Qout[i] = Ibuf2[i] - Qout[i];
	}

	return 0;
}

int ifft_InputOutput3(cfloat* in, int size, cfloat* out)
{
      int N  = size;
      if(N!=4096 && N!=2048 && N!=1536 && N!=1024 && N!=512 && N!=256 && N!=128)
      	{
      	    printf("ifft doesn't support size = %d\n", size);
      	    assert(0);
      	}
//	int logN =(int)(log10(size)/log10(2));
//
//	if(pow(2.0, logN) != N && N != 1536 || N > 4096)
//	{
//		printf("ifft doesn't support size = %d\n", size);
//		assert(0);
//	}
//    Init();          //2016-1-3xiugai
	int i;
	double sqrtn;

	sqrtn = sqrt((double)size);
	sqrtn = 1.0 / sqrtn;

	for(i = 0; i < size; i++)
	{
		Ibuf1[i] = in[i].re;
		Qbuf1[i] = 0;
	}

	/*fft_u.*/fft_InputOutput5(Ibuf1, Qbuf1, size, Iout1, Qout1);

	for(i = 0; i < size; i++)
	{
		Ibuf1[i] = in[i].im;
		Qbuf1[i] = 0;
	}

	/*fft_u.*/fft_InputOutput5(Ibuf1, Qbuf1, size, Ibuf2, Qbuf2);

	for(i = 0; i < size; i++)
	{
		out[i].re = Iout1[i] + Qbuf2[i];
		out[i].im = Ibuf2[i] - Qout1[i];
	}
   //clear();
	return 0;
}
