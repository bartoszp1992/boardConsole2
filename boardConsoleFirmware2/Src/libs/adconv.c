/*
 * adconv.c
 *
 *  Created on: Oct 13, 2023
 *      Author: bartosz
 *
 *      Addon for HAL ADC handling
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

#include <libs/adconv.h>

/*
 * initialize ADC
 * @param: adconv structure
 * @param: HAL adc handler
 */
void adconv_init(adconv_TypeDef *adconv, ADC_HandleTypeDef *adc) {

	adconv->adc = adc;

	for (uint8_t i = 0; i < ADCONV_CHANNEL_COUNT; i++) {
		adconv->readings[i] = 0;
	}

	for (uint8_t i = 0; i < ADCONV_CHANNEL_COUNT; i++) {
		adconv->multipliers[i] = 1;
	}


	HAL_Delay(10);

	HAL_ADCEx_Calibration_Start(adconv->adc);
	HAL_ADC_Start_DMA(adc, adconv->readings, ADCONV_CHANNEL_COUNT);

}

/*
 * configure channel
 * @param: adc struct
 * @param: channel ADCONV_CH_
 * @param: multiplier
 *
 */
void adconv_configChannel(adconv_TypeDef *adconv, uint8_t channel,
		float multiplier) {

//	adconv->offsets[channel] = offset;
	adconv->multipliers[channel] = multiplier;

}

/*
 * get value
 * @param: adconv struct
 * @param: channel number: ADCOV_CH_*
 * @retval: value
 */
uint32_t adconv_getValue(adconv_TypeDef *adc, uint8_t channel) {

	float retval;

	retval = adc->readings[channel] * (float)adc->multipliers[channel];

	return (uint32_t) retval;
}
