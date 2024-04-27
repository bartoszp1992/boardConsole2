/*
 * interrupts.c
 *
 *  Created on: Oct 12, 2023
 *      Author: bartosz
 */

/*
 Board Console Interrupts
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

#include <boardConsole/interrupts.h>

extern tachometer_TypeDef tacho;
extern motohours_TypeDef mth;
extern interface_TypeDef interface;
extern menu_TypeDef settingsMenu;
extern gps_TypeDef gps;
extern blinkpin_TypeDef buzzer;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	nmea0183_grabFrame_IT(&gps, huart);
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin) {

	tachometer_extiCallback(&tacho, GPIO_Pin);

}

void periodElapsedCallback(TIM_HandleTypeDef *htim) {

	tachometer_timerCallback(&tacho, htim);

	if (htim == &htim6)
		blinkpin_execute(&buzzer);

	if (tachometer_read(&tacho)) {
		motohours_inc(&mth, htim);
	}

}

void HAL_COMP_TriggerCallback(COMP_HandleTypeDef *hcomp) {

	if (hcomp->Instance == COMP1) {
		TIM3->CCR1 = 0;
		HAL_GPIO_WritePin(LED_STAT2_GPIO_Port, LED_STAT2_Pin, GPIO_PIN_SET);
	}

	motohours_dumpSetFlag(&mth, hcomp);

}
