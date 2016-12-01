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

#include <stdio.h>
#include <math.h>

#include "uerx_turbo.h"
#include "uerx_turboDec.h"
#include "uerx_turboDebug.h"


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
			)
/*************************
* Turbo Decoder for LTE as defined in TS 36.212 v1.2.0
* input:
*    received code block with systematic and 2 x partity information
*    blockLength
*    number of SISO units running in parallel
*    number of Windows with 1 parallel SISO
*    max number of iterations
*
* returns:
*    -1 if blockLength not found
*    index otherwise such that  LTE_CODE_BLOCK_LENGTHS[index] = blockLength
*    actual number of iterations
*    decoded code block
*
*************************/
{
	int BLIdx, it=0, i, bl;
	/* int debugLevel = TURBO_DEBUG_STAKES; */

	#pragma SET_DATA_SECTION("uerx_data")
	/* parallel SISO units for both Half-Iterations */
	static t_psiso_unit psisoUnits[MAX_PSISOS];

	/* systematic and parity information */
	static fixed sys1[LTE_MAX_BLOCK_LEN], parity1[LTE_MAX_BLOCK_LEN], sys2[LTE_MAX_BLOCK_LEN], parity2[LTE_MAX_BLOCK_LEN];
	/* tail for systematic and parity information */
	static fixed tail_sys1[3], tail_parity1[3], tail_sys2[3], tail_parity2[3];
	/* states for trellis termination (for both Half-Iterations) */
	static fixed trellis_term1[8], trellis_term2[8];

	/* common buffer (for both Half-Iterations) for extrinsic information and soft(hard)-output */
	static fixed ext[LTE_MAX_BLOCK_LEN]={0};
	static fixed softOutput[LTE_MAX_BLOCK_LEN]={0};
	static fixed hardBits[LTE_MAX_BLOCK_LEN]={0};
	#pragma SET_DATA_SECTION()
	/* first check whether the given code block length is valid or not */
	BLIdx = blockLengthIdx(blockLength);
//printf("BLIdx=%i,blockLength=%i\n",BLIdx,blockLength);
    /* check if given windowLengths are valid or not */
    bl=checkSISOParams(blockLength,
                       numberOfPSISOs,
                       PSISOLength,   
                       PSISOLengthLast,
                       numberOfWindows,
                       windowLength,   
                       lastWindowLengthPSISO,
                       lastWindowLengthPSISOLast);
	if(BLIdx >= 0 && bl > 0) {

		/* allocate parallel SISO units */
		for(i=0; i<numberOfPSISOs; i++) {
			if(i == (numberOfPSISOs-1)) {
				/* setting for last PSISO */
			    turboAllocPsisoUnit(&(psisoUnits[i]), 
					                numberOfPSISOs, 
									numberOfWindows, 
									windowLength,
									lastWindowLengthPSISOLast);

			    turboResetPsisoUnit(&(psisoUnits[i]), 
					                BLIdx, 
									PSISOLengthLast, 
									numberOfWindows, 
									windowLength, 
									lastWindowLengthPSISOLast, 
									i, 
									TURBO_FIRST_HALFITERATION);
			} else {
				/* settings for PSISOs except the last PSISO */
			    turboAllocPsisoUnit(&(psisoUnits[i]), 
					                numberOfPSISOs, 
									numberOfWindows, 
									windowLength,
									lastWindowLengthPSISO);
			    
				turboResetPsisoUnit(&(psisoUnits[i]), 
					                BLIdx, 
									PSISOLength, 
									numberOfWindows, 
									windowLength, 
									lastWindowLengthPSISO, 
									i, 
									TURBO_FIRST_HALFITERATION);
			}

		}

		/*initialize fixed-point data formats*/
		fpInit();

		/* disassemble the code block into systematic and parity information (for both Half-Iterations) */
		for(i=0; i<blockLength; i++) {
            sys1[i] = -codeBlock[3*i];           
			parity1[i] = -codeBlock[3*i+1]; 
			parity2[i] = -codeBlock[3*i+2];     
			ext[i]     = (fixed) 0;
		}

		/* interleave the systematic information */
#ifdef TURBO_LTE
        lteTurboInterleave(sys1, sys2, BLIdx);
#else
        umtsTurboInterleave(sys1, sys2, blockLength);
#endif /* TURBO_LTE */

		for(i=0; i<3; i++){    /* disassemble the tail */
			tail_sys1[i]    = -codeBlock[3*blockLength+2*i];
			tail_parity1[i] = -codeBlock[3*blockLength+2*i+1];
			tail_sys2[i]    = -codeBlock[3*blockLength+2*i+6];
			tail_parity2[i] = -codeBlock[3*blockLength+2*i+7];
		}

        /* trellis termination for initialization of beta (for both Half-Iterations) */
        turboTrellisTermination(tail_sys1, tail_parity1, trellis_term1); //printf("term1");
        turboTrellisTermination(tail_sys2, tail_parity2, trellis_term2); //printf("term2");

        /******* run the actual Turbo Decoder *******/
		it=0;
        while(maxHalfIterations>=0) {
			/* while loop over half-Iterations (see break conditions below) */
            /* while loop is never executed if maxHalfIterations==0         */

			/* first Half-Iteration */
			turboHalfIteration(sys1, 
			                 parity1, 
			                 trellis_term1, 
							 psisoUnits, 
							 TURBO_FIRST_HALFITERATION,
							 numberOfPSISOs, 
							 numberOfWindows,
							 BLIdx,
							 ext,
							 softOutput,
							 debugLevel,
							 it
							 );

			/* hard decision on soft output */
			turboHardDecoding(softOutput, BLIdx, hardBits);

			it++;

			if(it>=maxHalfIterations) {
				break;
			}

    		/* interleave extrinsic information */
#ifdef TURBO_LTE
			lteTurboInterleave(ext, ext, BLIdx);
#else
			umtsTurboInterleave(ext, ext, blockLength);
#endif /* TURBO_LTE */
			
			/* second Half-Iteration */
			turboHalfIteration(sys2, 
			                 parity2, 
			                 trellis_term2, 
							 psisoUnits, 
							 TURBO_SECOND_HALFITERATION,
							 numberOfPSISOs, 
							 numberOfWindows,
							 BLIdx,
							 ext, 
							 softOutput,
							 debugLevel,
							 it
							 );

			/* hard decision on soft output */
			turboHardDecoding(softOutput, BLIdx, hardBits);

			/* deinterleave hard decoded bits */
#ifdef TURBO_LTE
			lteTurboDeinterleave(hardBits, hardBits, BLIdx);
#else
			umtsTurboDeinterleave(hardBits, hardBits, blockLength);
#endif /* TURBO_LTE */

			it++;

			if(it>=maxHalfIterations) {
				break;
			}

			/* de-interleave extrinsic information */
#ifdef TURBO_LTE
			lteTurboDeinterleave(ext, ext, BLIdx);
#else
			umtsTurboDeinterleave(ext, ext, blockLength);
#endif /* TURBO_LTE */

		}  /* end while loop: */

		/* hard decision for bypassed Turbo Decoder (maxHalfIterations=0) */
		if(maxHalfIterations==0) {
            /* hard decision on systematic input */
			turboHardDecoding(sys1, BLIdx, hardBits);
		}

		for(i=0; i<blockLength; i++)
        {   decodedBits[i] = (fixed) hardBits[i];  }

		*numIterations = it;

		/* destroy parallel SISO units */
		for(i=0; i<numberOfPSISOs; i++) {
			turboFreePsisoUnit( &(psisoUnits[i]) );
		}
        

	} /* end if: BLIdx >= 0 */

	return BLIdx;
} 


void turboHalfIteration(fixed systematic[LTE_MAX_BLOCK_LEN],   /* In */
                      fixed parity[LTE_MAX_BLOCK_LEN],         /* In */
					  fixed trellisTerm[8],                    /* In */
					  t_psiso_unit psisoUnits[MAX_PSISOS],     /* In */
					  int halfIteration,                       /* In */
					  int numberOfPSISOs,                      /* In */
					  int numberOfWindows,                     /* In */
					  int blockLengthIdx,                      /* In */
					  fixed ext[LTE_MAX_BLOCK_LEN],            /* In/Out */
					  fixed softOutput[LTE_MAX_BLOCK_LEN],     /* Out */
					  int debugLevel,                          /* In */
					  int it
					  )
/*************************
* Performs one Half-Iteration of Turbo decoder on all parallel SISOs
* input:
*    systematic information
*    parity information
*    array of parallel SISO units
*    number of parallel SISOs
*
* returns:
*    extrinsic information
*    softOut information
*
*************************/
{
	int psiso, wnd, i, windowSize, windowSizePrev, iPsiso_lb, iPsiso_a;
	fixed sys[LTE_MAX_BLOCK_LEN];
                                                   
#ifdef TURBO_LTE
	int blockLength = LTE_CODE_BLOCK_LENGTHS[blockLengthIdx];
#else
    int blockLength = blockLengthIdx + UMTS_MIN_BLOCK_LEN;
#endif /* TURBO_LTE */

	/*Declaration of read/write pointer for alpha/beta/lambda calculation*/
	/*  + Code:  p_[r|w][s|p|a|b]_[a|b|l]                                */
	/*           r=read, w=write                                         */
	/*           s=systematiy, p=parity                                  */
	/*           a=alpha, b=beta, l=lambda, ext=extinsic                 */
	/*                                                                   */
	/*  + e.g. p_rs_a:  pointer for reading systematic information to    */
	/*                  compute alpha                                    */
	/*         p_w_a:   pointer for writing alpha (into the FIFO)        */
	/*-------------------------------------------------------------------*/
	int p_rs_a, p_rp_a, p_ra_a, p_rs_b, p_rp_b, p_rs_l, p_rp_l;
	int p_ra_l;
	int p_w_a, p_w_ext;
	fixed lambda;

	/* add extrinsic information to systematic information */
	for(i=0; i<blockLength; i++) {
		sys[i] = fpAdd(systematic[i], fpMul(ext[i], (fixed) 1, &FP_FORMAT_SYS_INT), &FP_FORMAT_SYS_INT);
	}
	//  sys[i] = systematic[i] + ext[i];

	/* initialize pointer for reading/writing */
    p_rs_a  =0;
	p_rs_b  =-1;
	p_rs_l  =-1;
	p_w_ext =-1;
	windowSizePrev=0;

	for(psiso=0; psiso<numberOfPSISOs; psiso++) {

        iPsiso_lb=0;
		iPsiso_a=0;
		for(wnd=0; wnd<numberOfWindows; wnd++) {

			windowSize = psisoUnits[psiso].window_lengths[wnd];
//printf("wnd=%d,windowSize=%i\n", wnd,windowSize);
		    /* initialize alpha */
			turboInitAlpha(psisoUnits, psiso, wnd, numberOfPSISOs, numberOfWindows, halfIteration,it);

			if(wnd==0) {
				/* trace init state */
				turboDebugAlpha(psisoUnits[psiso].alpha_fifo[0], psiso, wnd, 0, numberOfPSISOs, numberOfWindows, windowSize, debugLevel);
			}

			p_w_a=1;
			/* compute alpha */
			for(i=0; i<windowSize; i++) {
				p_rp_a = p_rs_a;
				p_ra_a = p_w_a-1;
				turboAlpha(sys[p_rs_a], 
					       parity[p_rp_a], 
						   psisoUnits[psiso].alpha_fifo[p_ra_a], 
						   psisoUnits[psiso].alpha_fifo[p_w_a]);

				turboDebugSisoIn(systematic[p_rs_a], parity[p_rp_a], ext[p_rs_a], psiso, iPsiso_a, halfIteration, debugLevel); 
				/* turboDebugAlpha(psisoUnits[psiso].alpha_fifo[p_w_a], psiso, wnd, i, numberOfPSISOs, numberOfWindows, windowSize, debugLevel); */

				p_rs_a++;
				p_w_a++;
				iPsiso_a++;
			}

			/* initialize beta */
			turboInitBeta(psisoUnits, 
				          trellisTerm, 
						  psiso, 
						  wnd, 
						  numberOfPSISOs, 
						  numberOfWindows, 
						  halfIteration);

			/* trace beta state */
			turboDebugBeta(psisoUnits[psiso].beta, psiso, wnd, -1, numberOfPSISOs, numberOfWindows, windowSize, debugLevel);

			p_rs_b  += windowSize+windowSizePrev;
			p_rs_l  += windowSize+windowSizePrev;
			p_w_ext += windowSize+windowSizePrev;
			p_ra_l   = windowSize-1;
			/* compute lambda and beta*/
			for(i=0; i<windowSize; i++) {
				
				p_rp_l=p_rs_l;
				turboLambda(sys[p_rs_l], 
					        parity[p_rp_l], 
							psisoUnits[psiso].alpha_fifo[p_ra_l], 
							psisoUnits[psiso].beta,
							&lambda);

				/* write softoutput and extrinsic information */
                softOutput[p_w_ext] = lambda;
				ext[p_w_ext] = fpMul((lambda - sys[p_w_ext]), (fixed) TURBO_EXT_SCALING, &FP_FORMAT_EXT);

//				ext[p_w_ext] = fpMul((lambda - sys[p_w_ext]), 1, &FP_FORMAT_EXT);

//				ext[p_w_ext] = lambda / 2 - fpAdd(sys[p_w_ext], ext[p_w_ext], &FP_FORMAT_EXT);

//              ext[p_w_ext] = lambda / 2 - (sys[p_w_ext] + ext[p_w_ext]);
				
				turboDebugSoftoutIn(sys[p_w_ext], (lambda - sys[p_w_ext]), psiso, iPsiso_lb, halfIteration, debugLevel);
				turboDebugSoftout(softOutput[p_w_ext], psiso, iPsiso_lb, halfIteration, debugLevel);

				p_rp_b = p_rs_b;
				turboBeta(sys[p_rs_b], 
					      parity[p_rp_b], 
						  psisoUnits[psiso].beta, 
						  psisoUnits[psiso].beta);

				/* turboDebugBeta(psisoUnits[psiso].beta, psiso, wnd, i, numberOfPSISOs, numberOfWindows, windowSize, debugLevel); */
				
				p_w_ext--;
				p_rs_b--;
				p_rs_l--;
                p_ra_l--;
				iPsiso_lb++;
			} /* end: loop over single window */

			/* store the previous window size */
			windowSizePrev=windowSize;

		/*save last beta state after last sliding window */
		for(i=0; i<8; i++) {
			if(halfIteration == TURBO_FIRST_HALFITERATION) {
			    psisoUnits[psiso].beta_final_states_1[wnd][i] = psisoUnits[psiso].beta[i];
			} else {
			    psisoUnits[psiso].beta_final_states_2[wnd][i] = psisoUnits[psiso].beta[i];
			}
		}  

		} /* end: loop over sliding windows */

/*        for(i=0; i<8; i++) {
			if(halfIteration == TURBO_FIRST_HALFITERATION) {
			    psisoUnits[psiso].beta_final_states_1[numberOfWindows-1][i] = psisoUnits[psiso].beta[i];
			} else {
			    psisoUnits[psiso].beta_final_states_2[numberOfWindows-1][i] = psisoUnits[psiso].beta[i];
			}
		}  */

if((it>>1)%2==0) {
	for(i=0; i<8; i++) {
		if(halfIteration == TURBO_FIRST_HALFITERATION) {
			psisoUnits[psiso].alpha_final_state_1_1[i] = psisoUnits[psiso].alpha_fifo[windowSizePrev][i];
		} else {
			psisoUnits[psiso].alpha_final_state_2_1[i] = psisoUnits[psiso].alpha_fifo[windowSizePrev][i];
		}
	}
} else {
	for(i=0; i<8; i++) {
		if(halfIteration == TURBO_FIRST_HALFITERATION) {
			psisoUnits[psiso].alpha_final_state_1_2[i] = psisoUnits[psiso].alpha_fifo[windowSizePrev][i];
		} else {
			psisoUnits[psiso].alpha_final_state_2_2[i] = psisoUnits[psiso].alpha_fifo[windowSizePrev][i];
		}
	}
}	

	} /* end: loop over parallel SISO units */

	/*save last alpha state after decoding (for completeness)*/
/*	for(i=0; i<8; i++) {
		if(halfIteration == TURBO_FIRST_HALFITERATION) {
			psisoUnits[numberOfPSISOs-1].alpha_final_state_1[i] = psisoUnits[numberOfPSISOs-1].alpha_fifo[windowSizePrev][i];
		} else {
			psisoUnits[numberOfPSISOs-1].alpha_final_state_2[i] = psisoUnits[numberOfPSISOs-1].alpha_fifo[windowSizePrev][i];
		}
	} */
}



void turboTrellisTermination(fixed sys[3],                         /* In */
						     fixed parity[3],                      /* In */
						     fixed trellisTerm[8]                  /* Out */
						    )
/*************************
* Trellis Termination of Turbo decoder as defined in TS 36.212 v1.2.0 
* input:
*    systematic information
*    parity information
*    number of parallel SISOs
*    number of windows within one PSISO
*
* returns:
*    initial state for beta accumulation (written in psisoUnits[])
*
*************************/
{
	fixed beta[8];
	int i;

	/* start at n=blockLength+3 int the trellis */
    beta[0]=(fixed) 0.0;

	/* n=blockLength+2: beta[0] stays at 0.0 */
	beta[1]=fpAdd(sys[2],  parity[2], &FP_FORMAT_BETA);

	/* n=blockLength+1: beta[0] stays at 0.0 */
	beta[3]=fpAdd(beta[1], parity[1], &FP_FORMAT_BETA);
	beta[2]=fpAdd(beta[1], sys[1],    &FP_FORMAT_BETA);
	beta[1]=fpAdd(sys[1],  parity[1], &FP_FORMAT_BETA);

	/* n=blockLength: beta[0] stays at 0.0, beta[7]->beta[3] */
	beta[7]=beta[3];
	beta[6]=fpAdd(beta[3], sys[0]+parity[0], &FP_FORMAT_BETA);
	beta[5]=fpAdd(beta[2], sys[0]          , &FP_FORMAT_BETA);
	beta[4]=fpAdd(beta[2],        parity[0], &FP_FORMAT_BETA);
	beta[3]=fpAdd(beta[1],        parity[0], &FP_FORMAT_BETA);
	beta[2]=fpAdd(beta[1], sys[0]          , &FP_FORMAT_BETA);
	beta[1]=fpAdd(sys[0],         parity[0], &FP_FORMAT_BETA);

	for(i=0; i<8; i++) {
        trellisTerm[i] = beta[i]; //printf("%i",trellisTerm[i]);
	}   

    //  my addtional
/*	for (i = 0; i < 8; i++)
	{
		trellisTerm[i] = -3;
	}  */  

}

void turboAlpha(fixed sys, fixed parity, fixed alphaPrev[8], fixed alphaNext[8])
/*************************
* Alpha Accumulation (Butterfly operation)
* input:
*    systematic information
*    parity information
*    previous alpha
*
* returns:
*   next alpha
*
*************************/
{

	fixed alpha0;
	fixed alphaTmp[8];

	fixed alpha_max;

	/* state alpha[0] is always 0 due to normalization */
	alphaTmp[0] = (fixed) 0;

	alpha0 = maxVal((fixed) 0, alphaPrev[1] + sys + parity);    /* normalization (actual alpha[0]) */

	/* Butterfly operation */
	alphaTmp[1] = maxVal(alphaPrev[2] + sys         , alphaPrev[3] +       parity);
	alphaTmp[2] = maxVal(alphaPrev[4] +       parity, alphaPrev[5] + sys         );
	alphaTmp[3] = maxVal(alphaPrev[6] + sys + parity, alphaPrev[7]               );
	alphaTmp[4] = maxVal((fixed) 0    + sys + parity, alphaPrev[1]               );
	alphaTmp[5] = maxVal(alphaPrev[2] +       parity, alphaPrev[3] + sys         );
	alphaTmp[6] = maxVal(alphaPrev[4] + sys         , alphaPrev[5] +       parity);
	alphaTmp[7] = maxVal(alphaPrev[6]               , alphaPrev[7] + sys + parity);       

/*	alpha0 = maxVal((fixed) 0, alphaPrev[1]);    

	alphaTmp[1] = maxVal(alphaPrev[2] +       parity, alphaPrev[3] + sys         );
	alphaTmp[2] = maxVal(alphaPrev[4] + sys         , alphaPrev[5] +       parity);
	alphaTmp[3] = maxVal(alphaPrev[6]               , alphaPrev[7] + sys + parity);
	alphaTmp[4] = maxVal((fixed) 0                  , alphaPrev[1] + sys + parity);
	alphaTmp[5] = maxVal(alphaPrev[2] + sys         , alphaPrev[3]       + parity);
	alphaTmp[6] = maxVal(alphaPrev[4] +       parity, alphaPrev[5] + sys         );
	alphaTmp[7] = maxVal(alphaPrev[6] + sys + parity, alphaPrev[7]               );       */


    /* normalization with alpha0 and saturation */
	alphaNext[0] = (fixed) 0;
    alphaNext[1] = fpAdd(alphaTmp[1], -alpha0, &FP_FORMAT_ALPHA);
    alphaNext[2] = fpAdd(alphaTmp[2], -alpha0, &FP_FORMAT_ALPHA);
    alphaNext[3] = fpAdd(alphaTmp[3], -alpha0, &FP_FORMAT_ALPHA);
    alphaNext[4] = fpAdd(alphaTmp[4], -alpha0, &FP_FORMAT_ALPHA);
    alphaNext[5] = fpAdd(alphaTmp[5], -alpha0, &FP_FORMAT_ALPHA);
    alphaNext[6] = fpAdd(alphaTmp[6], -alpha0, &FP_FORMAT_ALPHA);
    alphaNext[7] = fpAdd(alphaTmp[7], -alpha0, &FP_FORMAT_ALPHA);    

}

void turboBeta(fixed sys, fixed parity, fixed betaPrev[8], fixed betaNext[8]) 
/*************************
* Alpha Accumulation (Butterfly operation)
* input:
*    systematic information
*    parity information
*    previous beta
*
* returns:
*   next beta
*
*************************/
{
    fixed beta0;
	fixed betaTmp[8];

	fixed beta_max;
	
	/* state beta[0] is always 0 due to normalization */
	betaTmp[0] = (fixed) 0;

	beta0 = maxVal((fixed) 0, betaPrev[4] + sys + parity);    /* normalization (actual beta[0]) */

	/* Butterfly operation */	
	betaTmp[1] = maxVal((fixed) 0   + sys + parity, betaPrev[4]               );
	betaTmp[2] = maxVal(betaPrev[1] + sys         , betaPrev[5] +       parity);
	betaTmp[3] = maxVal(betaPrev[1] +       parity, betaPrev[5] + sys         );
	betaTmp[4] = maxVal(betaPrev[2] +       parity, betaPrev[6] + sys         );
	betaTmp[5] = maxVal(betaPrev[2] + sys         , betaPrev[6] +       parity);
	betaTmp[6] = maxVal(betaPrev[3] + sys + parity, betaPrev[7]               );
	betaTmp[7] = maxVal(betaPrev[3]               , betaPrev[7] + sys + parity);      

/*	beta0 = maxVal((fixed) 0, betaPrev[4]      ); 
	betaTmp[1] = maxVal((fixed) 0                 , betaPrev[4] + sys + parity);
	betaTmp[2] = maxVal(betaPrev[1] +       parity, betaPrev[5] + sys         );
	betaTmp[3] = maxVal(betaPrev[1] + sys         , betaPrev[5] +       parity);
	betaTmp[4] = maxVal(betaPrev[2] + sys         , betaPrev[6] +       parity);
	betaTmp[5] = maxVal(betaPrev[2] +       parity, betaPrev[6] + sys         );
	betaTmp[6] = maxVal(betaPrev[3]               , betaPrev[7] + sys + parity);
	betaTmp[7] = maxVal(betaPrev[3] + sys + parity, betaPrev[7]               );      */

	/* normalization with beta0 and saturation */
	betaNext[0] = (fixed) 0;
	betaNext[1] = fpAdd(betaTmp[1], -beta0, &FP_FORMAT_BETA);
	betaNext[2] = fpAdd(betaTmp[2], -beta0, &FP_FORMAT_BETA);
	betaNext[3] = fpAdd(betaTmp[3], -beta0, &FP_FORMAT_BETA);
	betaNext[4] = fpAdd(betaTmp[4], -beta0, &FP_FORMAT_BETA);
	betaNext[5] = fpAdd(betaTmp[5], -beta0, &FP_FORMAT_BETA);
	betaNext[6] = fpAdd(betaTmp[6], -beta0, &FP_FORMAT_BETA);
	betaNext[7] = fpAdd(betaTmp[7], -beta0, &FP_FORMAT_BETA);    

}

void turboLambda(fixed sys, fixed parity, fixed alpha[8], fixed beta[8], fixed* lambda)
/*************************
* Lambda Calculation
* input:
*    systematic information
*    parity information
*    alpha state
*    beta state
*
* returns:
*   next lambda
*
*************************/
{
    fixed m[8], M[2];

	m[0] = maxVal(alpha[0] + beta[4], alpha[1] + beta[0]) + parity;
	m[1] = maxVal(alpha[1] + beta[4], alpha[0] + beta[0]);
	m[2] = maxVal(alpha[2] + beta[1], alpha[3] + beta[5]);
	m[3] = maxVal(alpha[3] + beta[1], alpha[2] + beta[5]) + parity;
	m[4] = maxVal(alpha[4] + beta[6], alpha[5] + beta[2]);
	m[5] = maxVal(alpha[5] + beta[6], alpha[4] + beta[2]) + parity;
	m[6] = maxVal(alpha[6] + beta[3], alpha[7] + beta[7]) + parity;
	m[7] = maxVal(alpha[7] + beta[3], alpha[6] + beta[7]);

	M[0] = maxVal(maxVal(m[0], m[2]), maxVal(m[4], m[6]) ) + sys;
	M[1] = maxVal(maxVal(m[1], m[3]), maxVal(m[5], m[7]) );            
 

/*	m[0] =  alpha[0] + beta[4] - sys - parity;
	m[1] =  alpha[3] + beta[5] - sys + parity;
	m[2] =  alpha[4] + beta[6] - sys + parity;
	m[3] =  alpha[7] + beta[7] - sys - parity;
	m[4] =  alpha[1] + beta[0] - sys - parity;
	m[5] =  alpha[2] + beta[1] - sys + parity;
	m[6] =  alpha[5] + beta[2] - sys + parity;
	m[7] =  alpha[6] + beta[3] - sys - parity;

	M[0] = cal_max(m, 8);

	m[0] =  alpha[0] + beta[0] + sys + parity;
	m[1] =  alpha[1] + beta[4] + sys + parity;
	m[2] =  alpha[2] + beta[5] + sys - parity;
	m[3] =  alpha[5] + beta[6] + sys - parity;
	m[4] =  alpha[6] + beta[7] + sys + parity;
	m[5] =  alpha[7] + beta[3] + sys + parity;
	m[6] =  alpha[4] + beta[2] + sys - parity;
	m[7] =  alpha[3] + beta[1] + sys - parity;   

    M[1] = cal_max(m, 8);       */

/*	m[0] =  alpha[0] + beta[4]               ;
	m[1] =  alpha[3] + beta[5] +       parity;
	m[2] =  alpha[4] + beta[6] +       parity;
	m[3] =  alpha[7] + beta[7]               ;
	m[4] =  alpha[1] + beta[0]               ;
	m[5] =  alpha[2] + beta[1] +       parity;
	m[6] =  alpha[5] + beta[2] +       parity;
	m[7] =  alpha[6] + beta[3]               ;
	
	M[0] = cal_max(m, 8);
	
	m[0] =  alpha[0] + beta[0] + sys + parity;
	m[1] =  alpha[1] + beta[4] + sys + parity;
	m[2] =  alpha[2] + beta[5] + sys         ;
	m[3] =  alpha[5] + beta[6] + sys         ;
	m[4] =  alpha[6] + beta[7] + sys + parity;
	m[5] =  alpha[7] + beta[3] + sys + parity;
	m[6] =  alpha[4] + beta[2] + sys         ;
	m[7] =  alpha[3] + beta[1] + sys         ;   
	
    M[1] = cal_max(m, 8);      */

	*lambda = (M[0] - M[1]);
}


void turboInitAlpha(t_psiso_unit psisoUnits[MAX_PSISOS], /* In/Out */
				   int psiso,                            /* In */
				   int window,                           /* In */
				   int numberOfPSISOs,                   /* In */
				   int numberOfWindows,                  /* In */
				   int halfIteration,
				   int it)                    /* In */
/*************************
* Next Iteration Initialization of alpha
*    - stores final states of alpha
*    - initializes alpha
*
* input:
*    PSISO units
*    index of PSISO for alpha initialization
*    number of PSISOs
*    halfIteration
*
* returns:
*   PSISO units
*
*************************/
{
	int i, prevWindowLength;
	fixed *alpha_final_state, *alpha_init_state;
    
    if(window==0) {
		/* does next iteration initialization */

		/* store final state & initialize alpha */
		if(psiso > 0) {
			/* last window size of previous PSISO */
			prevWindowLength = psisoUnits[psiso-1].window_lengths[numberOfWindows-1];

			if((it>>1)%2==0) {
			  if(halfIteration == TURBO_FIRST_HALFITERATION) {
			  	/* first half-iteration */
			  	alpha_final_state = psisoUnits[psiso-1].alpha_final_state_1_2;
			  } else {
			  	/* second half-iteration */
			  	alpha_final_state = psisoUnits[psiso-1].alpha_final_state_2_2;
			  }
      } else {
      	if(halfIteration == TURBO_FIRST_HALFITERATION) {
			  	/* first half-iteration */
			  	alpha_final_state = psisoUnits[psiso-1].alpha_final_state_1_1;
			  } else {
			  	/* second half-iteration */
			  	alpha_final_state = psisoUnits[psiso-1].alpha_final_state_2_1;
			  }
      }

			/* initial state and final state are written into same location */
			alpha_init_state  = alpha_final_state;
			for(i=0; i<8; i++) {
				psisoUnits[psiso].alpha_fifo[0][i] = alpha_init_state[i];
				alpha_final_state[i] = psisoUnits[psiso-1].alpha_fifo[prevWindowLength][i];
			}

		} else {
			/* first PSISO */
			psisoUnits[psiso].alpha_fifo[0][0] = (fixed) 0;
			for(i=1; i<8; i++) {
#ifdef TURBO_SOPHIA
				psisoUnits[psiso].alpha_fifo[0][i] = (fixed) MIN_ALPHA_SOPHIA;
#else
				psisoUnits[psiso].alpha_fifo[0][i] = minValue(&FP_FORMAT_ALPHA);
#endif /* TURBO_SOPHIA */

			}
		}

	} else {
		/* just copy the last element of the alpha-FIFO to the beginning*/
		/* That's NOT done in HW (just for simulation) */

		/* previous window size of current PSISO */
		prevWindowLength = psisoUnits[psiso].window_lengths[window-1];

		for(i=0; i<8; i++) {
			psisoUnits[psiso].alpha_fifo[0][i]=psisoUnits[psiso].alpha_fifo[prevWindowLength][i];
		}
	}
}


void turboInitBeta(t_psiso_unit psisoUnits[MAX_PSISOS], /* In/Out */
				   fixed trellisTerm[8],                /* In */
				   int psiso,                           /* In */
				   int window,                          /* In */
				   int numberOfPSISOs,                  /* In */
				   int numberOfWindows,                 /* In */
				   int halfIteration)                   /* In */
/*************************
* Next Iteration Initialization of beta
*    - stores final states of beta
*    - initializes beta
*
* input:
*    PSISO units
*    state of trellis termination for this Half-Iteration
*    index of PSISO for beta initialization
*    index of window for beta initialization
*    number of PSISOs
*    number of windows within one PSISO
*    halfIteration
*
* returns:
*   PSISO units
*
*************************/
{
	int i;
	fixed *beta_init_state, *beta_final_state;


	if(window > 0) {
    	/* store the final state of beta */
	    if(halfIteration == TURBO_FIRST_HALFITERATION) {
		    /* first Half-Iteration */
		    beta_final_state = psisoUnits[psiso].beta_final_states_1[window-1];
		} else {
		    /* second Half-Iteration */
		    beta_final_state = psisoUnits[psiso].beta_final_states_2[window-1];
		}
   
		for(i=0; i<8; i++) {
		    beta_final_state[i] = psisoUnits[psiso].beta[i];
		}
	} 

    /* initialization of beta */
	if(window < numberOfWindows) {
		if(window == (numberOfWindows-1)) {
			if(psiso == (numberOfPSISOs-1)) {
				/* last window AND last PSISO -> use trellis termiantion */
				beta_init_state = trellisTerm;
			} else {
				/* last window NOT last PSISO -> 1st beta from psiso+1 */
				if(halfIteration == TURBO_FIRST_HALFITERATION) {
					/* first Half-Iteration */
					beta_init_state = psisoUnits[psiso+1].beta_final_states_1[0];
				} else {
					/* second Half-Iteration */
					beta_init_state = psisoUnits[psiso+1].beta_final_states_2[0];
				}
			}
		} else {
			if(halfIteration == TURBO_FIRST_HALFITERATION) {
				/* first Half-Iteration */
				beta_init_state = psisoUnits[psiso].beta_final_states_1[window+1];
			} else {
				/* second Half-Iteration */
				beta_init_state = psisoUnits[psiso].beta_final_states_2[window+1];
			}
		}

		for(i=0; i<8; i++) {
			psisoUnits[psiso].beta[i] = beta_init_state[i];
		}
	}

}



void turboHardDecoding(fixed softOutput[LTE_MAX_BLOCK_LEN],  /* In */
					   int blockLengthIdx,               /* In */
					   fixed hardOutput[LTE_MAX_BLOCK_LEN]   /* Out */
					  )
/*************************
* Turbo Hard Decoding
*
* input:
*   softbits
*
* returns:
*   hardbits
*
*************************/
{
    int i;

#ifdef TURBO_LTE
	int blockLength = LTE_CODE_BLOCK_LENGTHS[blockLengthIdx];
#else
    int blockLength = blockLengthIdx + UMTS_MIN_BLOCK_LEN;
#endif /* TURBO_LTE */

	for(i=0; i<blockLength; i++) {
		if(softOutput[i] >= ((fixed) 0) )
			hardOutput[i] = (fixed) 1;
		else
			hardOutput[i] = (fixed) 0;
	}
}


