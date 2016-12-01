//#include "fft.h"
//#include <stdio.h>
//#include <math.h>
//#include <string.h>
//#include <stdlib.h>
//#include <assert.h>
//
////#pragma DATA_SECTION("uerx_data")
// float Iin[16384];
////#pragma DATA_SECTION("uerx_data")
// float Qin[16384];
////#pragma DATA_SECTION("uerx_data")
// float Iout[16384];
////#pragma DATA_SECTION("uerx_data")
// float Qout[16384];
//
//extern "C"
//{
//#include "sharefunc.h"
//}
//
//int fft::DFTInputOutput(float *Iin, float *Qin, int size, float *Iout, float *Qout)
//{
//	int n, k;
//	float re, im;
//	double omega, omegan, sqrtn;
//	double cosarg, sinarg;
//
//	sqrtn = sqrt(size + 0.0);
//	sqrtn = 1.0 / sqrtn;
//
//
//	for(k = 0; k < size; k++)
//	{
//		re = 0.0;
//		im = 0.0;
//		omega = 2 * PI * (k + 0.0) / (size + 0.0);
//		for(n = 0; n < size; n++)
//		{
//			omegan = -omega * n;
//			cosarg= cos(omegan);
//			sinarg= sin(omegan);
//			re = (float)(re + Iin[n] * cosarg - Qin[n] * sinarg);
//			im = (float)(im + Iin[n] * sinarg + Qin[n] * cosarg);
//		}
//		Iout[k] = (float)(re * sqrtn);
//		Qout[k] = (float)(im * sqrtn);
//	}
//	return size;
//}
//
//int fft::fft_Init()
//{
//	c3_1 = -1.5000000000000E+00;
//	c3_2 =  8.6602540378444E-01;
//
//	u5   =  1.2566370614359E+00;
//	c5_1 = -1.2500000000000E+00;
//	c5_2 =  5.5901699437495E-01;
//	c5_3 = -9.5105651629515E-01;
//	c5_4 = -1.5388417685876E+00;
//	c5_5 =  3.6327126400268E-01;
//	c8   =  7.0710678118655E-01;
//
//	memset(twiddleRe, 0, sizeof(double) *maxPrimeFactor);
//	memset(twiddleIm, 0, sizeof(double) *maxPrimeFactor);
//	memset(trigRe, 0, sizeof(double) *maxPrimeFactor);
//	memset(trigIm, 0, sizeof(double) *maxPrimeFactor);
//	memset(zRe, 0, sizeof(double) *maxPrimeFactor);
//	memset(zIm, 0, sizeof(double) *maxPrimeFactor);
//	memset(vRe, 0, sizeof(double) *maxPrimeFactorDiv2);
//	memset(vIm, 0, sizeof(double) *maxPrimeFactorDiv2);
//	memset(wRe, 0, sizeof(double) *maxPrimeFactorDiv2);
//	memset(wIm, 0, sizeof(double) *maxPrimeFactorDiv2);
//
//	// Zhang Pengtao
////	Iin = new float[16384];
////	Qin = new float[16384];
////	Iout = new float[16384];
////	Qout = new float[16384];
//	return 0;
//}
//
//int fft::fft_Clear()
//{
//
////	delete[] Iin;
////	delete[] Qin;
////	delete[] Iout;
////	delete[] Qout;
//	return 0;
//}
//
//void fft::factorize(int n, int *nFact, int fact[])
//{
//    int i,j,k;
//    int nRadix;
//    int radices[7];
//    int factors[maxFactorCount];
//
//    nRadix    =  6;
//    radices[1]=  2;
//    radices[2]=  3;
//    radices[3]=  4;
//    radices[4]=  5;
//    radices[5]=  8;
//    radices[6]= 10;
//
//    if (n==1)
//    {
//        j=1;
//        factors[1]=1;
//    }
//    else j=0;
//    i=nRadix;
//    while ((n>1) && (i>0))
//    {
//		if ((n % radices[i]) == 0)
//		{
//			n=n / radices[i];
//			j=j+1;
//			factors[j]=radices[i];
//		}
//		else  i=i-1;
//    }
//    if (factors[j] == 2)                         // substitute factors 2*8 with 4*4
//    {
//		i = j-1;
//		while ((i>0) && (factors[i] != 8)) i--;
//		if (i>0)
//		{
//			factors[j] = 4;
//			factors[i] = 4;
//		}
//    }
//    if (n>1)
//    {
//        for (k=2; k<sqrt((double)n)+1; k++)
//            while ((n % k) == 0)
//            {
//                n=n / k;
//                j=j+1;
//                factors[j]=k;
//            }
//			if (n>1)
//			{
//				j=j+1;
//				factors[j]=n;
//			}
//    }
//    for (i=1; i<=j; i++)
//    {
//		fact[i] = factors[j-i+1];
//    }
//    *nFact=j;
//}                                                // factorize
//
//void fft::transTableSetup(int sofar[], int actual[], int remain[], int *nFact, int *nPoints)
//{
//    int i;
//
//    factorize(*nPoints, nFact, actual);
//    if (actual[1] > maxPrimeFactor)
//    {
//        printf("\nPrime factor of FFT length too large : %6d",actual[1]);
//        printf("\nPlease modify the value of maxPrimeFactor in mixFFT.c");
//        exit(1);
//    }
//    remain[0]=*nPoints;
//    sofar[1]=1;
//    remain[1]=*nPoints / actual[1];
//    for (i=2; i<=*nFact; i++)
//    {
//        sofar[i]=sofar[i-1]*actual[i-1];
//        remain[i]=remain[i-1] / actual[i];
//    }
//}                                                // transTableSetup
//
//
//void fft::permute(int nPoint, int nFact, int fact[], int remain[],
//				  float *Iin, float *Qin, float *Iout, float *Qout)
//{
//    int i,j,k;
//    int count[maxFactorCount];
//
//    for (i=1; i<=nFact; i++)
//		count[i]=0;
//    k=0;
//    for (i=0; i<=nPoint-2; i++)
//    {
//		Iout[i] = Iin[k];
//		Qout[i] = Qin[k];
//        j=1;
//        k=k+remain[j];
//        count[1] = count[1]+1;
//        while (count[j] >= fact[j])
//        {
//            count[j]=0;
//            k=k-remain[j-1]+remain[j+1];
//            j=j+1;
//            count[j]=count[j]+1;
//        }
//    }
//    Iout[nPoint-1] = Iin[nPoint-1];
//    Qout[nPoint-1] = Qin[nPoint-1];
//}                                                // permute
//
//
//void fft::initTrig(int radix)
//{
//    int i;
//    double w,xre,xim;
//
//    w=2*pi/radix;
//    trigRe[0]=1; trigIm[0]=0;
//    xre=cos(w);
//    xim=-sin(w);
//    trigRe[1]=xre; trigIm[1]=xim;
//    for (i=2; i<radix; i++)
//    {
//        trigRe[i]=xre*trigRe[i-1] - xim*trigIm[i-1];
//        trigIm[i]=xim*trigRe[i-1] + xre*trigIm[i-1];
//    }
//}                                                // initTrig
//
//void fft::FFT_4(double aRe[], double aIm[])
//{
//    double  t1_re,t1_im, t2_re,t2_im;
//    double  m2_re,m2_im, m3_re,m3_im;
//
//    t1_re=aRe[0] + aRe[2]; t1_im=aIm[0] + aIm[2];
//    t2_re=aRe[1] + aRe[3]; t2_im=aIm[1] + aIm[3];
//
//    m2_re=aRe[0] - aRe[2]; m2_im=aIm[0] - aIm[2];
//    m3_re=aIm[1] - aIm[3]; m3_im=aRe[3] - aRe[1];
//
//    aRe[0]=t1_re + t2_re; aIm[0]=t1_im + t2_im;
//    aRe[2]=t1_re - t2_re; aIm[2]=t1_im - t2_im;
//    aRe[1]=m2_re + m3_re; aIm[1]=m2_im + m3_im;
//    aRe[3]=m2_re - m3_re; aIm[3]=m2_im - m3_im;
//}                                                // FFT_4
//
//void fft::FFT_5(double aRe[], double aIm[])
//{
//    double  t1_re,t1_im, t2_re,t2_im, t3_re,t3_im;
//    double  t4_re,t4_im, t5_re,t5_im;
//    double  m2_re,m2_im, m3_re,m3_im, m4_re,m4_im;
//    double  m1_re,m1_im, m5_re,m5_im;
//    double  s1_re,s1_im, s2_re,s2_im, s3_re,s3_im;
//    double  s4_re,s4_im, s5_re,s5_im;
//
//    t1_re=aRe[1] + aRe[4]; t1_im=aIm[1] + aIm[4];
//    t2_re=aRe[2] + aRe[3]; t2_im=aIm[2] + aIm[3];
//    t3_re=aRe[1] - aRe[4]; t3_im=aIm[1] - aIm[4];
//    t4_re=aRe[3] - aRe[2]; t4_im=aIm[3] - aIm[2];
//    t5_re=t1_re + t2_re; t5_im=t1_im + t2_im;
//    aRe[0]=aRe[0] + t5_re; aIm[0]=aIm[0] + t5_im;
//    m1_re=c5_1*t5_re; m1_im=c5_1*t5_im;
//    m2_re=c5_2*(t1_re - t2_re); m2_im=c5_2*(t1_im - t2_im);
//
//    m3_re=-c5_3*(t3_im + t4_im); m3_im=c5_3*(t3_re + t4_re);
//    m4_re=-c5_4*t4_im; m4_im=c5_4*t4_re;
//    m5_re=-c5_5*t3_im; m5_im=c5_5*t3_re;
//
//    s3_re=m3_re - m4_re; s3_im=m3_im - m4_im;
//    s5_re=m3_re + m5_re; s5_im=m3_im + m5_im;
//    s1_re=aRe[0] + m1_re; s1_im=aIm[0] + m1_im;
//    s2_re=s1_re + m2_re; s2_im=s1_im + m2_im;
//    s4_re=s1_re - m2_re; s4_im=s1_im - m2_im;
//
//    aRe[1]=s2_re + s3_re; aIm[1]=s2_im + s3_im;
//    aRe[2]=s4_re + s5_re; aIm[2]=s4_im + s5_im;
//    aRe[3]=s4_re - s5_re; aIm[3]=s4_im - s5_im;
//    aRe[4]=s2_re - s3_re; aIm[4]=s2_im - s3_im;
//}                                                // FFT_5
//
//void fft::FFT_8()
//{
//    double  aRe[4], aIm[4], bRe[4], bIm[4], gem;
//
//    aRe[0] = zRe[0];    bRe[0] = zRe[1];
//    aRe[1] = zRe[2];    bRe[1] = zRe[3];
//    aRe[2] = zRe[4];    bRe[2] = zRe[5];
//    aRe[3] = zRe[6];    bRe[3] = zRe[7];
//
//    aIm[0] = zIm[0];    bIm[0] = zIm[1];
//    aIm[1] = zIm[2];    bIm[1] = zIm[3];
//    aIm[2] = zIm[4];    bIm[2] = zIm[5];
//    aIm[3] = zIm[6];    bIm[3] = zIm[7];
//
//    FFT_4(aRe, aIm); FFT_4(bRe, bIm);
//
//    gem    = c8*(bRe[1] + bIm[1]);
//    bIm[1] = c8*(bIm[1] - bRe[1]);
//    bRe[1] = gem;
//    gem    = bIm[2];
//    bIm[2] =-bRe[2];
//    bRe[2] = gem;
//    gem    = c8*(bIm[3] - bRe[3]);
//    bIm[3] =-c8*(bRe[3] + bIm[3]);
//    bRe[3] = gem;
//
//    zRe[0] = aRe[0] + bRe[0]; zRe[4] = aRe[0] - bRe[0];
//    zRe[1] = aRe[1] + bRe[1]; zRe[5] = aRe[1] - bRe[1];
//    zRe[2] = aRe[2] + bRe[2]; zRe[6] = aRe[2] - bRe[2];
//    zRe[3] = aRe[3] + bRe[3]; zRe[7] = aRe[3] - bRe[3];
//
//    zIm[0] = aIm[0] + bIm[0]; zIm[4] = aIm[0] - bIm[0];
//    zIm[1] = aIm[1] + bIm[1]; zIm[5] = aIm[1] - bIm[1];
//    zIm[2] = aIm[2] + bIm[2]; zIm[6] = aIm[2] - bIm[2];
//    zIm[3] = aIm[3] + bIm[3]; zIm[7] = aIm[3] - bIm[3];
//}                                                // FFT_8
//
//void fft::FFT_10()
//{
//    double  aRe[5], aIm[5], bRe[5], bIm[5];
//
//    aRe[0] = zRe[0];    bRe[0] = zRe[5];
//    aRe[1] = zRe[2];    bRe[1] = zRe[7];
//    aRe[2] = zRe[4];    bRe[2] = zRe[9];
//    aRe[3] = zRe[6];    bRe[3] = zRe[1];
//    aRe[4] = zRe[8];    bRe[4] = zRe[3];
//
//    aIm[0] = zIm[0];    bIm[0] = zIm[5];
//    aIm[1] = zIm[2];    bIm[1] = zIm[7];
//    aIm[2] = zIm[4];    bIm[2] = zIm[9];
//    aIm[3] = zIm[6];    bIm[3] = zIm[1];
//    aIm[4] = zIm[8];    bIm[4] = zIm[3];
//
//    FFT_5(aRe, aIm); FFT_5(bRe, bIm);
//
//    zRe[0] = aRe[0] + bRe[0]; zRe[5] = aRe[0] - bRe[0];
//    zRe[6] = aRe[1] + bRe[1]; zRe[1] = aRe[1] - bRe[1];
//    zRe[2] = aRe[2] + bRe[2]; zRe[7] = aRe[2] - bRe[2];
//    zRe[8] = aRe[3] + bRe[3]; zRe[3] = aRe[3] - bRe[3];
//    zRe[4] = aRe[4] + bRe[4]; zRe[9] = aRe[4] - bRe[4];
//
//    zIm[0] = aIm[0] + bIm[0]; zIm[5] = aIm[0] - bIm[0];
//    zIm[6] = aIm[1] + bIm[1]; zIm[1] = aIm[1] - bIm[1];
//    zIm[2] = aIm[2] + bIm[2]; zIm[7] = aIm[2] - bIm[2];
//    zIm[8] = aIm[3] + bIm[3]; zIm[3] = aIm[3] - bIm[3];
//    zIm[4] = aIm[4] + bIm[4]; zIm[9] = aIm[4] - bIm[4];
//}                                                // FFT_10
//
//void fft::FFT_odd(int radix)
//{
//    double  rere, reim, imre, imim;
//    int     i,j,k,n,max;
//
//    n = radix;
//    max = (n + 1)/2;
//    for (j=1; j < max; j++)
//    {
//		vRe[j] = zRe[j] + zRe[n-j];
//		vIm[j] = zIm[j] - zIm[n-j];
//		wRe[j] = zRe[j] - zRe[n-j];
//		wIm[j] = zIm[j] + zIm[n-j];
//    }
//
//    for (j=1; j < max; j++)
//    {
//        zRe[j]=zRe[0];
//        zIm[j]=zIm[0];
//        zRe[n-j]=zRe[0];
//        zIm[n-j]=zIm[0];
//        k=j;
//        for (i=1; i < max; i++)
//        {
//            rere = trigRe[k] * vRe[i];
//            imim = trigIm[k] * vIm[i];
//            reim = trigRe[k] * wIm[i];
//            imre = trigIm[k] * wRe[i];
//
//            zRe[n-j] += rere + imim;
//            zIm[n-j] += reim - imre;
//            zRe[j]   += rere - imim;
//            zIm[j]   += reim + imre;
//
//            k = k + j;
//            if (k >= n)  k = k - n;
//        }
//    }
//    for (j=1; j < max; j++)
//    {
//        zRe[0]=zRe[0] + vRe[j];
//        zIm[0]=zIm[0] + wIm[j];
//    }
//}                                                // FFT_odd
//
//
//void fft::twiddleTransf(int sofarRadix, int radix, int remainRadix, float* Iout, float* Qout)
//{                                                // twiddleTransf
//    double  cosw, sinw, gem;
//    double  t1_re,t1_im, t2_re,t2_im, t3_re,t3_im;
//    double  t4_re,t4_im, t5_re,t5_im;
//    double  m2_re,m2_im, m3_re,m3_im, m4_re,m4_im;
//    double  m1_re,m1_im, m5_re,m5_im;
//    double  s1_re,s1_im, s2_re,s2_im, s3_re,s3_im;
//    double  s4_re,s4_im, s5_re,s5_im;
//
//    initTrig(radix);
//    omega = 2*pi/(double)(sofarRadix*radix);
//    cosw =  cos(omega);
//    sinw = -sin(omega);
//    tw_re = 1.0;
//    tw_im = 0;
//    dataOffset=0;
//    groupOffset=dataOffset;
//    adr=groupOffset;
//    for (dataNo=0; dataNo<sofarRadix; dataNo++)
//    {
//        if (sofarRadix>1)
//        {
//            twiddleRe[0] = 1.0;
//            twiddleIm[0] = 0.0;
//            twiddleRe[1] = tw_re;
//            twiddleIm[1] = tw_im;
//            for (twNo=2; twNo<radix; twNo++)
//            {
//                twiddleRe[twNo]=tw_re*twiddleRe[twNo-1]
//					- tw_im*twiddleIm[twNo-1];
//                twiddleIm[twNo]=tw_im*twiddleRe[twNo-1]
//					+ tw_re*twiddleIm[twNo-1];
//            }
//            gem   = cosw*tw_re - sinw*tw_im;
//            tw_im = sinw*tw_re + cosw*tw_im;
//            tw_re = gem;
//        }
//        for (groupNo=0; groupNo<remainRadix; groupNo++)
//        {
//            if ((sofarRadix>1) && (dataNo > 0))
//            {
//                zRe[0]=Iout[adr];
//                zIm[0]=Qout[adr];
//                blockNo=1;
//                do {
//                    adr = adr + sofarRadix;
//                    zRe[blockNo]=  twiddleRe[blockNo] * Iout[adr]
//						- twiddleIm[blockNo] * Qout[adr];
//                    zIm[blockNo]=  twiddleRe[blockNo] * Qout[adr]
//						+ twiddleIm[blockNo] * Iout[adr];
//
//                    blockNo++;
//                } while (blockNo < radix);
//            }
//            else
//                for (blockNo=0; blockNo<radix; blockNo++)
//                {
//					zRe[blockNo]=Iout[adr];
//					zIm[blockNo]=Qout[adr];
//					adr=adr+sofarRadix;
//                }
//				switch(radix)
//				{
//				case  2  : gem=zRe[0] + zRe[1];
//					zRe[1]=zRe[0] -  zRe[1]; zRe[0]=gem;
//					gem=zIm[0] + zIm[1];
//					zIm[1]=zIm[0] - zIm[1]; zIm[0]=gem;
//					break;
//				case  3  : t1_re=zRe[1] + zRe[2]; t1_im=zIm[1] + zIm[2];
//					zRe[0]=zRe[0] + t1_re; zIm[0]=zIm[0] + t1_im;
//					m1_re=c3_1*t1_re; m1_im=c3_1*t1_im;
//					m2_re=c3_2*(zIm[1] - zIm[2]);
//					m2_im=c3_2*(zRe[2] -  zRe[1]);
//					s1_re=zRe[0] + m1_re; s1_im=zIm[0] + m1_im;
//					zRe[1]=s1_re + m2_re; zIm[1]=s1_im + m2_im;
//					zRe[2]=s1_re - m2_re; zIm[2]=s1_im - m2_im;
//					break;
//				case  4  : t1_re=zRe[0] + zRe[2]; t1_im=zIm[0] + zIm[2];
//					t2_re=zRe[1] + zRe[3]; t2_im=zIm[1] + zIm[3];
//
//					m2_re=zRe[0] - zRe[2]; m2_im=zIm[0] - zIm[2];
//					m3_re=zIm[1] - zIm[3]; m3_im=zRe[3] - zRe[1];
//
//					zRe[0]=t1_re + t2_re; zIm[0]=t1_im + t2_im;
//					zRe[2]=t1_re - t2_re; zIm[2]=t1_im - t2_im;
//					zRe[1]=m2_re + m3_re; zIm[1]=m2_im + m3_im;
//					zRe[3]=m2_re - m3_re; zIm[3]=m2_im - m3_im;
//					break;
//				case  5  : t1_re=zRe[1] + zRe[4]; t1_im=zIm[1] + zIm[4];
//					t2_re=zRe[2] + zRe[3]; t2_im=zIm[2] + zIm[3];
//					t3_re=zRe[1] - zRe[4]; t3_im=zIm[1] - zIm[4];
//					t4_re=zRe[3] - zRe[2]; t4_im=zIm[3] - zIm[2];
//					t5_re=t1_re + t2_re; t5_im=t1_im + t2_im;
//					zRe[0]=zRe[0] + t5_re; zIm[0]=zIm[0] + t5_im;
//					m1_re=c5_1*t5_re; m1_im=c5_1*t5_im;
//					m2_re=c5_2*(t1_re - t2_re);
//					m2_im=c5_2*(t1_im - t2_im);
//
//					m3_re=-c5_3*(t3_im + t4_im);
//					m3_im=c5_3*(t3_re + t4_re);
//					m4_re=-c5_4*t4_im; m4_im=c5_4*t4_re;
//					m5_re=-c5_5*t3_im; m5_im=c5_5*t3_re;
//
//					s3_re=m3_re - m4_re; s3_im=m3_im - m4_im;
//					s5_re=m3_re + m5_re; s5_im=m3_im + m5_im;
//					s1_re=zRe[0] + m1_re; s1_im=zIm[0] + m1_im;
//					s2_re=s1_re + m2_re; s2_im=s1_im + m2_im;
//					s4_re=s1_re - m2_re; s4_im=s1_im - m2_im;
//
//					zRe[1]=s2_re + s3_re; zIm[1]=s2_im + s3_im;
//					zRe[2]=s4_re + s5_re; zIm[2]=s4_im + s5_im;
//					zRe[3]=s4_re - s5_re; zIm[3]=s4_im - s5_im;
//					zRe[4]=s2_re - s3_re; zIm[4]=s2_im - s3_im;
//					break;
//				case  8  : FFT_8(); break;
//				case 10  : FFT_10(); break;
//				default  : FFT_odd(radix); break;
//				}
//				adr=groupOffset;
//				for (blockNo=0; blockNo<radix; blockNo++)
//				{
//					Iout[adr]=(float)zRe[blockNo];
//					Qout[adr]=(float)zIm[blockNo];
//					adr=adr+sofarRadix;
//				}
//				groupOffset=groupOffset+sofarRadix*radix;
//				adr=groupOffset;
//        }
//        dataOffset=dataOffset+1;
//        groupOffset=dataOffset;
//        adr=groupOffset;
//    }
//}                                                // twiddleTransf
//
//int fft::fft_InputOutput5(float *Iin, float *Qin, int size, float *Iout, float *Qout)
//{
//	int N  = size;
//	int N2 = N >> 1;
////	int logN = (int)(log10(size) / log10(2));
////
////	if(pow(2.0, logN) != N || N > 4096)
////	{
////		printf("fft doesn't support size = %d\n", size);
////		assert(0);
////	}
//
//	if((N > 4096) || (N%128 != 0))
//	{
//		printf("fft doesn't support size = %d\n",size);
//		assert(0);
//	}
//
//    int   sofarRadix[maxFactorCount], actualRadix[maxFactorCount], remainRadix[maxFactorCount];
//    int   nFactor;
//    int   count;
//    double sqrtn;
//
//    pi = 4*atan(1.0);
//
//    transTableSetup(sofarRadix, actualRadix, remainRadix, &nFactor, &size);
//
//    permute(size, nFactor, actualRadix, remainRadix, Iin, Qin, Iout, Qout);
//
//    for (count=1; count<=nFactor; count++)
//		twiddleTransf(sofarRadix[count], actualRadix[count], remainRadix[count], Iout, Qout);
//
//    sqrtn = sqrt((double)size);
//    sqrtn = 1.0 / sqrtn;
//    for(count = 0; count < size; count++)
//    {
//		Iout[count] = (float)(Iout[count] * sqrtn);
//		Qout[count] = (float)(Qout[count] * sqrtn);
//    }
//    return size;
//}                                                // FFT
//
//int fft::fft_InputOutput3(cfloat *in, int size, cfloat *out)
//{
//	int N  = size;
//	int N2 = N >> 1;
//
////	int logN = (int)(log10(size) / log10(2));
////
////	if(pow(2.0, logN) != N || N > 4096)
////	{
////		printf("fft doesn't support size = %d\n", size);
////		assert(0);
////	}
//
//	if((N > 4096) || (N%128 != 0))
//	{
//		printf("fft doesn't support size = %d\n",size);
//		assert(0);
//	}
//
//	C2R(in, size, Iin, Qin);
//
//	fft_InputOutput5(Iin, Qin, size, Iout, Qout);
//
//	R2C(Iout, Qout, size, out);
//
//	return size;
//}
//
//#pragma DATA_SECTION("uerx_data")
//fft fft_u;
//
//extern "C"int fft_Init()
//{
//	int length;
//	length = fft_u.fft_Init();
//	return length;
//}
//
//extern "C"int fft_Clear()
//{
//	int length;
//	length = fft_u.fft_Clear();
//	return length;
//}
//
//extern "C"int fft_InputOutput3(cfloat *in, int size, cfloat *out)
//{
//	int length;
//	length = fft_u.fft_InputOutput3(in,size,out);
//	return length;
//}
//
