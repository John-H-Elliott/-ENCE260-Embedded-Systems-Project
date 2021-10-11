/** @file   game.c
    @author ZHAN (mzh99)      ELLIOTT (jel119)
    @date   20 October 2021
    @brief  --

    @defgroup --
*/
#include <stdlib.h>
#include <stdio.h>
#include "system.h"
#include "led.h"
#include "button.h"
#include "navswitch.h"
#include "pacer.h"
#include "tinygl.h"
#include "../../fonts/font5x7_1.h"
#include "dodge_dash.h"


/* Define game statements used in the game */
typedef enum Game_state_e
{
    GAME_START,
    GAME_WIN,
    GAME_LOSS,
    GAME_RESET
} Game_state_t;
















int main (void)
{
    system_init ();


    while (1)
    {



    }
}