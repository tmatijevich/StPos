 /*******************************************************************************
 * File: SuperTrakGlobalPosition.c
 * Author: Tyler Matijevich
 * Date: 2021-05-22
*******************************************************************************/

#include <StPos.h>
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
	static UINT sectionCount;
	UINT sectionAddress[50];
	static UINT sectionType[50];
	static UINT sectionMapping[51];
	UINT originIndex;
	static DINT startingPositions[50];
	USINT i, i_0, j;
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
		
		diag->SectionCount = sectionCount;
		if((sectionCount == 0) || (sectionCount > 50))
			return ST_POS_ERROR_LAYOUT;
			
		/* Read the configured SuperTrak section network */
		diag->ServiceChannel1081Result = SuperTrakServChanRead(
			0, // System parameter
			stPAR_SECTION_ADDRESS, // Parameter
			0, // Start index
			sectionCount, // Count
			(UDINT)&sectionAddress[0], // Buffer
			sizeof(sectionAddress) // Size
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
			if(i_0 == 0) i = sectionCount - 1;
			else i = i_0 - 1;
		} else {
			if(i_0 == sectionCount - 1) i = 0;
			else i = i_0 + 1;
		}
		j = 0;
		while(i != originIndex) {
			j++;
			if(j > 50) return ST_POS_ERROR_LAYOUT;
			
			// Set the section's starting position based on the last section's starting position and length
			if(sectionType[i_0]) startingPositions[i] = startingPositions[i_0] + 1030000;
			else startingPositions[i] = startingPositions[i_0] + 1000000;
			
			i_0 = i;
			if(direction == stDIRECTION_LEFT) {
				if(i_0 == 0) i = sectionCount - 1;
				else i = i_0 - 1;
			} else {
				if(i_0 == sectionCount - 1) i = 0;
				else i = i_0 + 1;
			}
		}
	}
	else {
		/* Verify the input section */
		if((section == 0) || (section > sectionCount)) {
			diag->SectionCount = sectionCount;
			return ST_POS_ERROR_SECTION;
		}
		
		/* Verify the input section position */
		if(sectionType[sectionMapping[section]]) { // Curve
			if((sectionPosition < 0) || (sectionPosition > 1030000))
				return ST_POS_ERROR_POSITION;
		} 
		else {
			if((sectionPosition < 0) || (sectionPosition > 1000000))
				return ST_POS_ERROR_POSITION;
		}
		
		/* Derive the global position */
		if(direction == stDIRECTION_RIGHT)
			*globalPosition = startingPositions[sectionMapping[section]] + sectionPosition;
		else {
			if(sectionType[sectionMapping[section]])
				*globalPosition = startingPositions[sectionMapping[section]] + 1030000 - sectionPosition;
			else
				*globalPosition = startingPositions[sectionMapping[section]] + 1000000 - sectionPosition;
		}
		
	} // Read
	
	/* Update the previous time */
	diag->PreviousTimeResult = RTC_gettime(&previousTime);
	if(diag->PreviousTimeResult != ERR_OK)
		return ST_POS_ERROR_RTC;
	
	return 0;
} // End function
