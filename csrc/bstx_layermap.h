#pragma once

#include "foo.h"



	int layer_siso(cfloat *inbuf, int inlen, cfloat *outbuf);

	int layer_diversity(cfloat *inbuf, int inlen, int num_layers, cfloat **outbuf);

	int layer_spatial(cfloat **inbuf, int *inlen_cw, int num_layers, int num_codewords, cfloat **outbuf);



	int layer_pcfich(cfloat *inbuf, int inlen, int num_layers, cfloat **outbuf);

	int layer_phich(cfloat *inbuf, int inlen, int num_layers, cfloat **outbuf);

	int layer_pdcch(cfloat *inbuf, int inlen, int num_layers, cfloat **outbuf);

	int layer_pbch(cfloat *inbuf, int inlen, int num_layers, cfloat **outbuf);

	int layer_pdsch(cfloat **inbuf, int *inlen_cw, int transmit_scheme, int num_layers, int num_codewords
		, cfloat **outbuf);


