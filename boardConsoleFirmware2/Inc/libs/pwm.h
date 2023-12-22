/*
 * pwm.h
 *
 *  Created on: Nov 2, 2023
 *      Author: bartosz
 */

/*
  	PWM handler
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

#ifndef LIBS_PWM_H_
#define LIBS_PWM_H_

#include <stdint.h>
#include <stm32g0xx_hal.h>

#define PWM_CH1 TIM_CHANNEL_1
#define PWM_CH2 TIM_CHANNEL_2
#define PWM_CH3 TIM_CHANNEL_3
#define PWM_CH4 TIM_CHANNEL_4

typedef struct{

	TIM_HandleTypeDef* timer;
	uint32_t channel;
	uint32_t maxDuty;

}pwm_TypeDef;

void pwm_init(pwm_TypeDef* pwm, TIM_HandleTypeDef* timer, uint32_t channel);
void pwm_setDuty(pwm_TypeDef* pwm, uint32_t duty);

#endif /* LIBS_PWM_H_ */
