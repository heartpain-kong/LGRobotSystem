/* ============================================================================
 * bsp_gpio.h — GPIO 通用接口(薄封装,仿 cod-h7-template-main/BSP/bsp_gpio 与
 * 说明 / 用法 / 坑 见同目录 bsp_gpio.md
 * ========================================================================== */

#ifndef BSP_GPIO_H
#define BSP_GPIO_H

#include "main.h"       /* 从 CubeMX 拿到 GPIOx / GPIO_PIN_x / HAL */

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 写一个引脚
 * @param gpiox 端口(GPIOA..GPIOH)
 * @param pin   引脚(GPIO_PIN_x)
 * @param state GPIO_PIN_SET / GPIO_PIN_RESET
 * @return void
 */
void BSP_GPIO_Write(GPIO_TypeDef *gpiox, uint16_t pin, GPIO_PinState state);

/**
 * @brief 读一个引脚
 * @param gpiox 端口
 * @param pin   引脚
 * @return GPIO_PIN_SET / GPIO_PIN_RESET
 */
GPIO_PinState BSP_GPIO_Read(GPIO_TypeDef *gpiox, uint16_t pin);

/**
 * @brief 翻转一个引脚
 * @param gpiox 端口
 * @param pin   引脚
 * @return void
 */
void BSP_GPIO_Toggle(GPIO_TypeDef *gpiox, uint16_t pin);

/**
 * @brief 一次写多个引脚(同一端口)
 * @param gpiox 端口
 * @param pins  引脚掩码(如 GPIO_PIN_0 | GPIO_PIN_1)
 * @param state 电平
 * @return void
 */
void BSP_GPIO_Write_Multi(GPIO_TypeDef *gpiox, uint16_t pins, GPIO_PinState state);

/**
 * @brief 把"低电平有效"的引脚按逻辑值写(active = true 写有效电平)
 * @param gpiox       端口
 * @param pin         引脚
 * @param active_level 有效电平(如 GPIO_PIN_RESET 表示低有效)
 * @param active      true = 输出有效电平;false = 输出无效电平
 * @return void
 */
void BSP_GPIO_Write_Active(GPIO_TypeDef *gpiox, uint16_t pin,
                           GPIO_PinState active_level, bool active);

#ifdef __cplusplus
}
#endif

#endif // BSP_GPIO_H
