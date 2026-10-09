/**
  ******************************************************************************
  * @file    stm32f10x_i2c.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件提供所有 I2C 固件函数。
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
#include "stm32f10x_i2c.h"
#include "stm32f10x_rcc.h"


/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @defgroup I2C
  * @brief I2C 驱动模块
  * @{
  */ 

/** @defgroup I2C_Private_TypesDefinitions   I2C 私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup I2C_Private_Defines   I2C 私有宏定义
  * @{
  */

/* I2C SPE 掩码 */
#define CR1_PE_Set              ((uint16_t)0x0001)
#define CR1_PE_Reset            ((uint16_t)0xFFFE)

/* I2C START 掩码 */
#define CR1_START_Set           ((uint16_t)0x0100)
#define CR1_START_Reset         ((uint16_t)0xFEFF)

/* I2C STOP 掩码 */
#define CR1_STOP_Set            ((uint16_t)0x0200)
#define CR1_STOP_Reset          ((uint16_t)0xFDFF)

/* I2C ACK 掩码 */
#define CR1_ACK_Set             ((uint16_t)0x0400)
#define CR1_ACK_Reset           ((uint16_t)0xFBFF)

/* I2C ENGC 掩码 */
#define CR1_ENGC_Set            ((uint16_t)0x0040)
#define CR1_ENGC_Reset          ((uint16_t)0xFFBF)

/* I2C SWRST 掩码 */
#define CR1_SWRST_Set           ((uint16_t)0x8000)
#define CR1_SWRST_Reset         ((uint16_t)0x7FFF)

/* I2C PEC 掩码 */
#define CR1_PEC_Set             ((uint16_t)0x1000)
#define CR1_PEC_Reset           ((uint16_t)0xEFFF)

/* I2C ENPEC 掩码 */
#define CR1_ENPEC_Set           ((uint16_t)0x0020)
#define CR1_ENPEC_Reset         ((uint16_t)0xFFDF)

/* I2C ENARP 掩码 */
#define CR1_ENARP_Set           ((uint16_t)0x0010)
#define CR1_ENARP_Reset         ((uint16_t)0xFFEF)

/* I2C NOSTRETCH 掩码 */
#define CR1_NOSTRETCH_Set       ((uint16_t)0x0080)
#define CR1_NOSTRETCH_Reset     ((uint16_t)0xFF7F)

/* I2C 寄存器掩码 */
#define CR1_CLEAR_Mask          ((uint16_t)0xFBF5)

/* I2C DMAEN 掩码 */
#define CR2_DMAEN_Set           ((uint16_t)0x0800)
#define CR2_DMAEN_Reset         ((uint16_t)0xF7FF)

/* I2C LAST 掩码 */
#define CR2_LAST_Set            ((uint16_t)0x1000)
#define CR2_LAST_Reset          ((uint16_t)0xEFFF)

/* I2C FREQ 掩码 */
#define CR2_FREQ_Reset          ((uint16_t)0xFFC0)

/* I2C ADD0 掩码 */
#define OAR1_ADD0_Set           ((uint16_t)0x0001)
#define OAR1_ADD0_Reset         ((uint16_t)0xFFFE)

/* I2C ENDUAL 掩码 */
#define OAR2_ENDUAL_Set         ((uint16_t)0x0001)
#define OAR2_ENDUAL_Reset       ((uint16_t)0xFFFE)

/* I2C ADD2 掩码 */
#define OAR2_ADD2_Reset         ((uint16_t)0xFF01)

/* I2C F/S 掩码 */
#define CCR_FS_Set              ((uint16_t)0x8000)

/* I2C CCR 掩码 */
#define CCR_CCR_Set             ((uint16_t)0x0FFF)

/* I2C FLAG 掩码 */
#define FLAG_Mask               ((uint32_t)0x00FFFFFF)

/* I2C 中断使能掩码 */
#define ITEN_Mask               ((uint32_t)0x07000000)

/**
  * @}
  */

/** @defgroup I2C_Private_Macros   I2C 私有宏
  * @{
  */

/**
  * @}
  */

/** @defgroup I2C_Private_Variables   I2C 私有变量
  * @{
  */

/**
  * @}
  */

/** @defgroup I2C_Private_FunctionPrototypes   I2C 私有函数原型
  * @{
  */

/**
  * @}
  */

/** @defgroup I2C_Private_Functions   I2C 私有函数
  * @{
  */

/**
  * @brief  将 I2Cx 外设寄存器反初始化为其默认复位值。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @retval 无
  */
void I2C_DeInit(I2C_TypeDef* I2Cx)
{
  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  if (I2Cx == I2C1)
  {
    /* 使 I2C1 进入复位状态 */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
    /* 将 I2C1 从复位状态释放 */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
  }
  else
  {
    /* 使 I2C2 进入复位状态 */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, ENABLE);
    /* 将 I2C2 从复位状态释放 */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, DISABLE);
  }
}

/**
  * @brief  根据 I2C_InitStruct 中指定的
  *   参数初始化 I2Cx 外设。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @param  I2C_InitStruct: 指向 I2C_InitTypeDef 结构的指针，
  *   该结构包含指定 I2C 外设的配置信息。
  * @retval 无
  */
void I2C_Init(I2C_TypeDef* I2Cx, I2C_InitTypeDef* I2C_InitStruct)
{
  uint16_t tmpreg = 0, freqrange = 0;
  uint16_t result = 0x04;
  uint32_t pclk1 = 8000000;
  RCC_ClocksTypeDef  rcc_clocks;
  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_CLOCK_SPEED(I2C_InitStruct->I2C_ClockSpeed));
  assert_param(IS_I2C_MODE(I2C_InitStruct->I2C_Mode));
  assert_param(IS_I2C_DUTY_CYCLE(I2C_InitStruct->I2C_DutyCycle));
  assert_param(IS_I2C_OWN_ADDRESS1(I2C_InitStruct->I2C_OwnAddress1));
  assert_param(IS_I2C_ACK_STATE(I2C_InitStruct->I2C_Ack));
  assert_param(IS_I2C_ACKNOWLEDGE_ADDRESS(I2C_InitStruct->I2C_AcknowledgedAddress));

/*---------------------------- I2Cx CR2 配置 ------------------------*/
  /* 获取 I2Cx CR2 值 */
  tmpreg = I2Cx->CR2;
  /* 清零频率 FREQ[5:0] 位 */
  tmpreg &= CR2_FREQ_Reset;
  /* 获取 pclk1 频率值 */
  RCC_GetClocksFreq(&rcc_clocks);
  pclk1 = rcc_clocks.PCLK1_Frequency;
  /* 根据 pclk1 值设置频率位 */
  freqrange = (uint16_t)(pclk1 / 1000000);
  tmpreg |= freqrange;
  /* 写入 I2Cx CR2 */
  I2Cx->CR2 = tmpreg;

/*---------------------------- I2Cx CCR 配置 ------------------------*/
  /* 关闭所选 I2C 外设以配置 TRISE */
  I2Cx->CR1 &= CR1_PE_Reset;
  /* 复位 tmpreg 值 */
  /* 清零 F/S、DUTY 和 CCR[11:0] 位 */
  tmpreg = 0;

  /* 配置标准模式下的速度 */
  if (I2C_InitStruct->I2C_ClockSpeed <= 100000)
  {
    /* 标准模式速度计算 */
    result = (uint16_t)(pclk1 / (I2C_InitStruct->I2C_ClockSpeed << 1));
    /* 判断 CCR 值是否小于 0x4*/
    if (result < 0x04)
    {
      /* 设置允许的最小值 */
      result = 0x04;  
    }
    /* 设置标准模式的速度值 */
    tmpreg |= result;	  
    /* 设置标准模式的最大上升时间 */
    I2Cx->TRISE = freqrange + 1; 
  }
  /* 配置快速模式下的速度 */
  else /*(I2C_InitStruct->I2C_ClockSpeed <= 400000)*/
  {
    if (I2C_InitStruct->I2C_DutyCycle == I2C_DutyCycle_2)
    {
      /* 快速模式速度计算：Tlow/Thigh = 2 */
      result = (uint16_t)(pclk1 / (I2C_InitStruct->I2C_ClockSpeed * 3));
    }
    else /*I2C_InitStruct->I2C_DutyCycle == I2C_DutyCycle_16_9*/
    {
      /* 快速模式速度计算：Tlow/Thigh = 16/9 */
      result = (uint16_t)(pclk1 / (I2C_InitStruct->I2C_ClockSpeed * 25));
      /* 置位 DUTY 位 */
      result |= I2C_DutyCycle_16_9;
    }

    /* 判断 CCR 值是否小于 0x1*/
    if ((result & CCR_CCR_Set) == 0)
    {
      /* 设置允许的最小值 */
      result |= (uint16_t)0x0001;  
    }
    /* 设置快速模式的速度值并置位 F/S 位 */
    tmpreg |= (uint16_t)(result | CCR_FS_Set);
    /* 设置快速模式的最大上升时间 */
    I2Cx->TRISE = (uint16_t)(((freqrange * (uint16_t)300) / (uint16_t)1000) + (uint16_t)1);  
  }

  /* 写入 I2Cx CCR */
  I2Cx->CCR = tmpreg;
  /* 使能所选 I2C 外设 */
  I2Cx->CR1 |= CR1_PE_Set;

/*---------------------------- I2Cx CR1 配置 ------------------------*/
  /* 获取 I2Cx CR1 值 */
  tmpreg = I2Cx->CR1;
  /* 清零 ACK、SMBTYPE 和 SMBUS 位 */
  tmpreg &= CR1_CLEAR_Mask;
  /* 配置 I2Cx：模式和应答 */
  /* 根据 I2C_Mode 值设置 SMBTYPE 和 SMBUS 位 */
  /* 根据 I2C_Ack 值设置 ACK 位 */
  tmpreg |= (uint16_t)((uint32_t)I2C_InitStruct->I2C_Mode | I2C_InitStruct->I2C_Ack);
  /* 写入 I2Cx CR1 */
  I2Cx->CR1 = tmpreg;

/*---------------------------- I2Cx OAR1 配置 -----------------------*/
  /* 设置 I2Cx 自身地址 1 和应答地址 */
  I2Cx->OAR1 = (I2C_InitStruct->I2C_AcknowledgedAddress | I2C_InitStruct->I2C_OwnAddress1);
}

/**
  * @brief  将 I2C_InitStruct 的每个成员填充为默认值。
  * @param  I2C_InitStruct: 指向将被初始化的 I2C_InitTypeDef 结构的指针。
  * @retval 无
  */
void I2C_StructInit(I2C_InitTypeDef* I2C_InitStruct)
{
/*---------------- 复位 I2C 初始化结构的参数值 ----------------*/
  /* 初始化 I2C_ClockSpeed 成员 */
  I2C_InitStruct->I2C_ClockSpeed = 5000;
  /* 初始化 I2C_Mode 成员 */
  I2C_InitStruct->I2C_Mode = I2C_Mode_I2C;
  /* 初始化 I2C_DutyCycle 成员 */
  I2C_InitStruct->I2C_DutyCycle = I2C_DutyCycle_2;
  /* 初始化 I2C_OwnAddress1 成员 */
  I2C_InitStruct->I2C_OwnAddress1 = 0;
  /* 初始化 I2C_Ack 成员 */
  I2C_InitStruct->I2C_Ack = I2C_Ack_Disable;
  /* 初始化 I2C_AcknowledgedAddress 成员 */
  I2C_InitStruct->I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
}

/**
  * @brief  使能或关闭指定的 I2C 外设。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @param  NewState: I2Cx 外设的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void I2C_Cmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能所选 I2C 外设 */
    I2Cx->CR1 |= CR1_PE_Set;
  }
  else
  {
    /* 关闭所选 I2C 外设 */
    I2Cx->CR1 &= CR1_PE_Reset;
  }
}

/**
  * @brief  使能或关闭指定的 I2C DMA 请求。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @param  NewState: I2C DMA 传输的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void I2C_DMACmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能所选 I2C DMA 请求 */
    I2Cx->CR2 |= CR2_DMAEN_Set;
  }
  else
  {
    /* 关闭所选 I2C DMA 请求 */
    I2Cx->CR2 &= CR2_DMAEN_Reset;
  }
}

/**
  * @brief  指定下一次 DMA 传输是否为最后一次。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @param  NewState: I2C DMA 最后一次传输的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void I2C_DMALastTransferCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 下一次 DMA 传输为最后一次传输 */
    I2Cx->CR2 |= CR2_LAST_Set;
  }
  else
  {
    /* 下一次 DMA 传输不是最后一次传输 */
    I2Cx->CR2 &= CR2_LAST_Reset;
  }
}

/**
  * @brief  产生 I2Cx 通信 START 条件。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @param  NewState: I2C START 条件产生的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void I2C_GenerateSTART(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 产生 START 条件 */
    I2Cx->CR1 |= CR1_START_Set;
  }
  else
  {
    /* 关闭 START 条件产生 */
    I2Cx->CR1 &= CR1_START_Reset;
  }
}

/**
  * @brief  产生 I2Cx 通信 STOP 条件。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @param  NewState: I2C STOP 条件产生的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void I2C_GenerateSTOP(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 产生 STOP 条件 */
    I2Cx->CR1 |= CR1_STOP_Set;
  }
  else
  {
    /* 关闭 STOP 条件产生 */
    I2Cx->CR1 &= CR1_STOP_Reset;
  }
}

/**
  * @brief  使能或关闭指定的 I2C 应答功能。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @param  NewState: I2C 应答的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void I2C_AcknowledgeConfig(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能应答 */
    I2Cx->CR1 |= CR1_ACK_Set;
  }
  else
  {
    /* 关闭应答 */
    I2Cx->CR1 &= CR1_ACK_Reset;
  }
}

/**
  * @brief  配置指定的 I2C 自身地址 2。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @param  Address: 指定 7 位 I2C 自身地址 2。
  * @retval 无
  */
void I2C_OwnAddress2Config(I2C_TypeDef* I2Cx, uint8_t Address)
{
  uint16_t tmpreg = 0;

  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  /* 读取寄存器旧值 */
  tmpreg = I2Cx->OAR2;

  /* 复位 I2Cx 自身地址 2 的位 [7:1] */
  tmpreg &= OAR2_ADD2_Reset;

  /* 设置 I2Cx 自身地址 2 */
  tmpreg |= (uint16_t)((uint16_t)Address & (uint16_t)0x00FE);

  /* 保存寄存器新值 */
  I2Cx->OAR2 = tmpreg;
}

/**
  * @brief  使能或关闭指定的 I2C 双地址模式。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @param  NewState: I2C 双地址模式的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void I2C_DualAddressCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能双地址模式 */
    I2Cx->OAR2 |= OAR2_ENDUAL_Set;
  }
  else
  {
    /* 关闭双地址模式 */
    I2Cx->OAR2 &= OAR2_ENDUAL_Reset;
  }
}

/**
  * @brief  使能或关闭指定的 I2C 广播呼叫功能。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @param  NewState: I2C 广播呼叫的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void I2C_GeneralCallCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能广播呼叫 */
    I2Cx->CR1 |= CR1_ENGC_Set;
  }
  else
  {
    /* 关闭广播呼叫 */
    I2Cx->CR1 &= CR1_ENGC_Reset;
  }
}

/**
  * @brief  使能或关闭指定的 I2C 中断。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @param  I2C_IT: 指定要使能或关闭的 I2C 中断源。
  *   该参数可取以下值的任意组合：
  *     @arg I2C_IT_BUF: 缓冲中断掩码
  *     @arg I2C_IT_EVT: 事件中断掩码
  *     @arg I2C_IT_ERR: 错误中断掩码
  * @param  NewState: 指定 I2C 中断的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void I2C_ITConfig(I2C_TypeDef* I2Cx, uint16_t I2C_IT, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_I2C_CONFIG_IT(I2C_IT));
  
  if (NewState != DISABLE)
  {
    /* 使能所选 I2C 中断 */
    I2Cx->CR2 |= I2C_IT;
  }
  else
  {
    /* 关闭所选 I2C 中断 */
    I2Cx->CR2 &= (uint16_t)~I2C_IT;
  }
}

/**
  * @brief  通过 I2Cx 外设发送一个数据字节。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @param  Data: 要发送的字节。
  * @retval 无
  */
void I2C_SendData(I2C_TypeDef* I2Cx, uint8_t Data)
{
  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  /* 将要发送的数据写入 DR 寄存器 */
  I2Cx->DR = Data;
}

/**
  * @brief  返回 I2Cx 外设最近接收到的数据。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @retval 接收数据的值。
  */
uint8_t I2C_ReceiveData(I2C_TypeDef* I2Cx)
{
  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  /* 返回 DR 寄存器中的数据 */
  return (uint8_t)I2Cx->DR;
}

/**
  * @brief  发送地址字节以选择从器件。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @param  Address: 指定将要发送的从机地址
  * @param  I2C_Direction: 指定 I2C 器件将作为
  *   发送器还是接收器。该参数可取以下值之一
  *     @arg I2C_Direction_Transmitter: 发送器模式
  *     @arg I2C_Direction_Receiver: 接收器模式
  * @retval 无
  */
void I2C_Send7bitAddress(I2C_TypeDef* I2Cx, uint8_t Address, uint8_t I2C_Direction)
{
  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_DIRECTION(I2C_Direction));
  /* 判断方向以置位/清零读/写位 */
  if (I2C_Direction != I2C_Direction_Transmitter)
  {
    /* 置位地址位 0 表示读 */
    Address |= OAR1_ADD0_Set;
  }
  else
  {
    /* 清零地址位 0 表示写 */
    Address &= OAR1_ADD0_Reset;
  }
  /* 发送地址 */
  I2Cx->DR = Address;
}

/**
  * @brief  读取指定的 I2C 寄存器并返回其值。
  * @param  I2C_Register: 指定要读取的寄存器。
  *   该参数可取以下值之一：
  *     @arg I2C_Register_CR1:  CR1 寄存器。
  *     @arg I2C_Register_CR2:   CR2 寄存器。
  *     @arg I2C_Register_OAR1:  OAR1 寄存器。
  *     @arg I2C_Register_OAR2:  OAR2 寄存器。
  *     @arg I2C_Register_DR:    DR 寄存器。
  *     @arg I2C_Register_SR1:   SR1 寄存器。
  *     @arg I2C_Register_SR2:   SR2 寄存器。
  *     @arg I2C_Register_CCR:   CCR 寄存器。
  *     @arg I2C_Register_TRISE: TRISE 寄存器。
  * @retval 所读寄存器的值。
  */
uint16_t I2C_ReadRegister(I2C_TypeDef* I2Cx, uint8_t I2C_Register)
{
  __IO uint32_t tmp = 0;

  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_REGISTER(I2C_Register));

  tmp = (uint32_t) I2Cx;
  tmp += I2C_Register;

  /* 返回所选寄存器的值 */
  return (*(__IO uint16_t *) tmp);
}

/**
  * @brief  使能或关闭指定的 I2C 软件复位。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @param  NewState: I2C 软件复位的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void I2C_SoftwareResetCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 外设处于复位状态 */
    I2Cx->CR1 |= CR1_SWRST_Set;
  }
  else
  {
    /* 外设未处于复位状态 */
    I2Cx->CR1 &= CR1_SWRST_Reset;
  }
}

/**
  * @brief  选择主接收模式下指定的 I2C NACK 位置。
  *         当要接收的数据个数等于 2 时，本函数在 I2C 主接收模式下很有用。
  *         此时应在数据接收开始之前调用本函数（参数为 I2C_NACKPosition_Next），
  *         具体方式如参考手册“主接收”一节中推荐的 2 字节接收流程所述。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @param  I2C_NACKPosition: 指定 NACK 位置。
  *   该参数可取以下值之一：
  *     @arg I2C_NACKPosition_Next: 表示下一个字节将是最后
  *          接收的字节。
  *     @arg I2C_NACKPosition_Current: 表示当前字节是最后
  *          接收的字节。
  *
  * @note    本函数配置的位 (POS) 与 I2C_PECPositionConfig() 相同，
  *          但本函数用于 I2C 模式，而 I2C_PECPositionConfig()
  *          用于 SMBUS 模式。
  *
  * @retval 无
  */
void I2C_NACKPositionConfig(I2C_TypeDef* I2Cx, uint16_t I2C_NACKPosition)
{
  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_NACK_POSITION(I2C_NACKPosition));
  
  /* 检查输入参数 */
  if (I2C_NACKPosition == I2C_NACKPosition_Next)
  {
    /* 移位寄存器中的下一个字节是最后接收的字节 */
    I2Cx->CR1 |= I2C_NACKPosition_Next;
  }
  else
  {
    /* 移位寄存器中的当前字节是最后接收的字节 */
    I2Cx->CR1 &= I2C_NACKPosition_Current;
  }
}

/**
  * @brief  为指定的 I2C 将 SMBusAlert 引脚驱动为高电平或低电平。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @param  I2C_SMBusAlert: 指定 SMBAlert 引脚电平。
  *   该参数可取以下值之一：
  *     @arg I2C_SMBusAlert_Low: SMBAlert 引脚驱动为低电平
  *     @arg I2C_SMBusAlert_High: SMBAlert 引脚驱动为高电平
  * @retval 无
  */
void I2C_SMBusAlertConfig(I2C_TypeDef* I2Cx, uint16_t I2C_SMBusAlert)
{
  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_SMBUS_ALERT(I2C_SMBusAlert));
  if (I2C_SMBusAlert == I2C_SMBusAlert_Low)
  {
    /* 将 SMBusAlert 引脚驱动为低电平 */
    I2Cx->CR1 |= I2C_SMBusAlert_Low;
  }
  else
  {
    /* 将 SMBusAlert 引脚驱动为高电平  */
    I2Cx->CR1 &= I2C_SMBusAlert_High;
  }
}

/**
  * @brief  使能或关闭指定的 I2C PEC 传输。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @param  NewState: I2C PEC 传输的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void I2C_TransmitPEC(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能所选的 I2C PEC 传输 */
    I2Cx->CR1 |= CR1_PEC_Set;
  }
  else
  {
    /* 关闭所选的 I2C PEC 传输 */
    I2Cx->CR1 &= CR1_PEC_Reset;
  }
}

/**
  * @brief  选择指定的 I2C PEC 位置。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @param  I2C_PECPosition: 指定 PEC 位置。
  *   该参数可取以下值之一：
  *     @arg I2C_PECPosition_Next: 表示下一个字节为 PEC
  *     @arg I2C_PECPosition_Current: 表示当前字节为 PEC
  *
  * @note    本函数配置的位（POS）与 I2C_NACKPositionConfig() 相同，
  *          但本函数用于 SMBUS 模式，而 I2C_NACKPositionConfig()
  *          用于 I2C 模式。
  *
  * @retval 无
  */
void I2C_PECPositionConfig(I2C_TypeDef* I2Cx, uint16_t I2C_PECPosition)
{
  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_PEC_POSITION(I2C_PECPosition));
  if (I2C_PECPosition == I2C_PECPosition_Next)
  {
    /* 移位寄存器中的下一个字节为 PEC */
    I2Cx->CR1 |= I2C_PECPosition_Next;
  }
  else
  {
    /* 移位寄存器中的当前字节为 PEC */
    I2Cx->CR1 &= I2C_PECPosition_Current;
  }
}

/**
  * @brief  使能或关闭已传输字节的 PEC 值计算。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @param  NewState: I2Cx PEC 值计算的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void I2C_CalculatePEC(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能所选的 I2C PEC 计算 */
    I2Cx->CR1 |= CR1_ENPEC_Set;
  }
  else
  {
    /* 关闭所选的 I2C PEC 计算 */
    I2Cx->CR1 &= CR1_ENPEC_Reset;
  }
}

/**
  * @brief  返回指定 I2C 的 PEC 值。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @retval PEC 值。
  */
uint8_t I2C_GetPEC(I2C_TypeDef* I2Cx)
{
  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  /* 返回所选的 I2C PEC 值 */
  return ((I2Cx->SR2) >> 8);
}

/**
  * @brief  使能或关闭指定的 I2C ARP。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @param  NewState: I2Cx ARP 的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void I2C_ARPCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能所选的 I2C ARP */
    I2Cx->CR1 |= CR1_ENARP_Set;
  }
  else
  {
    /* 关闭所选的 I2C ARP */
    I2Cx->CR1 &= CR1_ENARP_Reset;
  }
}

/**
  * @brief  使能或关闭指定的 I2C 时钟延展。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @param  NewState: I2Cx 时钟延展的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void I2C_StretchClockCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState == DISABLE)
  {
    /* 使能所选的 I2C 时钟延展 */
    I2Cx->CR1 |= CR1_NOSTRETCH_Set;
  }
  else
  {
    /* 关闭所选的 I2C 时钟延展 */
    I2Cx->CR1 &= CR1_NOSTRETCH_Reset;
  }
}

/**
  * @brief  选择指定的 I2C 快速模式占空比。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @param  I2C_DutyCycle: 指定快速模式占空比。
  *   该参数可取以下值之一：
  *     @arg I2C_DutyCycle_2: I2C 快速模式 Tlow/Thigh = 2
  *     @arg I2C_DutyCycle_16_9: I2C 快速模式 Tlow/Thigh = 16/9
  * @retval 无
  */
void I2C_FastModeDutyCycleConfig(I2C_TypeDef* I2Cx, uint16_t I2C_DutyCycle)
{
  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_DUTY_CYCLE(I2C_DutyCycle));
  if (I2C_DutyCycle != I2C_DutyCycle_16_9)
  {
    /* I2C 快速模式 Tlow/Thigh=2 */
    I2Cx->CCR &= I2C_DutyCycle_2;
  }
  else
  {
    /* I2C 快速模式 Tlow/Thigh=16/9 */
    I2Cx->CCR |= I2C_DutyCycle_16_9;
  }
}



/**
 * @brief
 ****************************************************************************************
 *
 *                         I2C 状态监控函数
 *
 ****************************************************************************************
 * 本 I2C 驱动根据应用需求和约束，提供三种不同的 I2C 状态监控方式：
 *
 *
 * 1) 基本状态监控：
 *    使用 I2C_CheckEvent() 函数：
 *    它将状态寄存器（SR1 和 SR2）的内容与给定事件
 *    （可以是一个或多个标志的组合）进行比较。
 *    如果当前状态包含给定标志则返回 SUCCESS，
 *    如果当前状态缺少一个或多个标志则返回 ERROR。
 *    - 适用场合：
 *      - 由于事件在产品参考手册（RM0008）中有完整描述，本函数适用于大多数
 *        应用以及启动阶段的活动。
 *      - 也适用于需要定义自己事件的用户。
 *    - 局限性：
 *      - 如果发生错误（即除被监控的标志外还有错误标志被置位），
 *        即使通信保持挂起或实际状态已损坏，I2C_CheckEvent() 函数也可能
 *        返回 SUCCESS。
 *        在这种情况下，建议使用错误中断来监控错误事件，
 *        并在中断 IRQ 处理函数中处理它们。
 *
 *        @note
 *        对于错误管理，建议使用以下函数：
 *          - I2C_ITConfig() 用于配置并使能错误中断（I2C_IT_ERR）。
 *          - I2Cx_ER_IRQHandler() 在发生错误中断时被调用。
 *            其中 x 是外设实例（I2C1、I2C2 ...）
 *          - I2C_GetFlagStatus() 或 I2C_GetITStatus() 在 I2Cx_ER_IRQHandler()
 *            中调用，以确定发生了哪个错误。
 *          - I2C_ClearFlag() 或 I2C_ClearITPendingBit() 和/或
 *            I2C_SoftwareResetCmd() 和/或 I2C_GenerateStop() 用于清除错误
 *            标志和错误源，并恢复到正确的通信状态。
 *
 *
 *  2) 高级状态监控：
 *     使用 I2C_GetLastEvent() 函数，它在一个字（uint32_t）中返回两个状态
 *     寄存器的镜像（状态寄存器 2 的值左移 16 位后与状态寄存器 1 拼接）。
 *     - 适用场合：
 *       - 本函数适用于上述相同的应用，但它可以克服 I2C_GetFlagStatus()
 *         函数的上述局限性。
 *         返回值可以与库中已定义的事件（stm32f10x_i2c.h）
 *         或用户自定义的值进行比较。
 *       - 本函数适用于同时监控多个标志的场合。
 *       - 与 I2C_CheckEvent() 函数相反，本函数允许用户选择何时接受一个
 *         事件（当所有事件标志都置位且没有其它标志置位时，
 *         或者像 I2C_CheckEvent() 函数那样只要求所需标志置位时）。
 *     - 局限性：
 *       - 用户可能需要定义自己的事件。
 *       - 如果用户决定只检查常规通信标志（而忽略错误标志），
 *         那么关于错误管理的同样说明也适用于本函数。
 *
 *
 *  3) 基于标志的状态监控：
 *     使用 I2C_GetFlagStatus() 函数，它仅返回单个标志的状态
 *     （即 I2C_FLAG_RXNE ...）。
 *     - 适用场合：
 *        - 本函数可用于特定应用或调试阶段。
 *        - 适用于只需检查一个标志的场合（大多数 I2C 事件需要通过多个
 *          标志监控）。
 *     - 局限性：
 *        - 调用本函数时会访问状态寄存器。某些标志在访问状态寄存器时会
 *          被清除。因此检查一个标志的状态可能会清除其它标志。
 *        - 为了监控单个事件，可能需要调用本函数两次或更多次。
 *
 *  关于事件的详细描述，请参阅 stm32f10x_i2c.h 文件中的
 *  I2C_Events 一节。
 *
 */

/**
 *
 *  1) 基本状态监控
 *******************************************************************************
 */

/**
  * @brief  检查最后一个 I2Cx 事件是否等于作为参数传入的事件。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @param  I2C_EVENT: 指定要检查的事件。
  *   该参数可取以下值之一：
  *     @arg I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED           : EV1
  *     @arg I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED              : EV1
  *     @arg I2C_EVENT_SLAVE_TRANSMITTER_SECONDADDRESS_MATCHED     : EV1
  *     @arg I2C_EVENT_SLAVE_RECEIVER_SECONDADDRESS_MATCHED        : EV1
  *     @arg I2C_EVENT_SLAVE_GENERALCALLADDRESS_MATCHED            : EV1
  *     @arg I2C_EVENT_SLAVE_BYTE_RECEIVED                         : EV2
  *     @arg (I2C_EVENT_SLAVE_BYTE_RECEIVED | I2C_FLAG_DUALF)      : EV2
  *     @arg (I2C_EVENT_SLAVE_BYTE_RECEIVED | I2C_FLAG_GENCALL)    : EV2
  *     @arg I2C_EVENT_SLAVE_BYTE_TRANSMITTED                      : EV3
  *     @arg (I2C_EVENT_SLAVE_BYTE_TRANSMITTED | I2C_FLAG_DUALF)   : EV3
  *     @arg (I2C_EVENT_SLAVE_BYTE_TRANSMITTED | I2C_FLAG_GENCALL) : EV3
  *     @arg I2C_EVENT_SLAVE_ACK_FAILURE                           : EV3_2
  *     @arg I2C_EVENT_SLAVE_STOP_DETECTED                         : EV4
  *     @arg I2C_EVENT_MASTER_MODE_SELECT                          : EV5
  *     @arg I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED            : EV6
  *     @arg I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED               : EV6
  *     @arg I2C_EVENT_MASTER_BYTE_RECEIVED                        : EV7
  *     @arg I2C_EVENT_MASTER_BYTE_TRANSMITTING                    : EV8
  *     @arg I2C_EVENT_MASTER_BYTE_TRANSMITTED                     : EV8_2
  *     @arg I2C_EVENT_MASTER_MODE_ADDRESS10                       : EV9
  *
  * @note: 关于事件的详细描述，请参阅
  *    stm32f10x_i2c.h 文件中的 I2C_Events 一节。
  *
  * @retval 一个 ErrorStatus 枚举值：
  * - SUCCESS: 最后一个事件等于 I2C_EVENT
  * - ERROR: 最后一个事件与 I2C_EVENT 不同
  */
ErrorStatus I2C_CheckEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
  uint32_t lastevent = 0;
  uint32_t flag1 = 0, flag2 = 0;
  ErrorStatus status = ERROR;

  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_EVENT(I2C_EVENT));

  /* 读取 I2Cx 状态寄存器 */
  flag1 = I2Cx->SR1;
  flag2 = I2Cx->SR2;
  flag2 = flag2 << 16;

  /* 从 I2C 状态寄存器获取最后一个事件值 */
  lastevent = (flag1 | flag2) & FLAG_Mask;

  /* 检查最后一个事件是否包含 I2C_EVENT */
  if ((lastevent & I2C_EVENT) == I2C_EVENT)
  {
    /* SUCCESS: 最后一个事件等于 I2C_EVENT */
    status = SUCCESS;
  }
  else
  {
    /* ERROR: 最后一个事件与 I2C_EVENT 不同 */
    status = ERROR;
  }
  /* 返回状态 */
  return status;
}

/**
 *
 *  2) 高级状态监控
 *******************************************************************************
 */

/**
  * @brief  返回最后一个 I2Cx 事件。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  *
  * @note: 关于事件的详细描述，请参阅
  *    stm32f10x_i2c.h 文件中的 I2C_Events 一节。
  *
  * @retval 最后一个事件
  */
uint32_t I2C_GetLastEvent(I2C_TypeDef* I2Cx)
{
  uint32_t lastevent = 0;
  uint32_t flag1 = 0, flag2 = 0;

  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));

  /* 读取 I2Cx 状态寄存器 */
  flag1 = I2Cx->SR1;
  flag2 = I2Cx->SR2;
  flag2 = flag2 << 16;

  /* 从 I2C 状态寄存器获取最后一个事件值 */
  lastevent = (flag1 | flag2) & FLAG_Mask;

  /* 返回状态 */
  return lastevent;
}

/**
 *
 *  3) 基于标志的状态监控
 *******************************************************************************
 */

/**
  * @brief  检查指定的 I2C 标志是否置位。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @param  I2C_FLAG: 指定要检查的标志。
  *   该参数可取以下值之一：
  *     @arg I2C_FLAG_DUALF: 双地址标志（从模式）
  *     @arg I2C_FLAG_SMBHOST: SMBus 主机头标志（从模式）
  *     @arg I2C_FLAG_SMBDEFAULT: SMBus 默认头标志（从模式）
  *     @arg I2C_FLAG_GENCALL: 通用呼叫头标志（从模式）
  *     @arg I2C_FLAG_TRA: 发送/接收标志
  *     @arg I2C_FLAG_BUSY: 总线忙标志
  *     @arg I2C_FLAG_MSL: 主/从标志
  *     @arg I2C_FLAG_SMBALERT: SMBus 警报标志
  *     @arg I2C_FLAG_TIMEOUT: 超时或 Tlow 错误标志
  *     @arg I2C_FLAG_PECERR: 接收中的 PEC 错误标志
  *     @arg I2C_FLAG_OVR: 上溢/下溢标志（从模式）
  *     @arg I2C_FLAG_AF: 应答失败标志
  *     @arg I2C_FLAG_ARLO: 仲裁丢失标志（主模式）
  *     @arg I2C_FLAG_BERR: 总线错误标志
  *     @arg I2C_FLAG_TXE: 数据寄存器空标志（发送器）
  *     @arg I2C_FLAG_RXNE: 数据寄存器非空标志（接收器）
  *     @arg I2C_FLAG_STOPF: 停止位检测标志（从模式）
  *     @arg I2C_FLAG_ADD10: 10 位头已发送标志（主模式）
  *     @arg I2C_FLAG_BTF: 字节传输完成标志
  *     @arg I2C_FLAG_ADDR: 地址已发送标志（主模式）"ADSL"
  *   地址匹配标志（从模式）"ENDA"
  *     @arg I2C_FLAG_SB: 起始位标志（主模式）
  * @retval I2C_FLAG 的新状态（SET 或 RESET）。
  */
FlagStatus I2C_GetFlagStatus(I2C_TypeDef* I2Cx, uint32_t I2C_FLAG)
{
  FlagStatus bitstatus = RESET;
  __IO uint32_t i2creg = 0, i2cxbase = 0;

  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_GET_FLAG(I2C_FLAG));

  /* 获取 I2Cx 外设基地址 */
  i2cxbase = (uint32_t)I2Cx;
  
  /* 读取标志寄存器索引 */
  i2creg = I2C_FLAG >> 28;
  
  /* 获取标志的 bit[23:0] */
  I2C_FLAG &= FLAG_Mask;
  
  if(i2creg != 0)
  {
    /* 获取 I2Cx SR1 寄存器地址 */
    i2cxbase += 0x14;
  }
  else
  {
    /* 标志位于 I2Cx SR2 寄存器中 */
    I2C_FLAG = (uint32_t)(I2C_FLAG >> 16);
    /* 获取 I2Cx SR2 寄存器地址 */
    i2cxbase += 0x18;
  }
  
  if(((*(__IO uint32_t *)i2cxbase) & I2C_FLAG) != (uint32_t)RESET)
  {
    /* I2C_FLAG 已置位 */
    bitstatus = SET;
  }
  else
  {
    /* I2C_FLAG 已复位 */
    bitstatus = RESET;
  }
  
  /* 返回 I2C_FLAG 状态 */
  return  bitstatus;
}



/**
  * @brief  清除 I2Cx 的挂起标志。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @param  I2C_FLAG: 指定要清除的标志。
  *   该参数可取以下值的任意组合：
  *     @arg I2C_FLAG_SMBALERT: SMBus 警报标志
  *     @arg I2C_FLAG_TIMEOUT: 超时或 Tlow 错误标志
  *     @arg I2C_FLAG_PECERR: 接收中的 PEC 错误标志
  *     @arg I2C_FLAG_OVR: 上溢/下溢标志（从模式）
  *     @arg I2C_FLAG_AF: 应答失败标志
  *     @arg I2C_FLAG_ARLO: 仲裁丢失标志（主模式）
  *     @arg I2C_FLAG_BERR: 总线错误标志
  *
  * @note
  *   - STOPF（停止位检测）通过软件序列清除：先对 I2C_SR1 寄存器执行读操作
  *     （I2C_GetFlagStatus()），随后对 I2C_CR1 寄存器执行写操作
  *     （I2C_Cmd() 以重新使能 I2C 外设）。
  *   - ADD10（10 位头已发送）通过软件序列清除：先对 I2C_SR1 执行读操作
  *     （I2C_GetFlagStatus()），随后向 DR 寄存器写入地址的第二个字节。
  *   - BTF（字节传输完成）通过软件序列清除：先对 I2C_SR1 寄存器执行读操作
  *     （I2C_GetFlagStatus()），随后对 I2C_DR 寄存器执行读/写操作
  *     （I2C_SendData()）。
  *   - ADDR（地址已发送）通过软件序列清除：先对 I2C_SR1 寄存器执行读操作
  *     （I2C_GetFlagStatus()），随后对 I2C_SR2 寄存器执行读操作
  *     （(void)(I2Cx->SR2)）。
  *   - SB（起始位）通过软件序列清除：先对 I2C_SR1 寄存器执行读操作
  *     （I2C_GetFlagStatus()），随后对 I2C_DR 寄存器执行写操作
  *     （I2C_SendData()）。
  * @retval 无
  */
void I2C_ClearFlag(I2C_TypeDef* I2Cx, uint32_t I2C_FLAG)
{
  uint32_t flagpos = 0;
  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_CLEAR_FLAG(I2C_FLAG));
  /* 获取 I2C 标志位置 */
  flagpos = I2C_FLAG & FLAG_Mask;
  /* 清除所选的 I2C 标志 */
  I2Cx->SR1 = (uint16_t)~flagpos;
}

/**
  * @brief  检查指定的 I2C 中断是否发生。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @param  I2C_IT: 指定要检查的中断源。
  *   该参数可取以下值之一：
  *     @arg I2C_IT_SMBALERT: SMBus 警报标志
  *     @arg I2C_IT_TIMEOUT: 超时或 Tlow 错误标志
  *     @arg I2C_IT_PECERR: 接收中的 PEC 错误标志
  *     @arg I2C_IT_OVR: 上溢/下溢标志（从模式）
  *     @arg I2C_IT_AF: 应答失败标志
  *     @arg I2C_IT_ARLO: 仲裁丢失标志（主模式）
  *     @arg I2C_IT_BERR: 总线错误标志
  *     @arg I2C_IT_TXE: 数据寄存器空标志（发送器）
  *     @arg I2C_IT_RXNE: 数据寄存器非空标志（接收器）
  *     @arg I2C_IT_STOPF: 停止位检测标志（从模式）
  *     @arg I2C_IT_ADD10: 10 位头已发送标志（主模式）
  *     @arg I2C_IT_BTF: 字节传输完成标志
  *     @arg I2C_IT_ADDR: 地址已发送标志（主模式）"ADSL"
  *                       地址匹配标志（从模式）"ENDAD"
  *     @arg I2C_IT_SB: 起始位标志（主模式）
  * @retval I2C_IT 的新状态（SET 或 RESET）。
  */
ITStatus I2C_GetITStatus(I2C_TypeDef* I2Cx, uint32_t I2C_IT)
{
  ITStatus bitstatus = RESET;
  uint32_t enablestatus = 0;

  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_GET_IT(I2C_IT));

  /* 检查中断源是否使能 */
  enablestatus = (uint32_t)(((I2C_IT & ITEN_Mask) >> 16) & (I2Cx->CR2)) ;
  
  /* 获取标志的 bit[23:0] */
  I2C_IT &= FLAG_Mask;

  /* 检查指定 I2C 标志的状态 */
  if (((I2Cx->SR1 & I2C_IT) != (uint32_t)RESET) && enablestatus)
  {
    /* I2C_IT 已置位 */
    bitstatus = SET;
  }
  else
  {
    /* I2C_IT 已复位 */
    bitstatus = RESET;
  }
  /* 返回 I2C_IT 状态 */
  return  bitstatus;
}

/**
  * @brief  清除 I2Cx 的中断挂起位。
  * @param  I2Cx: x 可取 1 或 2，用于选择 I2C 外设。
  * @param  I2C_IT: 指定要清除的中断挂起位。
  *   该参数可取以下值的任意组合：
  *     @arg I2C_IT_SMBALERT: SMBus 警报中断
  *     @arg I2C_IT_TIMEOUT: 超时或 Tlow 错误中断
  *     @arg I2C_IT_PECERR: 接收中的 PEC 错误中断
  *     @arg I2C_IT_OVR: 上溢/下溢中断（从模式）
  *     @arg I2C_IT_AF: 应答失败中断
  *     @arg I2C_IT_ARLO: 仲裁丢失中断（主模式）
  *     @arg I2C_IT_BERR: 总线错误中断
  *
  * @note
  *   - STOPF（停止位检测）通过软件序列清除：先对 I2C_SR1 寄存器执行读操作
  *     （I2C_GetITStatus()），随后对 I2C_CR1 寄存器执行写操作
  *     （I2C_Cmd() 以重新使能 I2C 外设）。
  *   - ADD10（10 位头已发送）通过软件序列清除：先对 I2C_SR1 执行读操作
  *     （I2C_GetITStatus()），随后向 I2C_DR 寄存器写入地址的第二个字节。
  *   - BTF（字节传输完成）通过软件序列清除：先对 I2C_SR1 寄存器执行读操作
  *     （I2C_GetITStatus()），随后对 I2C_DR 寄存器执行读/写操作
  *     （I2C_SendData()）。
  *   - ADDR（地址已发送）通过软件序列清除：先对 I2C_SR1 寄存器执行读操作
  *     （I2C_GetITStatus()），随后对 I2C_SR2 寄存器执行读操作
  *     （(void)(I2Cx->SR2)）。
  *   - SB（起始位）通过软件序列清除：先对 I2C_SR1 寄存器执行读操作
  *     （I2C_GetITStatus()），随后对 I2C_DR 寄存器执行写操作
  *     （I2C_SendData()）。
  * @retval 无
  */
void I2C_ClearITPendingBit(I2C_TypeDef* I2Cx, uint32_t I2C_IT)
{
  uint32_t flagpos = 0;
  /* 检查参数 */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_CLEAR_IT(I2C_IT));
  /* 获取 I2C 标志位置 */
  flagpos = I2C_IT & FLAG_Mask;
  /* 清除所选的 I2C 标志 */
  I2Cx->SR1 = (uint16_t)~flagpos;
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
