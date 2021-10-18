/** 
 * @file    game.c
 * @author  MENGHAO ZHAN (mzh99)
 * @author  JOHN ELLIOTT (jel119)
 * @date    5 October 2021
 * 
 * @brief   This is the main game file. This defines the game variables and initializes the boards systems.
 *          It then loops (forever) going through different game stages as the player plays the game.
 * 
 * @note    For information on how to play the game consult the README.md file.
**/

#include <stdint.h>
#include <avr/io.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "system.h"
#include "navswitch.h"
#include "led.h"
#include "button.h"
#include "pacer.h"
#include "tinygl.h"
#include "../fonts/font3x5_1.h"
#include "ninja.h"
#include "lasers.h"
#include "music.h"

#define LOOP_RATE 20000           // Define polling rate [This is high due to the music being played]. 
#define GAME_LOOP 30              // Actual game rate.
#define DISPLAY_TASK_RATE 500     // Message display rate.
#define DISPLAY_TEXT_RATE 10      // Text speed rate.
#define FLASH_RATE 300            // This is the rate of the laser flashing.
#define LASER_DEFAULT_RATE 5000   // This is the rate of the lasers firing.
#define MAX_INT 65535             // This is the maximum value of a uint16_t.
#define MAX_ARRAY 24              // This defines the max size of the end screen char array.
#define LASER_SPEED_MAX 3500      // This is the maximum speed that the lasers will be speed up by.
#define SMOKE_BOMB_MAX 5          // This is the number of smoke bombs the player has.
#define LASER_SPEED_INCREASE 100  // This is the speed which the lasers are increased by each time.

/*---------------------- | Game Setup | ----------------------*/

typedef enum Game_state_e
{
    START_SCREEN,
    GAME_SETUP,
    GAME_START,
    GAME_OVER
} Game_state_t;


void screen_init(void) 
{ 
    tinygl_init(DISPLAY_TASK_RATE);
    tinygl_font_set(&font3x5_1); // Can't use 5x7_1 as the font seems to stop the tweeter from working.
    tinygl_text_speed_set(DISPLAY_TEXT_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
}


/*---------------------------------- | Main Game | ----------------------------------*/

int main (void)
{
    /*---------------------- Game Initialization ----------------------*/
    // Basic system inits.
    system_init();
    led_init();
    led_set(LED1, OFF); // Off as the game doesn't start until the button is pressed.
    button_init();
    navswitch_init();
    screen_init();
    tweeter_task_init ();
    tune_task_init ();
    
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

    // Music and Game rates.
    uint16_t tune_tick = 0;
    uint16_t screen_tick = 0;
    
    tinygl_text("DODGE&DASH! PUSH BUTTON TO START");

    /*---------------------- Main loop ----------------------*/
    while (ON)
    {
        pacer_wait(); // Wait for next tick.
        
        tweeter_task(); // Loops set to work at tweeter task rate.
        tune_tick++;
        if (tune_tick >= TUNE_TASK_RATE) {
            tune_tick = 0;
            tune_task();
        }

        /*---------------------- Main Game ----------------------*/
        // As the pacer loop is very quick due to the music rate having to be fast a ticker is check to set a slower polling rate.
        screen_tick++; 
        if (screen_tick >= GAME_LOOP) {
            screen_tick = 0;
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
                smoke_bombs = SMOKE_BOMB_MAX;
                lasers_on = OFF;
                bomb_thrown = false;
                

                led_set(LED1, ON); // Turns the blue light on to show the ninja is alive and games starting.
                lightup_boarders();
                bitmap = get_valid_bitmap();

                ninja_init(&ninja); // Start in the bottem right.
                update_ninja_pos(ninja); // First call to update which turns on the players inital LED.

                game_state = GAME_START; // Starts game.
                music_play(); 
                break;


            case GAME_START: // Main Game.
                if (ninja.active) {
                    ninja_movement(&ninja);

                    flash_tick++;
                    if (flash_tick >= FLASH_RATE) { // Flashing counter.
                        flash_tick = 0;
                        if (!lasers_on) {
                            change_laser_flash(bitmap); // Only flashes if the lasers are off.
                        } else {
                            ninja_flash(ninja); // Only flashes if the lasers are on.
                        }
                    }
                    // This pauses the lasers while the user has the navstick pushed down.
                    // But they only have a certain number {smoke_bombs} of uses.
                    if (!lasers_on && thrown_smoke_bomb() && (smoke_bombs > 0)) {
                        laser_tick = 0;
                        bomb_thrown = true;
                    } else if (bomb_thrown) { // Navstick is back up so remove a smoke_bomb.
                        smoke_bombs--;
                        bomb_thrown = false;
                    } else if (smoke_bombs == 0) {
                        tinygl_point_t corner_point = {LASER_COL_MIN, LASER_ROW_MIN};
                        tinygl_draw_point(corner_point, OFF);  // Turns off corner to show the ninja is out of smoke bombs.
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
                            // Re-updates the ninjas position so that if their light was turned off during the flashing it is turned on when the laser are turned off.
                            update_ninja_pos(ninja); 
                            if (score != MAX_INT) { // Assures the value doesn't somehow exceed uint16_t max size.
                                score++; // Laser survied so score increases.
                            }
                            if (laser_speed < LASER_SPEED_MAX) { // Ensures lasers aren't too fast.
                                laser_speed += LASER_SPEED_INCREASE;
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
                
                char end_text[MAX_ARRAY]; 
                // There is no overflow possible as the array can hold the string (with \0) and maximum size of the uint16_t.
                sprintf(end_text,"GAME OVER! SCORE: %d", score);  // Adds score to the game over string.
                tinygl_text(end_text);
                game_state = START_SCREEN;
                music_off();
                break;
            default:
                break;
            }

            tinygl_update(); // Updates the screen.
        }
    }

    return 0;
}
