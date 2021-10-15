/** 
 * @file   game.c
 * @author ZHAN (mzh99)
 * @author JOHN ELLIOTT (jel119)
 * @date   20 October 2021
 * @brief  --
 * @defgroup --
**/

#include <stdint.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include "system.h"
#include "navswitch.h"
#include "led.h"
#include "button.h"
#include "pacer.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include "ninja.h"
#include "lasers.h"


#define LOOP_RATE 500           // Define polling rate. 
#define DISPLAY_TASK_RATE 15    // Message display rate.
#define FLASH_RATE 150            // This is the rate of the laser flashing.
#define LASER_DEFAULT_RATE 1500    // This is the rate of the lasers firing.
#define MAX_INT 65535           // This is the maximum value of a uint16_t.


/*---------------------- | Game Setup | ----------------------*/

typedef enum Game_state_e
{
    START_SCREEN,
    GAME_SETUP,
    GAME_START,
    GAME_OVER,
    GAME_RESET
} Game_state_t;


void screen_init(void) 
{
        // Text modules    
    tinygl_init(LOOP_RATE);
    tinygl_font_set(&font5x7_1);
    tinygl_text_speed_set(DISPLAY_TASK_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
}


/*---------------------------------- | Main Game | ----------------------------------*/

int main (void)
{
    /*---------------------- Game Initialisation ----------------------*/
    // Basic system inits.
    system_init();
    led_init();
    led_set(LED1, OFF); // Off as the game doesn't start until the button is pressed.
    button_init();
    navswitch_init();
    screen_init();
    
    // Start Counter with 8-bit prescaler.
    TCCR1A = 0x00; 
    TCCR1B = 0x05; 
    TCCR1C = 0x00;

    // Tasks counters.
    uint16_t flash_tick;
    uint16_t laser_tick;
    uint16_t laser_speed;
    uint8_t smoke_bombs;
    Laser_bitmap_t bitmap;
    bool lasers_on;
    bool bomb_thrown;

    // Score record. 
    uint16_t score;

    // Ninja (player) and game state initialization.
    ninja_t ninja;
    Game_state_t game_state = START_SCREEN;

    // Initializes the pacer.
    pacer_init(LOOP_RATE);

    tinygl_text("Dodge&Dash! Push Button To Start");

    /*---------------------- Main loop ----------------------*/
    while (ON)
    {
        pacer_wait(); // Wait for next tick.

        switch(game_state) 
        {
        case START_SCREEN: // Waits for player to click the start button.
            button_update();
            if (button_push_event_p(BUTTON1)) { 
                game_state = GAME_SETUP;
                tinygl_clear();
            }
            break;


        case GAME_SETUP: // Sets/resets the game variables. 
            flash_tick = 0;
            laser_tick = 0;
            score = 0;
            laser_speed = 0;
            smoke_bombs = 5;
            lasers_on = OFF;
            bomb_thrown = false;
            

            led_set(LED1, ON); // Turns the blue light on to show the ninja is alive and games starting.
            lightup_boarders();
            bitmap = get_valid_bitmap();

            ninja_init(&ninja); // Start in the bottem right.
            update_ninja_pos(ninja); // First call to update which turns on the players inital LED.

            game_state = GAME_START; // Starts game.
            break;


        case GAME_START: // Main Game.
        
            if (ninja.active) {
                ninja_movement(&ninja);

                flash_tick++;
                if ((flash_tick >= FLASH_RATE) && (!lasers_on)) { // Only flashes if the lasers are off.
                flash_tick = 0;
                change_laser_flash(bitmap);
                }
                // This pauses the lasers while the user has the navstick pushed down.
                // But they only have a certain number {smoke_bombs} of uses.
                if (!lasers_on && thrown_smoke_bomb() && (smoke_bombs > 0)) {
                    laser_tick = 0;
                    bomb_thrown = true;
                } else if (bomb_thrown) { // Navstick is back up so remove a smoke_bomb.
                    smoke_bombs--;
                    bomb_thrown = false;
                }
            }
            
            laser_tick++;
            if (laser_tick >= (LASER_DEFAULT_RATE - laser_speed)) { // Determines when the lasers are shot.
                laser_tick = 0;
                change_laser_fire(bitmap);
                lasers_on = !lasers_on;

                // Only update these variables when lasers turn back off (player survies).
                
                if (!lasers_on) {
                    if (ninja.active) { // Checks if the player is still alive or on final lasers (dead).
                        bitmap = get_valid_bitmap();
                        if (score != MAX_INT) { // Assures the value doesn't somehow exceed uint16_t max size.
                            score++; // Laser survied so score increases.
                        }
                        if (laser_speed < 1200) { // Ensures lasers aren't too fast.
                            laser_speed += 50;
                        }
                    } else {
                        game_state = GAME_OVER; // Final lasers are done and the game is over.
                    }
                }
            }

            // If the lasers are on then the players position is checked to see if they were hit with a laser.
            if (lasers_on && (laser_hit_ninja(bitmap, &ninja))) { 
                ninja.active = OFF;
                led_set(LED1, OFF);
                laser_speed = 0;  // Reset to make sure the player can see the final lasers.
            }
            break;


        case GAME_OVER: // Resets to the start to wait for a new game.
            tinygl_clear(); // Clears the game screen.
            
            char end_text[24]; 
            // There is no overflow possible as the array can hold the string (with \0) and maximum size of the uint16_t.
            sprintf(end_text,"Game Over! Score: %d", score); 
            tinygl_text(end_text);
            game_state = START_SCREEN;
            break;
        default:
            break;
        }
        tinygl_update(); // Updates the screen.
    }

    return 0;
}
