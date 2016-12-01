#include "foo.h"
#include "uerx_deblockseg.h"

int deblockseg(int16 **inbuf, int *size_each_codeseg, int num_codeseg, int num_nullbit
								, int *crc_error, int16 *outbuf)
{
	int i, j;
	int sum_out;
	int crc_error_blockseg;

	int16 generator_buf[25] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1};

	// function body
	if (num_codeseg > 1)
	{
		sum_out = 0;

		*crc_error = 0;

		for(i=0; i<num_codeseg; i++)
		{
			crc_error_blockseg = deblockseg_crc_check(inbuf[i], size_each_codeseg[i], generator_buf, 25);

			if (crc_error_blockseg == 1)
			{
				*crc_error = 1;

			}

			if(i == 0)
			{
				for(j=0; j<size_each_codeseg[i]-24-num_nullbit; j++)
				{
					outbuf[j+sum_out] = inbuf[i][j];
				}

				sum_out += size_each_codeseg[i]-24-num_nullbit;
			}
			else
			{
				for(j=0; j<size_each_codeseg[i]-24; j++)
				{
					outbuf[j+sum_out] = inbuf[i][j];
				}

				sum_out += size_each_codeseg[i]-24;
			}			
		} // end for(i=0; i<num_codeseg; i++)
	}
	else
	{
		*crc_error = 0;

		for(i=0; i<size_each_codeseg[0]-num_nullbit; i++)
		{
			outbuf[i] = inbuf[0][num_nullbit+i];
		}
		
		sum_out = size_each_codeseg[0]-num_nullbit;
	}

	return sum_out;
}


int deblockseg_crc_check(int16 *inbuf, int inlen, int16 *generator_buf, int generator_len)
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
