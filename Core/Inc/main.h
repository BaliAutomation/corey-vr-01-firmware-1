/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void increment(int pin);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DA1_OUT_Pin GPIO_PIN_0
#define DA1_OUT_GPIO_Port GPIOA
#define DA2_OUT_Pin GPIO_PIN_1
#define DA2_OUT_GPIO_Port GPIOA
#define DA3_OUT_Pin GPIO_PIN_2
#define DA3_OUT_GPIO_Port GPIOA
#define DS1_IN_Pin GPIO_PIN_3
#define DS1_IN_GPIO_Port GPIOA
#define DS2_IN_Pin GPIO_PIN_4
#define DS2_IN_GPIO_Port GPIOA
#define DS3_IN_Pin GPIO_PIN_5
#define DS3_IN_GPIO_Port GPIOA
#define SV_OUT_Pin GPIO_PIN_6
#define SV_OUT_GPIO_Port GPIOA
#define FR_OUT_Pin GPIO_PIN_7
#define FR_OUT_GPIO_Port GPIOA
#define HU_IN_Pin GPIO_PIN_0
#define HU_IN_GPIO_Port GPIOB
#define HU_IN_EXTI_IRQn EXTI0_IRQn
#define HV_IN_Pin GPIO_PIN_1
#define HV_IN_GPIO_Port GPIOB
#define HV_IN_EXTI_IRQn EXTI1_IRQn
#define I2C_USB2_SCL_Pin GPIO_PIN_10
#define I2C_USB2_SCL_GPIO_Port GPIOB
#define I2C_USB2_SDA_Pin GPIO_PIN_11
#define I2C_USB2_SDA_GPIO_Port GPIOB
#define OUT1_Pin GPIO_PIN_12
#define OUT1_GPIO_Port GPIOB
#define OUT2_Pin GPIO_PIN_13
#define OUT2_GPIO_Port GPIOB
#define OUT3_Pin GPIO_PIN_14
#define OUT3_GPIO_Port GPIOB
#define OUT4_Pin GPIO_PIN_15
#define OUT4_GPIO_Port GPIOB
#define RCMP_OUT_Pin GPIO_PIN_8
#define RCMP_OUT_GPIO_Port GPIOA
#define HW_IN_Pin GPIO_PIN_4
#define HW_IN_GPIO_Port GPIOB
#define HW_IN_EXTI_IRQn EXTI4_IRQn
#define HOME_IN_Pin GPIO_PIN_5
#define HOME_IN_GPIO_Port GPIOB
#define I2C_USB1_SCL_Pin GPIO_PIN_6
#define I2C_USB1_SCL_GPIO_Port GPIOB
#define I2C_USB1_SDA_Pin GPIO_PIN_7
#define I2C_USB1_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
