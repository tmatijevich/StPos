/*********************************************************************************
 * File:      SuperTrakReadLayout.c
 * Author:    Tyler Matijevich
 * Created:   May 24, 2021/16:20 
 *********************************************************************************/ 

#include "StPosMain.h"

/* Declare global variables */
unsigned short 			sectionCount; 						/* SuperTrak active section count */
unsigned short 			sectionAddress[MAX_SECTION]; 		/* SuperTrak section user addresses in network order */
unsigned short 			sectionType[MAX_SECTION]; 			/* SuperTrak section type 0..4 in network order */
unsigned short 			sectionMapping[MAX_SECTION + 1]; 	/* Map from user address (SuperTrak section number 1..50) to network order 0..49 */
signed long 			startingPosition[MAX_SECTION]; 		/* [um] Starting position of each section according to originSection and direction */
const signed long 		sectionLengths[] = { 				/* Length of a SuperTrak section according to type */
	1000000, 												/* 0 - Standard straight 1 meter */
	1030000, 												/* 1 - Standard curve 1 meter */
	1000000, 												/* 2 - Lower power straight 1 meter */
	759347, 												/* 3 - Wide curve 1.5 meter left */
	759347 													/* 4 - Wide curve 1.5 meter right */
};
unsigned short 			endIndex; 							/* Index in sectionAddress[] representing the last section according to originSection */
unsigned char 			previousOriginSection; 				/* The value of originSection in the last call to SuperTrakReadLayout() */
unsigned char 			previousDirection; 					/* The value of direction in the last call to SuperTrakReadLayout() */
unsigned char 			readSuccess; 						/* The last call to SuperTrakReadLayout() was successful */

/* Read/update the global SuperTrak network layout reference */
signed long SuperTrakReadLayout(unsigned char originSection, signed long direction, struct SuperTrakPositionDiagType *diag) {
	
	/* Declare local variables */
	unsigned short originIndex;
	unsigned char i, i_0, j;
	
	/* Reset global variables */
	sectionCount = 0;
	for(i = 0; i < MAX_SECTION; i++) {
		sectionAddress[i] 	= 0;
		sectionType[i] 		= 0;
		sectionMapping[i] 	= 0;
		startingPosition[i] = 0;
	}
	sectionMapping[MAX_SECTION] = 0;
	
	/* Read the SuperTrak section count */
	diag->ServiceChannelResult_1080 = SuperTrakServChanRead(
		0, /* System parameter */
		stPAR_SECTION_COUNT, /* Parameter */
		0, /* Start index */
		1, /* Count */
		(unsigned long)&sectionCount, /* Buffer address */
		sizeof(&sectionCount) /* Size of buffer */
	);
	if(diag->ServiceChannelResult_1080 != scERR_SUCCESS) 
		return stPOS_ERROR_SERV_CHAN;
	else if((sectionCount == 0) || (sectionCount > MAX_SECTION)) {
		diag->SectionCount = sectionCount;
		return stPOS_ERROR_LAYOUT;
	}
	
	/* 
		Read the SuperTrak system layout network addresses 
		Increasing network order i moves in the positive right direction stDIRECTION_RIGHT 
		Decreasing network order i moves in the negative left direction st_DIRECTION_LEFT 
	*/
	diag->ServiceChannelResult_1081 = SuperTrakServChanRead(
		0, /* System parameter */
		stPAR_SECTION_ADDRESS, /* Parameter */
		0, /* Start index */
		sectionCount, /* Count */
		(unsigned long)&sectionAddress[0], /* Buffer address */
		sizeof(sectionAddress) /* Size of buffer */
	);
	if(diag->ServiceChannelResult_1081 != scERR_SUCCESS)
		return stPOS_ERROR_SERV_CHAN;
	
	/* Read each section's type */
	for(i = 0; i < sectionCount; i++) {
		diag->ServiceChannelResult_1082 = SuperTrakServChanRead(
			sectionAddress[i], /* Section parameter */
			stPAR_SECTION_TYPE, /* Parameter */
			0, /* Start index */
			1, /* Count */
			(unsigned long)&sectionType[i], /* Buffer address */
			sizeof(&sectionType[i]) /* Size of buffer */
		);
		if(diag->ServiceChannelResult_1082 != scERR_SUCCESS) {
			diag->SectionNumber = sectionAddress[i];
			return stPOS_ERROR_SERV_CHAN;
		}
		else if((sectionAddress[i] == 0) || (sectionAddress[i] > MAX_SECTION)) {
			diag->SectionAddress = i;
			diag->SectionNumber = sectionAddress[i];
			return stPOS_ERROR_LAYOUT;
		}
		/* Verify this section's type is within 0..4 */
		else if(sectionType[i] > MAX_TYPE) {
			diag->SectionAddress = i;
			return stPOS_ERROR_LAYOUT;
		}
	}
	
	/* Verify the origin section and create the mapping */
	if((originSection == 0) || (originSection > sectionCount))
		return stPOS_ERROR_ORIGIN;
	for(i = 0; i < sectionCount; i++) {
		sectionMapping[sectionAddress[i]] = i; /* 1..50 Use to map from section number to network address */
		if(originSection == sectionAddress[i]) {
			originIndex = i;
			/* Determine the tail section user address */
			if(direction == stDIRECTION_RIGHT) { /* (Look left) */
				if(originIndex == 0)
					endIndex = sectionCount - 1;
				else
					endIndex = originIndex - 1;
			}
			else { /* Direction left (look right) */
				if(originIndex == sectionCount - 1)
					endIndex = 0;
				else
					endIndex = originIndex + 1;
			}
		}
	}
	
	/* Set the global starting position of each section in the network layout */
	i_0 = originIndex; j = 0;
	if(direction == stDIRECTION_LEFT) {
		if(i_0 == 0) i = sectionCount - 1;
		else i = i_0 - 1;
	}
	else {
		if(i_0 == (sectionCount - 1)) i = 0;
		else i = i_0 + 1;
	}
	while(i != originIndex) {
		if(++j > MAX_SECTION) {
			diag->SectionAddress = i;
			return stPOS_ERROR_LAYOUT;
		}
		
		/* Set the section's starting position based on the last section's starting position */
		startingPosition[i] = startingPosition[i_0] + sectionLengths[sectionType[i_0]];
		
		i_0 = i;
		if(direction == stDIRECTION_LEFT) {
			if(i_0 == 0) i = sectionCount - 1;
			else i = i_0 - 1;
		}
		else {
			if(i_0 == (sectionCount - 1)) i = 0;
			else i = i_0 + 1;
		}
	}

	return stPOS_ERROR_NONE;

} /* End function */
