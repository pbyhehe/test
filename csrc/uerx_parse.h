#pragma once

// The number of transmit and receiver antennas
#define NUM_BSTX_ANTENNAS	2;
#define NUM_UERX_ANTENNAS   2;

// Transmission Mode 1, DCI_0  --> (SISO OR SIMO)
// Transmission Mode 2, DCI_0  --> (SFBC)
// Transmission Mode 3, DCI_2A --> (SFBC OR OpenloopSM)
// Transmission Mode 4, DCI_2  --> (SFBC OR CloseloopSM)
// Transmission Mode 5
// Transmission Mode 6
// Transmission Mode 7
//#define TRANSMISSION_MODE_UE[0]	 4;
//#define TRANSMISSION_MODE_UE[1]	 4;

// 0:DCI_0, 1:DCI_1, 2:DCI_1A, 3:DCI_1B, 4:DCI_1D, 5:DCI_2, 6:DCI_2A, 7:DCI_1C, 8:DCI_3, 9:DCI_3A
//#define DCI_FORMAT_UE[0]  5;
//#define DCI_FORMAT_UE[1]  5;

// 0:SI_RNTI_type, 1:P_RNTI_type, 2:RA_RNTI_type, 3:C_RNTI_type, 4:SPSC_RNTI_type, 5:TC_RNTI_type
//#define RNTI_TYPE_UE[0]  3;
//#define RNTI_TYPE_UE[1]  3;

// Number of RNTI for each UE
//#define NUM_RNTI_UE[0]  0;
//#define NUM_RNTI_UE[1]  0;

// 0:1CCE, 1:2CCE, 2:4CCE, 3:8CCE
//#define PDCCH_FORMAT_UE[0]  1;
//#define PDCCH_FORMAT_UE[1]  1;

// The number of ofdm for pdcch
#define L_PDCCH	 2;

// 5(qpsk 1/3), 7(qpsk 1/2), 11(16qam 1/3), 14(16qam 1/2), 17(64qam 1/3), 25(64qam 3/4)
//#define MCS_CW0_UE[0]  17;
//#define MCS_CW1_UE[0]  17;

//#define MCS_CW0_UE[1] = 17;
//#define MCS_CW1_UE[1] = 17;

// 2 bstx antenna : 2 codeword --> 2 layers
//#define NUM_CODEWORDS_UE[0]  2;
//#define NUM_CODEWORDS_UE[1]  2;

// precoding codebook index
#define PRECODING_CODEBOOK_INDEX  1;

// The total number of RB for DL and UL
#define NUM_RB_DL  100;
#define NUM_RB_UL  100;

// The FFT and Sample rate // 141128 linjiangnan
#define N_FFT  2048; // 141128 linjiangnan

// the allocation RB for each UE
//#define RB_START_UE[0]  0;
//#define NUM_RB_ALLOC_UE[0]  100;
#define NUM_RB_ALLOC_UE	100

//#define RB_START_UE[1]  0;
//#define NUM_RB_ALLOC_UE[1]  0;

// Downlink power allocation parameters(dB)
#define RA_PDSCH   -3;
#define RB_PDSCH   -3;
#define RA_PSS     -3;
#define RA_SSS     -3;
#define RA_PBCH    -3;
#define RB_PBCH    -3;
#define RA_PHICH   -3;
#define RB_PHICH   -3;
#define RA_PDCCH   -3;
#define RB_PDCCH   -3;
#define RB_PCFICH  -3;

// The cell indication number
#define N_ID_CELL  0;

// The ULDL switch config and special subframe config
#define ULDL_SWITCH_CONFIG  3;
#define SPECIAL_SUBFRAME_CONFIG  0;

// TD-LTE or FD_LTE, 0:FDD, 1:TDD
#define FDD_TDD_MODE  1;

// The cyclic prefix config, 0:normal CP, 1:extendex CP
#define CP_CONFIG  0;

// The space of subcarriers, 0:15KHz, 1:7.5KHz
#define SPACE_SUBCARRIERS_CONFIG  0;

// pusch hopping or not for each UE, 0:no hopping, 1:hopping
//#define PUSCH_HOPPING_FLAG_UE[0]  0;
//#define PUSCH_HOPPING_FLAG_UE[1]  0;

// User special reference signal config for each UE, 0:no URS, 1:use URS
//#define URS_CONFIG_UE[0]  0;
//#define URS_CONFIG_UE[1]  0;

// Transmit antenna select config for each UE, 0:no select, 1:with select
//#define TRANSMIT_ANTENNA_SELECTE_UE[0] 0;
//#define TRANSMIT_ANTENNA_SELECTE_UE[1] 0;

// Antenna port for each UE
//#define PORT_UE[0]  0;
//#define PORT_UE[1]  0;

// The category for each UE
//#define CATEGORY_UE[0]  5;
//#define CATEGORY_UE[1]  5;

// TPC PUSCH for each UE
//#define TPC_PUSCH_UE[0]  0;
//#define TPC_PUSCH_UE[1]  0;

// DAI for each UE
//#define DAI_UE[0]  0;
//#define DAI_UE[1]  0;

// Cyclic shift DMRS for each UE
//#define CYCLIC_SHIFT_DMRS_UE[0]  7;
//#define CYCLIC_SHIFT_DMRS_UE[1]  7;

// UL index for each UE
//#define UL_INDEX_UE[0]  1;
//#define UL_INDEX_UE[1]  1;

// CQI request flag for each UE, 0:no request, 1:request
//#define CQI_REQUEST_UE[0]  0;
//#define CQI_REQUEST_UE[1]  0;

// Resource element allocation type for each UE, 0:local, 1:distribute
//#define RE_ALLOC_TYPE_UE[0]	 0;
//#define RE_ALLOC_TYPE_UE[1]	 0;

// TPC PUCCH for each UE
//#define TPC_PUCCH_UE[0]  0;
//#define TPC_PUCCH_UE[1]  0;

// Local or distribute VRB flag for each UE, 0:local, 1:distribute
//#define LOCAL_DISTRIBUTE_VRB_UE[0]  0;
//#define LOCAL_DISTRIBUTE_VRB_UE[1]  0;

// Preamble index - 6 bits for each UE
//#define PREAMBLE_INDEX_UE[0]  0;
//#define PREAMBLE_INDEX_UE[1]  0;

// prach mask index - 4 bits for each UE
//#define PRACH_MASK_INDEX_UE[0]  0;
//#define PRACH_MASK_INDEX_UE[1]  0;

// Transblock to codeword swap for each UE
//#define TB_TO_CODEWORD_SWAP_UE[0]  0;
//#define TB_TO_CODEWORD_SWAP_UE[1]  0;

// The number of pdsch, pdcch and phich
#define NUM_PDSCH  1;
#define NUM_PDCCH  1;
#define NUM_PHICH  4;
// The config of phich
#define NG_CONFIG_PHICH  0;
#define DURATION_CONFIG_PHICH  0;

// The group index and sequence index of phich
//#define GROUP_INDEX_PHICH[0]  0;
//#define GROUP_INDEX_PHICH[1]  0;
//#define GROUP_INDEX_PHICH[2]  0;
//#define GROUP_INDEX_PHICH[3]  0;

//#define SEQUE_INDEX_PHICH[0]  0;
//#define SEQUE_INDEX_PHICH[1]  1;
//#define SEQUE_INDEX_PHICH[2]  2;
//#define SEQUE_INDEX_PHICH[3]  3;


// Table B.2.2-1 Channel model parameters
// Model			Maximum Doppler frequency
// 300 EPA 5Hz		5 Hz
// 301 EVA 5Hz		5 Hz
// 301 EVA 70Hz		70 Hz
// 302 ETU 70Hz		70 Hz
// 302 ETU 300Hz	300 Hz

#define SNR_DB                   26;
#define CHANNEL_TYPE			 300;
#define DOPPLER_HZ				 5;
#define CHANNEL_CORR			 0;		   // 0:Low_corration, 1:Medium_corration, 2:High_corration
#define UP_DL_CHANNEL			 1;		   // 0:uplink, 1:downlink
#define FREQUENCY_OFFSET_PPM	 0;	       // 7.68M SamplingRate: 1ppm === phase 0.0020
#define SAMPLING_RATE_MHZ		 30.72;      // MHz
#define FREQUENCY_CARRIER_MHZ	 2400;	       // MHz
