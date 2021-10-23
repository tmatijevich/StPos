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
	unsigned char i;
	
	/* Clear layout and diagnostic information */
	memset(layout, 0, sizeof(*layout));
	memset(info, 0, sizeof(*info));
	
	/* Update layout if invalid */
	if(layoutValid == false) {
		layoutResult = SuperTrakReadLayout(1, stDIRECTION_RIGHT, info); /* Call with default origin and direction */
		if(layoutResult != stPOS_ERROR_NONE)
			return layoutResult;
	}
	
	/* Share system layout, values are truncated to save memory */
	layout->sectionCount = sectionCount; 
	for(i = 0; i < sectionCount; i++) {
		layout->sectionAddress[i] 	= sectionAddress[i];
		layout->sectionType[i] 		= sectionType[i];
	}
	layout->layoutLinear = layoutLinear;
	
	return stPOS_ERROR_NONE;
	
} /* End function */
