#include "uerx_agc.h"

#define AGC_EN 1


int agc_InputOutput(
	int N_ant_bsrx, // in
	int inlen, // in
	cfloat **inbuf, // in
	cfloat **outbuf) // out
{
	int16   ant_idx;
    int32   data_idx;
	
    float   agc_abs_re;
    float   agc_abs_im;
    float   agc_max_tmp;
    float   agc_max;
    float   agc_gain;
	
	
    /* ============= */
    /* RFIU/RFIC AGC */
    /* ============= */
#if AGC_EN == 1
	
    cfloat  out_sc;
	
    /* Max data */
    agc_max = 0.0;
	for (ant_idx = 0; ant_idx < N_ant_bsrx; ant_idx++)
	{
		for (data_idx = 0; data_idx < inlen; data_idx++)
		{
			// abs
			agc_abs_re = (inbuf[ant_idx][data_idx].re >= 0) ? (inbuf[ant_idx][data_idx].re) : (-inbuf[ant_idx][data_idx].re);
			agc_abs_im = (inbuf[ant_idx][data_idx].im >= 0) ? (inbuf[ant_idx][data_idx].im) : (-inbuf[ant_idx][data_idx].im);
			
			// max
			agc_max_tmp = (agc_abs_re >= agc_abs_im) ? (agc_abs_re) : (agc_abs_im);
			agc_max     = (agc_max_tmp >= agc_max) ? (agc_max_tmp) : (agc_max);
		}
	}
	
	
    /* Gain */
    agc_gain = 0.9 / agc_max; // to <16,1>, avoid DDC & IQC overflow	// 0.9, 0.15
// 	for (ant_idx = 0; ant_idx < N_ant_bsrx; ant_idx++)
// 	{
// 		outgain[ant_idx][0] = agc_gain;
// 	}
	
    /* Gain Control */
	for (ant_idx = 0; ant_idx < N_ant_bsrx; ant_idx++)
	{
		for (data_idx = 0; data_idx < inlen; data_idx++)
		{
			out_sc.re = inbuf[ant_idx][data_idx].re * agc_gain;
			out_sc.im = inbuf[ant_idx][data_idx].im * agc_gain;
			
			outbuf[ant_idx][data_idx].re = out_sc.re;
			outbuf[ant_idx][data_idx].im = out_sc.im;
		}
	}
	
	
    /* ====== */
    /* Bypass */
    /* ====== */
#elif AGC_EN == 0
	
    /* Gain */
	agc_gain = 1.0;
// 	for (ant_idx = 0; ant_idx < N_ant_bsrx; ant_idx++)
// 	{
// 		outgain[ant_idx][0] = agc_gain;
// 	}
	
    /* output */
	for (ant_idx = 0; ant_idx < N_ant_bsrx; ant_idx++)
	{
		for (data_idx = 0; data_idx < inlen; data_idx++)
		{
			outbuf[ant_idx][data_idx].re = inbuf[ant_idx][data_idx].re;
			outbuf[ant_idx][data_idx].im = inbuf[ant_idx][data_idx].im;
		}
	}
	
#endif
	
	
    return inlen;
}



int agc(cfloat **inbuf, int inlen, int num_uerx_ant, cfloat **outbuf)
{
	int p, i;
	float gain_agc = 256;

	for(p=0; p<num_uerx_ant; p++)
	{
		for(i=0; i<inlen; i++)
		{
			outbuf[p][i] = cfloatmultiplyfloat(inbuf[p][i],gain_agc);
		}
	}

	return inlen;
}


int fix_gain_ctrl(cfloat **inbuf, int inlen, int num_uerx_ant, int ofdm_idx, cfloat **outbuf)
{
	int p, i;
	float max_abs_indata;
	float abs_re, abs_im;

	#pragma SET_DATA_SECTION("uerx_data")
	static float gain_fgc[2];
	#pragma SET_DATA_SECTION()

	if(ofdm_idx == 0) // update the gain every subframe
	{
		for(p=0; p<num_uerx_ant; p++)
		{
			max_abs_indata = 0;

			for(i=0; i<inlen; i++)
			{
				abs_re = abs(inbuf[p][i].re);
				abs_im = abs(inbuf[p][i].im);

				if(abs_re > max_abs_indata)		max_abs_indata = abs_re;
				
				if(abs_im > max_abs_indata)		max_abs_indata = abs_im;
			}
			
			// outbuf<16, 11>, 1 bit for sign, 1 bit for reserver, 2^9 = 512
			gain_fgc[p] = 512 / max_abs_indata;
		}		
	}

	for(p=0; p<num_uerx_ant; p++)
	{
		for(i=0; i<inlen; i++)
		{
			outbuf[p][i] = cfloatmultiplyfloat(inbuf[p][i],gain_fgc[p]);
		}
	}	

	return inlen;
}

