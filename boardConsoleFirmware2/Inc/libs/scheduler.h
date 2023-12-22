/*
 * scheduler.h
 *
 *  Created on: Oct 12, 2023
 *      Author: bartosz
 */

/*
  	Scheduler
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

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <stdint.h>

//user config
#define SCHEDULER_PROCESS_COUNT 10

//end of user config

typedef struct {

	void (*function)(void);
	uint8_t number;

} schedulerTask_TypeDef;

void scheduler_init(void);
void scheduler_processInit(schedulerTask_TypeDef *process,
		void (*function)(void), uint8_t number, uint32_t priority);
void scheduler_process(schedulerTask_TypeDef *process);
void scheduler_callback(void);

#endif /* SCHEDULER_H_ */
