/*********************************************************************************
 * File:      StPosMain.h
 * Author:    Tyler Matijevich
 * Created:   May 24, 2021/16:15 
 *********************************************************************************/ 

#ifndef stPOS_MAIN_H
#define stPOS_MAIN_H

#ifdef __cplusplus
extern "C" 
{
#endif

/* Reference headers */
#include <StPos.h> 		/* Automatically generated from IEC declarations by Automation Studio for this library */
#include <string.h> 	/* C90 standard library */
#include <stdbool.h> 	/* C99 standard header */

/* Constants */
#ifndef stDIRECTION_RIGHT   		/* Define directions if not already from SuperTrak.h */
#define stDIRECTION_RIGHT 	1
#endif
#ifndef stDIRECTION_LEFT
#define stDIRECTION_LEFT 	0
#endif
#define stPOS_TYPE_MAX 		5 		/* 0: Straight 1m, 1: Curve 500mm, 2: Low-power straight, 3: Curve (left) 800mm, 4: Curve (right) 800mm, 5: 90 degree curve */

/* Global variables (promise to declare in source file) */
extern long 			nominalLengths[];  						/* Nominal section lengths of each section type */
extern unsigned short 	sectionCount; 							/* SuperTrak active section count */
extern unsigned short 	sectionAddress[stPOS_SECTION_MAX]; 		/* SuperTrak section user addresses in network order */
extern unsigned short 	sectionType[stPOS_SECTION_MAX]; 		/* SuperTrak section type 0..5 in network order */
extern unsigned short 	sectionMapping[stPOS_SECTION_MAX + 1]; 	/* Map from user address (SuperTrak section number 1..64) to network order 0..63 */
extern long 			sectionLength[stPOS_SECTION_MAX]; 		/* SuperTrak calibrated section length in network order */
extern long 			startingPosition[stPOS_SECTION_MAX]; 	/* [um] Starting position of each section according to originSection and direction */
extern long 			totalLength; 							/* Total length of the SuperTrak section network */
extern unsigned short 	originIndex; 							/* Index in sectionAddress[] representing the first section according to originSection */
extern unsigned short 	endIndex; 								/* Index in sectionAddress[] representing the last section according to originSection */
extern unsigned short 	previousOriginSection; 					/* The value of originSection in the last call to SuperTrakReadLayout() */
extern unsigned char 	previousDirection; 						/* The value of direction in the last call to SuperTrakReadLayout() */
extern unsigned char 	layoutValid; 							/* The last call to SuperTrakReadLayout() was successful */
extern unsigned char 	layoutLinear; 							/* The layout has no curved sections */

/* Function prototypes */
long SuperTrakReadLayout(unsigned char originSection, long direction, struct SuperTrakPositionInfoType *info);

#ifdef __cplusplus
};
#endif

#endif /* stPOS_MAIN_H */

