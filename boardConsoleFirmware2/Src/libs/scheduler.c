/*
 * scheduler.c
 *
 *  Created on: Oct 12, 2023
 *      Author: bartosz
 *
 *      universal scheduler
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

#include <libs/scheduler.h>

struct {

	uint64_t mainCounter;
	uint8_t runFlag[SCHEDULER_PROCESS_COUNT];
	uint32_t priorities[SCHEDULER_PROCESS_COUNT];

} Schedule;


/*
 * initialize scheduler
 */
void scheduler_init(void) {
	Schedule.mainCounter = 0;
}

/*
 * initialize process
 * @param: process struct
 * @param: function pointer
 * 			set the function to run. function must be void type with void parameters
 * @param: number of process. every number should be unique
 * @param: priority- minimum interval between function runs[ms]
 */
void scheduler_processInit(schedulerTask_TypeDef *process,
		void (*function)(void), uint8_t number, uint32_t priority) {

	process->function = function; //set function to run
	process->number = number;

	Schedule.priorities[number] = priority;

}

/*
 * run this function in while loop for every created task
 * @param: process structure
 */
void scheduler_process(schedulerTask_TypeDef *process) {
	if (Schedule.runFlag[process->number]) {
		process->function();
		Schedule.runFlag[process->number] = 0;
	}

}

/*
 * run in SysTick_Handler()
 */
void scheduler_callback(void) {
	Schedule.mainCounter++;
	for (uint8_t i = 0; i < SCHEDULER_PROCESS_COUNT; i++) {
		if (Schedule.mainCounter % Schedule.priorities[i] == 0) {
			Schedule.runFlag[i] = 1;
		}
	}
}
