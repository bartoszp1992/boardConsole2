/*
 * pwm.c
 *
 *  Created on: Nov 2, 2023
 *      Author: bartosz
 */


#include "libs/pwm.h"

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

/*
 * initialize PWM
 * @param: pwm struct
 * @param: timer hal struct
 * @param: channel PWM_CH*
 */
void pwm_init(pwm_TypeDef* pwm, TIM_HandleTypeDef* timer, uint32_t channel){

	pwm->timer = timer;
	pwm->maxDuty = pwm->timer->Instance->ARR;
	pwm->channel = channel;

	HAL_TIM_PWM_Start(pwm->timer, pwm->channel);

}


/*
 * @param: pwm struct
 * @param: channel PWM_CH*
 * @param: duty
 */
void pwm_setDuty(pwm_TypeDef* pwm, uint32_t duty){

	uint32_t ccr = 0;

	ccr = duty;

	__HAL_TIM_SET_COMPARE(pwm->timer, pwm->channel, ccr);

}
