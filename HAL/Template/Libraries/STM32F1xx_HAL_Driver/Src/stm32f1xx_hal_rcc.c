/**
  ******************************************************************************
  * @file    stm32f1xx_hal_rcc.c
  * @author  MCD Application Team
  * @brief   HAL库 RCC 模块驱动
  *          该文件提供了固件函数，用于管理复位和时钟控制（RCC）外设的以下功能：
  *           + 初始化和反初始化函数
  *           + 外设控制函数
  *
  @verbatim
  ==============================================================================
                      ##### RCC 特性 #####
  ==============================================================================
    [..]
      复位后，设备使用内部高速振荡器（HSI 8MHz）运行，Flash为0等待周期，
      启用Flash预取缓冲区，除了内部SRAM、Flash和JTAG之外，所有外设都处于关闭状态。
      (+) 高速（AHB）和低速（APB）总线上没有预分频；映射到这些总线上的所有外设都以HSI速度运行。
      (+) 所有外设的时钟都被关闭，除了SRAM和FLASH。
      (+) 所有GPIO处于浮空输入状态，除了分配给调试目的的JTAG引脚。
    [..]  一旦设备从复位开始运行，用户应用程序必须：
      (+) 配置用于驱动系统时钟的时钟源（如果应用程序需要更高的频率/性能）
      (+) 配置系统时钟频率和Flash设置
      (+) 配置AHB和APB总线的预分频
      (+) 启用要使用的外设的时钟
      (+) 配置时钟源以用于那些时钟不是从系统时钟派生的外设（I2S、RTC、ADC、USB OTG FS）

                      ##### RCC 限制 #####
  ==============================================================================
    [..]
      在启用RCC外设时，应考虑RCC外设时钟启用和有效外设启用之间的延迟，以便管理对寄存器的外设读/写操作。
      (+) 此延迟取决于外设映射。
        (++) 对于AHB和APB外设，需要进行一次虚拟读取

    [..]
      解决方法:
      (#) 对于AHB和APB外设，在每个__HAL_RCC_PPP_CLK_ENABLE()宏中插入了对外设寄存器的虚拟读取。

  @endverbatim
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

/** @defgroup RCC RCC
* @brief RCC HAL 模块驱动
  * @{
  */

#ifdef HAL_RCC_MODULE_ENABLED

/* 私有类型 -----------------------------------------------------------*/
/* 私有定义 ------------------------------------------------------------*/
/** @defgroup RCC_Private_Constants RCC 私有常量
 * @{
 */
/**
  * @}
  */
/* 私有宏 -------------------------------------------------------------*/
/** @defgroup RCC_Private_Macros RCC 私有宏
  * @{
  */

#define MCO1_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define MCO1_GPIO_PORT        GPIOA
#define MCO1_PIN              GPIO_PIN_8

/**
  * @}
  */

/* 私有变量 ---------------------------------------------------------*/
/** @defgroup RCC_Private_Variables RCC 私有变量
  * @{
  */
/**
  * @}
  */

/* 私有函数原型 -----------------------------------------------*/

static void RCC_Delay(uint32_t mdelay);

/* 导出函数 --------------------------------------------------------*/

/** @defgroup RCC_Exported_Functions RCC 导出函数
  * @{
  */

/** @defgroup RCC_Exported_Functions_Group1 初始化和反初始化函数
  *  @brief    初始化和配置函数
  *
  @verbatim
  ===============================================================================
           ##### 初始化和反初始化函数 #####
  ===============================================================================
    [..]
      该部分提供了允许配置内部/外部振荡器（HSE、HSI、LSE、LSI、PLL、CSS和MCO）以及系统总线时钟（SYSCLK、AHB、APB1和APB2）的函数。

    [..] 内部/外部时钟和PLL配置
      (#) HSI (内部高速RC震荡器)，工厂修正的8MHz RC振荡器，直接或通过PLL作为系统时钟源
      (#) LSI (内部低速RC震荡器), 约40 KHz 低速RC震荡器，用于作为IWDG独立看门狗和RTC实时时钟的时钟源

      (#) HSE (外部高速振荡器), 4至24 MHz（STM32F100xx）或4至16 MHz（STM32F101x/STM32F102x/STM32F103x）或3至25 MHz（STM32F105x/STM32F107x）晶体振荡器，
          可以直接或通过PLL用作系统时钟源。 也可用作RTC时钟源。

      (#) LSE (外部低速震荡器), 32 KHz 振荡器用作 RTC实时时钟的时钟源

      (#) PLL (由HSI或HSE输入), 具有不同的输出时钟:
        (++) 第一个输出用于生成高速系统时钟（STM32F10xxx最高达72 MHz，STM32F100xx最高达24 MHz）
        (++) 第二个输出用于生成USB OTG FS的时钟（48 MHz）

      (#) CSS（时钟安全系统），一旦使用宏__HAL_RCC_CSS_ENABLE()启用，
          如果发生HSE时钟故障（HSE直接或通过PLL用作系统时钟源），
          系统时钟会自动切换到HSI，并在启用CSS的情况下生成中断。
          该中断与Cortex-M3 NMI（不可屏蔽中断）异常向量相关联。

      (#) MCO1（微控制器时钟输出），用于在PA8引脚上输出SYSCLK、HSI、HSE 或 PLL时钟（除以2）
            + STM32F105x/STM32F107x上的PLL2CLK、PLL3CLK/2、PLL3CLK和XTI


    [..] 系统、AHB和APB总线时钟配置
      (#) 可以使用多个时钟源来驱动系统时钟（SYSCLK）：HSI、HSE和PLL。
          AHB时钟（HCLK）通过可配置的预分频器从系统时钟派生，作为CPU、内存和映射到AHB总线上的外设（DMA、GPIO等）的时钟。
          APB1（PCLK1）和 APB2（PCLK2）时钟通过可配置的预分频器从AHB时钟派生，作为映射到这些总线上的外设的时钟。
          您可以使用"HAL_RCC_GetSysClockFreq()"函数来获取这些时钟的频率。

      -@- 所有外设时钟都是从系统时钟（SYSCLK）派生的，除了：
          (+@) RTC：RTC时钟可以从 LSI、LSE 或 HSE时钟除以128派生。
          (+@) USB OTG FS：USB OTG FS需要频率为 48 MHz 才能正常工作。此时钟是通过主PLL输出进行USB预分频后派生的。
          (+@) STM32F105x/STM32F107x上的I2S接口可以从PLL3CLK派生
          (+@) IWDG时钟始终为LSI时钟

      (#) 对于STM32F10xxx，SYSCLK 和 HCLK/PCLK2的最大频率为72 MHz，PCLK1为36 MHz。
          对于STM32F100xx，SYSCLK 和 HCLK/PCLK1/PCLK2的最大频率为24 MHz。
          根据SYSCLK频率，应相应调整Flash延迟。
  @endverbatim
  * @{
  */

/*
  关于基于FLASH延迟设置的SYSCLK的额外考虑：
        +-----------------------------------------------+
        |    延迟       |     SYSCLK 时钟频率 (MHz)      |
        |---------------|-------------------------------|
        |0WS(1CPU 周期) |       0 < SYSCLK <= 24        |
        |---------------|-------------------------------|
        |1WS(2CPU 周期) |      24 < SYSCLK <= 48        |
        |---------------|-------------------------------|
        |2WS(3CPU 周期) |      48 < SYSCLK <= 72        |
        +-----------------------------------------------+
  */

/**
  * @brief  重置RCC时钟配置为默认复位状态。
  * @note   时钟配置的默认复位状态如下:
  *            - HSI 开启作为系统时钟源
  *            - HSE, PLL, PLL2 和 PLL3 关闭
  *            - AHB, APB1 和 APB2 预分频为1
  *            - CSS 和 MCO1 关闭
  *            - 全部中断禁用
  *            - 全部标志清除
  * @note   该函数不修改的设备配置
  *            - 外设时钟源
  *            - LSI, LSE 和 RTC 时钟
  * @retval HAL_StatusTypeDef
  */
HAL_StatusTypeDef HAL_RCC_DeInit(void)
{
  uint32_t tickstart;

  /* 获取起始滴答值 */
  tickstart = HAL_GetTick();

  /* 设置 HSION（内部高速时钟使能）位 */
  SET_BIT(RCC->CR, RCC_CR_HSION);

  /* 等待 HSI 就绪 */
  while (READ_BIT(RCC->CR, RCC_CR_HSIRDY) == RESET)
  {
    if ((HAL_GetTick() - tickstart) > HSI_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

  /* 设置 HSITRIM（内部高速时钟调整）位为复位值（默认16） */
  MODIFY_REG(RCC->CR, RCC_CR_HSITRIM, (0x10U << RCC_CR_HSITRIM_Pos));

  /* 获取起始滴答值 */
  tickstart = HAL_GetTick();

  /* 重置 CFGR（时钟配置）寄存器 */
  CLEAR_REG(RCC->CFGR);

  /* 等待系统时钟切换完成，通过读CFGR（时钟配置寄存器）的SWS（系统时钟切换状态位）是否为HSI */
  while (READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RESET)
  {
    if ((HAL_GetTick() - tickstart) > CLOCKSWITCH_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

  /* 更新系统时钟频率到全局变量 */
  SystemCoreClock = HSI_VALUE;

  /* 重置系统时钟中断优先级 */
  if (HAL_InitTick(uwTickPrio) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* 获取起始滴答值 */
  tickstart = HAL_GetTick();

  /* 第二步 清除 PLLON（PLL使能）位 */
  CLEAR_BIT(RCC->CR, RCC_CR_PLLON);

  /* 等待PLL关闭 */
  while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) != RESET)
  {
    if ((HAL_GetTick() - tickstart) > PLL_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

  /* 确保PLLSRC（PLL输入时钟源）和PLLMUL（PLL倍频系数）位复位 */
  CLEAR_REG(RCC->CFGR);

  /* 获取起始滴答值 */
  tickstart = HAL_GetTick();

  /* 重置 HSEON（外部高速时钟使能）和 CSSON（时钟安全系统使能）位 */
  CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_CSSON);

  /* 等待 HSE 关闭 */
  while (READ_BIT(RCC->CR, RCC_CR_HSERDY) != RESET)
  {
    if ((HAL_GetTick() - tickstart) > HSE_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

  /* 重置 HSEBYP（外部高速时钟旁路）位 */
  CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);

#if defined(RCC_PLL2_SUPPORT)
  /* 获取起始滴答值 */
  tickstart = HAL_GetTick();

  /* 重置 PLL2ON（PLL2使能）位 */
  CLEAR_BIT(RCC->CR, RCC_CR_PLL2ON);

  /* 等待 PLL2 禁用 */
  while (READ_BIT(RCC->CR, RCC_CR_PLL2RDY) != RESET)
  {
    if ((HAL_GetTick() - tickstart) > PLL2_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }
#endif /* RCC_PLL2_SUPPORT */

#if defined(RCC_PLLI2S_SUPPORT)
  /* 获取起始滴答值 */
  tickstart = HAL_GetTick();

  /* 重置 PLL3ON（PLL3使能）位 */
  CLEAR_BIT(RCC->CR, RCC_CR_PLL3ON);

  /* 等大 PLL3 禁用 */
  while (READ_BIT(RCC->CR, RCC_CR_PLL3RDY) != RESET)
  {
    if ((HAL_GetTick() - tickstart) > PLLI2S_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }
#endif /* RCC_PLLI2S_SUPPORT */

#if defined(RCC_CFGR2_PREDIV1)
  /* 重置 CFGR2（时钟配置寄存器2） */
  CLEAR_REG(RCC->CFGR2);
#endif /* RCC_CFGR2_PREDIV1 */

  /* 重置 CSR（控制/状态寄存器）中的全部标志 */
  SET_BIT(RCC->CSR, RCC_CSR_RMVF);

  /* 禁用全部中断 */
  CLEAR_REG(RCC->CIR);

  return HAL_OK;
}

/**
  * @brief  初始化RCC振荡器（根据RCC_OscInitTypeDef中指定的参数）
  * @param  RCC_OscInitStruct p指向RCC_OscInitTypeDef结构体的指针，该结构体包含RCC振荡器的配置信息。
  * @note   PLL(锁相环)作为系统时钟时不可关闭。
  * @note   当USB OTG FS时钟启用时，PLL(锁相环)不可关闭(特定于带有USB FS的设备)
  * @note   函数不支持 LSE Bypass 转换为 LSE On 和 LSE On 转换为 LSE Bypass。
  *         用户应该首先请求切换到 LSE Off，然后再切换到 LSE On 或 LSE Bypass。
  * @note   函数不支持将 HSE Bypass 转换为 HSE On 和 将 HSE On 转换为 HSE Bypass。
  *         用户应首先请求切换到 HSE Off，然后再切换到 HSE On 或 HSE Bypass 。
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCC_OscConfig(RCC_OscInitTypeDef  *RCC_OscInitStruct)
{
  uint32_t tickstart;
  uint32_t pll_config;

  /* 检查空指针 */
  if (RCC_OscInitStruct == NULL)
  {
    return HAL_ERROR;
  }

  /* 检查参数合法性 */
  assert_param(IS_RCC_OSCILLATORTYPE(RCC_OscInitStruct->OscillatorType));

  /*------------------------------- HSE 配置 ------------------------*/
  if (((RCC_OscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_HSE) == RCC_OSCILLATORTYPE_HSE)
  {
    /* 检查参数合法性 */
    assert_param(IS_RCC_HSE(RCC_OscInitStruct->HSEState));

    /* 当HSE被用作系统时钟或PLL的时钟源时，不允许将其禁用 */
    if ((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_SYSCLKSOURCE_STATUS_HSE)
        || ((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_SYSCLKSOURCE_STATUS_PLLCLK) && (__HAL_RCC_GET_PLL_OSCSOURCE() == RCC_PLLSOURCE_HSE)))
    {
      if ((__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) != RESET) && (RCC_OscInitStruct->HSEState == RCC_HSE_OFF))
      {
        return HAL_ERROR;
      }
    }
    else
    {
      /* 设置新的 HSE 配置 ---------------------------------------*/
      __HAL_RCC_HSE_CONFIG(RCC_OscInitStruct->HSEState);


      /* 检查 HSE 状态 */
      if (RCC_OscInitStruct->HSEState != RCC_HSE_OFF)
      {
        /* 获取起始滴答值 */
        tickstart = HAL_GetTick();

        /* 等待 HSE 就绪 */
        while (__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) == RESET)
        {
          if ((HAL_GetTick() - tickstart) > HSE_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
      else
      {
        /* 获取起始滴答值 */
        tickstart = HAL_GetTick();

        /* 等待 HSE 禁用 */
        while (__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) != RESET)
        {
          if ((HAL_GetTick() - tickstart) > HSE_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
    }
  }
  /*----------------------------- HSI 配置 --------------------------*/
  if (((RCC_OscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_HSI) == RCC_OSCILLATORTYPE_HSI)
  {
    /* 检查参数合法性 */
    assert_param(IS_RCC_HSI(RCC_OscInitStruct->HSIState));
    assert_param(IS_RCC_CALIBRATION_VALUE(RCC_OscInitStruct->HSICalibrationValue));

    /* 当选择锁相环作为系统时钟时，检查是否使用HSI作为系统时钟或作为锁相环源 */
    if ((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_SYSCLKSOURCE_STATUS_HSI)
        || ((__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_SYSCLKSOURCE_STATUS_PLLCLK) && (__HAL_RCC_GET_PLL_OSCSOURCE() == RCC_PLLSOURCE_HSI_DIV2)))
    {
      /* 当HSI被用作系统时钟时，它不可被禁用 */
      if ((__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) != RESET) && (RCC_OscInitStruct->HSIState != RCC_HSI_ON))
      {
        return HAL_ERROR;
      }
      /* 否则，只允许校准 */
      else
      {
        /* 调整内部高速振荡器(HSI)校准值 */
        __HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(RCC_OscInitStruct->HSICalibrationValue);
      }
    }
    else
    {
      /* 检查 HSI 状态 */
      if (RCC_OscInitStruct->HSIState != RCC_HSI_OFF)
      {
        /* 启用内部高速振荡器(HSI) */
        __HAL_RCC_HSI_ENABLE();

        /* 获取起始滴答值 */
        tickstart = HAL_GetTick();

        /* 等待 HSI 就绪 */
        while (__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) == RESET)
        {
          if ((HAL_GetTick() - tickstart) > HSI_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }

        /* 调整内部高速振荡器(HSI)校准值 */
        __HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(RCC_OscInitStruct->HSICalibrationValue);
      }
      else
      {
        /* 禁用内部高速振荡器(HSI) */
        __HAL_RCC_HSI_DISABLE();

        /* 获取起始滴答值 */
        tickstart = HAL_GetTick();

        /* 等待 HSI 关闭 */
        while (__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) != RESET)
        {
          if ((HAL_GetTick() - tickstart) > HSI_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
    }
  }
  /*------------------------------ LSI 配置 -------------------------*/
  if (((RCC_OscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_LSI) == RCC_OSCILLATORTYPE_LSI)
  {
    /* 检查参数合法性 */
    assert_param(IS_RCC_LSI(RCC_OscInitStruct->LSIState));

    /* 检查 LSI 状态 */
    if (RCC_OscInitStruct->LSIState != RCC_LSI_OFF)
    {
      /* 启用内部低速振荡器 (LSI) */
      __HAL_RCC_LSI_ENABLE();

      /* 获取起始滴答值 */
      tickstart = HAL_GetTick();

      /* 等待 LSI 就绪 */
      while (__HAL_RCC_GET_FLAG(RCC_FLAG_LSIRDY) == RESET)
      {
        if ((HAL_GetTick() - tickstart) > LSI_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
      /*  为了获得在指定范围内完全稳定的时钟，应当添加1毫秒的软件延迟。*/
      RCC_Delay(1);
    }
    else
    {
      /* 关闭内部低速振荡器 (LSI) */
      __HAL_RCC_LSI_DISABLE();

      /* 获取起始滴答值 */
      tickstart = HAL_GetTick();

      /* 等待 LSI 关闭 */
      while (__HAL_RCC_GET_FLAG(RCC_FLAG_LSIRDY) != RESET)
      {
        if ((HAL_GetTick() - tickstart) > LSI_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
    }
  }
  /*------------------------------ LSE 配置 -------------------------*/
  if (((RCC_OscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_LSE) == RCC_OSCILLATORTYPE_LSE)
  {
    FlagStatus       pwrclkchanged = RESET;

    /* 检查参数合法性 */
    assert_param(IS_RCC_LSE(RCC_OscInitStruct->LSEState));

    /* 更新备份域控制寄存器中的LSE配置    */
    /* 需要启用对必要的备份域的写访问权限 */
    if (__HAL_RCC_PWR_IS_CLK_DISABLED())
    {
      __HAL_RCC_PWR_CLK_ENABLE();
      pwrclkchanged = SET;
    }

    if (HAL_IS_BIT_CLR(PWR->CR, PWR_CR_DBP))
    {
      /* 启用对备份域的写访问权限 */
      SET_BIT(PWR->CR, PWR_CR_DBP);

      /* 等待备份域写保护禁用 */
      tickstart = HAL_GetTick();

      while (HAL_IS_BIT_CLR(PWR->CR, PWR_CR_DBP))
      {
        if ((HAL_GetTick() - tickstart) > RCC_DBP_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
    }

    /* 设置新的 LSE 配置 -----------------------------------------*/
    __HAL_RCC_LSE_CONFIG(RCC_OscInitStruct->LSEState);
    /* 检查 LSE 状态 */
    if (RCC_OscInitStruct->LSEState != RCC_LSE_OFF)
    {
      /* 获取起始滴答值 */
      tickstart = HAL_GetTick();

      /* 等待 LSE 就绪 */
      while (__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) == RESET)
      {
        if ((HAL_GetTick() - tickstart) > RCC_LSE_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
    }
    else
    {
      /* 获取起始滴答值 */
      tickstart = HAL_GetTick();

      /* 等待 LSE 禁用 */
      while (__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) != RESET)
      {
        if ((HAL_GetTick() - tickstart) > RCC_LSE_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
    }

    /* 如果需要，关闭电源power的时钟（前面开备份域写保护，开了电源部分的时钟） */
    if (pwrclkchanged == SET)
    {
      __HAL_RCC_PWR_CLK_DISABLE();
    }
  }

#if defined(RCC_CR_PLL2ON)
  /*-------------------------------- PLL2 配置 -----------------------*/
  /* 检查参数合法性 */
  assert_param(IS_RCC_PLL2(RCC_OscInitStruct->PLL2.PLL2State));
  if ((RCC_OscInitStruct->PLL2.PLL2State) != RCC_PLL2_NONE)
  {
    /* 如果PLL2时钟间接地被用作系统时钟（即被用作系统时钟的PLL时钟输入），则无法清除此位 */
    if ((__HAL_RCC_GET_PLL_OSCSOURCE() == RCC_PLLSOURCE_HSE) && \
        (__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_SYSCLKSOURCE_STATUS_PLLCLK) && \
        ((READ_BIT(RCC->CFGR2, RCC_CFGR2_PREDIV1SRC)) == RCC_CFGR2_PREDIV1SRC_PLL2))
    {
      return HAL_ERROR;
    }
    else
    {
      if ((RCC_OscInitStruct->PLL2.PLL2State) == RCC_PLL2_ON)
      {
        /* 检查参数合法性 */
        assert_param(IS_RCC_PLL2_MUL(RCC_OscInitStruct->PLL2.PLL2MUL));
        assert_param(IS_RCC_HSE_PREDIV2(RCC_OscInitStruct->PLL2.HSEPrediv2Value));

        /* 只有在禁用PLLI2S（PLL3）时才能写入预分频器Prediv2 */
        /* 如果新值与已编程的值不同，才返回错误 */
        if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL3ON) && \
            (__HAL_RCC_HSE_GET_PREDIV2() != RCC_OscInitStruct->PLL2.HSEPrediv2Value))
        {
          return HAL_ERROR;
        }

        /* 禁用 PLL2. */
        __HAL_RCC_PLL2_DISABLE();

        /* 获取起始滴答值 */
        tickstart = HAL_GetTick();

        /* 等待 PLL2 禁用 */
        while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLL2RDY) != RESET)
        {
          if ((HAL_GetTick() - tickstart) > PLL2_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }

        /* 配置 HSE 预分频器 prediv2 --------------------------------*/
        __HAL_RCC_HSE_PREDIV2_CONFIG(RCC_OscInitStruct->PLL2.HSEPrediv2Value);

        /* 配置 PLL2 倍频系数 */
        __HAL_RCC_PLL2_CONFIG(RCC_OscInitStruct->PLL2.PLL2MUL);

        /* 启用 PLL2. */
        __HAL_RCC_PLL2_ENABLE();

        /* 获取起始滴答值 */
        tickstart = HAL_GetTick();

        /* 等待 PLL2 就绪 */
        while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLL2RDY)  == RESET)
        {
          if ((HAL_GetTick() - tickstart) > PLL2_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
      else
      {
        /* 设置HSE振荡器时钟作为PREDIV1的时钟源 */
        CLEAR_BIT(RCC->CFGR2, RCC_CFGR2_PREDIV1SRC);

        /* 禁用 PLL2. */
        __HAL_RCC_PLL2_DISABLE();

        /* 获取起始滴答值 */
        tickstart = HAL_GetTick();

        /* 等待 PLL2 禁用 */
        while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLL2RDY)  != RESET)
        {
          if ((HAL_GetTick() - tickstart) > PLL2_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
    }
  }

#endif /* RCC_CR_PLL2ON */
  /*-------------------------------- PLL 配置 -----------------------*/
  /* 检查参数合法性 */
  assert_param(IS_RCC_PLL(RCC_OscInitStruct->PLL.PLLState));
  if ((RCC_OscInitStruct->PLL.PLLState) != RCC_PLL_NONE)
  {
    /* 检查PLL(锁相环)是否用作系统时钟 */
    if (__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_SYSCLKSOURCE_STATUS_PLLCLK)
    {
      if ((RCC_OscInitStruct->PLL.PLLState) == RCC_PLL_ON)
      {
        /* 检查参数合法性 */
        assert_param(IS_RCC_PLLSOURCE(RCC_OscInitStruct->PLL.PLLSource));
        assert_param(IS_RCC_PLL_MUL(RCC_OscInitStruct->PLL.PLLMUL));

        /*关闭 PLL. */
        __HAL_RCC_PLL_DISABLE();

        /* 获取起始滴答值 */
        tickstart = HAL_GetTick();

        /* 等待 PLL 禁用 */
        while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY)  != RESET)
        {
          if ((HAL_GetTick() - tickstart) > PLL_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }

        /* 配置 HSE 预分频器 prediv --------------------------------*/
        /* 只有在PLL被禁用时才能写入。仅当PLL的时钟源与HSE不同时才使用。 */
        if (RCC_OscInitStruct->PLL.PLLSource == RCC_PLLSOURCE_HSE)
        {
          /* 检查参数 */
          assert_param(IS_RCC_HSE_PREDIV(RCC_OscInitStruct->HSEPredivValue));
#if defined(RCC_CFGR2_PREDIV1SRC)
          assert_param(IS_RCC_PREDIV1_SOURCE(RCC_OscInitStruct->Prediv1Source));

          /* 设置 预分频器PREDIV1 的源 */
          SET_BIT(RCC->CFGR2, RCC_OscInitStruct->Prediv1Source);
#endif /* RCC_CFGR2_PREDIV1SRC */

          /* 设置 预分频器PREDIV1 预分频值 */
          __HAL_RCC_HSE_PREDIV_CONFIG(RCC_OscInitStruct->HSEPredivValue);
        }

        /* 配置 PLL 是中原和倍频系数 */
        __HAL_RCC_PLL_CONFIG(RCC_OscInitStruct->PLL.PLLSource,
                             RCC_OscInitStruct->PLL.PLLMUL);
        /* 启用 PLL. */
        __HAL_RCC_PLL_ENABLE();

        /* 获取起始滴答值 */
        tickstart = HAL_GetTick();

        /* 等待 PLL 就绪 */
        while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY)  == RESET)
        {
          if ((HAL_GetTick() - tickstart) > PLL_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
      else
      {
        /* 禁用 PLL. */
        __HAL_RCC_PLL_DISABLE();

        /* 获取起始滴答值 */
        tickstart = HAL_GetTick();

        /* 等待 PLL 关闭 */
        while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY)  != RESET)
        {
          if ((HAL_GetTick() - tickstart) > PLL_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
    }
    else
    {
      /* 检查是否有请求禁用作为系统时钟源的PLL */
      if ((RCC_OscInitStruct->PLL.PLLState) == RCC_PLL_OFF)
      {
        return HAL_ERROR;
      }
      else
      {
        /* 如果请求重复当前配置，则不返回HAL_ERROR */
        pll_config = RCC->CFGR;
        if ((READ_BIT(pll_config, RCC_CFGR_PLLSRC) != RCC_OscInitStruct->PLL.PLLSource) ||
            (READ_BIT(pll_config, RCC_CFGR_PLLMULL) != RCC_OscInitStruct->PLL.PLLMUL))
        {
          return HAL_ERROR;
        }
      }
    }
  }

  return HAL_OK;
}

/**
  * @brief  初始化CPU、AHB和APB总线时钟（根据RCC_ClkInitStruct中的参数）
  * 
  * @param  RCC_ClkInitStruct 指向RCC_OscInitTypeDef结构体的指针，该结构体包含RCC外设的配置信息。
  * 
  * @param  FLatency FLASH 等待周期
  *                         该参数的值取决于芯片
  * @note   SystemCoreClock CMSIS变量用于存储系统时钟频率，并由本函数内部调用的@ref HAL_RCC_GetHCLKFreq()函数更新。
  *
  * @note   在复位启动、从停止和待机模式唤醒，或者HSE直接或间接作为系统时钟失败的情况下（如果时钟安全系统CSS已启用），HSI会被用作系统时钟源（由硬件启用）。
  *
  * @note   只有当目标时钟源准备就绪（经过启动延迟后时钟稳定或PLL锁定）时，才会发生从一种时钟源到另一种时钟源的切换。
  *         如果选择了尚未准备就绪的时钟源，切换将在时钟源准备就绪时发生。
  *         您可以使用 @ref HAL_RCC_GetClockConfig() 函数来了解当前作为系统时钟源的时钟。
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCC_ClockConfig(RCC_ClkInitTypeDef  *RCC_ClkInitStruct, uint32_t FLatency)
{
  uint32_t tickstart;

  /* 检查空指针 */
  if (RCC_ClkInitStruct == NULL)
  {
    return HAL_ERROR;
  }

  /* 检查参数合法性 */
  assert_param(IS_RCC_CLOCKTYPE(RCC_ClkInitStruct->ClockType));
  assert_param(IS_FLASH_LATENCY(FLatency));

  /* 为了正确从闪存中读取数据，必须根据设备的CPU时钟频率（HCLK）正确设置等待周期（LATENCY） */

#if defined(FLASH_ACR_LATENCY)
  /* 由于CPU频率更高而增加等待周期的数量 */
  if (FLatency > __HAL_FLASH_GET_LATENCY())
  {
    /* 将新的等待周期数量编程到FLASH_ACR寄存器中的LATENCY位 */
    __HAL_FLASH_SET_LATENCY(FLatency);

    /* 通过读取FLASH_ACR寄存器来检查新的等待周期数量是否被考虑到访问闪存内存 */
    if (__HAL_FLASH_GET_LATENCY() != FLatency)
  {
    return HAL_ERROR;
  }
}

#endif /* FLASH_ACR_LATENCY */
/*-------------------------- HCLK 配置 --------------------------*/
if (((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_HCLK) == RCC_CLOCKTYPE_HCLK)
  {
    /* 设置最高的APBx分频器，以确保无论我们减小还是增加HCLK，都不会经历非规范阶段。 */
    if (((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_PCLK1) == RCC_CLOCKTYPE_PCLK1)
    {
      MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_HCLK_DIV16);
    }

    if (((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_PCLK2) == RCC_CLOCKTYPE_PCLK2)
    {
      MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, (RCC_HCLK_DIV16 << 3));
    }

    /* 设置新的HCLK时钟分频器 */
    assert_param(IS_RCC_HCLK(RCC_ClkInitStruct->AHBCLKDivider));
    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_ClkInitStruct->AHBCLKDivider);
  }

  /*------------------------- SYSCLK 配置 ---------------------------*/
  if (((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_SYSCLK) == RCC_CLOCKTYPE_SYSCLK)
  {
    assert_param(IS_RCC_SYSCLKSOURCE(RCC_ClkInitStruct->SYSCLKSource));

    /* HSE 作为系统时钟源 */
    if (RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_HSE)
    {
      /* 检查 HSE 就绪标志 */
      if (__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) == RESET)
      {
        return HAL_ERROR;
      }
    }
    /* PLL 作为系统时钟源 */
    else if (RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_PLLCLK)
    {
      /* 检查 PLL 就绪标志 */
      if (__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) == RESET)
      {
        return HAL_ERROR;
      }
    }
    /* HSI 作为系统时钟源 */
    else
    {
      /* 检查 HSI 就绪标志 */
      if (__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) == RESET)
      {
        return HAL_ERROR;
      }
    }
    __HAL_RCC_SYSCLK_CONFIG(RCC_ClkInitStruct->SYSCLKSource);

    /* 获取起始滴答值 */
    tickstart = HAL_GetTick();

    while (__HAL_RCC_GET_SYSCLK_SOURCE() != (RCC_ClkInitStruct->SYSCLKSource << RCC_CFGR_SWS_Pos))
    {
      if ((HAL_GetTick() - tickstart) > CLOCKSWITCH_TIMEOUT_VALUE)
      {
        return HAL_TIMEOUT;
      }
    }
  }

#if defined(FLASH_ACR_LATENCY)
  /* 由于CPU频率降低而减少等待周期的数量 */
  if (FLatency < __HAL_FLASH_GET_LATENCY())
  {
    /* 将新的等待周期数量编程到FLASH_ACR寄存器中的LATENCY位 */
    __HAL_FLASH_SET_LATENCY(FLatency);

    /* 通过读取FLASH_ACR寄存器来检查新的等待周期数量是否被考虑到访问闪存内存 */
    if (__HAL_FLASH_GET_LATENCY() != FLatency)
  {
    return HAL_ERROR;
  }
}
#endif /* FLASH_ACR_LATENCY */

/*-------------------------- PCLK1 配置 ---------------------------*/
if (((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_PCLK1) == RCC_CLOCKTYPE_PCLK1)
  {
    assert_param(IS_RCC_PCLK(RCC_ClkInitStruct->APB1CLKDivider));
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_ClkInitStruct->APB1CLKDivider);
  }

  /*-------------------------- PCLK2 配置 ---------------------------*/
  if (((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_PCLK2) == RCC_CLOCKTYPE_PCLK2)
  {
    assert_param(IS_RCC_PCLK(RCC_ClkInitStruct->APB2CLKDivider));
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, ((RCC_ClkInitStruct->APB2CLKDivider) << 3));
  }

  /* 更新 SystemCoreClock 全局变量 */
  SystemCoreClock = HAL_RCC_GetSysClockFreq() >> AHBPrescTable[(RCC->CFGR & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos];

  /* 考虑新的系统时钟设置，配置时间基准的来源*/
  HAL_InitTick(uwTickPrio);

  return HAL_OK;
}

/**
  * @}
  */

/** @defgroup RCC_Exported_Functions_Group2 外设控制函数
  *  @brief   RCC 时钟控制函数
  *
  @verbatim
  ===============================================================================
                  ##### 外设控制函数 #####
  ===============================================================================
    [..]
    本小节提供了一组允许控制RCC时钟频率的函数。

  @endverbatim
  * @{
  */

/**
  * @brief  选择要输出到MCO引脚的时钟源
  * @note   MCO 引脚应配置为复用功能模式。
  * @param  RCC_MCOx 指定时钟源的输出方向
  *          此参数可为以下值之一：
  *            @arg @ref RCC_MCO1 要输出到MCO1引脚（PA8）的时钟源
  * @param  RCC_MCOSource 指定要输出的时钟源
  *          此参数可为以下值之一：
  *            @arg @ref RCC_MCO1SOURCE_NOCLOCK     未选择 MCO 源
  *            @arg @ref RCC_MCO1SOURCE_SYSCLK      系统时钟 作为 MCO 源
  *            @arg @ref RCC_MCO1SOURCE_HSI         HSI 作为 MCO 源
  *            @arg @ref RCC_MCO1SOURCE_HSE         HSE 作为 MCO 源
  @if STM32F105xC
  *            @arg @ref RCC_MCO1SOURCE_PLLCLK       PLL 时钟2分频作为 MCO 源
  *            @arg @ref RCC_MCO1SOURCE_PLL2CLK      PLL2 时钟作为 MCO 源
  *            @arg @ref RCC_MCO1SOURCE_PLL3CLK_DIV2 PLL3 时钟2分频作为 MCO 源
  *            @arg @ref RCC_MCO1SOURCE_EXT_HSE      XT1 外部 3-25 MHz 振荡器作为 MCO 源
  *            @arg @ref RCC_MCO1SOURCE_PLL3CLK      PLL3 时钟作为 MCO 源
  @endif
  @if STM32F107xC
  *            @arg @ref RCC_MCO1SOURCE_PLLCLK       PLL 时钟2分频作为 MCO 源
  *            @arg @ref RCC_MCO1SOURCE_PLL2CLK      PLL2 时钟作为 MCO 源
  *            @arg @ref RCC_MCO1SOURCE_PLL3CLK_DIV2 PLL3 时钟2分频作为 MCO 源
  *            @arg @ref RCC_MCO1SOURCE_EXT_HSE      XT1 外部 3-25 MHz 振荡器作为 MCO 源
  *            @arg @ref RCC_MCO1SOURCE_PLL3CLK      PLL3 时钟作为 MCO 源
  @endif
  * @param  RCC_MCODiv 指定 MCO 分频
  *          此参数可为以下值之一：
  *            @arg @ref RCC_MCODIV_1 MCO无输出分频
  * @retval None
  */
void HAL_RCC_MCOConfig(uint32_t RCC_MCOx, uint32_t RCC_MCOSource, uint32_t RCC_MCODiv)
{
  GPIO_InitTypeDef gpio = {0U};

  /* 检查参数合法性 */
  assert_param(IS_RCC_MCO(RCC_MCOx));
  assert_param(IS_RCC_MCODIV(RCC_MCODiv));
  assert_param(IS_RCC_MCO1SOURCE(RCC_MCOSource));

  /* 防止未使用参数的编译警告 */
  UNUSED(RCC_MCOx);
  UNUSED(RCC_MCODiv);

  /* 将MCO1引脚配置为复用功能模式 */
  gpio.Mode      = GPIO_MODE_AF_PP;
  gpio.Speed     = GPIO_SPEED_FREQ_HIGH;
  gpio.Pull      = GPIO_NOPULL;
  gpio.Pin       = MCO1_PIN;

  /* MCO1 时钟开启 */
  MCO1_CLK_ENABLE();

  HAL_GPIO_Init(MCO1_GPIO_PORT, &gpio);

  /* 配置 MCO 时钟源 */
  __HAL_RCC_MCO1_CONFIG(RCC_MCOSource, RCC_MCODiv);
}

/**
  * @brief  启用时钟安全系统
  * @note   如果在HSE振荡器时钟上检测到故障，则该振荡器会自动禁用，
  *         并生成中断以通知软件发生故障（时钟安全系统中断，CSSI），从而允许MCU执行救援操作。
  *         CSSI与Cortex-M3 NMI（不可屏蔽中断）异常向量相关联。
  * @retval None
  */
void HAL_RCC_EnableCSS(void)
{
  *(__IO uint32_t *) RCC_CR_CSSON_BB = (uint32_t)ENABLE;
}

/**
  * @brief  禁用时钟安全系统
  * @retval None
  */
void HAL_RCC_DisableCSS(void)
{
  *(__IO uint32_t *) RCC_CR_CSSON_BB = (uint32_t)DISABLE;
}

/**
  * @brief  获取系统时钟频率
  * @note   此函数计算得到的系统频率并非芯片上的实际频率，它是基于预定义常数和所选的时钟源计算得出的。
  * @note     如果系统时钟源是HSI，函数返回值基于 HSI_VALUE(*)
  * @note     如果系统时钟源是HSE，函数返回值基于 HSE_VALUE 除以分频因数(**)
  * @note     如果系统时钟源是PLL, 函数返回值基于 HSE_VALUE 除以分频因数(**) 或者 HSI_VALUE(*)  并乘以锁相环因子。
  * @note     (*) HSI_VALUE 是在 stm32f1xx_hal_conf.h 文件中定义的常量（默认值为 8 MHz），
  *               但实际值可能会因电压和温度的变化而有所不同。
  * @note     (**) HSE_VALUE 是在 stm32f1xx_hal_conf.h 文件中定义的常量（默认值为 8 MHz），
  *                用户必须确保 HSE_VALUE 与实际使用的晶体的频率相同。否则，此函数可能会产生错误的结果。
  *
  * @note   当使用的HSE晶体为小数时，此函数的结果可能不正确。
  *
  * @note   用户应用程序可以使用此函数来计算通信外设的波特率或配置其他参数。
  *
  * @note   每当SYSCLK发生变化时，必须调用此函数以更新正确的SYSCLK值。
  *         否则，基于此函数的任何配置都将是不正确的。
  *
  * @retval SYSCLK 频率
  */
uint32_t HAL_RCC_GetSysClockFreq(void)
{
#if defined(RCC_CFGR2_PREDIV1SRC)
  static const uint8_t aPLLMULFactorTable[14U] = {0, 0, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 13};
  static const uint8_t aPredivFactorTable[16U] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
#else
  static const uint8_t aPLLMULFactorTable[16U] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 16};
#if defined(RCC_CFGR2_PREDIV1)
  static const uint8_t aPredivFactorTable[16U] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
#else
  static const uint8_t aPredivFactorTable[2U] = {1, 2};
#endif /*RCC_CFGR2_PREDIV1*/

#endif
  uint32_t tmpreg = 0U, prediv = 0U, pllclk = 0U, pllmul = 0U;
  uint32_t sysclockfreq = 0U;
#if defined(RCC_CFGR2_PREDIV1SRC)
  uint32_t prediv2 = 0U, pll2mul = 0U;
#endif /*RCC_CFGR2_PREDIV1SRC*/

  tmpreg = RCC->CFGR;

  /* 获取系统时钟源 -------------------------------------------------------*/
  switch (tmpreg & RCC_CFGR_SWS)
  {
    case RCC_SYSCLKSOURCE_STATUS_HSE:  /* HSE 被用作系统时钟 */
    {
      sysclockfreq = HSE_VALUE;
      break;
    }
    case RCC_SYSCLKSOURCE_STATUS_PLLCLK:  /* PLL 被用作系统时钟 */
    {
      pllmul = aPLLMULFactorTable[(uint32_t)(tmpreg & RCC_CFGR_PLLMULL) >> RCC_CFGR_PLLMULL_Pos];
      if ((tmpreg & RCC_CFGR_PLLSRC) != RCC_PLLSOURCE_HSI_DIV2)
      {
#if defined(RCC_CFGR2_PREDIV1)
        prediv = aPredivFactorTable[(uint32_t)(RCC->CFGR2 & RCC_CFGR2_PREDIV1) >> RCC_CFGR2_PREDIV1_Pos];
#else
        prediv = aPredivFactorTable[(uint32_t)(RCC->CFGR & RCC_CFGR_PLLXTPRE) >> RCC_CFGR_PLLXTPRE_Pos];
#endif /*RCC_CFGR2_PREDIV1*/
#if defined(RCC_CFGR2_PREDIV1SRC)

        if (HAL_IS_BIT_SET(RCC->CFGR2, RCC_CFGR2_PREDIV1SRC))
        {
          /* PLL2 作为 预分频器Prediv1 的源 */
          /* PLLCLK = PLL2CLK / PREDIV1 * PLLMUL with PLL2CLK = HSE/PREDIV2 * PLL2MUL */
          prediv2 = ((RCC->CFGR2 & RCC_CFGR2_PREDIV2) >> RCC_CFGR2_PREDIV2_Pos) + 1;
          pll2mul = ((RCC->CFGR2 & RCC_CFGR2_PLL2MUL) >> RCC_CFGR2_PLL2MUL_Pos) + 2;
          pllclk = (uint32_t)(((uint64_t)HSE_VALUE * (uint64_t)pll2mul * (uint64_t)pllmul) / ((uint64_t)prediv2 * (uint64_t)prediv));
        }
        else
        {
          /* HSE 作为 PLL 时钟源 : PLLCLK = HSE/PREDIV1 * PLLMUL */
          pllclk = (uint32_t)((HSE_VALUE * pllmul) / prediv);
        }

        /* 如果PLLMUL被设置为13，意味着它是为了覆盖PLLMUL为6.5的情况（避免使用浮点数） */
        /* 在这种情况下需要将pllclk除以2 */
        if (pllmul == aPLLMULFactorTable[(uint32_t)(RCC_CFGR_PLLMULL6_5) >> RCC_CFGR_PLLMULL_Pos])
        {
          pllclk = pllclk / 2;
        }
#else
        /* HSE 作为 PLL 时钟源 : PLLCLK = HSE/PREDIV1 * PLLMUL */
        pllclk = (uint32_t)((HSE_VALUE  * pllmul) / prediv);
#endif /*RCC_CFGR2_PREDIV1SRC*/
      }
      else
      {
        /* HSI 作为 PLL 时钟源 : PLLCLK = HSI/2 * PLLMUL */
        pllclk = (uint32_t)((HSI_VALUE >> 1) * pllmul);
      }
      sysclockfreq = pllclk;
      break;
    }
    case RCC_SYSCLKSOURCE_STATUS_HSI:  /* HSI 被用作系统时钟源 */
    default: /* HSI 被用作系统时钟 */
    {
      sysclockfreq = HSI_VALUE;
      break;
    }
  }
  return sysclockfreq;
}

/**
  * @brief  获取 HCLK 频率
  * @note   每次 HCLK更改时，必须调用此函数以更新正确的HCLK值。否则，基于该功能的配置将不正确。
  *
  * @note   SystemCoreClock CMSIS变量用于存储系统时钟频率并在此函数中更新
  * @retval HCLK frequency
  */
uint32_t HAL_RCC_GetHCLKFreq(void)
{
  return SystemCoreClock;
}

/**
  * @brief  获取 PCLK1 频率
  * @note   每次 PCLK1 改变时，必须调用这个函数来更新正确的PCLK1值。否则，基于该功能的配置将不正确。
  * @retval PCLK1 frequency
  */
uint32_t HAL_RCC_GetPCLK1Freq(void)
{
  /* 获取 HCLK 源并计算 PCLK1 频率 ---------------------------*/
  return (HAL_RCC_GetHCLKFreq() >> APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos]);
}

/**
  * @brief  获取 PCLK2 频率
  * @note   每次PCLK2更改时，必须调用此函数来更新正确的PCLK2值。否则，基于该功能的配置将不正确。
  * @retval PCLK2 frequency
  */
uint32_t HAL_RCC_GetPCLK2Freq(void)
{
  /* 获取 HCLK 源并计算 PCLK2 频率 ---------------------------*/
  return (HAL_RCC_GetHCLKFreq() >> APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE2) >> RCC_CFGR_PPRE2_Pos]);
}

/**
  * @brief  获取 RCC 振荡器配置（根据内部RCC配置寄存器的设置）
  * @param  RCC_OscInitStruct 指向将要存储配置的RCC_OscInitTypeDef结构体的指针
  * @retval None
  */
void HAL_RCC_GetOscConfig(RCC_OscInitTypeDef  *RCC_OscInitStruct)
{
  /* 检查参数合法性 */
  assert_param(RCC_OscInitStruct != NULL);

  /* 设置振荡器类型参数的所有可能值 ---------------*/
  RCC_OscInitStruct->OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI  \
                                      | RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_LSI;

#if defined(RCC_CFGR2_PREDIV1SRC)
  /* 获取 预分频器Prediv1 的输入源 --------------------------------------------------*/
  RCC_OscInitStruct->Prediv1Source = READ_BIT(RCC->CFGR2, RCC_CFGR2_PREDIV1SRC);
#endif /* RCC_CFGR2_PREDIV1SRC */

  /* 获取 HSE 配置 -----------------------------------------------*/
  if ((RCC->CR & RCC_CR_HSEBYP) == RCC_CR_HSEBYP)
  {
    RCC_OscInitStruct->HSEState = RCC_HSE_BYPASS;
  }
  else if ((RCC->CR & RCC_CR_HSEON) == RCC_CR_HSEON)
  {
    RCC_OscInitStruct->HSEState = RCC_HSE_ON;
  }
  else
  {
    RCC_OscInitStruct->HSEState = RCC_HSE_OFF;
  }
  RCC_OscInitStruct->HSEPredivValue = __HAL_RCC_HSE_GET_PREDIV();

  /* 获取 HSI 配置 -----------------------------------------------*/
  if ((RCC->CR & RCC_CR_HSION) == RCC_CR_HSION)
  {
    RCC_OscInitStruct->HSIState = RCC_HSI_ON;
  }
  else
  {
    RCC_OscInitStruct->HSIState = RCC_HSI_OFF;
  }

  RCC_OscInitStruct->HSICalibrationValue = (uint32_t)((RCC->CR & RCC_CR_HSITRIM) >> RCC_CR_HSITRIM_Pos);

  /* 获取 LSE 配置 -----------------------------------------------*/
  if ((RCC->BDCR & RCC_BDCR_LSEBYP) == RCC_BDCR_LSEBYP)
  {
    RCC_OscInitStruct->LSEState = RCC_LSE_BYPASS;
  }
  else if ((RCC->BDCR & RCC_BDCR_LSEON) == RCC_BDCR_LSEON)
  {
    RCC_OscInitStruct->LSEState = RCC_LSE_ON;
  }
  else
  {
    RCC_OscInitStruct->LSEState = RCC_LSE_OFF;
  }

  /* 获取 LSI 配置 -----------------------------------------------*/
  if ((RCC->CSR & RCC_CSR_LSION) == RCC_CSR_LSION)
  {
    RCC_OscInitStruct->LSIState = RCC_LSI_ON;
  }
  else
  {
    RCC_OscInitStruct->LSIState = RCC_LSI_OFF;
  }


  /* 获取 PLL 配置 -----------------------------------------------*/
  if ((RCC->CR & RCC_CR_PLLON) == RCC_CR_PLLON)
  {
    RCC_OscInitStruct->PLL.PLLState = RCC_PLL_ON;
  }
  else
  {
    RCC_OscInitStruct->PLL.PLLState = RCC_PLL_OFF;
  }
  RCC_OscInitStruct->PLL.PLLSource = (uint32_t)(RCC->CFGR & RCC_CFGR_PLLSRC);
  RCC_OscInitStruct->PLL.PLLMUL = (uint32_t)(RCC->CFGR & RCC_CFGR_PLLMULL);
#if defined(RCC_CR_PLL2ON)
  /* 获取 PLL2 配置 -----------------------------------------------*/
  if ((RCC->CR & RCC_CR_PLL2ON) == RCC_CR_PLL2ON)
  {
    RCC_OscInitStruct->PLL2.PLL2State = RCC_PLL2_ON;
  }
  else
  {
    RCC_OscInitStruct->PLL2.PLL2State = RCC_PLL2_OFF;
  }
  RCC_OscInitStruct->PLL2.HSEPrediv2Value = __HAL_RCC_HSE_GET_PREDIV2();
  RCC_OscInitStruct->PLL2.PLL2MUL = (uint32_t)(RCC->CFGR2 & RCC_CFGR2_PLL2MUL);
#endif /* RCC_CR_PLL2ON */
}

/**
  * @brief  获取 RCC 时钟配置（根据内部RCC配置寄存器的设置）
  * @param  RCC_ClkInitStruct 用于存储配置的RCC_ClkInitTypeDef结构体的指针
  * @param  pFLatency 存储Flash延迟的指针
  * @retval None
  */
void HAL_RCC_GetClockConfig(RCC_ClkInitTypeDef  *RCC_ClkInitStruct, uint32_t *pFLatency)
{
  /* 检查参数合法性 */
  assert_param(RCC_ClkInitStruct != NULL);
  assert_param(pFLatency != NULL);

  /* 设置时钟类型参数的所有可能值 --------------------*/
  RCC_ClkInitStruct->ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;

  /* 获取 SYSCLK 配置 --------------------------------------------*/
  RCC_ClkInitStruct->SYSCLKSource = (uint32_t)(RCC->CFGR & RCC_CFGR_SW);

  /* 获取 HCLK 配置 ----------------------------------------------*/
  RCC_ClkInitStruct->AHBCLKDivider = (uint32_t)(RCC->CFGR & RCC_CFGR_HPRE);

  /* 获取 APB1 配置 ----------------------------------------------*/
  RCC_ClkInitStruct->APB1CLKDivider = (uint32_t)(RCC->CFGR & RCC_CFGR_PPRE1);

  /* 获取 APB2 配置 ----------------------------------------------*/
  RCC_ClkInitStruct->APB2CLKDivider = (uint32_t)((RCC->CFGR & RCC_CFGR_PPRE2) >> 3);

#if   defined(FLASH_ACR_LATENCY)
  /* 获取 Flash 等待周期 (Latency) 配置 ------------------------*/
  *pFLatency = (uint32_t)(FLASH->ACR & FLASH_ACR_LATENCY);
#else
  /* 对于VALUE系列的芯片，只能设置LATENCY_0 */
  *pFLatency = (uint32_t)FLASH_LATENCY_0;
#endif
}

/**
  * @brief 处理 RCC CSS（时钟安全系统）中断请求的函数
  * @note 这个API应该在NMI_Handler()下调用。
  * @retval None
  */
void HAL_RCC_NMI_IRQHandler(void)
{
  /* 检查 RCC CSSF 标志  */
  if (__HAL_RCC_GET_IT(RCC_IT_CSS))
  {
    /* RCC 时钟安全系统中断用户回调 */
    HAL_RCC_CSSCallback();

    /* 清除 RCC CSS 挂起位 */
    __HAL_RCC_CLEAR_IT(RCC_IT_CSS);
  }
}

/**
  * @brief  基于CPU周期方法的延迟(毫秒)
  * @param  mdelay: 指定延迟时间长度，单位为毫秒。
  * @retval None
  */
static void RCC_Delay(uint32_t mdelay)
{
  __IO uint32_t Delay = mdelay * (SystemCoreClock / 8U / 1000U);
  do
  {
    __NOP();
  }
  while (Delay --);
}

/**
  * @brief  RCC时钟安全系统中断回调
  * @retval none
  */
__weak void HAL_RCC_CSSCallback(void)
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

#endif /* HAL_RCC_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

 