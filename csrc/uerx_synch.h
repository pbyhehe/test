#pragma once

#include "typedefine.h"
#include "fifo.h"
#include "corr.h"

typedef struct synch1
{
	cfloat	 Lcpss0_frq62[62];
	cfloat	 Lcpss1_frq62[62];
	cfloat	 Lcpss2_frq62[62];

	cfloat	 Lcpss0_frq2048c[2048];
	cfloat	 Lcpss1_frq2048c[2048];
	cfloat	 Lcpss2_frq2048c[2048];
}synch1;

	int GetIDcell(cfloat *rxsss_freq, int N_ID2, int *N_ID_cell);

    int synch_Init(synch1 *uerx_synch_u);

	int synch_Clear();

	int psscorr(synch1 *uerx_synch_u,cfloat *indata, int inlen, int *num_id_cell, int *pos_peak);

	int	rmfreqofst(cfloat **inbuf, int inlen, int num_uerx_ant, float inphase, cfloat **outbuf);

	int afccarse(cfloat indata, float sampling_rate_MHz, float *ptr_coarse_phase);


