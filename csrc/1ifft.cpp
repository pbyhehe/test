//#include "foo.h"
//#include "ifft.h"
//
////#pragma DATA_SECTION("uerx_data")
////float Ibuf1[16384];
////#pragma DATA_SECTION("uerx_data")
////float Qbuf1[16384];
////#pragma DATA_SECTION("uerx_data")
////float Ibuf2[16384];
////#pragma DATA_SECTION("uerx_data")
////float Qbuf2[16384];
////#pragma DATA_SECTION("uerx_data")
////float Iout[16384];
////#pragma DATA_SECTION("uerx_data")
////float Qout[16384];
//int ifft::IDFTInputOutput(float *Iin, float *Qin, int size, float *Iout, float *Qout)
//{
//	int n, k;
//	float re, im;
//	double omega, omegan, sqrtn;
//	double cosarg, sinarg;
//
//	sqrtn = sqrt(size + 0.0);
//	sqrtn = 1.0 / sqrtn;
//
//	for(k = 0; k < size; k++)
//	{
//		re = 0.0;
//		im = 0.0;
//		omega = 2 * PI * (k + 0.0) / (size + 0.0);
//		for(n = 0; n < size; n++)
//		{
//			omegan = omega * n;
//			cosarg = cos(omegan);
//			sinarg = sin(omegan);
//
//			re += (float)(Iin[n] * cosarg - Qin[n] * sinarg);
//			im += (float)(Iin[n] * sinarg + Qin[n] * cosarg);
//		}
//		Iout[k] = (float)(re * sqrtn);
//		Qout[k] = (float)(im * sqrtn);
//	}
//	return size;
//}
//
//
//int ifft::ifft_Init()
//{
//	fft_u.fft_Init();
//	Ibuf1 = new float[16384];
//	Qbuf1 = new float[16384];
//	Ibuf2 = new float[16384];
//	Qbuf2 = new float[16384];
//	Iout = new float[16384];
//	Qout = new float[16384];
//
//	return 0;
//}
//
//int ifft::ifft_Clear()
//{
//	fft_u.fft_Clear();
//
//	delete[] Ibuf1;
//	delete[] Qbuf1;
//	delete[] Ibuf2;
//	delete[] Qbuf2;
//	delete[] Iout;
//	delete[] Qout;
//	return 0;
//}
//
//int ifft::ifft_InputOutput5(float * Iin, float *Qin, int size, float *Iout, float *Qout)
//{
//	int N  = size;
//	//int N2 = N >> 1;
////	int logN = (int)(log10(size) / log10(2));
////
////	if(pow(2.0, logN) != N || N > 4096)
////	{
////		printf("ifft doesn't support size = %d\n", size);
////		return -1;
////	}
//	if((N > 4096) || (N%128 != 0))
//		{
//			printf("ifft doesn't support size = %d\n",size);
//			assert(0);
//		}
//
//	int i;
//	double sqrtn;
//
//	sqrtn = sqrt((double)size);
//	sqrtn = 1.0 / sqrtn;
//
//	for(i = 0; i < size; i++)
//	{
//		Ibuf1[i] = Iin[i];
//		Qbuf1[i] = 0;
//	}
//
//	fft_u.fft_InputOutput5(Ibuf1, Qbuf1, size, Iout, Qout);
//
//	for(i = 0; i < size; i++)
//	{
//		Ibuf1[i] = Qin[i];
//		Qbuf1[i] = 0;
//	}
//
//	fft_u.fft_InputOutput5(Ibuf1, Qbuf1, size, Ibuf2, Qbuf2);
//
//	for(i = 0; i < size; i++)
//	{
//		Iout[i] += Qbuf2[i];
//		Qout[i] = Ibuf2[i] - Qout[i];
//	}
//
//	return 0;
//}
//
//int ifft::ifft_InputOutput3(cfloat* in, int size, cfloat* out)
//{
//	int N  = size;
//	//int N2 = N >> 1;
////	int logN = (int)(log10(size) / log10(2));
////
////	if(pow(2.0, logN) != N && N != 1536 || N > 4096)
////	{
////		printf("ifft doesn't support size = %d\n", size);
////		assert(0);
////	}
//	if((N > 4096) || (N%128 != 0))
//		{
//			printf("ifft doesn't support size = %d\n",size);
//			assert(0);
//		}
//
//	int i;
//	double sqrtn;
//
//	sqrtn = sqrt((double)size);
//	sqrtn = 1.0 / sqrtn;
//
//	for(i = 0; i < size; i++)
//	{
//		Ibuf1[i] = in[i].re;
//		Qbuf1[i] = 0;
//	}
//
//	fft_u.fft_InputOutput5(Ibuf1, Qbuf1, size, Iout, Qout);
//
//	for(i = 0; i < size; i++)
//	{
//		Ibuf1[i] = in[i].im;
//		Qbuf1[i] = 0;
//	}
//
//	fft_u.fft_InputOutput5(Ibuf1, Qbuf1, size, Ibuf2, Qbuf2);
//
//	for(i = 0; i < size; i++)
//	{
//		out[i].re = Iout[i] + Qbuf2[i];
//		out[i].im = Ibuf2[i] - Qout[i];
//	}
//
//	return 0;
//}
//
//#pragma DATA_SECTION("uerx_data")
//ifft ifft_u;
//
//extern "C" int ifft_Init()
//{
//	int length;
//	length = ifft_u.ifft_Init();
//	return length;
//}
//
//extern "C" int ifft_Clear()
//{
//	int length;
//	length = ifft_u.ifft_Clear();
//	return length;
//}
//
//extern "C" int ifft_InputOutput3(cfloat* in, int size, cfloat* out)
//{
//	int length;
//	length = ifft_u.ifft_InputOutput3(in, size, out);
//	return length;
//}
//
//
