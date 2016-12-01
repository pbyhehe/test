// -----------------------------------------------------------------------------
// Name    : uerx_viterbi_dec.cpp
// Author  : YangShuo
// Email   : YangShuo@ict.ac.cn
// Comment : decode LTE(3,1,7) tail biting Convolutional codes,input LLR,OUTPUT outbuf.
// -----------------------------------------------------------------------------
// RELEASE HISTORY
// VERSION DATE       AUTHOR DESCRIPTION
//
// 2009-12-30 YANGSHUO	  3.0 version  
// 4 time cycle->LLR  ->  P0=(1-tanh(LLR/2))/2  ->p0(0 ~ 1)to(0 ~ 15)->add DECODE
// 2010-01-15 YANGSHUO	  3.5 version	
// little adjust
// 2010-04-22 YANGSHUO    4.0 version	
//  Directly quantify the LLR (Non-uniform)
//  2010-04-22 YANGSHUO    4.1 version
//  Directly quantify the LLR (Non-uniform,Accuracy+)	
//  2010-04-28 YANGSHUO    4.15 version
//  Accuracy+++
//  2010-04-28 YANGSHUO    4.16 version
//  float->float
// -----------------------------------------------------------------------------


#include "foo.h"
#include "uerx_viterbi_dec.h"


int viterbi_InputOutput(float *inbuf0, float *inbuf1, float *inbuf2, int inlen, int16 *outbuf)
{

	int i, k;
	int next_state_inbit0;
	int next_state_inbit1;
	int now_state1;
	int now_state2;
	int inlen_4times;

	int survive_path_inbit0;
	int survive_path_inbit1;

	int prob_inbit0_now_state1;
	int prob_inbit1_now_state1;
	int prob_inbit0_now_state2;
	int prob_inbit1_now_state2;
	
    int survive_prob_inbit0;
    int survive_prob_inbit1;
	
	int max_prob;

	int output_conv_dec_3bit[2][64];

	int bm[8];

	#pragma SET_DATA_SECTION("uerx_data")
	static float in[3][1000];
    
	static int p0_in[3][1000];
	static int p1_in[3][1000];

	static int survive_prob_matrix[1001][64];
	static int survivor_path_matrix[1001][64];	

	static int trace[1001];

	static int outbit_dec[1000];
	#pragma SET_DATA_SECTION()

	// function body	
	// output_conv_dec_3bit[inbit][state] --> convlution decode 3 bits
	output_conv_dec_3bit[0][0]	= 0;	output_conv_dec_3bit[1][0]	= 7;
	output_conv_dec_3bit[0][1]	= 7;	output_conv_dec_3bit[1][1]	= 0;
	output_conv_dec_3bit[0][2]	= 4;	output_conv_dec_3bit[1][2]	= 3;
	output_conv_dec_3bit[0][3]	= 3;	output_conv_dec_3bit[1][3]	= 4;
	output_conv_dec_3bit[0][4]	= 1;	output_conv_dec_3bit[1][4]	= 6;
	output_conv_dec_3bit[0][5]	= 6;	output_conv_dec_3bit[1][5]	= 1;
	output_conv_dec_3bit[0][6]	= 5;	output_conv_dec_3bit[1][6]	= 2;
	output_conv_dec_3bit[0][7]	= 2;	output_conv_dec_3bit[1][7]	= 5;
	output_conv_dec_3bit[0][8]	= 6;	output_conv_dec_3bit[1][8]	= 1;
	output_conv_dec_3bit[0][9]	= 1;	output_conv_dec_3bit[1][9]	= 6;
	output_conv_dec_3bit[0][10]	= 2;	output_conv_dec_3bit[1][10]	= 5;
	output_conv_dec_3bit[0][11]	= 5;	output_conv_dec_3bit[1][11]	= 2;
	output_conv_dec_3bit[0][12]	= 7;	output_conv_dec_3bit[1][12]	= 0;
	output_conv_dec_3bit[0][13]	= 0;	output_conv_dec_3bit[1][13]	= 7;
	output_conv_dec_3bit[0][14]	= 3;	output_conv_dec_3bit[1][14]	= 4;
	output_conv_dec_3bit[0][15]	= 4;	output_conv_dec_3bit[1][15]	= 3;
	output_conv_dec_3bit[0][16]	= 7;	output_conv_dec_3bit[1][16]	= 0;
	output_conv_dec_3bit[0][17]	= 0;	output_conv_dec_3bit[1][17]	= 7;
	output_conv_dec_3bit[0][18]	= 3;	output_conv_dec_3bit[1][18]	= 4;
	output_conv_dec_3bit[0][19]	= 4;	output_conv_dec_3bit[1][19]	= 3;
	output_conv_dec_3bit[0][20]	= 6;	output_conv_dec_3bit[1][20]	= 1;
	output_conv_dec_3bit[0][21]	= 1;	output_conv_dec_3bit[1][21]	= 6;
	output_conv_dec_3bit[0][22]	= 2;	output_conv_dec_3bit[1][22]	= 5;
	output_conv_dec_3bit[0][23]	= 5;	output_conv_dec_3bit[1][23]	= 2;
	output_conv_dec_3bit[0][24]	= 1;	output_conv_dec_3bit[1][24]	= 6;
	output_conv_dec_3bit[0][25]	= 6;	output_conv_dec_3bit[1][25]	= 1;
	output_conv_dec_3bit[0][26]	= 5;	output_conv_dec_3bit[1][26]	= 2;
	output_conv_dec_3bit[0][27]	= 2;	output_conv_dec_3bit[1][27]	= 5;
	output_conv_dec_3bit[0][28]	= 0;	output_conv_dec_3bit[1][28]	= 7;
	output_conv_dec_3bit[0][29]	= 7;	output_conv_dec_3bit[1][29]	= 0;
	output_conv_dec_3bit[0][30]	= 4;	output_conv_dec_3bit[1][30]	= 3;
	output_conv_dec_3bit[0][31]	= 3;	output_conv_dec_3bit[1][31]	= 4;
	output_conv_dec_3bit[0][32]	= 3;	output_conv_dec_3bit[1][32]	= 4;
	output_conv_dec_3bit[0][33]	= 4;	output_conv_dec_3bit[1][33]	= 3;
	output_conv_dec_3bit[0][34]	= 7;	output_conv_dec_3bit[1][34]	= 0;
	output_conv_dec_3bit[0][35]	= 0;	output_conv_dec_3bit[1][35]	= 7;
	output_conv_dec_3bit[0][36]	= 2;	output_conv_dec_3bit[1][36]	= 5;
	output_conv_dec_3bit[0][37]	= 5;	output_conv_dec_3bit[1][37]	= 2;
	output_conv_dec_3bit[0][38]	= 6;	output_conv_dec_3bit[1][38]	= 1;
	output_conv_dec_3bit[0][39]	= 1;	output_conv_dec_3bit[1][39]	= 6;
	output_conv_dec_3bit[0][40]	= 5;	output_conv_dec_3bit[1][40]	= 2;
	output_conv_dec_3bit[0][41]	= 2;	output_conv_dec_3bit[1][41]	= 5;
	output_conv_dec_3bit[0][42]	= 1;	output_conv_dec_3bit[1][42]	= 6;
	output_conv_dec_3bit[0][43]	= 6;	output_conv_dec_3bit[1][43]	= 1;
	output_conv_dec_3bit[0][44]	= 4;	output_conv_dec_3bit[1][44]	= 3;
	output_conv_dec_3bit[0][45]	= 3;	output_conv_dec_3bit[1][45]	= 4;
	output_conv_dec_3bit[0][46]	= 0;	output_conv_dec_3bit[1][46]	= 7;
	output_conv_dec_3bit[0][47]	= 7;	output_conv_dec_3bit[1][47]	= 0;
	output_conv_dec_3bit[0][48]	= 4;	output_conv_dec_3bit[1][48]	= 3;
	output_conv_dec_3bit[0][49]	= 3;	output_conv_dec_3bit[1][49]	= 4;
	output_conv_dec_3bit[0][50]	= 0;	output_conv_dec_3bit[1][50]	= 7;
	output_conv_dec_3bit[0][51]	= 7;	output_conv_dec_3bit[1][51]	= 0;
	output_conv_dec_3bit[0][52]	= 5;	output_conv_dec_3bit[1][52]	= 2;
	output_conv_dec_3bit[0][53]	= 2;	output_conv_dec_3bit[1][53]	= 5;
	output_conv_dec_3bit[0][54]	= 1;	output_conv_dec_3bit[1][54]	= 6;
	output_conv_dec_3bit[0][55]	= 6;	output_conv_dec_3bit[1][55]	= 1;
	output_conv_dec_3bit[0][56]	= 2;	output_conv_dec_3bit[1][56]	= 5;
	output_conv_dec_3bit[0][57]	= 5;	output_conv_dec_3bit[1][57]	= 2;
	output_conv_dec_3bit[0][58]	= 6;	output_conv_dec_3bit[1][58]	= 1;
	output_conv_dec_3bit[0][59]	= 1;	output_conv_dec_3bit[1][59]	= 6;
	output_conv_dec_3bit[0][60]	= 3;	output_conv_dec_3bit[1][60]	= 4;
	output_conv_dec_3bit[0][61]	= 4;	output_conv_dec_3bit[1][61]	= 3;
	output_conv_dec_3bit[0][62]	= 7;	output_conv_dec_3bit[1][62]	= 0;
	output_conv_dec_3bit[0][63]	= 0;	output_conv_dec_3bit[1][63]	= 7;
	
	//loop four times
	inlen_4times = 4*inlen;

	for (i=0; i<inlen; i++)
	{
		in[0][i+0*inlen] = inbuf0[i];
		in[0][i+1*inlen] = inbuf0[i];
		in[0][i+2*inlen] = inbuf0[i];
		in[0][i+3*inlen] = inbuf0[i];

		in[1][i+0*inlen] = inbuf1[i];
		in[1][i+1*inlen] = inbuf1[i];
		in[1][i+2*inlen] = inbuf1[i];
		in[1][i+3*inlen] = inbuf1[i];

		in[2][i+0*inlen] = inbuf2[i];
		in[2][i+1*inlen] = inbuf2[i];
		in[2][i+2*inlen] = inbuf2[i];
		in[2][i+3*inlen] = inbuf2[i];
	}

	// three data quantization
	for(k=0; k<3; k++)
	{
		for (i=0; i<inlen_4times; i++)
		{
			if (in[k][i]>=3.1583)			
			{
				p1_in[k][i]=0;
				p0_in[k][i]=15;
			}
			else if(in[k][i]>=2.9348 && in[k][i]<3.1583)
			{
				p1_in[k][i]=1;
				p0_in[k][i]=15;
			}
			else if(in[k][i]>=2.7087 && in[k][i]<2.9348)   
			{
				p1_in[k][i]=2;
				p0_in[k][i]=15;
			}
			else if(in[k][i]>=2.4795 && in[k][i]<2.7087)
			{
				p1_in[k][i]=3;
				p0_in[k][i]=15;
			}
			else if(in[k][i]>=2.2461 && in[k][i]<2.4795)
			{
				p1_in[k][i]=4;
				p0_in[k][i]=15;
			}
			else if(in[k][i]>=2.0072 && in[k][i]<2.2461)
			{
				p1_in[k][i]=5;
				p0_in[k][i]=15;
			}
			else if(in[k][i]>=1.7615 && in[k][i]<2.0072)
			{
				p1_in[k][i]=6;
				p0_in[k][i]=15;
			}        
			else if(in[k][i]>=1.5063 && in[k][i]<1.7615)
			{
				p1_in[k][i]=7;
				p0_in[k][i]=15;
			}
			else if(in[k][i]>=1.4364 && in[k][i]<1.5063)
			{
				p1_in[k][i]=8;
				p0_in[k][i]=15;
			}
			else if(in[k][i]>=1.2389 && in[k][i]<1.4364)
			{
				p1_in[k][i]=8;
				p0_in[k][i]=14;
			}
			else if(in[k][i]>=0.9542 && in[k][i]<1.2389)
			{
				p1_in[k][i]=9;
				p0_in[k][i]=14;
			}
			else if (in[k][i]>=0.6448 && in[k][i]<0.9542)
			{
				p1_in[k][i]=10;
				p0_in[k][i]=14;
			}
			else if(in[k][i]>=0.6309 && in[k][i]<0.6448)
			{
				p1_in[k][i]=11;
				p0_in[k][i]=14;
			}
			else if(in[k][i]>=0.2982 && in[k][i]<0.6309)
			{
				p1_in[k][i]=11;
				p0_in[k][i]=13;
			}
			else if(in[k][i]>=0.1093 && in[k][i]<0.2982)
			{
				p1_in[k][i]=12;
				p0_in[k][i]=13;
			}
			else if(in[k][i]>=-0.1093 && in[k][i]<0.1093)
			{
				p1_in[k][i]=12;
				p0_in[k][i]=12;
			}
			else if(in[k][i]>=-0.2982 && in[k][i]<-0.1093)
			{
				p1_in[k][i]=13;
				p0_in[k][i]=12;
			}
			else if(in[k][i]>=-0.6309 && in[k][i]<-0.2982)
			{
				p1_in[k][i]=13;
				p0_in[k][i]=11;
			}
			else if(in[k][i]>=-0.6448 && in[k][i]<-0.6309)
			{
				p1_in[k][i]=14;
				p0_in[k][i]=11;
			}
			else if(in[k][i]>=-0.9542 && in[k][i]<-0.6448)
			{
				p1_in[k][i]=14;
				p0_in[k][i]=10;
			}
			else if(in[k][i]>=-1.2389 && in[k][i]<-0.9542)
			{
				p1_in[k][i]=14;
				p0_in[k][i]=9;
			}
			else if(in[k][i]>=-1.4364 && in[k][i]<-1.2389)
			{
				p1_in[k][i]=14;
				p0_in[k][i]=8;
			}
			else if(in[k][i]>=-1.5063 && in[k][i]<-1.4364)
			{
				p1_in[k][i]=15;
				p0_in[k][i]=8;
			}
			else if(in[k][i]>=-1.7615 && in[k][i]<-1.5063)
			{
				p1_in[k][i]=15;
				p0_in[k][i]=7;
			}
			else if(in[k][i]>=-2.0072 && in[k][i]<-1.7615)
			{
				p1_in[k][i]=15;
				p0_in[k][i]=6;
			}
			else if(in[k][i]>=-2.2461 && in[k][i]<-2.0072)
			{
				p1_in[k][i]=15;
				p0_in[k][i]=5;
			}
			else if(in[k][i]>=-2.4795 && in[k][i]<-2.2461)
			{
				p1_in[k][i]=15;
				p0_in[k][i]=4;
			}
			else if(in[k][i]>=-2.7087 && in[k][i]<-2.4795)
			{
				p1_in[k][i]=15;
				p0_in[k][i]=3;
			}
			else if(in[k][i]>=-2.9348 && in[k][i]<-2.7087)
			{
				p1_in[k][i]=15;
				p0_in[k][i]=2;
			}
			else if(in[k][i]>=-3.1583 && in[k][i]<-2.9348)
			{
				p1_in[k][i]=15;
				p0_in[k][i]=1;
			}
			else if(in[k][i]<-3.1583)
			{
				p1_in[k][i]=15;
				p0_in[k][i]=0;
			}
		}
	}

	// init the first stage probability
	for (i=0; i<64; i++)
	{
		survive_prob_matrix[0][i] = 0;
	}

	for (i=0; i<inlen_4times; i++)
	{
		bm[0] = p0_in[0][i] + p0_in[1][i] + p0_in[2][i]; //000 bm
		bm[1] = p0_in[0][i] + p0_in[1][i] + p1_in[2][i]; //001 bm
		bm[2] = p0_in[0][i] + p1_in[1][i] + p0_in[2][i]; //010 bm
		bm[3] = p0_in[0][i] + p1_in[1][i] + p1_in[2][i]; //011 bm
		bm[4] = p1_in[0][i] + p0_in[1][i] + p0_in[2][i]; //100 bm
		bm[5] = p1_in[0][i] + p0_in[1][i] + p1_in[2][i]; //101 bm
		bm[6] = p1_in[0][i] + p1_in[1][i] + p0_in[2][i]; //110 bm
		bm[7] = p1_in[0][i] + p1_in[1][i] + p1_in[2][i]; //111 bm

		//32 butterfly 
		for(k=0; k<32; k++)
		{  
			//LTE(3,1,7) butterfly
		   now_state1 = 2*k + 0;
		   now_state2 = 2*k + 1;
		   
		   next_state_inbit0 = (now_state1/2) + 0;
           next_state_inbit1 = (now_state1/2) + 32;
		   
		   //survive_prob_matrix is a matrix(inlen,64)
		   prob_inbit0_now_state1 = survive_prob_matrix[i][now_state1] + bm[output_conv_dec_3bit[0][now_state1]];
           prob_inbit1_now_state1 = survive_prob_matrix[i][now_state1] + bm[output_conv_dec_3bit[1][now_state1]];

           prob_inbit0_now_state2 = survive_prob_matrix[i][now_state2] + bm[output_conv_dec_3bit[0][now_state2]];
           prob_inbit1_now_state2 = survive_prob_matrix[i][now_state2] + bm[output_conv_dec_3bit[1][now_state2]];
 
		   // if input bit is 0, the survive path will be the bigger one
		   if (prob_inbit0_now_state1 > prob_inbit0_now_state2)
           {   
			   survive_path_inbit0 = now_state1;
               survive_prob_inbit0 = prob_inbit0_now_state1;
		   }
           else
		   {  
			   survive_path_inbit0  = now_state2;
               survive_prob_inbit0  = prob_inbit0_now_state2;
           }

           // if input bit is 1, the survive path will be the bigger one
           if (prob_inbit1_now_state1 > prob_inbit1_now_state2)
           {   
			   survive_path_inbit1 = now_state1;
               survive_prob_inbit1 = prob_inbit1_now_state1;
		   }
           else
		   {    
			   survive_path_inbit1 = now_state2;
               survive_prob_inbit1 = prob_inbit1_now_state2;
		   }

		   survive_prob_matrix[i+1][next_state_inbit0] = survive_prob_inbit0;
		   survive_prob_matrix[i+1][next_state_inbit1] = survive_prob_inbit1;

		   survivor_path_matrix[i+1][next_state_inbit0] = survive_path_inbit0;
		   survivor_path_matrix[i+1][next_state_inbit1] = survive_path_inbit1;
		}
	}

	//track back from the most survive_prob_matrix
	max_prob = survive_prob_matrix[inlen_4times][0];
	for (i=0; i<64; i++)
	{	
		if(survive_prob_matrix[inlen_4times][i] >= max_prob)
		{	
			max_prob = survive_prob_matrix[inlen_4times][i];
			
			trace[inlen_4times] = i;
		}
	}

	for (i=inlen_4times; i>=2; i--)
	{
		trace[i-1] = survivor_path_matrix[i][trace[i]];

		if(trace[i-1] < 32)
			outbit_dec[i-2] = 0;
		else
			outbit_dec[i-2] = 1;
	}
		
	for (i=0; i<inlen; i++)
	{
		outbuf[i] = outbit_dec[i+inlen];
	}

	return inlen;
}
