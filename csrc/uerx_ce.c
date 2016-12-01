#include "foo.h"
//#include "fft.h"

#include "uerx_define.h"
#include "uerx_fixed_cfg.h"
#include "uerx_sharefunc.h"
#include "uerx_ce.h"


int get_RS(int num_id_cell, int num_bstx_ant, int num_rb_dl, int cp_cfg, int subfrm_idx, int ofdm_idx
					   , cfloat *rs_buf, int *rs_start)
{
 	int k, m;	
	int c_cp;
	int ns;
	int l_num;
	int cinit;
	int outlen;
	int num_ofdm_slot;
	int pesedu_buf[4*UERX_MAX_NUM_RB_DL];	

	if (cp_cfg == UERX_NORMAL_CP)
	{
		c_cp = 1;
		num_ofdm_slot = 7;
	}
	else 
	{
		c_cp = 0;
		num_ofdm_slot = 6;
	}		

	ns = 2*subfrm_idx + ofdm_idx/num_ofdm_slot;
	l_num = ofdm_idx % num_ofdm_slot;	

	outlen = 0;

	if (	l_num == 0 
		||	l_num == num_ofdm_slot-3 
		|| (l_num == 1 && num_bstx_ant == 4))
	{
		cinit = (1<<10)*(7*(ns+1)+l_num+1)*(2*num_id_cell+1)+2*num_id_cell+c_cp;

		/*uerx_sharefunc_u.*/uerx_pseudo_rand(cinit, pesedu_buf, 4*UERX_MAX_NUM_RB_DL);
		
		// RS generation
		for (k=0; k<num_rb_dl*2; k++)      
		{
			m = k+UERX_MAX_NUM_RB_DL-num_rb_dl;
			
			rs_buf[k].re = 0.7071 * (1-2*pesedu_buf[2*m+0]);
			rs_buf[k].im = 0.7071 * (1-2*pesedu_buf[2*m+1]);
		}
		
		// RS start position in the resource element mapping
		rs_start[0] = (l_num==0)? ((0+num_id_cell)%6) : ((3+num_id_cell)%6);	// p=0
		rs_start[1] = (l_num==0)? ((3+num_id_cell)%6) : ((0+num_id_cell)%6);	// p=1
		rs_start[2] = (3*(ns%2)+num_id_cell)%6;									// p=2
		rs_start[3] = (3+3*(ns%2)+num_id_cell)%6;								// p=3
	
		outlen = 2*num_rb_dl;
	}

	return outlen;
}


int ce(cfloat **inbuf, int num_subcarrier, cfloat rs_buf[220], int rs_start[4], int num_rs
				, int num_bstx_ant, int num_uerx_ant, cfloat **outH)
{
	int p, j, k, m;

	#pragma SET_DATA_SECTION("uerx_data")
	static cfloat H[4][2048];
	#pragma SET_DATA_SECTION()
	
	// function body
	if(num_rs)                           
	{
		// channel estimation	
		for(p=0; p<num_uerx_ant; p++)
		{
			for (j=0; j<num_bstx_ant; j++)
			{
				m = rs_start[j];	
				for (k=0; k<num_rs; k++)
				{	// H = inbuf * conj(rs_buf)
					H[num_bstx_ant*p+j][6*k+m].re = inbuf[p][6*k+m].re*rs_buf[k].re + inbuf[p][6*k+m].im*rs_buf[k].im; 
					H[num_bstx_ant*p+j][6*k+m].im = inbuf[p][6*k+m].im*rs_buf[k].re - inbuf[p][6*k+m].re*rs_buf[k].im; 
				}
			}
		}

		for(p=0; p<num_uerx_ant; p++)
		{
			for (j=0; j<num_bstx_ant; j++)
			{
				m = rs_start[j];	

				// copy H to the start
				for (k=0; k<m; k++)
				{
					H[num_bstx_ant*p+j][k].re = H[num_bstx_ant*p+j][m].re;
					H[num_bstx_ant*p+j][k].im = H[num_bstx_ant*p+j][m].im;
				}

				// linear insert channel estimation
				for (k=0; k<num_rs-1; k++)
				{
					H[num_bstx_ant*p+j][6*k+m+1].re = (5.0/6)*H[num_bstx_ant*p+j][6*k+m].re + (1.0/6)*H[num_bstx_ant*p+j][6*k+m+6].re; 
					H[num_bstx_ant*p+j][6*k+m+1].im = (5.0/6)*H[num_bstx_ant*p+j][6*k+m].im + (1.0/6)*H[num_bstx_ant*p+j][6*k+m+6].im; 
					
					H[num_bstx_ant*p+j][6*k+m+2].re = (4.0/6)*H[num_bstx_ant*p+j][6*k+m].re + (2.0/6)*H[num_bstx_ant*p+j][6*k+m+6].re; 
					H[num_bstx_ant*p+j][6*k+m+2].im = (4.0/6)*H[num_bstx_ant*p+j][6*k+m].im + (2.0/6)*H[num_bstx_ant*p+j][6*k+m+6].im; 
					
					H[num_bstx_ant*p+j][6*k+m+3].re = (3.0/6)*H[num_bstx_ant*p+j][6*k+m].re + (3.0/6)*H[num_bstx_ant*p+j][6*k+m+6].re; 
					H[num_bstx_ant*p+j][6*k+m+3].im = (3.0/6)*H[num_bstx_ant*p+j][6*k+m].im + (3.0/6)*H[num_bstx_ant*p+j][6*k+m+6].im; 
					
					H[num_bstx_ant*p+j][6*k+m+4].re = (2.0/6)*H[num_bstx_ant*p+j][6*k+m].re + (4.0/6)*H[num_bstx_ant*p+j][6*k+m+6].re; 
					H[num_bstx_ant*p+j][6*k+m+4].im = (2.0/6)*H[num_bstx_ant*p+j][6*k+m].im + (4.0/6)*H[num_bstx_ant*p+j][6*k+m+6].im; 
					
					H[num_bstx_ant*p+j][6*k+m+5].re = (1.0/6)*H[num_bstx_ant*p+j][6*k+m].re + (5.0/6)*H[num_bstx_ant*p+j][6*k+m+6].re; 
					H[num_bstx_ant*p+j][6*k+m+5].im = (1.0/6)*H[num_bstx_ant*p+j][6*k+m].im + (5.0/6)*H[num_bstx_ant*p+j][6*k+m+6].im; 
				}

				// copy H to the end
				for (k=num_subcarrier+m-5; k<num_subcarrier; k++)
				{
					H[num_bstx_ant*p+j][k].re = H[num_bstx_ant*p+j][num_subcarrier+m-6].re;
					H[num_bstx_ant*p+j][k].im = H[num_bstx_ant*p+j][num_subcarrier+m-6].im;
				}
			}	
		}
	}

	// outH
	for(p=0; p<num_uerx_ant; p++)
	{
		for (j=0; j<num_bstx_ant; j++)
		{
			for (k=0; k<num_subcarrier; k++)
			{
				outH[num_bstx_ant*p+j][k] = H[num_bstx_ant*p+j][k];
			}
		}
	}

	// return the length of outbuf
	return num_subcarrier;	
}








 


