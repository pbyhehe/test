#pragma once



	int crc_check(int16 *inbuf, int inlen, int16 *generator_buf, int generator_len);

    int decrcattach_pdsch(int16 *inbuf, int inlen, int16 *outbuf, int *crc_error);

	int decrcattach_pbch(int16 *inbuf, int inlen, int num_bstx_ant, int16 *outbuf, int *crc_error);

	int decrcattach_pdcch(int16 *inbuf, int inlen, int ue_port, int ue_port_select_enable
										, int16 *x_rnti, int16 *outbuf, int *crc_error);
