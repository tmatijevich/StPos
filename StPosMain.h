/*********************************************************************************
 * File:      StPosMain.h
 * Author:    Tyler Matijevich
 * Created:   May 24, 2021/16:15 
 *********************************************************************************/ 

#ifndef __ST_POS_MAIN_H__
#define __ST_POS_MAIN_H__

/* Reference headers */
#include <StPos.h>
#include <string.h>
#include <stdbool.h>

/* Function prototypes */
DINT SuperTrakReadLayout(USINT originSection, DINT direction, struct SuperTrakPositionDiagType* diag);

#endif

