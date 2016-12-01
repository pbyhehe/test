#include "foo.h"
#include "uerx_desymbintelv_pdcch.h"


int desymbintelv_pdcch(cfloat *inbuf, int inlen, int num_id_cell, cfloat *outbuf)
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
	
	// De cyclic shift
	for (i=0; i<num_quadruplet; i++)
	{
		j = (i-num_id_cell) % num_quadruplet;
		j = (j < 0) ? j+num_quadruplet : j;
		
		y_buf[4*i+0] = inbuf[4*j+0];
		y_buf[4*i+1] = inbuf[4*j+1];
		y_buf[4*i+2] = inbuf[4*j+2];
		y_buf[4*i+3] = inbuf[4*j+3];
	}

	// insert null data and change colmn	
	idx = 0;
	
	for (j=0; j<num_col; j++)
	{
		k = P[j];
		
		for (i=0; i<num_row; i++)
		{
			if(i==0 && k<num_null_quadruplet)
				continue;

			z_buf[i][k][0] = y_buf[idx+0];
			z_buf[i][k][1] = y_buf[idx+1];
			z_buf[i][k][2] = y_buf[idx+2];
			z_buf[i][k][3] = y_buf[idx+3];
			idx += 4;
		}
	}

	// read from the matrix row by row  in terms of quard symbol and delete the null data
	idx = 0;

	for (i=0; i<num_row; i++)
	{
		for (j=0; j<num_col; j++)
		{
			if(i==0 && j<num_null_quadruplet)
				continue;

			outbuf[idx+0] = z_buf[i][j][0];
			outbuf[idx+1] = z_buf[i][j][1];
			outbuf[idx+2] = z_buf[i][j][2];
			outbuf[idx+3] = z_buf[i][j][3];
			idx += 4;
		}
	}	
	
	return inlen;	
}

