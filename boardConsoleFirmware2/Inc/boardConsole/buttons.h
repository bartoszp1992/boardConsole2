/*
 * buttons.h
 *
 *  Created on: Oct 23, 2023
 *      Author: bartosz
 */

/*
  	Board Console Buttons module
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

#include "gpio.h"


#define BUTTONS_R 0x1
#define BUTTONS_M 0x2
#define BUTTONS_RM 0x3
#define BUTTONS_L 0x4
#define BUTTONS_LR 0x5
#define BUTTONS_LM 0x6
#define BUTTONS_ALL 0x7

uint8_t buttons_read(void);





