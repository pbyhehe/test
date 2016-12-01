/*************************************
*
*  Standard Definitions for Turbo Code for LTE based on 3GPP TS 36.212 v1.2.0
*
*  input:
*  
*  output:
*
*
*/

#ifndef TURBO_H
#define TURBO_H

#include "uerx_turbo_fixedPoint.h"

/* with this switch TURBO en/decoder can be chosen for LTE and UMTS */
/* WARNING: Do not enable this switch here - do it from outside !!! */
 #define TURBO_LTE 

/* LTE specific */
#define LTE_MAX_BLOCK_LEN 6144
#define LTE_MIN_BLOCK_LEN 40
#define LTE_NUM_BLOCK_LEN 188

/* UMTS specific */
#define UMTS_MAX_BLOCK_LEN 5114
#define UMTS_MIN_BLOCK_LEN 40

#define MAX_PSISOS 8
#define NUM_WINDOWS_MAX 256
#define MAX_WINDOW_LENGTH 6144
#define MIN_WINDOW_LENGTH 10
#define MAX_HALF_ITERATIONS 100


#ifdef FXP_ENABLED
#define TURBO_EXT_SCALING 4 /* =0.75*4 */
#else
#define TURBO_EXT_SCALING 0.75
#endif

#define TURBO_FIRST_HALFITERATION 1
#define TURBO_SECOND_HALFITERATION 2


/* constants from SOPHIA */
/* #define TURBO_SOPHIA */
#define MIN_ALPHA_SOPHIA -64

/* global constants*/
extern int LTE_CODE_BLOCK_LENGTHS[LTE_MAX_BLOCK_LEN];
extern int LTE_INTERLEAVE_PARAM_F1[LTE_MAX_BLOCK_LEN];
extern int LTE_INTERLEAVE_PARAM_F2[LTE_MAX_BLOCK_LEN];
extern int NUMBER_OF_PSISOS[4];

/* data container for a single parallel SISO unit */
typedef struct PSISO_UNIT {
  
    /*internal SISO working buffers*/
    fixed  **alpha_fifo;      /* FIFO buffer for alpha calculation */
    fixed  beta[8];          /* buffer for saving the last beta */

	/* buffers for Next Iteration Initialization (for both Half-Iterations)*/
	/*  + initial states */
	/* fixed alpha_init_state[8]; */
	/* fixed **beta_init_states;  */

	/*  + final states */
	fixed alpha_final_state_1_1[8];
	fixed alpha_final_state_1_2[8];
    fixed alpha_final_state_2_1[8];
    fixed alpha_final_state_2_2[8];
	fixed **beta_final_states_1;
	fixed **beta_final_states_2;
	fixed alpha_final_state_1[8];
	fixed alpha_final_state_2[8];

	int psiso_length;        /* length of 1 PSISO ( =blockLength / numberOfSISOs ) */
	int *window_lengths;     /* length of single sliding windows */
	int window_length_max;   /* max length of sliding window */
	int window_length_min;   /* min length of sliding window */
	int numberOfWindows;     /* number of windows (micro-windows) within 1 SISO */

    int half_iteration;     /* Half-Iteration */
	int psiso_no;           /* number of PSISO unit */

} t_psiso_unit;

/* LTE specific */
void lteTurboInterleave(fixed input[LTE_MAX_BLOCK_LEN], fixed output[LTE_MAX_BLOCK_LEN], int blockLengthIdx);
void lteTurboDeinterleave(fixed input[LTE_MAX_BLOCK_LEN], fixed output[LTE_MAX_BLOCK_LEN], int blockLengthIdx);

/* UMTS specific */
void umtsTurboInterleave(fixed input[UMTS_MAX_BLOCK_LEN], fixed output[UMTS_MAX_BLOCK_LEN], int blockLength);
void umtsTurboDeinterleave(fixed input[UMTS_MAX_BLOCK_LEN], fixed output[UMTS_MAX_BLOCK_LEN], int blockLength);
void umtsInterleavePerm(int permutation[UMTS_MAX_BLOCK_LEN], int blockLength);
static int mulMod(int a, int b, int p);


void turboFreePsisoUnit(t_psiso_unit* psiso);

void turboAllocPsisoUnit(t_psiso_unit* psiso,    /* In/Out */
					  int numberOfPSISOs,        /* In */
					  int numberOfWindows,       /* In */
					  int windowLength,          /* In */
					  int windowLengthLast       /* In */
					  );

void turboResetPsisoUnit(t_psiso_unit* psiso,    /* In/Out */
					  int blockLengthIdx,        /* In */
					  int psisoLength,           /* In */
					  int numberOfWindows,       /* In */
					  int windowLength,          /* In */
					  int windowLengthLast,      /* In */
					  int psisoNo,               /* In */
					  int halfIteration          /* In */
					  );

int checkSISOParams(int blockLength,                            /* In */
                    int numberOfPSISOs,                         /* In */
                    int PSISOLength,                            /* In */
                    int PSISOLengthLast,                        /* In */
                    int numberOfWindows,                        /* In */
                    int windowLength,                           /* In */
                    int lastWindowLengthPSISO,                  /* In */
                    int lastWindowLengthPSISOLast               /* In */
					);

int blockLengthIdx(int blockLength);
fixed maxVal(fixed a, fixed b);

fixed cal_max(fixed R[], int inlen);

#endif /* TURBO_H */

