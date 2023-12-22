/*
 * blinkpin.c
 *
 *  Created on: Nov 2, 2023
 *      Author: bartosz
 */

/*
  	Blinkpin library
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

#include "libs/blinkpin.h"

void blinkpin_init(blinkpin_TypeDef *blinkpin, GPIO_TypeDef *port, uint32_t pin) {

	blinkpin->pin = pin;
	blinkpin->port = port;
	blinkpin->stage = 31;
	blinkpin->mainCounter = 0;
	blinkpin->stat = BLINKPIN_STAT_OFF;

}

void blinkpin_pattern(blinkpin_TypeDef *blinkpin, uint32_t pattern,
		uint32_t prescaler, uint8_t repeat) {

	blinkpin->type = BLINKPIN_TYPE_PATTERN;
	blinkpin->stage = 0;
	blinkpin->pattern = pattern;
	blinkpin->prescaler = prescaler;
	blinkpin->repeat = repeat;

}

void blinkpin_static(blinkpin_TypeDef *blinkpin, uint8_t state) {

	blinkpin->type = BLINKPIN_TYPE_STATIC;
	blinkpin->stat = state;
}

void blinkpin_blink(blinkpin_TypeDef *blinkpin, uint32_t prescaler) {

	blinkpin->type = BLINKPIN_TYPE_BLINK;
	blinkpin->prescaler = prescaler;

}

void blinkpin_execute(blinkpin_TypeDef *blinkpin) {

	if (blinkpin->type == BLINKPIN_TYPE_PATTERN) {

		if (blinkpin->stage < 32
				&& (!(blinkpin->mainCounter % blinkpin->prescaler))) {

			uint8_t actualState = (blinkpin->pattern >> (31 - blinkpin->stage))
					& 1;

			if (actualState)
				HAL_GPIO_WritePin(blinkpin->port, blinkpin->pin, GPIO_PIN_SET);
			else
				HAL_GPIO_WritePin(blinkpin->port, blinkpin->pin,
						GPIO_PIN_RESET);

			blinkpin->stage++;

			if (blinkpin->repeat == BLINKPIN_REPEAT_ON && blinkpin->stage >= 31){
				blinkpin->stage = 0;
			}


		}

	} else if (blinkpin->type == BLINKPIN_TYPE_STATIC) {

		if (blinkpin->stat == BLINKPIN_STAT_ON)
			HAL_GPIO_WritePin(blinkpin->port, blinkpin->pin, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(blinkpin->port, blinkpin->pin, GPIO_PIN_RESET);

	} else if (blinkpin->type == BLINKPIN_TYPE_BLINK) {
		if (!(blinkpin->mainCounter % blinkpin->prescaler)) {
			HAL_GPIO_TogglePin(blinkpin->port, blinkpin->pin);
		}
	}

	blinkpin->mainCounter++;

}
