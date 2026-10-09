/**
  ******************************************************************************
  * @file    stm32f1xx_hal_exti.c
  * @author  MCD Application Team
  * @brief   HAL库 EXTI 模块驱动
  *          本文件提供固件函数，用于管理扩展中断和事件控制器（EXTI）外设的以下功能：
  *           + 初始化和反初始化函数
  *           + IO 操作函数
  *
  ******************************************************************************
  * @attention
  *
  * 版权所有 (c) 2019 STMicroelectronics。
  * 保留所有权利。
  *
  * 本软件根据可以在此软件组件的根目录中找到的 LICENSE 文件中的条款进行许可。
  * 如果此软件没有附带 LICENSE 文件，则按原样提供。
  *
  ******************************************************************************
  @verbatim
  ==============================================================================
                    ##### EXTI 外设特性 #####
  ==============================================================================
  [..]
    (+) 此驱动程序可以配置每个EXTI线路

    (+) EXTI线路可以配置为3种不同的模式：
        (++) 中断模式
        (++) 事件模式
        (++) 中断和事件模式

    (+) 可配置的EXTI线路可以配置为3种不同的触发器：
        (++) 上升沿触发
        (++) 下降沿触发
        (++) 上升沿和下降沿都触发

    (+) 当设置为中断模式时，可配置的EXTI线路具有两个不同的中断挂起寄存器，可以区分发生的转换：
        (++) 上升沿挂起中断
        (++) 下降沿挂起中断

    (+) EXTI线路0到15与GPIO引脚号0到15相对应。GPIO端口可以通过复用器选择。

                     ##### 如何使用驱动 #####
  ==============================================================================
  [..]

    (#) 使用 HAL_EXTI_SetConfigLine() 配置EXTI线路。
        (++) 通过从 EXTI_ConfigTypeDef 结构设置"Line"成员来选择中断线路号。
        (++) 使用 EXTI_ConfigTypeDef 结构的"Mode"成员配置中断 和/或 事件模式。
        (++) 对于可配置的线路，使用 EXTI_ConfigTypeDef 结构的"Trigger"成员配置上升 和/或 下降触发器。
        (++) 对于与GPIO相关联的EXTI线路，使用 GPIO_InitTypeDef 结构的"GPIOSel"成员选择GPIO端口。

    (#) 使用 HAL_EXTI_GetConfigLine() 获取指定线路的当前EXTI配置。
        (++) 提供EXTI句柄作为第一个参数。
        (++) 将指向 EXTI_ConfigTypeDef 结构的指针作为第二个参数提供。

    (#) 使用 HAL_EXTI_ClearConfigLine() 清除指定线路的EXTI配置。
        (++) 提供EXTI句柄作为参数。

    (#) 使用 HAL_EXTI_RegisterCallback() 注册用户回调函数以处理EXTI中断。
        (++) 提供EXTI句柄作为第一个参数。
        (++) 使用 EXTI_CallbackIDTypeDef 之中的一个值指定注册哪个回调。
        (++) 提供回调函数指针。

    (#) 使用 HAL_EXTI_GetPending() 获取中断挂起位状态。

    (#) 使用 HAL_EXTI_ClearPending() 清除中断挂起位。

    (#) 使用 HAL_EXTI_GenerateSWI() 生成软件中断。

  @endverbatim
  */

/* 头文件 ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @addtogroup EXTI
  * @{
  */
/** MISRA C:2012偏差规则，适用于以下规则：
  * Rule-18.1_b - 中等: 数组"EXTICR"第一个下标区间[0,7]可能超出了[0,3]的范围，适用于以下API：
  * HAL_EXTI_SetConfigLine
  * HAL_EXTI_GetConfigLine
  * HAL_EXTI_ClearConfigLine
  */

#ifdef HAL_EXTI_MODULE_ENABLED

/* 私有类型 -----------------------------------------------------------*/
/* 私有定义 -----------------------------------------------------------*/
/** @defgroup EXTI_Private_Constants EXTI 私有常量
  * @{
  */

/**
  * @}
  */

/* 私有宏 ------------------------------------------------------------*/
/* 私有变量 ---------------------------------------------------------*/
/* 私有函数原型 -----------------------------------------------*/
/* 导出函数 --------------------------------------------------------*/

/** @addtogroup EXTI_Exported_Functions
  * @{
  */

/** @addtogroup EXTI_Exported_Functions_Group1
  *  @brief    配置函数
  *
@verbatim
 ===============================================================================
              ##### 配置函数 #####
 ===============================================================================

@endverbatim
  * @{
  */

/**
  * @brief  设置Exti线的配置
  * @param  hexti Exti 句柄
  * @param  pExtiConfig 要设置的EXTI配置结构体指针
  * @retval HAL Status.
  */
HAL_StatusTypeDef HAL_EXTI_SetConfigLine(EXTI_HandleTypeDef *hexti, EXTI_ConfigTypeDef *pExtiConfig)
{
  uint32_t regval;
  uint32_t linepos;
  uint32_t maskline;

  /* 检查空指针 */
  if ((hexti == NULL) || (pExtiConfig == NULL))
  {
    return HAL_ERROR;
  }

  /* 检查参数 */
  assert_param(IS_EXTI_LINE(pExtiConfig->Line));
  assert_param(IS_EXTI_MODE(pExtiConfig->Mode));

  /* 指定要处理的线号 */
  hexti->Line = pExtiConfig->Line;

  /* 计算线掩码 */
  linepos = (pExtiConfig->Line & EXTI_PIN_MASK);
  maskline = (1uL << linepos);

  /* 为配置线 设置触发器 */
  if ((pExtiConfig->Line & EXTI_CONFIG) != 0x00u)
  {
    assert_param(IS_EXTI_TRIGGER(pExtiConfig->Trigger));

    /* 配置上升沿触发 */
    if ((pExtiConfig->Trigger & EXTI_TRIGGER_RISING) != 0x00u)
    {
      EXTI->RTSR |= maskline;
    }
    else
    {
      EXTI->RTSR &= ~maskline;
    }

    /* 配置下降沿触发 */
    if ((pExtiConfig->Trigger & EXTI_TRIGGER_FALLING) != 0x00u)
    {
      EXTI->FTSR |= maskline;
    }
    else
    {
      EXTI->FTSR &= ~maskline;
    }


    /* 配置gpio端口选择gpio exti线 */
    if ((pExtiConfig->Line & EXTI_GPIO) == EXTI_GPIO)
    {
      assert_param(IS_EXTI_GPIO_PORT(pExtiConfig->GPIOSel));
      assert_param(IS_EXTI_GPIO_PIN(linepos));
      
      regval = AFIO->EXTICR[linepos >> 2u];
      regval &= ~(AFIO_EXTICR1_EXTI0 << (AFIO_EXTICR1_EXTI1_Pos * (linepos & 0x03u)));
      regval |= (pExtiConfig->GPIOSel << (AFIO_EXTICR1_EXTI1_Pos * (linepos & 0x03u)));
      AFIO->EXTICR[linepos >> 2u] = regval;
    }
  }

  /* 读取当前模式，配置中断模式 */
  if ((pExtiConfig->Mode & EXTI_MODE_INTERRUPT) != 0x00u)
  {
    EXTI->IMR |= maskline;
  }
  else
  {
    EXTI->IMR &= ~maskline;
  }

  /* 读取当前模式，配置事件模式 */
  if ((pExtiConfig->Mode & EXTI_MODE_EVENT) != 0x00u)
  {
    EXTI->EMR |= maskline;
  }
  else
  {
    EXTI->EMR &= ~maskline;
  }

  return HAL_OK;
}

/**
  * @brief  获取Exti线的配置
  * @param  hexti Exti 句柄
  * @param  pExtiConfig 存储Exti配置的结构体指针
  * @retval HAL Status.
  */
HAL_StatusTypeDef HAL_EXTI_GetConfigLine(EXTI_HandleTypeDef *hexti, EXTI_ConfigTypeDef *pExtiConfig)
{
  uint32_t regval;
  uint32_t linepos;
  uint32_t maskline;

  /* 检查空指针 */
  if ((hexti == NULL) || (pExtiConfig == NULL))
  {
    return HAL_ERROR;
  }

  /* 检查参数 */
  assert_param(IS_EXTI_LINE(hexti->Line));

  /* 将句柄行号存储到配置结构中 */
  pExtiConfig->Line = hexti->Line;

  /* 计算线偏移 */
  linepos = (pExtiConfig->Line & EXTI_PIN_MASK);
  maskline = (1uL << linepos);

  /* 1] 获取模式 : 中断 */

  /* 检查选中线是否开启中断模式 */
  if ((EXTI->IMR & maskline) != 0x00u)
  {
    pExtiConfig->Mode = EXTI_MODE_INTERRUPT;
  }
  else
  {
    pExtiConfig->Mode = EXTI_MODE_NONE;
  }

  /* 获取事件模式 */
  /* 检查选中先是否开启事件模式 */
  if ((EXTI->EMR & maskline) != 0x00u)
  {
    pExtiConfig->Mode |= EXTI_MODE_EVENT;
  }

  /* 获取默认触发和GPIO组配置 */
  pExtiConfig->Trigger = EXTI_TRIGGER_NONE;
  pExtiConfig->GPIOSel = 0x00u;

  /* 2] 获取选中线的上升沿触发配置 */
  if ((pExtiConfig->Line & EXTI_CONFIG) != 0x00u)
  {
    /* 检查所选线的配置是否使能 */
    if ((EXTI->RTSR & maskline) != 0x00u)
    {
      pExtiConfig->Trigger = EXTI_TRIGGER_RISING;
    }

    /* 获取下降沿配置 */
    /* 检查所选线的配置是否使能 */
    if ((EXTI->FTSR & maskline) != 0x00u)
    {
      pExtiConfig->Trigger |= EXTI_TRIGGER_FALLING;
    }

    /* 获取Gpio线的Gpio端口选择 */
    if ((pExtiConfig->Line & EXTI_GPIO) == EXTI_GPIO)
    {
      assert_param(IS_EXTI_GPIO_PIN(linepos));

      regval = AFIO->EXTICR[linepos >> 2u];
      pExtiConfig->GPIOSel = (regval >> (AFIO_EXTICR1_EXTI1_Pos * (linepos & 0x03u))) & AFIO_EXTICR1_EXTI0;
    }
  }

  return HAL_OK;
}

/**
  * @brief  清除Exti线的全部配置
  * @param  hexti Exti 句柄
  * @retval HAL Status.
  */
HAL_StatusTypeDef HAL_EXTI_ClearConfigLine(EXTI_HandleTypeDef *hexti)
{
  uint32_t regval;
  uint32_t linepos;
  uint32_t maskline;

  /* 检查空指针 */
  if (hexti == NULL)
  {
    return HAL_ERROR;
  }

  /* 检查参数 */
  assert_param(IS_EXTI_LINE(hexti->Line));

  /* 计算线偏移 */
  linepos = (hexti->Line & EXTI_PIN_MASK);
  maskline = (1uL << linepos);

  /* 1] 清除中断模式 */
  EXTI->IMR = (EXTI->IMR & ~maskline);

  /* 2] 清除事件模式 */
  EXTI->EMR = (EXTI->EMR & ~maskline);

  /* 3] 清除触发器 */
  if ((hexti->Line & EXTI_CONFIG) != 0x00u)
  {
    EXTI->RTSR = (EXTI->RTSR & ~maskline);
    EXTI->FTSR = (EXTI->FTSR & ~maskline);

    /* 获取Gpio线路的Gpio端口选择 */
    if ((hexti->Line & EXTI_GPIO) == EXTI_GPIO)
    {
      assert_param(IS_EXTI_GPIO_PIN(linepos));

      regval = AFIO->EXTICR[linepos >> 2u];
      regval &= ~(AFIO_EXTICR1_EXTI0 << (AFIO_EXTICR1_EXTI1_Pos * (linepos & 0x03u)));
      AFIO->EXTICR[linepos >> 2u] = regval;
    }
  }

  return HAL_OK;
}

/**
  * @brief  Exti 线注册用户回调
  * @param  hexti Exti 句柄
  * @param  CallbackID 用户回调标识符
  *         该参数可以是 @arg @ref EXTI_CallbackIDTypeDef 中的一个值.
  * @param  pPendingCbfn 回调函数指针
  * @retval HAL Status.
  */
HAL_StatusTypeDef HAL_EXTI_RegisterCallback(EXTI_HandleTypeDef *hexti, EXTI_CallbackIDTypeDef CallbackID, void (*pPendingCbfn)(void))
{
  HAL_StatusTypeDef status = HAL_OK;

  switch (CallbackID)
  {
    case  HAL_EXTI_COMMON_CB_ID:
      hexti->PendingCallback = pPendingCbfn;
      break;

    default:
      status = HAL_ERROR;
      break;
  }

  return status;
}

/**
  * @brief  将线号存储到句柄私有字段
  * @param  hexti Exti 句柄
  * @param  ExtiLine Exti 线号
  *         取值范围为0 ~ @ref EXTI_LINE_NB.
  * @retval HAL Status.
  */
HAL_StatusTypeDef HAL_EXTI_GetHandle(EXTI_HandleTypeDef *hexti, uint32_t ExtiLine)
{
  /* 检查参数 */
  assert_param(IS_EXTI_LINE(ExtiLine));

  /* 检查空指针 */
  if (hexti == NULL)
  {
    return HAL_ERROR;
  }
  else
  {
    /* 将线号存储到句柄私有字段 */
    hexti->Line = ExtiLine;

    return HAL_OK;
  }
}

/**
  * @}
  */

/** @addtogroup EXTI_Exported_Functions_Group2
  *  @brief EXTI IO 操作函数.
  *
@verbatim
 ===============================================================================
                       ##### IO 操作函数 #####
 ===============================================================================

@endverbatim
  * @{
  */

/**
  * @brief  处理 EXTI 中断请求.
  * @param  hexti Exti 句柄
  * @retval none.
  */
void HAL_EXTI_IRQHandler(EXTI_HandleTypeDef *hexti)
{
  uint32_t regval;
  uint32_t maskline;

  /* 计算线偏移 */
  maskline = (1uL << (hexti->Line & EXTI_PIN_MASK));

  /* 获取挂起位  */
  regval = (EXTI->PR & maskline);
  if (regval != 0x00u)
  {
    /* 清除挂起位 */
    EXTI->PR = maskline;

    /* 调用回调 */
    if (hexti->PendingCallback != NULL)
    {
      hexti->PendingCallback();
    }
  }
}

/**
  * @brief  获取EXTI线的中断挂起位
  * @param  hexti Exti 句柄
  * @param  Edge 指定要检查的挂起边沿
  *         该参数可以是以下值之一:
  *           @arg @ref EXTI_TRIGGER_RISING_FALLING
  *         该参数保留是为了与其他系列兼容。
  * @retval 挂起 1 未挂起 0
  */
uint32_t HAL_EXTI_GetPending(EXTI_HandleTypeDef *hexti, uint32_t Edge)
{
  uint32_t regval;
  uint32_t maskline;
  uint32_t linepos;

  /* 检查参数 */
  assert_param(IS_EXTI_LINE(hexti->Line));
  assert_param(IS_EXTI_CONFIG_LINE(hexti->Line));
  assert_param(IS_EXTI_PENDING_EDGE(Edge));

  /* 防止未使用参数的编译警告 */
  UNUSED(Edge);

  /* 计算线偏移 */
  linepos = (hexti->Line & EXTI_PIN_MASK);
  maskline = (1uL << linepos);

  /* 挂起 1 未挂起 0 */
  regval = ((EXTI->PR & maskline) >> linepos);
  return regval;
}

/**
  * @brief  清除EXTI线中断挂起位
  * @param  hexti Exti 句柄
  * @param  Edge 指定要清除的挂起边沿
  *         该参数可以是以下值之一:
  *           @arg @ref EXTI_TRIGGER_RISING_FALLING
  *         该参数保留是为了与其他系列兼容。
  * @retval None.
  */
void HAL_EXTI_ClearPending(EXTI_HandleTypeDef *hexti, uint32_t Edge)
{
  uint32_t maskline;

  /* 检查参数 */
  assert_param(IS_EXTI_LINE(hexti->Line));
  assert_param(IS_EXTI_CONFIG_LINE(hexti->Line));
  assert_param(IS_EXTI_PENDING_EDGE(Edge));

  /* 防止未使用参数编译警告 */
  UNUSED(Edge);

  /* 计算线偏移 */
  maskline = (1uL << (hexti->Line & EXTI_PIN_MASK));

  /* 清除挂起位 */
  EXTI->PR =  maskline;
}

/**
  * @brief  对指定EXTI线生成软件中断
  * @param  hexti Exti 句柄
  * @retval None.
  */
void HAL_EXTI_GenerateSWI(EXTI_HandleTypeDef *hexti)
{
  uint32_t maskline;

  /* 检查参数 */
  assert_param(IS_EXTI_LINE(hexti->Line));
  assert_param(IS_EXTI_CONFIG_LINE(hexti->Line));

  /* 计算线偏移 */
  maskline = (1uL << (hexti->Line & EXTI_PIN_MASK));

  /* 生成软件中断 */
  EXTI->SWIER = maskline;
}

/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_EXTI_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */


