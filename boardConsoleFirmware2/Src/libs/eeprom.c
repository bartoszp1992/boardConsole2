/*
 * eeprom.c
 *
 *  Created on: Oct 14, 2023
 *      Author: bartosz
 *
 *      universal eeprom handling
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

#include <libs/eeprom.h>

/*
 * initialize eeprom memory
 * @param: eeprom structure
 * @param: i2c HAL handler
 * @param: i2c memory address
 * @param: write time(for delay)
 * @param: size of single cell address
 * @param: timeout for i2c
 */
uint8_t eeprom_init(eeprom_TypeDef *eeprom, I2C_HandleTypeDef *i2c,
		uint16_t addr, uint8_t writeTime, uint8_t memAddrSize, uint32_t timeout) {
	eeprom->i2c = i2c;
	eeprom->addr = addr;
	eeprom->writeTime = writeTime;
	eeprom->memAddrSize = memAddrSize;
	eeprom->timeout = timeout;

	uint8_t data;

	//test read
	if (HAL_I2C_Mem_Read(eeprom->i2c, addr, 0x00, eeprom->memAddrSize, &data, 1,
			eeprom->timeout) == HAL_OK)
		return EEPROM_STAT_OK;
	else
		return EEPROM_STAT_ERR;

	HAL_Delay(5);
}

/*
 * save data to memory
 * ! watch out- using delay
 * @param: eeprom struct
 * @param: internal memory address to save data
 * @param: data
 * @retval: status- EEPROM_STAT_*
 */
uint8_t eeprom_writeByte(eeprom_TypeDef *eeprom, uint16_t addr, uint8_t data) {

	uint8_t retval;

	if (HAL_I2C_Mem_Write(eeprom->i2c, eeprom->addr, addr, eeprom->memAddrSize,
			&data, 1, eeprom->timeout) == HAL_OK)
		retval = EEPROM_STAT_OK;
	else
		retval = EEPROM_STAT_ERR;

	HAL_Delay(eeprom->writeTime);

	return retval;
}

/*
 * read from eeprom
 * @param: eeprom struct
 * @param: mem internal addr to read
 * @retval: stored value
 */
uint8_t eeprom_readByte(eeprom_TypeDef *eeprom, uint16_t addr) {

	uint8_t data;

	HAL_I2C_Mem_Read(eeprom->i2c, eeprom->addr, addr, eeprom->memAddrSize,
			&data, 1, eeprom->timeout);

	return data;
}

uint8_t eeprom_write(eeprom_TypeDef *eeprom, uint16_t addr, uint8_t *data,
		uint8_t size) {

	uint8_t stat = EEPROM_STAT_OK;

	for (uint8_t i = 0; i < size; i++) {
		if (eeprom_writeByte(eeprom, addr + i, data[i]) != EEPROM_STAT_OK)
			stat = EEPROM_STAT_ERR;
	}

	return stat;
}

void eeprom_read(eeprom_TypeDef *eeprom, uint16_t addr, uint8_t *data,
		uint8_t size) {

	for (uint8_t i = 0; i < size; i++) {
		data[i] = eeprom_readByte(eeprom, addr + i);
	}

}

uint8_t eeprom_writeWord(eeprom_TypeDef *eeprom, uint16_t addr, int32_t data) {

	uint8_t stat = EEPROM_STAT_OK;

	union {
		uint8_t dataArr[4];
		int32_t dataInt;
	} arr2int;

	arr2int.dataInt = data;

	if (eeprom_write(eeprom, addr, arr2int.dataArr, 4) != EEPROM_STAT_OK)
		stat = EEPROM_STAT_ERR;

	return stat;

}

int32_t eeprom_readWord(eeprom_TypeDef* eeprom, uint16_t addr) {

	union {
		uint8_t dataArr[4];
		int32_t dataInt;
	} arr2int;


	eeprom_read(eeprom, addr, arr2int.dataArr, 4);


	return arr2int.dataInt;

}
