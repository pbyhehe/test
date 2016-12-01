#include "foo.h"
#include "uerx_phychdec_top.h"
#include "uerx_turbo_fixedPoint.h"



int decode_pdsch(float *inbuf, int inlen, int transblock_size, int catgry_ue, int num_harqprocess
									, int Qm, int rv_idx, int NL, int k_mimo, int NDI, int harqprocess_idx
									, int repet_times, int codeword_idx, int16 *outbuf)
{
	int i, j, d, k;
	
	int outlen_debolckseg;
	
	int size_each_codeseg[25];
	int num_codeseg;
	int num_nullbit;
	
	int error_decoder;

	int my_block_length;
	int numCodeBlockBanks;
	int numberOfPSISOs;
	int numberOfWindows;
	int PSISOLength;
	int windowLength;
    int PSISOLengthLast;
    int lastWindowLengthPSISO;
	int lastWindowLengthPSISOLast;
	int maxHalfIterations;
	int debugLevel;
	int numIterations;

	#pragma SET_DATA_SECTION("uerx_data")
	static float system1_inbuf[6400];
	static float Parity1_inbuf[6400];
	static float Parity2_inbuf[6400];
	static float system2_inbuf[4];
	
	static int16 outbuf_deblockseg[154300];

	static fixed codeBlock[19212];

	static float *d0_turbo[13], d0_turbo_mem[13][7000];
	static float *d1_turbo[13], d1_turbo_mem[13][7000];
	static float *d2_turbo[13], d2_turbo_mem[13][7000];
	static int16 *outbit_dec[13], outbit_dec_mem[13][6400];
	#pragma SET_DATA_SECTION()

	for (i=0; i<13; i++)
	{
		d0_turbo[i] = d0_turbo_mem[i];
		d1_turbo[i] = d1_turbo_mem[i];
		d2_turbo[i] = d2_turbo_mem[i];
		outbit_dec[i] = outbit_dec_mem[i];
	}

	#pragma SET_DATA_SECTION("uerx_data")
	static float *buf0_harq[30], buf0_harq_mem[30][310000];
	static float *buf1_harq[30], buf1_harq_mem[30][310000];
	static float *buf2_harq[30], buf2_harq_mem[30][310000];
	#pragma SET_DATA_SECTION()

	for (i=0; i<30; i++)
	{
		buf0_harq[i] = buf0_harq_mem[i];
		buf1_harq[i] = buf1_harq_mem[i];
		buf2_harq[i] = buf2_harq_mem[i];
	}

	// function body	
 	/*uerx_deratematch_u.*/deratematch_pdsch(inbuf, inlen, transblock_size, catgry_ue, num_harqprocess, Qm
		, rv_idx, NL, k_mimo, d0_turbo, d1_turbo, d2_turbo, size_each_codeseg, &num_codeseg, &num_nullbit);

	// store the d0_turbo, d1_turbo, d2_turbo
	d = 0;

	for(i=0; i<num_codeseg; i++)
	{
		for(j=0; j<size_each_codeseg[i]+4; j++)
		{
			buf0_harq[num_harqprocess*codeword_idx+harqprocess_idx][d+repet_times*76000] = d0_turbo[i][j];
			buf1_harq[num_harqprocess*codeword_idx+harqprocess_idx][d+repet_times*76000] = d1_turbo[i][j];
			buf2_harq[num_harqprocess*codeword_idx+harqprocess_idx][d+repet_times*76000] = d2_turbo[i][j];

			d++;
		}
	}
	
	if (!NDI)
	{
		// maximum ratio combining
		for(k=0; k<repet_times-1; k++)
		{
			d = 0;

			for(i=0; i<num_codeseg; i++)
			{
				for(j=0; j<size_each_codeseg[i]+4; j++)
				{
					d0_turbo[i][j] += buf0_harq[num_harqprocess*codeword_idx+harqprocess_idx][d+repet_times*76000];
					d1_turbo[i][j] += buf1_harq[num_harqprocess*codeword_idx+harqprocess_idx][d+repet_times*76000];
					d2_turbo[i][j] += buf2_harq[num_harqprocess*codeword_idx+harqprocess_idx][d+repet_times*76000];

					d++;
				}
			}
		}
	}
 
	for (i=0; i<num_codeseg; i++)
	{
		for (j=0; j<size_each_codeseg[i]; j++)
		{
			system1_inbuf[j] = d0_turbo[i][j];
			Parity1_inbuf[j] = d1_turbo[i][j];
			Parity2_inbuf[j] = d2_turbo[i][j];
		}  
		
		// resume the tail bit, 
		system1_inbuf[size_each_codeseg[i]+0] = d0_turbo[i][size_each_codeseg[i]+0];
		Parity1_inbuf[size_each_codeseg[i]+0] = d1_turbo[i][size_each_codeseg[i]+0];
		Parity2_inbuf[size_each_codeseg[i]+0] = d1_turbo[i][size_each_codeseg[i]+2];		

		system1_inbuf[size_each_codeseg[i]+1] = d2_turbo[i][size_each_codeseg[i]+0];
		Parity1_inbuf[size_each_codeseg[i]+1] = d0_turbo[i][size_each_codeseg[i]+1];
		Parity2_inbuf[size_each_codeseg[i]+1] = d0_turbo[i][size_each_codeseg[i]+3];		
		
		system1_inbuf[size_each_codeseg[i]+2] = d1_turbo[i][size_each_codeseg[i]+1];
		Parity1_inbuf[size_each_codeseg[i]+2] = d2_turbo[i][size_each_codeseg[i]+1];
		Parity2_inbuf[size_each_codeseg[i]+2] = d2_turbo[i][size_each_codeseg[i]+3];

		system2_inbuf[0] = d0_turbo[i][size_each_codeseg[i]+2];
		system2_inbuf[1] = d2_turbo[i][size_each_codeseg[i]+2];
		system2_inbuf[2] = d1_turbo[i][size_each_codeseg[i]+3];

		my_block_length = size_each_codeseg[i];
		numCodeBlockBanks = 1;
		numberOfPSISOs = 4;
		numberOfWindows = 1;
		PSISOLength = (int) ceil(my_block_length/(double)numberOfPSISOs);
		windowLength = (int) (PSISOLength / (double) numberOfWindows);
        PSISOLengthLast = my_block_length - PSISOLength*(numberOfPSISOs-1);
        lastWindowLengthPSISO = PSISOLength - (numberOfWindows-1)*windowLength;
		lastWindowLengthPSISOLast = PSISOLengthLast - (numberOfWindows-1)*windowLength;
		maxHalfIterations = 16;
		debugLevel = 0;
		
		for(k=0; k<my_block_length; k++) 
		{
			codeBlock[3*k+0] = system1_inbuf[k];
			codeBlock[3*k+1] = Parity1_inbuf[k];
			codeBlock[3*k+2] = Parity2_inbuf[k];   
		}
		
		for (k=0; k<3; k++)
		{
			codeBlock[3*my_block_length+2*k+0] = system1_inbuf[my_block_length+k];
			codeBlock[3*my_block_length+2*k+1] = Parity1_inbuf[my_block_length+k];
			codeBlock[3*my_block_length+2*k+7] = Parity2_inbuf[my_block_length+k];
            codeBlock[3*my_block_length+2*k+6] = system2_inbuf[k];
		}
		
		turboDec(codeBlock, my_block_length, numCodeBlockBanks, numberOfPSISOs, PSISOLength, PSISOLengthLast
			, numberOfWindows, windowLength, lastWindowLengthPSISO, lastWindowLengthPSISOLast, maxHalfIterations
			, debugLevel, &numIterations, outbit_dec[i]);
	}

	outlen_debolckseg = /*uerx_deblockseg_u.*/deblockseg(outbit_dec, size_each_codeseg, num_codeseg, num_nullbit
		, &error_decoder, outbuf_deblockseg);

	if (!error_decoder)
	{	// if deblockseg is rigth, then do decrcattach
		/*uerx_decrcattach_u.*/decrcattach_pdsch(outbuf_deblockseg, outlen_debolckseg, outbuf, &error_decoder);
	}

	return error_decoder;
}

int decode_pdcch(float *inbuf, int inlen, int transblock_size, int port_ue, int enable_port_select
									, int16 *x_rnti, int16 *outbuf)
{
	int outlen_deratematch;
	int outlen_viterbi;
	
	int error_decoder;

	float outbuf0_deratematch[500];
	float outbuf1_deratematch[500];
	float outbuf2_deratematch[500];

	int16 outbuf_viterbi[100];

	// function body	
	outlen_deratematch = /*uerx_deratematch_u.*/deratematch_pdcch(inbuf, inlen, transblock_size
		, outbuf0_deratematch, outbuf1_deratematch, outbuf2_deratematch);

    outlen_viterbi = viterbi_InputOutput(outbuf0_deratematch, outbuf1_deratematch, outbuf2_deratematch
		, outlen_deratematch, outbuf_viterbi);   

    /*uerx_decrcattach_u.*/decrcattach_pdcch(outbuf_viterbi, outlen_viterbi, port_ue, enable_port_select, x_rnti
		, outbuf, &error_decoder);

	return error_decoder;
}

int decode_pbch(float *inbuf, int inlen, int transblock_size, int num_bstx_ant, int16 *outbuf)
{
	int outlen_deratematch;
	int outlen_viterbi;

	int error_decoder;	

	float outbuf0_deratematch[500];
	float outbuf1_deratematch[500];
	float outbuf2_deratematch[500];

	int16 outbuf_viterbi[100];
	
	// function body
	outlen_deratematch = /*uerx_deratematch_u.*/deratematch_pbch(inbuf, inlen, transblock_size
		, outbuf0_deratematch, outbuf1_deratematch, outbuf2_deratematch);

    outlen_viterbi = viterbi_InputOutput(outbuf0_deratematch, outbuf1_deratematch, outbuf2_deratematch
		, outlen_deratematch, outbuf_viterbi);    

    /*uerx_decrcattach_u.*/decrcattach_pbch(outbuf_viterbi, outlen_viterbi, num_bstx_ant, outbuf, &error_decoder);

	return error_decoder;
}
