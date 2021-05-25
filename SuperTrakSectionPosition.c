/*********************************************************************************
 * File:      SuperTrakSectionPosition.c
 * Author:    Tyler Matijevich
 * Created:   May 24, 2021/17:37 
 *********************************************************************************/ 

#include "StPosMain.h"

/* Reference global variables */
extern UINT sectionCount;
extern UINT sectionAddress[50];
extern UINT sectionType[50];
extern DINT startingPosition[50]; // [um]
extern USINT previousOriginSection;
extern USINT previousDirection;
extern BOOL readSuccess;

/* Return the section position on the SuperTrak loop */
DINT SuperTrakSectionPosition(DINT globalPosition, USINT originSection, USINT direction, USINT* section, DINT* sectionPosition, struct SuperTrakPositionDiagType* diag) {
	
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
	
	/* Declare local variables */
	DINT readResult, lowerBound, upperBound;
	USINT i;
	
	/* Re-read the SuperTrak layout */
	if((originSection != previousOriginSection) || (direction != previousDirection) || (readSuccess == false)) {
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
		lowerBound = startingPosition[i];
		if(sectionType[i]) upperBound = startingPosition[i] + 1030000;
		else upperBound = startingPosition[i] + 1000000;
		if(direction == stDIRECTION_RIGHT) {
			if((lowerBound <= globalPosition) && (globalPosition < upperBound)) {
				*section = sectionAddress[i];
				*sectionPosition = globalPosition - lowerBound;
				return stPOS_ERROR_NONE;
			}
		}
		else {
			if((lowerBound < globalPosition) && (globalPosition <= upperBound)) {
				*section = sectionAddress[i];
				if(sectionType[i]) *sectionPosition = 1030000 - (globalPosition - lowerBound);
				else *sectionPosition = 1000000 - (globalPosition - lowerBound);
				return stPOS_ERROR_NONE;
			}
		}
	}
	
	return stPOS_ERROR_POSITION;

} /* End function */
