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
#define MAX_SECTION 				50
#define MAX_TYPE 					4
#define SECTION_POSITION_EXTENSION 	5000 /* From unique encoder calibrations, allow section position inputs to exceed
											the length of section by 5 mm */

/* Global variables (promise to declare in source file) */
extern unsigned short 			sectionCount; 						/* SuperTrak active section count */
extern unsigned short 			sectionAddress[MAX_SECTION]; 		/* SuperTrak section user addresses in network order */
extern unsigned short 			sectionType[MAX_SECTION]; 			/* SuperTrak section type 0..4 in network order */
extern unsigned short 			sectionMapping[MAX_SECTION + 1]; 	/* Map from user address (SuperTrak section number 1..50) to network order 0..49 */
extern signed long 				startingPosition[MAX_SECTION]; 		/* [um] Starting position of each section according to originSection and direction */
extern const signed long 		sectionLengths[]; 					/* Length of a SuperTrak section according to type */
extern signed long 				totalLength; 						/* Total length of the SuperTrak section network */
extern unsigned short 			endIndex; 							/* Index in sectionAddress[] representing the last section according to originSection */
extern unsigned char 			previousOriginSection; 				/* The value of originSection in the last call to SuperTrakReadLayout() */
extern unsigned char 			previousDirection; 					/* The value of direction in the last call to SuperTrakReadLayout() */
extern unsigned char 			readSuccess; 						/* The last call to SuperTrakReadLayout() was successful */

/* Function prototypes */
signed long SuperTrakReadLayout(unsigned char originSection, signed long direction, struct SuperTrakPositionInfoType *info);

#endif /* __ST_POS_MAIN_H__ */

