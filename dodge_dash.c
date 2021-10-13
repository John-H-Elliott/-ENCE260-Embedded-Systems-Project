/** 
 * @file   dodge_dash.c
 * @author ZHAN (mzh99)      
 * @author JOHN ELLIOTT (jel119)
 * @date   20 October 2021
 * @brief  --
 * @defgroup --
**/

#include <stdio.h>
#include "navswitch.h"
#include "dodge_dash.h"

/*---------------------- initial the position of ninja  ----------------------*/
void ninja_init(ninja_t* ninja)
{
    ninja->pos.x = TINYGL_WIDTH - 1;
    ninja->pos.y = TINYGL_HEIGHT - 1;
    ninja->active = 1;
}


/*---------------------- change the position of ninja  with the navigation switch----------------------*/
void ninja_movement(ninja_t* ninja)
{
    navswitch_update(); // Update info from navswitch.
    if (navswitch_push_event_p(NAVSWITCH_WEST) && ninja->pos.x > 1)
    {
        ninja->pos.x--;
    }

    if (navswitch_push_event_p(NAVSWITCH_EAST) && ninja->pos.x < TINYGL_WIDTH - 1)
    {
        ninja->pos.x++;
    }

    if (navswitch_push_event_p(NAVSWITCH_SOUTH) && ninja->pos.y < TINYGL_HEIGHT - 1)
    {
        ninja->pos.y++;
    }

    if (navswitch_push_event_p(NAVSWITCH_NORTH) && ninja->pos.y > 1)
    {
        ninja->pos.y--;
        
    }

    update_ninja_pos(*ninja);
    
}



/*---------------------- change the position of ninja  with the navigation switch----------------------*/

void update_ninja_pos(ninja_t ninja) 
{
    static tinygl_point_t last_pos = {5, 7}; // Impossible position.
    // Note: As tinygl_draw_point call display_pixel_set which returns on invalid point 
    //       The first time the update_ninja_pos is called its impossible position wont cause an error.
    tinygl_draw_point(last_pos, 0); // Turns off last point.
    tinygl_draw_point(ninja.pos, ninja.active); // Turns on new point.
    last_pos = ninja.pos;
}


