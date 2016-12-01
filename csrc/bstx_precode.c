#include "foo.h"
#include "bstx_precode.h"
#include "bstx_define.h"




int precode_siso(cfloat *inbuf, int inlen, cfloat *outbuf)
{
	int i;

	for(i=0; i<inlen; i++)
	{
		outbuf[i] = inbuf[i];
	}

	return inlen;
}


int precode_diversity(cfloat **inbuf, int inlen, int inlen_layer_cw0, int num_layers, cfloat **outbuf)
{
	int i;
	int outlen;

	static cfloat A_mem[16][8];
	static cfloat *A[16]=
	{
		A_mem[0], A_mem[1], A_mem[2],  A_mem[3],  A_mem[4],  A_mem[5],  A_mem[6],  A_mem[7],
		A_mem[8], A_mem[9], A_mem[10], A_mem[11], A_mem[12], A_mem[13], A_mem[14], A_mem[15]
	};
	
	static cfloat re_im_mem[16][7500];
	static cfloat *re_im_buf[16]=
	{
		re_im_mem[0], re_im_mem[1], re_im_mem[2],  re_im_mem[3],  re_im_mem[4],  re_im_mem[5],  re_im_mem[6],  re_im_mem[7],
		re_im_mem[8], re_im_mem[9], re_im_mem[10], re_im_mem[11], re_im_mem[12], re_im_mem[13], re_im_mem[14], re_im_mem[15]
	};

	static cfloat y_mem[16][7500];
	static cfloat *y_buf[16] = 
	{
		y_mem[0], y_mem[1], y_mem[2],  y_mem[3],  y_mem[4],  y_mem[5],  y_mem[6],  y_mem[7],
		y_mem[8], y_mem[9], y_mem[10], y_mem[11], y_mem[12], y_mem[13], y_mem[14], y_mem[15]
	};


	if(num_layers == 2)
	{ 		
		precode_matrix(A, A_ANT2_ID);

		//memset(re_im_buf,0,sizeof(cfloat)*inlen*2);
		for (i=0; i<inlen; i++)
		{
			re_im_buf[0][i].re = inbuf[0][i].re;
			re_im_buf[1][i].re = inbuf[1][i].re;
			re_im_buf[2][i].re = inbuf[0][i].im;
			re_im_buf[3][i].re = inbuf[1][i].im;
		}

		matrix_mult(A, 4, 4, re_im_buf, 4, inlen, y_buf);

		for (i=0; i<inlen; i++)
		{
			// transmition on antenna port 0
			outbuf[0][2*i+0] = y_buf[0][i];
			outbuf[0][2*i+1] = y_buf[2][i];
			
			// transmition on antenna port 1
			outbuf[1][2*i+0] = y_buf[1][i];
			outbuf[1][2*i+1] = y_buf[3][i];
		}		
		
		outlen = 2*inlen;
	}
	else if(num_layers == 4)
	{
		precode_matrix(A, A_ANT4_ID);
		
		//memset(re_im_buf, 0, sizeof(cfloat)*inlen*2);
		for (i=0; i<inlen; i++)
		{
			re_im_buf[0][i].re = inbuf[0][i].re;
			re_im_buf[1][i].re = inbuf[1][i].re;
			re_im_buf[2][i].re = inbuf[2][i].re;
			re_im_buf[3][i].re = inbuf[3][i].re;
			re_im_buf[4][i].re = inbuf[0][i].im;
			re_im_buf[5][i].re = inbuf[1][i].im;
			re_im_buf[6][i].re = inbuf[2][i].im;
			re_im_buf[7][i].re = inbuf[3][i].im;
		}

		matrix_mult(A, 16, 8, re_im_buf, 8, inlen, y_buf);

		for (i=0; i<inlen; i++)
		{
			// transmition on antenna port 0
			outbuf[0][4*i+0] = y_buf[0][i];
			outbuf[0][4*i+1] = y_buf[4][i];
			outbuf[0][4*i+2] = y_buf[8][i];
			outbuf[0][4*i+3] = y_buf[12][i];
			
			// transmition on antenna port 1
			outbuf[1][4*i+0] = y_buf[1][i];
			outbuf[1][4*i+1] = y_buf[5][i];
			outbuf[1][4*i+2] = y_buf[9][i];
			outbuf[1][4*i+3] = y_buf[13][i];

			// transmition on antenna port 2
			outbuf[2][4*i+0] = y_buf[2][i];
			outbuf[2][4*i+1] = y_buf[6][i];
			outbuf[2][4*i+2] = y_buf[10][i];
			outbuf[2][4*i+3] = y_buf[14][i];
			
			// transmition on antenna port 3
			outbuf[3][4*i+0] = y_buf[3][i];			
			outbuf[3][4*i+1] = y_buf[7][i];			
			outbuf[3][4*i+2] = y_buf[11][i];			
			outbuf[3][4*i+3] = y_buf[15][i];
		}	
		
		outlen = inlen_layer_cw0;
	}	

	return outlen;
}



int precode_spatial(cfloat **inbuf, int inlen, int num_layers, int num_bstx_ant, int cdd_cfg
								 , int codebook_idx, cfloat **outbuf)
{
	int i, j, p;
	int outlen;

	static cfloat U_mem[4][4];
	static cfloat *U[4] = {U_mem[0], U_mem[1], U_mem[2],  U_mem[3]};

	static cfloat D_mem[4][4];
	static cfloat *D[4] = {D_mem[0], D_mem[1], D_mem[2],  D_mem[3]};

	static cfloat W_mem[4][4];
	static cfloat *W[4] = {W_mem[0], W_mem[1], W_mem[2],  W_mem[3]};

	static cfloat W_D_mem[4][4];
	static cfloat *W_D[4] = {W_D_mem[0], W_D_mem[1], W_D_mem[2],  W_D_mem[3]};

	static cfloat W_D_U_mem[4][4];
	static cfloat *W_D_U[4] = {W_D_U_mem[0], W_D_U_mem[1], W_D_U_mem[2],  W_D_U_mem[3]};

	static cfloat u_n_mem[4][1];
	static cfloat *u_n[4] = {u_n_mem[0], u_n_mem[1], u_n_mem[2],  u_n_mem[3]};
	
	static cfloat u_n_H_mem[1][4];
	static cfloat *u_n_H[1] = {u_n_H_mem[0]};
	
	static cfloat Wn_mem[4][4];
	static cfloat *Wn[4] = {Wn_mem[0], Wn_mem[1], Wn_mem[2],  Wn_mem[3]};

	static cfloat y_mem[4][15000];
	static cfloat *y_buf[16] = {y_mem[0], y_mem[1], y_mem[2],  y_mem[3]};
     
	static cfloat x_col_mem[4][1];
	static cfloat *x_col[4] = {x_col_mem[0], x_col_mem[1], x_col_mem[2],  x_col_mem[3]};

	static cfloat y_col_mem[4][1];
	static cfloat *y_col[4] = {y_col_mem[0], y_col_mem[1], y_col_mem[2],  y_col_mem[3]};
	

	switch(num_layers) // init U
	{
	case 2:
		precode_matrix(U, U_LAY2_ID);
		break;

	case 3:
		precode_matrix(U, U_LAY3_ID);
		break;

	case 4:
		precode_matrix(U, U_LAY4_ID);
		break;

	default:
		break;
	}

	//init the u_n
	switch (codebook_idx)
	{
	case 0:
		precode_matrix(u_n, U0_ID);	
		break;

	case 1:
		precode_matrix(u_n, U1_ID);
		break;

	case 2:
		precode_matrix(u_n, U2_ID);
		break;

	case 3:
		precode_matrix(u_n, U3_ID);
		break;

	case 4:
		precode_matrix(u_n, U4_ID);
		break;

	case 5:
		precode_matrix(u_n, U5_ID);
		break;

	case 6:
		precode_matrix(u_n, U6_ID);
		break;

	case 7:
		precode_matrix(u_n, U7_ID);
		break;

	case 8:
		precode_matrix(u_n, U8_ID);
		break;

	case 9:
		precode_matrix(u_n, U9_ID);
		break;

	case 10:
		precode_matrix(u_n, U10_ID);
		break;

	case 11:
		precode_matrix(u_n, U11_ID);
		break;

	case 12:
		precode_matrix(u_n, U12_ID);
		break;

	case 13:
		precode_matrix(u_n, U13_ID);
		break;

	case 14:
		precode_matrix(u_n, U14_ID);
		break;

	case 15:
		precode_matrix(u_n, U15_ID);
		break;

	default:
		;
	}                     
	
	//use the u_n to get Wn
	for (i=0; i<4; i++)
	{
		u_n_H[i][0].re =  u_n[0][i].re;
		u_n_H[i][0].im = -u_n[0][i].im;
	}

	//Wn = I - 2u_n*u_n_H/u_n_H*u_n
	matrix_mult(u_n, 4, 1, u_n_H, 1, 4, Wn);
	
	for (i=0; i<4; i++)								   		               
	{
		for (j=0; j<4; j++)
		{
			if(i == j)
			{
				Wn[i][j].re = 1 - 2*Wn[i][j].re/4;
				Wn[i][j].im = 0 - 2*Wn[i][j].im/4;
			}
			else
			{
				Wn[i][j].re = 0 - 2*Wn[i][j].re/4;
				Wn[i][j].im = 0 - 2*Wn[i][j].im/4;
			}
		}
	}

	if (num_bstx_ant == 2) // to compute W
	{
		if (num_layers == 1)
		{
			switch (codebook_idx)
			{
			case 0:
				precode_matrix(W, W_ANT2_LAY1_CODIDX0_ID);	
				break;

			case 1:
				precode_matrix(W, W_ANT2_LAY1_CODIDX1_ID);	
				break;

			case 2:
				precode_matrix(W, W_ANT2_LAY1_CODIDX2_ID);	
				break;

			case 3:
				precode_matrix(W, W_ANT2_LAY1_CODIDX3_ID);	
				break;
			}
		} 
		else if(num_layers == 2)
		{
			switch (codebook_idx)
			{
			case 0:
				precode_matrix(W, W_ANT2_LAY2_CODIDX0_ID);	
				break;

			case 1:
				precode_matrix(W, W_ANT2_LAY2_CODIDX1_ID);	
				break;

			case 2:
				precode_matrix(W, W_ANT2_LAY2_CODIDX2_ID);	
				break;
			}
		}
	} 
	else if(num_bstx_ant == 4)
	{
		if (num_layers == 1)
		{
			for (i=0; i<4; i++)
			{
				W[i][0].re = Wn[i][0].re;
				W[i][0].im = Wn[i][0].im;
			}
		} 
		else if(num_layers == 2)
		{
			if (codebook_idx == 0 || codebook_idx == 4 || codebook_idx == 5 || codebook_idx == 9 )
			{
				for (i=0; i<4; i++)
				{
					W[i][0].re = Wn[i][0].re / sqrt(2);
					W[i][0].im = Wn[i][0].im / sqrt(2);
					W[i][1].re = Wn[i][3].re / sqrt(2);
					W[i][1].im = Wn[i][3].im / sqrt(2);
				}
			}
			else if(codebook_idx == 1 || codebook_idx == 2 || codebook_idx == 3 || codebook_idx == 8 
				    || codebook_idx == 12 || codebook_idx == 15)
			{
				for (i=0; i<4; i++)
				{
					W[i][0].re = Wn[i][0].re / sqrt(2);
					W[i][0].im = Wn[i][0].im / sqrt(2);
					W[i][1].re = Wn[i][1].re / sqrt(2);
					W[i][1].im = Wn[i][1].im / sqrt(2);
				}
			}
			else
			{
				for (i=0; i<4; i++)
				{
					W[i][0].re = Wn[i][0].re / sqrt(2);
					W[i][0].im = Wn[i][0].im / sqrt(2);
					W[i][1].re = Wn[i][2].re / sqrt(2);
					W[i][1].im = Wn[i][2].im / sqrt(2);
				}
			}
		}
		else if(num_layers == 3)
		{
			if (codebook_idx == 0 || codebook_idx == 4 || codebook_idx == 5 || codebook_idx == 8)
			{
				for (i=0; i<4; i++)
				{
					W[i][0].re = Wn[i][0].re / sqrt(3);
					W[i][0].im = Wn[i][0].im / sqrt(3);
					W[i][1].re = Wn[i][1].re / sqrt(3);
					W[i][1].im = Wn[i][1].im / sqrt(3);
					W[i][2].re = Wn[i][3].re / sqrt(3);
					W[i][2].im = Wn[i][3].im / sqrt(3);
				}
			}
			else if (codebook_idx == 6 || codebook_idx == 7 || codebook_idx == 9 || codebook_idx == 11)
			{
				for (i=0; i<4; i++)
				{
					W[i][0].re = Wn[i][0].re / sqrt(3);
					W[i][0].im = Wn[i][0].im / sqrt(3);
					W[i][1].re = Wn[i][2].re / sqrt(3);
					W[i][1].im = Wn[i][2].im / sqrt(3);
					W[i][2].re = Wn[i][3].re / sqrt(3);
					W[i][2].im = Wn[i][3].im / sqrt(3);
				}
			}
			else
			{
				for (i=0; i<4; i++)
				{
					W[i][0].re = Wn[i][0].re / sqrt(3);
					W[i][0].im = Wn[i][0].im / sqrt(3);
					W[i][1].re = Wn[i][1].re / sqrt(3);
					W[i][1].im = Wn[i][1].im / sqrt(3);
					W[i][2].re = Wn[i][2].re / sqrt(3);
					W[i][2].im = Wn[i][2].im / sqrt(3);
				}
			}
		}
		else if(num_layers == 4)
		{
			if (codebook_idx == 2 || codebook_idx == 3 || codebook_idx == 14 || codebook_idx == 15)
			{
				for (i=0; i<4; i++)
				{
					W[i][0].re = Wn[i][2].re / 2;
					W[i][0].im = Wn[i][2].im / 2;
					W[i][1].re = Wn[i][1].re / 2;
					W[i][1].im = Wn[i][1].im / 2;
					W[i][2].re = Wn[i][0].re / 2;
					W[i][2].im = Wn[i][0].im / 2;
					W[i][3].re = Wn[i][3].re / 2;
					W[i][3].im = Wn[i][3].im / 2;
				}
			}
			else if (codebook_idx == 6 || codebook_idx == 7 || codebook_idx == 10 || codebook_idx == 11)
			{
				for (i=0; i<4; i++)
				{
					W[i][0].re = Wn[i][0].re / 2;
					W[i][0].im = Wn[i][0].im / 2;
					W[i][1].re = Wn[i][2].re / 2;
					W[i][1].im = Wn[i][2].im / 2;
					W[i][2].re = Wn[i][1].re / 2;
					W[i][2].im = Wn[i][1].im / 2;
					W[i][3].re = Wn[i][3].re / 2;
					W[i][3].im = Wn[i][3].im / 2;
				}
			}
			else
			{
				for (i=0; i<4; i++)
				{
					W[i][0].re = Wn[i][0].re / 2;
					W[i][0].im = Wn[i][0].im / 2;
					W[i][1].re = Wn[i][1].re / 2;
					W[i][1].im = Wn[i][1].im / 2;
					W[i][2].re = Wn[i][2].re / 2;
					W[i][2].im = Wn[i][2].im / 2;
					W[i][3].re = Wn[i][3].re / 2;
					W[i][3].im = Wn[i][3].im / 2;
				}
			}
		}
	} //end init W


	// precoding	
	if (cdd_cfg == 0 && (num_bstx_ant == 2 || num_bstx_ant == 4))	// without cdd_cfg
	{									
		matrix_mult(W, num_bstx_ant, num_layers, inbuf, num_layers, inlen, outbuf);

		outlen = inlen;
	}
	else if(cdd_cfg==1 && (num_bstx_ant==2 || num_bstx_ant==4))		// large delay cdd_cfg
	{												       
		for (i=0; i<inlen; i++)
		{	
			memset(D[0], 0, sizeof(cfloat)*4);                 // clear D
			memset(D[1], 0, sizeof(cfloat)*4);                 // clear D
			memset(D[2], 0, sizeof(cfloat)*4);                 // clear D
			memset(D[3], 0, sizeof(cfloat)*4);                 // clear D

			switch (num_layers)
			{
			case 2:
				D[0][0].re = 1;						    // D = [1  0; 0	exp(-j2*PI*i/2)]
				D[0][0].im = 0;
				D[1][1].re = (float)(cos(-2.0*PI*i/2));		
				D[1][1].im = (float)(sin(-2.0*PI*i/2));
				break;
			case 3:									    // D = 
				D[0][0].re = 1;							//	{				
				D[0][0].im = 0;
				D[1][1].re = (float)(cos(-2.0*PI*i/3)); //		1			0			0
				D[1][1].im = (float)(sin(-2.0*PI*i/3)); //		0	exp(-j2*PI*i/3)		0
				D[2][2].re = (float)(cos(-4.0*PI*i/3)); //		0			0		exp(-j4*PI*i/3)
				D[2][2].im = (float)(sin(-4.0*PI*i/3)); //  }
				break;
			case 4:										// D = 
				D[0][0].re = 1;							//	{	
				D[0][0].im = 0;
				D[1][1].re = (float)(cos(-2.0*PI*i/4)); //	   1	0			 0			0
				D[1][1].im = (float)(sin(-2.0*PI*i/4)); //	   0 exp(-j2*PI*i/4) 0			0
				D[2][2].re = (float)(cos(-4.0*PI*i/4)); //	   0	0		exp(-j4*PI*i/4)	0
				D[2][2].im = (float)(sin(-4.0*PI*i/4)); //	   0	0			 0	 exp(-j6*PI*i/4)	
				D[3][3].re = (float)(cos(-6.0*PI*i/4)); //  }
				D[3][3].im = (float)(sin(-6.0*PI*i/4));
				break;
			default:
				assert(0);
			}	                                           //end init D
			                             
			// compute the W * D * U. the result is sent to W_D_U_mult
			matrix_mult(W, num_bstx_ant, num_layers, D,	num_layers, num_layers, W_D);
			matrix_mult(W_D, num_bstx_ant, num_layers, U, num_layers, num_layers, W_D_U);	

			// compute the i col outbuf
			for (j=0; j<num_layers; j++)
			{
				x_col[j][0] = inbuf[j][i]; 
			}

			matrix_mult(W_D_U, num_bstx_ant, num_layers, x_col, num_layers, 1, y_col);

			for (p=0; p<num_bstx_ant; p++)
			{
				outbuf[p][i] = y_col[p][0]; 
			}                                              // end compute the i col outbuf
		}                                                  // end compute the outbuf

		outlen = inlen;
	}
	else
	{											
		assert(0);
	}

	return outlen;
}


int precode_pcfich(cfloat **inbuf, int inlen, int num_bstx_ant, cfloat **outbuf)
{
	int outlen;

	if(num_bstx_ant == 1)
	{
		outlen = precode_siso(inbuf[0], inlen, outbuf[0]);
	}
	else
	{
		outlen = precode_diversity(inbuf, inlen, 16, num_bstx_ant, outbuf);
	}

	return outlen;
}


int precode_pdcch(cfloat **inbuf, int inlen, int inlen_layer_cw0, int num_bstx_ant, cfloat **outbuf)
{
	int outlen;

	if(num_bstx_ant == 1)
	{
		outlen = precode_siso(inbuf[0], inlen, outbuf[0]);
	}
	else
	{
		outlen = precode_diversity(inbuf, inlen, inlen_layer_cw0, num_bstx_ant, outbuf);
	}

	return outlen;
}

int precode_pbch(cfloat **inbuf, int inlen, int inlen_layer_cw0, int num_bstx_ant, cfloat **outbuf)
{
	int outlen;

	if(num_bstx_ant == 1)
	{
		outlen = precode_siso(inbuf[0], inlen, outbuf[0]);
	}
	else
	{
		outlen = precode_diversity(inbuf, inlen, inlen_layer_cw0, num_bstx_ant, outbuf);
	}

	return outlen;
}


int precode_phich(cfloat **inbuf, int inlen, int num_bstx_ant, int group_idx, int cp_cfg, cfloat **outbuf)
{
	int i;
	
	static cfloat A_mem[16][8];
	static cfloat *A[16]=
	{
		A_mem[0], A_mem[1], A_mem[2],  A_mem[3],  A_mem[4],  A_mem[5],  A_mem[6],  A_mem[7],
		A_mem[8], A_mem[9], A_mem[10], A_mem[11], A_mem[12], A_mem[13], A_mem[14], A_mem[15]
	};

	static cfloat x_col_mem[8][1];
	static cfloat *x_col[8] = 
	{
		x_col_mem[0], x_col_mem[1], x_col_mem[2],  x_col_mem[3],  x_col_mem[4],  x_col_mem[5],  x_col_mem[6],  x_col_mem[7]
	};

	static cfloat y_col_mem[16][1];
	static cfloat *y_col[16] = 
	{
		y_col_mem[0], y_col_mem[1], y_col_mem[2],  y_col_mem[3],  y_col_mem[4],  y_col_mem[5],  y_col_mem[6],  y_col_mem[7],
		y_col_mem[8], y_col_mem[9], y_col_mem[10], y_col_mem[11], y_col_mem[12], y_col_mem[13], y_col_mem[14], y_col_mem[15]
	};


	if(num_bstx_ant == 1)
	{
		precode_siso(inbuf[0], inlen, outbuf[0]);
	}
	else if(num_bstx_ant == 2)
	{
		precode_diversity(inbuf, inlen, 12, num_bstx_ant, outbuf);
	}
	else
	{
		for (i=0; i<3; i++) // 4 antennas precoding for phich
		{
			if (   (cp_cfg == BSTX_NORMAL_CP && ((i+group_idx)%2 == 0)) 
				|| (cp_cfg == BSTX_EXTEND_CP && ((i+group_idx/2)%2 == 0)) )
			{
				precode_matrix(A, A1_PHICH_ID);
			}
			else
			{
				precode_matrix(A, A2_PHICH_ID);
			}
			
			x_col[0][i].re = inbuf[0][i].re;
			x_col[1][i].re = inbuf[1][i].re;
			x_col[2][i].re = inbuf[2][i].re;
			x_col[3][i].re = inbuf[3][i].re;
			x_col[4][i].re = inbuf[0][i].im;
			x_col[5][i].re = inbuf[1][i].im;
			x_col[6][i].re = inbuf[2][i].im;
			x_col[7][i].re = inbuf[3][i].im;
			
			matrix_mult(A, 16, 8, x_col, 8, 1, y_col);
			
			// transmition on antenna port 0
			outbuf[0][4*i+0] = y_col[0][0]; 
			outbuf[0][4*i+1] = y_col[4][0]; 
			outbuf[0][4*i+2] = y_col[8][0]; 
			outbuf[0][4*i+3] = y_col[12][0]; 
			
			// transmition on antenna port 1
			outbuf[1][4*i+0] = y_col[1][0]; 
			outbuf[1][4*i+1] = y_col[5][0]; 
			outbuf[1][4*i+2] = y_col[9][0]; 
			outbuf[1][4*i+3] = y_col[13][0]; 

			// transmition on antenna port 2
			outbuf[2][4*i+0] = y_col[2][0]; 
			outbuf[2][4*i+2] = y_col[6][0]; 
			outbuf[2][4*i+3] = y_col[10][0]; 
			outbuf[2][4*i+4] = y_col[14][0]; 

			// transmition on antenna port 3
			outbuf[3][4*i+0] = y_col[3][0];		
			outbuf[3][4*i+1] = y_col[7][0];		
			outbuf[3][4*i+2] = y_col[11][0]; 		
			outbuf[3][4*i+3] = y_col[15][0];
		}
	}

	return 12;
}



int precode_pdsch(cfloat **inbuf, int inlen, int inlen_layer_cw0, int num_layers, int num_bstx_ant
								 , int transmit_scheme, int cdd_cfg, int codebook_idx, cfloat **outbuf)
{
	int outlen;

	if (transmit_scheme == BSTX_SISO)
	{
		outlen = precode_siso(inbuf[0], inlen, outbuf[0]);
	}
	else if (transmit_scheme == BSTX_DIVERSITY)
	{
		outlen = precode_diversity(inbuf, inlen, inlen_layer_cw0, num_bstx_ant, outbuf);		
	}
	else // spatial
	{
		outlen = precode_spatial(inbuf, inlen, num_layers, num_bstx_ant, cdd_cfg, codebook_idx, outbuf);
	}

	return outlen;	
}



int matrix_mult(cfloat **inbuf0, int num_row0, int num_col0, cfloat **inbuf1, int num_row1, int num_col1
							  , cfloat **outbuf)
{
	int i, j, k;

	cfloat sum, a_i_k, b_k_j;

	assert(num_col0 == num_row1);

	for (i=0; i<num_row0; i++)
	{
		for (j=0; j<num_col1; j++)
		{
			sum.re = 0;
			sum.im = 0;

			for (k=0; k<num_col0; k++)
			{
				a_i_k =	inbuf0[i][k];
				b_k_j =	inbuf1[k][j];

				sum.re += a_i_k.re*b_k_j.re - a_i_k.im*b_k_j.im;
				sum.im += a_i_k.re*b_k_j.im + a_i_k.im*b_k_j.re;
			}

			outbuf[i][j].re = sum.re;
			outbuf[i][j].im = sum.im;
		}
	}

	return num_row0*num_col1;
}




int precode_matrix(cfloat **outbuf, int matrix_type)
{
	int i, j;
	int outlen;	


	/*                                                     
	* 2 num_layers
	* U_re_lay2 = 1/sqrt(2) * 
	*		{
	*			1			1
	*			1		exp(-j*2*PI/2)
	*		}
	*/
	static float U_re_lay2[2][2] =											
	{																
		{0.707107f,	 0.707107f},		
		{0.707107f,	-0.707107f}		
	};
	static float U_im_lay2[2][2]=	
	{
		{0.0f,	0.0f},
		{0.0f,	0.0f}
	};	
	
	/*
	* 3 num_layers
	* U = 1.0 /sqrt(3.0) *
	*	{
	*		1		1				1
	*		1	exp(-j*2*PI/3)	exp(-j*4*PI/3)
	*		1	exp(-j*4*PI/3)	exp(-j*8*PI/3)
	*	}
	*/
	static float U_re_lay3[3][3] =											
	{																
		{0.577351f,  0.577350f,	 0.577350f},									
		{0.577351f, -0.288675f, -0.288675f},							
		{0.577351f, -0.288675f, -0.288675f}							
	};
	static float U_im_lay3[3][3] = 
	{																
		{0.0f,  0.0f,  0.0f},									
		{0.0f, -0.5f,  0.5f},							
		{0.0f,  0.5f, -0.5f}							
	};	

	/*
	* 4 num_layers
	* U = 1/2 *
	*	{
	*		1			1				1				1
	*		1	exp(-j*2*PI/4)	exp(-j*4*PI/4)	exp(-j*6*PI/4)
	*		1	exp(-j*4*PI/4)	exp(-j*8*PI/4)	exp(-j*12*PI/4)
	*		1	exp(-j*6*PI/4)	exp(-j*12*PI/4)	exp(-j*18*PI/4)
	*	}
	*/	
	static float U_re_lay4[4][4] =									
	{														
		{0.5f,    0.5f,   0.5f,   0.5f},		
		{0.5f,    0.0f,  -0.5f,   0.0f},		
		{0.5f,   -0.5f,   0.5f,  -0.5f},
		{0.5f,    0.0f,  -0.5f,   0.0f}
	};
	static float U_im_lay4[4][4] = 
	{														
		{0.0f,   0.0f,  0.0f,   0.0f},		
		{0.0f,  -0.5f,  0.0f,   0.5f},		
		{0.0f,   0.0f,  0.0f,   0.0f},
		{0.0f,   0.5f,  0.0f,  -0.5f}
	};
	
	
	/*
	* 2 antennas, 1 layer, codebook index 0
	* W = 1/sqrt(2) *
	*		{
	*			1
	*			1
	*		}
	*/
	static float W_re_ant2_lay1_codidx0[2][1] = 
	{
		{0.707107f},
		{0.707107f}
	};
	static float W_im_ant2_lay1_codidx0[2][1] = 
	{
		{0.0f},	
		{0.0f}
	};
		
	/*
	* 2 antennas, 1 layer, codebook index 1
	* W = 1/sqrt(2) *
	*		{
	*			1
	*			-1
	*		}
	*/
	static float W_re_ant2_lay1_codidx1[2][1] = 
	{
		{0.707107f},
		{-0.707107f}
	};
	static float W_im_ant2_lay1_codidx1[2][1] = 
	{
		{0.0f},
		{-0.0f}
	};
	
	/*
	* 2 antennas, 1 layer, codebook index 2
	* W = 1/sqrt(2) *
	*		{
	*			1
	*			j
	*		}
	*/
	static float W_re_ant2_lay1_codidx2[2][1] = 
	{
		{0.707107f},
		{0.0f}
	};
	static float W_im_ant2_lay1_codidx2[2][1] = 
	{
		{0.0f},
		{0.707107f}
	};
	
	
	/*
	* 2 antennas, 1 layer, codebook index 3
	* W = 1/sqrt(2) *
	*		{
	*			1
	*			-j
	*		}
	*/
	static float W_re_ant2_lay1_codidx3[2][1] = 
	{
		{0.707107f},
		{0.0f}
	};
	static float W_im_ant2_lay1_codidx3[2][1] = 
	{
		{0.0f},
		{-0.707107f}
	};
		
	/*
	* 2 antennas, 2 layer, codebook index 0
	* W = 1/sqrt(2) *
	*		{
	*			1	0
	*			0	1
	*		}
	*/
	static float W_re_ant2_lay2_codidx0[2][2] = 
	{
		{0.707107f,	0.0f},
		{0.0f,	    0.707107f}
	};
	static float W_im_ant2_lay2_codidx0[2][2] = 
	{
		{0.0f,	0.0f},
		{0.0f,	0.0f}
	};
	
	/*
	* 2 antennas, 2 layer, codebook index 1
	* W = 1/2 *
	*		{
	*			1	1
	*			1	-1
	*		}
	*/
	static float W_re_ant2_lay2_codidx1[2][2] = 
	{
		{0.5f,   0.5f},
		{0.5f,  -0.5f}
	};
	static float W_im_ant2_lay2_codidx1[2][2] = 
	{
		{0.0f,   0.0f},
		{0.0f,  -0.0f}
	};	
	
	/*
	* 2 antennas, 2 layer, codebook index 2
	* W = 1/2 *
	*		{
	*			1	1
	*			j	-j
	*		}
	*/
	static float W_re_ant2_lay2_codidx2[2][2] = 
	{
		{0.5f, 0.5f},
		{0.0f, 0.0f}
	};
	static float W_im_ant2_lay2_codidx2[2][2] = 
	{
		{0.0f, 0.0f},
		{0.5f, -0.5f}
	};
	
	
	/*
	* 4 antennas
	* init u
	* u0	=	[1 -1 -1 -1]T
	* u1	=	[1 -j  1  j]T
	* u2	=	[1  1 -1  1]T
	* u3	=	[1  j  1 -j]T
	* u4	=	[1	(-1 -j)/sqrt(2)	  -j	( 1-j)/sqrt(2)]T
	* u5	=	[1	( 1 -j)/sqrt(2)	   j	(-1-j)/sqrt(2)]T
	* u6	=	[1	( 1 +j)/sqrt(2)	  -j	(-1+j)/sqrt(2)]T
	* u7	=	[1	(-1 +j)/sqrt(2)	   j	( 1+j)/sqrt(2)]T
	* u8	=	[1 -1  1  1]T
	* u9	=	[1 -j -1 -j]T
	* u10	=	[1  1  1 -1]T
	* u11	=	[1  j -1  j]T
	* u12	=	[1 -1 -1  1]T
	* u13	=	[1 -1  1 -1]T
	* u14	=	[1  1 -1 -1]T
	* u15	=	[1  1  1  1]T
	*/
	
	static float u0_re[4][1] = 
	{
		{1.0f},    {-1.0f},    {-1.0f},    {-1.0f}
	};
	static float u0_im[4][1] = 
	{
		{0.0f},    {0.0f},    {0.0f},    {0.0f}
	};
	
	static float u1_re[4][1] = 
	{
		{1.0f},     {0.0f},     {1.0f},     {0.0f}
	};
	static float u1_im[4][1] = 
	{
		{0.0f},    {-1.0f},     {0.0f},     {1.0f}
	};	
	
	static float u2_re[4][1] = 
	{
		{1.0f},     {1.0f},    {-1.0f},     {1.0f}
	};
	static float u2_im[4][1] = 
	{
		{0.0f},     {0.0f},    {0.0f},     {0.0f}
	};
	
	static float u3_re[4][1] = 
	{
		{1.0f},     {0.0f},     {1.0f},     {0.0f}
	};	
	static float u3_im[4][1] = 
	{
		{0.0f},     {1.0f},     {0.0f},    {-1.0f}
	};	
	
	static float u4_re[4][1] = 
	{
		{1.0f},    {-0.7071f},     {0.0f},     {0.7071f}
	};	
	static float u4_im[4][1] = 
	{
		{0.0f},	{-0.7071f},    {-1.0f},    {-0.7071f}
	};	
	
	static float u5_re[4][1] = 
	{
		{1.0f},     {0.7071f},     {0.0f},   {-0.7071f}
	};	
	static float u5_im[4][1] = 
	{
		{0.0f},	{-0.7071f},     {1.0f},    {-0.7071f}
	};
	
	static float u6_re[4][1] = 
	{
		{1.0f},     {0.7071f},     {0.0f},    {-0.7071f}
	};	
	static float u6_im[4][1] = 
	{
		{0.0f},     {0.7071f},    {-1.0f},    {0.7071f}
	};
	
	static float u7_re[4][1] = 
	{
		{1.0f},    {-0.7071f},     {0.0f},     {0.7071f}
	};	
	static float u7_im[4][1] = 
	{
		{0.0f},     {0.7071f},     {1.0f},     {0.7071f}
	};
	
	static float u8_re[4][1] = 
	{
		{1.0f},    {-1.0f},     {1.0f},     {1.0f}
	};
	static float u8_im[4][1] = 
	{
		{0.0f},    {0.0f},     {0.0f},     {0.0f}
	};
	
	static float u9_re[4][1] = 
	{
		{1.0f},     {0.0f},    {-1.0f},     {0.0f}
	};	
	static float u9_im[4][1] = 
	{
		{0.0f},    {-1.0f},     {0.0f},    {-1.0f}
	};
	
	static float u10_re[4][1] = 
	{
		{1.0f},     {1.0f},     {1.0f},    {-1.0f}
	};
	static float u10_im[4][1] = 
	{
		{0.0f},     {0.0f},     {0.0f},    {-0.0f}
	};
	
	static float u11_re[4][1] = 
	{
		{1.0f},     {0.0f},    {-1.0f},     {0.0f}
	};	
	static float u11_im[4][1] = 
	{
		{0.0f},     {1.0f},     {0.0f},     {1.0f}
	};	
	
	static float u12_re[4][1] = 
	{
		{1.0f},    {-1.0f},    {-1.0f},     {1.0f}
	};
	static float u12_im[4][1] = 
	{
		{0.0f},    {0.0f},    {0.0f},     {0.0f}
	};
	
	static float u13_re[4][1] = 
	{
		{1.0f},	{-1.0f},     {1.0f},    {-1.0f}
	};
	static float u13_im[4][1] = 
	{
		{0.0f},	{0.0f},     {0.0f},    {0.0f}
	};
	
	static float u14_re[4][1] = 
	{
		{1.0f},     {1.0f},    {-1.0f},    {-1.0f}
	};
	static float u14_im[4][1] = 
	{
		{0.0f},     {0.0f},    {0.0f},    {0.0f}
	};
	
	static float u15_re[4][1] = 
	{
		{1.0f},     {1.0f},     {1.0f},     {1.0f}
	};
	static float u15_im[4][1] = 
	{
		{0.0f},     {0.0f},     {0.0f},     {0.0f}
	};
	
	/*                                                         //init A
	* 2 antennas
	* A	 = 1/sqrt(2)	*
	*	{
	*		1,   0,   j,   0,
	*		0,	-1,	  0,   j,
	*		0,	 1,	  0,   j,
	*		1,	 0,	 -j,   0
	*	}
	*/	
	static float A_re_ant2[4][4] = 
	{
		{0.7071f,   0.0f,      0.0f,   0.0f},
		{0.0f,     -0.7071f,   0.0f,   0.0f},
		{0.0f,      0.7071f,   0.0f,   0.0f},
		{0.7071f,   0.0f,      0.0f,   0.0f}
	};	
	static float A_im_ant2[4][4] = 
	{
		{0.0f,	0.0f,	0.7071f,   0.0f},
		{0.0f,	0.0f,	0.0f,	   0.7071f},
		{0.0f,	0.0f,	0.0f,	   0.7071f},
		{0.0f,	0.0f,   -0.7071f,  0.0f}
	};
	
	/*
	* 4 antennas
	* A	 = 1/sqrt(2)	*
	* 	{												       //row index
	* 		1,	0,	0,	0,	j,	0,	0,	0,				       //0
	* 		0,	0,	0,	0,	0,	0,	0,	0,				       //1	
	* 		0, -1,	0,	0,	0,	j,	0,	0,				       //2
	* 		0,	0,	0,	0,	0,	0,	0,	0,				       //3
	* 		0,	1,	0,	0,	0,	j,	0,	0,				       //4
	* 		0,	0,	0,	0,	0,	0,	0,	0,				       //5	
	* 		1,	0,	0,	0, -j,	0,	0,	0,				       //6
	* 		0,	0,	0,	0,	0,	0,	0,	0,				       //7	
	* 		0,	0,	0,	0,	0,	0,	0,	0,				       //8
	* 		0,	0,	1,	0,	0,	0,	j,	0,				       //9	
	* 		0,	0,	0,	0,	0,	0,	0,	0,				       //10
	* 		0,	0,	0, -1,	0,	0,	0,	j,				       //11
	* 		0,	0,	0,	0,	0,	0,	0,	0,				       //12
	*		0,	0,	0,	1,	0,	0,	0,	j,				       //13
	*		0,	0,	0,	0,	0,	0,	0,	0,				       //14
	*		0,	0,	1,	0,	0,	0, -j,	0				       //15
	*	}
	*/
	
	static float A_re_ant4[16][8] = 
	{					                                           //row
		{0.7071f,  0,	    0,	      0,        0,	0,	0,	0},     //0
		{0,	      0,	    0,	      0,        0,	0,	0,	0},	   //1	
		{0,       -0.7071f,  0,	      0,        0,	0,	0,	0},	   //2
		{0,	      0,	    0,	      0,        0,	0,	0,	0},	   //3
		{0,	      0.7071f,  0,	      0,        0,	0,	0,	0},	   //4
		{0,	      0,	    0,	      0,        0,	0,	0,	0},	   //5	
		{0.7071f,  0,	    0,	      0,        0,	0,	0,	0},	   //6
		{0,	      0,	    0,	      0,        0,	0,	0,	0},	   //7	
		{0,	      0,	    0,	      0,        0,	0,	0,	0},	   //8
		{0,	      0,	    0.7071f,  0,        0,	0,	0,	0},	   //9	
		{0,	      0,	    0,	      0,        0,	0,	0,	0},	   //10
		{0,	      0,	    0,       -0.7071f,  0,	0,	0,	0},	   //11
		{0,	      0,	    0,	      0,        0,	0,	0,	0},	   //12
		{0,	      0,	    0,	      0.7071f,	0,	0,	0,	0},	   //13
		{0,	      0,	    0,	      0,        0,	0,	0,	0},	   //14
		{0,	      0,	    0.7071f,  0,        0,	0,	0,	0}	   //15
	};
	static float A_im_ant4[16][8]	 = 
	{					                                           //row 
		{0,	0,	0,	0,	0.7071f,  0,	    0,        0},	       //0
		{0,	0,	0,	0,	0,	      0,	    0,        0},	       //1	
		{0,	0,	0,	0,	0,	      0.7071f,  0,        0},	       //2
		{0,	0,	0,	0,	0,	      0,	    0,        0},	       //3
		{0,	0,	0,	0,	0,	      0.7071f,  0,        0},	       //4
		{0,	0,	0,	0,	0,	      0,	    0,        0},	       //5	
		{0,	0,	0,	0, -0.7071f,  0,	    0,        0},	       //6
		{0,	0,	0,	0,	0,	      0,       	0,        0},	       //7	
		{0,	0,	0,	0,	0,	      0,       	0,	      0},	       //8
		{0,	0,	0,	0,	0,	      0,       	0.7071f,  0},	       //9	
		{0,	0,	0,	0,	0,	      0,        0,	      0},	       //10
		{0,	0,	0,	0,	0,	      0,       	0,	      0.7071f},	   //11
		{0,	0,	0,	0,	0,	      0,	    0,	      0},	       //12
		{0,	0,	0,	0,	0,	      0,       	0,	      0.7071f},     //13
		{0,	0,	0,	0,	0,	      0,       	0,	      0},	       //14
		{0,	0,	0,	0,	0,	      0,       -0.7071f,  0}		       //15
	};
	
	/*
	* A1 =  1/sqrt(2)	*
	* 	{											            // row index
	* 		1,	0,	0,	0,	j,	0,	0,	0,			            //	0
	* 		0,	0,	0,	0,	0,	0,	0,	0,			            //	1
	* 		0,	-1,	0,	0,	0,	j,	0,	0,			            //	2
	* 		0,	0,	0,	0,	0,	0,	0,	0,			            //	3
	* 		0,	1,	0,	0,	0,	j,	0,	0,			            //	4
	* 		0,	0,	0,	0,	0,	0,	0,	0,			            //	5
	* 		1,	0,	0,	0,	-j,	0,	0,	0,			            //	6
	* 		0,	0,	0,	0,	0,	0,	0,	0,			            //	7
	* 		0,	0,	1,	0,	0,	0,	j,	0,			            //	8
	* 		0,	0,	0,	0,	0,	0,	0,	0,			            //	9
	* 		0,	0,	0,	-1,	0,	0,	0,	j,			            //	10
	* 		0,	0,	0,	0,	0,	0,	0,	0,			            //	11
	* 		0,	0,	0,	1,	0,	0,	0,	j,			            //	12
	* 		0,	0,	0,	0,	0,	0,	0,	0,			            //	13
	* 		0,	0,	1,	0,	0,	0,	-j,	0,			            //	14
	* 		0,	0,	0,	0,	0,	0,	0,	0,			            //	15
	* 	}
	*/	
	//on four antanna ports
	static float A1_re_phich[16][8] = 
	{							                                // row 
		{0.7071f,  0,	    0,	      0,	    0,	0,	0,	0},	//	0
		{0,	      0,	    0,	      0,	    0,	0,	0,	0},	//	1
		{0,       -0.7071f,  0,	      0,	    0,	0,	0,	0},	//	2
		{0,	      0,	    0,	      0,	    0,	0,	0,	0},	//	3
		{0,	      0.7071f,  0,	      0,	    0,	0,	0,	0},	//	4
		{0,	      0,	    0,	      0,	    0,	0,	0,	0},	//	5
		{0.7071f,  0,	    0,	      0,	    0,	0,	0,	0},	//	6
		{0,	      0,	    0,	      0,	    0,	0,	0,	0},	//	7
		{0,	      0,	    0.7071f,  0,	    0,	0,	0,	0},	//	8
		{0,	      0,	    0,	      0,	    0,	0,	0,	0},	//	9
		{0,	      0,	    0,       -0.7071f,  0,	0,	0,	0},	//	10
		{0,	      0,	    0,	      0,	    0,	0,	0,	0},	//	11
		{0,	      0,	    0,	      0.7071f,  0,	0,	0,	0},	//	12
		{0,	      0,	    0,	      0,	    0,	0,	0,	0},	//	13
		{0,	      0,	    0.7071f,  0,	    0,	0,	0,	0},	//	14
		{0,	      0,	    0,	      0,	    0,	0,	0,	0}	//	15
	};
	static float A1_im_phich[16][8] = 
	{				                                            // row
		{0,	0,	0,	0,	0.7071f,  0,	    0,	      0},	    //	0
		{0,	0,	0,	0,	0,	      0,	    0,	      0},		//	1
		{0,	0,	0,	0,	0,	      0.7071f,  0,	      0},		//	2
		{0,	0,	0,	0,	0,	      0,	    0,	      0},		//	3
		{0,	0,	0,	0,	0,	      0.7071f,  0,	      0},		//	4
		{0,	0,	0,	0,	0,	      0,	    0,	      0},		//	5
		{0,	0,	0,	0, -0.7071f,  0,	    0,	      0},		//	6
		{0,	0,	0,	0,	0,	      0,	    0,	      0},		//	7
		{0,	0,	0,	0,	0,	      0,	    0.7071f,  0},		//	8
		{0,	0,	0,	0,	0,	      0,	    0,	      0},		//	9
		{0,	0,	0,	0,	0,	      0,	    0,	      0.7071f},	//	10
		{0,	0,	0,	0,	0,	      0,	    0,	      0},		//	11
		{0,	0,	0,	0,	0,	      0,	    0,	      0.7071f},	//	12
		{0,	0,	0,	0,	0,	      0,	    0,	      0},		//	13
		{0,	0,	0,	0,	0,	      0,        -0.7071f, 0},		//	14
		{0,	0,	0,	0,	0,	      0,	    0,	      0}		//	15
	};
	
	/*
	* 	A2 = 1/sqrt(2)	*
	* 	{											            // row index
	* 	 	0,	0,	0,	0,	0,	0,	0,	0,			            //	0
	* 		1,	0,	0,	0,	j,	0,	0,	0,			            //	1
	* 		0,	0,	0,	0,	0,	0,	0,	0,			            //	2
	* 		0,	-1,	0,	0,	0,	j,	0,	0,			            //	3
	* 		0,	0,	0,	0,	0,	0,	0,	0,			            //	4
	* 		0,	1,	0,	0,	0,	j,	0,	0,			            //	5
	* 		0,	0,	0,	0,	0,	0,	0,	0,			            //	6
	* 		1,	0,	0,	0,	-j,	0,	0,	0,			            //	7
	* 		0,	0,	0,	0,	0,	0,	0,	0,			            //	8
	* 		0,	0,	1,	0,	0,	0,	j,	0,			            //	9
	* 		0,	0,	0,	0,	0,	0,	0,	0,			            //	10
	* 		0,	0,	0,	-1,	0,	0,	0,	j,			            //	11
	* 		0,	0,	0,	0,	0,	0,	0,	0,			            //	12
	* 		0,	0,	0,	1,	0,	0,	0,	j,			            //	13
	* 		0,	0,	0,	0,	0,	0,	0,	0,			            //	14
	* 		0,	0,	1,	0,	0,	0,	-j,	0			            //	15
	* 	};
	*/
	static float A2_re_phich[16][8] = 
	{											                   // row
		{0,	      0,	   0,	     0,	       0,	0,	0,	0},	   // 0
		{0.7071f,  0,	   0,	     0,	       0,	0,	0,	0},    // 1
		{0,	      0,	   0,	     0,	       0,	0,	0,	0},	   // 2
		{0,	     -0.7071f, 0,	     0,	       0,	0,	0,	0},	   // 3
		{0,	      0,       0,	     0,	       0,	0,	0,	0},    // 4
		{0,	      0.7071f, 0,	     0,	       0,	0,	0,	0},    // 5
		{0,	      0,       0,	     0,	       0,	0,	0,	0},	   // 6
		{0.7071f,  0,       0,	     0,	       0,	0,	0,	0},    // 7
		{0,	      0,	   0,	     0,	       0,	0,	0,	0},	   // 8
		{0,	      0,	   0.7071f,  0,	       0,	0,	0,	0},    // 9
		{0,	      0,	   0,	     0,	       0,	0,	0,	0},	   // 10
		{0,	      0,	   0,	    -0.7071f,  0,	0,	0,	0},	   // 11
		{0,	      0,	   0,	     0,	       0,	0,	0,	0},	   // 12
		{0,	      0,	   0,	     0.7071f,  0,	0,	0,	0},	   // 13
		{0,	      0,	   0,	     0,	       0,	0,	0,	0},	   // 14
		{0,	      0,	   0.7071f,	 0,	       0,	0,	0,	0}	   // 15
	};
	static float A2_im_phich[16][8] = 
	{											                   // row
		{0,	0,	0,	0,	0,	      0,	    0,	      0},		   //	0
		{0,	0,	0,	0,	0.7071f,  0,	    0,	      0},		   //	1
		{0,	0,	0,	0,	0,	      0,	    0,	      0},		   //	2
		{0,	0,	0,	0,	0,	      0.7071f,  0,	      0},		   //	3
		{0,	0,	0,	0,	0,	      0,	    0,	      0},		   //	4
		{0,	0,	0,	0,	0,	      0.7071f,  0,	      0},		   //	5
		{0,	0,	0,	0,	0,	      0,	    0,	      0},		   //	6
		{0,	0,	0,	0, -0.7071f,  0,	    0,	      0},		   //	7
		{0,	0,	0,	0,	0,	      0,	    0,	      0},		   //	8
		{0,	0,	0,	0,	0,	      0,	    0.7071f,  0},		   //	9
		{0,	0,	0,	0,	0,	      0,	    0,	      0},		   //	10
		{0,	0,	0,	0,	0,	      0,	    0,	      0.7071f},	   //	11
		{0,	0,	0,	0,	0,	      0,	    0,	      0},		   //	12
		{0,	0,	0,	0,	0,	      0,	    0,	      0.7071f},	   //	13
		{0,	0,	0,	0,	0,	      0,	    0,	      0},		   //	14
		{0,	0,	0,	0,	0,	      0,	   -0.7071f,  0}		       //	15
	};

	// get precode matrix
	switch(matrix_type)
	{
	case U_LAY2_ID:
		for(i=0; i<2; i++)
		{
			for(j=0; j<2; j++)
			{
				outbuf[i][j].re = U_re_lay2[i][j];
				outbuf[i][j].im = U_im_lay2[i][j];
			}
		}
		outlen = 4;
		break;

	case U_LAY3_ID:
		for(i=0; i<3; i++)
		{
			for(j=0; j<3; j++)
			{
				outbuf[i][j].re = U_re_lay3[i][j];
				outbuf[i][j].im = U_im_lay3[i][j];
			}
		}
		outlen = 9;
		break;		
		
	case U_LAY4_ID:
		for(i=0; i<4; i++)
		{
			for(j=0; j<4; j++)
			{
				outbuf[i][j].re = U_re_lay4[i][j];
				outbuf[i][j].im = U_im_lay4[i][j];
			}
		}
		outlen = 16;
		break;		
		
	case W_ANT2_LAY1_CODIDX0_ID:
		for(i=0; i<2; i++)
		{
			for(j=0; j<1; j++)
			{
				outbuf[i][j].re = W_re_ant2_lay1_codidx0[i][j];
				outbuf[i][j].im = W_im_ant2_lay1_codidx0[i][j];
			}
		}
		outlen = 2;
		break;	
		
	case W_ANT2_LAY1_CODIDX1_ID:
		for(i=0; i<2; i++)
		{
			for(j=0; j<1; j++)
			{
				outbuf[i][j].re = W_re_ant2_lay1_codidx1[i][j];
				outbuf[i][j].im = W_im_ant2_lay1_codidx1[i][j];
			}
		}
		outlen = 2;
		break;
		
	case W_ANT2_LAY1_CODIDX2_ID:
		for(i=0; i<2; i++)
		{
			for(j=0; j<1; j++)
			{
				outbuf[i][j].re = W_re_ant2_lay1_codidx2[i][j];
				outbuf[i][j].im = W_im_ant2_lay1_codidx2[i][j];
			}
		}
		outlen = 2;
		break;

	case W_ANT2_LAY1_CODIDX3_ID:
		for(i=0; i<2; i++)
		{
			for(j=0; j<1; j++)
			{
				outbuf[i][j].re = W_re_ant2_lay1_codidx3[i][j];
				outbuf[i][j].im = W_im_ant2_lay1_codidx3[i][j];
			}
		}
		outlen = 2;
		break;
	
	case W_ANT2_LAY2_CODIDX0_ID:
		for(i=0; i<2; i++)
		{
			for(j=0; j<2; j++)
			{
				outbuf[i][j].re = W_re_ant2_lay2_codidx0[i][j];
				outbuf[i][j].im = W_im_ant2_lay2_codidx0[i][j];
			}
		}
		outlen = 2;
		break;
		
	case W_ANT2_LAY2_CODIDX1_ID:
		for(i=0; i<2; i++)
		{
			for(j=0; j<2; j++)
			{
				outbuf[i][j].re = W_re_ant2_lay2_codidx1[i][j];
				outbuf[i][j].im = W_im_ant2_lay2_codidx1[i][j];
			}
		}
		outlen = 2;
		break;
		
	case W_ANT2_LAY2_CODIDX2_ID:
		for(i=0; i<2; i++)
		{
			for(j=0; j<2; j++)
			{
				outbuf[i][j].re = W_re_ant2_lay2_codidx2[i][j];
				outbuf[i][j].im = W_im_ant2_lay2_codidx2[i][j];
			}
		}
		outlen = 2;
		break;	
	
	case U0_ID:
		for(i=0; i<4; i++)
		{
			for(j=0; j<1; j++)
			{
				outbuf[i][j].re = u0_re[i][j];
				outbuf[i][j].im = u0_im[i][j];
			}
		}
		outlen = 4;
		break;	

	case U1_ID:
		for(i=0; i<4; i++)
		{
			for(j=0; j<1; j++)
			{
				outbuf[i][j].re = u1_re[i][j];
				outbuf[i][j].im = u1_im[i][j];
			}
		}
		outlen = 4;
		break;			
	
	case U2_ID:
		for(i=0; i<4; i++)
		{
			for(j=0; j<1; j++)
			{
				outbuf[i][j].re = u2_re[i][j];
				outbuf[i][j].im = u2_im[i][j];
			}
		}
		outlen = 4;
		break;		

	case U3_ID:
		for(i=0; i<4; i++)
		{
			for(j=0; j<1; j++)
			{
				outbuf[i][j].re = u3_re[i][j];
				outbuf[i][j].im = u3_im[i][j];
			}
		}
		outlen = 4;
		break;			
		
	case U4_ID:
		for(i=0; i<4; i++)
		{
			for(j=0; j<1; j++)
			{
				outbuf[i][j].re = u4_re[i][j];
				outbuf[i][j].im = u4_im[i][j];
			}
		}
		outlen = 4;
		break;			
		
	case U5_ID:
		for(i=0; i<4; i++)
		{
			for(j=0; j<1; j++)
			{
				outbuf[i][j].re = u5_re[i][j];
				outbuf[i][j].im = u5_im[i][j];
			}
		}
		outlen = 4;
		break;			
	
	case U6_ID:
		for(i=0; i<4; i++)
		{
			for(j=0; j<1; j++)
			{
				outbuf[i][j].re = u6_re[i][j];
				outbuf[i][j].im = u6_im[i][j];
			}
		}
		outlen = 4;
		break;			
		
	case U7_ID:
		for(i=0; i<4; i++)
		{
			for(j=0; j<1; j++)
			{
				outbuf[i][j].re = u7_re[i][j];
				outbuf[i][j].im = u7_im[i][j];
			}
		}
		outlen = 4;
		break;			
	
	case U8_ID:
		for(i=0; i<4; i++)
		{
			for(j=0; j<1; j++)
			{
				outbuf[i][j].re = u8_re[i][j];
				outbuf[i][j].im = u8_im[i][j];
			}
		}
		outlen = 4;
		break;	

	case U9_ID:
		for(i=0; i<4; i++)
		{
			for(j=0; j<1; j++)
			{
				outbuf[i][j].re = u9_re[i][j];
				outbuf[i][j].im = u9_im[i][j];
			}
		}
		outlen = 4;
		break;	
		
	case U10_ID:
		for(i=0; i<4; i++)
		{
			for(j=0; j<1; j++)
			{
				outbuf[i][j].re = u10_re[i][j];
				outbuf[i][j].im = u10_im[i][j];
			}
		}
		outlen = 4;
		break;	
		
	case U11_ID:
		for(i=0; i<4; i++)
		{
			for(j=0; j<1; j++)
			{
				outbuf[i][j].re = u11_re[i][j];
				outbuf[i][j].im = u11_im[i][j];
			}
		}
		outlen = 4;
		break;			
		
	case U12_ID:
		for(i=0; i<4; i++)
		{
			for(j=0; j<1; j++)
			{
				outbuf[i][j].re = u12_re[i][j];
				outbuf[i][j].im = u12_im[i][j];
			}
		}
		outlen = 4;
		break;	

	case U13_ID:
		for(i=0; i<4; i++)
		{
			for(j=0; j<1; j++)
			{
				outbuf[i][j].re = u13_re[i][j];
				outbuf[i][j].im = u13_im[i][j];
			}
		}
		outlen = 4;
		break;	

	case U14_ID:
		for(i=0; i<4; i++)
		{
			for(j=0; j<1; j++)
			{
				outbuf[i][j].re = u14_re[i][j];
				outbuf[i][j].im = u14_im[i][j];
			}
		}
		outlen = 4;
		break;	

	case U15_ID:
		for(i=0; i<4; i++)
		{
			for(j=0; j<1; j++)
			{
				outbuf[i][j].re = u15_re[i][j];
				outbuf[i][j].im = u15_im[i][j];
			}
		}
		outlen = 4;
		break;			

	case A_ANT2_ID:
		for(i=0; i<4; i++)
		{
			for(j=0; j<4; j++)
			{
				outbuf[i][j].re = A_re_ant2[i][j];
				outbuf[i][j].im = A_im_ant2[i][j];
			}
		}
		outlen = 16;
		break;	

	case A_ANT4_ID:
		for(i=0; i<16; i++)
		{
			for(j=0; j<8; j++)
			{
				outbuf[i][j].re = A_re_ant4[i][j];
				outbuf[i][j].im = A_im_ant4[i][j];
			}
		}
		outlen = 128;
		break;	
		
	case A1_PHICH_ID:
		for(i=0; i<16; i++)
		{
			for(j=0; j<8; j++)
			{
				outbuf[i][j].re = A1_re_phich[i][j];
				outbuf[i][j].im = A1_im_phich[i][j];
			}
		}
		outlen = 128;
		break;		
	
	case A2_PHICH_ID:
		for(i=0; i<16; i++)
		{
			for(j=0; j<8; j++)
			{
				outbuf[i][j].re = A2_re_phich[i][j];
				outbuf[i][j].im = A2_im_phich[i][j];
			}
		}
		outlen = 128;
		break;			
		
	default:
		assert(0);
	}
	
	return outlen;
}
