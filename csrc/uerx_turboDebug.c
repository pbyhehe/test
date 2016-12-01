/*************************************
*
*  Debug and Trace Functionality for Turbo Decoder
*
*  input:
*  
*  output:
*
*
************************************/

#include <stdio.h>

#include "uerx_turbo.h"
#include "uerx_turboDebug.h"


/* data container tracing intermediate Turbo Data */
#pragma DATA_SECTION(traceData, "uerx_data")
t_trace_data traceData;



void turboTraceInit(int debugLevel,             /* In */
					int numberOfPSISOs,         /* In */
					fixed numberOfWindows         /* In */
					)
/*************************
* initializes trace functionality
* creates data container for traceing
*************************/
{
	int i, j, w;
    if(debugLevel & TURBO_DEBUG_ALPHA_INIT) {
		/* init data container for alpha init states */
		for(i=0; i<MAX_PSISOS; i++) {
			for(j=0; j<8; j++) {
				/* reset alpha init state */
                traceData.alpha_init_states[i][j] = (fixed) 0;
			}
		}
	}

    if(debugLevel & TURBO_DEBUG_BETA_INIT) {
		/* init data container for beta init states */
		for(i=0; i<MAX_PSISOS; i++) {
			if(i < numberOfPSISOs) {
			    //traceData.beta_init_states[i] = (fixed**) malloc( numberOfWindows*sizeof(fixed*) );
			    traceData.beta_init_states[i] = (fixed**)malloc(numberOfWindows * sizeof(fixed*));//new fixed*[numberOfWindows];
                for(w=0; w<numberOfWindows; w++) {
				    //traceData.beta_init_states[i][w] = (fixed*) malloc( 8*sizeof(fixed) );
				    traceData.beta_init_states[i][w] = (fixed*)malloc(sizeof(fixed)*8);//new fixed[8];
				
				    /* reset beta init states */
				    for(j=0; j<8; j++) {
                        traceData.beta_init_states[i][w][j] = (fixed) 0;
					}
				}
			} else {
                traceData.beta_init_states[i] = NULL;
			}
		}
	}

	if(debugLevel & TURBO_DEBUG_EXT) {
		/* init data container */
        for(i=0; i<MAX_PSISOS; i++) {
			for(j=0; j<LTE_MAX_BLOCK_LEN; j++) {
                traceData.ext_out[i][j] = (fixed) 0;
                traceData.sys_in[i][j] = (fixed) 0;
                traceData.par_in[i][j] = (fixed) 0;
                traceData.ext_in[i][j] = (fixed) 0;
			}
		}
	}
}

void turboTraceClose(int debugLevel,             /* In */
					 int numberOfPSISOs,         /* In */
					 int numberOfWindows         /* In */
					 )
/*************************
* closes trace functionality
* free memory for data container
*************************/
{
	int i ,w;
    if(debugLevel & TURBO_DEBUG_BETA_INIT) {
		/* destroy data container for beta init states */
		for(i=0; i<numberOfPSISOs; i++) {
			for(w=0; w<numberOfWindows; w++) {
				//free( traceData.beta_init_states[i][w] );
				free(traceData.beta_init_states[i][w]);
			}
			//free( traceData.beta_init_states[i] );
			free(traceData.beta_init_states[i]);
		}
	}
}



void turboDebugAlpha(fixed* alpha,          /* In */
					 int psiso,             /* In */
					 int window,            /* In */
					 int windowStep,        /* In */
					 int numberOfPSISOs,    /* In */
					 int numberOfWindows,   /* In */
					 int windowLength,      /* In */
					 int debugLevel         /* In */
					 )
/*************************
* writes the alpha state into trace container
* input:
*    alpha
*    current PSISO
*    current window
*    current windowStep
*    number of PSISOs
*    number of Windows
*    windowLength
*
* returns:
*
*************************/
{
	int i;
	if(debugLevel & TURBO_DEBUG_ALPHA_INIT) {
		for(i=0; i<8; i++) {
			traceData.alpha_init_states[psiso][i] = (fixed) alpha[i];
		}
	}
}


void turboDebugBeta(fixed* beta,            /* In */
					 int psiso,             /* In */
					 int window,            /* In */
					 int windowStep,        /* In */
					 int numberOfPSISOs,    /* In */
					 int numberOfWindows,   /* In */
					 int windowLength,      /* In */
					 int debugLevel         /* In */
					 )
/*************************
* writes the beta state into trace container
* input:
*    beta
*    current PSISO
*    current window
*    current windowStep
*    number of PSISOs
*    number of Windows
*    windowLength
*
* returns:
*
*************************/
{
	int i;

	if(debugLevel & TURBO_DEBUG_BETA_INIT) {
		for(i=0; i<8; i++) {
			traceData.beta_init_states[psiso][window][i] = (fixed) beta[i];
		}
	}
}

void turboDebugSisoIn(fixed sys_in,               /* In */
					  fixed par_in,               /* In */
					  fixed ext_in,               /* In */
				      int psiso,                  /* In */
					  int iCyclePerHalfIteration, /* In */
				      int halfIteration,          /* In */
					  int debugLevel              /* In */
					  )
/*************************
* writes the PSISO input into trace container
* input:
*    systematic
*    parity
*    current PSISO
*    cycle per Half-Iteration
*    halfIteration
*    debug level
*
* returns:
*
*************************/
{
	if(debugLevel & TURBO_DEBUG_SISO_IN) {
        traceData.sys_in[psiso][iCyclePerHalfIteration]=sys_in;
        traceData.par_in[psiso][iCyclePerHalfIteration]=par_in;
		traceData.ext_in[psiso][iCyclePerHalfIteration]=ext_in;
	}
}



 
void turboDebugSoftout(fixed softOut,               /* In */
				       int psiso,                   /* In */
				       int iCyclePerHalfIteration,  /* In */
				       int halfIteration,           /* In */
				       int debugLevel               /* In */
				      )
/*************************
* writes the soft output information into file
* input:
*    systematic information
*    parallel SISO index
*    cycle per Half-Iteration
*    halfIteration
*    debug level
*
* returns:
*
*************************/
{
    if(debugLevel & TURBO_DEBUG_SOFTOUT) {
		traceData.soft_out[psiso][iCyclePerHalfIteration]=softOut;
	}
}


void turboDebugSoftoutIn(fixed softOutSys,            /* In */
						 fixed softOutExt,            /* In */
				         int psiso,                   /* In */
				         int iCyclePerHalfIteration,  /* In */
				         int halfIteration,           /* In */
				         int debugLevel               /* In */
				        )
/*************************
* writes the systematic information into file (required for soft output)
* input:
*    systematic information
*    parallel SISO index
*    cycle per Half-Iteration
*    halfIteration
*    debug level
*
* returns:
*
*************************/
{
    if(debugLevel & TURBO_DEBUG_SOFTOUT_IN) {
		traceData.soft_out_sys[psiso][iCyclePerHalfIteration]=softOutSys;
		traceData.soft_out_ext[psiso][iCyclePerHalfIteration]=softOutExt;
	}
}




