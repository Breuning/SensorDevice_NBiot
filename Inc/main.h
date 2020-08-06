/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void Hardware_Init(void); 
void McuReset(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED2_NETWORK_Pin GPIO_PIN_13
#define LED2_NETWORK_GPIO_Port GPIOC
#define LED1_RUNNING_Pin GPIO_PIN_14
#define LED1_RUNNING_GPIO_Port GPIOC
#define PC_TX_Pin GPIO_PIN_2
#define PC_TX_GPIO_Port GPIOA
#define PC_RX_Pin GPIO_PIN_3
#define PC_RX_GPIO_Port GPIOA
#define NB_PWRKEY_Pin GPIO_PIN_6
#define NB_PWRKEY_GPIO_Port GPIOA
#define SW4_Pin GPIO_PIN_7
#define SW4_GPIO_Port GPIOA
#define SW3_Pin GPIO_PIN_4
#define SW3_GPIO_Port GPIOC
#define SW2_Pin GPIO_PIN_5
#define SW2_GPIO_Port GPIOC
#define SW1_Pin GPIO_PIN_0
#define SW1_GPIO_Port GPIOB
#define GAS_TX_Pin GPIO_PIN_10
#define GAS_TX_GPIO_Port GPIOB
#define GAS_RX_Pin GPIO_PIN_11
#define GAS_RX_GPIO_Port GPIOB
#define LORANODE_NRST_Pin GPIO_PIN_6
#define LORANODE_NRST_GPIO_Port GPIOC
#define LORANODE_BUSY_Pin GPIO_PIN_7
#define LORANODE_BUSY_GPIO_Port GPIOC
#define LORANODE_MODE_Pin GPIO_PIN_8
#define LORANODE_MODE_GPIO_Port GPIOC
#define LORANODE_STAT_Pin GPIO_PIN_9
#define LORANODE_STAT_GPIO_Port GPIOC
#define LORANODE_WAKE_Pin GPIO_PIN_8
#define LORANODE_WAKE_GPIO_Port GPIOA
#define LORA_TX_Pin GPIO_PIN_9
#define LORA_TX_GPIO_Port GPIOA
#define LORA_RX_Pin GPIO_PIN_10
#define LORA_RX_GPIO_Port GPIOA
#define CAN_RX_Pin GPIO_PIN_11
#define CAN_RX_GPIO_Port GPIOA
#define CAN_TX_Pin GPIO_PIN_12
#define CAN_TX_GPIO_Port GPIOA
#define RS485_TX_Pin GPIO_PIN_10
#define RS485_TX_GPIO_Port GPIOC
#define RS485_RX_Pin GPIO_PIN_11
#define RS485_RX_GPIO_Port GPIOC
#define RS485_SEL_Pin GPIO_PIN_12
#define RS485_SEL_GPIO_Port GPIOC
#define SW5_Pin GPIO_PIN_2
#define SW5_GPIO_Port GPIOD
#define SW6_Pin GPIO_PIN_3
#define SW6_GPIO_Port GPIOB
#define SW7_Pin GPIO_PIN_4
#define SW7_GPIO_Port GPIOB
#define SW8_Pin GPIO_PIN_5
#define SW8_GPIO_Port GPIOB
#define I2C1_SCL_Pin GPIO_PIN_6
#define I2C1_SCL_GPIO_Port GPIOB
#define I2C1_SDA_Pin GPIO_PIN_7
#define I2C1_SDA_GPIO_Port GPIOB
#define NB_PEN_Pin GPIO_PIN_8
#define NB_PEN_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define LORANODE_GPIO_Port GPIOC

typedef enum
{
	FALSE,
	TRUE,
}BOOL;

typedef enum{
	false = 0,
	true = 1,
}bool;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
