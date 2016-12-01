#pragma once

#include "bstx_define.h"
#include "bstx_crcattach.h"




	int blockseg_core(int16 *inbuf, int inlen, int16 **outbuf, int *outlen_seg, int *num_seg, int *num_nullbit);



	int blockseg_pdsch(int16 *inbuf, int inlen, int16 **outbuf, int *outlen_seg, int *num_seg, int *num_nullbit);

