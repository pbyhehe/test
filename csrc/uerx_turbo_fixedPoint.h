/*************************************
*
*  Fixed Point Definitions for Turbo Code for LTE based on 3GPP TS 36.212 v1.2.0
*
*  input:
*  
*  output:
*
*/

#ifndef FIXED_POINT_H
#define FIXED_POINT_H

/* floating-point / fixed-point switch*/
/* The Turbo Encoder needs to be fixed-point */


#define FXP_ENABLED

#ifdef FXP_ENABLED
    /* fixed point implementation */
    typedef int fixed;
#else
    /* floating point implementation*/
	typedef double fixed;
#endif /* FXP_ENABLED */

typedef short INT16;
typedef unsigned char UINT8;
typedef char INT8;

/* definition of data formats */
#define BETA_BW 8
#define BETA_DO_ROUNDING 0  
#define BETA_DO_SATURATION 1 /*1*/
#define BETA_IS_SIGNED 1
#define BETA_SYMMETRIC 0
#define BETA_SHIFT_AFTER_MUL 0

#define ALPHA_BW 8
#define ALPHA_DO_ROUNDING 0
#define ALPHA_DO_SATURATION 1 /*1*/
#define ALPHA_IS_SIGNED 1
#define ALPHA_SYMMETRIC 0
#define ALPHA_SHIFT_AFTER_MUL 0

/* switch to determine fixed-point settings to use*/

#define FXP_SETTINGS_NUR 

#ifdef FXP_SETTINGS_NUR
    /* use settings according to Nur */
    #define EXT_BW 5
    #define EXT_DO_ROUNDING 1   /*1*/
    #define EXT_DO_SATURATION 1 /*1*/
    #define EXT_IS_SIGNED 1
    #define EXT_SYMMETRIC 0
    #define EXT_SHIFT_AFTER_MUL -3   /* -2 because of multiplication with 0.75=(3 << 2) */
	                                 /* -1 because of compression */
    #define SYS_INT_BW 7
    #define SYS_INT_DO_ROUNDING 1   /*1*/
    #define SYS_INT_DO_SATURATION 1 /*1*/
    #define SYS_INT_IS_SIGNED 1
    #define SYS_INT_SYMMETRIC 0
    #define SYS_INT_SHIFT_AFTER_MUL 1  /* =1 because of decompresion */
#else
    /* use settings according to PRISM */
    #define EXT_BW 6
    #define EXT_DO_ROUNDING 1   /*1*/
    #define EXT_DO_SATURATION 1 /*1*/
    #define EXT_IS_SIGNED 1
    #define EXT_SYMMETRIC 0
    #define EXT_SHIFT_AFTER_MUL -2   /* -2 because of multiplication with 0.75=(3 << 2) */
	                                 /* -1 because of compression */
    #define SYS_INT_BW 7
    #define SYS_INT_DO_ROUNDING 1   /*1*/
    #define SYS_INT_DO_SATURATION 1 /*1*/
    #define SYS_INT_IS_SIGNED 1
    #define SYS_INT_SYMMETRIC 0
    #define SYS_INT_SHIFT_AFTER_MUL 0  /* =1 because of decompresion */
#endif

/* data container for fixed point definition */
typedef struct FP_DATA_FORMAT {

	int bitwidth;            /* data bitwidth */
	int is_signed;           /* signed or unsigned data */
	int symmetric;           /* whether data range is symmetric or not */
	double flt_fp_scaling;   /* scaling factor between floating and fixed point */ 

	int shiftAfterMul;       /* shift after multiplication */
	int do_saturation;       /* whether do saturation on this data type or not */
	int do_rounding;         /* whether do rounding or not */

}t_fp_data_format;

/* global fixed point data formats */
extern t_fp_data_format FP_FORMAT_BETA;
extern t_fp_data_format FP_FORMAT_ALPHA;
extern t_fp_data_format FP_FORMAT_EXT;
extern t_fp_data_format FP_FORMAT_SYS_INT;

fixed fpAdd(fixed a, fixed b, t_fp_data_format * dataFormat);
fixed fpMul(fixed a, fixed b, t_fp_data_format * dataFormat);

fixed minValue(t_fp_data_format * dataFormat);
fixed maxValue(t_fp_data_format * dataFormat);

// int float_to_fix(float x, int frac_length, int bit_width);

void fpInit();


#endif /* FIXED_POINT_H */


