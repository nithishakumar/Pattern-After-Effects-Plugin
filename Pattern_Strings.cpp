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

#include "Pattern.h"

struct TableString {
	A_u_long	index;
	A_char		str[256];
};



TableString		g_strs[StrID_NUMTYPES] = {
	StrID_NONE,						"",
	StrID_Name,					     "Pattern",
	StrID_Description,			   	 "Creates Islamic Star Pattern Animations from Polygons in Contact",
	StrID_BgColor_Param_Name,        "Background Color",
	StrID_ChooseMode_Param_Name,     "Mode",
	StrID_ModeChoices_Param_Name,    "Centered |"
								     "(-|"
								     "Multiple ",
    StrID_ChooseTiling_Param_Name,   "Tiling",
	StrID_TilingChoices_Param_Name,  "6.6.6 |"
									 "(-|"
									 "4.4.4 |"
									 "(-|"
									 "4.8.8 |"
									 "(-|"
									 "3.12.12 |"
									 "(-|"
									 "4.6.12 |",
	StrID_SideLength_Param_Name,     "Side Length",
	StrID_NumberofSides_Param_Name,  "Number of Sides",
	StrID_Angle_Param_Name,          "Angle (Degrees)",
	StrID_ChooseDelta_Param_Name,    "Delta Mode",
	StrID_DeltaChoices_Param_Name,   "Towards |"
									 "(-|"
									 "Away ",
	StrID_Delta_Param_Name,          "Delta",
	StrID_HideTiling_Param_Name,     "Hide Tiling",
	StrID_TColorChkBox_Param_Name,   "Show Tiling Background",
	StrID_TBgColor_Param_Name,       "Tiling Background Color",
	StrID_LineColor_Param_Name,      "Pattern Color",
	StrID_LineWidth_Param_Name,      "Pattern Weight",
};


char	*GetStringPtr(int strNum)
{
	return g_strs[strNum].str;
}
	