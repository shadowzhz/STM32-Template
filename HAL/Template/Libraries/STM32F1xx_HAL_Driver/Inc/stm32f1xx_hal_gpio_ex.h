/**
  ******************************************************************************
  * @file    stm32f1xx_hal_gpio_ex.h
  * @author  MCD Application Team
  * @brief   HAL库 GPIO 扩展模块头文件
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
#ifndef STM32F1xx_HAL_GPIO_EX_H
#define STM32F1xx_HAL_GPIO_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/* 头文件 ------------------------------------------------------------------*/
#include "stm32f1xx_hal_def.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @defgroup GPIOEx GPIOEx
  * @{
  */
/* 导出类型 ------------------------------------------------------------*/
/* 导出常量 --------------------------------------------------------*/

/** @defgroup GPIOEx_Exported_Constants GPIOEx 导出常量
  * @{
  */

/** @defgroup GPIOEx_EVENTOUT Cortex 事件输出 配置
  * @brief 这个部分定义了使用Cortex 事件输出 信号
  * @{
  */

/** @defgroup GPIOEx_EVENTOUT_PIN 事件输出引脚
  * @{
  */

#define AFIO_EVENTOUT_PIN_0  AFIO_EVCR_PIN_PX0 /*!< 事件输出在引脚 0 */
#define AFIO_EVENTOUT_PIN_1  AFIO_EVCR_PIN_PX1 /*!< 事件输出在引脚 1 */
#define AFIO_EVENTOUT_PIN_2  AFIO_EVCR_PIN_PX2 /*!< 事件输出在引脚 2 */
#define AFIO_EVENTOUT_PIN_3  AFIO_EVCR_PIN_PX3 /*!< 事件输出在引脚 3 */
#define AFIO_EVENTOUT_PIN_4  AFIO_EVCR_PIN_PX4 /*!< 事件输出在引脚 4 */
#define AFIO_EVENTOUT_PIN_5  AFIO_EVCR_PIN_PX5 /*!< 事件输出在引脚 5 */
#define AFIO_EVENTOUT_PIN_6  AFIO_EVCR_PIN_PX6 /*!< 事件输出在引脚 6 */
#define AFIO_EVENTOUT_PIN_7  AFIO_EVCR_PIN_PX7 /*!< 事件输出在引脚 7 */
#define AFIO_EVENTOUT_PIN_8  AFIO_EVCR_PIN_PX8 /*!< 事件输出在引脚 8 */
#define AFIO_EVENTOUT_PIN_9  AFIO_EVCR_PIN_PX9 /*!< 事件输出在引脚 9 */
#define AFIO_EVENTOUT_PIN_10 AFIO_EVCR_PIN_PX10 /*!< 事件输出在引脚 10 */
#define AFIO_EVENTOUT_PIN_11 AFIO_EVCR_PIN_PX11 /*!< 事件输出在引脚 11 */
#define AFIO_EVENTOUT_PIN_12 AFIO_EVCR_PIN_PX12 /*!< 事件输出在引脚 12 */
#define AFIO_EVENTOUT_PIN_13 AFIO_EVCR_PIN_PX13 /*!< 事件输出在引脚 13 */
#define AFIO_EVENTOUT_PIN_14 AFIO_EVCR_PIN_PX14 /*!< 事件输出在引脚 14 */
#define AFIO_EVENTOUT_PIN_15 AFIO_EVCR_PIN_PX15 /*!< 事件输出在引脚 15 */

#define IS_AFIO_EVENTOUT_PIN(__PIN__) (((__PIN__) == AFIO_EVENTOUT_PIN_0) || \
                                       ((__PIN__) == AFIO_EVENTOUT_PIN_1) || \
                                       ((__PIN__) == AFIO_EVENTOUT_PIN_2) || \
                                       ((__PIN__) == AFIO_EVENTOUT_PIN_3) || \
                                       ((__PIN__) == AFIO_EVENTOUT_PIN_4) || \
                                       ((__PIN__) == AFIO_EVENTOUT_PIN_5) || \
                                       ((__PIN__) == AFIO_EVENTOUT_PIN_6) || \
                                       ((__PIN__) == AFIO_EVENTOUT_PIN_7) || \
                                       ((__PIN__) == AFIO_EVENTOUT_PIN_8) || \
                                       ((__PIN__) == AFIO_EVENTOUT_PIN_9) || \
                                       ((__PIN__) == AFIO_EVENTOUT_PIN_10) || \
                                       ((__PIN__) == AFIO_EVENTOUT_PIN_11) || \
                                       ((__PIN__) == AFIO_EVENTOUT_PIN_12) || \
                                       ((__PIN__) == AFIO_EVENTOUT_PIN_13) || \
                                       ((__PIN__) == AFIO_EVENTOUT_PIN_14) || \
                                       ((__PIN__) == AFIO_EVENTOUT_PIN_15))
/**
  * @}
  */

/** @defgroup GPIOEx_EVENTOUT_PORT 事件输出 端口
  * @{
  */

#define AFIO_EVENTOUT_PORT_A AFIO_EVCR_PORT_PA /*!< 事件输出在端口 A */
#define AFIO_EVENTOUT_PORT_B AFIO_EVCR_PORT_PB /*!< 事件输出在端口 B */
#define AFIO_EVENTOUT_PORT_C AFIO_EVCR_PORT_PC /*!< 事件输出在端口 C */
#define AFIO_EVENTOUT_PORT_D AFIO_EVCR_PORT_PD /*!< 事件输出在端口 D */
#define AFIO_EVENTOUT_PORT_E AFIO_EVCR_PORT_PE /*!< 事件输出在端口 E */

#define IS_AFIO_EVENTOUT_PORT(__PORT__) (((__PORT__) == AFIO_EVENTOUT_PORT_A) || \
                                         ((__PORT__) == AFIO_EVENTOUT_PORT_B) || \
                                         ((__PORT__) == AFIO_EVENTOUT_PORT_C) || \
                                         ((__PORT__) == AFIO_EVENTOUT_PORT_D) || \
                                         ((__PORT__) == AFIO_EVENTOUT_PORT_E))
/**
  * @}
  */

/**
  * @}
  */

/** @defgroup GPIOEx_AFIO_AF_REMAPPING 复用功能重映射
  * @brief 本节定义将复用功能重新映射到一些其他端口/引脚
  * @{
  */

/**
  * @brief 启用SPI1复用功能 NSS、SCK、MISO和MOSI的重新映射。
  * @note  ENABLE: 重映射     (NSS/PA15, SCK/PB3, MISO/PB4, MOSI/PB5)
  * @retval None
  */
#define __HAL_AFIO_REMAP_SPI1_ENABLE()  AFIO_REMAP_ENABLE(AFIO_MAPR_SPI1_REMAP)

/**
  * @brief 禁用SPI1复用功能 NSS、SCK、MISO和MOSI的重新映射。
  * @note  DISABLE: 无重映射 (NSS/PA4,  SCK/PA5, MISO/PA6, MOSI/PA7)
  * @retval None
  */
#define __HAL_AFIO_REMAP_SPI1_DISABLE()  AFIO_REMAP_DISABLE(AFIO_MAPR_SPI1_REMAP)

/**
  * @brief 启用I2C1复用功能 SCL 和 SDA的重映射.
  * @note  ENABLE: 重映射     (SCL/PB8, SDA/PB9)
  * @retval None
  */
#define __HAL_AFIO_REMAP_I2C1_ENABLE()  AFIO_REMAP_ENABLE(AFIO_MAPR_I2C1_REMAP)

/**
  * @brief 关闭I2C1 复用功能 SCL 和 SDA的重映射.
  * @note  DISABLE: 无重映射 (SCL/PB6, SDA/PB7)
  * @retval None
  */
#define __HAL_AFIO_REMAP_I2C1_DISABLE() AFIO_REMAP_DISABLE(AFIO_MAPR_I2C1_REMAP)

/**
  * @brief 启用USART1复用功能 TX和RX的重新映射。
  * @note  ENABLE: 重映射     (TX/PB6, RX/PB7)
  * @retval None
  */
#define __HAL_AFIO_REMAP_USART1_ENABLE()  AFIO_REMAP_ENABLE(AFIO_MAPR_USART1_REMAP)

/**
  * @brief 禁用USART1复用功能 TX和RX的重新映射。
  * @note  DISABLE: 无重映射 (TX/PA9, RX/PA10)
  * @retval None
  */
#define __HAL_AFIO_REMAP_USART1_DISABLE() AFIO_REMAP_DISABLE(AFIO_MAPR_USART1_REMAP)

/**
  * @brief 启用USART2复用功能 CTS, RTS, CK, TX和RX的重新映射。
  * @note  ENABLE: 重映射     (CTS/PD3, RTS/PD4, TX/PD5, RX/PD6, CK/PD7)
  * @retval None
  */
#define __HAL_AFIO_REMAP_USART2_ENABLE()  AFIO_REMAP_ENABLE(AFIO_MAPR_USART2_REMAP)

/**
  * @brief 禁用USART2复用功能 CTS, RTS, CK, TX和RX的重新映射。
  * @note  DISABLE: 无重映射 (CTS/PA0, RTS/PA1, TX/PA2, RX/PA3, CK/PA4)
  * @retval None
  */
#define __HAL_AFIO_REMAP_USART2_DISABLE() AFIO_REMAP_DISABLE(AFIO_MAPR_USART2_REMAP)

/**
  * @brief 启用USART3复用功能 CTS, RTS, CK, TX和RX的 全部 重新映射。
  * @note  ENABLE: 全部重映射     (TX/PD8,  RX/PD9,  CK/PD10, CTS/PD11, RTS/PD12)
  * @retval None
  */
#define __HAL_AFIO_REMAP_USART3_ENABLE()  AFIO_REMAP_PARTIAL(AFIO_MAPR_USART3_REMAP_FULLREMAP, AFIO_MAPR_USART3_REMAP_FULLREMAP)

/**
  * @brief 启用USART3复用功能 CTS, RTS, CK, TX和RX的 部分 重新映射。
  * @note  PARTIAL: 部分重映射 (TX/PC10, RX/PC11, CK/PC12, CTS/PB13, RTS/PB14)
  * @retval None
  */
#define __HAL_AFIO_REMAP_USART3_PARTIAL()  AFIO_REMAP_PARTIAL(AFIO_MAPR_USART3_REMAP_PARTIALREMAP, AFIO_MAPR_USART3_REMAP_FULLREMAP)

/**
  * @brief 禁用USART3复用功能 CTS, RTS, CK, TX和RX的 全部 重新映射。
  * @note  DISABLE: 无重映射      (TX/PB10, RX/PB11, CK/PB12, CTS/PB13, RTS/PB14)
  * @retval None
  */
#define __HAL_AFIO_REMAP_USART3_DISABLE()  AFIO_REMAP_PARTIAL(AFIO_MAPR_USART3_REMAP_NOREMAP, AFIO_MAPR_USART3_REMAP_FULLREMAP)

/**
  * @brief 启用TIM1复用功能 通道1 到 4, 1N 到 3N，外部触发(ETR) 和 刹车输入(BKIN)的 全部 重新映射
  * @note  ENABLE: 全部重映射     (ETR/PE7,  CH1/PE9, CH2/PE11, CH3/PE13, CH4/PE14, BKIN/PE15, CH1N/PE8,  CH2N/PE10, CH3N/PE12)
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM1_ENABLE()  AFIO_REMAP_PARTIAL(AFIO_MAPR_TIM1_REMAP_FULLREMAP, AFIO_MAPR_TIM1_REMAP_FULLREMAP)

/**
  * @brief 启用TIM1复用功能 通道1 到 4, 1N 到 3N，外部触发(ETR) 和 刹车输入(BKIN)的 部分 重新映射
  * @note  PARTIAL: 部分重映射 (ETR/PA12, CH1/PA8, CH2/PA9,  CH3/PA10, CH4/PA11, BKIN/PA6,  CH1N/PA7,  CH2N/PB0,  CH3N/PB1)
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM1_PARTIAL()  AFIO_REMAP_PARTIAL(AFIO_MAPR_TIM1_REMAP_PARTIALREMAP, AFIO_MAPR_TIM1_REMAP_FULLREMAP)

/**
  * @brief 禁用TIM1复用功能 通道1 到 4, 1N 到 3N，外部触发(ETR) 和 刹车输入(BKIN)的重新映射
  * @note  DISABLE: 无重映射      (ETR/PA12, CH1/PA8, CH2/PA9,  CH3/PA10, CH4/PA11, BKIN/PB12, CH1N/PB13, CH2N/PB14, CH3N/PB15)
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM1_DISABLE()  AFIO_REMAP_PARTIAL(AFIO_MAPR_TIM1_REMAP_NOREMAP, AFIO_MAPR_TIM1_REMAP_FULLREMAP)

/**
  * @brief 启用TIM2复用功能 通道1到4 和 外部触发(ETR)的 全部 重新映射
  * @note  ENABLE: 全部重映射       (CH1/ETR/PA15, CH2/PB3, CH3/PB10, CH4/PB11)
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM2_ENABLE()  AFIO_REMAP_PARTIAL(AFIO_MAPR_TIM2_REMAP_FULLREMAP, AFIO_MAPR_TIM2_REMAP_FULLREMAP)

/**
  * @brief 启用TIM2复用功能 通道1到4 和 外部触发(ETR)的 部分 重新映射
  * @note  PARTIAL_2: 部分重映射模式2 (CH1/ETR/PA0,  CH2/PA1, CH3/PB10, CH4/PB11)
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM2_PARTIAL_2()  AFIO_REMAP_PARTIAL(AFIO_MAPR_TIM2_REMAP_PARTIALREMAP2, AFIO_MAPR_TIM2_REMAP_FULLREMAP)

/**
  * @brief 启用TIM2复用功能 通道1到4 和 外部触发(ETR)的 部分 重新映射
  * @note  PARTIAL_1: 部分重映射模式1 (CH1/ETR/PA15, CH2/PB3, CH3/PA2,  CH4/PA3)
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM2_PARTIAL_1()  AFIO_REMAP_PARTIAL(AFIO_MAPR_TIM2_REMAP_PARTIALREMAP1, AFIO_MAPR_TIM2_REMAP_FULLREMAP)

/**
  * @brief 禁用TIM2复用功能 通道1到4 和 外部触发(ETR)的重新映射
  * @note  DISABLE: 无重映射        (CH1/ETR/PA0,  CH2/PA1, CH3/PA2,  CH4/PA3)
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM2_DISABLE()  AFIO_REMAP_PARTIAL(AFIO_MAPR_TIM2_REMAP_NOREMAP, AFIO_MAPR_TIM2_REMAP_FULLREMAP)

/**
  * @brief 启用TIM3复用功能 通道1到4的 全部 重新映射
  * @note  ENABLE: 全部重映射     (CH1/PC6, CH2/PC7, CH3/PC8, CH4/PC9)
  * @note  TIM3_ETR 在 PE0 无重映射.
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM3_ENABLE()  AFIO_REMAP_PARTIAL(AFIO_MAPR_TIM3_REMAP_FULLREMAP, AFIO_MAPR_TIM3_REMAP_FULLREMAP)

/**
  * @brief 启用TIM3复用功能 通道1到4的 部分 重新映射
  * @note  PARTIAL: 部分重映射 (CH1/PB4, CH2/PB5, CH3/PB0, CH4/PB1)
  * @note  TIM3_ETR 在 PE0 无重映射.
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM3_PARTIAL()  AFIO_REMAP_PARTIAL(AFIO_MAPR_TIM3_REMAP_PARTIALREMAP, AFIO_MAPR_TIM3_REMAP_FULLREMAP)

/**
  * @brief 禁用TIM3复用功能 通道1到4的重新映射
  * @note  DISABLE: 无重映射      (CH1/PA6, CH2/PA7, CH3/PB0, CH4/PB1)
  * @note  TIM3_ETR 在 PE0 无重映射.
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM3_DISABLE()  AFIO_REMAP_PARTIAL(AFIO_MAPR_TIM3_REMAP_NOREMAP, AFIO_MAPR_TIM3_REMAP_FULLREMAP)

/**
  * @brief 启用TIM4复用功能 通道1到4的重新映射。
  * @note  ENABLE: 全部重映射 (TIM4_CH1/PD12, TIM4_CH2/PD13, TIM4_CH3/PD14, TIM4_CH4/PD15)
  * @note  TIM4_ETR 在 PE0 无重映射.
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM4_ENABLE()  AFIO_REMAP_ENABLE(AFIO_MAPR_TIM4_REMAP)

/**
  * @brief 禁用TIM4复用功能 通道1到4的重新映射。
  * @note  DISABLE: 无重映射  (TIM4_CH1/PB6,  TIM4_CH2/PB7,  TIM4_CH3/PB8,  TIM4_CH4/PB9)
  * @note  TIM4_ETR 在 PE0 无重映射.
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM4_DISABLE() AFIO_REMAP_DISABLE(AFIO_MAPR_TIM4_REMAP)

#if defined(AFIO_MAPR_CAN_REMAP_REMAP1)

/**
  * @brief 启用或禁用CAN_RX和CAN_TX在具有单个CAN接口的设备上的CAN复用功能的重新映射1。
  * @note  CASE 1: CAN_RX 重映射到 PA11, CAN_TX 重映射到 PA12
  * @retval None
  */
#define __HAL_AFIO_REMAP_CAN1_1()  AFIO_REMAP_PARTIAL(AFIO_MAPR_CAN_REMAP_REMAP1, AFIO_MAPR_CAN_REMAP)

/**
  * @brief 启用或禁用CAN_RX和CAN_TX在具有单个CAN接口的设备上的CAN复用功能的重新映射2。
  * @note  CASE 2: CAN_RX 重映射到 PB8,  CAN_TX 重映射到 PB9 (not available on 36-pin package)
  * @retval None
  */
#define __HAL_AFIO_REMAP_CAN1_2()  AFIO_REMAP_PARTIAL(AFIO_MAPR_CAN_REMAP_REMAP2, AFIO_MAPR_CAN_REMAP)

/**
  * @brief 启用或禁用CAN_RX和CAN_TX在具有单个CAN接口的设备上的CAN复用功能的重新映射3。
  * @note  CASE 3: CAN_RX 重映射到 PD0,  CAN_TX 重映射到 PD1
  * @retval None
  */
#define __HAL_AFIO_REMAP_CAN1_3()  AFIO_REMAP_PARTIAL(AFIO_MAPR_CAN_REMAP_REMAP3, AFIO_MAPR_CAN_REMAP)

#endif

/**
  * @brief 不使用HSE振荡器时，启用PD0和PD1的重新映射。
  *        当不使用HSE振荡器（应用程序在内部8 MHz RC上运行）时，PD0和PD1可以映射到OSC_IN和OSC_OUT。
  *        这仅适用于36、48和64引脚封装（PD0和PD1在100引脚和144引脚封装中可用，无需重新映射）。
  * @note  ENABLE: PD0 重映射到 OSC_IN, PD1 重映射到 OSC_OUT.
  * @retval None
  */
#define __HAL_AFIO_REMAP_PD01_ENABLE()  AFIO_REMAP_ENABLE(AFIO_MAPR_PD01_REMAP)

/**
  * @brief 不使用HSE振荡器时，禁用PD0和PD1的重新映射。
  *        当不使用HSE振荡器（应用程序在内部8 MHz RC上运行）时，PD0和PD1可以映射到OSC_IN和OSC_OUT。
  *        这仅适用于36、48和64引脚封装（PD0和PD1在100引脚和144引脚封装中可用，无需重新映射）。
  * @note  DISABLE: PD0 和 PD1 无重映射
  * @retval None
  */
#define __HAL_AFIO_REMAP_PD01_DISABLE() AFIO_REMAP_DISABLE(AFIO_MAPR_PD01_REMAP)

#if defined(AFIO_MAPR_TIM5CH4_IREMAP)
/**
  * @brief 启用TIM5CH4的重映射
  * @note  ENABLE: LSI内部时钟连接到TIM5_CH4输入进行校准。
  * @note  此功能仅适用于高密度类器件。
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM5CH4_ENABLE()  AFIO_REMAP_ENABLE(AFIO_MAPR_TIM5CH4_IREMAP)

/**
  * @brief 禁用TIM5CH4的重映射
  * @note  DISABLE: TIM5_CH4 被连接到 PA3
  * @note  此功能仅适用于高密度类器件。
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM5CH4_DISABLE() AFIO_REMAP_DISABLE(AFIO_MAPR_TIM5CH4_IREMAP)
#endif

#if defined(AFIO_MAPR_ETH_REMAP)
/**
  * @brief 启用以太网MAC与物理层之间的重新映射连接。
  * @note  ENABLE: 重映射     (RX_DV-CRS_DV/PD8, RXD0/PD9, RXD1/PD10, RXD2/PD11, RXD3/PD12)
  * @note  此位仅在connectivity类设备中可用，否则保留。
  * @retval None
  */
#define __HAL_AFIO_REMAP_ETH_ENABLE()  AFIO_REMAP_ENABLE(AFIO_MAPR_ETH_REMAP)

/**
  * @brief 禁用以太网MAC与物理层之间的重新映射连接。
  * @note  DISABLE: 无重映射 (RX_DV-CRS_DV/PA7, RXD0/PC4, RXD1/PC5,  RXD2/PB0,  RXD3/PB1)
  * @note  此位仅在connectivity类设备中可用，否则保留。
  * @retval None
  */
#define __HAL_AFIO_REMAP_ETH_DISABLE() AFIO_REMAP_DISABLE(AFIO_MAPR_ETH_REMAP)
#endif

#if defined(AFIO_MAPR_CAN2_REMAP)

/**
  * @brief 启用CAN2复用功能CAN2_RX和CAN2_TX的重映射
  * @note  ENABLE: 重映射     (CAN2_RX/PB5,  CAN2_TX/PB6)
  * @note  此位仅在connectivity类设备中可用，否则保留。
  * @retval None
  */
#define __HAL_AFIO_REMAP_CAN2_ENABLE()  AFIO_REMAP_ENABLE(AFIO_MAPR_CAN2_REMAP)

/**
  * @brief 禁用CAN2复用功能CAN2_RX和CAN2_TX的重映射
  * @note  DISABLE: 无重映射 (CAN2_RX/PB12, CAN2_TX/PB13)
  * @note  此位仅在connectivity类设备中可用，否则保留。
  * @retval None
  */
#define __HAL_AFIO_REMAP_CAN2_DISABLE() AFIO_REMAP_DISABLE(AFIO_MAPR_CAN2_REMAP)
#endif

#if defined(AFIO_MAPR_MII_RMII_SEL)
/**
  * @brief 该函数配置以太网MAC，使其内部使用外部的RMII PHY
  * @note  ETH_RMII: 配置以太网MAC以与RMII PHY建立连接
  * @note  此位仅在connectivity类设备中可用，否则保留。
  * @retval None
  */
#define __HAL_AFIO_ETH_RMII() AFIO_REMAP_ENABLE(AFIO_MAPR_MII_RMII_SEL)

/**
  * @brief 该函数配置以太网MAC，使其内部使用外部的MII
  * @note  ETH_MII: 配置以太网MAC以与MII PHY建立连接
  * @note  此位仅在connectivity类设备中可用，否则保留。
  * @retval None
  */
#define __HAL_AFIO_ETH_MII()  AFIO_REMAP_DISABLE(AFIO_MAPR_MII_RMII_SEL)
#endif

/**
  * @brief 启用ADC1_ETRGINJ (ADC 1外部触发注入转换)的重映射。
  * @note  ENABLE: ADC1外部事件触发注入转换输入连接TIM8 Channel4。
  * @retval None
  */
#define __HAL_AFIO_REMAP_ADC1_ETRGINJ_ENABLE()  AFIO_REMAP_ENABLE(AFIO_MAPR_ADC1_ETRGINJ_REMAP)

/**
  * @brief 禁用ADC1_ETRGINJ (ADC 1外部触发注入转换)的重映射。
  * @note  DISABLE: ADC1外部触发注入转换输入连接 EXTI15
  * @retval None
  */
#define __HAL_AFIO_REMAP_ADC1_ETRGINJ_DISABLE() AFIO_REMAP_DISABLE(AFIO_MAPR_ADC1_ETRGINJ_REMAP)

/**
  * @brief 启用ADC1_ETRGREG (ADC 1外部触发常规转换)的重新映射。
  * @note  ENABLE: ADC1外部事件常规转换连接TIM8 TRG0。
  * @retval None
  */
#define __HAL_AFIO_REMAP_ADC1_ETRGREG_ENABLE()  AFIO_REMAP_ENABLE(AFIO_MAPR_ADC1_ETRGREG_REMAP)

/**
  * @brief 禁用ADC1_ETRGREG (ADC 1外部触发常规转换)的重映射。
  * @note  DISABLE: ADC1外部触发常规转换连接EXTI11
  * @retval None
  */
#define __HAL_AFIO_REMAP_ADC1_ETRGREG_DISABLE() AFIO_REMAP_DISABLE(AFIO_MAPR_ADC1_ETRGREG_REMAP)

#if defined(AFIO_MAPR_ADC2_ETRGINJ_REMAP)

/**
  * @brief 启用ADC2_ETRGREG (ADC 2外部触发注入转换)的重映射。
  * @note  ENABLE: ADC2外部事件注入转换连接到TIM8 Channel4。
  * @retval None
  */
#define __HAL_AFIO_REMAP_ADC2_ETRGINJ_ENABLE()  AFIO_REMAP_ENABLE(AFIO_MAPR_ADC2_ETRGINJ_REMAP)

/**
  * @brief 禁用ADC2_ETRGREG (ADC 2外部触发注入转换)的重映射。
  * @note  DISABLE: ADC2外部触发注入转换连接到EXTI15
  * @retval None
  */
#define __HAL_AFIO_REMAP_ADC2_ETRGINJ_DISABLE() AFIO_REMAP_DISABLE(AFIO_MAPR_ADC2_ETRGINJ_REMAP)
#endif

#if defined (AFIO_MAPR_ADC2_ETRGREG_REMAP)

/**
  * @brief 启用ADC2_ETRGREG (ADC 2外部触发常规转换)的重映射。
  * @note  ENABLE: ADC2外部事件常规转换连接TIM8 TRG0。
  * @retval None
  */
#define __HAL_AFIO_REMAP_ADC2_ETRGREG_ENABLE()  AFIO_REMAP_ENABLE(AFIO_MAPR_ADC2_ETRGREG_REMAP)

/**
  * @brief 禁用ADC2_ETRGREG (ADC 2外部触发常规转换)的重映射。
  * @note  DISABLE: ADC2外部触发正则转换连接EXTI11
  * @retval None
  */
#define __HAL_AFIO_REMAP_ADC2_ETRGREG_DISABLE() AFIO_REMAP_DISABLE(AFIO_MAPR_ADC2_ETRGREG_REMAP)
#endif

/**
  * @brief 启用串口线JTAG配置
  * @note  ENABLE: 全部 SWJ (JTAG-DP + SW-DP): 复位状态
  * @retval None
  */
#define __HAL_AFIO_REMAP_SWJ_ENABLE()  AFIO_DBGAFR_CONFIG(AFIO_MAPR_SWJ_CFG_RESET)

/**
  * @brief 启用串口线JTAG配置
  * @note  NONJTRST: 全部 SWJ (JTAG-DP + SW-DP) 但除了 NJTRST
  * @retval None
  */
#define __HAL_AFIO_REMAP_SWJ_NONJTRST()  AFIO_DBGAFR_CONFIG(AFIO_MAPR_SWJ_CFG_NOJNTRST)

/**
  * @brief 启用串口线JTAG配置
  * @note  NOJTAG: JTAG-DP 禁用 且 SW-DP 开启
  * @retval None
  */

#define __HAL_AFIO_REMAP_SWJ_NOJTAG()  AFIO_DBGAFR_CONFIG(AFIO_MAPR_SWJ_CFG_JTAGDISABLE)

/**
  * @brief 禁用串口线JTAG配置
  * @note  DISABLE: JTAG-DP 禁用 且 SW-DP 禁用
  * @retval None
  */
#define __HAL_AFIO_REMAP_SWJ_DISABLE()  AFIO_DBGAFR_CONFIG(AFIO_MAPR_SWJ_CFG_DISABLE)

#if defined(AFIO_MAPR_SPI3_REMAP)

/**
  * @brief 启用 SPI3 复用功能重映射 SPI3_NSS/I2S3_WS, SPI3_SCK/I2S3_CK, SPI3_MISO, SPI3_MOSI/I2S3_SD.
  * @note  ENABLE: 重映射     (SPI3_NSS-I2S3_WS/PA4,  SPI3_SCK-I2S3_CK/PC10, SPI3_MISO/PC11, SPI3_MOSI-I2S3_SD/PC12)
  * @note  此位仅在connectivity类设备中可用，否则保留。
  * @retval None
  */
#define __HAL_AFIO_REMAP_SPI3_ENABLE()  AFIO_REMAP_ENABLE(AFIO_MAPR_SPI3_REMAP)

/**
  * @brief 禁用 SPI3 复用功能重映射 SPI3_NSS/I2S3_WS, SPI3_SCK/I2S3_CK, SPI3_MISO, SPI3_MOSI/I2S3_SD.
  * @note  DISABLE: 无重映射 (SPI3_NSS-I2S3_WS/PA15, SPI3_SCK-I2S3_CK/PB3,  SPI3_MISO/PB4,  SPI3_MOSI-I2S3_SD/PB5).
  * @note  此位仅在connectivity类设备中可用，否则保留。
  * @retval None
  */
#define __HAL_AFIO_REMAP_SPI3_DISABLE() AFIO_REMAP_DISABLE(AFIO_MAPR_SPI3_REMAP)
#endif

#if defined(AFIO_MAPR_TIM2ITR1_IREMAP)

/**
  * @brief TIM2_ITR1内部映射到USB
  * @note  TO_USB: 将USB OTG SOF（帧起始）输出连接到TIM2_ITR1以进行校准
  * @note  此位仅在connectivity类设备中可用，否则保留。
  * @retval None
  */
#define __HAL_AFIO_TIM2ITR1_TO_USB() AFIO_REMAP_ENABLE(AFIO_MAPR_TIM2ITR1_IREMAP)

/**
  * @brief TIM2_ITR1内部映射到ETH
  * @note  TO_ETH: 将TIM2_ITR1内部连接到以太网PTP输出以进行校准
  * @note  此位仅在connectivity类设备中可用，否则保留。
  * @retval None
  */
#define __HAL_AFIO_TIM2ITR1_TO_ETH() AFIO_REMAP_DISABLE(AFIO_MAPR_TIM2ITR1_IREMAP)
#endif

#if defined(AFIO_MAPR_PTP_PPS_REMAP)

/**
  * @brief 启用ADC2_ETRGREG（ADC 2外部触发常规转换）的重映射
  * @note  ENABLE: PTP_PPS 输出到 PB5 引脚
  * @note  此位仅在connectivity类设备中可用，否则保留。
  * @retval None
  */
#define __HAL_AFIO_ETH_PTP_PPS_ENABLE()  AFIO_REMAP_ENABLE(AFIO_MAPR_PTP_PPS_REMAP)

/**
  * @brief 禁用ADC2_ETRGREG（ADC 2外部触发常规转换）的重映射
  * @note  DISABLE: PTP_PPS 不输出到 PB5 引脚
  * @note  此位仅在connectivity类设备中可用，否则保留。
  * @retval None
  */
#define __HAL_AFIO_ETH_PTP_PPS_DISABLE() AFIO_REMAP_DISABLE(AFIO_MAPR_PTP_PPS_REMAP)
#endif

#if defined(AFIO_MAPR2_TIM9_REMAP)

/**
  * @brief 启用 TIM9_CH1 和 TIM9_CH2 的重映射
  * @note  ENABLE: 重映射     (TIM9_CH1 在 PE5 和 TIM9_CH2 在 PE6).
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM9_ENABLE()  SET_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM9_REMAP)

/**
  * @brief 禁用 TIM9_CH1 和 TIM9_CH2的重映射
  * @note  DISABLE: 无重映射 (TIM9_CH1 在 PA2 和 TIM9_CH2 在 PA3).
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM9_DISABLE() CLEAR_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM9_REMAP)
#endif

#if defined(AFIO_MAPR2_TIM10_REMAP)

/**
  * @brief 启用 TIM10_CH1 的重映射
  * @note  ENABLE: 重映射     (TIM10_CH1 在 PF6).
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM10_ENABLE()  SET_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM10_REMAP)

/**
  * @brief 禁用 TIM10_CH1 的重映射
  * @note  DISABLE: 无重映射 (TIM10_CH1 在 PB8).
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM10_DISABLE() CLEAR_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM10_REMAP)
#endif

#if defined(AFIO_MAPR2_TIM11_REMAP)
/**
  * @brief 启用 TIM11_CH1 的重映射
  * @note  ENABLE: 重映射     (TIM11_CH1 在 PF7).
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM11_ENABLE()  SET_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM11_REMAP)

/**
  * @brief 禁用 TIM11_CH1 的重映射
  * @note  DISABLE: 无重映射 (TIM11_CH1 在 PB9).
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM11_DISABLE() CLEAR_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM11_REMAP)
#endif

#if defined(AFIO_MAPR2_TIM13_REMAP)

/**
  * @brief 启用 TIM13_CH1 的重映射
  * @note  ENABLE: 重映射     STM32F100:(TIM13_CH1 在 PF8). 其他:(TIM13_CH1 在 PB0).
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM13_ENABLE()  SET_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM13_REMAP)

/**
  * @brief 禁用 TIM13_CH1 的重映射
  * @note  DISABLE: 无重映射 STM32F100:(TIM13_CH1 在 PA6). 其他:(TIM13_CH1 在 PC8).
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM13_DISABLE() CLEAR_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM13_REMAP)
#endif

#if defined(AFIO_MAPR2_TIM14_REMAP)

/**
  * @brief 启用 TIM14_CH1 的重映射
  * @note  ENABLE: 重映射     STM32F100:(TIM14_CH1 在 PB1). 其他(TIM14_CH1 在 PF9).
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM14_ENABLE()  SET_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM14_REMAP)

/**
  * @brief 禁用 TIM14_CH1 的重映射
  * @note  DISABLE: 无重映射 STM32F100:(TIM14_CH1 在 PC9). 其他(TIM14_CH1 在 PA7).
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM14_DISABLE() CLEAR_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM14_REMAP)
#endif

#if defined(AFIO_MAPR2_FSMC_NADV_REMAP)

/**
  * @brief 控制不使用FSMC_NADV信号输出
  * @note  DISCONNECTED: NADV信号未连接。该I/O引脚可以被另一个外设使用。
  * @retval None
  */
#define __HAL_AFIO_FSMCNADV_DISCONNECTED() SET_BIT(AFIO->MAPR2, AFIO_MAPR2_FSMC_NADV_REMAP)

/**
  * @brief 控制使用FSMC_NADV信号输出
  * @note  CONNECTED: NADV信号连接到输出（默认）。
  * @retval None
  */
#define __HAL_AFIO_FSMCNADV_CONNECTED()    CLEAR_BIT(AFIO->MAPR2, AFIO_MAPR2_FSMC_NADV_REMAP)
#endif

#if defined(AFIO_MAPR2_TIM15_REMAP)

/**
  * @brief 启用 TIM15_CH1 和 TIM15_CH2 的重映射
  * @note  ENABLE: 重映射     (TIM15_CH1 在 PB14 和 TIM15_CH2 在 PB15).
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM15_ENABLE()  SET_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM15_REMAP)

/**
  * @brief 禁用 TIM15_CH1 和 TIM15_CH2 的重映射
  * @note  DISABLE: 无重映射 (TIM15_CH1 在 PA2  和 TIM15_CH2 在 PA3).
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM15_DISABLE() CLEAR_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM15_REMAP)
#endif

#if defined(AFIO_MAPR2_TIM16_REMAP)

/**
  * @brief 启用 TIM16_CH1 的重映射
  * @note  ENABLE: 重映射     (TIM16_CH1 在 PA6).
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM16_ENABLE()  SET_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM16_REMAP)

/**
  * @brief 禁用 TIM16_CH1 的重映射
  * @note  DISABLE: 无重映射 (TIM16_CH1 在 PB8).
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM16_DISABLE() CLEAR_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM16_REMAP)
#endif

#if defined(AFIO_MAPR2_TIM17_REMAP)

/**
  * @brief 启用 TIM17_CH1 的重映射
  * @note  ENABLE: 重映射     (TIM17_CH1 在 PA7).
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM17_ENABLE()  SET_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM17_REMAP)

/**
  * @brief 禁用 TIM17_CH1 的重映射
  * @note  DISABLE: 无重映射 (TIM17_CH1 在 PB9).
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM17_DISABLE() CLEAR_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM17_REMAP)
#endif

#if defined(AFIO_MAPR2_CEC_REMAP)

/**
  * @brief 启用 CEC 的重映射
  * @note  ENABLE: 重映射     (CEC 在 PB10).
  * @retval None
  */
#define __HAL_AFIO_REMAP_CEC_ENABLE()  SET_BIT(AFIO->MAPR2, AFIO_MAPR2_CEC_REMAP)

/**
  * @brief 禁用 CEC 的重映射
  * @note  DISABLE: 无重映射 (CEC 在 PB8).
  * @retval None
  */
#define __HAL_AFIO_REMAP_CEC_DISABLE() CLEAR_BIT(AFIO->MAPR2, AFIO_MAPR2_CEC_REMAP)
#endif

#if defined(AFIO_MAPR2_TIM1_DMA_REMAP)

/**
  * @brief 启用TIM1_CH1和TIM1_CH2的DMA请求在DMA1通道上的映射
  * @note  ENABLE: 重映射 (TIM1_CH1 DMA 请求/DMA1 Channel6, TIM1_CH2 DMA 请求/DMA1 Channel6)
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM1DMA_ENABLE()  SET_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM1_DMA_REMAP)

/**
  * @brief 禁用TIM1_CH1和TIM1_CH2的DMA请求在DMA1通道上的重映射
  * @note  DISABLE: 无重映射 (TIM1_CH1 DMA 请求/DMA1 Channel2, TIM1_CH2 DMA 请求/DMA1 Channel3).
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM1DMA_DISABLE() CLEAR_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM1_DMA_REMAP)
#endif

#if defined(AFIO_MAPR2_TIM67_DAC_DMA_REMAP)

/**
  * @brief 启用TIM6_DAC1和TIM7_DAC2 DMA请求在DMA1通道上的重映射
  * @note  ENABLE: 重映射 (TIM6_DAC1 DMA 请求/DMA1 Channel3, TIM7_DAC2 DMA 请求/DMA1 Channel4)
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM67DACDMA_ENABLE()  SET_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM67_DAC_DMA_REMAP)

/**
  * @brief 禁用TIM6_DAC1和TIM7_DAC2 DMA请求在DMA1通道上的重映射
  * @note  DISABLE: 无重映射 (TIM6_DAC1 DMA 请求/DMA2 Channel3, TIM7_DAC2 DMA 请求/DMA2 Channel4)
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM67DACDMA_DISABLE() CLEAR_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM67_DAC_DMA_REMAP)
#endif

#if defined(AFIO_MAPR2_TIM12_REMAP)

/**
  * @brief 启用 TIM12_CH1 和 TIM12_CH2 的重映射
  * @note  ENABLE: 重映射     (TIM12_CH1 在 PB12 和 TIM12_CH2 在 PB13).
  * @note  这个位仅在高密度类型设备中可用
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM12_ENABLE()  SET_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM12_REMAP)

/**
  * @brief 禁用 TIM12_CH1 和 TIM12_CH2 的重映射
  * @note  DISABLE: 无重映射 (TIM12_CH1 在 PC4  和 TIM12_CH2 在 PC5).
  * @note  这个位仅在高密度类型设备中可用
  * @retval None
  */
#define __HAL_AFIO_REMAP_TIM12_DISABLE() CLEAR_BIT(AFIO->MAPR2, AFIO_MAPR2_TIM12_REMAP)
#endif

#if defined(AFIO_MAPR2_MISC_REMAP)

/**
  * @brief 启用 杂项 重映射
  *        这个位由软件设置和清除。它控制杂项功能。
  *        向量表中的DMA2通道5中断位置。
  *        DAC触发器3的定时器选择（TSEL [2：0] = 011，有关详细信息，请参阅DAC_CR寄存器）。
  * @note  ENABLE: DMA2通道5中断被单独映射到位置60，TIM15 TRGO事件被选择为DAC触发器3，TIM15触发TIM1/3。
  * @note  这个位仅在高密度类型设备中可用
  * @retval None
  */
#define __HAL_AFIO_REMAP_MISC_ENABLE()  SET_BIT(AFIO->MAPR2, AFIO_MAPR2_MISC_REMAP)

/**
  * @brief 禁用 杂项 重映射
  *        这个位由软件设置和清除。它控制杂项功能。
  *        向量表中的DMA2通道5中断位置。
  *        DAC触发器3的定时器选择（TSEL [2：0] = 011，有关详细信息，请参阅DAC_CR寄存器）。
  * @note  DISABLE: DMA2通道5中断与DMA2通道4映射在位置59处，TIM5 TRGO事件被选择为DAC触发器3，TIM5触发TIM1/3。
  * @note  这个位仅在高密度类型设备中可用
  * @retval None
  */
#define __HAL_AFIO_REMAP_MISC_DISABLE() CLEAR_BIT(AFIO->MAPR2, AFIO_MAPR2_MISC_REMAP)
#endif

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup GPIOEx_Private_Macros GPIOEx 私有宏
  * @{
  */
#if defined(STM32F101x6) || defined(STM32F102x6) || defined(STM32F102xB) || defined(STM32F103x6)
#define GPIO_GET_INDEX(__GPIOx__) (((__GPIOx__) == (GPIOA))? 0uL :\
                                   ((__GPIOx__) == (GPIOB))? 1uL :\
                                   ((__GPIOx__) == (GPIOC))? 2uL :3uL)
#elif defined(STM32F100xB) || defined(STM32F101xB) || defined(STM32F103xB) || defined(STM32F105xC) || defined(STM32F107xC)
#define GPIO_GET_INDEX(__GPIOx__) (((__GPIOx__) == (GPIOA))? 0uL :\
                                   ((__GPIOx__) == (GPIOB))? 1uL :\
                                   ((__GPIOx__) == (GPIOC))? 2uL :\
                                   ((__GPIOx__) == (GPIOD))? 3uL :4uL)
#elif defined(STM32F100xE) || defined(STM32F101xE) || defined(STM32F101xG) || defined(STM32F103xE) || defined(STM32F103xG)
#define GPIO_GET_INDEX(__GPIOx__) (((__GPIOx__) == (GPIOA))? 0uL :\
                                   ((__GPIOx__) == (GPIOB))? 1uL :\
                                   ((__GPIOx__) == (GPIOC))? 2uL :\
                                   ((__GPIOx__) == (GPIOD))? 3uL :\
                                   ((__GPIOx__) == (GPIOE))? 4uL :\
                                   ((__GPIOx__) == (GPIOF))? 5uL :6uL)
#endif

#define AFIO_REMAP_ENABLE(REMAP_PIN)       do{ uint32_t tmpreg = AFIO->MAPR; \
                                               tmpreg |= AFIO_MAPR_SWJ_CFG;  \
                                               tmpreg |= REMAP_PIN;          \
                                               AFIO->MAPR = tmpreg;          \
                                               }while(0u)

#define AFIO_REMAP_DISABLE(REMAP_PIN)      do{ uint32_t tmpreg = AFIO->MAPR;  \
                                               tmpreg |= AFIO_MAPR_SWJ_CFG;   \
                                               tmpreg &= ~REMAP_PIN;          \
                                               AFIO->MAPR = tmpreg;           \
                                               }while(0u)

#define AFIO_REMAP_PARTIAL(REMAP_PIN, REMAP_PIN_MASK) do{ uint32_t tmpreg = AFIO->MAPR; \
                                                          tmpreg &= ~REMAP_PIN_MASK;    \
                                                          tmpreg |= AFIO_MAPR_SWJ_CFG;  \
                                                          tmpreg |= REMAP_PIN;          \
                                                          AFIO->MAPR = tmpreg;          \
                                                          }while(0u)

#define AFIO_DBGAFR_CONFIG(DBGAFR_SWJCFG)  do{ uint32_t tmpreg = AFIO->MAPR;     \
                                               tmpreg &= ~AFIO_MAPR_SWJ_CFG_Msk; \
                                               tmpreg |= DBGAFR_SWJCFG;          \
                                               AFIO->MAPR = tmpreg;              \
                                               }while(0u)

/**
  * @}
  */

/* 导出宏 ------------------------------------------------------------*/
/* 导出函数 --------------------------------------------------------*/

/** @addtogroup GPIOEx_Exported_Functions
  * @{
  */

/** @addtogroup GPIOEx_Exported_Functions_Group1
  * @{
  */
 
void HAL_GPIOEx_ConfigEventout(uint32_t GPIO_PortSource, uint32_t GPIO_PinSource);
void HAL_GPIOEx_EnableEventout(void);
void HAL_GPIOEx_DisableEventout(void);

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

#endif /* STM32F1xx_HAL_GPIO_EX_H */

