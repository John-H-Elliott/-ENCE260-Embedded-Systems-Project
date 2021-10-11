/** @file   dodge_dash.h
    @author ZHAN (mzh99)      ELLIOTT (jel119)
    @date   20 October 2021
    @brief  --

    @defgroup --
**/
#ifndef DODGE_DASH_H
#define DODGE_DASH_H

#include <stdlib.h>
#include "tinygl.h"


enum {LASER_ROW_MAX = 4, LASER_COL_MAX = 6};

/*---------------------- define pixel type ----------------------*/
typedef enum {PIX_OFF, PIX_NINJA, PIX_LASER} dodge_pix_type_t;

/*---------------------- define dodge type ----------------------*/
typedef enum {DODGE_NINJA, DODGE_LASER} dodge_type_t;

/*---------------------- type define dodge position by X,Y ----------------------*/
typedef struct
{
    int8_t x;
    int8_t y;
} dodge_pos_t;

/*---------------------- type define ninja position by X,Y ----------------------*/
typedef struct
{
    dodge_pos_t pos;
    bool active;
} dodge_obj_t;



/*---------------------- main structure ----------------------*/
typedef struct
{
    dodge_pos_t ninja;
    dodge_pos_t laser;


    bool active;
} dodge_t;





#endif