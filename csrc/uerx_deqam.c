#include "foo.h"
#include "uerx_deqam.h"
#include "uerx_fixed_cfg.h"

#define min2(a, b)				(( (a) < (b) )? (a): (b))
#define min4(a,b,c,d)			(( min2(a,b) <  min2(c, d) )? (min2(a, b)): (min2(c, d)))



int deqam_clsm2layer(cfloat *inbuf, cfloat *csi, cfloat *ns, int inlen, int qm, float *outbuf)
{
 	int i, j;
	int isLittle0, isLittle1, isLittle2, isLittle3, isLittle4, isLittle5;
#if LTEDL_FIXD_FLG == 0 || (LTEDL_FIXD_FLG == 8  && LTEDL_DEQAM_DEQAM == 0)
	float pt1_sc, pt3_sc, pt5_sc, pt7_sc;
	float re_sc, im_sc;
	float csi_sc;
	float b0_sc, b1_sc, b2_sc, b3_sc, b4_sc, b5_sc;
	float re_negt1_sc, re_negt3_sc, re_negt5_sc, re_negt7_sc
		, re_plus1_sc, re_plus3_sc, re_plus5_sc, re_plus7_sc
		, im_negt1_sc, im_negt3_sc, im_negt5_sc, im_negt7_sc
		, im_plus1_sc, im_plus3_sc, im_plus5_sc, im_plus7_sc;
	float re_negt7_pow_sc, re_negt5_pow_sc, re_negt3_pow_sc, re_negt1_pow_sc, re_plus1_pow_sc, re_plus3_pow_sc, re_plus5_pow_sc, re_plus7_pow_sc;
	float im_negt7_pow_sc, im_negt5_pow_sc, im_negt3_pow_sc, im_negt1_pow_sc, im_plus1_pow_sc, im_plus3_pow_sc, im_plus5_pow_sc, im_plus7_pow_sc;
	float pt1_csi_sc, pt3_csi_sc, pt5_csi_sc, pt7_csi_sc;
	float f0_1_sc, f0_0_sc, f1_1_sc, f1_0_sc, f2_1_sc, f2_0_sc, f3_1_sc, f3_0_sc;
	float f4_1_sc, f4_0_sc, f5_1_sc, f5_0_sc;
	float quant_step_sc;
	float ns_sc;
	float ns_csi_sc;
	float f0_sc, f1_sc, f2_sc, f3_sc, f4_sc, f5_sc;
	float b0_ns_csi_sc, b1_ns_csi_sc, b2_ns_csi_sc, b3_ns_csi_sc, b4_ns_csi_sc, b5_ns_csi_sc;
#elif LTEDL_FIXD_FLG == 3 || (LTEDL_FIXD_FLG == 8  && LTEDL_DEQAM_DEQAM == 3)
	sc_fixed<16, 2, SC_RND, SC_SAT> pt1_sc, pt3_sc, pt5_sc, pt7_sc;
	sc_fixed<16, 37, SC_RND, SC_SAT> re_sc, im_sc, csi_sc;
	sc_fixed<16, 29, SC_RND, SC_SAT> ns_sc;
	sc_fixed<16, 65, SC_RND, SC_SAT> ns_csi_sc;
	sc_fixed<16, 38, SC_RND, SC_SAT> pt1_csi_sc, pt3_csi_sc, pt5_csi_sc, pt7_csi_sc
		, re_negt1_sc, re_negt3_sc, re_negt5_sc, re_negt7_sc
		, re_plus1_sc, re_plus3_sc, re_plus5_sc, re_plus7_sc
		, im_negt1_sc, im_negt3_sc, im_negt5_sc, im_negt7_sc
		, im_plus1_sc, im_plus3_sc, im_plus5_sc, im_plus7_sc;
	sc_fixed<32, 75, SC_RND, SC_SAT> f0_1_sc, f0_0_sc, f1_1_sc, f1_0_sc, f2_1_sc, f2_0_sc
		, f3_1_sc, f3_0_sc, f4_1_sc, f4_0_sc, f5_1_sc, f5_0_sc
		, f0_sc, f1_sc, f2_sc, f3_sc, f4_sc, f5_sc
		, b0_ns_csi_sc, b1_ns_csi_sc, b2_ns_csi_sc, b3_ns_csi_sc, b4_ns_csi_sc, b5_ns_csi_sc
		, re_negt1_pow_sc, re_negt3_pow_sc, re_negt5_pow_sc, re_negt7_pow_sc
		, re_plus1_pow_sc, re_plus3_pow_sc, re_plus5_pow_sc, re_plus7_pow_sc
		, im_negt1_pow_sc, im_negt3_pow_sc, im_negt5_pow_sc, im_negt7_pow_sc
		, im_plus1_pow_sc, im_plus3_pow_sc, im_plus5_pow_sc, im_plus7_pow_sc;
	sc_fixed<16, 3, SC_RND, SC_SAT> b0_sc, b1_sc, b2_sc, b3_sc, b4_sc, b5_sc, quant_step_sc;
#endif	

	if (qm == 2)
	{
		for(i=0; i<inlen; i++)
		{
  			re_sc = inbuf[i].re;
			im_sc = inbuf[i].im;
			ns_sc = ns[i].re;

			b0_sc = 0;
			b1_sc = 0;

			quant_step_sc = 2;

			for (j=0; j<8; j++)
			{
				isLittle0 = (b0_sc*ns_sc) < re_sc*4; // (re_sc*4) == <16,39> 
				isLittle1 = (b1_sc*ns_sc) < im_sc*4;
				
				if (isLittle0)
					b0_sc += quant_step_sc;
				else
					b0_sc -= quant_step_sc;		
				
				if (isLittle1)
					b1_sc += quant_step_sc;
				else
					b1_sc -= quant_step_sc;

				quant_step_sc /= 2;
			}
			
			outbuf[2*i+0] =	b0_sc; 
			outbuf[2*i+1] = b1_sc; 
		}
	}
	else if (qm == 4)
	{
		pt1_sc = 0.3162;	// 0.3162 == 1/sqrt(10)
		pt3_sc = 0.9487;	// 0.9487 == 3/sqrt(10)

		for(i=0; i<inlen; i++)
		{
			re_sc  = inbuf[i].re;
			im_sc  = inbuf[i].im;
 			csi_sc = csi[i].re;
			ns_sc  = ns[i].re;
			ns_csi_sc = ns_sc*csi_sc;

			// compare
			pt1_csi_sc = pt1_sc*csi_sc;
			pt3_csi_sc = pt3_sc*csi_sc;

			re_negt1_sc = re_sc + pt1_csi_sc;  
			im_negt1_sc = im_sc + pt1_csi_sc;
			
			re_negt3_sc = re_sc + pt3_csi_sc;
			im_negt3_sc = im_sc + pt3_csi_sc;

			re_plus1_sc = re_sc - pt1_csi_sc;
			im_plus1_sc = im_sc - pt1_csi_sc;

			re_plus3_sc = re_sc - pt3_csi_sc;
			im_plus3_sc = im_sc - pt3_csi_sc;
			
			// <16,73> = <16,37> * <16,37>
			re_negt1_pow_sc = re_negt1_sc * re_negt1_sc; 
			im_negt1_pow_sc = im_negt1_sc * im_negt1_sc;
			
			re_negt3_pow_sc = re_negt3_sc * re_negt3_sc;
			im_negt3_pow_sc = im_negt3_sc * im_negt3_sc;

			re_plus1_pow_sc = re_plus1_sc * re_plus1_sc;
			im_plus1_pow_sc = im_plus1_sc * im_plus1_sc;
			
			re_plus3_pow_sc = re_plus3_sc * re_plus3_sc;
			im_plus3_pow_sc = im_plus3_sc * im_plus3_sc;
			
			// f0,f1,f2,f3
			f0_0_sc	= min2(re_negt1_pow_sc, re_negt3_pow_sc);
			f0_1_sc	= min2(re_plus1_pow_sc, re_plus3_pow_sc);
			f1_0_sc	= min2(im_negt1_pow_sc, im_negt3_pow_sc);
			f1_1_sc	= min2(im_plus1_pow_sc, im_plus3_pow_sc);
			f2_0_sc	= min2(re_negt3_pow_sc, re_plus3_pow_sc);
			f2_1_sc	= min2(re_negt1_pow_sc, re_plus1_pow_sc);
			f3_0_sc	= min2(im_negt3_pow_sc, im_plus3_pow_sc);
			f3_1_sc	= min2(im_negt1_pow_sc, im_plus1_pow_sc);

			f0_sc = f0_0_sc -  f0_1_sc;			
			f1_sc = f1_0_sc -  f1_1_sc;			
			f2_sc = f2_0_sc -  f2_1_sc;
			f3_sc = f3_0_sc -  f3_1_sc;
			
			// b0,b1,b2,b3
			b0_sc = 0;
			b1_sc = 0;
			b2_sc = 0;
			b3_sc = 0;
			
			quant_step_sc = 2;
			
			for (j=0; j<5; j++)
			{			
				b0_ns_csi_sc = b0_sc*ns_csi_sc;
				b1_ns_csi_sc = b1_sc*ns_csi_sc;
				b2_ns_csi_sc = b2_sc*ns_csi_sc;
				b3_ns_csi_sc = b3_sc*ns_csi_sc;

				isLittle0 = b0_ns_csi_sc < f0_sc;
				isLittle1 = b1_ns_csi_sc < f1_sc;
				isLittle2 = b2_ns_csi_sc < f2_sc;
				isLittle3 = b3_ns_csi_sc < f3_sc;				
				
				if (isLittle0)
					b0_sc += quant_step_sc;
				else
					b0_sc -= quant_step_sc;
				
				if (isLittle1)
					b1_sc += quant_step_sc;
				else
					b1_sc -= quant_step_sc;
				
				if (isLittle2)
					b2_sc += quant_step_sc;
				else
					b2_sc -= quant_step_sc;
				
				if (isLittle3)
					b3_sc += quant_step_sc;
				else
					b3_sc -= quant_step_sc;
				
				quant_step_sc /= 2;
			}
			
			outbuf[4*i+0] = b0_sc;	
			outbuf[4*i+1] = b1_sc;			        
			outbuf[4*i+2] = b2_sc;
			outbuf[4*i+3] = b3_sc;
		}
	}
	else if (qm == 6)
	{
		pt1_sc = 0.1543;	// 0.1543 == 1/sqrt(42)
		pt3_sc = 0.4629;	// 0.4629 == 3/sqrt(42)
		pt5_sc = 0.7715;	// 0.7715 == 5/sqrt(42)
		pt7_sc = 1.0801;	// 1.0801 == 7/sqrt(42)		

		for(i=0; i<inlen; i++)
		{
			re_sc  = inbuf[i].re;
			im_sc  = inbuf[i].im;
			csi_sc = csi[i].re;
			ns_sc  = ns[i].re;
			ns_csi_sc = ns_sc*csi_sc;

			// compare
			pt1_csi_sc = pt1_sc*csi_sc;
			pt3_csi_sc = pt3_sc*csi_sc;
			pt5_csi_sc = pt5_sc*csi_sc;
			pt7_csi_sc = pt7_sc*csi_sc;

			re_negt1_sc = re_sc + pt1_csi_sc;  
			im_negt1_sc = im_sc + pt1_csi_sc;
			
			re_negt3_sc = re_sc + pt3_csi_sc;
			im_negt3_sc = im_sc + pt3_csi_sc;
			
			re_negt5_sc = re_sc + pt5_csi_sc;
			im_negt5_sc = im_sc + pt5_csi_sc;
			
			re_negt7_sc = re_sc + pt7_csi_sc;
			im_negt7_sc = im_sc + pt7_csi_sc;

			re_plus1_sc = re_sc - pt1_csi_sc;
			im_plus1_sc = im_sc - pt1_csi_sc;

			re_plus3_sc = re_sc - pt3_csi_sc;
			im_plus3_sc = im_sc - pt3_csi_sc;

			re_plus5_sc = re_sc - pt5_csi_sc;
			im_plus5_sc = im_sc - pt5_csi_sc;

			re_plus7_sc = re_sc - pt7_csi_sc;
			im_plus7_sc = im_sc - pt7_csi_sc;

			//save.Run(re_negt1_sc, "re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(re_negt3_sc, "re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(re_negt5_sc, "re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(re_negt7_sc, "re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(re_plus1_sc, "re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(re_plus3_sc, "re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(re_plus5_sc, "re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(re_plus7_sc, "re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(im_negt1_sc, "re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(im_negt3_sc, "re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(im_negt5_sc, "re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(im_negt7_sc, "re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(im_plus1_sc, "re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(im_plus3_sc, "re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(im_plus5_sc, "re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(im_plus7_sc, "re_im_negt_plus_deqam", TO_SAVE);

			// <16,73> = <16,37> * <16,37>
			re_negt1_pow_sc = re_negt1_sc * re_negt1_sc;  
			im_negt1_pow_sc = im_negt1_sc * im_negt1_sc;
			
			re_negt3_pow_sc = re_negt3_sc * re_negt3_sc;
			im_negt3_pow_sc = im_negt3_sc * im_negt3_sc;
			
			re_negt5_pow_sc = re_negt5_sc * re_negt5_sc;
			im_negt5_pow_sc = im_negt5_sc * im_negt5_sc;
			
			re_negt7_pow_sc = re_negt7_sc * re_negt7_sc;
			im_negt7_pow_sc = im_negt7_sc * im_negt7_sc;

			re_plus1_pow_sc = re_plus1_sc * re_plus1_sc;
			im_plus1_pow_sc = im_plus1_sc * im_plus1_sc;

			re_plus3_pow_sc = re_plus3_sc * re_plus3_sc;
			im_plus3_pow_sc = im_plus3_sc * im_plus3_sc;

			re_plus5_pow_sc = re_plus5_sc * re_plus5_sc;
			im_plus5_pow_sc = im_plus5_sc * im_plus5_sc;

			re_plus7_pow_sc = re_plus7_sc * re_plus7_sc;
			im_plus7_pow_sc = im_plus7_sc * im_plus7_sc;
			

			// f0,f1,f2,f3,f4,f5
			f0_0_sc	= min4(re_negt1_pow_sc, re_negt3_pow_sc, re_negt5_pow_sc, re_negt7_pow_sc);
			f0_1_sc	= min4(re_plus1_pow_sc, re_plus3_pow_sc, re_plus5_pow_sc, re_plus7_pow_sc);
			f1_0_sc	= min4(im_negt1_pow_sc, im_negt3_pow_sc, im_negt5_pow_sc, im_negt7_pow_sc);
			f1_1_sc	= min4(im_plus1_pow_sc, im_plus3_pow_sc, im_plus5_pow_sc, im_plus7_pow_sc);
			f2_0_sc = min4(re_negt5_pow_sc, re_negt7_pow_sc, re_plus5_pow_sc, re_plus7_pow_sc);
			f2_1_sc = min4(re_negt1_pow_sc, re_negt3_pow_sc, re_plus1_pow_sc, re_plus3_pow_sc);
			f3_0_sc	= min4(im_negt5_pow_sc, im_negt7_pow_sc, im_plus5_pow_sc, im_plus7_pow_sc);
			f3_1_sc	= min4(im_negt1_pow_sc, im_negt3_pow_sc, im_plus1_pow_sc, im_plus3_pow_sc);
			f4_0_sc = min4(re_negt7_pow_sc, re_negt1_pow_sc, re_plus7_pow_sc, re_plus1_pow_sc);
			f4_1_sc = min4(re_negt3_pow_sc, re_negt5_pow_sc, re_plus3_pow_sc, re_plus5_pow_sc);
			f5_0_sc	= min4(im_negt7_pow_sc, im_negt1_pow_sc, im_plus7_pow_sc, im_plus1_pow_sc);
			f5_1_sc = min4(im_negt3_pow_sc, im_negt5_pow_sc, im_plus3_pow_sc, im_plus5_pow_sc);

			f0_sc = f0_0_sc -  f0_1_sc;
			f1_sc = f1_0_sc -  f1_1_sc;			
			f2_sc = f2_0_sc -  f2_1_sc;				
			f3_sc = f3_0_sc -  f3_1_sc;		
			f4_sc = f4_0_sc -  f4_1_sc;	
			f5_sc = f5_0_sc -  f5_1_sc;	

			
			// clear b0,b1,b2,b3,b4,b5			
			b0_sc = 0;
			b1_sc = 0;
			b2_sc = 0;
			b3_sc = 0;
			b4_sc = 0;
			b5_sc = 0;

			quant_step_sc = 2;
			
			// soft division
			for (j=0; j<5; j++)
			{		
				b0_ns_csi_sc = b0_sc*ns_csi_sc;
				b1_ns_csi_sc = b1_sc*ns_csi_sc;
				b2_ns_csi_sc = b2_sc*ns_csi_sc;
				b3_ns_csi_sc = b3_sc*ns_csi_sc;
				b4_ns_csi_sc = b4_sc*ns_csi_sc;
				b5_ns_csi_sc = b5_sc*ns_csi_sc;

				isLittle0 = b0_ns_csi_sc < f0_sc;
				isLittle1 = b1_ns_csi_sc < f1_sc;
				isLittle2 = b2_ns_csi_sc < f2_sc;
				isLittle3 = b3_ns_csi_sc < f3_sc;
				isLittle4 = b4_ns_csi_sc < f4_sc;
				isLittle5 = b5_ns_csi_sc < f5_sc;	

				if (isLittle0)
					b0_sc += quant_step_sc;
				else
					b0_sc -= quant_step_sc;
				
				if (isLittle1)
					b1_sc += quant_step_sc;
				else
					b1_sc -= quant_step_sc;

				if (isLittle2)
					b2_sc += quant_step_sc;
				else
					b2_sc -= quant_step_sc;
				
				if (isLittle3)
					b3_sc += quant_step_sc;
				else
					b3_sc -= quant_step_sc;
				
				if (isLittle4)
					b4_sc += quant_step_sc;
				else
					b4_sc -= quant_step_sc;

				if (isLittle5)
					b5_sc += quant_step_sc;
				else
					b5_sc -= quant_step_sc;

				quant_step_sc /= 2;
			}

			outbuf[6*i+0] = b0_sc;
			outbuf[6*i+1] = b1_sc;
			outbuf[6*i+2] = b2_sc;
			outbuf[6*i+3] = b3_sc;
			outbuf[6*i+4] = b4_sc;
			outbuf[6*i+5] =	b5_sc;
		}
	}
	else
	{
		assert(0);
	}

	return (inlen * qm);
}

int deqam(cfloat *inbuf, cfloat *csi, float nosieVar, int inlen, int qm, float *outbuf)
{
 	int i, j;
	int isLittle0, isLittle1, isLittle2, isLittle3, isLittle4, isLittle5;
#if LTEDL_FIXD_FLG == 0 || (LTEDL_FIXD_FLG == 8  && LTEDL_DEQAM_DEQAM == 0)
	float pt1_sc, pt5_sc, pt3_sc, pt7_sc;
	float re_sc, im_sc;
	float b0_sc, b1_sc, b2_sc, b3_sc, b4_sc, b5_sc;
	float re_negt7_pow_sc, re_negt5_pow_sc, re_negt3_pow_sc, re_negt1_pow_sc, re_plus1_pow_sc, re_plus3_pow_sc, re_plus5_pow_sc, re_plus7_pow_sc;
	float im_negt7_pow_sc, im_negt5_pow_sc, im_negt3_pow_sc, im_negt1_pow_sc, im_plus1_pow_sc, im_plus3_pow_sc, im_plus5_pow_sc, im_plus7_pow_sc;
	float csi_sc;
	float ns_sc;
	float f0_1_sc, f0_0_sc, f1_1_sc, f1_0_sc, f2_1_sc, f2_0_sc, f3_1_sc, f3_0_sc;
	float f4_1_sc, f4_0_sc, f5_1_sc, f5_0_sc;
	float quant_step_sc;
	float ns_csi_sc;
	float f0_sc, f1_sc, f2_sc, f3_sc, f4_sc, f5_sc;
	float pt1_csi_sc, pt3_csi_sc, pt5_csi_sc, pt7_csi_sc
		, re_negt1_sc, re_negt3_sc, re_negt5_sc, re_negt7_sc
		, re_plus1_sc, re_plus3_sc, re_plus5_sc, re_plus7_sc
		, im_negt1_sc, im_negt3_sc, im_negt5_sc, im_negt7_sc
		, im_plus1_sc, im_plus3_sc, im_plus5_sc, im_plus7_sc;
	float b0_ns_csi_sc, b1_ns_csi_sc, b2_ns_csi_sc, b3_ns_csi_sc, b4_ns_csi_sc, b5_ns_csi_sc;
#elif LTEDL_FIXD_FLG == 3 || (LTEDL_FIXD_FLG == 8  && LTEDL_DEQAM_DEQAM == 3)
	sc_fixed<16, 2, SC_RND, SC_SAT> pt1_sc, pt3_sc, pt5_sc, pt7_sc;
	sc_fixed<16, 21, SC_RND, SC_SAT> csi_sc;
	sc_fixed<16, 16, SC_RND, SC_SAT> ns_sc;
	sc_fixed<16, 21, SC_RND, SC_SAT> re_sc, im_sc;
	sc_fixed<16, 22, SC_RND, SC_SAT> pt1_csi_sc, pt3_csi_sc, pt5_csi_sc, pt7_csi_sc
		, re_negt1_sc, re_negt3_sc, re_negt5_sc, re_negt7_sc
		, re_plus1_sc, re_plus3_sc, re_plus5_sc, re_plus7_sc
		, im_negt1_sc, im_negt3_sc, im_negt5_sc, im_negt7_sc
		, im_plus1_sc, im_plus3_sc, im_plus5_sc, im_plus7_sc;
	sc_fixed<32, 43, SC_RND, SC_SAT> f0_1_sc, f0_0_sc, f1_1_sc, f1_0_sc
		, f2_1_sc, f2_0_sc, f3_1_sc, f3_0_sc, f4_1_sc, f4_0_sc, f5_1_sc, f5_0_sc
		, f0_sc, f1_sc, f2_sc, f3_sc, f4_sc, f5_sc
		, re_negt1_pow_sc, re_negt3_pow_sc, re_negt5_pow_sc, re_negt7_pow_sc
		, re_plus1_pow_sc, re_plus3_pow_sc, re_plus5_pow_sc, re_plus7_pow_sc
		, im_negt1_pow_sc, im_negt3_pow_sc, im_negt5_pow_sc, im_negt7_pow_sc
		, im_plus1_pow_sc, im_plus3_pow_sc, im_plus5_pow_sc, im_plus7_pow_sc
		, b0_ns_csi_sc, b1_ns_csi_sc, b2_ns_csi_sc, b3_ns_csi_sc, b4_ns_csi_sc, b5_ns_csi_sc;
	sc_fixed<16, 3, SC_RND, SC_SAT> b0_sc, b1_sc, b2_sc, b3_sc, b4_sc, b5_sc, quant_step_sc;
	sc_fixed<16, 36, SC_RND, SC_SAT> ns_csi_sc;
#endif	

	if (qm == 2)
	{
		for(i=0; i<inlen; i++)
		{
  			re_sc = inbuf[i].re;
			im_sc = inbuf[i].im;
			ns_sc = nosieVar;

			b0_sc = 0;
			b1_sc = 0;

			quant_step_sc = 2;

			for (j=0; j<8; j++)
			{
				isLittle0 = (b0_sc*ns_sc) < re_sc*4; // (re_sc*4) == <16,39> 
				isLittle1 = (b1_sc*ns_sc) < im_sc*4;
				
				if (isLittle0)
					b0_sc += quant_step_sc;
				else
					b0_sc -= quant_step_sc;		
				
				if (isLittle1)
					b1_sc += quant_step_sc;
				else
					b1_sc -= quant_step_sc;

				quant_step_sc /= 2;
			}
			
			outbuf[2*i+0] =	b0_sc; 
			outbuf[2*i+1] = b1_sc; 
		}
	}
	else if (qm == 4)
	{
		pt1_sc = 0.3162;	// 0.3162 == 1/sqrt(10)
		pt3_sc = 0.9487;	// 0.9487 == 3/sqrt(10)

		for(i=0; i<inlen; i++)
		{
			re_sc  = inbuf[i].re;
			im_sc  = inbuf[i].im;
 			csi_sc = csi[i].re;
			ns_sc  = nosieVar;
			ns_csi_sc = ns_sc*csi_sc;

			// compare
			pt1_csi_sc = pt1_sc*csi_sc;
			pt3_csi_sc = pt3_sc*csi_sc;

			re_negt1_sc = re_sc + pt1_csi_sc;  
			im_negt1_sc = im_sc + pt1_csi_sc;
			
			re_negt3_sc = re_sc + pt3_csi_sc;
			im_negt3_sc = im_sc + pt3_csi_sc;

			re_plus1_sc = re_sc - pt1_csi_sc;
			im_plus1_sc = im_sc - pt1_csi_sc;

			re_plus3_sc = re_sc - pt3_csi_sc;
			im_plus3_sc = im_sc - pt3_csi_sc;
			
			// <16,73> = <16,37> * <16,37>
			re_negt1_pow_sc = re_negt1_sc * re_negt1_sc; 
			im_negt1_pow_sc = im_negt1_sc * im_negt1_sc;
			
			re_negt3_pow_sc = re_negt3_sc * re_negt3_sc;
			im_negt3_pow_sc = im_negt3_sc * im_negt3_sc;

			re_plus1_pow_sc = re_plus1_sc * re_plus1_sc;
			im_plus1_pow_sc = im_plus1_sc * im_plus1_sc;
			
			re_plus3_pow_sc = re_plus3_sc * re_plus3_sc;
			im_plus3_pow_sc = im_plus3_sc * im_plus3_sc;
			
			// f0,f1,f2,f3
			f0_0_sc	= min2(re_negt1_pow_sc, re_negt3_pow_sc);
			f0_1_sc	= min2(re_plus1_pow_sc, re_plus3_pow_sc);
			f1_0_sc	= min2(im_negt1_pow_sc, im_negt3_pow_sc);
			f1_1_sc	= min2(im_plus1_pow_sc, im_plus3_pow_sc);
			f2_0_sc	= min2(re_negt3_pow_sc, re_plus3_pow_sc);
			f2_1_sc	= min2(re_negt1_pow_sc, re_plus1_pow_sc);
			f3_0_sc	= min2(im_negt3_pow_sc, im_plus3_pow_sc);
			f3_1_sc	= min2(im_negt1_pow_sc, im_plus1_pow_sc);

			f0_sc = f0_0_sc -  f0_1_sc;			
			f1_sc = f1_0_sc -  f1_1_sc;			
			f2_sc = f2_0_sc -  f2_1_sc;
			f3_sc = f3_0_sc -  f3_1_sc;
			
			// b0,b1,b2,b3
			b0_sc = 0;
			b1_sc = 0;
			b2_sc = 0;
			b3_sc = 0;
			
			quant_step_sc = 2;
			
			for (j=0; j<5; j++)
			{			
				b0_ns_csi_sc = b0_sc*ns_csi_sc;
				b1_ns_csi_sc = b1_sc*ns_csi_sc;
				b2_ns_csi_sc = b2_sc*ns_csi_sc;
				b3_ns_csi_sc = b3_sc*ns_csi_sc;

				isLittle0 = b0_ns_csi_sc < f0_sc;
				isLittle1 = b1_ns_csi_sc < f1_sc;
				isLittle2 = b2_ns_csi_sc < f2_sc;
				isLittle3 = b3_ns_csi_sc < f3_sc;				
				
				if (isLittle0)
					b0_sc += quant_step_sc;
				else
					b0_sc -= quant_step_sc;
				
				if (isLittle1)
					b1_sc += quant_step_sc;
				else
					b1_sc -= quant_step_sc;
				
				if (isLittle2)
					b2_sc += quant_step_sc;
				else
					b2_sc -= quant_step_sc;
				
				if (isLittle3)
					b3_sc += quant_step_sc;
				else
					b3_sc -= quant_step_sc;
				
				quant_step_sc /= 2;
			}
			
			outbuf[4*i+0] = b0_sc;	
			outbuf[4*i+1] = b1_sc;			        
			outbuf[4*i+2] = b2_sc;
			outbuf[4*i+3] = b3_sc;
		}
	}
	else if (qm == 6)
	{
		pt1_sc = 0.1543;	// 0.1543 == 1/sqrt(42)
		pt3_sc = 0.4629;	// 0.4629 == 3/sqrt(42)
		pt5_sc = 0.7715;	// 0.7715 == 5/sqrt(42)
		pt7_sc = 1.0801;	// 1.0801 == 7/sqrt(42)		

		for(i=0; i<inlen; i++)
		{
			re_sc  = inbuf[i].re;
			im_sc  = inbuf[i].im;
			csi_sc = csi[i].re;
			ns_sc  = nosieVar;
			ns_csi_sc = ns_sc*csi_sc;

			// compare
			pt1_csi_sc = pt1_sc*csi_sc;
			pt3_csi_sc = pt3_sc*csi_sc;
			pt5_csi_sc = pt5_sc*csi_sc;
			pt7_csi_sc = pt7_sc*csi_sc;

			re_negt1_sc = re_sc + pt1_csi_sc;  
			im_negt1_sc = im_sc + pt1_csi_sc;
			
			re_negt3_sc = re_sc + pt3_csi_sc;
			im_negt3_sc = im_sc + pt3_csi_sc;
			
			re_negt5_sc = re_sc + pt5_csi_sc;
			im_negt5_sc = im_sc + pt5_csi_sc;
			
			re_negt7_sc = re_sc + pt7_csi_sc;
			im_negt7_sc = im_sc + pt7_csi_sc;

			re_plus1_sc = re_sc - pt1_csi_sc;
			im_plus1_sc = im_sc - pt1_csi_sc;

			re_plus3_sc = re_sc - pt3_csi_sc;
			im_plus3_sc = im_sc - pt3_csi_sc;

			re_plus5_sc = re_sc - pt5_csi_sc;
			im_plus5_sc = im_sc - pt5_csi_sc;

			re_plus7_sc = re_sc - pt7_csi_sc;
			im_plus7_sc = im_sc - pt7_csi_sc;

			// <16,73> = <16,37> * <16,37>
			re_negt1_pow_sc = re_negt1_sc * re_negt1_sc;  
			im_negt1_pow_sc = im_negt1_sc * im_negt1_sc;
			
			re_negt3_pow_sc = re_negt3_sc * re_negt3_sc;
			im_negt3_pow_sc = im_negt3_sc * im_negt3_sc;
			
			re_negt5_pow_sc = re_negt5_sc * re_negt5_sc;
			im_negt5_pow_sc = im_negt5_sc * im_negt5_sc;
			
			re_negt7_pow_sc = re_negt7_sc * re_negt7_sc;
			im_negt7_pow_sc = im_negt7_sc * im_negt7_sc;

			re_plus1_pow_sc = re_plus1_sc * re_plus1_sc;
			im_plus1_pow_sc = im_plus1_sc * im_plus1_sc;

			re_plus3_pow_sc = re_plus3_sc * re_plus3_sc;
			im_plus3_pow_sc = im_plus3_sc * im_plus3_sc;

			re_plus5_pow_sc = re_plus5_sc * re_plus5_sc;
			im_plus5_pow_sc = im_plus5_sc * im_plus5_sc;

			re_plus7_pow_sc = re_plus7_sc * re_plus7_sc;
			im_plus7_pow_sc = im_plus7_sc * im_plus7_sc;
			
			//save.Run(re_negt1_pow_sc, "pow_re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(re_negt3_pow_sc, "pow_re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(re_negt5_pow_sc, "pow_re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(re_negt7_pow_sc, "pow_re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(re_plus1_pow_sc, "pow_re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(re_plus3_pow_sc, "pow_re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(re_plus5_pow_sc, "pow_re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(re_plus7_pow_sc, "pow_re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(im_negt1_pow_sc, "pow_re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(im_negt3_pow_sc, "pow_re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(im_negt5_pow_sc, "pow_re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(im_negt7_pow_sc, "pow_re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(im_plus1_pow_sc, "pow_re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(im_plus3_pow_sc, "pow_re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(im_plus5_pow_sc, "pow_re_im_negt_plus_deqam", TO_SAVE);
			//save.Run(im_plus7_pow_sc, "pow_re_im_negt_plus_deqam", TO_SAVE);

			// f0,f1,f2,f3,f4,f5
			f0_0_sc	= min4(re_negt1_pow_sc, re_negt3_pow_sc, re_negt5_pow_sc, re_negt7_pow_sc);
			f0_1_sc	= min4(re_plus1_pow_sc, re_plus3_pow_sc, re_plus5_pow_sc, re_plus7_pow_sc);
			f1_0_sc	= min4(im_negt1_pow_sc, im_negt3_pow_sc, im_negt5_pow_sc, im_negt7_pow_sc);
			f1_1_sc	= min4(im_plus1_pow_sc, im_plus3_pow_sc, im_plus5_pow_sc, im_plus7_pow_sc);
			f2_0_sc = min4(re_negt5_pow_sc, re_negt7_pow_sc, re_plus5_pow_sc, re_plus7_pow_sc);
			f2_1_sc = min4(re_negt1_pow_sc, re_negt3_pow_sc, re_plus1_pow_sc, re_plus3_pow_sc);
			f3_0_sc	= min4(im_negt5_pow_sc, im_negt7_pow_sc, im_plus5_pow_sc, im_plus7_pow_sc);
			f3_1_sc	= min4(im_negt1_pow_sc, im_negt3_pow_sc, im_plus1_pow_sc, im_plus3_pow_sc);
			f4_0_sc = min4(re_negt7_pow_sc, re_negt1_pow_sc, re_plus7_pow_sc, re_plus1_pow_sc);
			f4_1_sc = min4(re_negt3_pow_sc, re_negt5_pow_sc, re_plus3_pow_sc, re_plus5_pow_sc);
			f5_0_sc	= min4(im_negt7_pow_sc, im_negt1_pow_sc, im_plus7_pow_sc, im_plus1_pow_sc);
			f5_1_sc = min4(im_negt3_pow_sc, im_negt5_pow_sc, im_plus3_pow_sc, im_plus5_pow_sc);

			f0_sc = f0_0_sc -  f0_1_sc;
			f1_sc = f1_0_sc -  f1_1_sc;			
			f2_sc = f2_0_sc -  f2_1_sc;				
			f3_sc = f3_0_sc -  f3_1_sc;		
			f4_sc = f4_0_sc -  f4_1_sc;	
			f5_sc = f5_0_sc -  f5_1_sc;	

			//save.Run(f0_sc, "f0t5_deqam", TO_SAVE);
			//save.Run(f1_sc, "f0t5_deqam", TO_SAVE);
			//save.Run(f2_sc, "f0t5_deqam", TO_SAVE);
			//save.Run(f3_sc, "f0t5_deqam", TO_SAVE);
			//save.Run(f4_sc, "f0t5_deqam", TO_SAVE);
			//save.Run(f5_sc, "f0t5_deqam", TO_SAVE);
			
			// clear b0,b1,b2,b3,b4,b5			
			b0_sc = 0;
			b1_sc = 0;
			b2_sc = 0;
			b3_sc = 0;
			b4_sc = 0;
			b5_sc = 0;

			quant_step_sc = 2;
			
			// soft division
			for (j=0; j<5; j++)
			{		
				b0_ns_csi_sc = b0_sc*ns_csi_sc;
				b1_ns_csi_sc = b1_sc*ns_csi_sc;
				b2_ns_csi_sc = b2_sc*ns_csi_sc;
				b3_ns_csi_sc = b3_sc*ns_csi_sc;
				b4_ns_csi_sc = b4_sc*ns_csi_sc;
				b5_ns_csi_sc = b5_sc*ns_csi_sc;

				isLittle0 = b0_ns_csi_sc < f0_sc;
				isLittle1 = b1_ns_csi_sc < f1_sc;
				isLittle2 = b2_ns_csi_sc < f2_sc;
				isLittle3 = b3_ns_csi_sc < f3_sc;
				isLittle4 = b4_ns_csi_sc < f4_sc;
				isLittle5 = b5_ns_csi_sc < f5_sc;	

				if (isLittle0)
					b0_sc += quant_step_sc;
				else
					b0_sc -= quant_step_sc;
				
				if (isLittle1)
					b1_sc += quant_step_sc;
				else
					b1_sc -= quant_step_sc;

				if (isLittle2)
					b2_sc += quant_step_sc;
				else
					b2_sc -= quant_step_sc;
				
				if (isLittle3)
					b3_sc += quant_step_sc;
				else
					b3_sc -= quant_step_sc;
				
				if (isLittle4)
					b4_sc += quant_step_sc;
				else
					b4_sc -= quant_step_sc;

				if (isLittle5)
					b5_sc += quant_step_sc;
				else
					b5_sc -= quant_step_sc;

				quant_step_sc /= 2;
			}

			outbuf[6*i+0] = b0_sc;
			outbuf[6*i+1] = b1_sc;
			outbuf[6*i+2] = b2_sc;
			outbuf[6*i+3] = b3_sc;
			outbuf[6*i+4] = b4_sc;
			outbuf[6*i+5] =	b5_sc;
		}
	}
	else
	{
		assert(0);
	}

	return (inlen * qm);
}
