#include "foo.h"
#include "uerx_define.h"
#include "uerx_decrcattach.h"


int decrcattach_pdsch(int16 *inbuf, int inlen, int16 *outbuf, int *crc_error)
{
    int i;
	int isall_zero;

	int16 generator_buf[25] = {1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1};

	// function body
	isall_zero = 1;

	for (i=0; i<inlen; i++)
	{
		if (inbuf[i] != 0)
		{
			isall_zero = 0;
			break;
		}
	}

	if (isall_zero)
	{
		*crc_error = 1;
	}
	else
	{
		*crc_error = crc_check(inbuf, inlen, generator_buf, 25);
	}

	// remove CRC
	for(i=0; i<inlen-24; i++)
	{
		outbuf[i] = inbuf[i];
	}

	return (inlen-24);
}


int decrcattach_pbch(int16 *inbuf, int inlen, int num_bstx_ant, int16 *outbuf, int *crc_error)
{
    int i, j;
	int isall_zero;
	
	int16 block_buf[40];

	int16 *ptr_x_ant;

	#pragma SET_DATA_SECTION("uerx_data")
	static int16 x_ant_1[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	static int16 x_ant_2[16] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	static int16 x_ant_4[16] = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};

	static int16 generator_buf[17] = {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};	
	#pragma SET_DATA_SECTION()

	// function body
	if (num_bstx_ant==1)
	{
		ptr_x_ant = x_ant_1;
	}
	else if (num_bstx_ant==2)
	{
		ptr_x_ant = x_ant_2;
	}
	else if (num_bstx_ant==4)
	{
		ptr_x_ant = x_ant_4;
	}

	for (i=0, j=0; i<inlen; i++)
	{
		if(i<inlen-16)
			block_buf[i] = inbuf[i];
		else
			block_buf[i] = inbuf[i] ^ ptr_x_ant[j++];
	}

	isall_zero = 1;

	for (i=0; i<inlen; i++)
	{
		if (block_buf[i] != 0)
		{
			isall_zero = 0;
			break;
		}
	}

	if (isall_zero)
	{
		*crc_error = 1;
	}
	else
	{
		*crc_error = crc_check(block_buf, inlen, generator_buf, 17);
	}

	// remove CRC
	for(i=0; i<inlen-16; i++)
	{
		outbuf[i] = block_buf[i];
	}

	return (inlen-16);
}

int decrcattach_pdcch(int16 *inbuf, int inlen, int ue_port, int ue_port_select_enable
										, int16 *x_rnti, int16 *outbuf, int *crc_error)
{
    int i, j;
	int isall_zero;
	
	int16 generator_buf[17] = {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
	
	#pragma SET_DATA_SECTION("uerx_data")
	static int16 block_buf[151400];
	static int16 x_as_ueport0[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	static int16 x_as_ueport1[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
	#pragma SET_DATA_SECTION("uerx_data")
	int16 *ptr_x_as;



    // function body
	if(ue_port == 0)
	{
		ptr_x_as = x_as_ueport0;
	}
	else
	{
		ptr_x_as = x_as_ueport1;
	}
	
	for (i=0, j=0; i<inlen; i++)
	{
		if(i<inlen-16)
		{
			block_buf[i] = inbuf[i];
		}
		else
		{
			if(ue_port_select_enable == 0)
			{
				block_buf[i] = inbuf[i] ^ x_rnti[j++];
			}
			else
			{
				block_buf[i] = inbuf[i] ^ x_rnti[j] ^ ptr_x_as[j++];
			}
		}
	}

	isall_zero = 1;

	for (i=0; i<inlen; i++)
	{
		if (block_buf[i] != 0)
		{
			isall_zero = 0;
			break;
		}
	}

	if (isall_zero)
	{
		*crc_error = 1;
	}
	else
	{
		*crc_error = crc_check(block_buf, inlen, generator_buf, 17);
	}

	for(i=0; i<inlen-16; i++)
	{
		outbuf[i] = block_buf[i];
	}

	return (inlen-16);
}

int crc_check(int16 *inbuf, int inlen, int16 *generator_buf, int generator_len)
{ 	
	int i, j;
	int crc_error;
	int isall_zero;
	
	int16 remainder[25];
	
	// function body
	assert(generator_len==9 || generator_len==17 || generator_len==25);

	isall_zero = 1;

	for (i=0; i<inlen; i++)
	{
		if (inbuf[i] != 0)
		{
			isall_zero = 0;
			break;
		}
	}

	if (isall_zero)
	{
		crc_error = 1;
	}
	else
	{		
		// Init remainder
		for(i=0; i<generator_len; i++)
		{
			remainder[i] = 0;
		}
		
		for(i=0; i<inlen; i++)
		{
			// shift to low index
			for(j=0; j<generator_len-1; j++)
			{
				remainder[j] = remainder[j+1];
			}
			
			// get next bit from inbuf
			remainder[generator_len-1] = inbuf[i];

			// sub by mod2
			if(remainder[0] == 1)
			{
				for(j=0; j<generator_len; j++)
				{
					remainder[j] = remainder[j] ^ generator_buf[j];
				}
			}
		}
		
		crc_error = 0;

		for (i=0; i<generator_len; i++)
		{
			if (remainder[i] == 1)
			{
				crc_error = 1;
				break;
			}
		}
	}
	
	return crc_error;	
}
