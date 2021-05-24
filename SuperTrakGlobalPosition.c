 /*******************************************************************************
 * File: SuperTrakGlobalPosition.c
 * Author: Tyler Matijevich
 * Date: 2021-05-22
*******************************************************************************/

#include <ST_Pos.h>
#include <string.h>
#include <stdbool.h>

/* Return the global reference position on the SuperTrak loop */
DINT SuperTrakGlobalPosition(USINT section, DINT sectionPosition, USINT originSection, USINT direction, DINT* globalPosition, struct SuperTrakPositionDiagType* diag) {
	
	/* Declare real-time structures */
	struct RTCtime_typ currentTime;
	static struct RTCtime_typ previousTime;
	
	/* Update current time and compare with previous time */
	diag->CurrentTimeResult = RTC_gettime(&currentTime);
	if(diag->CurrentTimeResult != ERR_OK)
		return ST_POS_ERROR_RTC;
	if(memcmp(&currentTime, &previousTime, sizeof(currentTime)))
		diag->Read = true;
	else
		diag->Read = false;
		
	/* Reset the diagnostic information */
	diag->ServiceChannel1080Result = 0;
	diag->ServiceChannel1081Result = 0;
	diag->ServiceChannel1082Result = 0;
	diag->SectionCount 	= 0;
	diag->Index 		= 0;
	diag->Address 		= 0;
	
	/* Read SuperTrak parameters */
	UINT sectionCount;
	UINT sectionAddress[50];
	UINT sectionType[50];
	UINT sectionMapping[51];
	UINT originIndex;
	DINT startingPositions[51];
	USINT i, i_0;
	if(diag->Read) {
		/* Read the SuperTrak section count */
		diag->ServiceChannel1080Result = SuperTrakServChanRead(
			0, // System parameter
			stPAR_SECTION_COUNT, // Parameter
			0, // Start index
			1, // Count
			(UDINT)&sectionCount, // Buffer
			sizeof(&sectionCount) // Size
		);
		if(diag->ServiceChannel1080Result != scERR_SUCCESS)
			return ST_POS_ERROR_SERV_CHAN;
		else if((sectionCount == 0) || (sectionCount > 50)) {
			diag->SectionCount = sectionCount; // Record the invalid section count
			return ST_POS_ERROR_LAYOUT;
		}
		
		/* Read the configured SuperTrak section network */
		diag->ServiceChannel1081Result = SuperTrakServChanRead(
			0, // System parameter
			stPAR_SECTION_ADDRESS, // Parameter
			0, // Start index
			sectionCount, // Count
			(UDINT)&sectionAddress, // Buffer
			sizeof(&sectionAddress) // Size
		);
		if(diag->ServiceChannel1081Result != scERR_SUCCESS)
			return ST_POS_ERROR_SERV_CHAN;
		
		/* Check each section's type */
		for(i = 0; i < sectionCount; i++) {
			diag->ServiceChannel1082Result = SuperTrakServChanRead(
				sectionAddress[i], // Section parameter
				stPAR_SECTION_TYPE, // Parameter
				0, // Start index
				1, // Count
				(UDINT)&sectionType[i], // Buffer
				sizeof(&sectionType[i]) // Size
			);
			if(diag->ServiceChannel1082Result != scERR_SUCCESS) {
				diag->Index = i;
				return ST_POS_ERROR_SERV_CHAN;
			}
			else if((sectionAddress[i] == 0) || (sectionAddress[i] > sectionCount)) { // Compare each address against the count
				diag->SectionCount 	= sectionCount;
				diag->Index 		= i;
				diag->Address 		= sectionAddress[i];
				return ST_POS_ERROR_LAYOUT;
			}
		}
		
		/* Verify origin and create the mapping */
		if((originSection == 0) || (originSection > sectionCount)) {
			return ST_POS_ERROR_ORIGIN;
		}
		for(i = 0; i < sectionCount; i++) {
			sectionMapping[sectionAddress[i]] = i; // 1..50 Use the mapping to go from the section number to the index in the address network
			if(originSection == sectionAddress[i])
				originIndex = i;
		}
		
		/* Set the global starting positions of each section in the network layout */
		i_0 = originIndex;
		if(direction == stDIRECTION_LEFT) {
			if(i_0 == 1) i = sectionCount;
			else i = i_0 - 1;
		} else {
			if(i_0 == sectionCount) i = 1;
			else i = i_0 + 1;
		}
		while(i != originIndex) {
			// Set the section's starting position based on the last section's starting position and length
			if(sectionType[i_0]) startingPositions[i] = startingPositions[i_0] + 1030000;
			else startingPositions[i] = startingPositions[i_0] + 1000000;
			
			if(direction == stDIRECTION_LEFT) {
				if(i_0 == 1) i = sectionCount;
				else i = i_0 - 1;
			} else {
				if(i_0 == sectionCount) i = 1;
				else i = i_0 + 1;
			}
		}
	}
	else {
		
	} // Read
	
	/* Update the previous time */
	diag->PreviousTimeResult = RTC_gettime(&previousTime);
	if(diag->PreviousTimeResult != ERR_OK)
		return ST_POS_ERROR_RTC;
	
	return 0;
} // End function
