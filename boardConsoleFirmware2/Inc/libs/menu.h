/*
 * menu.h
 *
 *  Created on: 14 kwi 2022
 *      Author: bartosz
 */

/*
  	Multilevel menu library
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

#ifndef INC_MENU_H_
#define INC_MEN

#include <stdint.h>
#include <string.h>

//USER CONFIGURATION

//item value size
#define MENU_ITEM_NAME_SIZE 16

//matrix size
#define MENU_LEVELS 2
#define MENU_ENTRIES_MAX 15

//#define MENU_ENTRIES_MAIN 10 //entries in main menu - becouse main have no parent and dont know what is last element.

//END OF USER COFIGURATION

#define MENU_NONE 255

#define MENU_SWITCH_ENTER 0
#define MENU_SWITCH_EXIT 1
#define MENU_SWITCH_UP 2
#define MENU_SWITCH_DOWN 3

#define MENU_CHANGE_DECREASE 0
#define MENU_CHANGE_INCREASE 1

#define MENU_ERROR 0
#define MENU_OK 1

#define MENU_CURRENT_EDIT_EN 1
#define MENU_CURRENT_EDIT_DIS 0

#define MENU_ACTIVE_FLAG_EN 1
#define MENU_ACTIVE_FLAG_DIS 0

#define MENU_HAVE_SUBMENU 1
#define MENU_HAVENT_SUMBENU 0

//each menu position
typedef struct {

	char name[MENU_ITEM_NAME_SIZE]; //value for display
	int32_t value;

	int32_t maxValue;
	int32_t minValue;

	uint8_t entry;
	uint8_t level;
	uint8_t parent; //parent entry number- MENU_NONE if no parent

	uint8_t childFirst; //first child entry number
	uint8_t childLast; //last child entry number



} menuItem_TypeDef;

//whole menu
typedef struct {

	menuItem_TypeDef items[MENU_ENTRIES_MAX][MENU_LEVELS];
	uint8_t currentEntry;
	uint8_t currentLevel;
	uint8_t currentEditFlag;//set when editing value
	uint8_t currentParent;
	uint8_t currentChildFirst;
	uint8_t currentChildLast;

	uint8_t levels;
	uint8_t entriesMain;
	uint8_t activeFlag;//set when menu active(use for external functionality, e.g. entering menu)



} menu_TypeDef;

void menu_init(menu_TypeDef *menu);
void menu_itemChangeValue(menu_TypeDef *menu, uint8_t changeDir);
void menu_itemSetValue(menu_TypeDef *menu, uint8_t entry, uint8_t level,
		int32_t value);
void menu_itemInit(menu_TypeDef *menu, uint8_t entry, uint8_t level, char *name,
		int32_t minValue, int32_t maxValue, uint8_t parent, uint8_t childFirst,
		uint8_t childLast);
int32_t menu_readCurrentValue(menu_TypeDef *menu);
void menu_readCurrentName(menu_TypeDef *menu, char *name, uint8_t size);
int32_t menu_itemReadValue(menu_TypeDef* menu, uint8_t entry, uint8_t level);
uint8_t menu_switch(menu_TypeDef *menu, uint8_t direction);
uint8_t menu_readCurrentEditFlag(menu_TypeDef* menu);
uint8_t menu_readActiveFlag(menu_TypeDef* menu);
uint8_t menu_readIfHaveSubmenu(menu_TypeDef* menu);
void menu_turnOn(menu_TypeDef *menu);
void menu_turnOff(menu_TypeDef *menu);

#endif /* INC_MENU_H_ */
