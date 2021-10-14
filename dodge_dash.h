/** 
 * @file   dodge_dash.h
 * @author ZHAN (mzh99)      
 * @author JOHN ELLIOTT (jel119)
 * @date   20 October 2021
 * @brief  --
 * @defgroup --
**/

#ifndef DODGE_DASH_H
#define DODGE_DASH_H

#include "tinygl.h"



enum {LASER_ROW_MAX = 4, LASER_COL_MAX = 6};

/*---------------------- define pixel type ----------------------*/
typedef enum {PIX_OFF, PIX_NINJA, PIX_LASER} dodge_pix_type_t;

/*---------------------- define dodge type ----------------------*/
typedef enum {DODGE_NINJA, DODGE_LASER} dodge_type_t;





/*
 Define polling rates in Hz.  
enum {DISPLAY_UPDATE_RATE = 500};
enum {FLASHER_UPDATE_RATE = 500};
enum {BUTTON_POLL_RATE = 100};
enum {GAME_UPDATE_RATE = 100};
enum {GAME_OVER_PERIOD = 2};
enum {BUTTON_HOLD_PERIOD = 1};*/








/*---------------------- type define ninja current position by X,Y ----------------------*/
typedef struct 
{
    /* Current pos of ninja.  */
    tinygl_point_t pos;
    /* Current state of ninja.  */
    bool active;

} ninja_t;





/*---------------------- main structure ----------------------*/









/*---------------------- functions ----------------------*/

void ninja_init(ninja_t* ninja);

void ninja_movement(ninja_t* ninja);

void update_ninja_pos(ninja_t ninja);


#endif