/*********************************************************************************
 * File:      StPosMain.h
 * Author:    Tyler Matijevich
 * Created:   May 24, 2021/16:15 
 *********************************************************************************/ 

#ifndef __ST_POS_MAIN_H__
#define __ST_POS_MAIN_H__

/* Reference headers */
#include <StPos.h> 		/* Automatically generated from IEC declarations by Automation Studio for this library */
#include <string.h> 	/* C90 standard library */
#include <stdbool.h> 	/* C99 standard header */

/* Constants */
#ifndef stDIRECTION_RIGHT   				/* Define directions if not already from SuperTrak.h */
#define stDIRECTION_RIGHT 			1
#endif
#ifndef stDIRECTION_LEFT
#define stDIRECTION_LEFT 			0
#endif
#define MAX_TYPE 					5 		/* 0: Straight 1m, 1: Curve 500mm, 2: Low-power straight, 3: Curve (left) 800mm, 4: Curve (right) 800mm, 5: 90 degree curve */

/* Global variables (promise to declare in source file) */
extern unsigned short 			sectionCount; 						/* SuperTrak active section count */
extern unsigned short 			sectionAddress[MAX_SECTION]; 		/* SuperTrak section user addresses in network order */
extern unsigned short 			sectionType[MAX_SECTION]; 			/* SuperTrak section type 0..4 in network order */
extern unsigned short 			sectionMapping[MAX_SECTION + 1]; 	/* Map from user address (SuperTrak section number 1..50) to network order 0..49 */
extern signed long 				sectionLength[MAX_SECTION]; 		/* SuperTrak calibrated section length in network order */
extern signed long 				startingPosition[MAX_SECTION]; 		/* [um] Starting position of each section according to originSection and direction */
extern signed long 				totalLength; 						/* Total length of the SuperTrak section network */
extern unsigned short 			originIndex; 						/* Index in sectionAddress[] representing the first section according to originSection */
extern unsigned short 			endIndex; 							/* Index in sectionAddress[] representing the last section according to originSection */
extern unsigned short 			previousOriginSection; 				/* The value of originSection in the last call to SuperTrakReadLayout() */
extern unsigned char 			previousDirection; 					/* The value of direction in the last call to SuperTrakReadLayout() */
extern unsigned char 			layoutValid; 						/* The last call to SuperTrakReadLayout() was successful */
extern unsigned char 			layoutLinear; 						/* The layout has no curved sections */

/* Function prototypes */
signed long SuperTrakReadLayout(unsigned char originSection, signed long direction, struct SuperTrakPositionInfoType *info);

#endif /* __ST_POS_MAIN_H__ */

