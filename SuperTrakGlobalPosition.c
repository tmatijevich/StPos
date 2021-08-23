 /*******************************************************************************
 * File: SuperTrakGlobalPosition.c
 * Author: Tyler Matijevich
 * Date: 2021-05-22
*******************************************************************************/

#include "StPosMain.h"

/* Reference global variables */
extern unsigned short sectionCount;
extern unsigned short sectionType[50];
extern unsigned short sectionMapping[51];
extern signed long startingPosition[50]; // [um]
extern const signed long sectionLengths[];
extern unsigned short endIndex;

/* Declare global variables */
unsigned char previousOriginSection;
signed long previousDirection;
unsigned char readSuccess;

/* Return the global reference position on the SuperTrak loop */
signed long SuperTrakGlobalPosition(unsigned char section, signed long sectionPosition, unsigned char originSection, signed long direction, signed long *globalPosition, struct SuperTrakPositionDiagType *diag) {
		
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
	signed long readResult;
	
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
