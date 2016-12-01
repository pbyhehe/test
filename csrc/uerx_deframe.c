#include "foo.h"
#include "uerx_deframe.h"

const int DELAY_LOWPASS = 64;	// The delay for lowpass filter


int deframe(int cellsearch_end, cfloat **inbuf, int sync_pos, int num_uerx_ant, int fft_size
						  , int idx_frm_pbch, cfloat **rxofdm_time_list, int *subfrm_idx, int *frm_idx)
{
	int i, j, p;
	int ret;

	#pragma SET_DATA_SECTION("uerx_data")
	static int idx_subfrm_stroe;
	static int idx_frm_stroe;
	static int subfram_start;
	static int len_subfram;
	static int L_CP, S_CP;
	static int defrm_state = 0;
	static cfloat pingpang_buf[2][61440];
	#pragma SET_DATA_SECTION()

	// cell search end, and then init the deframe module
	ret = 0;

	if(defrm_state == 0 && cellsearch_end == 1)
	{
		defrm_state = 1;

		idx_frm_stroe = idx_frm_pbch;
		idx_subfrm_stroe = 1;

		len_subfram = 30720*fft_size/2048;

		subfram_start = len_subfram + sync_pos*fft_size/128 - 6592*fft_size/2048 - DELAY_LOWPASS;

		if(subfram_start > len_subfram)
		{
			subfram_start -= len_subfram;
			idx_subfrm_stroe -= 1;
		}

		L_CP = 160*fft_size / 2048;
		S_CP = 144*fft_size / 2048;		

		// copy data to the head of pingpang_buf
		for(p=0; p<num_uerx_ant; p++)
		{
			for(i=0; i<len_subfram; i++)
			{
				pingpang_buf[p][i] = inbuf[p][i];
			}
		}
	}
	else if(defrm_state == 1)
	{
		idx_subfrm_stroe++;
		if(idx_subfrm_stroe >= 10)
		{
			idx_subfrm_stroe = 0;
			idx_frm_stroe++;
		}

		*subfrm_idx = idx_subfrm_stroe;
		*frm_idx    = idx_frm_stroe;

		// copy data to the tail of pingpang_buf
		for(p=0; p<num_uerx_ant; p++)
		{
			for(i=0; i<len_subfram; i++)
			{
				pingpang_buf[p][i+len_subfram] = inbuf[p][i];
			}
		}
		
		// derame 14 ofdm symbol from 1 subframe
		for(p=0; p<num_uerx_ant; p++)
		{
			for(i=0; i<7; i++)
			{			
				for(j=0; j<fft_size; j++)
				{
					rxofdm_time_list[p][j + (i+0)*fft_size] 
						= pingpang_buf[p][j + 1*L_CP+(i+0)*S_CP+(i+0)*fft_size + subfram_start];
					rxofdm_time_list[p][j + (i+7)*fft_size]
						= pingpang_buf[p][j + 2*L_CP+(i+6)*S_CP+(i+7)*fft_size + subfram_start];
				}
			}
		}

		// copy data to the head of pingpang_buf
		for(p=0; p<num_uerx_ant; p++)
		{
			for(i=0; i<len_subfram; i++)
			{
				pingpang_buf[p][i] = inbuf[p][i];
			}
		}

		ret = 1;
	}

	return ret;
}
