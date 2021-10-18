/** 
 * @file    music.h
 * @author  ZHAN (mzh99)      
 * @author  JOHN ELLIOTT (jel119)
 * @date    18 October 2021
 * 
 * @brief   This module defines the task rates of the music and its system.
 *          Also, defines the functions in music.c.
 * 
 * 
 * @note    Original code by M.P. Hayes. Original code can be 
 *          found in the jukebox1.c file in the apps directory.
**/

#ifndef MUSIC_H
#define MUSIC_H

#include <stdint.h>
#include "tweeter.h"

/* --------------------------- | Defines | ---------------------------*/

/* Connect piezo tweeter to pins 6 and 8 of UCFK4 P1 connector
   for push-pull operation.  */
#define PIEZO1_PIO PIO_DEFINE (PORT_D, 4)
#define PIEZO2_PIO PIO_DEFINE (PORT_D, 6)

/* Define polling rates in Hz.  */
#define TWEETER_TASK_RATE 20000  // Rate of the tweeter.
#define TUNE_TASK_RATE 150       // Rate of the music tunes.
#define TUNE_BPM_RATE 150        // BPM of the music.


/* --------------------------- | Functions | ---------------------------*/

void note_play(tweeter_t, tweeter_note_t, uint8_t);

void tweeter_task_init(void);

void tweeter_task(void);

void tune_task_init(void);

void tune_task(void);

void music_play(void);

void music_off(void);

#endif
