/**
  ******************************************************************************
  * @file    stm32f1xx_hal_pwr.c
  * @author  MCD Application Team
  * @brief   HAL库 PWR 模块驱动
  *          该文件提供固件函数来管理功​​率控制器（PWR）外设的以下功​​能：
  *           + 初始化/反初始化函数
  *           + 外设控制函数
  *
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

/* 头文件 ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @defgroup PWR PWR
  * @brief    PWR HAL 模块驱动
  * @{
  */

#ifdef HAL_PWR_MODULE_ENABLED

/* 外设类型 -----------------------------------------------------------*/
/* 外设定义 ------------------------------------------------------------*/

/** @defgroup PWR_Private_Constants PWR 外设常量
  * @{
  */
  
/** @defgroup PWR_PVD_Mode_Mask PWR PVD（可编程电压检测器）模式掩码
  * @{
  */ 
#define PVD_MODE_IT               0x00010000U   /* PVD（可编程电压检测器）中断模式 */
#define PVD_MODE_EVT              0x00020000U   /* PVD（可编程电压检测器）事件模式 */
#define PVD_RISING_EDGE           0x00000001U   /* PVD（可编程电压检测器）上升沿 */
#define PVD_FALLING_EDGE          0x00000002U   /* PVD（可编程电压检测器）下降沿 */
/**
  * @}
  */


/** @defgroup PWR_register_alias_address PWR 寄存器别名地址
  * @{
  */ 
/* ------------- 别名区域中的PWR寄存器位地址 ---------------*/
#define PWR_OFFSET               (PWR_BASE - PERIPH_BASE)
#define PWR_CR_OFFSET            0x00U
#define PWR_CSR_OFFSET           0x04U
#define PWR_CR_OFFSET_BB         (PWR_OFFSET + PWR_CR_OFFSET)
#define PWR_CSR_OFFSET_BB        (PWR_OFFSET + PWR_CSR_OFFSET)
/**
  * @}
  */
   
/** @defgroup PWR_CR_register_alias PWR CR（电源控制寄存器）寄存器别名地址
  * @{
  */  
/* --- CR（电源控制）寄存器 ---*/
/* LPSDSR（深睡眠下的低功耗）位的别名地址 */
#define LPSDSR_BIT_NUMBER        PWR_CR_LPDS_Pos
#define CR_LPSDSR_BB             ((uint32_t)(PERIPH_BB_BASE + (PWR_CR_OFFSET_BB * 32U) + (LPSDSR_BIT_NUMBER * 4U)))

/* DBP（取消后备区域的写保护）位的别名地址 */
#define DBP_BIT_NUMBER            PWR_CR_DBP_Pos
#define CR_DBP_BB                ((uint32_t)(PERIPH_BB_BASE + (PWR_CR_OFFSET_BB * 32U) + (DBP_BIT_NUMBER * 4U)))

/* PVDE（电源电压监测器(PVD)使能）位的别名地址 */
#define PVDE_BIT_NUMBER           PWR_CR_PVDE_Pos
#define CR_PVDE_BB               ((uint32_t)(PERIPH_BB_BASE + (PWR_CR_OFFSET_BB * 32U) + (PVDE_BIT_NUMBER * 4U)))

/**
  * @}
  */

/** @defgroup PWR_CSR_register_alias PWR CSR（电源控制/状态寄存器）寄存器别名地址
  * @{
  */

/* --- CSR（电源控制/状态）寄存器 ---*/
/* EWUP1（使能WKUP引脚）位的别名地址 */
#define CSR_EWUP_BB(VAL)         ((uint32_t)(PERIPH_BB_BASE + (PWR_CSR_OFFSET_BB * 32U) + (POSITION_VAL(VAL) * 4U)))
/**
  * @}
  */
  
/**
  * @}
  */

/* 私有变量 ---------------------------------------------------------*/
/* 私有函数原型 -----------------------------------------------*/
/** @defgroup PWR_Private_Functions PWR 私有函数
 * brief   WFE cortex command overloaded for HAL_PWR_EnterSTOPMode 仅用于使用(请参阅解决方法一节)
 * @{
 */
static void PWR_OverloadWfe(void);

/* 私有函数 ---------------------------------------------------------*/
__NOINLINE
static void PWR_OverloadWfe(void)
{
  __asm volatile( "wfe" );
  __asm volatile( "nop" );
}

/**
  * @}
  */


/** @defgroup PWR_Exported_Functions PWR 导出函数
  * @{
  */

/** @defgroup PWR_Exported_Functions_Group1 初始化和反初始化函数
  *  @brief   初始化和反初始化函数
  *
@verbatim
 ===============================================================================
              ##### 初始化和反初始化函数 #####
 ===============================================================================
    [..]
      在复位后，备份域（RTC寄存器、RTC备份数据寄存器）受到保护，防止可能的意外写访问。
      要启用对RTC域和RTC寄存器的访问，请按照以下步骤进行：
        (+) 使用 __HAL_RCC_PWR_CLK_ENABLE() 宏启用电源控制器（PWR）APB1接口时钟。
        (+) 使用 HAL_PWR_EnableBkUpAccess() 函数启用对RTC域的访问。

@endverbatim
  * @{
  */

/**
  * @brief  反初始化PWR外设寄存器为默认复位值
  * @retval None
  */
void HAL_PWR_DeInit(void)
{
  __HAL_RCC_PWR_FORCE_RESET();
  __HAL_RCC_PWR_RELEASE_RESET();
}

/**
  * @brief  启用对备份域的访问（RTC寄存器、RTC备份数据寄存器）
  * @note   如果使用HSE除以128作为RTC时钟，则需要备份域访问功能保持开启。
  * @retval None
  */
void HAL_PWR_EnableBkUpAccess(void)
{
  /* 启用对RTC和备份寄存器的访问 */
  *(__IO uint32_t *) CR_DBP_BB = (uint32_t)ENABLE;
}

/**
  * @brief  禁用对备份域的访问 (RTC寄存器，RTC备份数据寄存器)
  * @note   如果使用HSE除以128作为RTC时钟，则需要备份域访问功能保持开启。
  * @retval None
  */
void HAL_PWR_DisableBkUpAccess(void)
{
  /* 禁用访问RTC和备份寄存器 */
  *(__IO uint32_t *) CR_DBP_BB = (uint32_t)DISABLE;
}

/**
  * @}
  */

/** @defgroup PWR_Exported_Functions_Group2 外设控制函数
  * @brief    低功耗模式配置函数
  *
@verbatim
 ===============================================================================
                 ##### 外设控制函数 #####
 ===============================================================================
     
    *** PVD（可编程电压检测器）配置 ***
    =========================
    [..]
      (+) PVD用于通过将其与PWR_CR中的PVD Level（PLS[2:0]位）比较来监视VDD电源供应的阈值。

      (+) PVDO标志可用于指示VDD/VDDA是高于还是低于PVD阈值。
          如果启用，此事件内部连接到EXTI线16，并且可以生成中断。
          这是通过 __HAL_PVD_EXTI_ENABLE_IT() 宏来实现的。
      (+) 在待机模式下，PVD被停止。

    *** 唤醒引脚配置 ***
    ================================
    [..]
      (+) 唤醒引脚用于将系统从待机模式唤醒，引脚强制为下拉输入模式，且激活上升沿检测
      (+) 唤醒引脚 1 为 PA.00.

    [..]

    *** 低功耗模式配置 ***
    =====================================
     [..]
      该设备具有3种低功耗模式：
      (+) 睡眠模式：CPU时钟关闭，包括NVIC、SysTick等Cortex-M3核心外设在内的所有外设继续运行。
      (+) 停止模式：所有时钟都停止。
      (+) 待机模式：1.8V域电源被关闭。
  
  
   *** 睡眠模式 ***
   ==================
    [..]
      (+) 进入：
          通过使用 HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFx) 函数进入睡眠模式：
          (++) PWR_SLEEPENTRY_WFI: 使用WFI指令进入睡眠模式
          (++) PWR_SLEEPENTRY_WFE: 使用WFE指令进入睡眠模式
     
      (+) 退出：
        (++) WFI进入模式，任何被嵌套的矢量中断控制器(NVIC)承认的外设中断都可以从休眠模式唤醒设备。
        (++) WFE进入模式，任何唤醒事件都可以将设备从休眠模式唤醒。
           (+++) 任何在NVIC配置且在Cortex中设置了SEVONPEND位的外设中断(HAL_PWR_EnableSEVOnPend)
           (+++) 任何以Event模式配置的EXTI线路(内部或外部)

   *** 停止模式 ***
   =================
    [..]
      停止模式是基于Cortex-M3深度睡眠模式结合外围时钟门控。稳压器可以配置在正常或低功率模式。
      在停止模式下，1.8 V域中的所有时钟都停止，锁相环、HSI和HSE RC振荡器被禁用。SRAM和寄存器内容被保留。
      在停止模式下，所有I/O引脚保持与运行模式下相同的状态。

      (+) 进入:
          停止模式通过 HAL_PWR_EnterSTOPMode(PWR_REGULATOR_VALUE, PWR_SLEEPENTRY_WFx) 函数进入:
          (++) PWR_REGULATOR_VALUE= PWR_MAINREGULATOR_ON:         主稳压器开启
          (++) PWR_REGULATOR_VALUE= PWR_LOWPOWERREGULATOR_ON: Low 低功耗稳压器开启
          (++) PWR_SLEEPENTRY_WFx= PWR_SLEEPENTRY_WFI:            使用WFI指令进入停止模式
          (++) PWR_SLEEPENTRY_WFx= PWR_SLEEPENTRY_WFE:            使用WFE指令进入停止模式
      (+) 退出:
          (++) WFI进入模式, 任何在NVIC中配置了中断模式的EXTI线(内部或外部)
          (++) WFE进入模式, 任何以Event模式配置的EXTI线(内部或外部)。

   *** 待机模式 ***
   ====================
     [..]
      待机模式可以实现最低的功耗。它基于Cortex-M3深度睡眠模式，禁用主稳压器，1.8 V域掉电。
      HSI振荡器和HSE振荡器也被关闭。SRAM和寄存器内容除了备份域中的寄存器和待机电路中的寄存器外都会丢失。
      
      (+) 进入:
        (++) 通过 HAL_PWR_EnterSTANDBYMode() 函数进入待机模式。
      (+) 退出:
        (++) 唤醒引脚上升沿，RTC闹钟事件上升沿，NRSTpin外部复位，IWDG复位

   *** 低功耗模式的自动唤醒（AWU） ***
       =============================================
       [..]
        
       (+) MCU可以通过RTC闹钟事件从低功耗模式唤醒，而不依赖于外部中断（自动唤醒模式）。
   
       (+) 从停止模式和待机模式的RTC自动唤醒（AWU）

           (++) 要从停止模式中的RTC闹钟事件唤醒，需要使用 HAL_RTC_SetAlarm_IT() 函数配置RTC以生成RTC闹钟。

   *** 与硅限制相关的PWR解决方案 ***
       ====================================================
       [..]
       以下是已知的STM32F1xx产品上的所有硅限制列表。

       (#)在PWR HAL驱动程序中实施的解决方案
          (##)使用WFE进入调试停止模式-通过内部函数过载WFE
        
@endverbatim
  * @{
  */

/**
  * @brief  配置PVD(电源电压检测器)检测的电压阈值
  * @param  sConfigPVD: 指向PWR_PVDTypeDef结构体的指针，该结构体包含PVD的配置信息。
  * @note   有关每个检测级别对应的电压阈值的详细信息，请参阅设备的电气特性数据表。
  * @retval None
  */
void HAL_PWR_ConfigPVD(PWR_PVDTypeDef *sConfigPVD)
{
  /* 检查参数 */
  assert_param(IS_PWR_PVD_LEVEL(sConfigPVD->PVDLevel));
  assert_param(IS_PWR_PVD_MODE(sConfigPVD->Mode));

  /* 根据PVDLevel值设置PLS[7:5]位 */
  MODIFY_REG(PWR->CR, PWR_CR_PLS, sConfigPVD->PVDLevel);
  
  /* 清除所有事件和中断配置 */
  __HAL_PWR_PVD_EXTI_DISABLE_EVENT();
  __HAL_PWR_PVD_EXTI_DISABLE_IT();
  __HAL_PWR_PVD_EXTI_DISABLE_FALLING_EDGE(); 
  __HAL_PWR_PVD_EXTI_DISABLE_RISING_EDGE();

  /* 配置中断模式 */
  if((sConfigPVD->Mode & PVD_MODE_IT) == PVD_MODE_IT)
  {
    __HAL_PWR_PVD_EXTI_ENABLE_IT();
  }
  
  /* 配置事件模式 */
  if((sConfigPVD->Mode & PVD_MODE_EVT) == PVD_MODE_EVT)
  {
    __HAL_PWR_PVD_EXTI_ENABLE_EVENT();
  }
  
  /* 配置检测沿 */
  if((sConfigPVD->Mode & PVD_RISING_EDGE) == PVD_RISING_EDGE)
  {
    __HAL_PWR_PVD_EXTI_ENABLE_RISING_EDGE();
  }
  
  if((sConfigPVD->Mode & PVD_FALLING_EDGE) == PVD_FALLING_EDGE)
  {
    __HAL_PWR_PVD_EXTI_ENABLE_FALLING_EDGE();
  }
}

/**
  * @brief  启用PVD(电源电压检测器)
  * @retval None
  */
void HAL_PWR_EnablePVD(void)
{
  /* 启用电源电压检测 */
  *(__IO uint32_t *) CR_PVDE_BB = (uint32_t)ENABLE;
}

/**
  * @brief  禁用PVD(电源电压检测器)
  * @retval None
  */
void HAL_PWR_DisablePVD(void)
{
  /* 禁用电源电压检测 */
  *(__IO uint32_t *) CR_PVDE_BB = (uint32_t)DISABLE;
}

/**
  * @brief 启用引脚唤醒功能
  * @param WakeUpPinx: 指定启用的唤醒引脚
  *        该参数可以是以下值之一：
  *           @arg PWR_WAKEUP_PIN1
  * @retval None
  */
void HAL_PWR_EnableWakeUpPin(uint32_t WakeUpPinx)
{
  /* 检查参数 */
  assert_param(IS_PWR_WAKEUP_PIN(WakeUpPinx));
  /* 启用唤醒引脚 */
  *(__IO uint32_t *) CSR_EWUP_BB(WakeUpPinx) = (uint32_t)ENABLE;
}

/**
  * @brief 禁用引脚唤醒功能
  * @param WakeUpPinx: 指定要禁用的唤醒引脚
  *        该参数可以是以下值之一：
  *           @arg PWR_WAKEUP_PIN1
  * @retval None
  */
void HAL_PWR_DisableWakeUpPin(uint32_t WakeUpPinx)
{
  /* 检查参数 */
  assert_param(IS_PWR_WAKEUP_PIN(WakeUpPinx));
  /* 启用唤醒引脚 */
  *(__IO uint32_t *) CSR_EWUP_BB(WakeUpPinx) = (uint32_t)DISABLE;
}

/**
  * @brief 进入睡眠模式
  * @note  在睡眠模式下，所有I/O引脚保持与运行模式下相同的状态。
  * @param Regulator: 在睡眠模式下，稳压器状态无效-支持从传统软件的可移植性
  * @param SLEEPEntry: 指定使用WFI或WFE指令进入SLEEP模式，
  *           当使用WFI进入休眠时，注意Systick中断会导致唤醒，如果不想要其唤醒需要在进入前关闭此中断。
  *           该参数可以是以下值之一：
  *            @arg PWR_SLEEPENTRY_WFI: 使用WFI命令进入睡眠模式
  *            @arg PWR_SLEEPENTRY_WFE: 使用WFE命令进入睡眠模式
  * @retval None
  */
void HAL_PWR_EnterSLEEPMode(uint32_t Regulator, uint8_t SLEEPEntry)
{
  /* 检查参数 */
  /* 没有检查稳压器，因为参数没有在SLEEP模式下使用 */
  /* 防止未使用参数的编译警告 */
  UNUSED(Regulator);

  assert_param(IS_PWR_SLEEP_ENTRY(SLEEPEntry));

  /* 清除Cortex系统控制寄存器的SLEEPDEEP位 */
  CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));

  /* 选择进入睡眠的方式 -------------------------------------------------*/
  if(SLEEPEntry == PWR_SLEEPENTRY_WFI)
  {
    /* 等待中断唤醒 */
    __WFI();
  }
  else
  {
    /* 等待事件唤醒 */
    __SEV();
    __WFE();
    __WFE();
  }
}

/**
  * @brief 进入停止模式
  * @note  在停止模式下，所有I/O引脚保持与运行模式下相同的状态。
  * @note  当使用中断或唤醒事件退出停止模式时，HSI RC振荡器被选择为系统时钟。
  * @note  当稳压器在低功耗模式下工作时，从停止模式唤醒时会产生额外的启动延迟。
  *        可以在停止模式中保持内部调节器开启，可以减少启动时间，但功耗更高。
  * @param Regulator: 指定在停止模式下的稳压器状态
  *          该参数可以是以下值之一：
  *            @arg PWR_MAINREGULATOR_ON:     停止模式，主稳压器开启
  *            @arg PWR_LOWPOWERREGULATOR_ON: 停止模式，低功耗稳压器开启
  * @param STOPEntry: 指定使用WFI或WFE指令进入STOP模式
  *          该参数可以是以下值之一：
  *            @arg PWR_STOPENTRY_WFI: 使用WFI指令进入停止模式
  *            @arg PWR_STOPENTRY_WFE: 使用WFE指令进入停止模式
  * @retval None
  */
void HAL_PWR_EnterSTOPMode(uint32_t Regulator, uint8_t STOPEntry)
{
  /* 检查参数 */
  assert_param(IS_PWR_REGULATOR(Regulator));
  assert_param(IS_PWR_STOP_ENTRY(STOPEntry));

  /* 清除PWR寄存器中的PDDS位以指定当CPU进入深度睡眠时进入停止模式 */ 
  CLEAR_BIT(PWR->CR,  PWR_CR_PDDS);

  /* 根据稳压参数值，通过在调节器寄存器中设置lds位来选择稳压模式 */
  MODIFY_REG(PWR->CR, PWR_CR_LPDS, Regulator);

  /* 设置Cortex系统控制寄存器的SLEEPDEEP位 */
  SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));

  /* 选择进入方式 --------------------------------------------------*/
  if(STOPEntry == PWR_STOPENTRY_WFI)
  {
    /* 等待中断唤醒 */
    __WFI();
  }
  else
  {
    /* 等待事件唤醒 */
    __SEV();
    PWR_OverloadWfe(); /* 在本地重新定义WFE */
    PWR_OverloadWfe(); /* 在本地重新定义WFE */
  }
  /* 重置Cortex系统控制寄存器的SLEEPDEEP位（这是唤醒之后的第一步，不是进入停止模式的最后一步） */
  CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
}

/**
  * @brief 进入待机模式
  * @note  待机模式下，除以下情况外，所有I/O引脚均为高阻抗:
  *          - 复位 (仍然可用)
  *          - 入侵检测引脚 如果配置为tamper或校准
  *          - 如果启用了WKUP引脚(PA0)
  * @retval None
  */
void HAL_PWR_EnterSTANDBYMode(void)
{
  /* 选择待机模式 */
  SET_BIT(PWR->CR, PWR_CR_PDDS);

  /* 设置Cortex系统控制寄存器的SLEEPDEEP位 */
  SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));

  /* 此选项用于确保完成存储操作 */
#if defined ( __CC_ARM)
  __force_stores();
#endif
  /* 等待中断唤醒，唤醒后程序复位运行 */
  __WFI();
}


/**
  * @brief 启用在退出时进入睡眠（当从中断处理程序模式返回到线程模式时进入休眠）。
  * @note 设置SCR寄存器的SLEEPONEXIT位。当设置此位时，处理器在中断处理结束后重新进入SLEEP模式。
  *       当期望处理器只在中断处理时运行时，设置这个位是有用的。 
  * @retval None
  */
void HAL_PWR_EnableSleepOnExit(void)
{
  /* 设置Cortex系统控制寄存器的SLEEPONEXIT位 */
  SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPONEXIT_Msk));
}


/**
  * @brief 禁用在退出时进入睡眠（当从中断处理程序模式返回到线程模式时进入休眠）。
  * @note 设置SCR寄存器的SLEEPONEXIT位。当设置此位时，处理器在中断处理结束后重新进入SLEEP模式。
  *       当期望处理器只在中断处理时运行时，设置这个位是有用的。          
  * @retval None
  */
void HAL_PWR_DisableSleepOnExit(void)
{
  /* 清除Cortex系统控制寄存器的SLEEPONEXIT位 */
  CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPONEXIT_Msk));
}


/**
  * @brief 启用 CORTEX M3 SEVONPEND 位
  * @note  设置SCR寄存器的 SEVONPEND 位，设置此位时，当中断从非活动变为挂起时，将导致WFE唤醒
  * @retval None
  */
void HAL_PWR_EnableSEVOnPend(void)
{
  /* 设置Cortex系统控制寄存器的SEVONPEND位 */
  SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SEVONPEND_Msk));
}


/**
  * @brief 禁用 CORTEX M3 SEVONPEND 位
  * @note  清除SCR寄存器的SEVONPEND位。当设置此位时，中断从非活动变为挂起时，将导致WFE唤醒。      
  * @retval None
  */
void HAL_PWR_DisableSEVOnPend(void)
{
  /* 清除Cortex系统控制寄存器的SEVONPEND位 */
  CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SEVONPEND_Msk));
}



/**
  * @brief  PWR PVD(电源电压检测器)中断请求处理函数
  * @note   这个API应该在 PVD_IRQHandler() 下调用
  * @retval None
  */
void HAL_PWR_PVD_IRQHandler(void)
{
  /* 检查 PWR EXTI 线标志 */
  if(__HAL_PWR_PVD_EXTI_GET_FLAG() != RESET)
  {
    /* PWR PVD 中断用户回调 */
    HAL_PWR_PVDCallback();

    /* 清除 PWR Exti 挂起位 */
    __HAL_PWR_PVD_EXTI_CLEAR_FLAG();
  }
}

/**
  * @brief  PWR PVD(电源电压检测器)中断回调
  * @retval None
  */
__weak void HAL_PWR_PVDCallback(void)
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

#endif /* HAL_PWR_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

