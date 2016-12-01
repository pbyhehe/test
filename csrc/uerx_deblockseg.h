#pragma once



	int deblockseg_crc_check(int16 *inbuf, int inlen, int16 *generator_buf, int generator_len);



	int deblockseg(int16 **inbuf, int *size_each_codeseg, int num_codeseg, int num_nullbit
								, int *crc_error, int16 *outbuf);

