/**
  ******************************************************************************
  * @file    stm32f1xx_hal_rcc_ex.c
  * @author  MCD Application Team
  * @brief   HAL库 RCC 扩展模块驱动
  *          这个文件提供固件函数，用于管理 RCC 外设扩展功能：
  *           + 扩展外设控制功能
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

#ifdef HAL_RCC_MODULE_ENABLED

/** @defgroup RCCEx RCCEx
  * @brief RCC HAL 扩展模块驱动
  * @{
  */

/* 私有类型 -----------------------------------------------------------*/
/* 私有定义 ------------------------------------------------------------*/
/** @defgroup RCCEx_Private_Constants RCCEx 私有常量
  * @{
  */
/**
  * @}
  */

/* 私有宏 -------------------------------------------------------------*/
/** @defgroup RCCEx_Private_Macros RCCEx 私有宏
  * @{
  */
/**
  * @}
  */

/* 私有变量 ---------------------------------------------------------*/
/* 私有函数原型 -----------------------------------------------*/
/* 私有函数 ---------------------------------------------------------*/

/** @defgroup RCCEx_Exported_Functions RCCEx 扩展函数
  * @{
  */

/** @defgroup RCCEx_Exported_Functions_Group1 外设控制函数
  *  @brief  扩展外设控制函数
  *
@verbatim
 ===============================================================================
                ##### 扩展外设控制函数  #####
 ===============================================================================
    [..]
    这个小节提供了一组函数，允许控制RCC时钟频率。
    [..]
    (@) 重要提示：在使用HAL_RCCEx_PeriphCLKConfig()选择RTC时钟源时，必须小心；
        在这种情况下，备份域将被重置以修改RTC时钟源，
        因此RTC寄存器（包括备份寄存器）将被设置为它们的复位值。

@endverbatim
  * @{
  */

/**
  * @brief  初始化RCC扩展外设时钟（根据 RCC_PeriphCLKInitTypeDef 中指定的参数）
  * @param  PeriphClkInit 指向包含扩展外设时钟（RTC时钟）配置信息的RCC_PeriphCLKInitTypeDef结构体的指针。
  *
  * @note   在使用HAL_RCCEx_PeriphCLKConfig()选择RTC时钟源时，必须小心；
  *         在这种情况下，备份域将被重置以修改RTC时钟源，因此RTC寄存器（包括备份寄存器）将被设置为它们的复位值。
  *
  * @note   对于STM32F105xC或STM32F107xC设备，如果在2个I2S接口中请求了PLL I2S，则PLL I2S将被启用。
  *         当启用PLL I2S时，您需要调用HAL_RCCEx_DisablePLLI2S手动禁用它。
  *
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit)
{
  uint32_t tickstart = 0U, temp_reg = 0U;
#if defined(STM32F105xC) || defined(STM32F107xC)
  uint32_t  pllactive = 0U;
#endif /* STM32F105xC || STM32F107xC */

  /* 检查参数 */
  assert_param(IS_RCC_PERIPHCLOCK(PeriphClkInit->PeriphClockSelection));

  /*------------------------------- RTC/LCD 配置 ------------------------*/
  if ((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_RTC) == RCC_PERIPHCLK_RTC))
  {
    FlagStatus pwrclkchanged = RESET;

    /* 检查用于输出RTCCLK的RTC参数 */
    assert_param(IS_RCC_RTCCLKSOURCE(PeriphClkInit->RTCClockSelection));

    /* 一旦调用函数以更改RTC时钟源，将激活电源域。 */
    /* 需要启用必要的备份域写访问权限 */
    if (__HAL_RCC_PWR_IS_CLK_DISABLED())
    {
      __HAL_RCC_PWR_CLK_ENABLE();
      pwrclkchanged = SET;
    }

    if (HAL_IS_BIT_CLR(PWR->CR, PWR_CR_DBP))
    {
      /*启用备份域的写访问权限 */
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

    /* 仅当从复位值修改RTC时钟源选择时，才重置备份域。 */
    temp_reg = (RCC->BDCR & RCC_BDCR_RTCSEL);
    if ((temp_reg != 0x00000000U) && (temp_reg != (PeriphClkInit->RTCClockSelection & RCC_BDCR_RTCSEL)))
    {
      /* 在重置备份域之前，存储BDCR（备份域控制）寄存器的内容 */
      temp_reg = (RCC->BDCR & ~(RCC_BDCR_RTCSEL));
      /* 只有在重置备份域后才能更改RTC时钟选择 */
      __HAL_RCC_BACKUPRESET_FORCE();
      __HAL_RCC_BACKUPRESET_RELEASE();
      /* 恢复BDCR（备份域控制）寄存器的内容 */
      RCC->BDCR = temp_reg;

      /* 如果LSE已启用，等待LSERDY（LSE就绪） */
      if (HAL_IS_BIT_SET(temp_reg, RCC_BDCR_LSEON))
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
    }
    __HAL_RCC_RTC_CONFIG(PeriphClkInit->RTCClockSelection);

    /* 如果需要，关闭电源时钟（关闭备份域写保护需要开电源区时钟） */
    if (pwrclkchanged == SET)
    {
      __HAL_RCC_PWR_CLK_DISABLE();
    }
  }

  /*------------------------------ ADC clock 配置 ------------------*/
  if (((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_ADC) == RCC_PERIPHCLK_ADC)
  {
    /* 检查参数 */
    assert_param(IS_RCC_ADCPLLCLK_DIV(PeriphClkInit->AdcClockSelection));

    /* 配置 ADC 时钟源 */
    __HAL_RCC_ADC_CONFIG(PeriphClkInit->AdcClockSelection);
  }

#if defined(STM32F105xC) || defined(STM32F107xC)
  /*------------------------------ I2S2 配置 ------------------------*/
  if (((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2S2) == RCC_PERIPHCLK_I2S2)
  {
    /* 检查参数 */
    assert_param(IS_RCC_I2S2CLKSOURCE(PeriphClkInit->I2s2ClockSelection));

    /* 配置 I2S2 时钟源 */
    __HAL_RCC_I2S2_CONFIG(PeriphClkInit->I2s2ClockSelection);
  }

  /*------------------------------ I2S3 配置 ------------------------*/
  if (((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2S3) == RCC_PERIPHCLK_I2S3)
  {
    /* 检查参数 */
    assert_param(IS_RCC_I2S3CLKSOURCE(PeriphClkInit->I2s3ClockSelection));

    /* 配置 I2S3 时钟源 */
    __HAL_RCC_I2S3_CONFIG(PeriphClkInit->I2s3ClockSelection);
  }

  /*------------------------------ PLL I2S 配置 ----------------------*/
  /* 检查是否需要启用PLL I2S */
  if (HAL_IS_BIT_SET(RCC->CFGR2, RCC_CFGR2_I2S2SRC) || HAL_IS_BIT_SET(RCC->CFGR2, RCC_CFGR2_I2S3SRC))
  {
    /* 更新标志以指示应激活PLLI2S */
    pllactive = 1;
  }

  /* 检查是否需要启用PLL I2S */
  if (pllactive == 1)
  {
    /* 仅在未激活时启用PLLI2S（PLL3） */
    if (HAL_IS_BIT_CLR(RCC->CR, RCC_CR_PLL3ON))
    {
      /* 检查参数 */
      assert_param(IS_RCC_PLLI2S_MUL(PeriphClkInit->PLLI2S.PLLI2SMUL));
      assert_param(IS_RCC_HSE_PREDIV2(PeriphClkInit->PLLI2S.HSEPrediv2Value));

      /* 只有在PLL2被禁用时才能写入HSE预分频Prediv2 */
      /* 仅当新值与已编程值不同时返回错误  */
      if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL2ON) && \
          (__HAL_RCC_HSE_GET_PREDIV2() != PeriphClkInit->PLLI2S.HSEPrediv2Value))
      {
        return HAL_ERROR;
      }

      /* 配置 HSE 预分频因子prediv2 --------------------------------*/
      __HAL_RCC_HSE_PREDIV2_CONFIG(PeriphClkInit->PLLI2S.HSEPrediv2Value);

      /* 配置 PLLI2S（PLL3）的倍频因子 */
      __HAL_RCC_PLLI2S_CONFIG(PeriphClkInit->PLLI2S.PLLI2SMUL);

      /* 启用 PLLI2S（PLL3）. */
      __HAL_RCC_PLLI2S_ENABLE();

      /* 获取起始滴答值*/
      tickstart = HAL_GetTick();

      /* 等待 PLLI2S（PLL3） 就绪 */
      while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLLI2SRDY)  == RESET)
      {
        if ((HAL_GetTick() - tickstart) > PLLI2S_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
    }
    else
    {
      /* 仅当用户希望更改PLLI2SMUL（PLL3倍频系数）而PLLI2S（PLL3）处于活动状态时返回错误 */
      if (READ_BIT(RCC->CFGR2, RCC_CFGR2_PLL3MUL) != PeriphClkInit->PLLI2S.PLLI2SMUL)
      {
        return HAL_ERROR;
      }
    }
  }
#endif /* STM32F105xC || STM32F107xC */

#if defined(STM32F102x6) || defined(STM32F102xB) || defined(STM32F103x6)\
 || defined(STM32F103xB) || defined(STM32F103xE) || defined(STM32F103xG)\
 || defined(STM32F105xC) || defined(STM32F107xC)
  /*------------------------------ USB 时钟配置 ------------------*/
  if (((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USB) == RCC_PERIPHCLK_USB)
  {
    /* 检查参数 */
    assert_param(IS_RCC_USBPLLCLK_DIV(PeriphClkInit->UsbClockSelection));

    /* 配置 USB 时钟源 */
    __HAL_RCC_USB_CONFIG(PeriphClkInit->UsbClockSelection);
  }
#endif /* STM32F102x6 || STM32F102xB || STM32F103x6 || STM32F103xB || STM32F103xE || STM32F103xG || STM32F105xC || STM32F107xC */

  return HAL_OK;
}

/**
  * @brief  获取外设时钟配置（根据内部RCC配置寄存器）
  * @param  PeriphClkInit 指向一个RCC_PeriphCLKInitTypeDef结构的指针，该结构返回扩展外设时钟（RTC、I2S、ADC时钟）的配置信息。
  * @retval None
  */
void HAL_RCCEx_GetPeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit)
{
  uint32_t srcclk = 0U;

  /* 设置扩展时钟类型参数的所有可能值------------*/
  PeriphClkInit->PeriphClockSelection = RCC_PERIPHCLK_RTC;

  /* 获取 RTC 配置 -----------------------------------------------*/
  srcclk = __HAL_RCC_GET_RTC_SOURCE();
  /* 时钟源是 LSE 或 LSI */
  PeriphClkInit->RTCClockSelection = srcclk;

  /* 获取 ADC 时钟配置 -----------------------------------------*/
  PeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_ADC;
  PeriphClkInit->AdcClockSelection = __HAL_RCC_GET_ADC_SOURCE();

#if defined(STM32F105xC) || defined(STM32F107xC)
  /* 获取 I2S2 时钟配置 -----------------------------------------*/
  PeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_I2S2;
  PeriphClkInit->I2s2ClockSelection = __HAL_RCC_GET_I2S2_SOURCE();

  /* 获取 I2S3 时钟配置 -----------------------------------------*/
  PeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_I2S3;
  PeriphClkInit->I2s3ClockSelection = __HAL_RCC_GET_I2S3_SOURCE();

#endif /* STM32F105xC || STM32F107xC */

#if defined(STM32F103xE) || defined(STM32F103xG)
  /* 获取 I2S2 时钟配置 -----------------------------------------*/
  PeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_I2S2;
  PeriphClkInit->I2s2ClockSelection = RCC_I2S2CLKSOURCE_SYSCLK;

  /* 获取 I2S3 时钟配置 -----------------------------------------*/
  PeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_I2S3;
  PeriphClkInit->I2s3ClockSelection = RCC_I2S3CLKSOURCE_SYSCLK;

#endif /* STM32F103xE || STM32F103xG */

#if defined(STM32F102x6) || defined(STM32F102xB) || defined(STM32F103x6)\
 || defined(STM32F103xB) || defined(STM32F103xE) || defined(STM32F103xG)\
 || defined(STM32F105xC) || defined(STM32F107xC)
  /* 获取 USB 时钟配置 -----------------------------------------*/
  PeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_USB;
  PeriphClkInit->UsbClockSelection = __HAL_RCC_GET_USB_SOURCE();
#endif /* STM32F102x6 || STM32F102xB || STM32F103x6 || STM32F103xB || STM32F103xE || STM32F103xG || STM32F105xC || STM32F107xC */
}

/**
  * @brief  返回外设时钟频率
  * @note   如果外设时钟未知则返回0
  * @param  PeriphClk 外设时钟标识符
  *         该参数可以是以下值之一:
  *            @arg @ref RCC_PERIPHCLK_RTC  RTC 外设时钟
  *            @arg @ref RCC_PERIPHCLK_ADC  ADC 外设时钟
  @if STM32F103xE
  *            @arg @ref RCC_PERIPHCLK_I2S2 I2S2 外设时钟
  *            @arg @ref RCC_PERIPHCLK_I2S3 I2S3 外设时钟
  *            @arg @ref RCC_PERIPHCLK_I2S3 I2S3 外设时钟
  @endif
  @if STM32F103xG
  *            @arg @ref RCC_PERIPHCLK_I2S2 I2S2 外设时钟
  *            @arg @ref RCC_PERIPHCLK_I2S3 I2S3 外设时钟
  *            @arg @ref RCC_PERIPHCLK_I2S3 I2S3 外设时钟
  *            @arg @ref RCC_PERIPHCLK_I2S2 I2S2 外设时钟
  @endif
  @if STM32F105xC
  *            @arg @ref RCC_PERIPHCLK_I2S2 I2S2 外设时钟
  *            @arg @ref RCC_PERIPHCLK_I2S3 I2S3 外设时钟
  *            @arg @ref RCC_PERIPHCLK_I2S3 I2S3 外设时钟
  *            @arg @ref RCC_PERIPHCLK_I2S2 I2S2 外设时钟
  *            @arg @ref RCC_PERIPHCLK_I2S3 I2S3 外设时钟
  *            @arg @ref RCC_PERIPHCLK_I2S3 I2S3 外设时钟
  *            @arg @ref RCC_PERIPHCLK_I2S2 I2S2 外设时钟
  *            @arg @ref RCC_PERIPHCLK_USB  USB 外设时钟
  @endif
  @if STM32F107xC
  *            @arg @ref RCC_PERIPHCLK_I2S2 I2S2 外设时钟
  *            @arg @ref RCC_PERIPHCLK_I2S3 I2S3 外设时钟
  *            @arg @ref RCC_PERIPHCLK_I2S3 I2S3 外设时钟
  *            @arg @ref RCC_PERIPHCLK_I2S2 I2S2 外设时钟
  *            @arg @ref RCC_PERIPHCLK_I2S3 I2S3 外设时钟
  *            @arg @ref RCC_PERIPHCLK_I2S3 I2S3 外设时钟
  *            @arg @ref RCC_PERIPHCLK_I2S2 I2S2 外设时钟
  *            @arg @ref RCC_PERIPHCLK_USB  USB 外设时钟
  @endif
  @if STM32F102xx
  *            @arg @ref RCC_PERIPHCLK_USB  USB 外设时钟
  @endif
  @if STM32F103xx
  *            @arg @ref RCC_PERIPHCLK_USB  USB 外设时钟
  @endif
  * @retval 频率 单位 Hz（0：表示外设没有可用的频率）
  */
uint32_t HAL_RCCEx_GetPeriphCLKFreq(uint32_t PeriphClk)
{
#if defined(STM32F105xC) || defined(STM32F107xC)
  static const uint8_t aPLLMULFactorTable[14U] = {0, 0, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 13};
  static const uint8_t aPredivFactorTable[16U] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

  uint32_t prediv1 = 0U, pllclk = 0U, pllmul = 0U;
  uint32_t pll2mul = 0U, pll3mul = 0U, prediv2 = 0U;
#endif /* STM32F105xC || STM32F107xC */
#if defined(STM32F102x6) || defined(STM32F102xB) || defined(STM32F103x6) || \
    defined(STM32F103xB) || defined(STM32F103xE) || defined(STM32F103xG)
  static const uint8_t aPLLMULFactorTable[16U] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 16};
  static const uint8_t aPredivFactorTable[2U] = {1, 2};

  uint32_t prediv1 = 0U, pllclk = 0U, pllmul = 0U;
#endif /* STM32F102x6 || STM32F102xB || STM32F103x6 || STM32F103xB || STM32F103xE || STM32F103xG */
  uint32_t temp_reg = 0U, frequency = 0U;

  /* 检查参数 */
  assert_param(IS_RCC_PERIPHCLOCK(PeriphClk));

  switch (PeriphClk)
  {
#if defined(STM32F102x6) || defined(STM32F102xB) || defined(STM32F103x6)\
 || defined(STM32F103xB) || defined(STM32F103xE) || defined(STM32F103xG)\
 || defined(STM32F105xC) || defined(STM32F107xC)
    case RCC_PERIPHCLK_USB:
    {
      /* 获取 RCC 配置 ------------------------------------------------------*/
      temp_reg = RCC->CFGR;

      /* 检查PLL是否启用 */
      if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLLON))
      {
        pllmul = aPLLMULFactorTable[(uint32_t)(temp_reg & RCC_CFGR_PLLMULL) >> RCC_CFGR_PLLMULL_Pos];
        if ((temp_reg & RCC_CFGR_PLLSRC) != RCC_PLLSOURCE_HSI_DIV2)
        {
#if defined(STM32F105xC) || defined(STM32F107xC) || defined(STM32F100xB)\
 || defined(STM32F100xE)
          prediv1 = aPredivFactorTable[(uint32_t)(RCC->CFGR2 & RCC_CFGR2_PREDIV1) >> RCC_CFGR2_PREDIV1_Pos];
#else
          prediv1 = aPredivFactorTable[(uint32_t)(RCC->CFGR & RCC_CFGR_PLLXTPRE) >> RCC_CFGR_PLLXTPRE_Pos];
#endif /* STM32F105xC || STM32F107xC || STM32F100xB || STM32F100xE */

#if defined(STM32F105xC) || defined(STM32F107xC)
          if (HAL_IS_BIT_SET(RCC->CFGR2, RCC_CFGR2_PREDIV1SRC))
          {
            /* PLL2 被选中作为预分频器1（Prediv1）的时钟源 */
            /* PLLCLK = PLL2CLK / PREDIV1 * PLLMUL 使用 PLL2CLK = HSE/PREDIV2 * PLL2MUL */
            prediv2 = ((RCC->CFGR2 & RCC_CFGR2_PREDIV2) >> RCC_CFGR2_PREDIV2_Pos) + 1;
            pll2mul = ((RCC->CFGR2 & RCC_CFGR2_PLL2MUL) >> RCC_CFGR2_PLL2MUL_Pos) + 2;
            pllclk = (uint32_t)((((HSE_VALUE / prediv2) * pll2mul) / prediv1) * pllmul);
          }
          else
          {
            /* HSE 作为 PLL 时钟源 : PLLCLK = HSE/PREDIV1 * PLLMUL */
            pllclk = (uint32_t)((HSE_VALUE / prediv1) * pllmul);
          }

          /* 如果PLLMUL设置为13，意味着它是为了覆盖PLLMUL 6.5的情况（避免使用浮点数） */
          /* 在这种情况下，需要将pllclk除以2 */
          if (pllmul == aPLLMULFactorTable[(uint32_t)(RCC_CFGR_PLLMULL6_5) >> RCC_CFGR_PLLMULL_Pos])
          {
            pllclk = pllclk / 2;
          }
#else
          if ((temp_reg & RCC_CFGR_PLLSRC) != RCC_PLLSOURCE_HSI_DIV2)
          {
            /* HSE 作为 PLL 时钟源 : PLLCLK = HSE/PREDIV1 * PLLMUL */
            pllclk = (uint32_t)((HSE_VALUE / prediv1) * pllmul);
          }
#endif /* STM32F105xC || STM32F107xC */
        }
        else
        {
          /* HSI 作为 PLL 时钟源 : PLLCLK = HSI/2 * PLLMUL */
          pllclk = (uint32_t)((HSI_VALUE >> 1) * pllmul);
        }

        /* 计算USB频率*/
#if defined(STM32F105xC) || defined(STM32F107xC)
        /* USBCLK = PLLVCO = (2 x PLLCLK) / USB 预分频 */
        if (__HAL_RCC_GET_USB_SOURCE() == RCC_USBCLKSOURCE_PLL_DIV2)
        {
          /* 为USB选择了2分频 */
          frequency = pllclk;
        }
        else
        {
          /* 为USB选择了3分频 */
          frequency = (2 * pllclk) / 3;
        }
#else
        /* USBCLK = PLLCLK / USB 预分频 */
        if (__HAL_RCC_GET_USB_SOURCE() == RCC_USBCLKSOURCE_PLL)
        {
          /* USB未选择预分频 */
          frequency = pllclk;
        }
        else
        {
          /* USB选择了1.5预分频 */
          frequency = (pllclk * 2) / 3;
        }
#endif
      }
      break;
    }
#endif /* STM32F102x6 || STM32F102xB || STM32F103x6 || STM32F103xB || STM32F103xE || STM32F103xG || STM32F105xC || STM32F107xC */
#if defined(STM32F103xE) || defined(STM32F103xG) || defined(STM32F105xC) || defined(STM32F107xC)
    case RCC_PERIPHCLK_I2S2:
    {
#if defined(STM32F103xE) || defined(STM32F103xG)
      /* SYSCLK用作I2S2的时钟源 */
      frequency = HAL_RCC_GetSysClockFreq();
#else
      if (__HAL_RCC_GET_I2S2_SOURCE() == RCC_I2S2CLKSOURCE_SYSCLK)
      {
        /* SYSCLK用作I2S2的时钟源 */
        frequency = HAL_RCC_GetSysClockFreq();
      }
      else
      {
        /* 检查 PLLI2S（PLL3）是否启用 */
        if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL3ON))
        {
          /* PLLI2SVCO = 2 * PLLI2SCLK = 2 * (HSE/PREDIV2 * PLL3MUL) */
          prediv2 = ((RCC->CFGR2 & RCC_CFGR2_PREDIV2) >> RCC_CFGR2_PREDIV2_Pos) + 1;
          pll3mul = ((RCC->CFGR2 & RCC_CFGR2_PLL3MUL) >> RCC_CFGR2_PLL3MUL_Pos) + 2;
          frequency = (uint32_t)(2 * ((HSE_VALUE / prediv2) * pll3mul));
        }
      }
#endif /* STM32F103xE || STM32F103xG */
      break;
    }
    case RCC_PERIPHCLK_I2S3:
    {
#if defined(STM32F103xE) || defined(STM32F103xG)
      /* SYSCLK用作I2S3的时钟源 */
      frequency = HAL_RCC_GetSysClockFreq();
#else
      if (__HAL_RCC_GET_I2S3_SOURCE() == RCC_I2S3CLKSOURCE_SYSCLK)
      {
        /* SYSCLK用作I2S3的时钟源 */
        frequency = HAL_RCC_GetSysClockFreq();
      }
      else
      {
        /* 检查 PLLI2S（PLL3）是否启用 */
        if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL3ON))
        {
          /* PLLI2SVCO = 2 * PLLI2SCLK = 2 * (HSE/PREDIV2 * PLL3MUL) */
          prediv2 = ((RCC->CFGR2 & RCC_CFGR2_PREDIV2) >> RCC_CFGR2_PREDIV2_Pos) + 1;
          pll3mul = ((RCC->CFGR2 & RCC_CFGR2_PLL3MUL) >> RCC_CFGR2_PLL3MUL_Pos) + 2;
          frequency = (uint32_t)(2 * ((HSE_VALUE / prediv2) * pll3mul));
        }
      }
#endif /* STM32F103xE || STM32F103xG */
      break;
    }
#endif /* STM32F103xE || STM32F103xG || STM32F105xC || STM32F107xC */
    case RCC_PERIPHCLK_RTC:
    {
      /* 获取 RCC BDCR（备份域控制寄存器）配置 ------------------------------------------------------*/
      temp_reg = RCC->BDCR;

      /* 如果RTC时钟选择是LSE，检查LSE是否就绪 */
      if (((temp_reg & RCC_BDCR_RTCSEL) == RCC_RTCCLKSOURCE_LSE) && (HAL_IS_BIT_SET(temp_reg, RCC_BDCR_LSERDY)))
      {
        frequency = LSE_VALUE;
      }
      /* 如果RTC时钟选择为LSI，检查LSI是否就绪 */
      else if (((temp_reg & RCC_BDCR_RTCSEL) == RCC_RTCCLKSOURCE_LSI) && (HAL_IS_BIT_SET(RCC->CSR, RCC_CSR_LSIRDY)))
      {
        frequency = LSI_VALUE;
      }
      else if (((temp_reg & RCC_BDCR_RTCSEL) == RCC_RTCCLKSOURCE_HSE_DIV128) && (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY)))
      {
        frequency = HSE_VALUE / 128U;
      }
      /* RTC的时钟未启用 */
      else
      {
        /* 无事可做:频率已初始化为0U */
      }
      break;
    }
    case RCC_PERIPHCLK_ADC:
    {
      frequency = HAL_RCC_GetPCLK2Freq() / (((__HAL_RCC_GET_ADC_SOURCE() >> RCC_CFGR_ADCPRE_Pos) + 1) * 2);
      break;
    }
    default:
    {
      break;
    }
  }
  return (frequency);
}

/**
  * @}
  */

#if defined(STM32F105xC) || defined(STM32F107xC)
/** @defgroup RCCEx_Exported_Functions_Group2 PLLI2S 管理函数
  *  @brief  PLLI2S 管理函数
  *
@verbatim
 ===============================================================================
                ##### 扩展 PLLI2S 管理函数  #####
 ===============================================================================
    [..]
    本小节提供了一组函数，用于控制PLLI2S的激活或反激活
@endverbatim
  * @{
  */

/**
  * @brief  启用 PLLI2S
  * @param  PLLI2SInit 指向RCC_PLLI2SInitTypeDef结构的指针，该结构包含PLLI2S的配置信息
  * @note   如果被I2S2或I2S3接口使用，则不修改PLLI2S配置
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_EnablePLLI2S(RCC_PLLI2SInitTypeDef  *PLLI2SInit)
{
  uint32_t tickstart = 0U;

  /* 检查PLLI2S（PLL3）是否已经被I2S2或I2S3启用 */
  if (HAL_IS_BIT_CLR(RCC->CFGR2, RCC_CFGR2_I2S2SRC) && HAL_IS_BIT_CLR(RCC->CFGR2, RCC_CFGR2_I2S3SRC))
  {
    /* 检查参数 */
    assert_param(IS_RCC_PLLI2S_MUL(PLLI2SInit->PLLI2SMUL));
    assert_param(IS_RCC_HSE_PREDIV2(PLLI2SInit->HSEPrediv2Value));

    /* 只有在PLL2被禁用时才能写入预分频器Prediv2 */
    /* 仅当新值与已编程值不同时返回错误 */
    if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL2ON) && \
        (__HAL_RCC_HSE_GET_PREDIV2() != PLLI2SInit->HSEPrediv2Value))
    {
      return HAL_ERROR;
    }

    /* 禁用 PLLI2S（PLL3）. */
    __HAL_RCC_PLLI2S_DISABLE();

    /* 获取起始滴答值*/
    tickstart = HAL_GetTick();

    /* 等待 PLLI2S （PLL3）就绪 */
    while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLLI2SRDY)  != RESET)
    {
      if ((HAL_GetTick() - tickstart) > PLLI2S_TIMEOUT_VALUE)
      {
        return HAL_TIMEOUT;
      }
    }

    /* 配置 HSE 预分频器 prediv2 --------------------------------*/
    __HAL_RCC_HSE_PREDIV2_CONFIG(PLLI2SInit->HSEPrediv2Value);


    /* 配置 PLLI2S（PLL3）倍频因数 */
    __HAL_RCC_PLLI2S_CONFIG(PLLI2SInit->PLLI2SMUL);

    /* 启用 PLLI2S（PLL3）. */
    __HAL_RCC_PLLI2S_ENABLE();

    /* 获取起始滴答值*/
    tickstart = HAL_GetTick();

    /* 等待 PLLI2S（PLL3）就绪 */
    while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLLI2SRDY)  == RESET)
    {
      if ((HAL_GetTick() - tickstart) > PLLI2S_TIMEOUT_VALUE)
      {
        return HAL_TIMEOUT;
      }
    }
  }
  else
  {
    /* 由于已被I2S2或I2S3使用，无法修改PLLI2S（PLL3） */
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  禁用 PLLI2S
  * @note   如果被I2S2或I2S3接口使用，则无法禁用PLLI2S（PLL3）
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_DisablePLLI2S(void)
{
  uint32_t tickstart = 0U;

  /* 根据I2S2或I2S3的需求，禁用PLLI2S（PLL3）*/
  if (HAL_IS_BIT_CLR(RCC->CFGR2, RCC_CFGR2_I2S2SRC) && HAL_IS_BIT_CLR(RCC->CFGR2, RCC_CFGR2_I2S3SRC))
  {
    /* 禁用 PLLI2S（PLL3） */
    __HAL_RCC_PLLI2S_DISABLE();

    /* 获取起始滴答值*/
    tickstart = HAL_GetTick();

    /* 等待 PLLI2S（PLL3）就绪 */
    while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLLI2SRDY)  != RESET)
    {
      if ((HAL_GetTick() - tickstart) > PLLI2S_TIMEOUT_VALUE)
      {
        return HAL_TIMEOUT;
      }
    }
  }
  else
  {
    /* PLLI2S（PLL3）目前被I2S2或I2S3使用，无法被禁用。*/
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @}
  */

/** @defgroup RCCEx_Exported_Functions_Group3 PLL2 管理函数
  *  @brief  PLL2 管理函数
  *
@verbatim
 ===============================================================================
                ##### 扩展 PLL2 管理函数  #####
 ===============================================================================
    [..]
    本小节提供了一组函数，用于控制PLL2的激活或反激活
@endverbatim
  * @{
  */

/**
  * @brief  启用 PLL2
  * @param  PLL2Init 指向RCC_PLL2InitTypeDef结构的指针，该结构包含PLL2的配置信息
  * @note   如果PLL2间接作为系统时钟的使用，则不修改PLL2配置
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_EnablePLL2(RCC_PLL2InitTypeDef  *PLL2Init)
{
  uint32_t tickstart = 0U;

  /* 如果PLL2时钟间接用作系统时钟（即用作系统时钟的PLL时钟输入），则无法清除此位。 */
  if ((__HAL_RCC_GET_PLL_OSCSOURCE() == RCC_PLLSOURCE_HSE) && \
      (__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_SYSCLKSOURCE_STATUS_PLLCLK) && \
      ((READ_BIT(RCC->CFGR2, RCC_CFGR2_PREDIV1SRC)) == RCC_CFGR2_PREDIV1SRC_PLL2))
  {
    return HAL_ERROR;
  }
  else
  {
    /* 检查参数 */
    assert_param(IS_RCC_PLL2_MUL(PLL2Init->PLL2MUL));
    assert_param(IS_RCC_HSE_PREDIV2(PLL2Init->HSEPrediv2Value));

    /* 只有在禁用PLLI2S（PLL3）时才能写入Prediv2 */
    /* 仅当新值与已编程值不同时返回错误 */
    if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL3ON) && \
        (__HAL_RCC_HSE_GET_PREDIV2() != PLL2Init->HSEPrediv2Value))
    {
      return HAL_ERROR;
    }

    /* 禁用 PLL2. */
    __HAL_RCC_PLL2_DISABLE();

    /* 获取起始滴答值*/
    tickstart = HAL_GetTick();

    /* 等待 PLL2 禁用 */
    while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLL2RDY) != RESET)
    {
      if ((HAL_GetTick() - tickstart) > PLL2_TIMEOUT_VALUE)
      {
        return HAL_TIMEOUT;
      }
    }

    /* 配置 HSE 预分频器prediv2 --------------------------------*/
    __HAL_RCC_HSE_PREDIV2_CONFIG(PLL2Init->HSEPrediv2Value);

    /* 配置 PLL2 倍频 */
    __HAL_RCC_PLL2_CONFIG(PLL2Init->PLL2MUL);

    /* 启用 PLL2. */
    __HAL_RCC_PLL2_ENABLE();

    /* 获取起始滴答值*/
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

  return HAL_OK;
}

/**
  * @brief  禁用 PLL2
  * @note   如果间接作为系统时钟使用，PLL2不会被禁用
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_DisablePLL2(void)
{
  uint32_t tickstart = 0U;

  /* 如果PLL2时钟被间接用作系统时钟（即作为用作系统时钟的PLL时钟输入），则无法清除此位。 */
  if ((__HAL_RCC_GET_PLL_OSCSOURCE() == RCC_PLLSOURCE_HSE) && \
      (__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_SYSCLKSOURCE_STATUS_PLLCLK) && \
      ((READ_BIT(RCC->CFGR2, RCC_CFGR2_PREDIV1SRC)) == RCC_CFGR2_PREDIV1SRC_PLL2))
  {
    return HAL_ERROR;
  }
  else
  {
    /* 禁用 PLL2. */
    __HAL_RCC_PLL2_DISABLE();

    /* 获取起始滴答值*/
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

  return HAL_OK;
}

/**
  * @}
  */
#endif /* STM32F105xC || STM32F107xC */

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


