/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "boardConsole/boardconsoleapp.h"
#include "FreeRTOS.h"
#include "task.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_STAT2_Pin GPIO_PIN_13
#define LED_STAT2_GPIO_Port GPIOC
#define ADC_BAT_Pin GPIO_PIN_0
#define ADC_BAT_GPIO_Port GPIOA
#define DECAY_Pin GPIO_PIN_1
#define DECAY_GPIO_Port GPIOA
#define ADC_FUEL_Pin GPIO_PIN_3
#define ADC_FUEL_GPIO_Port GPIOA
#define ADC_TRIM_Pin GPIO_PIN_4
#define ADC_TRIM_GPIO_Port GPIOA
#define TEST_Pin GPIO_PIN_5
#define TEST_GPIO_Port GPIOA
#define BACKLIGHT_Pin GPIO_PIN_6
#define BACKLIGHT_GPIO_Port GPIOA
#define BUTTON_R_Pin GPIO_PIN_0
#define BUTTON_R_GPIO_Port GPIOB
#define BUTTON_M_Pin GPIO_PIN_1
#define BUTTON_M_GPIO_Port GPIOB
#define BUTTON_L_Pin GPIO_PIN_2
#define BUTTON_L_GPIO_Port GPIOB
#define LCD_DB7_Pin GPIO_PIN_10
#define LCD_DB7_GPIO_Port GPIOB
#define LCD_DB6_Pin GPIO_PIN_11
#define LCD_DB6_GPIO_Port GPIOB
#define LCD_DB5_Pin GPIO_PIN_12
#define LCD_DB5_GPIO_Port GPIOB
#define LCD_DB4_Pin GPIO_PIN_13
#define LCD_DB4_GPIO_Port GPIOB
#define LCD_E_Pin GPIO_PIN_14
#define LCD_E_GPIO_Port GPIOB
#define LCD_RS_Pin GPIO_PIN_15
#define LCD_RS_GPIO_Port GPIOB
#define BUZZER_Pin GPIO_PIN_8
#define BUZZER_GPIO_Port GPIOA
#define LED_STAT1_Pin GPIO_PIN_8
#define LED_STAT1_GPIO_Port GPIOC
#define IGNITION_Pin GPIO_PIN_5
#define IGNITION_GPIO_Port GPIOB
#define IGNITION_EXTI_IRQn EXTI4_15_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
