#include "foo.h"
#include "bstx_ofdm.h"

#define DUMP_OFDM 1

extern int ifft_InputOutput3(cfloat* in, int size, cfloat* out);

int ofdm_InputOutput(cfloat **inbuf, int num_rb_dl, int fft_size, int num_bstx_ant, int cp_cfg, cfloat **outbuf) // 20141128 linjiangnan
{
	int p, j, k, m;
	int idx;
// 	int fft_size; // 20141128 linjiangnan
	int len_cp;
	int len_long_normal_cp;
	int len_short_normal_cp;
    int len_extend_cp;
	int num_data_subcarrier;
	int num_symb_slot;

	cfloat ifft_inbuf[2048];
	cfloat ifft_outbuf[2048];

	// function body
	num_data_subcarrier = num_rb_dl * 12; // 20141128 linjiangnan

	if(fft_size == 2048) // 20141128 linjiangnan
	{
		len_long_normal_cp	= 160;
		len_short_normal_cp = 144;
		len_extend_cp		= 512;
	}
	else if(fft_size == 1536) // 20141128 linjiangnan
	{
		len_long_normal_cp	= 120;
		len_short_normal_cp = 108;
		len_extend_cp		= 384;
	}
	else if(fft_size == 1024) // 20141128 linjiangnan
	{
		len_long_normal_cp	= 80;
		len_short_normal_cp = 72;
		len_extend_cp		= 256;
	}
	else if(fft_size == 512) // 20141128 linjiangnan
	{
		len_long_normal_cp	= 40;
		len_short_normal_cp = 36;
		len_extend_cp		= 128;
	}
	else if(fft_size == 256) // 20141128 linjiangnan
	{
		len_long_normal_cp	= 20;
		len_short_normal_cp = 18;
		len_extend_cp		= 64;
	}
	else if(fft_size == 128) // 20141128 linjiangnan
	{
		len_long_normal_cp	= 10;
		len_short_normal_cp = 9;
		len_extend_cp		= 32;
	}
	else
	{
		assert(0);
	}

	if(cp_cfg == BSTX_NORMAL_CP)
		num_symb_slot = 7;
	else
		num_symb_slot = 6;

 	m = fft_size - num_data_subcarrier/2;

	for (p=0; p<num_bstx_ant; p++) // generate ofdm symbol every antenna port
	{
		idx = 0;

		for(j=0; j<2*num_symb_slot; j++) // each ofdm symbol
		{
			memset(ifft_inbuf, 0, sizeof(cfloat)*2048);

			for(k=0; k<num_data_subcarrier/2; k++)
			{
				ifft_inbuf[k+1] = inbuf[p][j*1320 + k+num_data_subcarrier/2]; // start after dc
				ifft_inbuf[k+m] = inbuf[p][j*1320 + k];
			}

//			int ifft_InputOutput3(cfloat* in, int size, cfloat* out);//2015-11-5    xiugai

			ifft_InputOutput3(ifft_inbuf, fft_size, ifft_outbuf); // IFFT



			if (cp_cfg == 0)
			{
				len_cp = (j%num_symb_slot == 0) ? len_long_normal_cp : len_short_normal_cp;
			}
			else
			{
				len_cp = len_extend_cp;
			}

			// add cp into ofdm
			for(k=0; k<len_cp; k++)
			{
				outbuf[p][idx++] = ifft_outbuf[k+fft_size-len_cp];
			}

			for(k=0; k<fft_size; k++)
			{
				outbuf[p][idx++] = ifft_outbuf[k];
			}
		}
	}

	return idx;
}

