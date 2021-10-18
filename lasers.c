/** 
 * @file    lasers.c
 * @author  ZHAN (mzh99)      
 * @author  JOHN ELLIOTT (jel119)
 * @date    15 October 2021
 * 
 * @brief   This module defines the how led-mat lights up for the game.
 *          Also, it makes bitmaps for the game and checks if the player has been hit by a laser.
**/

#include "lasers.h"
#include <avr/io.h>
#include <stdlib.h>


/* ---------------------------- | Key Functions | ---------------------------- */

void lightup_boarders(void) // Turns on the board lights of the screen.
{
    tinygl_point_t point1 = {LASER_COL_MIN, LASER_ROW_MIN};
    tinygl_point_t point2 = {LASER_COL_MAX, LASER_ROW_MIN};
    tinygl_point_t point3 = {LASER_COL_MIN, LASER_ROW_MAX};
    tinygl_draw_line(point1, point2, ON);
    tinygl_draw_line(point1, point3, ON);
}

Laser_bitmap_t get_valid_bitmap(void) // Gets a bitmap which ensures the player can live.
{
    srand(TCNT1); // Seeds the random number to the press of the button to give a sense of randomness.
    // 62 and 14 are digit equivlents of binary (maximum - 1) bitmap.
    Laser_bitmap_t laser_bitmap= {rand() % 62, rand() % 14}; // Note: (row-bit-right is top), (col-bit-right is left)
    return laser_bitmap;
}

/* ---------------------------- | Laser Functions | ---------------------------- */

void change_laser_fire(Laser_bitmap_t bitmap) // Toggles if the lasers are showing in playable area.
{
    static bool is_on = true;
    // Loop through rows.
    for (uint8_t i = LASER_ROW_MIN; i < LASER_ROW_MAX; i++) {
        if (bitmap.rows & (ON << i)) {
            tinygl_point_t point1 = {LASER_COL_MIN, i + 1};
            tinygl_point_t point2 = {LASER_COL_MAX, i + 1};
            tinygl_draw_line(point1, point2, is_on);
        }
    }
    // Loop through cols.
    for (uint8_t i = LASER_COL_MIN; i < LASER_COL_MAX; i++) {
        if (bitmap.cols & (ON << i)) {
            tinygl_point_t point1 = {i + 1, LASER_ROW_MIN};
            tinygl_point_t point2 = {i + 1, LASER_ROW_MAX};
            tinygl_draw_line(point1, point2, is_on);
        }
    }
    is_on = !is_on;
}

void change_laser_flash(Laser_bitmap_t bitmap) // Toggles lasers LED on/off to flash.
{
    static bool is_on = false;
    // Loop through rows.
    for (uint8_t i = LASER_ROW_MIN; i < LASER_ROW_MAX; i++) {
        tinygl_point_t point1 = {LASER_COL_MIN, i + 1};
        if (bitmap.rows & (ON << i)) {
            tinygl_draw_point(point1, is_on);
        } else {
            tinygl_draw_point(point1, ON);
        }
    }
    // Loop through cols.
    for (uint8_t i = LASER_COL_MIN; i < LASER_COL_MAX; i++) {
        tinygl_point_t point1 = {i + 1, LASER_ROW_MIN};
        if (bitmap.cols & (ON << i)) {
            tinygl_draw_point(point1, is_on);
        } else {
            tinygl_draw_point(point1, ON);
        }
    }
    is_on = !is_on;
}

bool laser_hit_ninja(Laser_bitmap_t bitmap, ninja_t* ninja) // Checks if players postion is hit by a laser.
{
    bool is_hit = false;
    // Loop through rows.
    for (uint8_t i = LASER_ROW_MIN; i < LASER_ROW_MAX; i++) {
        if ((bitmap.rows & (ON << i)) && (ninja->pos.y == (i + 1))) {
            ninja->active = false;
            is_hit = true;
        }
    }
    // Loop through cols.
    for (uint8_t i = LASER_COL_MIN; i < LASER_COL_MAX; i++) {
        if ((bitmap.cols & (ON << i)) && (ninja->pos.x == (i + 1))){
            ninja->active = false;
            is_hit = true;
        }
    }
    return is_hit;
}
