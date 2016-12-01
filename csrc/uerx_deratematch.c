#include "foo.h"
#include "uerx_deratematch.h"
#include <math.h>

#define min(a, b)		(( (a) < (b) )? (a): (b))


//int deratematch_pdsch(float *inbuf, int inlen, int transblock_size, int catgry_ue
//										, int num_harqprocess, int qm, int rv_idx, int NL, int k_mimo
//										, float **d0_buf, float **d1_buf, float **d2_buf
//										, int *code_buf, int *code_num, int *F)
//{
//	short block_idx,nullbit_table_idx;
//	int Sub_inter_idx,D_len,Er_len,D_sum0,D_sum1;
//	int k,kk,tmp;
//
//	short tm;
//
//	short z[13]={0};
//	short p1[32]={0};
//	short d1[32]={0};
//	short q[32]={1,2,3,4,5,6,7,7,8,9,10,11,12,13,14,14,15,16,17,18,19,20,21,21,22,23,24,25,26,27,28,28};
//	short p[32] = {0,16,8,24,4,20,12,28,2,18,10,26,6,22,14,30,1,17,9,25,5,21,13,29,3,19,11,27,7,23,15,31};
//	short q1[32]={30,32,34,36,38,40,42,42,44,46,48,50,52,54,56,56,58,60,62,64,66,68,70,70,72,74,76,77,79,81,83,83};
//
//    int D[13]={0};
//    int num_matrix[13]={0};
//    int num_row[13]={0};
//    int D1[13]={0};
//    int k0[13]={0};
//    int Kw[13]={0};
//    int Ncb[13]={0};
//
//    int G_pdsch;
//    int G1_pdsch;
//    int r;
//    int i;
//    int N_soft;
//    int Er[13]={0};
//    int size_each_codeseg[13]={0};
//
//    blockseg(transblock_size+24, size_each_codeseg, code_num, F);
//
//	G_pdsch=inlen;
//	G1_pdsch = G_pdsch / (NL*qm);
//
//	r = G1_pdsch % (*code_num);
//
//    for(i=0;i<(*code_num);i++)
//	{
//	D[i] = size_each_codeseg[i]+4;
//	num_row[i] = (D[i]+31) / 32;
//	num_matrix[i] = num_row[i] * 32;
//	D1[i] = num_row[i]*32 - D[i];
//	Kw[i] = 3*num_matrix[i];
//	if(catgry_ue==1)		N_soft = 250368;
//	else if(catgry_ue==2)	N_soft = 1237248;
//	else if(catgry_ue==3)   N_soft = 1237248;
//	else if(catgry_ue==4)	N_soft = 1827072;
//	else if(catgry_ue==5)	N_soft = 3667200;
//	else					assert(0);
//    int M_limit=8;
//    int N_IR;
//	N_IR = N_soft / (k_mimo*min(num_harqprocess,M_limit));
//	Ncb[i] = min(N_IR/(*code_num), Kw[i]);
//	if(i <= ((*code_num)-r-1))   Er[i] = G1_pdsch/(*code_num) * NL*qm;
//	else					  Er[i] = (G1_pdsch+(*code_num)-1)/(*code_num) * NL*qm;
//	k0[i] = num_row[i] * ((Ncb[i]+8*num_row[i]-1)/(8*num_row[i]) *2*rv_idx + 2);
//	z[i]=k0[i]/num_row[i];
//	}
//
////	D_sum0 = 0;
////	for(block_idx = 0; block_idx < block_begin; block_idx++)
////	{
////		D_sum0 +=size_each_codeseg[block_idx];
////	}
//	D_sum1 = size_each_codeseg[0];
//
//    D_sum0 = 0;
//
//    int countt=0;
//	for(block_idx = 0; block_idx <(*code_num); block_idx++)
//	{
//		D_len = D1[block_idx];
//		Er_len = Er[block_idx];
//		D_sum0 = size_each_codeseg[block_idx];
//
//
//		/***********DeRatematch the first sub_block*****************/
//		for(nullbit_table_idx = 0; nullbit_table_idx < 32; nullbit_table_idx++)
//		{
//			tmp = p[nullbit_table_idx];
//			p1[nullbit_table_idx] = num_row[block_idx]*tmp-q[tmp]-k0[block_idx]+z[block_idx];
//		}
//
//		#pragma MUST_ITERATE(32,,32)
//		for(Sub_inter_idx = 0;Sub_inter_idx < D[block_idx]-4;Sub_inter_idx++)
//		{
//			kk  = Sub_inter_idx + D_len;
//			k= p1[kk&0x1f]+(kk>>5);
//			if(k<0)	d0_buf[block_idx][Sub_inter_idx] = 0;
//			else d0_buf[block_idx][Sub_inter_idx]=inbuf[k];
//			tm=d0_buf[block_idx][Sub_inter_idx];
//			d0_buf[block_idx][Sub_inter_idx]=tm;
//			kk++;
//		}
//
//		/***********DeRatematch the second sub_block*****************/
//		for(nullbit_table_idx = 0;nullbit_table_idx < 32;nullbit_table_idx++)
//		{
//			tmp = p[nullbit_table_idx];
//			d1[nullbit_table_idx] = 2*num_row[block_idx]*tmp+num_matrix[block_idx]-k0[block_idx]+z[block_idx]-q1[tmp];
//		}
//
//		#pragma MUST_ITERATE(32,,32)
//		for(Sub_inter_idx = 0;Sub_inter_idx < D[block_idx]-4;Sub_inter_idx++)
//		{
//			kk=Sub_inter_idx+D_len;
//			k=d1[kk%32]+kk/32*2;
//			if(k>(Er_len-1))	d1_buf[block_idx][Sub_inter_idx] = 0;
//			else d1_buf[block_idx][Sub_inter_idx]=inbuf[k];
//			tm=d1_buf[block_idx][Sub_inter_idx];
//			d1_buf[block_idx][Sub_inter_idx]=tm;
//		}
//
//		/***********DeRatematch the third sub_block*****************/
//		for(nullbit_table_idx = 0;nullbit_table_idx < 32;nullbit_table_idx++)
//		{
//			tmp = p[nullbit_table_idx];
//			d1[nullbit_table_idx] = 2*num_row[block_idx]*tmp+num_matrix[block_idx]-k0[block_idx]+z[block_idx]+1-q1[tmp];
//		}
//
//		#pragma MUST_ITERATE(32,,32)
//		for(Sub_inter_idx = 0;Sub_inter_idx < D[block_idx]-4;Sub_inter_idx++)
//		{
//			kk=Sub_inter_idx+D_len-1;
//			k=d1[kk%32]+kk/32*2;
//			if(k>(Er_len-1))	d2_buf[block_idx][Sub_inter_idx] = 0;
//			else d2_buf[block_idx][Sub_inter_idx]=inbuf[k];
//			tm=d2_buf[block_idx][Sub_inter_idx];
//			d2_buf[block_idx][Sub_inter_idx]=tm;
//		}
//            countt++;
//	}
//
//}


int deratematch_pdsch(float *inbuf, int inlen, int transblock_size, int catgry_ue
										, int num_harqprocess, int qm, int rv_idx, int NL, int k_mimo
										, float **d0_buf, float **d1_buf, float **d2_buf
										, int *code_buf, int *code_num, int *F)
{
	short block_idx,nullbit_table_idx;
	int Sub_inter_idx,D_len,Er_len,D_sum0,D_sum1;
	int k,kk,tmp;
	float out0[50000]={0};
	float out1[50000]={0};
	float out2[50000]={0};

	short tm;

	short z[13]={0};
	short p1[32]={0};
	short d1[32]={0};
	short q[32]={1,2,3,4,5,6,7,7,8,9,10,11,12,13,14,14,15,16,17,18,19,20,21,21,22,23,24,25,26,27,28,28};
	short p[32] = {0,16,8,24,4,20,12,28,2,18,10,26,6,22,14,30,1,17,9,25,5,21,13,29,3,19,11,27,7,23,15,31};
	short q1[32]={30,32,34,36,38,40,42,42,44,46,48,50,52,54,56,56,58,60,62,64,66,68,70,70,72,74,76,77,79,81,83,83};

    int D[13]={0};
    int num_matrix[13]={0};
    int num_row[13]={0};
    int D1[13]={0};
    int k0[13]={0};
    int Kw[13]={0};
    int Ncb[13]={0};

    int G_pdsch;
    int G1_pdsch;
    int r;
    int i;
    int N_soft;
    int Er[13]={0};
    int size_each_codeseg[13]={0};

    blockseg(transblock_size+24, size_each_codeseg, code_num, F);

	G_pdsch=inlen;
	G1_pdsch = G_pdsch / (NL*qm);

	r = G1_pdsch % (*code_num);

    for(i=0;i<(*code_num);i++)
	{
	D[i] = size_each_codeseg[i]+4;
	num_row[i] = (D[i]+31) / 32;
	num_matrix[i] = num_row[i] * 32;
	D1[i] = num_row[i]*32 - D[i];
	Kw[i] = 3*num_matrix[i];
	if(catgry_ue==1)		N_soft = 250368;
	else if(catgry_ue==2)	N_soft = 1237248;
	else if(catgry_ue==3)   N_soft = 1237248;
	else if(catgry_ue==4)	N_soft = 1827072;
	else if(catgry_ue==5)	N_soft = 3667200;
	else					assert(0);
    int M_limit=8;
    int N_IR;
	N_IR = N_soft / (k_mimo*min(num_harqprocess,M_limit));
	Ncb[i] = min(N_IR/(*code_num), Kw[i]);
	if(i <= ((*code_num)-r-1))   Er[i] = G1_pdsch/(*code_num) * NL*qm;
	else					  Er[i] = (G1_pdsch+(*code_num)-1)/(*code_num) * NL*qm;
	k0[i] = num_row[i] * ((Ncb[i]+8*num_row[i]-1)/(8*num_row[i]) *2*rv_idx + 2);
	z[i]=k0[i]/num_row[i];
	}

    int x=0;
    int y=Er[0]-1;
    int m=0;
	for(block_idx = 0; block_idx <(*code_num); block_idx++)
	{
		D_len = D1[block_idx];
		Er_len = Er[block_idx];


		/***********DeRatematch the first sub_block*****************/
		for(nullbit_table_idx = 0; nullbit_table_idx < 32; nullbit_table_idx++)
		{
			tmp = p[nullbit_table_idx];
			p1[nullbit_table_idx] = num_row[block_idx]*tmp-q[tmp]-k0[block_idx]+z[block_idx];
			p1[nullbit_table_idx]+=x;
		}

		#pragma MUST_ITERATE(32,,32)
		for(Sub_inter_idx = 0;Sub_inter_idx < D[block_idx];Sub_inter_idx++)
		{
			kk  = Sub_inter_idx + D_len;
			k= p1[kk&0x1f]+(kk>>5);
			if(k<x)	d0_buf[block_idx][Sub_inter_idx] = 0;
			else d0_buf[block_idx][Sub_inter_idx]=inbuf[k];
			tm=d0_buf[block_idx][Sub_inter_idx];
			d0_buf[block_idx][Sub_inter_idx]=tm;
			out0[Sub_inter_idx+m]=tm;
		}

		/***********DeRatematch the second sub_block*****************/
		for(nullbit_table_idx = 0;nullbit_table_idx < 32;nullbit_table_idx++)
		{
			tmp = p[nullbit_table_idx];
			d1[nullbit_table_idx] = 2*num_row[block_idx]*tmp+num_matrix[block_idx]-k0[block_idx]+z[block_idx]-q1[tmp];
			d1[nullbit_table_idx]+=x;
		}

		#pragma MUST_ITERATE(32,,32)
		for(Sub_inter_idx = 0;Sub_inter_idx < D[block_idx];Sub_inter_idx++)
		{
			kk=Sub_inter_idx+D_len;
			k=d1[kk%32]+kk/32*2;
			if(k>y)	d1_buf[block_idx][Sub_inter_idx] = 0;
			else d1_buf[block_idx][Sub_inter_idx]=inbuf[k];
			tm=d1_buf[block_idx][Sub_inter_idx];
			d1_buf[block_idx][Sub_inter_idx]=tm;
			out1[Sub_inter_idx+m]=tm;
		}

		/***********DeRatematch the third sub_block*****************/
		for(nullbit_table_idx = 0;nullbit_table_idx < 32;nullbit_table_idx++)
		{
			tmp = p[nullbit_table_idx];
			d1[nullbit_table_idx] = 2*num_row[block_idx]*tmp+num_matrix[block_idx]-k0[block_idx]+z[block_idx]+1-q1[tmp];
			d1[nullbit_table_idx]+=x;
		}

		#pragma MUST_ITERATE(32,,32)
		for(Sub_inter_idx = 0;Sub_inter_idx < D[block_idx];Sub_inter_idx++)
		{
			kk=Sub_inter_idx+D_len-1;
			k=d1[kk%32]+kk/32*2;
			if(k>y)	d2_buf[block_idx][Sub_inter_idx] = 0;
			else d2_buf[block_idx][Sub_inter_idx]=inbuf[k];
			tm=d2_buf[block_idx][Sub_inter_idx];
			d2_buf[block_idx][Sub_inter_idx]=tm;
			out2[Sub_inter_idx+m]=tm;
		}
            x+=Er_len;
            y+=Er_len;
            m+=D[block_idx];
	}

}


//int deratematch_pdsch(float *inbuf, int inlen, int transblock_size, int catgry_ue
//										, int num_harqprocess, int qm, int rv_idx, int NL, int k_mimo
//										, float **d00_buf, float **d11_buf, float **d22_buf
//										, int *code_buf, int *code_num, int *F)
//
//{
//		int k=0;
//		int tmp=0;
//		int j=0;
//		int i;
//		int z[13]={0};
//		int kk;
//		int r;
//		int p1[32]={0};
//		int d1[32]={0};
//		int q[32]={1,2,3,4,5,6,7,7,8,9,10,11,12,13,14,14,15,16,17,18,19,20,21,21,22,23,24,25,26,27,28,28};
//		int p[32] = {0,16,8,24,4,20,12,28,2,18,10,26,6,22,14,30,1,17,9,25,5,21,13,29,3,19,11,27,7,23,15,31};
//		int q1[32]={30,32,34,36,38,40,42,42,44,46,48,50,52,54,56,56,58,60,62,64,66,68,70,70,72,74,76,77,79,81,83,83};
//
//        int D[13]={0};
//        int size_each_codeseg[13]={0};
//        int num_matrix[13]={0};
//        int num_row[13]={0};
//        int D1[13]={0};
//        int k0[13]={0};
//        short d0_buf[50000]={0};
//        short d1_buf[50000]={0};
//        short d2_buf[50000]={0};
//
//
//		blockseg(transblock_size, size_each_codeseg, code_num, F);
//
//		int count[10]={0};
//		int block[10]={56,88,144,256,808,712,1192,328,14688,22920};
//		for(i=0;i<10;i++)
//		{
//			transblock_size=block[i]+24;
//			blockseg(transblock_size, size_each_codeseg, code_num, F);
//			count[i]=*F;
//		}
//
//		int G_pdsch;
//		int G1_pdsch;
//		int Kw[13]={0};
//		int Ncb[13]={0};
//		int Er[13]={0};
//        int N_soft;
//		G_pdsch=inlen;
//		G1_pdsch = G_pdsch / (NL*qm);
//
//		r = G1_pdsch % (*code_num);
//
//        for(i=0;i<(*code_num);i++)
//		{
//		D[i] = size_each_codeseg[i]+4;
//		num_row[i] = (D[i]+31) / 32;
//		num_matrix[i] = num_row[i] * 32;
//		D1[i] = num_row[i]*32 - D[i];
//		Kw[i] = 3*num_matrix[i];
//			if(catgry_ue==1)		N_soft = 250368;
//			else if(catgry_ue==2)	N_soft = 1237248;
//			else if(catgry_ue==3)   N_soft = 1237248;
//			else if(catgry_ue==4)	N_soft = 1827072;
//			else if(catgry_ue==5)	N_soft = 3667200;
//			else					assert(0);
//		    int M_limit=8;
//		    int N_IR;
//			N_IR = N_soft / (k_mimo*min(num_harqprocess,M_limit));
//			Ncb[i] = min(N_IR/(*code_num), Kw[i]);
//		if(i <= ((*code_num)-r-1))   Er[i] = G1_pdsch/(*code_num) * NL*qm;
//		else					  Er[i] = (G1_pdsch+(*code_num)-1)/(*code_num) * NL*qm;
//		k0[i] = num_row[i] * ((Ncb[i]+8*num_row[i]-1)/(8*num_row[i]) *2*rv_idx + 2);
//		z[i]=k0[i]/num_row[i];
//		}
//
//        int m;
//        int n;
//        int x;
//        int y;
//        m=0;
//        n=D[0];
//        x=0;
//        y=Er[0]-1;
//
//for(i=0;i<(*code_num);i++)
//{
//          for(j=0;j<32;j++)
//		  {
//			tmp = p[j];
//			p1[j] = num_row[i]*tmp-q[tmp]-k0[i]+z[i];
//			p1[j]+=x;
//		  }
//
//
//        for(j=0;j<D[i];j++)
//		{
//			kk  = j+D1[i];
//			k= p1[kk&0x1f]+(kk>>5);
//			if(k<x)	d0_buf[j+m] = 0;
//			else d0_buf[j+m]=inbuf[k];
//			kk++;
//		}
//
//        for(j=0;j<32;j++)
//		{
//			tmp = p[j];
//			d1[j] = 2*num_row[i]*tmp+num_matrix[i]-k0[i]+z[i]-q1[tmp];
//			d1[j]+=x;
//		}
//
//		for(j=0;j<D[i];j++)
//		{
//			kk=j+D1[i];
//			k=d1[kk%32]+kk/32*2;
//			if(k>y)	d1_buf[j+m] = 0;
//			else d1_buf[j+m]=inbuf[k];
//		}
//
//        for(j=0;j<32;j++)
//		{
//			tmp = p[j];
//			d1[j] = 2*num_row[i]*tmp+num_matrix[i]-k0[i]+z[i]+1-q1[tmp];
//			d1[j]+=x;
//		}
//
//		for(j=0;j<D[i];j++)
//		{
//			kk=j+D1[i]-1;
//			k=d1[kk%32]+kk/32*2;
//			if(k>y)	d2_buf[j+m] = 0;
//			else d2_buf[j+m]=inbuf[k];
//		}
//
//        m+=D[i];
//		x+=Er[i];
//		y+=Er[i];
//}
//int co=0;
//for(i=0;i<(*code_num);i++)
//{
//	for(j=0;j<D[i];j++)
//	{
//		d00_buf[i][j]=d0_buf[co];
//		d00_buf[i][j]=d0_buf[co];
//		d00_buf[i][j]=d0_buf[co];
//		jj++;
//	}
//}
//		return 0;
//
//}

//int deratematch_pdsch(float *inbuf, int inlen, int transblock_size, int catgry_ue
//										, int num_harqprocess, int qm, int rv_idx, int NL, int k_mimo
//										, float **outbuf0, float **outbuf1, float **outbuf2
//										, int *size_each_codeseg, int *num_codeseg, int *num_nullbit)
//{
//	int i,j,d;
//    int G1_pdsch;
//	int size_each_intlvseg[13];
//	int size_each_selectseg[13];
//
//	#pragma SET_DATA_SECTION("uerx_data")
//	static int16 inbuf2_turbo[7000];
//
//	static int16 *outbuf0_turbo[13], outbuf0_turbo_mem[13][7000];
//	static int16 *outbuf1_turbo[13], outbuf1_turbo_mem[13][7000];
//	static int16 *outbuf2_turbo[13], outbuf2_turbo_mem[13][7000];
//	static int16 *outbuf0_intlv[13], outbuf0_intlv_mem[13][7000];
//	static int16 *outbuf1_intlv[13], outbuf1_intlv_mem[13][7000];
//	static int16 *outbuf2_intlv[13], outbuf2_intlv_mem[13][7000];
//	static int16 *outbuf_collect[13], outbuf_collect_mem[13][21000];
//	static int16 *outbuf_select[13], outbuf_select_mem[13][21000];
//
//	static float *outbuf_deblockseg[13], outbuf_deblockseg_mem[13][21000];
//	static float *outbuf_deselect[13], outbuf_deselect_mem[13][21000];
//	static float *outbuf0_decollect[13], outbuf0_decollect_mem[13][7000];
//	static float *outbuf1_decollect[13], outbuf1_decollect_mem[13][7000];
//	static float *outbuf2_decollect[13], outbuf2_decollect_mem[13][7000];
//	#pragma SET_DATA_SECTION("uerx_data")
//
//	for(i=0; i<13; i++)
//	{
//		outbuf0_turbo[i] = outbuf0_turbo_mem[i];
//		outbuf1_turbo[i] = outbuf1_turbo_mem[i];
//		outbuf2_turbo[i] = outbuf2_turbo_mem[i];
//		outbuf0_intlv[i] = outbuf0_intlv_mem[i];
//		outbuf1_intlv[i] = outbuf1_intlv_mem[i];
//		outbuf2_intlv[i] = outbuf2_intlv_mem[i];
//		outbuf_collect[i] = outbuf_collect_mem[i];
//		outbuf_select[i] = outbuf_select_mem[i];
//
//		outbuf_deblockseg[i] = outbuf_deblockseg_mem[i];
//		outbuf_deselect[i] = outbuf_deselect_mem[i];
//		outbuf0_decollect[i] = outbuf0_decollect_mem[i];
//		outbuf1_decollect[i] = outbuf1_decollect_mem[i];
//		outbuf2_decollect[i] = outbuf2_decollect_mem[i];
//	}
//
//	// function body
//	blockseg(transblock_size+24, size_each_codeseg, num_codeseg, num_nullbit);
//
//	int D[10];
//	int num_row[10];
//	int num_matrix[10];
//	int D1[10];
//	for(i=0; i<(*num_codeseg); i++)
//		{
//			if(i == 0)
//			{
//				// the first block, set the null bit of outbuf0_turbo, outbuf1_turbo and outbuf2_turbo
//				for(j=0; j<size_each_codeseg[0]+4; j++)
//				{
//					if(j < *num_nullbit)
//					{
//						outbuf0_turbo[0][j] = -1;
//						outbuf1_turbo[0][j] = -1;
//					}
////					else
////					{
////						outbuf0_turbo[0][j] = 0;
////						outbuf1_turbo[0][j] = 0;
////					}
//				}
//
//				for(j=0; j<size_each_codeseg[0]; j++)
//				{
//					if(j < *num_nullbit)
//					{
//						inbuf2_turbo[j] = -1;
//					}
////					else
////					{
////						inbuf2_turbo[j] = 0;
////					}
//				}
//
//				intlv_turbo(inbuf2_turbo, size_each_codeseg[0], outbuf2_turbo[0]);
//
//				outbuf2_turbo[0][0+size_each_codeseg[0]] = 0;
//				outbuf2_turbo[0][1+size_each_codeseg[0]] = 0;
//				outbuf2_turbo[0][2+size_each_codeseg[0]] = 0;
//				outbuf2_turbo[0][3+size_each_codeseg[0]] = 0;
//			}
//
//	    intlv_sub_1th_2th_turbo(outbuf0_turbo, size_each_codeseg, *num_codeseg, outbuf0_intlv, size_each_intlvseg);
//		intlv_sub_1th_2th_turbo(outbuf1_turbo, size_each_codeseg, *num_codeseg, outbuf1_intlv, size_each_intlvseg);
//		intlv_sub_3th_turbo(outbuf2_turbo, size_each_codeseg, *num_codeseg, outbuf2_intlv, size_each_intlvseg);
//
//
//		 for(i=1;i<(*num_codeseg);i++)
//	    {
////			for(j=0; j<size_each_codeseg[i]+4; j++)
////						{
////							outbuf0_intlv[i][j] = 0;
////							outbuf1_intlv[i][j] = 0;
////							outbuf2_intlv[i][j] = 0;
////						}
////	    size_each_intlvseg[i]=6176;
//	    D[i]=size_each_codeseg[i]+4;
//	    num_row[i]=(D[i]+31)/32;
//	    num_matrix[i]=num_row[i]*32;
//	    size_each_intlvseg[i]=num_matrix[i];
//	    D1[i]=num_row[i]*32-D[i];
//	    if(D1[i]==28)
//	    {
//	    outbuf0_intlv[i][0]=-1,outbuf0_intlv[i][num_row[i]]=-1,outbuf0_intlv[i][2*num_row[i]]=-1,outbuf0_intlv[i][3*num_row[i]]=-1,outbuf0_intlv[i][4*num_row[i]]=-1,outbuf0_intlv[i][5*num_row[i]]=-1,outbuf0_intlv[i][6*num_row[i]]=-1;
//	   	outbuf0_intlv[i][8*num_row[i]]=-1,outbuf0_intlv[i][9*num_row[i]]=-1,outbuf0_intlv[i][10*num_row[i]]=-1,outbuf0_intlv[i][11*num_row[i]]=-1,outbuf0_intlv[i][12*num_row[i]]=-1,outbuf0_intlv[i][13*num_row[i]]=-1;
//	    outbuf0_intlv[i][14*num_row[i]]=-1,outbuf0_intlv[i][16*num_row[i]]=-1,outbuf0_intlv[i][17*num_row[i]]=-1, outbuf0_intlv[i][18*num_row[i]]=-1,outbuf0_intlv[i][19*num_row[i]]=-1,outbuf0_intlv[i][20*num_row[i]]=-1;
//	   	outbuf0_intlv[i][21*num_row[i]]=-1,outbuf0_intlv[i][22*num_row[i]]=-1,outbuf0_intlv[i][24*num_row[i]]=-1,outbuf0_intlv[i][25*num_row[i]]=-1,outbuf0_intlv[i][26*num_row[i]]=-1,outbuf0_intlv[i][27*num_row[i]]=-1;
//	    outbuf0_intlv[i][28*num_row[i]]=-1,outbuf0_intlv[i][29*num_row[i]]=-1,outbuf0_intlv[i][30*num_row[i]]=-1,outbuf1_intlv[i][0]=-1,outbuf1_intlv[i][num_row[i]]=-1,outbuf1_intlv[i][2*num_row[i]]=-1;
//	    outbuf1_intlv[i][3*num_row[i]]=-1,outbuf1_intlv[i][4*num_row[i]]=-1,outbuf1_intlv[i][5*num_row[i]]=-1,outbuf1_intlv[i][6*num_row[i]]=-1,outbuf1_intlv[i][8*num_row[i]]=-1,outbuf1_intlv[i][9*num_row[i]]=-1;
//	    outbuf1_intlv[i][10*num_row[i]]=-1,outbuf1_intlv[i][11*num_row[i]]=-1,outbuf1_intlv[i][12*num_row[i]]=-1,outbuf1_intlv[i][13*num_row[i]]=-1,outbuf1_intlv[i][14*num_row[i]]=-1,outbuf1_intlv[i][16*num_row[i]]=-1;
//	    outbuf1_intlv[i][17*num_row[i]]=-1,outbuf1_intlv[i][18*num_row[i]]=-1,outbuf1_intlv[i][19*num_row[i]]=-1,outbuf1_intlv[i][20*num_row[i]]=-1,outbuf1_intlv[i][21*num_row[i]]=-1,outbuf1_intlv[i][22*num_row[i]]=-1;
//	    outbuf1_intlv[i][24*num_row[i]]=-1,outbuf1_intlv[i][25*num_row[i]]=-1,outbuf1_intlv[i][26*num_row[i]]=-1,outbuf1_intlv[i][27*num_row[i]]=-1,outbuf1_intlv[i][28*num_row[i]]=-1,outbuf1_intlv[i][29*num_row[i]]=-1;
//	    outbuf1_intlv[i][30*num_row[i]]=-1,outbuf2_intlv[i][0]=-1,outbuf2_intlv[i][num_row[i]]=-1,outbuf2_intlv[i][2*num_row[i]]=-1,outbuf2_intlv[i][3*num_row[i]]=-1,outbuf2_intlv[i][4*num_row[i]]=-1;
//	    outbuf2_intlv[i][5*num_row[i]]=-1,outbuf2_intlv[i][6*num_row[i]]=-1,outbuf2_intlv[i][8*num_row[i]]=-1,outbuf2_intlv[i][9*num_row[i]]=-1,outbuf2_intlv[i][10*num_row[i]]=-1,outbuf2_intlv[i][12*num_row[i]]=-1;
//	    outbuf2_intlv[i][13*num_row[i]]=-1,outbuf2_intlv[i][14*num_row[i]]=-1,outbuf2_intlv[i][16*num_row[i]]=-1,outbuf2_intlv[i][17*num_row[i]]=-1,outbuf2_intlv[i][18*num_row[i]]=-1,outbuf2_intlv[i][19*num_row[i]]=-1;
//	    outbuf2_intlv[i][20*num_row[i]]=-1,outbuf2_intlv[i][21*num_row[i]]=-1,outbuf2_intlv[i][22*num_row[i]]=-1,outbuf2_intlv[i][23*num_row[i]]=-1,outbuf2_intlv[i][24*num_row[i]]=-1,outbuf2_intlv[i][25*num_row[i]]=-1;
//	    outbuf2_intlv[i][26*num_row[i]]=-1,outbuf2_intlv[i][28*num_row[i]]=-1,outbuf2_intlv[i][29*num_row[i]]=-1,outbuf2_intlv[i][30*num_row[i]]=-1,outbuf2_intlv[i][32*num_row[i]-1]=-1;
//	    }
//	    }
//	}
//	    uerx_bit_collect_turbo(outbuf0_intlv, outbuf1_intlv, outbuf2_intlv, size_each_intlvseg, *num_codeseg, outbuf_collect);
//
//
//	// decode code block concatenation
//	d = 0;
//	G1_pdsch = inlen / (NL*qm);
//
//	for(i=0; i<(*num_codeseg); i++)
//		{
//		int r,Er;
//		r = G1_pdsch % (*num_codeseg);
//		if(i <= ((*num_codeseg)-r-1))   Er = G1_pdsch/(*num_codeseg) * NL*qm;
//				else					    Er = (G1_pdsch+(*num_codeseg)-1)/(*num_codeseg) * NL*qm;
//		size_each_selectseg[i] = Er;
//		}
////	for(i=0; i<*num_codeseg; i++)
////	{
////		for(j=0; j<size_each_selectseg[i]; j++)
////		{
////			outbuf_deblockseg[i][j] = inbuf[d];
////			d++;
////		}
////	}
//
//	// decode bit selection and pruning
//	deselect_turbo(outbuf_collect, size_each_intlvseg, *num_codeseg, rv_idx, qm, NL, num_harqprocess
//		, inlen, k_mimo, catgry_ue, inbuf/*outbuf_deblockseg*/, outbuf_deselect);
//
//	// decode bit collection
//	decollect_turbo(outbuf_deselect, size_each_intlvseg, *num_codeseg, outbuf0_decollect, outbuf1_decollect
//		, outbuf2_decollect);
//
//	// decode sub-block interleaver
//	deintlv_sub_1th_2th_turbo(outbuf0_decollect, size_each_codeseg, *num_codeseg, outbuf0);
//	deintlv_sub_1th_2th_turbo(outbuf1_decollect, size_each_codeseg, *num_codeseg, outbuf1);
//	deintlv_sub_3th_turbo(outbuf2_decollect, size_each_codeseg, *num_codeseg, outbuf2);
//
// 	return 0;
//}

int mi(int a,int b)
{
   if(a<b)
	   return a;
   else
	   return b;
}

//int deratematch_pdsch(float *inbuf1, int inlen, int transblock_size, int catgry_ue
//										, int num_harqprocess, int qm, int rv_idx, int NL, int k_mimo
//										, float **d0_buf, float **d1_buf, float **d2_buf
//										, int *code_buf, int *code_num, int *F)
//{
//    int N_soft;
//	static float inbuf[77184]={0};
//	int k=0;
//	int tmp=0;
//	int j=0;
//	int i;
//	int z[13]={0};
//	int kk;
//	int r;
//	int p1[32]={0};
//	int d1[32]={0};
//	int q[32]={1,2,3,4,5,6,7,7,8,9,10,11,12,13,14,14,15,16,17,18,19,20,21,21,22,23,24,25,26,27,28,28};
//	int p[32] = {0,16,8,24,4,20,12,28,2,18,10,26,6,22,14,30,1,17,9,25,5,21,13,29,3,19,11,27,7,23,15,31};
//	int q1[32]={30,32,34,36,38,40,42,42,44,46,48,50,52,54,56,56,58,60,62,64,66,68,70,70,72,74,76,77,79,81,83,83};
//
//    int D[13]={0};
//    int size_each_codeseg[13]={0};
//    int num_matrix[13]={0};
//    int num_row[13]={0};
//    int D1[13]={0};
//    int k0[13]={0};
//
//
//
//	for(i=0;i<inlen;i++)
//	{
//		inbuf[i]=inbuf1[i];
//	}
//
//	blockseg(transblock_size+24, size_each_codeseg, code_num, F);
//
//	int G_pdsch;
//	int G1_pdsch;
//	int Kw[13]={0};
//	int Ncb[13]={0};
//	int Er[13]={0};
//
//	G_pdsch=inlen;
//	G1_pdsch = G_pdsch / (NL*qm);
//
//	r = G1_pdsch % (*code_num);
//
//    for(i=0;i<(*code_num);i++)
//	{
//	D[i] = size_each_codeseg[i]+4;
//	num_row[i] = (D[i]+31) / 32;
//	num_matrix[i] = num_row[i] * 32;
//	D1[i] = num_row[i]*32 - D[i];
//	Kw[i] = 3*num_matrix[i];
//	if(catgry_ue==1)		N_soft = 250368;
//	else if(catgry_ue==2)	N_soft = 1237248;
//	else if(catgry_ue==3)   N_soft = 1237248;
//	else if(catgry_ue==4)	N_soft = 1827072;
//	else if(catgry_ue==5)	N_soft = 3667200;
//	else					assert(0);
//    int M_limit=8;
//    int N_IR;
//	N_IR = N_soft / (k_mimo*min(num_harqprocess,M_limit));
//	Ncb[i] = min(N_IR/(*code_num), Kw[i]);
////	Ncb[i] = mi(code_num, 5);
////	Ncb[i] = ((N_IR/code_num) < Kw[i])?(N_IR/code_num):Kw[i];
//	if(i <= ((*code_num)-r-1))   Er[i] = G1_pdsch/(*code_num) * NL*qm;
//	else					  Er[i] = (G1_pdsch+(*code_num)-1)/(*code_num) * NL*qm;
//	k0[i] = num_row[i] * ((Ncb[i]+8*num_row[i]-1)/(8*num_row[i]) *2*rv_idx + 2);
//	z[i]=k0[i]/num_row[i];
//	}
//
//
//
//    int m;
//    int n;
//    int x;
//    int y;
//    m=0;
//    n=D[0];
//    x=0;
//    y=Er[0]-1;
//
//for(i=0;i<(*code_num);i++)
//{
//      for(j=0;j<32;j++)
//	  {
//		tmp = p[j];
//		p1[j] = num_row[i]*tmp-q[tmp]-k0[i]+z[i];
//		p1[j]+=x;
//	  }
//
//
//    for(j=0;j<D[i];j++)
//	{
//		kk  = j+D1[i];
//		k= p1[kk&0x1f]+(kk>>5);
//		if(k<x)	d0_buf[i][j] = 0;
//		else d0_buf[i][j]=inbuf[k];
//		kk++;
//	}
//
//    for(j=0;j<32;j++)
//	{
//		tmp = p[j];
//		d1[j] = 2*num_row[i]*tmp+num_matrix[i]-k0[i]+z[i]-q1[tmp];
//		d1[j]+=x;
//	}
//
//	for(j=0;j<D[i];j++)
//	{
//		kk=j+D1[i];
//		k=d1[kk%32]+kk/32*2;
//		if(k>y)	d1_buf[i][j] = 0;
//		else d1_buf[i][j]=inbuf[k];
//	}
//
//    for(j=0;j<32;j++)
//	{
//		tmp = p[j];
//		d1[j] = 2*num_row[i]*tmp+num_matrix[i]-k0[i]+z[i]+1-q1[tmp];
//		d1[j]+=x;
//	}
//
//	for(j=0;j<D[i];j++)
//	{
//		kk=j+D1[i]-1;
//		k=d1[kk%32]+kk/32*2;
//		if(k>y)	d2_buf[i][j] = 0;
//		else d2_buf[i][j]=inbuf[k];
//	}
//
////    m+=D[i];
//	x+=Er[i];
//	y+=Er[i];
//}
//
//	return 0;
//
//}



//int deratematch_pdsch(float *inbuf, int inlen, int transblock_size, int catgry_ue
//										, int num_harqprocess, int qm, int rv_idx, int NL, int k_mimo
//										, float **outbuf0, float **outbuf1, float **outbuf2
//										, int *size_each_codeseg, int *num_codeseg, int *num_nullbit)
//{
//	int i,j,d;
//
//	int size_each_intlvseg[13];
//	int size_each_selectseg[13];
//
//	#pragma SET_DATA_SECTION("uerx_data")
//	static int16 inbuf2_turbo[7000];
//
//	static int16 *outbuf0_turbo[13], outbuf0_turbo_mem[13][7000];
//	static int16 *outbuf1_turbo[13], outbuf1_turbo_mem[13][7000];
//	static int16 *outbuf2_turbo[13], outbuf2_turbo_mem[13][7000];
//	static int16 *outbuf0_intlv[13], outbuf0_intlv_mem[13][7000];
//	static int16 *outbuf1_intlv[13], outbuf1_intlv_mem[13][7000];
//	static int16 *outbuf2_intlv[13], outbuf2_intlv_mem[13][7000];
//	static int16 *outbuf_collect[13], outbuf_collect_mem[13][21000];
//	static int16 *outbuf_select[13], outbuf_select_mem[13][21000];
//
//	static float *outbuf_deblockseg[13], outbuf_deblockseg_mem[13][21000];
//	static float *outbuf_deselect[13], outbuf_deselect_mem[13][21000];
//	static float *outbuf0_decollect[13], outbuf0_decollect_mem[13][7000];
//	static float *outbuf1_decollect[13], outbuf1_decollect_mem[13][7000];
//	static float *outbuf2_decollect[13], outbuf2_decollect_mem[13][7000];
//	#pragma SET_DATA_SECTION("uerx_data")
//
//	for(i=0; i<13; i++)
//	{
//		outbuf0_turbo[i] = outbuf0_turbo_mem[i];
//		outbuf1_turbo[i] = outbuf1_turbo_mem[i];
//		outbuf2_turbo[i] = outbuf2_turbo_mem[i];
//		outbuf0_intlv[i] = outbuf0_intlv_mem[i];
//		outbuf1_intlv[i] = outbuf1_intlv_mem[i];
//		outbuf2_intlv[i] = outbuf2_intlv_mem[i];
//		outbuf_collect[i] = outbuf_collect_mem[i];
//		outbuf_select[i] = outbuf_select_mem[i];
//
//		outbuf_deblockseg[i] = outbuf_deblockseg_mem[i];
//		outbuf_deselect[i] = outbuf_deselect_mem[i];
//		outbuf0_decollect[i] = outbuf0_decollect_mem[i];
//		outbuf1_decollect[i] = outbuf1_decollect_mem[i];
//		outbuf2_decollect[i] = outbuf2_decollect_mem[i];
//	}
//
//	// function body
//	blockseg(transblock_size+24, size_each_codeseg, num_codeseg, num_nullbit);
//
//
//
//
//	// init the outbuf0_turbo, outbuf1_turbo and outbuf2_turbo
//	for(i=0; i<(*num_codeseg); i++)
//	{
//		if(i == 0)
//		{
//			// the first block, set the null bit of outbuf0_turbo, outbuf1_turbo and outbuf2_turbo
//			for(j=0; j<size_each_codeseg[0]+4; j++)
//			{
//				if(j < *num_nullbit)
//				{
//					outbuf0_turbo[0][j] = -1;
//					outbuf1_turbo[0][j] = -1;
//				}
//				else
//				{
//					outbuf0_turbo[0][j] = 0;
//					outbuf1_turbo[0][j] = 0;
//				}
//			}
//
//			for(j=0; j<size_each_codeseg[0]; j++)
//			{
//				if(j < *num_nullbit)
//				{
//					inbuf2_turbo[j] = -1;
//				}
//				else
//				{
//					inbuf2_turbo[j] = 0;
//				}
//			}
//
//			intlv_turbo(inbuf2_turbo, size_each_codeseg[0], outbuf2_turbo[0]);
//
//			outbuf2_turbo[0][0+size_each_codeseg[0]] = 0;
//			outbuf2_turbo[0][1+size_each_codeseg[0]] = 0;
//			outbuf2_turbo[0][2+size_each_codeseg[0]] = 0;
//			outbuf2_turbo[0][3+size_each_codeseg[0]] = 0;
//		}
//		else
//		{
//			for(j=0; j<size_each_codeseg[i]+4; j++)
//			{
//				outbuf0_turbo[i][j] = 0;
//				outbuf1_turbo[i][j] = 0;
//				outbuf2_turbo[i][j] = 0;
//			}
//		}
//    }
//
//	// sub-block interleaver
//	intlv_sub_1th_2th_turbo(outbuf0_turbo, size_each_codeseg, *num_codeseg, outbuf0_intlv, size_each_intlvseg);
//	intlv_sub_1th_2th_turbo(outbuf1_turbo, size_each_codeseg, *num_codeseg, outbuf1_intlv, size_each_intlvseg);
//	intlv_sub_3th_turbo(outbuf2_turbo, size_each_codeseg, *num_codeseg, outbuf2_intlv, size_each_intlvseg);
//
//	// bit collection
//	uerx_bit_collect_turbo(outbuf0_intlv, outbuf1_intlv, outbuf2_intlv, size_each_intlvseg, *num_codeseg, outbuf_collect);
//
//	// bit selection
//	select_turbo(outbuf_collect, size_each_intlvseg, *num_codeseg, rv_idx, qm, NL, num_harqprocess
//		, inlen, k_mimo, catgry_ue, outbuf_select, size_each_selectseg);
//
//	// decode code block concatenation
//	d = 0;
//
//	for(i=0; i<*num_codeseg; i++)
//	{
//		for(j=0; j<size_each_selectseg[i]; j++)
//		{
//			outbuf_deblockseg[i][j] = inbuf[d];
//			d++;
//		}
//	}
//
//	// decode bit selection and pruning
//	deselect_turbo(outbuf_collect, size_each_intlvseg, *num_codeseg, rv_idx, qm, NL, num_harqprocess
//		, inlen, k_mimo, catgry_ue, outbuf_deblockseg, outbuf_deselect);
//
//	// decode bit collection
//	decollect_turbo(outbuf_deselect, size_each_intlvseg, *num_codeseg, outbuf0_decollect, outbuf1_decollect
//		, outbuf2_decollect);
//
//	// decode sub-block interleaver
//	deintlv_sub_1th_2th_turbo(outbuf0_decollect, size_each_codeseg, *num_codeseg, outbuf0);
//	deintlv_sub_1th_2th_turbo(outbuf1_decollect, size_each_codeseg, *num_codeseg, outbuf1);
//	deintlv_sub_3th_turbo(outbuf2_decollect, size_each_codeseg, *num_codeseg, outbuf2);
//
// 	return 0;
//}

int deratematch_pdcch(float *inbuf, int inlen, int transblock_size, float *outbuf0
										, float *outbuf1, float *outbuf2)
{
	int i;
	int outlen_intlv;
	int outlen_collect;

	#pragma SET_DATA_SECTION("uerx_data")
	static int16 d0_conv[128];
	static int16 d1_conv[128];
	static int16 d2_conv[128];
	static int16 outbuf0_intlv[128];
	static int16 outbuf1_intlv[128];
	static int16 outbuf2_intlv[128];
	static int16 outbuf_collect[384];

	static float outbuf0_decollect[128];
	static float outbuf1_decollect[128];
	static float outbuf2_decollect[128];
	static float outbuf_deselect[384];
	#pragma SET_DATA_SECTION()

	// function body
	for(i=0; i<transblock_size; i++)
	{
		d0_conv[i] = 0;
		d1_conv[i] = 0;
		d2_conv[i] = 0;
	}

	outlen_intlv = intlv_sub_conv(d0_conv, transblock_size+16, outbuf0_intlv);
	outlen_intlv = intlv_sub_conv(d1_conv, transblock_size+16, outbuf1_intlv);
	outlen_intlv = intlv_sub_conv(d2_conv, transblock_size+16, outbuf2_intlv);

	outlen_collect = collect_conv(outbuf0_intlv, outbuf1_intlv, outbuf2_intlv, outlen_intlv, outbuf_collect);

	// decode bit selection
    deselect_conv(outbuf_collect, outlen_collect, inbuf, inlen, outbuf_deselect);

	// decode bit collection
	decollect_conv(outbuf_deselect, outlen_collect, outbuf0_decollect, outbuf1_decollect, outbuf2_decollect);

	// decode interleaver
	deintlv_sub_conv(outbuf0_decollect, transblock_size+16, outbuf0);
	deintlv_sub_conv(outbuf1_decollect, transblock_size+16, outbuf1);
	deintlv_sub_conv(outbuf2_decollect, transblock_size+16, outbuf2);

	return (transblock_size+16);
}

int deratematch_pbch(float *inbuf, int inlen, int transblock_size, float *outbuf0
										, float *outbuf1, float *outbuf2)
{
	int i;
	int outlen_intlv;
	int outlen_collect;
	
	#pragma SET_DATA_SECTION("uerx_data")
	static int16 d0_conv[128];
	static int16 d1_conv[128];
	static int16 d2_conv[128];
	static int16 outbuf0_intlv[128];
	static int16 outbuf1_intlv[128];
	static int16 outbuf2_intlv[128];
	static int16 outbuf_collect[384];

	static float outbuf0_decollect[128];
	static float outbuf1_decollect[128];
	static float outbuf2_decollect[128];
	static float outbuf_deselect[384];
	#pragma SET_DATA_SECTION()

	// function body
	for(i=0; i<transblock_size; i++)
	{
		d0_conv[i] = 0;
		d1_conv[i] = 0;
		d2_conv[i] = 0;
	}

	outlen_intlv = intlv_sub_conv(d0_conv, transblock_size+16, outbuf0_intlv);
	outlen_intlv = intlv_sub_conv(d1_conv, transblock_size+16, outbuf1_intlv);
	outlen_intlv = intlv_sub_conv(d2_conv, transblock_size+16, outbuf2_intlv);

	outlen_collect = collect_conv(outbuf0_intlv, outbuf1_intlv, outbuf2_intlv, outlen_intlv, outbuf_collect);

	// decode bit selection
    deselect_conv(outbuf_collect, outlen_collect, inbuf, inlen, outbuf_deselect);


	// decode bit collection
	decollect_conv(outbuf_deselect, outlen_collect, outbuf0_decollect, outbuf1_decollect
		, outbuf2_decollect);;

	// decode interleaver
	deintlv_sub_conv(outbuf0_decollect, transblock_size+16, outbuf0);
	deintlv_sub_conv(outbuf1_decollect, transblock_size+16, outbuf1);
	deintlv_sub_conv(outbuf2_decollect, transblock_size+16, outbuf2);

	return (transblock_size+16);
}


int intlv_turbo(int16 *inbuf, int inlen, int16 *outbuf)
{
    int i, m;
	int f1, f2;
	
	// get f1 and f2
	for(i=0; i<188; i++)
	{
		if(UERX_TURBO_INTLV_PARAMETER[i][1] == inlen)
		{
			f1 = UERX_TURBO_INTLV_PARAMETER[i][2];
			f2 = UERX_TURBO_INTLV_PARAMETER[i][3];

			break;
		}
	}

	// interleaver for the first code segment
	for(i=0; i<inlen; i++)
	{
		m = i*((f1+f2*i)%inlen) % inlen;

		outbuf[i] = inbuf[m];
	}

	return 0;	
}


int intlv_sub_1th_2th_turbo(int16 **inbuf, int *size_each_codeseg, int num_codeseg
												   , int16 **outbuf, int *size_each_intlvseg)
{
	int i, j, k, m;
	int D;	// the number bit of every input segment
	int D1; // the number of dummy bit
	int num_row; // the row number of interleaver
	int num_matrix; // the size of matrix
	
	int16 buffer[200][32];

	// inter column permutation pattern for sub-block interleaver
	int p[32] = {0,16,8,24,4,20,12,28,2,18,10,26,6,22,14,30,1,17,9,25,5,21,13,29,3,19,11,27,7,23,15,31};

	// function body
	for(k=0; k<1/*num_codeseg*/; k++)
	{
		D = size_each_codeseg[k]+4;
		num_row = (D+31) / 32;
		num_matrix = num_row * 32;
		D1 = num_row*32 - D;

		// dummy bits are padded
		for(i=0; i<num_row; i++)
		{
			for(j=0; j<32; j++)
			{
				m = i*32+j - D1;
				
				if(m < 0) buffer[i][j] = -1;
				else	  buffer[i][j] = inbuf[k][m];
			}
		}
		
		m = 0; // clear the index for every segment output

		// column exchange and output the interleaver result
		for(j=0; j<32; j++)
		{
			for(i=0; i<num_row; i++)
			{
				outbuf[k][m] = buffer[i][p[j]];
				m++;
			}
		}

		size_each_intlvseg[k]	 = num_matrix;
	}

	return 0;
}


int intlv_sub_3th_turbo(int16 **inbuf, int *size_each_codeseg, int num_codeseg
												   , int16 **outbuf, int *size_each_intlvseg)
{
	int i, k, m;
	int D;	// the number bit of every input segment
	int D1; // the number of dummy bit
	int num_row; // the row number of interleaver
	int num_matrix; // the size of matrix

	int16 buffer[6400];

	// inter column permutation pattern for sub-block interleaver
	int p[32] = {0,16,8,24,4,20,12,28,2,18,10,26,6,22,14,30,1,17,9,25,5,21,13,29,3,19,11,27,7,23,15,31};

	// function body
	for(k=0; k<1/*num_codeseg*/; k++)
	{
		D = size_each_codeseg[k]+4;
		num_row = (D+31) / 32;
		num_matrix = num_row * 32;
		D1 = num_row*32 - D;

		// dummy bit are padded
		for(i=0; i<D1; i++)
		{
			buffer[i] = -1;
		}
			
		for(i=D1; i<num_matrix; i++)
		{
			buffer[i] = inbuf[k][i-D1];
		}

		// get inbuf data
		for(i=0; i<num_matrix; i++)
		{
			m = (p[i/num_row] + 32*(i%num_row) + 1) % num_matrix;
            outbuf[k][i] = buffer[m];
		}

		size_each_intlvseg[k]	 = num_matrix;
	}

	return 0;
}

int uerx_bit_collect_turbo(int16 **inbuf0, int16 **inbuf1, int16 **inbuf2, int *size_each_intlvseg
										, int num_codeseg, int16 **outbuf)
{
	int i, j;
	
	// function body
	for(i=0; i<num_codeseg; i++)
	{
		for(j=0; j<size_each_intlvseg[i]; j++)
		{
			outbuf[i][j]						   = inbuf0[i][j];
			outbuf[i][2*j+0+size_each_intlvseg[i]] = inbuf1[i][j];
			outbuf[i][2*j+1+size_each_intlvseg[i]] = inbuf2[i][j];
		}			    
	}

	return 0;
}

int select_turbo(int16 **inbuf, int *size_each_intlvseg, int num_codeseg, int rv_idx, int qm
									   , int NL, int num_harqprocess, int G_pdsch, int k_mimo, int catgry_ue
									   , int16 **outbuf, int *size_each_selectseg)
{
	int i, j, k, k0, Er, r;
	int N_soft, M_limit, N_IR;
	int G1_pdsch, Kw, Ncb;
	int num_row;

	// Total number of soft channel bits
	if(catgry_ue==1)		N_soft = 250368;
	else if(catgry_ue==2)	N_soft = 1237248;
	else if(catgry_ue==3)   N_soft = 1237248;
	else if(catgry_ue==4)	N_soft = 1827072;
	else if(catgry_ue==5)	N_soft = 3667200;
	else					assert(0);

	M_limit = 8;

	N_IR = N_soft / (k_mimo*min(num_harqprocess,M_limit));

	G1_pdsch = G_pdsch / (NL*qm);

	r = G1_pdsch % num_codeseg;

	for(i=0; i<num_codeseg; i++)
	{
    	num_row = size_each_intlvseg[i] / 32;
		Kw = 3*size_each_intlvseg[i];

		Ncb = min(N_IR/num_codeseg, Kw);

		// the rate matching output sequence length for the r-th coded block
		if(i <= (num_codeseg-r-1))   Er = G1_pdsch/num_codeseg * NL*qm;  
		else					    Er = (G1_pdsch+num_codeseg-1)/num_codeseg * NL*qm;

		k0 = num_row * ((Ncb+8*num_row-1)/(8*num_row) *2*rv_idx + 2); 

        k = j = 0;
        while(k < Er)
		{
			if(inbuf[i][(k0+j)%Ncb] != -1)
			{
				outbuf[i][k] = inbuf[i][(k0+j)%Ncb];
				k++;
				j++;
			}
			else
			{
				j++;
			}
		}
		
		size_each_selectseg[i] = Er;
	}

	return 0;
}


int deselect_turbo(int16 **inbuf_flag, int *size_each_intlvseg, int num_codeseg, int rv_idx
										   , int qm, int NL, int num_harqprocess, int G_pdsch, int k_mimo
										   , int catgry_ue, float *inbuf, float **outbuf)
{
	int i, j, k, k0, Er, r;
	int N_soft, M_limit, N_IR;
	int G1_pdsch, Kw, Ncb;
	int num_row;
    int k1;
	// Total number of soft channel bits
	if(catgry_ue==1)		N_soft = 250368;  
	else if(catgry_ue==2)	N_soft = 1237248;
	else if(catgry_ue==3)   N_soft = 1237248;
	else if(catgry_ue==4)	N_soft = 1827072;
	else if(catgry_ue==5)	N_soft = 3667200;
	else					assert(0);

	M_limit = 8;

	N_IR = N_soft / (k_mimo*min(num_harqprocess,M_limit));

	G1_pdsch = G_pdsch / (NL*qm);

	r = G1_pdsch % num_codeseg;
	k1=0;
	for(i=0; i<num_codeseg; i++)
	{
    	num_row = size_each_intlvseg[i] / 32;
		Kw = 3*size_each_intlvseg[i];

		Ncb = min(N_IR/num_codeseg, Kw);

		// the rate matching output sequence length for the r-th coded block
		if(i <= (num_codeseg-r-1))   Er = G1_pdsch/num_codeseg * NL*qm;  
		else					    Er = (G1_pdsch+num_codeseg-1)/num_codeseg * NL*qm;

		k0 = num_row * ((Ncb+8*num_row-1)/(8*num_row) *2*rv_idx + 2); 

		for(j=0; j<Kw; j++)
		{
			outbuf[i][j] = 0;
		}

        k = j = 0;

        while(k < Er)
		{
			if(inbuf_flag[i][(k0+j)%Ncb] != -1)
			{
				// maximum ratio combining
				outbuf[i][(k0+j)%Ncb] += /*inbuf[i][k]*/inbuf[k1];
				k1++;
				k++;
				j++;
			}
			else
			{
				j++;
			}
		}
 //       k1+=Er;
	}

	return 0;
}


int decollect_turbo(float **inbuf, int *size_each_intlvseg, int num_codeseg
											, float **outbuf0, float **outbuf1, float **outbuf2)
{
	int i, j;
	
	// function body
	for(i=0; i<num_codeseg; i++)
	{
		for(j=0; j<size_each_intlvseg[i]; j++)
		{
			outbuf0[i][j] = inbuf[i][j];
			outbuf1[i][j] = inbuf[i][2*j+0+size_each_intlvseg[i]];
			outbuf2[i][j] = inbuf[i][2*j+1+size_each_intlvseg[i]];
		}			    
	}

	return 0;
}




int deintlv_sub_1th_2th_turbo(float **inbuf, int *size_each_codeseg, int num_codeseg
													   , float **outbuf)
{
	int i, j, k, m;
	int D;	// the number bit of every input segment
	int D1; // the number of dummy bit
	int num_row; // the row number of interleaver
	int num_matrix; // the size of matrix
	
	int16 buffer[200][32];

	// inter column permutation pattern for sub-block interleaver
	int p[32] = {0,16,8,24,4,20,12,28,2,18,10,26,6,22,14,30,1,17,9,25,5,21,13,29,3,19,11,27,7,23,15,31};

	// function body
	for(k=0; k<num_codeseg; k++)
	{
		D = size_each_codeseg[k]+4;
		num_row = (D+31) / 32;
		num_matrix = num_row * 32;
		D1 = num_row*32 - D;		
		
		// clear the index for every segment output
		m = 0; 

		// column exchange and output the interleaver result
		for(j=0; j<32; j++)
		{
			for(i=0; i<num_row; i++)
			{
				buffer[i][p[j]] = inbuf[k][m];
				m++;
			}
		}

		// dummy bits are removed
		for(i=0; i<num_row; i++)
		{
			for(j=0; j<32; j++)
			{
				m = i*32+j - D1;
				
				if(m >= 0)
				{
					outbuf[k][m] = buffer[i][j];
				}
			}
		}
	}

	return 0;
}


int deintlv_sub_3th_turbo(float **inbuf, int *size_each_codeseg, int num_codeseg
													   , float **outbuf)
{
	int i, k, m;
	int D;	// the number bit of every input segment
	int D1; // the number of dummy bit
	int num_row; // the row number of interleaver
	int num_matrix; // the size of matrix

	int16 buffer[6400];

	// inter column permutation pattern for sub-block interleaver
	int p[32] = {0,16,8,24,4,20,12,28,2,18,10,26,6,22,14,30,1,17,9,25,5,21,13,29,3,19,11,27,7,23,15,31};

	// function body
	for(k=0; k<num_codeseg; k++)
	{
		D = size_each_codeseg[k]+4;
		num_row = (D+31) / 32;
		num_matrix = num_row * 32;
		D1 = num_row*32 - D;		

		// get inbuf data
		for(i=0; i<num_matrix; i++)
		{
			m = (p[i/num_row] + 32*(i%num_row) + 1) % num_matrix;
            buffer[m] = inbuf[k][i];
		}

		// dummy bit are removed			
		for(i=D1; i<num_matrix; i++)
		{
			outbuf[k][i-D1] = buffer[i];
		}
	}

	return 0;
}



int intlv_sub_conv(int16 *inbuf, int inlen, int16 *outbuf)
{
	int i, j, m;
	int D;			// the number bit of every input segment
	int D1;			// the number of dummy bit
	int num_row;	// the row number of interleaver
	int16 buffer[200][32];

	// inter column permutation pattern for sub-block interleaver
	int p[32] = {1,17,9,25,5,21,13,29,3,19,11,27,7,23,15,31,0,16,8,24,4,20,12,28,2,18,10,26,6,22,14,30};

	D = inlen;
	num_row = (D+31) / 32;
	D1 = num_row * 32 - D;

	// dummy bits are padded
	for(i=0; i<num_row; i++)
	{
		for(j=0; j<32; j++)
		{
			m = i*32+j - D1;
			
			if(m < 0) buffer[i][j] = -1;
			else	  buffer[i][j] = inbuf[m];
		}
	}
	
	// clear the index for every segment output
	m = 0; 

	// column exchange and output the interleaver result
	for(j=0; j<32; j++)
	{
		for(i=0; i<num_row; i++)
		{
			outbuf[m++] = buffer[i][p[j]];
		}
	}

	return m;
}


int collect_conv(int16 *inbuf0, int16 *inbuf1, int16 *inbuf2, int inlen, int16 *outbuf)
{
	int i;
	
	for(i=0; i<inlen; i++)
	{
		outbuf[i+0*inlen] = inbuf0[i];
		outbuf[i+1*inlen] = inbuf1[i];
		outbuf[i+2*inlen] = inbuf2[i];
	}

	return (3*inlen);
}


int deselect_conv(int16 *inbuf_flag, int inlen_flag, float *inbuf, int inlen, float *outbuf)
{
	int i, j;

	for(i=0; i<inlen_flag; i++)
	{
		outbuf[i] = 0;
	}

	i = j = 0;
    while(i < inlen)
	{
		if(inbuf_flag[j%inlen_flag] != -1)
		{
			// maximum ratio combining
			outbuf[j%inlen_flag] += inbuf[i];
			i++;
			j++;
		}
		else
		{
			j++;
		}
	}

    return inlen_flag;
}


int decollect_conv(float *inbuf, int inlen, float *outbuf0, float *outbuf1, float *outbuf2)
{
	int i;
	int outlen;

	outlen = inlen/3;
	
	for(i=0; i<outlen; i++)
	{
		 outbuf0[i] = inbuf[i + 0*outlen];
		 outbuf1[i] = inbuf[i + 1*outlen];
		 outbuf2[i] = inbuf[i + 2*outlen];
	}

	return outlen;
}



int deintlv_sub_conv(float *inbuf, int sizeof_tbs_add_crc, float *outbuf)
{
	int i, j, m;
	int D;			// the number bit of every input segment
	int D1;			// the number of dummy bit
	int num_row;	// the row number of interleaver
	int16 buffer[200][32];

	// inter column permutation pattern for sub-block interleaver
	int p[32] = {1,17,9,25,5,21,13,29,3,19,11,27,7,23,15,31,0,16,8,24,4,20,12,28,2,18,10,26,6,22,14,30};

	D = sizeof_tbs_add_crc;
	num_row = (D+31) / 32;
	D1 = num_row * 32 - D;
	
	// clear the index for every segment output
	m = 0; 

	// column exchange and output the interleaver result
	for(j=0; j<32; j++)
	{
		for(i=0; i<num_row; i++)
		{
			 buffer[i][p[j]] = inbuf[m++];
		}
	}

	// dummy bits are removed
	for(i=0; i<num_row; i++)
	{
		for(j=0; j<32; j++)
		{
			m = i*32+j - D1;
			
			if(m >= 0)
			{
				 outbuf[m] = buffer[i][j];
			}
		}
	}

	return sizeof_tbs_add_crc;
}




int blockseg(int transblock_size, int *size_each_codseg, int *num_codeseg, int *num_nullbit)
{
	int i;
	int L;
	int B1;
	int K_plus, K_sub;
	int C_all, C_plus, C_sub;	

	int16 generator_CRC24B[25] = {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1};
	
	// function body
	if(transblock_size <= 6144)
	{
		for(i=0; i<188; i++)
		{
			if(UERX_TURBO_INTLV_PARAMETER[i][1] >= transblock_size)
			{
				K_plus = UERX_TURBO_INTLV_PARAMETER[i][1];
				break;
			}
		}

		*num_nullbit = K_plus - transblock_size;

		// output parameter
		*num_codeseg	= 1;
		size_each_codseg[0] = K_plus;

		return K_plus;
	}
	else
	{
		L = 24;
		C_all = (transblock_size+6144-L-1) / (6144-L);
		B1 = transblock_size + C_all*L;

		for(i=0; i<188; i++)
		{
			if(C_all*UERX_TURBO_INTLV_PARAMETER[i][1] >= B1)
			{
				K_plus = UERX_TURBO_INTLV_PARAMETER[i][1];
				K_sub  = UERX_TURBO_INTLV_PARAMETER[i-1][1];
				break;
			}
		}

		C_sub = (C_all*K_plus-B1) / (K_plus-K_sub);
		C_plus = C_all - C_sub;
		*num_nullbit = C_plus*K_plus + C_sub*K_sub - B1;

		// output parameter
		*num_codeseg = C_all;

		for(i=0; i<C_all; i++)
		{
			if(i<C_sub)		size_each_codseg[i] = K_sub;
			else			size_each_codseg[i] = K_plus;
		}

		return (C_plus*K_plus + C_sub*K_sub);
	}		
}
