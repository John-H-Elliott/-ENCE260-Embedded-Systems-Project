/** 
 * @file   dodge_dash.h
 * @author ZHAN (mzh99)      
 * @author JOHN ELLIOTT (jel119)
 * @date   20 October 2021
 * @brief  --
 * @defgroup --
**/

#ifndef LASERS_H
#define LASERS_H

#include <stdint.h>
#include <stdbool.h>
#include "tinygl.h"
#include "ninja.h"

#define LASER_ROW_MIN 0
#define LASER_COL_MIN 0
#define LASER_ROW_MAX 6
#define LASER_COL_MAX 4
#define ON 1
#define OFF 0

/*----------------------- | Structs | -----------------------*/

typedef struct {
    uint8_t rows;
    uint8_t cols;
} Laser_bitmap_t;


/*---------------------- | Functions | ----------------------*/

void lightup_boarders(void);

void change_laser_fire(Laser_bitmap_t);

void change_laser_flash(Laser_bitmap_t);

Laser_bitmap_t get_valid_bitmap(void);

bool laser_hit_ninja(Laser_bitmap_t, ninja_t*);


#endif