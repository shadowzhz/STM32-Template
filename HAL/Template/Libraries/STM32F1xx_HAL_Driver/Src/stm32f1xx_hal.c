/**
  ******************************************************************************
  * @file    stm32f1xx_hal.c
  * @author  MCD Application Team
  * @brief   HAL库 模块驱动
  *          这是HAL库初始化的公共部分
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
                     ##### 如何使用驱动 #####
  ==============================================================================
    [..]
    公共HAL驱动程序包含一组公共的api, PPP外设驱动程序和用户可以使用这些api来开始使用HAL。
    [..]
    HAL包含两个api类别:
         (+) 公共 HAL APIs
         (+) 服务 HAL APIs

  @endverbatim
  ******************************************************************************
  */

/* 头文件 ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @defgroup HAL HAL
  * @brief HAL 模块驱动
  * @{
  */

#ifdef HAL_MODULE_ENABLED

/* 私有类型 -----------------------------------------------------------*/
/* 私有定义 ------------------------------------------------------------*/

/** @defgroup HAL_Private_Constants HAL 私有常量
  * @{
  */
/**
 * @brief STM32F1xx HAL 驱动版本号
   */
#define __STM32F1xx_HAL_VERSION_MAIN   (0x01U) /*!< [31:24] 主版本号 */
#define __STM32F1xx_HAL_VERSION_SUB1   (0x01U) /*!< [23:16] 子版本号1 */
#define __STM32F1xx_HAL_VERSION_SUB2   (0x09U) /*!< [15:8]  子版本号2 */
#define __STM32F1xx_HAL_VERSION_RC     (0x00U) /*!< [7:0]  待发布版本 */
#define __STM32F1xx_HAL_VERSION         ((__STM32F1xx_HAL_VERSION_MAIN << 24)\
                                        |(__STM32F1xx_HAL_VERSION_SUB1 << 16)\
                                        |(__STM32F1xx_HAL_VERSION_SUB2 << 8 )\
                                        |(__STM32F1xx_HAL_VERSION_RC))

#define IDCODE_DEVID_MASK    0x00000FFFU

/**
  * @}
  */

/* 私有宏 -------------------------------------------------------------*/
/* 私有变量 ---------------------------------------------------------*/

/** @defgroup HAL_Private_Variables HAL 私有变量
  * @{
  */
__IO uint32_t uwTick;
uint32_t uwTickPrio   = (1UL << __NVIC_PRIO_BITS); /* 无效的 PRIO */
HAL_TickFreqTypeDef uwTickFreq = HAL_TICK_FREQ_DEFAULT;  /* 1KHz */
/**
  * @}
  */
/* 私有函数原型 -----------------------------------------------*/
/* 导出函数 ---------------------------------------------------------*/

/** @defgroup HAL_Exported_Functions HAL 导出函数
  * @{
  */

/** @defgroup HAL_Exported_Functions_Group1 初始化和反初始化函数
 *  @brief    初始化和反初始化函数
 *
@verbatim
 ===============================================================================
              ##### 初始化和反初始化函数 #####
 ===============================================================================
   [..]  本节提供的函数允许:
      (+) 初始化闪存接口、NVIC分配和初始时钟配置。
          如果需要超时功能，则还会初始化systick，如果启用，则初始化备份域。
      (+) 反初始化HAL的公共部分。
      (+) 配置时间基准源，使其具有专用的Tick中断优先级，以便每1毫秒产生一次中断。
        (++) 默认情况下，SysTick定时器用作时间基准源，但用户可以自己实现其自己的时间基准源（例如通用定时器或其他时间源），
             但需要注意的是，时间基准持续时间应保持为1毫秒，因为PPP_TIMEOUT_VALUE是以毫秒为单位定义和处理的。
        (++) 时间基准配置函数（HAL_InitTick()）在程序重置后由HAL_Init()自动调用或在任何时候在时钟配置时由HAL_RCC_ClockConfig()调用。
        (++) 时间基准源被配置为定期产生中断。如果在外设中断过程中调用HAL_Delay（），
             则必须注意Tick中断线必须具有比外设中断更高的优先级（数字更低）。否则，调用者中断过程将被阻塞。
       (++)  为了在用户文件中进行其他实现时能够进行覆盖，影响时间基准配置的函数被声明为__weak（弱引用）。
@endverbatim
  * @{
  */

/**
  * @brief  初始化HAL库
  * @note   它必须是主程序中执行的第一条指令（在调用任何其他HAL函数之前），它执行以下操作：
  *           配置Flash预取。
  *           配置SysTick以每1毫秒产生一次中断，其由HSI时钟（在此阶段，时钟尚未配置，因此系统正在从内部HSI运行，频率为16 MHz）时钟。
  *           将NVIC优先级组设置为4。
  *           调用“stm32f1xx_hal_msp.c”用户文件中定义的HAL_MspInit()回调函数进行全局底层硬件初始化。
  *
  * @note   SysTick用作HAL_Delay()函数的时间基准，
  *         应用程序需要确保SysTick时间基准始终设置为1毫秒，以确保HAL正确运行。
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_Init(void)
{
  /* 配置Flash预取 */
#if (PREFETCH_ENABLE != 0)
#if defined(STM32F101x6) || defined(STM32F101xB) || defined(STM32F101xE) || defined(STM32F101xG) || \
    defined(STM32F102x6) || defined(STM32F102xB) || \
    defined(STM32F103x6) || defined(STM32F103xB) || defined(STM32F103xE) || defined(STM32F103xG) || \
    defined(STM32F105xC) || defined(STM32F107xC)

  /* 有些设备上不支持预取缓冲区 */
  __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
#endif
#endif /* PREFETCH_ENABLE */

  /* 设置中断优先级组 */
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* 将SysTick用作时间基准源，并配置为1毫秒的滴答（复位后的默认时钟为HSI） */
  HAL_InitTick(TICK_INT_PRIORITY);

  /* 初始化底层硬件 */
  HAL_MspInit();

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief 反初始化HAL的公共部分并停止SysTick作为时间基准
  * @note  此函数是可选的
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DeInit(void)
{
  /* 重置全部外设 */
  __HAL_RCC_APB1_FORCE_RESET();
  __HAL_RCC_APB1_RELEASE_RESET();

  __HAL_RCC_APB2_FORCE_RESET();
  __HAL_RCC_APB2_RELEASE_RESET();

#if defined(STM32F105xC) || defined(STM32F107xC)
  __HAL_RCC_AHB_FORCE_RESET();
  __HAL_RCC_AHB_RELEASE_RESET();
#endif

  /* 反初始化底层硬件 */
  HAL_MspDeInit();

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @brief  初始化 MSP.
  * @retval None
  */
__weak void HAL_MspInit(void)
{
  /* NOTE : 当需要回调函数时，不应修改此函数，可以在用户文件中重写以覆盖此函数。
   */
}

/**
  * @brief  反初始化 MSP.
  * @retval None
  */
__weak void HAL_MspDeInit(void)
{
  /* NOTE : 当需要回调函数时，不应修改此函数，可以在用户文件中重写以覆盖此函数。
   */
}

/**
  * @brief 配置时间基准的源：
  *        时间源被配置为具有1毫秒时间基准和专用的Tick中断优先级。
  * @note  此函数在程序开始时由HAL_Init()自动调用，或者在通过HAL_RCC_ClockConfig()重新配置时调用。
  * @note  在默认实现中，SysTick定时器是时间基准的源。
  *        它用于定期生成中断。
  *        如果从外设的ISR过程中调用HAL_Delay()，必须注意SysTick中断的优先级必须高于外设中断（数字较低）。
  *        否则，调用者的ISR过程将被阻塞。 此函数声明为__weak，以便在用户文件中根据其他实现进行重写。
  * @param TickPriority Tick中断优先级。
  * @retval HAL status
  */
__weak HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
  /* 配置SysTick以在1毫秒的时间基准上产生中断。*/
  if (HAL_SYSTICK_Config(SystemCoreClock / (1000U / uwTickFreq)) > 0U)
  {
    return HAL_ERROR;
  }

  /* 配置 SysTick 中断优先级 */
  if (TickPriority < (1UL << __NVIC_PRIO_BITS))
  {
    HAL_NVIC_SetPriority(SysTick_IRQn, TickPriority, 0U);
    uwTickPrio = TickPriority;
  }
  else
  {
    return HAL_ERROR;
  }

  /* 返回函数状态 */
  return HAL_OK;
}

/**
  * @}
  */

/** @defgroup HAL_Exported_Functions_Group2 HAL 控制函数
  *  @brief    HAL 控制函数
  *
@verbatim
 ===============================================================================
                      ##### HAL 控制函数 #####
 ===============================================================================
    [..]  此部分提供了以下功能的函数：
      (+) 提供以毫秒为单位的滴答值
      (+) 提供以毫秒为单位的阻塞延迟
      (+) 暂停时间基准源中断
      (+) 恢复时间基准源中断
      (+) 获取HAL API驱动程序版本
      (+) 获取设备标识符
      (+) 获取设备修订标识符
      (+) 启用/禁用 在睡眠模式中使用调试模块
      (+) 启用/禁用 在停止模式中使用调试模块
      (+) 启用/禁用 在待机模式中使用调试模块

@endverbatim
  * @{
  */

/**
  * @brief 用于全局变量"uwTick"增加，作为应用程序的时间基准。
  * @note  在默认实现中，该变量在SysTick中断服务例程中每1毫秒递增一次。
  * @note  此函数被声明为__weak，以便在用户文件中有其他实现时进行覆盖。
  * @retval None
  */
__weak void HAL_IncTick(void)
{
  uwTick += uwTickFreq;
}

/**
  * @brief 获取以毫秒为单位的滴答值。
  * @note  此函数被声明为__weak，以便在用户文件中有其他实现时进行覆盖。
  * @retval tick value
  */
__weak uint32_t HAL_GetTick(void)
{
  return uwTick;
}

/**
  * @brief  返回滴答优先级
  * @retval 滴答优先级
  */
uint32_t HAL_GetTickPrio(void)
{
  return uwTickPrio;
}

/**
  * @brief 设置新的滴答频率
  * @retval status
  */
HAL_StatusTypeDef HAL_SetTickFreq(HAL_TickFreqTypeDef Freq)
{
  HAL_StatusTypeDef status  = HAL_OK;
  HAL_TickFreqTypeDef prevTickFreq;

  assert_param(IS_TICKFREQ(Freq));

  if (uwTickFreq != Freq)
  {
    /* 备份uwTickFreq频率 */
    prevTickFreq = uwTickFreq;

    /* 更新HAL_InitTick()使用的全局变量uwTickFreq */
    uwTickFreq = Freq;

    /* 应用新的滴答频率  */
    status = HAL_InitTick(uwTickPrio);

    if (status != HAL_OK)
    {
      /* 恢复先前的Tick频率 */
      uwTickFreq = prevTickFreq;
    }
  }

  return status;
}

/**
  * @brief 返回滴答频率
  * @retval 滴答频率
  *         滴答周期的频率（单位为Hz）
  */
HAL_TickFreqTypeDef HAL_GetTickFreq(void)
{
  return uwTickFreq;
}

/**
  * @brief 根据递增的变量提供最小延迟（以毫秒为单位）
  * @note  在默认实现中，SysTick定时器是时间基准的来源。它用于在固定时间间隔内生成中断，从而递增uwTick。
  * @note  此函数被声明为__weak，在用户文件中有其他实现时可以被覆盖。
  * @param Delay 指定延迟时间长度，单位为毫秒。
  * @retval None
  */
__weak void HAL_Delay(uint32_t Delay)
{
  uint32_t tickstart = HAL_GetTick();
  uint32_t wait = Delay;

  /* 添加一个频率以确保最小等待时间 */
  if (wait < HAL_MAX_DELAY)
  {
    wait += (uint32_t)(uwTickFreq);
  }

  while ((HAL_GetTick() - tickstart) < wait)
  {
  }
}

/**
  * @brief 暂停时间基准源中断
  * @note  在默认实现中，SysTick定时器是时间基准的来源。它用于在固定的时间间隔内生成中断。
  *        一旦调用了HAL_SuspendTick()函数，SysTick中断将被禁用，因此滴答值的增加将被暂停。
  * @note  此函数被声明为__weak，在用户文件中有其他实现时可以被覆盖。
  * @retval None
  */
__weak void HAL_SuspendTick(void)
{
  /* 关闭系统滴答中断 */
  CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);
}

/**
  * @brief 恢复时间基准源中断
  * @note  在默认实现中，SysTick定时器是时间基准的来源。它用于在固定的时间间隔内生成中断。
  *        一旦调用了HAL_ResumeTick()函数，SysTick中断将被启用，因此滴答值的增加将被恢复。
  * @note  此函数被声明为__weak，在用户文件中有其他实现时可以被覆盖。
  * @retval None
  */
__weak void HAL_ResumeTick(void)
{
  /* 开启系统滴答中断 */
  SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);
}

/**
  * @brief  返回HAL版本
  * @retval 版本 0xXYZR (每个十进制数有8位，R代表RC)
  */
uint32_t HAL_GetHalVersion(void)
{
  return __STM32F1xx_HAL_VERSION;
}

/**
  * @brief 返回设备的修订标识符。
  * @note  在设备 STM32F10xx8 和 STM32F10xxB,
  *                  STM32F101xC/D/E 和 STM32F103xC/D/E,
  *                  STM32F101xF/G 和 STM32F103xF/G
  *                  STM32F10xx4 和 STM32F10xx6
  *        调试寄存器DBGMCU_IDCODE和DBGMCU_CR仅在调试模式下可访问（在正常模式下用户软件无法访问）。
  *        有关更多详细信息，请参阅这些设备的勘误表。
  * @retval 设备修订标识符
  */
uint32_t HAL_GetREVID(void)
{
  return ((DBGMCU->IDCODE) >> DBGMCU_IDCODE_REV_ID_Pos);
}

/**
  * @brief 返回设备标识符
  * @note  在设备 STM32F10xx8 和 STM32F10xxB,
  *                  STM32F101xC/D/E 和 STM32F103xC/D/E,
  *                  STM32F101xF/G 和 STM32F103xF/G
  *                  STM32F10xx4 和 STM32F10xx6
  *       调试寄存器DBGMCU_IDCODE和DBGMCU_CR仅在调试模式下可访问（在正常模式下用户软件无法访问）。
  *       有关更多详细信息，请参阅这些设备的勘误表。
  * @retval 设备标识符
  */
uint32_t HAL_GetDEVID(void)
{
  return ((DBGMCU->IDCODE) & IDCODE_DEVID_MASK);
}

/**
  * @brief  返回唯一设备标识符（基于96位的UID）的第一个字。
  * @retval 设备标识符
  */
uint32_t HAL_GetUIDw0(void)
{
   return(READ_REG(*((uint32_t *)UID_BASE)));
}

/**
  * @brief  返回唯一设备标识符（基于96位的UID）的第二个字。
  * @retval 设备标识符
  */
uint32_t HAL_GetUIDw1(void)
{
   return(READ_REG(*((uint32_t *)(UID_BASE + 4U))));
}

/**
  * @brief  返回唯一设备标识符（基于96位的UID）的第三个字。
  * @retval 设备标识符
  */
uint32_t HAL_GetUIDw2(void)
{
   return(READ_REG(*((uint32_t *)(UID_BASE + 8U))));
}

/**
  * @brief 在睡眠模式中 启用 调试模块
  * @note  在设备 STM32F10xx8 和 STM32F10xxB,
  *                  STM32F101xC/D/E 和 STM32F103xC/D/E,
  *                  STM32F101xF/G 和 STM32F103xF/G
  *                  STM32F10xx4 和 STM32F10xx6
  *       调试寄存器DBGMCU_IDCODE和DBGMCU_CR仅在调试模式下可访问（在正常模式下用户软件无法访问）。
  *       有关更多详细信息，请参阅这些设备的勘误表。
  * @retval None
  */
void HAL_DBGMCU_EnableDBGSleepMode(void)
{
  SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_SLEEP);
}

/**
  * @brief 在睡眠模式中 禁用 调试模块
  * @note  在设备 STM32F10xx8 和 STM32F10xxB,
  *                  STM32F101xC/D/E 和 STM32F103xC/D/E,
  *                  STM32F101xF/G 和 STM32F103xF/G
  *                  STM32F10xx4 和 STM32F10xx6
  *       调试寄存器DBGMCU_IDCODE和DBGMCU_CR仅在调试模式下可访问（在正常模式下用户软件无法访问）。
  *       有关更多详细信息，请参阅这些设备的勘误表。
  * @retval None
  */
void HAL_DBGMCU_DisableDBGSleepMode(void)
{
  CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_SLEEP);
}

/**
  * @brief 在停止模式中 启用 调试模块
  * @note  在设备 STM32F10xx8 和 STM32F10xxB,
  *                  STM32F101xC/D/E 和 STM32F103xC/D/E,
  *                  STM32F101xF/G 和 STM32F103xF/G
  *                  STM32F10xx4 和 STM32F10xx6
  *       调试寄存器DBGMCU_IDCODE和DBGMCU_CR仅在调试模式下可访问（在正常模式下用户软件无法访问）。
  *       有关更多详细信息，请参阅这些设备的勘误表。
  * @note 对于所有STM32F1设备：
  *       如果在Stop模式调试期间启用系统时钟定时器中断（在DBGMCU_CR寄存器中设置DBG_STOP位），它将唤醒系统退出Stop模式。
  *       解决方法：要调试Stop模式，请禁用系统时钟定时器中断。
  *       有关更多详细信息，请参阅这些设备的勘误表。
  * @retval None
  */
void HAL_DBGMCU_EnableDBGStopMode(void)
{
  SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_STOP);
}

/**
  * @brief 在停止模式中 禁用 调试模块
  * @note  在设备 STM32F10xx8 和 STM32F10xxB,
  *                  STM32F101xC/D/E 和 STM32F103xC/D/E,
  *                  STM32F101xF/G 和 STM32F103xF/G
  *                  STM32F10xx4 和 STM32F10xx6
  *       调试寄存器DBGMCU_IDCODE和DBGMCU_CR仅在调试模式下可访问（在正常模式下用户软件无法访问）。
  *       有关更多详细信息，请参阅这些设备的勘误表。
  * @retval None
  */
void HAL_DBGMCU_DisableDBGStopMode(void)
{
  CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_STOP);
}

/**
  * @brief 在待机模式中 启用 调试模块
  * @note  在设备 STM32F10xx8 和 STM32F10xxB,
  *                  STM32F101xC/D/E 和 STM32F103xC/D/E,
  *                  STM32F101xF/G 和 STM32F103xF/G
  *                  STM32F10xx4 和 STM32F10xx6
  *       调试寄存器DBGMCU_IDCODE和DBGMCU_CR仅在调试模式下可访问（在正常模式下用户软件无法访问）。
  *       有关更多详细信息，请参阅这些设备的勘误表。
  * @retval None
  */
void HAL_DBGMCU_EnableDBGStandbyMode(void)
{
  SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_STANDBY);
}

/**
  * @brief 在待机模式中 禁用 调试模块
  * @note  在设备 STM32F10xx8 和 STM32F10xxB,
  *                  STM32F101xC/D/E 和 STM32F103xC/D/E,
  *                  STM32F101xF/G 和 STM32F103xF/G
  *                  STM32F10xx4 和 STM32F10xx6
  *       调试寄存器DBGMCU_IDCODE和DBGMCU_CR仅在调试模式下可访问（在正常模式下用户软件无法访问）。
  *       有关更多详细信息，请参阅这些设备的勘误表。
  * @retval None
  */
void HAL_DBGMCU_DisableDBGStandbyMode(void)
{
  CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_STANDBY);
}

/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */


  