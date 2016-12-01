#include "foo.h"
#include "bstx_mod_pdcch.h"
#include "bstx_define.h"



int multiplex_pdcch(int16 **inbuf, int *pdcch_format_list, int num_pdcch
					, int num_grp_phich, int mi_phich, int fdd_tdd_mode, int num_ofdm_pdcch, int num_rb_dl
					, int num_bstx_ant, int cp_cfg, int16 *outbuf)
{
	int i, k;
	int outlen;
	int pdcch_format;
    int num_bit_pdcch;
	int base_addr_cce_start;

	int num_REG_phich;
	int num_REG_pcfich;
	int num_REG_pdcch;
	int num_REG_sum;
	int num_REG_ofdm[4];	
	
	// calculate the REG number available for PDCCH 		
	num_REG_pcfich = 4;

	if(fdd_tdd_mode == BSTX_TDD_MUXMODE)		
		num_REG_phich = 3 * mi_phich * num_grp_phich;
	else if(fdd_tdd_mode == BSTX_FDD_MUXMODE)	
		num_REG_phich = 3 * num_grp_phich;
	else										
		assert(0);
	
	num_REG_ofdm[0] = 2*num_rb_dl;
	num_REG_ofdm[1] = (num_bstx_ant<=2) ? (3*num_rb_dl) : (2*num_rb_dl);
	num_REG_ofdm[2] = 3*num_rb_dl;
	num_REG_ofdm[3] = (cp_cfg==0) ? (3*num_rb_dl) : (2*num_rb_dl);
	
	num_REG_sum = 0;
	
	for (i=0; i<num_ofdm_pdcch; i++)
	{
		num_REG_sum += num_REG_ofdm[i];
	}
	
	num_REG_pdcch = num_REG_sum - num_REG_pcfich - num_REG_phich;
	
	// padding null bits		
	outlen = 8 * num_REG_pdcch; // M_tot, huangshoujun, right
// 	outlen = 8 * 9 * (num_REG_pdcch / 9); // M_tot, zuohuanhuan, wrong

	memset(outbuf, 0, sizeof(int16) * outlen); // linjiangnan


	base_addr_cce_start = 0;
	
	for (k=0; k<num_pdcch; k++)
	{
		pdcch_format  = pdcch_format_list[k];

		num_bit_pdcch = 72 * (1 << pdcch_format);
		
		for(i=0; i<num_bit_pdcch; i++)
		{
			outbuf[i+base_addr_cce_start] = inbuf[k][i];
		}
		
		base_addr_cce_start += num_bit_pdcch;
	}
	
	
	return outlen;
}



int interleaver_pdcch(cfloat *inbuf, int inlen, int num_id_cell, cfloat *outbuf)
{
	int i,j,k;
	int idx;
	int num_col;
    int num_row;
	int num_quadruplet;
	int num_null_quadruplet;
	
	cfloat z_buf[25][32][4];
    cfloat y_buf[3200];
	
	int P[32] = {1,17,9,25,5,21,13,29,3,19,11,27,7,23,15,31,0,16,8,24,4,20,12,28,2,18,10,26,6,22,14,30};	
	
	// function body
	num_quadruplet = inlen / 4;
	num_col = 32;	
	num_row = (num_quadruplet+num_col-1) / num_col;			// calculate the row number of the interleaver matrix			
	num_null_quadruplet = num_row*num_col - num_quadruplet; // null quadruplets inserted before the data stream

	// insert null data and write the inbuf to the matrix row by row  in terms of quard symbol 	
	idx = 0;

	for (i=0; i<num_row; i++)
	{
		for (j=0; j<num_col; j++)
		{
			if(i==0 && j<num_null_quadruplet)
				continue;

			z_buf[i][j][0] = inbuf[idx+0];
			z_buf[i][j][1] = inbuf[idx+1];
			z_buf[i][j][2] = inbuf[idx+2];
			z_buf[i][j][3] = inbuf[idx+3];
			idx += 4;
		}
	}
	
	// change the column, then read out from matrix column by column and delete the null data		
	idx = 0;
	
	for (j=0; j<num_col; j++)
	{
		k = P[j];
		
		for (i=0; i<num_row; i++)
		{
			if(i==0 && k<num_null_quadruplet)
				continue;

			y_buf[idx+0] = z_buf[i][k][0];
			y_buf[idx+1] = z_buf[i][k][1];
			y_buf[idx+2] = z_buf[i][k][2];
			y_buf[idx+3] = z_buf[i][k][3];
			idx += 4;
		}
	}	
	
	// cyclic shift
	for (i=0; i<num_quadruplet; i++)
	{
		j = (i+num_id_cell) % num_quadruplet;
		
		outbuf[4*i+0] = y_buf[4*j+0];
		outbuf[4*i+1] = y_buf[4*j+1];
		outbuf[4*i+2] = y_buf[4*j+2];
		outbuf[4*i+3] = y_buf[4*j+3];
	}	
	
	return inlen;	
}



