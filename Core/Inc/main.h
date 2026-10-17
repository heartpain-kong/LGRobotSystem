/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* ============================================================================
 * 「错误现场」全局变量 —— 调试时先看 g_fault_code,一眼知道停在哪一类错误里
 *
 *   g_fault_code = 0  正常
 *                = 1  vApplicationMallocFailedHook  → FreeRTOS 动态内存分配失败(堆不够)
 *                = 2  vApplicationStackOverflowHook → 某个任务栈溢出(名字在 g_fault_task)
 *                = 3  Error_Handler()               → HAL/外设初始化失败
 *                = 4  HardFault_Handler()           → 硬件异常(寄存器在 g_fault_*)
 *
 * 为什么需要它:上面四个的默认实现都只是一个 `for(;;)`,用调试器看**完全一样**,
 *               分不清是堆爆、栈溢出、外设没起来还是跑飞了。
 * 定义在 Core/Src/main.c 的 USER CODE Variables 段。
 * ========================================================================== */
extern volatile uint32_t g_fault_code;
extern volatile const char *g_fault_task;      /* 栈溢出时:出事的任务名 */
extern volatile uint32_t g_fault_cfsr;         /* HardFault:SCB->CFSR  可配置故障状态 */
extern volatile uint32_t g_fault_hfsr;         /* HardFault:SCB->HFSR  硬故障状态 */
extern volatile uint32_t g_fault_mmfar;        /* HardFault:SCB->MMFAR 出错地址(MPU/总线) */
extern volatile uint32_t g_fault_bfar;         /* HardFault:SCB->BFAR  出错地址(总线) */
extern volatile uint32_t g_fault_psp;          /* HardFault:当时的 PSP */
extern volatile uint32_t g_fault_msp;          /* HardFault:当时的 MSP */
extern volatile uint32_t g_fault_pc_psp;       /* HardFault:出错时的 PC(从 PSP 栈帧取,任务里跑飞时有效) */

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
#define POWER_5V_Pin GPIO_PIN_15
#define POWER_5V_GPIO_Port GPIOC
#define ACCEL_CS_Pin GPIO_PIN_0
#define ACCEL_CS_GPIO_Port GPIOC
#define GYRO_CS_Pin GPIO_PIN_3
#define GYRO_CS_GPIO_Port GPIOC
#define W25Q64_HOLD__OSPI_IO3_Pin GPIO_PIN_1
#define W25Q64_HOLD__OSPI_IO3_GPIO_Port GPIOA
#define W25Q64_WP__OSPI_IO2_Pin GPIO_PIN_3
#define W25Q64_WP__OSPI_IO2_GPIO_Port GPIOA
#define W25Q64_SO__OSPI_IO1_Pin GPIO_PIN_0
#define W25Q64_SO__OSPI_IO1_GPIO_Port GPIOB
#define W25Q64_CLK__OSPI_CLK_Pin GPIO_PIN_2
#define W25Q64_CLK__OSPI_CLK_GPIO_Port GPIOB
#define ACCEL_INT_Pin GPIO_PIN_10
#define ACCEL_INT_GPIO_Port GPIOE
#define W25Q64_CS__OSPI_NCS_Pin GPIO_PIN_11
#define W25Q64_CS__OSPI_NCS_GPIO_Port GPIOE
#define GYRO_INT_Pin GPIO_PIN_12
#define GYRO_INT_GPIO_Port GPIOE
#define W25Q64_SI__OSPI_IO0_Pin GPIO_PIN_11
#define W25Q64_SI__OSPI_IO0_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
