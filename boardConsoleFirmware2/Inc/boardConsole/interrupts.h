/*
 * interrupts.h
 *
 *  Created on: Oct 12, 2023
 *      Author: bartosz
 */

/*
  	Interrupts handling for boardConsole
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

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include <stdint.h>
#include "libs/tachometer.h"
#include "libs/motohours.h"
#include "boardConsole/boardconsoleapp.h"
#include "gpio.h"


void periodElapsedCallback(TIM_HandleTypeDef *htim);

#endif /* INTERRUPTS_H_ */
