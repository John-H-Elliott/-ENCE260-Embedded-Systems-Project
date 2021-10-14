/** 
 * @file   game.c
 * @author ZHAN (mzh99)
 * @author JOHN ELLIOTT (jel119)
 * @date   20 October 2021
 * @brief  --
 * @defgroup --
**/

#include <avr/io.h>
#include <stdlib.h>
#include <stdbool.h>
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



#define FLASH_RATE 10 /* This is the rate (Hz) of flasher changes.  */

/*---------------------- Define game statements used in the game ----------------------*/

typedef enum Game_state_e
{
    START_SCREEN,
    GAME_START,
    GAME_OVER,
    GAME_RESET
} Game_state_t;

typedef struct {
    uint8_t rows;
    uint8_t cols;
} Laser_bitmap;








/* POLL_RATE (Hz)
   MOD_FREQ (Hz)
   MOD_DUTY (percent)
   FLASHER_PERIOD (s)      - period between flashes
   FLASHER_DUTY (percent)  - proportion of flash period that is lit
   FLASHES (integer)       - how many flashes per flash pattern
   PERIOD (s)              - how often the flash pattern repeats
*/




void lightup_boarders(void) 
{
    tinygl_point_t point1 = {0, 0};
    tinygl_point_t point2 = {4, 0};
    tinygl_point_t point3 = {0, 6};
    tinygl_draw_line(point1, point2, 1);
    tinygl_draw_line(point1, point3, 1);
}

void turn_on_bitmap(Laser_bitmap bitmap)
{
    //loop through rows
    for (uint8_t i=0; i < 6; i++) {
        if (bitmap.rows & (1 << i)) {
            tinygl_point_t point1 = {0, i + 1};
            tinygl_point_t point2 = {4, i + 1};
            tinygl_draw_line(point1, point2, 1);
        }
    }
    //loop through cols
    for (uint8_t i=0; i < 4; i++) {
        if (bitmap.cols & (1 << i)) {
            tinygl_point_t point1 = {i + 1, 0};
            tinygl_point_t point2 = {i + 1, 6};
            tinygl_draw_line(point1, point2, 1);
        }
    }
}


void turn_off_bitmap(Laser_bitmap bitmap)
{
    //loop through rows
    for (uint8_t i=0; i < 6; i++) {
        if (bitmap.rows & (1 << i)) {
            tinygl_point_t point1 = {0, i + 1};
            tinygl_point_t point2 = {4, i + 1};
            tinygl_draw_line(point1, point2, 0);
        }
    }
    //loop through cols
    for (uint8_t i=0; i < 4; i++) {
        if (bitmap.cols & (1 << i)) {
            tinygl_point_t point1 = {i + 1, 0};
            tinygl_point_t point2 = {i + 1, 6};
            tinygl_draw_line(point1, point2, 0);
        }
    }
}


void prev_flash_off(Laser_bitmap bitmap)
{
    //loop through rows
    for (uint8_t i=0; i < 6; i++) {
        if (bitmap.rows & (1 << i)) {
            tinygl_point_t point1 = {0, i + 1};
            tinygl_draw_point(point1, 0);
        }
    }
    //loop through cols
    for (uint8_t i=0; i < 4; i++) {
        if (bitmap.cols & (1 << i)) {
            tinygl_point_t point1 = {i + 1, 0};
            tinygl_draw_point(point1, 0);
        }
    }
}

void prev_flash_on(Laser_bitmap bitmap)
{
    //loop through rows
    for (uint8_t i=0; i < 6; i++) {
        if (bitmap.rows & (1 << i)) {
            tinygl_point_t point1 = {0, i + 1};
            tinygl_draw_point(point1, 1);
        }
    }
    //loop through cols
    for (uint8_t i=0; i < 4; i++) {
        if (bitmap.cols & (1 << i)) {
            tinygl_point_t point1 = {i + 1, 0};
            tinygl_draw_point(point1, 1);
        }
    }
}







Laser_bitmap get_valid_bitmap(void)
{
    srand(TCNT1);
    Laser_bitmap bitmap = {rand() % 62, rand() % 14}; // (row-bit-right is top), (col-bit-right is left)
    return bitmap;
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

    /** Clock
    TCCR0A = 0x00; 
    TCCR0B = 0x05; TCNT0 = 0; */

    TCCR1A = 0x00; 
    TCCR1B = 0x05; 
    TCCR1C = 0x00;

    uint16_t flash_tick = 0;
    bool flash_flag = false; 


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
        flash_tick++;

        switch (game_state)
        {
        case START_SCREEN:
            button_update();
            if (button_push_event_p(BUTTON1)) {
                game_state = GAME_START;
                tinygl_clear();
                led_set(LED1, ninja.active); // Turns the blue light on to show the ninja is alive and games starting.
                lightup_boarders();
                Laser_bitmap bitmap = get_valid_bitmap();
                turn_on_bitmap(bitmap);

                if (flash_tick >= LOOP_RATE / FLASH_RATE ) { // Flasher part
                    flash_tick = 0;
                    if (!flash_flag)
                    {
                        prev_flash_off(bitmap);
                        flash_flag = true;
                    }
                    else
                    {
                        prev_flash_on(bitmap);
                        flash_flag = false;
                    }
                }

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