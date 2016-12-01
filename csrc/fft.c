#include "fft.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

//fft fft_u;
float *Iin, *Qin, *Iout, *Qout;

int fft_DFTInputOutput(float *Iin, float *Qin, int size, float *Iout, float *Qout)
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
			omegan = -omega * n;
			cosarg= cos(omegan);
			sinarg= sin(omegan);
			re = (float)(re + Iin[n] * cosarg - Qin[n] * sinarg);
			im = (float)(im + Iin[n] * sinarg + Qin[n] * cosarg);
		}
		Iout[k] = (float)(re * sqrtn);
		Qout[k] = (float)(im * sqrtn);
	}
	return size;
}

int fft_Init()
{
	fft.c3_1 = -1.5000000000000E+00;
	fft.c3_2 =  8.6602540378444E-01;

	fft.u5   =  1.2566370614359E+00;
	fft.c5_1 = -1.2500000000000E+00;
	fft.c5_2 =  5.5901699437495E-01;
	fft.c5_3 = -9.5105651629515E-01;
	fft.c5_4 = -1.5388417685876E+00;
	fft.c5_5 =  3.6327126400268E-01;
	fft.c8   =  7.0710678118655E-01;

	memset(fft.twiddleRe, 0, sizeof(double) *maxPrimeFactor);
	memset(fft.twiddleIm, 0, sizeof(double) *maxPrimeFactor);
	memset(fft.trigRe, 0, sizeof(double) *maxPrimeFactor);
	memset(fft.trigIm, 0, sizeof(double) *maxPrimeFactor);
	memset(fft.zRe, 0, sizeof(double) *maxPrimeFactor);
	memset(fft.zIm, 0, sizeof(double) *maxPrimeFactor);
	memset(fft.vRe, 0, sizeof(double) *maxPrimeFactorDiv2);
	memset(fft.vIm, 0, sizeof(double) *maxPrimeFactorDiv2);
	memset(fft.wRe, 0, sizeof(double) *maxPrimeFactorDiv2);
	memset(fft.wIm, 0, sizeof(double) *maxPrimeFactorDiv2);


	Iin = malloc(16384*sizeof(float));
	Qin = malloc(16384*sizeof(float));
	Iout = malloc(16384*sizeof(float));
	Qout = malloc(16384*sizeof(float));
	return 0;
}

int fft_Clear()
{
	free (Iin);
	free (Qin);
	free (Iout);
	free (Qout);
	return 0;
}

void factorize(int n, int *nFact, int fact[])
{
    int i,j,k;
    int nRadix;
    int radices[7];
    int factors[maxFactorCount];

    nRadix    =  6;
    radices[1]=  2;
    radices[2]=  3;
    radices[3]=  4;
    radices[4]=  5;
    radices[5]=  8;
    radices[6]= 10;

    if (n==1)
    {
        j=1;
        factors[1]=1;
    }
    else j=0;
    i=nRadix;
    while ((n>1) && (i>0))
    {
		if ((n % radices[i]) == 0)
		{
			n=n / radices[i];
			j=j+1;
			factors[j]=radices[i];
		}
		else  i=i-1;
    }
    if (factors[j] == 2)                         // substitute factors 2*8 with 4*4
    {
		i = j-1;
		while ((i>0) && (factors[i] != 8)) i--;
		if (i>0)
		{
			factors[j] = 4;
			factors[i] = 4;
		}
    }
    if (n>1)
    {
        for (k=2; k<sqrt((double)n)+1; k++)
            while ((n % k) == 0)
            {
                n=n / k;
                j=j+1;
                factors[j]=k;
            }
			if (n>1)
			{
				j=j+1;
				factors[j]=n;
			}
    }
    for (i=1; i<=j; i++)
    {
		fact[i] = factors[j-i+1];
    }
    *nFact=j;
}                                                // factorize

void transTableSetup(int sofar[], int actual[], int remain[], int *nFact, int *nPoints)
{
    int i;

    factorize(*nPoints, nFact, actual);
    if (actual[1] > maxPrimeFactor)
    {
        printf("\nPrime factor of FFT length too large : %6d",actual[1]);
        printf("\nPlease modify the value of maxPrimeFactor in mixFFT.c");
        exit(1);
    }
    remain[0]=*nPoints;
    sofar[1]=1;
    remain[1]=*nPoints / actual[1];
    for (i=2; i<=*nFact; i++)
    {
        sofar[i]=sofar[i-1]*actual[i-1];
        remain[i]=remain[i-1] / actual[i];
    }
}                                                // transTableSetup


void permute(int nPoint, int nFact, int fact[], int remain[],
				  float *Iin, float *Qin, float *Iout, float *Qout)
{
    int i,j,k;
    int count[maxFactorCount];

    for (i=1; i<=nFact; i++)
		count[i]=0;
    k=0;
    for (i=0; i<=nPoint-2; i++)
    {
		Iout[i] = Iin[k];
		Qout[i] = Qin[k];
        j=1;
        k=k+remain[j];
        count[1] = count[1]+1;
        while (count[j] >= fact[j])
        {
            count[j]=0;
            k=k-remain[j-1]+remain[j+1];
            j=j+1;
            count[j]=count[j]+1;
        }
    }
    Iout[nPoint-1] = Iin[nPoint-1];
    Qout[nPoint-1] = Qin[nPoint-1];
}                                                // permute


void initTrig(int radix)
{
    int i;
    double w,xre,xim;

    w=2*fft.pi/radix;
    fft.trigRe[0]=1; fft.trigIm[0]=0;
    xre=cos(w);
    xim=-sin(w);
    fft.trigRe[1]=xre; fft.trigIm[1]=xim;
    for (i=2; i<radix; i++)
    {
        fft.trigRe[i]=xre*fft.trigRe[i-1] - xim*fft.trigIm[i-1];
        fft.trigIm[i]=xim*fft.trigRe[i-1] + xre*fft.trigIm[i-1];
    }
}                                                // initTrig

void FFT_4(double aRe[], double aIm[])
{
    double  t1_re,t1_im, t2_re,t2_im;
    double  m2_re,m2_im, m3_re,m3_im;

    t1_re=aRe[0] + aRe[2]; t1_im=aIm[0] + aIm[2];
    t2_re=aRe[1] + aRe[3]; t2_im=aIm[1] + aIm[3];

    m2_re=aRe[0] - aRe[2]; m2_im=aIm[0] - aIm[2];
    m3_re=aIm[1] - aIm[3]; m3_im=aRe[3] - aRe[1];

    aRe[0]=t1_re + t2_re; aIm[0]=t1_im + t2_im;
    aRe[2]=t1_re - t2_re; aIm[2]=t1_im - t2_im;
    aRe[1]=m2_re + m3_re; aIm[1]=m2_im + m3_im;
    aRe[3]=m2_re - m3_re; aIm[3]=m2_im - m3_im;
}                                                // FFT_4

void FFT_5(double aRe[], double aIm[])
{
    double  t1_re,t1_im, t2_re,t2_im, t3_re,t3_im;
    double  t4_re,t4_im, t5_re,t5_im;
    double  m2_re,m2_im, m3_re,m3_im, m4_re,m4_im;
    double  m1_re,m1_im, m5_re,m5_im;
    double  s1_re,s1_im, s2_re,s2_im, s3_re,s3_im;
    double  s4_re,s4_im, s5_re,s5_im;

    t1_re=aRe[1] + aRe[4]; t1_im=aIm[1] + aIm[4];
    t2_re=aRe[2] + aRe[3]; t2_im=aIm[2] + aIm[3];
    t3_re=aRe[1] - aRe[4]; t3_im=aIm[1] - aIm[4];
    t4_re=aRe[3] - aRe[2]; t4_im=aIm[3] - aIm[2];
    t5_re=t1_re + t2_re; t5_im=t1_im + t2_im;
    aRe[0]=aRe[0] + t5_re; aIm[0]=aIm[0] + t5_im;
    m1_re=fft.c5_1*t5_re; m1_im=fft.c5_1*t5_im;
    m2_re=fft.c5_2*(t1_re - t2_re); m2_im=fft.c5_2*(t1_im - t2_im);

    m3_re=-fft.c5_3*(t3_im + t4_im); m3_im=fft.c5_3*(t3_re + t4_re);
    m4_re=-fft.c5_4*t4_im; m4_im=fft.c5_4*t4_re;
    m5_re=-fft.c5_5*t3_im; m5_im=fft.c5_5*t3_re;

    s3_re=m3_re - m4_re; s3_im=m3_im - m4_im;
    s5_re=m3_re + m5_re; s5_im=m3_im + m5_im;
    s1_re=aRe[0] + m1_re; s1_im=aIm[0] + m1_im;
    s2_re=s1_re + m2_re; s2_im=s1_im + m2_im;
    s4_re=s1_re - m2_re; s4_im=s1_im - m2_im;

    aRe[1]=s2_re + s3_re; aIm[1]=s2_im + s3_im;
    aRe[2]=s4_re + s5_re; aIm[2]=s4_im + s5_im;
    aRe[3]=s4_re - s5_re; aIm[3]=s4_im - s5_im;
    aRe[4]=s2_re - s3_re; aIm[4]=s2_im - s3_im;
}                                                // FFT_5

void FFT_8()
{
    double  aRe[4], aIm[4], bRe[4], bIm[4], gem;

    aRe[0] = fft.zRe[0];    bRe[0] = fft.zRe[1];
    aRe[1] = fft.zRe[2];    bRe[1] = fft.zRe[3];
    aRe[2] = fft.zRe[4];    bRe[2] = fft.zRe[5];
    aRe[3] = fft.zRe[6];    bRe[3] = fft.zRe[7];

    aIm[0] = fft.zIm[0];    bIm[0] = fft.zIm[1];
    aIm[1] = fft.zIm[2];    bIm[1] = fft.zIm[3];
    aIm[2] = fft.zIm[4];    bIm[2] = fft.zIm[5];
    aIm[3] = fft.zIm[6];    bIm[3] = fft.zIm[7];

    FFT_4(aRe, aIm); FFT_4(bRe, bIm);

    gem    = fft.c8*(bRe[1] + bIm[1]);
    bIm[1] = fft.c8*(bIm[1] - bRe[1]);
    bRe[1] = gem;
    gem    = bIm[2];
    bIm[2] =-bRe[2];
    bRe[2] = gem;
    gem    = fft.c8*(bIm[3] - bRe[3]);
    bIm[3] =-fft.c8*(bRe[3] + bIm[3]);
    bRe[3] = gem;

    fft.zRe[0] = aRe[0] + bRe[0]; fft.zRe[4] = aRe[0] - bRe[0];
    fft.zRe[1] = aRe[1] + bRe[1]; fft.zRe[5] = aRe[1] - bRe[1];
    fft.zRe[2] = aRe[2] + bRe[2]; fft.zRe[6] = aRe[2] - bRe[2];
    fft.zRe[3] = aRe[3] + bRe[3]; fft.zRe[7] = aRe[3] - bRe[3];

    fft.zIm[0] = aIm[0] + bIm[0]; fft.zIm[4] = aIm[0] - bIm[0];
    fft.zIm[1] = aIm[1] + bIm[1]; fft.zIm[5] = aIm[1] - bIm[1];
    fft.zIm[2] = aIm[2] + bIm[2]; fft.zIm[6] = aIm[2] - bIm[2];
    fft.zIm[3] = aIm[3] + bIm[3]; fft.zIm[7] = aIm[3] - bIm[3];
}                                                // FFT_8

void FFT_10()
{
    double  aRe[5], aIm[5], bRe[5], bIm[5];

    aRe[0] = fft.zRe[0];    bRe[0] = fft.zRe[5];
    aRe[1] = fft.zRe[2];    bRe[1] = fft.zRe[7];
    aRe[2] = fft.zRe[4];    bRe[2] = fft.zRe[9];
    aRe[3] = fft.zRe[6];    bRe[3] = fft.zRe[1];
    aRe[4] = fft.zRe[8];    bRe[4] = fft.zRe[3];

    aIm[0] = fft.zIm[0];    bIm[0] = fft.zIm[5];
    aIm[1] = fft.zIm[2];    bIm[1] = fft.zIm[7];
    aIm[2] = fft.zIm[4];    bIm[2] = fft.zIm[9];
    aIm[3] = fft.zIm[6];    bIm[3] = fft.zIm[1];
    aIm[4] = fft.zIm[8];    bIm[4] = fft.zIm[3];

    FFT_5(aRe, aIm); FFT_5(bRe, bIm);

    fft.zRe[0] = aRe[0] + bRe[0]; fft.zRe[5] = aRe[0] - bRe[0];
    fft.zRe[6] = aRe[1] + bRe[1]; fft.zRe[1] = aRe[1] - bRe[1];
    fft.zRe[2] = aRe[2] + bRe[2]; fft.zRe[7] = aRe[2] - bRe[2];
    fft.zRe[8] = aRe[3] + bRe[3]; fft.zRe[3] = aRe[3] - bRe[3];
    fft.zRe[4] = aRe[4] + bRe[4]; fft.zRe[9] = aRe[4] - bRe[4];

    fft.zIm[0] = aIm[0] + bIm[0]; fft.zIm[5] = aIm[0] - bIm[0];
    fft.zIm[6] = aIm[1] + bIm[1]; fft.zIm[1] = aIm[1] - bIm[1];
    fft.zIm[2] = aIm[2] + bIm[2]; fft.zIm[7] = aIm[2] - bIm[2];
    fft.zIm[8] = aIm[3] + bIm[3]; fft.zIm[3] = aIm[3] - bIm[3];
    fft.zIm[4] = aIm[4] + bIm[4]; fft.zIm[9] = aIm[4] - bIm[4];
}                                                // FFT_10

void FFT_odd(int radix)
{
    double  rere, reim, imre, imim;
    int     i,j,k,n,max;

    n = radix;
    max = (n + 1)/2;
    for (j=1; j < max; j++)
    {
		fft.vRe[j] = fft.zRe[j] + fft.zRe[n-j];
		fft.vIm[j] = fft.zIm[j] - fft.zIm[n-j];
		fft.wRe[j] = fft.zRe[j] - fft.zRe[n-j];
		fft.wIm[j] = fft.zIm[j] + fft.zIm[n-j];
    }

    for (j=1; j < max; j++)
    {
        fft.zRe[j]=fft.zRe[0];
        fft.zIm[j]=fft.zIm[0];
        fft.zRe[n-j]=fft.zRe[0];
        fft.zIm[n-j]=fft.zIm[0];
        k=j;
        for (i=1; i < max; i++)
        {
            rere = fft.trigRe[k] * fft.vRe[i];
            imim = fft.trigIm[k] * fft.vIm[i];
            reim = fft.trigRe[k] * fft.wIm[i];
            imre = fft.trigIm[k] * fft.wRe[i];

            fft.zRe[n-j] += rere + imim;
            fft.zIm[n-j] += reim - imre;
            fft.zRe[j]   += rere - imim;
            fft.zIm[j]   += reim + imre;

            k = k + j;
            if (k >= n)  k = k - n;
        }
    }
    for (j=1; j < max; j++)
    {
        fft.zRe[0]=fft.zRe[0] + fft.vRe[j];
        fft.zIm[0]=fft.zIm[0] + fft.wIm[j];
    }
}                                                // FFT_odd


void twiddleTransf(int sofarRadix, int radix, int remainRadix, float* Iout, float* Qout)
{                                                // twiddleTransf
    double  cosw, sinw, gem;
    double  t1_re,t1_im, t2_re,t2_im, t3_re,t3_im;
    double  t4_re,t4_im, t5_re,t5_im;
    double  m2_re,m2_im, m3_re,m3_im, m4_re,m4_im;
    double  m1_re,m1_im, m5_re,m5_im;
    double  s1_re,s1_im, s2_re,s2_im, s3_re,s3_im;
    double  s4_re,s4_im, s5_re,s5_im;

    initTrig(radix);
    fft.omega = 2*fft.pi/(double)(sofarRadix*radix);
    cosw =  cos(fft.omega);
    sinw = -sin(fft.omega);
    fft.tw_re = 1.0;
    fft.tw_im = 0;
    fft.dataOffset=0;
    fft.groupOffset=fft.dataOffset;
    fft.adr=fft.groupOffset;
    for (fft.dataNo=0; fft.dataNo<sofarRadix; fft.dataNo++)
    {
        if (sofarRadix>1)
        {
            fft.twiddleRe[0] = 1.0;
            fft.twiddleIm[0] = 0.0;
            fft.twiddleRe[1] = fft.tw_re;
            fft.twiddleIm[1] = fft.tw_im;
            for (fft.twNo=2; fft.twNo<radix; fft.twNo++)
            {
                fft.twiddleRe[fft.twNo]=fft.tw_re*fft.twiddleRe[fft.twNo-1]
					- fft.tw_im*fft.twiddleIm[fft.twNo-1];
                fft.twiddleIm[fft.twNo]=fft.tw_im*fft.twiddleRe[fft.twNo-1]
					+ fft.tw_re*fft.twiddleIm[fft.twNo-1];
            }
            gem   = cosw*fft.tw_re - sinw*fft.tw_im;
            fft.tw_im = sinw*fft.tw_re + cosw*fft.tw_im;
            fft.tw_re = gem;
        }
        for (fft.groupNo=0; fft.groupNo<remainRadix; fft.groupNo++)
        {
            if ((sofarRadix>1) && (fft.dataNo > 0))
            {
                fft.zRe[0]=Iout[fft.adr];
                fft.zIm[0]=Qout[fft.adr];
                fft.blockNo=1;
                do {
                    fft.adr = fft.adr + sofarRadix;
                    fft.zRe[fft.blockNo]=  fft.twiddleRe[fft.blockNo] * Iout[fft.adr]
						- fft.twiddleIm[fft.blockNo] * Qout[fft.adr];
                    fft.zIm[fft.blockNo]=  fft.twiddleRe[fft.blockNo] * Qout[fft.adr]
						+ fft.twiddleIm[fft.blockNo] * Iout[fft.adr];

                    fft.blockNo++;
                } while (fft.blockNo < radix);
            }
            else
                for (fft.blockNo=0; fft.blockNo<radix; fft.blockNo++)
                {
					fft.zRe[fft.blockNo]=Iout[fft.adr];
					fft.zIm[fft.blockNo]=Qout[fft.adr];
					fft.adr=fft.adr+sofarRadix;
                }
				switch(radix)
				{
				case  2  : gem=fft.zRe[0] + fft.zRe[1];
					fft.zRe[1]=fft.zRe[0] -  fft.zRe[1]; fft.zRe[0]=gem;
					gem=fft.zIm[0] + fft.zIm[1];
					fft.zIm[1]=fft.zIm[0] - fft.zIm[1]; fft.zIm[0]=gem;
					break;
				case  3  : t1_re=fft.zRe[1] + fft.zRe[2]; t1_im=fft.zIm[1] + fft.zIm[2];
					fft.zRe[0]=fft.zRe[0] + t1_re; fft.zIm[0]=fft.zIm[0] + t1_im;
					m1_re=fft.c3_1*t1_re; m1_im=fft.c3_1*t1_im;
					m2_re=fft.c3_2*(fft.zIm[1] - fft.zIm[2]);
					m2_im=fft.c3_2*(fft.zRe[2] -  fft.zRe[1]);
					s1_re=fft.zRe[0] + m1_re; s1_im=fft.zIm[0] + m1_im;
					fft.zRe[1]=s1_re + m2_re; fft.zIm[1]=s1_im + m2_im;
					fft.zRe[2]=s1_re - m2_re; fft.zIm[2]=s1_im - m2_im;
					break;
				case  4  : t1_re=fft.zRe[0] + fft.zRe[2]; t1_im=fft.zIm[0] + fft.zIm[2];
					t2_re=fft.zRe[1] + fft.zRe[3]; t2_im=fft.zIm[1] + fft.zIm[3];

					m2_re=fft.zRe[0] - fft.zRe[2]; m2_im=fft.zIm[0] - fft.zIm[2];
					m3_re=fft.zIm[1] - fft.zIm[3]; m3_im=fft.zRe[3] - fft.zRe[1];

					fft.zRe[0]=t1_re + t2_re; fft.zIm[0]=t1_im + t2_im;
					fft.zRe[2]=t1_re - t2_re; fft.zIm[2]=t1_im - t2_im;
					fft.zRe[1]=m2_re + m3_re; fft.zIm[1]=m2_im + m3_im;
					fft.zRe[3]=m2_re - m3_re; fft.zIm[3]=m2_im - m3_im;
					break;
				case  5  : t1_re=fft.zRe[1] + fft.zRe[4]; t1_im=fft.zIm[1] + fft.zIm[4];
					t2_re=fft.zRe[2] + fft.zRe[3]; t2_im=fft.zIm[2] + fft.zIm[3];
					t3_re=fft.zRe[1] - fft.zRe[4]; t3_im=fft.zIm[1] - fft.zIm[4];
					t4_re=fft.zRe[3] - fft.zRe[2]; t4_im=fft.zIm[3] - fft.zIm[2];
					t5_re=t1_re + t2_re; t5_im=t1_im + t2_im;
					fft.zRe[0]=fft.zRe[0] + t5_re; fft.zIm[0]=fft.zIm[0] + t5_im;
					m1_re=fft.c5_1*t5_re; m1_im=fft.c5_1*t5_im;
					m2_re=fft.c5_2*(t1_re - t2_re);
					m2_im=fft.c5_2*(t1_im - t2_im);

					m3_re=-fft.c5_3*(t3_im + t4_im);
					m3_im=fft.c5_3*(t3_re + t4_re);
					m4_re=-fft.c5_4*t4_im; m4_im=fft.c5_4*t4_re;
					m5_re=-fft.c5_5*t3_im; m5_im=fft.c5_5*t3_re;

					s3_re=m3_re - m4_re; s3_im=m3_im - m4_im;
					s5_re=m3_re + m5_re; s5_im=m3_im + m5_im;
					s1_re=fft.zRe[0] + m1_re; s1_im=fft.zIm[0] + m1_im;
					s2_re=s1_re + m2_re; s2_im=s1_im + m2_im;
					s4_re=s1_re - m2_re; s4_im=s1_im - m2_im;

					fft.zRe[1]=s2_re + s3_re; fft.zIm[1]=s2_im + s3_im;
					fft.zRe[2]=s4_re + s5_re; fft.zIm[2]=s4_im + s5_im;
					fft.zRe[3]=s4_re - s5_re; fft.zIm[3]=s4_im - s5_im;
					fft.zRe[4]=s2_re - s3_re; fft.zIm[4]=s2_im - s3_im;
					break;
				case  8  : FFT_8(); break;
				case 10  : FFT_10(); break;
				default  : FFT_odd(radix); break;
				}
				fft.adr=fft.groupOffset;
				for (fft.blockNo=0; fft.blockNo<radix; fft.blockNo++)
				{
					Iout[fft.adr]=(float)fft.zRe[fft.blockNo];
					Qout[fft.adr]=(float)fft.zIm[fft.blockNo];
					fft.adr=fft.adr+sofarRadix;
				}
				fft.groupOffset=fft.groupOffset+sofarRadix*radix;
				fft.adr=fft.groupOffset;
        }
        fft.dataOffset=fft.dataOffset+1;
        fft.groupOffset=fft.dataOffset;
        fft.adr=fft.groupOffset;
    }
}                                                // twiddleTransf
extern void C2R(cfloat *cin, int size, float* Iout, float* Qout);
extern void R2C(float* Iin, float* Qin, int size, cfloat *cout);
int fft_InputOutput5(float *Iin, float *Qin, int size, float *Iout, float *Qout)
{
	int N  = size;
	if(N!=4096 && N!=2048 && N!=1536 && N!=1024 && N!=512 && N!=256 && N!=128)
	{
	    printf("fft doesn't support size = %d\n", size);
	    assert(0);
	}
//	int logN=(int)(log10(size)/log10(2));
//	if(pow(2.0,logN)!=N || N>4096)
//	{
//		printf("fft doesn't support size=%d\n",size);
//		assert(0);
//	}

    int   sofarRadix[maxFactorCount], actualRadix[maxFactorCount], remainRadix[maxFactorCount];
    int   nFactor;
    int   count;
    double sqrtn;

    fft.pi = 4*atan(1.0);

    transTableSetup(sofarRadix, actualRadix, remainRadix, &nFactor, &size);

    permute(size, nFactor, actualRadix, remainRadix, Iin, Qin, Iout, Qout);

    for (count=1; count<=nFactor; count++)
		twiddleTransf(sofarRadix[count], actualRadix[count], remainRadix[count], Iout, Qout);

    sqrtn = sqrt((double)size);
    sqrtn = 1.0 / sqrtn;
    for(count = 0; count < size; count++)
    {
		Iout[count] = (float)(Iout[count] * sqrtn);
		Qout[count] = (float)(Qout[count] * sqrtn);
    }
    return size;
}                                                // FFT



int fft_InputOutput3(cfloat *in, int size, cfloat *out)
{
	int N  = size;
	if(N!=4096 && N!=2048 && N!=1536 && N!=1024 && N!=512 && N!=256 && N!=128)
		{
		    printf("fft doesn't support size = %d\n", size);
		    assert(0);
		}
//	int logN =(int)(log10(size)/log10(2));
//
//	if(pow(2.0, logN) != N || N > 4096)
//	{
//		printf("fft doesn't support size = %d\n", size);
//		assert(0);
//	}



	C2R(in, size, Iin, Qin);

	fft_InputOutput5(Iin, Qin, size, Iout, Qout);

	R2C(Iout, Qout, size, out);

	return size;
}
