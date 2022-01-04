 /*******************************************************************************
 * File:      SuperTrakGlobalPosition.c
 * Author:    Tyler Matijevich
 * Date:      2021-05-22
*******************************************************************************/

#include "StPosMain.h"

/* Return the global reference position on the SuperTrak loop */
long SuperTrakGlobalPosition(unsigned char section, long sectionPosition, unsigned char originSection, long direction, long *globalPosition, struct SuperTrakPositionInfoType *info) {
		
	/* Declare local variables */
	long layoutResult;
	
	/* Reset the solution */
	*globalPosition = 0;
	
	/* Reset diagnostic information */
	memset(info, 0, sizeof(*info));
	
	/* Re-read the SuperTrak layout */
	if(originSection != previousOriginSection || direction != previousDirection || layoutValid == false) {
		layoutResult = SuperTrakReadLayout(originSection, direction, info);
		if(layoutResult != stPOS_ERROR_NONE)
			return layoutResult;
	}
	
	/* Verify the input section */
	if(section == 0 || section > sectionCount) {
		info->section 		= section;
		info->sectionCount 	= sectionCount;
		return stPOS_ERROR_INPUTSECTION;
	}
	
	/* Verify the input section position */
	if(sectionPosition < 0 || sectionPosition > sectionLength[sectionMapping[section]]) { 
		info->section 				= section;
		info->sectionPosition 		= sectionPosition;
		info->sectionPositionMax 	= sectionLength[sectionMapping[section]];
		return stPOS_ERROR_INPUTSECTIONPOS;
	}
	
	/* Derive the global position */
	if(direction == stDIRECTION_RIGHT) {
		if(sectionMapping[section] == endIndex && sectionPosition == sectionLength[sectionMapping[section]] && layoutLinear == false) /* Rollover the end section */
			*globalPosition = 0;
		else 
			*globalPosition = startingPosition[sectionMapping[section]] + sectionPosition;
	}
	else { /* Left */
		if(sectionMapping[section] == endIndex && sectionPosition == 0 && layoutLinear == false) /* Rollover the end section */
			*globalPosition = 0;
		else 
			*globalPosition = startingPosition[sectionMapping[section]] + sectionLength[sectionMapping[section]] - sectionPosition;
	}
	
	return stPOS_ERROR_NONE;
	
} /* End function */
