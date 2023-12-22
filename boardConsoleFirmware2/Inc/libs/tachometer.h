/*
 * tachometer.h
 *
 *  Created on: Oct 12, 2023
 *      Author: bartosz
 */

/*
  	Tachometer library
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

#ifndef TACHOMETER_H_
#define TACHOMETER_H_

#include <stdint.h>
#include "stm32g0xx_hal.h"

#define TACHOMETER_MODE_2IGN_PER_REV 0
#define TACHOMETER_MODE_1IGN_PER_REV 1
#define TACHOMETER_MODE_05IGN_PER_REV 2


typedef struct {

	TIM_HandleTypeDef *timer;
	uint32_t ticks;
	uint32_t RPM;
	uint8_t mode;

	GPIO_TypeDef *port;//exti pin define
	uint16_t pin;


} tachometer_TypeDef;

void tachometer_start(tachometer_TypeDef *tacho, TIM_HandleTypeDef *timer,
		GPIO_TypeDef *port, uint16_t pin, uint16_t noRuningTreshold, uint8_t mode);
uint32_t tachometer_read(tachometer_TypeDef* tacho);
void tachometer_extiCallback(tachometer_TypeDef *tacho, uint16_t GPIO_Pin);
void tachometer_timerCallback(tachometer_TypeDef* tacho, TIM_HandleTypeDef* timer);

#endif /* TACHOMETER_H_ */
