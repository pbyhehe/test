//#pragma once
//
//#include "typedefine.h"
//
//#define  maxPrimeFactor        37
//#define  maxPrimeFactorDiv2    (maxPrimeFactor+1)/2
//#define  maxFactorCount        20
//
//
//class fft
//{
//	double  c3_1;  /*  c3_1 = cos(2*pi/3)-1;          */
//	double  c3_2;  /*  c3_2 = sin(2*pi/3);            */
//
//	double  u5;  /*  u5   = 2*pi/5;                 */
//	double  c5_1;  /*  c5_1 = (cos(u5)+cos(2*u5))/2-1;*/
//	double  c5_2;  /*  c5_2 = (cos(u5)-cos(2*u5))/2;  */
//	double  c5_3;  /*  c5_3 = -sin(u5);               */
//	double  c5_4;  /*  c5_4 = -(sin(u5)+sin(2*u5));   */
//	double  c5_5;  /*  c5_5 = (sin(u5)-sin(2*u5));    */
//	double  c8;  /*  c8 = 1/sqrt(2);    */
//
//	double   pi;
//	int      groupOffset,dataOffset,blockOffset,adr;
//	int      groupNo,dataNo,blockNo,twNo;
//	double   omega, tw_re,tw_im;
//	double   twiddleRe[maxPrimeFactor], twiddleIm[maxPrimeFactor];
//	double	 trigRe[maxPrimeFactor], trigIm[maxPrimeFactor];
//	double   zRe[maxPrimeFactor], zIm[maxPrimeFactor];
//	double   vRe[maxPrimeFactorDiv2], vIm[maxPrimeFactorDiv2];
//	double   wRe[maxPrimeFactorDiv2], wIm[maxPrimeFactorDiv2];
//
////	float *Iin,*Qin,*Iout,*Qout;
//
//
//public:
//
//
//	void factorize(int n, int *nFact, int fact[]);
//	void transTableSetup(int sofar[], int actual[], int remain[], int *nFact, int *nPoints);
//	void permute(int nPoint, int nFact, int fact[], int remain[],
//		float *Iin, float *Qin, float *Iout, float *Qout);
//	void initTrig(int radix);
//	void FFT_4(double aRe[], double aIm[]);
//	void FFT_5(double aRe[], double aIm[]);
//	void FFT_8();
//
//	void FFT_10();
//	void FFT_odd(int radix);
//	int fft_Init();
//	int fft_Clear();
//	void twiddleTransf(int sofarRadix, int radix, int remainRadix, float *Iout, float *Qout);
//	int fft_InputOutput5(float *Iin, float *Qin, int size, float *Iout, float *Qout);
//	int fft_InputOutput3(cfloat *Iin, int size, cfloat *out);
//	int DFTInputOutput(float *Iin, float *Qin, int size, float *Iout, float *Qout);
//};
//
//extern fft fft_u;
