/*************************************
*
*  Fixed Point Definitions for Turbo Code for LTE based on 3GPP TS 36.212 v1.2.0
*
*  input:
*  
*  output:
*
*
*/

#include <math.h>
#include <float.h>

#include "uerx_turbo_fixedPoint.h"

#include "C:\ti\pdk_C6670_1_1_2_6\packages\ti\csl\csl.h"


/* global fixed point data formats */
#pragma DATA_SECTION(FP_FORMAT_ALPHA,"uerx_turbo_par")//2015-12-29xiugai
t_fp_data_format FP_FORMAT_ALPHA;
#pragma DATA_SECTION(FP_FORMAT_BETA,"uerx_turbo_par")
t_fp_data_format FP_FORMAT_BETA;
#pragma DATA_SECTION(FP_FORMAT_EXT,"uerx_turbo_par")
t_fp_data_format FP_FORMAT_EXT;
#pragma DATA_SECTION(FP_FORMAT_SYS_INT,"uerx_turbo_par")
t_fp_data_format FP_FORMAT_SYS_INT;

fixed fpAdd(fixed a, fixed b, t_fp_data_format * dataFormat)
/*************************
* fixed point addition
*    conditional saturation
*
* input
*    a and b (values for addition)
*    bitwidth after addition
*    switch for conditional saturation
*
* returns:
*    a+b
*
*************************/
{
	fixed res;

#ifdef FXP_ENABLED

	res = a+b;
	
	/* conditional saturation */
	if(dataFormat->do_saturation) {
        if(res > maxValue(dataFormat))
			res = maxValue(dataFormat);
		if(res < minValue(dataFormat))
			res = minValue(dataFormat);
	}

#else
    res = a+b;
#endif /* FXP_ENABLED */

	return res;
}

fixed fpMul(fixed a, fixed b, t_fp_data_format * dataFormat)
/*************************
* fixed point multiplication
*    conditional saturation
*    conditional truncation / rounding after shifting
*
* input
*    a and b (values for multiplication)
*    fixed point data format
*
* returns:
*    a*b
*
*************************/
{
    fixed res;

#ifdef FXP_ENABLED
    double tmp=a*b;

	/*shift the result*/
	if(dataFormat->shiftAfterMul >= 0)
		tmp *= (1<<dataFormat->shiftAfterMul);
	else
		tmp /= (1<<(-dataFormat->shiftAfterMul));

	/* conditional rounding */
    if(dataFormat->do_rounding) {
		/*do rounding*/
		if(tmp > 0.0) {
			/* tmp = floor((double) tmp + 0.5); */
			tmp = (int) (tmp + 0.5);
		    res = (fixed) tmp;
		} else {
			/* tmp = -floor((double) -tmp + 0.5); */
			tmp = -(int) (-tmp + 0.5);
			res = (fixed) tmp;
		}
	} else {
		/*do truncation*/
		res = (fixed) tmp;
	}

	/* conditional saturation */
	if(dataFormat->do_saturation) {
        if(res > maxValue(dataFormat))
			res = maxValue(dataFormat);
		if(res < minValue(dataFormat))
			res = minValue(dataFormat);
	}


#else
    res = a*b;

#endif /* FXP_ENABLED */

	return res;
}



void fpInit()
/*************************
* fixed point initialization
*    initializes data formats
*
* input
*
* returns:
*
*************************/
{
	/*initialize beta data format*/
	FP_FORMAT_BETA.bitwidth       = BETA_BW;
	FP_FORMAT_BETA.do_rounding    = BETA_DO_ROUNDING;
	FP_FORMAT_BETA.do_saturation  = BETA_DO_SATURATION;
	FP_FORMAT_BETA.is_signed      = BETA_IS_SIGNED;
	FP_FORMAT_BETA.symmetric      = BETA_SYMMETRIC;
	FP_FORMAT_BETA.shiftAfterMul  = BETA_SHIFT_AFTER_MUL;
	FP_FORMAT_BETA.flt_fp_scaling = 1.0;


	/*initialize alpha data format*/
	FP_FORMAT_ALPHA.bitwidth       = ALPHA_BW;
	FP_FORMAT_ALPHA.do_rounding    = ALPHA_DO_ROUNDING;
	FP_FORMAT_ALPHA.do_saturation  = ALPHA_DO_SATURATION;
	FP_FORMAT_ALPHA.is_signed      = ALPHA_IS_SIGNED;
	FP_FORMAT_ALPHA.symmetric      = ALPHA_SYMMETRIC;
	FP_FORMAT_ALPHA.shiftAfterMul  = ALPHA_SHIFT_AFTER_MUL;
	FP_FORMAT_ALPHA.flt_fp_scaling = 1.0;

	/*initialize extrinsic data format*/
	FP_FORMAT_EXT.bitwidth       = EXT_BW;
	FP_FORMAT_EXT.do_rounding    = EXT_DO_ROUNDING;
	FP_FORMAT_EXT.do_saturation  = EXT_DO_SATURATION;
	FP_FORMAT_EXT.is_signed      = EXT_IS_SIGNED;
	FP_FORMAT_EXT.symmetric      = EXT_SYMMETRIC;
	FP_FORMAT_EXT.shiftAfterMul  = EXT_SHIFT_AFTER_MUL;
	FP_FORMAT_EXT.flt_fp_scaling = 1.0;

	/* initialize internal systematic data format */
	FP_FORMAT_SYS_INT.bitwidth      = SYS_INT_BW;
	FP_FORMAT_SYS_INT.do_rounding   = SYS_INT_DO_ROUNDING;
	FP_FORMAT_SYS_INT.do_saturation = SYS_INT_DO_SATURATION;
	FP_FORMAT_SYS_INT.is_signed     = SYS_INT_IS_SIGNED;
	FP_FORMAT_SYS_INT.symmetric     = SYS_INT_SYMMETRIC;
	FP_FORMAT_SYS_INT.shiftAfterMul = SYS_INT_SHIFT_AFTER_MUL;
	FP_FORMAT_SYS_INT.flt_fp_scaling = 1.0;

}

fixed minValue(t_fp_data_format * dataFormat)
/*************************
* min value of a floating / fixed point number 
* according to the given dataFormat
*
* input
*   dataFormat
*
* returns:
*   min value
*
*************************/
{
	fixed minValue;

#ifdef FXP_ENABLED
	int bitwidth = dataFormat->bitwidth;

	if(dataFormat->is_signed) {
		if(dataFormat->symmetric) {
		    minValue = -((1 << (bitwidth-1)) -1);
		} else {
		    minValue = -(1 << (bitwidth-1) );
		}
	} else {
		minValue = (fixed) 0;
	}	

#else
	minValue = -DBL_MAX;
#endif /* FXP_ENABLED */

	return minValue;
}


fixed maxValue(t_fp_data_format * dataFormat)
/*************************
* max value of a floating / fixed point number 
* according to the given dataFormat
*
* input
*   dataFormat
*
* returns:
*   max value
*
*************************/
{
	fixed maxValue;

#ifdef FXP_ENABLED
	int bitwidth = dataFormat->bitwidth;

	if(dataFormat->is_signed) {
		if(dataFormat->symmetric) {
	        maxValue = (1 << (bitwidth-1) )-1;
		} else {
	        maxValue = (1 << (bitwidth-1) )-1;
		}

	} else {
	    maxValue = (1 << (bitwidth) )-1;
	}

#else
	maxValue = DBL_MAX;
#endif /* FXP_ENABLED */

	return maxValue;
}

/*int float_to_fix(float x, int frac_length, int bit_width)
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
}    */

