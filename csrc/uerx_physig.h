#pragma once

#include "uerx_sharefunc.h"





	int physig_InputOutput1(int N_ID_cell,cfloat *outbuf);									                 //pss

	int physig_InputOutput2(int N_ID_cell, int16 *outbuf);                                                   //sss

	int physig_InputOutput3(int N_sourceblk, int N_ID_cell, int CP_config, int subfrm_idx, cfloat *ref_sym); //rs

	int GetRsPbch(int16 antenna_num, int N_ID_cell, int CP_config, float belta_ref, cint16 *rs_pbch);

	int GetRefPP_1(int N_ID_cell, int N_TX_Ant, int N_DL_RB, int CP_config, int n_subfrm, int n_ofdmsym, 
		cint16 *refpp[4], int16 *ref_pos);

	int GetRefPP(int N_ID_cell, int N_TX_Ant, int N_fft, int N_RB_DL  , int CP_config, int n_subfrm, int n_ofdmsym, 
		cfloat *refpp[4], int16 *ref_initdelta, int16* data_startpos);



