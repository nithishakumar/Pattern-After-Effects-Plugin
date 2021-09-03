/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2007 Adobe Systems Incorporated                       */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Systems Incorporated and its suppliers, if    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Systems Incorporated and its    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Systems         */
/* Incorporated.                                                   */
/*                                                                 */
/*******************************************************************/

#pragma once

#ifndef Pattern_H
#define Pattern_H

typedef unsigned char		u_char;
typedef unsigned short		u_short;
typedef unsigned short		u_int16;
typedef unsigned long		u_long;
typedef short int			int16;
#define PF_TABLE_BITS	12
#define PF_TABLE_SZ_16	4096

#define PF_DEEP_COLOR_AWARE 1	// make sure we get 16bpc pixels; 
								// AE_Effect.h checks for this.

#include "AEConfig.h"

#ifdef AE_OS_WIN
	typedef unsigned short PixelType;
	#include <Windows.h>
#endif

#include "entry.h"
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_Macros.h"
#include "Param_Utils.h"
#include "AE_EffectCBSuites.h"
#include "String_Utils.h"
#include "AE_GeneralPlug.h"
#include "AEFX_ChannelDepthTpl.h"
#include "AEGP_SuiteHandler.h"
#define CAIRO_WIN32_STATIC_BUILD
#include "cairo.h"
#include "Pattern_Strings.h"

/* Versioning information */

#define	MAJOR_VERSION	1
#define	MINOR_VERSION	0
#define	BUG_VERSION		0
#define	STAGE_VERSION	PF_Stage_DEVELOP
#define	BUILD_VERSION	1


/* Parameter defaults */

#define	Pattern_ANGLE_MIN		 0
#define	Pattern_ANGLE_MAX		 90
#define	Pattern_ANGLE_DFLT		 60
#define	Pattern_DELTA_DFLT		 0
#define	Pattern_DELTA_MIN		 0
#define	Pattern_DELTA_MAX		 200
#define Pattern_STROKE_MIN       1
#define Pattern_STROKE_MAX       10
#define Pattern_STROKE_DFLT      3
#define Pattern_SIDELENGTH_MIN   60
#define Pattern_SIDELENGTH_MAX   250
#define Pattern_SIDELENGTH_DFLT  120
#define Pattern_NUMSIDES_MIN     3
#define Pattern_NUMSIDES_MAX     45
#define Pattern_NUMSIDES_DFLT    6
	
enum {
	TILING_666 = 1,
	TILING_FILLER1,
	TILING_444,
	TILING_FILLER2,
	TILING_488,
	TILING_FILLER3,
	TILING_31212,
	TILING_FILLER4,
	TILING_4612,
	TILING_SIZE = TILING_4612
};

enum {
	MODE_CENTERED = 1,
	MODE_FILLER1,
	MODE_MULTIPLE,
	MODE_SIZE = MODE_MULTIPLE
};

enum {
	DELTA_TOWARDS = 1,
	DELTA_FILLER1,
	DELTA_AWAY,
	DELTA_SIZE = DELTA_AWAY
};

enum {
	Pattern_INPUT = 0,
	Pattern_BGCOLOR,
	Pattern_CHOOSEMODE,
	Pattern_CHOOSETILING,
	Pattern_SIDELENGTH,
	Pattern_NUMSIDES,
	Pattern_ANGLE,
	Pattern_CHOOSEDELTA,
	Pattern_DELTA,
	Pattern_HIDETILINGCHKBOX,
	Pattern_PATTERNCOLOR,
	Pattern_PATTERNWIDTH,
	Pattern_NUM_PARAMS 
};

enum {
	BGCOLOR_ID = 1,
	CHOOSEMODE_ID,
	CHOOSETILING_ID,
	SIDELENGTH_ID,
	NUMSIDES_ID,
	ANGLE_ID,
	CHOOSEDELTA_ID,
	DELTA_ID,
	HIDETILINGCHKBOX_ID,
	LINECOLOR_ID,
	LINEWIDTH_ID,
};

// Struct to store cairo data in AE renderable form:
struct cairoRefcon {
	PF_EffectWorld output;
	unsigned char* data;
	int stride;
};



extern "C" {

	DllExport
	PF_Err
	EffectMain(
		PF_Cmd			cmd,
		PF_InData		*in_data,
		PF_OutData		*out_data,
		PF_ParamDef		*params[],
		PF_LayerDef		*output,
		void			*extra);

}

#endif // Pattern_H
