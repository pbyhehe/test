#include "foo.h"
#include "bstx_define.h"
#include "bstx_payload.h"
#include <stdio.h>

#define DUMP_SEQ_LOCAL 0

// PSS
int sequence_pss(int num_ID_cell, cfloat* outbuf)
{
	int i;

	static float pss0_re[62] = 
	{
		  1.0000,-0.7971, 0.3653,-0.7331, 0.9802, 0.9556,-0.5000, 0.7660
		,-0.2225, 0.6235, 0.4562, 0.3653, 0.9556, 0.7660,-0.5000,-0.7331
		, 0.9802,-0.2225, 0.6235,-0.7971,-0.5000,-0.5000,-0.7971,-0.9888
		, 0.9556, 0.9802,-0.2225, 1.0000, 0.7661,-0.7331,-0.9888,-0.9888
		,-0.7331, 0.7661, 1.0000,-0.2225, 0.9802, 0.9556,-0.9888,-0.7971
		,-0.5000,-0.5000,-0.7971, 0.6235,-0.2226, 0.9802,-0.7331,-0.5000
		, 0.7661, 0.9556, 0.3654, 0.4562, 0.6234,-0.2225, 0.7661,-0.4999
		, 0.9556, 0.9802,-0.7330, 0.3654,-0.7971, 1.0000
	};

	static float pss0_im[62] = 
	{
		  0.0000,-0.6038,-0.9309,-0.6802, 0.1981, 0.2948,-0.8660,-0.6428
		,-0.9749, 0.7818, 0.8899,-0.9309, 0.2948,-0.6428, 0.8660, 0.6802
		, 0.1982, 0.9749, 0.7818,-0.6038,-0.8660, 0.8660,-0.6038, 0.1490
		,-0.2947, 0.1982,-0.9749, 0.0000,-0.6428, 0.6802, 0.1490, 0.1490
		, 0.6802,-0.6428, 0.0000,-0.9749, 0.1982,-0.2947, 0.1490,-0.6038
		, 0.8660,-0.8660,-0.6038, 0.7819, 0.9749, 0.1982, 0.6801, 0.8660
		,-0.6427, 0.2948,-0.9309, 0.8899, 0.7819,-0.9749,-0.6427,-0.8661
		, 0.2948, 0.1982,-0.6802,-0.9308,-0.6039, 0.0000
	};

	static float pss1_re[62] = 
	{
		  1.0000,-0.9691,-0.7331, 0.0747,-0.7971, 0.8262,-0.5000, 0.7660
		,-0.9010,-0.2225,-0.4113,-0.7330, 0.8262, 0.7660,-0.5000, 0.0747
		,-0.7971,-0.9010,-0.2225,-0.9691,-0.5000,-0.5000,-0.9691, 0.9556
		, 0.8262,-0.7971,-0.9010, 1.0000, 0.7660, 0.0748, 0.9556, 0.9556
		, 0.0748, 0.7660, 1.0000,-0.9010,-0.7972, 0.8263, 0.9556,-0.9691
		,-0.5000,-0.5000,-0.9691,-0.2226,-0.9009,-0.7972, 0.0748,-0.4999
		, 0.7660, 0.8262,-0.7330,-0.4112,-0.2226,-0.9010, 0.7660,-0.5001
		, 0.8262,-0.7972, 0.0746,-0.7330,-0.9691, 1.0000
	};

	static float pss1_im[62] = 
	{
		  0.0000,-0.2468,-0.6802, 0.9972, 0.6038, 0.5633, 0.8660, 0.6428
		, 0.4339, 0.9749,-0.9115,-0.6802, 0.5633, 0.6428,-0.8660,-0.9972
		, 0.6038,-0.4339, 0.9749,-0.2468, 0.8660,-0.8660,-0.2468,-0.2947
		,-0.5633, 0.6038, 0.4339, 0.0000, 0.6428,-0.9972,-0.2947,-0.2947
		,-0.9972, 0.6428, 0.0000, 0.4339, 0.6038,-0.5633,-0.2947,-0.2468
		,-0.8660, 0.8660,-0.2468, 0.9749,-0.4339, 0.6038,-0.9972,-0.8661
		, 0.6428, 0.5634,-0.6802,-0.9115, 0.9749, 0.4338, 0.6428, 0.8660
		, 0.5634, 0.6037, 0.9972,-0.6802,-0.2468, 0.0000
	};

	static float pss2_re[62] = 
	{
		  1.0000,-0.9691,-0.7331, 0.0747,-0.7971, 0.8262,-0.5000, 0.7660
		,-0.9010,-0.2225,-0.4113,-0.7331, 0.8262, 0.7660,-0.5000, 0.0747
		,-0.7971,-0.9010,-0.2225,-0.9691,-0.5000,-0.5000,-0.9691, 0.9556
		, 0.8262,-0.7971,-0.9010, 1.0000, 0.7661, 0.0747, 0.9556, 0.9556
		, 0.0747, 0.7661, 1.0000,-0.9010,-0.7971, 0.8262, 0.9556,-0.9691
		,-0.5000,-0.5000,-0.9691,-0.2225,-0.9010,-0.7971, 0.0747,-0.5001
		, 0.7661, 0.8263,-0.7331,-0.4114,-0.2224,-0.9009, 0.7661,-0.4999
		, 0.8263,-0.7971, 0.0748,-0.7331,-0.9691, 1.0000
	};

	static float pss2_im[62] = 
	{
		  0.0000, 0.2468, 0.6802,-0.9972,-0.6038,-0.5633,-0.8660,-0.6428
		,-0.4339,-0.9749, 0.9115, 0.6802,-0.5633,-0.6428, 0.8660, 0.9972
		,-0.6038, 0.4339,-0.9749, 0.2467,-0.8660, 0.8660, 0.2467, 0.2948
		, 0.5633,-0.6038,-0.4339, 0.0000,-0.6428, 0.9972, 0.2948, 0.2948
		, 0.9972,-0.6428, 0.0000,-0.4339,-0.6038, 0.5634, 0.2948, 0.2467
		, 0.8660,-0.8661, 0.2467,-0.9749, 0.4338,-0.6039, 0.9972, 0.8660
		,-0.6427,-0.5633, 0.6801, 0.9115,-0.9749,-0.4340,-0.6427,-0.8661
		,-0.5632,-0.6039,-0.9972, 0.6801, 0.2467, 0.0000
	};

	if(num_ID_cell%3 == 0)
	{
		for(i=0; i<62; i++)
		{
			outbuf[i].re = pss0_re[i];
			outbuf[i].im = pss0_im[i];
		}
	}
	else if(num_ID_cell%3 == 1)
	{
		for(i=0; i<62; i++)
		{
			outbuf[i].re = pss1_re[i];
			outbuf[i].im = pss1_im[i];
		}
	}
	else if(num_ID_cell%3 == 2)
	{
		for(i=0; i<62; i++)
		{
			outbuf[i].re = pss2_re[i];
			outbuf[i].im = pss2_im[i];
		}
	}

	return 62;
}


// SSS
int sequence_sss(int num_ID_cell, int subfrm_idx, cfloat *outbuf)
{
	int i;
	int N_ID_1;
	int N_ID_2;
	int qq;
	int q;
	int mm;
	int m0;
	int m1;
	int x[31];
	int ss[31];
	int s0[31];
	int s1[31];
	int cc[31];
	int c0[31];
	int c1[31];
	int zz[31];
	int zm0[31];
	int zm1[31];
	
	N_ID_1 = (int)(num_ID_cell / 3);
	N_ID_2 = num_ID_cell%3;
	qq     = (int)(N_ID_1 / 30);
	q      = (int)( ( N_ID_1 + qq*(qq+1) / 2 ) / 30 );
	mm     = N_ID_1 + q * (q + 1) / 2;
	m0     = mm % 31;
	m1     = ( m0 + (int)(mm / 31) + 1)%31;
	
	// s0[31], s1[31]
	x[0] = 0;
	x[1] = 0;
	x[2] = 0;
	x[3] = 0;
	x[4] = 1;
	
	for(i=0; i<26; i++)
	{
		x[i+5] = x[i+2] ^ x[i];
	}
	
	for(i=0; i<31; i++)
	{
		ss[i] = 1-2*x[i];
	}
	
	for(i=0; i<31; i++)
	{
		s0[i] = ss[(i+m0)%31];
		s1[i] = ss[(i+m1)%31];
	}

	
	// c0[31],c1[31]
	x[0] = 0;
	x[1] = 0;
	x[2] = 0;
	x[3] = 0;
	x[4] = 1;
	
	for(i=0; i < 26; i++)
	{
		x[i+5] = x[i+3] ^ x[i];
	}
	
	for(i=0; i < 31; i++)
	{
		cc[i] = 1-2*x[i];
	}
	
	for(i=0; i < 31; i++)
	{
		c0[i] = cc[(i+N_ID_2)%31];
		c1[i] = cc[(i+N_ID_2+3)%31];
	}

	
	// zm0[31],zm1[31]
	x[0] = 0;
	x[1] = 0;
	x[2] = 0;
	x[3] = 0;
	x[4] = 1;
	
	for (i=0; i<26; i++)
	{
		x[i+5] = x[i+4] ^ x[i+2] ^ x[i+1] ^ x[i];
	}
	
	for(i=0; i<31; i++)
	{
		zz[i] = 1-2*x[i];
	}
	
	for (i=0; i<31; i++)
	{
		zm0[i] = zz[(i + (m0%8))%31];
		zm1[i] = zz[(i + (m1%8))%31];
	}

	
	// get sss
	if(subfrm_idx == 0)
	{
		for(i=0; i<31; i++)
		{
			outbuf[2*i+0].re = (s0[i]*c0[i]);
			outbuf[2*i+0].im = 0;
			outbuf[2*i+1].re = (s1[i]*c1[i]*zm0[i]);
			outbuf[2*i+1].im = 0;
		}
	}
	else
	{
		for(i=0; i<31; i++)
		{
			outbuf[2*i+0].re = (s1[i]*c0[i]);
			outbuf[2*i+0].im = 0;
			outbuf[2*i+1].re = (s0[i]*c1[i]*zm1[i]);
			outbuf[2*i+1].im = 0;
		}
	}

	return 62;
}



int sequence_rs(int num_ID_cell, int cp_cfg, int subfrm_idx, cfloat *outbuf)
{
	int i, j, k;
	int N_cp;
	int cinit;
	int slot_idx;
	int rand_seq[4*110];
	
	int *ptr_l_num;

	int l_num_normal_cp_list[3] = {0, 1, 4};
	int l_num_extend_cp_list[3] = {0, 1, 3};
	
	// normal cp
	if (cp_cfg==0)  
	{
		N_cp = 1;
		ptr_l_num = l_num_normal_cp_list;
	}
	else 
	{
		N_cp = 0;
		ptr_l_num = l_num_extend_cp_list;
	}
	
	for (i=0; i<2; i++)
	{
		slot_idx = 2*subfrm_idx+i; // in a wireless frame
		
		for (j=0; j<3; j++)
		{
			//Pseudo-random sequence cinit initialization
			cinit = (1<<10)*(7*(slot_idx+1)+ptr_l_num[j]+1)*(2*num_ID_cell+1) + 2*num_ID_cell + N_cp;
			
			bstx_pseudo_rand(cinit, 4*110, rand_seq);
			

			for(k=0; k<2*110; k++)                    // RS generation
			{
				outbuf[k+220*(3*i+j)].re = 0.7071*(1-2*rand_seq[2*k+0]);
				outbuf[k+220*(3*i+j)].im = 0.7071*(1-2*rand_seq[2*k+1]);
			}
		}		
	}	
 
	return 12*110; // 2*3*220
}


// pbch written by MAC
int input_pbch(int16 *outbuf)
{
	int i;

	for(i=0; i<24; i++) 
	{
// 		outbuf[i] = sour.GetBit(); // huangshoujun_server
		outbuf[i] = 1; // zuohuanhuan
	}

	return 24;
}


int input_pdcch(int dci_format, int num_rb_dl, int num_rb_ul, int num_bstx_ant, int uldl_switch_cfg
							  , bstx_ctrl_pdsch *ctrl_pdsch_u, int16 *outbuf)
{
	int outlen;

	outlen = dci_write(dci_format, num_rb_dl, num_rb_ul, num_bstx_ant, uldl_switch_cfg, ctrl_pdsch_u, outbuf);

	return outlen;
}


int input_pcfich(int num_ofdm_pdcch)
{
	return num_ofdm_pdcch;
}

int input_pdsch(int transblock_size, int16 *outbuf)
{
	int i;


	for (i=0; i<transblock_size; i++)
	{
// 		outbuf[i] = sour.GetBit(); // huangshoujun_server
		outbuf[i] = 1; // zuohuanhuan
//		outbuf[i] = rand() % 2;//caohuan
	}
//	outbuf[0]=0;
//	outbuf[1]=0;
//	outbuf[5000]=0;
//	outbuf[6000]=0;
	return transblock_size;
}




int bstx_pseudo_rand(int cinit, int sequence_len, int *outbuf)
{
	int i, j;
	int x1[32]= {0,0,0,1,0,0,1,0,0,1,1,0,1,1,0,0,0,0,1,0,0,1,0,1,1,0,0,1,0,1,0,1}; // push 1600
	int x2[32];

	// init x2
	for (i=0; i<31; i++)
	{
		x2[i] = cinit%2;
		cinit /= 2;
	}
	
	// push x2 1600
	for(i = 31; i < 1600; i++)
	{
		x2[i%32] = x2[(i-28)%32] ^ x2[(i-29)%32] ^ x2[(i-30)%32] ^ x2[(i-31)%32];
	}
	
	// get pseudo rand sequence
	j = 0;
	for (i=1600; i<sequence_len+1600; i++)
	{
		x1[i%32] = x1[(i-28)%32] ^ x1[(i-31)%32];
		x2[i%32] = x2[(i-28)%32] ^ x2[(i-29)%32] ^ x2[(i-30)%32] ^ x2[(i-31)%32];
		
		outbuf[j] = x1[i%32] ^ x2[i%32];
		j++;
	}
	
	return sequence_len;	
}
