/** 
 * @file   game.c
 * @author ZHAN (mzh99)
 * @author JOHN ELLIOTT (jel119)
 * @date   20 October 2021
 * @brief  --
 * @defgroup --
**/

#include "system.h"
#include "navswitch.h"
#include "led.h"
#include "button.h"
#include "pacer.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include "dodge_dash.h"

#define NINJA_SPEED 1           /* The Ninja move speed  */
#define LOOP_RATE 500           /* Define polling rate in Hz. */
#define DISPLAY_TASK_RATE 10   /* Message display rate */


/*---------------------- Define game statements used in the game ----------------------*/

typedef enum Game_state_e
{
    START_SCREEN,
    GAME_START,
    GAME_OVER,
    GAME_RESET
} Game_state_t;


void lightup_boarders(void) 
{
    tinygl_point_t point1 = {0, 0};
    tinygl_point_t point2 = {4, 0};
    tinygl_point_t point3 = {0, 6};
    tinygl_draw_line(point1, point2, 1);
    tinygl_draw_line(point1, point3, 1);
}


int main (void)
{
    /*---------------------- Modules Initialisation ----------------------*/
    // Basic system modules
    system_init();
    led_init();
    led_set(LED1, 0); // Off as the game doesn't start until the button is pressed.
    button_init();
    navswitch_init();

    // Text modules    
    tinygl_init(LOOP_RATE);
    tinygl_font_set(&font5x7_1);
    tinygl_text_speed_set(DISPLAY_TASK_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);

    // Ninja (player) and game state initialization.
    ninja_t ninja;
    ninja_init(&ninja); // Start in the bottem right.
    update_ninja_pos(ninja);
    Game_state_t game_state = START_SCREEN;

    // Initializes the pacer.
    pacer_init(LOOP_RATE);
    tinygl_text("Dodge & Dash! Press Button To Start");
    /*---------------------- Main loop ----------------------*/
    while (1)
    {
        pacer_wait(); // Wait for next tick.
        switch (game_state)
        {
        case START_SCREEN:
            button_update();
            if (button_push_event_p(BUTTON1)) {
                game_state = GAME_START;
                tinygl_clear();
                led_set(LED1, ninja.active); // Turns the blue light on to show the ninja is alive and games starting.
                lightup_boarders();
            }
            break;
        case GAME_START:
            ninja_movement(&ninja);

            break;
        case GAME_OVER:
            /* code */
            break;
        case GAME_RESET:
            /* code */
            break;
        default:
            break;
        }
        tinygl_update();
    }
    return 0;
}