/*********************************************************************************
 * File:      SuperTrakSectionPosition.c
 * Author:    Tyler Matijevich
 * Created:   May 24, 2021/17:37 
 *********************************************************************************/ 

#include "StPosMain.h"

/* Return the section position on the SuperTrak loop */
signed long SuperTrakSectionPosition(signed long globalPosition, unsigned char originSection, signed long direction, unsigned char *section, signed long *sectionPosition, struct SuperTrakPositionInfoType *info) {
	
	/* Declare local variables */
	signed long layoutResult, lowerBound, upperBound;
	unsigned char i;
	
	/* Reset the solution */
	*section = 0;
	*sectionPosition = 0;
	
	/* Reset diagnostic information */
	memset(info, 0, sizeof(*info));
	
	/* Re-read the SuperTrak layout */
	if(originSection != previousOriginSection || direction != previousDirection || layoutValid == false) {
		layoutResult = SuperTrakReadLayout(originSection, direction, info);
		if(layoutResult != stPOS_ERROR_NONE)
			return layoutResult;
	}
	
	/* Verify the input global position */
	if(globalPosition < 0 || globalPosition > totalLength) { 
		info->globalPosition 	= globalPosition;
		info->totalLength 		= totalLength;
		return stPOS_ERROR_INPUTGLOBALPOS;
	}
	
	/* Search for the section */
	for(i = 0; i < sectionCount; i++) {
		lowerBound 		= startingPosition[i];
		upperBound 		= startingPosition[i] + sectionLength[i];
		if(direction == stDIRECTION_RIGHT) {
			if(i == endIndex && globalPosition == upperBound) {
				if(layoutLinear == false) {
					*section 			= sectionAddress[originIndex];
					*sectionPosition 	= 0;
				}
				else { /* Do not wrap for linear layouts */
					*section 			= sectionAddress[i];
					*sectionPosition 	= sectionLength[i];
				}
				return stPOS_ERROR_NONE;
			}
			else if(lowerBound <= globalPosition && globalPosition < upperBound) {
				*section 			= sectionAddress[i];
				*sectionPosition 	= globalPosition - lowerBound;
				return stPOS_ERROR_NONE;
			}
		}
		else { /* Left */
			if(i == originIndex && globalPosition == lowerBound) {
				if(layoutLinear == false) {
					*section 			= sectionAddress[endIndex];
					*sectionPosition 	= 0;
				}
				else { /* Do not wrap for linear layouts */
					*section 			= sectionAddress[i];
					*sectionPosition 	= sectionLength[i];
				}
				return stPOS_ERROR_NONE;
			}
			else if(lowerBound < globalPosition && globalPosition <= upperBound) {
				*section 			= sectionAddress[i];
				*sectionPosition 	= sectionLength[i] - (globalPosition - lowerBound);
				return stPOS_ERROR_NONE;
			}
		}
	}
	
	return stPOS_ERROR_NETWORKGLOBAL; /* Section not found */

} /* End function */
