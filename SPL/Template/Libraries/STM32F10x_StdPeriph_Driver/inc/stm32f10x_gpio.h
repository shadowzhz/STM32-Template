/**
  ******************************************************************************
  * @file    stm32f10x_gpio.h
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件包含 GPIO 固件库的所有函数原型。
  ******************************************************************************
  * @attention
  *
  * 本固件仅供指引之用，旨在为客户提供有关其产品的编码信息，
  * 以便客户节省时间。因此，对于因本固件内容及/或客户将本文
  * 所含编码信息用于其产品而产生的任何索赔所导致的任何直接、
  * 间接或后果性损害，STMicroelectronics 概不负责。
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* 定义以下宏，以防止本头文件被递归包含 -------------------------------------*/
#ifndef __STM32F10x_GPIO_H
#define __STM32F10x_GPIO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @addtogroup GPIO  GPIO 驱动模块
  * @{
  */

/** @defgroup GPIO_Exported_Types   GPIO 导出类型
  * @{
  */

#define IS_GPIO_ALL_PERIPH(PERIPH) (((PERIPH) == GPIOA) || \
                                    ((PERIPH) == GPIOB) || \
                                    ((PERIPH) == GPIOC) || \
                                    ((PERIPH) == GPIOD) || \
                                    ((PERIPH) == GPIOE) || \
                                    ((PERIPH) == GPIOF) || \
                                    ((PERIPH) == GPIOG))
                                     
/**
  * @brief  输出最高频率选择
  */

typedef enum
{ 
  GPIO_Speed_10MHz = 1,
  GPIO_Speed_2MHz, 
  GPIO_Speed_50MHz
}GPIOSpeed_TypeDef;
#define IS_GPIO_SPEED(SPEED) (((SPEED) == GPIO_Speed_10MHz) || ((SPEED) == GPIO_Speed_2MHz) || \
                              ((SPEED) == GPIO_Speed_50MHz))

/**
  * @brief  配置模式枚举
  */

typedef enum
{ GPIO_Mode_AIN = 0x0,
  GPIO_Mode_IN_FLOATING = 0x04,
  GPIO_Mode_IPD = 0x28,
  GPIO_Mode_IPU = 0x48,
  GPIO_Mode_Out_OD = 0x14,
  GPIO_Mode_Out_PP = 0x10,
  GPIO_Mode_AF_OD = 0x1C,
  GPIO_Mode_AF_PP = 0x18
}GPIOMode_TypeDef;

#define IS_GPIO_MODE(MODE) (((MODE) == GPIO_Mode_AIN) || ((MODE) == GPIO_Mode_IN_FLOATING) || \
                            ((MODE) == GPIO_Mode_IPD) || ((MODE) == GPIO_Mode_IPU) || \
                            ((MODE) == GPIO_Mode_Out_OD) || ((MODE) == GPIO_Mode_Out_PP) || \
                            ((MODE) == GPIO_Mode_AF_OD) || ((MODE) == GPIO_Mode_AF_PP))

/**
  * @brief  GPIO 初始化结构定义
  */

typedef struct
{
  uint16_t GPIO_Pin;             /*!< 指定要配置的 GPIO 引脚。
                                      该参数可取 @ref GPIO_pins_define 的任意值 */

  GPIOSpeed_TypeDef GPIO_Speed;  /*!< 指定所选引脚的速度。
                                      该参数可取 @ref GPIOSpeed_TypeDef 的值 */

  GPIOMode_TypeDef GPIO_Mode;    /*!< 指定所选引脚的工作模式。
                                      该参数可取 @ref GPIOMode_TypeDef 的值 */
}GPIO_InitTypeDef;


/**
  * @brief  Bit_SET 和 Bit_RESET 枚举
  */

typedef enum
{ Bit_RESET = 0,
  Bit_SET
}BitAction;

#define IS_GPIO_BIT_ACTION(ACTION) (((ACTION) == Bit_RESET) || ((ACTION) == Bit_SET))

/**
  * @}
  */

/** @defgroup GPIO_Exported_Constants   GPIO 导出常量
  * @{
  */

/** @defgroup GPIO_pins_define   GPIO 引脚定义
  * @{
  */

#define GPIO_Pin_0                 ((uint16_t)0x0001)  /*!< 已选择引脚 0 */
#define GPIO_Pin_1                 ((uint16_t)0x0002)  /*!< 已选择引脚 1 */
#define GPIO_Pin_2                 ((uint16_t)0x0004)  /*!< 已选择引脚 2 */
#define GPIO_Pin_3                 ((uint16_t)0x0008)  /*!< 已选择引脚 3 */
#define GPIO_Pin_4                 ((uint16_t)0x0010)  /*!< 已选择引脚 4 */
#define GPIO_Pin_5                 ((uint16_t)0x0020)  /*!< 已选择引脚 5 */
#define GPIO_Pin_6                 ((uint16_t)0x0040)  /*!< 已选择引脚 6 */
#define GPIO_Pin_7                 ((uint16_t)0x0080)  /*!< 已选择引脚 7 */
#define GPIO_Pin_8                 ((uint16_t)0x0100)  /*!< 已选择引脚 8 */
#define GPIO_Pin_9                 ((uint16_t)0x0200)  /*!< 已选择引脚 9 */
#define GPIO_Pin_10                ((uint16_t)0x0400)  /*!< 已选择引脚 10 */
#define GPIO_Pin_11                ((uint16_t)0x0800)  /*!< 已选择引脚 11 */
#define GPIO_Pin_12                ((uint16_t)0x1000)  /*!< 已选择引脚 12 */
#define GPIO_Pin_13                ((uint16_t)0x2000)  /*!< 已选择引脚 13 */
#define GPIO_Pin_14                ((uint16_t)0x4000)  /*!< 已选择引脚 14 */
#define GPIO_Pin_15                ((uint16_t)0x8000)  /*!< 已选择引脚 15 */
#define GPIO_Pin_All               ((uint16_t)0xFFFF)  /*!< 已选择所有引脚 */

#define IS_GPIO_PIN(PIN) ((((PIN) & (uint16_t)0x00) == 0x00) && ((PIN) != (uint16_t)0x00))

#define IS_GET_GPIO_PIN(PIN) (((PIN) == GPIO_Pin_0) || \
                              ((PIN) == GPIO_Pin_1) || \
                              ((PIN) == GPIO_Pin_2) || \
                              ((PIN) == GPIO_Pin_3) || \
                              ((PIN) == GPIO_Pin_4) || \
                              ((PIN) == GPIO_Pin_5) || \
                              ((PIN) == GPIO_Pin_6) || \
                              ((PIN) == GPIO_Pin_7) || \
                              ((PIN) == GPIO_Pin_8) || \
                              ((PIN) == GPIO_Pin_9) || \
                              ((PIN) == GPIO_Pin_10) || \
                              ((PIN) == GPIO_Pin_11) || \
                              ((PIN) == GPIO_Pin_12) || \
                              ((PIN) == GPIO_Pin_13) || \
                              ((PIN) == GPIO_Pin_14) || \
                              ((PIN) == GPIO_Pin_15))

/**
  * @}
  */

/** @defgroup GPIO_Remap_define   GPIO 重映射定义
  * @{
  */

#define GPIO_Remap_SPI1             ((uint32_t)0x00000001)  /*!< SPI1 复用功能映射 */
#define GPIO_Remap_I2C1             ((uint32_t)0x00000002)  /*!< I2C1 复用功能映射 */
#define GPIO_Remap_USART1           ((uint32_t)0x00000004)  /*!< USART1 复用功能映射 */
#define GPIO_Remap_USART2           ((uint32_t)0x00000008)  /*!< USART2 复用功能映射 */
#define GPIO_PartialRemap_USART3    ((uint32_t)0x00140010)  /*!< USART3 部分复用功能映射 */
#define GPIO_FullRemap_USART3       ((uint32_t)0x00140030)  /*!< USART3 完全复用功能映射 */
#define GPIO_PartialRemap_TIM1      ((uint32_t)0x00160040)  /*!< TIM1 部分复用功能映射 */
#define GPIO_FullRemap_TIM1         ((uint32_t)0x001600C0)  /*!< TIM1 完全复用功能映射 */
#define GPIO_PartialRemap1_TIM2     ((uint32_t)0x00180100)  /*!< TIM2 部分复用功能映射 1 */
#define GPIO_PartialRemap2_TIM2     ((uint32_t)0x00180200)  /*!< TIM2 部分复用功能映射 2 */
#define GPIO_FullRemap_TIM2         ((uint32_t)0x00180300)  /*!< TIM2 完全复用功能映射 */
#define GPIO_PartialRemap_TIM3      ((uint32_t)0x001A0800)  /*!< TIM3 部分复用功能映射 */
#define GPIO_FullRemap_TIM3         ((uint32_t)0x001A0C00)  /*!< TIM3 完全复用功能映射 */
#define GPIO_Remap_TIM4             ((uint32_t)0x00001000)  /*!< TIM4 复用功能映射 */
#define GPIO_Remap1_CAN1            ((uint32_t)0x001D4000)  /*!< CAN1 复用功能映射 */
#define GPIO_Remap2_CAN1            ((uint32_t)0x001D6000)  /*!< CAN1 复用功能映射 */
#define GPIO_Remap_PD01             ((uint32_t)0x00008000)  /*!< PD01 复用功能映射 */
#define GPIO_Remap_TIM5CH4_LSI      ((uint32_t)0x00200001)  /*!< LSI 连接到 TIM5 通道 4 输入捕获用于校准 */
#define GPIO_Remap_ADC1_ETRGINJ     ((uint32_t)0x00200002)  /*!< ADC1 外部触发注入转换重映射 */
#define GPIO_Remap_ADC1_ETRGREG     ((uint32_t)0x00200004)  /*!< ADC1 外部触发规则转换重映射 */
#define GPIO_Remap_ADC2_ETRGINJ     ((uint32_t)0x00200008)  /*!< ADC2 外部触发注入转换重映射 */
#define GPIO_Remap_ADC2_ETRGREG     ((uint32_t)0x00200010)  /*!< ADC2 外部触发规则转换重映射 */
#define GPIO_Remap_ETH              ((uint32_t)0x00200020)  /*!< 以太网重映射（仅适用于互联型器件） */
#define GPIO_Remap_CAN2             ((uint32_t)0x00200040)  /*!< CAN2 重映射（仅适用于互联型器件） */
#define GPIO_Remap_SWJ_NoJTRST      ((uint32_t)0x00300100)  /*!< 使能全部 SWJ（JTAG-DP + SW-DP）但不含 JTRST */
#define GPIO_Remap_SWJ_JTAGDisable  ((uint32_t)0x00300200)  /*!< 关闭 JTAG-DP 并使能 SW-DP */
#define GPIO_Remap_SWJ_Disable      ((uint32_t)0x00300400)  /*!< 关闭全部 SWJ（JTAG-DP + SW-DP） */
#define GPIO_Remap_SPI3             ((uint32_t)0x00201100)  /*!< SPI3/I2S3 复用功能映射（仅适用于互联型器件） */
#define GPIO_Remap_TIM2ITR1_PTP_SOF ((uint32_t)0x00202000)  /*!< 以太网 PTP 输出或 USB OTG SOF（帧起始）连接到
                                                                 TIM2 内部触发 1 用于校准
                                                                 （仅适用于互联型器件） */
#define GPIO_Remap_PTP_PPS          ((uint32_t)0x00204000)  /*!< PB05 上的以太网 MAC PPS_PTS 输出（仅适用于互联型器件） */

#define GPIO_Remap_TIM15            ((uint32_t)0x80000001)  /*!< TIM15 复用功能映射（仅适用于超值型器件） */
#define GPIO_Remap_TIM16            ((uint32_t)0x80000002)  /*!< TIM16 复用功能映射（仅适用于超值型器件） */
#define GPIO_Remap_TIM17            ((uint32_t)0x80000004)  /*!< TIM17 复用功能映射（仅适用于超值型器件） */
#define GPIO_Remap_CEC              ((uint32_t)0x80000008)  /*!< CEC 复用功能映射（仅适用于超值型器件） */
#define GPIO_Remap_TIM1_DMA         ((uint32_t)0x80000010)  /*!< TIM1 DMA 请求映射（仅适用于超值型器件） */

#define GPIO_Remap_TIM9             ((uint32_t)0x80000020)  /*!< TIM9 复用功能映射（仅适用于超大容量器件） */
#define GPIO_Remap_TIM10            ((uint32_t)0x80000040)  /*!< TIM10 复用功能映射（仅适用于超大容量器件） */
#define GPIO_Remap_TIM11            ((uint32_t)0x80000080)  /*!< TIM11 复用功能映射（仅适用于超大容量器件） */
#define GPIO_Remap_TIM13            ((uint32_t)0x80000100)  /*!< TIM13 复用功能映射（仅适用于高容量超值型和超大容量器件） */
#define GPIO_Remap_TIM14            ((uint32_t)0x80000200)  /*!< TIM14 复用功能映射（仅适用于高容量超值型和超大容量器件） */
#define GPIO_Remap_FSMC_NADV        ((uint32_t)0x80000400)  /*!< FSMC_NADV 复用功能映射（仅适用于高容量超值型和超大容量器件） */

#define GPIO_Remap_TIM67_DAC_DMA    ((uint32_t)0x80000800)  /*!< TIM6/TIM7 和 DAC DMA 请求重映射（仅适用于高容量超值型器件） */
#define GPIO_Remap_TIM12            ((uint32_t)0x80001000)  /*!< TIM12 复用功能映射（仅适用于高容量超值型器件） */
#define GPIO_Remap_MISC             ((uint32_t)0x80002000)  /*!< 杂项重映射（DMA2 通道 5 位置和 DAC 触发重映射，
                                                                 仅适用于高容量超值型器件） */                                                       

#define IS_GPIO_REMAP(REMAP) (((REMAP) == GPIO_Remap_SPI1) || ((REMAP) == GPIO_Remap_I2C1) || \
                              ((REMAP) == GPIO_Remap_USART1) || ((REMAP) == GPIO_Remap_USART2) || \
                              ((REMAP) == GPIO_PartialRemap_USART3) || ((REMAP) == GPIO_FullRemap_USART3) || \
                              ((REMAP) == GPIO_PartialRemap_TIM1) || ((REMAP) == GPIO_FullRemap_TIM1) || \
                              ((REMAP) == GPIO_PartialRemap1_TIM2) || ((REMAP) == GPIO_PartialRemap2_TIM2) || \
                              ((REMAP) == GPIO_FullRemap_TIM2) || ((REMAP) == GPIO_PartialRemap_TIM3) || \
                              ((REMAP) == GPIO_FullRemap_TIM3) || ((REMAP) == GPIO_Remap_TIM4) || \
                              ((REMAP) == GPIO_Remap1_CAN1) || ((REMAP) == GPIO_Remap2_CAN1) || \
                              ((REMAP) == GPIO_Remap_PD01) || ((REMAP) == GPIO_Remap_TIM5CH4_LSI) || \
                              ((REMAP) == GPIO_Remap_ADC1_ETRGINJ) ||((REMAP) == GPIO_Remap_ADC1_ETRGREG) || \
                              ((REMAP) == GPIO_Remap_ADC2_ETRGINJ) ||((REMAP) == GPIO_Remap_ADC2_ETRGREG) || \
                              ((REMAP) == GPIO_Remap_ETH) ||((REMAP) == GPIO_Remap_CAN2) || \
                              ((REMAP) == GPIO_Remap_SWJ_NoJTRST) || ((REMAP) == GPIO_Remap_SWJ_JTAGDisable) || \
                              ((REMAP) == GPIO_Remap_SWJ_Disable)|| ((REMAP) == GPIO_Remap_SPI3) || \
                              ((REMAP) == GPIO_Remap_TIM2ITR1_PTP_SOF) || ((REMAP) == GPIO_Remap_PTP_PPS) || \
                              ((REMAP) == GPIO_Remap_TIM15) || ((REMAP) == GPIO_Remap_TIM16) || \
                              ((REMAP) == GPIO_Remap_TIM17) || ((REMAP) == GPIO_Remap_CEC) || \
                              ((REMAP) == GPIO_Remap_TIM1_DMA) || ((REMAP) == GPIO_Remap_TIM9) || \
                              ((REMAP) == GPIO_Remap_TIM10) || ((REMAP) == GPIO_Remap_TIM11) || \
                              ((REMAP) == GPIO_Remap_TIM13) || ((REMAP) == GPIO_Remap_TIM14) || \
                              ((REMAP) == GPIO_Remap_FSMC_NADV) || ((REMAP) == GPIO_Remap_TIM67_DAC_DMA) || \
                              ((REMAP) == GPIO_Remap_TIM12) || ((REMAP) == GPIO_Remap_MISC))
                              
/**
  * @}
  */ 

/** @defgroup GPIO_Port_Sources   GPIO 端口源
  * @{
  */

#define GPIO_PortSourceGPIOA       ((uint8_t)0x00)
#define GPIO_PortSourceGPIOB       ((uint8_t)0x01)
#define GPIO_PortSourceGPIOC       ((uint8_t)0x02)
#define GPIO_PortSourceGPIOD       ((uint8_t)0x03)
#define GPIO_PortSourceGPIOE       ((uint8_t)0x04)
#define GPIO_PortSourceGPIOF       ((uint8_t)0x05)
#define GPIO_PortSourceGPIOG       ((uint8_t)0x06)
#define IS_GPIO_EVENTOUT_PORT_SOURCE(PORTSOURCE) (((PORTSOURCE) == GPIO_PortSourceGPIOA) || \
                                                  ((PORTSOURCE) == GPIO_PortSourceGPIOB) || \
                                                  ((PORTSOURCE) == GPIO_PortSourceGPIOC) || \
                                                  ((PORTSOURCE) == GPIO_PortSourceGPIOD) || \
                                                  ((PORTSOURCE) == GPIO_PortSourceGPIOE))

#define IS_GPIO_EXTI_PORT_SOURCE(PORTSOURCE) (((PORTSOURCE) == GPIO_PortSourceGPIOA) || \
                                              ((PORTSOURCE) == GPIO_PortSourceGPIOB) || \
                                              ((PORTSOURCE) == GPIO_PortSourceGPIOC) || \
                                              ((PORTSOURCE) == GPIO_PortSourceGPIOD) || \
                                              ((PORTSOURCE) == GPIO_PortSourceGPIOE) || \
                                              ((PORTSOURCE) == GPIO_PortSourceGPIOF) || \
                                              ((PORTSOURCE) == GPIO_PortSourceGPIOG))

/**
  * @}
  */

/** @defgroup GPIO_Pin_sources   GPIO 引脚源
  * @{
  */

#define GPIO_PinSource0            ((uint8_t)0x00)
#define GPIO_PinSource1            ((uint8_t)0x01)
#define GPIO_PinSource2            ((uint8_t)0x02)
#define GPIO_PinSource3            ((uint8_t)0x03)
#define GPIO_PinSource4            ((uint8_t)0x04)
#define GPIO_PinSource5            ((uint8_t)0x05)
#define GPIO_PinSource6            ((uint8_t)0x06)
#define GPIO_PinSource7            ((uint8_t)0x07)
#define GPIO_PinSource8            ((uint8_t)0x08)
#define GPIO_PinSource9            ((uint8_t)0x09)
#define GPIO_PinSource10           ((uint8_t)0x0A)
#define GPIO_PinSource11           ((uint8_t)0x0B)
#define GPIO_PinSource12           ((uint8_t)0x0C)
#define GPIO_PinSource13           ((uint8_t)0x0D)
#define GPIO_PinSource14           ((uint8_t)0x0E)
#define GPIO_PinSource15           ((uint8_t)0x0F)

#define IS_GPIO_PIN_SOURCE(PINSOURCE) (((PINSOURCE) == GPIO_PinSource0) || \
                                       ((PINSOURCE) == GPIO_PinSource1) || \
                                       ((PINSOURCE) == GPIO_PinSource2) || \
                                       ((PINSOURCE) == GPIO_PinSource3) || \
                                       ((PINSOURCE) == GPIO_PinSource4) || \
                                       ((PINSOURCE) == GPIO_PinSource5) || \
                                       ((PINSOURCE) == GPIO_PinSource6) || \
                                       ((PINSOURCE) == GPIO_PinSource7) || \
                                       ((PINSOURCE) == GPIO_PinSource8) || \
                                       ((PINSOURCE) == GPIO_PinSource9) || \
                                       ((PINSOURCE) == GPIO_PinSource10) || \
                                       ((PINSOURCE) == GPIO_PinSource11) || \
                                       ((PINSOURCE) == GPIO_PinSource12) || \
                                       ((PINSOURCE) == GPIO_PinSource13) || \
                                       ((PINSOURCE) == GPIO_PinSource14) || \
                                       ((PINSOURCE) == GPIO_PinSource15))

/**
  * @}
  */

/** @defgroup Ethernet_Media_Interface   以太网媒体接口
  * @{
  */ 
#define GPIO_ETH_MediaInterface_MII    ((u32)0x00000000) 
#define GPIO_ETH_MediaInterface_RMII   ((u32)0x00000001)                                       

#define IS_GPIO_ETH_MEDIA_INTERFACE(INTERFACE) (((INTERFACE) == GPIO_ETH_MediaInterface_MII) || \
                                                ((INTERFACE) == GPIO_ETH_MediaInterface_RMII))

/**
  * @}
  */                                                
/**
  * @}
  */

/** @defgroup GPIO_Exported_Macros   GPIO 导出宏
  * @{
  */

/**
  * @}
  */

/** @defgroup GPIO_Exported_Functions   GPIO 导出函数
  * @{
  */

void GPIO_DeInit(GPIO_TypeDef* GPIOx);
void GPIO_AFIODeInit(void);
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct);
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);
uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal);
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal);
void GPIO_PinLockConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_EventOutputConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource);
void GPIO_EventOutputCmd(FunctionalState NewState);
void GPIO_PinRemapConfig(uint32_t GPIO_Remap, FunctionalState NewState);
void GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource);
void GPIO_ETH_MediaInterfaceConfig(uint32_t GPIO_ETH_MediaInterface);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_GPIO_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****文件结束****/
