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

#define GAME_RATE 1 /* GAME  */
#define PACER_RATE 500 /* Pacer loop - controls main loop */
#define DISPLAY_TASK_RATE 250 /* Message display rate */


/*---------------------- Define game statements used in the game ----------------------*/
typedef enum Game_state_e
{
    GAME_START,
    GAME_WIN,
    GAME_LOSS,
    GAME_RESET
} Game_state_t;




/*---------------------- 1 ----------------------*/

















int main (void)
{
    /*---------------------- Modules Initialisation ----------------------*/
    system_init();
    led_init();
    button_init ();
    navswitch_init ();
    pacer_init (PACER_RATE);

    tinygl_font_set(&font5x7_1);
    tinygl_init(PACER_RATE);
    tinygl_text_speed_set(DISPLAY_TASK_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_speed_set (25);




    /*---------------------- Main loop ----------------------*/
    while (1)
    {
        pacer_wait();


        

    }
}