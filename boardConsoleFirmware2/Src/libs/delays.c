/*
 * delays.c
 *
 *	The MIT License.
 *  Created on: 11.07.2018
 *      Author: Mateusz Salamon
 *      www.msalamon.pl
 *
 *  us delay HAL addon
 *
 *  set used timer's PSC for clock freq[Mhz]-1 and max possible CP
 *
 */


#include <libs/delays.h>

struct{

	TIM_HandleTypeDef *timer;

}delays;


/*
 * initialize delays
 */
void delay_us_init(TIM_HandleTypeDef* timer){

	delays.timer = timer;
	HAL_TIM_Base_Start(timer);
}

/*
 * delay
 */
void delay_us(uint16_t us)
{
	delays.timer->Instance->CNT = 0;
	while(delays.timer->Instance->CNT <= us);
}
