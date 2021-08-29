/*********************************************************************************
 * File:      SuperTrakSectionPosition.c
 * Author:    Tyler Matijevich
 * Created:   May 24, 2021/17:37 
 *********************************************************************************/ 

#include "StPosMain.h"

/* Return the section position on the SuperTrak loop */
signed long SuperTrakSectionPosition(signed long globalPosition, unsigned char originSection, signed long direction, unsigned char *section, signed long *sectionPosition, struct SuperTrakPositionInfoType *info) {
	
	/* Declare local variables */
	signed long sectionLength; /* Store the length of the current section */
	signed long readResult, lowerBound, upperBound;
	unsigned char i;
	
	/* Reset the solution, zero if error */
	*section = 0;
	*sectionPosition = 0;
	
	/* Reset diagnostic information */
	memset(info, 0, sizeof(*info));
	
	/* Re-read the SuperTrak layout */
	if(originSection != previousOriginSection || direction != previousDirection || readSuccess == false) {
		readResult = SuperTrakReadLayout(originSection, direction, info);
		if(readResult != stPOS_ERROR_NONE) {
			readSuccess = false;
			return readResult;
		}
		else
			readSuccess = true;
	}
	
	/* Verify the input global position */
	if(globalPosition < 0 || globalPosition > totalLength) { 
		info->globalPosition 	= globalPosition;
		info->totalLength 		= totalLength;
		return stPOS_ERROR_INPUTGLOBALPOS;
	}
	
	/* Search for the section */
	for(i = 0; i < sectionCount; i++) {
		sectionLength 	= sectionLengths[sectionType[i]];
		lowerBound 		= startingPosition[i];
		upperBound 		= startingPosition[i] + sectionLength;
		if(direction == stDIRECTION_RIGHT) {
			if(i == endIndex && globalPosition == upperBound) {
				*section 			= originSection;
				*sectionPosition 	= 0;
				return stPOS_ERROR_NONE;
			}
			else if(lowerBound <= globalPosition && globalPosition < upperBound) {
				*section = sectionAddress[i];
				*sectionPosition = globalPosition - lowerBound;
				return stPOS_ERROR_NONE;
			}
		}
		else { /* LEFT */
			if(i == sectionMapping[originSection] && globalPosition == lowerBound) {
				*section 			= sectionAddress[endIndex];
				*sectionPosition 	= 0;
				return stPOS_ERROR_NONE;
			}
			else if(lowerBound < globalPosition && globalPosition <= upperBound) {
				*section = sectionAddress[i];
				*sectionPosition = sectionLength - (globalPosition - lowerBound);
				return stPOS_ERROR_NONE;
			}
		}
	}
	
	return stPOS_ERROR_NETWORKGLOBAL; /* Section not found */

} /* End function */
