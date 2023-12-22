/*
 * menu.c
 * MULTILEVEL MENU HANDLING LIBRARY
 *
 *
 * 	Created on: 15 apr 2022
 *      Author: Bartosz
 *      varsion: 1.1
 *
 *      changelog:
 *      1.1 - changed directive prefix as MENU
 *      1.2 - automatically updating current item
 *      2.0 - new menu mechanisms
 *
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

#include "libs/menu.h"

/**
 * @brief  Reset current. Run always before items init!
 *
 * @note   Sets current as positon 0, 0
 *
 * @param  Menu item handler
 *
 * @retval None
 */
void menu_init(menu_TypeDef *menu) {

	menu->entriesMain = 0;
	menu->currentEditFlag = MENU_CURRENT_EDIT_DIS;
	menu->activeFlag = MENU_ACTIVE_FLAG_DIS;

}

/*
 * change value of item(increase/decrease)
 * param: menu struct
 * @param: change direction MENU_DECREASE/INCREASE
 */
void menu_itemChangeValue(menu_TypeDef *menu, uint8_t changeDir) {

	if (changeDir == MENU_CHANGE_INCREASE
			&& menu->items[menu->currentEntry][menu->currentLevel].value
					< menu->items[menu->currentEntry][menu->currentLevel].maxValue) {
		menu->items[menu->currentEntry][menu->currentLevel].value++;
	} else if (changeDir == MENU_CHANGE_DECREASE
			&& menu->items[menu->currentEntry][menu->currentLevel].value
					> menu->items[menu->currentEntry][menu->currentLevel].minValue) {
		menu->items[menu->currentEntry][menu->currentLevel].value--;
	}

}

/**
 * @brief  Set values in menu entry
 *
 * @param  Menu item handler
 *
 * @param entry number
 *
 * @param level number
 *
 * @param value
 */
void menu_itemSetValue(menu_TypeDef *menu, uint8_t entry, uint8_t level,
		int32_t value) {

	menu->items[entry][level].value = value;

}

/**
 * @brief  Initialize menu item
 *
 * @note   Sets structure of each entry
 *
 * @param  Menu item handler
 *
 * @param entry number
 *
 * @param item level
 *
 * @param name string
 *
 * @param min value
 *
 * @param max value
 *
 * @param item parent entry- MENU_NONE if no parent
 *
 * @param item first child entry number, MENU_NONE if no child
 *
 * @param item last child entry number, ENU_NONE if no child
 *
 */
void menu_itemInit(menu_TypeDef *menu, uint8_t entry, uint8_t level, char *name,
		int32_t minValue, int32_t maxValue, uint8_t parent, uint8_t childFirst,
		uint8_t childLast) {

	menu->items[entry][level].entry = entry;
	menu->items[entry][level].level = level;
	menu->items[entry][level].parent = parent;
	menu->items[entry][level].childFirst = childFirst;
	menu->items[entry][level].childLast = childLast;

	uint8_t nameLength = strlen(name);
	if (nameLength > MENU_ITEM_NAME_SIZE) {
		nameLength = MENU_ITEM_NAME_SIZE;
	}

	for (uint8_t i = 0; i <= nameLength; i++) {

		menu->items[entry][level].name[i] = name[i];
	}

	for (uint8_t i = nameLength; i < MENU_ITEM_NAME_SIZE; i++) {
		menu->items[entry][level].name[i] = ' ';
	}

	menu->items[entry][level].maxValue = maxValue;
	menu->items[entry][level].minValue = minValue;

	if (level == 0)
		menu->entriesMain++;

}

/*
 * return current value of item
 *
 * @param: menu struct
 * @retval: entry value
 */
int32_t menu_readCurrentValue(menu_TypeDef *menu) {

	return menu->items[menu->currentEntry][menu->currentLevel].value;

}

/*
 * return current item name
 * @param: manu struct
 * @param: pointer to save name
 * @param: size
 */
void menu_readCurrentName(menu_TypeDef *menu, char *name, uint8_t size) {
	memcpy(name, menu->items[menu->currentEntry][menu->currentLevel].name,
			size);
}

/*
 * read value of pointed item
 * @param menu struct
 * @param entry number to read
 * @param entry level to read
 */
int32_t menu_itemReadValue(menu_TypeDef *menu, uint8_t entry, uint8_t level) {
	return menu->items[entry][level].value;
}

/**
 * @brief  Switch current element of menu
 *
 * @note   current element stores actual selected item
 *
 * @param  Menu item handler
 *
 * @param switch direction
 *
 * @retval OK if switched, ERROR if not switched(no parent, no child, no above, or no under)
 */
uint8_t menu_switch(menu_TypeDef *menu, uint8_t direction) {

	if (direction == MENU_SWITCH_UP) {

		if (menu->currentEditFlag == MENU_CURRENT_EDIT_DIS) {

			if ((menu->currentLevel == 0 && menu->currentEntry > 0)
					|| (menu->currentLevel != 0
							&& menu->currentEntry
									> menu->items[menu->currentParent][menu->currentLevel
											- 1].childFirst)) {

				menu->currentEntry--;

				menu->currentParent =
						menu->items[menu->currentEntry][menu->currentLevel].parent;
				menu->currentChildFirst =
						menu->items[menu->currentEntry][menu->currentLevel].childFirst;
				menu->currentChildLast =
						menu->items[menu->currentEntry][menu->currentLevel].childLast;
				return MENU_OK;
			} else {
				return MENU_ERROR;
			}
		} else { //if edit flag enabled

			menu_itemChangeValue(menu, MENU_CHANGE_DECREASE);
			return MENU_OK;

		}

	} else if (direction == MENU_SWITCH_DOWN) {

		if (menu->currentEditFlag == MENU_CURRENT_EDIT_DIS) {

			if ((menu->currentLevel == 0
					&& menu->currentEntry < menu->entriesMain - 1) /////////////////////////
					|| (menu->currentLevel != 0
							&& menu->currentEntry
									< menu->items[menu->currentParent][menu->currentLevel
											- 1].childLast)) {

				menu->currentEntry++;

				menu->currentParent =
						menu->items[menu->currentEntry][menu->currentLevel].parent;
				menu->currentChildFirst =
						menu->items[menu->currentEntry][menu->currentLevel].childFirst;
				menu->currentChildLast =
						menu->items[menu->currentEntry][menu->currentLevel].childLast;
				return MENU_OK;

			} else {
				return MENU_ERROR;
			}
		} else { //if edit flag enabled
			menu_itemChangeValue(menu, MENU_CHANGE_INCREASE);
			return MENU_OK;
		}

	} else if (direction == MENU_SWITCH_ENTER) {

		if (menu->currentChildFirst != MENU_NONE) { //if child exist, enter into

			menu->currentParent = menu->currentEntry;
			menu->currentEntry = menu->currentChildFirst;
			menu->currentLevel++;

			menu->currentChildFirst =
					menu->items[menu->currentEntry][menu->currentLevel].childFirst;
			menu->currentChildLast =
					menu->items[menu->currentEntry][menu->currentLevel].childLast;

			return MENU_OK;
		} else { //if child not exist, enter to edit mode
			menu->currentEditFlag = MENU_CURRENT_EDIT_EN;
			return MENU_OK;
		}

	} else if (direction == MENU_SWITCH_EXIT) {

		if (menu->currentEditFlag == MENU_CURRENT_EDIT_EN) {
			menu->currentEditFlag = MENU_CURRENT_EDIT_DIS;
			return MENU_OK;
		} else if (menu->currentParent != MENU_NONE) {

			menu->currentEntry = menu->currentParent;
			menu->currentLevel--;
			menu->currentParent =
					menu->items[menu->currentParent][menu->currentLevel].parent;
			menu->currentChildFirst =
					menu->items[menu->currentEntry][menu->currentLevel].childFirst;
			menu->currentChildLast =
					menu->items[menu->currentEntry][menu->currentLevel].childLast;

			return MENU_OK;
		} else {
			menu_turnOff(menu);
			return MENU_OK;
		}
	} else {
		return MENU_ERROR;
	}
}

/*
 * returns that element is in edit mode
 * @param menu structure
 * @retval MENU_CURRENT_EDIT_EN/DIS
 */
uint8_t menu_readCurrentEditFlag(menu_TypeDef *menu) {
	return menu->currentEditFlag;
}

/*
 * returns that menu is active
 * @param menu struct
 * @param MENU_ACTIVE_FLAG_EN/DIS
 */
uint8_t menu_readActiveFlag(menu_TypeDef *menu) {
	return menu->activeFlag;
}


/*
 * read if current position have submenu
 * @param: menu struct
 * @retval: MENU_HAVE/HAVENT_SUBMENU
 */
uint8_t menu_readIfHaveSubmenu(menu_TypeDef* menu){


	if(menu->currentChildFirst == MENU_NONE){
		return MENU_HAVENT_SUMBENU;
	}else{
		return MENU_HAVE_SUBMENU;
	}
}

//turn on menu
void menu_turnOn(menu_TypeDef *menu) {

	menu->currentEntry = 0;
	menu->currentLevel = 0;
	menu->currentParent =
			menu->items[menu->currentEntry][menu->currentLevel].parent;
	menu->currentChildFirst =
			menu->items[menu->currentEntry][menu->currentLevel].childFirst;
	menu->currentChildLast =
			menu->items[menu->currentEntry][menu->currentLevel].childLast;

	menu->activeFlag = MENU_ACTIVE_FLAG_EN;
}

//turn off menu
void menu_turnOff(menu_TypeDef *menu) {
	menu->activeFlag = MENU_ACTIVE_FLAG_DIS;
	menu->currentEditFlag = MENU_CURRENT_EDIT_DIS;
}
