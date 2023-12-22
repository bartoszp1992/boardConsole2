/*
 * nmea0183.h
 *
 *  Created on: 10 paź 2023
 *      Author: bartosz
 */

/*
  	NMEA0183 parser
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

#ifndef NMEA0183_H_
#define NMEA0183_H_

#include "stm32g0xx_hal.h"
#include <string.h>

//user config
#define NMEA0183_BUFFER_SIZE 100 //buffer size
#define NMEA0183_TIMEOUT 10 //uart timeout

#define NMEA0183_MAX_PARAMETER_COUNT 30//max parameters in one frame
#define NMEA0183_MAX_PARAMETER_SIZE 12//max parameter length present in any frame

#define NMEA0183_FRAMETYPES_COUNT 6//summary number of frametypes

#define NMEA0183_FRAMETYPE_RMC 0
#define NMEA0183_FRAMETYPE_VTG 1
#define NMEA0183_FRAMETYPE_GGA 2
#define NMEA0183_FRAMETYPE_GSA 3
#define NMEA0183_FRAMETYPE_GSV 4
#define NMEA0183_FRAMETYPE_GLL 5
//add more frametypoed here if needed


//specified frames and numbers for parameters
#define NMEA0183_FRAME_SPEED NMEA0183_FRAMETYPE_VTG
#define NMEA0183_PARAM_SPEED 5

#define NMEA0183_FRAME_LONGITUDE NMEA0183_FRAMETYPE_RMC
#define NMEA0183_PARAM_LONGITUDE 3

#define NMEA0183_FRAME_TIME NMEA0183_FRAMETYPE_RMC
#define NMEA0183_PARAM_TIME 1

#define NMEA0183_FRAME_AZIMUTH NMEA0183_FRAMETYPE_VTG
#define NMEA0183_PARAM_AZIMUTH 1

//end of user config

#define NMEA0183_FRAMETYPE_UNKNOWN NMEA0183_FRAMETYPES_COUNT


//statuses
#define NMEA0183_STAT_GRABBED 0 //grabbed frame
#define NMEA0183_STAT_OK 1 //waiting for frame begin $
#define NMEA0183_STAT_NO_DATA 2//no data receiving
#define NMEA0183_STAT_NOT_VALID 3//frame end not reached
#define NMEA0183_STAT_OVERFLOW 4 // destination too short



typedef struct{

	char frame[NMEA0183_BUFFER_SIZE];
	char parameters[NMEA0183_MAX_PARAMETER_COUNT][NMEA0183_MAX_PARAMETER_SIZE];
	uint32_t parameterActualSize[NMEA0183_MAX_PARAMETER_COUNT];
	uint8_t parametersActualCount;

}gpsFrame_TypeDef;

typedef struct{

	UART_HandleTypeDef* uart;

	uint8_t receiveBuffer[NMEA0183_BUFFER_SIZE]; //scratchpad for received frame
	uint8_t RBC; //receiving buffer counter- points to receiveBuffer index actual received
	uint8_t endFlag; //set to 1 when reaches end of single frame

	//classified frames

	gpsFrame_TypeDef classifiedFrames[NMEA0183_FRAMETYPES_COUNT+1]; ///+1 for unknown

}gps_TypeDef;
void nmea0183_init_IT(gps_TypeDef *gps, UART_HandleTypeDef *uart);
void nmea0183_init_poll(gps_TypeDef *gps, UART_HandleTypeDef* uart);
void nmea0183_grabFrame_IT(gps_TypeDef *gps, UART_HandleTypeDef *uart);
uint8_t nmea0183_grabFrame_poll(gps_TypeDef *gps);
uint8_t nmea0183_retval(gps_TypeDef *gps, uint8_t frame, uint8_t parameter,
		char *destination, uint8_t size);

#endif /* NMEA0183_H_ */
