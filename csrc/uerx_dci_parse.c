#include "foo.h"
#include "uerx_define.h"
#include "uerx_dci_parse.h"

#pragma SET_DATA_SECTION("uerx_data")
int QAM_table[32][2] ={{2,0},	{2,1},	{2,2},	{2,3},
					   {2,4},	{2,5},	{2,6},	{2,7},
					   {2,8},	{2,9},	{4,9},	{4,10},
					   {4,11},	{4,12},	{4,13},	{4,14},
					   {4,15},	{6,15},	{6,16},	{6,17},
					   {6,18},	{6,19},	{6,20},	{6,21},
					   {6,22},	{6,23},	{6,24},	{6,25},
					   {6,26},	{2,30},	{4,30},	{6,30}};

int TB_table1[27][110]={{16,32,56,88,120,152,176,208,224,256,288,328,344,376,392,424,456,488,504,536,568,600,616,648,680,712,744,776,776,808,840,872,904,936,968,1000,1032,1032,1064,1096,1128,1160,1192,1224,1256,1256,1288,1320,1352,1384,1416,1416,1480,1480,1544,1544,1608,1608,1608,1672,1672,1736,1736,1800,1800,1800,1864,1864,1928,1928,1992,1992,2024,2088,2088,2088,2152,2152,2216,2216,2280,2280,2280,2344,2344,2408,2408,2472,2472,2536,2536,2536,2600,2600,2664,2664,2728,2728,2728,2792,2792,2856,2856,2856,2984,2984,2984,2984,2984,3112},
						{24,56,88,144,176,208,224,256,328,344,376,424,456,488,520,568,600,632,680,712,744,776,808,872,904,936,968,1000,1032,1064,1128,1160,1192,1224,1256,1288,1352,1384,1416,1416,1480,1544,1544,1608,1608,1672,1736,1736,1800,1800,1864,1864,1928,1992,1992,2024,2088,2088,2152,2152,2216,2280,2280,2344,2344,2408,2472,2472,2536,2536,2600,2600,2664,2728,2728,2792,2792,2856,2856,2856,2984,2984,2984,3112,3112,3112,3240,3240,3240,3240,3368,3368,3368,3496,3496,3496,3496,3624,3624,3624,3752,3752,3752,3752,3880,3880,3880,4008,4008,4008},
						{32,72,144,176,208,256,296,328,376,424,472,520,568,616,648,696,744,776,840,872,936,968,1000,1064,1096,1160,1192,1256,1288,1320,1384,1416,1480,1544,1544,1608,1672,1672,1736,1800,1800,1864,1928,1992,2024,2088,2088,2152,2216,2216,2280,2344,2344,2408,2472,2536,2536,2600,2664,2664,2728,2792,2856,2856,2856,2984,2984,3112,3112,3112,3240,3240,3240,3368,3368,3368,3496,3496,3496,3624,3624,3624,3752,3752,3880,3880,3880,4008,4008,4008,4136,4136,4136,4264,4264,4264,4392,4392,4392,4584,4584,4584,4584,4584,4776,4776,4776,4776,4968,4968},
						{40,104,176,208,256,328,392,440,504,568,616,680,744,808,872,904,968,1032,1096,1160,1224,1256,1320,1384,1416,1480,1544,1608,1672,1736,1800,1864,1928,1992,2024,2088,2152,2216,2280,2344,2408,2472,2536,2536,2600,2664,2728,2792,2856,2856,2984,2984,3112,3112,3240,3240,3368,3368,3496,3496,3624,3624,3624,3752,3752,3880,3880,4008,4008,4136,4136,4264,4264,4392,4392,4392,4584,4584,4584,4776,4776,4776,4776,4968,4968,4968,5160,5160,5160,5352,5352,5352,5352,5544,5544,5544,5736,5736,5736,5736,5992,5992,5992,5992,6200,6200,6200,6200,6456,6456},
						{56,120,208,256,328,408,488,552,632,696,776,840,904,1000,1064,1128,1192,1288,1352,1416,1480,1544,1608,1736,1800,1864,1928,1992,2088,2152,2216,2280,2344,2408,2472,2600,2664,2728,2792,2856,2984,2984,3112,3112,3240,3240,3368,3496,3496,3624,3624,3752,3752,3880,4008,4008,4136,4136,4264,4264,4392,4392,4584,4584,4584,4776,4776,4968,4968,4968,5160,5160,5160,5352,5352,5544,5544,5544,5736,5736,5736,5992,5992,5992,5992,6200,6200,6200,6456,6456,6456,6456,6712,6712,6712,6968,6968,6968,6968,7224,7224,7224,7480,7480,7480,7480,7736,7736,7736,7992},
						{72,144,224,328,424,504,600,680,776,872,968,1032,1128,1224,1320,1384,1480,1544,1672,1736,1864,1928,2024,2088,2216,2280,2344,2472,2536,2664,2728,2792,2856,2984,3112,3112,3240,3368,3496,3496,3624,3752,3752,3880,4008,4008,4136,4264,4392,4392,4584,4584,4776,4776,4776,4968,4968,5160,5160,5352,5352,5544,5544,5736,5736,5736,5992,5992,5992,6200,6200,6200,6456,6456,6712,6712,6712,6968,6968,6968,7224,7224,7224,7480,7480,7480,7736,7736,7736,7992,7992,7992,8248,8248,8248,8504,8504,8760,8760,8760,8760,9144,9144,9144,9144,9528,9528,9528,9528,9528},
						{328,176,256,392,504,600,712,808,936,1032,1128,1224,1352,1480,1544,1672,1736,1864,1992,2088,2216,2280,2408,2472,2600,2728,2792,2984,2984,3112,3240,3368,3496,3496,3624,3752,3880,4008,4136,4136,4264,4392,4584,4584,4776,4776,4968,4968,5160,5160,5352,5352,5544,5736,5736,5992,5992,5992,6200,6200,6456,6456,6456,6712,6712,6968,6968,6968,7224,7224,7480,7480,7736,7736,7736,7992,7992,8248,8248,8248,8504,8504,8760,8760,8760,9144,9144,9144,9144,9528,9528,9528,9528,9912,9912,9912,10296,10296,10296,10296,10680,10680,10680,10680,11064,11064,11064,11448,11448,11448},
						{104,224,328,472,584,712,840,968,1096,1224,1320,1480,1608,1672,1800,1928,2088,2216,2344,2472,2536,2664,2792,2984,3112,3240,3368,3368,3496,3624,3752,3880,4008,4136,4264,4392,4584,4584,4776,4968,4968,5160,5352,5352,5544,5736,5736,5992,5992,6200,6200,6456,6456,6712,6712,6712,6968,6968,7224,7224,7480,7480,7736,7736,7992,7992,8248,8248,8504,8504,8760,8760,8760,9144,9144,9144,9528,9528,9528,9912,9912,9912,10296,10296,10296,10680,10680,10680,11064,11064,11064,11448,11448,11448,11448,11832,11832,11832,12216,12216,12216,12576,12576,12576,12960,12960,12960,12960,13536,13536},
						{120,256,392,536,680,808,968,1096,1256,1384,1544,1672,1800,1928,2088,2216,2344,2536,2664,2792,2984,3112,3240,3368,3496,3624,3752,3880,4008,4264,4392,4584,4584,4776,4968,4968,5160,5352,5544,5544,5736,5992,5992,6200,6200,6456,6456,6712,6968,6968,7224,7224,7480,7480,7736,7736,7992,7992,8248,8504,8504,8760,8760,9144,9144,9144,9528,9528,9528,9912,9912,9912,10296,10296,10680,10680,10680,11064,11064,11064,11448,11448,11448,11832,11832,12216,12216,12216,12576,12576,12576,12960,12960,12960,13536,13536,13536,13536,14112,14112,14112,14112,14688,14688,14688,14688,15264,15264,15264,15264},
						{136,296,456,616,776,936,1096,1256,1416,1544,1736,1864,2024,2216,2344,2536,2664,2856,2984,3112,3368,3496,3624,3752,4008,4136,4264,4392,4584,4776,4968,5160,5160,5352,5544,5736,5736,5992,6200,6200,6456,6712,6712,6968,6968,7224,7480,7480,7736,7992,7992,8248,8248,8504,8760,8760,9144,9144,9144,9528,9528,9912,9912,10296,10296,10296,10680,10680,11064,11064,11064,11448,11448,11832,11832,11832,12216,12216,12576,12576,12960,12960,12960,13536,13536,13536,13536,14112,14112,14112,14112,14688,14688,14688,15264,15264,15264,15264,15840,15840,15840,16416,16416,16416,16416,16992,16992,16992,16992,17568},
						{144,328,504,680,872,1032,1224,1384,1544,1736,1928,2088,2280,2472,2664,2792,2984,3112,3368,3496,3752,3880,4008,4264,4392,4584,4776,4968,5160,5352,5544,5736,5736,5992,6200,6200,6456,6712,6712,6968,7224,7480,7480,7736,7992,7992,8248,8504,8504,8760,9144,9144,9144,9528,9528,9912,9912,10296,10296,10680,10680,11064,11064,11448,11448,11448,11832,11832,12216,12216,12576,12576,12960,12960,12960,13536,13536,13536,14112,14112,14112,14688,14688,14688,14688,15264,15264,15264,15840,15840,15840,16416,16416,16416,16992,16992,16992,16992,17568,17568,17568,18336,18336,18336,18336,18336,19080,19080,19080,19080},
						{176,376,584,776,1000,1192,1384,1608,1800,2024,2216,2408,2600,2792,2984,3240,3496,3624,3880,4008,4264,4392,4584,4776,4968,5352,5544,5736,5992,5992,6200,6456,6712,6968,6968,7224,7480,7736,7736,7992,8248,8504,8760,8760,9144,9144,9528,9528,9912,9912,10296,10680,10680,11064,11064,11448,11448,11832,11832,12216,12216,12576,12576,12960,12960,13536,13536,13536,14112,14112,14112,14688,14688,14688,15264,15264,15840,15840,15840,16416,16416,16416,16992,16992,16992,17568,17568,17568,18336,18336,18336,18336,19080,19080,19080,19080,19848,19848,19848,19848,20616,20616,20616,21384,21384,21384,21384,22152,22152,22152},
						{208,440,680,904,1128,1352,1608,1800,2024,2280,2472,2728,2984,3240,3368,3624,3880,4136,4392,4584,4776,4968,5352,5544,5736,5992,6200,6456,6712,6712,6968,7224,7480,7736,7992,8248,8504,8760,8760,9144,9528,9528,9912,9912,10296,10680,10680,11064,11064,11448,11832,11832,12216,12216,12576,12576,12960,12960,13536,13536,14112,14112,14112,14688,14688,15264,15264,15264,15840,15840,16416,16416,16416,16992,16992,17568,17568,17568,18336,18336,18336,19080,19080,19080,19080,19848,19848,19848,20616,20616,20616,21384,21384,21384,21384,22152,22152,22152,22920,22920,22920,23688,23688,23688,23688,24496,24496,24496,24496,25456},
						{224,488,744,1000,1256,1544,1800,2024,2280,2536,2856,3112,3368,3624,3880,4136,4392,4584,4968,5160,5352,5736,5992,6200,6456,6712,6968,7224,7480,7736,7992,8248,8504,8760,9144,9144,9528,9912,9912,10296,10680,10680,11064,11448,11448,11832,12216,12216,12576,12960,12960,13536,13536,14112,14112,14688,14688,14688,15264,15264,15840,15840,16416,16416,16992,16992,16992,17568,17568,18336,18336,18336,19080,19080,19080,19848,19848,19848,20616,20616,20616,21384,21384,21384,22152,22152,22152,22920,22920,22920,23688,23688,23688,24496,24496,24496,25456,25456,25456,25456,26416,26416,26416,26416,27376,27376,27376,27376,28336,28336},
						{256,552,840,1128,1416,1736,1992,2280,2600,2856,3112,3496,3752,4008,4264,4584,4968,5160,5544,5736,5992,6200,6456,6968,7224,7480,7736,7992,8248,8504,8760,9144,9528,9912,9912,10296,10680,11064,11064,11448,11832,12216,12216,12576,12960,12960,13536,13536,14112,14112,14688,14688,15264,15264,15840,15840,16416,16416,16992,16992,17568,17568,18336,18336,18336,19080,19080,19848,19848,19848,20616,20616,20616,21384,21384,22152,22152,22152,22920,22920,22920,23688,23688,24496,24496,24496,25456,25456,25456,25456,26416,26416,26416,27376,27376,27376,28336,28336,28336,28336,29296,29296,29296,29296,30576,30576,30576,30576,31704,31704},
						{280,600,904,1224,1544,1800,2152,2472,2728,3112,3368,3624,4008,4264,4584,4968,5160,5544,5736,6200,6456,6712,6968,7224,7736,7992,8248,8504,8760,9144,9528,9912,10296,10296,10680,11064,11448,11832,11832,12216,12576,12960,12960,13536,13536,14112,14688,14688,15264,15264,15840,15840,16416,16416,16992,16992,17568,17568,18336,18336,18336,19080,19080,19848,19848,20616,20616,20616,21384,21384,22152,22152,22152,22920,22920,23688,23688,23688,24496,24496,24496,25456,25456,25456,26416,26416,26416,27376,27376,27376,28336,28336,28336,29296,29296,29296,29296,30576,30576,30576,30576,31704,31704,31704,31704,32856,32856,32856,34008,34008},
						{328,632,968,1288,1608,1928,2280,2600,2984,3240,3624,3880,4264,4584,4968,5160,5544,5992,6200,6456,6712,7224,7480,7736,7992,8504,8760,9144,9528,9912,9912,10296,10680,11064,11448,11832,12216,12216,12576,12960,13536,13536,14112,14112,14688,14688,15264,15840,15840,16416,16416,16992,16992,17568,17568,18336,18336,19080,19080,19848,19848,19848,20616,20616,21384,21384,22152,22152,22152,22920,22920,23688,23688,24496,24496,24496,25456,25456,25456,26416,26416,26416,27376,27376,27376,28336,28336,28336,29296,29296,29296,30576,30576,30576,30576,31704,31704,31704,31704,32856,32856,32856,34008,34008,34008,34008,35160,35160,35160,35160},
						{336,696,1064,1416,1800,2152,2536,2856,3240,3624,4008,4392,4776,5160,5352,5736,6200,6456,6712,7224,7480,7992,8248,8760,9144,9528,9912,10296,10296,10680,11064,11448,11832,12216,12576,12960,13536,13536,14112,14688,14688,15264,15264,15840,16416,16416,16992,17568,17568,18336,18336,19080,19080,19848,19848,20616,20616,20616,21384,21384,22152,22152,22920,22920,23688,23688,24496,24496,24496,25456,25456,26416,26416,26416,27376,27376,27376,28336,28336,29296,29296,29296,30576,30576,30576,30576,31704,31704,31704,32856,32856,32856,34008,34008,34008,35160,35160,35160,35160,36696,36696,36696,36696,37888,37888,37888,39232,39232,39232,39232},
						{376,776,1160,1544,1992,2344,2792,3112,3624,4008,4392,4776,5160,5544,5992,6200,6712,7224,7480,7992,8248,8760,9144,9528,9912,10296,10680,11064,11448,11832,12216,12576,12960,13536,14112,14112,14688,15264,15264,15840,16416,16416,16992,17568,17568,18336,18336,19080,19080,19848,19848,20616,21384,21384,22152,22152,22920,22920,23688,23688,24496,24496,24496,25456,25456,26416,26416,27376,27376,27376,28336,28336,29296,29296,29296,30576,30576,30576,31704,31704,31704,32856,32856,32856,34008,34008,34008,35160,35160,35160,36696,36696,36696,37888,37888,37888,37888,39232,39232,39232,40576,40576,40576,40576,42368,42368,42368,42368,43816,43816},
						{408,840,1288,1736,2152,2600,2984,3496,3880,4264,4776,5160,5544,5992,6456,6968,7224,7736,8248,8504,9144,9528,9912,10296,10680,11064,11448,12216,12576,12960,13536,13536,14112,14688,15264,15264,15840,16416,16992,16992,17568,18336,18336,19080,19080,19848,20616,20616,21384,21384,22152,22152,22920,22920,23688,24496,24496,25456,25456,25456,26416,26416,27376,27376,28336,28336,29296,29296,29296,30576,30576,30576,31704,31704,32856,32856,32856,34008,34008,34008,35160,35160,35160,36696,36696,36696,37888,37888,37888,39232,39232,39232,40576,40576,40576,40576,42368,42368,42368,43816,43816,43816,43816,45352,45352,45352,46888,46888,46888,46888},
						{440,904,1384,1864,2344,2792,3240,3752,4136,4584,5160,5544,5992,6456,6968,7480,7992,8248,8760,9144,9912,10296,10680,11064,11448,12216,12576,12960,13536,14112,14688,14688,15264,15840,16416,16992,16992,17568,18336,18336,19080,19848,19848,20616,20616,21384,22152,22152,22920,22920,23688,24496,24496,25456,25456,26416,26416,27376,27376,28336,28336,29296,29296,29296,30576,30576,31704,31704,31704,32856,32856,34008,34008,34008,35160,35160,35160,36696,36696,36696,37888,37888,39232,39232,39232,40576,40576,40576,42368,42368,42368,42368,43816,43816,43816,45352,45352,45352,46888,46888,46888,46888,48936,48936,48936,48936,48936,51024,51024,51024},
						{488,1000,1480,1992,2472,2984,3496,4008,4584,4968,5544,5992,6456,6968,7480,7992,8504,9144,9528,9912,10680,11064,11448,12216,12576,12960,13536,14112,14688,15264,15840,15840,16416,16992,17568,18336,18336,19080,19848,19848,20616,21384,21384,22152,22920,22920,23688,24496,24496,25456,25456,26416,26416,27376,27376,28336,28336,29296,29296,30576,30576,31704,31704,31704,32856,32856,34008,34008,35160,35160,35160,36696,36696,36696,37888,37888,39232,39232,39232,40576,40576,40576,42368,42368,42368,43816,43816,43816,45352,45352,45352,46888,46888,46888,46888,48936,48936,48936,48936,51024,51024,51024,51024,52752,52752,52752,52752,55056,55056,55056},
						{520,1064,1608,2152,2664,3240,3752,4264,4776,5352,5992,6456,6968,7480,7992,8504,9144,9528,10296,10680,11448,11832,12576,12960,13536,14112,14688,15264,15840,16416,16992,16992,17568,18336,19080,19080,19848,20616,21384,21384,22152,22920,22920,23688,24496,24496,25456,25456,26416,27376,27376,28336,28336,29296,29296,30576,30576,31704,31704,32856,32856,34008,34008,34008,35160,35160,36696,36696,36696,37888,37888,39232,39232,40576,40576,40576,42368,42368,42368,43816,43816,43816,45352,45352,45352,46888,46888,46888,48936,48936,48936,48936,51024,51024,51024,51024,52752,52752,52752,55056,55056,55056,55056,57336,57336,57336,57336,59256,59256,59256},
						{552,1128,1736,2280,2856,3496,4008,4584,5160,5736,6200,6968,7480,7992,8504,9144,9912,10296,11064,11448,12216,12576,12960,13536,14112,14688,15264,15840,16416,16992,17568,18336,19080,19848,19848,20616,21384,22152,22152,22920,23688,24496,24496,25456,25456,26416,27376,27376,28336,28336,29296,29296,30576,30576,31704,31704,32856,32856,34008,34008,35160,35160,36696,36696,37888,37888,37888,39232,39232,40576,40576,40576,42368,42368,43816,43816,43816,45352,45352,45352,46888,46888,46888,48936,48936,48936,51024,51024,51024,51024,52752,52752,52752,55056,55056,55056,55056,57336,57336,57336,57336,59256,59256,59256,59256,61664,61664,61664,61664,63776},
						{584,1192,1800,2408,2984,3624,4264,4968,5544,5992,6712,7224,7992,8504,9144,9912,10296,11064,11448,12216,12960,13536,14112,14688,15264,15840,16416,16992,17568,18336,19080,19848,19848,20616,21384,22152,22920,22920,23688,24496,25456,25456,26416,26416,27376,28336,28336,29296,29296,30576,31704,31704,32856,32856,34008,34008,35160,35160,36696,36696,36696,37888,37888,39232,39232,40576,40576,42368,42368,42368,43816,43816,45352,45352,45352,46888,46888,46888,48936,48936,48936,51024,51024,51024,52752,52752,52752,52752,55056,55056,55056,57336,57336,57336,57336,59256,59256,59256,61664,61664,61664,61664,63776,63776,63776,63776,66592,66592,66592,66592},
						{616,1256,1864,2536,3112,3752,4392,5160,5736,6200,6968,7480,8248,8760,9528,10296,10680,11448,12216,12576,13536,14112,14688,15264,15840,16416,16992,17568,18336,19080,19848,20616,20616,21384,22152,22920,23688,24496,24496,25456,26416,26416,27376,28336,28336,29296,29296,30576,31704,31704,32856,32856,34008,34008,35160,35160,36696,36696,37888,37888,39232,39232,40576,40576,40576,42368,42368,43816,43816,43816,45352,45352,46888,46888,46888,48936,48936,48936,51024,51024,51024,52752,52752,52752,55056,55056,55056,55056,57336,57336,57336,59256,59256,59256,61664,61664,61664,61664,63776,63776,63776,63776,66592,66592,66592,66592,68808,68808,68808,71112},
						{712,1480,2216,2984,3752,4392,5160,5992,6712,7480,8248,8760,9528,10296,11064,11832,12576,13536,14112,14688,15264,16416,16992,17568,18336,19080,19848,20616,21384,22152,22920,23688,24496,25456,25456,26416,27376,28336,29296,29296,30576,30576,31704,32856,32856,34008,35160,35160,36696,36696,37888,37888,39232,40576,40576,40576,42368,42368,43816,43816,45352,45352,46888,46888,48936,48936,48936,51024,51024,52752,52752,52752,55056,55056,55056,55056,57336,57336,57336,59256,59256,59256,61664,61664,61664,63776,63776,63776,66592,66592,66592,68808,68808,68808,71112,71112,71112,73712,73712,75376,75376,75376,75376,75376,75376,75376,75376,75376,75376,75376}};

int TB_table2[28][8]={{1544,3112,3752,7480,10296,20616,28336,57336},
					  {1608,3240,3880,7736,10680,21384,29296,59256},
					  {1672,3368,4008,7992,11064,22152,30576,61664},
					  {1736,3496,4136,8248,11448,22920,31704,63776},
					  {1800,3624,4264,8504,11832,23688,32856,66592},
					  {1864,3752,4392,8760,12216,24496,34008,68808},
					  {1928,3880,4584,9144,12576,25456,35160,71112},
					  {1992,4008,4776,9528,12960,25456,36696,73712},
					  {2024,4008,4968,9912,13536,27376,37888,76208},
					  {2088,4136,5160,10296,14112,28336,39232,78704},
					  {2152,4264,5352,10680,14688,29296,40576,81176},
					  {2216,4392,5544,11064,15264,30576,42368,84760},
					  {2280,4584,5736,11448,15840,31704,43816,87936},
					  {2344,4776,5992,11832,16416,32856,45352,90816},
					  {2408,4776,6200,12576,16992,34008,46888,93800},
					  {2472,4968,6456,12960,17568,35160,48936,97896},
					  {2536,5160,6712,13536,18336,36696,51024,101840},
					  {2600,5160,6968,14112,19080,37888,52752,105528},
					  {2664,5352,7224,14688,19848,39232,55056,110136},
					  {2728,5544,7480,14688,20616,40576,57336,115040},
					  {2792,5544,7736,15264,21384,42368,59256,119816},
					  {2856,5736,7992,15840,22152,43816,61664,124464},
					  {2984,5992,8248,16416,22920,45352,63776,128496},
					  {3112,6200,8504,16992,23688,46888,66592,133208},
					  {3240,6456,8760,17568,24496,48936,68808,137792},
					  {3368,6712,9144,18336,25456,51024,71112,142248},
					  {3496,6968,9528,19080,26416,52752,73712,146856},
					  {3624,7224,9912,19848,27376,55056,75376,149776}};
#pragma SET_DATA_SECTION()

int dci_Init(uerx_dci_parse  *uerx_dciparse_u)
{
	int i;

	(*uerx_dciparse_u).QAM_TBSidx_table = (int**)malloc(sizeof(int*)*32);//new int* [32];
	for (i = 0; i < 32; i ++)
	{
		(*uerx_dciparse_u).QAM_TBSidx_table[i]	= (int*)malloc(sizeof(int)*32);//new int[2];
		memcpy((*uerx_dciparse_u).QAM_TBSidx_table[i],QAM_table[i],2*sizeof(int));
	}

//	dci.TBS_table1 = new int *[27];
	(*uerx_dciparse_u).TBS_table1 = malloc(sizeof(int *)*27);
	for (i = 0; i < 27; i ++)
	{
//		dci.TBS_table1[i]	= new int[110];
		(*uerx_dciparse_u).TBS_table1[i]	= malloc(sizeof(int)*110);
		memcpy((*uerx_dciparse_u).TBS_table1[i],TB_table1[i],110*sizeof(int));
	}

	(*uerx_dciparse_u).TBS_table2 = malloc(sizeof(int *)*28);
//	dci.TBS_table2 = new int *[28];
	for (i = 0; i < 28; i ++)
	{
		(*uerx_dciparse_u).TBS_table2[i]	= malloc(sizeof(int)*8);
//		dci.TBS_table2[i]	= new int[8];
		memcpy((*uerx_dciparse_u).TBS_table2[i],TB_table2[i],8*4);
	}

	(*uerx_dciparse_u).TBS_last[0] = 1800;
	(*uerx_dciparse_u).TBS_last[1] = -1;

	return 0;
}

int dci_Clear(uerx_dci_parse  *uerx_dciparse_u)
{
	int i;
	
	
	for (i = 0; i < 32; i ++)
	{
		//delete [] dci.QAM_TBSidx_table[i];
		free((*uerx_dciparse_u).QAM_TBSidx_table[i]);
	}
	//delete [] dci.QAM_TBSidx_table;
	free((*uerx_dciparse_u).QAM_TBSidx_table);
	
	
	for (i = 0; i < 27; i ++)
	{
	//	delete [] dci.TBS_table1[i];
		free((*uerx_dciparse_u).TBS_table1[i]);
	}
	//  delete [] dci.TBS_table1;
	free((*uerx_dciparse_u).TBS_table1);
	
	for (i = 0; i < 28; i ++)
	{
	//	delete [] dci.TBS_table2[i];
		free((*uerx_dciparse_u).TBS_table2[i]);
	}
	//  delete [] dci.TBS_table2;
	free((*uerx_dciparse_u).TBS_table2);
	
	
	return 0;
}
/************************************************************************/
/*                                 */
/************************************************************************/

int dciparse(uerx_dci_parse  *uerx_dciparse_u,int *DCI_format, int16 N_DL_RB, int16 N_RB_UL
								 ,  int16 mux_mode, int16 RNTI_type
								 , int16 transmission_mode, int16 num_bstx_ant
								 , int16 *inbuf, int inlen,  int16 RI_last,	int16 PMI_last
								 , int16 * codeword_num, int16 *diversity_spatial
								 , int16 *layers_total, int16 layer_per_codeword[2]
								 , int16 RB_arr[2][110], int16 *pdsch_rb_alloc
								 , int16 *HARQ_process_idx, int16 NDI[2], int16 RV[2]
								 , int TBS[2], int16 qm[2], int16 *TPC_PUCCH , int16 *DAI
								 , int16 *codebook_idx, int16 *cobook_column_idx,int Preamble_idx,int PRACH_Mask_idx)
{
	int i;

	if (*DCI_format == UERX_DCI_1A || *DCI_format == UERX_DCI_0)
	{
		*DCI_format = (inbuf[0] == 0)? UERX_DCI_0 : UERX_DCI_1A;
	}
	(*uerx_dciparse_u).DCI_type = *DCI_format;
	
	switch ((*uerx_dciparse_u).DCI_type)
	{
// 	case UERX_DCI_0:
// 		{
// 			DCI0_parser( N_RB_UL, inbuf,  pinf);
// 			
// 		}
// 		break;
	case UERX_DCI_1:
		{
			DCI1_parser( uerx_dciparse_u,inbuf,  N_DL_RB, mux_mode, RNTI_type, transmission_mode, num_bstx_ant,
				 codeword_num,  diversity_spatial,  layers_total,  layer_per_codeword ,
				 RB_arr , pdsch_rb_alloc, HARQ_process_idx, NDI, RV,  TBS, qm,  TPC_PUCCH , DAI);
		}
		break;
 	case UERX_DCI_1A:
		{
		//DCI1A_parser( N_DL_RB, inbuf,  sUerx_inf_u /*pinf*/);
		DCI1A_parser(uerx_dciparse_u, inbuf, N_DL_RB, mux_mode, RNTI_type,transmission_mode,
						num_bstx_ant,  RI_last,	 PMI_last,
						 codeword_num,  diversity_spatial,  layers_total,
						 layer_per_codeword,  RB_arr, pdsch_rb_alloc,
						 HARQ_process_idx, NDI ,  RV,
						 TBS ,  qm ,  TPC_PUCCH ,  DAI,  codebook_idx, cobook_column_idx, inlen, Preamble_idx, PRACH_Mask_idx);
		}
	break;
	case UERX_DCI_2:
		{
			DCI2_2A_parser(uerx_dciparse_u,UERX_DCI_2, inbuf, N_DL_RB, mux_mode, RNTI_type,transmission_mode,
				num_bstx_ant,  RI_last,	 PMI_last,
				 codeword_num,  diversity_spatial,  layers_total,
				 layer_per_codeword,  RB_arr, pdsch_rb_alloc,
				 HARQ_process_idx, NDI ,  RV,
				 TBS ,  qm ,  TPC_PUCCH ,  DAI,  codebook_idx,  cobook_column_idx);
			for (i = 0; i < 110; i++)
			{
				if (RB_arr[0][i] == i)
				{
					RB_arr[0][i] = 1;
					RB_arr[1][i] = 1;
				}
				else
				{
					RB_arr[0][i] = 0;
					RB_arr[1][i] = 0;
				}
			}
		}
		
		break;
	case UERX_DCI_2A:
		{
			DCI2_2A_parser(uerx_dciparse_u,UERX_DCI_2A, inbuf, N_DL_RB, mux_mode, RNTI_type,transmission_mode,
				num_bstx_ant,  RI_last,	 PMI_last,
				codeword_num,  diversity_spatial,  layers_total,
				layer_per_codeword,  RB_arr, pdsch_rb_alloc, 
				HARQ_process_idx, NDI ,  RV,
				 TBS ,  qm ,  TPC_PUCCH ,  DAI,  codebook_idx,  cobook_column_idx);			
		}
// 		
// 		break;
	default:
		;
	}
	
	return 0;
	
}

int /*uerx_dci_parse::*/Get_pdsch_gap( uerx_dci_parse  *uerx_dciparse_u,int16 N_DL_RB )
{
//	extern uerx_dci_parse          uerx_dciparse_u;
	if (N_DL_RB >= 6 && N_DL_RB <= 49)
		(*uerx_dciparse_u).N_step_RB = 2;
	else if (N_DL_RB >= 50 && N_DL_RB <= 110)
		(*uerx_dciparse_u).N_step_RB = 4;
	
	if (N_DL_RB >= 6 && N_DL_RB <= 10)
		(*uerx_dciparse_u).N_gap1 = (int)(ceil(N_DL_RB / 2));
	else if (N_DL_RB == 11)
		(*uerx_dciparse_u).N_gap1 = 4;
	else if (N_DL_RB >= 12 && N_DL_RB <= 19)
		(*uerx_dciparse_u).N_gap1 = 8;
	else if (N_DL_RB >= 20 && N_DL_RB <= 26)
		(*uerx_dciparse_u).N_gap1 = 12;
	else if (N_DL_RB >= 27 && N_DL_RB <= 44)
		(*uerx_dciparse_u).N_gap1 = 18;
	else if (N_DL_RB >= 45 && N_DL_RB <= 49)
		(*uerx_dciparse_u).N_gap1 = 27;
	else if (N_DL_RB >= 50 && N_DL_RB <= 63)
	{
		(*uerx_dciparse_u).N_gap1 = 27;
		(*uerx_dciparse_u).N_gap2 = 9;
	}
	else if (N_DL_RB >= 64 && N_DL_RB <= 79)
	{
		(*uerx_dciparse_u).N_gap1 = 32;
		(*uerx_dciparse_u).N_gap2 = 16;
	}
	else if (N_DL_RB >= 80 && N_DL_RB <= 110)
	{
		(*uerx_dciparse_u).N_gap1 = 48;
		(*uerx_dciparse_u).N_gap2 = 16;
	}
	
	return 0 ;
	
}

int uerx_GetDCIlen(uerx_dci_parse *uerx_dciparse_u,int16 DCI_format, int16 N_RB_UL, int16 N_RB_DL, int16 N_TX_ant)
{
	int flag = 0;
//	extern uerx_dci_parse          uerx_dciparse_u;
	int type2_len_DL = (int)( ceil( log10((N_RB_DL + 1.0) * N_RB_DL / 2) / log10(2.0)));
	int type2_len_UL = (int)( ceil( log10((N_RB_UL + 1.0) * N_RB_UL / 2) / log10(2.0)));
	int size[10] = {12, 14, 16, 20, 24, 26, 32, 40, 44, 56};
	int i;
    int outlen;
	
	Get_pdsch_gap(uerx_dciparse_u,N_RB_DL);
	
	(*uerx_dciparse_u).P = (N_RB_DL <= 10)? 1
		:(N_RB_DL >= 11 && N_RB_DL <= 26)?  2
		:(N_RB_DL >= 27 && N_RB_DL <= 63)?  3
		:(N_RB_DL >= 64 && N_RB_DL <= 110)? 4: 0;
	(*uerx_dciparse_u).first_call_N_gap	= 1;
	
	int type01_len = (int)( ceil( (N_RB_DL + 0.0) / (*uerx_dciparse_u).P));     // 17
	
	(*uerx_dciparse_u).DCI_0_len	= 1 + 1 + type2_len_UL + 5 + 1 + 2 + 3 + 2 +1;
	(*uerx_dciparse_u).DCI_1_len	= type01_len + 5 + 4 + 1 + 2 + 2 + 2;
	(*uerx_dciparse_u).DCI_1A_len	= 1 + 1 + type2_len_DL + 5 + 4 + 1 + 2 + 2 + 2;
	(*uerx_dciparse_u).DCI_1B_len	= 1 + type2_len_DL + 5 + 4 + 1 + 2 + 2 + 1;
	(*uerx_dciparse_u).DCI_1B_len  = (N_TX_ant == 2) ? ((*uerx_dciparse_u).DCI_1B_len + 2) : ((*uerx_dciparse_u).DCI_1B_len + 4);
	(*uerx_dciparse_u).DCI_1C_len	= 13;
	
	if(N_TX_ant == 2)
	{
		(*uerx_dciparse_u).DCI_2_len	= type01_len + 2 + 2+ 4 + 1 + 5 + 1 + 2 + 5 + 1 + 2 + 3;
		(*uerx_dciparse_u).DCI_2A_len	= type01_len + 2 + 2+ 4 + 1 + 5 + 1 + 2 + 5 + 1 + 2 + 0;
	}
	else if(N_TX_ant == 4)
	{
		(*uerx_dciparse_u).DCI_2_len	= type01_len + 2 + 2+ 4 + 1 + 5 + 1 + 2 + 5 + 1 + 2 + 6;
		(*uerx_dciparse_u).DCI_2A_len	= type01_len + 2 + 2+ 4 + 1 + 5 + 1 + 2 + 5 + 1 + 2 + 2;
	}
	else
	{
		(*uerx_dciparse_u).DCI_2_len = 0;
		(*uerx_dciparse_u).DCI_2A_len = 0;
	}
	
	if (N_RB_DL > 10)
	{
		(*uerx_dciparse_u).DCI_1_len  += 1;
		(*uerx_dciparse_u).DCI_2_len  += 1;
		(*uerx_dciparse_u).DCI_2A_len += 1;
	}
	
	if ((*uerx_dciparse_u).DCI_1A_len < (*uerx_dciparse_u).DCI_0_len)
	{
		(*uerx_dciparse_u).DCI_1A_len = (*uerx_dciparse_u).DCI_0_len;
	}
	
	for( i = 0; i < 10; i++)
	{
		if((*uerx_dciparse_u).DCI_1A_len == size[i])
		{
			flag = 1;
			break;
		}
		else
			flag = 0;
	}
	if (flag == 1)
	{
		(*uerx_dciparse_u).DCI_1A_len = (*uerx_dciparse_u).DCI_1A_len + 1;
	}
	
	if((*uerx_dciparse_u).DCI_0_len < (*uerx_dciparse_u).DCI_1A_len)
	{
		(*uerx_dciparse_u).DCI_0_len = (*uerx_dciparse_u).DCI_1A_len;
	}
	
	if ((*uerx_dciparse_u).DCI_0_len != (*uerx_dciparse_u).DCI_1A_len)
	{
//		PrintInfo(1,"DCI_0_len != DCI_1A_len");
	}
	if ((*uerx_dciparse_u).DCI_1_len == (*uerx_dciparse_u).DCI_0_len)
	{
		(*uerx_dciparse_u).DCI_1_len = (*uerx_dciparse_u).DCI_1_len + 1;
	}
	
	for( i = 0; i < 10; i++)
	{
		if((*uerx_dciparse_u).DCI_1_len == size[i])
		{
			flag = 1;
			break;
		}
		else
			flag = 0;
	}
	
	if (flag == 1)
	{
		(*uerx_dciparse_u).DCI_1_len = (*uerx_dciparse_u).DCI_1_len + 1;
	}
	
    outlen = (DCI_format == UERX_DCI_0)?  (*uerx_dciparse_u).DCI_0_len:
	(DCI_format == UERX_DCI_1)?  (*uerx_dciparse_u).DCI_1_len:
	(DCI_format == UERX_DCI_1A)? (*uerx_dciparse_u).DCI_1A_len:
	(DCI_format == UERX_DCI_2)?  (*uerx_dciparse_u).DCI_2_len:
	(DCI_format == UERX_DCI_2A)? (*uerx_dciparse_u).DCI_2A_len:
	(DCI_format == UERX_DCI_1C)? 13: 0;
   	return outlen;
}


// int uerx_dci_parse::DCI0_parser(int N_RB_UL, int16 *inbuf, UERX_inf *pinf)
// {
// 	pinf->re_allocation_type	= 2;
// 	int cur_len					= 1;
// 	int mux_mode				= pinf->mux_mode;
//     int HARQ_proc_bitnum		= (mux_mode == TDD_muxmode)? 4: 3;
// 	
// 	int len	= int(ceil( log10((N_RB_UL + 1.0) * N_RB_UL / 2) / log10(2.0))); // 11 for N_DL_RB = 50
// 	
// 	pinf->pusch_hopping_flag	= inbuf[cur_len];
// 	cur_len						++;
// 	
// 	memcpy(pinf->RB_assig_inf_arr,	&inbuf[cur_len], len * sizeof(int16));
// 	cur_len						+= len; 
// 	
// 	memcpy(pinf->MCS_arr,			&inbuf[cur_len], 5 * sizeof(int16));
// 	cur_len						+= 5;
// 	int MCS_idx					= bit2dec(pinf->MCS_arr, 5, UERX_LOWERBITS_FIRST);
// 	
// 	pinf->NDI[0]				=	inbuf[cur_len];
// 	cur_len						++;
// 	
// 	memcpy(pinf->TPC_PUSCH_arr,		&inbuf[cur_len], 2 * sizeof(int16));
// 	cur_len						+= 2;
// 	pinf->TPC_PUSCH				=bit2dec(pinf->TPC_PUSCH_arr, 2, UERX_LOWERBITS_FIRST);
// 	
// 	memcpy(pinf->cyclicshift_DMRS_arr,	&inbuf[cur_len], 3 * sizeof(int16));
// 	cur_len						+= 3;
// 	pinf->cyclicshift_DMRS		=bit2dec(pinf->cyclicshift_DMRS_arr, 3, UERX_LOWERBITS_FIRST);
// 	
// 	PrintInfo(1,"UERX_DCI_0 : Re_allocation_type\t= %d \n", pinf->re_allocation_type);
// 	PrintInfo(1,"UERX_DCI_0 : MCS		\t= %d  \n", MCS_idx);
// 	PrintInfo(1,"UERX_DCI_0 : NDI		\t= %d  \n", pinf->NDI[0]);
// 	PrintInfo(1,"UERX_DCI_0 : TPC_PUSCH	\t= %d  \n", pinf->TPC_PUSCH);
// 	PrintInfo(1,"UERX_DCI_0 : DMRS		\t= %d  \n", pinf->cyclicshift_DMRS);
// 	
// 	if (pinf->UL_DL_switch_config == 0)
// 	{
// 		memcpy(pinf->UL_index_arr,	&inbuf[cur_len], 2 * sizeof(int16));
// 		cur_len						+= 2;
// 		pinf->UL_index				=bit2dec(pinf->UL_index_arr, 2, UERX_LOWERBITS_FIRST);
// 		PrintInfo(1,"UERX_DCI_0 : UL_index	\t= %d  \n", pinf->UL_index);
// 	}
// 	else
// 	{
// 		memcpy(pinf->DAI_arr	,	&inbuf[cur_len], 2 * sizeof(int16));
// 		cur_len						+= 2;
// 		pinf->DAI					=bit2dec(pinf->DAI_arr, 2, UERX_LOWERBITS_FIRST);
// 		PrintInfo(1,"UERX_DCI_0 : DAI		\t= %d  \n", pinf->DAI);
// 	}
// 	
// 	pinf->CQI_request			= inbuf[cur_len];
// 	cur_len++; 
// 	
// 	PrintInfo(1,"UERX_DCI_0 : len		\t= %d  \n", cur_len);
// 	
// 	return 0;
// } 


// int uerx_dci_parse::DCI1_parser(int N_DL_RB, int16 *inbuf, UERX_inf *pinf)
int DCI1_parser(uerx_dci_parse  *uerx_dciparse_u,
	int16 *inbuf,
	int16 N_DL_RB,
	int16 mux_mode,
	int16 RNTI_type,
	int16 transmission_mode,
	int16 num_bstx_ant,
	int16 * codeword_num,
	int16 *diversity_spatial,
	int16 *layers_total,
	int16 layer_per_codeword[2],
	int16 RB_arr[2][110],
	int16 *pdsch_rb_alloc,
	int16 *HARQ_process_idx,
	int16 NDI[2],
	int16 RV[2],
	int32 TBS[2],
	int16 qm[2],
	int16 *TPC_PUCCH,
	int16 *DAI)
{
	int cur_len, P;
	int16 RB_assig_inf_arr[30];
	int16 MCS_arr[5];
	int16 HARQ_process_number_arr[4];
	int16 RV_arr[2];
	int16 TPC_PUCCH_arr[2];
	int16 DAI_arr[2];
    int HARQ_proc_bitnum = (mux_mode == UERX_TDD_MUXMODE)? 4: 3;
	int MCS_flag;
	int16 re_allocation_type;

	cur_len = 0;
	*codeword_num = 1;
	int16 Local_Distrib_VRB_flag = 0;

   	if (RNTI_type == UERX_C_RNTI || RNTI_type == UERX_SPSC_RNTI )
	{
		if ( transmission_mode == UERX_TM_MODE1)
		{
			*diversity_spatial	=  UERX_SISO_PORT0_PRECOD;
		}
		else if  (transmission_mode == UERX_TM_MODE2)
		{
			*diversity_spatial	=  UERX_SFBC_PRECOD;
		}
		else if ( transmission_mode == UERX_TM_MODE7)
		{
			*diversity_spatial	=  UERX_SISO_PORT5_PRECOD;
		}
	}
	else if ( RNTI_type == UERX_TC_RNTI)
	{
		 *diversity_spatial	=  ( num_bstx_ant == 1)? UERX_SISO_PRECOD: UERX_SFBC_PRECOD;
	}
	else
	{
//		PrintInfo(1, "NO DCI 1 when RNTI_ type = %d", RNTI_type);
		return -1;
	}
 
	*layers_total					= num_bstx_ant;
	layer_per_codeword[0]			= num_bstx_ant;
	layer_per_codeword[1]			= 0;	

	P = (N_DL_RB <= 10)? 1:
	(N_DL_RB >= 11 && N_DL_RB <= 26)?  2:
	(N_DL_RB >= 27 && N_DL_RB <= 63)?  3:
	(N_DL_RB >= 64 && N_DL_RB <= 110)? 4: 0;
	
	int len					= (int)( ceil( (N_DL_RB + 0.0) / P));
	
	re_allocation_type = (N_DL_RB <= 10)? 0: inbuf[0] ;					//  re_allocation_type
	cur_len					 = (N_DL_RB <= 10)? 0: 1 ;
	//  re_allocation_assignment
	
	memcpy(&RB_assig_inf_arr[0], &inbuf[cur_len], len * sizeof(int16)); 
	
	cur_len += len; 
	
	RB_assig_inf_interpret(uerx_dciparse_u, RB_assig_inf_arr, len, N_DL_RB, re_allocation_type, Local_Distrib_VRB_flag, RB_arr, pdsch_rb_alloc);
	
	memcpy(&MCS_arr[0], &inbuf[cur_len],  5 * sizeof(int16));			 // MCS
	
	cur_len += 5;
	
	// HARQ_process_number
	memcpy(&HARQ_process_number_arr[0], & inbuf[cur_len],  HARQ_proc_bitnum * sizeof(int16)); 
	
	cur_len += HARQ_proc_bitnum;
	
	*HARQ_process_idx = bit2dec(HARQ_process_number_arr, HARQ_proc_bitnum, UERX_LOWERBITS_FIRST);
	
	
	NDI[0] = inbuf[cur_len];												// NDI
	
	cur_len += 1;
	
	memcpy(&RV_arr[0], &inbuf[cur_len],  2 * sizeof(int16));	             // RV
	
	cur_len += 2;
	
	RV_interpret(RV_arr, (*uerx_dciparse_u).DCI_type, 0 , 2, RNTI_type, RV);
	
	MCS_flag = MCS_interpret1(uerx_dciparse_u,MCS_arr, 5, 0, RNTI_type, (*uerx_dciparse_u).DCI_type,  *pdsch_rb_alloc,
		layer_per_codeword,  TBS,  qm,  *diversity_spatial);
	
	if (MCS_flag)
	{
		return 1;																// should not pdcch detect
	}
	
	memcpy(& TPC_PUCCH_arr[0], &inbuf[cur_len],  2 * sizeof(int16));	    // TPC FOR PUCCH
	
	cur_len += 2;
	
	 *TPC_PUCCH = bit2dec( TPC_PUCCH_arr, 2, UERX_LOWERBITS_FIRST);
	
	
	memcpy(& DAI_arr[0], &inbuf[cur_len],  2 * sizeof(int16));			    // downlink assignmengt index
	
	cur_len += 2;
	
	 *DAI = bit2dec( DAI_arr, 2, UERX_LOWERBITS_FIRST);
	
//	PrintInfo(1,"UERX_DCI_1 : Re_allocation_type\t= %d \n",  re_allocation_type);
 
	if ( *diversity_spatial == UERX_SFBC_PRECOD)
	{
//		PrintInfo(1, "UERX_DCI_1 : UERX_SFBC_PRECOD\n");
	}
	else if  (*diversity_spatial == UERX_SISO_PORT0_PRECOD)
	{
//		PrintInfo(1, "UERX_DCI_1 : Single antenna port 0\n");
	}
	else if ( *diversity_spatial == UERX_SISO_PORT5_PRECOD)
	{
//		PrintInfo(1, "UERX_DCI_1 : Single antenna port 5\n");
	}
//	PrintInfo(1,"UERX_DCI_1 : QAM		\t= %d;\tTBS		\t= %d \n",  qm[0],  TBS[0]);
//	PrintInfo(1,"UERX_DCI_1 : HARQ_process_idx\t= %d  \n",  HARQ_process_idx);
//	PrintInfo(1,"UERX_DCI_1 : NDI		\t= %d  \n", NDI[0]);
//	PrintInfo(1,"UERX_DCI_1 : RV		\t= %d  \n",  RV[0]);
//	PrintInfo(1,"UERX_DCI_1 : TPC_PUCCH	\t= %d  \n",  TPC_PUCCH);
//	PrintInfo(1,"UERX_DCI_1 : DAI		\t= %d  \n",  DAI);
//	PrintInfo(1,"UERX_DCI_1 : len		\t= %d  \n", cur_len);
	return 0;
}


int DCI1A_parser( uerx_dci_parse  *uerx_dciparse_u,int16 *inbuf, int16 N_DL_RB, int16 mux_mode, int16 RNTI_type, int16 transmission_mode,
									int16 num_bstx_ant, int16 RI_last,	int16 PMI_last,
									int16 * codeword_num, int16 *diversity_spatial, int16 *layers_total,
									int16 layer_per_codeword[2], int16 RB_arr[2][110], int16 *pdsch_rb_alloc,
									int16 *HARQ_process_idx, int16 NDI[2], int16 RV[2],
									int TBS[2], int16 qm[2], int16 *TPC_PUCCH , int16 *DAI, int16 *codebook_idx,
									int16 *cobook_column_idx,int DCI_sourlen,int Preamble_idx,int PRACH_Mask_idx)
{

	int i;
    int HARQ_proc_bitnum = (mux_mode == UERX_TDD_MUXMODE)? 4: 3;
 	int MCS_flag;
 	int cur_len = 1;
 	int type2_len		= (int)(ceil( log10((N_DL_RB + 1.0) * N_DL_RB / 2) / log10(2.0))); // 11 for N_DL_RB = 50
 	int N_PRB_1A;

//	int16 TB_disabled[2];
	int16 MCS_arr1[5]		= {0};


	int16 RV_arr1[2]		= {0};

	int16 Preamble_idx_arr1[6] = {0};
	int16 PRACH_Mask_idx_arr1[4] = {0};
//	int16 MCS1, MCS2;
//	int16 RV1, RV2  ;
//	int16 NDI1, NDI2;
//	int16 TB2codeword_swap;

	int16 RB_assig_inf_arr[30];
	int16 re_allocation_type;
	int16 Local_Distrib_VRB_flag = 0;

	int16 HARQ_process_number_arr[4];
	int16 TPC_PUCCH_arr[2];
	int16 DAI_arr[2];

 	*codeword_num	= 1;
 	re_allocation_type	= 2;
 	if (RNTI_type == UERX_C_RNTI)
 	{
 		if (transmission_mode == UERX_TM_MODE1)
 		{
 			*diversity_spatial	=  UERX_SISO_PRECOD;
 		}
 		else if (transmission_mode == UERX_TM_MODE2 || transmission_mode == UERX_TM_MODE3 ||
 			transmission_mode == UERX_TM_MODE4 || transmission_mode == UERX_TM_MODE5 ||
 			transmission_mode == UERX_TM_MODE6)
 		{
 			*diversity_spatial	=  UERX_DIVERSITY;
 		}
 		else if (transmission_mode == UERX_TM_MODE7)
 		{
 			*diversity_spatial	=  (num_bstx_ant == 1)? UERX_SISO_PRECOD: UERX_DIVERSITY;
 		}
 	}
 	else if (RNTI_type == UERX_SPSC_RNTI)
 	{
		if (transmission_mode == UERX_TM_MODE1)
 		{
 			*diversity_spatial	=  UERX_SISO_PRECOD;
 		}
 		else if (transmission_mode == UERX_TM_MODE7)
 		{
 			*diversity_spatial	=  UERX_SISO_PORT5_PRECOD;
 		}
 		else if (transmission_mode == UERX_TM_MODE2 || transmission_mode == UERX_TM_MODE3 ||
 			transmission_mode == UERX_TM_MODE4 || transmission_mode == UERX_TM_MODE5 ||
			transmission_mode == UERX_TM_MODE6)
 		{
 			*diversity_spatial	=  UERX_DIVERSITY;
 		}

 	}
 	else if (RNTI_type == UERX_TC_RNTI)
 	{
 		*diversity_spatial	=  (num_bstx_ant == 1)? UERX_SISO_PRECOD: UERX_DIVERSITY;
 	}
 	else
 	{
 		*diversity_spatial	=  (num_bstx_ant == 1)? UERX_SISO_PRECOD: UERX_DIVERSITY;
 	}

 	if (*diversity_spatial == UERX_DIVERSITY)
 	{
 		*layers_total				= num_bstx_ant;
 		layer_per_codeword[0]			= 1;
 		layer_per_codeword[1]			= 1;
 	}
 	else
 	{
 		*layers_total				= num_bstx_ant;
 		layer_per_codeword[0]			= 1;
 		layer_per_codeword[1]			= 1;
 	}

 	Local_Distrib_VRB_flag = inbuf[cur_len]; 	//  re_allocation_type 2: local or distributed
 	cur_len ++;

 	memcpy(&RB_assig_inf_arr[0], &inbuf[cur_len], type2_len * sizeof(int16));
 	cur_len += type2_len;								   //  re_allocation_assignment

   	if (RNTI_type == UERX_C_RNTI && DCI1A_Is_Randacces(&inbuf[1], type2_len, DCI_sourlen ))	               // dci 1a used for random access process
  	{
  			RB_assig_inf_interpret(uerx_dciparse_u,RB_assig_inf_arr, type2_len, N_DL_RB, re_allocation_type, Local_Distrib_VRB_flag,
  			RB_arr, pdsch_rb_alloc);

   		memcpy(&Preamble_idx_arr1, &inbuf[cur_len], 6 * sizeof(int16));
 		cur_len += 6;
  		Preamble_idx		= bit2dec(Preamble_idx_arr1, 6, UERX_LOWERBITS_FIRST);

  		memcpy(&PRACH_Mask_idx_arr1, &inbuf[cur_len], 4 * sizeof(int16));
  		cur_len += 4;
  		PRACH_Mask_idx	= bit2dec(PRACH_Mask_idx_arr1, 4, UERX_LOWERBITS_FIRST);

  		//PrintInfo(1,"UERX_DCI_1A : Re_allocation_type\t= 2;   \tLocal_Distrib_VRB_flag \t= %d\n"
  	//		, Local_Distrib_VRB_flag);
  		//PrintInfo(1,"UERX_DCI_1A : pdsch_rb_start	\t= %d;  \tpdsch_rb	\t= %d\n"
  	//		, pdsch_rb_start, pdsch_rb_alloc);
  		//PrintInfo(1,"UERX_DCI_1A : Preamble_idx	\t= %d\n", Preamble_idx);
  		//PrintInfo(1,"UERX_DCI_1A : PRACH_Mask_idx	\t= %d\n", PRACH_Mask_idx);
  	}
  	else
  	{
  		memcpy(&MCS_arr1, & inbuf[cur_len],  5 * sizeof(int16));			// MCS
  		cur_len += 5;
 		// HARQ_process_number
  		memcpy(&HARQ_process_number_arr[0], & inbuf[cur_len],  HARQ_proc_bitnum * sizeof(int16));
  		cur_len += HARQ_proc_bitnum;

  		NDI[0] = inbuf[cur_len];												  //NDI
  		cur_len += 1;

  		memcpy(&RV_arr1, &inbuf[cur_len],  2 * sizeof(int16));	            // RV
  		cur_len += 2;
  		RV[0]	= bit2dec(RV_arr1, 2, UERX_LOWERBITS_FIRST);


  		memcpy(&TPC_PUCCH_arr[0], &inbuf[cur_len],  2 * sizeof(int16)); // TPC FOR PUCCH
  		cur_len += 2;

  		memcpy(&DAI_arr[0], &inbuf[cur_len],  2 * sizeof(int16));		  // downlink assignmengt index
  		cur_len += 2;

//  		PrintInfo(1,"UERX_DCI_1A : Re_allocation_type\t= 2;\tLocal_Distrib_VRB_flag\t= %d \n"
//  			, Local_Distrib_VRB_flag);

  		if (Local_Distrib_VRB_flag ==0)    //0:locallized_VRB           //Resorce block assignment
  		{
  			RB_assig_inf_interpret(uerx_dciparse_u,RB_assig_inf_arr, type2_len, N_DL_RB, re_allocation_type, Local_Distrib_VRB_flag,
  				RB_arr, pdsch_rb_alloc);
  		}
  		else
 		{
  			if (N_DL_RB < 50 || ( RNTI_type == UERX_RA_RNTI || RNTI_type == UERX_P_RNTI
  			||   RNTI_type == UERX_SI_RNTI))
  			{
  				RB_assig_inf_interpret(uerx_dciparse_u,RB_assig_inf_arr, type2_len, N_DL_RB, re_allocation_type, Local_Distrib_VRB_flag,
  					RB_arr, pdsch_rb_alloc);
  			}
  			else
  			{
  				(*uerx_dciparse_u).N_gap = (RB_assig_inf_arr[0] == 0)? (*uerx_dciparse_u).N_gap1: (*uerx_dciparse_u).N_gap2;
//  				PrintInfo(1,"UERX_DCI_1A : MSB of RBassign indicates the N_gap = %d  \n", N_gap);

   				RB_assig_inf_interpret(uerx_dciparse_u,&RB_assig_inf_arr[1], type2_len - 1, N_DL_RB, re_allocation_type, Local_Distrib_VRB_flag,
  					RB_arr, pdsch_rb_alloc);

  			}
  		}

  		if (RNTI_type == UERX_RA_RNTI || RNTI_type == UERX_P_RNTI|| RNTI_type == UERX_SI_RNTI)//NDI&TPC
  		{
  			if (N_DL_RB >= 50 && Local_Distrib_VRB_flag == 1)// 1:distributer_VRB
  			{
  				(*uerx_dciparse_u).N_gap			= (NDI[0] == 0)?(*uerx_dciparse_u).N_gap1: (*uerx_dciparse_u).N_gap2;
//  				PrintInfo(1,"UERX_DCI_1A : NDI indicates the N_gap = %d  \n", N_gap);
  			}
  			else
  			{
//  				PrintInfo(1,"UERX_DCI_1A : NDI 1 bit Reserved  \n");
  			}

  			N_PRB_1A	= (TPC_PUCCH_arr[1] == 0)? 2: 3;

  			 // HARQ_process_idx     reserved
  			// DAI			 reserved

//  			PrintInfo(1,"UERX_DCI_1A : TPC_PUCCH[0] is reserved; TPC_PUCCH[1] = %d, indicates N_PRB_1A\t= %d\n"
// 				, TPC_PUCCH_arr[1], N_PRB_1A);
// 			PrintInfo(1,"UERX_DCI_1A : HARQ_process	2	bits Reserved\n");
// 			PrintInfo(1,"UERX_DCI_1A : DAI			2	bits Reserved\n");
  		}
  		else
  		{
  			*HARQ_process_idx  = bit2dec(HARQ_process_number_arr
  				, HARQ_proc_bitnum, UERX_LOWERBITS_FIRST);
//  			PrintInfo(1,"UERX_DCI_1A : HARQ_process_idx\t= %d  \n", HARQ_process_idx);

//  			PrintInfo(1,"UERX_DCI_1A : NDI		= %d  \n", NDI[0]);

  			*TPC_PUCCH = bit2dec(TPC_PUCCH_arr, 2, UERX_LOWERBITS_FIRST);
//  			PrintInfo(1,"UERX_DCI_1A : TPC_PUCCH = %d  \n", TPC_PUCCH);

  			*DAI				= bit2dec(DAI_arr, 2, UERX_LOWERBITS_FIRST);
//  			PrintInfo(1,"UERX_DCI_1A : DAI		\t= %d  \n",DAI);
 		}
  	}
  	MCS_flag = MCS_interpret1(uerx_dciparse_u,MCS_arr1, 5, 0, RNTI_type, (*uerx_dciparse_u).DCI_type,  *pdsch_rb_alloc,
  		layer_per_codeword, TBS, qm,*diversity_spatial);

  	if (MCS_flag)
  	{
  		return 1;									 // should not pdcch detect
  	}
//  	PrintInfo(1,"UERX_DCI_1A : QAM		\t= %d;\tTBS		\t= %d \n"
//  		, qm[0], TBS[0]);
//  	PrintInfo(1,"UERX_DCI_1A : RV		\t= %d\n",RV[0]);
//
//  	//PrintInfo(1,"UERX_DCI_1A : Pdsch_rb_start	\t= %d;\tpdsch_rb	\t= %d\n"
//  		//, pdsch_rb_start, pdsch_rb_alloc);
//
//  	PrintInfo(1,"UERX_DCI_1A : len\t= %d \n", cur_len);

  	return 0;
  }


/*
	pinf->re_allocation_type	= 2;
	int i;
 	int mux_mode	= pinf->mux_mode;
    int HARQ_proc_bitnum = (mux_mode == TDD_muxmode)? 4: 3;
 	int MCS_flag;
 	int cur_len = 1;
 	int type2_len		= int(ceil( log10((N_DL_RB + 1.0) * N_DL_RB / 2) / log10(2.0))); // 11 for N_DL_RB = 50

 	pinf->codeword_num	= 1;

 	if (pinf->RNTI_type == UERX_C_RNTI)
 	{
 		if (pinf->transmission_mode == UERX_TM_MODE1)
 		{
 			pinf->diversity_spatial	=  UERX_SISO_PRECOD;
 		}
 		else if (	pinf->transmission_mode == UERX_TM_MODE2 || pinf->transmission_mode == UERX_TM_MODE3 ||
 			pinf->transmission_mode == UERX_TM_MODE4 || pinf->transmission_mode == UERX_TM_MODE5 ||
 			pinf->transmission_mode == UERX_TM_MODE6)
 		{
 			pinf->diversity_spatial	=  DIVERSITY;
 		}
 		else if (pinf->transmission_mode == UERX_TM_MODE7)
 		{
 			pinf->diversity_spatial	=  (pinf->num_bstx_ant == 1)? UERX_SISO_PRECOD: DIVERSITY;
 		}
 	}
 	else if (pinf->RNTI_type == UERX_SPSC_RNTI)
 	{
		if (pinf->transmission_mode == UERX_TM_MODE1)
 		{
 			pinf->diversity_spatial	=  UERX_SISO_PRECOD;
 		}
 		else if (pinf->transmission_mode == UERX_TM_MODE7)
 		{
 			pinf->diversity_spatial	=  UERX_SISO_PORT5_PRECOD;
 		}
 		else if (	pinf->transmission_mode == UERX_TM_MODE2 || pinf->transmission_mode == UERX_TM_MODE3 ||
 			pinf->transmission_mode == UERX_TM_MODE4 || pinf->transmission_mode == UERX_TM_MODE5 ||
			pinf->transmission_mode == UERX_TM_MODE6)
 		{
 			pinf->diversity_spatial	=  DIVERSITY;
 		}

 	}
 	else if (pinf->RNTI_type == UERX_TC_RNTI)
 	{
 		pinf->diversity_spatial	=  (pinf->num_bstx_ant == 1)? UERX_SISO_PRECOD: DIVERSITY;
 	}
 	else
 	{
 		pinf->diversity_spatial	=  (pinf->num_bstx_ant == 1)? UERX_SISO_PRECOD: DIVERSITY;
 	}

 	if (pinf->diversity_spatial == DIVERSITY)
 	{
 		pinf->layers_total					= pinf->num_bstx_ant;
 		pinf->layer_per_codeword[0]			= 1;
 		pinf->layer_per_codeword[1]			= 1;
 	}
 	else
 	{
 		pinf->layers_total					= pinf->num_bstx_ant;
 		pinf->layer_per_codeword[0]			= 1;
 		pinf->layer_per_codeword[1]			= 1;
 	}

 	pinf->Local_Distrib_VRB_flag = inbuf[cur_len]; 	//  re_allocation_type 2: local or distributed
 	cur_len ++;

 	memcpy(&pinf->RB_assig_inf_arr[0], &inbuf[cur_len], type2_len * sizeof(int16));
 	cur_len += type2_len;								   //  re_allocation_assignment

   	if (pinf->RNTI_type == UERX_C_RNTI && DCI1A_Is_Randacces(&inbuf[1], type2_len, pinf->DCI_sourlen ))	               // dci 1a used for random access process
  	{
    		RB_assig_inf_interpret(pinf->RB_assig_inf_arr, type2_len, pinf);
  		RB_assig_inf_interpret(pinf->RB_assig_inf_arr, type2_len, N_DL_RB, pinf->re_allocation_type, pinf->Local_Distrib_VRB_flag,
  			pinf->RB_arr, pinf->pdsch_rb_alloc);

   		memcpy(&pinf->Preamble_idx_arr[0], &inbuf[cur_len], 6 * sizeof(int16));
 		cur_len += 6;
  		pinf->Preamble_idx		= bit2dec(pinf->Preamble_idx_arr, 6, UERX_LOWERBITS_FIRST);

  		memcpy(&pinf->PRACH_Mask_idx_arr[0], &inbuf[cur_len], 4 * sizeof(int16));
  		cur_len += 4;
  		pinf->PRACH_Mask_idx	= bit2dec(pinf->PRACH_Mask_idx_arr, 4, UERX_LOWERBITS_FIRST);

  		PrintInfo(1,"UERX_DCI_1A : Re_allocation_type\t= 2;   \tLocal_Distrib_VRB_flag \t= %d\n"
  			, pinf->Local_Distrib_VRB_flag);
  		PrintInfo(1,"UERX_DCI_1A : pdsch_rb_start	\t= %d;  \tpdsch_rb	\t= %d\n"
  			, pinf->pdsch_rb_start, pinf->pdsch_rb_alloc);
  		PrintInfo(1,"UERX_DCI_1A : Preamble_idx	\t= %d\n", pinf->Preamble_idx);
  		PrintInfo(1,"UERX_DCI_1A : PRACH_Mask_idx	\t= %d\n", pinf->PRACH_Mask_idx);
  	}
  	else
  	{
  		memcpy(&pinf->MCS_arr[0], & inbuf[cur_len],  5 * sizeof(int16));			// MCS
  		cur_len += 5;
 		// HARQ_process_number
  		memcpy(&pinf->HARQ_process_number_arr[0], & inbuf[cur_len],  HARQ_proc_bitnum * sizeof(int16));
  		cur_len += HARQ_proc_bitnum;

  		pinf->NDI[0] = inbuf[cur_len];												  //NDI
  		cur_len += 1;

  		memcpy(&pinf->RV_arr[0], &inbuf[cur_len],  2 * sizeof(int16));	            // RV
  		cur_len += 2;
  		pinf->RV[0]	= bit2dec(pinf->RV_arr, 2, UERX_LOWERBITS_FIRST);


  		memcpy(&pinf->TPC_PUCCH_arr[0], &inbuf[cur_len],  2 * sizeof(int16)); // TPC FOR PUCCH
  		cur_len += 2;

  		memcpy(&pinf->DAI_arr[0], &inbuf[cur_len],  2 * sizeof(int16));		  // downlink assignmengt index
  		cur_len += 2;

  		PrintInfo(1,"UERX_DCI_1A : Re_allocation_type\t= 2;\tLocal_Distrib_VRB_flag\t= %d \n"
  			, pinf->Local_Distrib_VRB_flag);

  		if (pinf->Local_Distrib_VRB_flag == locallized_VRB)//Resorce block assignment
  		{
  			RB_assig_inf_interpret(pinf->RB_assig_inf_arr, type2_len, N_DL_RB, pinf->re_allocation_type, pinf->Local_Distrib_VRB_flag,
  				pinf->RB_arr, pinf->pdsch_rb_alloc);
  		}
  		else
 		{
  			if (pinf->N_DL_RB < 50 || (pinf->RNTI_type == UERX_RA_RNTI || pinf->RNTI_type == UERX_P_RNTI
  			|| pinf->RNTI_type == UERX_SI_RNTI))
  			{
  				RB_assig_inf_interpret(pinf->RB_assig_inf_arr, type2_len, N_DL_RB, pinf->re_allocation_type, pinf->Local_Distrib_VRB_flag,
  					pinf->RB_arr, pinf->pdsch_rb_alloc);
  			}
  			else
  			{
  				pinf->N_gap = (pinf->RB_assig_inf_arr[0] == 0)? N_gap1: N_gap2;
  				PrintInfo(1,"UERX_DCI_1A : MSB of RBassign indicates the N_gap = %d  \n", pinf->N_gap);

   				RB_assig_inf_interpret(&pinf->RB_assig_inf_arr[1], type2_len - 1, N_DL_RB, pinf->re_allocation_type, pinf->Local_Distrib_VRB_flag,
  					pinf->RB_arr, pinf->pdsch_rb_alloc);

  			}
  		}

  		if (pinf->RNTI_type == UERX_RA_RNTI || pinf->RNTI_type == UERX_P_RNTI|| pinf->RNTI_type == UERX_SI_RNTI)//NDI&TPC
  		{
  			if (pinf->N_DL_RB >= 50 && pinf->Local_Distrib_VRB_flag == distributer_VRB)
  			{
  				pinf->N_gap			= (pinf->NDI[0] == 0)? pinf->N_gap1: pinf->N_gap2;
  				PrintInfo(1,"UERX_DCI_1A : NDI indicates the N_gap = %d  \n", pinf->N_gap);
  			}
  			else
  			{
  				PrintInfo(1,"UERX_DCI_1A : NDI 1 bit Reserved  \n");
  			}

  			pinf->N_PRB_1A			= (pinf->TPC_PUCCH_arr[1] == 0)? 2: 3;

  			 // HARQ_process_idx     reserved
  			// DAI			    	reserved

  			PrintInfo(1,"UERX_DCI_1A : TPC_PUCCH[0] is reserved; TPC_PUCCH[1] = %d, indicates N_PRB_1A\t= %d\n"
  				, pinf->TPC_PUCCH_arr[1], pinf->N_PRB_1A);
  			PrintInfo(1,"UERX_DCI_1A : HARQ_process	2	bits Reserved\n");
  			PrintInfo(1,"UERX_DCI_1A : DAI			2	bits Reserved\n");


  		}
  		else
  		{
  			pinf->HARQ_process_idx  = bit2dec(pinf->HARQ_process_number_arr
  				, HARQ_proc_bitnum, UERX_LOWERBITS_FIRST);
  			PrintInfo(1,"UERX_DCI_1A : HARQ_process_idx\t= %d  \n", pinf->HARQ_process_idx);

  			PrintInfo(1,"UERX_DCI_1A : NDI		= %d  \n", pinf->NDI[0]);

  			pinf->TPC_PUCCH = bit2dec(pinf->TPC_PUCCH_arr, 2, UERX_LOWERBITS_FIRST);
  			PrintInfo(1,"UERX_DCI_1A : TPC_PUCCH = %d  \n", pinf->TPC_PUCCH);

  			pinf->DAI				= bit2dec(pinf->DAI_arr, 2, UERX_LOWERBITS_FIRST);
  			PrintInfo(1,"UERX_DCI_1A : DAI		\t= %d  \n", pinf->DAI);
 		}
  	}

  	MCS_flag = MCS_interpret(pinf->MCS_arr, 5, 0, pinf->RNTI_type, DCI_type,  pinf->pdsch_rb_alloc,
  		pinf->layer_per_codeword, pinf->TBS, pinf->qm,pinf->diversity_spatial);

  	if (MCS_flag)
  	{
  		return 1;									 // should not pdcch detect
  	}
  	PrintInfo(1,"UERX_DCI_1A : QAM		\t= %d;\tTBS		\t= %d \n"
  		, pinf->qm[0], pinf->TBS[0]);
  	PrintInfo(1,"UERX_DCI_1A : RV		\t= %d\n", pinf->RV[0]);

  	PrintInfo(1,"UERX_DCI_1A : Pdsch_rb_start	\t= %d;\tpdsch_rb	\t= %d\n"
  		, pinf->pdsch_rb_start, pinf->pdsch_rb_alloc);

  	PrintInfo(1,"UERX_DCI_1A : len\t= %d \n", cur_len);

  	return 0;
  }
*/
int DCI2_2A_parser(uerx_dci_parse  *uerx_dciparse_u,int16 DCI_format, int16 *inbuf, int16 N_DL_RB, int16 mux_mode, int16 RNTI_type, int16 transmission_mode,
									int16 num_bstx_ant, int16 RI_last,	int16 PMI_last,
									int16 * codeword_num, int16 *diversity_spatial, int16 *layers_total,
									int16 layer_per_codeword[2], int16 RB_arr[2][110], int16 *pdsch_rb_alloc,
									int16 *HARQ_process_idx, int16 NDI[2], int16 RV[2],
									int TBS[2], int16 qm[2], int16 *TPC_PUCCH , int16 *DAI, int16 *codebook_idx,
									int16 *cobook_column_idx)
 
{
	int16 cur_len				= 0;
    int16 HARQ_proc_bitnum	= (mux_mode == UERX_TDD_MUXMODE)? 4: 3;
	
	int16 MCS_flag			= 0;
	int16 TB_disabled[2];
	int16 MCS_arr1[5]		= {0};
	int16 MCS_arr2[5]		= {0};

	int16 RV_arr1[2]		= {0};
	int16 RV_arr2[2]		= {0};
	int16 MCS1, MCS2;
	int16 RV1, RV2  ;
	int16 NDI1, NDI2;
	int16 TB2codeword_swap;
	
	int16 RB_assig_inf_arr[30];
	int16 re_allocation_type;
	int16 Local_Distrib_VRB_flag = 0;
	
	int16 HARQ_process_number_arr[4];
	int16 TPC_PUCCH_arr[2];
	int16 DAI_arr[2];

	int16 Precoding_info_arr[6];
	int16 precod_info_len;
	
	int16 P	=	(N_DL_RB <= 10)? 1:
	(N_DL_RB >= 11 && N_DL_RB <= 26)?  2:
	(N_DL_RB >= 27 && N_DL_RB <= 63)?  3:
	(N_DL_RB >= 64 && N_DL_RB <= 110)? 4: 0;
	
	int16 len					= (int)(ceil( (N_DL_RB + 0.0) / P));

	
	re_allocation_type = (N_DL_RB <= 10)? 0: inbuf[0] ;		      //  re_allocation_type
	cur_len					 = (N_DL_RB <= 10)? 0: 1 ;
	//  re_allocation_assignment
	
	memcpy(&RB_assig_inf_arr[0], &inbuf[cur_len], len * sizeof(int16)); 
	
	cur_len += len; 
	
	RB_assig_inf_interpret(uerx_dciparse_u,RB_assig_inf_arr, len, N_DL_RB, re_allocation_type, Local_Distrib_VRB_flag,
		RB_arr, pdsch_rb_alloc);
	
	
	memcpy(&TPC_PUCCH_arr[0], &inbuf[cur_len],  2 * sizeof(int16)); // TPC FOR PUCCH
	
	cur_len += 2;
	
	*TPC_PUCCH = bit2dec(TPC_PUCCH_arr, 2, UERX_LOWERBITS_FIRST);
	
	
	memcpy(&DAI_arr[0], &inbuf[cur_len],  2 * sizeof(int16));		  // downlink assignmengt index
	
	cur_len += 2;
	
	*DAI = bit2dec(DAI_arr, 2, UERX_LOWERBITS_FIRST);
	
	// HARQ_process_number
	memcpy(&HARQ_process_number_arr[0], &inbuf[cur_len],  HARQ_proc_bitnum * sizeof(int16)); 
	
	cur_len += HARQ_proc_bitnum;
	
	*HARQ_process_idx = bit2dec(HARQ_process_number_arr, HARQ_proc_bitnum, UERX_LOWERBITS_FIRST);
	
	//	PrintInfo(1,"\n\t----   DCI Parser as follows:  -------------\n");
	
//	if (DCI_format == UERX_DCI_2)
//		PrintInfo(1,"\n\tDCI_2");
//	else
//		PrintInfo(1,"\n\tDCI_2A");
//
//	PrintInfo(1,"\n\tRe_allocation_type\t= %d \n",  re_allocation_type);
//
//	//	PrintInfo(1,"\n\tTPC_PUCCH	\t= %d  \n", pinf->TPC_PUCCH);
//	//	PrintInfo(1,"\n\tDAI		\t= %d\n", pinf->DAI);
//		PrintInfo(1,"\n\tHARQ_process_idx\t= %d",  HARQ_process_idx);
	
	 TB2codeword_swap	= inbuf[cur_len];
	cur_len ++;
	
	// get the MCS  RV NDI of each TB
	// 1st Transport block
	memcpy(MCS_arr1, &inbuf[cur_len],  5 * sizeof(int16));				 // MCS
	cur_len += 5;
	MCS1	= bit2dec(MCS_arr1, 5, UERX_LOWERBITS_FIRST);
	
	NDI1 = inbuf[cur_len];												 // NDI
	cur_len ++;
	
	memcpy(RV_arr1, &inbuf[cur_len],  2 * sizeof(int16));				 // RV
	cur_len += 2;
	RV1	= bit2dec(RV_arr1, 2, UERX_LOWERBITS_FIRST);
	
	// 2nd Transport block
	memcpy(MCS_arr2, &inbuf[cur_len],  5 * sizeof(int16));				 // MCS
	cur_len += 5;
	MCS2	= bit2dec(MCS_arr2, 5, UERX_LOWERBITS_FIRST);
	
	NDI2 = inbuf[cur_len];												 // NDI
	cur_len ++;
	
	memcpy(RV_arr2, &inbuf[cur_len],  2 * sizeof(int16));				 // RV
	cur_len += 2;
	RV2	= bit2dec(RV_arr2, 2, UERX_LOWERBITS_FIRST);
	
	// get the codeword num  
	TB_disabled[0]		= (MCS1 == 0 && RV1 == 1)? 1: 0;		// disabled
	
	TB_disabled[1]		= (MCS2 == 0 && RV2 == 1)? 1: 0;		// disabled
	
	*codeword_num			= (TB_disabled[0] == 0 && TB_disabled[1] == 0)?  2:
	(TB_disabled[0] == 1 && TB_disabled[1] == 0)?  1:
	(TB_disabled[0] == 0 && TB_disabled[1] == 1)?  1: 0;
	
//	PrintInfo(1, "\n\tCodeword_num	\t= %d",  codeword_num);
	
	// get total N_layers  and codebook idx and Diversity or Spatial 
	
	precod_info_len				= ( num_bstx_ant == 2)? ((DCI_format == UERX_DCI_2)? 3: 0) :
	((DCI_format == UERX_DCI_2)? 6: 2) ;
	
	memcpy( Precoding_info_arr, &inbuf[cur_len], precod_info_len * sizeof(int16) );
	cur_len += precod_info_len;	
	
//	if ((DCI_format == UERX_DCI_2A || DCI_format == UERX_DCI_2) &&  num_bstx_ant == 2) //CR20130518, DCI_2 added by HuangShan
	if (DCI_format == UERX_DCI_2A &&  num_bstx_ant == 2) // 20141201 linjiangnan, (precod_info_len==0)
	{
		if ( *codeword_num == 2)
		{
			*layers_total			= 2;
			*diversity_spatial	    = UERX_OPENLOOP_SM_PRECOD;
			*codebook_idx			= 0;

		}
		else if (*codeword_num == 1)
		{
			*layers_total			= 2;
			*diversity_spatial 	= UERX_SFBC_PRECOD;
		}
	}
	else
	{
		Precoding_Info_interpret(Precoding_info_arr, precod_info_len,  *codeword_num,  RI_last,  PMI_last,
			 layers_total,  diversity_spatial, codebook_idx  , cobook_column_idx);

	}
	
//	PrintInfo(1, "\n\tTotal _ layer	= %d",	 layers_total);
	if ( *diversity_spatial == UERX_SFBC_PRECOD)
	{
//		PrintInfo(1, "\n\tMIMO scheme:\n\tSFBC_prec");
	}
	else if ( *diversity_spatial == UERX_CLOSLOOP_SM_PRECOD)
	{
//		PrintInfo(1, "\n\tMIMO scheme:\tCloseloopSM\n\tcodebook_idx\t= %d", codebook_idx);
	}
	else if ( *diversity_spatial == UERX_OPENLOOP_SM_PRECOD)
	{
//		PrintInfo(1, "\n\tMIMO scheme:\tOpenloopSM");
	}
	
	// get layer of each codeword
    switch ( *layers_total)
    {
	case 1: 
		 layer_per_codeword[0]			= 1;
		 layer_per_codeword[1]			= 0;
		break;
	case 2:
		if ( *codeword_num == 2)
		{
			layer_per_codeword[0]		= 1;
			layer_per_codeword[1]		= 1;
		}
		else
		{
			 layer_per_codeword[0]		= 2;
			 layer_per_codeword[1]		= 0;
		}
		break;
	case 3:
		 layer_per_codeword[0]			= 1;
		 layer_per_codeword[1]			= 2;
		break;
	case 4:
		 layer_per_codeword[0]			= 2;
		 layer_per_codeword[1]			= 2;
		break;
	default:
		break;
    }
	
	//	PrintInfo(1, "\n\tCW0_layer	\t= %d,\tCW1_layer	\t= %d"
	// , pinf->layer_per_codeword[0], pinf->layer_per_codeword[1]);
	
	// then  get the Qm  and TBS of each codeword
	if ( TB_disabled[0] == 0 &&  TB_disabled[1] == 0)
	{
		
//		PrintInfo(1,"\n\tTransport block to codeword swap flag = %d",  TB2codeword_swap);
		if ( TB2codeword_swap == 0)
		{
//			PrintInfo(1,"\n\tTB 1 --> CW 0;  TB 2 --> CW 1");
			// TB 1--> CW 0
			MCS_flag		= MCS_interpret1(uerx_dciparse_u,MCS_arr1, 5, 0,  RNTI_type, (*uerx_dciparse_u).DCI_type
				,  *pdsch_rb_alloc, layer_per_codeword,  TBS,  qm
				, *diversity_spatial);
			
			 RV[0]		= RV1;			
			 NDI[0]	= NDI1;
			
			// TB 2--> CW 1
			MCS_flag		= MCS_interpret1(uerx_dciparse_u,MCS_arr2, 5, 1,  RNTI_type, (*uerx_dciparse_u).DCI_type
				,   *pdsch_rb_alloc,  layer_per_codeword,  TBS,  qm
				,  *diversity_spatial);

			RV[1]		= RV2;			
			 NDI[1]	= NDI2;
		}
		else
		{
//			PrintInfo(1,"\n\tTB 1 --> CW 1;  TB 2 --> CW 0");
			// TB 1--> CW 1
			MCS_flag = MCS_interpret1(uerx_dciparse_u,MCS_arr1, 5, 1,  RNTI_type, (*uerx_dciparse_u).DCI_type
				,  *pdsch_rb_alloc, layer_per_codeword,  TBS
				,  qm, *diversity_spatial);

			 RV[1]		= RV1;			
			 NDI[1]	= NDI1;
			// TB 2--> CW 0
			MCS_flag = MCS_interpret1(uerx_dciparse_u,MCS_arr2, 5, 0,  RNTI_type, (*uerx_dciparse_u).DCI_type
				,  *pdsch_rb_alloc,  layer_per_codeword,  TBS
				,  qm, *diversity_spatial);
			
			 RV[0]		= RV2;			
			 NDI[0]	= NDI2;			
		}
		
//		PrintInfo(1, "\n\tTB 1:		MCS\t= %d,	\tRV\t= %d,\tNDI\t= %d", MCS1, RV1, NDI1);
//		PrintInfo(1, "\n\tTB 2:		MCS\t= %d,	\tRV\t= %d,\tNDI\t= %d", MCS2, RV2, NDI2);
//
//		PrintInfo(1, "\n\tCW 0:Qm = %d,\tTBS\t= %d,	\tRV\t= %d,\tNDI\t=%d"
//		,  qm[0],  TBS[0],  RV[0],  NDI[0]);
//		PrintInfo(1, "\n\tCW 1:Qm = %d,\tTBS\t= %d,	\tRV\t= %d,\tNDI\t=%d"
//		,  qm[1],  TBS[1],  RV[1],  NDI[1]);
	}
	else if( TB_disabled[0] == 1 &&  TB_disabled[1] == 0)	// TB 1 DISABLED
	{
		// PrintInfo(1,"\n\tTransport block to codeword swap flag reserved!!\n\tTB 2--> CW 0");
		// TB 2--> CW 0
		MCS_flag = MCS_interpret1(uerx_dciparse_u,MCS_arr2, 5, 0,  RNTI_type, (*uerx_dciparse_u).DCI_type,  *pdsch_rb_alloc
			,  layer_per_codeword, TBS, qm, *diversity_spatial);
		 RV[0]		= RV2;		
		 NDI[0]	= NDI2;
		
		// PrintInfo(1, "\n\tTB 2:		MCS\t= %d,	\tRV\t= %d,\tNDI\t= %d", MCS2, RV2, NDI2);		
		// PrintInfo(1, "\n\tCW 0:Qm = %d,\tTBS\t= %d,	\tRV\t= %d,\tNDI\t= %d"
		// , pinf->qm[0], pinf->TBS[0], pinf->RV[0], pinf->NDI[0]);		
	}
	else if( TB_disabled[1] == 1 &&  TB_disabled[0] == 0)	// TB 2 DISABLED
	{
		// PrintInfo(1,"\n\tTransport block to codeword swap flag reserved!!\n\tTB 1--> CW 0");
		// TB 1--> CW 0
		MCS_flag = MCS_interpret1(uerx_dciparse_u,MCS_arr1, 5, 0,  RNTI_type, (*uerx_dciparse_u).DCI_type,  *pdsch_rb_alloc,
			 layer_per_codeword,  TBS,  qm, *diversity_spatial);
		 RV[0]		= RV1;
		
		NDI[0]	= NDI1;
		
		// PrintInfo(1, "\n\tTB 1:		MCS\t= %d,	\tRV\t= %d,\tNDI\t= %d", MCS1, RV1, NDI1);		
		// PrintInfo(1, "\n\tCW 0:Qm = %d,\tTBS\t= %d,	\tRV\t= %d,\tNDI\t= %d"
		// , pinf->qm[0], pinf->TBS[0], pinf->RV[0], pinf->NDI[0]);		
	}
	else if( TB_disabled[0] == 1 &&  TB_disabled[1] == 1)	// TB 1 AND 2 BOTH DISABLED
	{
		// PrintInfo(1,"\n\tTransport block 1 AND 2 disabled, ERROR !!!!");
		return 1;
	}
	
	//	PrintInfo(1,"\n\tDCI_sourcelen	\t= %d", cur_len);	
	//	PrintInfo(1,"\n\t---------   DCI Parser END -------------\n");
	
	return 0;
}


//int uerx_dci_parse::RB_assig_inf_interpret(int16 inbuf[], int inlen, UERX_inf *pinf)
int RB_assig_inf_interpret(uerx_dci_parse  *uerx_dciparse_u,int16 inbuf[], int inlen, int16 N_DL_RB, int16 re_allocation_type, int16 Local_Distrib_VRB_flag,
											int16 RB_arr[2][110], int16 *pdsch_rb_alloc)
{
	int P, n_RBG;
 	int i, j;
	int p_start = 0;
	int test_case;
	int pdsch_rb_start = 0;

	P = (N_DL_RB <= 10)? 1:
	(N_DL_RB >= 11 && N_DL_RB <= 26)?  2:
	(N_DL_RB >= 27 && N_DL_RB <= 63)?  3:
	(N_DL_RB >= 64 && N_DL_RB <= 110)? 4: 0;


	int subRBG_bitnum, sub_RBG_idx;		
	int span_flag;
	int N_RB_TYPE1;
	int delta_shift;
	int N_RBGsubset;
	int *n_RBGsubset_arr;

	int N_VRB_DL;
	int RIV;
	int RB_start_tmp;
	int L_CRBs_tmp;

	
	int n_VRB_DL_b;
	int n_VRB, n_VRB_b, n_PRB_p, n_PRB_p_p;
	
	int num_interl;
	int **interl_arr = NULL;
	int n_row;
	int n_col;
	int n_null;
	int k_count		= 0;
	int m_count		= 0;
	int interl_block_idx;

	int16 n_VRB_arr[110];
	int n_VRB_intl_arr[110];

	int n_PRB_b_arr[2][110];
	int n_PRB_arr[2][110];

	int16 RBG_arr[30];
 
	memset(n_VRB_arr, 0, 110 * sizeof(int16));
	memset(RBG_arr, 0, 30 * sizeof(int16));
	memset(RB_arr[0],0, 110 * sizeof(int16));
	memset(RB_arr[1],0, 110 * sizeof(int16));

 	n_RBG = ceil(N_DL_RB / (P + 0.0));

	switch (re_allocation_type)
	{
	case UERX_RE_ALLOC_TYPE0:
		
		for (i = 0; i < n_RBG - 1; i ++)
		{
			RBG_arr[i] = P;
		}
		RBG_arr[n_RBG - 1]	 = N_DL_RB - P * (n_RBG - 1) ;
 
 		*pdsch_rb_alloc = 0;

		// asume successive allocation 
		for (i = 0; i < n_RBG ; i ++)
		{
			if (inbuf[i] == 1)
			{
				for (j = 0; j < RBG_arr[i]; j ++)
				{
					RB_arr[0][*pdsch_rb_alloc + j] = i * P + j;
					RB_arr[1][*pdsch_rb_alloc + j] = i * P + j;
				}
				*pdsch_rb_alloc += RBG_arr[i];
			}
		} 

 		break;
	case UERX_RE_ALLOC_TYPE1:
	
		*pdsch_rb_alloc = 0;

		subRBG_bitnum	= (P <= 2)? 1: 2;									// ceil(log2(P))
		sub_RBG_idx		= (P <= 2)? inbuf[0]: (inbuf[0] * 2 + inbuf[1]);

		span_flag		= inbuf[subRBG_bitnum];
	
		N_RB_TYPE1		= n_RBG - subRBG_bitnum - 1;

		n_RBGsubset_arr	= malloc(sizeof(int)*N_RB_TYPE1);//new int[N_RB_TYPE1];
		memset(n_RBGsubset_arr, 0, N_RB_TYPE1 * sizeof(int));

		if (span_flag == 0)
		{
			delta_shift = 0;
		}
		else
		{
			int tmp1 = ( (N_DL_RB - 1) / P) % P;
			int tmp2 = ( (N_DL_RB - 1) / (P * P)) * P;
		
			if (sub_RBG_idx < tmp1)
			{
				N_RBGsubset = tmp2 + P;
			}
			else if (sub_RBG_idx == tmp1)
			{
				N_RBGsubset = tmp2 + (N_DL_RB - 1) % P +1;
			}
			else if (sub_RBG_idx > tmp1)
			{
				N_RBGsubset = tmp2;
			}

			delta_shift = N_RBGsubset - N_RB_TYPE1;
		}

		for (i = 0; i < N_RB_TYPE1; i ++)
		{
			n_RBGsubset_arr[i]	= floor( (i + delta_shift) / P) * P * P + sub_RBG_idx * P + ((i + delta_shift) % P);
		}

		for(i = 0; i < N_RB_TYPE1; i ++)
		{
			if (inbuf[subRBG_bitnum + 1 + i] == 1)
			{
				RB_arr[0][*pdsch_rb_alloc]	= n_RBGsubset_arr[i];
				RB_arr[1][*pdsch_rb_alloc]	= n_RBGsubset_arr[i];
			
				*pdsch_rb_alloc += 1;
			}			
		}
 	
		free(n_RBGsubset_arr);

		break;
	case UERX_RE_ALLOC_TYPE2:
		*pdsch_rb_alloc = 0;
		int16 N_gap;
		if (Local_Distrib_VRB_flag == 0)
		{
			N_VRB_DL = N_DL_RB;
		}
		else if( Local_Distrib_VRB_flag == 1)
		{
			if ( N_gap == (*uerx_dciparse_u).N_gap1)
			{
				N_VRB_DL	= 2 * (( N_gap < N_DL_RB - N_gap)? N_gap: N_DL_RB - N_gap);
				n_VRB_DL_b	= N_VRB_DL;
			}
			else if ( N_gap == (*uerx_dciparse_u).N_gap2)
			{
				N_VRB_DL	= floor(N_DL_RB / (2 * N_gap)) * 2 * N_gap;
				n_VRB_DL_b	= 2 * N_gap;
			}
		}

		// get RIV, RB_start_tmp, L_CRBs_tmp, n_VRB_used_arr, n_VRB_arr
		RIV = 0;
		for (i = 0; i < inlen; i ++)
		{
			RIV += inbuf[i] * (1 << (inlen - 1 - i));
		}

//		PrintInfo(1,"\n\tRIV\t= %d\n\t",RIV);

		RB_start_tmp	= RIV % N_DL_RB;
		
		L_CRBs_tmp		= RIV / N_DL_RB + 1;
		
		if (L_CRBs_tmp <= N_DL_RB / 2 + 1)
		{
			if ((L_CRBs_tmp >= 1) && (L_CRBs_tmp + RB_start_tmp <= N_VRB_DL ))
			{
//				PrintInfo(1,"\n\tRB_start\t= %d;\tL_CRBs\t=%d;\n\t",RB_start_tmp, L_CRBs_tmp);
			}
			else
			{
				RB_start_tmp	= N_DL_RB - 1 - (RIV % N_DL_RB);
				L_CRBs_tmp		= N_DL_RB + 1 - RIV / N_DL_RB;				
				
	/*			if (L_CRBs_tmp - 1 > N_DL_RB / 2)
				{
					if ((L_CRBs_tmp >= 1) && (L_CRBs_tmp + RB_start_tmp <= N_VRB_DL))
					{
						PrintInfo(1,"\n\tRB_start\t= %d;\tL_CRBs\t=%d;\n\t",RB_start_tmp, L_CRBs_tmp);
					}
				}
				else
				{
					PrintInfo(1,"\n\tRIV error!\n\t");
				}*/
			}
		}
		else
		{
			RB_start_tmp	= N_DL_RB - 1 - (RIV % N_DL_RB);
			L_CRBs_tmp		= N_DL_RB + 1 - RIV / N_DL_RB;				
			
/*			if (L_CRBs_tmp - 1 > N_DL_RB / 2)
			{
				if ((L_CRBs_tmp >= 1) && (L_CRBs_tmp + RB_start_tmp <= N_VRB_DL))
				{
					for (i = 0 ; i < L_CRBs_tmp; i ++ )
					{
						PrintInfo(1,"\n\tRB_start\t= %d;\tL_CRBs\t=%d;\n\t",RB_start_tmp, L_CRBs_tmp);
					}
				}
			}
			else
			{
				PrintInfo(1,"\n\tRIV error!\n\t");
			}*/
		}

		for (i = 0; i < N_VRB_DL ; i ++)
		{
			n_VRB_arr[i]	= i;
		}

		// VRB index interleaver

		if (Local_Distrib_VRB_flag == 1)
		{
			num_interl	= N_VRB_DL / n_VRB_DL_b;
			n_row		= ceil(n_VRB_DL_b / (4.0 * P)) * P;
			n_col		= 4;
			n_null		= 4 * n_row - n_VRB_DL_b;

			interl_arr = malloc(sizeof(int)*n_row);//new int *[n_row];
			for (i = 0; i < n_row; i ++)
			{
				interl_arr[i]	= malloc(sizeof(int)*4);//new int[4];
				memset(interl_arr[i], 0, 4 * sizeof(int));
			}

			for (interl_block_idx = 0; interl_block_idx < num_interl; interl_block_idx ++)
			{
				for (i = 0; i < n_row; i ++)
				{
					for (j = 0; j < 4; j ++)
					{
						if ( (i >= n_row - floor(n_null / 2)) && (j % 2 == 1))
						{
							interl_arr[i][j] = -1;
						}
						else
						{
							interl_arr[i][j]	= n_VRB_arr[k_count];
							k_count ++;
						}
					}
				}

				for (j = 0; j < 4; j++ )
				{
					for (i = 0; i < n_row; i ++)
					{
						if ( (i >= n_row - floor(n_null / 2)) && (j % 2 == 1))
						{
						}
						else
						{
							n_VRB_intl_arr[m_count] = interl_arr[i][j];
							m_count	++;
						}
					}
				}
			}

  			// after interleaver 

			// n_VRB ---> n_PRB_b

			for (i = 0; i < N_VRB_DL; i ++)
			{
				n_VRB		= n_VRB_intl_arr[i];

				n_VRB_b		= n_VRB % n_VRB_DL_b;

				n_PRB_p		= 2 * n_row * (n_VRB_b % 2) + n_VRB_b / 2 + n_VRB_DL_b * (n_VRB / n_VRB_DL_b);
				
				n_PRB_p_p	= n_row * (n_VRB_b % 4) + n_VRB_b / 4 + n_VRB_DL_b * (n_VRB / n_VRB_DL_b);
							
				if ((n_null != 0) && ( n_VRB_b >= n_VRB_DL_b - n_null) && (n_VRB_b % 2 == 1))
				{
					n_PRB_b_arr[0][i] =  n_PRB_p - n_row;				
				}
				else if ((n_null != 0) && ( n_VRB_b >= n_VRB_DL_b - n_null) && (n_VRB_b % 2 == 0))
				{
					n_PRB_b_arr[0][i] =  n_PRB_p - n_row + n_null / 2;				
				}
				else if ((n_null != 0) && ( n_VRB_b < n_VRB_DL_b - n_null) && (n_VRB_b % 4 >= 2))
				{
					n_PRB_b_arr[0][i] =  n_PRB_p_p - n_null / 2;		
				}
				else
				{
					n_PRB_b_arr[0][i] =  n_PRB_p_p;
				}
			}
		
			for (i = 0; i < N_VRB_DL; i ++)
			{
				n_VRB		= n_VRB_intl_arr[i];

				n_PRB_b_arr[1][i]	= (n_PRB_b_arr[0][i] + n_VRB_DL_b / 2) % n_VRB_DL_b + n_VRB_DL_b * (n_VRB / n_VRB_DL_b);
			}

 			for (i = 0; i < 2; i ++)
			{
				for (j = 0; j < N_VRB_DL ; j ++)
				{
					if (n_PRB_b_arr[i][j] < n_VRB_DL_b / 2)
					{
						n_PRB_arr[i][j]	= n_PRB_b_arr[i][j];
					}
					else
					{
						n_PRB_arr[i][j]	= n_PRB_b_arr[i][j] + N_gap - n_VRB_DL_b / 2;
					}
				}

				for (j = 0; j < N_VRB_DL; j ++)
				{
					if ((n_PRB_b_arr[i][j] >= RB_start_tmp) && (n_PRB_b_arr[i][j] < RB_start_tmp + L_CRBs_tmp )) 
					{
 						RB_arr[i][ n_PRB_b_arr[i][j] ] = j;
					}
				}
			}
		}
		else
		{
			for (i = 0; i < 2; i ++)
			{
				for (j = 0; j < N_VRB_DL; j ++)
				{
					if ((j >= RB_start_tmp) && (j < RB_start_tmp + L_CRBs_tmp )) 
					{
 						RB_arr[i][ j - RB_start_tmp ] = 1;//j; //modify
					}
				}
			}

		}

		*pdsch_rb_alloc	= L_CRBs_tmp;

 
		for (i = 0; i < n_row; i ++)
		{
			free(interl_arr[i]);
		}
		free(interl_arr);

		break;
	default:
		;
	}
	return 0;
}

int MCS_interpret1(uerx_dci_parse  *uerx_dciparse_u,int16 MCS[], int inlen, int16 transblock_idx, int16 RNTI_type, int16 DCI_type,
		int16 PRB_num, int16 TB2layer[2], int TBS[2], int16 qm[2], int16 diver_spatial)
{	
	int MCS_idx = bit2dec(MCS, inlen, UERX_LOWERBITS_FIRST);
	int i, tbs_idx;	
	int tbs_temp, columnidx;
	
	// get qm and the transblock size idx
	if (RNTI_type == UERX_P_RNTI || RNTI_type == UERX_RA_RNTI || RNTI_type == UERX_SI_RNTI)
	{
		qm[transblock_idx]						= 2;
		
		if (DCI_type == UERX_DCI_1A || DCI_type == UERX_DCI_1C)
			tbs_idx								= MCS_idx;
		else
		{
//			PrintInfo(1,"RNTI_type and DCI format error!!");
			return 1;
		}
	}
	else
	{
		qm[transblock_idx]						= (*uerx_dciparse_u).QAM_TBSidx_table[MCS_idx][0];
		
		if (MCS_idx >= 29 )
		{
			if ( (*uerx_dciparse_u).TBS_last[transblock_idx] == -1)
			{
//				PrintInfo(1, "The 1st Transport block size error! \n");
				return 1;
			}
		}
		tbs_idx									= (*uerx_dciparse_u).QAM_TBSidx_table[MCS_idx][1];
	}
	
	// get the transblock size
	if (DCI_type == UERX_DCI_1C)
	{
		TBS[transblock_idx]					= UERX_TBS_DCI_1C[tbs_idx];
		(*uerx_dciparse_u).TBS_last[transblock_idx]			= TBS[transblock_idx];
	}
	else
	{
		if (MCS_idx < 29)
		{
			// get the transblock size
			if (!(TB2layer[transblock_idx] == 2 && (diver_spatial == UERX_OPENLOOP_SM_PRECOD 
				|| diver_spatial == UERX_CLOSLOOP_SM_PRECOD)))
			{
				TBS[transblock_idx]			= (*uerx_dciparse_u).TBS_table1[tbs_idx][PRB_num - 1];
			}
			else
			{			
				if (PRB_num <= 55)
				{
					TBS[transblock_idx] 	= (*uerx_dciparse_u).TBS_table1[tbs_idx][2 * PRB_num - 1];
				}
				else
				{
					tbs_temp				= (*uerx_dciparse_u).TBS_table1[tbs_idx][PRB_num - 1];
					columnidx = (tbs_temp <= 3624)? 0:
					(tbs_temp <= 9912)? 2:
					(tbs_temp <= 27376)? 4: 6;
					
					for (i = 0; i < 28; i++)
					{
						if ((*uerx_dciparse_u).TBS_table2[i][columnidx] == tbs_temp)
						{
							TBS[transblock_idx]	= (*uerx_dciparse_u).TBS_table2[i][columnidx + 1];
						}
					}					
				}
			}
			
			(*uerx_dciparse_u).TBS_last[transblock_idx] = 	TBS[transblock_idx];
		}
		else
		{
			TBS[transblock_idx] = (*uerx_dciparse_u).TBS_last[transblock_idx];
		}	
	}	
	
	return 0;
}

int MCS_interpret2(uerx_dci_parse  *uerx_dciparse_u,int16 MCS_index, int16 N_RB, int16 NL, int *transport_block, int16 *Qm)
{
	int i;
	int TBS_index;
	int tbs_temp;
	int columnidx;
	
    *Qm = (*uerx_dciparse_u).QAM_TBSidx_table[MCS_index][0];
	
	TBS_index = (*uerx_dciparse_u).QAM_TBSidx_table[MCS_index][1];
	
	if (NL == 1)
		*transport_block = (*uerx_dciparse_u).TBS_table1[TBS_index][N_RB-1];
	else
	{
		if (N_RB <= 55)
			*transport_block = (*uerx_dciparse_u).TBS_table1[TBS_index][N_RB-1];
		else
		{
            tbs_temp = (*uerx_dciparse_u).TBS_table1[TBS_index][N_RB-1];
			
			columnidx = (tbs_temp <= 3624)? 0:
			(tbs_temp <= 9912)? 2:
			(tbs_temp <= 27376)? 4: 6;
			
			for (i = 0; i < 28; i++)
			{
				if ((*uerx_dciparse_u).TBS_table2[i][columnidx] == tbs_temp)
				{
					*transport_block = (*uerx_dciparse_u).TBS_table2[i][columnidx + 1];
				}
			}
		}		
	}
	
	return 0;
}


int RV_interpret(int16 RV_arr[], int16 DCI_type, int16 tbs_idx, int inlen, int16 RNTI_type, int16 RV[2])
{
	if ((DCI_type == UERX_DCI_1C) && (RNTI_type == UERX_P_RNTI || RNTI_type == UERX_RA_RNTI ))
	{
		RV[tbs_idx]	= 0;
	}
	else if((DCI_type == UERX_DCI_1C) && ( RNTI_type == UERX_SI_RNTI))
	{
		 RV[tbs_idx] = 0;  // ?????  according to 36.321
	}
	else
	{
		 RV[tbs_idx] = bit2dec(RV_arr, inlen, UERX_LOWERBITS_FIRST);
	}
	
	return 0;
}


int bit2dec(int16 inbuf[], int inlen, int16 type)
{
	int out = 0;
	int i;
	
	if (type == UERX_LOWERBITS_FIRST)
	{
		for (i = 0; i < inlen; i++)
		{
			out += inbuf[i] << (inlen - 1 - i);
		}
		
	}
	
	if (type == UERX_HIGHERBITS_FIRST)
	{
		for (i = 0; i < inlen; i++)
		{
			out += inbuf[i] << i;
		}
		
	}
	
	return out;
}

int Precoding_Info_interpret(int16 inbuf[], int inlen, int16 codeword_num, int16 RI_last, int16 PMI_last,
											  int16 *layers_total, int16 * diversity_spatial, int16 *cobook_index, int16 *cobook_column_idx)
{
	int idx = bit2dec(inbuf, inlen, UERX_LOWERBITS_FIRST);
	
	if (inlen == 3) // 2 antenna ports, 36.212 Table 5.3.3.1.5-3
	{
		if ( codeword_num == 1) // One codeword
		{
			switch (idx)
			{
			case 0:
				*layers_total			= 2;
				 *diversity_spatial		= UERX_SFBC_PRECOD;
				break;
			case 1:
				 *layers_total			= 1;
				 *diversity_spatial		= UERX_CLOSLOOP_SM_PRECOD;
				 *cobook_index			= 0;
				break;
			case 2:
				 *layers_total			= 1;
				 *diversity_spatial		= UERX_CLOSLOOP_SM_PRECOD;
				 *cobook_index			= 1;
				break;
			case 3:
				 *layers_total			= 1;
				 *diversity_spatial		= UERX_CLOSLOOP_SM_PRECOD;
				 *cobook_index			= 2;
				break;
			case 4:
				 *layers_total			= 1;
				 *diversity_spatial		= UERX_CLOSLOOP_SM_PRECOD;
				 *cobook_index			= 3;
				break;
			case 5:
				 *layers_total			= 1;
				 *diversity_spatial		= UERX_CLOSLOOP_SM_PRECOD;
				 *cobook_index			=  PMI_last;
				if (RI_last == 2)
					 *cobook_column_idx	= 0;
				break;
			case 6:
				 *layers_total			= 1;
				 *diversity_spatial		= UERX_CLOSLOOP_SM_PRECOD;
				 *cobook_index			=  PMI_last;
				if ( RI_last == 2)
					 *cobook_column_idx	= 1;
				break;
			case 7:
//				PrintInfo(1,"TX_ant_num = 2:  DCI 2: idx =%d, Precoding_info Reserved!!", idx);
				return 1;
			default:
				break;				
			}	
		}
		else // Two codewords
		{	 
			switch (idx)
			{
			case 0:
				 *layers_total			= 2;
				* diversity_spatial		= UERX_CLOSLOOP_SM_PRECOD;
				 *cobook_index			= 1;
				
				break;
			case 1:
				 *layers_total			= 2;
				 *diversity_spatial		= UERX_CLOSLOOP_SM_PRECOD;
				 *cobook_index			= 2;
				break;
			case 2:
				 *layers_total			= 2; // linjiangnan 20141218
				 *diversity_spatial		= UERX_CLOSLOOP_SM_PRECOD;
				 *cobook_index			=  PMI_last;
				break;
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
//				PrintInfo(1,"TX_ant_num = 2:  DCI 2: idx =%d, Precoding_info Reserved!!", idx);
				assert (0); // linjiangnan
			default:
				break;
			}
		}
	}
	else if(inlen == 6) // 4 antenna ports, 36.212 Table 5.3.3.1.5-3
	{
		if ( codeword_num == 1) // One codeword
		{
			if (idx >= 35)
			{
//				PrintInfo(1,"TX_ant_num = 4: idx =%d, Precoding_info Reserved!!", idx);
				return 1;
			}
			 *layers_total			=	(idx == 0)? 4:
			(idx >= 1 && idx <= 17)? 1:
			(idx >= 18 && idx <=34)? 2: 0;
			
			* diversity_spatial		= (idx == 0)? UERX_SFBC_PRECOD: UERX_CLOSLOOP_SM_PRECOD;
			
			 *cobook_index			=	(idx >= 1 && idx <= 16)?  (idx - 1):
			(idx == 17 || idx == 34)?  PMI_last:
			(idx >= 18 && idx <=33)? (idx - 18): 0;
		}
		else // Two codewords
		{
			
			if (idx >= 51)
			{
//				PrintInfo(1,"TX_ant_num = 4:   idx =%d, Precoding_info Reserved!!\n", idx);
				return 1;
				
			}
			 *layers_total			=	(idx >= 0  && idx <= 16)? 2:
			(idx >= 17 && idx <= 33)? 3:
			(idx >= 34 && idx <= 50)? 4: 0;
			
			 *diversity_spatial		=   UERX_CLOSLOOP_SM_PRECOD;
			
			
			 *cobook_index			=	(idx >= 0  && idx <= 15)?  idx:
			(idx == 16 || idx == 33)?  PMI_last:
			(idx >= 17 && idx <= 32)?  (idx - 17):
			(idx >= 34 && idx <= 49)?  (idx - 34): 0;			
		}
	}
	else if(inlen == 2) // 4 antenna ports 36.212 Table 5.3.3.1.5A-2
	{
		if ( codeword_num == 1) // One codeword
		{
			if (idx >= 2)
			{
//				PrintInfo(1,"DCI 2A: Precoding info bits Reserved!!\n");
				return 1;
			}
			*diversity_spatial		=   (idx ==0)? UERX_SFBC_PRECOD:
			(idx == 1)? UERX_OPENLOOP_SM_PRECOD: 0;
			 *layers_total			=	(idx == 0)? 4:
			(idx == 1)? 2:0;
		}
		else // Two codewords
		{
			if (idx == 3)
			{
//				PrintInfo(1,"DCI 2A: Precoding info bits Reserved!!\n");
				return 1;
			}
			 *diversity_spatial		=   UERX_OPENLOOP_SM_PRECOD;
			 *layers_total			=	idx + 2;
			
		}
	}
	
	return 0;
}

 int DCI1A_Is_Randacces(int16 * inbuf, int type2_len, int DCI1A_len)
 {
 	int i ;
 	int cur_len = 0;

 	if (inbuf[0] != 0)
 	{
		//PrintInfo(1, "UERX_DCI_1A for Random access Parser error! Local_Distrib_VRB_flag !=0 ");
		return 0;
 	}
 	cur_len ++;

 	for (i = cur_len; i < cur_len + type2_len; i++)
 	{
 		if (inbuf[i] != 1)
		{
		//	PrintInfo(1, "UERX_DCI_1A for Random access Parser error! RB_assig_inf[%d] != 1 ", i); 			return 0;
 		}
 	}
 	cur_len += type2_len;

 	cur_len += 10;

 	for (i = cur_len; i < DCI1A_len; i++)
 	{
 		if (inbuf[i] != 0)
 		{
 //			PrintInfo(1, "UERX_DCI_1A for Random access Parser error! RB_assig_inf[%d] != 1 ", i);
 			return 0;
 		}
 	}

 	return 1;
 }

