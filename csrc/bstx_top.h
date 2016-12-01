#pragma once

#include "bstx_crcattach.h"
#include "bstx_blockseg.h"
#include "bstx_encode.h"
#include "bstx_ratematch.h"
#include "bstx_blockconcate.h"
#include "bstx_mod_pdcch.h"
#include "bstx_scramble.h"
#include "bstx_qammap.h"
#include "bstx_mod_phich.h"
#include "bstx_layermap.h"
#include "bstx_precode.h"
#include "bstx_phyprc.h"
#include "bstx_remap.h"
#include "bstx_ofdm.h"
#include "bstx_payload.h"




//	bstx_phyprc			Bstx_phyprc_u;
//
//	bstx_payload		Bstx_payload_u;
//
//	bstx_crcattach		Bstx_crcattach_u;
//	bstx_blockseg       Bstx_blockseg_u;
//	bstx_encode         Bstx_encode_u;
//	bstx_ratematch      Bstx_ratematch_u;
//	bstx_blockconcate	Bstx_blockconcate_u;
//
//	bstx_mod_pdcch		Bstx_mod_pdcch_u;
//	bstx_scramble		Bstx_scramble_u;
//	bstx_qammap   		Bstx_qammap_u;
//	bstx_mod_phich      Bstx_mod_phich_u;
//	bstx_layermap		Bstx_layermap_u;
//	bstx_precode		Bstx_precode_u;
//	bstx_remap			Bstx_remap_u;
//	bstx_ofdm			Bstx_ofdm_u;
//



	int Init2();
	int Clear2();
	int InputOutput3(int subfrm_idx, int frm_idx, cfloat **outbuf);


