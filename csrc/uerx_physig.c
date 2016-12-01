#include "foo.h"
#include "uerx_define.h"
#include "uerx_physig.h"


/************************************************************************/
/*
$funcname:	int uerx_physig::InputOutput(int16 N_ID_cell, cfloat* outbuf)

  $funcabs :	generator the primary synchronization signal.
  
	$parameters: 	 
	{
	$paraname0:  N_ID_cell ;  $paradir: in ;  $paratype: int16 ;    $pararange: [0,503];       
	$paraabs: cell ID;
	$paraname1:  outbuf	   ;  $paradir: in ;  $paratype: cfloat * ;	$pararange:		   ;	
	$paraabs: output buff. including the primary synchronization signal.
	}
	
	  $return: return the length of the outbuf. length = 62.
	  
		$creator: HuangShoujun  ;     $creattime: 06-24-2009;
		$mendor1:				;     $mendtime1:           ;    
		$mendor2:				;     $mendtime2:           ;
		$ref    : LTE 36.211,v8.6.0 p76
		$notes  :                                                                       
*/
/************************************************************************/
int physig_InputOutput1(int N_ID_cell, cfloat* outbuf)                    // pss
{
	int N_ID_2, u;
	int i;
	
	N_ID_2 = N_ID_cell%3;
	
	if (N_ID_2 == 0)		u = 25;
	else if(N_ID_2 == 1)	u = 29;
	else					u = 34;
	
	for(i = 0; i < 31; i++)
	{
		outbuf[i].re = (float)cos(0.0 - PI*u*i*(i+1) / 63);
		outbuf[i].im = (float)sin(0.0 - PI*u*i*(i+1) / 63);
	}
	
	for(i = 31; i < 62; i++)
	{
		outbuf[i].re = (float)cos(0.0 - PI*u*(i+1)*(i+2) / 63);
		outbuf[i].im = (float)sin(0.0 - PI*u*(i+1)*(i+2) / 63);
	}
	
	return 62;
}

/************************************************************************/
/*
$funcname:	int uerx_physig::InputOutput(int N_ID_cell, cfloat* outbuf)

  $funcabs :	generator the second synchronization signal.
  
	$parameters: 	 
	{
	$paraname0:  N_ID_cell ;  $paradir: in ;  $paratype: int16 ;    $pararange: [0,503];       
	$paraabs: cell ID;
	$paraname1:  outbuf	   ;  $paradir: in ;  $paratype: cfloat * ;	$pararange:		   ;	
	$paraabs: output buff. including the second synchronization signal.
	}
	
	  $return: return the length of the outbuf. length = 124.
	  
		$creator: HuangShoujun  ;     $creattime: 06-24-2009;
		$mendor1:				;     $mendtime1:           ;    
		$mendor2:				;     $mendtime2:           ;
		$ref    : LTE 36.211,v8.6.0 p73
		$notes  : the outbuf including subfram0 and subframe5 second symchronization signal.
		outbuf[0]~outbuf[61] is subframe0 second symchronization signal.
		outbuf[62]~outbuf[123] is subframe5 second symchronization signal.
		
*/
/************************************************************************/
int physig_InputOutput2(int N_ID_cell, int16 *outbuf)          // sss
{
	int N_ID_1;
	int N_ID_2;
	int qq;
	int q;
	int mm;
	int m0;
	int m1;
	int x[31];
	int i;
	int ss[31];
	int s0[31];
	int s1[31];
	int cc[31];
	int c0[31];
	int c1[31];
	int zz[31];
	int zm0[31];
	int zm1[31];
	
	N_ID_1 = (int)(N_ID_cell / 3);
	N_ID_2 = N_ID_cell%3;
	qq     = (int)(N_ID_1 / 30);
	q      = (int)( ( N_ID_1 + qq*(qq+1) / 2 ) / 30 );
	mm     = N_ID_1 + q * (q + 1) / 2;
	m0     = mm % 31;
	m1     = ( m0 + (int)(mm / 31) + 1)%31;
	
	memset(x,0,sizeof(int)*31);                                      //s0[31], s1[31]
	x[4]   = 1;
	
	for(i = 0; i<26; i++)
	{
		x[i+5] = x[i+2] ^ x[i];
	}
	
	for(i = 0; i<31; i++)
	{
		ss[i] = 1-2*x[i];
	}
	
	memset(s0,0,sizeof(int)*31);
	
	for(i = 0; i<31; i++)
	{
		s0[i] = ss[(i+m0)%31];
		s1[i] = ss[(i+m1)%31];
	}
	
	memset(x,0,sizeof(int)*31);                                      //c0[31],c1[31]
	x[4]=1;
	
	for(i = 0; i < 26; i++)
	{
		x[i+5] = x[i+3] ^ x[i];
	}
	
	for(i = 0; i < 31; i++)
	{
		cc[i] = 1-2*x[i];
	}
	
	for(i = 0; i < 31; i++)
	{
		c0[i] = cc[(i+N_ID_2)%31];
		c1[i] = cc[(i+N_ID_2+3)%31];
	}
	
	memset(x, 0, sizeof(int) * 31);                                  //zm0[31],zm1[31]
	x[4]=1;
	
	for (i = 0; i < 26; i++)
		x[i+5] = x[i+4] ^ x[i+2] ^ x[i+1] ^ x[i];
	
	for(i = 0; i < 31; i++)
		zz[i] = 1-2*x[i];
	
	for (i = 0; i < 31; i++)
	{
		zm0[i] = zz[(i + (m0%8))%31];
		zm1[i] = zz[(i + (m1%8))%31];
	}
	
	for(i = 0; i < 31; i++)                                          //d[62]	
	{
		outbuf[2*i] = (s0[i]*c0[i]);
		outbuf[2*i+1] = (s1[i]*c1[i]*zm0[i]);
	}
	
	for(i = 31; i < 62; i++)                                         //d[62]
	{
		outbuf[2*i] = (s1[i-31]*c0[i-31]);
		outbuf[2*i+1] = (s0[i-31]*c1[i-31]*zm1[i-31]);
	}
	
	return 124;
}


/************************************************************************/
/*
$funcname: int uerx_physig::InputOutput(int N_sourceblk, int N_ID_cell, int CP_config, int subfrm_idx, cfloat *ref_sym);

  $funcabs : generate reference signal of each subframe;
  
	$parameters: 
	{ 
	
      $paraname: N_sourceblk; $paradir: in;  $paratype: int    ; $pararange:[0~110]; $paraabs: RB number of system;
	  $paraname: N_ID_cell  ; $paradir: in;  $paratype: int    ; $pararange:[0~503]; $paraabs: cell id index;
	  $paraname: CP_config  ; $paradir: in;  $paratype: int    ; $pararange:[0,1]  ; $paraabs: 0, normal config, 1, extended config;
	  $paraname: subfrm_idx ; $paradir: in;  $paratype: int    ; $pararange:[0~9]  ; $paraabs: subframe index in each radio frame;
      $paraname: ref_sym;     $paradir: out; $paratype: cfloat*; $pararange:       ; $paraabs: buffer of reference signal symbols of each subframe; 
	  }
	  
		$creator: Feng Xuelin;				    $creattime: 06-18-2009;
		$mendor1:		     ;                  $mendtime1:           ;
		$mendor2:		     ;                  $mendtime2:           ;
		$ref    : LTE 36.211,v8.6.0 and simulink model at \cvsroot\ltetdd_tx\ltetdd_dw\top_phase_1;
$notes  :            ;															*/
/************************************************************************/

int physig_InputOutput3(int N_sourceblk, int N_ID_cell, int CP_config, int subfrm_idx, cfloat *ref_sym)
{
	int i,j, k, N_cp, cinit, flag_PRS, ns, l_num;
    int rs_set[3];
	
	// length of Pseudo-random sequence needed for cell-specific reference signal(RS)
    int PRS_len = UERX_MAX_NUM_RB_DL*4;
	
	// Pseudo-random sequence needed for RS
	int PRS[UERX_MAX_NUM_RB_DL*4];
	
	// power ratio of the ref 
    // int16 pow_norm=pow(10, float(belta_ref/10));
	
	//Pseudo-random sequence cinit initialization
	// normal cp
	if (CP_config==0)  
	{
		N_cp = 1;
	}
	else 
	{
		N_cp = 0;
	}
    
	rs_set[0]=0;
	rs_set[1]=1;
	rs_set[2]=CP_config?  3: 4;
	
	for (i=0; i< 2; i++ )
	{
		ns = 2* subfrm_idx + i;
		
		for (j=0; j<3; j++)
		{
            l_num = rs_set[j];

			cinit = (1<<10)*(7*(ns + 1)+l_num+1)*(2 * N_ID_cell + 1) + 2*N_ID_cell + N_cp;
			
			flag_PRS = /*uerx_sharefunc_u.*/uerx_pseudo_rand(cinit, PRS, PRS_len);
			
			for (k = 0; k < UERX_MAX_NUM_RB_DL * 2; k++)                    // RS generation
			{
				ref_sym[( i * 3 + j ) * 220 + k].re = (float)((1 - 2 * PRS[2 * k]) / sqrt(2));
				ref_sym[( i * 3 + j ) * 220 + k].im = (float)((1 - 2 * PRS[2 * k + 1]) / sqrt(2));
			}
		}		
	}	
 
	return UERX_MAX_NUM_RB_DL * 2 * 3 * 2;
}

int GetRsPbch(int16 antenna_num, int N_ID_cell, int CP_config
							, float belta_ref, cint16 rs_pbch[])
{
	int i, k, N_cp, cinit, flag_PRS, ns, l_num_idx, l_num, outlen, N_Symb;
	int l_num_arr[3];
	
	// length of Pseudo-random sequence needed for cell-specific reference signal(RS)
    int PRS_len = UERX_MAX_NUM_RB_DL*4;	
	// Pseudo-random sequence needed for RS
	int PRS[UERX_MAX_NUM_RB_DL*4];
	
	if (CP_config==0)  
	{
		N_Symb = 7;
		N_cp   = 1;
	}
	else 
	{
		N_Symb = 6;		
		N_cp   = 0;
	}
	
	l_num_arr[0] = 0;  l_num_arr[1] = 1;  l_num_arr[2] = N_Symb - 3;
	
	//float belta_ref = 2.5;
	float pow_norm = powf(10, belta_ref / 20);
	pow_norm  =  (float)(sqrt(2));
	
	ns = 1;                                                          // pbch ns=1
	
	for (l_num_idx = 0; l_num_idx < 3; l_num_idx ++)
	{
		l_num	=	l_num_arr[l_num_idx];
		cinit	= (1<<10)*(7*(ns+1)+l_num+1)*(2*N_ID_cell+1) + 2*N_ID_cell + N_cp;
		
		flag_PRS = /*uerx_sharefunc_u.*/uerx_pseudo_rand(cinit, PRS, PRS_len);
		
		for (i = 0 ; i < 12 ; i++)
		{
			k = UERX_MAX_NUM_RB_DL - 6 + i;
			
			rs_pbch[l_num_idx * 12 + i ].re = 1 - 2 * PRS[2 * k];
			rs_pbch[l_num_idx * 12 + i ].im = 1 - 2 * PRS[2 * k + 1];
		}			
	} 
	
	outlen = 36;
	
	return outlen;
}

int GetRefPP_1(int N_ID_cell, int N_TX_Ant, int N_fft, int CP_config
							 , int n_subfrm, int n_ofdmsym, cint16 *refpp[4], int16 *ref_pos)
{
	int N_sc_RB = 12;
	int N_RB_DL = 50;
	int k, i, m, kk, N_cp;
	int N_symb = (CP_config == 0)? 7: 6;
	int16 v_p[4][2]	= {{0,  3 }, {3, 0}, {0, 3},{3, 6}};  	
	int16 v_shift	= N_ID_cell % 6;
	
	// length of Pseudo-random sequence needed for cell-specific reference signal(RS)
    int PRS_len = UERX_MAX_NUM_RB_DL*4;
	
	// Pseudo-random sequence needed for RS
	int PRS[UERX_MAX_NUM_RB_DL*4];
	
	if (CP_config==0)  
	{
		N_cp = 1;
	}
	else 
	{
		N_cp = 0;
	}
    
	int ns ;  
	
	int l_num, cinit;
	
	int rs_set[3];
	
	rs_set[0] = 0;
	rs_set[1] = 1;
	rs_set[2] = N_symb - 4;
	
	ns = 2 * n_subfrm + n_ofdmsym / N_symb;
	
	l_num = n_ofdmsym % N_symb;
	
	cinit = ( 1<<10 ) * ( 7 * ( ns + 1 ) + l_num + 1 ) * ( 2 * N_ID_cell + 1 ) + 2 * N_ID_cell + N_cp;
				
	/*uerx_sharefunc_u.*/uerx_pseudo_rand(cinit, PRS, PRS_len);
	
	switch (N_TX_Ant)
	{
	case 1:
		if (l_num != rs_set[0]  || l_num != rs_set[2])
		{
			return 0;   
		}
		
		for (k = 0; k < N_RB_DL * 2; k++)                            // RS generation
		{
			m = ( k < N_RB_DL )? (UERX_MAX_NUM_RB_DL + k) : (UERX_MAX_NUM_RB_DL - N_RB_DL + k - N_RB_DL );
			
			refpp[ 0 ][ k ].re = 1 - 2 * PRS[2 * m];
			refpp[ 0 ][ k ].im = 1 - 2 * PRS[2 * m + 1];
			
			kk = (l_num == 0)? (6 * k + (v_p[0][0] + v_shift) % 6):(6 * k + (v_p[0][1] + v_shift) % 6);
			
			i = (kk < N_RB_DL*N_sc_RB/2) ? (kk+N_fft- N_RB_DL*N_sc_RB/2) : (kk-N_RB_DL*N_sc_RB/2+1); 
			
			ref_pos[k] = i;
		}
		break;
	case 2:

		break;
	case 3:
		
		break;
	case 4:
		break;
	}
	
	return 1;	                                                     // 1:ref is there / 0: no ref	
}

int GetRefPP(int N_ID_cell, int N_TX_Ant, int N_fft, int N_RB_DL, int CP_config, int n_subfrm
						   , int n_ofdmsym, cfloat *refpp[4], int16 *ref_initdelta, int16* data_startpos)
{
	int N_sc_RB = 12;
 	int k, m, N_cp;	
	float gain = 1;	
	int N_symb = (CP_config == 0)? 7: 6;	
	int16 v_p[4][2]	= {{0,  3 }, {3, 0}, {0, 3},{3, 6}};  
	int16 v_shift	= N_ID_cell % 6;
	
	// length of Pseudo-random sequence needed for cell-specific reference signal(RS)
    int PRS_len = UERX_MAX_NUM_RB_DL*4;	
	// Pseudo-random sequence needed for RS
	int PRS[UERX_MAX_NUM_RB_DL*4];
	
	if (CP_config==0)  
	{
		N_cp = 1;
	}
	else 
	{
		N_cp = 0;
	}
    
	int ns;  	
	int l_num, cinit;	
	int rs_set[3];
	
	rs_set[0] = 0;
	rs_set[1] = 1;
	rs_set[2] = N_symb - 3;
	
	ns    = 2 * n_subfrm + n_ofdmsym / N_symb;	
	l_num = n_ofdmsym % N_symb;	
	cinit = ( 1<<10 ) * ( 7 * ( ns + 1 ) + l_num + 1 ) * ( 2 * N_ID_cell + 1 ) + 2 * N_ID_cell + N_cp;
				
	/*uerx_sharefunc_u.*/uerx_pseudo_rand(cinit, PRS, PRS_len);
	
	switch (N_TX_Ant)
	{
	case 1:
		if (l_num != rs_set[0] && l_num != rs_set[2])
		{
			return 0;   
		}
		
		for (k = 0; k < N_RB_DL * 2; k++)                            // RS generation
		{
			m = k + UERX_MAX_NUM_RB_DL - N_RB_DL;
			
			refpp[ 0 ][ k ].re = (float)(1 - 2 * PRS[2 * m]);
			refpp[ 0 ][ k ].im = (float)(1 - 2 * PRS[2 * m + 1]);
			
			cfloatmultiplyequalfloat(&refpp[ 0 ][ k ] , gain);
			
			ref_initdelta[0] = (l_num == 0)? ( (v_p[0][0] + v_shift) % 6) : ((v_p[0][1] + v_shift) % 6);
		}
		break;
	case 2:
		if (l_num != rs_set[0] && l_num != rs_set[2])
		{
			return 0;   
		}
		
		for (k = 0; k < N_RB_DL * 2; k++)
		{
			m = k + UERX_MAX_NUM_RB_DL - N_RB_DL;
			
			refpp[ 0 ][ k ].re = (float)((1 - 2 * PRS[2 * m]));
			refpp[ 0 ][ k ].im = (float)((1 - 2 * PRS[2 * m + 1]));
			
			refpp[ 1 ][ k ].re = (float)((1 - 2 * PRS[2 * m]));
			refpp[ 1 ][ k ].im = (float)((1 - 2 * PRS[2 * m + 1]));
			
			ref_initdelta[0] = (l_num == 0)? ( (v_p[0][0] + v_shift) % 6):( (v_p[0][1] + v_shift) % 6);
			ref_initdelta[1] = (l_num == 0)? ( (v_p[1][0] + v_shift) % 6):( (v_p[1][1] + v_shift) % 6);			
		}
		break;
		
	case 4:
		if (l_num != rs_set[0]  && l_num != rs_set[1] && l_num != rs_set[2])
		{
			return 0;   
		}
		
		if (l_num == rs_set[0] || l_num == rs_set[2])                //rs_set = 0 1 4(or 3)
		{
			memset(refpp[ 2 ], 0, sizeof(cint16) * N_RB_DL * 2);
			memset(refpp[ 3 ], 0, sizeof(cint16) * N_RB_DL * 2);
			for (k = 0; k < N_RB_DL * 2; k++)
			{
				m = k + UERX_MAX_NUM_RB_DL - N_RB_DL;
				
				refpp[ 0 ][ k ].re = (float)(1 - 2 * PRS[2 * m]);
				refpp[ 0 ][ k ].im = (float)(1 - 2 * PRS[2 * m + 1]);
				
				refpp[ 1 ][ k ].re = (float)(1 - 2 * PRS[2 * m]);
				refpp[ 1 ][ k ].im = (float)(1 - 2 * PRS[2 * m + 1]);
				
				cfloatmultiplyequalfloat(&refpp[ 0 ][ k ] , gain);
				cfloatmultiplyequalfloat(&refpp[ 1 ][ k ] , gain);
				
				ref_initdelta[0] = (l_num == 0) ? ((v_p[0][0]+v_shift)%6) : ((v_p[0][1]+v_shift)%6);
				ref_initdelta[1] = (l_num == 0) ? ((v_p[1][0]+v_shift)%6) : ((v_p[1][1]+v_shift)%6);
				ref_initdelta[2] = 0;
				ref_initdelta[3] = 0;				
			}
		}
		else if (l_num == rs_set[1])
		{
			memset(refpp[ 0 ], 0, sizeof(cint16) * N_RB_DL * 2);
			memset(refpp[ 1 ], 0, sizeof(cint16) * N_RB_DL * 2);
			for (k = 0; k < N_RB_DL * 2; k++)
			{
				m = k + UERX_MAX_NUM_RB_DL - N_RB_DL;
				
				refpp[ 2 ][ k ].re = (float)( 1 - 2 * PRS[2 * m]);
				refpp[ 2 ][ k ].im = (float)(1 - 2 * PRS[2 * m + 1]);
				
				refpp[ 3 ][ k ].re = (float)(1 - 2 * PRS[2 * m]);
				refpp[ 3 ][ k ].im = (float)(1 - 2 * PRS[2 * m + 1]);
				
				cfloatmultiplyequalfloat(&refpp[ 2 ][ k ] , gain);
				cfloatmultiplyequalfloat(&refpp[ 3 ][ k ] , gain);
				
				ref_initdelta[0] = 0;
				ref_initdelta[1] = 0;
				ref_initdelta[2] = (n_ofdmsym == 1) ? ((v_p[2][0]+v_shift)%6) : ((v_p[2][1]+v_shift)%6);
				ref_initdelta[3] = (n_ofdmsym == 1) ? ((v_p[3][0]+v_shift)%6) : ((v_p[3][1]+v_shift)%6);			
			}
		}
		
		break;
	}
	
	*data_startpos = N_fft - N_RB_DL * N_sc_RB / 2 ;
	
	return 1;
}
