/** 
 * @file    music.c
 * @author  ZHAN (mzh99)      
 * @author  JOHN ELLIOTT (jel119)
 * @date    18 October 2021
 * 
 * @brief   This module defines the system for intilizing and playing/stopping the music.
 * 
 * @note    Original code by M.P. Hayes. Original code can be 
 *          found in the jukebox1.c file in the apps directory.
**/

#include "system.h"
#include "pio.h"
#include "tweeter.h"
#include "mmelody.h"
#include "music.h"

// Music information.
static tweeter_scale_t scale_table[] = TWEETER_SCALE_TABLE(TWEETER_TASK_RATE);
static tweeter_t tweeter;
static mmelody_t melody;
static mmelody_obj_t melody_info;
static tweeter_obj_t tweeter_info;

static const char tunes[] =
{
    #include "music.mmel"
};

// Function that is passed the mmelody initializer.
void note_play (tweeter_t tweeter, tweeter_note_t note, uint8_t velocity)
{
    tweeter_note_play(tweeter, note, velocity);
}

// Sets up the tweeter.
void tweeter_task_init (void)
{
    tweeter = tweeter_init(&tweeter_info, TWEETER_TASK_RATE, scale_table);

    pio_config_set(PIEZO1_PIO, PIO_OUTPUT_LOW);
    pio_config_set(PIEZO2_PIO, PIO_OUTPUT_HIGH);
}

// Updates the tweeter.
void tweeter_task (void)
{
    uint8_t state;

    state = tweeter_update(tweeter);
    pio_output_set(PIEZO1_PIO, state);
    pio_output_set(PIEZO2_PIO, !state);
}

// Sets up the melody and its rates.
void tune_task_init (void)
{
    melody = mmelody_init(&melody_info, TUNE_TASK_RATE, 
			   (mmelody_callback_t) note_play, tweeter);

    mmelody_speed_set(melody, TUNE_BPM_RATE);
}

// Updates the song (melody).
void tune_task(void)
{
    mmelody_update(melody);
}

// Starts playing the song in tunes.
void music_play(void)
{
    mmelody_play(melody, tunes);
}

// Plays nothing thereby stopping the music.
void music_off(void)
{
    mmelody_play(melody, 0);
}
