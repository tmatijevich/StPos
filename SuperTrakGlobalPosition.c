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

/* Declare global variables */
USINT previousOriginSection;
USINT previousDirection;
BOOL readSuccess;

/* Return the global reference position on the SuperTrak loop */
DINT SuperTrakGlobalPosition(USINT section, DINT sectionPosition, USINT originSection, USINT direction, DINT* globalPosition, struct SuperTrakPositionDiagType* diag) {
		
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
	
	/* Verify the input section position */
	if(sectionType[sectionMapping[section]]) { // Curve
		if((sectionPosition < 0) || (sectionPosition > 1030000))
			return stPOS_ERROR_POSITION;
	} 
	else {
		if((sectionPosition < 0) || (sectionPosition > 1000000))
			return stPOS_ERROR_POSITION;
	}
	
	/* Derive the global position */
	if(direction == stDIRECTION_RIGHT)
		*globalPosition = startingPosition[sectionMapping[section]] + sectionPosition;
	else {
		if(sectionType[sectionMapping[section]])
			*globalPosition = startingPosition[sectionMapping[section]] + 1030000 - sectionPosition;
		else
			*globalPosition = startingPosition[sectionMapping[section]] + 1000000 - sectionPosition;
	}
	
	return stPOS_ERROR_NONE;
	
} // End function
