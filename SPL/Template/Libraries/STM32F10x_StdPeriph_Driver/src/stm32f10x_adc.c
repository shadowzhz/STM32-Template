/**
  ******************************************************************************
  * @file    stm32f10x_adc.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件提供全部 ADC 固件函数。
  ******************************************************************************
  * @attention
  *
  * 本固件仅供参考，其目的在于为客户提供有关其产品的编码信息，以帮助客户节省时间。
  * 因此，对于因本固件的内容和/或客户将其中所含编码信息用于其产品而产生的任何索赔
  * 所造成的任何直接、间接或后果性损害，STMicroelectronics 概不承担任何责任。
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f10x_adc.h"
#include "stm32f10x_rcc.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @defgroup ADC   ADC 驱动
  * @brief ADC 驱动模块
  * @{
  */

/** @defgroup ADC_Private_TypesDefinitions   ADC 私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup ADC_Private_Defines   ADC 私有宏定义
  * @{
  */

/* ADC DISCNUM 掩码 */
#define CR1_DISCNUM_Reset           ((uint32_t)0xFFFF1FFF)

/* ADC DISCEN 掩码 */
#define CR1_DISCEN_Set              ((uint32_t)0x00000800)
#define CR1_DISCEN_Reset            ((uint32_t)0xFFFFF7FF)

/* ADC JAUTO 掩码 */
#define CR1_JAUTO_Set               ((uint32_t)0x00000400)
#define CR1_JAUTO_Reset             ((uint32_t)0xFFFFFBFF)

/* ADC JDISCEN 掩码 */
#define CR1_JDISCEN_Set             ((uint32_t)0x00001000)
#define CR1_JDISCEN_Reset           ((uint32_t)0xFFFFEFFF)

/* ADC AWDCH 掩码 */
#define CR1_AWDCH_Reset             ((uint32_t)0xFFFFFFE0)

/* ADC 模拟看门狗使能模式掩码 */
#define CR1_AWDMode_Reset           ((uint32_t)0xFF3FFDFF)

/* CR1 寄存器掩码 */
#define CR1_CLEAR_Mask              ((uint32_t)0xFFF0FEFF)

/* ADC ADON 掩码 */
#define CR2_ADON_Set                ((uint32_t)0x00000001)
#define CR2_ADON_Reset              ((uint32_t)0xFFFFFFFE)

/* ADC DMA 掩码 */
#define CR2_DMA_Set                 ((uint32_t)0x00000100)
#define CR2_DMA_Reset               ((uint32_t)0xFFFFFEFF)

/* ADC RSTCAL 掩码 */
#define CR2_RSTCAL_Set              ((uint32_t)0x00000008)

/* ADC CAL 掩码 */
#define CR2_CAL_Set                 ((uint32_t)0x00000004)

/* ADC SWSTART 掩码 */
#define CR2_SWSTART_Set             ((uint32_t)0x00400000)

/* ADC EXTTRIG 掩码 */
#define CR2_EXTTRIG_Set             ((uint32_t)0x00100000)
#define CR2_EXTTRIG_Reset           ((uint32_t)0xFFEFFFFF)

/* ADC 软件启动掩码 */
#define CR2_EXTTRIG_SWSTART_Set     ((uint32_t)0x00500000)
#define CR2_EXTTRIG_SWSTART_Reset   ((uint32_t)0xFFAFFFFF)

/* ADC JEXTSEL 掩码 */
#define CR2_JEXTSEL_Reset           ((uint32_t)0xFFFF8FFF)

/* ADC JEXTTRIG 掩码 */
#define CR2_JEXTTRIG_Set            ((uint32_t)0x00008000)
#define CR2_JEXTTRIG_Reset          ((uint32_t)0xFFFF7FFF)

/* ADC JSWSTART 掩码 */
#define CR2_JSWSTART_Set            ((uint32_t)0x00200000)

/* ADC 注入组软件启动掩码 */
#define CR2_JEXTTRIG_JSWSTART_Set   ((uint32_t)0x00208000)
#define CR2_JEXTTRIG_JSWSTART_Reset ((uint32_t)0xFFDF7FFF)

/* ADC TSPD 掩码 */
#define CR2_TSVREFE_Set             ((uint32_t)0x00800000)
#define CR2_TSVREFE_Reset           ((uint32_t)0xFF7FFFFF)

/* CR2 寄存器掩码 */
#define CR2_CLEAR_Mask              ((uint32_t)0xFFF1F7FD)

/* ADC SQx 掩码 */
#define SQR3_SQ_Set                 ((uint32_t)0x0000001F)
#define SQR2_SQ_Set                 ((uint32_t)0x0000001F)
#define SQR1_SQ_Set                 ((uint32_t)0x0000001F)

/* SQR1 寄存器掩码 */
#define SQR1_CLEAR_Mask             ((uint32_t)0xFF0FFFFF)

/* ADC JSQx 掩码 */
#define JSQR_JSQ_Set                ((uint32_t)0x0000001F)

/* ADC JL 掩码 */
#define JSQR_JL_Set                 ((uint32_t)0x00300000)
#define JSQR_JL_Reset               ((uint32_t)0xFFCFFFFF)

/* ADC SMPx 掩码 */
#define SMPR1_SMP_Set               ((uint32_t)0x00000007)
#define SMPR2_SMP_Set               ((uint32_t)0x00000007)

/* ADC JDRx 寄存器偏移量 */
#define JDR_Offset                  ((uint8_t)0x28)

/* ADC1 DR 寄存器基址 */
#define DR_ADDRESS                  ((uint32_t)0x4001244C)

/**
  * @}
  */

/** @defgroup ADC_Private_Macros   ADC 私有宏
  * @{
  */

/**
  * @}
  */

/** @defgroup ADC_Private_Variables   ADC 私有变量
  * @{
  */

/**
  * @}
  */

/** @defgroup ADC_Private_FunctionPrototypes   ADC 私有函数原型
  * @{
  */

/**
  * @}
  */

/** @defgroup ADC_Private_Functions   ADC 私有函数
  * @{
  */

/**
  * @brief  将 ADCx 外设寄存器反初始化，恢复为默认复位值。
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @retval 无
  */
void ADC_DeInit(ADC_TypeDef* ADCx)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  
  if (ADCx == ADC1)
  {
    /* 使 ADC1 进入复位状态 */
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);
    /* 将 ADC1 从复位状态释放 */
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);
  }
  else if (ADCx == ADC2)
  {
    /* 使 ADC2 进入复位状态 */
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, ENABLE);
    /* 将 ADC2 从复位状态释放 */
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, DISABLE);
  }
  else
  {
    if (ADCx == ADC3)
    {
      /* 使 ADC3 进入复位状态 */
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, ENABLE);
      /* 将 ADC3 从复位状态释放 */
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, DISABLE);
    }
  }
}

/**
  * @brief  根据 ADC_InitStruct 中指定的参数初始化 ADCx 外设。
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @param  ADC_InitStruct: 指向 ADC_InitTypeDef 结构体的指针，
  *         该结构体包含指定 ADC 外设的配置信息。
  * @retval 无
  */
void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct)
{
  uint32_t tmpreg1 = 0;
  uint8_t tmpreg2 = 0;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_MODE(ADC_InitStruct->ADC_Mode));
  assert_param(IS_FUNCTIONAL_STATE(ADC_InitStruct->ADC_ScanConvMode));
  assert_param(IS_FUNCTIONAL_STATE(ADC_InitStruct->ADC_ContinuousConvMode));
  assert_param(IS_ADC_EXT_TRIG(ADC_InitStruct->ADC_ExternalTrigConv));   
  assert_param(IS_ADC_DATA_ALIGN(ADC_InitStruct->ADC_DataAlign)); 
  assert_param(IS_ADC_REGULAR_LENGTH(ADC_InitStruct->ADC_NbrOfChannel));

  /*---------------------------- 配置 ADCx 的 CR1 寄存器 -----------------*/
  /* 读取 ADCx 的 CR1 寄存器值 */
  tmpreg1 = ADCx->CR1;
  /* 清除 DUALMOD 与 SCAN 位 */
  tmpreg1 &= CR1_CLEAR_Mask;
  /* 配置 ADCx：双重模式与扫描转换模式 */
  /* 根据 ADC_Mode 的值设置 DUALMOD 位 */
  /* 根据 ADC_ScanConvMode 的值设置 SCAN 位 */
  tmpreg1 |= (uint32_t)(ADC_InitStruct->ADC_Mode | ((uint32_t)ADC_InitStruct->ADC_ScanConvMode << 8));
  /* 写入 ADCx 的 CR1 寄存器 */
  ADCx->CR1 = tmpreg1;

  /*---------------------------- 配置 ADCx 的 CR2 寄存器 -----------------*/
  /* 读取 ADCx 的 CR2 寄存器值 */
  tmpreg1 = ADCx->CR2;
  /* 清除 CONT、ALIGN 与 EXTSEL 位 */
  tmpreg1 &= CR2_CLEAR_Mask;
  /* 配置 ADCx：外部触发事件与连续转换模式 */
  /* 根据 ADC_DataAlign 的值设置 ALIGN 位 */
  /* 根据 ADC_ExternalTrigConv 的值设置 EXTSEL 位 */
  /* 根据 ADC_ContinuousConvMode 的值设置 CONT 位 */
  tmpreg1 |= (uint32_t)(ADC_InitStruct->ADC_DataAlign | ADC_InitStruct->ADC_ExternalTrigConv |
            ((uint32_t)ADC_InitStruct->ADC_ContinuousConvMode << 1));
  /* 写入 ADCx 的 CR2 寄存器 */
  ADCx->CR2 = tmpreg1;

  /*---------------------------- 配置 ADCx 的 SQR1 寄存器 -----------------*/
  /* 读取 ADCx 的 SQR1 寄存器值 */
  tmpreg1 = ADCx->SQR1;
  /* 清除 L 位 */
  tmpreg1 &= SQR1_CLEAR_Mask;
  /* 配置 ADCx：规则通道序列的长度 */
  /* 根据 ADC_NbrOfChannel 的值设置 L 位 */
  tmpreg2 |= (uint8_t) (ADC_InitStruct->ADC_NbrOfChannel - (uint8_t)1);
  tmpreg1 |= (uint32_t)tmpreg2 << 20;
  /* 写入 ADCx 的 SQR1 寄存器 */
  ADCx->SQR1 = tmpreg1;
}

/**
  * @brief  将 ADC_InitStruct 的每个成员填充为默认值。
  * @param  ADC_InitStruct : 指向待初始化的 ADC_InitTypeDef 结构体的指针。
  * @retval 无
  */
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct)
{
  /* 复位 ADC 初始化结构体各成员的值 */
  /* 初始化 ADC_Mode 成员 */
  ADC_InitStruct->ADC_Mode = ADC_Mode_Independent;
  /* 初始化 ADC_ScanConvMode 成员 */
  ADC_InitStruct->ADC_ScanConvMode = DISABLE;
  /* 初始化 ADC_ContinuousConvMode 成员 */
  ADC_InitStruct->ADC_ContinuousConvMode = DISABLE;
  /* 初始化 ADC_ExternalTrigConv 成员 */
  ADC_InitStruct->ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  /* 初始化 ADC_DataAlign 成员 */
  ADC_InitStruct->ADC_DataAlign = ADC_DataAlign_Right;
  /* 初始化 ADC_NbrOfChannel 成员 */
  ADC_InitStruct->ADC_NbrOfChannel = 1;
}

/**
  * @brief  使能或关闭指定的 ADC 外设。
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @param  NewState: ADCx 外设的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void ADC_Cmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 置位 ADON 位，将 ADC 从掉电模式唤醒 */
    ADCx->CR2 |= CR2_ADON_Set;
  }
  else
  {
    /* 关闭所选 ADC 外设 */
    ADCx->CR2 &= CR2_ADON_Reset;
  }
}

/**
  * @brief  使能或关闭指定 ADC 的 DMA 请求。
  * @param  ADCx: x 可取 1 或 3，用于选择 ADC 外设。
  *   注意：ADC2 不具备 DMA 能力。
  * @param  NewState: 所选 ADC DMA 传输的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void ADC_DMACmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_ADC_DMA_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能所选 ADC 的 DMA 请求 */
    ADCx->CR2 |= CR2_DMA_Set;
  }
  else
  {
    /* 关闭所选 ADC 的 DMA 请求 */
    ADCx->CR2 &= CR2_DMA_Reset;
  }
}

/**
  * @brief  使能或关闭指定的 ADC 中断。
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @param  ADC_IT: 指定要使能或关闭的 ADC 中断源。
  *   该参数可取下列值的任意组合：
  *     @arg ADC_IT_EOC: 转换结束中断掩码
  *     @arg ADC_IT_AWD: 模拟看门狗中断掩码
  *     @arg ADC_IT_JEOC: 注入组转换结束中断掩码
  * @param  NewState: 指定 ADC 中断的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void ADC_ITConfig(ADC_TypeDef* ADCx, uint16_t ADC_IT, FunctionalState NewState)
{
  uint8_t itmask = 0;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_ADC_IT(ADC_IT));
  /* 获取 ADC 中断序号 */
  itmask = (uint8_t)ADC_IT;
  if (NewState != DISABLE)
  {
    /* 使能所选 ADC 中断 */
    ADCx->CR1 |= itmask;
  }
  else
  {
    /* 关闭所选 ADC 中断 */
    ADCx->CR1 &= (~(uint32_t)itmask);
  }
}

/**
  * @brief  复位所选 ADC 的校准寄存器。
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @retval 无
  */
void ADC_ResetCalibration(ADC_TypeDef* ADCx)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  /* 复位所选 ADC 的校准寄存器 */  
  ADCx->CR2 |= CR2_RSTCAL_Set;
}

/**
  * @brief  获取所选 ADC 复位校准寄存器的状态。
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @retval ADC 复位校准寄存器的新状态（SET 或 RESET）。
  */
FlagStatus ADC_GetResetCalibrationStatus(ADC_TypeDef* ADCx)
{
  FlagStatus bitstatus = RESET;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  /* 检查 RSTCAL 位的状态 */
  if ((ADCx->CR2 & CR2_RSTCAL_Set) != (uint32_t)RESET)
  {
    /* RSTCAL 位已置位 */
    bitstatus = SET;
  }
  else
  {
    /* RSTCAL 位已清零 */
    bitstatus = RESET;
  }
  /* 返回 RSTCAL 位的状态 */
  return  bitstatus;
}

/**
  * @brief  启动所选 ADC 的校准过程。
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @retval 无
  */
void ADC_StartCalibration(ADC_TypeDef* ADCx)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  /* 使能所选 ADC 的校准过程 */  
  ADCx->CR2 |= CR2_CAL_Set;
}

/**
  * @brief  获取所选 ADC 的校准状态。
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @retval ADC 校准的新状态（SET 或 RESET）。
  */
FlagStatus ADC_GetCalibrationStatus(ADC_TypeDef* ADCx)
{
  FlagStatus bitstatus = RESET;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  /* 检查 CAL 位的状态 */
  if ((ADCx->CR2 & CR2_CAL_Set) != (uint32_t)RESET)
  {
    /* CAL 位已置位：校准正在进行 */
    bitstatus = SET;
  }
  else
  {
    /* CAL 位已清零：校准结束 */
    bitstatus = RESET;
  }
  /* 返回 CAL 位的状态 */
  return  bitstatus;
}

/**
  * @brief  使能或关闭所选 ADC 的软件启动转换。
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @param  NewState: 所选 ADC 软件启动转换的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void ADC_SoftwareStartConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能所选 ADC 在外部事件上触发转换，并启动所选 ADC 的转换 */
    ADCx->CR2 |= CR2_EXTTRIG_SWSTART_Set;
  }
  else
  {
    /* 关闭所选 ADC 在外部事件上触发转换，并停止所选 ADC 的转换 */
    ADCx->CR2 &= CR2_EXTTRIG_SWSTART_Reset;
  }
}

/**
  * @brief  获取所选 ADC 软件启动转换的状态。
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @retval ADC 软件启动转换的新状态（SET 或 RESET）。
  */
FlagStatus ADC_GetSoftwareStartConvStatus(ADC_TypeDef* ADCx)
{
  FlagStatus bitstatus = RESET;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  /* 检查 SWSTART 位的状态 */
  if ((ADCx->CR2 & CR2_SWSTART_Set) != (uint32_t)RESET)
  {
    /* SWSTART 位已置位 */
    bitstatus = SET;
  }
  else
  {
    /* SWSTART 位已清零 */
    bitstatus = RESET;
  }
  /* 返回 SWSTART 位的状态 */
  return  bitstatus;
}

/**
  * @brief  配置所选 ADC 规则组的间断模式通道。
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @param  Number: 指定间断模式下规则通道的计数值。
  *         该数值必须在 1 到 8 之间。
  * @retval 无
  */
void ADC_DiscModeChannelCountConfig(ADC_TypeDef* ADCx, uint8_t Number)
{
  uint32_t tmpreg1 = 0;
  uint32_t tmpreg2 = 0;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_REGULAR_DISC_NUMBER(Number));
  /* 读取寄存器旧值 */
  tmpreg1 = ADCx->CR1;
  /* 清除原有的间断模式通道计数 */
  tmpreg1 &= CR1_DISCNUM_Reset;
  /* 设置间断模式通道计数 */
  tmpreg2 = Number - 1;
  tmpreg1 |= tmpreg2 << 13;
  /* 保存寄存器新值 */
  ADCx->CR1 = tmpreg1;
}

/**
  * @brief  针对指定 ADC，使能或关闭规则组通道的间断模式。
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @param  NewState: 所选 ADC 规则组通道间断模式的新状态。
  *         该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void ADC_DiscModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能所选 ADC 的规则组间断模式 */
    ADCx->CR1 |= CR1_DISCEN_Set;
  }
  else
  {
    /* 关闭所选 ADC 的规则组间断模式 */
    ADCx->CR1 &= CR1_DISCEN_Reset;
  }
}

/**
  * @brief  针对所选 ADC 的规则通道，配置其在序列器中的排序位置及采样时间。
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @param  ADC_Channel: 待配置的 ADC 通道。
  *   该参数可取下列值之一：
  *     @arg ADC_Channel_0: 选择 ADC 通道 0
  *     @arg ADC_Channel_1: 选择 ADC 通道 1
  *     @arg ADC_Channel_2: 选择 ADC 通道 2
  *     @arg ADC_Channel_3: 选择 ADC 通道 3
  *     @arg ADC_Channel_4: 选择 ADC 通道 4
  *     @arg ADC_Channel_5: 选择 ADC 通道 5
  *     @arg ADC_Channel_6: 选择 ADC 通道 6
  *     @arg ADC_Channel_7: 选择 ADC 通道 7
  *     @arg ADC_Channel_8: 选择 ADC 通道 8
  *     @arg ADC_Channel_9: 选择 ADC 通道 9
  *     @arg ADC_Channel_10: 选择 ADC 通道 10
  *     @arg ADC_Channel_11: 选择 ADC 通道 11
  *     @arg ADC_Channel_12: 选择 ADC 通道 12
  *     @arg ADC_Channel_13: 选择 ADC 通道 13
  *     @arg ADC_Channel_14: 选择 ADC 通道 14
  *     @arg ADC_Channel_15: 选择 ADC 通道 15
  *     @arg ADC_Channel_16: 选择 ADC 通道 16
  *     @arg ADC_Channel_17: 选择 ADC 通道 17
  * @param  Rank: 规则组序列器中的排序位置。该参数必须在 1 到 16 之间。
  * @param  ADC_SampleTime: 为所选通道设置的采样时间值。
  *   该参数可取下列值之一：
  *     @arg ADC_SampleTime_1Cycles5: 采样时间等于 1.5 个周期
  *     @arg ADC_SampleTime_7Cycles5: 采样时间等于 7.5 个周期
  *     @arg ADC_SampleTime_13Cycles5: 采样时间等于 13.5 个周期
  *     @arg ADC_SampleTime_28Cycles5: 采样时间等于 28.5 个周期
  *     @arg ADC_SampleTime_41Cycles5: 采样时间等于 41.5 个周期
  *     @arg ADC_SampleTime_55Cycles5: 采样时间等于 55.5 个周期
  *     @arg ADC_SampleTime_71Cycles5: 采样时间等于 71.5 个周期
  *     @arg ADC_SampleTime_239Cycles5: 采样时间等于 239.5 个周期
  * @retval 无
  */
void ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime)
{
  uint32_t tmpreg1 = 0, tmpreg2 = 0;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CHANNEL(ADC_Channel));
  assert_param(IS_ADC_REGULAR_RANK(Rank));
  assert_param(IS_ADC_SAMPLE_TIME(ADC_SampleTime));
  /* 若选择的是 ADC_Channel_10 ... ADC_Channel_17 */
  if (ADC_Channel > ADC_Channel_9)
  {
    /* 读取寄存器旧值 */
    tmpreg1 = ADCx->SMPR1;
    /* 计算要清除的掩码 */
    tmpreg2 = SMPR1_SMP_Set << (3 * (ADC_Channel - 10));
    /* 清除原有的通道采样时间 */
    tmpreg1 &= ~tmpreg2;
    /* 计算要设置的掩码 */
    tmpreg2 = (uint32_t)ADC_SampleTime << (3 * (ADC_Channel - 10));
    /* 设置新的通道采样时间 */
    tmpreg1 |= tmpreg2;
    /* 保存寄存器新值 */
    ADCx->SMPR1 = tmpreg1;
  }
  else /* 所选通道属于 ADC_Channel_[0..9] */
  {
    /* 读取寄存器旧值 */
    tmpreg1 = ADCx->SMPR2;
    /* 计算要清除的掩码 */
    tmpreg2 = SMPR2_SMP_Set << (3 * ADC_Channel);
    /* 清除原有的通道采样时间 */
    tmpreg1 &= ~tmpreg2;
    /* 计算要设置的掩码 */
    tmpreg2 = (uint32_t)ADC_SampleTime << (3 * ADC_Channel);
    /* 设置新的通道采样时间 */
    tmpreg1 |= tmpreg2;
    /* 保存寄存器新值 */
    ADCx->SMPR2 = tmpreg1;
  }
  /* 对应排序位置 1 到 6 */
  if (Rank < 7)
  {
    /* 读取寄存器旧值 */
    tmpreg1 = ADCx->SQR3;
    /* 计算要清除的掩码 */
    tmpreg2 = SQR3_SQ_Set << (5 * (Rank - 1));
    /* 清除所选排序位置原有的 SQx 位 */
    tmpreg1 &= ~tmpreg2;
    /* 计算要设置的掩码 */
    tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 1));
    /* 为所选排序位置设置 SQx 位 */
    tmpreg1 |= tmpreg2;
    /* 保存寄存器新值 */
    ADCx->SQR3 = tmpreg1;
  }
  /* 对应排序位置 7 到 12 */
  else if (Rank < 13)
  {
    /* 读取寄存器旧值 */
    tmpreg1 = ADCx->SQR2;
    /* 计算要清除的掩码 */
    tmpreg2 = SQR2_SQ_Set << (5 * (Rank - 7));
    /* 清除所选排序位置原有的 SQx 位 */
    tmpreg1 &= ~tmpreg2;
    /* 计算要设置的掩码 */
    tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 7));
    /* 为所选排序位置设置 SQx 位 */
    tmpreg1 |= tmpreg2;
    /* 保存寄存器新值 */
    ADCx->SQR2 = tmpreg1;
  }
  /* 对应排序位置 13 到 16 */
  else
  {
    /* 读取寄存器旧值 */
    tmpreg1 = ADCx->SQR1;
    /* 计算要清除的掩码 */
    tmpreg2 = SQR1_SQ_Set << (5 * (Rank - 13));
    /* 清除所选排序位置原有的 SQx 位 */
    tmpreg1 &= ~tmpreg2;
    /* 计算要设置的掩码 */
    tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 13));
    /* 为所选排序位置设置 SQx 位 */
    tmpreg1 |= tmpreg2;
    /* 保存寄存器新值 */
    ADCx->SQR1 = tmpreg1;
  }
}

/**
  * @brief  使能或关闭 ADCx 通过外部触发进行转换。
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @param  NewState: 所选 ADC 外部触发启动转换的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void ADC_ExternalTrigConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能所选 ADC 在外部事件上触发转换 */
    ADCx->CR2 |= CR2_EXTTRIG_Set;
  }
  else
  {
    /* 关闭所选 ADC 在外部事件上触发转换 */
    ADCx->CR2 &= CR2_EXTTRIG_Reset;
  }
}

/**
  * @brief  返回 ADCx 规则通道最近一次的转换结果数据。
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @retval 转换得到的数据值。
  */
uint16_t ADC_GetConversionValue(ADC_TypeDef* ADCx)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  /* 返回所选 ADC 的转换值 */
  return (uint16_t) ADCx->DR;
}

/**
  * @brief  返回双重模式下 ADC1 与 ADC2 最近一次的转换结果数据。
  * @retval 转换得到的数据值。
  */
uint32_t ADC_GetDualModeConversionValue(void)
{
  /* 返回双重模式的转换值 */
  return (*(__IO uint32_t *) DR_ADDRESS);
}

/**
  * @brief  使能或关闭所选 ADC 在规则组转换之后自动进行注入组转换。
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @param  NewState: 所选 ADC 自动注入转换的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void ADC_AutoInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能所选 ADC 的注入组自动转换 */
    ADCx->CR1 |= CR1_JAUTO_Set;
  }
  else
  {
    /* 关闭所选 ADC 的注入组自动转换 */
    ADCx->CR1 &= CR1_JAUTO_Reset;
  }
}

/**
  * @brief  针对指定 ADC，使能或关闭注入组通道的间断模式。
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @param  NewState: 所选 ADC 注入组通道间断模式的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void ADC_InjectedDiscModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能所选 ADC 的注入组间断模式 */
    ADCx->CR1 |= CR1_JDISCEN_Set;
  }
  else
  {
    /* 关闭所选 ADC 的注入组间断模式 */
    ADCx->CR1 &= CR1_JDISCEN_Reset;
  }
}

/**
  * @brief  配置 ADCx 注入通道转换的外部触发源。
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @param  ADC_ExternalTrigInjecConv: 指定启动注入转换的 ADC 触发源。
  *   该参数可取下列值之一：
  *     @arg ADC_ExternalTrigInjecConv_T1_TRGO: 选择定时器 1 的 TRGO 事件（适用于 ADC1、ADC2 和 ADC3）
  *     @arg ADC_ExternalTrigInjecConv_T1_CC4: 选择定时器 1 的捕获比较 4（适用于 ADC1、ADC2 和 ADC3）
  *     @arg ADC_ExternalTrigInjecConv_T2_TRGO: 选择定时器 2 的 TRGO 事件（适用于 ADC1 和 ADC2）
  *     @arg ADC_ExternalTrigInjecConv_T2_CC1: 选择定时器 2 的捕获比较 1（适用于 ADC1 和 ADC2）
  *     @arg ADC_ExternalTrigInjecConv_T3_CC4: 选择定时器 3 的捕获比较 4（适用于 ADC1 和 ADC2）
  *     @arg ADC_ExternalTrigInjecConv_T4_TRGO: 选择定时器 4 的 TRGO 事件（适用于 ADC1 和 ADC2）
  *     @arg ADC_ExternalTrigInjecConv_Ext_IT15_TIM8_CC4: 选择外部中断线 15 或定时器 8
  *                                                       的捕获比较 4 事件（适用于 ADC1 和 ADC2）
  *     @arg ADC_ExternalTrigInjecConv_T4_CC3: 选择定时器 4 的捕获比较 3（仅适用于 ADC3）
  *     @arg ADC_ExternalTrigInjecConv_T8_CC2: 选择定时器 8 的捕获比较 2（仅适用于 ADC3）
  *     @arg ADC_ExternalTrigInjecConv_T8_CC4: 选择定时器 8 的捕获比较 4（仅适用于 ADC3）
  *     @arg ADC_ExternalTrigInjecConv_T5_TRGO: 选择定时器 5 的 TRGO 事件（仅适用于 ADC3）
  *     @arg ADC_ExternalTrigInjecConv_T5_CC4: 选择定时器 5 的捕获比较 4（仅适用于 ADC3）
  *     @arg ADC_ExternalTrigInjecConv_None: 注入转换由软件启动而非外部触发
  *                                          （适用于 ADC1、ADC2 和 ADC3）
  * @retval 无
  */
void ADC_ExternalTrigInjectedConvConfig(ADC_TypeDef* ADCx, uint32_t ADC_ExternalTrigInjecConv)
{
  uint32_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_EXT_INJEC_TRIG(ADC_ExternalTrigInjecConv));
  /* 读取寄存器旧值 */
  tmpreg = ADCx->CR2;
  /* 清除注入组原有的外部事件选择 */
  tmpreg &= CR2_JEXTSEL_Reset;
  /* 设置注入组的外部事件选择 */
  tmpreg |= ADC_ExternalTrigInjecConv;
  /* 保存寄存器新值 */
  ADCx->CR2 = tmpreg;
}

/**
  * @brief  使能或关闭 ADCx 注入通道通过外部触发进行转换。
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @param  NewState: 所选 ADC 外部触发启动注入转换的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void ADC_ExternalTrigInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能所选 ADC 注入组的外部事件选择 */
    ADCx->CR2 |= CR2_JEXTTRIG_Set;
  }
  else
  {
    /* 关闭所选 ADC 注入组的外部事件选择 */
    ADCx->CR2 &= CR2_JEXTTRIG_Reset;
  }
}

/**
  * @brief  使能或关闭所选 ADC 注入通道转换的启动。
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @param  NewState: 所选 ADC 软件启动注入转换的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void ADC_SoftwareStartInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能所选 ADC 注入组在外部事件上触发转换，并启动所选 ADC 的注入转换 */
    ADCx->CR2 |= CR2_JEXTTRIG_JSWSTART_Set;
  }
  else
  {
    /* 关闭所选 ADC 注入组在外部事件上触发转换，并停止所选 ADC 的注入转换 */
    ADCx->CR2 &= CR2_JEXTTRIG_JSWSTART_Reset;
  }
}

/**
  * @brief  获取所选 ADC 软件启动注入转换的状态。
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @retval ADC 软件启动注入转换的新状态（SET 或 RESET）。
  */
FlagStatus ADC_GetSoftwareStartInjectedConvCmdStatus(ADC_TypeDef* ADCx)
{
  FlagStatus bitstatus = RESET;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  /* 检查 JSWSTART 位的状态 */
  if ((ADCx->CR2 & CR2_JSWSTART_Set) != (uint32_t)RESET)
  {
    /* JSWSTART 位已置位 */
    bitstatus = SET;
  }
  else
  {
    /* JSWSTART 位已清零 */
    bitstatus = RESET;
  }
  /* 返回 JSWSTART 位的状态 */
  return  bitstatus;
}

/**
  * @brief  针对所选 ADC 的注入通道，配置其在序列器中的排序位置及采样时间。
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @param  ADC_Channel: 待配置的 ADC 通道。
  *   该参数可取下列值之一：
  *     @arg ADC_Channel_0: 选择 ADC 通道 0
  *     @arg ADC_Channel_1: 选择 ADC 通道 1
  *     @arg ADC_Channel_2: 选择 ADC 通道 2
  *     @arg ADC_Channel_3: 选择 ADC 通道 3
  *     @arg ADC_Channel_4: 选择 ADC 通道 4
  *     @arg ADC_Channel_5: 选择 ADC 通道 5
  *     @arg ADC_Channel_6: 选择 ADC 通道 6
  *     @arg ADC_Channel_7: 选择 ADC 通道 7
  *     @arg ADC_Channel_8: 选择 ADC 通道 8
  *     @arg ADC_Channel_9: 选择 ADC 通道 9
  *     @arg ADC_Channel_10: 选择 ADC 通道 10
  *     @arg ADC_Channel_11: 选择 ADC 通道 11
  *     @arg ADC_Channel_12: 选择 ADC 通道 12
  *     @arg ADC_Channel_13: 选择 ADC 通道 13
  *     @arg ADC_Channel_14: 选择 ADC 通道 14
  *     @arg ADC_Channel_15: 选择 ADC 通道 15
  *     @arg ADC_Channel_16: 选择 ADC 通道 16
  *     @arg ADC_Channel_17: 选择 ADC 通道 17
  * @param  Rank: 注入组序列器中的排序位置。该参数必须在 1 到 4 之间。
  * @param  ADC_SampleTime: 为所选通道设置的采样时间值。
  *   该参数可取下列值之一：
  *     @arg ADC_SampleTime_1Cycles5: 采样时间等于 1.5 个周期
  *     @arg ADC_SampleTime_7Cycles5: 采样时间等于 7.5 个周期
  *     @arg ADC_SampleTime_13Cycles5: 采样时间等于 13.5 个周期
  *     @arg ADC_SampleTime_28Cycles5: 采样时间等于 28.5 个周期
  *     @arg ADC_SampleTime_41Cycles5: 采样时间等于 41.5 个周期
  *     @arg ADC_SampleTime_55Cycles5: 采样时间等于 55.5 个周期
  *     @arg ADC_SampleTime_71Cycles5: 采样时间等于 71.5 个周期
  *     @arg ADC_SampleTime_239Cycles5: 采样时间等于 239.5 个周期
  * @retval 无
  */
void ADC_InjectedChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime)
{
  uint32_t tmpreg1 = 0, tmpreg2 = 0, tmpreg3 = 0;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CHANNEL(ADC_Channel));
  assert_param(IS_ADC_INJECTED_RANK(Rank));
  assert_param(IS_ADC_SAMPLE_TIME(ADC_SampleTime));
  /* 若选择的是 ADC_Channel_10 ... ADC_Channel_17 */
  if (ADC_Channel > ADC_Channel_9)
  {
    /* 读取寄存器旧值 */
    tmpreg1 = ADCx->SMPR1;
    /* 计算要清除的掩码 */
    tmpreg2 = SMPR1_SMP_Set << (3*(ADC_Channel - 10));
    /* 清除原有的通道采样时间 */
    tmpreg1 &= ~tmpreg2;
    /* 计算要设置的掩码 */
    tmpreg2 = (uint32_t)ADC_SampleTime << (3*(ADC_Channel - 10));
    /* 设置新的通道采样时间 */
    tmpreg1 |= tmpreg2;
    /* 保存寄存器新值 */
    ADCx->SMPR1 = tmpreg1;
  }
  else /* 所选通道属于 ADC_Channel_[0..9] */
  {
    /* 读取寄存器旧值 */
    tmpreg1 = ADCx->SMPR2;
    /* 计算要清除的掩码 */
    tmpreg2 = SMPR2_SMP_Set << (3 * ADC_Channel);
    /* 清除原有的通道采样时间 */
    tmpreg1 &= ~tmpreg2;
    /* 计算要设置的掩码 */
    tmpreg2 = (uint32_t)ADC_SampleTime << (3 * ADC_Channel);
    /* 设置新的通道采样时间 */
    tmpreg1 |= tmpreg2;
    /* 保存寄存器新值 */
    ADCx->SMPR2 = tmpreg1;
  }
  /* 排序位置配置 */
  /* 读取寄存器旧值 */
  tmpreg1 = ADCx->JSQR;
  /* 获取 JL 值：通道数 = JL + 1 */
  tmpreg3 =  (tmpreg1 & JSQR_JL_Set)>> 20;
  /* 计算要清除的掩码：((Rank-1)+(4-JL-1)) */
  tmpreg2 = JSQR_JSQ_Set << (5 * (uint8_t)((Rank + 3) - (tmpreg3 + 1)));
  /* 清除所选排序位置原有的 JSQx 位 */
  tmpreg1 &= ~tmpreg2;
  /* 计算要设置的掩码：((Rank-1)+(4-JL-1)) */
  tmpreg2 = (uint32_t)ADC_Channel << (5 * (uint8_t)((Rank + 3) - (tmpreg3 + 1)));
  /* 为所选排序位置设置 JSQx 位 */
  tmpreg1 |= tmpreg2;
  /* 保存寄存器新值 */
  ADCx->JSQR = tmpreg1;
}

/**
  * @brief  配置注入通道的序列器长度
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @param  Length: 序列器长度。
  *   该参数必须是 1 到 4 之间的数值。
  * @retval 无
  */
void ADC_InjectedSequencerLengthConfig(ADC_TypeDef* ADCx, uint8_t Length)
{
  uint32_t tmpreg1 = 0;
  uint32_t tmpreg2 = 0;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_INJECTED_LENGTH(Length));
  
  /* 读取寄存器旧值 */
  tmpreg1 = ADCx->JSQR;
  /* 清除原有的注入序列长度 JL 位 */
  tmpreg1 &= JSQR_JL_Reset;
  /* 设置注入序列长度 JL 位 */
  tmpreg2 = Length - 1; 
  tmpreg1 |= tmpreg2 << 20;
  /* 保存寄存器新值 */
  ADCx->JSQR = tmpreg1;
}

/**
  * @brief  设置注入通道转换值的偏移量
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @param  ADC_InjectedChannel: 待设置偏移量的 ADC 注入通道。
  *   该参数可取下列值之一：
  *     @arg ADC_InjectedChannel_1: 选择注入通道 1
  *     @arg ADC_InjectedChannel_2: 选择注入通道 2
  *     @arg ADC_InjectedChannel_3: 选择注入通道 3
  *     @arg ADC_InjectedChannel_4: 选择注入通道 4
  * @param  Offset: 所选 ADC 注入通道的偏移量值
  *   该参数必须是一个 12 位数值。
  * @retval 无
  */
void ADC_SetInjectedOffset(ADC_TypeDef* ADCx, uint8_t ADC_InjectedChannel, uint16_t Offset)
{
  __IO uint32_t tmp = 0;
  
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_INJECTED_CHANNEL(ADC_InjectedChannel));
  assert_param(IS_ADC_OFFSET(Offset));  
  
  tmp = (uint32_t)ADCx;
  tmp += ADC_InjectedChannel;
  
  /* 设置所选注入通道的数据偏移量 */
  *(__IO uint32_t *) tmp = (uint32_t)Offset;
}

/**
  * @brief  返回 ADC 注入通道的转换结果
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @param  ADC_InjectedChannel: 已转换的 ADC 注入通道。
  *   该参数可取下列值之一：
  *     @arg ADC_InjectedChannel_1: 选择注入通道 1
  *     @arg ADC_InjectedChannel_2: 选择注入通道 2
  *     @arg ADC_InjectedChannel_3: 选择注入通道 3
  *     @arg ADC_InjectedChannel_4: 选择注入通道 4
  * @retval 转换得到的数据值。
  */
uint16_t ADC_GetInjectedConversionValue(ADC_TypeDef* ADCx, uint8_t ADC_InjectedChannel)
{
  __IO uint32_t tmp = 0;
  
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_INJECTED_CHANNEL(ADC_InjectedChannel));

  tmp = (uint32_t)ADCx;
  tmp += ADC_InjectedChannel + JDR_Offset;
  
  /* 返回所选注入通道的转换数据值 */
  return (uint16_t) (*(__IO uint32_t*)  tmp);   
}

/**
  * @brief  使能或关闭针对单个/全部规则通道或注入通道的模拟看门狗
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @param  ADC_AnalogWatchdog: ADC 模拟看门狗的配置。
  *   该参数可取下列值之一：
  *     @arg ADC_AnalogWatchdog_SingleRegEnable: 模拟看门狗监视单个规则通道
  *     @arg ADC_AnalogWatchdog_SingleInjecEnable: 模拟看门狗监视单个注入通道
  *     @arg ADC_AnalogWatchdog_SingleRegOrInjecEnable: 模拟看门狗监视单个规则或注入通道
  *     @arg ADC_AnalogWatchdog_AllRegEnable: 模拟看门狗监视全部规则通道
  *     @arg ADC_AnalogWatchdog_AllInjecEnable: 模拟看门狗监视全部注入通道
  *     @arg ADC_AnalogWatchdog_AllRegAllInjecEnable: 模拟看门狗监视全部规则通道与注入通道
  *     @arg ADC_AnalogWatchdog_None: 模拟看门狗不监视任何通道
  * @retval 无
  */
void ADC_AnalogWatchdogCmd(ADC_TypeDef* ADCx, uint32_t ADC_AnalogWatchdog)
{
  uint32_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_ANALOG_WATCHDOG(ADC_AnalogWatchdog));
  /* 读取寄存器旧值 */
  tmpreg = ADCx->CR1;
  /* 清除 AWDEN、AWDENJ 与 AWDSGL 位 */
  tmpreg &= CR1_AWDMode_Reset;
  /* 设置模拟看门狗的使能模式 */
  tmpreg |= ADC_AnalogWatchdog;
  /* 保存寄存器新值 */
  ADCx->CR1 = tmpreg;
}

/**
  * @brief  配置模拟看门狗的高、低阈值。
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @param  HighThreshold: ADC 模拟看门狗的高阈值。
  *   该参数必须是一个 12 位数值。
  * @param  LowThreshold: ADC 模拟看门狗的低阈值。
  *   该参数必须是一个 12 位数值。
  * @retval 无
  */
void ADC_AnalogWatchdogThresholdsConfig(ADC_TypeDef* ADCx, uint16_t HighThreshold,
                                        uint16_t LowThreshold)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_THRESHOLD(HighThreshold));
  assert_param(IS_ADC_THRESHOLD(LowThreshold));
  /* 设置 ADCx 的高阈值 */
  ADCx->HTR = HighThreshold;
  /* 设置 ADCx 的低阈值 */
  ADCx->LTR = LowThreshold;
}

/**
  * @brief  配置模拟看门狗所监视的单个通道
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @param  ADC_Channel: 待配置为由模拟看门狗监视的 ADC 通道。
  *   该参数可取下列值之一：
  *     @arg ADC_Channel_0: 选择 ADC 通道 0
  *     @arg ADC_Channel_1: 选择 ADC 通道 1
  *     @arg ADC_Channel_2: 选择 ADC 通道 2
  *     @arg ADC_Channel_3: 选择 ADC 通道 3
  *     @arg ADC_Channel_4: 选择 ADC 通道 4
  *     @arg ADC_Channel_5: 选择 ADC 通道 5
  *     @arg ADC_Channel_6: 选择 ADC 通道 6
  *     @arg ADC_Channel_7: 选择 ADC 通道 7
  *     @arg ADC_Channel_8: 选择 ADC 通道 8
  *     @arg ADC_Channel_9: 选择 ADC 通道 9
  *     @arg ADC_Channel_10: 选择 ADC 通道 10
  *     @arg ADC_Channel_11: 选择 ADC 通道 11
  *     @arg ADC_Channel_12: 选择 ADC 通道 12
  *     @arg ADC_Channel_13: 选择 ADC 通道 13
  *     @arg ADC_Channel_14: 选择 ADC 通道 14
  *     @arg ADC_Channel_15: 选择 ADC 通道 15
  *     @arg ADC_Channel_16: 选择 ADC 通道 16
  *     @arg ADC_Channel_17: 选择 ADC 通道 17
  * @retval 无
  */
void ADC_AnalogWatchdogSingleChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel)
{
  uint32_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CHANNEL(ADC_Channel));
  /* 读取寄存器旧值 */
  tmpreg = ADCx->CR1;
  /* 清除模拟看门狗的通道选择位 */
  tmpreg &= CR1_AWDCH_Reset;
  /* 设置模拟看门狗的通道 */
  tmpreg |= ADC_Channel;
  /* 保存寄存器新值 */
  ADCx->CR1 = tmpreg;
}

/**
  * @brief  使能或关闭温度传感器与 Vrefint 通道。
  * @param  NewState: 温度传感器的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void ADC_TempSensorVrefintCmd(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能温度传感器与 Vrefint 通道 */
    ADC1->CR2 |= CR2_TSVREFE_Set;
  }
  else
  {
    /* 关闭温度传感器与 Vrefint 通道 */
    ADC1->CR2 &= CR2_TSVREFE_Reset;
  }
}

/**
  * @brief  检查指定的 ADC 标志是否已置位。
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @param  ADC_FLAG: 指定要检查的标志。
  *   该参数可取下列值之一：
  *     @arg ADC_FLAG_AWD: 模拟看门狗标志
  *     @arg ADC_FLAG_EOC: 转换结束标志
  *     @arg ADC_FLAG_JEOC: 注入组转换结束标志
  *     @arg ADC_FLAG_JSTRT: 注入组转换开始标志
  *     @arg ADC_FLAG_STRT: 规则组转换开始标志
  * @retval ADC_FLAG 的新状态（SET 或 RESET）。
  */
FlagStatus ADC_GetFlagStatus(ADC_TypeDef* ADCx, uint8_t ADC_FLAG)
{
  FlagStatus bitstatus = RESET;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_GET_FLAG(ADC_FLAG));
  /* 检查指定 ADC 标志的状态 */
  if ((ADCx->SR & ADC_FLAG) != (uint8_t)RESET)
  {
    /* ADC_FLAG 已置位 */
    bitstatus = SET;
  }
  else
  {
    /* ADC_FLAG 已清零 */
    bitstatus = RESET;
  }
  /* 返回 ADC_FLAG 的状态 */
  return  bitstatus;
}

/**
  * @brief  清除 ADCx 的待处理标志。
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @param  ADC_FLAG: 指定要清除的标志。
  *   该参数可取下列值的任意组合：
  *     @arg ADC_FLAG_AWD: 模拟看门狗标志
  *     @arg ADC_FLAG_EOC: 转换结束标志
  *     @arg ADC_FLAG_JEOC: 注入组转换结束标志
  *     @arg ADC_FLAG_JSTRT: 注入组转换开始标志
  *     @arg ADC_FLAG_STRT: 规则组转换开始标志
  * @retval 无
  */
void ADC_ClearFlag(ADC_TypeDef* ADCx, uint8_t ADC_FLAG)
{
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CLEAR_FLAG(ADC_FLAG));
  /* 清除所选的 ADC 标志 */
  ADCx->SR = ~(uint32_t)ADC_FLAG;
}

/**
  * @brief  检查指定的 ADC 中断是否已经发生。
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @param  ADC_IT: 指定要检查的 ADC 中断源。
  *   该参数可取下列值之一：
  *     @arg ADC_IT_EOC: 转换结束中断掩码
  *     @arg ADC_IT_AWD: 模拟看门狗中断掩码
  *     @arg ADC_IT_JEOC: 注入组转换结束中断掩码
  * @retval ADC_IT 的新状态（SET 或 RESET）。
  */
ITStatus ADC_GetITStatus(ADC_TypeDef* ADCx, uint16_t ADC_IT)
{
  ITStatus bitstatus = RESET;
  uint32_t itmask = 0, enablestatus = 0;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_GET_IT(ADC_IT));
  /* 获取 ADC 中断序号 */
  itmask = ADC_IT >> 8;
  /* 获取 ADC_IT 使能位的状态 */
  enablestatus = (ADCx->CR1 & (uint8_t)ADC_IT) ;
  /* 检查指定 ADC 中断的状态 */
  if (((ADCx->SR & itmask) != (uint32_t)RESET) && enablestatus)
  {
    /* ADC_IT 已置位 */
    bitstatus = SET;
  }
  else
  {
    /* ADC_IT 已清零 */
    bitstatus = RESET;
  }
  /* 返回 ADC_IT 的状态 */
  return  bitstatus;
}

/**
  * @brief  清除 ADCx 的中断待处理位。
  * @param  ADCx: x 可取 1、2 或 3，用于选择 ADC 外设。
  * @param  ADC_IT: 指定要清除的 ADC 中断待处理位。
  *   该参数可取下列值的任意组合：
  *     @arg ADC_IT_EOC: 转换结束中断掩码
  *     @arg ADC_IT_AWD: 模拟看门狗中断掩码
  *     @arg ADC_IT_JEOC: 注入组转换结束中断掩码
  * @retval 无
  */
void ADC_ClearITPendingBit(ADC_TypeDef* ADCx, uint16_t ADC_IT)
{
  uint8_t itmask = 0;
  /* 检查参数 */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_IT(ADC_IT));
  /* 获取 ADC 中断序号 */
  itmask = (uint8_t)(ADC_IT >> 8);
  /* 清除所选的 ADC 中断待处理位 */
  ADCx->SR = ~(uint32_t)itmask;
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
