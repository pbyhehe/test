/*************************************
*
*  Standard Definitions for Turbo Code for LTE based on 3GPP TS 36.212 v1.2.0
*
*  input:
*  
*  output:
*
*
*/

#include <math.h>
#include <memory.h>
#include <string.h>
#include "uerx_turbo.h"


#pragma SET_DATA_SECTION("uerx_data")
/*all possible code block lengths defined for LTE*/
int LTE_CODE_BLOCK_LENGTHS[LTE_MAX_BLOCK_LEN] = {
                                              40,48,56,64,72,80,88,96,104,112,120,
                                              128,136,144,152,160,168,176,184,192,
                                              200,208,216,224,232,240,248,256,264,
                                              272,280,288,296,304,312,320,328,336,
                                              344,352,360,368,376,384,392,400,408,
                                              416,424,432,440,448,456,464,472,480,
                                              488,496,504,512,528,544,560,576,592,
                                              608,624,640,656,672,688,704,720,736,
                                              752,768,784,800,816,832,848,864,880,
                                              896,912,928,944,960,976,992,1008,1024,
                                              1056,1088,1120,1152,1184,1216,1248,1280,
                                              1312,1344,1376,1408,1440,1472,1504,1536,
                                              1568,1600,1632,1664,1696,1728,1760,1792,
                                              1824,1856,1888,1920,1952,1984,2016,2048,
                                              2112,2176,2240,2304,2368,2432,2496,2560,
                                              2624,2688,2752,2816,2880,2944,3008,3072,
                                              3136,3200,3264,3328,3392,3456,3520,3584,
                                              3648,3712,3776,3840,3904,3968,4032,4096,
                                              4160,4224,4288,4352,4416,4480,4544,4608,
                                              4672,4736,4800,4864,4928,4992,5056,5120,
                                              5184,5248,5312,5376,5440,5504,5568,5632,
                                              5696,5760,5824,5888,5952,6016,6080,6144
                                        };

/*interleaver parameters for LTE (depending on the code block length*/
int LTE_INTERLEAVE_PARAM_F1[LTE_MAX_BLOCK_LEN] = {3, 7, 19, 7, 7, 11, 5, 11, 7, 41, 103, 15, 9, 17, 9, 21, 101, 21, 57, 
                                              23, 13, 27, 11, 27, 85, 29, 33, 15, 
                                              17, 33, 103, 19, 19, 37, 19, 21, 21, 115, 193, 21, 133, 81, 45, 23, 
                                              243, 151, 155, 25, 51, 47, 91, 29, 
                                              29, 247, 29, 89, 91, 157, 55, 31, 17, 35, 227, 65, 19, 37, 41, 39, 
                                              185, 43, 21, 155, 79, 139, 23, 217, 
                                              25, 17, 127, 25, 239, 17, 137, 215, 29, 15, 147, 29, 59, 65, 55, 31,
                                              17, 171, 67, 35, 19, 39, 19, 199, 
                                              21, 211, 21, 43, 149, 45, 49, 71, 13, 17, 25, 183, 55, 127, 27, 29, 
                                              29, 57, 45, 31, 59, 185, 113, 31, 
                                              17, 171, 209, 253, 367, 265, 181, 39, 27, 127, 143, 43, 29, 45, 157,
                                              47, 13, 111, 443, 51, 51, 451, 
                                              257, 57, 313, 271, 179, 331, 363, 375, 127, 31, 33, 43, 33, 477, 35,
                                              233, 357, 337, 37, 71, 71, 37, 
                                              39, 127, 39, 39, 31, 113, 41, 251, 43, 21, 43, 45, 45, 161, 89, 323, 
                                              47, 23, 47, 263};
  
int LTE_INTERLEAVE_PARAM_F2[LTE_MAX_BLOCK_LEN] = {10, 12, 42, 16, 18, 20, 22, 24, 26, 84, 90, 32, 34, 108, 38, 120, 84,
                                              44, 46, 48, 50, 52, 36, 56, 58, 
                                              60, 62, 32, 198, 68, 210, 36, 74, 76, 78, 120, 82, 84, 86, 44, 90, 46,
                                              94, 48, 98, 40, 102, 52, 106, 
                                              72, 110, 168, 114, 58, 118, 180, 122, 62, 84, 64, 66, 68, 420, 96, 74,
                                              76, 234, 80, 82, 252, 86, 
                                              44, 120, 92, 94, 48, 98, 80, 102, 52, 106, 48, 110, 112, 114, 58, 118,
                                              60, 122, 124, 84, 64, 66, 
                                              204, 140, 72, 74, 76, 78, 240, 82, 252, 86, 88, 60, 92, 846, 48, 28, 
                                              80, 102, 104, 954, 96, 110, 
                                              112, 114, 116, 354, 120, 610, 124, 420, 64, 66, 136, 420, 216, 444, 456,
                                              468, 80, 164, 504, 172, 88, 300, 92, 188, 96, 28, 240, 204, 104, 212,
                                              192, 220, 336, 228, 232, 236, 120, 244, 248, 
                                              168, 64, 130, 264, 134, 408, 138, 280, 142, 480, 146, 444, 120, 152, 462,
                                              234, 158, 80, 96, 902, 166, 336, 170, 86, 174, 176, 178, 120, 182, 184,
                                              186, 94, 190, 480};

/* number SISO that makes sense for LTE */
int NUMBER_OF_PSISOS[4]={1,2,4,8};
#pragma SET_DATA_SECTION()


void lteTurboInterleave(fixed input[LTE_MAX_BLOCK_LEN], 
						fixed output[LTE_MAX_BLOCK_LEN],
						int blockLengthIdx) 
{
/*************************
*  interleave a code block for LTE
*************************/
/* prevent overflow for indices (use 64-bit integers)*/
    long long i, j;
	long long blockLength = LTE_CODE_BLOCK_LENGTHS[blockLengthIdx];
	long long f1=LTE_INTERLEAVE_PARAM_F1[blockLengthIdx];
	long long f2=LTE_INTERLEAVE_PARAM_F2[blockLengthIdx];
	/* input and output can be identical -> copy input before writing output*/
	fixed tmp[LTE_MAX_BLOCK_LEN];
	memcpy(tmp, input, (unsigned int) blockLength*sizeof(fixed));

    for(i=0; i<blockLength; i++) {
        j = (f1*i + f2*i*i) % blockLength;
        output[i] = tmp[j];
	}
}

void lteTurboDeinterleave(fixed input[LTE_MAX_BLOCK_LEN], 
						  fixed output[LTE_MAX_BLOCK_LEN], 
						  int blockLengthIdx) 
{
/*************************
*  de-interleave a code block for LTE
*************************/
/* prevent overflow for indices (use 64-bit integers)*/
    long long i, j;
	long long blockLength = LTE_CODE_BLOCK_LENGTHS[blockLengthIdx];
	long long f1=LTE_INTERLEAVE_PARAM_F1[blockLengthIdx];
	long long f2=LTE_INTERLEAVE_PARAM_F2[blockLengthIdx];

	/* input and output can be identical -> copy input before writing output*/
	fixed tmp[LTE_MAX_BLOCK_LEN];
	memcpy(tmp, input, (unsigned int) blockLength*sizeof(fixed));

    for(i=0; i<blockLength; i++) {
        j = (f1*i + f2*i*i) % blockLength;
        output[j] = tmp[i];
	}
}

void umtsTurboInterleave(fixed input[UMTS_MAX_BLOCK_LEN], fixed output[UMTS_MAX_BLOCK_LEN], int blockLength) {
/*************************
*  interleave a code block for UMTS
*************************/
	int i, permutation[UMTS_MAX_BLOCK_LEN];

	/* input and output can be identical -> copy input before writing output*/
	fixed tmp[UMTS_MAX_BLOCK_LEN];
	memcpy(tmp, input, blockLength*sizeof(fixed));

	umtsInterleavePerm(permutation, blockLength);
    for(i=0; i<blockLength; i++) {
        output[i]=tmp[permutation[i]];
	}
}


void umtsTurboDeinterleave(fixed input[UMTS_MAX_BLOCK_LEN], fixed output[UMTS_MAX_BLOCK_LEN], int blockLength) {
/*************************
*  de-interleave a code block for UMTS
*************************/
	int i, permutation[UMTS_MAX_BLOCK_LEN];
	
	/* input and output can be identical -> copy input before writing output*/
	fixed tmp[UMTS_MAX_BLOCK_LEN];
	memcpy(tmp, input, blockLength*sizeof(fixed));

	umtsInterleavePerm(permutation, blockLength);
    for(i=0; i<blockLength; i++) {
        output[permutation[i]]=tmp[i];
	}
}

void umtsInterleavePerm(int permutation[UMTS_MAX_BLOCK_LEN], int blockLength) {
/*************************
*  compute the permutation pattern required for (de)interleaving for UMTS
*  this code is taken from TAU reference model provided by Frank Heinle
*************************/

  int R, l, C, p, nu;
  const int *T;
  int K = blockLength;

#pragma SET_DATA_SECTION("uerx_data")
  static int UU[20], mu[20];
  static int TT[20];
  int rck;
  int excludePrimeIndex;
  int i, j, k;

  static const INT16 primeTable[] = {
    7,  11,  13,  17,  19,  23,  29,  31,  37,  41,  43,
    47,  53,  59,  61,  67,  71,  73,  79,  83,  89,  97,
    101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151,
    157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211,
    223, 227, 229, 233, 239, 241, 251, 257
  };

  /* here you find the index of the prime to exclude ...
     needs 5 bits per entry
     31 is for indicating no exclusion
     note that for 239 we have two: 7, and 17; only 7 is in the table,
     17 must be treated as special case
  */
  static const UINT8 primeExcludeTable[] = {
    31, 31, 31, 31, 31, 1, 0, 31, 31, 31, 0,
    5, 2, 6, 31, 1, 0, 31, 2, 9, 1, 31,
    31, 3, 12, 31, 0, 0, 2, 3, 5, 8, 31,
    2, 31, 19, 10, 20, 31, 4, 31, 0, 1, 0,
    8, 26, 4, 6, 0, 31, 31, 31
  };

  static const UINT8 primitiveRootTable[] = {
    3, 2, 2, 3, 2, 5, 2, 3, 2, 6, 3,
    5, 2, 2, 2, 2, 7, 5, 3, 2, 3, 5,
    2, 5, 2, 6, 3, 3, 2, 3, 2, 2, 6,
    5, 2, 5, 2, 2, 2, 19, 5, 2, 3, 2,
    3, 2, 6, 3, 7, 7, 6, 3
  };

  static const int interRow0[5] = {
    4, 3, 2, 1, 0
  };
  static const int interRow1[10] = {
    9, 8, 7, 6, 5, 4, 3, 2, 1, 0
  };
  static const int interRow2[20] = {
    19, 9, 14, 4, 0, 2, 5, 7, 12, 18, 16, 13, 17, 15, 3, 1, 6, 11, 8, 10
  };
  static const int interRow3[20] = {
    19, 9, 14, 4, 0, 2, 5, 7, 12, 18, 10, 8, 13, 17, 3, 1, 16, 6, 15, 11
  };
#pragma SET_DATA_SECTION()

  /**********************
   * determine <R>, <l> *
   **********************/
  
  if (K <= 159) {
      R = 5;
      l = 0;
  } /* if */
  else if (K <= 200) {
      R = 10;
      l = 1;
  } /* else if */
  else if (K <= 480) {
      R = 20;
      l = 2;
  } /* else if */
  else if (K <= 530) {
      R = 10;
      l = 1;
  } /* else if */
  else {
      R = 20;
      l = 2;
  } /* else */

  /* we now have R=(1<<l)*5 = (1<<(l+2))+(1<<l) */



  /*****************
   * determine <T> *
   *****************/

  if (K <= 159) {
      T = interRow0;
  } /* if */
  else if (K <= 200 || (481 <= K && K <= 530)) {
      T = interRow1;
  } /* else if */
  else if ((2281 <= K && K <= 2480) || (3161 <= K && K <= 3210)) {
      T = interRow2;
  } /* else if */
  else {
      T = interRow3;
  } /* else */



  /*****************
   * determine <p> *
   *****************/
  {
      int left, right;

      left = 0;
      right = 51;

      do {
	  int temp;

	  j = (left + right) >> 1;

	  temp = (primeTable[j] + 1) << l;
	  temp += temp << 2;

	  if (K > temp)
	      left = j + 1;
	  else
	      right = j;

      } while (left < right);
      
      p = primeTable[left];
      nu = primitiveRootTable[left];
      excludePrimeIndex = primeExcludeTable[left];

  }



  /*****************
   * determine <C> *
   *****************/

  if (481 <= K && K <= 530) {

      C = p;

  } /* if */
  else {
      int temp;

      temp = (p - 1) << l;
      temp += (temp << 2);
      
      if (K <= temp)
	  C = p - 1;
      else {
	  
	  temp += R;
	  
	  if (K <= temp)
	      C = p;
	  else
	      C = p + 1;
	  
      } /* else */

  } /* else */



  /******************
   * determine <mu> *
   ******************/
  {
      int nu2;
      int rho[6];
      int q, qq;
      
      nu2 = mulMod(nu, nu, p);
      rho[0] = nu2;
      for (i = 1; i < 6; ++i) {
      
	  rho[i] = mulMod(nu2, rho[i - 1], p);
	  
      } /* for */

      j = 0;
      qq = 1;
      mu[0] = nu;
      for (i = 1; i < R; ++i) {

	  if (excludePrimeIndex == j) {
	      
	      ++j;
	      if (p == 239) { /* special case */

		  excludePrimeIndex = 3;  /* 17 */
	      
	      } /* if */
	  
	  } /* if */
      
	  q = primeTable[j];

	  mu[i] = mulMod(mu[i - 1], rho[((q - qq) >> 1) - 1], p);

	  qq = q;
      
	  ++j;
      
      } /* for */
  }

  
  /************************
   * determine <UU>, <TT> *
   ************************/

  for (i = 0; i < R; ++i) {

      UU[i] = 1;
      TT[i] = C * T[i];
      
  } /* for */



  {
      int temp;

      temp = C << l;
      temp += temp << 2;
      
      rck = (K == temp);
  }



  /*************
   * main loop *
   *************/

  k = 0;

  for (j = 0; j < C; ++j) {

      for (i = 0; i < R; ++i) {
	  int adr, U;

	  if (C == p) {

	      if (j == p - 1)
		  U = 0;
	      else
		  U = UU[i];

	  } /* if */
	  else if (C == p + 1) {

	      if (j == 0 && rck && i == 0) {
		  
		  U = p;
		  
	      } /* if */
	      else if (j == p - 1) {
		  
		  U = 0;
		  
	      } /* else if */
	      else if (j == p) {
		  
		  if (rck && i == 0)
		      U = 1;
		  else
		      U = p;
		  
	      } /* else if */
	      else {
		  
		  U = UU[i];
		  
	      } /* else */

	  } /* else if */
	  else {

	      U = UU[i] - 1;

	  } /* else */

	  adr = TT[i] + U;

	  if (adr < K) {

	      permutation[k] = (int) adr;
	      ++k;

	  } /* if */

	  /* important: <mu[i]> first is better as it fits into 8 bits */
	  UU[i] = mulMod(mu[i], UU[i], p);

      } /* for */

  } /* for */
}

static int mulMod(int a, int b, int p)
/*************************
* this function is only to support umtsInterleavePerm
* this code is taken from the TAU reference model provided by Frank Heinle
*************************/
{
  int y;

  /* we should have <a <= b> for faster execution, but this is not necessary */
  if (a & 1)
      y = b;
  else
      y = 0;
  
  for (;;) {
      
      a >>= 1;

      if (!a)
	  break;
      
      b <<= 1;
      if (b >= p)
	  b -= p;

      if (a & 1) {
	  
	  y += b;
	  if (y >= p)
	      y -= p;
	  
      } /* if */
      
  } /* for */

  return y;
  
} /* mulMod */


int blockLengthIdx(int blockLength) {
/*************************
* returns the index of a given code block length
*   - For LTE from above code length table (LTE_CODE_BLOCK_LENGTHS)
*   - For UMTS index is chosen from 40 ... 5114
*
* returns:
*    -1 if blockLength not found
*    index otherwise such that  LTE_CODE_BLOCK_LENGTHS[index] = blockLength
*
*************************/

	int blockLengthIdx=-1;

#ifdef TURBO_LTE
	int i;
    /* find index from code block table */
	for(i=0; i<LTE_MAX_BLOCK_LEN; i++) {
		if(blockLength == LTE_CODE_BLOCK_LENGTHS[i])
			blockLengthIdx=i;
	}
#else
	/* find index between 40...5114 */
	if(blockLength >= UMTS_MIN_BLOCK_LEN && blockLength <= UMTS_MAX_BLOCK_LEN) {
        blockLengthIdx = blockLength-UMTS_MIN_BLOCK_LEN;
	}

#endif /* TURBO_LTE */
	return blockLengthIdx;
}


fixed cal_max(fixed R[], int inlen)
{
	int i;
	int result = R[0];

	for (i = 1; i < inlen; i++)
	{
		result = (result > R[i]) ? result : R[i];
	}

	return result;
}



fixed maxVal(fixed a, fixed b)

//fixed maxVal(fixed a, fixed b)
/*************************
* max operation
* 
* returns:
*    a      if a>=b
*    b      otherwise
*
*************************/
{
    fixed res;
	
	/*  fixed x, y, z, w, result;
	
	  if(a>=b)
	  res=a;
	  else
	  res=b;
	  
		x = fpAdd(a, -b, &FP_FORMAT_ALPHA);
		
		  if(x < 0)
		  x = -x;
		  
			y = x / 2;
			
			  z = fpAdd(1, -y, &FP_FORMAT_ALPHA);
			  
				w = (z > 0) ? z : 0;
				
				  result = fpAdd(res, w, &FP_FORMAT_ALPHA);
				  
	return result;   */
	
    if(a >= b)
		res = a;
    else
		res = b;
	
    return res;
}



void turboAllocPsisoUnit(t_psiso_unit* psiso,    /* In/Out */
					     int numberOfPSISOs,        /* In */
					     int numberOfWindows,       /* In */
					     int windowLength,          /* In */
					     int windowLengthLast)      /* In */
/*************************
* creates a single parallel SISO unit
*    memory allocation
*
* returns:
*    none
*
*************************/
{
	int windowLengthMax, windowLengthMin, i;

	/* allocate memory for look-up of window lengths */
    psiso->window_lengths = (int*) malloc( numberOfWindows*sizeof(int) );
    //psiso->window_lengths = malloc(sizeof(int)*numberOfWindows);//new int[numberOfWindows];

	/* determine min./max. length of a single window within a parallel SISO */
	if(windowLength > windowLengthLast) {
        windowLengthMax = windowLength;
        windowLengthMin = windowLengthLast;
    } else {
        windowLengthMax = windowLengthLast;
        windowLengthMin = windowLength;
	}
	psiso->window_length_max = windowLengthMax;
	psiso->window_length_min = windowLengthMin;

	/* allocate memory for PSISO buffer */
	psiso->alpha_fifo = (fixed**) malloc( (windowLengthMax+1)*sizeof(fixed*) );
	//psiso->alpha_fifo = malloc(sizeof(fixed *)*windowLengthMax+1);//new fixed*[windowLengthMax+1];
	for(i=0; i<(windowLengthMax+1); i++) {
        psiso->alpha_fifo[i] = (fixed*) malloc( 8*sizeof(fixed) );
    //    psiso->alpha_fifo[i] = malloc(sizeof(fixed)*8);//new fixed[8];
	}

	/* allocate memory for initial/final states of beta (for both Half-Iteration)*/
	  psiso->beta_final_states_1  = (fixed**) malloc( numberOfWindows*sizeof(fixed*) );
	//psiso->beta_final_states_1  = malloc(sizeof(fixed *)*numberOfWindows);//new fixed*[numberOfWindows];
	  psiso->beta_final_states_2 =  (fixed**) malloc( numberOfWindows*sizeof(fixed*) );
	//psiso->beta_final_states_2	= malloc(sizeof(fixed *)*numberOfWindows);//new fixed*[numberOfWindows];
	for(i=0; i<numberOfWindows; i++) {
		psiso->beta_final_states_1[i]  = (fixed*) malloc( 8*sizeof(fixed) );
	//	psiso->beta_final_states_1[i]  = malloc(sizeof(fixed)*8);//new fixed[8];
		psiso->beta_final_states_2[i] = (fixed*) malloc( 8*sizeof(fixed) );
	//	psiso->beta_final_states_2[i]  = malloc(sizeof(fixed)*8);//new fixed[8];
	}
}

void turboFreePsisoUnit(t_psiso_unit* psiso)
/*************************
* destroys a single parallel SISO unit
*    frees memory
*
* returns:
*    none
*
*************************/
{
	int numberOfWindows = psiso->numberOfWindows;
	int windowLengthMax = psiso->window_length_max;
	int i;

	/* free memory of PSISO */
    for(i=0; i<numberOfWindows; i++) {
		//free((void*) psiso->beta_final_states_1[i]);
		//delete [] psiso->beta_final_states_1[i];
		free(psiso->beta_final_states_1[i]);
		//free((void*) psiso->beta_final_states_2[i]);
		free(psiso->beta_final_states_2[i]);
	}
	//free((void*) psiso->beta_final_states_1);
		free(psiso->beta_final_states_1);
	//free((void*) psiso->beta_final_states_2);
	    free(psiso->beta_final_states_2);

	for(i=0; i<(windowLengthMax+1); i++) {
		//free((void*) psiso->alpha_fifo[i]);
		free(psiso->alpha_fifo[i]);
	}
	//free((void*) psiso->alpha_fifo);
	free(psiso->alpha_fifo);
	//free((void*) psiso->window_lengths);
	free(psiso->window_lengths);
}

void turboResetPsisoUnit(t_psiso_unit* psiso,    /* In/Out */
					  int blockLengthIdx,        /* In */
					  int psisoLength,           /* In */
					  int numberOfWindows,       /* In */
					  int windowLength,          /* In */
					  int windowLengthLast,      /* In */
					  int psisoNo,               /* In */
					  int halfIteration)         /* In */
/*************************
* resets a single parallel SISO unit
*    parametrization
*    resets internal buffers
*
* returns:
*    PSISO unit
*
*************************/
{

	int windowLengthMax, windowLengthMin, i, j;


#ifdef TURBO_LTE
	int blockLength = LTE_CODE_BLOCK_LENGTHS[blockLengthIdx];
#else
    int blockLength = blockLengthIdx + UMTS_MIN_BLOCK_LEN;
#endif /* TURBO_LTE */

	psiso->psiso_length = psisoLength;
	psiso->psiso_no     = psisoNo;
	psiso->numberOfWindows = numberOfWindows;
	psiso->half_iteration  = halfIteration;

	/* (min./max.) length of a single window within a parallel SISO */
	windowLengthMin = psiso->window_length_min;
	windowLengthMax = psiso->window_length_max;

	/* set window lengths as given as parameters */
	for(i=0; i<(numberOfWindows-1); i++)
	{	psiso->window_lengths[i] = windowLength; }
	psiso->window_lengths[numberOfWindows-1] = windowLengthLast;
    

	/* reset alpha fifo */
	for(i=0; i<(windowLengthMax+1); i++) {
		for(j=0; j<8; j++)
			psiso->alpha_fifo[i][j] = (fixed) 0;
	}

	/* reset beta */
	for(i=0; i<8; i++) {
	    psiso->beta[i] = (fixed) 0;
	}

	/* reset final states of alpha */
	for(i=0; i<8; i++) {
        psiso->alpha_final_state_1_1[i] = (fixed) 0;
        psiso->alpha_final_state_2_1[i] = (fixed) 0;
        psiso->alpha_final_state_1_2[i] = (fixed) 0;
        psiso->alpha_final_state_2_2[i] = (fixed) 0;
	}


	/* reset final states of beta */
	for(i=0; i<numberOfWindows; i++) {
        for(j=0; j<8; j++) {
			psiso->beta_final_states_1[i][j] = (fixed) 0;
			psiso->beta_final_states_2[i][j] = (fixed) 0;
		}
	}
}


int checkSISOParams(int blockLength,                            /* In */
                    int numberOfPSISOs,                         /* In */
                    int PSISOLength,                            /* In */
                    int PSISOLengthLast,                        /* In */
                    int numberOfWindows,                        /* In */
                    int windowLength,                           /* In */
                    int lastWindowLengthPSISO,                  /* In */
                    int lastWindowLengthPSISOLast               /* In */
					)
/*************************
* checks the SISO parameters for consistency
*
*
* returns:
*    true or false
*
*************************/
{
	int retVal;
	retVal=1;

	/* check if codeBlockLength fits to number and length of the PSISOs */
    if(blockLength != PSISOLength*(numberOfPSISOs-1) + PSISOLengthLast) {
		retVal=0;
	}

	/* check if the window parameter fit to the PSISOs (except the last PSISO) */
	if(PSISOLength != windowLength*(numberOfWindows-1) + lastWindowLengthPSISO) {
		retVal=0;
	}

	/* check if the window parameter fit to the PSISOs (last PSISO) */
	if(PSISOLengthLast != windowLength*(numberOfWindows-1) + lastWindowLengthPSISOLast) {
		retVal=0;
	}

	return retVal;
}

