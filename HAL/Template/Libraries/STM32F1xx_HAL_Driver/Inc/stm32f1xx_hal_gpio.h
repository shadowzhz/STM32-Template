/**
  ******************************************************************************
  * @file    stm32f1xx_hal_gpio.h
  * @author  MCD Application Team
  * @brief   HAL库 GPIO 模块头文件
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
#ifndef STM32F1xx_HAL_GPIO_H
#define STM32F1xx_HAL_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/* 头文件 ------------------------------------------------------------------*/
#include "stm32f1xx_hal_def.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @addtogroup GPIO
  * @{
  */

/* 导出类型 ------------------------------------------------------------*/
/** @defgroup GPIO_Exported_Types GPIO 导出类型
  * @{
  */

/**
  * @brief GPIO 初始化结构体定义
  */
typedef struct
{
  uint32_t Pin;       /*!< 指定被配置的GPIO引脚
                           该参数可以是任何一个值：@ref GPIO_pins_define */

  uint32_t Mode;      /*!< 指定所选引脚的工作模式.
                           该参数取值为： @ref GPIO_mode_define */

  uint32_t Pull;      /*!< 指定所选引脚的上拉或下拉.
                           该参数取值为： @ref GPIO_pull_define */

  uint32_t Speed;     /*!< 指定所选引脚的速度.
                           该参数取值为： @ref GPIO_speed_define */
} GPIO_InitTypeDef;

/**
  * @brief  GPIO Bit SET 和 Bit RESET 状态枚举
  */
typedef enum
{
  GPIO_PIN_RESET = 0u,
  GPIO_PIN_SET
} GPIO_PinState;
/**
  * @}
  */

/* 导出常量 --------------------------------------------------------*/

/** @defgroup GPIO_Exported_Constants GPIO 导出常量
  * @{
  */

/** @defgroup GPIO_pins_define GPIO 引脚定义
  * @{
  */
#define GPIO_PIN_0                 ((uint16_t)0x0001)  /* 引脚 0    */
#define GPIO_PIN_1                 ((uint16_t)0x0002)  /* 引脚 1    */
#define GPIO_PIN_2                 ((uint16_t)0x0004)  /* 引脚 2    */
#define GPIO_PIN_3                 ((uint16_t)0x0008)  /* 引脚 3    */
#define GPIO_PIN_4                 ((uint16_t)0x0010)  /* 引脚 4    */
#define GPIO_PIN_5                 ((uint16_t)0x0020)  /* 引脚 5    */
#define GPIO_PIN_6                 ((uint16_t)0x0040)  /* 引脚 6    */
#define GPIO_PIN_7                 ((uint16_t)0x0080)  /* 引脚 7    */
#define GPIO_PIN_8                 ((uint16_t)0x0100)  /* 引脚 8    */
#define GPIO_PIN_9                 ((uint16_t)0x0200)  /* 引脚 9    */
#define GPIO_PIN_10                ((uint16_t)0x0400)  /* 引脚 10   */
#define GPIO_PIN_11                ((uint16_t)0x0800)  /* 引脚 11   */
#define GPIO_PIN_12                ((uint16_t)0x1000)  /* 引脚 12   */
#define GPIO_PIN_13                ((uint16_t)0x2000)  /* 引脚 13   */
#define GPIO_PIN_14                ((uint16_t)0x4000)  /* 引脚 14   */
#define GPIO_PIN_15                ((uint16_t)0x8000)  /* 引脚 15   */
#define GPIO_PIN_All               ((uint16_t)0xFFFF)  /* 全部引脚 */

#define GPIO_PIN_MASK              0x0000FFFFu /* 用于断言测试的引脚掩码 */
/**
  * @}
  */

/** @defgroup GPIO_mode_define GPIO 模式定义
  * @brief GPIO 配置模式
  *        元素值约定: 0xX0yz00YZ
  *           - X  : GPIO 模式或者EXTI模式
  *           - y  : 外部中断 或 事件触发检测
  *           - z  : 外部中断或事件的IO配置
  *           - Y  : 输出类型(推挽/开漏)
  *           - Z  : IO方向模式(输入、输出、复用或模拟)
  * @{
  */
#define  GPIO_MODE_INPUT                        0x00000000u   /*!< 浮空输入模式                   */
#define  GPIO_MODE_OUTPUT_PP                    0x00000001u   /*!< 推挽输出模式                 */
#define  GPIO_MODE_OUTPUT_OD                    0x00000011u   /*!< 开漏输出模式                */
#define  GPIO_MODE_AF_PP                        0x00000002u   /*!< 推挽式复用模式     */
#define  GPIO_MODE_AF_OD                        0x00000012u   /*!< 开漏式复用模式    */
#define  GPIO_MODE_AF_INPUT                     GPIO_MODE_INPUT          /*!< 输入复用模式         */

#define  GPIO_MODE_ANALOG                       0x00000003u   /*!< 模拟模式  */

#define  GPIO_MODE_IT_RISING                    0x10110000u   /*!< 外部中断模式上升沿触发          */
#define  GPIO_MODE_IT_FALLING                   0x10210000u   /*!< 外部中断模式下降沿触发         */
#define  GPIO_MODE_IT_RISING_FALLING            0x10310000u   /*!< 外部中断模式上升/下降双沿触发  */

#define  GPIO_MODE_EVT_RISING                   0x10120000u   /*!< 外部事件模式上升沿触发               */
#define  GPIO_MODE_EVT_FALLING                  0x10220000u   /*!< 外部事件模式下降沿触发              */
#define  GPIO_MODE_EVT_RISING_FALLING           0x10320000u   /*!< 外部事件模式上升/下降双沿触发       */

/**
  * @}
  */

/** @defgroup GPIO_speed_define  GPIO 速度定义
  * @brief GPIO 输出最大频率
  * @{
  */
#define  GPIO_SPEED_FREQ_LOW              (GPIO_CRL_MODE0_1) /*!< 低速 2MHz */
#define  GPIO_SPEED_FREQ_MEDIUM           (GPIO_CRL_MODE0_0) /*!< 中速 10MHz */
#define  GPIO_SPEED_FREQ_HIGH             (GPIO_CRL_MODE0)   /*!< 高速 50MHz */

/**
  * @}
  */

/** @defgroup GPIO_pull_define GPIO 上下拉定义
  * @brief GPIO 上拉/下拉
  * @{
  */
#define  GPIO_NOPULL        0x00000000u   /*!< 无上下拉  */
#define  GPIO_PULLUP        0x00000001u   /*!< 上拉              */
#define  GPIO_PULLDOWN      0x00000002u   /*!< 下拉              */
/**
  * @}
  */

/**
  * @}
  */

/* 导出宏 ------------------------------------------------------------*/
/** @defgroup GPIO_Exported_Macros GPIO 导出宏
  * @{
  */

/**
  * @brief 检查是否设置了指定的EXTI线标志。
  * @param  __EXTI_LINE__: 指定需要检测的EXTI线标志.
  *         参数可选 GPIO_PIN_x  x取值(0..15)
  * @retval __EXTI_LINE__的状态 (SET 或 RESET).
  */
#define __HAL_GPIO_EXTI_GET_FLAG(__EXTI_LINE__) (EXTI->PR & (__EXTI_LINE__))

/**
  * @brief  清除EXTI线挂起的标志.
  * @param  __EXTI_LINE__: 指定需要清除的EXTI线标志.
  *         该参数可以是GPIO_PIN_x的任意组合，其中x可以是(0..15)
  * @retval None
  */
#define __HAL_GPIO_EXTI_CLEAR_FLAG(__EXTI_LINE__) (EXTI->PR = (__EXTI_LINE__))

/**
  * @brief  检查指定的EXTI线中断是否被置位。
  * @param  __EXTI_LINE__: 指定需要被检查的EXTI线.
  *          参数可选 GPIO_PIN_x  x取值(0..15)
  * @retval __EXTI_LINE__的状态 (SET 或 RESET).
  */
#define __HAL_GPIO_EXTI_GET_IT(__EXTI_LINE__) (EXTI->PR & (__EXTI_LINE__))

/**
  * @brief  清除EXTI线的中断挂起位.
  * @param  __EXTI_LINE__: 指定需要清除的EXTI线.
  *          该参数可以是GPIO_PIN_x的任意组合，其中x可以是(0..15)
  * @retval None
  */
#define __HAL_GPIO_EXTI_CLEAR_IT(__EXTI_LINE__) (EXTI->PR = (__EXTI_LINE__))

/**
  * @brief  在选定的EXTI线上生成软件中断。
  * @param  __EXTI_LINE__: 指定EXTI线
  *          参数可选 GPIO_PIN_x  x取值(0..15)
  * @retval None
  */
#define __HAL_GPIO_EXTI_GENERATE_SWIT(__EXTI_LINE__) (EXTI->SWIER |= (__EXTI_LINE__))
/**
  * @}
  */

/* 引入GPIO扩展库 */
#include "stm32f1xx_hal_gpio_ex.h"

/* 导出函数 --------------------------------------------------------*/
/** @addtogroup GPIO_Exported_Functions
  * @{
  */

/** @addtogroup GPIO_Exported_Functions_Group1
  * @{
  */
/* 初始化和反初始化函数 *****************************/

void  HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init);
void  HAL_GPIO_DeInit(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pin);
/**
  * @}
  */

/** @addtogroup GPIO_Exported_Functions_Group2
  * @{
  */
/* IO操作函数 *****************************************************/

GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
HAL_StatusTypeDef HAL_GPIO_LockPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void HAL_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

/**
  * @}
  */

/**
  * @}
  */
/* 私有类型 -------------------------------------------------------------*/
/* 私有变量 ---------------------------------------------------------*/
/* 私有常量 ---------------------------------------------------------*/
/** @defgroup GPIO_Private_Constants GPIO 私有常量
  * @{
  */

/**
  * @}
  */

/* 私有宏 ------------------------------------------------------------*/
/** @defgroup GPIO_Private_Macros GPIO 私有宏
  * @{
  */
#define IS_GPIO_PIN_ACTION(ACTION) (((ACTION) == GPIO_PIN_RESET) || ((ACTION) == GPIO_PIN_SET))
#define IS_GPIO_PIN(PIN)           (((((uint32_t)PIN) & GPIO_PIN_MASK ) != 0x00u) && ((((uint32_t)PIN) & ~GPIO_PIN_MASK) == 0x00u))
#define IS_GPIO_MODE(MODE) (((MODE) == GPIO_MODE_INPUT)              ||\
                            ((MODE) == GPIO_MODE_OUTPUT_PP)          ||\
                            ((MODE) == GPIO_MODE_OUTPUT_OD)          ||\
                            ((MODE) == GPIO_MODE_AF_PP)              ||\
                            ((MODE) == GPIO_MODE_AF_OD)              ||\
                            ((MODE) == GPIO_MODE_IT_RISING)          ||\
                            ((MODE) == GPIO_MODE_IT_FALLING)         ||\
                            ((MODE) == GPIO_MODE_IT_RISING_FALLING)  ||\
                            ((MODE) == GPIO_MODE_EVT_RISING)         ||\
                            ((MODE) == GPIO_MODE_EVT_FALLING)        ||\
                            ((MODE) == GPIO_MODE_EVT_RISING_FALLING) ||\
                            ((MODE) == GPIO_MODE_ANALOG))
#define IS_GPIO_SPEED(SPEED) (((SPEED) == GPIO_SPEED_FREQ_LOW) || \
                              ((SPEED) == GPIO_SPEED_FREQ_MEDIUM) || ((SPEED) == GPIO_SPEED_FREQ_HIGH))
#define IS_GPIO_PULL(PULL) (((PULL) == GPIO_NOPULL) || ((PULL) == GPIO_PULLUP) || \
                            ((PULL) == GPIO_PULLDOWN))
/**
  * @}
  */

/* 私有函数 ---------------------------------------------------------*/
/** @defgroup GPIO_Private_Functions GPIO 私有函数
  * @{
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

#endif /* STM32F1xx_HAL_GPIO_H */

