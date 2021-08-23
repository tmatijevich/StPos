/*********************************************************************************
 * File:      StPosMain.h
 * Author:    Tyler Matijevich
 * Created:   May 24, 2021/16:15 
 *********************************************************************************/ 

#ifndef __ST_POS_MAIN_H__
#define __ST_POS_MAIN_H__

#define MAX_SECTION 			50
#define MAX_TYPE 				4

/* Reference headers */
#include <StPos.h>
#include <string.h>
#include <stdbool.h>

/* Function prototypes */
DINT SuperTrakReadLayout(USINT originSection, DINT direction, struct SuperTrakPositionDiagType* diag);

#endif

