#pragma once

#include "foo.h"


	


	int agc_InputOutput(
		int N_ant_bsrx, // in
		int inlen, // in
		cfloat **inbuf, // in
		cfloat **outbuf); // out

	int agc(cfloat **inbuf, int inlen, int num_uerx_ant, cfloat **outbuf);

	int fix_gain_ctrl(cfloat **inbuf, int inlen, int num_uerx_ant, int ofdm_idx, cfloat **outbuf);

