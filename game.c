/** 
 * @file   game.c
 * @author ZHAN (mzh99)
 * @author JOHN ELLIOTT (jel119)
 * @date   20 October 2021
 * @brief  --
 * @defgroup --
**/

#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include "system.h"
#include "navswitch.h"
#include "led.h"
#include "button.h"
#include "pacer.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include "ninja.h"
#include "lasers.h"

#define LOOP_RATE 500           /* Define polling rate in Hz. */
#define DISPLAY_TASK_RATE 10    /* Message display rate */
#define FLASH_RATE 3 /* This is the rate (Hz) of flasher changes.  */
#define LASER_RATE 20 /* This is the rate (Hz) of flasher changes.  */

/*---------------------- Define game statements used in the game ----------------------*/

typedef enum Game_state_e
{
    START_SCREEN,
    GAME_START,
    GAME_OVER,
    GAME_RESET
} Game_state_t;



/* POLL_RATE (Hz)
   MOD_FREQ (Hz)
   MOD_DUTY (percent)
   FLASHER_PERIOD (s)      - period between flashes
   FLASHER_DUTY (percent)  - proportion of flash period that is lit
   FLASHES (integer)       - how many flashes per flash pattern
   PERIOD (s)              - how often the flash pattern repeats
*/

void init_game(Game_state_t *game_state, ninja_t *ninja, Laser_bitmap_t *bitmap) {
    *game_state = GAME_START;
    tinygl_clear();
    led_set(LED1, ninja->active); // Turns the blue light on to show the ninja is alive and games starting.
    lightup_boarders();
    *bitmap = get_valid_bitmap();
    update_ninja_pos(*ninja);
}

void screen_init(void) {
        // Text modules    
    tinygl_init(LOOP_RATE);
    tinygl_font_set(&font5x7_1);
    tinygl_text_speed_set(DISPLAY_TASK_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
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
    // Start Counter with 8bit-prescaler.
    TCCR1A = 0x00; 
    TCCR1B = 0x05; 
    TCCR1C = 0x00;

    // Tasks counters
    uint16_t flash_tick;
    uint16_t laser_tick;
    Laser_bitmap_t bitmap;
    bool lasers_on;

    screen_init();

    // Ninja (player) and game state initialization.
    ninja_t ninja;
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
                ninja_init(&ninja); // Start in the bottem right.
                flash_tick = 0;
                laser_tick = 0;
                lasers_on = false;
                init_game(&game_state, &ninja, &bitmap);
                
            }
            break;

        case GAME_START:
            ninja_movement(&ninja);

            flash_tick++;
            if ((flash_tick >= (LOOP_RATE / FLASH_RATE)) && (!lasers_on)) {
                flash_tick = 0;
                change_laser_flash(bitmap);
            }
            
            laser_tick++;
            if (laser_tick >= (LOOP_RATE * FLASH_RATE)) {
                laser_tick = 0;
                change_laser_fire(bitmap);
                lasers_on = !lasers_on;
                if (!lasers_on) {
                    bitmap = get_valid_bitmap();
                }
            }
            
            if (lasers_on && (laser_hit_ninja(bitmap, &ninja))) {
                game_state = GAME_OVER;
                led_set(LED1, ninja.active);
                change_laser_fire(bitmap);
                tinygl_clear();
                tinygl_text("Game Over!");
            }
            break;
        case GAME_OVER:
            game_state = START_SCREEN;
            break;
        default:
            break;
        }
        tinygl_update();
    }
    return 0;
}