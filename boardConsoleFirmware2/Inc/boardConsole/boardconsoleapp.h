/*
 * boardconsoleapp.h
 *
 *  Created on: Oct 12, 2023
 *      Author: bartosz
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

#ifndef BOARDCONSOLEAPP_H_
#define BOARDCONSOLEAPP_H_


#include "FreeRTOS.h"
#include "task.h"
#include "boardConsole/interface.h"
#include "boardConsole/buttons.h"
#include "libs/hd44780.h"
#include <string.h>
#include "libs/nmea0183.h"
#include "libs/scheduler.h"
#include "libs/tachometer.h"
#include "interface.h"
#include "libs/adconv.h"
#include "libs/eeprom.h"
#include "libs/motohours.h"
#include "libs/menu.h"
#include "gpio.h"
#include "usart.h"
#include "tim.h"
#include "i2c.h"
#include "comp.h"
#include "adc.h"
#include "libs/pwm.h"
#include "libs/blinkpin.h"

#define CONFIG_ADC_MULTIPLIER_VOLTAGE 8.8645


//periods
#define PERDIOD_DISPLAY 100
#define PERIOD_MTH_DUMP 1
#define PERIOD_BUTTONS 50
#define PERIOD_BRIGHTNESS 100
#define PERIOD_LOW_FUEL 30000

//priorities
#define PRIORITY_DISPLAY 1
#define PRIORITY_MTH_DUMP 1
#define PRIORITY_BUTTONS 1
#define PRIORITY_BRIGHTNESS 1
#define PRIORITY_LOW_FUEL 1



//eeprom addresses
#define EEPROM_ADDR_MOTOSECONDS 0x00
#define EEPROM_ADDR_FUEL_REVERSED_FLAG 0x04
#define EEPROM_ADDR_TRIM_REVERSED_FLAG 0x05
#define EEPROM_ADDR_FUEL_MAX 0x06
#define EEPROM_ADDR_FUEL_MIN 0x0a
#define EEPROM_ADDR_TRIM_MAX 0x0e
#define EEPROM_ADDR_TRIM_MIN 0x12
#define EEPROM_ADDR_BRIGHTNESS 0x16
#define EEPROM_ADDR_CLOCK_OFFSET 0x17
#define EEPROM_ADDR_TEMPERATURE_OFFSET 0x18
#define EEPROM_ADDR_VOLTAGE_OFFSET 0x22
#define EEPROM_ADDR_IGN_TYPE 0x26



#define SETTINGS_BRIGHTNESS_ENTRY 0
#define SETTINGS_BRIGHTNESS_LEVEL 0

#define SETTINGS_CLOCK_OFFSET_ENTRY 1
#define SETTINGS_CLOCK_OFFSET_LEVEL 0

#define SETTINGS_TEMP_OFFSET_ENTRY 2
#define SETTINGS_TEMP_OFFSET_LEVEL 0

#define SETTINGS_TRIM_ENTRY 3
#define SETTINGS_TRIM_LEVEL 0

	#define SETTINGS_TRIM_MAX_ENTRY 0
	#define SETTINGS_TRIM_MAX_LEVEL 1

	#define SETTINGS_TRIM_MIN_ENTRY 1
	#define SETTINGS_TRIM_MIN_LEVEL 1

	#define SETTINGS_TRIM_REV_ENTRY 2
	#define SETTINGS_TRIM_REV_LEVEL 1

#define SETTINGS_FUEL_ENTRY 4
#define SETTINGS_FUEL_LEVEL 0

	#define SETTINGS_FUEL_MAX_ENTRY 3
	#define SETTINGS_FUEL_MAX_LEVEL 1

	#define SETTINGS_FUEL_MIN_ENTRY 4
	#define SETTINGS_FUEL_MIN_LEVEL 1

	#define SETTINGS_FUEL_REV_ENTRY 5
	#define SETTINGS_FUEL_REV_LEVEL 1


#define SETTINGS_VOLTAGE_OFFSET_ENTRY 5
#define SETTINGS_VOLTAGE_OFFSET_LEVEL 0

#define SETTINGS_IGN_TYPE_ENTRY 6
#define SETTINGS_IGN_TYPE_LEVEL 0

#define SETTINGS_DEFAULT_BRIGHTNESS 50
#define SETTINGS_DEFAULT_CLOCK_OFFSET 2
#define SETTINGS_DEFAULT_FUEL_REV 0
#define SETTINGS_DEFAULT_TRIM_REV 1
#define SETTINGS_DEFAULT_FUEL_MAX 2047
#define SETTINGS_DEFAULT_FUEL_MIN 1023
#define SETTINGS_DEFAULT_TRIM_MAX 1023
#define SETTINGS_DEFAULT_TRIM_MIN 0
#define SETTINGS_DEFAULT_VOLTAGE_OFFSET 850
#define SETTINGS_DEFAULT_TEMP_OFFSET -9
#define SETTINGS_DEFAULT_IGN_TYPE 3




void boardComputer_process(void);

#endif /* BOARDCONSOLEAPP_H_ */
