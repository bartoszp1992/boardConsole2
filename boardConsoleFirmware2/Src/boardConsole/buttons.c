/*
 * buttons.c
 *
 *  Created on: Oct 23, 2023
 *      Author: bartosz
 */

/*
  	Board Console Buttons Module
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

#include "boardConsole/buttons.h"

uint8_t buttons_read(void) {

	uint8_t buttons = (!HAL_GPIO_ReadPin(BUTTON_L_GPIO_Port, BUTTON_L_Pin) << 2)
			| (!HAL_GPIO_ReadPin(BUTTON_M_GPIO_Port, BUTTON_M_Pin) << 1)
			| (!HAL_GPIO_ReadPin(BUTTON_R_GPIO_Port, BUTTON_R_Pin) << 0);


	return buttons;

}
