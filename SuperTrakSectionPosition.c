/*********************************************************************************
 * File:      SuperTrakSectionPosition.c
 * Author:    Tyler Matijevich
 * Created:   May 24, 2021/17:37 
 *********************************************************************************/ 

#include "StPosMain.h"

/* Return the section position on the SuperTrak loop */
signed long SuperTrakSectionPosition(signed long globalPosition, unsigned char originSection, signed long direction, unsigned char *section, signed long *sectionPosition, struct SuperTrakPositionDiagType *diag) {
	
	/* Declare local variables */
	signed long sectionLength; /* Store the length of the current section */
	signed long readResult, lowerBound, upperBound;
	unsigned char i;
	
	/* Reset diagnostic information */
	diag->ServiceChannelResult_1080 = 0;
	diag->ServiceChannelResult_1081 = 0;
	diag->ServiceChannelResult_1082 = 0;
	diag->SectionCount 		= 0;
	diag->SectionAddress 	= 0;
	diag->SectionNumber 	= 0;
	
	/* Reset the solution, zero if error */
	*section = 0;
	*sectionPosition = 0;
	
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
	
	return stPOS_ERROR_POSITION;

} /* End function */
