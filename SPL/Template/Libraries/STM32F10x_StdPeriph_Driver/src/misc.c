/**
  ******************************************************************************
  * @file    misc.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件提供全部杂项固件函数（CMSIS 函数的补充）。
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
#include "misc.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @defgroup MISC   MISC 驱动模块
  * @brief MISC 驱动模块
  * @{
  */

/** @defgroup MISC_Private_TypesDefinitions   MISC 私有类型定义
  * @{
  */

/**
  * @}
  */ 

/** @defgroup MISC_Private_Defines   MISC 私有宏定义
  * @{
  */

#define AIRCR_VECTKEY_MASK    ((uint32_t)0x05FA0000)
/**
  * @}
  */

/** @defgroup MISC_Private_Macros   MISC 私有宏
  * @{
  */

/**
  * @}
  */

/** @defgroup MISC_Private_Variables   MISC 私有变量
  * @{
  */

/**
  * @}
  */

/** @defgroup MISC_Private_FunctionPrototypes   MISC 私有函数原型
  * @{
  */

/**
  * @}
  */

/** @defgroup MISC_Private_Functions   MISC 私有函数
  * @{
  */

/**
  * @brief  配置优先级分组：抢占优先级与子优先级各占多少位。
  * @param  NVIC_PriorityGroup: 指定优先级分组的位数分配。
  *   该参数可取下列值之一：
  *     @arg NVIC_PriorityGroup_0: 抢占优先级占 0 位
  *                                 子优先级占 4 位
  *     @arg NVIC_PriorityGroup_1: 抢占优先级占 1 位
  *                                 子优先级占 3 位
  *     @arg NVIC_PriorityGroup_2: 抢占优先级占 2 位
  *                                 子优先级占 2 位
  *     @arg NVIC_PriorityGroup_3: 抢占优先级占 3 位
  *                                 子优先级占 1 位
  *     @arg NVIC_PriorityGroup_4: 抢占优先级占 4 位
  *                                 子优先级占 0 位
  * @retval 无
  */
void NVIC_PriorityGroupConfig(uint32_t NVIC_PriorityGroup)
{
  /* 检查参数 */
  assert_param(IS_NVIC_PRIORITY_GROUP(NVIC_PriorityGroup));
  
  /* 根据 NVIC_PriorityGroup 的值设置 PRIGROUP[10:8] 位 */
  SCB->AIRCR = AIRCR_VECTKEY_MASK | NVIC_PriorityGroup;
}

/**
  * @brief  根据 NVIC_InitStruct 中指定的参数初始化 NVIC 外设。
  * @param  NVIC_InitStruct: 指向 NVIC_InitTypeDef 结构体的指针，
  *         该结构体包含指定 NVIC 外设的配置信息。
  * @retval 无
  */
void NVIC_Init(NVIC_InitTypeDef* NVIC_InitStruct)
{
  uint32_t tmppriority = 0x00, tmppre = 0x00, tmpsub = 0x0F;
  
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NVIC_InitStruct->NVIC_IRQChannelCmd));
  assert_param(IS_NVIC_PREEMPTION_PRIORITY(NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority));  
  assert_param(IS_NVIC_SUB_PRIORITY(NVIC_InitStruct->NVIC_IRQChannelSubPriority));
    
  if (NVIC_InitStruct->NVIC_IRQChannelCmd != DISABLE)
  {
    /* 计算对应的中断优先级 ------------------------------------------------*/    
    tmppriority = (0x700 - ((SCB->AIRCR) & (uint32_t)0x700))>> 0x08;
    tmppre = (0x4 - tmppriority);
    tmpsub = tmpsub >> tmppriority;

    tmppriority = (uint32_t)NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority << tmppre;
    tmppriority |=  NVIC_InitStruct->NVIC_IRQChannelSubPriority & tmpsub;
    tmppriority = tmppriority << 0x04;
        
    NVIC->IP[NVIC_InitStruct->NVIC_IRQChannel] = tmppriority;
    
    /* 使能所选中断通道 --------------------------------------*/
    NVIC->ISER[NVIC_InitStruct->NVIC_IRQChannel >> 0x05] =
      (uint32_t)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (uint8_t)0x1F);
  }
  else
  {
    /* 关闭所选中断通道 -------------------------------------*/
    NVIC->ICER[NVIC_InitStruct->NVIC_IRQChannel >> 0x05] =
      (uint32_t)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (uint8_t)0x1F);
  }
}

/**
  * @brief  设置向量表的位置与偏移量。
  * @param  NVIC_VectTab: 指定向量表位于 RAM 还是 FLASH 中。
  *   该参数可取下列值之一：
  *     @arg NVIC_VectTab_RAM
  *     @arg NVIC_VectTab_FLASH
  * @param  Offset: 向量表基址偏移量。该值必须是 0x200 的整数倍。
  * @retval 无
  */
void NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset)
{ 
  /* 检查参数 */
  assert_param(IS_NVIC_VECTTAB(NVIC_VectTab));
  assert_param(IS_NVIC_OFFSET(Offset));  
   
  SCB->VTOR = NVIC_VectTab | (Offset & (uint32_t)0x1FFFFF80);
}

/**
  * @brief  选择系统进入低功耗模式的条件。
  * @param  LowPowerMode: 指定系统进入低功耗模式的新方式。
  *   该参数可取下列值之一：
  *     @arg NVIC_LP_SEVONPEND
  *     @arg NVIC_LP_SLEEPDEEP
  *     @arg NVIC_LP_SLEEPONEXIT
  * @param  NewState: 低功耗条件的新状态。该参数可取 ENABLE 或 DISABLE。
  * @retval 无
  */
void NVIC_SystemLPConfig(uint8_t LowPowerMode, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_NVIC_LP(LowPowerMode));
  assert_param(IS_FUNCTIONAL_STATE(NewState));  
  
  if (NewState != DISABLE)
  {
    SCB->SCR |= LowPowerMode;
  }
  else
  {
    SCB->SCR &= (uint32_t)(~(uint32_t)LowPowerMode);
  }
}

/**
  * @brief  配置 SysTick 的时钟源。
  * @param  SysTick_CLKSource: 指定 SysTick 的时钟源。
  *   该参数可取下列值之一：
  *     @arg SysTick_CLKSource_HCLK_Div8: 选择 AHB 时钟 8 分频作为 SysTick 时钟源。
  *     @arg SysTick_CLKSource_HCLK: 选择 AHB 时钟作为 SysTick 时钟源。
  * @retval 无
  */
void SysTick_CLKSourceConfig(uint32_t SysTick_CLKSource)
{
  /* 检查参数 */
  assert_param(IS_SYSTICK_CLK_SOURCE(SysTick_CLKSource));
  if (SysTick_CLKSource == SysTick_CLKSource_HCLK)
  {
    SysTick->CTRL |= SysTick_CLKSource_HCLK;
  }
  else
  {
    SysTick->CTRL &= SysTick_CLKSource_HCLK_Div8;
  }
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
