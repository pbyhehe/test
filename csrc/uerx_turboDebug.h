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

#ifndef TURBO_DEBUG_H
#define TURBO_DEBUG_H

/* define different debug levels */
#define TURBO_DEBUG_ALPHA_INIT   1
#define TURBO_DEBUG_BETA_INIT    2
#define TURBO_DEBUG_SISO_IN      4
#define TURBO_DEBUG_ALPHA_STATES 8
#define TURBO_DEBUG_EXT          16
#define TURBO_DEBUG_SOFTOUT_IN   32
#define TURBO_DEBUG_SOFTOUT      64
#define TURBO_DEBUG_STAKES       128
#define TURBO_DEBUG_ADDR         256

#define TURBO_DEBUG_APPEND       2048

/* combine debug levels */
#define TURBO_DEBUG_NONE    0   /* no trace at all */
#define TURBO_DEBUG_ALL     511 /* all */

#include "uerx_turbo_fixedPoint.h"
#include "uerx_turbo.h"


/* data container tracing intermediate Turbo Data */
typedef struct TRACE_DATA {

	/* init states for alpha-computation */
    fixed alpha_init_states[MAX_PSISOS][8];

	/* alpha state during alpha computation */
	fixed **alpha_states[MAX_PSISOS];

	/* init states for beta-computation */
	fixed **beta_init_states[MAX_PSISOS];

	/* input systematic information */
	fixed sys_in[MAX_PSISOS][LTE_MAX_BLOCK_LEN];

	/* input parity information */
	fixed par_in[MAX_PSISOS][LTE_MAX_BLOCK_LEN];

	/* input extrinsic information */
	fixed ext_in[MAX_PSISOS][LTE_MAX_BLOCK_LEN];

	/* output extrinsic information */
	fixed ext_out[MAX_PSISOS][LTE_MAX_BLOCK_LEN];

	/* systematic information required for soft output */
	fixed soft_out_sys[MAX_PSISOS][LTE_MAX_BLOCK_LEN];

	/* extrinsic information required for soft output */
	fixed soft_out_ext[MAX_PSISOS][LTE_MAX_BLOCK_LEN];

	/* soft output information */
	fixed soft_out[MAX_PSISOS][LTE_MAX_BLOCK_LEN];

} t_trace_data;

extern t_trace_data traceData;


void turboDebugAlpha(fixed* alpha,          /* In */
					 int psiso,             /* In */
					 int window,            /* In */
					 int windowStep,        /* In */
					 int numberOfPSISOs,    /* In */
					 int numberOfWindows,   /* In */
					 int windowLength,      /* In */
					 int debugLevel         /* In */
					 );

void turboDebugBeta(fixed* beta,            /* In */
					 int psiso,             /* In */
					 int window,            /* In */
					 int windowStep,        /* In */
					 int numberOfPSISOs,    /* In */
					 int numberOfWindows,   /* In */
					 int windowLength,      /* In */
					 int debugLevel         /* In */
					 );

void turboDebugSisoIn(fixed sys_in,               /* In */
					  fixed par_in,               /* In */
					  fixed ext_in,               /* In */
				      int psiso,                  /* In */
					  int iCyclePerHalfIteration, /* In */
				      int halfIteration,          /* In */
					  int debugLevel              /* In */
					  );

void turboDebugExt(fixed ext_out,               /* In */
				   int psiso,                   /* In */
				   int iCyclePerHalfIteration,  /* In */
				   int halfIteration,           /* In */
				   int debugLevel               /* In */
				   );

void turboDebugSoftout(fixed softOut,               /* In */
				       int psiso,                   /* In */
				       int iCyclePerHalfIteration,  /* In */
				       int halfIteration,           /* In */
				       int debugLevel               /* In */
				      );

void turboDebugSoftoutIn(fixed softOutSys,            /* In */
						 fixed softOutExt,            /* In */
				         int psiso,                   /* In */
				         int iCyclePerHalfIteration,  /* In */
				         int halfIteration,           /* In */
				         int debugLevel               /* In */
				        );

void turboDebugStakes(fixed trellisTermination[8],         /* In */
					  t_psiso_unit psisoUnits[MAX_PSISOS], /* In */
					  int numberOfPSISOs,                  /* In */
					  int numberOfWindows,                 /* In */
				      int halfIteration,                   /* In */
					  int debugLevel                       /* In */
					  );

void turboTraceInit(int debugLevel,             /* In */
					int numberOfPSISOs,         /* In */
					int numberOfWindows         /* In */
					);

void turboTraceClose(int debugLevel,            /* In */
					 int numberOfPSISOs,        /* In */
					 int numberOfWindows        /* In */
					 );

void turboDebugTrace(int debugLevel,             /* In */
					 int numberOfPSISOs,         /* In */
					 int numberOfWindows,        /* In */
					 int psisoLength             /* In */
					 );

void turboDebugAddr(int debugLevel,        /* In */
					int blockLengthIdx,    /* In */
					int numCodeBlockBanks, /* In */
					int numberOfPSISOs,    /* In */
					int halfIteration      /* In */
				   );

#endif /* TURBO_DEBUG_H */

