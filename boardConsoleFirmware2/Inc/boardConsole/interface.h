/*
 * interface.h
 *
 *  Created on: Oct 13, 2023
 *      Author: bartosz
 */

/*
  	Board Console Interface
    Copyright (C) 2023  Bartosz Pracz

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    Copy of license is available in repository main folder
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_

#include "libs/nmea0183.h"
#include "libs/hd44780.h"
#include <stdlib.h>
#include "libs/tachometer.h"
#include "boardconsoleapp.h"

#define INTERFACE_SCREEN_COUNT 3
#define INTERFACE_SCREEN_0 0
#define INTERFACE_SCREEN_1 1
#define INTERFACE_SCREEN_2 2
//#define INTERFACE_SCREEN_SETTINGS 3

#define INTERFACE_SWITCH_SCREEN_DIR_LEFT 0
#define INTERFACE_SWITCH_SCREEN_DIR_RIGHT 1

//screen 1
#define INTERFACE_POS_SPEEDO_X 0
#define INTERFACE_POS_SPEEDO_Y 0

#define INTERFACE_POS_TACHO_D_X 8
#define INTERFACE_POS_TACHO_D_Y 0

#define INTERFACE_POS_TACHO_A_X 0
#define INTERFACE_POS_TACHO_A_Y 1

#define INTERFACE_POS_COURSE_X 0
#define INTERFACE_POS_COURSE_Y 1

#define INTERFACE_POS_CLOCK_X 11
#define INTERFACE_POS_CLOCK_Y 0

#define INTERFACE_POS_VOLTAGE_X 0
#define INTERFACE_POS_VOLTAGE_Y 0

#define INTERFACE_POS_TEMP_X 0
#define INTERFACE_POS_TEMP_Y 1

#define INTERFACE_POS_FUEL_X 0
#define INTERFACE_POS_FUEL_Y 0

#define INTERFACE_POS_TRIM_X 11
#define INTERFACE_POS_TRIM_Y 0

#define INTERFACE_POS_MOTOHOURS_X 5
#define INTERFACE_POS_MOTOHOURS_Y 1

#define INTERFACE_POS_SETTINGS_NAME_X 0
#define INTERFACE_POS_SETTINGS_NAME_Y 0

#define INTERFACE_POS_SETTINGS_VALUE_X 0
#define INTERFACE_POS_SETTINGS_VALUE_Y 1



//screen 2

//screen 3

#define INTERFACE_ICON_BARRIER 0
#define INTERFACE_ICON_DISTRIBUER 1
#define INTERFACE_ICON_OUTBOARD 2
#define INTERFACE_ICON_TRIM 3
#define INTERFACE_ICON_FILL 4
#define INTERFACE_ICON_TRIANGLE 5
#define INTERFACE_ICON_DEGREE 6
#define INTERFACE_ICON_ARROW 7


extern gps_TypeDef gps;

typedef struct {
	//[lines][columns]
	char framebuffer[2][16];
	uint8_t actualScreen;

}interface_TypeDef;

void interface_init(interface_TypeDef* interface);
void interface_swipeScreen(interface_TypeDef* interface, uint8_t direction);
void interface_switchScreen(interface_TypeDef* interface, uint8_t screen);
void interface_display(interface_TypeDef* interface);


#endif /* INTERFACE_H_ */
