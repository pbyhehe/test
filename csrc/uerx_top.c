#include "uerx_top.h"
#include "uerx_parse.h"

extern int fft_InputOutput3(cfloat *Iin, int size, cfloat *out);

#define DUMP_RXTOP 1

#pragma DATA_SECTION(uerx_dciparse_u, "uerx_data")
    uerx_dci_parse          uerx_dciparse_u;
#pragma DATA_SECTION(uerx_synch_u, "uerx_data")
    synch1				    uerx_synch_u;
#pragma DATA_SECTION(uerx_lowpass_u, "uerx_data")
    fir1				    uerx_lowpass_u[4];
#pragma DATA_SECTION(Uerx_inf_u, "uerx_data")
    uerx_inf                Uerx_inf_u;

#define AFC_ON		0
//extern float snr_dB;
extern int NUM_SYS_FRM;

int top_Init(float snr_dB)
{

	int i, n, p;

	// init system parameter
	Uerx_inf_u.fdd_tdd_mode			= FDD_TDD_MODE;
	Uerx_inf_u.num_ID_cell			= N_ID_CELL;
	Uerx_inf_u.num_uerx_ant			= NUM_UERX_ANTENNAS;
	Uerx_inf_u.cp_cfg				= CP_CONFIG;
	Uerx_inf_u.sampling_rate_MHz	= SAMPLING_RATE_MHZ;
	Uerx_inf_u.num_bstx_ant			= NUM_BSTX_ANTENNAS;
	Uerx_inf_u.uldl_switch_cfg		= ULDL_SWITCH_CONFIG;
	Uerx_inf_u.spec_subfrm_cfg		= SPECIAL_SUBFRAME_CONFIG;
	Uerx_inf_u.num_ofdm_pdcch		= L_PDCCH;
	Uerx_inf_u.num_RB_DL			= NUM_RB_DL;
	Uerx_inf_u.num_RB_UL			= NUM_RB_UL;
	Uerx_inf_u.size_fft				= N_FFT; // 20141128 linjiangnan
	Uerx_inf_u.snr_dB				= snr_dB;

	// power assignment
	Uerx_inf_u.pcfich_RB			= RB_PCFICH;
	Uerx_inf_u.phich_RA				= RA_PHICH;
	Uerx_inf_u.phich_RB				= RB_PHICH;
	Uerx_inf_u.pdcch_RA				= RA_PDCCH;
	Uerx_inf_u.pdcch_RB				= RB_PDCCH;
	Uerx_inf_u.pdsch_RA				= RA_PDSCH;
	Uerx_inf_u.pdsch_RB				= RB_PDSCH;
	Uerx_inf_u.pbch_RA				= RA_PBCH;
	Uerx_inf_u.pbch_RB				= RB_PBCH;

	// phich parameters written by mac
	Uerx_inf_u.idx_group_phich		= 0;	//GROUP_INDEX_PHICH[3]
	Uerx_inf_u.idx_seque_phich		= 3;	//SEQUE_INDEX_PHICH[3]
	Uerx_inf_u.ng_cfg_phich			= NG_CONFIG_PHICH;
	Uerx_inf_u.duration_cfg_phich	= DURATION_CONFIG_PHICH;

	// pdcch parameters written by mac
	Uerx_inf_u.pdcch_format			= 1;	//PDCCH_FORMAT_UE[0]
	Uerx_inf_u.dci_format			= 5;	//DCI_FORMAT_UE[0]
	Uerx_inf_u.num_rnti				= 0;	//NUM_RNTI_UE[0]
	Uerx_inf_u.ue_port				= 0;	//PORT_UE[0]
	Uerx_inf_u.uetx_ant_selec_cfg	= 0;	//TRANSMIT_ANTENNA_SELECTE_UE[0]

	// pdsch parameters written by mac
	Uerx_inf_u.rb_start_pdsch		= 0;	//RB_START_UE[0]
	Uerx_inf_u.num_alloc_rb			= 100;	//NUM_RB_ALLOC_UE[0]
	Uerx_inf_u.ue_category			= 5;	//CATEGORY_UE[0]
	Uerx_inf_u.trans_mode			= 4;	//TRANSMISSION_MODE_UE[0]
	Uerx_inf_u.ue_RS_cfg			= 0;	//URS_CONFIG_UE[0]
	Uerx_inf_u.rnti_type			= 3;	//RNTI_TYPE_UE[0]

	// rnti number to rnti array
	n = Uerx_inf_u.num_rnti;
	for (i=15; i>=0; i--)
	{
		Uerx_inf_u.num_rnti_bin[i] = n % 2;
		n /= 2;
	}

	// caculate the pdsch parameters
	Uerx_inf_u.ri_last	= -1;
	Uerx_inf_u.pmi_last	= -1;

	Uerx_inf_u.num_harq_process = UERX_NUM_HARQ_PROCESS_LIST[Uerx_inf_u.uldl_switch_cfg];

	for(i=0; i<110; i++)
	{
		if(i >= Uerx_inf_u.rb_start_pdsch && i < (Uerx_inf_u.rb_start_pdsch+Uerx_inf_u.num_alloc_rb))
		{
			Uerx_inf_u.rb_alloc_bitmap[0][i] = 1;
			Uerx_inf_u.rb_alloc_bitmap[1][i] = 1;
		}
		else
		{
			Uerx_inf_u.rb_alloc_bitmap[0][i] = 0;
			Uerx_inf_u.rb_alloc_bitmap[1][i] = 0;
		}
	}


	synch_Init(&uerx_synch_u);
	dci_Init(&uerx_dciparse_u);
	decode_Init();
	//uerx_phychdec_top_u.Init();

	for (p=0; p<4; p++)
	{
//		if((int)Uerx_inf_u.sampling_rate_MHz == 30)
//		{
//			uerx_lowpass_u[p].SetParameter("../configdata/testcase0/cell_search_lowpass_128taps_20M.txt");
//		}
//		else if ((int)Uerx_inf_u.sampling_rate_MHz == 15)
//		{
//			uerx_lowpass_u[p].SetParameter("../configdata/testcase0/cell_search_lowpass_128taps_10M.txt");
//		}
//		else if ((int)Uerx_inf_u.sampling_rate_MHz == 7)
//		{
//			uerx_lowpass_u[p].SetParameter("../configdata/testcase0/cell_search_lowpass_128taps_5M.txt");
//		}
//		else if ((int)Uerx_inf_u.sampling_rate_MHz == 3)
//		{
//			uerx_lowpass_u[p].SetParameter("../configdata/testcase0/cell_search_lowpass_128taps_3M.txt");
//		}
//		else if ((int)Uerx_inf_u.sampling_rate_MHz == 1)
//		{
//			uerx_lowpass_u[p].SetParameter("../configdata/testcase0/cell_search_lowpass_128taps_1.4M.txt");
//		}
//		else
//		{
//			assert(0);
//		}
		fir_Init(&uerx_lowpass_u[p]);
//		uerx_lowpass_u[p].Init();
	}
	int Num_downsamp;
	Uerx_inf_u.num_subcarrier = Uerx_inf_u.num_RB_DL * 12; // 20141128 linjiangnan
	Num_downsamp = Uerx_inf_u.size_fft / 128; // 20141128 linjiangnan

// 	if(Uerx_inf_u.num_RB_DL == 100)
// 	{
// 		Uerx_inf_u.num_subcarrier = 1200;
// 		Uerx_inf_u.size_fft = 2048;
//
// 		Num_downsamp = 2048 / 128;
// 	}
// 	else if(Uerx_inf_u.num_RB_DL == 75)
// 	{
// 		Uerx_inf_u.num_subcarrier = 900;
// 		Uerx_inf_u.size_fft = 2048;
//
// 		Num_downsamp = 2048 / 128;
// 	}
// 	else if (Uerx_inf_u.num_RB_DL == 50)
// 	{
// 		Uerx_inf_u.num_subcarrier = 600;
// 		Uerx_inf_u.size_fft = 1024;
//
// 		Num_downsamp = 1024 / 128;
// 	}
// 	else if(Uerx_inf_u.num_RB_DL == 25)
// 	{
// 		Uerx_inf_u.num_subcarrier = 300;
// 		Uerx_inf_u.size_fft = 512;
//
// 		Num_downsamp = 512 / 128;
// 	}
// 	else if(Uerx_inf_u.num_RB_DL == 15)
// 	{
// 		Uerx_inf_u.num_subcarrier = 180;
// 		Uerx_inf_u.size_fft = 256;
//
// 		Num_downsamp = 256 / 128;
// 	}
// 	else if(Uerx_inf_u.num_RB_DL == 6)
// 	{
// 		Uerx_inf_u.num_subcarrier = 72;
// 		Uerx_inf_u.size_fft = 128;
//
// 		Num_downsamp = 128 / 128;
// 	}
// 	else
// 	{
// 		assert(0);
// 	}


	return 0;
}

int uerx_top_Clear()
{
	synch_Clear();
	dci_Clear(&uerx_dciparse_u);
	decode_Clear();
//	uerx_phychdec_top_u.Clear();

	fir_Clear(&uerx_lowpass_u[0]);
	fir_Clear(&uerx_lowpass_u[1]);
	fir_Clear(&uerx_lowpass_u[2]);
	fir_Clear(&uerx_lowpass_u[3]);
//	uerx_lowpass_u[0].Clear();
//	uerx_lowpass_u[1].Clear();
//	uerx_lowpass_u[2].Clear();
//	uerx_lowpass_u[3].Clear();

	return 0;
}

int uerx_top_InputOutput(int frm_idx, int subfrm_idx, cfloat **inbuf, int inlen)
{
	int i, p, c;
	int ofdm_idx;
// 	int subfrm_idx, frm_idx;
	int ret_deframe;	
	int crc_check_pdcch;
	int crc_check_pdsch;
	int crc_check_pbch;
	int tbs_pdcch;
	int qm, rv_idx;
	int k_mimo;
	int new_data_indicat;
	int repet_times;
	int codeword_idx;
	int num_grp_phich;
	int mi_phich;
	int dci_sourlen;		
	int frm_idx_pbch;
	int outlen_getRS;
	int rs_start[4];	
	
	int len, len0, len1;
	int outlen_downsample;
	int outlen_rmvRS;
	int outlen_equ;
	int outlen_deremap;
	int outlen_deintelv;
	int outlen_deqam;
	int outlen_deqam_cw[2];
	int outlen_descram;	
	int outlen_descram_cw[2];
	
	int16 payload_pbch[24];
	int16 payload_phich;
	
	float nosieVar;

	#pragma SET_DATA_SECTION("uerx_data")
	static cfloat lwpsdata[2][30720];
	static cfloat dsdata[2][2048];

	static float outS_deqam[2][7200];	
	static float outS_descram_cw[2][7200*13];
	static float outS_descram_pbch[480];
	static float phase_step = 0;	
		
	static cfloat rs_buf[220];
	
	static cfloat outX_deremap_ctrlch[3600];
		
	static cfloat outX_deintlv_pdcch[3600];
	static cfloat outX_phich_deseq[3];
	static cfloat csi_deremap_ctrlch[3600];
	
	static cfloat csi_deintlv_pdcch[3600];
	static cfloat csi_phich_deseq[3];
	
	static cfloat *rmfqdata[2], rmfqdata_mem[2][30720];
	static cfloat *rxofdm_time_list[2], rxofdm_time_list_mem[2][14*2048];
	static cfloat *rxofdm_time[2], rxofdm_time_mem[2][2048];
	static cfloat *rxofdm_freq[2], rxofdm_freq_mem[2][2048];
	static cfloat *outY[2], outY_mem[2][2048];
	static cfloat *outY_deremap[2], outY_deremap_mem[2][1200];
	static cfloat *outH_ce[4], outH_ce_mem[4][1200];
	static cfloat *outH_deremap[4], outH_deremap_mem[4][1200];
	static cfloat *outX_equ[3], outX_equ_mem[3][1200];
	static cfloat *csi_equ[3], csi_equ_mem[3][1200];
	static cfloat *ns_equ[3], ns_equ_mem[3][1200];
	static int sync_end = 0;
	static int sync_pos = 0;
	static int totallen_descram_cw[2], totallen_descram_pbch;
	static int right_block_pcfich, right_block_phich, right_block_pdcch;
	static int right_block_pbch, right_block_pdsch;
	static int recive_block_pcfich, recive_block_phich, recive_block_pdcch;
	static int recive_block_pbch, recive_block_pdsch;
	static int tbs_cw[2];
	static int reset_descram_pdsch, reset_descram_pbch;

	static int16 num_ofdm_pdcch, num_codeword, diver_spatial;
	static int16 layers_total, num_layer_spatial_cw[2];
	static int16 outBit_dec[151376];

	static int16 num_rb_alloc;
	static int16 harqprocess_idx;
	static int16 ndi_cw[2], rv_cw[2];
	static int16 qm_cw[2], tpc_pucch, dai, codebook_idx, cobook_column_idx;
	#pragma SET_DATA_SECTION()

	for(p=0; p<2; p++)
	{
		rmfqdata[p]			= rmfqdata_mem[p];
		rxofdm_time_list[p] = rxofdm_time_list_mem[p];
		rxofdm_time[p]		= rxofdm_time_mem[p];
		rxofdm_freq[p]		= rxofdm_freq_mem[p];
		outY[p]				= outY_mem[p];
		outY_deremap[p]		= outY_deremap_mem[p];
	}


	for(i=0; i<4; i++)
	{
		outH_ce[i] = outH_ce_mem[i];
		outH_deremap[i] = outH_deremap_mem[i];
	}


	for(i=0; i<3; i++)
	{
		outX_equ[i] = outX_equ_mem[i];
		csi_equ[i] = csi_equ_mem[i];
		ns_equ[i] = ns_equ_mem[i];
	}	

	int num_RB_DL			= Uerx_inf_u.num_RB_DL;
	int num_RB_UL			= Uerx_inf_u.num_RB_UL;
	int num_subcarrier		= Uerx_inf_u.num_subcarrier;
	int size_fft			= Uerx_inf_u.size_fft;
	int	ng_cfg_phich		= Uerx_inf_u.ng_cfg_phich;
	int num_ID_cell			= Uerx_inf_u.num_ID_cell;
	int sampling_rate_MHz	= Uerx_inf_u.sampling_rate_MHz;
	int pdcch_format		= Uerx_inf_u.pdcch_format;			// by detection
	int dci_format			= Uerx_inf_u.dci_format;			// by detection
	int uldl_switch_cfg		= Uerx_inf_u.uldl_switch_cfg;
	int cp_cfg				= Uerx_inf_u.cp_cfg;
	int fdd_tdd_mode		= Uerx_inf_u.fdd_tdd_mode;
	int num_bstx_ant		= Uerx_inf_u.num_bstx_ant;
	int num_uerx_ant		= Uerx_inf_u.num_uerx_ant;
	int ue_RS_cfg			= Uerx_inf_u.ue_RS_cfg;		
	int duration_cfg_phich	= Uerx_inf_u.duration_cfg_phich;
	int catgry_ue			= Uerx_inf_u.ue_category;
	int ue_port				= Uerx_inf_u.ue_port;
	int	uetx_ant_sel_cfg	= Uerx_inf_u.uetx_ant_selec_cfg;
	int rnti_type			= Uerx_inf_u.rnti_type;
	int trans_mode			= Uerx_inf_u.trans_mode;
	int ri_last				= Uerx_inf_u.ri_last;
	int pmi_last			= Uerx_inf_u.pmi_last;
	int idx_group_phich		= Uerx_inf_u.idx_group_phich;
	int idx_seque_phich		= Uerx_inf_u.idx_seque_phich;
	int num_rnti			= Uerx_inf_u.num_rnti;
	int num_harq_process	= Uerx_inf_u.num_harq_process;
	//add 14.6.9
	int Preamble_idx		=Uerx_inf_u.Preamble_idx;
	int PRACH_Mask_idx		=Uerx_inf_u.PRACH_Mask_idx;


	float snr_dB			= Uerx_inf_u.snr_dB;	

	int16 *num_rnti_bin		= Uerx_inf_u.num_rnti_bin;

	float gain_pcfich_RouB	= pow(10, -Uerx_inf_u.pcfich_RB / 20);	
	float gain_phich_RouA	= pow(10, -Uerx_inf_u.phich_RA / 20);
	float gain_phich_RouB	= pow(10, -Uerx_inf_u.phich_RB / 20);  
	float gain_pdcch_RouA	= pow(10, -Uerx_inf_u.pdcch_RA / 20);
	float gain_pdcch_RouB	= pow(10, -Uerx_inf_u.pdcch_RB / 20);  
	float gain_pdsch_RouA	= pow(10, -Uerx_inf_u.pdsch_RA / 20);
	float gain_pdsch_RouB	= pow(10, -Uerx_inf_u.pdsch_RB / 20); 
	float gain_pbch_RouA	= pow(10, -Uerx_inf_u.pbch_RA / 20);
	float gain_pbch_RouB	= pow(10, -Uerx_inf_u.pbch_RB / 20); 
//	UERX InBuffer
//	if (DUMP_RXTOP) save.Run(inbuf[0], inlen, "uerx_irx_in_ant0", TO_SAVE);
//	if (DUMP_RXTOP) save.Run(inbuf[1], inlen, "uerx_irx_in_ant1", TO_SAVE);

	// AGC
	agc_InputOutput(
		num_uerx_ant,
		inlen,
		inbuf,
		inbuf);
//	if (DUMP_RXTOP) save.Run(inbuf[0], inlen, "uerx_irx_agc_ant0", TO_SAVE);
//	if (DUMP_RXTOP) save.Run(inbuf[1], inlen, "uerx_irx_agc_ant1", TO_SAVE);
	nosieVar = 1 / pow(10, snr_dB/10.0);
//	if (DUMP_RXTOP) save.Run(nosieVar, "uerx_irx_nosieVar", TO_SAVE);


// 	// CFO Estimation
// 	if (AFC_ON)
// 	{
// 		for (i=0; i<inlen; i++)
// 		{
// 			uerx_synch_u.afccarse(
// 				inbuf[0][i],
// 				sampling_rate_MHz,
// 				&phase_step);
// 		}
// 	}
// 
// 	// remove frequency offset
// 	uerx_synch_u.rmfreqofst(
// 		inbuf,
// 		inlen,
// 		num_uerx_ant,
// 		phase_step,
// 		rmfqdata);
// 
// 	// no frequency offset
// 	for (i = 0; i<inlen; i++)
// 	{
// 		rmfqdata[0][i] = inbuf[0][i];
// 		rmfqdata[1][i] = inbuf[1][i];
// 	}
// 
// 	if (DUMP_RXTOP) save.Run(rmfqdata[0], inlen, "uerx_irx_rmvcfo_ant0", TO_SAVE);
// 	if (DUMP_RXTOP) save.Run(rmfqdata[1], inlen, "uerx_irx_rmvcfo_ant1", TO_SAVE);	
// 	
// 	// synchronization
// 	for (p=0; p<num_uerx_ant; p++)
// 	{
// 		uerx_lowpass_u[p].InputOutput(
// 			rmfqdata[p],
// 			inlen,
// 			lwpsdata[p]);
// 		
// 		outlen_downsample = downsample(
// 			lwpsdata[p],
// 			inlen,
// 			Num_downsamp,
// 			dsdata[p]);
// 	}		
// 		
// 	if (DUMP_RXTOP) save.Run(dsdata[0], outlen_downsample, "uerx_irx_downsample_ant0", TO_SAVE);
// 	if (DUMP_RXTOP) save.Run(dsdata[1], outlen_downsample, "uerx_irx_downsample_ant1", TO_SAVE);
// 
// 
// 	if (!sync_end)
// 	{
// 		sync_end = uerx_synch_u.psscorr(
// 			dsdata[0],
// 			outlen_downsample,
// 			num_ID_cell,
// 			sync_pos);
// 
// 		frm_idx_pbch = NUM_SYS_FRM;
// 	}
// 
// 
// 	// Deframe
// 	ret_deframe = uerx_deframe_u.deframe(
// 		sync_end,
// 		rmfqdata,
// 		sync_pos,
// 		num_uerx_ant,
// 		size_fft,
// 		frm_idx_pbch,
// 		rxofdm_time_list,
// 		subfrm_idx,
// 		frm_idx);


	num_ID_cell = N_ID_CELL; // N_ID_CELL
	ret_deframe = 1;

	// Remove CP
	int N_CP_long, N_CP_short, N_CP, start_symbol;
	if (cp_cfg==0) // CP_NORMAL
	{
		N_CP_long = size_fft / 128 * 10;
		N_CP_short = size_fft / 128 * 9;
	}
	else
	{
		N_CP_long = size_fft / 128 * 16;
		N_CP_short = size_fft / 128 * 16;
	}

	start_symbol = 0;
	for(ofdm_idx=0; ofdm_idx<14; ofdm_idx++)
	{
		if (ofdm_idx==0 || ofdm_idx==7)
		{
			N_CP = N_CP_long;
		}
		else
		{
			N_CP = N_CP_short;
		}

		for (p=0; p<num_uerx_ant; p++)
		{	
			for (i = 0; i < size_fft; i++)
			{
				rxofdm_time_list[p][ofdm_idx*size_fft + i] = inbuf[p][start_symbol+N_CP + i];
			}
		}

		start_symbol += N_CP + size_fft;
	}
//	if (DUMP_RXTOP) save.Run(rxofdm_time_list[0], size_fft*14, "uerx_irx_deframe_ant0", TO_SAVE);
//	if (DUMP_RXTOP) save.Run(rxofdm_time_list[1], size_fft*14, "uerx_irx_deframe_ant1", TO_SAVE);

	// DeOFDM
	for(ofdm_idx=0; ofdm_idx<14; ofdm_idx++)
	{
		for (p=0; p<num_uerx_ant; p++)
		{
			for(i=0; i<size_fft; i++)
			{
				rxofdm_time[p][i] = rxofdm_time_list[p][i + ofdm_idx*size_fft];
			}
		}

		// if get one ofdm and is downlink
		if (ret_deframe && /*uerx_sharefunc_u.*/uerx_isDL(subfrm_idx, uldl_switch_cfg))// && frm_idx==0 && subfrm_idx==8)
		{
//			if (DUMP_RXTOP) save.Run(rxofdm_time[0], size_fft, "uerx_irx_rmvcp_ant0", TO_SAVE);
//		    if (DUMP_RXTOP) save.Run(rxofdm_time[1], size_fft, "uerx_irx_rmvcp_ant1", TO_SAVE);
			for (p=0; p<num_uerx_ant; p++)
			{	// rxofdm_time<16,13>, rxofdm_freq<16,11>
				fft_InputOutput3(
					rxofdm_time[p],
					size_fft,
					rxofdm_freq[p]);
			}

//			if (DUMP_RXTOP) save.Run(rxofdm_freq[0], size_fft, "uerx_irx_fft_ant0", TO_SAVE);
//			if (DUMP_RXTOP) save.Run(rxofdm_freq[1], size_fft, "uerx_irx_fft_ant1", TO_SAVE);
			    get_subcarrier(
				rxofdm_freq,
				size_fft,
				num_subcarrier,
				num_uerx_ant,
				outY);
//			if (DUMP_RXTOP) save.Run(outY[0], num_subcarrier, "uerx_irx_dexplace_ant0", TO_SAVE);
//			if (DUMP_RXTOP) save.Run(outY[1], num_subcarrier, "uerx_irx_dexplace_ant1", TO_SAVE);

// 			// inbuf<16,X>, outbuf<16,11>
// 			uerx_agc_u.fix_gain_ctrl(
// 				outY,
// 				num_subcarrier,
// 				num_uerx_ant,
// 				ofdm_idx,
// 				outY);
// 
// 			if (DUMP_RXTOP) save.Run(outY[0], num_subcarrier, "uerx_irx_dexplace_ant0_fix", TO_SAVE);
// 			if (DUMP_RXTOP) save.Run(outY[1], num_subcarrier, "uerx_irx_dexplace_ant0_fix", TO_SAVE);

			// channel estimation
			outlen_getRS = get_RS(
				num_ID_cell,
				num_bstx_ant,
				num_RB_DL,
				cp_cfg,
				subfrm_idx,
				ofdm_idx,
				rs_buf,
				rs_start);

//			if (DUMP_RXTOP) save.Run(rs_buf, outlen_getRS, "uerx_irx_rs_gen", TO_SAVE); // linjiangnan
			 ce(
				outY,
				num_subcarrier,
				rs_buf,
				rs_start,
				outlen_getRS,
				num_bstx_ant,
				num_uerx_ant,
				outH_ce);
//			if (DUMP_RXTOP) save.Run(outH_ce[0], num_subcarrier, "uerx_irx_ce_H0", TO_SAVE);
//			if (DUMP_RXTOP) save.Run(outH_ce[1], num_subcarrier, "uerx_irx_ce_H1", TO_SAVE);
//			if (DUMP_RXTOP) save.Run(outH_ce[2], num_subcarrier, "uerx_irx_ce_H2", TO_SAVE);
//			if (DUMP_RXTOP) save.Run(outH_ce[3], num_subcarrier, "uerx_irx_ce_H3", TO_SAVE);
		
			// init every subfram starting
			if (ofdm_idx == 0)
			{				
				totallen_descram_cw[0] = 0;
				totallen_descram_cw[1] = 0;
				totallen_descram_pbch = 0;

				reset_descram_pdsch = 1;
				reset_descram_pbch = 1;
			}


			/* CCH */
			if (ofdm_idx==0 || ofdm_idx<num_ofdm_pdcch)
			{

				// remove RS for PCFICH,PHICH,PDCCH
				rmvRS_ctrlch(
					outY,
					num_uerx_ant,
					num_RB_DL,
					ofdm_idx,
					num_ID_cell,
					num_bstx_ant,
					outY);

					
				outlen_rmvRS = rmvRS_ctrlch(
					outH_ce,
					num_uerx_ant*num_bstx_ant,
					num_RB_DL,
					ofdm_idx,
					num_ID_cell,
					num_bstx_ant,
					outH_ce);				
//				if (DUMP_RXTOP) save.Run(outH_ce[0], outlen_rmvRS, "uerx_irx_cch_rmvrs_ce_H0", TO_SAVE);
//				if (DUMP_RXTOP) save.Run(outH_ce[1], outlen_rmvRS, "uerx_irx_cch_rmvrs_ce_H1", TO_SAVE);
//				if (DUMP_RXTOP) save.Run(outH_ce[2], outlen_rmvRS, "uerx_irx_cch_rmvrs_ce_H2", TO_SAVE);
//				if (DUMP_RXTOP) save.Run(outH_ce[3], outlen_rmvRS, "uerx_irx_cch_rmvrs_ce_H3", TO_SAVE);
//				if (DUMP_RXTOP) save.Run(outlen_rmvRS, "uerx_irx_cch_rmvrs_len", TO_SAVE);
				// EQU for PCFICH,PHICH,PDCCH
				outlen_equ = equ_ctrch(
					outY,
					outH_ce,
					outlen_rmvRS,
					nosieVar,
					num_bstx_ant,
					num_uerx_ant,
					outX_equ[ofdm_idx],
					csi_equ[ofdm_idx]);
//				if (DUMP_RXTOP) save.Run(outX_equ[ofdm_idx], outlen_equ, "uerx_irx_cch_equ", TO_SAVE);
//				if (DUMP_RXTOP) save.Run(csi_equ[ofdm_idx], outlen_equ, "uerx_irx_cch_equ_csi", TO_SAVE);
//				if (DUMP_RXTOP) save.Run(outlen_equ, "uerx_irx_cch_len", TO_SAVE);
			} // end if (ofdm_idx==0 || ofdm_idx<num_ofdm_pdcch)


			/* PCFICH */
			if (ofdm_idx == 0)
			{
				// DeRemap for PCFICH
				deremap_pcfich(
					outX_equ[0],
					num_RB_DL,
					num_ID_cell,
					gain_pcfich_RouB,
					outX_deremap_ctrlch);

				deremap_pcfich(
					csi_equ[0],
					num_RB_DL,
					num_ID_cell,
					1,
					csi_deremap_ctrlch);
//				if (DUMP_RXTOP) save.Run(outX_deremap_ctrlch, 16, "uerx_irx_pcfich_redemap", TO_SAVE);
//				if (DUMP_RXTOP) save.Run(csi_deremap_ctrlch, 16, "uerx_irx_pcfich_redemap_csi", TO_SAVE);

				// DeQAM for PCFICH
				deqam(
					outX_deremap_ctrlch,
					csi_deremap_ctrlch,
					nosieVar,
					16,
					UERX_QPSK_BITS,
					outS_deqam[0]);
				
//				if (DUMP_RXTOP) save.Run(outS_deqam[0], 32, "uerx_irx_pcfich_deqam", TO_SAVE);
				// descramb for PCFICH
				descram_pcfich(
					outS_deqam[0],
					32,
					num_ID_cell,
					subfrm_idx,
					outS_descram_cw[0]);
//				if (DUMP_RXTOP) save.Run(outS_descram_cw[0], 32, "uerx_irx_pcfich_descramble", TO_SAVE);


				// decode for PCFICH
				num_ofdm_pdcch = decode_pcfich(
					outS_descram_cw[0],
					32,
					num_RB_DL);

//				if (DUMP_RXTOP) save.Run(num_ofdm_pdcch, "uerx_irx_pcfich_decode", TO_SAVE);
				// just for statistic
				recive_block_pcfich++;
				if (num_ofdm_pdcch == Uerx_inf_u.num_ofdm_pdcch)
				{
					right_block_pcfich++;
				}


			} // end if (ofdm_idx == 0)			


			/* PDCCH & PHICH*/
			if (ofdm_idx == num_ofdm_pdcch-1)
			{
				/* PHICH(1) */
				// PHICH groups
				num_grp_phich = get_num_phich_group(
					num_RB_DL,
					ng_cfg_phich,
					cp_cfg);

				// mi
				mi_phich = uerx_get_mi_phich(
					subfrm_idx,
					uldl_switch_cfg);	
//				if (DUMP_RXTOP) save.Run(mi_phich, "uerx_irx_phich_mi", TO_SAVE);

				/* PDCCH */
				// deremap PDCCH
				deremap_pdcch(
					outX_equ,
					num_RB_DL,
					num_grp_phich,
					mi_phich,
					fdd_tdd_mode,
					cp_cfg,
					subfrm_idx,
					num_ofdm_pdcch,
					num_ID_cell,
					num_bstx_ant,
					duration_cfg_phich,
					gain_pdcch_RouA,
					gain_pdcch_RouB,
					outX_deremap_ctrlch);

				outlen_deremap = deremap_pdcch(
					csi_equ,
					num_RB_DL,
					num_grp_phich,
					mi_phich,
					fdd_tdd_mode,
					cp_cfg, subfrm_idx,
					num_ofdm_pdcch,
					num_ID_cell,
					num_bstx_ant,
					duration_cfg_phich,
					1,
					1,
					csi_deremap_ctrlch);
//				if (DUMP_RXTOP) save.Run(outX_deremap_ctrlch, outlen_deremap, "uerx_irx_pdcch_redemap", TO_SAVE);
//				if (DUMP_RXTOP) save.Run(csi_deremap_ctrlch, outlen_deremap, "uerx_irx_pdcch_redemap_csi", TO_SAVE);
//				if (DUMP_RXTOP) save.Run(outlen_deremap, "uerx_irx_pdcch_redemap_len", TO_SAVE);

				// deinterlive for PDCCH
				desymbintelv_pdcch(
					outX_deremap_ctrlch, 
					outlen_deremap,
					num_ID_cell,
					outX_deintlv_pdcch);

				outlen_deintelv = desymbintelv_pdcch(
					csi_deremap_ctrlch,
					outlen_deremap,
					num_ID_cell,
					csi_deintlv_pdcch);
//				if (DUMP_RXTOP) save.Run(outX_deintlv_pdcch, outlen_deintelv, "uerx_irx_pdcch_deinterleave", TO_SAVE);
//				if (DUMP_RXTOP) save.Run(csi_deintlv_pdcch, outlen_deintelv, "uerx_irx_pdcch_deinterleave_csi", TO_SAVE);
//				if (DUMP_RXTOP) save.Run(outlen_deintelv, "uerx_irx_pdcch_deinterleave_len", TO_SAVE);

				// PDCCH DeQAM
				outlen_deqam = deqam(
					outX_deintlv_pdcch,
					csi_deintlv_pdcch,
					nosieVar,
					outlen_deintelv,
					UERX_QPSK_BITS,
					outS_deqam[0]);
//				if (DUMP_RXTOP) save.Run(outS_deqam[0], outlen_deqam, "uerx_irx_pdcch_deqam", TO_SAVE);
//				if (DUMP_RXTOP) save.Run(outlen_deqam, "uerx_irx_pdcch_deqam_len", TO_SAVE);
				// PDCCH DeScramble
				outlen_descram = descram_pdcch(
					outS_deqam[0],
					outlen_deqam,
					num_ID_cell,
					subfrm_idx,
					outS_descram_cw[0]);

//				if (DUMP_RXTOP) save.Run(outS_descram_cw[0], outlen_descram, "uerx_irx_pdcch_descramble", TO_SAVE);
//				if (DUMP_RXTOP) save.Run(outlen_descram, "uerx_irx_pdcch_descramble_len", TO_SAVE);
				// PDCCH Decode
				len = (1<<pdcch_format)*72;
				dci_sourlen	= uerx_GetDCIlen(&uerx_dciparse_u,
					dci_format,
					num_RB_UL,
					num_RB_DL,
					num_bstx_ant);

				tbs_pdcch = dci_sourlen;

				crc_check_pdcch = decode_pdcch(
					outS_descram_cw[0],
					len,
					tbs_pdcch,
					ue_port,
					uetx_ant_sel_cfg,
					num_rnti_bin,
					outBit_dec);
//				if (DUMP_RXTOP) save.Run(outBit_dec, tbs_pdcch, "uerx_irx_pdcch_decode", TO_SAVE);

				dciparse(&uerx_dciparse_u,
					&dci_format,
					num_RB_DL,
					num_RB_UL,
					fdd_tdd_mode,
					rnti_type,
					trans_mode,
					num_bstx_ant,
					outBit_dec,
					tbs_pdcch,
					ri_last,
					pmi_last,
					&num_codeword,
					&diver_spatial,
					&layers_total,
					num_layer_spatial_cw,
					Uerx_inf_u.rb_alloc_bitmap,
					&num_rb_alloc,
					&harqprocess_idx,
					ndi_cw,
					rv_cw,
					tbs_cw,
					qm_cw,
					&tpc_pucch,
					&dai,
					&codebook_idx,
					&cobook_column_idx,
					Preamble_idx,
					PRACH_Mask_idx);

				// just for statistic
				recive_block_pdcch++;
				if (crc_check_pdcch == 0)
				{
					right_block_pdcch++;
				}

				/* PHICH(2) */
				if (mi_phich)
				{

					// PHICH REDemap
					deremap_phich(
						outX_equ,
						num_RB_DL,
						subfrm_idx,
						num_ID_cell,
						num_bstx_ant,
						duration_cfg_phich,
						cp_cfg,
						idx_group_phich,
						gain_phich_RouA,
						gain_phich_RouB,
						outX_deremap_ctrlch);
					
					// PHICH CSI REDemap
					outlen_deremap = deremap_phich(
						csi_equ,
						num_RB_DL,
						subfrm_idx,
						num_ID_cell,
						num_bstx_ant,
						duration_cfg_phich,
						cp_cfg,
						idx_group_phich,
						1,
						1,
						csi_deremap_ctrlch);
//					if (DUMP_RXTOP) save.Run(outX_deremap_ctrlch, outlen_deremap, "uerx_irx_phich_redemap", TO_SAVE);
//					if (DUMP_RXTOP) save.Run(csi_deremap_ctrlch, outlen_deremap, "uerx_irx_phich_redemap_csi", TO_SAVE);
//				    if (DUMP_RXTOP) save.Run(outlen_deremap, "uerx_irx_phich_redemap_len", TO_SAVE);
					// PHICH decode
					payload_phich = decode_phich(
						outX_deremap_ctrlch,
						csi_deremap_ctrlch,
						outlen_deremap,
						subfrm_idx,
						num_ID_cell,
						cp_cfg,
						idx_seque_phich);

					// just for statistic
					recive_block_phich++;
					if (payload_phich == UERX_NACK)
					{
						right_block_phich++;
					}
				}				

			} // end if (ofdm_idx == num_ofdm_pdcch-1)


			/* PDSCH */
			if (ofdm_idx >= num_ofdm_pdcch)
			{
				// PDSCH REDemap
				for (i=0; i<110; i++)
				{
					Uerx_inf_u.rb_alloc_bitmap[0][i] = 1;
					Uerx_inf_u.rb_alloc_bitmap[1][i] = 1;
				}

				/*uerx_deremap_u.*/deremap_pdsch(
					outY,
					num_uerx_ant,
					num_RB_DL,
					Uerx_inf_u.rb_alloc_bitmap,
					subfrm_idx,
					ofdm_idx,
					num_ID_cell,
					num_bstx_ant,
					cp_cfg,
					gain_pdsch_RouA,
					gain_pdsch_RouB,
					outY_deremap);
//				if (DUMP_RXTOP) save.Run(outY_deremap[0], outlen_deremap, "uerx_irx_pdsch_redemap_ant0", TO_SAVE);
//				if (DUMP_RXTOP) save.Run(outY_deremap[1], outlen_deremap, "uerx_irx_pdsch_redemap_ant1", TO_SAVE);

				// PDSCH CE REDemap
				outlen_deremap = deremap_pdsch(
					outH_ce,
					num_uerx_ant * num_bstx_ant,
					num_RB_DL,
					Uerx_inf_u.rb_alloc_bitmap,
					subfrm_idx,
					ofdm_idx,
					num_ID_cell,
					num_bstx_ant,
					cp_cfg,
					1,
					1,
					outH_deremap);

//				if (DUMP_RXTOP) save.Run(outH_deremap[0], outlen_deremap, "uerx_irx_pdsch_redemap_ce_H0", TO_SAVE);
//				if (DUMP_RXTOP) save.Run(outH_deremap[1], outlen_deremap, "uerx_irx_pdsch_redemap_ce_H1", TO_SAVE);
//				if (DUMP_RXTOP) save.Run(outH_deremap[2], outlen_deremap, "uerx_irx_pdsch_redemap_ce_H2", TO_SAVE);
//				if (DUMP_RXTOP) save.Run(outH_deremap[3], outlen_deremap, "uerx_irx_pdsch_redemap_ce_H3", TO_SAVE);
//				if (DUMP_RXTOP) save.Run(outlen_deremap, "uerx_irx_pdsch_redemap_len", TO_SAVE);


				// PDSCH EQU
				outlen_equ = equ_pdsch(
					outY_deremap,
					outH_deremap,
					outlen_deremap,
					nosieVar,
					num_bstx_ant,
					num_uerx_ant,
					diver_spatial,
					layers_total,
					codebook_idx,
					outX_equ,
					csi_equ,
					ns_equ);
//				if (DUMP_RXTOP) save.Run(outX_equ[0], outlen_equ, "uerx_irx_pdsch_equ_codeword0", TO_SAVE);
//				if (DUMP_RXTOP) save.Run(outX_equ[1], outlen_equ, "uerx_irx_pdsch_equ_codeword1", TO_SAVE);
//				if (DUMP_RXTOP) save.Run(csi_equ[0], outlen_equ, "uerx_irx_pdsch_equ_csi_codeword0", TO_SAVE);
//				if (DUMP_RXTOP) save.Run(csi_equ[1], outlen_equ, "uerx_irx_pdsch_equ_csi_codeword1", TO_SAVE);
//				if (DUMP_RXTOP) save.Run(ns_equ[0], outlen_equ, "uerx_irx_pdsch_equ_noise_codeword0", TO_SAVE);
//				if (DUMP_RXTOP) save.Run(ns_equ[1], outlen_equ, "uerx_irx_pdsch_equ_noise_codeword1", TO_SAVE);
//				if (DUMP_RXTOP) save.Run(len, "uerx_irx_pdsch_equ_len", TO_SAVE);

				// one codeword
				if ((num_codeword==1 && layers_total==1) || diver_spatial==UERX_DIVERSITY)
				{
					// PDSCH DeQAM
					outlen_deqam = deqam(
						outX_equ[0],
						csi_equ[0],
						nosieVar,
						outlen_equ,
						qm_cw[0],
						outS_deqam[0]);

//					if (DUMP_RXTOP) save.Run(outS_deqam[0], outlen_deqam, "uerx_irx_pdsch_deqam_codeword0", TO_SAVE);
//					if (DUMP_RXTOP) save.Run(outlen_deqam, "uerx_irx_pdsch_deqam_codeword0_len", TO_SAVE);

					// PDSCH DeScramble
					outlen_descram = descram_pdsch(
						outS_deqam[0],
						outlen_deqam,
						reset_descram_pdsch,
						num_ID_cell,
						subfrm_idx,
						0,
						num_rnti,
						&outS_descram_cw[0][totallen_descram_cw[0]]);

//					if (DUMP_RXTOP) save.Run(&outS_descram_cw[0][totallen_descram_cw[0]], outlen_descram, "uerx_orx_pdsch_descramble_codeword0", TO_SAVE);
//					if (DUMP_RXTOP) save.Run(outlen_descram, "uerx_orx_pdsch_descramble_codeword0_len", TO_SAVE);
					
					reset_descram_pdsch = 0;

					totallen_descram_cw[0] += outlen_descram;
				}
				// two codewords
				else if (num_codeword==2 && layers_total==2)
				{
					// PDSCH DeQAM
					outlen_deqam_cw[0] = deqam_clsm2layer(
						outX_equ[0],
						csi_equ[0],
						ns_equ[0],
						outlen_equ,
						qm_cw[0],
						outS_deqam[0]);

//					if (DUMP_RXTOP) save.Run(outS_deqam[0], outlen_deqam_cw[0], "uerx_irx_pdsch_deqam_codeword0", TO_SAVE);
//					if (DUMP_RXTOP) save.Run(outlen_deqam_cw[0], "uerx_irx_pdsch_deqam_codeword0_len", TO_SAVE);

					outlen_deqam_cw[1] = deqam_clsm2layer(
						outX_equ[1],
						csi_equ[1],
						ns_equ[1],
						outlen_equ,
						qm_cw[1],
						outS_deqam[1]);

//					if (DUMP_RXTOP) save.Run(outS_deqam[1], outlen_deqam_cw[1], "uerx_irx_pdsch_deqam_codeword1", TO_SAVE);
//					if (DUMP_RXTOP) save.Run(outlen_deqam_cw[1], "uerx_irx_pdsch_deqam_codeword1_len", TO_SAVE);

					// PDSCH DeScramble
					outlen_descram_cw[0] = descram_pdsch(
						outS_deqam[0],
						outlen_deqam_cw[0],
						reset_descram_pdsch,
						num_ID_cell,
						subfrm_idx,
						0,
						num_rnti,
						&outS_descram_cw[0][totallen_descram_cw[0]]);

//					if (DUMP_RXTOP) save.Run(&outS_descram_cw[0][totallen_descram_cw[0]], outlen_descram_cw[0], "uerx_orx_pdsch_descramble_codeword0", TO_SAVE);
//					if (DUMP_RXTOP) save.Run(outlen_descram_cw[0], "uerx_orx_pdsch_descramble_codeword0_len", TO_SAVE);
					
					outlen_descram_cw[1] = descram_pdsch(
						outS_deqam[1],
						outlen_deqam_cw[1],
						reset_descram_pdsch,
						num_ID_cell,
						subfrm_idx,
						1,
						num_rnti,
						&outS_descram_cw[1][totallen_descram_cw[1]]);

//					if (DUMP_RXTOP) save.Run(&outS_descram_cw[1][totallen_descram_cw[1]], outlen_descram_cw[1], "uerx_orx_pdsch_descramble_codeword1", TO_SAVE);
//					if (DUMP_RXTOP) save.Run(outlen_descram_cw[1], "uerx_orx_pdsch_descramble_codeword1_len", TO_SAVE);

					reset_descram_pdsch = 0;

					totallen_descram_cw[0] += outlen_descram_cw[0];
					totallen_descram_cw[1] += outlen_descram_cw[1];
				}
				else
				{
					assert(0);
				}

			} // end if (ofdm_idx >= num_ofdm_pdcch)


			/* PDSCH */
			if (ofdm_idx == 13)
			{

				for (c=0; c<num_codeword; c++)
				{					
					// PDSCH Decode
					rv_idx = 0;
					k_mimo = (trans_mode == UERX_TM_MODE3 || trans_mode == UERX_TM_MODE4)? 2: 1;
					new_data_indicat = 1;					
					harqprocess_idx = 1;
					repet_times = 0;
					codeword_idx = 0;

					crc_check_pdsch = /*uerx_phychdec_top_u.*/decode_pdsch(
						outS_descram_cw[c],
						totallen_descram_cw[c],
						tbs_cw[c],
						catgry_ue,
						num_harq_process,
						qm_cw[c],
						rv_idx,
						num_layer_spatial_cw[c],
						k_mimo,
						new_data_indicat,
						harqprocess_idx,
						repet_times,
						codeword_idx,
						outBit_dec);

					// just for statistic
					recive_block_pdsch++;
					if (crc_check_pdsch == 0)
					{
						right_block_pdsch++;
					}
					else
					{
					}
					
				}
			}


			/* PBCH */
			if (subfrm_idx==0 && ofdm_idx>=7 && ofdm_idx<=10)
			{
				// PBCH REDemap
				deremap_pbch(
					outY,
					num_uerx_ant,
					num_RB_DL,
					subfrm_idx,
					ofdm_idx,
					num_ID_cell,
					num_bstx_ant,
					cp_cfg,
					gain_pbch_RouA,
					gain_pbch_RouB,
					outY_deremap);

				// PBCH CE REDemap
				outlen_deremap = deremap_pbch(
					outH_ce,
					num_uerx_ant * num_bstx_ant,
					num_RB_DL,
					subfrm_idx,
					ofdm_idx,
					num_ID_cell,
					num_bstx_ant,
					cp_cfg,
					1,
					1,
					outH_deremap);

				// EQU for PBCH
				outlen_equ = equ_ctrch(
					outY_deremap,
					outH_deremap,
					outlen_deremap,
					nosieVar,
					num_bstx_ant,
					num_uerx_ant,
					outX_equ[0],
					csi_equ[0]);
				

				// deQAM for PBCH
				outlen_deqam = deqam(outX_equ[0], csi_equ[0], nosieVar, outlen_equ, UERX_QPSK_BITS, outS_deqam[0]);
				

				// descramb for PBCH
				outlen_descram = descram_pbch(outS_deqam[0], outlen_deqam, reset_descram_pbch
					, num_ID_cell, cp_cfg, frm_idx, &outS_descram_pbch[totallen_descram_pbch]);


				reset_descram_pbch = 0;
				
				totallen_descram_pbch += outlen_descram;
			}


			/* PBCH */
			if (subfrm_idx==0 && ofdm_idx==10)
			{				
				// decode for PBCH
				crc_check_pbch = decode_pbch(outS_descram_pbch, totallen_descram_pbch, 24
					, num_bstx_ant, outBit_dec);

				// just for statistic
				recive_block_pbch++;
				if (crc_check_pbch == 0)
				{
					right_block_pbch++;
				}

			}
		} // if (ret_deframe && isDL(subfrm_idx, uldl_switch_cfg))
	} // end for(ofdm_idx=0; ofdm_idx<14; ofdm_idx++)

	return 0;
}
