#include "foo.h"
#include "uerx_define.h"
#include "uerx_fixed_cfg.h"
#include "uerx_equ.h"


int SISO_decod(cfloat *Y_inbuf, cfloat *H_inbuf, int inlen, float nosieVar
						  , cfloat *X_outbuf, cfloat *csi_outbuf)
{
	int i;	
#if LTEDL_FIXD_FLG == 0 || (LTEDL_FIXD_FLG==8  && LTEDL_EQU_SISO_DECOD == 0)	
	cfloat H_sc, Hc_sc;
	cfloat Y_sc;
	float csi_sc;
	cfloat X_sc;	
#elif LTEDL_FIXD_FLG == 3 || (LTEDL_FIXD_FLG==8  && LTEDL_EQU_SISO_DECOD == 3)
	sc_cfixed<16, 11, SC_RND, SC_SAT> Y_sc, H_sc, Hc_sc;
	sc_cfixed<16, 21, SC_RND, SC_SAT> X_sc;
	sc_fixed<16, 21, SC_RND, SC_SAT> csi_sc;
#endif

	for (i=0; i<inlen; i++)
	{	
		H_sc.re = H_inbuf[i].re;
		H_sc.im = H_inbuf[i].im;
		Y_sc.re = Y_inbuf[i].re;
		Y_sc.im = Y_inbuf[i].im;

		// Hc_sc = conj(H)
		Hc_sc.re =  H_sc.re;
		Hc_sc.im = -H_sc.im;

		// X = Y_sc * Hc_sc
		X_sc.re = Y_sc.re*Hc_sc.re - Y_sc.im*Hc_sc.im;
		X_sc.im = Y_sc.re*Hc_sc.im + Y_sc.im*Hc_sc.re;	

		// csi
		csi_sc = H_sc.re*H_sc.re+H_sc.im*H_sc.im;

		X_outbuf[i].re   = X_sc.re;
		X_outbuf[i].im   = X_sc.im;
		csi_outbuf[i].re = csi_sc;	
		csi_outbuf[i].im = 0;
	}

	return inlen;
}

int  MRC_decod(cfloat **Y_inbuf, cfloat **H_inbuf, int inlen, float nosieVar, cfloat *X_outbuf
						  , cfloat *csi_outbuf)
{
	int i;
	
#if LTEDL_FIXD_FLG == 0 || (LTEDL_FIXD_FLG==8  && LTEDL_EQU_MRC_DECOD == 0)	
	cfloat H0_sc, H1_sc, Hc0_sc, Hc1_sc;
    cfloat Y0_sc, Y1_sc;
    cfloat Hc0_Y0_sc, Hc1_Y1_sc;
    cfloat X_sc;
    float csi_sc;
    float H0_2_sc, H1_2_sc;
#elif LTEDL_FIXD_FLG == 3 || (LTEDL_FIXD_FLG==8  && LTEDL_EQU_MRC_DECOD == 3)
	sc_cfixed<16, 11, SC_RND, SC_SAT> Y0_sc, Y1_sc, H0_sc, H1_sc, Hc0_sc, Hc1_sc;
    sc_cfixed<16, 21, SC_RND, SC_SAT> Hc0_Y0_sc, Hc1_Y1_sc;
	sc_cfixed<16, 21, SC_RND, SC_SAT> X_sc;
    sc_fixed<16, 21, SC_RND, SC_SAT> H0_2_sc, H1_2_sc, csi_sc;
#endif
	
	for (i = 0; i < inlen; i++)
	{	// get H	
		H0_sc.re = H_inbuf[0][i].re;
		H0_sc.im = H_inbuf[0][i].im;		
		H1_sc.re = H_inbuf[1][i].re;
		H1_sc.im = H_inbuf[1][i].im;		
		
		// get Y
		Y0_sc.re = Y_inbuf[0][i].re;
		Y0_sc.im = Y_inbuf[0][i].im;		
		Y1_sc.re = Y_inbuf[1][i].re;
		Y1_sc.im = Y_inbuf[1][i].im;		
				
		// X
		Hc0_sc.re = H0_sc.re;
		Hc0_sc.im = -H0_sc.im;		
		Hc1_sc.re = H1_sc.re;
		Hc1_sc.im = -H1_sc.im;
		
		Hc0_Y0_sc.re = (Hc0_sc.re*Y0_sc.re - Hc0_sc.im*Y0_sc.im);
		Hc0_Y0_sc.im = (Hc0_sc.im*Y0_sc.re + Hc0_sc.re*Y0_sc.im);
		
		Hc1_Y1_sc.re = (Hc1_sc.re*Y1_sc.re - Hc1_sc.im*Y1_sc.im);
		Hc1_Y1_sc.im = (Hc1_sc.im*Y1_sc.re + Hc1_sc.re*Y1_sc.im);	
		
		X_sc.re = Hc0_Y0_sc.re + Hc1_Y1_sc.re;
		X_sc.im = Hc0_Y0_sc.im + Hc1_Y1_sc.im;
		
		// |H|^2
		H0_2_sc = H0_sc.re*H0_sc.re + H0_sc.im*H0_sc.im;
		H1_2_sc = H1_sc.re*H1_sc.re + H1_sc.im*H1_sc.im;
		csi_sc = H0_2_sc + H1_2_sc;
		
		X_outbuf[i].re = X_sc.re;
		X_outbuf[i].im = X_sc.im;
		csi_outbuf[i].re = csi_sc;		
		csi_outbuf[i].im = 0;		
	}
	
	return inlen;
}

int SFBC_decod(cfloat **Y_inbuf, cfloat **H_inbuf, int inlen, int num_bstx_ant, int num_uerx_ant
						 , float nosieVar, cfloat *X_outbuf, cfloat *csi_outbuf)
{
	int	i;

#if LTEDL_FIXD_FLG == 0 || (LTEDL_FIXD_FLG==8  && LTEDL_EQU_SFBC_DECOD == 0)	
	float H0_2_sc, H1_2_sc, H2_2_sc, H3_2_sc;
	float csi_sc;
	cfloat H0_sc, H1_sc, H2_sc, H3_sc;
	cfloat Y0_sc, Y1_sc, Y2_sc, Y3_sc;
	cfloat Hc0_sc, Yc0_sc, Yc1_sc, Hc2_sc, Yc2_sc, Yc3_sc;
	cfloat Hc0_Y0_sc, H1_Yc1_sc, Hc2_Y2_sc, H3_Yc3_sc;
	cfloat Hc0_Y1_sc, H1_Yc0_sc, Hc2_Y3_sc, H3_Yc2_sc;
	cfloat X0_sc, X1_sc;	
#elif LTEDL_FIXD_FLG == 3 || (LTEDL_FIXD_FLG == 8  && LTEDL_EQU_SFBC_DECOD == 3)
	sc_fixed<16, 21, SC_RND, SC_SAT> H0_2_sc, H1_2_sc, H2_2_sc, H3_2_sc, csi_sc;
	sc_cfixed<16, 11, SC_RND, SC_SAT> Y0_sc, Y1_sc, Y2_sc, Y3_sc, H0_sc, H1_sc, H2_sc, H3_sc
		, Hc0_sc, Yc0_sc, Yc1_sc, Hc2_sc, Yc2_sc, Yc3_sc;
	sc_cfixed<16, 21, SC_RND, SC_SAT> Hc0_Y0_sc, H1_Yc1_sc, Hc2_Y2_sc, H3_Yc3_sc
		, Hc0_Y1_sc, H1_Yc0_sc, Hc2_Y3_sc, H3_Yc2_sc;	
	sc_cfixed<16, 21, SC_RND, SC_SAT> X0_sc, X1_sc;	
#endif

	if (num_bstx_ant==2 && num_uerx_ant==2)
	{
		for ( i=0; i<inlen/2; i++)
		{		
			// get H
			H0_sc.re = (H_inbuf[0][2*i+0].re + H_inbuf[0][2*i+1].re) / 2.0; 
			H0_sc.im = (H_inbuf[0][2*i+0].im + H_inbuf[0][2*i+1].im) / 2.0; 
			
			H1_sc.re = (H_inbuf[1][2*i+0].re + H_inbuf[1][2*i+1].re) / 2.0; 
			H1_sc.im = (H_inbuf[1][2*i+0].im + H_inbuf[1][2*i+1].im) / 2.0;		
			
			H2_sc.re = (H_inbuf[2][2*i+0].re + H_inbuf[2][2*i+1].re) / 2.0; 
			H2_sc.im = (H_inbuf[2][2*i+0].im + H_inbuf[2][2*i+1].im) / 2.0; 
			
			H3_sc.re = (H_inbuf[3][2*i+0].re + H_inbuf[3][2*i+1].re) / 2.0; 
			H3_sc.im = (H_inbuf[3][2*i+0].im + H_inbuf[3][2*i+1].im) / 2.0;	

			// csi
			H0_2_sc = H0_sc.re*H0_sc.re + H0_sc.im*H0_sc.im;
			H1_2_sc = H1_sc.re*H1_sc.re + H1_sc.im*H1_sc.im;
			H2_2_sc = H2_sc.re*H2_sc.re + H2_sc.im*H2_sc.im;
			H3_2_sc = H3_sc.re*H3_sc.re + H3_sc.im*H3_sc.im;
			
			csi_sc = 0.5 * (H0_2_sc+H1_2_sc+H2_2_sc+H3_2_sc);
			
			// get Y
			Y0_sc.re = Y_inbuf[0][2*i+0].re;  
			Y0_sc.im = Y_inbuf[0][2*i+0].im;
			
			Y1_sc.re = Y_inbuf[0][2*i+1].re;
			Y1_sc.im = Y_inbuf[0][2*i+1].im;
			
			Y2_sc.re = Y_inbuf[1][2*i+0].re;  
			Y2_sc.im = Y_inbuf[1][2*i+0].im;
			
			Y3_sc.re = Y_inbuf[1][2*i+1].re;
			Y3_sc.im = Y_inbuf[1][2*i+1].im;
			
			// X0, X1
			Hc0_sc.re =  H0_sc.re;
			Hc0_sc.im = -H0_sc.im;
			
			Hc2_sc.re =  H2_sc.re;
			Hc2_sc.im = -H2_sc.im;
			
			Yc0_sc.re =  Y0_sc.re;
			Yc0_sc.im = -Y0_sc.im;
			
			Yc1_sc.re =  Y1_sc.re;
			Yc1_sc.im = -Y1_sc.im;
			
			Yc2_sc.re =  Y2_sc.re;
			Yc2_sc.im = -Y2_sc.im;
			
			Yc3_sc.re =  Y3_sc.re;
			Yc3_sc.im = -Y3_sc.im;
			
			Hc0_Y0_sc.re = (Hc0_sc.re*Y0_sc.re - Hc0_sc.im*Y0_sc.im);					// X0
			Hc0_Y0_sc.im = (Hc0_sc.im*Y0_sc.re + Hc0_sc.re*Y0_sc.im);
			H1_Yc1_sc.re = (H1_sc.re*Yc1_sc.re - H1_sc.im*Yc1_sc.im);
			H1_Yc1_sc.im = (H1_sc.im*Yc1_sc.re + H1_sc.re*Yc1_sc.im);
			Hc2_Y2_sc.re = (Hc2_sc.re*Y2_sc.re - Hc2_sc.im*Y2_sc.im);
			Hc2_Y2_sc.im = (Hc2_sc.im*Y2_sc.re + Hc2_sc.re*Y2_sc.im);
			H3_Yc3_sc.re = (H3_sc.re*Yc3_sc.re - H3_sc.im*Yc3_sc.im);
			H3_Yc3_sc.im = (H3_sc.im*Yc3_sc.re + H3_sc.re*Yc3_sc.im);
			
			Hc0_Y1_sc.re = (Hc0_sc.re*Y1_sc.re - Hc0_sc.im*Y1_sc.im);					// X1
			Hc0_Y1_sc.im = (Hc0_sc.im*Y1_sc.re + Hc0_sc.re*Y1_sc.im);
			H1_Yc0_sc.re = (H1_sc.re*Yc0_sc.re - H1_sc.im*Yc0_sc.im);
			H1_Yc0_sc.im = (H1_sc.im*Yc0_sc.re + H1_sc.re*Yc0_sc.im);			
			Hc2_Y3_sc.re = (Hc2_sc.re*Y3_sc.re - Hc2_sc.im*Y3_sc.im);
			Hc2_Y3_sc.im = (Hc2_sc.im*Y3_sc.re + Hc2_sc.re*Y3_sc.im);
			H3_Yc2_sc.re = (H3_sc.re*Yc2_sc.re - H3_sc.im*Yc2_sc.im);
			H3_Yc2_sc.im = (H3_sc.im*Yc2_sc.re + H3_sc.re*Yc2_sc.im);
			
			X0_sc.re = 0.7071 * (Hc0_Y0_sc.re+H1_Yc1_sc.re+Hc2_Y2_sc.re+H3_Yc3_sc.re);	// X0
			X0_sc.im = 0.7071 * (Hc0_Y0_sc.im+H1_Yc1_sc.im+Hc2_Y2_sc.im+H3_Yc3_sc.im);
			
			X1_sc.re = 0.7071 * (Hc0_Y1_sc.re-H1_Yc0_sc.re+Hc2_Y3_sc.re-H3_Yc2_sc.re);	// X1
			X1_sc.im = 0.7071 * (Hc0_Y1_sc.im-H1_Yc0_sc.im+Hc2_Y3_sc.im-H3_Yc2_sc.im);			

			// output the X and csi
			X_outbuf[2*i+0].re = X0_sc.re;
			X_outbuf[2*i+0].im = X0_sc.im;

			X_outbuf[2*i+1].re = X1_sc.re;
			X_outbuf[2*i+1].im = X1_sc.im;

			csi_outbuf[2*i+0].re = csi_sc;
			csi_outbuf[2*i+0].im = 0;
			
			csi_outbuf[2*i+1].re = csi_sc;
			csi_outbuf[2*i+1].im = 0;
		}
	}
		
	return inlen;
}


int CLSM_decod(cfloat **Y_inbuf, cfloat **H_inbuf, int inlen, int num_col0_Nt,int num_row0_Nr, int layers
				, int cod_idx, float noiseVar, cfloat **X_outbuf, cfloat **csi_outbuf, cfloat **ns_outbuf)
{
	int k;	
#if LTEDL_FIXD_FLG == 0 || (LTEDL_FIXD_FLG==8  && LTEDL_EQU_CLSM_DECOD == 0)
	float W2_sc;
	float a1_sc, a2_sc;
	float noiseVar_sc, ns0_sc, ns1_sc;
	float a11_sc, a12_sc, a21_sc, a22_sc;
	float csi0_sc, csi1_sc, csi_sc;	
	cfloat h11_sc, h12_sc, h21_sc, h22_sc, H_sc[16];
	cfloat hc11_sc, hc12_sc, hc21_sc, hc22_sc;
	cfloat predmx_sc[16];
	cfloat y1_sc, y2_sc;	
	cfloat W_sc, W_cnj_sc;
	cfloat z0_sc, z1_sc, z2_sc, z3_sc;
	cfloat u0_sc, u1_sc, u2_sc, u3_sc;
	cfloat h11y2_sc, h21y1_sc, h22y1_sc, h12y2_sc;
	cfloat hc12y1_sc, hc22y2_sc, hc11y1_sc, hc21y2_sc;
	cfloat X0_sc, X1_sc, X_sc;
	cfloat h11h22_sc, h12h21_sc;
#elif LTEDL_FIXD_FLG == 3 || (LTEDL_FIXD_FLG==8  && LTEDL_EQU_CLSM_DECOD == 3)
	sc_cfixed<16, 11, SC_RND, SC_SAT> h11_sc, h12_sc, h21_sc, h22_sc
		, hc11_sc, hc12_sc, hc21_sc, hc22_sc, H_sc[16]
		, y1_sc, y2_sc;
	sc_cfixed<16, 1, SC_RND, SC_SAT>  predmx_sc[16];
	sc_cfixed<16, 21, SC_RND, SC_SAT> h11h22_sc, h12h21_sc
		, h11y2_sc, h21y1_sc, h22y1_sc, h12y2_sc, hc12y1_sc, hc22y2_sc, hc11y1_sc, hc21y2_sc
		, u0_sc, u1_sc, u2_sc, u3_sc
		, W_sc, W_cnj_sc;
	sc_cfixed<16, 37, SC_RND, SC_SAT> z0_sc, z1_sc, z2_sc, z3_sc, X0_sc, X1_sc;
	sc_cfixed<16, 21, SC_RND, SC_SAT> X_sc;
	sc_fixed<16, 16, SC_RND, SC_SAT> noiseVar_sc;
	sc_fixed<16, 21, SC_RND, SC_SAT> a11_sc, a12_sc, a21_sc, a22_sc, a1_sc, a2_sc;	
	sc_fixed<16, 37, SC_RND, SC_SAT> W2_sc, csi0_sc, csi1_sc;
	sc_fixed<16, 21, SC_RND, SC_SAT> csi_sc;
	sc_fixed<16, 29, SC_RND, SC_SAT> ns0_sc, ns1_sc;
#endif

	noiseVar_sc = noiseVar;

	// get precoding matrix
	if (layers==1)
	{
		if (cod_idx==0)
		{
			predmx_sc[0].re = 0.707;
			predmx_sc[0].im = 0;
			predmx_sc[1].re = 0.707;	
			predmx_sc[1].im = 0;
		}
		else if (cod_idx==1)
		{
			predmx_sc[0].re = 0.707;
			predmx_sc[0].im = 0;
			predmx_sc[1].re = -0.707;
			predmx_sc[1].im = 0;
		}
		else if (cod_idx==2)
		{
			predmx_sc[0].re = 0.707;
			predmx_sc[0].im = 0;
			predmx_sc[1].re = 0;
			predmx_sc[1].im = 0.707;
		}
		else if (cod_idx==3)
		{
			predmx_sc[0].re = 0.707;
			predmx_sc[0].im = 0;
			predmx_sc[1].re = 0;
			predmx_sc[1].im = -0.707;
		}
		else
		{
			assert(0);
		}
	}
	else if (layers==2)
	{
		if (cod_idx==0)
		{
			predmx_sc[0].re = 0.707;
			predmx_sc[0].im = 0;
			predmx_sc[1].re = 0;
			predmx_sc[1].im = 0;
			predmx_sc[2].re = 0;
			predmx_sc[2].im = 0;
			predmx_sc[3].re = 0.707;
			predmx_sc[3].im = 0;
		}
		else if (cod_idx==1)
		{
			predmx_sc[0].re = 0.5;
			predmx_sc[0].im = 0;
			predmx_sc[1].re = 0.5;
			predmx_sc[1].im = 0;
			predmx_sc[2].re = 0.5;
			predmx_sc[2].im = 0;
			predmx_sc[3].re = -0.5;
			predmx_sc[3].im = 0;
		}
		else if (cod_idx==2)
		{
			predmx_sc[0].re = 0.5;
			predmx_sc[0].im = 0;
			predmx_sc[1].re = 0.5;
			predmx_sc[1].im = 0;
			predmx_sc[2].re = 0;
			predmx_sc[2].im = 0.5;
			predmx_sc[3].re = 0;
			predmx_sc[3].im = -0.5;
		}
		else
		{
			assert(0);
		}
	}

	// sm decoder
	if(layers == 1)
	{
		for (k = 0; k < inlen; k++)
		{	// get H
			H_sc[0].re = H_inbuf[0][k].re;
			H_sc[0].im = H_inbuf[0][k].im;
			H_sc[1].re = H_inbuf[1][k].re;
			H_sc[1].im = H_inbuf[1][k].im;
			H_sc[2].re = H_inbuf[2][k].re;
			H_sc[2].im = H_inbuf[2][k].im;
			H_sc[3].re = H_inbuf[3][k].re;
			H_sc[3].im = H_inbuf[3][k].im;			

			// get equal H
			// h11_sc = H_sc[0] * predmx_sc[0]
			h11_sc.re  = H_sc[0].re*predmx_sc[0].re - H_sc[0].im*predmx_sc[0].im;
			h11_sc.im  = H_sc[0].re*predmx_sc[0].im + H_sc[0].im*predmx_sc[0].re;
			// h11_sc += H_sc[1] * predmx_sc[1]
			h11_sc.re += H_sc[1].re*predmx_sc[1].re - H_sc[1].im*predmx_sc[1].im;
			h11_sc.im += H_sc[1].re*predmx_sc[1].im + H_sc[1].im*predmx_sc[1].re;
			// h21_sc = H_sc[2] * predmx_sc[0]
			h21_sc.re  = H_sc[2].re*predmx_sc[0].re - H_sc[2].im*predmx_sc[0].im;
			h21_sc.im  = H_sc[2].re*predmx_sc[0].im + H_sc[2].im*predmx_sc[0].re;
			// h21_sc += H_sc[3] * predmx_sc[1]
			h21_sc.re += H_sc[3].re*predmx_sc[1].re - H_sc[3].im*predmx_sc[1].im;
			h21_sc.im += H_sc[3].re*predmx_sc[1].im + H_sc[3].im*predmx_sc[1].re;

			// get Y
			y1_sc.re = Y_inbuf[0][k].re;
			y1_sc.im = Y_inbuf[0][k].im;
			y2_sc.re = Y_inbuf[1][k].re;
			y2_sc.im = Y_inbuf[1][k].im;

			// X
			// hc11_sc = conj(h11_sc)
			hc11_sc.re =  h11_sc.re;
			hc11_sc.im = -h11_sc.im;			
			// hc21_sc = conj(h21_sc)
			hc21_sc.re = h21_sc.re;
			hc21_sc.im = -h21_sc.im;

			// hc11y1_sc=hc11_sc*y1_sc
			hc11y1_sc.re = (hc11_sc.re*y1_sc.re - hc11_sc.im*y1_sc.im);
			hc11y1_sc.im = (hc11_sc.re*y1_sc.im + hc11_sc.im*y1_sc.re);
			// hc21y2_sc=hc21_sc*y2_sc
			hc21y2_sc.re = (hc21_sc.re*y2_sc.re - hc21_sc.im*y2_sc.im);
			hc21y2_sc.im = (hc21_sc.re*y2_sc.im + hc21_sc.im*y2_sc.re);

			// u1_sc = hc11y1_sc + hc21y2_sc
			u1_sc.re = hc11y1_sc.re + hc21y2_sc.re;
			u1_sc.im = hc11y1_sc.im + hc21y2_sc.im;
			
			X_sc.re = u1_sc.re;
			X_sc.im = u1_sc.im;

			// csi
			a11_sc  = h11_sc.re*h11_sc.re+h11_sc.im*h11_sc.im;
			a21_sc  = h21_sc.re*h21_sc.re+h21_sc.im*h21_sc.im;			
			csi_sc = a11_sc+a21_sc;

			// output
			X_outbuf[0][k].re   = X_sc.re;
			X_outbuf[0][k].im   = X_sc.im;			
			csi_outbuf[0][k].re = csi_sc;
			csi_outbuf[0][k].im = 0;
		}
	}
	else if (layers == 2)
	{
		for (k = 0; k < inlen; k++)          
		{	// get H
			H_sc[0].re = H_inbuf[0][k].re;
			H_sc[0].im = H_inbuf[0][k].im;
			H_sc[1].re = H_inbuf[1][k].re;
			H_sc[1].im = H_inbuf[1][k].im;
			H_sc[2].re = H_inbuf[2][k].re;
			H_sc[2].im = H_inbuf[2][k].im;
			H_sc[3].re = H_inbuf[3][k].re;
			H_sc[3].im = H_inbuf[3][k].im;

			// get equal H
			// h11_sc = H_sc[0] * predmx_sc[0];
			h11_sc.re  = H_sc[0].re*predmx_sc[0].re - H_sc[0].im*predmx_sc[0].im;
			h11_sc.im  = H_sc[0].re*predmx_sc[0].im + H_sc[0].im*predmx_sc[0].re;
			// h11_sc += H_sc[1] * predmx_sc[2];
			h11_sc.re += H_sc[1].re*predmx_sc[2].re - H_sc[1].im*predmx_sc[2].im;
			h11_sc.im += H_sc[1].re*predmx_sc[2].im + H_sc[1].im*predmx_sc[2].re;
			// h12_sc = H_sc[0] * predmx_sc[1];
			h12_sc.re  = H_sc[0].re*predmx_sc[1].re - H_sc[0].im*predmx_sc[1].im;
			h12_sc.im  = H_sc[0].re*predmx_sc[1].im + H_sc[0].im*predmx_sc[1].re;
			// h12_sc += H_sc[1] * predmx_sc[3];
			h12_sc.re += H_sc[1].re*predmx_sc[3].re - H_sc[1].im*predmx_sc[3].im;
			h12_sc.im += H_sc[1].re*predmx_sc[3].im + H_sc[1].im*predmx_sc[3].re;
			// h21_sc = H_sc[2] * predmx_sc[0];
			h21_sc.re  = H_sc[2].re*predmx_sc[0].re - H_sc[2].im*predmx_sc[0].im;
			h21_sc.im  = H_sc[2].re*predmx_sc[0].im + H_sc[2].im*predmx_sc[0].re;
			// h21_sc += H_sc[3] * predmx_sc[2];
			h21_sc.re += H_sc[3].re*predmx_sc[2].re - H_sc[3].im*predmx_sc[2].im;
			h21_sc.im += H_sc[3].re*predmx_sc[2].im + H_sc[3].im*predmx_sc[2].re;
			// h22_sc = H_sc[2] * predmx_sc[1];
			h22_sc.re  = H_sc[2].re*predmx_sc[1].re - H_sc[2].im*predmx_sc[1].im;
			h22_sc.im  = H_sc[2].re*predmx_sc[1].im + H_sc[2].im*predmx_sc[1].re;				
			// h22_sc += H_sc[3] * predmx_sc[3];
			h22_sc.re += H_sc[3].re*predmx_sc[3].re - H_sc[3].im*predmx_sc[3].im;
			h22_sc.im += H_sc[3].re*predmx_sc[3].im + H_sc[3].im*predmx_sc[3].re;
			
			// hc11_sc = conj(h11_sc)
			hc11_sc.re =  h11_sc.re;
			hc11_sc.im = -h11_sc.im;
			// hc21_sc = conj(h21_sc)
			hc21_sc.re =  h21_sc.re;
			hc21_sc.im = -h21_sc.im;
			// hc12_sc = conj(h12_sc);
			hc12_sc.re =  h12_sc.re;
			hc12_sc.im = -h12_sc.im;
			// hc22_sc = conj(h22_sc);
			hc22_sc.re =  h22_sc.re;
			hc22_sc.im = -h22_sc.im;

			// get Y
			y1_sc.re = Y_inbuf[0][k].re;
			y1_sc.im = Y_inbuf[0][k].im;
			y2_sc.re = Y_inbuf[1][k].re;
			y2_sc.im = Y_inbuf[1][k].im;

			// a11 = |h_1_1|^2
			a11_sc = pow(h11_sc.re,2)+pow(h11_sc.im,2);		
			// a12 = |h_1_2|^2
			a12_sc = pow(h12_sc.re,2)+pow(h12_sc.im,2);   
			// a21 = |h_2_1|^2
			a21_sc = pow(h21_sc.re,2)+pow(h21_sc.im,2);
			// a22 = |h_2_2|^2
			a22_sc = pow(h22_sc.re,2)+pow(h22_sc.im,2);			

			a1_sc = a11_sc+a21_sc;
			a2_sc = a22_sc+a12_sc;

			// h11h22_sc = h11_sc * h22_sc
			h11h22_sc.re = h11_sc.re*h22_sc.re - h11_sc.im*h22_sc.im;
			h11h22_sc.im = h11_sc.re*h22_sc.im + h11_sc.im*h22_sc.re;

			// h12h21_sc = h12_sc * h21_sc;
			h12h21_sc.re = h12_sc.re*h21_sc.re - h12_sc.im*h21_sc.im;
			h12h21_sc.im = h12_sc.re*h21_sc.im + h12_sc.im*h21_sc.re;

			// W_sc = h11h22_sc - h12h21_sc;
			W_sc.re = h11h22_sc.re - h12h21_sc.re;
			W_sc.im = h11h22_sc.im - h12h21_sc.im;

			W_cnj_sc.re =  W_sc.re;
			W_cnj_sc.im = -W_sc.im;

			W2_sc = W_sc.re*W_sc.re + W_sc.im*W_sc.im;
			
			// h22y1_sc = h22_sc * y1_sc;
			h22y1_sc.re = h22_sc.re*y1_sc.re - h22_sc.im*y1_sc.im;
			h22y1_sc.im = h22_sc.re*y1_sc.im + h22_sc.im*y1_sc.re;

			// h12y2_sc = h12_sc * y2_sc;
			h12y2_sc.re = h12_sc.re*y2_sc.re - h12_sc.im*y2_sc.im;
			h12y2_sc.im = h12_sc.re*y2_sc.im + h12_sc.im*y2_sc.re;

			u0_sc.re = h22y1_sc.re - h12y2_sc.re;
			u0_sc.im = h22y1_sc.im - h12y2_sc.im;
			
			// hc11y1_sc = hc11_sc * y1_sc;
			hc11y1_sc.re = hc11_sc.re*y1_sc.re - hc11_sc.im*y1_sc.im;
			hc11y1_sc.im = hc11_sc.re*y1_sc.im + hc11_sc.im*y1_sc.re;

			// hc21y2_sc = hc21_sc * y2_sc;
			hc21y2_sc.re = hc21_sc.re*y2_sc.re - hc21_sc.im*y2_sc.im;
			hc21y2_sc.im = hc21_sc.re*y2_sc.im + hc21_sc.im*y2_sc.re;

			u1_sc.re = hc11y1_sc.re + hc21y2_sc.re;
			u1_sc.im = hc11y1_sc.im + hc21y2_sc.im;

			// z0_sc = W_cnj_sc*u0_sc;
			z0_sc.re = W_cnj_sc.re*u0_sc.re - W_cnj_sc.im*u0_sc.im;
			z0_sc.im = W_cnj_sc.re*u0_sc.im + W_cnj_sc.im*u0_sc.re;

			z1_sc.re = noiseVar_sc*u1_sc.re;
			z1_sc.im = noiseVar_sc*u1_sc.im;			
			
			X0_sc.re = z0_sc.re + z1_sc.re;
			X0_sc.im = z0_sc.im + z1_sc.im;

			csi0_sc = W2_sc + a1_sc*noiseVar_sc;

			// h11y2_sc = h11_sc * y2_sc;
			h11y2_sc.re = h11_sc.re*y2_sc.re - h11_sc.im*y2_sc.im;
			h11y2_sc.im = h11_sc.re*y2_sc.im + h11_sc.im*y2_sc.re;

			// h21y1_sc = h21_sc * y1_sc;
			h21y1_sc.re = h21_sc.re*y1_sc.re - h21_sc.im*y1_sc.im;
			h21y1_sc.im = h21_sc.re*y1_sc.im + h21_sc.im*y1_sc.re;

			u2_sc.re = h11y2_sc.re - h21y1_sc.re;
			u2_sc.im = h11y2_sc.im - h21y1_sc.im;

			// hc12y1_sc = hc12_sc * y1_sc;
			hc12y1_sc.re = hc12_sc.re*y1_sc.re - hc12_sc.im*y1_sc.im;
			hc12y1_sc.im = hc12_sc.re*y1_sc.im + hc12_sc.im*y1_sc.re;

			// hc22y2_sc = hc22_sc * y2_sc;
			hc22y2_sc.re = hc22_sc.re*y2_sc.re - hc22_sc.im*y2_sc.im;
			hc22y2_sc.im = hc22_sc.re*y2_sc.im + hc22_sc.im*y2_sc.re;

			u3_sc.re = hc12y1_sc.re + hc22y2_sc.re;
			u3_sc.im = hc12y1_sc.im + hc22y2_sc.im;

			// z2_sc = W_cnj_sc*u2_sc;
			z2_sc.re = W_cnj_sc.re*u2_sc.re - W_cnj_sc.im*u2_sc.im;
			z2_sc.im = W_cnj_sc.re*u2_sc.im + W_cnj_sc.im*u2_sc.re;

			z3_sc.re = noiseVar_sc*u3_sc.re;
			z3_sc.im = noiseVar_sc*u3_sc.im;

			X1_sc.re = z2_sc.re + z3_sc.re;
			X1_sc.im = z2_sc.im + z3_sc.im;

			csi1_sc = W2_sc + a2_sc*noiseVar_sc;

			// noise
			ns0_sc = a2_sc*noiseVar_sc + noiseVar_sc*noiseVar_sc;
			ns1_sc = a1_sc*noiseVar_sc + noiseVar_sc*noiseVar_sc;

			X_outbuf[0][k].re = X0_sc.re;
			X_outbuf[0][k].im = X0_sc.im;
			X_outbuf[1][k].re = X1_sc.re;
			X_outbuf[1][k].im = X1_sc.im;			

			csi_outbuf[0][k].re = csi0_sc;
			csi_outbuf[0][k].im = 0;
			csi_outbuf[1][k].re = csi1_sc;
			csi_outbuf[1][k].im = 0;

			ns_outbuf[0][k].re = ns0_sc;
			ns_outbuf[0][k].im = 0;
			ns_outbuf[1][k].re = ns1_sc;
			ns_outbuf[1][k].im = 0;
		}
	} 

	return inlen;
}

int equ_ctrch(cfloat **Y_inbuf, cfloat **H_inbuf, int inlen, float nosieVar, int num_bstx_ant
						, int num_uerx_ant, cfloat *X_outbuf, cfloat *csi_outbuf)
{
	int outlen = 0;

	if (num_bstx_ant == 1 && num_uerx_ant == 1)		 // SISO
	{
		outlen = SISO_decod(Y_inbuf[0], H_inbuf[0], inlen, nosieVar, X_outbuf, csi_outbuf);
	}
	else if (num_bstx_ant == 1 && num_uerx_ant == 2) // SIMO, MRC
	{
		outlen = MRC_decod(Y_inbuf, H_inbuf, inlen, nosieVar, X_outbuf, csi_outbuf);
	}
	else if (num_bstx_ant == 2 && num_uerx_ant == 1) // MISO, SFBC
	{
		outlen = SFBC_decod(Y_inbuf, H_inbuf, inlen, num_bstx_ant, num_uerx_ant
			, nosieVar, X_outbuf, csi_outbuf);
	}
	else if (num_bstx_ant == 2 && num_uerx_ant == 2) // MIMO, SFBC or Closloop_SM_prec
	{
		outlen = SFBC_decod(Y_inbuf, H_inbuf, inlen, num_bstx_ant, num_uerx_ant
			, nosieVar, X_outbuf, csi_outbuf);
	}
	else
	{
		assert(0);
	}

	return outlen;
}


int equ_pdsch(cfloat **Y_inbuf, cfloat **H_inbuf, int inlen, float nosieVar, int num_bstx_ant
						, int num_uerx_ant, int diver_spat, int num_layer_total, int codebook_idx, cfloat **X_outbuf
						, cfloat **csi_outbuf, cfloat **ns_outbuf)
{	
	int outlen = 0;
	
	if (num_bstx_ant == 1 && num_uerx_ant == 1)		// SISO
	{
		outlen = SISO_decod(Y_inbuf[0], H_inbuf[0], inlen, nosieVar, X_outbuf[0], csi_outbuf[0]);
	}
	else if (num_bstx_ant == 1 && num_uerx_ant == 2) // SIMO, MRC
	{
		outlen = MRC_decod(Y_inbuf, H_inbuf, inlen, nosieVar, X_outbuf[0], csi_outbuf[0]);
	}
	else if (num_bstx_ant == 2 && num_uerx_ant == 1) // SFBC
	{
		outlen = SFBC_decod(Y_inbuf, H_inbuf, inlen, num_bstx_ant, num_uerx_ant, nosieVar, X_outbuf[0], csi_outbuf[0]);
	}
	else if (num_bstx_ant == 2 && num_uerx_ant == 2) // MIMO, SFBC or Closloop_SM_prec
	{
		if (diver_spat == UERX_SPATIAL)
		{
			outlen = CLSM_decod(Y_inbuf, H_inbuf, inlen, num_bstx_ant, num_uerx_ant
				, num_layer_total, codebook_idx, nosieVar, X_outbuf, csi_outbuf, ns_outbuf);	
		} 
		else
		{
			outlen = SFBC_decod(Y_inbuf, H_inbuf, inlen, num_bstx_ant, num_uerx_ant, nosieVar, X_outbuf[0], csi_outbuf[0]);
		}
	}
	else
	{
		assert(0);
	}	
	
	return outlen;
}
