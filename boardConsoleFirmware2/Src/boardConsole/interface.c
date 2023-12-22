/*
 * interface.c
 *
 *  Created on: Oct 13, 2023
 *      Author: bartosz
 */

/*
  	Board Console Interface
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

#include <boardConsole/interface.h>

extern tachometer_TypeDef tacho;
extern adconv_TypeDef adConv;
extern motohours_TypeDef mth;
extern menu_TypeDef settingsMenu;
//extern boardComputer_TypeDef bc;

//icons
uint8_t distribuer[8] = { 0b00011, 0b11101, 0b10101, 0b11101, 0b11101, 0b11101,
		0b11110, 0b00000, };
uint8_t outboard[8] = { 0b00111, 0b11111, 0b11111, 0b00010, 0b00010, 0b00010,
		0b00110, 0b00000, };
uint8_t trim[8] = { 0b10000, 0b10000, 0b01000, 0b01000, 0b00101, 0b00011,
		0b00111, 0b00000, };
uint8_t fill[8] = { 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111,
		0b11111, 0b00000, };
uint8_t triangle[8] = { 0b10000, 0b11000, 0b11100, 0b11110, 0b11100, 0b11000,
		0b10000, 0b00000, };
uint8_t degree[8] = { 0b00110, 0b01001, 0b01001, 0b00110, 0b00000, 0b00000,
		0b00000, 0b00000, };
uint8_t barrier[8] = { 0b00110, 0b00110, 0b00110, 0b00110, 0b00110, 0b00110,
		0b00110, 0b00000, };
uint8_t arrow[8] = { 0b00100, 0b00100, 0b00100, 0b00100, 0b10101, 0b01110,
		0b00100, 0b00000, };

//struct {
//	//[lines][columns]
//	char framebuffer[2][16];
//	uint8_t actualScreen;
//	int8_t hoursOffset;
//
//} Interface;

static void interface_draw_speedo(interface_TypeDef *interface);
static void interface_draw_tacho_digital(interface_TypeDef *interface);
static void interface_draw_tacho_analog(interface_TypeDef *interface);
static void interface_draw_course(interface_TypeDef *interface);
static void interface_draw_clock(interface_TypeDef *interface);
static void interface_draw_voltage(interface_TypeDef *interface);
static void interface_draw_temperature(interface_TypeDef *interface);
static void interface_draw_fuel(interface_TypeDef *interface);
static void interface_draw_trim(interface_TypeDef *interface);
//static void interface_draw_motoseconds(interface_TypeDef *interface);
static void interface_draw_motohours(interface_TypeDef *interface);

static void interface_draw_settings(interface_TypeDef *interface);

void interface_init(interface_TypeDef *interface) {

	interface->actualScreen = INTERFACE_SCREEN_0;

	LCD_DefChar(0, barrier);
	LCD_DefChar(1, distribuer);
	LCD_DefChar(2, outboard);
	LCD_DefChar(3, trim);
	LCD_DefChar(4, fill);
	LCD_DefChar(5, triangle);
	LCD_DefChar(6, degree);
	LCD_DefChar(7, arrow);

}

void interface_swipeScreen(interface_TypeDef *interface, uint8_t direction) {

	if (direction == INTERFACE_SWITCH_SCREEN_DIR_LEFT
			&& interface->actualScreen > 0) {
		interface->actualScreen--;
	} else if (direction == INTERFACE_SWITCH_SCREEN_DIR_RIGHT
			&& interface->actualScreen < INTERFACE_SCREEN_COUNT) {
		interface->actualScreen++;
	}
}

void interface_switchScreen(interface_TypeDef *interface, uint8_t screen) {

	interface->actualScreen = screen;

}

void interface_display(interface_TypeDef *interface) {

	//clear framebuffer
	memset(interface->framebuffer, ' ', sizeof(interface->framebuffer));

	//if menuu inactive
	if (menu_readActiveFlag(&settingsMenu) == MENU_ACTIVE_FLAG_DIS) {
		if (interface->actualScreen == INTERFACE_SCREEN_0) {

			interface_draw_speedo(interface);
			interface_draw_tacho_digital(interface);
			interface_draw_tacho_analog(interface);
		} else if (interface->actualScreen == INTERFACE_SCREEN_1) {
			interface_draw_course(interface);
			interface_draw_clock(interface);
			interface_draw_fuel(interface);
			//
		} else if (interface->actualScreen == INTERFACE_SCREEN_2) {
			interface_draw_temperature(interface);
			interface_draw_voltage(interface);
			interface_draw_trim(interface);
			interface_draw_motohours(interface);
//			interface_draw_motoseconds(interface);
		}
	} else {
		interface_draw_settings(interface);
	}

//	}else if(interface->actualScreen == INTERFACE_SCREEN_SETTINGS){
//		interface_draw_settings(interface);
//	}

//send do display
	LCD_Locate(0, 0);
	LCD_Data(&interface->framebuffer[0][0], 16);
	LCD_Locate(0, 1);
	LCD_Data(&interface->framebuffer[1][0], 16);

}

static void interface_draw_speedo(interface_TypeDef *interface) {

	char gpsSpeed[6];

	nmea0183_retval(&gps, NMEA0183_FRAME_SPEED,
	NMEA0183_PARAM_SPEED, gpsSpeed, 6);

	if (gpsSpeed[2] == '.') { //speed > 9, e.g. 22.04
		memcpy(
				&interface->framebuffer[INTERFACE_POS_SPEEDO_Y][INTERFACE_POS_SPEEDO_X],
				gpsSpeed, 2);
		memcpy(
				&interface->framebuffer[INTERFACE_POS_SPEEDO_Y][INTERFACE_POS_SPEEDO_X
						+ 3], gpsSpeed + 3, 2);
	} else if (gpsSpeed[1] == '.') { //speed < 10 e.g. 1.05
		memcpy(
				&interface->framebuffer[INTERFACE_POS_SPEEDO_Y][INTERFACE_POS_SPEEDO_X
						+ 1], gpsSpeed, 2);
		memcpy(
				&interface->framebuffer[INTERFACE_POS_SPEEDO_Y][INTERFACE_POS_SPEEDO_X
						+ 3], gpsSpeed + 2, 2);
	}
	memcpy(
			&interface->framebuffer[INTERFACE_POS_SPEEDO_Y][INTERFACE_POS_SPEEDO_X
					+ 5], "kn", 2);
	interface->framebuffer[INTERFACE_POS_SPEEDO_Y][INTERFACE_POS_SPEEDO_X + 2] =
			'.';
}
static void interface_draw_tacho_digital(interface_TypeDef *interface) {
	uint32_t rpm = tachometer_read(&tacho);

	char buffer[9] = "         ";

	if (rpm < 10) {
		itoa(rpm, &buffer[4], 10);
	} else if (rpm < 100) {
		itoa(rpm, &buffer[3], 10);
	} else if (rpm < 1000) {
		itoa(rpm, &buffer[2], 10);
	} else if (rpm < 10000) {
		itoa(rpm, &buffer[1], 10);
	} else {
		itoa(rpm, &buffer[0], 10);
	}

	memcpy(&buffer[5], "rpm", 3);

	memcpy(
			&interface->framebuffer[INTERFACE_POS_TACHO_D_Y][INTERFACE_POS_TACHO_D_X],
			buffer, 8);

}
static void interface_draw_tacho_analog(interface_TypeDef *interface) {
	uint32_t rpm = tachometer_read(&tacho);
	uint8_t thousands = 0;

	thousands = (rpm + 500) / 1000;

	char buffer[14] = "              ";
	buffer[0] = '\0';
	buffer[13] = '\0';

	for (uint8_t i = 0; i < thousands; i++) {
		buffer[i + 1] = INTERFACE_ICON_FILL;
	}

	memcpy(
			&interface->framebuffer[INTERFACE_POS_TACHO_A_Y][INTERFACE_POS_TACHO_A_X],
			buffer, sizeof(buffer));

}
static void interface_draw_course(interface_TypeDef *interface) {
	char azimuth[10] = "          ";
	char compass[16];

	if (nmea0183_retval(&gps, NMEA0183_FRAME_AZIMUTH, NMEA0183_PARAM_AZIMUTH,
			azimuth, 10) != NMEA0183_STAT_OK) {
		memcpy(compass, "----------------", sizeof(compass));

	} else {

		uint8_t course = (uint8_t) atof(azimuth) / 22.5;

		switch (course) {
		case 0:
			memcpy(compass, "---W---^---E---S", sizeof(compass));
			break;
		case 1:
			memcpy(compass, "--W---^---E---S-", sizeof(compass));
			break;
		case 2:
			memcpy(compass, "-W---^---E---S--", sizeof(compass));
			break;
		case 3:
			memcpy(compass, "W---^---E---S---", sizeof(compass));
			break;
		case 4:
			memcpy(compass, "---^---E---S---W", sizeof(compass));
			break;
		case 5:
			memcpy(compass, "--^---E---S---W-", sizeof(compass));
			break;
		case 6:
			memcpy(compass, "-^---E---S---W--", sizeof(compass));
			break;
		case 7:
			memcpy(compass, "^---E---S---W---", sizeof(compass));
			break;
		case 8:
			memcpy(compass, "---E---S---W---^", sizeof(compass));
			break;
		case 9:
			memcpy(compass, "--E---S---W---^-", sizeof(compass));
			break;
		case 10:
			memcpy(compass, "-E---S---W---^--", sizeof(compass));
			break;
		case 11:
			memcpy(compass, "E---S---W---^---", sizeof(compass));
			break;
		case 12:
			memcpy(compass, "---S---W---^---E", sizeof(compass));
			break;
		case 13:
			memcpy(compass, "--S---W---^---E-", sizeof(compass));
			break;
		case 14:
			memcpy(compass, "-S---W---^---E--", sizeof(compass));
			break;
		case 15:
			memcpy(compass, "S---W---^---E---", sizeof(compass));
			break;
		default:
			memcpy(compass, "----------------", sizeof(compass));
			break;
		}

	}
	interface->framebuffer[0][7] = '\7';

	memcpy(
			&interface->framebuffer[INTERFACE_POS_COURSE_Y][INTERFACE_POS_COURSE_X],
			compass, sizeof(compass));
}
static void interface_draw_clock(interface_TypeDef *interface) {

	char nmeaClock[11];
	char hours[3]; //3rd element for \0 string end
	char minutes[2];
	int8_t hoursInt;
	char clock[5] = { "  :  " };

	if (nmea0183_retval(&gps, NMEA0183_FRAME_TIME, NMEA0183_PARAM_TIME,
			nmeaClock, 11) != NMEA0183_STAT_OK) {

	} else {

		memcpy(hours, &nmeaClock[0], 2);
		hours[2] = '\0';

		hoursInt = atoi(hours); //convert to int

		hoursInt = hoursInt
				+ menu_itemReadValue(&settingsMenu, SETTINGS_CLOCK_OFFSET_ENTRY,
				SETTINGS_CLOCK_OFFSET_LEVEL); // apply offset from Greenwich

		if (hoursInt > 23) {
			hoursInt = hoursInt - 24; //correct e.g. 25 to 2
		} else if (hoursInt < 0) {
			hoursInt = hoursInt + 24; //correct e.g -1 to 23
		}

		itoa(hoursInt, hours, 10);
		memcpy(minutes, &nmeaClock[2], 2);

		if (hoursInt < 10) {
			clock[0] = '0';
			memcpy(&clock[1], hours, 1);
		} else {
			memcpy(&clock[0], hours, 2);
		}

		memcpy(&clock[3], minutes, 2);
	}

	memcpy(
			&interface->framebuffer[INTERFACE_POS_CLOCK_Y][INTERFACE_POS_CLOCK_X],
			clock, 5);

}
static void interface_draw_voltage(interface_TypeDef *interface) {

	uint32_t milivoltsInt = adconv_getValue(&adConv, ADCONV_CH_VOLTAGE)
			+ menu_itemReadValue(&settingsMenu, SETTINGS_VOLTAGE_OFFSET_ENTRY,
			SETTINGS_VOLTAGE_OFFSET_LEVEL);
	uint32_t voltsInt = milivoltsInt / 1000;
	uint32_t tenthsInt = (milivoltsInt / 100) - (voltsInt * 10);

	char voltsArr[3];
	itoa(voltsInt, voltsArr, 10);

	char tenthsArr[2];
	itoa(tenthsInt, tenthsArr, 10);

	char value[5] = "  . V";

	if (voltsInt > 9) {
		memcpy(&value[0], voltsArr, 2);
	} else {
		memcpy(&value[1], voltsArr, 1);
	}
	memcpy(&value[3], tenthsArr, 1);

	memcpy(
			&interface->framebuffer[INTERFACE_POS_VOLTAGE_Y][INTERFACE_POS_VOLTAGE_X],
			value, 5);
}
static void interface_draw_temperature(interface_TypeDef *interface) {

//	float tempAdc = adconv_getValue(&adConv, ADCONV_CH_TEMP);
//	float temperature = (((float)TEMPSENSOR_CAL2_TEMP - (float)TEMPSENSOR_CAL1_TEMP)
//			/ (((float)*TEMPSENSOR_CAL2_ADDR) - ((float)*TEMPSENSOR_CAL1_ADDR)))
//			* (tempAdc - ((float)*TEMPSENSOR_CAL1_ADDR)) + (float)TEMPSENSOR_CAL1_TEMP;

	uint32_t tempADCLL = adconv_getValue(&adConv, ADCONV_CH_TEMP);

	int32_t temperatureLL = __LL_ADC_CALC_TEMPERATURE(3300, tempADCLL, LL_ADC_RESOLUTION_12B);

	uint32_t temperatureInt = temperatureLL
			+ menu_itemReadValue(&settingsMenu, SETTINGS_TEMP_OFFSET_ENTRY,
			SETTINGS_TEMP_OFFSET_LEVEL);

	char buffer[4] = "  \6C";

	if (temperatureInt < 10) {
		itoa(temperatureInt, &buffer[1], 10);
	} else {
		itoa(temperatureInt, &buffer[0], 10);
	}

	buffer[2] = '\6';
	buffer[3] = 'C';

	memcpy(&interface->framebuffer[INTERFACE_POS_TEMP_Y][INTERFACE_POS_TEMP_X],
			buffer, 4);

//	TEMPSENSOR_CAL1_ADDR;
//	TEMPSENSOR_CAL2_ADDR;
//	TEMPSENSOR_CAL1_TEMP;
//	TEMPSENSOR_CAL2_TEMP;

}
static void interface_draw_trim(interface_TypeDef *interface) {

	uint16_t adcReading = adconv_getValue(&adConv, ADCONV_CH_TRIM);

	uint8_t reversedFlag = menu_itemReadValue(&settingsMenu,
	SETTINGS_TRIM_REV_ENTRY, SETTINGS_TRIM_REV_LEVEL);

	int16_t maxLevel = menu_itemReadValue(&settingsMenu,
	SETTINGS_TRIM_MAX_ENTRY, SETTINGS_TRIM_MAX_LEVEL);

	int16_t minLevel = menu_itemReadValue(&settingsMenu,
	SETTINGS_TRIM_MIN_ENTRY, SETTINGS_TRIM_MIN_LEVEL);

	int16_t difference = maxLevel - minLevel;

	int16_t trim = ((adcReading - minLevel) * 100) / difference;

	if (reversedFlag)
		trim = 100 - trim;

	if (trim > 100)
		trim = 100;

	if (trim < 0)
		trim = 0;

	char buffer[6] = "\3\2    ";

	if (trim > 99) {
		itoa(trim, &buffer[2], 10);
	} else if (trim > 9) {
		itoa(trim, &buffer[3], 10);
	} else {
		itoa(trim, &buffer[4], 10);
	}

	buffer[5] = ' ';

	memcpy(&interface->framebuffer[INTERFACE_POS_TRIM_Y][INTERFACE_POS_TRIM_X],
			buffer, 5);

}
static void interface_draw_fuel(interface_TypeDef *interface) {
//	int32_t fuelAdc = adconv_getValue(&adConv, ADCONV_CH_FUEL)
//			+ menu_itemReadValue(&settingsMenu, SETTINGS_FUEL_OFFSET_ENTRY,
//			SETTINGS_FUEL_OFFSET_LEVEL);
//
//	if (fuelAdc < 0)
//		fuelAdc = 0;

	////////////////////////////////////////////////
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

	char buffer[7] = "\1      ";

	if (fuel > 0)
		buffer[1] = '\4';
	if (fuel > 1)
		buffer[2] = '\4';
	if (fuel > 2)
		buffer[3] = '\4';
	if (fuel > 3)
		buffer[4] = '\4';
	if (fuel > 4)
		buffer[5] = '\4';
	if (fuel > 5)
		buffer[6] = '\4';

	memcpy(&interface->framebuffer[INTERFACE_POS_FUEL_Y][INTERFACE_POS_FUEL_X],
			buffer, 7);
}
//static void interface_draw_motoseconds(interface_TypeDef *interface) {
//
//	char buffer[8] = "     mts";
//
//	uint32_t motoseconds = motohours_readMts(&mth);
//
//	if (motoseconds > 9999) {
//		itoa(motoseconds, &buffer[0], 10);
//	} else if (motoseconds > 999) {
//		itoa(motoseconds, &buffer[1], 10);
//	} else if (motoseconds > 99) {
//		itoa(motoseconds, &buffer[2], 10);
//	} else if (motoseconds > 9) {
//		itoa(motoseconds, &buffer[3], 10);
//	} else {
//		itoa(motoseconds, &buffer[4], 10);
//	}
//
//	buffer[5] = 'm';
//
//	memcpy(
//			&interface->framebuffer[INTERFACE_POS_MOTOHOURS_Y][INTERFACE_POS_MOTOHOURS_X],
//			buffer, sizeof(buffer));
//
//}

static void interface_draw_settings(interface_TypeDef *interface) {

	char name[16] = "                ";
	char value[16] = "                ";

//	memcpy(name, settingsMenu.items[settingsMenu.currentEntry][settingsMenu.currentLevel].name, 16);
	menu_readCurrentName(&settingsMenu, name, 16);

	if (menu_readIfHaveSubmenu(&settingsMenu) == MENU_HAVENT_SUMBENU) {
		itoa(menu_readCurrentValue(&settingsMenu), &value[4], 10);
	} else {
		memcpy(value, "          ENTER>", 16);
	}

	memcpy(
			&interface->framebuffer[INTERFACE_POS_SETTINGS_NAME_Y][INTERFACE_POS_SETTINGS_NAME_X],
			name, 16);

	for (uint8_t i = 0; i < 16; i++) {
		if (value[i] == '\0')
			value[i] = ' ';
	}

	if (menu_readCurrentEditFlag(&settingsMenu) == MENU_CURRENT_EDIT_EN) {
		value[0] = '<';
		value[15] = '>';
	}

//	} else {
//		memcpy(value, "ENTER", 15);
//	}

	memcpy(
			&interface->framebuffer[INTERFACE_POS_SETTINGS_VALUE_Y][INTERFACE_POS_SETTINGS_VALUE_X],
			value, 16);

}

static void interface_draw_motohours(interface_TypeDef *interface) {

	char buffer[11] = "     :  mth";

	uint32_t motohours = motohours_read(&mth);
	uint32_t motominutes = motohours_readMts(&mth) / 60;

	if (motominutes > 59) {
		motominutes = motominutes - motohours * 60;
	}

	if (motohours > 9999) {
		itoa(motohours, &buffer[0], 10);
	} else if (motohours > 999) {
		itoa(motohours, &buffer[1], 10);
	} else if (motohours > 99) {
		itoa(motohours, &buffer[2], 10);
	} else if (motohours > 9) {
		itoa(motohours, &buffer[3], 10);
	} else {
		itoa(motohours, &buffer[4], 10);
	}

	if (motominutes > 9) {
		itoa(motominutes, &buffer[6], 10);
	} else {
		itoa(motominutes, &buffer[7], 10);
		buffer[6] = '0';
	}
	buffer[5] = ':';
	buffer[8] = 'm';
	buffer[9] = 't';
	buffer[10] = 'h';

	memcpy(
			&interface->framebuffer[INTERFACE_POS_MOTOHOURS_Y][INTERFACE_POS_MOTOHOURS_X],
			buffer, sizeof(buffer));

}

