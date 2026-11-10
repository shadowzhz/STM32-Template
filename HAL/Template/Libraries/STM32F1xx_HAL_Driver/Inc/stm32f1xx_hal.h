/**
  ******************************************************************************
  * @file    stm32f1xx_hal.h
  * @author  MCD Application Team
  * @brief   该文件包含HAL模块驱动程序的所有函数原型。
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
  */

/* 定义以防止递归 -------------------------------------*/
#ifndef __STM32F1xx_HAL_H
#define __STM32F1xx_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

/* 头文件 ------------------------------------------------------------------*/
#include "stm32f1xx_hal_conf.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @addtogroup HAL
  * @{
  */

/* 导出常量 --------------------------------------------------------*/

/** @defgroup HAL_Exported_Constants HAL 导出常量
  * @{
  */

/** @defgroup HAL_TICK_FREQ 滴答频率
  * @{
  */
typedef enum
{
  HAL_TICK_FREQ_10HZ         = 100U,                /* 10HZ */
  HAL_TICK_FREQ_100HZ        = 10U,                 /* 100HZ */
  HAL_TICK_FREQ_1KHZ         = 1U,                  /* 1000HZ */
  HAL_TICK_FREQ_DEFAULT      = HAL_TICK_FREQ_1KHZ   /* 1000HZ */
} HAL_TickFreqTypeDef;
/**
  * @}
  */
/* 导出类型 ------------------------------------------------------------*/
extern __IO uint32_t uwTick;
extern uint32_t uwTickPrio;
extern HAL_TickFreqTypeDef uwTickFreq;

/**
  * @}
  */
/* 导出宏 ------------------------------------------------------------*/
/** @defgroup HAL_Exported_Macros HAL 导出宏
  * @{
  */

/** @defgroup DBGMCU_Freeze_Unfreeze 在调试模式下冻结/解冻外设
  * @brief   在调试模式下冻结/解冻外设
  * 注意: 在设备 STM32F10xx8 和 STM32F10xxB,
  *                  STM32F101xC/D/E 和 STM32F103xC/D/E,
  *                  STM32F101xF/G 和 STM32F103xF/G
  *                  STM32F10xx4 和 STM32F10xx6
  *       调试寄存器DBGMCU_IDCODE和DBGMCU_CR只能在调试模式下访问(正常模式下用户软件无法访问)。
  *       请参阅这些设备的勘误表了解更多细节。
  * @{
  */

/* APB1 上的外设 */
/**
  * @brief  TIM2 外设调试模式
  */
#define __HAL_DBGMCU_FREEZE_TIM2()            SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM2_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM2()          CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM2_STOP)

/**
  * @brief  TIM3 外设调试模式
  */
#define __HAL_DBGMCU_FREEZE_TIM3()            SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM3_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM3()          CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM3_STOP)

#if defined (DBGMCU_CR_DBG_TIM4_STOP)
/**
  * @brief  TIM4 外设调试模式
  */
#define __HAL_DBGMCU_FREEZE_TIM4()            SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM4_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM4()          CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM4_STOP)
#endif

#if defined (DBGMCU_CR_DBG_TIM5_STOP)
/**
  * @brief  TIM5 外设调试模式
  */
#define __HAL_DBGMCU_FREEZE_TIM5()            SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM5_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM5()          CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM5_STOP)
#endif

#if defined (DBGMCU_CR_DBG_TIM6_STOP)
/**
  * @brief  TIM6 外设调试模式
  */
#define __HAL_DBGMCU_FREEZE_TIM6()            SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM6_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM6()          CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM6_STOP)
#endif

#if defined (DBGMCU_CR_DBG_TIM7_STOP)
/**
  * @brief  TIM7 外设调试模式
  */
#define __HAL_DBGMCU_FREEZE_TIM7()            SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM7_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM7()          CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM7_STOP)
#endif

#if defined (DBGMCU_CR_DBG_TIM12_STOP)
/**
  * @brief  TIM12 外设调试模式
  */
#define __HAL_DBGMCU_FREEZE_TIM12()            SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM12_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM12()          CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM12_STOP)
#endif

#if defined (DBGMCU_CR_DBG_TIM13_STOP)
/**
  * @brief  TIM13 外设调试模式
  */
#define __HAL_DBGMCU_FREEZE_TIM13()            SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM13_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM13()          CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM13_STOP)
#endif

#if defined (DBGMCU_CR_DBG_TIM14_STOP)
/**
  * @brief  TIM14 外设调试模式
  */
#define __HAL_DBGMCU_FREEZE_TIM14()            SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM14_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM14()          CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM14_STOP)
#endif

/**
  * @brief  WWDG 外设调试模式
  */
#define __HAL_DBGMCU_FREEZE_WWDG()            SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_WWDG_STOP)
#define __HAL_DBGMCU_UNFREEZE_WWDG()          CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_WWDG_STOP)

/**
  * @brief  IWDG 外设调试模式
  */
#define __HAL_DBGMCU_FREEZE_IWDG()            SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_IWDG_STOP)
#define __HAL_DBGMCU_UNFREEZE_IWDG()          CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_IWDG_STOP)

/**
  * @brief  I2C1 外设调试模式
  */
#define __HAL_DBGMCU_FREEZE_I2C1_TIMEOUT()    SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_I2C1_SMBUS_TIMEOUT)
#define __HAL_DBGMCU_UNFREEZE_I2C1_TIMEOUT()  CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_I2C1_SMBUS_TIMEOUT)

#if defined (DBGMCU_CR_DBG_I2C2_SMBUS_TIMEOUT)
/**
  * @brief  I2C2 外设调试模式
  */
#define __HAL_DBGMCU_FREEZE_I2C2_TIMEOUT()    SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_I2C2_SMBUS_TIMEOUT)
#define __HAL_DBGMCU_UNFREEZE_I2C2_TIMEOUT()  CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_I2C2_SMBUS_TIMEOUT)
#endif

#if defined (DBGMCU_CR_DBG_CAN1_STOP)
/**
  * @brief  CAN1 外设调试模式
  */
#define __HAL_DBGMCU_FREEZE_CAN1()            SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_CAN1_STOP)
#define __HAL_DBGMCU_UNFREEZE_CAN1()          CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_CAN1_STOP)
#endif

#if defined (DBGMCU_CR_DBG_CAN2_STOP)
/**
  * @brief  CAN2 外设调试模式
  */
#define __HAL_DBGMCU_FREEZE_CAN2()            SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_CAN2_STOP)
#define __HAL_DBGMCU_UNFREEZE_CAN2()          CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_CAN2_STOP)
#endif

/* APB2 上的外设 */
#if defined (DBGMCU_CR_DBG_TIM1_STOP)
/**
  * @brief  TIM1 外设调试模式
  */
#define __HAL_DBGMCU_FREEZE_TIM1()            SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM1_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM1()          CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM1_STOP)
#endif

#if defined (DBGMCU_CR_DBG_TIM8_STOP)
/**
  * @brief  TIM8 外设调试模式
  */
#define __HAL_DBGMCU_FREEZE_TIM8()            SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM8_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM8()          CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM8_STOP)
#endif

#if defined (DBGMCU_CR_DBG_TIM9_STOP)
/**
  * @brief  TIM9 外设调试模式
  */
#define __HAL_DBGMCU_FREEZE_TIM9()            SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM9_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM9()          CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM9_STOP)
#endif

#if defined (DBGMCU_CR_DBG_TIM10_STOP)
/**
  * @brief  TIM10 外设调试模式
  */
#define __HAL_DBGMCU_FREEZE_TIM10()            SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM10_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM10()          CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM10_STOP)
#endif

#if defined (DBGMCU_CR_DBG_TIM11_STOP)
/**
  * @brief  TIM11 外设调试模式
  */
#define __HAL_DBGMCU_FREEZE_TIM11()            SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM11_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM11()          CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM11_STOP)
#endif


#if defined (DBGMCU_CR_DBG_TIM15_STOP)
/**
  * @brief  TIM15 外设调试模式
  */
#define __HAL_DBGMCU_FREEZE_TIM15()            SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM15_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM15()          CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM15_STOP)
#endif

#if defined (DBGMCU_CR_DBG_TIM16_STOP)
/**
  * @brief  TIM16 外设调试模式
  */
#define __HAL_DBGMCU_FREEZE_TIM16()            SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM16_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM16()          CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM16_STOP)
#endif

#if defined (DBGMCU_CR_DBG_TIM17_STOP)
/**
  * @brief  TIM17 外设调试模式
  */
#define __HAL_DBGMCU_FREEZE_TIM17()            SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM17_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM17()          CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_TIM17_STOP)
#endif

/**
  * @}
  */

/** @defgroup HAL_Private_Macros HAL 私有宏
  * @{
  */
#define IS_TICKFREQ(FREQ) (((FREQ) == HAL_TICK_FREQ_10HZ)  || \
                           ((FREQ) == HAL_TICK_FREQ_100HZ) || \
                           ((FREQ) == HAL_TICK_FREQ_1KHZ))
/**
  * @}
  */

/* 导出函数 --------------------------------------------------------*/
/** @addtogroup HAL_Exported_Functions
  * @{
  */
/** @addtogroup HAL_Exported_Functions_Group1
  * @{
  */
/* 初始化和反初始化函数  ******************************/

/**
  * @brief  初始化 HAL 库；该函数必须在 main() 函数开头最先被调用
  * @note   主要执行以下底层配置：
  *         1. 使能 Flash 预取缓冲区（若芯片支持）。
  *         2. 配置 NVIC 优先级分组为 Group 4（4位抢占优先级，0位响应优先级）。
  *         3. 初始化 SysTick 时基定时器为 1ms 中断（此时主频仍为复位默认的 HSI 8MHz）。
  *         4. 调用弱定义的 HAL_MspInit() 进行用户级全局硬件底层初始化。
  * @warning 【调用顺序铁律】
  *          必须最先调用！切勿在调用本函数之前配置系统时钟（SystemClock_Config）或初始化任何外设。
  * @retval HAL 状态（HAL_OK / HAL_ERROR）
  */
HAL_StatusTypeDef HAL_Init(void);

HAL_StatusTypeDef HAL_DeInit(void);
void HAL_MspInit(void);
void HAL_MspDeInit(void);

/**
  * @brief  初始化 SysTick 滴答时基源
  * @param  TickPriority: SysTick 中断优先级（取值范围 0~15）
  * @note   配置 SysTick 定时器以 1ms 的固定周期触发中断，递增全局计数器 uwTick。
  *         在 HAL_Init() 及 HAL_RCC_ClockConfig() 更新系统主频后会自动调用此函数重新计算重装载值。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority);
/**
  * @}
  */

/** @addtogroup HAL_Exported_Functions_Group2
  * @{
  */
/* 外设控制函数  ************************************************/

void HAL_IncTick(void);

/**
  * @brief  毫秒级阻塞延时函数
  * @param  Delay: 延时时长（单位：毫秒）
  * @note   延时基准基于 SysTick 中断每 1ms 递增的全局变量 uwTick。
  * @warning 【死锁陷阱 / 必看避坑】
  *          SysTick 中断优先级由 TICK_INT_PRIORITY（工程模板中固定设为 15，即最低优先级）决定。
  *          【严禁】在任何中断服务函数（ISR）或优先级高于/等于 SysTick 的中断回调中直接调用 HAL_Delay()！
  *          否则高优先级中断会阻断 SysTick 中断的执行，导致 uwTick 永远不会递增，MCU 将陷入永久死锁！
  *          中断内部如需计时，请采用非阻塞状态机、记录 HAL_GetTick() 差值，或使用 DWT / 定时器忙等待。
  * @retval None
  */
void HAL_Delay(uint32_t Delay);

/**
  * @brief  获取自系统启动以来的毫秒级 Tick 计数
  * @note   uwTick 为 32 位无符号整型（uint32_t），每 1ms 递增 1。
  *         连续运行约 49.7 天后计数器会溢出回绕至 0。
  *         判断非阻塞超时请统一使用无符号相减形式：
  *         @code
  *         if ((HAL_GetTick() - tickstart) >= TIMEOUT_MS) { ... }
  *         @endcode
  *         利用 C 语言无符号数溢出特性，此写法在 uwTick 溢出前后均能保证计算结果完全正确！
  * @retval 当前已流逝的毫秒数 (uwTick)
  */
uint32_t HAL_GetTick(void);

uint32_t HAL_GetTickPrio(void);
HAL_StatusTypeDef HAL_SetTickFreq(HAL_TickFreqTypeDef Freq);
HAL_TickFreqTypeDef HAL_GetTickFreq(void);
void HAL_SuspendTick(void);
void HAL_ResumeTick(void);
uint32_t HAL_GetHalVersion(void);
uint32_t HAL_GetREVID(void);
uint32_t HAL_GetDEVID(void);
uint32_t HAL_GetUIDw0(void);
uint32_t HAL_GetUIDw1(void);
uint32_t HAL_GetUIDw2(void);
void HAL_DBGMCU_EnableDBGSleepMode(void);
void HAL_DBGMCU_DisableDBGSleepMode(void);
void HAL_DBGMCU_EnableDBGStopMode(void);
void HAL_DBGMCU_DisableDBGStopMode(void);
void HAL_DBGMCU_EnableDBGStandbyMode(void);
void HAL_DBGMCU_DisableDBGStandbyMode(void);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
/* 私有类型 -------------------------------------------------------------*/
/* 私有变量 ---------------------------------------------------------*/
/** @defgroup HAL_Private_Variables HAL 私有变量
  * @{
  */
/**
  * @}
  */
/* 私有常量 ---------------------------------------------------------*/
/** @defgroup HAL_Private_Constants HAL 私有常量
  * @{
  */
/**
  * @}
  */
/* 私有宏 ------------------------------------------------------------*/
/* 私有函数 ---------------------------------------------------------*/
/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F1xx_HAL_H */


