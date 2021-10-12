/** @file   dodge_dash.h
    @author ZHAN (mzh99)      ELLIOTT (jel119)
    @date   20 October 2021
    @brief  --

    @defgroup --
**/
#ifndef DODGE_DASH_H
#define DODGE_DASH_H

#include "tinygl.h"



enum {LASER_ROW_MAX = 4, LASER_COL_MAX = 6};

/*---------------------- define pixel type ----------------------*/
typedef enum {PIX_OFF, PIX_NINJA, PIX_LASER} dodge_pix_type_t;

/*---------------------- define dodge type ----------------------*/
typedef enum {DODGE_NINJA, DODGE_LASER} dodge_type_t;


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