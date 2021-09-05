/*********************************************************************************
 * File:      SuperTrakTotalLength.c
 * Author:    Tyler Matijevich
 * Created:   August 29, 2021/16:18 
 *********************************************************************************/ 

#include "StPosMain.h"

signed long SuperTrakTotalLength(signed long *length, struct SuperTrakPositionInfoType *info) {
	
	signed long layoutResult; /* Store the result of read layout call */
	
	/* Clear solution */
	*length = 0;
	
	/* Reset diagnostic information */
	memset(info, 0, sizeof(*info));
	
	if(layoutValid)
		*length = totalLength; /* Read global value */
	else {
		layoutResult = SuperTrakReadLayout(1, stDIRECTION_RIGHT, info); /* Call with section 1 as origin (always value) and direction right */
		if(layoutResult != stPOS_ERROR_NONE)
			return layoutResult;
		else
			*length = totalLength; /* Read global value */
	}

	return stPOS_ERROR_NONE;
	
} /* End function */
