#pragma once
#include "fifo.h"


	



	int deframe(int cellsearch_end, cfloat **inbuf, int sync_pos, int num_uerx_ant, int fft_size
						  , int idx_frm_pbch, cfloat **rxofdm_time_list, int *subfrm_idx, int *frm_idx);
