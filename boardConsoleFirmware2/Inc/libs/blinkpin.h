/*
 * blinkpin.h
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

#ifndef LIBS_BLINKPIN_H_
#define LIBS_BLINKPIN_H_

#include "stdint.h"
#include "stm32g0xx_hal.h"

enum{
	BLINKPIN_TYPE_STATIC,
	BLINKPIN_TYPE_BLINK,
	BLINKPIN_TYPE_PATTERN
};

enum{
	BLINKPIN_REPEAT_ON,
	BLINKPIN_REPEAT_OFF
};

enum{
	BLINKPIN_STAT_ON,
	BLINKPIN_STAT_OFF
};


typedef struct{

	GPIO_TypeDef* port;
	uint32_t pin;
	uint8_t type;
	uint8_t repeat;
	uint32_t pattern;
	uint32_t prescaler;

	uint8_t stat;
	uint8_t stage;

	uint32_t mainCounter;

}blinkpin_TypeDef;

void blinkpin_init(blinkpin_TypeDef *blinkpin, GPIO_TypeDef *port, uint32_t pin);
void blinkpin_pattern(blinkpin_TypeDef *blinkpin, uint32_t pattern, uint32_t prescaler, uint8_t repeat);
void blinkpin_static(blinkpin_TypeDef *blinkpin, uint8_t state);
void blinkpin_blink(blinkpin_TypeDef *blinkpin, uint32_t prescaler);

void blinkpin_execute(blinkpin_TypeDef *blinkpin);


#endif /* LIBS_BLINKPIN_H_ */
