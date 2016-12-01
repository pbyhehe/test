#include "foo.h"
#include "uerx_synch.h"
#include "uerx_physig.h"

#define PSSCORR_TH		80000

extern float sampling_rate_MHz;

extern int fft_InputOutput3(cfloat *Iin, int size, cfloat *out);
extern int ifft_InputOutput3(cfloat* in, int size, cfloat* out);

int synch_Init(synch1 *uerx_synch_u)
{
	int i;

#pragma SET_DATA_SECTION("uerx_data")
	static cfloat buf0[128];
	static cfloat buf1[128];
	static cfloat buf2[128];
	static cfloat lcpss0_time[2048];
	static cfloat lcpss1_time[2048];
	static cfloat lcpss2_time[2048];
	static cfloat lcpss0_frq2048[2048];
	static cfloat lcpss1_frq2048[2048];
	static cfloat lcpss2_frq2048[2048];
#pragma SET_DATA_SECTION()

//#pragma SET_DATA_SECTION("uerx_data")
//	cfloat buf0[128];
//	cfloat buf1[128];
//	cfloat buf2[128];
//	cfloat lcpss0_time[2048];
//	cfloat lcpss1_time[2048];
//	cfloat lcpss2_time[2048];
//	cfloat lcpss0_frq2048[2048];
//	cfloat lcpss1_frq2048[2048];
//	cfloat lcpss2_frq2048[2048];
//#pragma SET_DATA_SECTION()
	
//	uerx_physig pss_u;
	
	// local 62 frequency carrier PSS
	physig_InputOutput1(0, (*uerx_synch_u).Lcpss0_frq62);
	physig_InputOutput1(1, (*uerx_synch_u).Lcpss1_frq62);
	physig_InputOutput1(2, (*uerx_synch_u).Lcpss2_frq62);

	memset(buf0, 0, sizeof(cfloat) * 128);
	memset(buf1, 0, sizeof(cfloat) * 128);
	memset(buf2, 0, sizeof(cfloat) * 128);

	memset(lcpss0_time, 0, sizeof(cfloat) * 2048);
	memset(lcpss1_time, 0, sizeof(cfloat) * 2048);
	memset(lcpss2_time, 0, sizeof(cfloat) * 2048);

	// resource mapping
	for(i=0; i<31; i++)
	{	// localpss_f0: n1-31,...,n1-2,n1-1, 1, 2 ...,31			
		// buf0:        0    ,...,29,  30    31,32...,61
		buf0[i+128-31] = (*uerx_synch_u).Lcpss0_frq62[i];
		buf0[i+1]	   = (*uerx_synch_u).Lcpss0_frq62[i+31];
		
		buf1[i+128-31] = (*uerx_synch_u).Lcpss1_frq62[i];
		buf1[i+1]	   = (*uerx_synch_u).Lcpss1_frq62[i+31];
		
		buf2[i+128-31] = (*uerx_synch_u).Lcpss2_frq62[i];
		buf2[i+1]	   = (*uerx_synch_u).Lcpss2_frq62[i+31];
	}	
	// local time pss

	ifft_InputOutput3(buf0, 128, lcpss0_time);
	ifft_InputOutput3(buf1, 128, lcpss1_time);
	ifft_InputOutput3(buf2, 128, lcpss2_time);

	// local freq2048 pss
	fft_InputOutput3(lcpss0_time, 2048, lcpss0_frq2048);
	fft_InputOutput3(lcpss1_time, 2048, lcpss1_frq2048);
	fft_InputOutput3(lcpss2_time, 2048, lcpss2_frq2048);
	
	// local conj freq2048 pss
	for(i=0; i<2048; i++)
	{
		(*uerx_synch_u).Lcpss0_frq2048c[i] = conj1(lcpss0_frq2048[i]);
		(*uerx_synch_u).Lcpss1_frq2048c[i] = conj1(lcpss1_frq2048[i]);
		(*uerx_synch_u).Lcpss2_frq2048c[i] = conj1(lcpss2_frq2048[i]);
	}

	return 0;
}

int synch_Clear()
{
	return 0;
}


int rmfreqofst(cfloat **inbuf, int inlen, int num_uerx_ant, float inphase, cfloat **outbuf)
{
	int i, p;
	cfloat factor;
	#pragma DATA_SECTION("uerx_data")
    static float acc_phase = 0.0;	

	for (i=0; i<inlen; i++)
	{
		acc_phase += inphase;
	
		if(acc_phase > 2*PI)	
			acc_phase -= 2*PI;
		else if(acc_phase < -2*PI)	
			acc_phase += 2*PI;

		factor.re =  cos(acc_phase);
		factor.im = -sin(acc_phase);

		for(p=0; p<num_uerx_ant; p++)
		{
			outbuf[p][i].re = inbuf[p][i].re*factor.re - inbuf[p][i].im*factor.im;
			outbuf[p][i].im = inbuf[p][i].re*factor.im + inbuf[p][i].im*factor.re;
		}
	}
	
	return inlen;
}



int afccarse(cfloat indata, float sampling_rate_MHz, float *ptr_coarse_phase)
{
	int i;
	int is_peak;
	int num_div;
	int afc_end;

	#pragma SET_DATA_SECTION("uerx_data")
	static int idx0;
	static int idx1;
	static int peak_count;

#if LTEDL_FIXD_FLG == 0 || (LTEDL_FIXD_FLG == 8  && LTEDL_SYNCH_AFCCARSE == 0)
	float		  normal_corr_countdown_8th_sc;
	cfloat		  corr_countdown_8th_sc;
	cfloat        data0_out_sc, data1_in_sc, data2_out_sc, data3_in_sc;
	static float  pow0_sc, pow1_sc;
	static float  fifo_normal_corr_sc[16];
	static cfloat corr_sc, corr_acc_sc, fifo_corr_sc[16];
	static cfloat fifo_buf_sc[2200];
#elif LTEDL_FIXD_FLG == 3 || (LTEDL_FIXD_FLG == 8  && LTEDL_SYNCH_AFCCARSE == 3)
	sc_fixed<16, 1, SC_RND, SC_SAT>		 	 normal_corr_countdown_8th_sc;
	sc_cfixed<16, 26, SC_RND, SC_SAT> 		 corr_countdown_8th_sc;
	sc_cfixed<16, 11, SC_RND, SC_SAT>        data0_out_sc, data1_in_sc, data2_out_sc, data3_in_sc;
	static sc_fixed<16, 26, SC_RND, SC_SAT>  pow0_sc, pow1_sc;
	static sc_fixed<16,	1, SC_RND, SC_SAT>   fifo_normal_corr_sc[16];
	static sc_cfixed<16, 26, SC_RND, SC_SAT> corr_sc, corr_acc_sc, fifo_corr_sc[16];
	static sc_cfixed<16, 11, SC_RND, SC_SAT> fifo_buf_sc[2200];
#endif	                      
	#pragma SET_DATA_SECTION()
	
	// function body
	afc_end = 0;

	if((int)(sampling_rate_MHz) == 30)		num_div = 1;
	else if((int)(sampling_rate_MHz) == 15)	num_div = 2;
	else if((int)(sampling_rate_MHz) == 7)	num_div = 4;
	else if((int)(sampling_rate_MHz) == 3)	num_div = 8;
	else if((int)(sampling_rate_MHz) == 1)	num_div = 16;
	else									assert(0);

	fifo_buf_sc[idx0].re = indata.re;
	fifo_buf_sc[idx0].im = indata.im;

	data3_in_sc.re = indata.re;
	data3_in_sc.im = indata.im;

	if (idx0-144/num_div >= 0)
	{
		data2_out_sc.re = fifo_buf_sc[idx0-144/num_div].re;
		data2_out_sc.im = fifo_buf_sc[idx0-144/num_div].im;
	}
	else
	{
		data2_out_sc.re = fifo_buf_sc[idx0-144/num_div+2200].re;
		data2_out_sc.im = fifo_buf_sc[idx0-144/num_div+2200].im;
	}

	if (idx0-2048/num_div >= 0)
	{
		data1_in_sc.re = fifo_buf_sc[idx0-2048/num_div].re;
		data1_in_sc.im = fifo_buf_sc[idx0-2048/num_div].im;
	}
	else
	{
		data1_in_sc.re = fifo_buf_sc[idx0-2048/num_div+2200].re;
		data1_in_sc.im = fifo_buf_sc[idx0-2048/num_div+2200].im;
	}

	if (idx0-(2048+144)/num_div >= 0)
	{
		data0_out_sc.re = fifo_buf_sc[idx0-(2048+144)/num_div].re;
		data0_out_sc.im = fifo_buf_sc[idx0-(2048+144)/num_div].im;
	}
	else
	{
		data0_out_sc.re = fifo_buf_sc[idx0-(2048+144)/num_div+2200].re;
		data0_out_sc.im = fifo_buf_sc[idx0-(2048+144)/num_div+2200].im;
	}

	corr_sc.re  +=  data3_in_sc.re*data1_in_sc.re+data3_in_sc.im*data1_in_sc.im
		- (data2_out_sc.re*data0_out_sc.re+data2_out_sc.im*data0_out_sc.im);
	corr_sc.im  +=  data3_in_sc.re*(-data1_in_sc.im) + data3_in_sc.im*data1_in_sc.re
		- (data2_out_sc.re*(-data0_out_sc.im) + data2_out_sc.im*data0_out_sc.re);
	
	pow0_sc +=  (data3_in_sc.re*data3_in_sc.re+data3_in_sc.im*data3_in_sc.im) 
		- (data2_out_sc.re*data2_out_sc.re+data2_out_sc.im*data2_out_sc.im);
	
	pow1_sc +=  (data1_in_sc.re*data1_in_sc.re+data1_in_sc.im*data1_in_sc.im)
		- (data0_out_sc.re*data0_out_sc.re+data0_out_sc.im*data0_out_sc.im);

	// save the corr and normal_pow_corr
	fifo_corr_sc[idx1] = corr_sc;

	if(pow0_sc*pow1_sc > 1e12/num_div)
		fifo_normal_corr_sc[idx1] = (corr_sc.re*corr_sc.re+corr_sc.im*corr_sc.im) / (pow0_sc*pow1_sc);
	else
		fifo_normal_corr_sc[idx1] = 0;

	// get the countdown 8th corr and normal_pow_corr
	corr_countdown_8th_sc = fifo_corr_sc[(idx1-8)&0x0000000f];

	normal_corr_countdown_8th_sc = fifo_normal_corr_sc[(idx1-8)&0x0000000f];

	// whether is peak
	is_peak = 1;               
	for (i=0; i<16; i++)
	{
		is_peak &= normal_corr_countdown_8th_sc >= fifo_normal_corr_sc[(idx1-i)&0x0000000f];
	}

	if(normal_corr_countdown_8th_sc>0.16 && is_peak)
	{
		peak_count++;

		cfloataddequalcfloat(&corr_acc_sc,corr_countdown_8th_sc);
	}

	if(peak_count >= 14)
	{
		*ptr_coarse_phase = (atan2(corr_acc_sc.im, corr_acc_sc.re) / (2048/num_div));

		// clear the static type number for next afccoarse
		peak_count = 0;
		
		idx0 = 0;
		idx1 = 0;
		pow0_sc = 0;
		pow1_sc = 0;

		corr_sc.re = 0;
		corr_sc.im = 0;

		corr_acc_sc.re = 0;
		corr_acc_sc.im = 0;

		for(i=0; i<16; i++)
		{
			fifo_normal_corr_sc[i] = 0;

			fifo_corr_sc[i].re = 0;
			fifo_corr_sc[i].im = 0;
		}

		for(i=0; i<2200; i++)
		{
			fifo_buf_sc[i].re = 0;
			fifo_buf_sc[i].im = 0;
		}

		afc_end = 1;

	}

	idx0++;	
	idx1++;

	if(idx0 >= 2200)
	{
		idx0 = 0;
	}

	if(idx1 >= 16)
	{
		idx1 = 0;
	}

	return afc_end;
}



int psscorr(synch1 *uerx_synch_u,cfloat *indata, int inlen, int *num_id_cell, int *pos_peak)
{
	int i;
	int N_ID2;
	int is_peak;
	int sync_end;

	#pragma SET_DATA_SECTION("uerx_data")
	static cfloat indata_2subfrm[2464];	// 2464 = 544+1920, 544 = ceil(540/16)*16
	static cfloat indata_freq[2048];
	static cfloat xcorr0_freq[2048], xcorr1_freq[2048], xcorr2_freq[2048];
	static cfloat xcorr0_time[2048], xcorr1_time[2048], xcorr2_time[2048];
	static cfloat rxpss_time[128];
	static cfloat rxsss_time[128];
	static cfloat rxpss_freq[128];
	static cfloat rxsss_freq[128];

	static float xcorr0_pow_time[2048];
	static float xcorr1_pow_time[2048];
	static float xcorr2_pow_time[2048];
	#pragma SET_DATA_SECTION()

	cfloat *lcpss_frq62;

	cfloat rxpss_frq62[62];
	cfloat rxsss_frq62[62];
	cfloat rxsss_fest62[62];
	cfloat H_pss[62];
	
	// function body
	assert(inlen == 1920);

	is_peak = 0;
	sync_end = 0;

	// init indata_2subfrm
	for(i=0; i<1920; i++)
	{
		indata_2subfrm[i+544] = indata[i];
	}

// 	save.Run(indata_2subfrm, 2464, "indata_2subfrm_in", TO_SAVE);

	// fft
	fft_InputOutput3(&indata_2subfrm[416], 2048, indata_freq);

// 	save.Run(indata_freq, 2048, "indata_freq_cs", TO_SAVE);
	
	// multiplex in frequency
	for(i=0; i<2048; i++)
	{
		xcorr0_freq[i] = cfloatmultiplycfloat((*uerx_synch_u).Lcpss0_frq2048c[i],indata_freq[i]);
		xcorr1_freq[i] = cfloatmultiplycfloat((*uerx_synch_u).Lcpss1_frq2048c[i],indata_freq[i]);
		xcorr2_freq[i] = cfloatmultiplycfloat((*uerx_synch_u).Lcpss2_frq2048c[i],indata_freq[i]);
	}

//	save.Run(xcorr0_freq, 2048, "xcorr0_freq_cs", TO_SAVE);
//	save.Run(xcorr1_freq, 2048, "xcorr1_freq_cs", TO_SAVE);
// 	save.Run(xcorr2_freq, 2048, "xcorr2_freq_cs", TO_SAVE);

	// ifft
	ifft_InputOutput3(xcorr0_freq, 2048, xcorr0_time);
	ifft_InputOutput3(xcorr1_freq, 2048, xcorr1_time);
	ifft_InputOutput3(xcorr2_freq, 2048, xcorr2_time);

//	save.Run(xcorr0_time, 2048, "xcorr0_time_cs", TO_SAVE);
//	save.Run(xcorr1_time, 2048, "xcorr1_time_cs", TO_SAVE);
//	save.Run(xcorr2_time, 2048, "xcorr2_time_cs", TO_SAVE);

	for(i=0; i<1922; i++)
	{
		xcorr0_pow_time[i] = xcorr0_time[i].re*xcorr0_time[i].re + xcorr0_time[i].im*xcorr0_time[i].im;
		xcorr1_pow_time[i] = xcorr1_time[i].re*xcorr1_time[i].re + xcorr1_time[i].im*xcorr1_time[i].im;
		xcorr2_pow_time[i] = xcorr2_time[i].re*xcorr2_time[i].re + xcorr2_time[i].im*xcorr2_time[i].im;
	}

// 	save.Run(&xcorr0_pow_time[1], 1920, "xcorr0_pow_time_cs", TO_SAVE);
// 	save.Run(&xcorr1_pow_time[1], 1920, "xcorr1_pow_time_cs", TO_SAVE);
// 	save.Run(&xcorr2_pow_time[1], 1920, "xcorr2_pow_time_cs", TO_SAVE);

	// search pss corr
	for(i=1; i<1921; i++)
	{
		if(xcorr0_pow_time[i] > PSSCORR_TH && xcorr0_pow_time[i] >= xcorr0_pow_time[i-1] && xcorr0_pow_time[i] >= xcorr0_pow_time[i+1])
		{
			N_ID2		= 0;
			is_peak		= 1;
			*pos_peak	= i-1;
			lcpss_frq62 = (*uerx_synch_u).Lcpss0_frq62;	// local PSS 62 carrier sequence, used as reference signal
		}
		else if(xcorr1_pow_time[i] > PSSCORR_TH && xcorr1_pow_time[i] >= xcorr1_pow_time[i-1] && xcorr1_pow_time[i] >= xcorr1_pow_time[i+1])
		{
			N_ID2		= 1;
			is_peak		= 1;
			*pos_peak	= i-1;
			lcpss_frq62 = (*uerx_synch_u).Lcpss1_frq62;	// local PSS 62 carrier sequence, used as reference signal
		}
		else if(xcorr2_pow_time[i] > PSSCORR_TH && xcorr2_pow_time[i] >= xcorr2_pow_time[i-1] && xcorr2_pow_time[i] >= xcorr2_pow_time[i+1])
		{
			N_ID2		= 2;
			is_peak		= 1;
			*pos_peak	= i-1;
			lcpss_frq62 = (*uerx_synch_u).Lcpss2_frq62;	// local PSS 62 carrier sequence, used as reference signal
		}
	}

	if(is_peak == 1)
	{
		// SSS                        PSS 
		// <0,13>   <1,0>    <1,1>    <1,2>    
		// 2048+160+2048+144+2048+144+2048=8640	
		// PSS = 8640/16 = 540;	

		// get PSS and SSS		
		for(i=0; i<128; i++)
		{
			rxpss_time[i] = indata_2subfrm[i+416+*pos_peak];	// 416 = 544-128
			rxsss_time[i] = indata_2subfrm[i+4+*pos_peak];	// 4   = 544-540
		}

		// use PSS to channel estimation
		fft_InputOutput3(rxpss_time, 128, rxpss_freq);
		fft_InputOutput3(rxsss_time, 128, rxsss_freq);

// 		save.Run(&*pos_peak, 1, "*pos_peak", TO_SAVE);
// 		save.Run(rxpss_time, 128, "rxpss_time", TO_SAVE);
// 		save.Run(rxsss_time, 128, "rxsss_time", TO_SAVE);
// 		save.Run(rxpss_freq, 128, "rxpss_freq", TO_SAVE);
// 		save.Run(rxsss_freq, 128, "rxsss_freq", TO_SAVE);

		// Demap for resource mapping
		for(i = 0; i<31; i++)        
		{
			rxpss_frq62[i].re	 = rxpss_freq[i+128-31].re;
			rxpss_frq62[i].im	 = rxpss_freq[i+128-31].im;
			rxpss_frq62[i+31].re = rxpss_freq[i+1].re;
			rxpss_frq62[i+31].im = rxpss_freq[i+1].im;
			
			rxsss_frq62[i].re    = rxsss_freq[i+128-31].re;	
			rxsss_frq62[i].im    = rxsss_freq[i+128-31].im;			
			rxsss_frq62[i+31].re = rxsss_freq[i+1].re;
			rxsss_frq62[i+31].im = rxsss_freq[i+1].im;
		}

// 		save.Run(rxpss_frq62, 62, "rxpss_frq62", TO_SAVE);
// 		save.Run(rxsss_frq62, 62, "rxsss_frq62", TO_SAVE);

		// SSS sequence channel estimation and equalization
		for (i=0; i<62; i++)
		{	// rxpss_frq62 = conj(rxpss_frq62)
			rxpss_frq62[i].re = rxpss_frq62[i].re;
			rxpss_frq62[i].im = -rxpss_frq62[i].im;

			// H_pss = rxpss_frq62 * lcpss_frq62
			H_pss[i].re = rxpss_frq62[i].re*lcpss_frq62[i].re - rxpss_frq62[i].im*lcpss_frq62[i].im;
			H_pss[i].im = rxpss_frq62[i].re*lcpss_frq62[i].im + rxpss_frq62[i].im*lcpss_frq62[i].re;
			
			// rxsss_fest62 = H_pss * rxsss_frq62
			rxsss_fest62[i].re = H_pss[i].re*rxsss_frq62[i].re - H_pss[i].im*rxsss_frq62[i].im;
			rxsss_fest62[i].im = H_pss[i].im*rxsss_frq62[i].re + H_pss[i].re*rxsss_frq62[i].im;
		}

		sync_end = GetIDcell(rxsss_fest62, N_ID2, num_id_cell);


	} // end if(is_peak == 1)

	// save 544 data in the end of indata
	for(i=0; i<544; i++)
	{
		indata_2subfrm[i] = indata[i+1376];
	}

// 	save.Run(indata_2subfrm, 2464, "indata_2subfrm_out", TO_SAVE);

	return sync_end;
}



int GetIDcell(cfloat *rxsss_freq, int N_ID2, int *N_ID_cell)
{
	int i, j;
	int d[62];
	int sum, max_sum, idx_max_sum; 
    int m0, m1;
	int	c0[31], c1[31];
	int s0m0[31], s1m1[31];
	int z1m0[31];
	int N_ID1;
	
	int cc[31] = {1,1,1,1,-1,1,-1,1,-1,-1,-1,1,-1,-1,1,1,1,-1,-1,-1,-1,-1,1,1,-1,-1,1,-1,1,1,-1};
	int ss[31] = {1,1,1,1,-1,1,1,-1,1,-1,-1,1,1,-1,-1,-1,-1,-1,1,1,1,-1,-1,1,-1,-1,-1,1,-1,1,-1};
	int zz[31] = {1,1,1,1,-1,-1,-1,1,1,-1,-1,1,-1,-1,-1,-1,-1,1,-1,1,1,1,-1,1,1,-1,1,-1,1,-1,-1};
	
	for (i = 0; i < 62; i++)        
	{
		if (rxsss_freq[i].re > 0)		d[i] =  1;
		else			                d[i] = -1;
	}

	//c0[31],c1[31]
	for(i = 0; i < 31; i++)
	{
		c0[i] = cc[(i + N_ID2)%31];
		c1[i] = cc[(i + N_ID2+3)%31];
	}

	// s0m0[31]
	for (i = 0; i < 31; i++)                                         
	{
		s0m0[i] = d[2*i] * c0[i];
	}

	// m0
	max_sum     = 0;                                                 
	idx_max_sum = 0;
	for (i = 0; i < 31; i++)
	{
		sum = 0;

		for (j = 0; j < 31; j++)
		{
			sum += s0m0[j] * ss[(i+j)%31];
		}

		if (sum > max_sum)
		{
			max_sum     = sum;
			idx_max_sum = i;
		}
	}

	if (max_sum < 16)	return 0;

	m0 = idx_max_sum;
	
	for (i = 0; i < 31; i++)
	{
		z1m0[i] = zz[(i + (m0%8))%31];
	}
	
	// s1m1[31]
	for (i = 0; i < 31; i++)                                         
	{
		s1m1[i] = d[2*i+1] * c1[i] * z1m0[i];
	}

	// m1
	max_sum     = 0;                                                 
	idx_max_sum = 0;
	
	for (i = 0; i < 31; i++)
	{
		sum = 0;
		
		for (j = 0; j < 31; j++)
		{
			sum += s1m1[j] * ss[(i+j)%31];
		}
		
		if (sum > max_sum)
		{
			max_sum     = sum;
			idx_max_sum = i;
		}
	}


	if (max_sum < 16)	return 0;

	m1 = idx_max_sum;

	// get N_ID1
	N_ID1 = (62+m0-m1)*(m1-m0-1)/2 + m0;

	if (N_ID1>=0 && N_ID1 < 168)		
	{
		*N_ID_cell = N_ID1*3 + N_ID2;

		return 1;
	}

	return 0;
}
