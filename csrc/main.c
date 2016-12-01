/*
 *  ======== main.c ========
 */

//#include <xdc/std.h>
//
//#include <xdc/runtime/System.h>
//
//#include <ti/sysbios/BIOS.h>
//
//#include <ti/sysbios/knl/Task.h>
#include "foo.h"
#include "bstx_top.h"
#include "uerx_top.h"
//float SNR_DB;
//float SNR_LIST[] = {-1.2f, -1.4f, -1.8f};
//int NUM_SNR = 1;
//extern int Init2();
//extern int Clear2();
//extern int InputOutput3(int subfrm_idx, int frm_idx, cfloat **outbuf);
int NUM_SYS_FRM;
//uerx_inf                Uerx_inf_u;
//uerx_dci_parse          uerx_dciparse_u;
//synch1				    uerx_synch_u;
//fir1				    uerx_lowpass_u[4];

// int NUM_SNR = 6;
// float SNR_LIST[6] = {-10, -5, 0, 5, 10, 15};

int NUM_SNR = 1;
float SNR_LIST[1] = {100};


int tdd_lte_run()
{
	int i, j;
	int frm_idx, subfrm_idx;
	int outlen;
	int num_subfrm_sim;


	float snr_dB;
	static cfloat *bstx_outbuf[4], bstx_outbuf_mem[4][30720];
	static cfloat *chan_outbuf[4], chan_outbuf_mem[4][30720];
	for(i=0; i<4; i++)
	{
		bstx_outbuf[i] = bstx_outbuf_mem[i];
		chan_outbuf[i] = chan_outbuf_mem[i];
	}

// 	bstx_top		bstx_txtop_u;
    //uerx_top		uerx_rxtop_u;
//	channel_matlab	channel_matlab_u;

	// function body
//	engEvalString(ep, "close all; clear all; clc;");

	num_subfrm_sim = 1;

	for (i=0; i<NUM_SNR; i++)
	{
		snr_dB = SNR_LIST[i];
//  		snr_dB = parse.FloatFromStr("SNR_DB");

		/*bstx_txtop_u.*/Init2();
		top_Init(snr_dB);
//		uerx_rxtop_u.Init(snr_dB);
//		channel_matlab_u.Init(snr_dB);

		for(j=0; j<num_subfrm_sim; j++)
		{
			//PrintInfo(1, "--------------------  subframe_idx = %d, \t\t SNR = %f\n", j, snr_dB);

			frm_idx     = j / 10;
			NUM_SYS_FRM = frm_idx;
			subfrm_idx  = j % 10;

		  outlen = /*bstx_txtop_u.*/InputOutput3(
				subfrm_idx,
				frm_idx,
				bstx_outbuf);

//		  Clear2();
//		  Init2();
// 			save.Run(bstx_outbuf[0], outlen, "bstx_outbuf0", TO_SAVE);
// 			save.Run(bstx_outbuf[1], outlen, "bstx_outbuf1", TO_SAVE);

//		outlen = channel_matlab_u.InputOutput(
//				bstx_outbuf,
//				outlen,
//				chan_outbuf);

			// save.Run(chan_outbuf[0], outlen, "chanout0", TO_SAVE);
			// save.Run(chan_outbuf[1], outlen, "chanout1", TO_SAVE);

			/*uerx_rxtop_u.*/uerx_top_InputOutput(
				frm_idx,
    			subfrm_idx,
		                bstx_outbuf,	//	chan_outbuf,
				outlen);
		};

		/*bstx_txtop_u.*/Clear2();
		//uerx_rxtop_u.Clear();
		uerx_top_Clear();
//		channel_matlab_u.Clear();
	}

	return 0;
}



int main(int argc, char* argv[])
{
	//InitSourSaveFFTParse("../configdata/", "bolt", "../dataout", "../datasource");
	tdd_lte_run();

	//CloseSourSaveFFTParse();

//#ifdef WIN32
//	_fcloseall();
//#endif

	return 0;
}
