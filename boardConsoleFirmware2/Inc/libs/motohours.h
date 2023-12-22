/*
 * motohours.h
 *
 *  Created on: Oct 16, 2023
 *      Author: bartosz
 */

/*
  	Motohours library
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

#ifndef MOTOHOURS_H_
#define MOTOHOURS_H_

#include "libs/eeprom.h"
#include "stm32g0xx_hal.h"

typedef struct {

	COMP_HandleTypeDef *comp;
	eeprom_TypeDef *eeprom;
	TIM_HandleTypeDef* timer;
	uint8_t memAddr;
	uint32_t motoseconds;
	uint8_t dumpFlag;

} motohours_TypeDef;

void motohours_init(motohours_TypeDef *mth, COMP_HandleTypeDef *comp,
		TIM_HandleTypeDef *timer, eeprom_TypeDef *eeprom, uint8_t memAddr);
void motohours_dumpSetFlag(motohours_TypeDef *mth, COMP_HandleTypeDef *comp);
void motohours_dump(motohours_TypeDef *mth);
uint32_t motohours_read(motohours_TypeDef *mth);
uint32_t motohours_readMts(motohours_TypeDef *mth);
void motohours_inc(motohours_TypeDef *mth, TIM_HandleTypeDef* timer);
void motohours_clear(motohours_TypeDef* mth);

#endif /* MOTOHOURS_H_ */
