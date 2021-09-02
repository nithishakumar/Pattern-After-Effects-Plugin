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

#include "Draw.h"
#include "Color.h"

static PF_Err 
About (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	AEGP_SuiteHandler suites(in_data->pica_basicP);
	
	suites.ANSICallbacksSuite1()->sprintf(	out_data->return_msg,
											"%s v%d.%d\r%s",
											STR(StrID_Name), 
											MAJOR_VERSION, 
											MINOR_VERSION, 
											STR(StrID_Description));
	return PF_Err_NONE;
}

static PF_Err 
GlobalSetup (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	out_data->my_version = PF_VERSION(	MAJOR_VERSION, 
										MINOR_VERSION,
										BUG_VERSION, 
										STAGE_VERSION, 
										BUILD_VERSION);

	out_data->out_flags =  PF_OutFlag_DEEP_COLOR_AWARE;	// just 16bpc, not 32bpc
	
	return PF_Err_NONE;
}

static PF_Err 
ParamsSetup (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err		err		= PF_Err_NONE;
	PF_ParamDef	def;	

	AEFX_CLR_STRUCT(def);

	
	// Calling function from the Color static
	// library to generate a color combination through
	// machine learning.
	color ob;
	vector<pixel> color_combo = ob.generate_color_combo();

	// Background Color:
	PF_ADD_COLOR(STR(StrID_BgColor_Param_Name),
		color_combo[1].red,
		color_combo[1].green,
		color_combo[1].blue,
		BGCOLOR_ID);

	AEFX_CLR_STRUCT(def);

	// Choose Mode:
	PF_ADD_POPUPX(STR(StrID_ChooseMode_Param_Name),
		              MODE_SIZE, MODE_MULTIPLE, 
		              STR(StrID_ModeChoices_Param_Name),
		              PF_ParamFlag_SUPERVISE, 
	               	  CHOOSEMODE_ID);
			 
	AEFX_CLR_STRUCT(def);

	// Choose type of tiling in multiple mode:
	PF_ADD_POPUPX(STR(StrID_ChooseTiling_Param_Name),
		TILING_SIZE, TILING_666,
		STR(StrID_TilingChoices_Param_Name),
		0,
		CHOOSETILING_ID);

	AEFX_CLR_STRUCT(def)

	// Side Length:
	PF_ADD_FLOAT_SLIDERX(STR(StrID_SideLength_Param_Name),
		Pattern_SIDELENGTH_MIN,
		Pattern_SIDELENGTH_MAX,
		Pattern_SIDELENGTH_MIN,
		Pattern_SIDELENGTH_MAX,
		Pattern_SIDELENGTH_DFLT,
		PF_Precision_THOUSANDTHS,
		0,
		PF_PUI_INVISIBLE,
		SIDELENGTH_ID);
	
	AEFX_CLR_STRUCT(def);

	// Number of sides:
	PF_ADD_FLOAT_SLIDERX(STR(StrID_NumberofSides_Param_Name),
		Pattern_NUMSIDES_MIN,
		Pattern_NUMSIDES_MAX,
		Pattern_NUMSIDES_MIN,
		Pattern_NUMSIDES_MAX,
		Pattern_NUMSIDES_DFLT,
	    PF_Precision_INTEGER,
		0,
		0,
		NUMSIDES_ID);

	AEFX_CLR_STRUCT(def);

	// Choose angle:
	PF_ADD_FLOAT_SLIDERX(STR(StrID_Angle_Param_Name),
		Pattern_ANGLE_MIN,
		Pattern_ANGLE_MAX,
		Pattern_ANGLE_MIN,
		Pattern_ANGLE_MAX,
		Pattern_ANGLE_DFLT,
		PF_Precision_THOUSANDTHS,
		0,
		0,
		ANGLE_ID);

	AEFX_CLR_STRUCT(def);

	// Choose delta mode:
	PF_ADD_POPUPX(STR(StrID_ChooseDelta_Param_Name),
		DELTA_SIZE, DELTA_TOWARDS,
		STR(StrID_DeltaChoices_Param_Name),
		0,
		CHOOSEDELTA_ID);

	// Choose delta value:
	PF_ADD_FLOAT_SLIDERX(STR(StrID_Delta_Param_Name),
		Pattern_DELTA_MIN,
		Pattern_DELTA_MAX,
		Pattern_DELTA_MIN,
		Pattern_DELTA_MAX,
		Pattern_DELTA_DFLT,
		PF_Precision_THOUSANDTHS,
		0,
		0,
		DELTA_ID);


	AEFX_CLR_STRUCT(def);

	// Hide Tiling:
	PF_ADD_CHECKBOXX(STR(StrID_HideTiling_Param_Name), 
		             true, 0, HIDETILINGCHKBOX_ID);

	AEFX_CLR_STRUCT(def);

	// Choose Line Stroke Color:
	PF_ADD_COLOR(STR(StrID_LineColor_Param_Name), color_combo[0].red, 
		    color_combo[0].green, color_combo[0].blue, LINECOLOR_ID);

	AEFX_CLR_STRUCT(def);

	// Choose Line Stroke Width:
	PF_ADD_FLOAT_SLIDERX(STR(StrID_LineWidth_Param_Name),
		Pattern_STROKE_MIN,
		Pattern_STROKE_MAX,
		Pattern_STROKE_MIN,
		Pattern_STROKE_MAX,
		Pattern_STROKE_DFLT,
		PF_Precision_HUNDREDTHS,
		0,
		0,
		LINEWIDTH_ID);

	
	out_data->num_params = Pattern_NUM_PARAMS;

	return err;
}

static PF_Err
MakeParamCopy(
	PF_ParamDef* actual[],	/* >> */
	PF_ParamDef copy[])		/* << */
{
	for (A_short iS = 0; iS < Pattern_NUM_PARAMS; ++iS) {
		AEFX_CLR_STRUCT(copy[iS]);	// clean params are important!
	}

	copy[Pattern_INPUT] = *actual[Pattern_INPUT];
	copy[Pattern_BGCOLOR] = *actual[Pattern_BGCOLOR];
	copy[Pattern_CHOOSEMODE] = *actual[Pattern_CHOOSEMODE];
	copy[Pattern_CHOOSETILING] = *actual[Pattern_CHOOSETILING];
	copy[Pattern_SIDELENGTH] = *actual[Pattern_SIDELENGTH];
	copy[Pattern_NUMSIDES] = *actual[Pattern_NUMSIDES];
	copy[Pattern_ANGLE] = *actual[Pattern_ANGLE];
	copy[Pattern_CHOOSEDELTA] = *actual[Pattern_CHOOSEDELTA];
	copy[Pattern_DELTA] = *actual[Pattern_DELTA];
	copy[Pattern_HIDETILINGCHKBOX] = *actual[Pattern_HIDETILINGCHKBOX];
	copy[Pattern_PATTERNCOLOR] = *actual[Pattern_PATTERNCOLOR];
	copy[Pattern_PATTERNWIDTH] = *actual[Pattern_PATTERNWIDTH];

	return PF_Err_NONE;

}

static PF_Err
UpdateParameterUI(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output)
{	PF_Err				err = PF_Err_NONE;
	AEGP_StreamRefH		SideLength_streamH = NULL, numSides_streamH = NULL,
		                Tiling_streamH = NULL;
	AEGP_SuiteHandler	suites(in_data->pica_basicP);
	AEGP_EffectRefH	    meH = NULL;

	// Copying parameters before changing their state:
	PF_ParamDef		param_copy[Pattern_NUM_PARAMS];
	ERR(MakeParamCopy(params, param_copy));


	if (params[Pattern_CHOOSEMODE]->u.pd.value == MODE_CENTERED) {

		ERR(suites.PFInterfaceSuite1()->AEGP_GetNewEffectForEffect(NULL, 
			                                  in_data->effect_ref, &meH));

		// Unhide number of sides:
		ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(NULL, meH, 
			                            Pattern_NUMSIDES, &numSides_streamH));
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(numSides_streamH,
			                            AEGP_DynStreamFlag_HIDDEN, FALSE, FALSE));

		// Hide tiling choices:
		ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(NULL, meH,
			Pattern_CHOOSETILING, &Tiling_streamH));
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(Tiling_streamH,
			AEGP_DynStreamFlag_HIDDEN, FALSE, TRUE));
	}

	if (params[Pattern_CHOOSEMODE]->u.pd.value == MODE_MULTIPLE) {

		ERR(suites.PFInterfaceSuite1()->AEGP_GetNewEffectForEffect(NULL, 
			                                  in_data->effect_ref, &meH));

		// Hide number of sides:
		ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(NULL, meH,
			                            Pattern_NUMSIDES, &numSides_streamH));
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(numSides_streamH,
			                                 AEGP_DynStreamFlag_HIDDEN, FALSE, TRUE));
		// Unhide tiling choices:
		ERR(suites.StreamSuite2()->AEGP_GetNewEffectStreamByIndex(NULL, meH,
			Pattern_CHOOSETILING, &Tiling_streamH));
		ERR(suites.DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(Tiling_streamH,
			AEGP_DynStreamFlag_HIDDEN, FALSE, FALSE));

	}

	return err;
}


// The functions below translate cairo graphics to AE renderable form:

PF_Pixel* sampleIntegral8(PF_EffectWorld& def, int x, int y) 
{
	return (PF_Pixel*)((char*)def.data + (y * def.rowbytes)
		   + (x * sizeof(PF_Pixel)));
}

PF_Err cairoCopy8(void* refcon, A_long threadInd, A_long itemNum,
	              A_long iterTotal) 
{
	cairoRefcon* data = (cairoRefcon*)refcon;
	int i = itemNum;
	PF_Err err = PF_Err_NONE;
	uint32_t* rowP;
	PF_Pixel8* worldPtr = sampleIntegral8(data->output, 0, i);
	rowP = (uint32_t*)(data->data + i * data->stride);
	for (int x = 0; x < data->output.width; x++) {
		worldPtr->alpha = rowP[x] >> 24;
		if (worldPtr->alpha) 
		{
			worldPtr->red = rowP[x] >> 16;
			worldPtr->green = rowP[x] >> 8;
			worldPtr->blue = rowP[x] >> 0;
		}
		worldPtr++;
	}

	return err;
}


static PF_Err 
Render (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err				err		= PF_Err_NONE;
	AEGP_SuiteHandler	suites(in_data->pica_basicP);
	PF_FILL(NULL, NULL, output);

	cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 
		                                        output->width, output->height);
	cairo_t* cr = cairo_create(surface);

	draw ob(cr, output, params);
	ob.pattern();

    cairoRefcon crefcon;
	AEFX_CLR_STRUCT(crefcon);
	crefcon.data = cairo_image_surface_get_data(surface);
	crefcon.stride = cairo_image_surface_get_stride(surface);
	crefcon.output = *output;
	ERR(suites.IterateSuite1()->AEGP_IterateGeneric(output->height,
		                                           &crefcon, cairoCopy8));
	return err;
}


extern "C" DllExport
PF_Err PluginDataEntryFunction(
	PF_PluginDataPtr inPtr,
	PF_PluginDataCB inPluginDataCallBackPtr,
	SPBasicSuite* inSPBasicSuitePtr,
	const char* inHostName,
	const char* inHostVersion)
{
	PF_Err result = PF_Err_INVALID_CALLBACK;

	result = PF_REGISTER_EFFECT(
		inPtr,
		inPluginDataCallBackPtr,
		"Pattern", // Name
		"ADBE Pattern", // Match Name
		"Motion Graphics Plug-ins", // Category
		AE_RESERVED_INFO); // Reserved Info

	return result;
}


PF_Err
EffectMain(
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extra)
{
	PF_Err		err = PF_Err_NONE;
	
	try {
		switch (cmd) {
			case PF_Cmd_ABOUT:

				err = About(in_data,
							out_data,
							params,
							output);
				break;
				
			case PF_Cmd_GLOBAL_SETUP:

				err = GlobalSetup(	in_data,
									out_data,
									params,
									output);
				break;
				
			case PF_Cmd_PARAMS_SETUP:

				err = ParamsSetup(	in_data,
									out_data,
									params,
									output);
				break;
				
			case PF_Cmd_RENDER:

				err = Render(	in_data,
								out_data,
								params,
								output);
				break;

			case PF_Cmd_UPDATE_PARAMS_UI:
				err = UpdateParameterUI(in_data,
					out_data,
					params,
					output);
				break;

			default:
				break;

		}
	}
	catch(PF_Err &thrown_err){
		err = thrown_err;
	}
	return err;
}

