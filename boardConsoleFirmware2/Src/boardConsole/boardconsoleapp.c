/*
 * boardconsoleapp.c
 *
 *  Created on: Oct 12, 2023
 *      Author: bartosz
 *
 *
 *
 *      DISPLAY/DELAY-US- TIM17
 *      MOTOHOURS- TIM16
 *      TACHO- TIM2
 *      BACKLIGHT- TIM3
 *      LOW FUEL ALERT- TIM6
 *
 *      changelog:
 *      v2.1- add unit setting
 *
 *
 */

/*
 Board Console Main launcher
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

#include <boardConsole/boardconsoleapp.h>

//boardComputerConfig_TypeDef config;

TaskHandle_t task_display, task_gpsGrab, task_mthDump, task_buttonRead,
		task_brightness, task_lowFuelAlert;

gps_TypeDef gps;
tachometer_TypeDef tacho;
adconv_TypeDef adConv;
eeprom_TypeDef eeprom;
motohours_TypeDef mth;
menu_TypeDef settingsMenu;
interface_TypeDef interface;
pwm_TypeDef brightnessPWM;
blinkpin_TypeDef buzzer;

void settingsTask(void *pvPremeters);
void displayTask(void *pvPremeters);
void gpsGrabTask(void *pvPremeters);
void mthDumpTask(void *pvPremeters);
void buttonsTask(void *pvPremeters);
void brightnessTask(void *pvPremeters);
void lowFuelAlertDisplayReInitTask(void *pvPremeters);

void playSplash(void);

void readConfigFromEEPROM(void);
void writeConfigToEEPROM(void);

UBaseType_t gpsWaterMark, displayWaterMark, mthDumpWaterMark,
		brightnessWaterMark, buttonsWaterMark, lowFuelWaterMark;

size_t freeHeap;

void boardComputer_process(void) {

//										BUZZER
	HAL_TIM_Base_Start_IT(&htim6);
	blinkpin_init(&buzzer, BUZZER_GPIO_Port, BUZZER_Pin);

//										DISPSLAY BRIGHTNESS

	pwm_init(&brightnessPWM, &htim3, PWM_CH1);
	pwm_setDuty(&brightnessPWM, 100);

//										DISPLAY

	delay_us_init(&htim17);
	LCD_Init();
	LCD_Locate(0, 0);
	LCD_String("Board Computer");
	LCD_Locate(0, 1);
	LCD_String("V2.1");
	HAL_Delay(1000);
	LCD_Cls();

//	playSplash();

//										GPS
//	nmea0183_init_poll(&gps, &huart1);
	nmea0183_init_IT(&gps, &huart1);

//										EEPROM

	eeprom_init(&eeprom, &hi2c1, 0x50 << 1, 7, 1, 10);

//										MENU

	menu_init(&settingsMenu);

////////////////////////////////
	menu_itemInit(&settingsMenu, SETTINGS_BRIGHTNESS_ENTRY,
	SETTINGS_BRIGHTNESS_LEVEL, ">brightness", 0, 100, MENU_NONE,
	MENU_NONE,
	MENU_NONE);

	menu_itemInit(&settingsMenu, SETTINGS_CLOCK_OFFSET_ENTRY,
	SETTINGS_CLOCK_OFFSET_LEVEL, ">clock offset", -24, 24, MENU_NONE,
	MENU_NONE,
	MENU_NONE);

	menu_itemInit(&settingsMenu, SETTINGS_TEMP_OFFSET_ENTRY,
	SETTINGS_TEMP_OFFSET_LEVEL, ">temp offset", -30, 15, MENU_NONE,
	MENU_NONE,
	MENU_NONE);

	menu_itemInit(&settingsMenu, SETTINGS_TRIM_ENTRY,
	SETTINGS_TRIM_LEVEL, ">trim", 0, 4095, MENU_NONE,
	SETTINGS_TRIM_MAX_ENTRY,
	SETTINGS_TRIM_REV_ENTRY);

	menu_itemInit(&settingsMenu, SETTINGS_TRIM_MAX_ENTRY,
	SETTINGS_TRIM_MAX_LEVEL, ">trim max", 0, 4095, SETTINGS_TRIM_ENTRY,
	MENU_NONE,
	MENU_NONE);

	menu_itemInit(&settingsMenu, SETTINGS_TRIM_MIN_ENTRY,
	SETTINGS_TRIM_MIN_LEVEL, ">trim min", 0, 4095, SETTINGS_TRIM_ENTRY,
	MENU_NONE,
	MENU_NONE);

	menu_itemInit(&settingsMenu, SETTINGS_TRIM_REV_ENTRY,
	SETTINGS_TRIM_REV_LEVEL, ">trim rev", 0, 1,
	SETTINGS_TRIM_ENTRY, MENU_NONE,
	MENU_NONE);

	menu_itemInit(&settingsMenu, SETTINGS_FUEL_ENTRY, SETTINGS_FUEL_LEVEL,
			">fuel", 0, 4095, MENU_NONE,
			SETTINGS_FUEL_MAX_ENTRY,
			SETTINGS_FUEL_REV_ENTRY);

	menu_itemInit(&settingsMenu, SETTINGS_FUEL_MAX_ENTRY,
	SETTINGS_FUEL_MAX_LEVEL, ">fuel max", 0, 4095, SETTINGS_FUEL_ENTRY,
	MENU_NONE,
	MENU_NONE);

	menu_itemInit(&settingsMenu, SETTINGS_FUEL_MIN_ENTRY,
	SETTINGS_FUEL_MIN_LEVEL, ">fuel min", 0, 4095, SETTINGS_FUEL_ENTRY,
	MENU_NONE,
	MENU_NONE);

	menu_itemInit(&settingsMenu, SETTINGS_FUEL_REV_ENTRY,
	SETTINGS_FUEL_REV_LEVEL, ">fuel rev", 0, 1,
	SETTINGS_FUEL_ENTRY, MENU_NONE,
	MENU_NONE);

	menu_itemInit(&settingsMenu, SETTINGS_VOLTAGE_OFFSET_ENTRY,
	SETTINGS_VOLTAGE_OFFSET_LEVEL, ">voltage offset", -1500, 1500,
	MENU_NONE,
	MENU_NONE,
	MENU_NONE);

	menu_itemInit(&settingsMenu, SETTINGS_IGN_TYPE_ENTRY,
	SETTINGS_IGN_TYPE_LEVEL, ">ign type", 1, 3, MENU_NONE,
	MENU_NONE,
	MENU_NONE);

	menu_itemInit(&settingsMenu, SETTINGS_SPEEDO_UNIT_ENTRY,
	SETTINGS_SPEEDO_UNIT_LEVEL, ">speedo unit", 1, 2, MENU_NONE,
	MENU_NONE,
	MENU_NONE);

//										EEPROM CLEAR

	uint8_t buttons = buttons_read();
	if (buttons == BUTTONS_ALL) {
		LCD_Cls();
		LCD_Locate(0, 0);
		LCD_String("Clearing set");

		uint8_t data[200];
		memset(data, 0xff, 200);
		eeprom_write(&eeprom, 0x04, data, 200);
		HAL_Delay(1000);

	} else if (buttons == BUTTONS_LR) {
		LCD_Cls();
		LCD_Locate(0, 0);
		LCD_String("Clearing mth");

		uint8_t data[4];
		memset(data, 0xff, 4);
		eeprom_write(&eeprom, EEPROM_ADDR_MOTOSECONDS, data, 4);
		HAL_Delay(1000);
	}

//										EEPROM READ
	readConfigFromEEPROM();

//										MOTOHOURS
	motohours_init(&mth, &hcomp1, &htim16, &eeprom, 0x00);

//										TACHO

	uint8_t ignitionType = menu_itemReadValue(&settingsMenu,
	SETTINGS_IGN_TYPE_ENTRY,
	SETTINGS_IGN_TYPE_LEVEL);

	if (ignitionType == 1) {
		tachometer_start(&tacho, &htim2, IGNITION_GPIO_Port, IGNITION_Pin, 500,
		TACHOMETER_MODE_05IGN_PER_REV);
	} else if (ignitionType == 2) {
		tachometer_start(&tacho, &htim2, IGNITION_GPIO_Port, IGNITION_Pin, 500,
		TACHOMETER_MODE_1IGN_PER_REV);
	} else if (ignitionType == 3) {
		tachometer_start(&tacho, &htim2, IGNITION_GPIO_Port, IGNITION_Pin, 500,
		TACHOMETER_MODE_2IGN_PER_REV);
	}

//										ADC
	adconv_init(&adConv, &hadc1);
	adconv_configChannel(&adConv, ADCONV_CH_TEMP, 1);
	adconv_configChannel(&adConv, ADCONV_CH_VOLTAGE,
	CONFIG_ADC_MULTIPLIER_VOLTAGE);
	adconv_configChannel(&adConv, ADCONV_CH_FUEL, 1);
	adconv_configChannel(&adConv, ADCONV_CH_TRIM, 1);

//										INTERFACE
	interface_init(&interface);

	//										SCHEDULER

	if (xTaskCreate(displayTask, ">DISPLAY", 200, NULL, PRIORITY_DISPLAY,
			&task_display) == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY)
		Error_Handler();
	if (xTaskCreate(mthDumpTask, ">MTH_DUMP", 200, NULL, PRIORITY_MTH_DUMP,
			&task_mthDump) == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY)
		Error_Handler();
	if (xTaskCreate(buttonsTask, ">BUTTONS", 200, NULL, PRIORITY_BUTTONS,
			&task_buttonRead) == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY)
		Error_Handler();
	if (xTaskCreate(brightnessTask, ">BRIGHTNESS", 200, NULL,
	PRIORITY_BRIGHTNESS,
			&task_brightness) == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY)
		Error_Handler();
	if (xTaskCreate(lowFuelAlertDisplayReInitTask, ">LOW FUEL ALERT", 200, NULL,
	PRIORITY_LOW_FUEL,
			&task_lowFuelAlert) == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY)
		Error_Handler();

	vTaskStartScheduler();

	while (1) {

	}
}

//										TASK DEFINES

void displayTask(void *pvPremeters) {

	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(PERDIOD_DISPLAY);
	xLastWakeTime = xTaskGetTickCount();

	while (1) {

		vTaskDelayUntil(&xLastWakeTime, xPeriod);
		HAL_GPIO_WritePin(LED_STAT1_GPIO_Port, LED_STAT1_Pin, GPIO_PIN_SET);
		interface_display(&interface);
		HAL_GPIO_WritePin(LED_STAT1_GPIO_Port, LED_STAT1_Pin, GPIO_PIN_RESET);
	}

}

void mthDumpTask(void *pvPremeters) {

	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(PERIOD_MTH_DUMP);
	xLastWakeTime = xTaskGetTickCount();

	while (1) {
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
		motohours_dump(&mth);
	}

}

void brightnessTask(void *pvPremeters) {

	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(PERIOD_BRIGHTNESS);
	xLastWakeTime = xTaskGetTickCount();

	while (1) {
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
		pwm_setDuty(&brightnessPWM,
				menu_itemReadValue(&settingsMenu, SETTINGS_BRIGHTNESS_ENTRY,
				SETTINGS_BRIGHTNESS_LEVEL));
	}

}

void buttonsTask(void *pvPremeters) {

	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(PERIOD_BUTTONS);
	xLastWakeTime = xTaskGetTickCount();

	while (1) {
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
		uint8_t buttonState = buttons_read();
		//										NORMAL OPERATION
		if (menu_readActiveFlag(&settingsMenu) == MENU_ACTIVE_FLAG_DIS) {
			if (buttonState == BUTTONS_L) {
				interface_switchScreen(&interface, INTERFACE_SCREEN_0);
			} else if (buttonState == BUTTONS_M) {
				interface_switchScreen(&interface, INTERFACE_SCREEN_1);
			} else if (buttonState == BUTTONS_R) {
				interface_switchScreen(&interface, INTERFACE_SCREEN_2);
			} else if (buttonState == BUTTONS_ALL) {
				menu_turnOn(&settingsMenu);
			}

			//										SETTINGS
		} else {
			if (buttonState == BUTTONS_L) {
				menu_switch(&settingsMenu, MENU_SWITCH_UP);
			} else if (buttonState == BUTTONS_R) {
				menu_switch(&settingsMenu, MENU_SWITCH_DOWN);
			} else if (buttonState == BUTTONS_LR) {

				//			menu_turnOff(&settingsMenu); //turn off menu
				writeConfigToEEPROM(); //write config to eeprom
				menu_switch(&settingsMenu, MENU_SWITCH_EXIT);

			} else if (buttonState == BUTTONS_M) {
				//			if (menu_readCurrentEditFlag(&settingsMenu) == MENU_CURRENT_EDIT_DIS)
				menu_switch(&settingsMenu, MENU_SWITCH_ENTER);
			}
		}

		HAL_Delay(80);
	}

}

void lowFuelAlertDisplayReInitTask(void *pvPremeters) {

	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(PERIOD_LOW_FUEL);
	xLastWakeTime = xTaskGetTickCount();

	while (1) {
		vTaskDelayUntil(&xLastWakeTime, xPeriod);



		uint16_t adcReading = adconv_getValue(&adConv, ADCONV_CH_FUEL);

		uint8_t reversedFlag = menu_itemReadValue(&settingsMenu,
		SETTINGS_FUEL_REV_ENTRY, SETTINGS_FUEL_REV_LEVEL);

		int16_t maxLevel = menu_itemReadValue(&settingsMenu,
		SETTINGS_FUEL_MAX_ENTRY, SETTINGS_FUEL_MAX_LEVEL);

		int16_t minLevel = menu_itemReadValue(&settingsMenu,
		SETTINGS_FUEL_MIN_ENTRY, SETTINGS_FUEL_MIN_LEVEL);

		int16_t difference = maxLevel - minLevel;

		int16_t fuelPercent = ((adcReading - minLevel) * 100) / difference;

		if (reversedFlag)
			fuelPercent = 100 - fuelPercent;

		if (fuelPercent > 100)
			fuelPercent = 100;
		if (fuelPercent < 0)
			fuelPercent = 0;

		////////////////////////////////////

		//convert to 0-6
		uint8_t fuel = fuelPercent / 15;

		if (fuel <= 0) {
			blinkpin_pattern(&buzzer, 0xF0F0F0F0, 3, BLINKPIN_REPEAT_OFF);
		}

		LCD_Init();
	}
}

//										OTHER FUNCTIONS
void readConfigFromEEPROM(void) {

	menu_itemSetValue(&settingsMenu, SETTINGS_BRIGHTNESS_ENTRY,
	SETTINGS_BRIGHTNESS_LEVEL,
			eeprom_readByte(&eeprom, EEPROM_ADDR_BRIGHTNESS));

	menu_itemSetValue(&settingsMenu, SETTINGS_CLOCK_OFFSET_ENTRY,
	SETTINGS_CLOCK_OFFSET_LEVEL,
			eeprom_readByte(&eeprom, EEPROM_ADDR_CLOCK_OFFSET));

	menu_itemSetValue(&settingsMenu, SETTINGS_FUEL_REV_ENTRY,
	SETTINGS_FUEL_REV_LEVEL,
			eeprom_readByte(&eeprom, EEPROM_ADDR_FUEL_REVERSED_FLAG));

	menu_itemSetValue(&settingsMenu, SETTINGS_TRIM_REV_ENTRY,
	SETTINGS_TRIM_REV_LEVEL,
			eeprom_readByte(&eeprom, EEPROM_ADDR_TRIM_REVERSED_FLAG));

	menu_itemSetValue(&settingsMenu, SETTINGS_FUEL_MAX_ENTRY,
	SETTINGS_FUEL_MAX_LEVEL, eeprom_readWord(&eeprom, EEPROM_ADDR_FUEL_MAX));

	menu_itemSetValue(&settingsMenu, SETTINGS_FUEL_MIN_ENTRY,
	SETTINGS_FUEL_MIN_LEVEL, eeprom_readWord(&eeprom, EEPROM_ADDR_FUEL_MIN));

	menu_itemSetValue(&settingsMenu, SETTINGS_TRIM_MAX_ENTRY,
	SETTINGS_TRIM_MAX_LEVEL, eeprom_readWord(&eeprom, EEPROM_ADDR_TRIM_MAX));

	menu_itemSetValue(&settingsMenu, SETTINGS_TRIM_MIN_ENTRY,
	SETTINGS_TRIM_MIN_LEVEL, eeprom_readWord(&eeprom, EEPROM_ADDR_TRIM_MIN));

	menu_itemSetValue(&settingsMenu, SETTINGS_VOLTAGE_OFFSET_ENTRY,
	SETTINGS_VOLTAGE_OFFSET_LEVEL,
			eeprom_readWord(&eeprom, EEPROM_ADDR_VOLTAGE_OFFSET));

	menu_itemSetValue(&settingsMenu, SETTINGS_TEMP_OFFSET_ENTRY,
	SETTINGS_TEMP_OFFSET_LEVEL,
			eeprom_readWord(&eeprom, EEPROM_ADDR_TEMPERATURE_OFFSET));

	menu_itemSetValue(&settingsMenu, SETTINGS_IGN_TYPE_ENTRY,
	SETTINGS_IGN_TYPE_LEVEL, eeprom_readByte(&eeprom, EEPROM_ADDR_IGN_TYPE));

	menu_itemSetValue(&settingsMenu, SETTINGS_SPEEDO_UNIT_ENTRY,
	SETTINGS_SPEEDO_UNIT_LEVEL,
			eeprom_readByte(&eeprom, EEPROM_ADDR_SPEED_UNIT));

	//write default values if eeprom is empty
	if (menu_itemReadValue(&settingsMenu, SETTINGS_BRIGHTNESS_ENTRY,
	SETTINGS_BRIGHTNESS_LEVEL) == 0xFF)
		menu_itemSetValue(&settingsMenu, SETTINGS_BRIGHTNESS_ENTRY,
		SETTINGS_BRIGHTNESS_LEVEL, SETTINGS_DEFAULT_BRIGHTNESS);

	if (menu_itemReadValue(&settingsMenu, SETTINGS_CLOCK_OFFSET_ENTRY,
	SETTINGS_CLOCK_OFFSET_LEVEL) == 0xFF)
		menu_itemSetValue(&settingsMenu, SETTINGS_CLOCK_OFFSET_ENTRY,
		SETTINGS_CLOCK_OFFSET_LEVEL, SETTINGS_DEFAULT_CLOCK_OFFSET);

	if (menu_itemReadValue(&settingsMenu, SETTINGS_FUEL_REV_ENTRY,
	SETTINGS_FUEL_REV_LEVEL) == 0xFF)
		menu_itemSetValue(&settingsMenu, SETTINGS_FUEL_REV_ENTRY,
		SETTINGS_FUEL_REV_LEVEL, SETTINGS_DEFAULT_FUEL_REV);

	if (menu_itemReadValue(&settingsMenu, SETTINGS_TRIM_REV_ENTRY,
	SETTINGS_TRIM_REV_LEVEL) == 0xFF)
		menu_itemSetValue(&settingsMenu, SETTINGS_TRIM_REV_ENTRY,
		SETTINGS_TRIM_REV_LEVEL, SETTINGS_DEFAULT_TRIM_REV);

	if (menu_itemReadValue(&settingsMenu, SETTINGS_FUEL_MAX_ENTRY,
	SETTINGS_FUEL_MAX_LEVEL) == 0xFFFFFFFF)
		menu_itemSetValue(&settingsMenu, SETTINGS_FUEL_MAX_ENTRY,
		SETTINGS_FUEL_MAX_LEVEL, SETTINGS_DEFAULT_FUEL_MAX);

	if (menu_itemReadValue(&settingsMenu, SETTINGS_FUEL_MIN_ENTRY,
	SETTINGS_FUEL_MIN_LEVEL) == 0xFFFFFFFF)
		menu_itemSetValue(&settingsMenu, SETTINGS_FUEL_MIN_ENTRY,
		SETTINGS_FUEL_MIN_LEVEL, SETTINGS_DEFAULT_FUEL_MIN);

	if (menu_itemReadValue(&settingsMenu, SETTINGS_TRIM_MAX_ENTRY,
	SETTINGS_TRIM_MAX_LEVEL) == 0xFFFFFFFF)
		menu_itemSetValue(&settingsMenu, SETTINGS_TRIM_MAX_ENTRY,
		SETTINGS_TRIM_MAX_LEVEL, SETTINGS_DEFAULT_TRIM_MAX);

	if (menu_itemReadValue(&settingsMenu, SETTINGS_TRIM_MIN_ENTRY,
	SETTINGS_TRIM_MIN_LEVEL) == 0xFFFFFFFF)
		menu_itemSetValue(&settingsMenu, SETTINGS_TRIM_MIN_ENTRY,
		SETTINGS_TRIM_MIN_LEVEL, SETTINGS_DEFAULT_TRIM_MIN);

	if (menu_itemReadValue(&settingsMenu, SETTINGS_VOLTAGE_OFFSET_ENTRY,
	SETTINGS_VOLTAGE_OFFSET_LEVEL) == 0xFFFFFFFF)
		menu_itemSetValue(&settingsMenu, SETTINGS_VOLTAGE_OFFSET_ENTRY,
		SETTINGS_VOLTAGE_OFFSET_LEVEL, SETTINGS_DEFAULT_VOLTAGE_OFFSET);

	if (menu_itemReadValue(&settingsMenu, SETTINGS_TEMP_OFFSET_ENTRY,
	SETTINGS_TEMP_OFFSET_LEVEL) == 0xFFFFFFFF)
		menu_itemSetValue(&settingsMenu, SETTINGS_TEMP_OFFSET_ENTRY,
		SETTINGS_TEMP_OFFSET_LEVEL, SETTINGS_DEFAULT_TEMP_OFFSET);

	if (menu_itemReadValue(&settingsMenu, SETTINGS_IGN_TYPE_ENTRY,
	SETTINGS_IGN_TYPE_LEVEL) == 0xFF)
		menu_itemSetValue(&settingsMenu, SETTINGS_IGN_TYPE_ENTRY,
		SETTINGS_IGN_TYPE_LEVEL, SETTINGS_DEFAULT_IGN_TYPE);

	if (menu_itemReadValue(&settingsMenu, SETTINGS_SPEEDO_UNIT_ENTRY,
	SETTINGS_SPEEDO_UNIT_LEVEL) == 0xFF)
		menu_itemSetValue(&settingsMenu, SETTINGS_SPEEDO_UNIT_ENTRY,
		SETTINGS_SPEEDO_UNIT_LEVEL, SETTINGS_DEFAULT_SPEEDO_UNIT);

}

void writeConfigToEEPROM(void) {

	eeprom_writeByte(&eeprom, EEPROM_ADDR_BRIGHTNESS,
			menu_itemReadValue(&settingsMenu, SETTINGS_BRIGHTNESS_ENTRY,
			SETTINGS_BRIGHTNESS_LEVEL));

	eeprom_writeByte(&eeprom, EEPROM_ADDR_CLOCK_OFFSET,
			menu_itemReadValue(&settingsMenu, SETTINGS_CLOCK_OFFSET_ENTRY,
			SETTINGS_CLOCK_OFFSET_LEVEL));

	eeprom_writeByte(&eeprom, EEPROM_ADDR_FUEL_REVERSED_FLAG,
			menu_itemReadValue(&settingsMenu, SETTINGS_FUEL_REV_ENTRY,
			SETTINGS_FUEL_REV_LEVEL));

	eeprom_writeByte(&eeprom, EEPROM_ADDR_TRIM_REVERSED_FLAG,
			menu_itemReadValue(&settingsMenu, SETTINGS_TRIM_REV_ENTRY,
			SETTINGS_TRIM_REV_LEVEL));

	eeprom_writeWord(&eeprom, EEPROM_ADDR_FUEL_MAX,
			menu_itemReadValue(&settingsMenu, SETTINGS_FUEL_MAX_ENTRY,
			SETTINGS_FUEL_MAX_LEVEL));

	eeprom_writeWord(&eeprom, EEPROM_ADDR_FUEL_MIN,
			menu_itemReadValue(&settingsMenu, SETTINGS_FUEL_MIN_ENTRY,
			SETTINGS_FUEL_MIN_LEVEL));

	eeprom_writeWord(&eeprom, EEPROM_ADDR_TRIM_MAX,
			menu_itemReadValue(&settingsMenu, SETTINGS_TRIM_MAX_ENTRY,
			SETTINGS_TRIM_MAX_LEVEL));

	eeprom_writeWord(&eeprom, EEPROM_ADDR_TRIM_MIN,
			menu_itemReadValue(&settingsMenu, SETTINGS_TRIM_MIN_ENTRY,
			SETTINGS_TRIM_MIN_LEVEL));

	eeprom_writeWord(&eeprom, EEPROM_ADDR_VOLTAGE_OFFSET,
			menu_itemReadValue(&settingsMenu, SETTINGS_VOLTAGE_OFFSET_ENTRY,
			SETTINGS_VOLTAGE_OFFSET_LEVEL));

	eeprom_writeWord(&eeprom, EEPROM_ADDR_TEMPERATURE_OFFSET,
			menu_itemReadValue(&settingsMenu, SETTINGS_TEMP_OFFSET_ENTRY,
			SETTINGS_TEMP_OFFSET_LEVEL));

	eeprom_writeByte(&eeprom, EEPROM_ADDR_IGN_TYPE,
			menu_itemReadValue(&settingsMenu, SETTINGS_IGN_TYPE_ENTRY,
			SETTINGS_IGN_TYPE_LEVEL));

	eeprom_writeByte(&eeprom, EEPROM_ADDR_SPEED_UNIT,
			menu_itemReadValue(&settingsMenu, SETTINGS_SPEEDO_UNIT_ENTRY,
					SETTINGS_SPEEDO_UNIT_LEVEL));
}


//void playSplash(void){
//
//	for(float speed = 0; speed<=99; speed+=0.5){
//
//		char speedArray[6] = "\0\0\0\0\0\0";
//
//		sprintf(speedArray, 6, "%.2f", speed);
//
//
//		LCD_Locate(0, 0);
//		LCD_String(speedArray);
//		HAL_Delay(1);
//	}
//
//	for(float speed = 99; speed>0; speed-=0.5){
//
//		char speedArray[6] = "\0\0\0\0\0\0";
//
//		sprintf(speedArray, 6, "%.2f", speed);
//
//
//		LCD_Locate(0, 0);
//		LCD_String(speedArray);
//		HAL_Delay(1);
//
//	}
//
//
//}
