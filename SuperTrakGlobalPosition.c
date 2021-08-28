 /*******************************************************************************
 * File:      SuperTrakGlobalPosition.c
 * Author:    Tyler Matijevich
 * Date:      2021-05-22
*******************************************************************************/

#include "StPosMain.h"

/* Return the global reference position on the SuperTrak loop */
signed long SuperTrakGlobalPosition(unsigned char section, signed long sectionPosition, unsigned char originSection, signed long direction, signed long *globalPosition, struct SuperTrakPositionDiagType *diag) {
		
	/* Declare local variables */
	signed long sectionLength; /* Store the length of the input section */
	signed long readResult;
	
	/* Reset diagnostic information */
	diag->ServiceChannelResult_1080 = 0;
	diag->ServiceChannelResult_1081 = 0;
	diag->ServiceChannelResult_1082 = 0;
	diag->SectionCount 		= 0;
	diag->SectionAddress 	= 0;
	diag->SectionNumber 	= 0;
	
	/* Reset the solution, zero if error */
	*globalPosition = 0;
	
	/* Re-read the SuperTrak layout */
	if(originSection != previousOriginSection || direction != previousDirection || readSuccess == false) {
		readResult = SuperTrakReadLayout(originSection, direction, diag);
		if(readResult != stPOS_ERROR_NONE) {
			readSuccess = false;
			return readResult;
		}
		else
			readSuccess = true;
	}
	
	/* Verify the input section */
	if(section == 0 || section > sectionCount) {
		diag->SectionCount = sectionCount;
		return stPOS_ERROR_SECTION;
	}
	sectionLength = sectionLengths[sectionType[sectionMapping[section]]];
	
	/* Verify the input section position */
	if(sectionPosition < 0 || sectionPosition > sectionLength + SECTION_POSITION_EXTENSION) /* Allow some rollover */ 
		return stPOS_ERROR_POSITION;
	
	/* Derive the global position */
	if(direction == stDIRECTION_RIGHT) {
		if(sectionMapping[section] == endIndex && sectionPosition >= sectionLength) /* Rollover the end section */
			*globalPosition = sectionPosition - sectionLength; /* Allow some rollover */
		else 
			*globalPosition = startingPosition[sectionMapping[section]] + sectionPosition;
	}
	else { /* Left */
		if(sectionMapping[section] == endIndex && sectionPosition == 0) /* Should not be rollover in the negative direction (encoder offset is zero at the first coil) */
			*globalPosition = 0;
		else 
			*globalPosition = startingPosition[sectionMapping[section]] + sectionLength - sectionPosition;
	}
	
	return stPOS_ERROR_NONE;
	
} /* End function */
