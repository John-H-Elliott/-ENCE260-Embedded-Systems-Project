/** @file   game.c
    @author ZHAN (mzh99)      ELLIOTT (jel119)
    @date   20 October 2021
    @brief  --

    @defgroup --
**/
#include <stdlib.h>
#include <stdio.h>
#include "system.h"
#include "led.h"
#include "button.h"
#include "navswitch.h"
#include "pacer.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include "dodge_dash.h"

#define NINJA_SPEED 1 /* The Ninja move speed  */
#define LOOP_RATE 300/* Define polling rate in Hz. */
#define DISPLAY_TASK_RATE 250 /* Message display rate */


/*---------------------- Define game statements used in the game ----------------------*/
/*
typedef enum Game_state_e
{
    GAME_START,
    GAME_WIN,
    GAME_LOSS,
    GAME_RESET
} Game_state_t;
*/



/*---------------------- 1 ----------------------*/

















int main (void)
{
    /*---------------------- Modules Initialisation ----------------------*/
    system_init();
    led_init();
    button_init ();
    navswitch_init ();
    pacer_init (LOOP_RATE);

    
    tinygl_init(LOOP_RATE);
    tinygl_font_set(&font5x7_1);
    tinygl_text_speed_set(DISPLAY_TASK_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_speed_set (25);



    ninja_t ninja;
    ninja_initalpos ()
    tinygl_draw_point (ninja.pos, ninja.active);
    led_set (LED1, ninja.active);

    /*---------------------- Main loop ----------------------*/
    while (1)
    {
        pacer_wait();  /* Wait for next tick.  */

        navswitch_update ();  /* Update info from navswitch  */
        ninja_movement ()    /* Update new position of ninja  */

        tinygl_update ();
        

    }
    return 0;
}