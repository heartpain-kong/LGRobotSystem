/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os2.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "dev_log.h"     /* 日志:USB 起来后打开实时控制台 */
#include "robot.h"      /* 机器人层入口:Robot_Init()(在下面 RTOS_THREADS 段里调用)。
                           只能包这种 C 兼容头,绝不能 include motor_XX/xxx.h(C++ 类) */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationStackOverflowHook(xTaskHandle xTask, char *pcTaskName);
void vApplicationMallocFailedHook(void);

/* USER CODE BEGIN 4 */
/* ============================================================================
 * 任务栈溢出钩子(configCHECK_FOR_STACK_OVERFLOW = 2)
 *
 * ⚠️ 这个空壳是 **CubeMX 自己生成的**(在 USER CODE 4 里 ⇒ 重新导出会保留 ✓),
 *    所以诊断代码要写在这里 —— 不要再在下面的 USER CODE Application 里另写一份,
 *    否则就会"重复定义"编译不过(踩过这个坑)。
 * 模式 2 = 任务切换时检查栈尾魔数被踩掉;不开这个检测,栈溢出只会静默踩坏别人的变量,
 * 最后表现为各种灵异 HardFault。
 * 变量定义在 Core/Src/main.c,声明在 Core/Inc/main.h。
 * ========================================================================== */
void vApplicationStackOverflowHook(xTaskHandle xTask, char *pcTaskName)
{
   (void)xTask;
   g_fault_code = 2u;                  /* 2 = 任务栈溢出 */
   g_fault_task = pcTaskName;          /* ← 调试器看这个,直接知道是哪个任务栈小了 */
   taskDISABLE_INTERRUPTS();
   for (;;) { }
}
/* USER CODE END 4 */

/* USER CODE BEGIN 5 */
/* ============================================================================
 * 动态内存分配失败钩子(configUSE_MALLOC_FAILED_HOOK = 1)
 *
 * 同样:**这个空壳是 CubeMX 生成的**(USER CODE 5)⇒ 诊断写这里,别在别处另写一份。
 * 堆不够时 pvPortMalloc 失败会被调用。典型后果是任务"静默地不存在"
 * (osThreadNew 返回 NULL),所以这里必须留下可看现场的信息。
 * ========================================================================== */
void vApplicationMallocFailedHook(void)
{
   g_fault_code = 1u;                  /* 1 = FreeRTOS 堆分配失败 */
   taskDISABLE_INTERRUPTS();
   for (;;) { }
}
/* USER CODE END 5 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* ★ 建机器人的 RTOS 任务 —— 这里,而不是 main.c 的 USER CODE 2。原因:
   *   CMSIS-RTOS2 要求必须**先 osKernelInitialize() 才能建线程**,
   *   而 main.c 里的 USER CODE 2 在 osKernelInitialize() **之前**
   *   (参考工程 basic_framework 用 CMSIS-RTOS v1 的 osThreadCreate,
   *    那个在 FreeRTOS 下不要求先 init,所以它能放在 main 里;v2 不行)。
   *   本函数(MX_FREERTOS_Init)由 main.c 在 osKernelInitialize() 之后调用,
   *   且仍在 osKernelStart() 之前 ⇒ 这里是建任务唯一合法的位置。
   *
   *   分工:Robot_Init()(各 BSP + 各子系统初始化)在 main.c 里调;
   *         这里只负责 **建任务**(OSTaskInit:按 robot_def.h 的周期/优先级/栈创建)。
   *   前置条件:main.c 的 DWT_Init() 与 Robot_Init() 都已执行。 */
  OSTaskInit();
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartDefaultTask */
  /* USB 已经初始化完 ⇒ 打开日志的实时控制台(在 Robot_Init 里注册过回调) */
  Log_Console_Enable(true);
  Log_Printf(LOG_LEVEL_INFO, "usb console on");
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* ============================================================================
 * FreeRTOS 出错钩子 —— 只做"记录现场 + 停住",不 printf、不延时、不申请内存
 *
 * 为什么必须实现:默认的钩子(或 configASSERT)就是一个 `for(;;)`,
 * 和 Error_Handler / HardFault 在调试器里**完全一样**,根本分不清出了什么事。
 * 现在:堆爆 → g_fault_code=1;任务栈溢出 → g_fault_code=2,任务名在 g_fault_task。
 * (变量定义在 Core/Src/main.c,声明在 Core/Inc/main.h)
 * ========================================================================== */

/* USER CODE END Application */

