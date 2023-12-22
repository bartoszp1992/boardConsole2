/*
 * tachometer.c
 *
 *  Created on: Oct 12, 2023
 *      Author: bartosz
 *
 *      library requires timer with max possible ARR and prescaler set for
 *      base freq[MHz]-1(e.g 63PSC for 64MHz clock)
 *
 *      set also timer interrupt
 *
 *      turn off auto reload-preload
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

#include <libs/tachometer.h>

/*
 * init tacho
 * @param: tacho structure
 * @param: related timer HAL handler
 * @param: port of ign pin
 * @param: pin od ign pin
 * @param: treshold below stay at 0 RPM
 * @param: TACHOMETER_MODE_*
 */
void tachometer_start(tachometer_TypeDef *tacho, TIM_HandleTypeDef *timer,
		GPIO_TypeDef *port, uint16_t pin, uint16_t noRuningTreshold,
		uint8_t mode) {

	tacho->port = port;
	tacho->pin = pin;
	tacho->timer = timer;
	tacho->ticks = 0;
	tacho->RPM = 0;

	tacho->timer->Instance->ARR = (1000000 / (noRuningTreshold / 60)) - 1;

	tacho->mode = mode;

	HAL_TIM_Base_Start_IT(timer);

}

/*s
 * switch tachometer mode
 * @param: tacho struct
 * @param: mode TACHOMETER_MODE_*
 */
void tachometer_switchMode(tachometer_TypeDef *tacho, uint8_t mode) {
	tacho->mode = mode;
}

/*
 * read tacho value
 * @param: tacho struct
 * @retval: revs in RPM
 */
uint32_t tachometer_read(tachometer_TypeDef *tacho) {

	return tacho->RPM;
}

/*
 * run in HAL_GPIO_EXTI_Rising_Callback()
 * @param: tacho struct
 * @param: GPIO_Pin parameter from HAL_GPIO_EXTI_Rising_Callback()
 */
void tachometer_extiCallback(tachometer_TypeDef *tacho, uint16_t GPIO_Pin) {
	if (GPIO_Pin == tacho->pin && (HAL_GPIO_ReadPin(tacho->port, tacho->pin)==1)) {

		tacho->ticks = tacho->timer->Instance->CNT;
		tacho->timer->Instance->CNT = 0;

		switch (tacho->mode) {
		case TACHOMETER_MODE_1IGN_PER_REV:
			tacho->RPM = (1000000 / tacho->ticks) * 60;
			break;
		case TACHOMETER_MODE_2IGN_PER_REV:
			tacho->RPM = ((1000000 / tacho->ticks) * 60) / 2;
			break;
		case TACHOMETER_MODE_05IGN_PER_REV:
			tacho->RPM = ((1000000 / tacho->ticks) * 60) * 2;
			break;

		}

	}
}

/*
 * run in HAL_TIM_PeriodElapsedCallback()
 * @param: tacho struct
 * @param: htim parameter from HAL_TIM_PeriodElapsedCallback()
 */
void tachometer_timerCallback(tachometer_TypeDef *tacho,
		TIM_HandleTypeDef *timer) {
	if (timer == tacho->timer) {
		tacho->RPM = 0;

	}
}
