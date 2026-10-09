/**
  ******************************************************************************
  * @file    stm32f10x_gpio.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件提供所有 GPIO 固件函数。
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

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @defgroup GPIO
  * @brief GPIO 驱动模块
  * @{
  */ 

/** @defgroup GPIO_Private_TypesDefinitions   GPIO 私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup GPIO_Private_Defines   GPIO 私有宏定义
  * @{
  */

/* ------------ 别名区中 RCC 寄存器位地址 ----------------*/
#define AFIO_OFFSET                 (AFIO_BASE - PERIPH_BASE)

/* --- EVENTCR 寄存器 -----*/

/* EVOE 位的别名字地址 */
#define EVCR_OFFSET                 (AFIO_OFFSET + 0x00)
#define EVOE_BitNumber              ((uint8_t)0x07)
#define EVCR_EVOE_BB                (PERIPH_BB_BASE + (EVCR_OFFSET * 32) + (EVOE_BitNumber * 4))


/* ---  MAPR 寄存器 ---*/ 
/* MII_RMII_SEL 位的别名字地址 */ 
#define MAPR_OFFSET                 (AFIO_OFFSET + 0x04) 
#define MII_RMII_SEL_BitNumber      ((u8)0x17) 
#define MAPR_MII_RMII_SEL_BB        (PERIPH_BB_BASE + (MAPR_OFFSET * 32) + (MII_RMII_SEL_BitNumber * 4))


#define EVCR_PORTPINCONFIG_MASK     ((uint16_t)0xFF80)
#define LSB_MASK                    ((uint16_t)0xFFFF)
#define DBGAFR_POSITION_MASK        ((uint32_t)0x000F0000)
#define DBGAFR_SWJCFG_MASK          ((uint32_t)0xF0FFFFFF)
#define DBGAFR_LOCATION_MASK        ((uint32_t)0x00200000)
#define DBGAFR_NUMBITS_MASK         ((uint32_t)0x00100000)
/**
  * @}
  */

/** @defgroup GPIO_Private_Macros   GPIO 私有宏
  * @{
  */

/**
  * @}
  */

/** @defgroup GPIO_Private_Variables   GPIO 私有变量
  * @{
  */

/**
  * @}
  */

/** @defgroup GPIO_Private_FunctionPrototypes   GPIO 私有函数原型
  * @{
  */

/**
  * @}
  */

/** @defgroup GPIO_Private_Functions   GPIO 私有函数
  * @{
  */

/**
  * @brief  将 GPIOx 外设寄存器反初始化为其默认复位值。
  * @param  GPIOx: x 可取 (A..G)，用于选择 GPIO 外设。
  * @retval 无
  */
void GPIO_DeInit(GPIO_TypeDef* GPIOx)
{
  /* 检查参数 */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  
  if (GPIOx == GPIOA)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA, DISABLE);
  }
  else if (GPIOx == GPIOB)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOB, DISABLE);
  }
  else if (GPIOx == GPIOC)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOC, DISABLE);
  }
  else if (GPIOx == GPIOD)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOD, DISABLE);
  }    
  else if (GPIOx == GPIOE)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOE, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOE, DISABLE);
  } 
  else if (GPIOx == GPIOF)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOF, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOF, DISABLE);
  }
  else
  {
    if (GPIOx == GPIOG)
    {
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOG, ENABLE);
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOG, DISABLE);
    }
  }
}

/**
  * @brief  将复用功能（重映射、事件控制和
  *   EXTI 配置）寄存器反初始化为其默认复位值。
  * @param  无
  * @retval 无
  */
void GPIO_AFIODeInit(void)
{
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, DISABLE);
}

/**
  * @brief  根据 GPIO_InitStruct 中指定的参数
  *         初始化 GPIOx 外设。
  * @param  GPIOx: x 可取 (A..G)，用于选择 GPIO 外设。
  * @param  GPIO_InitStruct: 指向 GPIO_InitTypeDef 结构的指针，
  *         该结构包含指定 GPIO 外设的配置信息。
  * @retval 无
  */
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)
{
  uint32_t currentmode = 0x00, currentpin = 0x00, pinpos = 0x00, pos = 0x00;
  uint32_t tmpreg = 0x00, pinmask = 0x00;
  /* 检查参数 */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_MODE(GPIO_InitStruct->GPIO_Mode));
  assert_param(IS_GPIO_PIN(GPIO_InitStruct->GPIO_Pin));  
  
/*---------------------------- GPIO 模式配置 -----------------------*/
  currentmode = ((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x0F);
  if ((((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x10)) != 0x00)
  { 
    /* 检查参数 */
    assert_param(IS_GPIO_SPEED(GPIO_InitStruct->GPIO_Speed));
    /* 输出模式 */
    currentmode |= (uint32_t)GPIO_InitStruct->GPIO_Speed;
  }
/*---------------------------- GPIO CRL 配置 ------------------------*/
  /* 配置 8 个低端端口引脚 */
  if (((uint32_t)GPIO_InitStruct->GPIO_Pin & ((uint32_t)0x00FF)) != 0x00)
  {
    tmpreg = GPIOx->CRL;
    for (pinpos = 0x00; pinpos < 0x08; pinpos++)
    {
      pos = ((uint32_t)0x01) << pinpos;
      /* 获取端口引脚位置 */
      currentpin = (GPIO_InitStruct->GPIO_Pin) & pos;
      if (currentpin == pos)
      {
        pos = pinpos << 2;
        /* 清零相应的低控制寄存器位 */
        pinmask = ((uint32_t)0x0F) << pos;
        tmpreg &= ~pinmask;
        /* 在相应位中写入模式配置 */
        tmpreg |= (currentmode << pos);
        /* 清零相应的 ODR 位 */
        if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPD)
        {
          GPIOx->BRR = (((uint32_t)0x01) << pinpos);
        }
        else
        {
          /* 置位相应的 ODR 位 */
          if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPU)
          {
            GPIOx->BSRR = (((uint32_t)0x01) << pinpos);
          }
        }
      }
    }
    GPIOx->CRL = tmpreg;
  }
/*---------------------------- GPIO CRH 配置 ------------------------*/
  /* 配置 8 个高端端口引脚 */
  if (GPIO_InitStruct->GPIO_Pin > 0x00FF)
  {
    tmpreg = GPIOx->CRH;
    for (pinpos = 0x00; pinpos < 0x08; pinpos++)
    {
      pos = (((uint32_t)0x01) << (pinpos + 0x08));
      /* 获取端口引脚位置 */
      currentpin = ((GPIO_InitStruct->GPIO_Pin) & pos);
      if (currentpin == pos)
      {
        pos = pinpos << 2;
        /* 清零相应的高控制寄存器位 */
        pinmask = ((uint32_t)0x0F) << pos;
        tmpreg &= ~pinmask;
        /* 在相应位中写入模式配置 */
        tmpreg |= (currentmode << pos);
        /* 清零相应的 ODR 位 */
        if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPD)
        {
          GPIOx->BRR = (((uint32_t)0x01) << (pinpos + 0x08));
        }
        /* 置位相应的 ODR 位 */
        if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPU)
        {
          GPIOx->BSRR = (((uint32_t)0x01) << (pinpos + 0x08));
        }
      }
    }
    GPIOx->CRH = tmpreg;
  }
}

/**
  * @brief  将 GPIO_InitStruct 的每个成员填充为默认值。
  * @param  GPIO_InitStruct : 指向将被初始化的 GPIO_InitTypeDef
  *         结构的指针。
  * @retval 无
  */
void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct)
{
  /* 复位 GPIO 初始化结构的参数值 */
  GPIO_InitStruct->GPIO_Pin  = GPIO_Pin_All;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IN_FLOATING;
}

/**
  * @brief  读取指定的输入端口引脚。
  * @param  GPIOx: x 可取 (A..G)，用于选择 GPIO 外设。
  * @param  GPIO_Pin:  指定要读取的端口位。
  *   该参数可取 GPIO_Pin_x，x 可取 (0..15)。
  * @retval 输入端口引脚值。
  */
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  uint8_t bitstatus = 0x00;
  
  /* 检查参数 */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin)); 
  
  if ((GPIOx->IDR & GPIO_Pin) != (uint32_t)Bit_RESET)
  {
    bitstatus = (uint8_t)Bit_SET;
  }
  else
  {
    bitstatus = (uint8_t)Bit_RESET;
  }
  return bitstatus;
}

/**
  * @brief  读取指定的 GPIO 输入数据端口。
  * @param  GPIOx: x 可取 (A..G)，用于选择 GPIO 外设。
  * @retval GPIO 输入数据端口值。
  */
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx)
{
  /* 检查参数 */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  
  return ((uint16_t)GPIOx->IDR);
}

/**
  * @brief  读取指定的输出数据端口位。
  * @param  GPIOx: x 可取 (A..G)，用于选择 GPIO 外设。
  * @param  GPIO_Pin:  指定要读取的端口位。
  *   该参数可取 GPIO_Pin_x，x 可取 (0..15)。
  * @retval 输出端口引脚值。
  */
uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  uint8_t bitstatus = 0x00;
  /* 检查参数 */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin)); 
  
  if ((GPIOx->ODR & GPIO_Pin) != (uint32_t)Bit_RESET)
  {
    bitstatus = (uint8_t)Bit_SET;
  }
  else
  {
    bitstatus = (uint8_t)Bit_RESET;
  }
  return bitstatus;
}

/**
  * @brief  读取指定的 GPIO 输出数据端口。
  * @param  GPIOx: x 可取 (A..G)，用于选择 GPIO 外设。
  * @retval GPIO 输出数据端口值。
  */
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx)
{
  /* 检查参数 */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    
  return ((uint16_t)GPIOx->ODR);
}

/**
  * @brief  置位选定的数据端口位。
  * @param  GPIOx: x 可取 (A..G)，用于选择 GPIO 外设。
  * @param  GPIO_Pin: 指定要写入的端口位。
  *   该参数可为 GPIO_Pin_x 的任意组合，x 可取 (0..15)。
  * @retval 无
  */
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  /* 检查参数 */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  GPIOx->BSRR = GPIO_Pin;
}

/**
  * @brief  清零选定的数据端口位。
  * @param  GPIOx: x 可取 (A..G)，用于选择 GPIO 外设。
  * @param  GPIO_Pin: 指定要写入的端口位。
  *   该参数可为 GPIO_Pin_x 的任意组合，x 可取 (0..15)。
  * @retval 无
  */
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  /* 检查参数 */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  GPIOx->BRR = GPIO_Pin;
}

/**
  * @brief  置位或清零选定的数据端口位。
  * @param  GPIOx: x 可取 (A..G)，用于选择 GPIO 外设。
  * @param  GPIO_Pin: 指定要写入的端口位。
  *   该参数可取 GPIO_Pin_x 之一，x 可取 (0..15)。
  * @param  BitVal: 指定要写入所选位的值。
  *   该参数可取 BitAction 枚举值之一：
  *     @arg Bit_RESET: 清零端口引脚
  *     @arg Bit_SET: 置位端口引脚
  * @retval 无
  */
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal)
{
  /* 检查参数 */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin));
  assert_param(IS_GPIO_BIT_ACTION(BitVal)); 
  
  if (BitVal != Bit_RESET)
  {
    GPIOx->BSRR = GPIO_Pin;
  }
  else
  {
    GPIOx->BRR = GPIO_Pin;
  }
}

/**
  * @brief  向指定的 GPIO 数据端口写入数据。
  * @param  GPIOx: x 可取 (A..G)，用于选择 GPIO 外设。
  * @param  PortVal: 指定要写入端口输出数据寄存器的值。
  * @retval 无
  */
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal)
{
  /* 检查参数 */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  
  GPIOx->ODR = PortVal;
}

/**
  * @brief  锁定 GPIO 引脚配置寄存器。
  * @param  GPIOx: x 可取 (A..G)，用于选择 GPIO 外设。
  * @param  GPIO_Pin: 指定要写入的端口位。
  *   该参数可为 GPIO_Pin_x 的任意组合，x 可取 (0..15)。
  * @retval 无
  */
void GPIO_PinLockConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  uint32_t tmp = 0x00010000;
  
  /* 检查参数 */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  tmp |= GPIO_Pin;
  /* 置位 LCKK 位 */
  GPIOx->LCKR = tmp;
  /* 清零 LCKK 位 */
  GPIOx->LCKR =  GPIO_Pin;
  /* 置位 LCKK 位 */
  GPIOx->LCKR = tmp;
  /* 读取 LCKK 位*/
  tmp = GPIOx->LCKR;
  /* 读取 LCKK 位*/
  tmp = GPIOx->LCKR;
}

/**
  * @brief  选择用作事件输出的 GPIO 引脚。
  * @param  GPIO_PortSource: 选择用作事件输出源的
  *   GPIO 端口。
  *   该参数可取 GPIO_PortSourceGPIOx，x 可取 (A..E)。
  * @param  GPIO_PinSource: 指定用于事件输出的引脚。
  *   该参数可取 GPIO_PinSourcex，x 可取 (0..15)。
  * @retval 无
  */
void GPIO_EventOutputConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource)
{
  uint32_t tmpreg = 0x00;
  /* 检查参数 */
  assert_param(IS_GPIO_EVENTOUT_PORT_SOURCE(GPIO_PortSource));
  assert_param(IS_GPIO_PIN_SOURCE(GPIO_PinSource));
    
  tmpreg = AFIO->EVCR;
  /* 清零 PORT[6:4] 和 PIN[3:0] 位 */
  tmpreg &= EVCR_PORTPINCONFIG_MASK;
  tmpreg |= (uint32_t)GPIO_PortSource << 0x04;
  tmpreg |= GPIO_PinSource;
  AFIO->EVCR = tmpreg;
}

/**
  * @brief  使能或关闭事件输出。
  * @param  NewState: 事件输出的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void GPIO_EventOutputCmd(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(__IO uint32_t *) EVCR_EVOE_BB = (uint32_t)NewState;
}

/**
  * @brief  更改指定引脚的映射。
  * @param  GPIO_Remap: 选择要重映射的引脚。
  *   该参数可取以下值之一：
  *     @arg GPIO_Remap_SPI1             : SPI1 复用功能映射
  *     @arg GPIO_Remap_I2C1             : I2C1 复用功能映射
  *     @arg GPIO_Remap_USART1           : USART1 复用功能映射
  *     @arg GPIO_Remap_USART2           : USART2 复用功能映射
  *     @arg GPIO_PartialRemap_USART3    : USART3 部分复用功能映射
  *     @arg GPIO_FullRemap_USART3       : USART3 完全复用功能映射
  *     @arg GPIO_PartialRemap_TIM1      : TIM1 部分复用功能映射
  *     @arg GPIO_FullRemap_TIM1         : TIM1 完全复用功能映射
  *     @arg GPIO_PartialRemap1_TIM2     : TIM2 部分复用功能映射 1
  *     @arg GPIO_PartialRemap2_TIM2     : TIM2 部分复用功能映射 2
  *     @arg GPIO_FullRemap_TIM2         : TIM2 完全复用功能映射
  *     @arg GPIO_PartialRemap_TIM3      : TIM3 部分复用功能映射
  *     @arg GPIO_FullRemap_TIM3         : TIM3 完全复用功能映射
  *     @arg GPIO_Remap_TIM4             : TIM4 复用功能映射
  *     @arg GPIO_Remap1_CAN1            : CAN1 复用功能映射
  *     @arg GPIO_Remap2_CAN1            : CAN1 复用功能映射
  *     @arg GPIO_Remap_PD01             : PD01 复用功能映射
  *     @arg GPIO_Remap_TIM5CH4_LSI      : LSI 连接到 TIM5 通道 4 输入捕获用于校准
  *     @arg GPIO_Remap_ADC1_ETRGINJ     : ADC1 外部触发注入转换重映射
  *     @arg GPIO_Remap_ADC1_ETRGREG     : ADC1 外部触发规则转换重映射
  *     @arg GPIO_Remap_ADC2_ETRGINJ     : ADC2 外部触发注入转换重映射
  *     @arg GPIO_Remap_ADC2_ETRGREG     : ADC2 外部触发规则转换重映射
  *     @arg GPIO_Remap_ETH              : 以太网重映射（仅适用于互联型器件）
  *     @arg GPIO_Remap_CAN2             : CAN2 重映射（仅适用于互联型器件）
  *     @arg GPIO_Remap_SWJ_NoJTRST      : 使能全部 SWJ（JTAG-DP + SW-DP）但不含 JTRST
  *     @arg GPIO_Remap_SWJ_JTAGDisable  : 关闭 JTAG-DP 并使能 SW-DP
  *     @arg GPIO_Remap_SWJ_Disable      : 关闭全部 SWJ（JTAG-DP + SW-DP）
  *     @arg GPIO_Remap_SPI3             : SPI3/I2S3 复用功能映射（仅适用于互联型器件）
  *                                        当使用本函数重映射 SPI3/I2S3 时，SWJ 被配置为
  *                                        使能全部 SWJ（JTAG-DP + SW-DP）但不含 JTRST。
  *     @arg GPIO_Remap_TIM2ITR1_PTP_SOF : 以太网 PTP 输出或 USB OTG SOF（帧起始）连接到
  *                                        TIM2 内部触发 1 用于校准（仅适用于互联型器件）
  *                                        若使能 GPIO_Remap_TIM2ITR1_PTP_SOF，则 TIM2 ITR1 连接到
  *                                        以太网 PTP 输出。复位时 TIM2 ITR1 连接到 USB OTG SOF 输出。
  *     @arg GPIO_Remap_PTP_PPS          : PB05 上的以太网 MAC PPS_PTS 输出（仅适用于互联型器件）
  *     @arg GPIO_Remap_TIM15            : TIM15 复用功能映射（仅适用于超值型器件）
  *     @arg GPIO_Remap_TIM16            : TIM16 复用功能映射（仅适用于超值型器件）
  *     @arg GPIO_Remap_TIM17            : TIM17 复用功能映射（仅适用于超值型器件）
  *     @arg GPIO_Remap_CEC              : CEC 复用功能映射（仅适用于超值型器件）
  *     @arg GPIO_Remap_TIM1_DMA         : TIM1 DMA 请求映射（仅适用于超值型器件）
  *     @arg GPIO_Remap_TIM9             : TIM9 复用功能映射（仅适用于超大容量器件）
  *     @arg GPIO_Remap_TIM10            : TIM10 复用功能映射（仅适用于超大容量器件）
  *     @arg GPIO_Remap_TIM11            : TIM11 复用功能映射（仅适用于超大容量器件）
  *     @arg GPIO_Remap_TIM13            : TIM13 复用功能映射（仅适用于高容量超值型和超大容量器件）
  *     @arg GPIO_Remap_TIM14            : TIM14 复用功能映射（仅适用于高容量超值型和超大容量器件）
  *     @arg GPIO_Remap_FSMC_NADV        : FSMC_NADV 复用功能映射（仅适用于高容量超值型和超大容量器件）
  *     @arg GPIO_Remap_TIM67_DAC_DMA    : TIM6/TIM7 和 DAC DMA 请求重映射（仅适用于高容量超值型器件）
  *     @arg GPIO_Remap_TIM12            : TIM12 复用功能映射（仅适用于高容量超值型器件）
  *     @arg GPIO_Remap_MISC             : 杂项重映射（DMA2 通道 5 位置和 DAC 触发重映射，
  *                                        仅适用于高容量超值型器件）
  * @param  NewState: 端口引脚重映射的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void GPIO_PinRemapConfig(uint32_t GPIO_Remap, FunctionalState NewState)
{
  uint32_t tmp = 0x00, tmp1 = 0x00, tmpreg = 0x00, tmpmask = 0x00;

  /* 检查参数 */
  assert_param(IS_GPIO_REMAP(GPIO_Remap));
  assert_param(IS_FUNCTIONAL_STATE(NewState));  
  
  if((GPIO_Remap & 0x80000000) == 0x80000000)
  {
    tmpreg = AFIO->MAPR2;
  }
  else
  {
    tmpreg = AFIO->MAPR;
  }

  tmpmask = (GPIO_Remap & DBGAFR_POSITION_MASK) >> 0x10;
  tmp = GPIO_Remap & LSB_MASK;

  if ((GPIO_Remap & (DBGAFR_LOCATION_MASK | DBGAFR_NUMBITS_MASK)) == (DBGAFR_LOCATION_MASK | DBGAFR_NUMBITS_MASK))
  {
    tmpreg &= DBGAFR_SWJCFG_MASK;
    AFIO->MAPR &= DBGAFR_SWJCFG_MASK;
  }
  else if ((GPIO_Remap & DBGAFR_NUMBITS_MASK) == DBGAFR_NUMBITS_MASK)
  {
    tmp1 = ((uint32_t)0x03) << tmpmask;
    tmpreg &= ~tmp1;
    tmpreg |= ~DBGAFR_SWJCFG_MASK;
  }
  else
  {
    tmpreg &= ~(tmp << ((GPIO_Remap >> 0x15)*0x10));
    tmpreg |= ~DBGAFR_SWJCFG_MASK;
  }

  if (NewState != DISABLE)
  {
    tmpreg |= (tmp << ((GPIO_Remap >> 0x15)*0x10));
  }

  if((GPIO_Remap & 0x80000000) == 0x80000000)
  {
    AFIO->MAPR2 = tmpreg;
  }
  else
  {
    AFIO->MAPR = tmpreg;
  }  
}

/**
  * @brief  选择用作 EXTI 线的 GPIO 引脚。
  * @param  GPIO_PortSource: 选择用作 EXTI 线源的 GPIO 端口。
  *   该参数可取 GPIO_PortSourceGPIOx，x 可取 (A..G)。
  * @param  GPIO_PinSource: 指定要配置的 EXTI 线。
  *   该参数可取 GPIO_PinSourcex，x 可取 (0..15)。
  * @retval 无
  */
void GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource)
{
  uint32_t tmp = 0x00;
  /* 检查参数 */
  assert_param(IS_GPIO_EXTI_PORT_SOURCE(GPIO_PortSource));
  assert_param(IS_GPIO_PIN_SOURCE(GPIO_PinSource));
  
  tmp = ((uint32_t)0x0F) << (0x04 * (GPIO_PinSource & (uint8_t)0x03));
  AFIO->EXTICR[GPIO_PinSource >> 0x02] &= ~tmp;
  AFIO->EXTICR[GPIO_PinSource >> 0x02] |= (((uint32_t)GPIO_PortSource) << (0x04 * (GPIO_PinSource & (uint8_t)0x03)));
}

/**
  * @brief  选择以太网媒体接口。
  * @note   本函数仅适用于 STM32 互联型器件。
  * @param  GPIO_ETH_MediaInterface: 指定媒体接口模式。
  *   该参数可取以下值之一：
  *     @arg GPIO_ETH_MediaInterface_MII: MII 模式
  *     @arg GPIO_ETH_MediaInterface_RMII: RMII 模式
  * @retval 无
  */
void GPIO_ETH_MediaInterfaceConfig(uint32_t GPIO_ETH_MediaInterface) 
{ 
  assert_param(IS_GPIO_ETH_MEDIA_INTERFACE(GPIO_ETH_MediaInterface)); 

  /* 配置 MII_RMII 选择位 */ 
  *(__IO uint32_t *) MAPR_MII_RMII_SEL_BB = GPIO_ETH_MediaInterface; 
}
  
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
