/**
  ******************************************************************************
  * @file    stm32f1xx_hal_pwr.h
  * @author  MCD Application Team
  * @brief   HAL库 PWR（电源）模块头文件
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

/* 定义防止递归 -------------------------------------*/
#ifndef __STM32F1xx_HAL_PWR_H
#define __STM32F1xx_HAL_PWR_H

#ifdef __cplusplus
 extern "C" {
#endif

/* 头文件 ------------------------------------------------------------------*/
#include "stm32f1xx_hal_def.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @addtogroup PWR
  * @{
  */

/* 导出类型 ------------------------------------------------------------*/

/** @defgroup PWR_Exported_Types PWR 导出类型
  * @{
  */ 

/**
  * @brief  PWR PVD 配置结构体定义
  */
typedef struct
{
  uint32_t PVDLevel;   /*!< PVDLevel: 指定PVD检测级别
                            该参数可以取值 @ref PWR_PVD_detection_level */

  uint32_t Mode;      /*!< Mode: 指定所选引脚的操作模式
                           该参数可以取值 @ref PWR_PVD_Mode */
}PWR_PVDTypeDef;


/**
  * @}
  */


/* 内部常量 --------------------------------------------------------*/

/** @addtogroup PWR_Private_Constants
  * @{
  */ 

#define PWR_EXTI_LINE_PVD  ((uint32_t)0x00010000)  /*!< 外部中断线16连接PVD EXTI线 */

/**
  * @}
  */

 
/* 导出常量 --------------------------------------------------------*/

/** @defgroup PWR_Exported_Constants PWR 导出常量
  * @{
  */ 

/** @defgroup PWR_PVD_detection_level PWR PVD 检测级别
  * @{
  */
#define PWR_PVDLEVEL_0                  PWR_CR_PLS_2V2  /* PVD检测电平 2.2V */
#define PWR_PVDLEVEL_1                  PWR_CR_PLS_2V3  /* PVD检测电平 2.3V */
#define PWR_PVDLEVEL_2                  PWR_CR_PLS_2V4  /* PVD检测电平 2.4V */
#define PWR_PVDLEVEL_3                  PWR_CR_PLS_2V5  /* PVD检测电平 2.5V */
#define PWR_PVDLEVEL_4                  PWR_CR_PLS_2V6  /* PVD检测电平 2.6V */
#define PWR_PVDLEVEL_5                  PWR_CR_PLS_2V7  /* PVD检测电平 2.7V */
#define PWR_PVDLEVEL_6                  PWR_CR_PLS_2V8  /* PVD检测电平 2.8V */
#define PWR_PVDLEVEL_7                  PWR_CR_PLS_2V9  /* PVD检测电平 2.9V */
                                                          
/**
  * @}
  */

/** @defgroup PWR_PVD_Mode PWR PVD 模式
  * @{
  */
#define PWR_PVD_MODE_NORMAL                 0x00000000U   /*!< 基础模式 */
#define PWR_PVD_MODE_IT_RISING              0x00010001U   /*!< 上升沿触发检测的外部中断模式 */
#define PWR_PVD_MODE_IT_FALLING             0x00010002U   /*!< 下降沿触发检测的外部中断模式 */
#define PWR_PVD_MODE_IT_RISING_FALLING      0x00010003U   /*!< 双沿触发检测的外部中断模式 */
#define PWR_PVD_MODE_EVENT_RISING           0x00020001U   /*!< 上升沿触发检测的事件模式 */
#define PWR_PVD_MODE_EVENT_FALLING          0x00020002U   /*!< 下降沿触发检测的事件模式 */
#define PWR_PVD_MODE_EVENT_RISING_FALLING   0x00020003U   /*!< 双沿触发检测的事件模式 */

/**
  * @}
  */


/** @defgroup PWR_WakeUp_Pins PWR 唤醒引脚
  * @{
  */

#define PWR_WAKEUP_PIN1                 PWR_CSR_EWUP  /* 唤醒引脚1 */

/**
  * @}
  */

/** @defgroup PWR_Regulator_state_in_SLEEP_STOP_mode PWR 在SLEEP/STOP模式稳压器状态
  * @{
  */
#define PWR_MAINREGULATOR_ON                        0x00000000U   /* 主稳压器开启 */
#define PWR_LOWPOWERREGULATOR_ON                    PWR_CR_LPDS   /* 低功耗稳压器开启 */

/**
  * @}
  */

/** @defgroup PWR_SLEEP_mode_entry PWR SLEEP 进入睡眠模式
  * @{
  */
#define PWR_SLEEPENTRY_WFI              ((uint8_t)0x01)   /* 由中断唤醒的睡眠模式进入 */
#define PWR_SLEEPENTRY_WFE              ((uint8_t)0x02)   /* 由事件唤醒的睡眠模式进入 */

/**
  * @}
  */

/** @defgroup PWR_STOP_mode_entry PWR STOP 进入停止模式
  * @{
  */
#define PWR_STOPENTRY_WFI               ((uint8_t)0x01)   /* 由中断唤醒的停止模式进入 */
#define PWR_STOPENTRY_WFE               ((uint8_t)0x02)   /* 由事件唤醒的停止模式进入 */

/**
  * @}
  */

/** @defgroup PWR_Flag PWR 标志
  * @{
  */
#define PWR_FLAG_WU                     PWR_CSR_WUF     /* 唤醒标志 */
#define PWR_FLAG_SB                     PWR_CSR_SBF     /* 待机标志 */
#define PWR_FLAG_PVDO                   PWR_CSR_PVDO    /* PVD触发标志 */


/**
  * @}
  */

/**
  * @}
  */

/* 导出宏 ------------------------------------------------------------*/
/** @defgroup PWR_Exported_Macros PWR 导出宏
  * @{
  */

/** @brief  检查 PWR 标志是否置位
  * @param  __FLAG__: 指定要检查的标志.
  *           此参数是下列值之一:
  *            @arg PWR_FLAG_WU: 唤醒标志. 该标志表示从WKUP引脚或RTC闹钟接收到唤醒事件。
  *                  如果在 WKUP 引脚电平已经高的情况下启用 WKUP 引脚（通过设置 EWUP 位），则会检测到额外的唤醒事件。
  *            @arg PWR_FLAG_SB: 待机标志。该标志表示系统从待机模式恢复。
  *            @arg PWR_FLAG_PVDO: PVD 输出. 该标志仅在HAL_PWR_EnablePVD()函数启用PVD后有效。
  *                  因此，在待机或复位后，直到设置了 PVDE 位，此位将等于 0。
  * @retval __FLAG__ 状态 (TRUE or FALSE).
  */
#define __HAL_PWR_GET_FLAG(__FLAG__) ((PWR->CSR & (__FLAG__)) == (__FLAG__))

/** @brief  清除 PWR 挂起的标志
  * @param  __FLAG__: 指定要清除的标志
  *          此参数是下列值之一:
  *            @arg PWR_FLAG_WU: 唤醒标志
  *            @arg PWR_FLAG_SB: 待机标志
  */
#define __HAL_PWR_CLEAR_FLAG(__FLAG__) SET_BIT(PWR->CR, ((__FLAG__) << 2))

/**
  * @brief 开启 PVD EXTI 线16 中断
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_ENABLE_IT()      SET_BIT(EXTI->IMR, PWR_EXTI_LINE_PVD)

/**
  * @brief 关闭 PVD EXTI 线16 中断
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_DISABLE_IT()     CLEAR_BIT(EXTI->IMR, PWR_EXTI_LINE_PVD)

/**
  * @brief 开启  PVD EXTI 线16 事件
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_ENABLE_EVENT()   SET_BIT(EXTI->EMR, PWR_EXTI_LINE_PVD)

/**
  * @brief 关闭  PVD EXTI 线16 事件
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_DISABLE_EVENT()  CLEAR_BIT(EXTI->EMR, PWR_EXTI_LINE_PVD)


/**
  * @brief  PVD EXTI 中断线配置为下降沿触发
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_ENABLE_FALLING_EDGE()  SET_BIT(EXTI->FTSR, PWR_EXTI_LINE_PVD)


/**
  * @brief 清除 PVD 外部中断的下降沿触发
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_DISABLE_FALLING_EDGE()  CLEAR_BIT(EXTI->FTSR, PWR_EXTI_LINE_PVD)


/**
  * @brief  PVD EXTI 中断线配置为上升沿触发
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_ENABLE_RISING_EDGE()   SET_BIT(EXTI->RTSR, PWR_EXTI_LINE_PVD)

/**
  * @brief 清除 PVD 外部中断的上升沿触发
  * This parameter can be:
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_DISABLE_RISING_EDGE()  CLEAR_BIT(EXTI->RTSR, PWR_EXTI_LINE_PVD)

/**
  * @brief  PVD EXTI 中断配置为上升&下降沿 双沿触发
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_ENABLE_RISING_FALLING_EDGE()   __HAL_PWR_PVD_EXTI_ENABLE_RISING_EDGE();__HAL_PWR_PVD_EXTI_ENABLE_FALLING_EDGE();

/**
  * @brief 清除 PVD 外部中断的上升&下降沿 双沿触发
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_DISABLE_RISING_FALLING_EDGE()  __HAL_PWR_PVD_EXTI_DISABLE_RISING_EDGE();__HAL_PWR_PVD_EXTI_DISABLE_FALLING_EDGE();



/**
  * @brief  检查是否设置了指定的PVD EXTI中断标志。
  * @retval EXTI PVD 线状态
  */
#define __HAL_PWR_PVD_EXTI_GET_FLAG()       (EXTI->PR & (PWR_EXTI_LINE_PVD))

/**
  * @brief  清除 PVD EXTI 标志.
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_CLEAR_FLAG()     (EXTI->PR = (PWR_EXTI_LINE_PVD))

/**
  * @brief  在PVD EXTI线上生成软件中断。
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_GENERATE_SWIT()  SET_BIT(EXTI->SWIER, PWR_EXTI_LINE_PVD)
/**
  * @}
  */

/* 私有宏 -------------------------------------------------------------*/
/** @defgroup PWR_Private_Macros PWR 私有宏
  * @{
  */
#define IS_PWR_PVD_LEVEL(LEVEL) (((LEVEL) == PWR_PVDLEVEL_0) || ((LEVEL) == PWR_PVDLEVEL_1)|| \
                                 ((LEVEL) == PWR_PVDLEVEL_2) || ((LEVEL) == PWR_PVDLEVEL_3)|| \
                                 ((LEVEL) == PWR_PVDLEVEL_4) || ((LEVEL) == PWR_PVDLEVEL_5)|| \
                                 ((LEVEL) == PWR_PVDLEVEL_6) || ((LEVEL) == PWR_PVDLEVEL_7))


#define IS_PWR_PVD_MODE(MODE) (((MODE) == PWR_PVD_MODE_IT_RISING)|| ((MODE) == PWR_PVD_MODE_IT_FALLING) || \
                              ((MODE) == PWR_PVD_MODE_IT_RISING_FALLING) || ((MODE) == PWR_PVD_MODE_EVENT_RISING) || \
                              ((MODE) == PWR_PVD_MODE_EVENT_FALLING) || ((MODE) == PWR_PVD_MODE_EVENT_RISING_FALLING) || \
                              ((MODE) == PWR_PVD_MODE_NORMAL)) 

#define IS_PWR_WAKEUP_PIN(PIN) (((PIN) == PWR_WAKEUP_PIN1))

#define IS_PWR_REGULATOR(REGULATOR) (((REGULATOR) == PWR_MAINREGULATOR_ON) || \
                                     ((REGULATOR) == PWR_LOWPOWERREGULATOR_ON))

#define IS_PWR_SLEEP_ENTRY(ENTRY) (((ENTRY) == PWR_SLEEPENTRY_WFI) || ((ENTRY) == PWR_SLEEPENTRY_WFE))

#define IS_PWR_STOP_ENTRY(ENTRY) (((ENTRY) == PWR_STOPENTRY_WFI) || ((ENTRY) == PWR_STOPENTRY_WFE))

/**
  * @}
  */



/* 导出函数 --------------------------------------------------------*/

/** @addtogroup PWR_Exported_Functions PWR 导出函数
  * @{
  */
  
/** @addtogroup PWR_Exported_Functions_Group1 初始化和反初始化函数
  * @{
  */

/* 初始化和反初始化函数 *******************************/

void HAL_PWR_DeInit(void);
void HAL_PWR_EnableBkUpAccess(void);
void HAL_PWR_DisableBkUpAccess(void);

/**
  * @}
  */

/** @addtogroup PWR_Exported_Functions_Group2 外设控制函数
  * @{
  */

/* 外设控制函数  ************************************************/

void HAL_PWR_ConfigPVD(PWR_PVDTypeDef *sConfigPVD);

/* #define HAL_PWR_ConfigPVD 12*/

void HAL_PWR_EnablePVD(void);
void HAL_PWR_DisablePVD(void);

/* 唤醒引脚配置函数 ****************************************/

void HAL_PWR_EnableWakeUpPin(uint32_t WakeUpPinx);
void HAL_PWR_DisableWakeUpPin(uint32_t WakeUpPinx);

/* 低功耗模式配置函数 ************************************/

void HAL_PWR_EnterSTOPMode(uint32_t Regulator, uint8_t STOPEntry);
void HAL_PWR_EnterSLEEPMode(uint32_t Regulator, uint8_t SLEEPEntry);
void HAL_PWR_EnterSTANDBYMode(void);

void HAL_PWR_EnableSleepOnExit(void);
void HAL_PWR_DisableSleepOnExit(void);
void HAL_PWR_EnableSEVOnPend(void);
void HAL_PWR_DisableSEVOnPend(void);



void HAL_PWR_PVD_IRQHandler(void);
void HAL_PWR_PVDCallback(void);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif


#endif /* __STM32F1xx_HAL_PWR_H */

