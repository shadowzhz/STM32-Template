/**
  ******************************************************************************
  * @file    stm32f10x_tim.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件提供所有 TIM 固件函数。
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @defgroup TIM TIM 外设驱动模块
  * @brief TIM 驱动模块
  * @{
  */

/** @defgroup TIM_Private_TypesDefinitions  TIM 私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup TIM_Private_Defines  TIM 私有宏定义
  * @{
  */

/* ---------------------- TIM 寄存器位掩码 ------------------------ */
#define SMCR_ETR_Mask               ((uint16_t)0x00FF) 
#define CCMR_Offset                 ((uint16_t)0x0018)
#define CCER_CCE_Set                ((uint16_t)0x0001)  
#define	CCER_CCNE_Set               ((uint16_t)0x0004) 

/**
  * @}
  */

/** @defgroup TIM_Private_Macros  TIM 私有宏
  * @{
  */

/**
  * @}
  */

/** @defgroup TIM_Private_Variables  TIM 私有变量
  * @{
  */

/**
  * @}
  */

/** @defgroup TIM_Private_FunctionPrototypes  TIM 私有函数原型
  * @{
  */

static void TI1_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter);
static void TI2_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter);
static void TI3_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter);
static void TI4_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter);
/**
  * @}
  */

/** @defgroup TIM_Private_Macros  TIM 私有宏
  * @{
  */

/**
  * @}
  */

/** @defgroup TIM_Private_Variables  TIM 私有变量
  * @{
  */

/**
  * @}
  */

/** @defgroup TIM_Private_FunctionPrototypes  TIM 私有函数原型
  * @{
  */

/**
  * @}
  */

/** @defgroup TIM_Private_Functions  TIM 私有函数
  * @{
  */

/**
  * @brief  将 TIMx 外设寄存器反初始化为其默认复位值。
  * @param  TIMx: x 可取 1 到 17，用于选择 TIM 外设。
  * @retval None
  */
void TIM_DeInit(TIM_TypeDef* TIMx)
{
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx)); 
 
  if (TIMx == TIM1)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM1, DISABLE);  
  }     
  else if (TIMx == TIM2)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, DISABLE);
  }
  else if (TIMx == TIM3)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM3, DISABLE);
  }
  else if (TIMx == TIM4)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM4, DISABLE);
  } 
  else if (TIMx == TIM5)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM5, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM5, DISABLE);
  } 
  else if (TIMx == TIM6)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM6, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM6, DISABLE);
  } 
  else if (TIMx == TIM7)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM7, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM7, DISABLE);
  } 
  else if (TIMx == TIM8)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM8, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM8, DISABLE);
  }
  else if (TIMx == TIM9)
  {      
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM9, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM9, DISABLE);  
   }  
  else if (TIMx == TIM10)
  {      
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM10, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM10, DISABLE);  
  }  
  else if (TIMx == TIM11) 
  {     
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM11, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM11, DISABLE);  
  }  
  else if (TIMx == TIM12)
  {      
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM12, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM12, DISABLE);  
  }  
  else if (TIMx == TIM13) 
  {       
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM13, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM13, DISABLE);  
  }
  else if (TIMx == TIM14) 
  {       
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM14, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM14, DISABLE);  
  }        
  else if (TIMx == TIM15)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM15, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM15, DISABLE);
  } 
  else if (TIMx == TIM16)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM16, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM16, DISABLE);
  } 
  else
  {
    if (TIMx == TIM17)
    {
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM17, ENABLE);
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM17, DISABLE);
    }  
  }
}

/**
  * @brief  根据 TIM_TimeBaseInitStruct 中指定的参数初始化 TIMx
  *         时基单元外设。
  * @param  TIMx: x 可取 1 到 17，用于选择 TIM 外设。
  * @param  TIM_TimeBaseInitStruct: 指向 TIM_TimeBaseInitTypeDef
  *         结构的指针，该结构包含指定 TIM 外设的配置信息。
  * @retval None
  */
void TIM_TimeBaseInit(TIM_TypeDef* TIMx, TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct)
{
  uint16_t tmpcr1 = 0;

  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx)); 
  assert_param(IS_TIM_COUNTER_MODE(TIM_TimeBaseInitStruct->TIM_CounterMode));
  assert_param(IS_TIM_CKD_DIV(TIM_TimeBaseInitStruct->TIM_ClockDivision));

  tmpcr1 = TIMx->CR1;  

  if((TIMx == TIM1) || (TIMx == TIM8)|| (TIMx == TIM2) || (TIMx == TIM3)||
     (TIMx == TIM4) || (TIMx == TIM5)) 
  {
    /* 选择计数模式 */
    tmpcr1 &= (uint16_t)(~((uint16_t)(TIM_CR1_DIR | TIM_CR1_CMS)));
    tmpcr1 |= (uint32_t)TIM_TimeBaseInitStruct->TIM_CounterMode;
  }
 
  if((TIMx != TIM6) && (TIMx != TIM7))
  {
    /* 设置时钟分频 */
    tmpcr1 &= (uint16_t)(~((uint16_t)TIM_CR1_CKD));
    tmpcr1 |= (uint32_t)TIM_TimeBaseInitStruct->TIM_ClockDivision;
  }

  TIMx->CR1 = tmpcr1;

  /* 设置自动重装载值 */
  TIMx->ARR = TIM_TimeBaseInitStruct->TIM_Period ;
 
  /* 设置预分频器值 */
  TIMx->PSC = TIM_TimeBaseInitStruct->TIM_Prescaler;
    
  if ((TIMx == TIM1) || (TIMx == TIM8)|| (TIMx == TIM15)|| (TIMx == TIM16) || (TIMx == TIM17))  
  {
    /* 设置重复计数器值 */
    TIMx->RCR = TIM_TimeBaseInitStruct->TIM_RepetitionCounter;
  }

  /* 产生一个更新事件，立即重新装载预分频器和重复计数器的值 */
  TIMx->EGR = TIM_PSCReloadMode_Immediate;           
}

/**
  * @brief  根据 TIM_OCInitStruct 中指定的参数初始化 TIMx 通道 1。
  * @param  TIMx: x 可取 1 到 17（6 和 7 除外），用于选择 TIM 外设。
  * @param  TIM_OCInitStruct: 指向 TIM_OCInitTypeDef 结构的指针，
  *         该结构包含指定 TIM 外设的配置信息。
  * @retval None
  */
void TIM_OC1Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  uint16_t tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
   
  /* 检查参数 */
  assert_param(IS_TIM_LIST8_PERIPH(TIMx));
  assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
  assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));   
 /* 关闭通道 1：清零 CC1E 位 */
  TIMx->CCER &= (uint16_t)(~(uint16_t)TIM_CCER_CC1E);
  /* 获取 TIMx CCER 寄存器的值 */
  tmpccer = TIMx->CCER;
  /* 获取 TIMx CR2 寄存器的值 */
  tmpcr2 =  TIMx->CR2;
  
  /* 获取 TIMx CCMR1 寄存器的值 */
  tmpccmrx = TIMx->CCMR1;
    
  /* 清零输出比较模式位 */
  tmpccmrx &= (uint16_t)(~((uint16_t)TIM_CCMR1_OC1M));
  tmpccmrx &= (uint16_t)(~((uint16_t)TIM_CCMR1_CC1S));

  /* 选择输出比较模式 */
  tmpccmrx |= TIM_OCInitStruct->TIM_OCMode;
  
  /* 清零输出极性电平 */
  tmpccer &= (uint16_t)(~((uint16_t)TIM_CCER_CC1P));
  /* 设置输出比较极性 */
  tmpccer |= TIM_OCInitStruct->TIM_OCPolarity;
  
  /* 设置输出状态 */
  tmpccer |= TIM_OCInitStruct->TIM_OutputState;
    
  if((TIMx == TIM1) || (TIMx == TIM8)|| (TIMx == TIM15)||
     (TIMx == TIM16)|| (TIMx == TIM17))
  {
    assert_param(IS_TIM_OUTPUTN_STATE(TIM_OCInitStruct->TIM_OutputNState));
    assert_param(IS_TIM_OCN_POLARITY(TIM_OCInitStruct->TIM_OCNPolarity));
    assert_param(IS_TIM_OCNIDLE_STATE(TIM_OCInitStruct->TIM_OCNIdleState));
    assert_param(IS_TIM_OCIDLE_STATE(TIM_OCInitStruct->TIM_OCIdleState));
    
    /* 清零输出 N 极性电平 */
    tmpccer &= (uint16_t)(~((uint16_t)TIM_CCER_CC1NP));
    /* 设置输出 N 极性 */
    tmpccer |= TIM_OCInitStruct->TIM_OCNPolarity;
    
    /* 清零输出 N 状态 */
    tmpccer &= (uint16_t)(~((uint16_t)TIM_CCER_CC1NE));    
    /* 设置输出 N 状态 */
    tmpccer |= TIM_OCInitStruct->TIM_OutputNState;
    
    /* 清零输出比较和输出比较 N 的空闲状态 */
    tmpcr2 &= (uint16_t)(~((uint16_t)TIM_CR2_OIS1));
    tmpcr2 &= (uint16_t)(~((uint16_t)TIM_CR2_OIS1N));
    
    /* 设置输出空闲状态 */
    tmpcr2 |= TIM_OCInitStruct->TIM_OCIdleState;
    /* 设置输出 N 空闲状态 */
    tmpcr2 |= TIM_OCInitStruct->TIM_OCNIdleState;
  }
  /* 写入 TIMx CR2 寄存器 */
  TIMx->CR2 = tmpcr2;
  
  /* 写入 TIMx CCMR1 寄存器 */
  TIMx->CCMR1 = tmpccmrx;

  /* 设置捕获比较寄存器的值 */
  TIMx->CCR1 = TIM_OCInitStruct->TIM_Pulse; 
 
  /* 写入 TIMx CCER 寄存器 */
  TIMx->CCER = tmpccer;
}

/**
  * @brief  根据 TIM_OCInitStruct 中指定的参数初始化 TIMx 通道 2。
  * @param  TIMx: x 可取 1、2、3、4、5、8、9、12 或 15，
  *         用于选择 TIM 外设。
  * @param  TIM_OCInitStruct: 指向 TIM_OCInitTypeDef 结构的指针，
  *         该结构包含指定 TIM 外设的配置信息。
  * @retval None
  */
void TIM_OC2Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  uint16_t tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
   
  /* 检查参数 */
  assert_param(IS_TIM_LIST6_PERIPH(TIMx)); 
  assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
  assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));   
   /* 关闭通道 2：清零 CC2E 位 */
  TIMx->CCER &= (uint16_t)(~((uint16_t)TIM_CCER_CC2E));
  
  /* 获取 TIMx CCER 寄存器的值 */  
  tmpccer = TIMx->CCER;
  /* 获取 TIMx CR2 寄存器的值 */
  tmpcr2 =  TIMx->CR2;
  
  /* 获取 TIMx CCMR1 寄存器的值 */
  tmpccmrx = TIMx->CCMR1;
    
  /* 清零输出比较模式和捕获/比较选择位 */
  tmpccmrx &= (uint16_t)(~((uint16_t)TIM_CCMR1_OC2M));
  tmpccmrx &= (uint16_t)(~((uint16_t)TIM_CCMR1_CC2S));
  
  /* 选择输出比较模式 */
  tmpccmrx |= (uint16_t)(TIM_OCInitStruct->TIM_OCMode << 8);
  
  /* 清零输出极性电平 */
  tmpccer &= (uint16_t)(~((uint16_t)TIM_CCER_CC2P));
  /* 设置输出比较极性 */
  tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OCPolarity << 4);
  
  /* 设置输出状态 */
  tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OutputState << 4);
    
  if((TIMx == TIM1) || (TIMx == TIM8))
  {
    assert_param(IS_TIM_OUTPUTN_STATE(TIM_OCInitStruct->TIM_OutputNState));
    assert_param(IS_TIM_OCN_POLARITY(TIM_OCInitStruct->TIM_OCNPolarity));
    assert_param(IS_TIM_OCNIDLE_STATE(TIM_OCInitStruct->TIM_OCNIdleState));
    assert_param(IS_TIM_OCIDLE_STATE(TIM_OCInitStruct->TIM_OCIdleState));
    
    /* 清零输出 N 极性电平 */
    tmpccer &= (uint16_t)(~((uint16_t)TIM_CCER_CC2NP));
    /* 设置输出 N 极性 */
    tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OCNPolarity << 4);
    
    /* 清零输出 N 状态 */
    tmpccer &= (uint16_t)(~((uint16_t)TIM_CCER_CC2NE));    
    /* 设置输出 N 状态 */
    tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OutputNState << 4);
    
    /* 清零输出比较和输出比较 N 的空闲状态 */
    tmpcr2 &= (uint16_t)(~((uint16_t)TIM_CR2_OIS2));
    tmpcr2 &= (uint16_t)(~((uint16_t)TIM_CR2_OIS2N));
    
    /* 设置输出空闲状态 */
    tmpcr2 |= (uint16_t)(TIM_OCInitStruct->TIM_OCIdleState << 2);
    /* 设置输出 N 空闲状态 */
    tmpcr2 |= (uint16_t)(TIM_OCInitStruct->TIM_OCNIdleState << 2);
  }
  /* 写入 TIMx CR2 寄存器 */
  TIMx->CR2 = tmpcr2;
  
  /* 写入 TIMx CCMR1 寄存器 */
  TIMx->CCMR1 = tmpccmrx;

  /* 设置捕获比较寄存器的值 */
  TIMx->CCR2 = TIM_OCInitStruct->TIM_Pulse;
  
  /* 写入 TIMx CCER 寄存器 */
  TIMx->CCER = tmpccer;
}

/**
  * @brief  根据 TIM_OCInitStruct 中指定的参数初始化 TIMx 通道 3。
  * @param  TIMx: x 可取 1、2、3、4、5 或 8，用于选择 TIM 外设。
  * @param  TIM_OCInitStruct: 指向 TIM_OCInitTypeDef 结构的指针，
  *         该结构包含指定 TIM 外设的配置信息。
  * @retval None
  */
void TIM_OC3Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  uint16_t tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
   
  /* 检查参数 */
  assert_param(IS_TIM_LIST3_PERIPH(TIMx)); 
  assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
  assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));   
  /* 关闭通道 2：清零 CC2E 位 */
  TIMx->CCER &= (uint16_t)(~((uint16_t)TIM_CCER_CC3E));
  
  /* 获取 TIMx CCER 寄存器的值 */
  tmpccer = TIMx->CCER;
  /* 获取 TIMx CR2 寄存器的值 */
  tmpcr2 =  TIMx->CR2;
  
  /* 获取 TIMx CCMR2 寄存器的值 */
  tmpccmrx = TIMx->CCMR2;
    
  /* 清零输出比较模式和捕获/比较选择位 */
  tmpccmrx &= (uint16_t)(~((uint16_t)TIM_CCMR2_OC3M));
  tmpccmrx &= (uint16_t)(~((uint16_t)TIM_CCMR2_CC3S));  
  /* 选择输出比较模式 */
  tmpccmrx |= TIM_OCInitStruct->TIM_OCMode;
  
  /* 清零输出极性电平 */
  tmpccer &= (uint16_t)(~((uint16_t)TIM_CCER_CC3P));
  /* 设置输出比较极性 */
  tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OCPolarity << 8);
  
  /* 设置输出状态 */
  tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OutputState << 8);
    
  if((TIMx == TIM1) || (TIMx == TIM8))
  {
    assert_param(IS_TIM_OUTPUTN_STATE(TIM_OCInitStruct->TIM_OutputNState));
    assert_param(IS_TIM_OCN_POLARITY(TIM_OCInitStruct->TIM_OCNPolarity));
    assert_param(IS_TIM_OCNIDLE_STATE(TIM_OCInitStruct->TIM_OCNIdleState));
    assert_param(IS_TIM_OCIDLE_STATE(TIM_OCInitStruct->TIM_OCIdleState));
    
    /* 清零输出 N 极性电平 */
    tmpccer &= (uint16_t)(~((uint16_t)TIM_CCER_CC3NP));
    /* 设置输出 N 极性 */
    tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OCNPolarity << 8);
    /* 清零输出 N 状态 */
    tmpccer &= (uint16_t)(~((uint16_t)TIM_CCER_CC3NE));
    
    /* 设置输出 N 状态 */
    tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OutputNState << 8);
    /* 清零输出比较和输出比较 N 的空闲状态 */
    tmpcr2 &= (uint16_t)(~((uint16_t)TIM_CR2_OIS3));
    tmpcr2 &= (uint16_t)(~((uint16_t)TIM_CR2_OIS3N));
    /* 设置输出空闲状态 */
    tmpcr2 |= (uint16_t)(TIM_OCInitStruct->TIM_OCIdleState << 4);
    /* 设置输出 N 空闲状态 */
    tmpcr2 |= (uint16_t)(TIM_OCInitStruct->TIM_OCNIdleState << 4);
  }
  /* 写入 TIMx CR2 寄存器 */
  TIMx->CR2 = tmpcr2;
  
  /* 写入 TIMx CCMR2 寄存器 */
  TIMx->CCMR2 = tmpccmrx;

  /* 设置捕获比较寄存器的值 */
  TIMx->CCR3 = TIM_OCInitStruct->TIM_Pulse;
  
  /* 写入 TIMx CCER 寄存器 */
  TIMx->CCER = tmpccer;
}

/**
  * @brief  根据 TIM_OCInitStruct 中指定的参数初始化 TIMx 通道 4。
  * @param  TIMx: x 可取 1、2、3、4、5 或 8，用于选择 TIM 外设。
  * @param  TIM_OCInitStruct: 指向 TIM_OCInitTypeDef 结构的指针，
  *         该结构包含指定 TIM 外设的配置信息。
  * @retval None
  */
void TIM_OC4Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  uint16_t tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
   
  /* 检查参数 */
  assert_param(IS_TIM_LIST3_PERIPH(TIMx)); 
  assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
  assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));   
  /* 关闭通道 4：清零 CC4E 位 */
  TIMx->CCER &= (uint16_t)(~((uint16_t)TIM_CCER_CC4E));
  
  /* 获取 TIMx CCER 寄存器的值 */
  tmpccer = TIMx->CCER;
  /* 获取 TIMx CR2 寄存器的值 */
  tmpcr2 =  TIMx->CR2;
  
  /* 获取 TIMx CCMR2 寄存器的值 */
  tmpccmrx = TIMx->CCMR2;
    
  /* 清零输出比较模式和捕获/比较选择位 */
  tmpccmrx &= (uint16_t)(~((uint16_t)TIM_CCMR2_OC4M));
  tmpccmrx &= (uint16_t)(~((uint16_t)TIM_CCMR2_CC4S));
  
  /* 选择输出比较模式 */
  tmpccmrx |= (uint16_t)(TIM_OCInitStruct->TIM_OCMode << 8);
  
  /* 清零输出极性电平 */
  tmpccer &= (uint16_t)(~((uint16_t)TIM_CCER_CC4P));
  /* 设置输出比较极性 */
  tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OCPolarity << 12);
  
  /* 设置输出状态 */
  tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OutputState << 12);
    
  if((TIMx == TIM1) || (TIMx == TIM8))
  {
    assert_param(IS_TIM_OCIDLE_STATE(TIM_OCInitStruct->TIM_OCIdleState));
    /* 清零输出比较空闲状态 */
    tmpcr2 &= (uint16_t)(~((uint16_t)TIM_CR2_OIS4));
    /* 设置输出空闲状态 */
    tmpcr2 |= (uint16_t)(TIM_OCInitStruct->TIM_OCIdleState << 6);
  }
  /* 写入 TIMx CR2 寄存器 */
  TIMx->CR2 = tmpcr2;
  
  /* 写入 TIMx CCMR2 寄存器 */  
  TIMx->CCMR2 = tmpccmrx;

  /* 设置捕获比较寄存器的值 */
  TIMx->CCR4 = TIM_OCInitStruct->TIM_Pulse;
  
  /* 写入 TIMx CCER 寄存器 */
  TIMx->CCER = tmpccer;
}

/**
  * @brief  根据 TIM_ICInitStruct 中指定的参数初始化 TIM 外设。
  * @param  TIMx: x 可取 1 到 17（6 和 7 除外），用于选择 TIM 外设。
  * @param  TIM_ICInitStruct: 指向 TIM_ICInitTypeDef 结构的指针，
  *         该结构包含指定 TIM 外设的配置信息。
  * @retval None
  */
void TIM_ICInit(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct)
{
  /* 检查参数 */
  assert_param(IS_TIM_CHANNEL(TIM_ICInitStruct->TIM_Channel));  
  assert_param(IS_TIM_IC_SELECTION(TIM_ICInitStruct->TIM_ICSelection));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICInitStruct->TIM_ICPrescaler));
  assert_param(IS_TIM_IC_FILTER(TIM_ICInitStruct->TIM_ICFilter));
  
  if((TIMx == TIM1) || (TIMx == TIM8) || (TIMx == TIM2) || (TIMx == TIM3) ||
     (TIMx == TIM4) ||(TIMx == TIM5))
  {
    assert_param(IS_TIM_IC_POLARITY(TIM_ICInitStruct->TIM_ICPolarity));
  }
  else
  {
    assert_param(IS_TIM_IC_POLARITY_LITE(TIM_ICInitStruct->TIM_ICPolarity));
  }
  if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_1)
  {
    assert_param(IS_TIM_LIST8_PERIPH(TIMx));
    /* TI1 配置 */
    TI1_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity,
               TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);
    /* 设置输入捕获预分频值 */
    TIM_SetIC1Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
  else if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_2)
  {
    assert_param(IS_TIM_LIST6_PERIPH(TIMx));
    /* TI2 配置 */
    TI2_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity,
               TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);
    /* 设置输入捕获预分频值 */
    TIM_SetIC2Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
  else if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_3)
  {
    assert_param(IS_TIM_LIST3_PERIPH(TIMx));
    /* TI3 配置 */
    TI3_Config(TIMx,  TIM_ICInitStruct->TIM_ICPolarity,
               TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);
    /* 设置输入捕获预分频值 */
    TIM_SetIC3Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
  else
  {
    assert_param(IS_TIM_LIST3_PERIPH(TIMx));
    /* TI4 配置 */
    TI4_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity,
               TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);
    /* 设置输入捕获预分频值 */
    TIM_SetIC4Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
}

/**
  * @brief  根据 TIM_ICInitStruct 中指定的参数配置 TIM 外设，
  *         以测量外部 PWM 信号。
  * @param  TIMx: x 可取 1、2、3、4、5、8、9、12 或 15，用于选择 TIM 外设。
  * @param  TIM_ICInitStruct: 指向 TIM_ICInitTypeDef 结构的指针，
  *         该结构包含指定 TIM 外设的配置信息。
  * @retval None
  */
void TIM_PWMIConfig(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct)
{
  uint16_t icoppositepolarity = TIM_ICPolarity_Rising;
  uint16_t icoppositeselection = TIM_ICSelection_DirectTI;
  /* 检查参数 */
  assert_param(IS_TIM_LIST6_PERIPH(TIMx));
  /* 选择相反的输入极性 */
  if (TIM_ICInitStruct->TIM_ICPolarity == TIM_ICPolarity_Rising)
  {
    icoppositepolarity = TIM_ICPolarity_Falling;
  }
  else
  {
    icoppositepolarity = TIM_ICPolarity_Rising;
  }
  /* 选择相反的输入 */
  if (TIM_ICInitStruct->TIM_ICSelection == TIM_ICSelection_DirectTI)
  {
    icoppositeselection = TIM_ICSelection_IndirectTI;
  }
  else
  {
    icoppositeselection = TIM_ICSelection_DirectTI;
  }
  if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_1)
  {
    /* TI1 配置 */
    TI1_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity, TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);
    /* 设置输入捕获预分频值 */
    TIM_SetIC1Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
    /* TI2 配置 */
    TI2_Config(TIMx, icoppositepolarity, icoppositeselection, TIM_ICInitStruct->TIM_ICFilter);
    /* 设置输入捕获预分频值 */
    TIM_SetIC2Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
  else
  { 
    /* TI2 配置 */
    TI2_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity, TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);
    /* 设置输入捕获预分频值 */
    TIM_SetIC2Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
    /* TI1 配置 */
    TI1_Config(TIMx, icoppositepolarity, icoppositeselection, TIM_ICInitStruct->TIM_ICFilter);
    /* 设置输入捕获预分频值 */
    TIM_SetIC1Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
}

/**
  * @brief  配置刹车功能、死区时间、锁定电平、OSSI、
  *         OSSR 状态和 AOE（自动输出使能）。
  * @param  TIMx: x 可取 1 或 8，用于选择 TIM 外设
  * @param  TIM_BDTRInitStruct: 指向 TIM_BDTRInitTypeDef 结构的指针，
  *         该结构包含 TIM 外设的 BDTR 寄存器配置信息。
  * @retval None
  */
void TIM_BDTRConfig(TIM_TypeDef* TIMx, TIM_BDTRInitTypeDef *TIM_BDTRInitStruct)
{
  /* 检查参数 */
  assert_param(IS_TIM_LIST2_PERIPH(TIMx));
  assert_param(IS_TIM_OSSR_STATE(TIM_BDTRInitStruct->TIM_OSSRState));
  assert_param(IS_TIM_OSSI_STATE(TIM_BDTRInitStruct->TIM_OSSIState));
  assert_param(IS_TIM_LOCK_LEVEL(TIM_BDTRInitStruct->TIM_LOCKLevel));
  assert_param(IS_TIM_BREAK_STATE(TIM_BDTRInitStruct->TIM_Break));
  assert_param(IS_TIM_BREAK_POLARITY(TIM_BDTRInitStruct->TIM_BreakPolarity));
  assert_param(IS_TIM_AUTOMATIC_OUTPUT_STATE(TIM_BDTRInitStruct->TIM_AutomaticOutput));
  /* 设置锁定电平、刹车使能位及其极性、OSSR 状态、OSSI 状态、
     死区时间和自动输出使能位 */
  TIMx->BDTR = (uint32_t)TIM_BDTRInitStruct->TIM_OSSRState | TIM_BDTRInitStruct->TIM_OSSIState |
             TIM_BDTRInitStruct->TIM_LOCKLevel | TIM_BDTRInitStruct->TIM_DeadTime |
             TIM_BDTRInitStruct->TIM_Break | TIM_BDTRInitStruct->TIM_BreakPolarity |
             TIM_BDTRInitStruct->TIM_AutomaticOutput;
}

/**
  * @brief  将 TIM_TimeBaseInitStruct 的每个成员填充为默认值。
  * @param  TIM_TimeBaseInitStruct : 指向将被初始化的
  *         TIM_TimeBaseInitTypeDef 结构的指针。
  * @retval None
  */
void TIM_TimeBaseStructInit(TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct)
{
  /* 设置默认配置 */
  TIM_TimeBaseInitStruct->TIM_Period = 0xFFFF;
  TIM_TimeBaseInitStruct->TIM_Prescaler = 0x0000;
  TIM_TimeBaseInitStruct->TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct->TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStruct->TIM_RepetitionCounter = 0x0000;
}

/**
  * @brief  将 TIM_OCInitStruct 的每个成员填充为默认值。
  * @param  TIM_OCInitStruct : 指向将被初始化的 TIM_OCInitTypeDef
  *         结构的指针。
  * @retval None
  */
void TIM_OCStructInit(TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  /* 设置默认配置 */
  TIM_OCInitStruct->TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStruct->TIM_OutputState = TIM_OutputState_Disable;
  TIM_OCInitStruct->TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStruct->TIM_Pulse = 0x0000;
  TIM_OCInitStruct->TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStruct->TIM_OCNPolarity = TIM_OCPolarity_High;
  TIM_OCInitStruct->TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStruct->TIM_OCNIdleState = TIM_OCNIdleState_Reset;
}

/**
  * @brief  将 TIM_ICInitStruct 的每个成员填充为默认值。
  * @param  TIM_ICInitStruct: 指向将被初始化的 TIM_ICInitTypeDef
  *         结构的指针。
  * @retval None
  */
void TIM_ICStructInit(TIM_ICInitTypeDef* TIM_ICInitStruct)
{
  /* 设置默认配置 */
  TIM_ICInitStruct->TIM_Channel = TIM_Channel_1;
  TIM_ICInitStruct->TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStruct->TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStruct->TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStruct->TIM_ICFilter = 0x00;
}

/**
  * @brief  将 TIM_BDTRInitStruct 的每个成员填充为默认值。
  * @param  TIM_BDTRInitStruct: 指向将被初始化的 TIM_BDTRInitTypeDef
  *         结构的指针。
  * @retval None
  */
void TIM_BDTRStructInit(TIM_BDTRInitTypeDef* TIM_BDTRInitStruct)
{
  /* 设置默认配置 */
  TIM_BDTRInitStruct->TIM_OSSRState = TIM_OSSRState_Disable;
  TIM_BDTRInitStruct->TIM_OSSIState = TIM_OSSIState_Disable;
  TIM_BDTRInitStruct->TIM_LOCKLevel = TIM_LOCKLevel_OFF;
  TIM_BDTRInitStruct->TIM_DeadTime = 0x00;
  TIM_BDTRInitStruct->TIM_Break = TIM_Break_Disable;
  TIM_BDTRInitStruct->TIM_BreakPolarity = TIM_BreakPolarity_Low;
  TIM_BDTRInitStruct->TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;
}

/**
  * @brief  使能或关闭指定的 TIM 外设。
  * @param  TIMx: x 可取 1 到 17，用于选择 TIMx 外设。
  * @param  NewState: TIMx 外设的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval None
  */
void TIM_Cmd(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* 使能 TIM 计数器 */
    TIMx->CR1 |= TIM_CR1_CEN;
  }
  else
  {
    /* 关闭 TIM 计数器 */
    TIMx->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));
  }
}

/**
  * @brief  使能或关闭 TIM 外设的主输出。
  * @param  TIMx: x 可取 1、8、15、16 或 17，用于选择 TIMx 外设。
  * @param  NewState: TIM 外设主输出的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval None
  */
void TIM_CtrlPWMOutputs(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_TIM_LIST2_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 使能 TIM 主输出 */
    TIMx->BDTR |= TIM_BDTR_MOE;
  }
  else
  {
    /* 关闭 TIM 主输出 */
    TIMx->BDTR &= (uint16_t)(~((uint16_t)TIM_BDTR_MOE));
  }  
}

/**
  * @brief  使能或关闭指定的 TIM 中断。
  * @param  TIMx: x 可取 1 到 17，用于选择 TIMx 外设。
  * @param  TIM_IT: 指定要使能或关闭的 TIM 中断源。
  *   该参数可取以下值之一或其任意组合：
  *     @arg TIM_IT_Update: TIM 更新中断源
  *     @arg TIM_IT_CC1: TIM 捕获比较 1 中断源
  *     @arg TIM_IT_CC2: TIM 捕获比较 2 中断源
  *     @arg TIM_IT_CC3: TIM 捕获比较 3 中断源
  *     @arg TIM_IT_CC4: TIM 捕获比较 4 中断源
  *     @arg TIM_IT_COM: TIM 换相中断源
  *     @arg TIM_IT_Trigger: TIM 触发中断源
  *     @arg TIM_IT_Break: TIM 刹车中断源
  * @note
  *   - TIM6 和 TIM7 只能产生更新中断。
  *   - TIM9、TIM12 和 TIM15 只能具有 TIM_IT_Update、TIM_IT_CC1、
  *      TIM_IT_CC2 或 TIM_IT_Trigger。
  *   - TIM10、TIM11、TIM13、TIM14、TIM16 和 TIM17 只能具有 TIM_IT_Update 或 TIM_IT_CC1。
  *   - TIM_IT_Break 仅用于 TIM1、TIM8 和 TIM15。
  *   - TIM_IT_COM 仅用于 TIM1、TIM8、TIM15、TIM16 和 TIM17。
  * @param  NewState: TIM 中断的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval None
  */
void TIM_ITConfig(TIM_TypeDef* TIMx, uint16_t TIM_IT, FunctionalState NewState)
{  
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_IT(TIM_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* 使能中断源 */
    TIMx->DIER |= TIM_IT;
  }
  else
  {
    /* 关闭中断源 */
    TIMx->DIER &= (uint16_t)~TIM_IT;
  }
}

/**
  * @brief  配置要由软件产生的 TIMx 事件。
  * @param  TIMx: x 可取 1 到 17，用于选择 TIM 外设。
  * @param  TIM_EventSource: 指定事件源。
  *   该参数可取以下值之一或多个：
  *     @arg TIM_EventSource_Update: 定时器更新事件源
  *     @arg TIM_EventSource_CC1: 定时器捕获比较 1 事件源
  *     @arg TIM_EventSource_CC2: 定时器捕获比较 2 事件源
  *     @arg TIM_EventSource_CC3: 定时器捕获比较 3 事件源
  *     @arg TIM_EventSource_CC4: 定时器捕获比较 4 事件源
  *     @arg TIM_EventSource_COM: 定时器 COM 事件源
  *     @arg TIM_EventSource_Trigger: 定时器触发事件源
  *     @arg TIM_EventSource_Break: 定时器刹车事件源
  * @note
  *   - TIM6 和 TIM7 只能产生更新事件。
  *   - TIM_EventSource_COM 和 TIM_EventSource_Break 仅用于 TIM1 和 TIM8。
  * @retval None
  */
void TIM_GenerateEvent(TIM_TypeDef* TIMx, uint16_t TIM_EventSource)
{ 
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_EVENT_SOURCE(TIM_EventSource));
  
  /* 设置事件源 */
  TIMx->EGR = TIM_EventSource;
}

/**
  * @brief  配置 TIMx 的 DMA 接口。
  * @param  TIMx: x 可取 1、2、3、4、5、8、15、16 或 17，
  *   用于选择 TIM 外设。
  * @param  TIM_DMABase: DMA 基地址。
  *   该参数可取以下值之一：
  *     @arg TIM_DMABase_CR, TIM_DMABase_CR2, TIM_DMABase_SMCR,
  *          TIM_DMABase_DIER, TIM1_DMABase_SR, TIM_DMABase_EGR,
  *          TIM_DMABase_CCMR1, TIM_DMABase_CCMR2, TIM_DMABase_CCER,
  *          TIM_DMABase_CNT, TIM_DMABase_PSC, TIM_DMABase_ARR,
  *          TIM_DMABase_RCR, TIM_DMABase_CCR1, TIM_DMABase_CCR2,
  *          TIM_DMABase_CCR3, TIM_DMABase_CCR4, TIM_DMABase_BDTR,
  *          TIM_DMABase_DCR.
  * @param  TIM_DMABurstLength: DMA 突发长度。
  *   该参数可取以下值之一：
  *   TIM_DMABurstLength_1Transfer 到 TIM_DMABurstLength_18Transfers。
  * @retval None
  */
void TIM_DMAConfig(TIM_TypeDef* TIMx, uint16_t TIM_DMABase, uint16_t TIM_DMABurstLength)
{
  /* 检查参数 */
  assert_param(IS_TIM_LIST4_PERIPH(TIMx));
  assert_param(IS_TIM_DMA_BASE(TIM_DMABase));
  assert_param(IS_TIM_DMA_LENGTH(TIM_DMABurstLength));
  /* 设置 DMA 基地址和 DMA 突发长度 */
  TIMx->DCR = TIM_DMABase | TIM_DMABurstLength;
}

/**
  * @brief  使能或关闭 TIMx 的 DMA 请求。
  * @param  TIMx: x 可取 1、2、3、4、5、6、7、8、15、16 或 17，
  *   用于选择 TIM 外设。
  * @param  TIM_DMASource: 指定 DMA 请求源。
  *   该参数可取以下值之一或其任意组合：
  *     @arg TIM_DMA_Update: TIM 更新中断源
  *     @arg TIM_DMA_CC1: TIM 捕获比较 1 DMA 源
  *     @arg TIM_DMA_CC2: TIM 捕获比较 2 DMA 源
  *     @arg TIM_DMA_CC3: TIM 捕获比较 3 DMA 源
  *     @arg TIM_DMA_CC4: TIM 捕获比较 4 DMA 源
  *     @arg TIM_DMA_COM: TIM 换相 DMA 源
  *     @arg TIM_DMA_Trigger: TIM 触发 DMA 源
  * @param  NewState: DMA 请求源的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval None
  */
void TIM_DMACmd(TIM_TypeDef* TIMx, uint16_t TIM_DMASource, FunctionalState NewState)
{ 
  /* 检查参数 */
  assert_param(IS_TIM_LIST9_PERIPH(TIMx));
  assert_param(IS_TIM_DMA_SOURCE(TIM_DMASource));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* 使能 DMA 源 */
    TIMx->DIER |= TIM_DMASource; 
  }
  else
  {
    /* 关闭 DMA 源 */
    TIMx->DIER &= (uint16_t)~TIM_DMASource;
  }
}

/**
  * @brief  配置 TIMx 内部时钟
  * @param  TIMx: x 可取 1、2、3、4、5、8、9、12 或 15，
  *         用于选择 TIM 外设。
  * @retval None
  */
void TIM_InternalClockConfig(TIM_TypeDef* TIMx)
{
  /* 检查参数 */
  assert_param(IS_TIM_LIST6_PERIPH(TIMx));
  /* 关闭从模式，使预分频器直接由内部时钟驱动 */
  TIMx->SMCR &=  (uint16_t)(~((uint16_t)TIM_SMCR_SMS));
}

/**
  * @brief  将 TIMx 内部触发配置为外部时钟
  * @param  TIMx: x 可取 1、2、3、4、5、9、12 或 15，用于选择 TIM 外设。
  * @param  TIM_ITRSource: 触发源。
  *   该参数可取以下值之一：
  * @param  TIM_TS_ITR0: 内部触发 0
  * @param  TIM_TS_ITR1: 内部触发 1
  * @param  TIM_TS_ITR2: 内部触发 2
  * @param  TIM_TS_ITR3: 内部触发 3
  * @retval None
  */
void TIM_ITRxExternalClockConfig(TIM_TypeDef* TIMx, uint16_t TIM_InputTriggerSource)
{
  /* 检查参数 */
  assert_param(IS_TIM_LIST6_PERIPH(TIMx));
  assert_param(IS_TIM_INTERNAL_TRIGGER_SELECTION(TIM_InputTriggerSource));
  /* 选择内部触发 */
  TIM_SelectInputTrigger(TIMx, TIM_InputTriggerSource);
  /* 选择外部时钟模式 1 */
  TIMx->SMCR |= TIM_SlaveMode_External1;
}

/**
  * @brief  将 TIMx 触发配置为外部时钟
  * @param  TIMx: x 可取 1、2、3、4、5、9、12 或 15，用于选择 TIM 外设。
  * @param  TIM_TIxExternalCLKSource: 触发源。
  *   该参数可取以下值之一：
  *     @arg TIM_TIxExternalCLK1Source_TI1ED: TI1 边沿检测器
  *     @arg TIM_TIxExternalCLK1Source_TI1: 滤波后的定时器输入 1
  *     @arg TIM_TIxExternalCLK1Source_TI2: 滤波后的定时器输入 2
  * @param  TIM_ICPolarity: 指定 TIx 极性。
  *   该参数可取以下值之一：
  *     @arg TIM_ICPolarity_Rising
  *     @arg TIM_ICPolarity_Falling
  * @param  ICFilter : 指定滤波器值。
  *   该参数必须为 0x0 到 0xF 之间的值。
  * @retval None
  */
void TIM_TIxExternalClockConfig(TIM_TypeDef* TIMx, uint16_t TIM_TIxExternalCLKSource,
                                uint16_t TIM_ICPolarity, uint16_t ICFilter)
{
  /* 检查参数 */
  assert_param(IS_TIM_LIST6_PERIPH(TIMx));
  assert_param(IS_TIM_TIXCLK_SOURCE(TIM_TIxExternalCLKSource));
  assert_param(IS_TIM_IC_POLARITY(TIM_ICPolarity));
  assert_param(IS_TIM_IC_FILTER(ICFilter));
  /* 配置定时器输入时钟源 */
  if (TIM_TIxExternalCLKSource == TIM_TIxExternalCLK1Source_TI2)
  {
    TI2_Config(TIMx, TIM_ICPolarity, TIM_ICSelection_DirectTI, ICFilter);
  }
  else
  {
    TI1_Config(TIMx, TIM_ICPolarity, TIM_ICSelection_DirectTI, ICFilter);
  }
  /* 选择触发源 */
  TIM_SelectInputTrigger(TIMx, TIM_TIxExternalCLKSource);
  /* 选择外部时钟模式 1 */
  TIMx->SMCR |= TIM_SlaveMode_External1;
}

/**
  * @brief  配置外部时钟模式 1
  * @param  TIMx: x 可取 1、2、3、4、5 或 8，用于选择 TIM 外设。
  * @param  TIM_ExtTRGPrescaler: 外部触发预分频器。
  *   该参数可取以下值之一：
  *     @arg TIM_ExtTRGPSC_OFF: ETRP 预分频器关闭。
  *     @arg TIM_ExtTRGPSC_DIV2: ETRP 频率除以 2。
  *     @arg TIM_ExtTRGPSC_DIV4: ETRP 频率除以 4。
  *     @arg TIM_ExtTRGPSC_DIV8: ETRP 频率除以 8。
  * @param  TIM_ExtTRGPolarity: 外部触发极性。
  *   该参数可取以下值之一：
  *     @arg TIM_ExtTRGPolarity_Inverted: 低电平有效或下降沿有效。
  *     @arg TIM_ExtTRGPolarity_NonInverted: 高电平有效或上升沿有效。
  * @param  ExtTRGFilter: 外部触发滤波器。
  *   该参数必须为 0x00 到 0x0F 之间的值
  * @retval None
  */
void TIM_ETRClockMode1Config(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler, uint16_t TIM_ExtTRGPolarity,
                             uint16_t ExtTRGFilter)
{
  uint16_t tmpsmcr = 0;
  /* 检查参数 */
  assert_param(IS_TIM_LIST3_PERIPH(TIMx));
  assert_param(IS_TIM_EXT_PRESCALER(TIM_ExtTRGPrescaler));
  assert_param(IS_TIM_EXT_POLARITY(TIM_ExtTRGPolarity));
  assert_param(IS_TIM_EXT_FILTER(ExtTRGFilter));
  /* 配置 ETR 时钟源 */
  TIM_ETRConfig(TIMx, TIM_ExtTRGPrescaler, TIM_ExtTRGPolarity, ExtTRGFilter);
  
  /* 获取 TIMx SMCR 寄存器的值 */
  tmpsmcr = TIMx->SMCR;
  /* 清零 SMS 位 */
  tmpsmcr &= (uint16_t)(~((uint16_t)TIM_SMCR_SMS));
  /* 选择外部时钟模式 1 */
  tmpsmcr |= TIM_SlaveMode_External1;
  /* 选择触发源：ETRF */
  tmpsmcr &= (uint16_t)(~((uint16_t)TIM_SMCR_TS));
  tmpsmcr |= TIM_TS_ETRF;
  /* 写入 TIMx SMCR 寄存器 */
  TIMx->SMCR = tmpsmcr;
}

/**
  * @brief  配置外部时钟模式 2
  * @param  TIMx: x 可取 1、2、3、4、5 或 8，用于选择 TIM 外设。
  * @param  TIM_ExtTRGPrescaler: 外部触发预分频器。
  *   该参数可取以下值之一：
  *     @arg TIM_ExtTRGPSC_OFF: ETRP 预分频器关闭。
  *     @arg TIM_ExtTRGPSC_DIV2: ETRP 频率除以 2。
  *     @arg TIM_ExtTRGPSC_DIV4: ETRP 频率除以 4。
  *     @arg TIM_ExtTRGPSC_DIV8: ETRP 频率除以 8。
  * @param  TIM_ExtTRGPolarity: 外部触发极性。
  *   该参数可取以下值之一：
  *     @arg TIM_ExtTRGPolarity_Inverted: 低电平有效或下降沿有效。
  *     @arg TIM_ExtTRGPolarity_NonInverted: 高电平有效或上升沿有效。
  * @param  ExtTRGFilter: 外部触发滤波器。
  *   该参数必须为 0x00 到 0x0F 之间的值
  * @retval None
  */
void TIM_ETRClockMode2Config(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler, 
                             uint16_t TIM_ExtTRGPolarity, uint16_t ExtTRGFilter)
{
  /* 检查参数 */
  assert_param(IS_TIM_LIST3_PERIPH(TIMx));
  assert_param(IS_TIM_EXT_PRESCALER(TIM_ExtTRGPrescaler));
  assert_param(IS_TIM_EXT_POLARITY(TIM_ExtTRGPolarity));
  assert_param(IS_TIM_EXT_FILTER(ExtTRGFilter));
  /* 配置 ETR 时钟源 */
  TIM_ETRConfig(TIMx, TIM_ExtTRGPrescaler, TIM_ExtTRGPolarity, ExtTRGFilter);
  /* 使能外部时钟模式 2 */
  TIMx->SMCR |= TIM_SMCR_ECE;
}

/**
  * @brief  配置 TIMx 外部触发（ETR）。
  * @param  TIMx: x 可取 1、2、3、4、5 或 8，用于选择 TIM 外设。
  * @param  TIM_ExtTRGPrescaler: 外部触发预分频器。
  *   该参数可取以下值之一：
  *     @arg TIM_ExtTRGPSC_OFF: ETRP 预分频器关闭。
  *     @arg TIM_ExtTRGPSC_DIV2: ETRP 频率除以 2。
  *     @arg TIM_ExtTRGPSC_DIV4: ETRP 频率除以 4。
  *     @arg TIM_ExtTRGPSC_DIV8: ETRP 频率除以 8。
  * @param  TIM_ExtTRGPolarity: 外部触发极性。
  *   该参数可取以下值之一：
  *     @arg TIM_ExtTRGPolarity_Inverted: 低电平有效或下降沿有效。
  *     @arg TIM_ExtTRGPolarity_NonInverted: 高电平有效或上升沿有效。
  * @param  ExtTRGFilter: 外部触发滤波器。
  *   该参数必须为 0x00 到 0x0F 之间的值
  * @retval None
  */
void TIM_ETRConfig(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler, uint16_t TIM_ExtTRGPolarity,
                   uint16_t ExtTRGFilter)
{
  uint16_t tmpsmcr = 0;
  /* 检查参数 */
  assert_param(IS_TIM_LIST3_PERIPH(TIMx));
  assert_param(IS_TIM_EXT_PRESCALER(TIM_ExtTRGPrescaler));
  assert_param(IS_TIM_EXT_POLARITY(TIM_ExtTRGPolarity));
  assert_param(IS_TIM_EXT_FILTER(ExtTRGFilter));
  tmpsmcr = TIMx->SMCR;
  /* 清零 ETR 位 */
  tmpsmcr &= SMCR_ETR_Mask;
  /* 设置预分频器、滤波器值和极性 */
  tmpsmcr |= (uint16_t)(TIM_ExtTRGPrescaler | (uint16_t)(TIM_ExtTRGPolarity | (uint16_t)(ExtTRGFilter << (uint16_t)8)));
  /* 写入 TIMx SMCR 寄存器 */
  TIMx->SMCR = tmpsmcr;
}

/**
  * @brief  配置 TIMx 预分频器。
  * @param  TIMx: x 可取 1 到 17，用于选择 TIM 外设。
  * @param  Prescaler: 指定预分频器寄存器值
  * @param  TIM_PSCReloadMode: 指定 TIM 预分频器重装载模式
  *   该参数可取以下值之一：
  *     @arg TIM_PSCReloadMode_Update: 预分频器在更新事件时装载。
  *     @arg TIM_PSCReloadMode_Immediate: 预分频器立即装载。
  * @retval None
  */
void TIM_PrescalerConfig(TIM_TypeDef* TIMx, uint16_t Prescaler, uint16_t TIM_PSCReloadMode)
{
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_PRESCALER_RELOAD(TIM_PSCReloadMode));
  /* 设置预分频器值 */
  TIMx->PSC = Prescaler;
  /* 置位或清零 UG 位 */
  TIMx->EGR = TIM_PSCReloadMode;
}

/**
  * @brief  指定要使用的 TIMx 计数模式。
  * @param  TIMx: x 可取 1、2、3、4、5 或 8，用于选择 TIM 外设。
  * @param  TIM_CounterMode: 指定要使用的计数模式
  *   该参数可取以下值之一：
  *     @arg TIM_CounterMode_Up: TIM 向上计数模式
  *     @arg TIM_CounterMode_Down: TIM 向下计数模式
  *     @arg TIM_CounterMode_CenterAligned1: TIM 中心对齐模式 1
  *     @arg TIM_CounterMode_CenterAligned2: TIM 中心对齐模式 2
  *     @arg TIM_CounterMode_CenterAligned3: TIM 中心对齐模式 3
  * @retval None
  */
void TIM_CounterModeConfig(TIM_TypeDef* TIMx, uint16_t TIM_CounterMode)
{
  uint16_t tmpcr1 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_LIST3_PERIPH(TIMx));
  assert_param(IS_TIM_COUNTER_MODE(TIM_CounterMode));
  tmpcr1 = TIMx->CR1;
  /* 清零 CMS 和 DIR 位 */
  tmpcr1 &= (uint16_t)(~((uint16_t)(TIM_CR1_DIR | TIM_CR1_CMS)));
  /* 设置计数模式 */
  tmpcr1 |= TIM_CounterMode;
  /* 写入 TIMx CR1 寄存器 */
  TIMx->CR1 = tmpcr1;
}

/**
  * @brief  选择输入触发源
  * @param  TIMx: x 可取 1、2、3、4、5、8、9、12 或 15，用于选择 TIM 外设。
  * @param  TIM_InputTriggerSource: 输入触发源。
  *   该参数可取以下值之一：
  *     @arg TIM_TS_ITR0: 内部触发 0
  *     @arg TIM_TS_ITR1: 内部触发 1
  *     @arg TIM_TS_ITR2: 内部触发 2
  *     @arg TIM_TS_ITR3: 内部触发 3
  *     @arg TIM_TS_TI1F_ED: TI1 边沿检测器
  *     @arg TIM_TS_TI1FP1: 滤波后的定时器输入 1
  *     @arg TIM_TS_TI2FP2: 滤波后的定时器输入 2
  *     @arg TIM_TS_ETRF: 外部触发输入
  * @retval None
  */
void TIM_SelectInputTrigger(TIM_TypeDef* TIMx, uint16_t TIM_InputTriggerSource)
{
  uint16_t tmpsmcr = 0;
  /* 检查参数 */
  assert_param(IS_TIM_LIST6_PERIPH(TIMx));
  assert_param(IS_TIM_TRIGGER_SELECTION(TIM_InputTriggerSource));
  /* 获取 TIMx SMCR 寄存器的值 */
  tmpsmcr = TIMx->SMCR;
  /* 清零 TS 位 */
  tmpsmcr &= (uint16_t)(~((uint16_t)TIM_SMCR_TS));
  /* 设置输入触发源 */
  tmpsmcr |= TIM_InputTriggerSource;
  /* 写入 TIMx SMCR 寄存器 */
  TIMx->SMCR = tmpsmcr;
}

/**
  * @brief  配置 TIMx 编码器接口。
  * @param  TIMx: x 可取 1、2、3、4、5 或 8，用于选择 TIM 外设。
  * @param  TIM_EncoderMode: 指定 TIMx 编码器模式。
  *   该参数可取以下值之一：
  *     @arg TIM_EncoderMode_TI1: 计数器根据 TI2FP2 电平在 TI1FP1 边沿上计数。
  *     @arg TIM_EncoderMode_TI2: 计数器根据 TI1FP1 电平在 TI2FP2 边沿上计数。
  *     @arg TIM_EncoderMode_TI12: 计数器根据另一路输入的电平，在 TI1FP1 和 TI2FP2
  *                                的边沿上计数。
  * @param  TIM_IC1Polarity: 指定 IC1 极性
  *   该参数可取以下值之一：
  *     @arg TIM_ICPolarity_Falling: IC 下降沿。
  *     @arg TIM_ICPolarity_Rising: IC 上升沿。
  * @param  TIM_IC2Polarity: 指定 IC2 极性
  *   该参数可取以下值之一：
  *     @arg TIM_ICPolarity_Falling: IC 下降沿。
  *     @arg TIM_ICPolarity_Rising: IC 上升沿。
  * @retval None
  */
void TIM_EncoderInterfaceConfig(TIM_TypeDef* TIMx, uint16_t TIM_EncoderMode,
                                uint16_t TIM_IC1Polarity, uint16_t TIM_IC2Polarity)
{
  uint16_t tmpsmcr = 0;
  uint16_t tmpccmr1 = 0;
  uint16_t tmpccer = 0;
    
  /* 检查参数 */
  assert_param(IS_TIM_LIST5_PERIPH(TIMx));
  assert_param(IS_TIM_ENCODER_MODE(TIM_EncoderMode));
  assert_param(IS_TIM_IC_POLARITY(TIM_IC1Polarity));
  assert_param(IS_TIM_IC_POLARITY(TIM_IC2Polarity));

  /* 获取 TIMx SMCR 寄存器的值 */
  tmpsmcr = TIMx->SMCR;
  
  /* 获取 TIMx CCMR1 寄存器的值 */
  tmpccmr1 = TIMx->CCMR1;
  
  /* 获取 TIMx CCER 寄存器的值 */
  tmpccer = TIMx->CCER;
  
  /* 设置编码器模式 */
  tmpsmcr &= (uint16_t)(~((uint16_t)TIM_SMCR_SMS));
  tmpsmcr |= TIM_EncoderMode;
  
  /* 将捕获比较 1 和捕获比较 2 选择为输入 */
  tmpccmr1 &= (uint16_t)(((uint16_t)~((uint16_t)TIM_CCMR1_CC1S)) & (uint16_t)(~((uint16_t)TIM_CCMR1_CC2S)));
  tmpccmr1 |= TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0;
  
  /* 设置 TI1 和 TI2 极性 */
  tmpccer &= (uint16_t)(((uint16_t)~((uint16_t)TIM_CCER_CC1P)) & ((uint16_t)~((uint16_t)TIM_CCER_CC2P)));
  tmpccer |= (uint16_t)(TIM_IC1Polarity | (uint16_t)(TIM_IC2Polarity << (uint16_t)4));
  
  /* 写入 TIMx SMCR 寄存器 */
  TIMx->SMCR = tmpsmcr;
  /* 写入 TIMx CCMR1 寄存器 */
  TIMx->CCMR1 = tmpccmr1;
  /* 写入 TIMx CCER 寄存器 */
  TIMx->CCER = tmpccer;
}

/**
  * @brief  强制 TIMx 输出 1 的波形为有效或无效电平。
  * @param  TIMx: x 可取 1 到 17（6 和 7 除外），用于选择 TIM 外设。
  * @param  TIM_ForcedAction: 指定要强制设置到输出波形上的动作。
  *   该参数可取以下值之一：
  *     @arg TIM_ForcedAction_Active: 强制 OC1REF 为有效电平
  *     @arg TIM_ForcedAction_InActive: 强制 OC1REF 为无效电平。
  * @retval None
  */
void TIM_ForcedOC1Config(TIM_TypeDef* TIMx, uint16_t TIM_ForcedAction)
{
  uint16_t tmpccmr1 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_LIST8_PERIPH(TIMx));
  assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));
  tmpccmr1 = TIMx->CCMR1;
  /* 清零 OC1M 位 */
  tmpccmr1 &= (uint16_t)~((uint16_t)TIM_CCMR1_OC1M);
  /* 配置强制输出模式 */
  tmpccmr1 |= TIM_ForcedAction;
  /* 写入 TIMx CCMR1 寄存器 */
  TIMx->CCMR1 = tmpccmr1;
}

/**
  * @brief  强制 TIMx 输出 2 的波形为有效或无效电平。
  * @param  TIMx: x 可取 1、2、3、4、5、8、9、12 或 15，用于选择 TIM 外设。
  * @param  TIM_ForcedAction: 指定要强制设置到输出波形上的动作。
  *   该参数可取以下值之一：
  *     @arg TIM_ForcedAction_Active: 强制 OC2REF 为有效电平
  *     @arg TIM_ForcedAction_InActive: 强制 OC2REF 为无效电平。
  * @retval None
  */
void TIM_ForcedOC2Config(TIM_TypeDef* TIMx, uint16_t TIM_ForcedAction)
{
  uint16_t tmpccmr1 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_LIST6_PERIPH(TIMx));
  assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));
  tmpccmr1 = TIMx->CCMR1;
  /* 清零 OC2M 位 */
  tmpccmr1 &= (uint16_t)~((uint16_t)TIM_CCMR1_OC2M);
  /* 配置强制输出模式 */
  tmpccmr1 |= (uint16_t)(TIM_ForcedAction << 8);
  /* 写入 TIMx CCMR1 寄存器 */
  TIMx->CCMR1 = tmpccmr1;
}

/**
  * @brief  强制 TIMx 输出 3 的波形为有效或无效电平。
  * @param  TIMx: x 可取 1、2、3、4、5 或 8，用于选择 TIM 外设。
  * @param  TIM_ForcedAction: 指定要强制设置到输出波形上的动作。
  *   该参数可取以下值之一：
  *     @arg TIM_ForcedAction_Active: 强制 OC3REF 为有效电平
  *     @arg TIM_ForcedAction_InActive: 强制 OC3REF 为无效电平。
  * @retval None
  */
void TIM_ForcedOC3Config(TIM_TypeDef* TIMx, uint16_t TIM_ForcedAction)
{
  uint16_t tmpccmr2 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_LIST3_PERIPH(TIMx));
  assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));
  tmpccmr2 = TIMx->CCMR2;
  /* 清零 OC1M 位 */
  tmpccmr2 &= (uint16_t)~((uint16_t)TIM_CCMR2_OC3M);
  /* 配置强制输出模式 */
  tmpccmr2 |= TIM_ForcedAction;
  /* 写入 TIMx CCMR2 寄存器 */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @brief  强制 TIMx 输出 4 的波形为有效或无效电平。
  * @param  TIMx: x 可取 1、2、3、4、5 或 8，用于选择 TIM 外设。
  * @param  TIM_ForcedAction: 指定要强制设置到输出波形上的动作。
  *   该参数可取以下值之一：
  *     @arg TIM_ForcedAction_Active: 强制 OC4REF 为有效电平
  *     @arg TIM_ForcedAction_InActive: 强制 OC4REF 为无效电平。
  * @retval None
  */
void TIM_ForcedOC4Config(TIM_TypeDef* TIMx, uint16_t TIM_ForcedAction)
{
  uint16_t tmpccmr2 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_LIST3_PERIPH(TIMx));
  assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));
  tmpccmr2 = TIMx->CCMR2;
  /* 清零 OC2M 位 */
  tmpccmr2 &= (uint16_t)~((uint16_t)TIM_CCMR2_OC4M);
  /* 配置强制输出模式 */
  tmpccmr2 |= (uint16_t)(TIM_ForcedAction << 8);
  /* 写入 TIMx CCMR2 寄存器 */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @brief  使能或关闭 TIMx 外设 ARR 上的预装载寄存器。
  * @param  TIMx: x 可取 1 到 17，用于选择 TIM 外设。
  * @param  NewState: TIMx 外设预装载寄存器的新状态
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval None
  */
void TIM_ARRPreloadConfig(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 置位 ARR 预装载位 */
    TIMx->CR1 |= TIM_CR1_ARPE;
  }
  else
  {
    /* 清零 ARR 预装载位 */
    TIMx->CR1 &= (uint16_t)~((uint16_t)TIM_CR1_ARPE);
  }
}

/**
  * @brief  选择 TIM 外设换相事件。
  * @param  TIMx: x 可取 1、8、15、16 或 17，用于选择 TIMx 外设
  * @param  NewState: 换相事件的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval None
  */
void TIM_SelectCOM(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_TIM_LIST2_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 置位 COM 位 */
    TIMx->CR2 |= TIM_CR2_CCUS;
  }
  else
  {
    /* 清零 COM 位 */
    TIMx->CR2 &= (uint16_t)~((uint16_t)TIM_CR2_CCUS);
  }
}

/**
  * @brief  选择 TIMx 外设的捕获比较 DMA 源。
  * @param  TIMx: x 可取 1、2、3、4、5、8、15、16 或 17，
  *         用于选择 TIM 外设。
  * @param  NewState: 捕获比较 DMA 源的新状态
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval None
  */
void TIM_SelectCCDMA(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_TIM_LIST4_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 置位 CCDS 位 */
    TIMx->CR2 |= TIM_CR2_CCDS;
  }
  else
  {
    /* 清零 CCDS 位 */
    TIMx->CR2 &= (uint16_t)~((uint16_t)TIM_CR2_CCDS);
  }
}

/**
  * @brief  置位或清零 TIM 外设的捕获比较预装载控制位。
  * @param  TIMx: x 可取 1、2、3、4、5、8 或 15，
  *         用于选择 TIMx 外设
  * @param  NewState: 捕获比较预装载控制位的新状态
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval None
  */
void TIM_CCPreloadControl(TIM_TypeDef* TIMx, FunctionalState NewState)
{ 
  /* 检查参数 */
  assert_param(IS_TIM_LIST5_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 置位 CCPC 位 */
    TIMx->CR2 |= TIM_CR2_CCPC;
  }
  else
  {
    /* 清零 CCPC 位 */
    TIMx->CR2 &= (uint16_t)~((uint16_t)TIM_CR2_CCPC);
  }
}

/**
  * @brief  使能或关闭 TIMx 外设 CCR1 上的预装载寄存器。
  * @param  TIMx: x 可取 1 到 17（6 和 7 除外），用于选择 TIM 外设。
  * @param  TIM_OCPreload: TIMx 外设预装载寄存器的新状态
  *   该参数可取以下值之一：
  *     @arg TIM_OCPreload_Enable
  *     @arg TIM_OCPreload_Disable
  * @retval None
  */
void TIM_OC1PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload)
{
  uint16_t tmpccmr1 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_LIST8_PERIPH(TIMx));
  assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));
  tmpccmr1 = TIMx->CCMR1;
  /* 清零 OC1PE 位 */
  tmpccmr1 &= (uint16_t)~((uint16_t)TIM_CCMR1_OC1PE);
  /* 使能或关闭输出比较预装载功能 */
  tmpccmr1 |= TIM_OCPreload;
  /* 写入 TIMx CCMR1 寄存器 */
  TIMx->CCMR1 = tmpccmr1;
}

/**
  * @brief  使能或关闭 TIMx 外设 CCR2 上的预装载寄存器。
  * @param  TIMx: x 可取 1、2、3、4、5、8、9、12 或 15，
  *         用于选择 TIM 外设。
  * @param  TIM_OCPreload: TIMx 外设预装载寄存器的新状态
  *   该参数可取以下值之一：
  *     @arg TIM_OCPreload_Enable
  *     @arg TIM_OCPreload_Disable
  * @retval None
  */
void TIM_OC2PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload)
{
  uint16_t tmpccmr1 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_LIST6_PERIPH(TIMx));
  assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));
  tmpccmr1 = TIMx->CCMR1;
  /* 清零 OC2PE 位 */
  tmpccmr1 &= (uint16_t)~((uint16_t)TIM_CCMR1_OC2PE);
  /* 使能或关闭输出比较预装载功能 */
  tmpccmr1 |= (uint16_t)(TIM_OCPreload << 8);
  /* 写入 TIMx CCMR1 寄存器 */
  TIMx->CCMR1 = tmpccmr1;
}

/**
  * @brief  使能或关闭 TIMx 外设 CCR3 上的预装载寄存器。
  * @param  TIMx: x 可取 1、2、3、4、5 或 8，用于选择 TIM 外设。
  * @param  TIM_OCPreload: TIMx 外设预装载寄存器的新状态
  *   该参数可取以下值之一：
  *     @arg TIM_OCPreload_Enable
  *     @arg TIM_OCPreload_Disable
  * @retval None
  */
void TIM_OC3PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload)
{
  uint16_t tmpccmr2 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_LIST3_PERIPH(TIMx));
  assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));
  tmpccmr2 = TIMx->CCMR2;
  /* 清零 OC3PE 位 */
  tmpccmr2 &= (uint16_t)~((uint16_t)TIM_CCMR2_OC3PE);
  /* 使能或关闭输出比较预装载功能 */
  tmpccmr2 |= TIM_OCPreload;
  /* 写入 TIMx CCMR2 寄存器 */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @brief  使能或关闭 TIMx 外设 CCR4 上的预装载寄存器。
  * @param  TIMx: x 可取 1、2、3、4、5 或 8，用于选择 TIM 外设。
  * @param  TIM_OCPreload: TIMx 外设预装载寄存器的新状态
  *   该参数可取以下值之一：
  *     @arg TIM_OCPreload_Enable
  *     @arg TIM_OCPreload_Disable
  * @retval None
  */
void TIM_OC4PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload)
{
  uint16_t tmpccmr2 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_LIST3_PERIPH(TIMx));
  assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));
  tmpccmr2 = TIMx->CCMR2;
  /* 清零 OC4PE 位 */
  tmpccmr2 &= (uint16_t)~((uint16_t)TIM_CCMR2_OC4PE);
  /* 使能或关闭输出比较预装载功能 */
  tmpccmr2 |= (uint16_t)(TIM_OCPreload << 8);
  /* 写入 TIMx CCMR2 寄存器 */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @brief  配置 TIMx 输出比较 1 的快速功能。
  * @param  TIMx: x 可取 1 到 17（6 和 7 除外），用于选择 TIM 外设。
  * @param  TIM_OCFast: 输出比较快速使能位的新状态。
  *   该参数可取以下值之一：
  *     @arg TIM_OCFast_Enable: TIM 输出比较快速使能
  *     @arg TIM_OCFast_Disable: TIM 输出比较快速关闭
  * @retval None
  */
void TIM_OC1FastConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCFast)
{
  uint16_t tmpccmr1 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_LIST8_PERIPH(TIMx));
  assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));
  /* 获取 TIMx CCMR1 寄存器的值 */
  tmpccmr1 = TIMx->CCMR1;
  /* 清零 OC1FE 位 */
  tmpccmr1 &= (uint16_t)~((uint16_t)TIM_CCMR1_OC1FE);
  /* 使能或关闭输出比较快速位 */
  tmpccmr1 |= TIM_OCFast;
  /* 写入 TIMx CCMR1 寄存器 */
  TIMx->CCMR1 = tmpccmr1;
}

/**
  * @brief  配置 TIMx 输出比较 2 的快速功能。
  * @param  TIMx: x 可取 1、2、3、4、5、8、9、12 或 15，
  *         用于选择 TIM 外设。
  * @param  TIM_OCFast: 输出比较快速使能位的新状态。
  *   该参数可取以下值之一：
  *     @arg TIM_OCFast_Enable: TIM 输出比较快速使能
  *     @arg TIM_OCFast_Disable: TIM 输出比较快速关闭
  * @retval None
  */
void TIM_OC2FastConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCFast)
{
  uint16_t tmpccmr1 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_LIST6_PERIPH(TIMx));
  assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));
  /* 获取 TIMx CCMR1 寄存器的值 */
  tmpccmr1 = TIMx->CCMR1;
  /* 清零 OC2FE 位 */
  tmpccmr1 &= (uint16_t)~((uint16_t)TIM_CCMR1_OC2FE);
  /* 使能或关闭输出比较快速位 */
  tmpccmr1 |= (uint16_t)(TIM_OCFast << 8);
  /* 写入 TIMx CCMR1 寄存器 */
  TIMx->CCMR1 = tmpccmr1;
}

/**
  * @brief  配置 TIMx 输出比较 3 的快速功能。
  * @param  TIMx: x 可取 1、2、3、4、5 或 8，用于选择 TIM 外设。
  * @param  TIM_OCFast: 输出比较快速使能位的新状态。
  *   该参数可取以下值之一：
  *     @arg TIM_OCFast_Enable: TIM 输出比较快速使能
  *     @arg TIM_OCFast_Disable: TIM 输出比较快速关闭
  * @retval None
  */
void TIM_OC3FastConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCFast)
{
  uint16_t tmpccmr2 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_LIST3_PERIPH(TIMx));
  assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));
  /* 获取 TIMx CCMR2 寄存器的值 */
  tmpccmr2 = TIMx->CCMR2;
  /* 清零 OC3FE 位 */
  tmpccmr2 &= (uint16_t)~((uint16_t)TIM_CCMR2_OC3FE);
  /* 使能或关闭输出比较快速位 */
  tmpccmr2 |= TIM_OCFast;
  /* 写入 TIMx CCMR2 寄存器 */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @brief  配置 TIMx 输出比较 4 的快速功能。
  * @param  TIMx: x 可取 1、2、3、4、5 或 8，用于选择 TIM 外设。
  * @param  TIM_OCFast: 输出比较快速使能位的新状态。
  *   该参数可取以下值之一：
  *     @arg TIM_OCFast_Enable: TIM 输出比较快速使能
  *     @arg TIM_OCFast_Disable: TIM 输出比较快速关闭
  * @retval None
  */
void TIM_OC4FastConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCFast)
{
  uint16_t tmpccmr2 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_LIST3_PERIPH(TIMx));
  assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));
  /* 获取 TIMx CCMR2 寄存器的值 */
  tmpccmr2 = TIMx->CCMR2;
  /* 清零 OC4FE 位 */
  tmpccmr2 &= (uint16_t)~((uint16_t)TIM_CCMR2_OC4FE);
  /* 使能或关闭输出比较快速位 */
  tmpccmr2 |= (uint16_t)(TIM_OCFast << 8);
  /* 写入 TIMx CCMR2 寄存器 */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @brief  在外部事件上清零或保护 OCREF1 信号
  * @param  TIMx: x 可取 1、2、3、4、5 或 8，用于选择 TIM 外设。
  * @param  TIM_OCClear: 输出比较清零使能位的新状态。
  *   该参数可取以下值之一：
  *     @arg TIM_OCClear_Enable: TIM 输出清零使能
  *     @arg TIM_OCClear_Disable: TIM 输出清零关闭
  * @retval None
  */
void TIM_ClearOC1Ref(TIM_TypeDef* TIMx, uint16_t TIM_OCClear)
{
  uint16_t tmpccmr1 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_LIST3_PERIPH(TIMx));
  assert_param(IS_TIM_OCCLEAR_STATE(TIM_OCClear));

  tmpccmr1 = TIMx->CCMR1;

  /* 清零 OC1CE 位 */
  tmpccmr1 &= (uint16_t)~((uint16_t)TIM_CCMR1_OC1CE);
  /* 使能或关闭输出比较清零位 */
  tmpccmr1 |= TIM_OCClear;
  /* 写入 TIMx CCMR1 寄存器 */
  TIMx->CCMR1 = tmpccmr1;
}

/**
  * @brief  在外部事件上清零或保护 OCREF2 信号
  * @param  TIMx: x 可取 1、2、3、4、5 或 8，用于选择 TIM 外设。
  * @param  TIM_OCClear: 输出比较清零使能位的新状态。
  *   该参数可取以下值之一：
  *     @arg TIM_OCClear_Enable: TIM 输出清零使能
  *     @arg TIM_OCClear_Disable: TIM 输出清零关闭
  * @retval None
  */
void TIM_ClearOC2Ref(TIM_TypeDef* TIMx, uint16_t TIM_OCClear)
{
  uint16_t tmpccmr1 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_LIST3_PERIPH(TIMx));
  assert_param(IS_TIM_OCCLEAR_STATE(TIM_OCClear));
  tmpccmr1 = TIMx->CCMR1;
  /* 清零 OC2CE 位 */
  tmpccmr1 &= (uint16_t)~((uint16_t)TIM_CCMR1_OC2CE);
  /* 使能或关闭输出比较清零位 */
  tmpccmr1 |= (uint16_t)(TIM_OCClear << 8);
  /* 写入 TIMx CCMR1 寄存器 */
  TIMx->CCMR1 = tmpccmr1;
}

/**
  * @brief  在外部事件上清零或保护 OCREF3 信号
  * @param  TIMx: x 可取 1、2、3、4、5 或 8，用于选择 TIM 外设。
  * @param  TIM_OCClear: 输出比较清零使能位的新状态。
  *   该参数可取以下值之一：
  *     @arg TIM_OCClear_Enable: TIM 输出清零使能
  *     @arg TIM_OCClear_Disable: TIM 输出清零关闭
  * @retval None
  */
void TIM_ClearOC3Ref(TIM_TypeDef* TIMx, uint16_t TIM_OCClear)
{
  uint16_t tmpccmr2 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_LIST3_PERIPH(TIMx));
  assert_param(IS_TIM_OCCLEAR_STATE(TIM_OCClear));
  tmpccmr2 = TIMx->CCMR2;
  /* 清零 OC3CE 位 */
  tmpccmr2 &= (uint16_t)~((uint16_t)TIM_CCMR2_OC3CE);
  /* 使能或关闭输出比较清零位 */
  tmpccmr2 |= TIM_OCClear;
  /* 写入 TIMx CCMR2 寄存器 */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @brief  在外部事件上清零或保护 OCREF4 信号
  * @param  TIMx: x 可取 1、2、3、4、5 或 8，用于选择 TIM 外设。
  * @param  TIM_OCClear: 输出比较清零使能位的新状态。
  *   该参数可取以下值之一：
  *     @arg TIM_OCClear_Enable: TIM 输出清零使能
  *     @arg TIM_OCClear_Disable: TIM 输出清零关闭
  * @retval None
  */
void TIM_ClearOC4Ref(TIM_TypeDef* TIMx, uint16_t TIM_OCClear)
{
  uint16_t tmpccmr2 = 0;
  /* 检查参数 */
  assert_param(IS_TIM_LIST3_PERIPH(TIMx));
  assert_param(IS_TIM_OCCLEAR_STATE(TIM_OCClear));
  tmpccmr2 = TIMx->CCMR2;
  /* 清零 OC4CE 位 */
  tmpccmr2 &= (uint16_t)~((uint16_t)TIM_CCMR2_OC4CE);
  /* 使能或关闭输出比较清零位 */
  tmpccmr2 |= (uint16_t)(TIM_OCClear << 8);
  /* 写入 TIMx CCMR2 寄存器 */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @brief  配置 TIMx 通道 1 极性。
  * @param  TIMx: x 可取 1 到 17（6 和 7 除外），用于选择 TIM 外设。
  * @param  TIM_OCPolarity: 指定 OC1 极性
  *   该参数可取以下值之一：
  *     @arg TIM_OCPolarity_High: 输出比较高电平有效
  *     @arg TIM_OCPolarity_Low: 输出比较低电平有效
  * @retval None
  */
void TIM_OC1PolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPolarity)
{
  uint16_t tmpccer = 0;
  /* 检查参数 */
  assert_param(IS_TIM_LIST8_PERIPH(TIMx));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));
  tmpccer = TIMx->CCER;
  /* 置位或清零 CC1P 位 */
  tmpccer &= (uint16_t)~((uint16_t)TIM_CCER_CC1P);
  tmpccer |= TIM_OCPolarity;
  /* 写入 TIMx CCER 寄存器 */
  TIMx->CCER = tmpccer;
}

/**
  * @brief  配置 TIMx 通道 1N 极性。
  * @param  TIMx: x 可取 1、8、15、16 或 17，用于选择 TIM 外设。
  * @param  TIM_OCNPolarity: 指定 OC1N 极性
  *   该参数可取以下值之一：
  *     @arg TIM_OCNPolarity_High: 输出比较高电平有效
  *     @arg TIM_OCNPolarity_Low: 输出比较低电平有效
  * @retval None
  */
void TIM_OC1NPolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCNPolarity)
{
  uint16_t tmpccer = 0;
  /* 检查参数 */
  assert_param(IS_TIM_LIST2_PERIPH(TIMx));
  assert_param(IS_TIM_OCN_POLARITY(TIM_OCNPolarity));
   
  tmpccer = TIMx->CCER;
  /* 置位或清零 CC1NP 位 */
  tmpccer &= (uint16_t)~((uint16_t)TIM_CCER_CC1NP);
  tmpccer |= TIM_OCNPolarity;
  /* 写入 TIMx CCER 寄存器 */
  TIMx->CCER = tmpccer;
}

/**
  * @brief  配置 TIMx 通道 2 极性。
  * @param  TIMx: x 可取 1、2、3、4、5、8、9、12 或 15，用于选择 TIM 外设。
  * @param  TIM_OCPolarity: 指定 OC2 极性
  *   该参数可取以下值之一：
  *     @arg TIM_OCPolarity_High: 输出比较高电平有效
  *     @arg TIM_OCPolarity_Low: 输出比较低电平有效
  * @retval None
  */
void TIM_OC2PolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPolarity)
{
  uint16_t tmpccer = 0;
  /* 检查参数 */
  assert_param(IS_TIM_LIST6_PERIPH(TIMx));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));
  tmpccer = TIMx->CCER;
  /* 置位或清零 CC2P 位 */
  tmpccer &= (uint16_t)~((uint16_t)TIM_CCER_CC2P);
  tmpccer |= (uint16_t)(TIM_OCPolarity << 4);
  /* 写入 TIMx CCER 寄存器 */
  TIMx->CCER = tmpccer;
}

/**
  * @brief  配置 TIMx 通道 2N 极性。
  * @param  TIMx: x 可取 1 或 8，用于选择 TIM 外设。
  * @param  TIM_OCNPolarity: 指定 OC2N 极性
  *   该参数可取以下值之一：
  *     @arg TIM_OCNPolarity_High: 输出比较高电平有效
  *     @arg TIM_OCNPolarity_Low: 输出比较低电平有效
  * @retval None
  */
void TIM_OC2NPolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCNPolarity)
{
  uint16_t tmpccer = 0;
  /* 检查参数 */
  assert_param(IS_TIM_LIST1_PERIPH(TIMx));
  assert_param(IS_TIM_OCN_POLARITY(TIM_OCNPolarity));
  
  tmpccer = TIMx->CCER;
  /* 置位或清零 CC2NP 位 */
  tmpccer &= (uint16_t)~((uint16_t)TIM_CCER_CC2NP);
  tmpccer |= (uint16_t)(TIM_OCNPolarity << 4);
  /* 写入 TIMx CCER 寄存器 */
  TIMx->CCER = tmpccer;
}

/**
  * @brief  配置 TIMx 通道 3 极性。
  * @param  TIMx: x 可取 1、2、3、4、5 或 8，用于选择 TIM 外设。
  * @param  TIM_OCPolarity: 指定 OC3 极性
  *   该参数可取以下值之一：
  *     @arg TIM_OCPolarity_High: 输出比较高电平有效
  *     @arg TIM_OCPolarity_Low: 输出比较低电平有效
  * @retval None
  */
void TIM_OC3PolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPolarity)
{
  uint16_t tmpccer = 0;
  /* 检查参数 */
  assert_param(IS_TIM_LIST3_PERIPH(TIMx));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));
  tmpccer = TIMx->CCER;
  /* 置位或清零 CC3P 位 */
  tmpccer &= (uint16_t)~((uint16_t)TIM_CCER_CC3P);
  tmpccer |= (uint16_t)(TIM_OCPolarity << 8);
  /* 写入 TIMx CCER 寄存器 */
  TIMx->CCER = tmpccer;
}

/**
  * @brief  配置 TIMx 通道 3N 极性。
  * @param  TIMx: x 可取 1 或 8，用于选择 TIM 外设。
  * @param  TIM_OCNPolarity: 指定 OC3N 极性
  *   该参数可取以下值之一：
  *     @arg TIM_OCNPolarity_High: 输出比较高电平有效
  *     @arg TIM_OCNPolarity_Low: 输出比较低电平有效
  * @retval None
  */
void TIM_OC3NPolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCNPolarity)
{
  uint16_t tmpccer = 0;
 
  /* 检查参数 */
  assert_param(IS_TIM_LIST1_PERIPH(TIMx));
  assert_param(IS_TIM_OCN_POLARITY(TIM_OCNPolarity));
    
  tmpccer = TIMx->CCER;
  /* 置位或清零 CC3NP 位 */
  tmpccer &= (uint16_t)~((uint16_t)TIM_CCER_CC3NP);
  tmpccer |= (uint16_t)(TIM_OCNPolarity << 8);
  /* 写入 TIMx CCER 寄存器 */
  TIMx->CCER = tmpccer;
}

/**
  * @brief  配置 TIMx 通道 4 极性。
  * @param  TIMx: x 可取 1、2、3、4、5 或 8，用于选择 TIM 外设。
  * @param  TIM_OCPolarity: 指定 OC4 极性
  *   该参数可取以下值之一：
  *     @arg TIM_OCPolarity_High: 输出比较高电平有效
  *     @arg TIM_OCPolarity_Low: 输出比较低电平有效
  * @retval None
  */
void TIM_OC4PolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPolarity)
{
  uint16_t tmpccer = 0;
  /* 检查参数 */
  assert_param(IS_TIM_LIST3_PERIPH(TIMx));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));
  tmpccer = TIMx->CCER;
  /* 置位或清零 CC4P 位 */
  tmpccer &= (uint16_t)~((uint16_t)TIM_CCER_CC4P);
  tmpccer |= (uint16_t)(TIM_OCPolarity << 12);
  /* 写入 TIMx CCER 寄存器 */
  TIMx->CCER = tmpccer;
}

/**
  * @brief  使能或关闭 TIM 捕获比较通道 x。
  * @param  TIMx: x 可取 1 到 17（6 和 7 除外），用于选择 TIM 外设。
  * @param  TIM_Channel: 指定 TIM 通道
  *   该参数可取以下值之一：
  *     @arg TIM_Channel_1: TIM 通道 1
  *     @arg TIM_Channel_2: TIM 通道 2
  *     @arg TIM_Channel_3: TIM 通道 3
  *     @arg TIM_Channel_4: TIM 通道 4
  * @param  TIM_CCx: 指定 TIM 通道 CCxE 位的新状态。
  *   该参数可取：TIM_CCx_Enable 或 TIM_CCx_Disable。
  * @retval None
  */
void TIM_CCxCmd(TIM_TypeDef* TIMx, uint16_t TIM_Channel, uint16_t TIM_CCx)
{
  uint16_t tmp = 0;

  /* 检查参数 */
  assert_param(IS_TIM_LIST8_PERIPH(TIMx));
  assert_param(IS_TIM_CHANNEL(TIM_Channel));
  assert_param(IS_TIM_CCX(TIM_CCx));

  tmp = CCER_CCE_Set << TIM_Channel;

  /* 清零 CCxE 位 */
  TIMx->CCER &= (uint16_t)~ tmp;

  /* 置位或清零 CCxE 位 */ 
  TIMx->CCER |=  (uint16_t)(TIM_CCx << TIM_Channel);
}

/**
  * @brief  使能或关闭 TIM 捕获比较通道 xN。
  * @param  TIMx: x 可取 1、8、15、16 或 17，用于选择 TIM 外设。
  * @param  TIM_Channel: 指定 TIM 通道
  *   该参数可取以下值之一：
  *     @arg TIM_Channel_1: TIM 通道 1
  *     @arg TIM_Channel_2: TIM 通道 2
  *     @arg TIM_Channel_3: TIM 通道 3
  * @param  TIM_CCxN: 指定 TIM 通道 CCxNE 位的新状态。
  *   该参数可取：TIM_CCxN_Enable 或 TIM_CCxN_Disable。
  * @retval None
  */
void TIM_CCxNCmd(TIM_TypeDef* TIMx, uint16_t TIM_Channel, uint16_t TIM_CCxN)
{
  uint16_t tmp = 0;

  /* 检查参数 */
  assert_param(IS_TIM_LIST2_PERIPH(TIMx));
  assert_param(IS_TIM_COMPLEMENTARY_CHANNEL(TIM_Channel));
  assert_param(IS_TIM_CCXN(TIM_CCxN));

  tmp = CCER_CCNE_Set << TIM_Channel;

  /* 清零 CCxNE 位 */
  TIMx->CCER &= (uint16_t) ~tmp;

  /* 置位或清零 CCxNE 位 */ 
  TIMx->CCER |=  (uint16_t)(TIM_CCxN << TIM_Channel);
}

/**
  * @brief  选择 TIM 输出比较模式。
  * @note   本函数在更改输出比较模式之前会先关闭选定通道。
  *         用户必须使用 TIM_CCxCmd 和 TIM_CCxNCmd 函数使能该通道。
  * @param  TIMx: x 可取 1 到 17（6 和 7 除外），用于选择 TIM 外设。
  * @param  TIM_Channel: 指定 TIM 通道
  *   该参数可取以下值之一：
  *     @arg TIM_Channel_1: TIM 通道 1
  *     @arg TIM_Channel_2: TIM 通道 2
  *     @arg TIM_Channel_3: TIM 通道 3
  *     @arg TIM_Channel_4: TIM 通道 4
  * @param  TIM_OCMode: 指定 TIM 输出比较模式。
  *   该参数可取以下值之一：
  *     @arg TIM_OCMode_Timing
  *     @arg TIM_OCMode_Active
  *     @arg TIM_OCMode_Toggle
  *     @arg TIM_OCMode_PWM1
  *     @arg TIM_OCMode_PWM2
  *     @arg TIM_ForcedAction_Active
  *     @arg TIM_ForcedAction_InActive
  * @retval None
  */
void TIM_SelectOCxM(TIM_TypeDef* TIMx, uint16_t TIM_Channel, uint16_t TIM_OCMode)
{
  uint32_t tmp = 0;
  uint16_t tmp1 = 0;

  /* 检查参数 */
  assert_param(IS_TIM_LIST8_PERIPH(TIMx));
  assert_param(IS_TIM_CHANNEL(TIM_Channel));
  assert_param(IS_TIM_OCM(TIM_OCMode));

  tmp = (uint32_t) TIMx;
  tmp += CCMR_Offset;

  tmp1 = CCER_CCE_Set << (uint16_t)TIM_Channel;

  /* 关闭通道：清零 CCxE 位 */
  TIMx->CCER &= (uint16_t) ~tmp1;

  if((TIM_Channel == TIM_Channel_1) ||(TIM_Channel == TIM_Channel_3))
  {
    tmp += (TIM_Channel>>1);

    /* 清零 CCMRx 寄存器中的 OCxM 位 */
    *(__IO uint32_t *) tmp &= (uint32_t)~((uint32_t)TIM_CCMR1_OC1M);
   
    /* 配置 CCMRx 寄存器中的 OCxM 位 */
    *(__IO uint32_t *) tmp |= TIM_OCMode;
  }
  else
  {
    tmp += (uint16_t)(TIM_Channel - (uint16_t)4)>> (uint16_t)1;

    /* 清零 CCMRx 寄存器中的 OCxM 位 */
    *(__IO uint32_t *) tmp &= (uint32_t)~((uint32_t)TIM_CCMR1_OC2M);
    
    /* 配置 CCMRx 寄存器中的 OCxM 位 */
    *(__IO uint32_t *) tmp |= (uint16_t)(TIM_OCMode << 8);
  }
}

/**
  * @brief  使能或关闭 TIMx 更新事件。
  * @param  TIMx: x 可取 1 到 17，用于选择 TIM 外设。
  * @param  NewState: TIMx UDIS 位的新状态
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval None
  */
void TIM_UpdateDisableConfig(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 置位更新禁止位 */
    TIMx->CR1 |= TIM_CR1_UDIS;
  }
  else
  {
    /* 清零更新禁止位 */
    TIMx->CR1 &= (uint16_t)~((uint16_t)TIM_CR1_UDIS);
  }
}

/**
  * @brief  配置 TIMx 更新请求中断源。
  * @param  TIMx: x 可取 1 到 17，用于选择 TIM 外设。
  * @param  TIM_UpdateSource: 指定更新源。
  *   该参数可取以下值之一：
  *     @arg TIM_UpdateSource_Regular: 更新源为计数器上溢/下溢，
  *                                    或置位 UG 位，或通过从模式控制器
  *                                    产生更新。
  *     @arg TIM_UpdateSource_Global: 更新源为计数器上溢/下溢。
  * @retval None
  */
void TIM_UpdateRequestConfig(TIM_TypeDef* TIMx, uint16_t TIM_UpdateSource)
{
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_UPDATE_SOURCE(TIM_UpdateSource));
  if (TIM_UpdateSource != TIM_UpdateSource_Global)
  {
    /* 置位 URS 位 */
    TIMx->CR1 |= TIM_CR1_URS;
  }
  else
  {
    /* 清零 URS 位 */
    TIMx->CR1 &= (uint16_t)~((uint16_t)TIM_CR1_URS);
  }
}

/**
  * @brief  使能或关闭 TIMx 的霍尔传感器接口。
  * @param  TIMx: x 可取 1、2、3、4、5 或 8，用于选择 TIM 外设。
  * @param  NewState: TIMx 霍尔传感器接口的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval None
  */
void TIM_SelectHallSensor(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_TIM_LIST6_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 置位 TI1S 位 */
    TIMx->CR2 |= TIM_CR2_TI1S;
  }
  else
  {
    /* 清零 TI1S 位 */
    TIMx->CR2 &= (uint16_t)~((uint16_t)TIM_CR2_TI1S);
  }
}

/**
  * @brief  选择 TIMx 的单脉冲模式。
  * @param  TIMx: x 可取 1 到 17，用于选择 TIM 外设。
  * @param  TIM_OPMode: 指定要使用的 OPM 模式。
  *   该参数可取以下值之一：
  *     @arg TIM_OPMode_Single
  *     @arg TIM_OPMode_Repetitive
  * @retval None
  */
void TIM_SelectOnePulseMode(TIM_TypeDef* TIMx, uint16_t TIM_OPMode)
{
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_OPM_MODE(TIM_OPMode));
  /* 清零 OPM 位 */
  TIMx->CR1 &= (uint16_t)~((uint16_t)TIM_CR1_OPM);
  /* 配置 OPM 模式 */
  TIMx->CR1 |= TIM_OPMode;
}

/**
  * @brief  选择 TIMx 触发输出模式。
  * @param  TIMx: x 可取 1、2、3、4、5、6、7、8、9、12 或 15，用于选择 TIM 外设。
  * @param  TIM_TRGOSource: 指定触发输出源。
  *   该参数可取以下值之一：
  *
  *  - 对于所有 TIMx
  *     @arg TIM_TRGOSource_Reset:  使用 TIM_EGR 寄存器中的 UG 位作为触发输出（TRGO）。
  *     @arg TIM_TRGOSource_Enable: 使用计数器使能位 CEN 作为触发输出（TRGO）。
  *     @arg TIM_TRGOSource_Update: 选择更新事件作为触发输出（TRGO）。
  *
  *  - 对于除 TIM6 和 TIM7 之外的所有 TIMx
  *     @arg TIM_TRGOSource_OC1: 当即将置位 CC1IF 标志、发生捕获或比较匹配时，
  *                              触发输出发送一个正脉冲（TRGO）。
  *     @arg TIM_TRGOSource_OC1Ref: 使用 OC1REF 信号作为触发输出（TRGO）。
  *     @arg TIM_TRGOSource_OC2Ref: 使用 OC2REF 信号作为触发输出（TRGO）。
  *     @arg TIM_TRGOSource_OC3Ref: 使用 OC3REF 信号作为触发输出（TRGO）。
  *     @arg TIM_TRGOSource_OC4Ref: 使用 OC4REF 信号作为触发输出（TRGO）。
  *
  * @retval None
  */
void TIM_SelectOutputTrigger(TIM_TypeDef* TIMx, uint16_t TIM_TRGOSource)
{
  /* 检查参数 */
  assert_param(IS_TIM_LIST7_PERIPH(TIMx));
  assert_param(IS_TIM_TRGO_SOURCE(TIM_TRGOSource));
  /* 清零 MMS 位 */
  TIMx->CR2 &= (uint16_t)~((uint16_t)TIM_CR2_MMS);
  /* 选择 TRGO 源 */
  TIMx->CR2 |=  TIM_TRGOSource;
}

/**
  * @brief  选择 TIMx 从模式。
  * @param  TIMx: x 可取 1、2、3、4、5、8、9、12 或 15，用于选择 TIM 外设。
  * @param  TIM_SlaveMode: 指定定时器从模式。
  *   该参数可取以下值之一：
  *     @arg TIM_SlaveMode_Reset: 选定触发信号（TRGI）的上升沿重新初始化
  *                               计数器并触发寄存器更新。
  *     @arg TIM_SlaveMode_Gated:     当触发信号（TRGI）为高电平时，计数器时钟被使能。
  *     @arg TIM_SlaveMode_Trigger:   计数器在触发信号 TRGI 的上升沿启动。
  *     @arg TIM_SlaveMode_External1: 选定触发信号（TRGI）的上升沿为计数器提供时钟。
  * @retval None
  */
void TIM_SelectSlaveMode(TIM_TypeDef* TIMx, uint16_t TIM_SlaveMode)
{
  /* 检查参数 */
  assert_param(IS_TIM_LIST6_PERIPH(TIMx));
  assert_param(IS_TIM_SLAVE_MODE(TIM_SlaveMode));
 /* 清零 SMS 位 */
  TIMx->SMCR &= (uint16_t)~((uint16_t)TIM_SMCR_SMS);
  /* 选择从模式 */
  TIMx->SMCR |= TIM_SlaveMode;
}

/**
  * @brief  置位或清零 TIMx 主/从模式。
  * @param  TIMx: x 可取 1、2、3、4、5、8、9、12 或 15，用于选择 TIM 外设。
  * @param  TIM_MasterSlaveMode: 指定定时器主从模式。
  *   该参数可取以下值之一：
  *     @arg TIM_MasterSlaveMode_Enable: 当前定时器与其从定时器之间
  *                                      实现同步（通过 TRGO）。
  *     @arg TIM_MasterSlaveMode_Disable: 无操作
  * @retval None
  */
void TIM_SelectMasterSlaveMode(TIM_TypeDef* TIMx, uint16_t TIM_MasterSlaveMode)
{
  /* 检查参数 */
  assert_param(IS_TIM_LIST6_PERIPH(TIMx));
  assert_param(IS_TIM_MSM_STATE(TIM_MasterSlaveMode));
  /* 清零 MSM 位 */
  TIMx->SMCR &= (uint16_t)~((uint16_t)TIM_SMCR_MSM);
  
  /* 置位或清零 MSM 位 */
  TIMx->SMCR |= TIM_MasterSlaveMode;
}

/**
  * @brief  设置 TIMx 计数器寄存器值
  * @param  TIMx: x 可取 1 到 17，用于选择 TIM 外设。
  * @param  Counter: 指定计数器寄存器的新值。
  * @retval None
  */
void TIM_SetCounter(TIM_TypeDef* TIMx, uint16_t Counter)
{
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  /* 设置计数器寄存器值 */
  TIMx->CNT = Counter;
}

/**
  * @brief  设置 TIMx 自动重装载寄存器值
  * @param  TIMx: x 可取 1 到 17，用于选择 TIM 外设。
  * @param  Autoreload: 指定自动重装载寄存器的新值。
  * @retval None
  */
void TIM_SetAutoreload(TIM_TypeDef* TIMx, uint16_t Autoreload)
{
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  /* 设置自动重装载寄存器值 */
  TIMx->ARR = Autoreload;
}

/**
  * @brief  设置 TIMx 捕获比较 1 寄存器值
  * @param  TIMx: x 可取 1 到 17（6 和 7 除外），用于选择 TIM 外设。
  * @param  Compare1: 指定捕获比较 1 寄存器的新值。
  * @retval None
  */
void TIM_SetCompare1(TIM_TypeDef* TIMx, uint16_t Compare1)
{
  /* 检查参数 */
  assert_param(IS_TIM_LIST8_PERIPH(TIMx));
  /* 设置捕获比较 1 寄存器值 */
  TIMx->CCR1 = Compare1;
}

/**
  * @brief  设置 TIMx 捕获比较 2 寄存器值
  * @param  TIMx: x 可取 1、2、3、4、5、8、9、12 或 15，用于选择 TIM 外设。
  * @param  Compare2: 指定捕获比较 2 寄存器的新值。
  * @retval None
  */
void TIM_SetCompare2(TIM_TypeDef* TIMx, uint16_t Compare2)
{
  /* 检查参数 */
  assert_param(IS_TIM_LIST6_PERIPH(TIMx));
  /* 设置捕获比较 2 寄存器值 */
  TIMx->CCR2 = Compare2;
}

/**
  * @brief  设置 TIMx 捕获比较 3 寄存器值
  * @param  TIMx: x 可取 1、2、3、4、5 或 8，用于选择 TIM 外设。
  * @param  Compare3: 指定捕获比较 3 寄存器的新值。
  * @retval None
  */
void TIM_SetCompare3(TIM_TypeDef* TIMx, uint16_t Compare3)
{
  /* 检查参数 */
  assert_param(IS_TIM_LIST3_PERIPH(TIMx));
  /* 设置捕获比较 3 寄存器值 */
  TIMx->CCR3 = Compare3;
}

/**
  * @brief  设置 TIMx 捕获比较 4 寄存器值
  * @param  TIMx: x 可取 1、2、3、4、5 或 8，用于选择 TIM 外设。
  * @param  Compare4: 指定捕获比较 4 寄存器的新值。
  * @retval None
  */
void TIM_SetCompare4(TIM_TypeDef* TIMx, uint16_t Compare4)
{
  /* 检查参数 */
  assert_param(IS_TIM_LIST3_PERIPH(TIMx));
  /* 设置捕获比较 4 寄存器值 */
  TIMx->CCR4 = Compare4;
}

/**
  * @brief  设置 TIMx 输入捕获 1 预分频器。
  * @param  TIMx: x 可取 1 到 17（6 和 7 除外），用于选择 TIM 外设。
  * @param  TIM_ICPSC: 指定输入捕获 1 预分频器的新值。
  *   该参数可取以下值之一：
  *     @arg TIM_ICPSC_DIV1: 无预分频
  *     @arg TIM_ICPSC_DIV2: 每 2 个事件捕获一次
  *     @arg TIM_ICPSC_DIV4: 每 4 个事件捕获一次
  *     @arg TIM_ICPSC_DIV8: 每 8 个事件捕获一次
  * @retval None
  */
void TIM_SetIC1Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC)
{
  /* 检查参数 */
  assert_param(IS_TIM_LIST8_PERIPH(TIMx));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));
  /* 清零 IC1PSC 位 */
  TIMx->CCMR1 &= (uint16_t)~((uint16_t)TIM_CCMR1_IC1PSC);
  /* 设置 IC1PSC 的值 */
  TIMx->CCMR1 |= TIM_ICPSC;
}

/**
  * @brief  设置 TIMx 输入捕获 2 的预分频器。
  * @param  TIMx: x 可取 1, 2, 3, 4, 5, 8, 9, 12 或 15 以选择 TIM 外设。
  * @param  TIM_ICPSC: 指定输入捕获 2 预分频器的新值。
  *   该参数可取以下值之一：
  *     @arg TIM_ICPSC_DIV1: 无预分频
  *     @arg TIM_ICPSC_DIV2: 每 2 个事件捕获一次
  *     @arg TIM_ICPSC_DIV4: 每 4 个事件捕获一次
  *     @arg TIM_ICPSC_DIV8: 每 8 个事件捕获一次
  * @retval 无
  */
void TIM_SetIC2Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC)
{
  /* 检查参数 */
  assert_param(IS_TIM_LIST6_PERIPH(TIMx));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));
  /* 清零 IC2PSC 位 */
  TIMx->CCMR1 &= (uint16_t)~((uint16_t)TIM_CCMR1_IC2PSC);
  /* 设置 IC2PSC 的值 */
  TIMx->CCMR1 |= (uint16_t)(TIM_ICPSC << 8);
}

/**
  * @brief  设置 TIMx 输入捕获 3 的预分频器。
  * @param  TIMx: x 可取 1, 2, 3, 4, 5 或 8 以选择 TIM 外设。
  * @param  TIM_ICPSC: 指定输入捕获 3 预分频器的新值。
  *   该参数可取以下值之一：
  *     @arg TIM_ICPSC_DIV1: 无预分频
  *     @arg TIM_ICPSC_DIV2: 每 2 个事件捕获一次
  *     @arg TIM_ICPSC_DIV4: 每 4 个事件捕获一次
  *     @arg TIM_ICPSC_DIV8: 每 8 个事件捕获一次
  * @retval 无
  */
void TIM_SetIC3Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC)
{
  /* 检查参数 */
  assert_param(IS_TIM_LIST3_PERIPH(TIMx));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));
  /* 清零 IC3PSC 位 */
  TIMx->CCMR2 &= (uint16_t)~((uint16_t)TIM_CCMR2_IC3PSC);
  /* 设置 IC3PSC 的值 */
  TIMx->CCMR2 |= TIM_ICPSC;
}

/**
  * @brief  设置 TIMx 输入捕获 4 的预分频器。
  * @param  TIMx: x 可取 1, 2, 3, 4, 5 或 8 以选择 TIM 外设。
  * @param  TIM_ICPSC: 指定输入捕获 4 预分频器的新值。
  *   该参数可取以下值之一：
  *     @arg TIM_ICPSC_DIV1: 无预分频
  *     @arg TIM_ICPSC_DIV2: 每 2 个事件捕获一次
  *     @arg TIM_ICPSC_DIV4: 每 4 个事件捕获一次
  *     @arg TIM_ICPSC_DIV8: 每 8 个事件捕获一次
  * @retval 无
  */
void TIM_SetIC4Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC)
{  
  /* 检查参数 */
  assert_param(IS_TIM_LIST3_PERIPH(TIMx));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));
  /* 清零 IC4PSC 位 */
  TIMx->CCMR2 &= (uint16_t)~((uint16_t)TIM_CCMR2_IC4PSC);
  /* 设置 IC4PSC 的值 */
  TIMx->CCMR2 |= (uint16_t)(TIM_ICPSC << 8);
}

/**
  * @brief  设置 TIMx 时钟分频值。
  * @param  TIMx: x 可取 1 到 17（6 和 7 除外）以选择
  *   TIM 外设。
  * @param  TIM_CKD: 指定时钟分频值。
  *   该参数可取以下值之一：
  *     @arg TIM_CKD_DIV1: TDTS = Tck_tim
  *     @arg TIM_CKD_DIV2: TDTS = 2*Tck_tim
  *     @arg TIM_CKD_DIV4: TDTS = 4*Tck_tim
  * @retval 无
  */
void TIM_SetClockDivision(TIM_TypeDef* TIMx, uint16_t TIM_CKD)
{
  /* 检查参数 */
  assert_param(IS_TIM_LIST8_PERIPH(TIMx));
  assert_param(IS_TIM_CKD_DIV(TIM_CKD));
  /* 清零 CKD 位 */
  TIMx->CR1 &= (uint16_t)~((uint16_t)TIM_CR1_CKD);
  /* 设置 CKD 的值 */
  TIMx->CR1 |= TIM_CKD;
}

/**
  * @brief  获取 TIMx 输入捕获 1 的值。
  * @param  TIMx: x 可取 1 到 17（6 和 7 除外）以选择 TIM 外设。
  * @retval 捕获比较寄存器 1 的值。
  */
uint16_t TIM_GetCapture1(TIM_TypeDef* TIMx)
{
  /* 检查参数 */
  assert_param(IS_TIM_LIST8_PERIPH(TIMx));
  /* 获取捕获寄存器 1 的值 */
  return TIMx->CCR1;
}

/**
  * @brief  获取 TIMx 输入捕获 2 的值。
  * @param  TIMx: x 可取 1, 2, 3, 4, 5, 8, 9, 12 或 15 以选择 TIM 外设。
  * @retval 捕获比较寄存器 2 的值。
  */
uint16_t TIM_GetCapture2(TIM_TypeDef* TIMx)
{
  /* 检查参数 */
  assert_param(IS_TIM_LIST6_PERIPH(TIMx));
  /* 获取捕获寄存器 2 的值 */
  return TIMx->CCR2;
}

/**
  * @brief  获取 TIMx 输入捕获 3 的值。
  * @param  TIMx: x 可取 1, 2, 3, 4, 5 或 8 以选择 TIM 外设。
  * @retval 捕获比较寄存器 3 的值。
  */
uint16_t TIM_GetCapture3(TIM_TypeDef* TIMx)
{
  /* 检查参数 */
  assert_param(IS_TIM_LIST3_PERIPH(TIMx)); 
  /* 获取捕获寄存器 3 的值 */
  return TIMx->CCR3;
}

/**
  * @brief  获取 TIMx 输入捕获 4 的值。
  * @param  TIMx: x 可取 1, 2, 3, 4, 5 或 8 以选择 TIM 外设。
  * @retval 捕获比较寄存器 4 的值。
  */
uint16_t TIM_GetCapture4(TIM_TypeDef* TIMx)
{
  /* 检查参数 */
  assert_param(IS_TIM_LIST3_PERIPH(TIMx));
  /* 获取捕获寄存器 4 的值 */
  return TIMx->CCR4;
}

/**
  * @brief  获取 TIMx 计数器的值。
  * @param  TIMx: x 可取 1 到 17 以选择 TIM 外设。
  * @retval 计数器寄存器的值。
  */
uint16_t TIM_GetCounter(TIM_TypeDef* TIMx)
{
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  /* 获取计数器寄存器的值 */
  return TIMx->CNT;
}

/**
  * @brief  获取 TIMx 预分频器的值。
  * @param  TIMx: x 可取 1 到 17 以选择 TIM 外设。
  * @retval 预分频器寄存器的值。
  */
uint16_t TIM_GetPrescaler(TIM_TypeDef* TIMx)
{
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  /* 获取预分频器寄存器的值 */
  return TIMx->PSC;
}

/**
  * @brief  检查指定的 TIM 标志是否置位。
  * @param  TIMx: x 可取 1 到 17 以选择 TIM 外设。
  * @param  TIM_FLAG: 指定要检查的标志。
  *   该参数可取以下值之一：
  *     @arg TIM_FLAG_Update: TIM 更新标志
  *     @arg TIM_FLAG_CC1: TIM 捕获比较 1 标志
  *     @arg TIM_FLAG_CC2: TIM 捕获比较 2 标志
  *     @arg TIM_FLAG_CC3: TIM 捕获比较 3 标志
  *     @arg TIM_FLAG_CC4: TIM 捕获比较 4 标志
  *     @arg TIM_FLAG_COM: TIM 换向标志
  *     @arg TIM_FLAG_Trigger: TIM 触发标志
  *     @arg TIM_FLAG_Break: TIM 刹车标志
  *     @arg TIM_FLAG_CC1OF: TIM 捕获比较 1 过捕获标志
  *     @arg TIM_FLAG_CC2OF: TIM 捕获比较 2 过捕获标志
  *     @arg TIM_FLAG_CC3OF: TIM 捕获比较 3 过捕获标志
  *     @arg TIM_FLAG_CC4OF: TIM 捕获比较 4 过捕获标志
  * @note
  *   - TIM6 和 TIM7 只能有一个更新标志。
  *   - TIM9、TIM12 和 TIM15 只能有 TIM_FLAG_Update、TIM_FLAG_CC1、
  *      TIM_FLAG_CC2 或 TIM_FLAG_Trigger。
  *   - TIM10、TIM11、TIM13、TIM14、TIM16 和 TIM17 只能有 TIM_FLAG_Update 或 TIM_FLAG_CC1。
  *   - TIM_FLAG_Break 仅用于 TIM1、TIM8 和 TIM15。
  *   - TIM_FLAG_COM 仅用于 TIM1、TIM8、TIM15、TIM16 和 TIM17。
  * @retval TIM_FLAG 的新状态（SET 或 RESET）。
  */
FlagStatus TIM_GetFlagStatus(TIM_TypeDef* TIMx, uint16_t TIM_FLAG)
{ 
  ITStatus bitstatus = RESET;  
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_GET_FLAG(TIM_FLAG));
  
  if ((TIMx->SR & TIM_FLAG) != (uint16_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

/**
  * @brief  清零 TIMx 的挂起标志。
  * @param  TIMx: x 可取 1 到 17 以选择 TIM 外设。
  * @param  TIM_FLAG: 指定要清零的标志位。
  *   该参数可为以下值的任意组合：
  *     @arg TIM_FLAG_Update: TIM 更新标志
  *     @arg TIM_FLAG_CC1: TIM 捕获比较 1 标志
  *     @arg TIM_FLAG_CC2: TIM 捕获比较 2 标志
  *     @arg TIM_FLAG_CC3: TIM 捕获比较 3 标志
  *     @arg TIM_FLAG_CC4: TIM 捕获比较 4 标志
  *     @arg TIM_FLAG_COM: TIM 换向标志
  *     @arg TIM_FLAG_Trigger: TIM 触发标志
  *     @arg TIM_FLAG_Break: TIM 刹车标志
  *     @arg TIM_FLAG_CC1OF: TIM 捕获比较 1 过捕获标志
  *     @arg TIM_FLAG_CC2OF: TIM 捕获比较 2 过捕获标志
  *     @arg TIM_FLAG_CC3OF: TIM 捕获比较 3 过捕获标志
  *     @arg TIM_FLAG_CC4OF: TIM 捕获比较 4 过捕获标志
  * @note
  *   - TIM6 和 TIM7 只能有一个更新标志。
  *   - TIM9、TIM12 和 TIM15 只能有 TIM_FLAG_Update、TIM_FLAG_CC1、
  *      TIM_FLAG_CC2 或 TIM_FLAG_Trigger。
  *   - TIM10、TIM11、TIM13、TIM14、TIM16 和 TIM17 只能有 TIM_FLAG_Update 或 TIM_FLAG_CC1。
  *   - TIM_FLAG_Break 仅用于 TIM1、TIM8 和 TIM15。
  *   - TIM_FLAG_COM 仅用于 TIM1、TIM8、TIM15、TIM16 和 TIM17。
  * @retval 无
  */
void TIM_ClearFlag(TIM_TypeDef* TIMx, uint16_t TIM_FLAG)
{  
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_CLEAR_FLAG(TIM_FLAG));
   
  /* 清除标志 */
  TIMx->SR = (uint16_t)~TIM_FLAG;
}

/**
  * @brief  检查 TIM 中断是否发生。
  * @param  TIMx: x 可取 1 到 17 以选择 TIM 外设。
  * @param  TIM_IT: 指定要检查的 TIM 中断源。
  *   该参数可取以下值之一：
  *     @arg TIM_IT_Update: TIM 更新中断源
  *     @arg TIM_IT_CC1: TIM 捕获比较 1 中断源
  *     @arg TIM_IT_CC2: TIM 捕获比较 2 中断源
  *     @arg TIM_IT_CC3: TIM 捕获比较 3 中断源
  *     @arg TIM_IT_CC4: TIM 捕获比较 4 中断源
  *     @arg TIM_IT_COM: TIM 换向中断源
  *     @arg TIM_IT_Trigger: TIM 触发中断源
  *     @arg TIM_IT_Break: TIM 刹车中断源
  * @note
  *   - TIM6 和 TIM7 只能产生更新中断。
  *   - TIM9、TIM12 和 TIM15 只能有 TIM_IT_Update、TIM_IT_CC1、
  *      TIM_IT_CC2 或 TIM_IT_Trigger。
  *   - TIM10、TIM11、TIM13、TIM14、TIM16 和 TIM17 只能有 TIM_IT_Update 或 TIM_IT_CC1。
  *   - TIM_IT_Break 仅用于 TIM1、TIM8 和 TIM15。
  *   - TIM_IT_COM 仅用于 TIM1、TIM8、TIM15、TIM16 和 TIM17。
  * @retval TIM_IT 的新状态（SET 或 RESET）。
  */
ITStatus TIM_GetITStatus(TIM_TypeDef* TIMx, uint16_t TIM_IT)
{
  ITStatus bitstatus = RESET;  
  uint16_t itstatus = 0x0, itenable = 0x0;
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_GET_IT(TIM_IT));
   
  itstatus = TIMx->SR & TIM_IT;
  
  itenable = TIMx->DIER & TIM_IT;
  if ((itstatus != (uint16_t)RESET) && (itenable != (uint16_t)RESET))
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

/**
  * @brief  清零 TIMx 的中断挂起位。
  * @param  TIMx: x 可取 1 到 17 以选择 TIM 外设。
  * @param  TIM_IT: 指定要清零的挂起位。
  *   该参数可为以下值的任意组合：
  *     @arg TIM_IT_Update: TIM1 更新中断源
  *     @arg TIM_IT_CC1: TIM 捕获比较 1 中断源
  *     @arg TIM_IT_CC2: TIM 捕获比较 2 中断源
  *     @arg TIM_IT_CC3: TIM 捕获比较 3 中断源
  *     @arg TIM_IT_CC4: TIM 捕获比较 4 中断源
  *     @arg TIM_IT_COM: TIM 换向中断源
  *     @arg TIM_IT_Trigger: TIM 触发中断源
  *     @arg TIM_IT_Break: TIM 刹车中断源
  * @note
  *   - TIM6 和 TIM7 只能产生更新中断。
  *   - TIM9、TIM12 和 TIM15 只能有 TIM_IT_Update、TIM_IT_CC1、
  *      TIM_IT_CC2 或 TIM_IT_Trigger。
  *   - TIM10、TIM11、TIM13、TIM14、TIM16 和 TIM17 只能有 TIM_IT_Update 或 TIM_IT_CC1。
  *   - TIM_IT_Break 仅用于 TIM1、TIM8 和 TIM15。
  *   - TIM_IT_COM 仅用于 TIM1、TIM8、TIM15、TIM16 和 TIM17。
  * @retval 无
  */
void TIM_ClearITPendingBit(TIM_TypeDef* TIMx, uint16_t TIM_IT)
{
  /* 检查参数 */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_IT(TIM_IT));
  /* 清零中断挂起位 */
  TIMx->SR = (uint16_t)~TIM_IT;
}

/**
  * @brief  将 TI1 配置为输入。
  * @param  TIMx: x 可取 1 到 17（6 和 7 除外）以选择 TIM 外设。
  * @param  TIM_ICPolarity : 输入极性。
  *   该参数可取以下值之一：
  *     @arg TIM_ICPolarity_Rising
  *     @arg TIM_ICPolarity_Falling
  * @param  TIM_ICSelection: 指定要使用的输入。
  *   该参数可取以下值之一：
  *     @arg TIM_ICSelection_DirectTI: 选择 TIM 输入 1 连接到 IC1。
  *     @arg TIM_ICSelection_IndirectTI: 选择 TIM 输入 1 连接到 IC2。
  *     @arg TIM_ICSelection_TRC: 选择 TIM 输入 1 连接到 TRC。
  * @param  TIM_ICFilter: 指定输入捕获滤波器。
  *   该参数必须为 0x00 到 0x0F 之间的值。
  * @retval 无
  */
static void TI1_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter)
{
  uint16_t tmpccmr1 = 0, tmpccer = 0;
  /* 关闭通道 1：清零 CC1E 位 */
  TIMx->CCER &= (uint16_t)~((uint16_t)TIM_CCER_CC1E);
  tmpccmr1 = TIMx->CCMR1;
  tmpccer = TIMx->CCER;
  /* 选择输入并设置滤波器 */
  tmpccmr1 &= (uint16_t)(((uint16_t)~((uint16_t)TIM_CCMR1_CC1S)) & ((uint16_t)~((uint16_t)TIM_CCMR1_IC1F)));
  tmpccmr1 |= (uint16_t)(TIM_ICSelection | (uint16_t)(TIM_ICFilter << (uint16_t)4));
  
  if((TIMx == TIM1) || (TIMx == TIM8) || (TIMx == TIM2) || (TIMx == TIM3) ||
     (TIMx == TIM4) ||(TIMx == TIM5))
  {
    /* 选择极性并置位 CC1E 位 */
    tmpccer &= (uint16_t)~((uint16_t)(TIM_CCER_CC1P));
    tmpccer |= (uint16_t)(TIM_ICPolarity | (uint16_t)TIM_CCER_CC1E);
  }
  else
  {
    /* 选择极性并置位 CC1E 位 */
    tmpccer &= (uint16_t)~((uint16_t)(TIM_CCER_CC1P | TIM_CCER_CC1NP));
    tmpccer |= (uint16_t)(TIM_ICPolarity | (uint16_t)TIM_CCER_CC1E);
  }

  /* 写入 TIMx CCMR1 和 CCER 寄存器 */
  TIMx->CCMR1 = tmpccmr1;
  TIMx->CCER = tmpccer;
}

/**
  * @brief  将 TI2 配置为输入。
  * @param  TIMx: x 可取 1, 2, 3, 4, 5, 8, 9, 12 或 15 以选择 TIM 外设。
  * @param  TIM_ICPolarity : 输入极性。
  *   该参数可取以下值之一：
  *     @arg TIM_ICPolarity_Rising
  *     @arg TIM_ICPolarity_Falling
  * @param  TIM_ICSelection: 指定要使用的输入。
  *   该参数可取以下值之一：
  *     @arg TIM_ICSelection_DirectTI: 选择 TIM 输入 2 连接到 IC2。
  *     @arg TIM_ICSelection_IndirectTI: 选择 TIM 输入 2 连接到 IC1。
  *     @arg TIM_ICSelection_TRC: 选择 TIM 输入 2 连接到 TRC。
  * @param  TIM_ICFilter: 指定输入捕获滤波器。
  *   该参数必须为 0x00 到 0x0F 之间的值。
  * @retval 无
  */
static void TI2_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter)
{
  uint16_t tmpccmr1 = 0, tmpccer = 0, tmp = 0;
  /* 关闭通道 2：清零 CC2E 位 */
  TIMx->CCER &= (uint16_t)~((uint16_t)TIM_CCER_CC2E);
  tmpccmr1 = TIMx->CCMR1;
  tmpccer = TIMx->CCER;
  tmp = (uint16_t)(TIM_ICPolarity << 4);
  /* 选择输入并设置滤波器 */
  tmpccmr1 &= (uint16_t)(((uint16_t)~((uint16_t)TIM_CCMR1_CC2S)) & ((uint16_t)~((uint16_t)TIM_CCMR1_IC2F)));
  tmpccmr1 |= (uint16_t)(TIM_ICFilter << 12);
  tmpccmr1 |= (uint16_t)(TIM_ICSelection << 8);
  
  if((TIMx == TIM1) || (TIMx == TIM8) || (TIMx == TIM2) || (TIMx == TIM3) ||
     (TIMx == TIM4) ||(TIMx == TIM5))
  {
    /* 选择极性并置位 CC2E 位 */
    tmpccer &= (uint16_t)~((uint16_t)(TIM_CCER_CC2P));
    tmpccer |=  (uint16_t)(tmp | (uint16_t)TIM_CCER_CC2E);
  }
  else
  {
    /* 选择极性并置位 CC2E 位 */
    tmpccer &= (uint16_t)~((uint16_t)(TIM_CCER_CC2P | TIM_CCER_CC2NP));
    tmpccer |= (uint16_t)(TIM_ICPolarity | (uint16_t)TIM_CCER_CC2E);
  }
  
  /* 写入 TIMx CCMR1 和 CCER 寄存器 */
  TIMx->CCMR1 = tmpccmr1 ;
  TIMx->CCER = tmpccer;
}

/**
  * @brief  将 TI3 配置为输入。
  * @param  TIMx: x 可取 1, 2, 3, 4, 5 或 8 以选择 TIM 外设。
  * @param  TIM_ICPolarity : 输入极性。
  *   该参数可取以下值之一：
  *     @arg TIM_ICPolarity_Rising
  *     @arg TIM_ICPolarity_Falling
  * @param  TIM_ICSelection: 指定要使用的输入。
  *   该参数可取以下值之一：
  *     @arg TIM_ICSelection_DirectTI: 选择 TIM 输入 3 连接到 IC3。
  *     @arg TIM_ICSelection_IndirectTI: 选择 TIM 输入 3 连接到 IC4。
  *     @arg TIM_ICSelection_TRC: 选择 TIM 输入 3 连接到 TRC。
  * @param  TIM_ICFilter: 指定输入捕获滤波器。
  *   该参数必须为 0x00 到 0x0F 之间的值。
  * @retval 无
  */
static void TI3_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter)
{
  uint16_t tmpccmr2 = 0, tmpccer = 0, tmp = 0;
  /* 关闭通道 3：清零 CC3E 位 */
  TIMx->CCER &= (uint16_t)~((uint16_t)TIM_CCER_CC3E);
  tmpccmr2 = TIMx->CCMR2;
  tmpccer = TIMx->CCER;
  tmp = (uint16_t)(TIM_ICPolarity << 8);
  /* 选择输入并设置滤波器 */
  tmpccmr2 &= (uint16_t)(((uint16_t)~((uint16_t)TIM_CCMR2_CC3S)) & ((uint16_t)~((uint16_t)TIM_CCMR2_IC3F)));
  tmpccmr2 |= (uint16_t)(TIM_ICSelection | (uint16_t)(TIM_ICFilter << (uint16_t)4));
    
  if((TIMx == TIM1) || (TIMx == TIM8) || (TIMx == TIM2) || (TIMx == TIM3) ||
     (TIMx == TIM4) ||(TIMx == TIM5))
  {
    /* 选择极性并置位 CC3E 位 */
    tmpccer &= (uint16_t)~((uint16_t)(TIM_CCER_CC3P));
    tmpccer |= (uint16_t)(tmp | (uint16_t)TIM_CCER_CC3E);
  }
  else
  {
    /* 选择极性并置位 CC3E 位 */
    tmpccer &= (uint16_t)~((uint16_t)(TIM_CCER_CC3P | TIM_CCER_CC3NP));
    tmpccer |= (uint16_t)(TIM_ICPolarity | (uint16_t)TIM_CCER_CC3E);
  }
  
  /* 写入 TIMx CCMR2 和 CCER 寄存器 */
  TIMx->CCMR2 = tmpccmr2;
  TIMx->CCER = tmpccer;
}

/**
  * @brief  将 TI4 配置为输入。
  * @param  TIMx: x 可取 1, 2, 3, 4, 5 或 8 以选择 TIM 外设。
  * @param  TIM_ICPolarity : 输入极性。
  *   该参数可取以下值之一：
  *     @arg TIM_ICPolarity_Rising
  *     @arg TIM_ICPolarity_Falling
  * @param  TIM_ICSelection: 指定要使用的输入。
  *   该参数可取以下值之一：
  *     @arg TIM_ICSelection_DirectTI: 选择 TIM 输入 4 连接到 IC4。
  *     @arg TIM_ICSelection_IndirectTI: 选择 TIM 输入 4 连接到 IC3。
  *     @arg TIM_ICSelection_TRC: 选择 TIM 输入 4 连接到 TRC。
  * @param  TIM_ICFilter: 指定输入捕获滤波器。
  *   该参数必须为 0x00 到 0x0F 之间的值。
  * @retval 无
  */
static void TI4_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter)
{
  uint16_t tmpccmr2 = 0, tmpccer = 0, tmp = 0;

   /* 关闭通道 4：清零 CC4E 位 */
  TIMx->CCER &= (uint16_t)~((uint16_t)TIM_CCER_CC4E);
  tmpccmr2 = TIMx->CCMR2;
  tmpccer = TIMx->CCER;
  tmp = (uint16_t)(TIM_ICPolarity << 12);
  /* 选择输入并设置滤波器 */
  tmpccmr2 &= (uint16_t)((uint16_t)(~(uint16_t)TIM_CCMR2_CC4S) & ((uint16_t)~((uint16_t)TIM_CCMR2_IC4F)));
  tmpccmr2 |= (uint16_t)(TIM_ICSelection << 8);
  tmpccmr2 |= (uint16_t)(TIM_ICFilter << 12);
  
  if((TIMx == TIM1) || (TIMx == TIM8) || (TIMx == TIM2) || (TIMx == TIM3) ||
     (TIMx == TIM4) ||(TIMx == TIM5))
  {
    /* 选择极性并置位 CC4E 位 */
    tmpccer &= (uint16_t)~((uint16_t)(TIM_CCER_CC4P));
    tmpccer |= (uint16_t)(tmp | (uint16_t)TIM_CCER_CC4E);
  }
  else
  {
    /* 选择极性并置位 CC4E 位 */
    tmpccer &= (uint16_t)~((uint16_t)(TIM_CCER_CC3P | TIM_CCER_CC4NP));
    tmpccer |= (uint16_t)(TIM_ICPolarity | (uint16_t)TIM_CCER_CC4E);
  }
  /* 写入 TIMx CCMR2 和 CCER 寄存器 */
  TIMx->CCMR2 = tmpccmr2;
  TIMx->CCER = tmpccer;
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
