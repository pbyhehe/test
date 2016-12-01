// 0:float, 1:matlab fixed, 2:f2f, 3:systemC fixed, 8:by other module
#define    LTEDL_FIXD_FLG               0 
											

#define    LTEDL_SYNCH_AFCCARSE			0

#define    LTEDL_DEQAM_DEQAM			0

#define    LTEDL_EQU_SISO_DECOD         0
#define    LTEDL_EQU_MRC_DECOD          0
#define    LTEDL_EQU_SFBC_DECOD         0
#define    LTEDL_EQU_CLSM_DECOD         1









#define    LTE_TURBODECODER_MAXLOGMAP_FIXD         0 // matlab fixed test pass
// turbo decoder fixed-point
#define    bit_width                               5 //  the input bit width
#define    frac_width                              2 //  the input fractional width
#define    alpha_width                             8 //  alpha bit width
#define    alpha_Q                                 2 //  alpha fractional length
#define    beta_width                              8 //  beta bit width
#define    beta_Q                                  2 //  beta fractional length
#define    Gamma_width                             8 //  Gammma bit width
#define    LLR_width                               8 //  LLR bit width
#define    LLR_Q                                   2 //  LLR fractional length
#define    Le_width                                6
#define    Le_Q                                    2
#define    turbo_fix_config                        1 //  1 is for turbo fix, 0 is for turbo float
#define    window_config                           0 //  1 for max-log-map parallel window, 0 for no window
