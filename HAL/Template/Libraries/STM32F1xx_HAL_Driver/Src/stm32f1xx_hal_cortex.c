/**
  ******************************************************************************
  * @file    stm32f1xx_hal_cortex.c
  * @author  MCD Application Team
  * @brief   HAL库 CORTEX 模块驱动
  *          本文件提供了管理CORTEX以下功能的固件函数：
  *           + 初始化和反初始化函数
  *           + 外设控制函数
  *
  @verbatim  
  ==============================================================================
                        ##### 如何使用驱动 #####
  ==============================================================================

    [..]  
    *** 如何使用CORTEX HAL驱动程序配置中断 ***
    ===========================================================
    [..]     
    本节提供了函数，允许配置NVIC中断（IRQ）。
    Cortex-M3异常由CMSIS函数管理。
   
    (#) 使用 HAL_NVIC_SetPriorityGrouping() 函数按下表格配置NVIC优先级分组。
    (#) 使用 HAL_NVIC_SetPriority() 配置所选IRQ通道的优先级。
    (#) 使用 HAL_NVIC_EnableIRQ() 启用所选的IRQ通道。
    (#) 有关如何配置优先级的详细信息，请参阅编程手册。
      
     -@- 当选择 NVIC_PRIORITYGROUP_0 时，不允许IRQ（中断）抢占。待处理的IRQ优先级将只由子优先级管理。
   
     -@- IRQ优先级顺序(从最优先到最不优先排序):
        (+@) 最低抢占优先级
        (+@) 最低子优先级
        (+@) 最低硬件优先级(IRQ号)
 
    [..]  
    *** 如何使用CORTEX HAL驱动配置系统滴答 ***
    ========================================================
    [..]
    设置SysTick定时器的时间基础。
           
   (+) HAL_SYSTICK_Config() 函数调用 SysTick_Config() 函数，它是一个CMSIS函数:
        (++) 配置SysTick重新加载寄存器，并将值作为函数参数传递。
        (++) 配置SysTick的IRQ优先级为最低的0x0F。
        (++) 重置SysTick计数器寄存器。
        (++) 配置“SysTick Counter”时钟源为“核心时钟源(HCLK)”。
        (++) 开启 SysTick 中断.
        (++) 启动 SysTick 计数.
    
   (+) 您可以通过调用宏__HAL_CORTEX_SYSTICKCLK_CONFIG(SYSTICK_CLKSOURCE_HCLK_DIV8) 
       在HAL_SYSTICK_Config()函数调用之后将SysTick时钟源更改为HCLK_Div8。
        __HAL_CORTEX_SYSTICKCLK_CONFIG()宏在stm32f1xx_hal_cortex.h文件中定义。

   (+) 你可以通过在调用 HAL_SYSTICK_Config() 函数之后调用 HAL_NVIC_SetPriority(SysTick_IRQn，…) 函数来改变SysTick IRQ的优先级。
       HAL_NVIC_SetPriority() 调用了 NVIC_SetPriority() 函数，这是一个CMSIS函数。

   (+) SysTick时间基准的调整方法如下:
                            
       装载值 = 滴答计数频率 (Hz) x  期望基础时间 (s)
       (++) 重载值是传递给HAL_SYSTICK_Config()函数的参数
       (++) 重载值不应超过0xFFFFFF
   
  @endverbatim
  ******************************************************************************
  * @attention
  *
  * 版权所有 (c) 2017 STMicroelectronics。
  * 保留所有权利。
  *
  * 本软件根据可以在此软件组件的根目录中找到的 LICENSE 文件中的条款进行许可。
  * 如果此软件没有附带 LICENSE 文件，则按原样提供。
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @defgroup CORTEX CORTEX
  * @brief CORTEX HAL 模块驱动
  * @{
  */

#ifdef HAL_CORTEX_MODULE_ENABLED

/* 私有类型 -------------------------------------------------------------*/
/* 私有变量 ---------------------------------------------------------*/
/* 私有常量 ---------------------------------------------------------*/
/* 私有宏 ------------------------------------------------------------*/
/* 私有函数 ---------------------------------------------------------*/
/* 导出函数 --------------------------------------------------------*/

/** @defgroup CORTEX_Exported_Functions CORTEX 导出函数
  * @{
  */


/** @defgroup CORTEX_Exported_Functions_Group1 初始化和反初始化函数
  *  @brief    初始化和反初始化函数
  *
@verbatim    
  ==============================================================================
              ##### 初始化和反初始化函数 #####
  ==============================================================================
    [..]
      本节提供了CORTEX HAL驱动程序功能，允许配置中断和Systick功能。

@endverbatim
  * @{
  */


/**
  * @brief  设置优先级分组（使用所需的解锁顺序设置优先级分组字段(抢占优先级和子优先级)）
  * @param  PriorityGroup: 优先级分组位长度。
  *         该参数可以是以下值之一:
  *         @arg NVIC_PRIORITYGROUP_0: 0 位作为抢占优先级
  *                                    4 位作为子优先级
  *         @arg NVIC_PRIORITYGROUP_1: 1 位作为抢占优先级
  *                                    3 位作为子优先级
  *         @arg NVIC_PRIORITYGROUP_2: 2 该参数可以是以下值之一:
  *                                    2 位作为子优先级
  *         @arg NVIC_PRIORITYGROUP_3: 3 该参数可以是以下值之一:
  *                                    1 位作为子优先级
  *         @arg NVIC_PRIORITYGROUP_4: 4 该参数可以是以下值之一:
  *                                    0 位作为子优先级
  * @note   当选择NVIC_PriorityGroup_0时，不允许中断抢占，中断顺序只由子优先级判断
  * @retval None
  */
void HAL_NVIC_SetPriorityGrouping(uint32_t PriorityGroup)
{
  /* 检查参数 */
  assert_param(IS_NVIC_PRIORITY_GROUP(PriorityGroup));
  
  /* 根据 PriorityGroup 的值设置 PRIGROUP[10:8] 位 */
  NVIC_SetPriorityGrouping(PriorityGroup);
}

/**
  * @brief  设置中断优先级
  * @param  IRQn: 外部中断号 
  *         该参数可以是IRQn_Type枚举类型的枚举数
  *         (有关完整的STM32器件IRQ通道列表，请参阅相应的CMSIS器件文件(stm32f10xx.h))
  * @param  PreemptPriority: IRQn通道的抢占优先级。
  *         取值范围为0 ~ 15，优先级值越低表示优先级越高
  * @param  SubPriority: IRQ通道的子优先级。
  *         取值范围为0 ~ 15，优先级值越低表示优先级越高        
  * @retval None
  */
void HAL_NVIC_SetPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority)
{ 
  uint32_t prioritygroup = 0x00U;
  
  /* 检查参数 */
  assert_param(IS_NVIC_SUB_PRIORITY(SubPriority));
  assert_param(IS_NVIC_PREEMPTION_PRIORITY(PreemptPriority));
  
  prioritygroup = NVIC_GetPriorityGrouping();
  
  NVIC_SetPriority(IRQn, NVIC_EncodePriority(prioritygroup, PreemptPriority, SubPriority));
}

/**
  * @brief  在NVIC中断控制器中启用特定于设备的中断。
  * @note   为了正确配置中断优先级，应该在此之前调用NVIC_PriorityGroupConfig()函数。
  * @param  IRQn 外部中断号。
  *         该参数可以是IRQn_Type枚举类型的枚举值
  *         (有关完整的STM32器件IRQ通道列表，请参阅相应的CMSIS器件文件 (stm32f10xxx.h))
  * @retval None
  */
void HAL_NVIC_EnableIRQ(IRQn_Type IRQn)
{
  /* 检查参数 */
  assert_param(IS_NVIC_DEVICE_IRQ(IRQn));

  /* 开启中断 */
  NVIC_EnableIRQ(IRQn);
}

/**
  * @brief  在NVIC中断控制器中禁用特定于设备的中断。
  * @param  IRQn 外部中断号。
  *         该参数可以是IRQn_Type枚举类型的枚举值
  *         (有关完整的STM32器件IRQ通道列表，请参阅相应的CMSIS器件文件 (stm32f10xxx.h))  
  * @retval None
  */
void HAL_NVIC_DisableIRQ(IRQn_Type IRQn)
{
  /* 检查参数 */
  assert_param(IS_NVIC_DEVICE_IRQ(IRQn));

  /* 关闭中断 */
  NVIC_DisableIRQ(IRQn);
}

/**
  * @brief  发起系统复位请求，复位MCU。
  * @retval None
  */
void HAL_NVIC_SystemReset(void)
{
  /* 系统复位 */
  NVIC_SystemReset();
}

/**
  * @brief  系统滴答配置
  * @note   初始化系统定时器及其中断，并启动系统滴答计数。计数器处于自由运行模式，以产生周期性中断。
  * @param  TicksNumb: 指定两个中断之间的滴答数
  * @retval status:  - 0  成功.
  *                  - 1  失败.
  */
uint32_t HAL_SYSTICK_Config(uint32_t TicksNumb)
{
   return SysTick_Config(TicksNumb);
}
/**
  * @}
  */

/** @defgroup CORTEX_Exported_Functions_Group2 外设控制函数
  *  @brief   Cortex 控制函数
  *
@verbatim   
  ==============================================================================
                      ##### 外设控制函数 #####
  ==============================================================================
    [..]
      本小节提供了一组允许控制CORTEX (NVIC, SYSTICK, MPU)功能的函数。
 
      
@endverbatim
  * @{
  */

#if (__MPU_PRESENT == 1U)
/**
  * @brief  禁用 MPU
  * @retval None
  */
void HAL_MPU_Disable(void)
{
  /* 确保未完成的传输已经完成 */
  __DMB();

  /* 禁用故障异常 */
  SCB->SHCSR &= ~SCB_SHCSR_MEMFAULTENA_Msk;
  
  /* 禁用MPU并清除控制寄存器 */
  MPU->CTRL = 0U;
}

/**
  * @brief  启用 MPU.
  * @param  MPU_Control: 指定在硬件错误、NMI、FAULTMASK和访问默认内存时MPU的控制模式。
  *          该参数可以是以下值之一:
  *            @arg MPU_HFNMI_PRIVDEF_NONE  在硬错误或NMI时，MPU不强制任何特权或默认内存访问
  *            @arg MPU_HARDFAULT_NMI       在硬错误或NMI时，MPU强制特权和默认内存访问
  *            @arg MPU_PRIVILEGED_DEFAULT  在硬错误或NMI时，MPU强制默认内存访问，但不强制特权访问
  *            @arg MPU_HFNMI_PRIVDEF       在硬错误、NMI或异常退出时，MPU强制默认内存访问和特权访问
  * @retval None
  */
void HAL_MPU_Enable(uint32_t MPU_Control)
{
  /* 启用 MPU */
  MPU->CTRL = MPU_Control | MPU_CTRL_ENABLE_Msk;
  
  /* 启用故障异常 */
  SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;
  
  /* 确保MPU设置生效 */
  __DSB();
  __ISB();
}

/**
  * @brief  初始化和配置要保护的区域和内存
  * @param  MPU_Init: 指向包含初始化和配置信息的MPU_Region_InitTypeDef结构的指针
  * @retval None
  */
void HAL_MPU_ConfigRegion(MPU_Region_InitTypeDef *MPU_Init)
{
  /* 检查参数 */
  assert_param(IS_MPU_REGION_NUMBER(MPU_Init->Number));
  assert_param(IS_MPU_REGION_ENABLE(MPU_Init->Enable));

  /* 设置区域编号 */
  MPU->RNR = MPU_Init->Number;

  if ((MPU_Init->Enable) != RESET)
  {
    /* 检查参数 */
    assert_param(IS_MPU_INSTRUCTION_ACCESS(MPU_Init->DisableExec));
    assert_param(IS_MPU_REGION_PERMISSION_ATTRIBUTE(MPU_Init->AccessPermission));
    assert_param(IS_MPU_TEX_LEVEL(MPU_Init->TypeExtField));
    assert_param(IS_MPU_ACCESS_SHAREABLE(MPU_Init->IsShareable));
    assert_param(IS_MPU_ACCESS_CACHEABLE(MPU_Init->IsCacheable));
    assert_param(IS_MPU_ACCESS_BUFFERABLE(MPU_Init->IsBufferable));
    assert_param(IS_MPU_SUB_REGION_DISABLE(MPU_Init->SubRegionDisable));
    assert_param(IS_MPU_REGION_SIZE(MPU_Init->Size));
    
    MPU->RBAR = MPU_Init->BaseAddress;
    MPU->RASR = ((uint32_t)MPU_Init->DisableExec             << MPU_RASR_XN_Pos)   |
                ((uint32_t)MPU_Init->AccessPermission        << MPU_RASR_AP_Pos)   |
                ((uint32_t)MPU_Init->TypeExtField            << MPU_RASR_TEX_Pos)  |
                ((uint32_t)MPU_Init->IsShareable             << MPU_RASR_S_Pos)    |
                ((uint32_t)MPU_Init->IsCacheable             << MPU_RASR_C_Pos)    |
                ((uint32_t)MPU_Init->IsBufferable            << MPU_RASR_B_Pos)    |
                ((uint32_t)MPU_Init->SubRegionDisable        << MPU_RASR_SRD_Pos)  |
                ((uint32_t)MPU_Init->Size                    << MPU_RASR_SIZE_Pos) |
                ((uint32_t)MPU_Init->Enable                  << MPU_RASR_ENABLE_Pos);
  }
  else
  {
    MPU->RBAR = 0x00U;
    MPU->RASR = 0x00U;
  }
}
#endif /* __MPU_PRESENT */

/**
  * @brief  从NVIC中断控制器获取优先级分组字段
  * @retval 优先级分组字段 (SCB->AIRCR [10:8])
  */
uint32_t HAL_NVIC_GetPriorityGrouping(void)
{
  /*  获取 PRIGROUP[10:8] 字段值 */
  return NVIC_GetPriorityGrouping();
}

/**
  * @brief  获取中断的优先级
  * @param  IRQn: 外部中断号。
  *         该参数可以是IRQn_Type枚举类型的枚举值
  *         (有关完整的STM32器件IRQ通道列表，请参阅相应的CMSIS器件文件 (stm32f10xxx.h))
  * @param   PriorityGroup: 优先级组位长度。
  *         该参数可以是以下值之一:
  *           @arg NVIC_PRIORITYGROUP_0: 0 该参数可以是以下值之一:
  *                                      4 位作为子优先级
  *           @arg NVIC_PRIORITYGROUP_1: 1 该参数可以是以下值之一:
  *                                      3 位作为子优先级
  *           @arg NVIC_PRIORITYGROUP_2: 2 该参数可以是以下值之一:
  *                                      2 位作为子优先级
  *           @arg NVIC_PRIORITYGROUP_3: 3 该参数可以是以下值之一:
  *                                      1 位作为子优先级
  *           @arg NVIC_PRIORITYGROUP_4: 4 该参数可以是以下值之一:
  *                                      0 位作为子优先级
  * @param  pPreemptPriority: 存储抢占优先级指针(从0开始)
  * @param  pSubPriority: 存储子优先级值的指针(从0开始)
  * @retval None
  */
void HAL_NVIC_GetPriority(IRQn_Type IRQn, uint32_t PriorityGroup, uint32_t *pPreemptPriority, uint32_t *pSubPriority)
{
  /* 检查参数 */
  assert_param(IS_NVIC_PRIORITY_GROUP(PriorityGroup));
 /* 获得Cortex-M系统或设备特定中断的优先级 */
  NVIC_DecodePriority(NVIC_GetPriority(IRQn), PriorityGroup, pPreemptPriority, pSubPriority);
}

/**
  * @brief 设置外部中断的挂起位
  * @param  IRQn 外部中断号
  *         该参数可以是IRQn_Type枚举类型的枚举值
  *         (有关完整的STM32器件IRQ通道列表，请参阅相应的CMSIS器件文件 (stm32f10xxx.h))  
  * @retval None
  */
void HAL_NVIC_SetPendingIRQ(IRQn_Type IRQn)
{
  /* 检查参数 */
  assert_param(IS_NVIC_DEVICE_IRQ(IRQn));
  
  /* 设置中断挂起 */
  NVIC_SetPendingIRQ(IRQn);
}

/**
  * @brief  获取挂起的中断(读取NVIC中的挂起寄存器并返回指定中断的挂起位)
  * @param  IRQn 外部中断号。
  *         该参数可以是IRQn_Type枚举类型的枚举值
  *         (有关完整的STM32器件IRQ通道列表，请参阅相应的CMSIS器件文件 (stm32f10xxx.h))  
  * @retval status: - 0  中断未挂起
  *                 - 1  中断挂起
  */
uint32_t HAL_NVIC_GetPendingIRQ(IRQn_Type IRQn)
{
  /* 检查参数 */
  assert_param(IS_NVIC_DEVICE_IRQ(IRQn));

  /* 挂起返回 1 否则返回 0 */
  return NVIC_GetPendingIRQ(IRQn);
}

/**
  * @brief  清除外部中断的挂起位
  * @param  IRQn 外部中断号。
  *         该参数可以是IRQn_Type枚举类型的枚举值
  *         (有关完整的STM32器件IRQ通道列表，请参阅相应的CMSIS器件文件 (stm32f10xxx.h))  
  * @retval None
  */
void HAL_NVIC_ClearPendingIRQ(IRQn_Type IRQn)
{
  /* 检查参数 */
  assert_param(IS_NVIC_DEVICE_IRQ(IRQn));

  /* 清除挂起中断 */
  NVIC_ClearPendingIRQ(IRQn);
}

/**
  * @brief 获取激活中断(读取NVIC中的活动寄存器并返回活动位)
  * @param IRQn 外部中断号
  *         该参数可以是IRQn_Type枚举类型的枚举值
  *         (有关完整的STM32器件IRQ通道列表，请参阅相应的CMSIS器件文件 (stm32f10xxx.h))  
  * @retval status: - 0  中断状态未激活
  *                 - 1  中断状态激活
  */
uint32_t HAL_NVIC_GetActive(IRQn_Type IRQn)
{
  /* 检查参数 */
  assert_param(IS_NVIC_DEVICE_IRQ(IRQn));

  /* 激活返回 1 否则返回 0 */
  return NVIC_GetActive(IRQn);
}

/**
  * @brief  配置 SysTick 时钟源
  * @param  CLKSource: 指定 SysTick 时钟源
  *         该参数可以是以下值之一:
  *             @arg SYSTICK_CLKSOURCE_HCLK_DIV8: AHB 时钟（HCLK） 8分频作为 SysTick 的时钟源
  *             @arg SYSTICK_CLKSOURCE_HCLK: AHB 作为 SysTick 时钟源
  * @retval None
  */
void HAL_SYSTICK_CLKSourceConfig(uint32_t CLKSource)
{
  /* 检查参数 */
  assert_param(IS_SYSTICK_CLK_SOURCE(CLKSource));
  if (CLKSource == SYSTICK_CLKSOURCE_HCLK)
  {
    SysTick->CTRL |= SYSTICK_CLKSOURCE_HCLK;
  }
  else
  {
    SysTick->CTRL &= ~SYSTICK_CLKSOURCE_HCLK;
  }
}

/**
  * @brief  SYSTICK 中断请求处理函数
  * @retval None
  */
void HAL_SYSTICK_IRQHandler(void)
{
  HAL_SYSTICK_Callback();
}

/**
  * @brief  SYSTICK 中断回调.
  * @retval None
  */
__weak void HAL_SYSTICK_Callback(void)
{
  /* NOTE : 当需要回调函数时，不应修改此函数，可以在用户文件中重写以覆盖此函数。
   */
}

/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_CORTEX_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

 