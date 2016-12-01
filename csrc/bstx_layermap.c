#include "foo.h"
#include "bstx_layermap.h"
#include "bstx_define.h"



int layer_siso(cfloat *inbuf, int inlen, cfloat *outbuf)
{
	int i;

	for(i=0; i<inlen; i++)
	{
		outbuf[i] = inbuf[i];
	}

	return inlen;
}


int layer_diversity(cfloat *inbuf, int inlen, int num_layers, cfloat **outbuf)
{
	int i;
	int outlen;

	if (num_layers==2)
	{								
		for(i=0; i<inlen/2; i++)
		{
			outbuf[0][i] = inbuf[2*i+0];
			outbuf[1][i] = inbuf[2*i+1];
		}
						
		outlen = inlen/2;
	}
	else if (num_layers==4)
	{
		if(inlen%4 != 0)
		{
			outlen = (inlen+2)/4;
			
			for(i=0; i<(outlen-1); i++)
			{
				outbuf[0][i] = inbuf[4*i+0];
				outbuf[1][i] = inbuf[4*i+1];
				outbuf[2][i] = inbuf[4*i+2];
				outbuf[3][i] = inbuf[4*i+3];
			}

			// i = (outlen-1)
			outbuf[0][i] = inbuf[4*i+0];
			outbuf[1][i] = inbuf[4*i+1];
			outbuf[2][i] = ZEROCFLOAT;	// overhead of inbuf length
			outbuf[3][i] = ZEROCFLOAT;	// overhead of inbuf length
		}
		else
		{
			for(i=0; i<inlen/4; i++)
			{
				outbuf[0][i] = inbuf[4*i+0];
				outbuf[1][i] = inbuf[4*i+1];
				outbuf[2][i] = inbuf[4*i+2];
				outbuf[3][i] = inbuf[4*i+3];
			}

			outlen = inlen/4;
		}
	}

	return outlen;
}


int layer_spatial(cfloat **inbuf, int *inlen_cw, int num_layers, int num_codewords, cfloat **outbuf)
{
	int i;
	int inlen_cw0;
	int inlen_cw1;
	int outlen;

	inlen_cw0 = inlen_cw[0];
	inlen_cw1 = inlen_cw[1];

	if (num_layers==1 && num_codewords==1)
	{
		for(i=0; i<inlen_cw0; i++)
		{
			outbuf[0][i] = inbuf[0][i];
		}
		
		outlen = inlen_cw0;
	}
	else if (num_layers==2 && num_codewords==2)
	{
		assert(inlen_cw0 == inlen_cw1);

		for(i=0; i<inlen_cw0; i++)
		{
			outbuf[0][i] = inbuf[0][i];
			outbuf[1][i] = inbuf[1][i];
		}
		
		outlen = inlen_cw0;
	}
	else if (num_layers==2 && num_codewords==1)
	{								
		for(i=0; i<inlen_cw0/2; i++)
		{
			outbuf[0][i] = inbuf[0][2*i+0];
			outbuf[1][i] = inbuf[0][2*i+1];
		}
						
		outlen = inlen_cw0/2;
	}						  
	else if (num_layers==3 && num_codewords==2)
	{		
		assert(inlen_cw0 == inlen_cw1/2);

		for(i=0; i<inlen_cw0; i++)
		{
			outbuf[0][i] = inbuf[0][i];
			outbuf[1][i] = inbuf[1][2*i+0];
			outbuf[2][i] = inbuf[1][2*i+1];
		}
							  
		outlen = inlen_cw0;
	}
	else if (num_layers==4 && num_codewords==2)
	{		
		assert(inlen_cw0 == inlen_cw1);

		for(i=0; i<inlen_cw0/2; i++)
		{
			outbuf[0][i] = inbuf[0][2*i+0];
			outbuf[1][i] = inbuf[0][2*i+1];
			outbuf[2][i] = inbuf[1][2*i+0];
			outbuf[3][i] = inbuf[1][2*i+1];
		}
		
		outlen = inlen_cw0/2;
	}

	return outlen;
}


int layer_pcfich(cfloat *inbuf, int inlen, int num_layers, cfloat **outbuf)
{	
	int outlen;

	if(num_layers == 1)
	{
		outlen = layer_siso(inbuf, inlen, outbuf[0]);
	}
	else
	{
		outlen = layer_diversity(inbuf, inlen, num_layers, outbuf);
	}

	return outlen;
}

int layer_phich(cfloat *inbuf, int inlen, int num_layers, cfloat **outbuf)
{	
	int outlen;

	if(num_layers == 1)
	{
		outlen = layer_siso(inbuf, inlen, outbuf[0]);
	}
	else
	{
		outlen = layer_diversity(inbuf, inlen, num_layers, outbuf);
	}

	return outlen;
}

int layer_pbch(cfloat *inbuf, int inlen, int num_layers, cfloat **outbuf)
{	
	int outlen;

	if(num_layers == 1)
	{
		outlen = layer_siso(inbuf, inlen, outbuf[0]);
	}
	else
	{
		outlen = layer_diversity(inbuf, inlen, num_layers, outbuf);
	}

	return outlen;
}

int layer_pdcch(cfloat *inbuf, int inlen, int num_layers, cfloat **outbuf)
{	
	int outlen;

	if(num_layers == 1)
	{
		outlen = layer_siso(inbuf, inlen, outbuf[0]);
	}
	else
	{
		outlen = layer_diversity(inbuf, inlen, num_layers, outbuf);
	}

	return outlen;
}



int layer_pdsch(cfloat **inbuf, int *inlen_cw, int transmit_scheme, int num_layers
								, int num_codewords, cfloat **outbuf)
{
	int outlen;

	if(transmit_scheme == BSTX_SISO)
	{
		outlen = layer_siso(inbuf[0], inlen_cw[0], outbuf[0]);
	}
	else if(transmit_scheme == BSTX_DIVERSITY)
	{
		outlen = layer_diversity(inbuf[0], inlen_cw[0], num_layers, outbuf);
	}
	else
	{
		outlen = layer_spatial(inbuf, inlen_cw, num_layers, num_codewords, outbuf);
	}		

	return outlen;
}
