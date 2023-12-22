/*
 * adconv.h
 *
 *  Created on: Oct 13, 2023
 *      Author: bartosz
 */

/*
  	ADC HAL addon
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

#ifndef ADCONV_H_
#define ADCONV_H_


#include "stm32g0xx_hal.h"

//user config
#define ADCONV_CHANNEL_COUNT 4

#define ADCONV_CH0 0
#define ADCONV_CH1 1
#define ADCONV_CH2 2
#define ADCONV_CH3 3

#define ADCONV_CH_VOLTAGE ADCONV_CH0 //[mV]
#define ADCONV_CH_FUEL ADCONV_CH1 //[%]
#define ADCONV_CH_TRIM ADCONV_CH2 //[%]
#define ADCONV_CH_TEMP ADCONV_CH3 //[degC]

//end of user config

typedef struct{

	ADC_HandleTypeDef* adc;
	uint32_t readings[ADCONV_CHANNEL_COUNT];
	float multipliers[ADCONV_CHANNEL_COUNT];

}adconv_TypeDef;

void adconv_init(adconv_TypeDef *adconv, ADC_HandleTypeDef *adc);
void adconv_configChannel(adconv_TypeDef *adconv, uint8_t channel, float multiplier);
uint32_t adconv_getValue(adconv_TypeDef *adc, uint8_t channel);


#endif /* ADCONV_H_ */
