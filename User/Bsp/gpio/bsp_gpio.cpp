//
// bsp_gpio.cpp — GPIO 通用接口(薄封装)
// 说明 / 用法 / 坑 见同目录 bsp_gpio.md

#include "bsp_gpio.h"

/**
 * @brief 写一个引脚
 * @param gpiox 端口
 * @param pin   引脚
 * @param state 电平
 * @return void
 */
void BSP_GPIO_Write(GPIO_TypeDef *gpiox, uint16_t pin, GPIO_PinState state)
{
    if (gpiox == nullptr)
    {
        return;
    }
    HAL_GPIO_WritePin(gpiox, pin, state);
}

/**
 * @brief 读一个引脚
 * @param gpiox 端口
 * @param pin   引脚
 * @return 电平
 */
GPIO_PinState BSP_GPIO_Read(GPIO_TypeDef *gpiox, uint16_t pin)
{
    if (gpiox == nullptr)
    {
        return GPIO_PIN_RESET;
    }
    return HAL_GPIO_ReadPin(gpiox, pin);
}

/**
 * @brief 翻转一个引脚
 * @param gpiox 端口
 * @param pin   引脚
 * @return void
 */
void BSP_GPIO_Toggle(GPIO_TypeDef *gpiox, uint16_t pin)
{
    if (gpiox == nullptr)
    {
        return;
    }
    HAL_GPIO_TogglePin(gpiox, pin);
}

/**
 * @brief 一次写多个引脚
 * @param gpiox 端口
 * @param pins  引脚掩码
 * @param state 电平
 * @return void
 */
void BSP_GPIO_Write_Multi(GPIO_TypeDef *gpiox, uint16_t pins, GPIO_PinState state)
{
    if (gpiox == nullptr)
    {
        return;
    }
    HAL_GPIO_WritePin(gpiox, pins, state);
}

/**
 * @brief 按"有效电平"语义写引脚(低有效器件用起来更直观)
 * @param gpiox        端口
 * @param pin          引脚
 * @param active_level 有效电平
 * @param active       true = 有效
 * @return void
 */
void BSP_GPIO_Write_Active(GPIO_TypeDef *gpiox, uint16_t pin,
                           GPIO_PinState active_level, bool active)
{
    if (gpiox == nullptr)
    {
        return;
    }

    GPIO_PinState level;

    if (active)
    {
        level = active_level;
    }
    else
    {
        level = (active_level == GPIO_PIN_RESET) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    }

    HAL_GPIO_WritePin(gpiox, pin, level);
}
