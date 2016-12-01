#include "foo.h"
#include "bstx_define.h"
#include "bstx_mod_phich.h"


int spread_phich(cfloat *inbuf, int inlen, int cp_cfg, int sequence_idx, cfloat *outbuf)
{
	int i;
	int outlen;
	
	cfloat z_buf[12];
	cfloat w_buf[12];
	cfloat w_normal_cp[8][12];
    cfloat w_extend_cp[4][6];

	// orthogonal sequence when normal cp
	w_normal_cp[0][0].re=1;  w_normal_cp[0][0].im=0;
	w_normal_cp[0][1].re=1;  w_normal_cp[0][1].im=0;
	w_normal_cp[0][2].re=1;  w_normal_cp[0][2].im=0;
	w_normal_cp[0][3].re=1;  w_normal_cp[0][3].im=0;
	w_normal_cp[0][4].re=1;  w_normal_cp[0][4].im=0;
	w_normal_cp[0][5].re=1;  w_normal_cp[0][5].im=0;
	w_normal_cp[0][6].re=1;  w_normal_cp[0][6].im=0;
	w_normal_cp[0][7].re=1;  w_normal_cp[0][7].im=0;
	w_normal_cp[0][8].re=1;  w_normal_cp[0][8].im=0;
	w_normal_cp[0][9].re=1;  w_normal_cp[0][9].im=0;
	w_normal_cp[0][10].re=1; w_normal_cp[0][10].im=0;
	w_normal_cp[0][11].re=1; w_normal_cp[0][11].im=0;

	w_normal_cp[1][0].re=1;   w_normal_cp[1][0].im=0;
	w_normal_cp[1][1].re=-1;  w_normal_cp[1][1].im=0;
	w_normal_cp[1][2].re=1;   w_normal_cp[1][2].im=0;
	w_normal_cp[1][3].re=-1;  w_normal_cp[1][3].im=0;
	w_normal_cp[1][4].re=1;   w_normal_cp[1][4].im=0;
	w_normal_cp[1][5].re=-1;  w_normal_cp[1][5].im=0;
	w_normal_cp[1][6].re=1;   w_normal_cp[1][6].im=0;
	w_normal_cp[1][7].re=-1;  w_normal_cp[1][7].im=0;
	w_normal_cp[1][8].re=1;   w_normal_cp[1][8].im=0;
	w_normal_cp[1][9].re=-1;  w_normal_cp[1][9].im=0;
	w_normal_cp[1][10].re=1;  w_normal_cp[1][10].im=0;
	w_normal_cp[1][11].re=-1; w_normal_cp[1][11].im=0;

	w_normal_cp[2][0].re=1;   w_normal_cp[2][0].im=0;
	w_normal_cp[2][1].re=1;   w_normal_cp[2][1].im=0;
	w_normal_cp[2][2].re=-1;  w_normal_cp[2][2].im=0;
	w_normal_cp[2][3].re=-1;  w_normal_cp[2][3].im=0;
	w_normal_cp[2][4].re=1;   w_normal_cp[2][4].im=0;
	w_normal_cp[2][5].re=1;   w_normal_cp[2][5].im=0;
	w_normal_cp[2][6].re=-1;  w_normal_cp[2][6].im=0;
	w_normal_cp[2][7].re=-1;  w_normal_cp[2][7].im=0;
	w_normal_cp[2][8].re=1;   w_normal_cp[2][8].im=0;
	w_normal_cp[2][9].re=1;   w_normal_cp[2][9].im=0;
	w_normal_cp[2][10].re=-1; w_normal_cp[2][10].im=0;
	w_normal_cp[2][11].re=-1; w_normal_cp[2][11].im=0;

	w_normal_cp[3][0].re=1;   w_normal_cp[3][0].im=0;
	w_normal_cp[3][1].re=-1;  w_normal_cp[3][1].im=0;
	w_normal_cp[3][2].re=-1;  w_normal_cp[3][2].im=0;
	w_normal_cp[3][3].re=1;   w_normal_cp[3][3].im=0;
	w_normal_cp[3][4].re=1;   w_normal_cp[3][4].im=0;
	w_normal_cp[3][5].re=-1;  w_normal_cp[3][5].im=0;
	w_normal_cp[3][6].re=-1;  w_normal_cp[3][6].im=0;
	w_normal_cp[3][7].re=1;   w_normal_cp[3][7].im=0;
	w_normal_cp[3][8].re=1;   w_normal_cp[3][8].im=0;
	w_normal_cp[3][9].re=-1;  w_normal_cp[3][9].im=0;
	w_normal_cp[3][10].re=-1; w_normal_cp[3][10].im=0;
	w_normal_cp[3][11].re=1;  w_normal_cp[3][11].im=0;

	w_normal_cp[4][0].re=0;  w_normal_cp[4][0].im=1;
	w_normal_cp[4][1].re=0;  w_normal_cp[4][1].im=1;
	w_normal_cp[4][2].re=0;  w_normal_cp[4][2].im=1;
	w_normal_cp[4][3].re=0;  w_normal_cp[4][3].im=1;
	w_normal_cp[4][4].re=0;  w_normal_cp[4][4].im=1;
	w_normal_cp[4][5].re=0;  w_normal_cp[4][5].im=1;
	w_normal_cp[4][6].re=0;  w_normal_cp[4][6].im=1;
	w_normal_cp[4][7].re=0;  w_normal_cp[4][7].im=1;
	w_normal_cp[4][8].re=0;  w_normal_cp[4][8].im=1;
	w_normal_cp[4][9].re=0;  w_normal_cp[4][9].im=1;
	w_normal_cp[4][10].re=0; w_normal_cp[4][10].im=1;
	w_normal_cp[4][11].re=0; w_normal_cp[4][11].im=1;

	w_normal_cp[5][0].re=0;  w_normal_cp[5][0].im=1;
	w_normal_cp[5][1].re=0;  w_normal_cp[5][1].im=-1;
	w_normal_cp[5][2].re=0;  w_normal_cp[5][2].im=1;
	w_normal_cp[5][3].re=0;  w_normal_cp[5][3].im=-1;
	w_normal_cp[5][4].re=0;  w_normal_cp[5][4].im=1;
	w_normal_cp[5][5].re=0;  w_normal_cp[5][5].im=-1;
	w_normal_cp[5][6].re=0;  w_normal_cp[5][6].im=1;
	w_normal_cp[5][7].re=0;  w_normal_cp[5][7].im=-1;
	w_normal_cp[5][8].re=0;  w_normal_cp[5][8].im=1;
	w_normal_cp[5][9].re=0;  w_normal_cp[5][9].im=-1;
	w_normal_cp[5][10].re=0; w_normal_cp[5][10].im=1;
	w_normal_cp[5][11].re=0; w_normal_cp[5][11].im=-1;

	w_normal_cp[6][0].re=0;  w_normal_cp[6][0].im=1;
	w_normal_cp[6][1].re=0;  w_normal_cp[6][1].im=1;
	w_normal_cp[6][2].re=0;  w_normal_cp[6][2].im=-1;
	w_normal_cp[6][3].re=0;  w_normal_cp[6][3].im=-1;
	w_normal_cp[6][4].re=0;  w_normal_cp[6][4].im=1;
	w_normal_cp[6][5].re=0;  w_normal_cp[6][5].im=1;
	w_normal_cp[6][6].re=0;  w_normal_cp[6][6].im=-1;
	w_normal_cp[6][7].re=0;  w_normal_cp[6][7].im=-1;
	w_normal_cp[6][8].re=0;  w_normal_cp[6][8].im=1;
	w_normal_cp[6][9].re=0;  w_normal_cp[6][9].im=1;
	w_normal_cp[6][10].re=0; w_normal_cp[6][10].im=-1;
	w_normal_cp[6][11].re=0; w_normal_cp[6][11].im=-1;

	w_normal_cp[7][0].re=0;  w_normal_cp[7][0].im=1;
	w_normal_cp[7][1].re=0;  w_normal_cp[7][1].im=-1;
	w_normal_cp[7][2].re=0;  w_normal_cp[7][2].im=-1;
	w_normal_cp[7][3].re=0;  w_normal_cp[7][3].im=1;
	w_normal_cp[7][4].re=0;  w_normal_cp[7][4].im=1;
	w_normal_cp[7][5].re=0;  w_normal_cp[7][5].im=-1;
	w_normal_cp[7][6].re=0;  w_normal_cp[7][6].im=-1;
	w_normal_cp[7][7].re=0;  w_normal_cp[7][7].im=1;
	w_normal_cp[7][8].re=0;  w_normal_cp[7][8].im=1;
	w_normal_cp[7][9].re=0;  w_normal_cp[7][9].im=-1;
	w_normal_cp[7][10].re=0; w_normal_cp[7][10].im=-1;
	w_normal_cp[7][11].re=0; w_normal_cp[7][11].im=1;

	// orthogonal sequence when extend cp
	w_extend_cp[0][0].re=1;  w_extend_cp[0][0].im=0;
	w_extend_cp[0][1].re=1;  w_extend_cp[0][1].im=0;
	w_extend_cp[0][2].re=1;  w_extend_cp[0][2].im=0;
	w_extend_cp[0][3].re=1;  w_extend_cp[0][3].im=0;
	w_extend_cp[0][4].re=1;  w_extend_cp[0][4].im=0;
	w_extend_cp[0][5].re=1;  w_extend_cp[0][5].im=0;

	w_extend_cp[1][0].re=1;   w_extend_cp[1][0].im=0;
	w_extend_cp[1][1].re=-1;  w_extend_cp[1][1].im=0;
	w_extend_cp[1][2].re=1;   w_extend_cp[1][2].im=0;
	w_extend_cp[1][3].re=-1;  w_extend_cp[1][3].im=0;
	w_extend_cp[1][4].re=1;   w_extend_cp[1][4].im=0;
	w_extend_cp[1][5].re=-1;  w_extend_cp[1][5].im=0;

	w_extend_cp[2][0].re=0;  w_extend_cp[2][0].im=1;
	w_extend_cp[2][1].re=0;  w_extend_cp[2][1].im=1;
	w_extend_cp[2][2].re=0;  w_extend_cp[2][2].im=1;
	w_extend_cp[2][3].re=0;  w_extend_cp[2][3].im=1;
	w_extend_cp[2][4].re=0;  w_extend_cp[2][4].im=1;
	w_extend_cp[2][5].re=0;  w_extend_cp[2][5].im=1;

	w_extend_cp[3][0].re=0;  w_extend_cp[3][0].im=1;
	w_extend_cp[3][1].re=0;  w_extend_cp[3][1].im=-1;
	w_extend_cp[3][2].re=0;  w_extend_cp[3][2].im=1;
	w_extend_cp[3][3].re=0;  w_extend_cp[3][3].im=-1;
	w_extend_cp[3][4].re=0;  w_extend_cp[3][4].im=1;
	w_extend_cp[3][5].re=0;  w_extend_cp[3][5].im=-1;
		
	// spread
	assert(inlen == 3);

	if(cp_cfg == BSTX_NORMAL_CP)
	{
		z_buf[0] = inbuf[0];
		z_buf[1] = inbuf[0];
		z_buf[2] = inbuf[0];
		z_buf[3] = inbuf[0];
		z_buf[4] = inbuf[1];
		z_buf[5] = inbuf[1];
		z_buf[6] = inbuf[1];
		z_buf[7] = inbuf[1];
		z_buf[8] = inbuf[2];
		z_buf[9] = inbuf[2];
		z_buf[10] = inbuf[2];
		z_buf[11] = inbuf[2];
		
		w_buf[0] = w_normal_cp[sequence_idx][0];
		w_buf[1] = w_normal_cp[sequence_idx][1];
		w_buf[2] = w_normal_cp[sequence_idx][2];
		w_buf[3] = w_normal_cp[sequence_idx][3];
		w_buf[4] = w_normal_cp[sequence_idx][4];
		w_buf[5] = w_normal_cp[sequence_idx][5];
		w_buf[6] = w_normal_cp[sequence_idx][6];
		w_buf[7] = w_normal_cp[sequence_idx][7];
		w_buf[8] = w_normal_cp[sequence_idx][8];
		w_buf[9] = w_normal_cp[sequence_idx][9];
		w_buf[10] = w_normal_cp[sequence_idx][10];
		w_buf[11] = w_normal_cp[sequence_idx][11];
		
		for (i=0; i<12; i++)       // for each symb of each phich seq
		{				 
			outbuf[i].re = z_buf[i].re*w_buf[i].re - z_buf[i].im*w_buf[i].im;
			outbuf[i].im = z_buf[i].re*w_buf[i].im + z_buf[i].im*w_buf[i].re;
		}

		outlen = 12;
	}
	else
	{
		z_buf[0] = inbuf[0];
		z_buf[1] = inbuf[0];
		z_buf[2] = inbuf[1];
		z_buf[3] = inbuf[1];
		z_buf[4] = inbuf[2];
		z_buf[5] = inbuf[2];
		
		w_buf[0] = w_extend_cp[sequence_idx][0];
		w_buf[1] = w_extend_cp[sequence_idx][1];
		w_buf[2] = w_extend_cp[sequence_idx][2];
		w_buf[3] = w_extend_cp[sequence_idx][3];
		w_buf[4] = w_extend_cp[sequence_idx][4];
		w_buf[5] = w_extend_cp[sequence_idx][5];
		
		for (i=0; i<6; i++)       // for each symb of each phich seq
		{				 
			outbuf[i].re = z_buf[i].re*w_buf[i].re - z_buf[i].im*w_buf[i].im;
			outbuf[i].im = z_buf[i].re*w_buf[i].im + z_buf[i].im*w_buf[i].re;
		}

		outlen = 6;
	} 
	
	return outlen;
}

// to enable resource group alignment
int grpallign_phich(cfloat *inbuf, int inlen, int cp_cfg, int group_idx, cfloat *outbuf)
{	
	int i;

	if (cp_cfg == BSTX_NORMAL_CP)
	{
		for (i=0; i<12; i++)
		{
			outbuf[i] = inbuf[i];
		}
	}
	else
	{	
		if(group_idx%2 == 0)
		{
			for(i=0; i<3; i++)
			{
				outbuf[4*i+0] = inbuf[2*i+0];
				outbuf[4*i+1] = inbuf[2*i+1];
				outbuf[4*i+2] = ZEROCFLOAT;
				outbuf[4*i+3] = ZEROCFLOAT;
			}
		}
		else
		{
			for(i=0; i<3; i++)
			{
				outbuf[4*i+0] = ZEROCFLOAT;
				outbuf[4*i+1] = ZEROCFLOAT;
				outbuf[4*i+2] = inbuf[2*i+0];
				outbuf[4*i+3] = inbuf[2*i+1];
			}
		}
	}

	return 12;
}



int mapunit_phich(cfloat **inbuf, int inlen, int num_bstx_ant, int cp_cfg, int num_grp_phich
								   , int mi_phich, int mux_mode, int *idx_group_phich_list, cfloat **outbuf)	
{    
	int i, j, p;
	int num_phich;
	int num_map_unit_phich; // the number of group

	cfloat ym_buf[1200];
	    
	num_phich = inlen/12; // the number of user

	if(cp_cfg == BSTX_NORMAL_CP)	
	{
		if(mux_mode == BSTX_TDD_MUXMODE)		num_map_unit_phich = mi_phich * num_grp_phich;
		else if(mux_mode == BSTX_FDD_MUXMODE)	num_map_unit_phich = num_grp_phich;
		else									assert(0);
	}
	else
	{
		if(mux_mode == BSTX_TDD_MUXMODE)			num_map_unit_phich = mi_phich * num_grp_phich / 2;
		else if(mux_mode == BSTX_FDD_MUXMODE)	num_map_unit_phich = num_grp_phich / 2;
		else								assert(0);
	}
	
	// tranfer to mapping unit, the sum of all PHICHs in each PHICH group
	for (p=0; p<num_bstx_ant; p++)
	{
		// sum PHICH sequences in the same PHICH group
		memset(ym_buf, 0, sizeof(cfloat)*1200);
		
		for(i=0; i<num_phich; i++)
		{
			for(j=0; j<12; j++)
			{
				cfloataddequalcfloat(&ym_buf[12*idx_group_phich_list[i]+j],inbuf[p][12*i+j]);
			}				
		}
		
		// PHICH groups are mapped to PHICH mapping units.
		if (cp_cfg == BSTX_NORMAL_CP)                            
		{
			for(i=0; i<num_map_unit_phich; i++)
			{
				for(j=0; j<12; j++)
				{
					outbuf[p][12*i+j] = ym_buf[12*i+j];//2015-11-10xiugai
				}
			}			
		}   			
		else
		{
			for(i=0; i<num_map_unit_phich; i++)
			{
				for(j=0; j<12; j++)
				{
					outbuf[p][12*i+j] = add(ym_buf[12*(2*i+0)+j],ym_buf[12*(2*i+1)+j]);//2015-11-10-xiugai
				}
			}
		}	// end if (cp_cfg == 0)

	} // end for (p=0; p<num_bstx_ant; p++)


	return 12*num_map_unit_phich;
}
