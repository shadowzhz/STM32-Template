/**
  ******************************************************************************
  * @file    stm32f1xx_hal_gpio_ex.c
  * @author  MCD Application Team
  * @brief   HAL库 GPIO扩展模块驱动
  *          本文件提供了管理通用输入/输出（GPIO）扩展外设的以下功能的固件函数。
  *           + 扩展功能函数
  *
  ******************************************************************************
  * @attention
  *
  * 版权所有 (c) 2016 STMicroelectronics。
  * 保留所有权利。
  *
  * 本软件根据可以在此软件组件的根目录中找到的 LICENSE 文件中的条款进行许可。
  * 如果此软件没有附带 LICENSE 文件，则按原样提供。
  *
  ******************************************************************************
  @verbatim
  ==============================================================================
                    ##### GPIO 外设扩展特性 #####
  ==============================================================================
  [..] STM32F1系列的GPIO模块，管理着AFIO寄存器：
       (+) 可以使用 EVENTOUT Cortex 功能

                     ##### 如何使用驱动 #####
  ==============================================================================
  [..] 此驱动程序提供了使用 EVENTOUT Cortex 功能的函数。
    (#) 使用函数 HAL_GPIOEx_ConfigEventout() 配置 EVENTOUT Cortex 功能。
    (#) 使用 HAL_GPIOEx_EnableEventout() 激活 EVENTOUT Cortex 功能。
    (#) 使用 HAL_GPIOEx_DisableEventout() 停用 EVENTOUT Cortex 功能。

  @endverbatim
  ******************************************************************************
  */

/* 头文件 ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @defgroup GPIOEx GPIOEx
  * @brief GPIO HAL 模块驱动
  * @{
  */

#ifdef HAL_GPIO_MODULE_ENABLED

/** @defgroup GPIOEx_Exported_Functions GPIOEx 导出函数
  * @{
  */

/** @defgroup GPIOEx_Exported_Functions_Group1 扩展特性函数
 *  @brief    扩展特性函数
 *
@verbatim
  ==============================================================================
                 ##### 扩展特性函数 #####
  ==============================================================================
    [..]  本节提供了以下功能的函数：
    (+) 使用函数 HAL_GPIOEx_ConfigEventout() 配置 EVENTOUT Cortex 功能。
    (+) 使用 HAL_GPIOEx_EnableEventout() 激活 EVENTOUT Cortex 功能。
    (+) 使用 HAL_GPIOEx_DisableEventout() 停用 EVENTOUT Cortex 功能。

@endverbatim
  * @{
  */

/**
  * @brief  配置 EVENTOUT Cortex 信号将连接的端口和引脚
  * @param  GPIO_PortSource 选择用于输出 Cortex EVENTOUT 信号的端口。
  *   该参数可以是 @ref GPIOEx_EVENTOUT_PORT 中的值。
  * @param  GPIO_PinSource 选择用于输出 Cortex EVENTOUT 信号的引脚。
  *   该参数可以是 @ref GPIOEx_EVENTOUT_PIN 中的值。
  * @retval None
  */
void HAL_GPIOEx_ConfigEventout(uint32_t GPIO_PortSource, uint32_t GPIO_PinSource)
{
  /* 检查参数 */
  assert_param(IS_AFIO_EVENTOUT_PORT(GPIO_PortSource));
  assert_param(IS_AFIO_EVENTOUT_PIN(GPIO_PinSource));

  /* 应用新配置 */
  MODIFY_REG(AFIO->EVCR, (AFIO_EVCR_PORT) | (AFIO_EVCR_PIN), (GPIO_PortSource) | (GPIO_PinSource));
}

/**
  * @brief  启用事件输出
  * @retval None
  */
void HAL_GPIOEx_EnableEventout(void)
{
  SET_BIT(AFIO->EVCR, AFIO_EVCR_EVOE);
}

/**
  * @brief  禁用事件输出
  * @retval None
  */
void HAL_GPIOEx_DisableEventout(void)
{
  CLEAR_BIT(AFIO->EVCR, AFIO_EVCR_EVOE);
}

/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_GPIO_MODULE_ENABLED */

/**
  * @}
  */

/**
  * @}
  */

