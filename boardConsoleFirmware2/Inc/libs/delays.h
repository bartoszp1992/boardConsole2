/*
 * delays.h
 *
 *	The MIT License.
 *  Created on: 11.07.2018
 *      Author: Mateusz Salamon
 *      www.msalamon.pl
 *
 */

#ifndef DELAYS_H_
#define DELAYS_H_

#include "stm32g0xx_hal.h"
//#include "tim.h"

//#define DELAYS_TIMER_HANDLER htim17

void delay_us(uint16_t us);

void delay_us_init(TIM_HandleTypeDef* timer);

#endif /* DELAYS_H_ */
