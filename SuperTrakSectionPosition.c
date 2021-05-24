/*********************************************************************************
 * File:      SuperTrakSectionPosition.c
 * Author:    Tyler Matijevich
 * Created:   May 24, 2021/17:37 
 *********************************************************************************/ 

#include "StPosMain.h"

/* Reference global variables */
extern UINT sectionCount;
extern UINT sectionType[50];
extern UINT sectionMapping[51];
extern DINT startingPosition[50]; // [um]
extern USINT previousOriginSection;
extern USINT previousDirection;
extern BOOL readSuccess;

/* Return the section position on the SuperTrak loop */
DINT SuperTrakSectionPosition(DINT globalPosition, USINT originSection, USINT direction, USINT* section, DINT* sectionPosition, struct SuperTrakPositionDiagType* diag) {

	return stPOS_ERROR_NONE;

} /* End function */

