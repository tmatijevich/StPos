 /*******************************************************************************
 * File:      SuperTrakGlobalPosition.c
 * Author:    Tyler Matijevich
 * Date:      2021-05-22
*******************************************************************************/

#include "StPosMain.h"

/* Return the global reference position on the SuperTrak loop */
signed long SuperTrakGlobalPosition(unsigned char section, signed long sectionPosition, unsigned char originSection, signed long direction, signed long *globalPosition, struct SuperTrakPositionInfoType *info) {
		
	/* Declare local variables */
	signed long sectionLength; /* Store the length of the input section */
	signed long layoutResult;
	
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
	sectionLength = sectionLengths[sectionType[sectionMapping[section]]];
	
	/* Verify the input section position */
	if(sectionPosition < 0 || sectionPosition > sectionLength + SECTION_LENGTH_TOLERANCE) { /* Allow some rollover */ 
		info->section 				= section;
		info->sectionPosition 		= sectionPosition;
		info->sectionPositionMax 	= sectionLength + SECTION_LENGTH_TOLERANCE;
		return stPOS_ERROR_INPUTSECTIONPOS;
	}
	
	/* Derive the global position */
	if(direction == stDIRECTION_RIGHT) {
		if(sectionMapping[section] == endIndex && sectionPosition >= sectionLength) /* Rollover the end section */
			*globalPosition = sectionPosition - sectionLength; /* Allow some rollover */
		else 
			*globalPosition = startingPosition[sectionMapping[section]] + sectionPosition;
	}
	else { /* Left */
		if(sectionMapping[section] == endIndex && sectionPosition == 0) /* Should not be rollover in the negative direction (encoder offset is zero at the first coil) */
			*globalPosition = 0; /* Wrap at global end */
		else if(section == originSection && sectionPosition > sectionLength)
			*globalPosition = totalLength + (sectionLength - sectionPosition); /* Wrap past global end */
		else 
			*globalPosition = startingPosition[sectionMapping[section]] + sectionLength - sectionPosition;
	}
	
	return stPOS_ERROR_NONE;
	
} /* End function */
