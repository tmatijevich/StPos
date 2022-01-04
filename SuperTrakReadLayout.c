/*********************************************************************************
 * File:      SuperTrakReadLayout.c
 * Author:    Tyler Matijevich
 * Created:   May 24, 2021/16:20 
 *********************************************************************************/ 

#include "StPosMain.h"

/* Declare global variables */
unsigned short 	sectionCount; 							/* SuperTrak active section count */
unsigned short 	sectionAddress[stPOS_SECTION_MAX]; 		/* SuperTrak section user addresses in network order */
unsigned short 	sectionType[stPOS_SECTION_MAX]; 		/* SuperTrak section type 0..5 in network order */
unsigned short 	sectionMapping[stPOS_SECTION_MAX + 1]; 	/* Map from user address (SuperTrak section number 1..64) to network order 0..63 */
long 			sectionLength[stPOS_SECTION_MAX]; 		/* SuperTrak calibrated section length in network order */
long 			startingPosition[stPOS_SECTION_MAX]; 	/* [um] Starting position of each section according to originSection and direction */
long 			totalLength; 							/* Total length of the SuperTrak section network */
unsigned short 	originIndex; 							/* Index in sectionAddress[] representing the first section according to originSection */
unsigned short 	endIndex; 								/* Index in sectionAddress[] representing the last section according to originSection */
unsigned short 	previousOriginSection; 					/* The value of originSection in the last call to SuperTrakReadLayout() */
unsigned char 	previousDirection; 						/* The value of direction in the last call to SuperTrakReadLayout() */
unsigned char 	layoutValid; 							/* The last call to SuperTrakReadLayout() was successful */
unsigned char 	layoutLinear; 							/* The layout has no curved sections */

/* Read/update the global SuperTrak network layout reference */
long SuperTrakReadLayout(unsigned char originSection, long direction, struct SuperTrakPositionInfoType *info) {
	
	/* Declare local variables */
	unsigned char i, i_0, j;
	
	/* Reset global variables */
	sectionCount 	= 0;
	memset(sectionAddress, 0, sizeof(sectionAddress));
	memset(sectionType, 0, sizeof(sectionType));
	memset(sectionMapping, 0, sizeof(sectionMapping));
	memset(startingPosition, 0, sizeof(startingPosition));
	totalLength 	= 0;
	endIndex 		= 0;
	layoutValid 	= false; /* Invalidate until successful completion */
	
	/* Read the SuperTrak section count */
	info->serviceChannelResult = SuperTrakServChanRead(
		0, 								/* System parameter */
		stPAR_SECTION_COUNT, 			/* Parameter */
		0, 								/* Start index */
		1, 								/* Count */
		(unsigned long)&sectionCount, 	/* Buffer address */
		sizeof(&sectionCount)  			/* Buffer size */
	);
	if(info->serviceChannelResult != scERR_SUCCESS) {
		info->serviceChannelParameter = stPAR_SECTION_COUNT;
		return stPOS_ERROR_SERVCHAN;
	}
	else if(sectionCount == 0 || sectionCount > stPOS_SECTION_MAX) {
		info->sectionCount = sectionCount;
		return stPOS_ERROR_SECTIONCOUNT;
	}
	
	/* 
		Read the SuperTrak system layout network addresses 
		Increasing network order i moves in the positive right direction stDIRECTION_RIGHT 
		Decreasing network order i moves in the negative left direction stDIRECTION_LEFT 
	*/
	info->serviceChannelResult = SuperTrakServChanRead(
		0, 									/* System parameter */
		stPAR_SECTION_ADDRESS, 				/* Parameter */
		0, 									/* Start index */
		sectionCount, 						/* Count */
		(unsigned long)&sectionAddress[0], 	/* Buffer address */
		sizeof(sectionAddress) 				/* Buffer size */
	);
	if(info->serviceChannelResult != scERR_SUCCESS) {
		info->serviceChannelParameter = stPAR_SECTION_ADDRESS;
		return stPOS_ERROR_SERVCHAN;
	}
	
	/* Read each section's type */
	layoutLinear = true; /* Assume true before loop, clear if a curve is found */
	for(i = 0; i < sectionCount; i++) {
		info->serviceChannelResult = SuperTrakServChanRead(
			sectionAddress[i], 					/* Section parameter */
			1506, 								/* Parameter */
			0, 									/* Start index */
			1, 									/* Count */
			(unsigned long)&sectionLength[i], 	/* Buffer address */
			sizeof(&sectionLength[i]) 			/* Buffer size */
		);
		if(info->serviceChannelResult != scERR_SUCCESS) {
			info->serviceChannelParameter = 1506;
			return stPOS_ERROR_SERVCHAN;
		}
		
		info->serviceChannelResult = SuperTrakServChanRead(
			sectionAddress[i], 				/* Section parameter */
			stPAR_SECTION_TYPE, 			/* Parameter */
			0, 								/* Start index */
			1, 								/* Count */
			(unsigned long)&sectionType[i], /* Buffer address */
			sizeof(&sectionType[i]) 		/* Buffer size */
		);
		if(info->serviceChannelResult != scERR_SUCCESS) {
			info->serviceChannelParameter = stPAR_SECTION_TYPE;
			return stPOS_ERROR_SERVCHAN;
		}
		/* Verify user address (unlikely service channel was successful) */
		else if(sectionAddress[i] == 0 || sectionAddress[i] > stPOS_SECTION_MAX) {
			info->section 		= sectionAddress[i];
			info->networkIndex 	= i;
			return stPOS_ERROR_SECTIONADDRESS;
		}
		/* Verify this section's type is within 0..5 */
		else if(sectionType[i] > stPOS_TYPE_MAX) {
			info->sectionType 	= sectionType[i];
			info->section 		= sectionAddress[i];
			return stPOS_ERROR_SECTIONTYPE;
		}
		
		/* Determine if linear or loop */
		if(sectionType[i] != 0 && sectionType[i] != 2 && layoutLinear) /* Straight or low-power straight */
			layoutLinear = false;
	}
	
	/* Verify the origin section */
	if((originSection == 0 || originSection > sectionCount) && layoutLinear == false) {
		info->originSection = originSection;
		info->sectionCount 	= sectionCount;
		return stPOS_ERROR_INPUTORIGIN;
	}
	
	/* Find the origin and create the mapping */
	for(i = 0; i < sectionCount; i++) {
		sectionMapping[sectionAddress[i]] = i; /* 1..64 Use to map from section number to network address */
		
		if(originSection == sectionAddress[i] && layoutLinear == false) {
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
	if(layoutLinear == true) {
		if(direction == stDIRECTION_RIGHT) {
			originIndex = 0;
			endIndex 	= sectionCount - 1;
		}
		else {
			originIndex = sectionCount - 1;
			endIndex 	= 0;
		}
	}
	
	/* Set the global starting position of each section in the network layout */
	i_0 = originIndex;
	
	/* Update the next index/section */
	if(direction == stDIRECTION_RIGHT) {
		if(i_0 == (sectionCount - 1)) i = 0;
		else i = i_0 + 1;
	}
	else { /* Left */
		if(i_0 == 0) i = sectionCount - 1;
		else i = i_0 - 1;
	}
	
	j = 0; /* Track interations through the loop */
	while(i != originIndex) {
		if(++j > sectionCount) 
			return stPOS_ERROR_NETWORKORDER; /* More interations than sections */
		
		/* Set the section's starting position based on the last section's starting position */
		startingPosition[i] = startingPosition[i_0] + sectionLength[i_0];
		
		i_0 = i;
		/* Update the next index/section */
		if(direction == stDIRECTION_RIGHT) {
			if(i_0 == (sectionCount - 1)) i = 0;
			else i = i_0 + 1;
		}
		else { /* Left */
			if(i_0 == 0) i = sectionCount - 1;
			else i = i_0 - 1;
		}
	}
	
	/* Compute the total length of the SuperTrak network */
	for(i = 0; i < sectionCount; i++)
		totalLength += sectionLength[i];
	
	/* Update previous inputs, validate */
	previousOriginSection 	= originSection;
	previousDirection 		= direction;
	layoutValid 			= true;

	return stPOS_ERROR_NONE;

} /* End function */
