/** 
 * @file   dodge_dash.h
 * @author ZHAN (mzh99)      
 * @author JOHN ELLIOTT (jel119)
 * @date   20 October 2021
 * @brief  --
 * @defgroup --
**/

#ifndef NINJA_H
#define NINJA_H

#include "tinygl.h"


/*---------------------- structs and enums ----------------------*/

typedef struct 
{
    /* Current pos of ninja.  */
    tinygl_point_t pos;
    /* Current state of ninja.  */
    bool active;

} ninja_t;


/*---------------------- functions ----------------------*/

void ninja_init(ninja_t*);

void ninja_movement(ninja_t*);

void update_ninja_pos(ninja_t);


#endif