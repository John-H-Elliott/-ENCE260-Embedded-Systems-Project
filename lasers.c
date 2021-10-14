/** 
 * @file   dodge_dash.c
 * @author ZHAN (mzh99)      
 * @author JOHN ELLIOTT (jel119)
 * @date   20 October 2021
 * @brief  --
 * @defgroup --
**/

#include "lasers.h"
#include <avr/io.h>
#include <stdlib.h>



void lightup_boarders(void) 
{
    tinygl_point_t point1 = {0, 0};
    tinygl_point_t point2 = {4, 0};
    tinygl_point_t point3 = {0, 6};
    tinygl_draw_line(point1, point2, 1);
    tinygl_draw_line(point1, point3, 1);
}


void change_laser_fire(Laser_bitmap_t bitmap)
{
    static bool is_on = true;
    //loop through rows
    for (uint8_t i=0; i < 6; i++) {
        if (bitmap.rows & (1 << i)) {
            tinygl_point_t point1 = {0, i + 1};
            tinygl_point_t point2 = {4, i + 1};
            tinygl_draw_line(point1, point2, is_on);
        }
    }
    //loop through cols
    for (uint8_t i=0; i < 4; i++) {
        if (bitmap.cols & (1 << i)) {
            tinygl_point_t point1 = {i + 1, 0};
            tinygl_point_t point2 = {i + 1, 6};
            tinygl_draw_line(point1, point2, is_on);
        }
    }
    is_on = !is_on;
}

void change_laser_flash(Laser_bitmap_t bitmap)
{
    static bool is_on = false;
    //loop through rows
    for (uint8_t i=0; i < 6; i++) {
        tinygl_point_t point1 = {0, i + 1};
        if (bitmap.rows & (1 << i)) {
            tinygl_draw_point(point1, is_on);
        } else {
            tinygl_draw_point(point1, 1);
        }
    }
    //loop through cols
    for (uint8_t i=0; i < 4; i++) {
        tinygl_point_t point1 = {i + 1, 0};
        if (bitmap.cols & (1 << i)) {
            tinygl_draw_point(point1, is_on);
        } else {
            tinygl_draw_point(point1, 1);
        }
    }
    is_on = !is_on;
}

Laser_bitmap_t get_valid_bitmap(void)
{
    srand(TCNT1);
    Laser_bitmap_t laser_bitmap= {rand() % 62, rand() % 14}; // (row-bit-right is top), (col-bit-right is left)
    return laser_bitmap;
}

bool laser_hit_ninja(Laser_bitmap_t bitmap, ninja_t *ninja) {
    bool is_hit = false;
    for (uint8_t i=0; i < 6; i++) {
        if ((bitmap.rows & (1 << i)) && (ninja->pos.y == (i + 1))) {
            ninja->active = false;
            is_hit = true;
        }
    }
    //loop through cols
    for (uint8_t i=0; i < 4; i++) {
        if ((bitmap.cols & (1 << i)) && (ninja->pos.x == (i + 1))){
            ninja->active = false;
            is_hit = true;
        }
    }
    return is_hit;
}