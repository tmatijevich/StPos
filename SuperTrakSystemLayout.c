/*********************************************************************************
 * File:      SuperTrakSystemLayout.c
 * Author:    Tyler Matijevich
 * Created:   October 23, 2021/6:25 PM 
 *********************************************************************************/ 

#include "StPosMain.h"

/* Obtain the SuperTrak system layout information */
signed long SuperTrakSystemLayout(struct SuperTrakSystemLayoutType *layout, struct SuperTrakPositionInfoType *info) {
	
	/* Declare local variables */
	signed long layoutResult;
	unsigned char i, j, k;
	unsigned short headSection, flowDirection;
	
	/* Clear layout and diagnostic information */
	memset(layout, 0, sizeof(*layout));
	memset(info, 0, sizeof(*info));
	
	/* Update layout if invalid */
	if(layoutValid == false) {
		layoutResult = SuperTrakReadLayout(1, stDIRECTION_RIGHT, info); /* Call with default origin and direction */
		if(layoutResult != stPOS_ERROR_NONE)
			return layoutResult;
	}
	
	/* Read head section and flow direction */
	info->serviceChannelResult = SuperTrakServChanRead(
		0, 								/* System parameter */
		stPAR_LOGICAL_HEAD_SECTION, 	/* Parameter */
		0, 								/* Start index */
		1, 								/* Count */
		(unsigned long)&headSection, 	/* Buffer address */
		sizeof(&headSection) 			/* Buffer size */
	);
	if(info->serviceChannelResult != scERR_SUCCESS) {
		info->serviceChannelParameter = stPAR_LOGICAL_HEAD_SECTION;
		return stPOS_ERROR_SERVCHAN;
	}
	else if(headSection == 0 || headSection > sectionCount) {
		info->headSection = headSection;
		return stPOS_ERROR_HEADSECTION;
	}
	info->serviceChannelResult = SuperTrakServChanRead(
		0, 								/* System parameter */
		stPAR_FLOW_DIRECTION, 			/* Parameter */
		0, 								/* Start index */
		1, 								/* Count */
		(unsigned long)&flowDirection, 	/* Buffer address */
		sizeof(&flowDirection) 			/* Buffer size */
	);
	if(info->serviceChannelResult != scERR_SUCCESS) {
		info->serviceChannelParameter = stPAR_FLOW_DIRECTION;
		return stPOS_ERROR_SERVCHAN;
	}
	else if(flowDirection != stDIRECTION_RIGHT && flowDirection != stDIRECTION_LEFT) {
		info->flowDirection = flowDirection;
		return stPOS_ERROR_FLOWDIRECTION;
	}
	
	/* Share system layout, values are truncated to save memory */
	layout->sectionCount = sectionCount; 
	for(i = 0; i < sectionCount; i++) {
		layout->networkOrder[i] 				= sectionAddress[i];
		layout->networkMapping[i+1] 			= sectionMapping[i+1];
		layout->sectionType[sectionAddress[i]] 	= sectionType[i];
	}
	layout->layoutLinear = layoutLinear;
	
	/* Find the head section and proceed to left or right */
	for(i = 0; i < sectionCount; i++) {
		if(sectionAddress[i] == headSection) {
			layout->flowOrder[0] = sectionAddress[i];
			if(flowDirection == stDIRECTION_RIGHT) {
				if(i == (sectionCount - 1)) i = 0;
				else i += 1;
			}
			else {
				if(i == 0) i = sectionCount - 1;
				else i -= 1;
			}
			break;
		}
	}
	j = 1; k = 0;
	while(sectionAddress[i] != headSection) {
		if(++k > sectionCount)
			return stPOS_ERROR_NETWORKORDER;
		layout->flowOrder[j++] = sectionAddress[i];
		if(flowDirection == stDIRECTION_RIGHT) {
			if(i == (sectionCount - 1)) i = 0;
			else i += 1;
		}
		else {
			if(i == 0) i = sectionCount - 1;
			else i -= 1;
		}
	}
	
	return stPOS_ERROR_NONE;
	
} /* End function */
