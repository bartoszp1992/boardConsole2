/*
 * nmea0183.c
 *
 *  Created on: 10 paź 2023
 *      Author: bartosz
 *
 *      universal NMEA0183 parser for nmea-compatible GPS
 *      add more frametypes if needed in nmea0183_classifyFrame()
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

#include <libs/nmea0183.h>

static void nmea0183_pullParameters(gpsFrame_TypeDef *gpsFrame);
static gpsFrame_TypeDef* nmea0183_classifyFrame(gps_TypeDef *gps, char *type);

/*
 * initialize interface
 * @param: gps struct
 * @param: related HAL uart struct handler
 */
void nmea0183_init_poll(gps_TypeDef *gps, UART_HandleTypeDef *uart) {

	gps->RBC = 0;
	gps->uart = uart;
	gps->endFlag = 0;

}


/*
 * initialize interface- interrupt mode
 * @param: gps struct
 * @param: related HAL uart struct handler
 */
void nmea0183_init_IT(gps_TypeDef *gps, UART_HandleTypeDef *uart) {
	gps->RBC = 0;
	gps->uart = uart;
	gps->endFlag = 0;

	HAL_UART_Receive_IT(gps->uart, &gps->receiveBuffer[gps->RBC], 1);
}

/*
 * get GPS data
 * @param: gps structure
 * @param: frame NMEA0183_FRAMETYPE_* or NMEA0183_FRAME_(param)
 * @param: number of parameter need to pull. e.g. NMEA0183_PARAM_(param) or just number
 * @param: destination pointer- where to copy data.
 * 			must be 1 byte more than expected string becouse of string end char \0
 * @param: max size of destination- array overflow protection. size of element from previous param
 * @retval: status NMEA0183_STAT_*
 */
uint8_t nmea0183_retval(gps_TypeDef *gps, uint8_t frame, uint8_t parameter,
		char *destination, uint8_t size) {

	parameter--; //numbering parameter from 1

	if (gps->classifiedFrames[frame].parameters[parameter][0] == '\0') { //when empty, theres be only strin eng char
		return NMEA0183_STAT_NO_DATA;
	} else {

		if (gps->classifiedFrames[frame].parameterActualSize[parameter]
				> size) {
			return NMEA0183_STAT_OVERFLOW;
		} else {
			memcpy(destination,
					&gps->classifiedFrames[frame].parameters[parameter],
					gps->classifiedFrames[frame].parameterActualSize[parameter]);
		}

		return NMEA0183_STAT_OK;
	}

}


/*
 * run frame grabbing- run in void HAL_UART_RxCpltCallback()
 * @param: gps structure
 * @param: uart struct from interrupt
 * @retval: none
 */
void nmea0183_grabFrame_IT(gps_TypeDef *gps, UART_HandleTypeDef *uart) {

	if (uart == gps->uart) {
		if (gps->receiveBuffer[gps->RBC] == '$') { //frame start
			gps->RBC = 0;
		} else if (gps->receiveBuffer[gps->RBC - 1] == '\r'
				&& gps->receiveBuffer[gps->RBC] == '\n') { //frame end

			//clear rest of the buffer
			memset((char*) &gps->receiveBuffer[gps->RBC + 1], 0,
			NMEA0183_BUFFER_SIZE - gps->RBC - 1);

			//get frame header
			char frameHeader[3];
			frameHeader[0] = gps->receiveBuffer[3];
			frameHeader[1] = gps->receiveBuffer[4];
			frameHeader[2] = gps->receiveBuffer[5];

			//get pointer to appropriate structure
			gpsFrame_TypeDef *destinationPointer = nmea0183_classifyFrame(gps,
					frameHeader);
			memcpy((char*) destinationPointer->frame,
					(char*) &gps->receiveBuffer,
					NMEA0183_BUFFER_SIZE);

			nmea0183_pullParameters(destinationPointer);

		}
		gps->RBC++;
		HAL_UART_Receive_IT(gps->uart, &gps->receiveBuffer[gps->RBC], 1);
	}

}

/*
 * run frame grabbing. run it cyclic(in main loop) as often as possible
 * @param: gps structure
 * @retval: status NMEA0183_STAT_*
 */
uint8_t nmea0183_grabFrame_poll(gps_TypeDef *gps) {
	uint8_t status = NMEA0183_STAT_OK;
	HAL_UART_Receive(gps->uart, &gps->receiveBuffer[0], 1, NMEA0183_TIMEOUT);

	if (gps->receiveBuffer[0] == '$') {
		gps->endFlag = 0;
		gps->RBC = 1;
		while (gps->endFlag == 0) {
			if (HAL_UART_Receive(gps->uart, &gps->receiveBuffer[gps->RBC], 1,
			NMEA0183_TIMEOUT) != HAL_OK) {
				status = NMEA0183_STAT_NO_DATA;
				break;
			} else {
				if (gps->receiveBuffer[gps->RBC - 1] == '\r'
						&& gps->receiveBuffer[gps->RBC] == '\n') {
					gps->endFlag = 1;

					//clear rest of the buffer
					memset((char*) &gps->receiveBuffer[gps->RBC + 1], 0,
					NMEA0183_BUFFER_SIZE - gps->RBC - 1);

					//get frame header
					char frameHeader[3];
					frameHeader[0] = gps->receiveBuffer[3];
					frameHeader[1] = gps->receiveBuffer[4];
					frameHeader[2] = gps->receiveBuffer[5];

					//get pointer to appropriate structure
					gpsFrame_TypeDef *destinationPointer =
							nmea0183_classifyFrame(gps, frameHeader);
					memcpy((char*) destinationPointer->frame,
							(char*) &gps->receiveBuffer, NMEA0183_BUFFER_SIZE);

					nmea0183_pullParameters(destinationPointer);

					status = NMEA0183_STAT_GRABBED;
				}
				gps->RBC++;
				if (gps->RBC >= NMEA0183_BUFFER_SIZE) {
					status = NMEA0183_STAT_NOT_VALID;
					break;
				}
			}
		}

	}
	return status;
}

/*
 * pull single parameters from received frame
 * @param: frame struct
 */
static void nmea0183_pullParameters(gpsFrame_TypeDef *gpsFrame) {

	gpsFrame->parametersActualCount = 0;
	char *parameterPointers[NMEA0183_MAX_PARAMETER_COUNT] = { };

	for (uint8_t i = 0; i <= NMEA0183_BUFFER_SIZE; i++) {

		if (gpsFrame->frame[i] == ',') {

			parameterPointers[gpsFrame->parametersActualCount] =
					&gpsFrame->frame[i + 1];
			gpsFrame->parametersActualCount++;
		} else if (gpsFrame->frame[i] == '\n'
				&& gpsFrame->frame[i - 1] == '\r') {
			parameterPointers[gpsFrame->parametersActualCount] =
					&gpsFrame->frame[i];
			break;
		}
	}

	for (uint8_t i = 0; i < gpsFrame->parametersActualCount; i++) {

		//previous pointer size was too small(have to be 32 bit value)
		gpsFrame->parameterActualSize[i] = (uint32_t) (parameterPointers[i + 1]
				- (uint32_t) parameterPointers[i]); //get parameter size(difference between pointers) + 1 byte for '\0'

		memset(&gpsFrame->parameters[i][0], ' ',
		NMEA0183_MAX_PARAMETER_SIZE); //reset memory

		gpsFrame->parameters[i][gpsFrame->parameterActualSize[i] - 1] = '\0'; //write string end on last position

		if (gpsFrame->parameterActualSize[i] != 1) { // if parameter grabbed(difference between pointers is 1 byte- ,,)

			if (gpsFrame->parameterActualSize[i] <= NMEA0183_MAX_PARAMETER_SIZE) { //parameter overflow protection
				memcpy(&gpsFrame->parameters[i][0], parameterPointers[i],
						gpsFrame->parameterActualSize[i] - 1); //copy parameter(skip string end)

			}

		}

	}
}

/*
 * Frame type classification
 * @param: gps struct
 * @param: frame header(eg. RMC)
 * @retval: destination- pointer to gps structure element for save frame
 */
static gpsFrame_TypeDef* nmea0183_classifyFrame(gps_TypeDef *gps, char *type) {
	if (!memcmp(type, "RMC", 3))
		return &gps->classifiedFrames[NMEA0183_FRAMETYPE_RMC];

#ifdef NMEA0183_FRAMETYPE_VTG
	else if (!memcmp(type, "VTG", 3))
		return &gps->classifiedFrames[NMEA0183_FRAMETYPE_VTG];
#endif
#ifdef NMEA0183_FRAMETYPE_GGA
	else if (!memcmp(type, "GGA", 3))
		return &gps->classifiedFrames[NMEA0183_FRAMETYPE_GGA];
#endif
#ifdef NMEA0183_FRAMETYPE_GSA
	else if (!memcmp(type, "GSA", 3))
		return &gps->classifiedFrames[NMEA0183_FRAMETYPE_GSA];
#endif
#ifdef NMEA0183_FRAMETYPE_GSV
	else if (!memcmp(type, "GSV", 3))
		return &gps->classifiedFrames[NMEA0183_FRAMETYPE_GSV];
#endif
#ifdef NMEA0183_FRAMETYPE_GLL
	else if (!memcmp(type, "GLL", 3))
		return &gps->classifiedFrames[NMEA0183_FRAMETYPE_GLL];
#endif

	//add more frametypes here if needed

	else
		return &gps->classifiedFrames[NMEA0183_FRAMETYPE_UNKNOWN];
}
