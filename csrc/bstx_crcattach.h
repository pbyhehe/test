#pragma once

#include "bstx_define.h"


	int crc_core(int16 *inbuf, int inlen, int16 *generator_buf, int generator_len,  int16 *outbuf);
	


	int crc_attach(int16 *inbuf, int inlen, int16 *generator_buf, int generator_len,  int16 *outbuf);	
	
	int crcattach_pdsch(int16 *inbuf, int inlen, int16 *outbuf);

	int crcattach_pbch(int16 *inbuf, int inlen, int num_bstx_ant, int16 *outbuf);

	int crcattach_pdcch(int16 *inbuf, int inlen, int ue_port, int ue_port_select_enable, int num_rnti, int16 *outbuf);

