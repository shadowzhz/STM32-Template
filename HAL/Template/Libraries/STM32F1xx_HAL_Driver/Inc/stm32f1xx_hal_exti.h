/**
  ******************************************************************************
  * @file    stm32f1xx_hal_exti.h
  * @author  MCD Application Team
  * @brief   HAL库 EXTI（外部中断线）模块头文件
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
#ifndef STM32F1xx_HAL_EXTI_H
#define STM32F1xx_HAL_EXTI_H

#ifdef __cplusplus
extern "C" {
#endif

/* 头文件 ------------------------------------------------------------------*/
#include "stm32f1xx_hal_def.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @defgroup EXTI EXTI
  * @brief EXTI HAL 模块驱动
  * @{
  */

/* 导出类型 ------------------------------------------------------------*/

/** @defgroup EXTI_Exported_Types EXTI 导出类型
  * @{
  */

/**
  * @brief  HAL EXTI EXTI通用回调ID枚举定义
  */
typedef enum
{
  HAL_EXTI_COMMON_CB_ID          = 0x00U
} EXTI_CallbackIDTypeDef;

/**
  * @brief  EXTI 句柄结构体定义
  */
typedef struct
{
  uint32_t Line;                    /*!<  Exti 线编号 */
  void (* PendingCallback)(void);   /*!<  Exti 挂起回调 */
} EXTI_HandleTypeDef;

/**
  * @brief  EXTI 配置结构体定义
  */
typedef struct
{
  uint32_t Line;      /*!< 需要配置的外部中断线
                           这个参数可以取值： @ref EXTI_Line */
  uint32_t Mode;      /*!< EXTI模式
                           该参数可以是的组合： @ref EXTI_Mode */
  uint32_t Trigger;   /*!< EXTI触发条件
                           这个参数可以取值： @ref EXTI_Trigger */
  uint32_t GPIOSel;   /*!< 要配置的 Exti GPIO 多路复用器选择。
                           此参数仅适用于线路 0 到 15。
                           这个参数可以取值： @ref EXTI_GPIOSel */
} EXTI_ConfigTypeDef;

/**
  * @}
  */

/* 导出常量 --------------------------------------------------------*/
/** @defgroup EXTI_Exported_Constants EXTI 导出常量
  * @{
  */

/** @defgroup EXTI_Line  EXTI 线
  * @{
  */
#define EXTI_LINE_0                        (EXTI_GPIO     | 0x00u)    /*!< 外部中断线 0 */
#define EXTI_LINE_1                        (EXTI_GPIO     | 0x01u)    /*!< 外部中断线 1 */
#define EXTI_LINE_2                        (EXTI_GPIO     | 0x02u)    /*!< 外部中断线 2 */
#define EXTI_LINE_3                        (EXTI_GPIO     | 0x03u)    /*!< 外部中断线 3 */
#define EXTI_LINE_4                        (EXTI_GPIO     | 0x04u)    /*!< 外部中断线 4 */
#define EXTI_LINE_5                        (EXTI_GPIO     | 0x05u)    /*!< 外部中断线 5 */
#define EXTI_LINE_6                        (EXTI_GPIO     | 0x06u)    /*!< 外部中断线 6 */
#define EXTI_LINE_7                        (EXTI_GPIO     | 0x07u)    /*!< 外部中断线 7 */
#define EXTI_LINE_8                        (EXTI_GPIO     | 0x08u)    /*!< 外部中断线 8 */
#define EXTI_LINE_9                        (EXTI_GPIO     | 0x09u)    /*!< 外部中断线 9 */
#define EXTI_LINE_10                       (EXTI_GPIO     | 0x0Au)    /*!< 外部中断线 10 */
#define EXTI_LINE_11                       (EXTI_GPIO     | 0x0Bu)    /*!< 外部中断线 11 */
#define EXTI_LINE_12                       (EXTI_GPIO     | 0x0Cu)    /*!< 外部中断线 12 */
#define EXTI_LINE_13                       (EXTI_GPIO     | 0x0Du)    /*!< 外部中断线 13 */
#define EXTI_LINE_14                       (EXTI_GPIO     | 0x0Eu)    /*!< 外部中断线 14 */
#define EXTI_LINE_15                       (EXTI_GPIO     | 0x0Fu)    /*!< 外部中断线 15 */
#define EXTI_LINE_16                       (EXTI_CONFIG   | 0x10u)    /*!< 外部中断线 16 连接到 PVD 输出 */
#define EXTI_LINE_17                       (EXTI_CONFIG   | 0x11u)    /*!< 外部中断线 17 连接到 RTC 闹钟事件 */
#if defined(EXTI_IMR_IM18)
#define EXTI_LINE_18                       (EXTI_CONFIG   | 0x12u)    /*!< 外部中断线 18 连接到 USB 从挂起状态唤醒事件 */
#endif /* EXTI_IMR_IM18 */
#if defined(EXTI_IMR_IM19)
#define EXTI_LINE_19                       (EXTI_CONFIG   | 0x13u)    /*!< 外部中断线 19 连接到 以太网 唤醒事件 */
#endif /* EXTI_IMR_IM19 */

/**
  * @}
  */

/** @defgroup EXTI_Mode  EXTI Mode 模式
  * @{
  */
#define EXTI_MODE_NONE                      0x00000000u   /* 无模式 */
#define EXTI_MODE_INTERRUPT                 0x00000001u   /* 中断模式 */
#define EXTI_MODE_EVENT                     0x00000002u   /* 事件模式 */
/**
  * @}
  */

/** @defgroup EXTI_Trigger  EXTI 触发
  * @{
  */
#define EXTI_TRIGGER_NONE                   0x00000000u   /* 无触发 */
#define EXTI_TRIGGER_RISING                 0x00000001u   /* 上升沿触发 */
#define EXTI_TRIGGER_FALLING                0x00000002u   /* 下降沿触发 */
#define EXTI_TRIGGER_RISING_FALLING         (EXTI_TRIGGER_RISING | EXTI_TRIGGER_FALLING)  /* 上升下降双沿触发 */
/**
  * @}
  */

/** @defgroup EXTI_GPIOSel  EXTI GPIOSel
  * @brief
  * @{
  */
#define EXTI_GPIOA                          0x00000000u
#define EXTI_GPIOB                          0x00000001u
#define EXTI_GPIOC                          0x00000002u
#define EXTI_GPIOD                          0x00000003u
#if defined (GPIOE)
#define EXTI_GPIOE                          0x00000004u
#endif /* GPIOE */
#if defined (GPIOF)
#define EXTI_GPIOF                          0x00000005u
#endif /* GPIOF */
#if defined (GPIOG)
#define EXTI_GPIOG                          0x00000006u
#endif /* GPIOG */
/**
  * @}
  */

/**
  * @}
  */

/* 导出宏 ------------------------------------------------------------*/
/** @defgroup EXTI_Exported_Macros EXTI 导出宏
  * @{
  */

/**
  * @}
  */

/* 私有常量 --------------------------------------------------------*/
/** @defgroup EXTI_Private_Constants EXTI 私有常量
  * @{
  */
/**
  * @brief  EXTI 线属性定义
  */
#define EXTI_PROPERTY_SHIFT                 24u   /* 属性偏移 */
#define EXTI_CONFIG                         (0x02uL << EXTI_PROPERTY_SHIFT) /* 配置 */
#define EXTI_GPIO                           ((0x04uL << EXTI_PROPERTY_SHIFT) | EXTI_CONFIG) /* GPIO */
#define EXTI_PROPERTY_MASK                  (EXTI_CONFIG | EXTI_GPIO) /* 属性掩码 */

/**
  * @brief  EXTI 引脚掩码
  */
#define EXTI_PIN_MASK                       0x0000001Fu

/**
  * @brief  EXTI 中断和事件模式掩码
  */
#define EXTI_MODE_MASK                      (EXTI_MODE_EVENT | EXTI_MODE_INTERRUPT)

/**
  * @brief  EXTI 触发可能性掩码
  */
#define EXTI_TRIGGER_MASK                   (EXTI_TRIGGER_RISING | EXTI_TRIGGER_FALLING)

/**
  * @brief  EXTI 线编号
  */
#if defined(EXTI_IMR_IM19)
#define EXTI_LINE_NB                        20UL
#elif defined(EXTI_IMR_IM18)
#define EXTI_LINE_NB                        19UL
#else /* EXTI_IMR_IM17 */
#define EXTI_LINE_NB                        18UL
#endif /* EXTI_IMR_IM19 */
/**
  * @}
  */

/* 私有宏 ------------------------------------------------------------*/
/** @defgroup EXTI_Private_Macros EXTI 私有宏
  * @{
  */
#define IS_EXTI_LINE(__EXTI_LINE__)          ((((__EXTI_LINE__) & ~(EXTI_PROPERTY_MASK | EXTI_PIN_MASK)) == 0x00u) && \
                                             ((((__EXTI_LINE__) & EXTI_PROPERTY_MASK) == EXTI_CONFIG)   || \
                                              (((__EXTI_LINE__) & EXTI_PROPERTY_MASK) == EXTI_GPIO))    && \
                                              (((__EXTI_LINE__) & EXTI_PIN_MASK) < EXTI_LINE_NB))

#define IS_EXTI_MODE(__EXTI_LINE__)          ((((__EXTI_LINE__) & EXTI_MODE_MASK) != 0x00u) && \
                                              (((__EXTI_LINE__) & ~EXTI_MODE_MASK) == 0x00u))

#define IS_EXTI_TRIGGER(__EXTI_LINE__)       (((__EXTI_LINE__) & ~EXTI_TRIGGER_MASK) == 0x00u)

#define IS_EXTI_PENDING_EDGE(__EXTI_LINE__)  ((__EXTI_LINE__) == EXTI_TRIGGER_RISING_FALLING)

#define IS_EXTI_CONFIG_LINE(__EXTI_LINE__)   (((__EXTI_LINE__) & EXTI_CONFIG) != 0x00u)

#if defined (GPIOG)
#define IS_EXTI_GPIO_PORT(__PORT__)     (((__PORT__) == EXTI_GPIOA) || \
                                         ((__PORT__) == EXTI_GPIOB) || \
                                         ((__PORT__) == EXTI_GPIOC) || \
                                         ((__PORT__) == EXTI_GPIOD) || \
                                         ((__PORT__) == EXTI_GPIOE) || \
                                         ((__PORT__) == EXTI_GPIOF) || \
                                         ((__PORT__) == EXTI_GPIOG))
#elif defined (GPIOF)
#define IS_EXTI_GPIO_PORT(__PORT__)     (((__PORT__) == EXTI_GPIOA) || \
                                         ((__PORT__) == EXTI_GPIOB) || \
                                         ((__PORT__) == EXTI_GPIOC) || \
                                         ((__PORT__) == EXTI_GPIOD) || \
                                         ((__PORT__) == EXTI_GPIOE) || \
                                         ((__PORT__) == EXTI_GPIOF))
#elif defined (GPIOE)
#define IS_EXTI_GPIO_PORT(__PORT__)     (((__PORT__) == EXTI_GPIOA) || \
                                         ((__PORT__) == EXTI_GPIOB) || \
                                         ((__PORT__) == EXTI_GPIOC) || \
                                         ((__PORT__) == EXTI_GPIOD) || \
                                         ((__PORT__) == EXTI_GPIOE))
#else
#define IS_EXTI_GPIO_PORT(__PORT__)     (((__PORT__) == EXTI_GPIOA) || \
                                         ((__PORT__) == EXTI_GPIOB) || \
                                         ((__PORT__) == EXTI_GPIOC) || \
                                         ((__PORT__) == EXTI_GPIOD))
#endif /* GPIOG */

#define IS_EXTI_GPIO_PIN(__PIN__)       ((__PIN__) < 16u)

/**
  * @}
  */

/* 导出函数 --------------------------------------------------------*/
/** @defgroup EXTI_Exported_Functions EXTI 导出函数
  * @brief    EXTI 导出函数
  * @{
  */

/** @defgroup EXTI_Exported_Functions_Group1 配置函数
  * @brief    配置函数
  * @{
  */
/* 配置函数 ****************************************************/

HAL_StatusTypeDef HAL_EXTI_SetConfigLine(EXTI_HandleTypeDef *hexti, EXTI_ConfigTypeDef *pExtiConfig);
HAL_StatusTypeDef HAL_EXTI_GetConfigLine(EXTI_HandleTypeDef *hexti, EXTI_ConfigTypeDef *pExtiConfig);
HAL_StatusTypeDef HAL_EXTI_ClearConfigLine(EXTI_HandleTypeDef *hexti);
HAL_StatusTypeDef HAL_EXTI_RegisterCallback(EXTI_HandleTypeDef *hexti, EXTI_CallbackIDTypeDef CallbackID, void (*pPendingCbfn)(void));
HAL_StatusTypeDef HAL_EXTI_GetHandle(EXTI_HandleTypeDef *hexti, uint32_t ExtiLine);
/**
  * @}
  */

/** @defgroup EXTI_Exported_Functions_Group2 IO操作函数
  * @brief    IO操作函数
  * @{
  */
/* IO操作函数 *****************************************************/

void              HAL_EXTI_IRQHandler(EXTI_HandleTypeDef *hexti);
uint32_t          HAL_EXTI_GetPending(EXTI_HandleTypeDef *hexti, uint32_t Edge);
void              HAL_EXTI_ClearPending(EXTI_HandleTypeDef *hexti, uint32_t Edge);
void              HAL_EXTI_GenerateSWI(EXTI_HandleTypeDef *hexti);

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

#endif /* STM32F1xx_HAL_EXTI_H */

