#include "bstx_blockconcate.h"



int bstx_blockconcate(int16 **inbuf, int *inlen_seg, int num_seg, int16 *outbuf)
{
	int i,d=0;

	for(i=0;i<num_seg;i++)
	{
		memcpy(outbuf+d,inbuf[i],sizeof(int16)*inlen_seg[i]);
		d=d+inlen_seg[i];
	}



	return d;
}
