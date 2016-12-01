#pragma once

#include "typedefine.h"


typedef struct UERX_INF
{
	// init system parameter
	int size_fft;
	int num_subcarrier;
	int cp_cfg;				// cycle prefix: 0:normal CP, 1:extend CP
	int num_uerx_ant;		// the num of receiver antenna
	int num_bstx_ant;		// the num of transmition antenna
	int num_ID_cell;		// the cell ID	
	int uldl_switch_cfg;	// 0:DSUUUDSUUU, 1:DSUUDDSUUD, 2:DSUDDDSUDD, 3:DSUUUDDDDD, 4:DSUUDDDDDDD, 5:DSUDDDDDDD, 6:DSUUUDSUUD		
	int spec_subfrm_cfg;	// 0:3DL, 1:9DL, 2:10DL, 3:11DL, 4:12DL, 5:3DL, 6:9D, 7:10D, 8:11D 
	int ue_RS_cfg;			
	int num_RB_DL;
	int num_RB_UL;
	int fdd_tdd_mode;		// 0:FDD, 1:TDD
	float snr_dB;
	float sampling_rate_MHz; // the sampling rate by MHz, 10MHz:15.36MHz, 20:30.72MHz

	// power assignment
	float pdsch_RA;	 // pdsch_RA = (power_pdsch/power_RS) (dB), which in ofdm without RS
	float pdsch_RB;	 // pdsch_RB = (power_pdsch/power_RS) (dB), which in ofdm with RS
	float pss_RA;	 // pss_RA = (power_pss/power_RS) (dB), which in ofdm without RS     
	float sss_RA;	 // sss_RA = (power_sss/power_RS) (dB), which in ofdm without RS
	float pbch_RA;	 // pbch_RA = (power_pbch/power_RS) (dB), which in ofdm without RS
	float pbch_RB;	 // pbch_RB = (power_pbch/power_RS) (dB), which in ofdm with RS
	float phich_RA;	 // phich_RA = (power_phich/power_RS) (dB), which in ofdm without RS
	float phich_RB;	 // phich_RB = (power_phich/power_RS) (dB), which in ofdm with RS
	float pdcch_RA;	 // pdcch_RA = (power_pdcch/power_RS) (dB), which in ofdm without RS
	float pdcch_RB;	 // pdcch_RB = (power_pdcch/power_RS) (dB), which in ofdm with RS
	float pcfich_RB; // pcfich_RB = (power_pcfich/power_RS) (dB), which in ofdm with RS

	// phich parameters written by mac
	int ng_cfg_phich;
	int duration_cfg_phich;
	int idx_group_phich;
	int idx_seque_phich;

	// pdcch parameters written by mac
	int16 num_rnti_bin[16];
	int num_rnti;	
	int dci_format;
	int pdcch_format;
	int num_ofdm_pdcch;
	int ue_port;
	int uetx_ant_selec_cfg;
	
	// pdsch parameters written by mac
	int16 rb_alloc_bitmap[2][110];		// rb_alloc_bitmap[2][110]
	int rb_start_pdsch;
	int num_alloc_rb;
	int ue_category;	
	int rnti_type;
	int trans_mode;
	int ri_last;
	int pmi_last;
	int num_harq_process;
	// dci 1a for prach ---14.6.9 add
	int Preamble_idx;
	int PRACH_Mask_idx;
}uerx_inf;


#define UERX_INDICATION_PBCH			1
#define UERX_INDICATION_PCFICH			2
#define UERX_INDICATION_PDCCH			3
#define UERX_INDICATION_PHICH			4
#define UERX_INDICATION_PDSCH			5
#define UERX_INDICATION_RS				6
#define UERX_INDICATION_PSS				7
#define UERX_INDICATION_SSS				8
#define UERX_INDICATION_PPCH			9
#define UERX_INDICATION_PMCH			10
#define UERX_INDICATION_NOCHANELL		15


#define UERX_DCI_0					0
#define UERX_DCI_1					1
#define UERX_DCI_1A					2
#define UERX_DCI_1B					3
#define UERX_DCI_1D					4
#define UERX_DCI_2					5
#define UERX_DCI_2A					6
#define UERX_DCI_1C					7
#define UERX_DCI_3					8
#define UERX_DCI_3A					9

#define UERX_NORMAL_CP				0
#define UERX_EXTEND_CP				1

#define UERX_SFBC_PRECOD				0
#define UERX_CLOSLOOP_SM_PRECOD		1
#define UERX_OPENLOOP_SM_PRECOD		2
#define UERX_SFBC_PRECOD_PHICH0		3
#define UERX_SFBC_PRECOD_PHICH1		4
#define UERX_MRC_PRECOD				5
#define UERX_SISO_PRECOD				6
#define UERX_NO_MIMO					7
#define UERX_RS_PRECOD				8

#define UERX_FDD_MUXMODE				0
#define UERX_TDD_MUXMODE				1

#define UERX_SI_RNTI					0
#define UERX_P_RNTI					1
#define UERX_RA_RNTI					2
#define UERX_C_RNTI					3
#define UERX_SPSC_RNTI				4
#define UERX_TC_RNTI					5


#define UERX_TM_MODE1					1
#define UERX_TM_MODE2					2
#define UERX_TM_MODE3					3
#define UERX_TM_MODE4					4
#define UERX_TM_MODE5					5
#define UERX_TM_MODE6					6
#define UERX_TM_MODE7					7

#define UERX_ACK						1
#define UERX_NACK						0


#define UERX_DIVERSITY				0
#define UERX_SPATIAL					1
#define UERX_SISO_PORT0_PRECOD		2
#define UERX_SISO_PORT5_PRECOD		3


#define UERX_RE_ALLOC_TYPE0			0
#define UERX_RE_ALLOC_TYPE1			1
#define UERX_RE_ALLOC_TYPE2			2


#define UERX_LOWERBITS_FIRST			0
#define UERX_HIGHERBITS_FIRST			1

#define UERX_N_SC_RB					12

#define UERX_MAX_NUM_RB_DL			110

#define UERX_BPSK_BITS				1
#define UERX_QPSK_BITS				2
#define UERX_QAM16_BITS				4
#define UERX_QAM64_BITS				6


#define UERX_NC_DEFAULT				1600

#pragma SET_DATA_SECTION("uerx_data")
static int UERX_NUM_HARQ_PROCESS_LIST[7] = { 4, 7, 10, 9, 12, 15, 6}; // find by uplink and downlink switch config


static int16 UERX_TURBO_INTLV_PARAMETER[188][4] = {
		{1,      40,    3,10}, 
		{2,      48,    7,12}, 
		{3,      56,   19,42}, 
		{4,      64,    7,16},
		{5,      72,    7,18}, 
		{6,      80,   11,20}, 
		{7,      88,    5,22}, 
		{8,      96,   11,24},
		{9,     104,    7,26}, 
		{10,    112,    41,84}, 
		{11,    120,   103,90}, 
		{12,    128,    15,32}, 
		{13,    136,     9,34},
		{14,    144,    17,108}, 
		{15,    152,    9,382}, 
		{16,    160,    21,120}, 
		{17,    168,    101,84}, 
		{18,    176,    21,44}, 
		{19,    184,    57,46}, 
		{20,    192,    23,48}, 
		{21,    200,    13,50}, 
		{22,    208,    27,52}, 
		{23,    216,    11,36}, 
		{24,    224,    27,56},
		{25,    232,    85,58}, 
		{26,    240,    29,60}, 
		{27,    248,    33,62}, 
		{28,    256,    15,32},
		{29,    264,   17,198},
		{30,    272,   33,68},
		{31,    280,   103,210},
		{32,    288,   19,36},
		{33,    296,   19,74},
		{34,    304,   37,76},
		{35,    312,   19,78},
		{36,    320,   21,120},
		{37,    328,   21,82},
		{38,    336,   115,84},
		{39,    344,   193,86},
		{40,    352,   21,44},
		{41,    360,  133,90},
		{42,    368,   81,46},
		{43,    376,   45,94},
		{44,    384,   23,48},
		{45,    392,   243,98},
		{46,    400,   151,40},
		{47,    408,   155,102},
		{48,    416,   25,52},
		{49,    424,   51,106},
		{50,    432,   47,72},
		{51,    440,   91,110},
		{52,    448,    29,168},
		{53,    456,    29,	14},
		{54,    464,   247,58},
		{55,    472,   29,118},
		{56,    480,   89,180},
		{57,    488,   91,122},
		{58,    496,   157,62},
		{59,    504,   55,	84},
		{60,    512,   31,	64},
		{61,    528,   17,	66},
		{62,    544,   35,	68},
		{63,    560,   227,420},
		{64,    576,    65,96},
		{65,    592,    19,	74},
		{66,    608,    37,	76},
		{67,    624,    41,234},
		{68,    640,    39,80},
		{69,    656,    185,82},
		{70,    672,    43,	252},
		{71,    688,    21,	86},
		{72,    704,    155,44},
		{73,    720,     79,120},
		{74,    736,     139,92},
		{75,    752,     23, 94},
		{76,    768,    217, 48},
		{77,    784,     25, 98},
		{78,    800,     17, 80},
		{79,    816,    127,102},
		{80,    832,     25,52},
		{81,    848,    239,106},
		{82,    864,      17,48},
		{83,    880,    137,110},
		{84,	896,	215,112},
		{85,	912,	29,	114},
		{86,	928,	15,	58},
		{87,	944,	147,118},
		{88,	960,	29,	60},
		{89,	976,	59,	122},
		{90,	992,	65,	124},
		{91,	1008,	55,	84},
		{92,	1024,	31,	64},
		{93,	1056,	17,	66},
		{94,	1088,  171,204},
		{95,	1120,  67, 140},
		{96,	1152,	35,	72},
		{97,	1184,	19,	74},
		{98,	1216,	39,	76},
		{99,	1248,	19,	78},
		{100,	1280,	199,240},
		{101,	1312,	21,	82},
		{102,	1344,	211,252},
		{103,	1376,	21,	86},
		{104,	1408,	43,	88},
		{105,	1440,	149,60},
		{106,	1472,	45,	92},
		{107,	1504,	49,	846},
		{108,	1536,	71,	48},
		{109,	1568,	13,	28},
		{110,	1600,	17,	80},
		{111,	1632,	25,	102},
		{112,	1664,	183,104},
		{113,	1696,	55,	954},
		{114,	1728,	127,96},
		{115,	1760,	27,	110},
		{116,	1792,	29,	112},
		{117,	1824,	29,	114},
		{118,	1856,	57,	116},
		{119,	1888,	45,	354},
		{120,	1920,	31,	120},
		{121,	1952,	59,	610},
		{122,	1984,	185,124},
		{123,	2016,	113,420},
		{124,	2048,	31,	64},
		{125,	2112,	17,	66},
		{126,	2176,	171,136},
		{127,	2240,	209,420},
		{128,	2304,	253,216},
		{129,	2368,	367,444},
		{130,	2432,	265,456},
		{131,	2496,	181,468},
		{132,	2560,	39,	80},
		{133,	2624,	27,	164},
		{134,	2688,	127,504},
		{135,	2752,	143,172},
		{136,	2816,	43,	88},
		{137,	2880,	29,	300},
		{138,	2944,	45,	92},
		{139,	3008,	157,188},
		{140,	3072,	47,	96},
		{141,	3136,	13,	28},
		{142,	3200,	111,240},
		{143,	3264,	443,204},
		{144,	3328,	51,	104},
		{145,	3392,	51,	212},
		{146,	3456,	451,192},
		{147,	3520,	257,220},
		{148,	3584,	57,	336},
		{149,	3648,	313,228},
		{150,	3712,	271,232},
		{151,	3776,	179,236},
		{152,	3840,	331,120},
		{153,	3904,	363,244},
		{154,	3968,	375,248},
		{155,	4032,	127,168},
		{156,	4096,	31,	64},
		{157,	4160,	33,	130},
		{158,	4224,	43,	264},
		{159,	4288,	33,	134},
		{160,	4352,	477,408},
		{161,	4416,	35,	138},
		{162,	4480,	233,280},
		{163,	4544,	357,142},
		{164,	4608,	337,480},
		{165,	4672,	37,	146},
		{166,	4736,	71,	444},
		{167,	4800,	71,	120},
		{168,	4864,	37,	152},
		{169,	4928,	39,	462},
		{170,	4992,	127,234},
		{171,	5056,	39,	158},
		{172,	5120,	39,	80},
		{173,	5184,	31,	96},
		{174,	5248,	113,902},
		{175,	5312,	41,	166},
		{176,	5376,	251,336},
		{177,	5440,	43,	170},
		{178,	5504,	21,	86},
		{179,	5568,	43,	174},
		{180,	5632,	45,	176},
		{181,	5696,	45,	178},
		{182,   5760,   161,120},
		{183,   5824,    89,182},
		{184,   5888,   323,184},
		{185,   5952,    47,186},
		{186,   6016,    23,94},
		{187,   6080,    47,190},
		{188,   6144,    263,480}
	};

	static int UERX_TBS_DCI_1C[32] = 
	{
		40,   56,   72,   120,  136,  144,  176,  208
		, 224,  256,  280,  296,  328,  336,  392,  488
		, 552,  600,  632,  696,  776,  840,  904,  1000
		, 1064, 1128, 1224, 1288, 1384, 1480, 1608, 1736
	};
#pragma SET_DATA_SECTION()
