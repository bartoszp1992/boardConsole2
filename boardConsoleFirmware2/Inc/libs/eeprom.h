/*
 * eeprom.h
 *
 *  Created on: Oct 14, 2023
 *      Author: bartosz
 */

/*
  	Universal EEPROM library
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

#ifndef EEPROM_H_
#define EEPROM_H_

#include "stm32g0xx_hal.h"

typedef struct {

	I2C_HandleTypeDef *i2c;
	uint16_t addr;
	uint8_t writeTime; //[ms]
	uint16_t memAddrSize;
	uint32_t timeout;

} eeprom_TypeDef;

#define EEPROM_STAT_OK 0
#define EEPROM_STAT_ERR 1

uint8_t eeprom_init(eeprom_TypeDef *eeprom, I2C_HandleTypeDef *i2c,
		uint16_t addr, uint8_t writeTime, uint8_t memAddrSize, uint32_t timeout);
uint8_t eeprom_writeByte(eeprom_TypeDef *eeprom, uint16_t addr, uint8_t data);
uint8_t eeprom_readByte(eeprom_TypeDef *eeprom, uint16_t addr);
uint8_t eeprom_write(eeprom_TypeDef *eeprom, uint16_t addr, uint8_t *data,
		uint8_t size);
void eeprom_read(eeprom_TypeDef *eeprom, uint16_t addr, uint8_t *data,
		uint8_t size);
uint8_t eeprom_writeWord(eeprom_TypeDef *eeprom, uint16_t addr, int32_t data);
int32_t eeprom_readWord(eeprom_TypeDef* eeprom, uint16_t addr);

#endif /* EEPROM_H_ */
