 /*******************************************************************************
 * File: SuperTrakGlobalPosition.c
 * Author: Tyler Matijevich
 * Date: 2021-05-22
*******************************************************************************/

#include "StPosMain.h"

/* Reference global variables */
extern UINT sectionCount;
extern UINT sectionType[50];
extern UINT sectionMapping[51];
extern DINT startingPosition[50]; // [um]
extern const unsigned long sectionLengths[];
extern UINT endIndex;

/* Declare global variables */
USINT previousOriginSection;
DINT previousDirection;
BOOL readSuccess;

/* Return the global reference position on the SuperTrak loop */
DINT SuperTrakGlobalPosition(USINT section, DINT sectionPosition, USINT originSection, DINT direction, DINT* globalPosition, struct SuperTrakPositionDiagType* diag) {
		
	unsigned long sectionLength; /* Store the length of the input section */
	
	/* Reset diagnostic information */
	diag->ServiceChannelResult_1080 = 0;
	diag->ServiceChannelResult_1081 = 0;
	diag->ServiceChannelResult_1082 = 0;
	diag->SectionCount 		= 0;
	diag->SectionAddress 	= 0;
	diag->SectionNumber 	= 0;
	
	/* Reset the solution, zero if error */
	*globalPosition = 0;
	
	/* Declare local variables */
	DINT readResult;
	
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
	
	/* Verify the input section */
	if((section == 0) || (section > sectionCount)) {
		diag->SectionCount = sectionCount;
		return stPOS_ERROR_SECTION;
	}
	sectionLength = sectionLengths[sectionType[sectionMapping[section]]];
	
	/* Verify the input section position */
	if((sectionPosition < 0) || (sectionPosition > sectionLength)) 
		return stPOS_ERROR_POSITION;
	
	/* Derive the global position */
	if(direction == stDIRECTION_RIGHT) {
		if((sectionMapping[section] == endIndex) && (sectionPosition == sectionLength))
			*globalPosition = 0;
		else 
			*globalPosition = startingPosition[sectionMapping[section]] + sectionPosition;
	}
	else { // Left
		if((sectionMapping[section] == endIndex) && (sectionPosition == 0))
			*globalPosition = 0;
		else 
			*globalPosition = startingPosition[sectionMapping[section]] + sectionLength - sectionPosition;
	}
	
	return stPOS_ERROR_NONE;
	
} // End function
