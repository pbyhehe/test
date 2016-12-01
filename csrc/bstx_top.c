#include "foo.h"
#include "bstx_define.h"
#include "bstx_top.h"
#include "bstx_sharefunc.h"

extern int fft_Init();

extern int ifft_Init();

extern int fft_Clear();

extern int ifft_Clear();

#define DUMP_TXTOP 1


int Init2()
{
	fft_Init();
	ifft_Init();
	return 0;
}

int Clear2()
{	
	fft_Clear();
	ifft_Clear();
	return 0;
}


int InputOutput3(int subfrm_idx, int frm_idx, cfloat **outbuf)
{
	int i, j, p, c;
	int ue_id;
	int num_bstx_ant;
	int num_rb_dl;
	int num_rb_ul;
	int N_fft; // 141128 linjiangnan
	int uldl_switch_cfg;
	int spec_subfrm_cfg;
	int len_subfrm;
	int num_phich;
	int mi_phich;
	int num_grp_phich;
	int fdd_tdd_mode;
	int duration_phich_cfg;
	int num_ofdm_pdcch;
	int num_id_cell;
	int cp_cfg;	
	int num_harq_process;
	int ue_category;
	int num_codeword;
	int transmit_mode;
	int k_mimo;
	int rv;
	int qm;
	int num_nullbit;
	int cdd_cfg;
	int num_layer_total;
	int num_layer_spatial_cw;
	int transmit_scheme;
	int codebook_idx;	
	int num_pdcch;
	int num_rnti;
	int num_RE;
	int ue_port;
	int dci_format;
	int pdcch_format;
	int ue_txant_selec;
	
	int payload_len;
	int outlen_crcattach;
	int outlen_encode;
	int outlen_concate;
	int outlen_multiplex_pdcch;
	int outlen_intlv_pdcch;
	int size_each_blockseg[13];
	int num_codeseg;
	int outlen_ratematch;
	int size_each_ratematchseg[13];
	int outlen_blockconcate;
	int outlen_scramble;
	int outlen_qammap;
	int outlen_qam_cw[2];
	int outlen_spread;
	int outlen_grpallign;
	int outlen_layer;
	int outlen_precod;
	int outlen_unit_map; // 141219 linjiangnan
	int outlen_ofdm;

	int16 *rb_alloc_bitmap[2];

	static int idx_group_phich_list[56];
	static int idx_seque_phich_list[56];
	static int pdcch_format_list[110];

	float gain_pss_RA;
	float gain_sss_RA;
	float gain_pbch_RA;
	float gain_pbch_RB;
	float gain_phich_RA;
	float gain_phich_RB;
	float gain_pdcch_RA;
	float gain_pdcch_RB;
	float gain_pdsch_RA;
	float gain_pdsch_RB;
	float gain_pcfich_RB;

	static cfloat ref_sequence[220*6];
	static cfloat sss_sequence[72];
	static cfloat pss_sequence[72];
	static int16 payload_pcfich;
	static int16 payload_phich;
	static int16 payload_pbch[24];
	static int16 payload_pdcch[100];
	static int16 payload_pdsch[75376];

	static int16 outbuf_crcattach[76000];
	static int16 outbuf_encode_pcfich[32];
	static int16 outbuf_encode_phich[3];

	static int16 *outbuf_blockseg[13], outbuf_blockseg_mem[13][7000];
	static int16 *outbuf0_encode[13], outbuf0_encode_mem[13][7000];
	static int16 *outbuf1_encode[13], outbuf1_encode_mem[13][7000];
	static int16 *outbuf2_encode[13], outbuf2_encode_mem[13][7000];
	static int16 *outbuf_ratematch[13], outbuf_ratematch_mem[13][21000];
	for(i=0; i<13; i++)
	{
		outbuf_blockseg[i]	= outbuf_blockseg_mem[i];
		outbuf0_encode[i]	= outbuf0_encode_mem[i];
		outbuf1_encode[i]	= outbuf1_encode_mem[i];
		outbuf2_encode[i]	= outbuf2_encode_mem[i];
		outbuf_ratematch[i]	= outbuf_ratematch_mem[i];
	}

	static int16 *outbuf_ratematch_pdcch[100], outbuf_ratematch_pdcch_mem[100][8*72];
	for(i=0; i<100; i++)
	{
		outbuf_ratematch_pdcch[i] = outbuf_ratematch_pdcch_mem[i];
	}
	
	static int16 outbuf_concate[6*14*1200];

	static int16 outbuf_scramble[6*14*1200];
	
	static cfloat *outbuf_qam_cw[2], outbuf_qammap_cw_mem[2][14*1200];
	for(i=0; i<2; i++)
	{
		outbuf_qam_cw[i]   = outbuf_qammap_cw_mem[i];
	} 

	static cfloat outbuf_spread_phich[12*56*8];
	static cfloat outbuf_scramble_phich[12*56*8];
	static cfloat outbuf_grpallign_phich[12*56*8];	

	static cfloat *outbuf_layer[4], outbuf_layer_mem[4][14*1200];
	static cfloat *outbuf_precode[4], outbuf_precode_mem[4][14*1200];
	static cfloat *outbuf_precode_pbch[4], outbuf_precode_pbch_mem[4][960];
	static cfloat *outbuf_precode_phich[4], outbuf_precode_phich_mem[4][12*56*8];
	static cfloat *outbuf_mapunit_phich[4], outbuf_mapunit_phich_mem[4][12*56*8];
	static cfloat *outbuf_remap[4], outbuf_remap_mem[4][1320*14];
	for(i=0; i<4; i++)
	{
		outbuf_layer[i]			= outbuf_layer_mem[i];
		outbuf_precode[i]		= outbuf_precode_mem[i];
		outbuf_precode_pbch[i]	= outbuf_precode_pbch_mem[i];
		outbuf_precode_phich[i]	= outbuf_precode_phich_mem[i];
		outbuf_mapunit_phich[i]	= outbuf_mapunit_phich_mem[i];
		outbuf_remap[i]			= outbuf_remap_mem[i];
	}

	static int16 outbuf_multiplex_pdcch[1200*3*2];
	static cfloat outbuf_intlv_pdcch[1200*3];
	
// 	static bstx_inf bstx_inf_u;
	bstx_inf bstx_inf_u;

	bstx_ctrl_pdsch *ptr_ctrl_pdsch;

	// function body
	/*Bstx_phyprc_u.*/init_sys_parameter(&bstx_inf_u, subfrm_idx);

	num_bstx_ant		= bstx_inf_u.num_bstx_ant;
	num_rb_dl			= bstx_inf_u.num_rb_dl;
	num_rb_ul			= bstx_inf_u.num_rb_ul;
	N_fft				= bstx_inf_u.N_fft; // 141128 linjiangnan
	uldl_switch_cfg		= bstx_inf_u.uldl_switch_cfg;
	spec_subfrm_cfg		= bstx_inf_u.spec_subfrm_cfg;
	num_phich			= bstx_inf_u.num_phich;
	mi_phich			= bstx_inf_u.mi_phich;	
	num_grp_phich		= bstx_inf_u.num_group_phich;
	fdd_tdd_mode		= bstx_inf_u.fdd_tdd_mode;
	duration_phich_cfg	= bstx_inf_u.duration_phich_cfg;
	num_ofdm_pdcch		= bstx_inf_u.num_ofdm_pdcch;
	num_id_cell			= bstx_inf_u.num_id_cell;
	cp_cfg				= bstx_inf_u.cp_cfg;
	num_pdcch			= bstx_inf_u.num_pdcch;
	num_harq_process	= bstx_inf_u.num_harq_process;
	
	gain_pss_RA    = pow(10, bstx_inf_u.pss_RA/20);
	gain_sss_RA    = pow(10, bstx_inf_u.sss_RA/20);
	gain_pbch_RA   = pow(10, bstx_inf_u.pbch_RA/20);
	gain_pbch_RB   = pow(10, bstx_inf_u.pbch_RB/20);
	gain_phich_RA  = pow(10, bstx_inf_u.phich_RA/20);
	gain_phich_RB  = pow(10, bstx_inf_u.phich_RB/20);
	gain_pdcch_RA  = pow(10, bstx_inf_u.pdcch_RA/20);
	gain_pdcch_RB  = pow(10, bstx_inf_u.pdcch_RB/20);
	gain_pdsch_RA  = pow(10, bstx_inf_u.pdsch_RA/20);
	gain_pdsch_RB  = pow(10, bstx_inf_u.pdsch_RB/20);
	gain_pcfich_RB = pow(10, bstx_inf_u.pcfich_RB/20);
	
	// the length of one subframe
	len_subfrm = N_fft * 15; // 141128 linjiangnan
	
	if(/*bstx_sharefunc_u.*/bstx_isUL(subfrm_idx, uldl_switch_cfg))
	{
		for (p=0; p<num_bstx_ant; p++)
		{
			for (i=0; i<len_subfrm; i++)
			{
				outbuf[p][i] = ZEROCFLOAT;
			}		
		}

		return len_subfrm;
	}
	
	// Clear the resource elements map buffer of each subframe 
	for(p=0; p<num_bstx_ant; p++)
	{
		for (i=0; i<14*1320; i++)
		{
			outbuf_remap[p][i] = ZEROCFLOAT;
		} 
	}
	

	// PSS
	if(subfrm_idx%5 == 1) 
	{
		/*Bstx_payload_u.*/sequence_pss(
			num_id_cell,
			pss_sequence);

		//if (DUMP_TXTOP) save.Run(pss_sequence, 62, "bstx_itx_seq_pss_old", TO_SAVE);

		/*Bstx_remap_u.*/remap_pss(
			pss_sequence,
			62,
			subfrm_idx,
			num_rb_dl,
			gain_pss_RA,
			outbuf_remap);
	}


	// SSS
	if(subfrm_idx%5 == 0) 
	{
		/*Bstx_payload_u.*/sequence_sss(
			num_id_cell,
			subfrm_idx,
			sss_sequence);

		//if (DUMP_TXTOP) save.Run(sss_sequence, 62, "bstx_itx_seq_sss_old", TO_SAVE);

		/*Bstx_remap_u.*/remap_sss(
			sss_sequence,
			62,
			subfrm_idx,
			num_rb_dl,
			cp_cfg,
			gain_sss_RA,
			outbuf_remap);
	}


	// RS
	/*Bstx_payload_u.*/sequence_rs(
		num_id_cell,
		cp_cfg,
		subfrm_idx,
		ref_sequence);

	//if (DUMP_TXTOP)	save.Run(ref_sequence, 12*110, "bstx_itx_seq_crs_old", TO_SAVE); // zuohuanhuan

	/*Bstx_remap_u.*/remap_rs(
		ref_sequence,
		num_rb_dl,
		subfrm_idx,
		num_bstx_ant,
		num_id_cell,
		cp_cfg,
		spec_subfrm_cfg,
		uldl_switch_cfg,
		outbuf_remap);


	// PBCH 
	if (subfrm_idx == 0)
	{
		if(frm_idx%4 == 0)
		{
			/* OTX */
			/*Bstx_payload_u.*/input_pbch(
				payload_pbch);

			//if (DUMP_TXTOP) save.Run(payload_pbch, 24, "bstx_otx_pbch_payload_old",TO_SAVE);

			outlen_crcattach = /*Bstx_crcattach_u.*/crcattach_pbch(
				payload_pbch,
				24,
				num_bstx_ant,
				outbuf_crcattach);
			
			//if (DUMP_TXTOP) save.Run(outbuf_crcattach, outlen_crcattach, "bstx_otx_pbch_crcattah_old",TO_SAVE);
			
			outlen_encode = /*Bstx_encode_u.*/encode_pbch(
				outbuf_crcattach,
				outlen_crcattach,
				outbuf0_encode[0],
				outbuf1_encode[0],
				outbuf2_encode[0]);

//			if (DUMP_TXTOP) save.Run(outbuf0_encode[0], outlen_encode, "bstx_otx_pbch_encode_stream_0_old",TO_SAVE);
//			if (DUMP_TXTOP) save.Run(outbuf1_encode[0], outlen_encode, "bstx_otx_pbch_encode_stream_1_old",TO_SAVE);
//			if (DUMP_TXTOP) save.Run(outbuf2_encode[0], outlen_encode, "bstx_otx_pbch_encode_stream_2_old",TO_SAVE);
			
			outlen_ratematch = /*Bstx_ratematch_u.*/ratematch_pbch(
				outbuf0_encode[0],
				outbuf1_encode[0],
				outbuf2_encode[0],
				outlen_encode,
				cp_cfg,
				outbuf_ratematch[0]);

			//if (DUMP_TXTOP) save.Run(outbuf_ratematch[0], outlen_ratematch, "bstx_otx_pbch_ratematch_old",TO_SAVE);
			
			outlen_scramble = /*Bstx_scramble_u.*/scramble_pbch(
				outbuf_ratematch[0],
				outlen_ratematch,
				num_id_cell,
				outbuf_scramble);

			//if (DUMP_TXTOP) save.Run(outbuf_scramble, outlen_scramble, "bstx_otx_pbch_scrambe_old",TO_SAVE);
			
			outlen_qammap = /*Bstx_qammap_u.*/qammap_pbch(
				outbuf_scramble,
				outlen_scramble,
				outbuf_qam_cw[0]);

			//if (DUMP_TXTOP) save.Run(outbuf_qam_cw[0], outlen_qammap, "bstx_otx_pbch_qammap_old",TO_SAVE);

			/* ITX */
			outlen_layer = /*Bstx_layermap_u.*/layer_pbch(
				outbuf_qam_cw[0],
				outlen_qammap,
				num_bstx_ant,
				outbuf_layer);

//			if (DUMP_TXTOP) save.Run(outbuf_layer[0], 240, "bstx_itx_pbch_layermap_ant0_old",TO_SAVE);
//			if (DUMP_TXTOP) save.Run(outbuf_layer[1], 240, "bstx_itx_pbch_layermap_ant1_old",TO_SAVE);

			/*Bstx_precode_u.*/precode_pbch(
				outbuf_layer,
				outlen_layer,
				outlen_qammap,
				num_bstx_ant,
				outbuf_precode_pbch);

//			if (DUMP_TXTOP) save.Run(outbuf_precode_pbch[0], 240, "bstx_itx_pbch_precode_ant0_old",TO_SAVE);
//			if (DUMP_TXTOP) save.Run(outbuf_precode_pbch[1], 240, "bstx_itx_pbch_precode_ant1_old",TO_SAVE);
		}

		for (p=0; p<num_bstx_ant; p++)
		{
			for (i=0; i<240; i++)
			{					
				outbuf_precode[p][i] = outbuf_precode_pbch[p][(frm_idx%4)*240+i];
			}
		}
// 		if (DUMP_TXTOP) save.Run(outbuf_precode[0], 240, "precod_pbch_outbuf_ant0_old",TO_SAVE);
// 		if (DUMP_TXTOP) save.Run(outbuf_precode[1], 240, "precod_pbch_outbuf_ant1_old",TO_SAVE);

		/*Bstx_remap_u.*/remap_pbch(
			outbuf_precode,
			num_rb_dl,
			subfrm_idx,
			num_id_cell,
			cp_cfg,
			num_bstx_ant,
			gain_pbch_RA,
			gain_pbch_RB,
			outbuf_remap);

	} 	


	// PCFICH
	payload_pcfich = num_ofdm_pdcch;

	//if (DUMP_TXTOP)	save.Run(&payload_pcfich, 1, "bstx_otx_pcfich_payload_old", TO_SAVE);

	outlen_encode = /*Bstx_encode_u.*/encode_pcfich(
		payload_pcfich,
		num_rb_dl,
		outbuf_encode_pcfich);

	//if (DUMP_TXTOP)	save.Run(outbuf_encode_pcfich, outlen_encode, "bstx_otx_pcfich_encode_old", TO_SAVE);

	outlen_scramble = /*Bstx_scramble_u.*/scramble_pcfich(
		outbuf_encode_pcfich,
		outlen_encode,
		num_id_cell,
		subfrm_idx,
		outbuf_scramble);

	//if (DUMP_TXTOP)	save.Run(outbuf_scramble, outlen_scramble, "bstx_itx_pcfich_scramble_old", TO_SAVE);
	
	outlen_qammap = /*Bstx_qammap_u.*/qammap_pcfich(
		outbuf_scramble,
		outlen_scramble,
		outbuf_qam_cw[0]);

	//if (DUMP_TXTOP) save.Run(outbuf_qam_cw[0], outlen_qammap, "bstx_itx_pcfich_qammap_old", TO_SAVE);
	
	/* ITX */
	outlen_layer = /*Bstx_layermap_u.*/layer_pcfich(
		outbuf_qam_cw[0],
		outlen_qammap,
		num_bstx_ant,
		outbuf_layer);

//	if (DUMP_TXTOP)	save.Run(outbuf_layer[0], outlen_layer, "bstx_itx_pcfich_layermap_ant0_old", TO_SAVE);
//	if (DUMP_TXTOP)	save.Run(outbuf_layer[1], outlen_layer, "bstx_itx_pcfich_layermap_ant1_old", TO_SAVE);

	outlen_precod = /*Bstx_precode_u.*/precode_pcfich(
		outbuf_layer,
		outlen_layer,
		num_bstx_ant,
		outbuf_precode);

//	if (DUMP_TXTOP) save.Run(outbuf_precode[0], outlen_precod, "bstx_itx_pcfich_precode_ant0_old", TO_SAVE);
//	if (DUMP_TXTOP) save.Run(outbuf_precode[1], outlen_precod, "bstx_itx_pcfich_precode_ant1_old", TO_SAVE);

	/*Bstx_remap_u.*/remap_pcfich(
		outbuf_precode,
		16,
		num_id_cell,
		num_bstx_ant,
		num_rb_dl,
		gain_pcfich_RB,
		outbuf_remap);

//	if (DUMP_TXTOP) save.Run(outbuf_remap[0], 14*1320, "bstx_itx_pcfich_remap_ant0_old", TO_SAVE);
//		if (DUMP_TXTOP) save.Run(outbuf_remap[1], 14*1320, "bstx_itx_pcfich_remap_ant1_old", TO_SAVE);

	// PHICH
	if (mi_phich != 0)  
	{
		for (i=0; i<num_phich; i++) // each PHICH -> list number
		{
			idx_group_phich_list[i] = bstx_inf_u.ctrl_arr_phich[i].idx_group_phich;
			idx_seque_phich_list[i] = bstx_inf_u.ctrl_arr_phich[i].idx_seque_phich;
		}

		for (i=0; i<bstx_inf_u.num_phich; i++)
		{
			// PHICH Payload
			payload_phich = BSTX_NACK;

			//if (DUMP_TXTOP)	save.Run(&payload_phich, 1, "bstx_itx_phich_payload_old", TO_SAVE);

			// PHICH Encode
			/*Bstx_encode_u.*/encode_phich(
				payload_phich,
				outbuf_encode_phich);

			//if (DUMP_TXTOP)	save.Run(outbuf_encode_phich, 3, "bstx_itx_phich_encode_old", TO_SAVE);

			// PHICH QAM Map
			outlen_qammap = /*Bstx_qammap_u.*/qammap_phich(
				outbuf_encode_phich,
				3,
				outbuf_qam_cw[0]);

			//if (DUMP_TXTOP)	save.Run(outbuf_qam_cw[0], outlen_qammap, "bstx_itx_phich_qammap_old", TO_SAVE);

			// PHICH Spread
			outlen_spread = /*Bstx_mod_phich_u.*/spread_phich(
				outbuf_qam_cw[0],
				3,
				cp_cfg,
				idx_seque_phich_list[i],
				outbuf_spread_phich);

			//if (DUMP_TXTOP)	save.Run(outbuf_spread_phich, outlen_spread, "bstx_itx_phich_spread_old", TO_SAVE);

			// PHICH Scramble
			outlen_scramble = /*Bstx_scramble_u.*/scramble_phich(
				outbuf_spread_phich,
				outlen_spread,
				subfrm_idx,
				num_id_cell,
				cp_cfg,
				outbuf_scramble_phich);

			//if (DUMP_TXTOP)	save.Run(outbuf_scramble_phich, outlen_scramble, "bstx_itx_phich_scramble_old", TO_SAVE);

			// PHICH Group Align
			outlen_grpallign = /*Bstx_mod_phich_u.*/grpallign_phich(
				outbuf_scramble_phich,
				outlen_scramble,
				cp_cfg,
				idx_group_phich_list[i],
				outbuf_grpallign_phich);

			//if (DUMP_TXTOP)	save.Run(outbuf_grpallign_phich, outlen_grpallign, "bstx_itx_phich_group_align_old", TO_SAVE);

			// PHICH Layer Map
			outlen_layer = /*Bstx_layermap_u.*/layer_phich(
				outbuf_grpallign_phich,
				outlen_grpallign,
				num_bstx_ant,
				outbuf_layer);

//			if (DUMP_TXTOP)	save.Run(outbuf_layer[0], outlen_layer, "bstx_itx_phich_layermap_ant0_old", TO_SAVE);
//			if (DUMP_TXTOP)	save.Run(outbuf_layer[1], outlen_layer, "bstx_itx_phich_layermap_ant1_old", TO_SAVE);
			
			// PHICH Precode
			outlen_precod = /*Bstx_precode_u.*/precode_phich(
				outbuf_layer,
				outlen_layer,
				num_bstx_ant,
				idx_group_phich_list[i],
				cp_cfg,
				outbuf_precode);

//			if (DUMP_TXTOP)	save.Run(outbuf_precode[0], outlen_precod, "bstx_itx_phich_precode_ant0_old", TO_SAVE);
//			if (DUMP_TXTOP)	save.Run(outbuf_precode[1], outlen_precod, "bstx_itx_phich_precode_ant1_old", TO_SAVE);

			// PHICH User Cascade
			for(p=0; p<num_bstx_ant; p++)
			{
				for(j=0; j<12; j++)
				{
					outbuf_precode_phich[p][12*i+j] = outbuf_precode[p][j];
				}
			}

		}
//		if (DUMP_TXTOP)	save.Run(outbuf_precode_phich[0],12*num_phich, "bstx_itx_phich_cascade_ant0_old", TO_SAVE);
//		if (DUMP_TXTOP)	save.Run(outbuf_precode_phich[1],12*num_phich, "bstx_itx_phich_cascade_ant1_old", TO_SAVE);

		// PHICH Unit Map
		outlen_unit_map = /*Bstx_mod_phich_u.*/mapunit_phich(
			outbuf_precode_phich,
			12*num_phich,
			num_bstx_ant,
			cp_cfg,
			num_grp_phich,
			mi_phich,
			fdd_tdd_mode,
			idx_group_phich_list,
			outbuf_mapunit_phich);

//	   if (DUMP_TXTOP)	save.Run(outbuf_mapunit_phich[0], outlen_unit_map, "bstx_itx_phich_unitmap_ant0_old", TO_SAVE);
//	   if (DUMP_TXTOP)	save.Run(outbuf_mapunit_phich[1], outlen_unit_map, "bstx_itx_phich_unitmap_ant1_old", TO_SAVE);

	   // PHICH RE Map
	   /*Bstx_remap_u.*/remap_phich(
		   outbuf_mapunit_phich,
		   num_rb_dl,
		   num_grp_phich,
		   mi_phich,
		   fdd_tdd_mode,
		   subfrm_idx,
		   num_id_cell,
		   num_bstx_ant,
		   cp_cfg,
		   duration_phich_cfg,
		   gain_phich_RA,
		   gain_phich_RB,
		   outbuf_remap);
//	   if (DUMP_TXTOP)	save.Run(outbuf_remap[0], 14*1320, "bstx_itx_phich_remap_ant0_old", TO_SAVE);
//	 	   if (DUMP_TXTOP)	save.Run(outbuf_remap[1], 14*1320, "bstx_itx_phich_remap_ant1_old", TO_SAVE);

	}


	// PDCCH
	/* OTX */
 	for(i=0; i<bstx_inf_u.num_pdcch; i++)
	{
		num_rnti       = bstx_inf_u.ctrl_arr_pdcch[i].num_rnti;
		ue_port		   = bstx_inf_u.ctrl_arr_pdcch[i].ue_port;
		dci_format	   = bstx_inf_u.ctrl_arr_pdcch[i].dci_format;
		pdcch_format   = bstx_inf_u.ctrl_arr_pdcch[i].pdcch_format;
		ue_txant_selec = bstx_inf_u.ctrl_arr_pdcch[i].ue_txant_selec;
		ptr_ctrl_pdsch = &bstx_inf_u.ctrl_arr_pdsch[i];

		payload_len = /*Bstx_payload_u.*/input_pdcch(
			dci_format,
			num_rb_dl,
			num_rb_ul,
			num_bstx_ant,
			uldl_switch_cfg,
			ptr_ctrl_pdsch,
			payload_pdcch);

		//if (DUMP_TXTOP)	save.Run(payload_pdcch, payload_len, "bstx_otx_pdcch_payload_old", TO_SAVE);

		outlen_crcattach = /*Bstx_crcattach_u.*/crcattach_pdcch(
			payload_pdcch,
			payload_len,
			ue_port,
			ue_txant_selec,
			num_rnti,
			outbuf_crcattach);

		//if (DUMP_TXTOP)	save.Run(outbuf_crcattach, outlen_crcattach, "bstx_otx_pdcch_crcattach_old", TO_SAVE);

		outlen_encode = /*Bstx_encode_u.*/encode_pdcch(
			outbuf_crcattach,
			outlen_crcattach,
			outbuf0_encode[0],
			outbuf1_encode[0],
			outbuf2_encode[0]);

//		if (DUMP_TXTOP) save.Run(outbuf0_encode[0], outlen_encode, "bstx_otx_pdcch_encode_stream_0_old",TO_SAVE);
//		if (DUMP_TXTOP) save.Run(outbuf1_encode[0], outlen_encode, "bstx_otx_pdcch_encode_stream_1_old",TO_SAVE);
//		if (DUMP_TXTOP) save.Run(outbuf2_encode[0], outlen_encode, "bstx_otx_pdcch_encode_stream_2_old",TO_SAVE);

		outlen_ratematch = /*Bstx_ratematch_u.*/ratematch_pdcch(
			outbuf0_encode[0],
			outbuf1_encode[0],
			outbuf2_encode[0],
			outlen_encode,
			pdcch_format,
			outbuf_ratematch_pdcch[i]);

		//if (DUMP_TXTOP)	save.Run(outbuf_ratematch_pdcch[i], outlen_ratematch, "bstx_otx_pdcch_ratematch_old", TO_SAVE);
	}

	for (i=0; i<bstx_inf_u.num_pdcch; i++)
	{
		pdcch_format_list[i] = bstx_inf_u.ctrl_arr_pdcch[i].pdcch_format;
	}

	outlen_multiplex_pdcch = /*Bstx_mod_pdcch_u.*/multiplex_pdcch(
		outbuf_ratematch_pdcch,
		pdcch_format_list,
		num_pdcch,
		num_grp_phich,
		mi_phich,
		fdd_tdd_mode,
		num_ofdm_pdcch,
		num_rb_dl,
		num_bstx_ant,
		cp_cfg,
		outbuf_multiplex_pdcch);

	//if (DUMP_TXTOP)	save.Run(outbuf_multiplex_pdcch, outlen_multiplex_pdcch, "bstx_otx_pdcch_multiplex_old", TO_SAVE);

	outlen_scramble = /*Bstx_scramble_u.*/scramble_pdcch(
		outbuf_multiplex_pdcch,
		outlen_multiplex_pdcch,
		num_id_cell,
		subfrm_idx,
		outbuf_scramble);

	//if (DUMP_TXTOP)	save.Run(outbuf_scramble, outlen_scramble, "bstx_otx_pdcch_scramble_old", TO_SAVE);

	outlen_qammap = /*Bstx_qammap_u.*/qammap_pdcch(
		outbuf_scramble,
		outlen_scramble,
		outbuf_qam_cw[0]);

	//if (DUMP_TXTOP)	save.Run(outbuf_qam_cw[0], outlen_qammap, "bstx_otx_pdcch_qammap_old", TO_SAVE);

	/* ITX */
 	outlen_intlv_pdcch = /*Bstx_mod_pdcch_u.*/interleaver_pdcch(
		outbuf_qam_cw[0],
		outlen_qammap,
		num_id_cell,
		outbuf_intlv_pdcch);

 	//if (DUMP_TXTOP)	save.Run(outbuf_intlv_pdcch, outlen_intlv_pdcch, "bstx_itx_pdcch_interleave_old", TO_SAVE);

	outlen_layer = /*Bstx_layermap_u.*/layer_pdcch(
		outbuf_intlv_pdcch,
		outlen_intlv_pdcch,
		num_bstx_ant,
		outbuf_layer);

// 	if (DUMP_TXTOP) save.Run(outbuf_layer[0], outlen_layer, "bstx_itx_pdcch_layermap_ant0_old", TO_SAVE);
// 	if (DUMP_TXTOP) save.Run(outbuf_layer[1], outlen_layer, "bstx_itx_pdcch_layermap_ant1_old", TO_SAVE);

	memset(outbuf_precode[0], 0, sizeof(cfloat)*(1200*14)); // zuohuanhuan
	memset(outbuf_precode[1], 0, sizeof(cfloat)*(1200*14)); // zuohuanhuan
	memset(outbuf_precode[2], 0, sizeof(cfloat)*(1200*14)); // zuohuanhuan
	memset(outbuf_precode[3], 0, sizeof(cfloat)*(1200*14)); // zuohuanhuan

	outlen_precod = /*Bstx_precode_u.*/precode_pdcch(
		outbuf_layer,
		outlen_layer,
		outlen_intlv_pdcch,
		num_bstx_ant,
		outbuf_precode);

// 	if (DUMP_TXTOP)	save.Run(outbuf_precode[0], outlen_precod, "bstx_itx_pdcch_precode_ant0_old", TO_SAVE);
// 	if (DUMP_TXTOP)	save.Run(outbuf_precode[1], outlen_precod, "bstx_itx_pdcch_precode_ant1_old", TO_SAVE);

	/*Bstx_remap_u.*/remap_pdcch(
		outbuf_precode,
		num_rb_dl,
		num_grp_phich,
		mi_phich,
		fdd_tdd_mode,
		cp_cfg,
		subfrm_idx,
		num_ofdm_pdcch,
		num_id_cell,
		num_bstx_ant,
		duration_phich_cfg,
		gain_pdcch_RA,
		gain_pdcch_RB,
		outbuf_remap);
//	if (DUMP_TXTOP)	save.Run(outbuf_remap[0], 1320*14, "bstx_itx_pdcch_outbuf_remap_ant0_old", TO_SAVE);
//	if (DUMP_TXTOP)	save.Run(outbuf_remap[1], 1320*14, "bstx_itx_pdcch_outbuf_remap_ant1_old", TO_SAVE);


	// PDSCH
	for(ue_id=0; ue_id<1; ue_id++)
	{	
		ue_category	    = bstx_inf_u.ctrl_arr_pdsch[ue_id].ue_category;
		num_codeword    = bstx_inf_u.ctrl_arr_pdsch[ue_id].num_codeword;
		num_layer_total = bstx_inf_u.ctrl_arr_pdsch[ue_id].num_layer_total;	
		transmit_mode   = bstx_inf_u.ctrl_arr_pdsch[ue_id].transmit_mode;
		transmit_scheme	= bstx_inf_u.ctrl_arr_pdsch[ue_id].transmit_scheme;
		cdd_cfg         = bstx_inf_u.ctrl_arr_pdsch[ue_id].cdd_cfg;	
		num_rnti        = bstx_inf_u.ctrl_arr_pdsch[ue_id].num_rnti;		
		codebook_idx    = bstx_inf_u.ctrl_arr_pdsch[ue_id].codebook_idx;
		rb_alloc_bitmap[0] = bstx_inf_u.ctrl_arr_pdsch[ue_id].rb_alloc_bitmap[0];
		rb_alloc_bitmap[1] = bstx_inf_u.ctrl_arr_pdsch[ue_id].rb_alloc_bitmap[1];
		
		k_mimo = (transmit_mode==BSTX_TM_MODE3 || transmit_mode==BSTX_TM_MODE4) ? 2 : 1;
		
		/* OTX */
		for (c=0; c<num_codeword; c++) //num_codeword
		{	
			rv				     = bstx_inf_u.ctrl_arr_pdsch[ue_id].rv_cw[c];
			qm				     = bstx_inf_u.ctrl_arr_pdsch[ue_id].qm_cw[c];			
			payload_len		     = bstx_inf_u.ctrl_arr_pdsch[ue_id].tbs_cw[c];
			num_layer_spatial_cw = bstx_inf_u.ctrl_arr_pdsch[ue_id].num_layer_spatial_cw[c];  

			num_RE = /*bstx_sharefunc_u.*/get_REnum_pdsch(
				num_rb_dl,
				rb_alloc_bitmap,
				subfrm_idx,
				num_ofdm_pdcch,
				num_bstx_ant,
				cp_cfg,
				uldl_switch_cfg,
				spec_subfrm_cfg);
    
			if (transmit_scheme == BSTX_SPATIAL)
			{
       			outlen_concate = num_RE * qm * num_layer_spatial_cw;
			}
			else
			{
				outlen_concate = num_RE * qm;
			}
			//if (DUMP_TXTOP)	save.Run(&outlen_concate, 1, "G", TO_SAVE);


			/*Bstx_payload_u.*/input_pdsch(
				payload_len,
				payload_pdsch);

			//if (DUMP_TXTOP)	save.Run(payload_pdsch, payload_len, "bstx_otx_pdsch_payload_old", TO_SAVE);


			outlen_crcattach = /*Bstx_crcattach_u.*/crcattach_pdsch(
				payload_pdsch,
				payload_len,
				outbuf_crcattach);

			//if (DUMP_TXTOP)	save.Run(outbuf_crcattach, outlen_crcattach, "bstx_otx_pdsch_crcattach_old", TO_SAVE);


			/*Bstx_blockseg_u.*/blockseg_pdsch(
				outbuf_crcattach,
				outlen_crcattach,
				outbuf_blockseg,
				size_each_blockseg,
				&num_codeseg,
				&num_nullbit);


			if(c==0)
			{

			//if (DUMP_TXTOP)	save.Run(size_each_blockseg, 13, "bstx_otx_pdsch_segment_len_c0_old", TO_SAVE);
			}else if(c==1)
			{

			//if (DUMP_TXTOP)	save.Run(size_each_blockseg, 13, "bstx_otx_pdsch_segment_len_c1_old", TO_SAVE);
			}else{}
//			if (DUMP_TXTOP)	save.Run(outbuf_blockseg[0], size_each_blockseg[0], "bstx_otx_pdsch_segment_cb0_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf_blockseg[1], size_each_blockseg[1], "bstx_otx_pdsch_segment_cb1_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf_blockseg[2], size_each_blockseg[2], "bstx_otx_pdsch_segment_cb2_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf_blockseg[3], size_each_blockseg[3], "bstx_otx_pdsch_segment_cb3_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf_blockseg[4], size_each_blockseg[4], "bstx_otx_pdsch_segment_cb4_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf_blockseg[5], size_each_blockseg[5], "bstx_otx_pdsch_segment_cb5_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf_blockseg[6], size_each_blockseg[6], "bstx_otx_pdsch_segment_cb6_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf_blockseg[7], size_each_blockseg[7], "bstx_otx_pdsch_segment_cb7_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf_blockseg[8], size_each_blockseg[8], "bstx_otx_pdsch_segment_cb8_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf_blockseg[9], size_each_blockseg[9], "bstx_otx_pdsch_segment_cb9_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf_blockseg[10], size_each_blockseg[10], "bstx_otx_pdsch_segment_cb10_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf_blockseg[11], size_each_blockseg[11], "bstx_otx_pdsch_segment_cb11_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf_blockseg[12], size_each_blockseg[12], "bstx_otx_pdsch_segment_cb12_old", TO_SAVE);


			/*Bstx_encode_u.*/encode_pdsch(
				outbuf_blockseg,
				size_each_blockseg,
				num_codeseg,
				num_nullbit,
				outbuf0_encode,
				outbuf1_encode,
				outbuf2_encode);

//			if (DUMP_TXTOP)	save.Run(outbuf0_encode[0], size_each_blockseg[0]+4, "bstx_otx_pdsch_encode_cb0_stream0_old", TO_SAVE);
//			if (DUMP_TXTOP)	save.Run(outbuf1_encode[0], size_each_blockseg[0]+4, "bstx_otx_pdsch_encode_cb0_stream1_old", TO_SAVE);
//			if (DUMP_TXTOP)	save.Run(outbuf2_encode[0], size_each_blockseg[0]+4, "bstx_otx_pdsch_encode_cb0_stream2_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf0_encode[1], size_each_blockseg[1]+4, "bstx_otx_pdsch_encode_cb1_stream0_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf1_encode[1], size_each_blockseg[1]+4, "bstx_otx_pdsch_encode_cb1_stream1_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf2_encode[1], size_each_blockseg[1]+4, "bstx_otx_pdsch_encode_cb1_stream2_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf0_encode[2], size_each_blockseg[2]+4, "bstx_otx_pdsch_encode_cb2_stream0_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf1_encode[2], size_each_blockseg[2]+4, "bstx_otx_pdsch_encode_cb2_stream1_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf2_encode[2], size_each_blockseg[2]+4, "bstx_otx_pdsch_encode_cb2_stream2_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf0_encode[3], size_each_blockseg[3]+4, "bstx_otx_pdsch_encode_cb3_stream0_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf1_encode[3], size_each_blockseg[3]+4, "bstx_otx_pdsch_encode_cb3_stream1_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf2_encode[3], size_each_blockseg[3]+4, "bstx_otx_pdsch_encode_cb3_stream2_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf0_encode[4], size_each_blockseg[4]+4, "bstx_otx_pdsch_encode_cb4_stream0_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf1_encode[4], size_each_blockseg[4]+4, "bstx_otx_pdsch_encode_cb4_stream1_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf2_encode[4], size_each_blockseg[4]+4, "bstx_otx_pdsch_encode_cb4_stream2_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf0_encode[5], size_each_blockseg[5]+4, "bstx_otx_pdsch_encode_cb5_stream0_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf1_encode[5], size_each_blockseg[5]+4, "bstx_otx_pdsch_encode_cb5_stream1_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf2_encode[5], size_each_blockseg[5]+4, "bstx_otx_pdsch_encode_cb5_stream2_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf0_encode[6], size_each_blockseg[6]+4, "bstx_otx_pdsch_encode_cb6_stream0_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf1_encode[6], size_each_blockseg[6]+4, "bstx_otx_pdsch_encode_cb6_stream1_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf2_encode[6], size_each_blockseg[6]+4, "bstx_otx_pdsch_encode_cb6_stream2_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf0_encode[7], size_each_blockseg[7]+4, "bstx_otx_pdsch_encode_cb7_stream0_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf1_encode[7], size_each_blockseg[7]+4, "bstx_otx_pdsch_encode_cb7_stream1_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf2_encode[7], size_each_blockseg[7]+4, "bstx_otx_pdsch_encode_cb7_stream2_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf0_encode[8], size_each_blockseg[8]+4, "bstx_otx_pdsch_encode_cb8_stream0_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf1_encode[8], size_each_blockseg[8]+4, "bstx_otx_pdsch_encode_cb8_stream1_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf2_encode[8], size_each_blockseg[8]+4, "bstx_otx_pdsch_encode_cb8_stream2_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf0_encode[9], size_each_blockseg[9]+4, "bstx_otx_pdsch_encode_cb9_stream0_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf1_encode[9], size_each_blockseg[9]+4, "bstx_otx_pdsch_encode_cb9_stream1_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf2_encode[9], size_each_blockseg[9]+4, "bstx_otx_pdsch_encode_cb9_stream2_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf0_encode[10], size_each_blockseg[10]+4, "bstx_otx_pdsch_encode_cb10_stream0_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf1_encode[10], size_each_blockseg[10]+4, "bstx_otx_pdsch_encode_cb10_stream1_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf2_encode[10], size_each_blockseg[10]+4, "bstx_otx_pdsch_encode_cb10_stream2_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf0_encode[11], size_each_blockseg[11]+4, "bstx_otx_pdsch_encode_cb11_stream0_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf1_encode[11], size_each_blockseg[11]+4, "bstx_otx_pdsch_encode_cb11_stream1_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf2_encode[11], size_each_blockseg[11]+4, "bstx_otx_pdsch_encode_cb11_stream2_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf0_encode[12], size_each_blockseg[12]+4, "bstx_otx_pdsch_encode_cb12_stream0_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf1_encode[12], size_each_blockseg[12]+4, "bstx_otx_pdsch_encode_cb12_stream1_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf2_encode[12], size_each_blockseg[12]+4, "bstx_otx_pdsch_encode_cb12_stream2_old", TO_SAVE);


			/*Bstx_ratematch_u.*/ratematch_pdsch(
				outbuf0_encode,
				outbuf1_encode,
				outbuf2_encode,
				size_each_blockseg,
				num_codeseg,
				rv,
				qm,
				ue_category,
				num_harq_process,
				outlen_concate,
				num_layer_spatial_cw,
				k_mimo,
				outbuf_ratematch,
				size_each_ratematchseg);

//			if (DUMP_TXTOP)	save.Run(outbuf_ratematch[0], size_each_ratematchseg[0], "bstx_otx_pdsch_ratematch_cb0_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf_ratematch[1], size_each_ratematchseg[1], "bstx_otx_pdsch_ratematch_cb1_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf_ratematch[2], size_each_ratematchseg[2], "bstx_otx_pdsch_ratematch_cb2_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf_ratematch[3], size_each_ratematchseg[3], "bstx_otx_pdsch_ratematch_cb3_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf_ratematch[4], size_each_ratematchseg[4], "bstx_otx_pdsch_ratematch_cb4_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf_ratematch[5], size_each_ratematchseg[5], "bstx_otx_pdsch_ratematch_cb5_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf_ratematch[6], size_each_ratematchseg[6], "bstx_otx_pdsch_ratematch_cb6_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf_ratematch[7], size_each_ratematchseg[7], "bstx_otx_pdsch_ratematch_cb7_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf_ratematch[8], size_each_ratematchseg[8], "bstx_otx_pdsch_ratematch_cb8_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf_ratematch[9], size_each_ratematchseg[9], "bstx_otx_pdsch_ratematch_cb9_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf_ratematch[10], size_each_ratematchseg[10], "bstx_otx_pdsch_ratematch_cb10_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf_ratematch[11], size_each_ratematchseg[11], "bstx_otx_pdsch_ratematch_cb11_old", TO_SAVE);
// 			if (DUMP_TXTOP)	save.Run(outbuf_ratematch[12], size_each_ratematchseg[12], "bstx_otx_pdsch_ratematch_cb12_old", TO_SAVE);
			
			
			outlen_blockconcate = /*Bstx_blockconcate_u.*/bstx_blockconcate(
				outbuf_ratematch,
				size_each_ratematchseg,
				num_codeseg,
				outbuf_concate);

			//if (DUMP_TXTOP)	save.Run(outbuf_concate, outlen_blockconcate, "bstx_otx_pdsch_concate_old", TO_SAVE);


			outlen_scramble = /*Bstx_scramble_u.*/scramble_pdsch(
				outbuf_concate,
				outlen_concate,
				num_id_cell,
				c,
				num_rnti,
				subfrm_idx,
				outbuf_scramble);

			//if (DUMP_TXTOP)	save.Run(outbuf_scramble, outlen_scramble, "bstx_otx_pdsch_scramble_old", TO_SAVE);


			outlen_qam_cw[c] = /*Bstx_qammap_u.*/qammap_pdsch(
				outbuf_scramble,
				outlen_scramble,
				qm,
				outbuf_qam_cw[c]);
//			if (DUMP_TXTOP) save.Run(outbuf_qam_cw[0], outlen_qam_cw[0], "bstx_otx_pdsch_qammap_cw0_old", TO_SAVE);
		}
//		if (DUMP_TXTOP) save.Run(outbuf_qam_cw[0], outlen_qam_cw[0], "bstx_otx_pdsch_qammap_cw0_old", TO_SAVE);//yishangqule
// 		if (DUMP_TXTOP) save.Run(outbuf_qam_cw[1], outlen_qam_cw[1], "bstx_otx_pdsch_qammap_cw1_old", TO_SAVE);


		/* ITX */
		outlen_layer = /*Bstx_layermap_u.*/layer_pdsch(
			outbuf_qam_cw,
			outlen_qam_cw,
			transmit_scheme,
			num_layer_total,
			num_codeword,
			outbuf_layer);

//		if (DUMP_TXTOP) save.Run(outbuf_layer[0], outlen_layer, "bstx_itx_pdsch_layermap_ant0_old", TO_SAVE);
//		if (DUMP_TXTOP) save.Run(outbuf_layer[1], outlen_layer, "bstx_itx_pdsch_layermap_ant1_old", TO_SAVE);

		outlen_precod = /*Bstx_precode_u.*/precode_pdsch(
			outbuf_layer,
			outlen_layer,
			outlen_qam_cw[0],
			num_layer_total,
			num_bstx_ant,
			transmit_scheme,
			cdd_cfg,
			codebook_idx,
			outbuf_precode);

//		if (DUMP_TXTOP) save.Run(outbuf_precode[0], outlen_precod, "bstx_itx_pdsch_precode_ant0_old", TO_SAVE);
//		if (DUMP_TXTOP) save.Run(outbuf_precode[1], outlen_precod, "bstx_itx_pdsch_precode_ant1_old", TO_SAVE);

		/*Bstx_remap_u.*/remap_pdsch(
			outbuf_precode,
			num_rb_dl,
			rb_alloc_bitmap,
			subfrm_idx,
			num_ofdm_pdcch,
			num_id_cell,
			num_bstx_ant,
			cp_cfg,
			uldl_switch_cfg,
			spec_subfrm_cfg,
			gain_pdsch_RA,
			gain_pdsch_RB,
			outbuf_remap);
	}

//	if (DUMP_TXTOP) save.Run(outbuf_remap[0], 110*12*14, "bstx_itx_remap_all_ant0_old", TO_SAVE);
//	if (DUMP_TXTOP) save.Run(outbuf_remap[1], 110*12*14, "bstx_itx_remap_all_ant1_old", TO_SAVE);


	// OFDM baseband signal generation

	outlen_ofdm = /*Bstx_ofdm_u.*/ofdm_InputOutput(
		outbuf_remap,
		num_rb_dl,
		N_fft, // 20141128 linjiangnan
		num_bstx_ant,
		cp_cfg,
		outbuf);
	
//	if (DUMP_TXTOP) save.Run(outbuf[0], outlen_ofdm, "bstx_itx_cpadd_ant0_old", TO_SAVE);
//	if (DUMP_TXTOP) save.Run(outbuf[1], outlen_ofdm, "bstx_itx_cpadd_ant1_old", TO_SAVE);


	return outlen_ofdm;
}




