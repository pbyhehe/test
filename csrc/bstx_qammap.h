#pragma once



	int qammodu_core(int16 *inbuf, int inlen, int qm, cfloat *outbuf);

	int qammap_pdsch(int16 *inbuf, int inlen, int qm, cfloat *outbuf);

	int qammap_pcfich(int16 *inbuf, int inlen, cfloat *outbuf);

	int qammap_phich(int16 *inbuf, int inlen, cfloat *outbuf);

	int qammap_pdcch(int16 *inbuf, int inlen, cfloat *outbuf);

	int qammap_pbch(int16 *inbuf, int inlen, cfloat *outbuf);






