/** @file   dodge_dash.c
    @author ZHAN (mzh99)      ELLIOTT (jel119)
    @date   20 October 2021
    @brief  --

    @defgroup --
**/

#include "dodge_dash.h"
#include "tinygl.h"


/*---------------------- initial the position of ninjia  ----------------------*/
static void ninja_initalpos ()
{
    ninja.pos.x = TINYGL_WIDTH / 2;
    ninja.pos.y = TINYGL_HEIGHT / 2;
    ninja.active = 1;
}

/*---------------------- change the position of ninjia  with the navigation switch----------------------*/
static void ninja_movement ()
{
    if (navswitch_push_event_p (NAVSWITCH_WEST) && ninja.pos.x > 1)
    {
        ninja.pos.x--;
    }

    if (navswitch_push_event_p (NAVSWITCH_EAST) && ninja.pos.x < TINYGL_WIDTH - 1)
    {
        ninja.pos.x++;
    }

    if (navswitch_push_event_p (NAVSWITCH_SOUTH) && ninja.pos.y < TINYGL_HEIGHT - 1)
    {
        ninja.pos.y++;
    }

    if (navswitch_push_event_p (NAVSWITCH_NORTH) && ninja.pos.y > 1)
    {
        ninja.pos.y--;
    }
}

/*---------------------- change the position of ninjia  with the navigation switch----------------------*/



