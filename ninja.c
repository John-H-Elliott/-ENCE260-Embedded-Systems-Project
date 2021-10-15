/** 
 * @file   dodge_dash.c
 * @author ZHAN (mzh99)      
 * @author JOHN ELLIOTT (jel119)
 * @date   20 October 2021
 * @brief  --
 * @defgroup --
**/

#include "ninja.h"
#include "navswitch.h"
#include "lasers.h"

/*---------------------- | Ninja Initialisation | ----------------------*/

void ninja_init(ninja_t* ninja)
{
    ninja->pos.x = LASER_COL_MAX;
    ninja->pos.y = LASER_ROW_MAX;
    ninja->active = ON; // Game is running.
}


/*---------------------- | Checks Navswitch to Update Player | ----------------------*/

void ninja_movement(ninja_t* ninja)
{
    navswitch_update(); // Update information from navswitch.

    if (navswitch_push_event_p(NAVSWITCH_WEST) && (ninja->pos.x > 1)) {
        ninja->pos.x--;
    } else if (navswitch_push_event_p(NAVSWITCH_EAST) && (ninja->pos.x < LASER_COL_MAX)) {
        ninja->pos.x++;
    } else if (navswitch_push_event_p(NAVSWITCH_SOUTH) && (ninja->pos.y < LASER_ROW_MAX)) {
        ninja->pos.y++;
    } else if (navswitch_push_event_p(NAVSWITCH_NORTH) && ninja->pos.y > 1) {
        ninja->pos.y--;
    }

    update_ninja_pos(*ninja);
}


bool thrown_smoke_bomb(void) 
{
    bool thrown = false;
    navswitch_update(); // Update information from navswitch.

    if (navswitch_down_p(NAVSWITCH_PUSH)) {
        thrown = true;
    }
    
    return thrown;
}


/*---------------------- | Updates Players LED-Mat Light | ----------------------*/

void update_ninja_pos(ninja_t ninja) 
{
    static tinygl_point_t last_pos = {5, 7}; // Impossible position.
    // Note: As tinygl_draw_point call display_pixel_set which returns on invalid point 
    //       The first time the update_ninja_pos is called its impossible position wont cause an error.
    tinygl_draw_point(last_pos, OFF);       // Turns off last point.
    tinygl_draw_point(ninja.pos, ON);       // Turns on new point.
    last_pos = ninja.pos;
}