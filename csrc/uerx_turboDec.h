/*************************************
*
* Turbo Decoder for LTE based on 3GPP TS 36.212 v1.2.0
*
*  input:
*  
*  output:
*
*
************************************/

#ifndef TURBO_DEC_H
#define TURBO_DEC_H

#include "uerx_turbo.h"
#include "uerx_turboDebug.h"
#include "../csrc/typedefine.h"

typedef   short	  int16;

int turboDec(fixed codeBlock[3*LTE_MAX_BLOCK_LEN+12],        /* In */
			 int blockLength,                                 /* In */
			 int numCodeBlockBanks,                           /* In */
			 int numberOfPSISOs,                              /* In */
			 int PSISOLength,                                 /* In */
			 int PSISOLengthLast,                             /* In */
			 int numberOfWindows,                             /* In */
			 int windowLength,                                /* In */
			 int lastWindowLengthPSISO,                       /* In */
			 int lastWindowLengthPSISOLast,                   /* In */
			 int maxHalfIterations,                           /* In */
			 int debugLevel,                                  /* In */
			 int *numIterations,                              /* Out */
			 int16 decodedBits[LTE_MAX_BLOCK_LEN]             /* Out */
			 );

void turboHalfIteration(fixed systematic[LTE_MAX_BLOCK_LEN],   /* In */
						fixed parity[LTE_MAX_BLOCK_LEN],         /* In */
						fixed trellisTerm[8],                /* In */
						t_psiso_unit psisoUnits[MAX_PSISOS], /* In */
						int halfIteration,                   /* In */
						int numberOfPSISOs,                  /* In */
						int numberOfWindows,                 /* In */
						int blockLengthIdx,                  /* In */
						fixed ext[LTE_MAX_BLOCK_LEN],            /* In/Out */
						fixed softOutput[LTE_MAX_BLOCK_LEN],     /* Out */
						int debugLevel,
						int it                      /* In */
						);

void turboTrellisTermination(fixed sys[3],                         /* In */
							 fixed parity[3],                      /* In */
							 fixed trellisTerm[8]                  /* Out */
							 );

void turboInitAlpha(t_psiso_unit psisoUnits[MAX_PSISOS], /* In/Out */
					int psiso,                            /* In */
					int window,                           /* In */
					int numberOfPSISOs,                   /* In */
					int numberOfWindows,                  /* In */
					int halfIteration,
					int it);                   /* In */

void turboInitBeta(t_psiso_unit psisoUnits[MAX_PSISOS], /* In/Out */
				   fixed trellisTerm[8],                /* In */
				   int psiso,                           /* In */
				   int window,                          /* In */
				   int numberOfPSISOs,                  /* In */
				   int numberOfWindows,                 /* In */
				   int halfIteration);                  /* In */

void turboAlpha(fixed sys, fixed parity, fixed alphaPrev[8], fixed alphaNext[8]);
void turboBeta(fixed sys, fixed parity, fixed betaPrev[8], fixed betaNext[8]);
void turboLambda(fixed sys, fixed parity, fixed alpha[8], fixed beta[8], fixed *lambda);

void turboHardDecoding(fixed softOutput[LTE_MAX_BLOCK_LEN],  /* In */
					   int blockLengthIdx,               /* In */
					   fixed hardOutput[LTE_MAX_BLOCK_LEN]   /* Out */
					   );

#endif /* TURBO_DEC_H */

