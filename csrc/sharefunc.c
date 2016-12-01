#include "sharefunc.h"


#define RECORDNOW                   1
#define KEEP_OUTPUT_FILE			0


cint16 ZEROCINT16;
cint32 ZEROCINT32;
cfloat ZEROCFLOAT;


int abs1(cint32 a)
{
	int b;
//	b = (int)sqrt(float(a.re * a.re + a.im * a.im));
	b = (int)sqrt((float)(a.re * a.re + a.im * a.im));
	return b;
}

int abs2(cint16 a)
{
	int b;
	b = (int)sqrt((float)(a.re * a.re + a.im * a.im));
	return b;
}

float abs3(cfloat a)
{
	float b;
	b = (float)sqrt(a.re * a.re + a.im * a.im);
	return b;
}

int abs4(cint16 a)
{
	int b;
	b = a.re * a.re + a.im * a.im;
	return b;
}

float abs5(cfloat a)
{
	float b;
	b = a.re * a.re + a.im * a.im;
	return b;
}

void C2R(cfloat *cin, int size, float* Iout, float* Qout)
{
	int i;

	for(i = 0; i < size; i++)
	{
		Iout[i] = cin[i].re;
		Qout[i] = cin[i].im;
	}
}

void R2C(float* Iin, float* Qin, int size, cfloat *cout)
{
	int i;

	for(i = 0; i < size; i++)
	{
		cout[i].re = Iin[i];
		cout[i].im = Qin[i];
	}
}

int intbits_fixd(float datain)
{
	int   intbits = 0;
	float abs_datain;
	
	abs_datain = (float)fabs(datain);
	
	while (!(abs_datain >= 0.5 && abs_datain < 1))
	{
		if (abs_datain < 0.5)
		{
			abs_datain    *= 2;
			intbits--;
		}
		else if(abs_datain >= 1)
		{
			abs_datain    /= 2;
			intbits++;
		}
	}
	
	return intbits;
}

int float_to_fix(float x, int frac_length, int bit_width)
{
	int result;
	int max; 
	int min;
	int a;
	float b;

	max = pow(2, bit_width-1) -1;
	
	min = -pow(2, bit_width-1);
	
    a = floor(x*pow(2, frac_length));

	b = x*pow(2, frac_length);

	if ((b - a) > 0.5)
	{
		result = ceil(x*pow(2, frac_length));
	}
	else
	{
		result = floor(x*pow(2, frac_length));
	}

	if (result > max)
		result = max;
	else if(result < min)
		result = min;

	return result;
}

int float_to_fix_round(float x, int frac_length, int bit_width)
{
	int result;
	int max; 
	int min;
	
	max = pow(2, bit_width-1) -1;
	
	min = -pow(2, bit_width-1);

    result = floor(x*pow(2, frac_length)+0.5);
	
	if (result > max)
		result = max;
	else if(result < min)
		result = min;
	
	return result;
}  

float fix_to_float(int x, int frac_length)  
{
	float result;

    result = (float)(x) / (pow(2, frac_length));

	return result;
}

int fix_add(int x, int Q1, int y, int Q2, int bit_width, int *result_Q)
{
	int result;
	int z;
	int max, min;

	max = pow(2, bit_width-1) -1;

	min = -pow(2, bit_width-1);

    if (Q1 == Q2)
	{
		result = x + y;
	}
	else if (Q1 > Q2)     // 对y左移（Q1-Q2）位
	{
		z = y*(pow(2, Q1-Q2));

		result = x + z;
	}
	else
	{
		z = x*(pow(2, Q2-Q1));

		result = y + z;
	}

	*result_Q = (Q1 > Q2) ? Q1 : Q2;

	if (result > max)
		result = max;
	else if (result < min)
		result = min;

	return result;
}

int fix_minus(int x, int Q1, int y, int Q2, int bit_width, int *result_Q)
{
	int result;
	int z;
	int max, min;
	
	max = pow(2, bit_width-1) -1;
	
	min = -pow(2, bit_width-1);
	
    if (Q1 == Q2)
	{
		result = x - y;
	}
	else if (Q1 > Q2)     // 对y左移（Q1-Q2）位
	{
		z = y*(pow(2, Q1-Q2));
		
		result = x - z;
	}
	else
	{
		z = x*(pow(2, Q2-Q1));
		
		result = z - y;
	}

	*result_Q = (Q1 > Q2) ? Q1 : Q2;

	if (result > max)
		result = max;
	else if (result < min)
		result = min;
	
	return result;
}

int fix_multiplex(int x, int Q1, int y, int Q2, int bit_width, int *result_Q)
{
	int result;

	int max, min;
	
	max = pow(2, bit_width-1) -1;
	
	min = -pow(2, bit_width-1);

	result = x*y;

	*result_Q = Q1 + Q2;

	if (result > max)
		result = max;
	else if (result < min)
		result = min;

    return result;
}

int fix_divise(int x, int Q1, int y, int Q2)
{
	int result;

	float flt_x, flt_y, flt_result;
	
    flt_x = fix_to_float(x, Q1);

	flt_y = fix_to_float(y, Q2);

    flt_result = flt_x/flt_y;

//    result = float_to_fix(flt_result, Q1-Q2, alpha_width);

	result = float_to_fix(flt_result, Q1-Q2, 5);

    return result;
}  

int fix_opposite(int x, int bit_width)
{
	int result;
	int max; 
	int min;
	
	max = pow(2, bit_width-1) -1;
	
	min = -pow(2, bit_width-1);

    result = -x;
	
	if (result > max)
		result = max;
	else if(result < min)
		result = min;
	
	return result;
}
 
int downsample(cfloat *inbuf, int inlen, int N_downsample, cfloat *outbuf)
{
	int i, outlen;
	float gain;

	assert((inlen%N_downsample) == 0);

	outlen = inlen / N_downsample;

	gain = sqrt(N_downsample);

	for(i=0; i<outlen; i++)
	{
		outbuf[i] = cfloatmultiplyfloat(inbuf[N_downsample*i],gain);
	}

	return outlen;
}



int Normalize(cfloat *inbuf, int inlen, cfloat *outbuf, float *maxvalue)
{
	float sum;
	int i;
	
	sum = 0.0;
	*maxvalue = 0.0;
	for(i = 0; i < inlen; i++)
	{
		sum += inbuf[i].re * inbuf[i].re;
		sum += inbuf[i].im * inbuf[i].im;
	}
	sum = (float)sqrt(sum);
	sum = 1.0f / sum;
	for(i = 0; i < inlen; i++)
	{
		outbuf[i].re = inbuf[i].re * sum;
		if(*maxvalue < fabs(outbuf[i].re))
			*maxvalue = (float)fabs(outbuf[i].re);
		outbuf[i].im = inbuf[i].im * sum;
		if(*maxvalue < fabs(outbuf[i].im))
			*maxvalue = (float)fabs(outbuf[i].im);
	}
	return inlen;
}     

float AMP(cfloat *a)
{
	return (float)sqrt((float)(*a).re * (float)(*a).re + (float)(*a).im * (float)(*a).im);
}

float FI(cfloat *a)
{
	return (float)atan2((float)(*a).im, (float)(*a).re);
}

void ZERO(cfloat *a)
{
	(*a).re = 0.0;
	(*a).im = 0.0;
}

cint16 setcint16(int16 re, int16 im)                          // below is complex math 
{
	cint16 c;
	c.re = re;
	c.im = im;
	return c;
}

cfloat setcfloat(float re, float im)
{
	cfloat c;
	c.re = re;
	c.im = im;
	return c;
}

cfloat add(cfloat a, cfloat b)
{
	cfloat c;
	c.re = a.re + b.re;
	c.im = a.im + b.im;
	return c;
}

cfloat opposite(cfloat a)
{
	cfloat c;
	c.re = - a.re;
	c.im = - a.im;
	return c;
}

cfloat cfloatsubcfloat(cfloat a, cfloat b)
{
	cfloat c;
	c.re = a.re - b.re;
	c.im = a.im - b.im;
	return c;
}

cfloat cfloatmultiplycfloat(cfloat a, cfloat b)
{
	cfloat ab;
	ab.re = a.re * b.re - a.im * b.im;
	ab.im = a.re * b.im + a.im * b.re;
	return ab;
}

cfloat  cfloatmultiplyfloat(cfloat a, float b)
{
	cfloat c;
	c.re = a.re * b;
	c.im = a.im * b;
	return c;
}

cfloat cfloatmultiplyint(cfloat a, int b)
{
	cfloat c;
	c.re = a.re * b;
	c.im = a.im * b;
	return c;
}

cint16 cint16multiplycint16(cint16 a, cint16 b)
{
	cint16 c;
	c.re = a.re * b.re - a.im * b.im;
	c.im = a.re * b.im + a.re * b.im;
	return c;
}

cint32 cint32multiplycint32(cint32 a, cint32 b)
{
	cint32 c;
	c.re = a.re * b.re - a.im * b.im;
	c.im = a.re * b.im + a.re * b.im;
	return c;
}

cfloat cfloatdividecfloat(cfloat *a, cfloat *b)
{
	cfloat c;
	
	float aamp = AMP(a);
	float afi = FI(a);
	float bamp = AMP(b);
	float bfi = FI(b);
	float camp, cfi;
	
	if(bamp)
	{
		camp = aamp / bamp;
		cfi = afi - bfi;
		
		c.re = camp * (float)cos(cfi);
		c.im = camp * (float)sin(cfi);
	}
	else
	{
		c.re = 100000000.0;
		c.im = 100000000.0;
	}
	
	return c;
}

cfloat cfloatdividefloat(cfloat *a, float *b)
{
	cfloat c;
	
	if(*b)
	{
		c.re = (*a).re / (*b);
		c.im = (*a).im / (*b);
	}
	else
	{
		c.re = 100000000.0;
		c.im = 100000000.0;
	}
	
	return c;
}

void cfloataddequalcfloat(cfloat *a, cfloat b)
{
	(*a).re += b.re;
	(*a).im += b.im;
}

void cint32addequalcint32(cint32 *a, cint32 b)
{
	(*a).re += b.re;
	(*a).im += b.im;
}

void cint16addequalcint16(cint16 *a, cint16 b)
{
	(*a).re += b.re;
	(*a).im += b.im;
}

void cfloatsubequalcfloat(cfloat *a, cfloat b)
{
	(*a).re -= b.re;
	(*a).im -= b.im;
}

//void operator*=(cint16 &a, int16 b)
//{
//	a.re *= b;
//	a.im *= b;
//}

void cint16multiplyequalfloat(cint16 *a, float b)
{
	(*a).re = (int16)((*a).re * b);
	(*a).im = (int16)((*a).im * b);
}

void cfloatmultiplyequalfloat(cfloat *a, float b)
{
	(*a).re *= b;
	(*a).im *= b;
}

void cfloatmultiplyequalcfloat(cfloat *a, cfloat b)
{
	float re, im;
	re = (*a).re;
	im = (*a).im;
	(*a).re = re * b.re - im * b.im;
	(*a).im = re * b.im + im * b.re;
}

void cfloatdivideequalcfloat(cfloat *a,  cfloat *b)
{
	float aamp = AMP(a);
	float afi = FI(a);
	
	float bamp  = AMP(b);
	float bfi = FI(b);
	
	if(bamp)
	{
		aamp /= (float)bamp;
		afi  -= (float)bfi;
		(*a).re = aamp * (float)cos(afi);
		(*a).im = aamp * (float)sin(afi);
	}
	else
	{
		(*a).re = 100000000.0;
		(*a).im = 100000000.0;
	}	
}

void cfloatdivideequalfloat(cfloat *a, float b)
{
	if(b)
	{
		(*a).re /= b;
		(*a).im /= b;
	}
	else
	{
		(*a).re = 100000000.0;
		(*a).im = 100000000.0;
	}
}

cfloat conj1(cfloat a)
{
	cfloat b;
	b.re = a.re;
	b.im = -a.im;
	return b;
}

cint16 conj3(cint16 a)
{
	cint16 b;
	b.re = a.re;
	b.im = -a.im;
	return b;
}

cint32 conj2(cint32 a)
{
	cint32 b;
	b.re = a.re;
	b.im = -a.im;
	return b;
}

int convolution(cfloat *inbuf, int inlen, cfloat *pfactor, cfloat *pstat, int statlen, cfloat *outbuf)
{
	int i, j;
	int loop;
	cfloat sum;
	
	for(i = 0; i < inlen; i++)
	{
		sum.re = 0.0;
		sum.im = 0.0;
		
		if(i < statlen)
			loop = i;
		else
			loop = statlen - 1;
		
		for(j = 0; j <= loop; j++)
			cfloataddequalcfloat(&sum,cfloatmultiplycfloat(inbuf[i - j],pfactor[j]));
		
		if(i < statlen)//
		{
			for(j = i + 1; j < statlen; j++)
				cfloataddequalcfloat(&sum,cfloatmultiplycfloat(pstat[j - i - 1],pfactor[j]));
		}
		
		outbuf[i] = sum;
	}
	
	for(i = 0; i < statlen; i++)
		pstat[i] = inbuf[inlen - i - 1];
	
	return inlen;
}                                                     // complex math <-

float getgauss(float mean, float sigma) 
{
	/* 
	* This uses the fact that a Rayleigh-distributed random variable R, with
	* the probability distribution F(R) = 0 if R &lt; 0 and F(R) =
	* 1 - exp(-R^2/2*sigma^2) if R &gt;= 0, is related to a pair of Gaussian
	* variables C and D through the transformation C = R * cos(theta) and
	* D = R * sin(theta), where theta is a uniformly distributed variable
	* in the interval (0, 2*pi()). From Contemporary Communication Systems
	* USING MATLAB(R), by John G. Proakis and Masoud Salehi, published by
	* PWS Publishing Company, 1998, pp 49-50. This is a pretty good book. 
	*/
	
    float u, r;              // uniform and Rayleigh random variables 
	
	u  = (float)rand();      // generate a uniformly distributed random number u between 0 and 1 - 1E-6
	u /= RAND_MAX;
	if(u >= 1)
		u = 0.999999f;       // FLOAT
	                         // generate a Rayleigh-distributed random number r using u 
    r = sigma * (float)sqrt( 2.0 * log( 1.0 / (1.0 - u) ) );
	
	u  = (float)rand();
	u /= RAND_MAX;
	
	if(u >= 1)
		u = 0.999999f;
                             // generate and return a Gaussian-distributed random number using r and u 
    return( (float)( mean + r * cos(2 * 3.1415926 * u) ) );
}
