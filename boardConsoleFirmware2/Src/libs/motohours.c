/*
 * motohours.c
 *
 *  Created on: Oct 16, 2023
 *      Author: bartosz
 *
 *      module for counting motohours
 *      set related timer for 1 second tick
 *
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

#include <libs/motohours.h>

/*
 * init
 * @param: motohours struct
 * @param: related comp HAL struct
 * @param: related timer HAL struct
 * @param: related eeprom struct
 * @param: mem addr for store motoseconds
 */
void motohours_init(motohours_TypeDef *mth, COMP_HandleTypeDef *comp,
		TIM_HandleTypeDef *timer, eeprom_TypeDef *eeprom, uint8_t memAddr) {
	mth->comp = comp;
	mth->eeprom = eeprom;
	mth->timer = timer;
	mth->dumpFlag = 0;
	mth->memAddr = memAddr;


	union {

		uint8_t bytes[4];
		uint32_t motoseconds;

	} readedData = { 0 };



	eeprom_read(mth->eeprom, mth->memAddr, readedData.bytes, 4);

	if (readedData.motoseconds == 0xFFFFFFFF) {
		mth->motoseconds = 0;
	} else {
		mth->motoseconds = readedData.motoseconds;
	}

	HAL_COMP_Start(mth->comp);
	HAL_TIM_Base_Start_IT(mth->timer);

}

/*
 * Set dump flag used in other function to save data if voltage fall was detected
 * (run on interrupt from falling supply)
 * !this function isn't saving data
 * intended to run in HAL_COMP_TriggerCallback()
 *
 * @param: motohours struct
 * @param: hcomp parameter from HAL_COMP_TriggerCallback()
 */
void motohours_dumpSetFlag(motohours_TypeDef *mth, COMP_HandleTypeDef *comp) {
	if (mth->comp->Instance == comp->Instance) {

		if (HAL_COMP_GetOutputLevel(mth->comp) == COMP_OUTPUT_LEVEL_LOW) {
			mth->dumpFlag = 1;
//			HAL_GPIO_TogglePin(TEST_GPIO_Port, TEST_Pin);
		}

	}
}

/*
 * dump memory
 * run as quick as possible after motohours_dumpSetFlag
 * this function check dump flag and makes eeprom write if necessary
 * Must end before supply falls
 *
 * @param: motohours struct
 */
void motohours_dump(motohours_TypeDef *mth) {
	if (mth->dumpFlag) {

		union {

			uint8_t bytes[4];
			uint32_t motoseconds;

		} dataToWrite = { 0 };

		dataToWrite.motoseconds = mth->motoseconds;


		eeprom_write(mth->eeprom, mth->memAddr, dataToWrite.bytes, 4);

		mth->dumpFlag = 0;
	}

}

/*
 * read motohours
 * @param: motohours struct
 * @retval: motohours
 */
uint32_t motohours_read(motohours_TypeDef *mth) {
	return mth->motoseconds / 60 / 60;
}

/*
 * read motoseconds
 * @param: motohours struct
 * @retval: motoseconds
 */
uint32_t motohours_readMts(motohours_TypeDef *mth) {
	return mth->motoseconds;
}

/*
 * motoseconds incremental
 * intended to run in HAL_TIM_PeriodElapsedCallback()
 *
 * @param: motohours struct
 * @param: htim parameter from HAL_TIM_PeriodElapsedCallback()
 */
void motohours_inc(motohours_TypeDef *mth, TIM_HandleTypeDef *timer) {
	if (timer->Instance == mth->timer->Instance) {
		mth->motoseconds++;

	}

}

void motohours_clear(motohours_TypeDef* mth){
	mth->motoseconds = 0;
}
