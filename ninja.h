/** 
 * @file    ninja.h
 * @author  ZHAN (mzh99)      
 * @author  JOHN ELLIOTT (jel119)
 * @date    10 October 2021
 * 
 * @brief   This module defines the player (ninja) and functions
 *          used by ninja.c.
**/

#ifndef NINJA_H
#define NINJA_H

#include <stdbool.h>
#include "tinygl.h"


/*----------------------- | Structs | -----------------------*/

typedef struct 
{
    tinygl_point_t pos; // Current postion of the ninja.
    bool active; // Current state of the ninja (alive or dead).
} ninja_t;


/*---------------------- | Functions | ----------------------*/

void ninja_init(ninja_t*);

void ninja_movement(ninja_t*);

bool thrown_smoke_bomb(void);

void update_ninja_pos(ninja_t);

void ninja_flash(ninja_t);

#endif
