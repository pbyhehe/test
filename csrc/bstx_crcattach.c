#include "foo.h"
#include "bstx_crcattach.h"



int crcattach_pdsch(int16 *inbuf, int inlen, int16 *outbuf)
{ 
	int outlen;

	static int16 generator_CRC24A[25] = {1,1,0,0,0,0,1,1,0,0,1,0,0,1,1,0,0,1,1,1,1,1,0,1,1};

	outlen = crc_attach(inbuf, inlen, generator_CRC24A, 25, outbuf);

	return outlen;
}

int crcattach_pbch(int16 *inbuf, int inlen, int num_bstx_ant, int16 *outbuf)
{ 
	int i;
	int outlen;
	
	static int16 x_ant_1[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	static int16 x_ant_2[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	static int16 x_ant_4[16] = {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1};

	static int16 generator_CRC16[17]  = {1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,1};

	outlen = crc_attach(inbuf, 24, generator_CRC16, 17, outbuf);
	
	if (num_bstx_ant==1)
	{
		for(i=0; i<16; i++)         
		{
			outbuf[inlen+i] = outbuf[inlen+i] ^ x_ant_1[i];
		}
	}
	else if (num_bstx_ant==2)
	{
		for(i=0; i<16; i++)        
		{
			outbuf[inlen+i] = outbuf[inlen+i] ^ x_ant_2[i];
		}
	}
	else if (num_bstx_ant==4)
	{
		for(i=0; i<16; i++)       
		{
			outbuf[inlen+i] = outbuf[inlen+i] ^ x_ant_4[i];
		}
	}
	else
	{
		assert(0);
	}

	return outlen;
}


int crcattach_pdcch(int16 *inbuf, int inlen, int ue_port, int ue_port_select_enable, int num_rnti
									, int16 *outbuf)
{ 
	int i;
	int outlen;
	int16 x_rnti[16];

	static int16 x_as_ueport0[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	static int16 x_as_ueport1[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};

	static int16 generator_CRC16[17]  = {1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,1};	

	// function body
	for(i=0; i<16; i++)
	{
		x_rnti[15-i] = num_rnti % 2;
		num_rnti /= 2;
	}

	outlen = crc_attach(inbuf, inlen, generator_CRC16, 17, outbuf);
	
	if(ue_port_select_enable == 0)		// ue transmit antenna selection is not configured or applicable
	{
		for (i=0; i<16; i++)
		{
			outbuf[inlen+i] = outbuf[inlen+i] ^ x_rnti[i];
		}
	}
	else if(ue_port_select_enable == 1)	// ue transmit antenna selection is configured and applicable
	{
		if(ue_port == 0)
		{
			for (i=0; i<16; i++)
			{
				outbuf[inlen+i] = outbuf[inlen+i] ^ x_rnti[i] ^ x_as_ueport0[i];
			}
		}
		else if(ue_port == 1)
		{
			for (i=0; i<16; i++)
			{
				outbuf[inlen+i] = outbuf[inlen+i] ^ x_rnti[i] ^ x_as_ueport1[i];
			}
		}
	}

	return outlen;
}


// CRC calculation
int crc_core(int16 *inbuf, int inlen, int16 *generator_buf, int generator_len,  int16 *outbuf)
{	
	int i, j;
	int16 remainder[25];

	// function body
	assert(generator_len==9 || generator_len==17 || generator_len==25);

	// Init remainder
	for(i=0; i<generator_len; i++)
	{
		remainder[i] = 0;
	}

	for(i=0; i<inlen+generator_len-1; i++)
	{
		// shift to low index
		for(j=0; j<generator_len-1; j++)
		{
			remainder[j] = remainder[j+1];
		}

		// get next bit from inbuf
		if(i < inlen)
		{
			remainder[generator_len-1] = inbuf[i];
		}
		else
		{
			remainder[generator_len-1] = 0;
		}
		
		// sub by mod2
		if(remainder[0] == 1)
		{
			for(j=0; j<generator_len; j++)
			{
				remainder[j] = remainder[j] ^ generator_buf[j];
			}
		}
	}
	
	for(i=0; i<generator_len-1; i++)
	{
        outbuf[i] = remainder[i+1];
	}
	
	return generator_len-1;
}

int crc_attach(int16 *inbuf, int inlen, int16 *generator_buf, int generator_len,  int16 *outbuf)
{
	int i, crc_len;
	int16 crc_buf[24];

	crc_len = crc_core(inbuf, inlen, generator_buf, generator_len, crc_buf);

	// add crc
	for(i=0; i<inlen; i++)
	{
		outbuf[i] = inbuf[i];
	}

	for(i=inlen; i<inlen+crc_len; i++)
	{
		outbuf[i] = crc_buf[i-inlen];
	}

	return (inlen+crc_len);
}

