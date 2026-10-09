/**
  ******************************************************************************
  * @file    stm32f10x_rcc.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件提供 RCC 的所有固件函数。
  ******************************************************************************
  * @attention
  *
  * 本固件仅供指导之用，旨在为客户提供有关其产品的编码信息，以节省他们的时间。
  * 因此，对于因本固件的内容和/或客户将此处包含的编码信息
  * 与其产品结合使用而提出的任何索赔所造成的任何直接、间接或后果性损害，
  * STMicroelectronics 概不承担任何责任。
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f10x_rcc.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @defgroup RCC
  * @brief RCC 驱动模块
  * @{
  */ 

/** @defgroup RCC_Private_TypesDefinitions   RCC 私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup RCC_Private_Defines   RCC 私有宏定义
  * @{
  */

/* ------------ RCC 寄存器在别名区中的位地址 ----------- */
#define RCC_OFFSET                (RCC_BASE - PERIPH_BASE)

/* --- CR 寄存器 ---*/

/* HSION 位的别名写字地址 */
#define CR_OFFSET                 (RCC_OFFSET + 0x00)
#define HSION_BitNumber           0x00
#define CR_HSION_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (HSION_BitNumber * 4))

/* PLLON 位的别名写字地址 */
#define PLLON_BitNumber           0x18
#define CR_PLLON_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PLLON_BitNumber * 4))

#ifdef STM32F10X_CL
 /* PLL2ON 位的别名写字地址 */
 #define PLL2ON_BitNumber          0x1A
 #define CR_PLL2ON_BB              (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PLL2ON_BitNumber * 4))

 /* PLL3ON 位的别名写字地址 */
 #define PLL3ON_BitNumber          0x1C
 #define CR_PLL3ON_BB              (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PLL3ON_BitNumber * 4))
#endif /* STM32F10X_CL */ 

/* CSSON 位的别名写字地址 */
#define CSSON_BitNumber           0x13
#define CR_CSSON_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (CSSON_BitNumber * 4))

/* --- CFGR 寄存器 ---*/

/* USBPRE 位的别名写字地址 */
#define CFGR_OFFSET               (RCC_OFFSET + 0x04)

#ifndef STM32F10X_CL
 #define USBPRE_BitNumber          0x16
 #define CFGR_USBPRE_BB            (PERIPH_BB_BASE + (CFGR_OFFSET * 32) + (USBPRE_BitNumber * 4))
#else
 #define OTGFSPRE_BitNumber        0x16
 #define CFGR_OTGFSPRE_BB          (PERIPH_BB_BASE + (CFGR_OFFSET * 32) + (OTGFSPRE_BitNumber * 4))
#endif /* STM32F10X_CL */ 

/* --- BDCR 寄存器 ---*/

/* RTCEN 位的别名写字地址 */
#define BDCR_OFFSET               (RCC_OFFSET + 0x20)
#define RTCEN_BitNumber           0x0F
#define BDCR_RTCEN_BB             (PERIPH_BB_BASE + (BDCR_OFFSET * 32) + (RTCEN_BitNumber * 4))

/* BDRST 位的别名写字地址 */
#define BDRST_BitNumber           0x10
#define BDCR_BDRST_BB             (PERIPH_BB_BASE + (BDCR_OFFSET * 32) + (BDRST_BitNumber * 4))

/* --- CSR 寄存器 ---*/

/* LSION 位的别名写字地址 */
#define CSR_OFFSET                (RCC_OFFSET + 0x24)
#define LSION_BitNumber           0x00
#define CSR_LSION_BB              (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (LSION_BitNumber * 4))

#ifdef STM32F10X_CL
/* --- CFGR2 寄存器 ---*/

 /* I2S2SRC 位的别名写字地址 */
 #define CFGR2_OFFSET              (RCC_OFFSET + 0x2C)
 #define I2S2SRC_BitNumber         0x11
 #define CFGR2_I2S2SRC_BB          (PERIPH_BB_BASE + (CFGR2_OFFSET * 32) + (I2S2SRC_BitNumber * 4))

 /* I2S3SRC 位的别名写字地址 */
 #define I2S3SRC_BitNumber         0x12
 #define CFGR2_I2S3SRC_BB          (PERIPH_BB_BASE + (CFGR2_OFFSET * 32) + (I2S3SRC_BitNumber * 4))
#endif /* STM32F10X_CL */

/* ---------------------- RCC 寄存器位掩码 ------------------------ */

/* CR 寄存器位掩码 */
#define CR_HSEBYP_Reset           ((uint32_t)0xFFFBFFFF)
#define CR_HSEBYP_Set             ((uint32_t)0x00040000)
#define CR_HSEON_Reset            ((uint32_t)0xFFFEFFFF)
#define CR_HSEON_Set              ((uint32_t)0x00010000)
#define CR_HSITRIM_Mask           ((uint32_t)0xFFFFFF07)

/* CFGR 寄存器位掩码 */
#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) 
 #define CFGR_PLL_Mask            ((uint32_t)0xFFC2FFFF)
#else
 #define CFGR_PLL_Mask            ((uint32_t)0xFFC0FFFF)
#endif /* STM32F10X_CL */ 

#define CFGR_PLLMull_Mask         ((uint32_t)0x003C0000)
#define CFGR_PLLSRC_Mask          ((uint32_t)0x00010000)
#define CFGR_PLLXTPRE_Mask        ((uint32_t)0x00020000)
#define CFGR_SWS_Mask             ((uint32_t)0x0000000C)
#define CFGR_SW_Mask              ((uint32_t)0xFFFFFFFC)
#define CFGR_HPRE_Reset_Mask      ((uint32_t)0xFFFFFF0F)
#define CFGR_HPRE_Set_Mask        ((uint32_t)0x000000F0)
#define CFGR_PPRE1_Reset_Mask     ((uint32_t)0xFFFFF8FF)
#define CFGR_PPRE1_Set_Mask       ((uint32_t)0x00000700)
#define CFGR_PPRE2_Reset_Mask     ((uint32_t)0xFFFFC7FF)
#define CFGR_PPRE2_Set_Mask       ((uint32_t)0x00003800)
#define CFGR_ADCPRE_Reset_Mask    ((uint32_t)0xFFFF3FFF)
#define CFGR_ADCPRE_Set_Mask      ((uint32_t)0x0000C000)

/* CSR 寄存器位掩码 */
#define CSR_RMVF_Set              ((uint32_t)0x01000000)

#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) 
/* CFGR2 寄存器位掩码 */
 #define CFGR2_PREDIV1SRC         ((uint32_t)0x00010000)
 #define CFGR2_PREDIV1            ((uint32_t)0x0000000F)
#endif
#ifdef STM32F10X_CL
 #define CFGR2_PREDIV2            ((uint32_t)0x000000F0)
 #define CFGR2_PLL2MUL            ((uint32_t)0x00000F00)
 #define CFGR2_PLL3MUL            ((uint32_t)0x0000F000)
#endif /* STM32F10X_CL */ 

/* RCC 标志掩码 */
#define FLAG_Mask                 ((uint8_t)0x1F)

/* CIR 寄存器字节 2（位 [15:8]）基地址 */
#define CIR_BYTE2_ADDRESS         ((uint32_t)0x40021009)

/* CIR 寄存器字节 3（位 [23:16]）基地址 */
#define CIR_BYTE3_ADDRESS         ((uint32_t)0x4002100A)

/* CFGR 寄存器字节 4（位 [31:24]）基地址 */
#define CFGR_BYTE4_ADDRESS        ((uint32_t)0x40021007)

/* BDCR 寄存器基地址 */
#define BDCR_ADDRESS              (PERIPH_BASE + BDCR_OFFSET)

/**
  * @}
  */ 

/** @defgroup RCC_Private_Macros   RCC 私有宏
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup RCC_Private_Variables   RCC 私有变量
  * @{
  */ 

static __I uint8_t APBAHBPrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};
static __I uint8_t ADCPrescTable[4] = {2, 4, 6, 8};

/**
  * @}
  */

/** @defgroup RCC_Private_FunctionPrototypes   RCC 私有函数原型
  * @{
  */

/**
  * @}
  */

/** @defgroup RCC_Private_Functions   RCC 私有函数
  * @{
  */

/**
  * @brief  将 RCC 时钟配置复位为默认复位状态。
  * @param  None
  * @retval 无
  */
void RCC_DeInit(void)
{
  /* 设置 HSION 位 */
  RCC->CR |= (uint32_t)0x00000001;

  /* 复位 SW、HPRE、PPRE1、PPRE2、ADCPRE 和 MCO 位 */
#ifndef STM32F10X_CL
  RCC->CFGR &= (uint32_t)0xF8FF0000;
#else
  RCC->CFGR &= (uint32_t)0xF0FF0000;
#endif /* STM32F10X_CL */   
  
  /* 复位 HSEON、CSSON 和 PLLON 位 */
  RCC->CR &= (uint32_t)0xFEF6FFFF;

  /* 复位 HSEBYP 位 */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* 复位 PLLSRC、PLLXTPRE、PLLMUL 和 USBPRE/OTGFSPRE 位 */
  RCC->CFGR &= (uint32_t)0xFF80FFFF;

#ifdef STM32F10X_CL
  /* 复位 PLL2ON 和 PLL3ON 位 */
  RCC->CR &= (uint32_t)0xEBFFFFFF;

  /* 关闭所有中断并清除挂起位  */
  RCC->CIR = 0x00FF0000;

  /* 复位 CFGR2 寄存器 */
  RCC->CFGR2 = 0x00000000;
#elif defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
  /* 关闭所有中断并清除挂起位  */
  RCC->CIR = 0x009F0000;

  /* 复位 CFGR2 寄存器 */
  RCC->CFGR2 = 0x00000000;      
#else
  /* 关闭所有中断并清除挂起位  */
  RCC->CIR = 0x009F0000;
#endif /* STM32F10X_CL */

}

/**
  * @brief  配置外部高速振荡器（HSE）。
  * @note   如果 HSE 被直接或通过 PLL 用作系统时钟，则不能停止它。
  * @param  RCC_HSE: 指定 HSE 的新状态。
  *   该参数可取以下值之一：
  *     @arg RCC_HSE_OFF: HSE 振荡器关闭
  *     @arg RCC_HSE_ON: HSE 振荡器开启
  *     @arg RCC_HSE_Bypass: HSE 振荡器由外部时钟旁路
  * @retval 无
  */
void RCC_HSEConfig(uint32_t RCC_HSE)
{
  /* 检查参数 */
  assert_param(IS_RCC_HSE(RCC_HSE));
  /* 在配置 HSE 之前复位 HSEON 和 HSEBYP 位 ------------------*/
  /* 复位 HSEON 位 */
  RCC->CR &= CR_HSEON_Reset;
  /* 复位 HSEBYP 位 */
  RCC->CR &= CR_HSEBYP_Reset;
  /* 配置 HSE（RCC_HSE_OFF 已由上面的代码段涵盖） */
  switch(RCC_HSE)
  {
    case RCC_HSE_ON:
      /* 设置 HSEON 位 */
      RCC->CR |= CR_HSEON_Set;
      break;
      
    case RCC_HSE_Bypass:
      /* 设置 HSEBYP 和 HSEON 位 */
      RCC->CR |= CR_HSEBYP_Set | CR_HSEON_Set;
      break;
      
    default:
      break;
  }
}

/**
  * @brief  等待 HSE 启动。
  * @param  None
  * @retval 一个 ErrorStatus 枚举值：
  * - SUCCESS: HSE 振荡器稳定且可以使用
  * - ERROR: HSE 振荡器尚未就绪
  */
ErrorStatus RCC_WaitForHSEStartUp(void)
{
  __IO uint32_t StartUpCounter = 0;
  ErrorStatus status = ERROR;
  FlagStatus HSEStatus = RESET;
  
  /* 等待 HSE 就绪，如果达到超时则退出 */
  do
  {
    HSEStatus = RCC_GetFlagStatus(RCC_FLAG_HSERDY);
    StartUpCounter++;  
  } while((StartUpCounter != HSE_STARTUP_TIMEOUT) && (HSEStatus == RESET));
  
  if (RCC_GetFlagStatus(RCC_FLAG_HSERDY) != RESET)
  {
    status = SUCCESS;
  }
  else
  {
    status = ERROR;
  }  
  return (status);
}

/**
  * @brief  调整内部高速振荡器（HSI）的校准值。
  * @param  HSICalibrationValue: 指定校准微调值。
  *   该参数必须是 0 到 0x1F 之间的数字。
  * @retval 无
  */
void RCC_AdjustHSICalibrationValue(uint8_t HSICalibrationValue)
{
  uint32_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_RCC_CALIBRATION_VALUE(HSICalibrationValue));
  tmpreg = RCC->CR;
  /* 清除 HSITRIM[4:0] 位 */
  tmpreg &= CR_HSITRIM_Mask;
  /* 根据 HSICalibrationValue 值设置 HSITRIM[4:0] 位 */
  tmpreg |= (uint32_t)HSICalibrationValue << 3;
  /* 保存新值 */
  RCC->CR = tmpreg;
}

/**
  * @brief  使能或关闭内部高速振荡器（HSI）。
  * @note   如果 HSI 被直接或通过 PLL 用作系统时钟，则不能停止它。
  * @param  NewState: HSI 的新状态。该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void RCC_HSICmd(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CR_HSION_BB = (uint32_t)NewState;
}

/**
  * @brief  配置 PLL 时钟源和倍频系数。
  * @note   本函数只能在 PLL 关闭时使用。
  * @param  RCC_PLLSource: 指定 PLL 输入时钟源。
  *   对于 @b STM32_Connectivity_line_devices 或 @b STM32_Value_line_devices，
  *   该参数可取以下值之一：
  *     @arg RCC_PLLSource_HSI_Div2: 选择 HSI 振荡器时钟除以 2 作为 PLL 时钟输入
  *     @arg RCC_PLLSource_PREDIV1: 选择 PREDIV1 时钟作为 PLL 时钟输入
  *   对于 @b other_STM32_devices，该参数可取以下值之一：
  *     @arg RCC_PLLSource_HSI_Div2: 选择 HSI 振荡器时钟除以 2 作为 PLL 时钟输入
  *     @arg RCC_PLLSource_HSE_Div1: 选择 HSE 振荡器时钟作为 PLL 时钟输入
  *     @arg RCC_PLLSource_HSE_Div2: 选择 HSE 振荡器时钟除以 2 作为 PLL 时钟输入
  * @param  RCC_PLLMul: 指定 PLL 倍频系数。
  *   对于 @b STM32_Connectivity_line_devices，该参数可为 RCC_PLLMul_x，其中 x:{[4,9], 6_5}
  *   对于 @b other_STM32_devices，该参数可为 RCC_PLLMul_x，其中 x:[2,16]
  * @retval 无
  */
void RCC_PLLConfig(uint32_t RCC_PLLSource, uint32_t RCC_PLLMul)
{
  uint32_t tmpreg = 0;

  /* 检查参数 */
  assert_param(IS_RCC_PLL_SOURCE(RCC_PLLSource));
  assert_param(IS_RCC_PLL_MUL(RCC_PLLMul));

  tmpreg = RCC->CFGR;
  /* 清除 PLLSRC、PLLXTPRE 和 PLLMUL[3:0] 位 */
  tmpreg &= CFGR_PLL_Mask;
  /* 设置 PLL 配置位 */
  tmpreg |= RCC_PLLSource | RCC_PLLMul;
  /* 保存新值 */
  RCC->CFGR = tmpreg;
}

/**
  * @brief  使能或关闭 PLL。
  * @note   如果 PLL 被用作系统时钟，则不能关闭它。
  * @param  NewState: PLL 的新状态。该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void RCC_PLLCmd(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(__IO uint32_t *) CR_PLLON_BB = (uint32_t)NewState;
}

#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL)
/**
  * @brief  配置 PREDIV1 分频系数。
  * @note
  *   - 本函数只能在 PLL 关闭时使用。
  *   - 本函数仅适用于 STM32 互联型和超值型器件。
  * @param  RCC_PREDIV1_Source: 指定 PREDIV1 时钟源。
  *   该参数可取以下值之一：
  *     @arg RCC_PREDIV1_Source_HSE: 选择 HSE 作为 PREDIV1 时钟
  *     @arg RCC_PREDIV1_Source_PLL2: 选择 PLL2 作为 PREDIV1 时钟
  * @note
  *   对于 @b STM32_Value_line_devices，该参数始终为 RCC_PREDIV1_Source_HSE
  * @param  RCC_PREDIV1_Div: 指定 PREDIV1 时钟分频系数。
  *   该参数可为 RCC_PREDIV1_Divx，其中 x:[1,16]
  * @retval 无
  */
void RCC_PREDIV1Config(uint32_t RCC_PREDIV1_Source, uint32_t RCC_PREDIV1_Div)
{
  uint32_t tmpreg = 0;
  
  /* 检查参数 */
  assert_param(IS_RCC_PREDIV1_SOURCE(RCC_PREDIV1_Source));
  assert_param(IS_RCC_PREDIV1(RCC_PREDIV1_Div));

  tmpreg = RCC->CFGR2;
  /* 清除 PREDIV1[3:0] 和 PREDIV1SRC 位 */
  tmpreg &= ~(CFGR2_PREDIV1 | CFGR2_PREDIV1SRC);
  /* 设置 PREDIV1 时钟源和分频系数 */
  tmpreg |= RCC_PREDIV1_Source | RCC_PREDIV1_Div ;
  /* 保存新值 */
  RCC->CFGR2 = tmpreg;
}
#endif

#ifdef STM32F10X_CL
/**
  * @brief  配置 PREDIV2 分频系数。
  * @note
  *   - 本函数只能在 PLL2 和 PLL3 均关闭时使用。
  *   - 本函数仅适用于 STM32 互联型器件。
  * @param  RCC_PREDIV2_Div: 指定 PREDIV2 时钟分频系数。
  *   该参数可为 RCC_PREDIV2_Divx，其中 x:[1,16]
  * @retval 无
  */
void RCC_PREDIV2Config(uint32_t RCC_PREDIV2_Div)
{
  uint32_t tmpreg = 0;

  /* 检查参数 */
  assert_param(IS_RCC_PREDIV2(RCC_PREDIV2_Div));

  tmpreg = RCC->CFGR2;
  /* 清除 PREDIV2[3:0] 位 */
  tmpreg &= ~CFGR2_PREDIV2;
  /* 设置 PREDIV2 分频系数 */
  tmpreg |= RCC_PREDIV2_Div;
  /* 保存新值 */
  RCC->CFGR2 = tmpreg;
}

/**
  * @brief  配置 PLL2 倍频系数。
  * @note
  *   - 本函数只能在 PLL2 关闭时使用。
  *   - 本函数仅适用于 STM32 互联型器件。
  * @param  RCC_PLL2Mul: 指定 PLL2 倍频系数。
  *   该参数可为 RCC_PLL2Mul_x，其中 x:{[8,14], 16, 20}
  * @retval 无
  */
void RCC_PLL2Config(uint32_t RCC_PLL2Mul)
{
  uint32_t tmpreg = 0;

  /* 检查参数 */
  assert_param(IS_RCC_PLL2_MUL(RCC_PLL2Mul));

  tmpreg = RCC->CFGR2;
  /* 清除 PLL2Mul[3:0] 位 */
  tmpreg &= ~CFGR2_PLL2MUL;
  /* 设置 PLL2 配置位 */
  tmpreg |= RCC_PLL2Mul;
  /* 保存新值 */
  RCC->CFGR2 = tmpreg;
}


/**
  * @brief  使能或关闭 PLL2。
  * @note
  *   - 如果 PLL2 被间接用作系统时钟（即它被用作 PLL 时钟输入，
  *     而该 PLL 又被用作系统时钟），则不能关闭它。
  *   - 本函数仅适用于 STM32 互联型器件。
  * @param  NewState: PLL2 的新状态。该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void RCC_PLL2Cmd(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(__IO uint32_t *) CR_PLL2ON_BB = (uint32_t)NewState;
}


/**
  * @brief  配置 PLL3 倍频系数。
  * @note
  *   - 本函数只能在 PLL3 关闭时使用。
  *   - 本函数仅适用于 STM32 互联型器件。
  * @param  RCC_PLL3Mul: 指定 PLL3 倍频系数。
  *   该参数可为 RCC_PLL3Mul_x，其中 x:{[8,14], 16, 20}
  * @retval 无
  */
void RCC_PLL3Config(uint32_t RCC_PLL3Mul)
{
  uint32_t tmpreg = 0;

  /* 检查参数 */
  assert_param(IS_RCC_PLL3_MUL(RCC_PLL3Mul));

  tmpreg = RCC->CFGR2;
  /* 清除 PLL3Mul[3:0] 位 */
  tmpreg &= ~CFGR2_PLL3MUL;
  /* 设置 PLL3 配置位 */
  tmpreg |= RCC_PLL3Mul;
  /* 保存新值 */
  RCC->CFGR2 = tmpreg;
}


/**
  * @brief  使能或关闭 PLL3。
  * @note   本函数仅适用于 STM32 互联型器件。
  * @param  NewState: PLL3 的新状态。该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void RCC_PLL3Cmd(FunctionalState NewState)
{
  /* 检查参数 */

  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CR_PLL3ON_BB = (uint32_t)NewState;
}
#endif /* STM32F10X_CL */

/**
  * @brief  配置系统时钟（SYSCLK）。
  * @param  RCC_SYSCLKSource: 指定用作系统时钟的时钟源。
  *   该参数可取以下值之一：
  *     @arg RCC_SYSCLKSource_HSI: 选择 HSI 作为系统时钟
  *     @arg RCC_SYSCLKSource_HSE: 选择 HSE 作为系统时钟
  *     @arg RCC_SYSCLKSource_PLLCLK: 选择 PLL 作为系统时钟
  * @retval 无
  */
void RCC_SYSCLKConfig(uint32_t RCC_SYSCLKSource)
{
  uint32_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_RCC_SYSCLK_SOURCE(RCC_SYSCLKSource));
  tmpreg = RCC->CFGR;
  /* 清除 SW[1:0] 位 */
  tmpreg &= CFGR_SW_Mask;
  /* 根据 RCC_SYSCLKSource 值设置 SW[1:0] 位 */
  tmpreg |= RCC_SYSCLKSource;
  /* 保存新值 */
  RCC->CFGR = tmpreg;
}

/**
  * @brief  返回用作系统时钟的时钟源。
  * @param  None
  * @retval 用作系统时钟的时钟源。返回值可以是以下之一：
  *     - 0x00: 使用 HSI 作为系统时钟
  *     - 0x04: 使用 HSE 作为系统时钟
  *     - 0x08: 使用 PLL 作为系统时钟
  */
uint8_t RCC_GetSYSCLKSource(void)
{
  return ((uint8_t)(RCC->CFGR & CFGR_SWS_Mask));
}

/**
  * @brief  配置 AHB 时钟（HCLK）。
  * @param  RCC_SYSCLK: 定义 AHB 时钟分频系数。该时钟由系统时钟（SYSCLK）派生。
  *   该参数可取以下值之一：
  *     @arg RCC_SYSCLK_Div1: AHB 时钟 = SYSCLK
  *     @arg RCC_SYSCLK_Div2: AHB 时钟 = SYSCLK/2
  *     @arg RCC_SYSCLK_Div4: AHB 时钟 = SYSCLK/4
  *     @arg RCC_SYSCLK_Div8: AHB 时钟 = SYSCLK/8
  *     @arg RCC_SYSCLK_Div16: AHB 时钟 = SYSCLK/16
  *     @arg RCC_SYSCLK_Div64: AHB 时钟 = SYSCLK/64
  *     @arg RCC_SYSCLK_Div128: AHB 时钟 = SYSCLK/128
  *     @arg RCC_SYSCLK_Div256: AHB 时钟 = SYSCLK/256
  *     @arg RCC_SYSCLK_Div512: AHB 时钟 = SYSCLK/512
  * @retval 无
  */
void RCC_HCLKConfig(uint32_t RCC_SYSCLK)
{
  uint32_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_RCC_HCLK(RCC_SYSCLK));
  tmpreg = RCC->CFGR;
  /* 清除 HPRE[3:0] 位 */
  tmpreg &= CFGR_HPRE_Reset_Mask;
  /* 根据 RCC_SYSCLK 值设置 HPRE[3:0] 位 */
  tmpreg |= RCC_SYSCLK;
  /* 保存新值 */
  RCC->CFGR = tmpreg;
}

/**
  * @brief  配置低速 APB 时钟（PCLK1）。
  * @param  RCC_HCLK: 定义 APB1 时钟分频系数。该时钟由 AHB 时钟（HCLK）派生。
  *   该参数可取以下值之一：
  *     @arg RCC_HCLK_Div1: APB1 时钟 = HCLK
  *     @arg RCC_HCLK_Div2: APB1 时钟 = HCLK/2
  *     @arg RCC_HCLK_Div4: APB1 时钟 = HCLK/4
  *     @arg RCC_HCLK_Div8: APB1 时钟 = HCLK/8
  *     @arg RCC_HCLK_Div16: APB1 时钟 = HCLK/16
  * @retval 无
  */
void RCC_PCLK1Config(uint32_t RCC_HCLK)
{
  uint32_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_RCC_PCLK(RCC_HCLK));
  tmpreg = RCC->CFGR;
  /* 清除 PPRE1[2:0] 位 */
  tmpreg &= CFGR_PPRE1_Reset_Mask;
  /* 根据 RCC_HCLK 值设置 PPRE1[2:0] 位 */
  tmpreg |= RCC_HCLK;
  /* 保存新值 */
  RCC->CFGR = tmpreg;
}

/**
  * @brief  配置高速 APB 时钟（PCLK2）。
  * @param  RCC_HCLK: 定义 APB2 时钟分频系数。该时钟由 AHB 时钟（HCLK）派生。
  *   该参数可取以下值之一：
  *     @arg RCC_HCLK_Div1: APB2 时钟 = HCLK
  *     @arg RCC_HCLK_Div2: APB2 时钟 = HCLK/2
  *     @arg RCC_HCLK_Div4: APB2 时钟 = HCLK/4
  *     @arg RCC_HCLK_Div8: APB2 时钟 = HCLK/8
  *     @arg RCC_HCLK_Div16: APB2 时钟 = HCLK/16
  * @retval 无
  */
void RCC_PCLK2Config(uint32_t RCC_HCLK)
{
  uint32_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_RCC_PCLK(RCC_HCLK));
  tmpreg = RCC->CFGR;
  /* 清除 PPRE2[2:0] 位 */
  tmpreg &= CFGR_PPRE2_Reset_Mask;
  /* 根据 RCC_HCLK 值设置 PPRE2[2:0] 位 */
  tmpreg |= RCC_HCLK << 3;
  /* 保存新值 */
  RCC->CFGR = tmpreg;
}

/**
  * @brief  使能或关闭指定的 RCC 中断。
  * @param  RCC_IT: 指定要使能或关闭的 RCC 中断源。
  *
  *   对于 @b STM32_Connectivity_line_devices，该参数可取以下值的任意组合
  *     @arg RCC_IT_LSIRDY: LSI 就绪中断
  *     @arg RCC_IT_LSERDY: LSE 就绪中断
  *     @arg RCC_IT_HSIRDY: HSI 就绪中断
  *     @arg RCC_IT_HSERDY: HSE 就绪中断
  *     @arg RCC_IT_PLLRDY: PLL 就绪中断
  *     @arg RCC_IT_PLL2RDY: PLL2 就绪中断
  *     @arg RCC_IT_PLL3RDY: PLL3 就绪中断
  *
  *   对于 @b other_STM32_devices，该参数可取以下值的任意组合
  *     @arg RCC_IT_LSIRDY: LSI 就绪中断
  *     @arg RCC_IT_LSERDY: LSE 就绪中断
  *     @arg RCC_IT_HSIRDY: HSI 就绪中断
  *     @arg RCC_IT_HSERDY: HSE 就绪中断
  *     @arg RCC_IT_PLLRDY: PLL 就绪中断
  *
  * @param  NewState: 指定 RCC 中断的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void RCC_ITConfig(uint8_t RCC_IT, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_RCC_IT(RCC_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* 对 RCC_CIR 位执行字节访问以使能所选中断 */
    *(__IO uint8_t *) CIR_BYTE2_ADDRESS |= RCC_IT;
  }
  else
  {
    /* 对 RCC_CIR 位执行字节访问以关闭所选中断 */
    *(__IO uint8_t *) CIR_BYTE2_ADDRESS &= (uint8_t)~RCC_IT;
  }
}

#ifndef STM32F10X_CL
/**
  * @brief  配置 USB 时钟（USBCLK）。
  * @param  RCC_USBCLKSource: 指定 USB 时钟源。该时钟由 PLL 输出派生。
  *   该参数可取以下值之一：
  *     @arg RCC_USBCLKSource_PLLCLK_1Div5: 选择 PLL 时钟除以 1.5 作为 USB
  *                                     时钟源
  *     @arg RCC_USBCLKSource_PLLCLK_Div1: 选择 PLL 时钟作为 USB 时钟源
  * @retval 无
  */
void RCC_USBCLKConfig(uint32_t RCC_USBCLKSource)
{
  /* 检查参数 */
  assert_param(IS_RCC_USBCLK_SOURCE(RCC_USBCLKSource));

  *(__IO uint32_t *) CFGR_USBPRE_BB = RCC_USBCLKSource;
}
#else
/**
  * @brief  配置 USB OTG FS 时钟（OTGFSCLK）。
  *   本函数仅适用于 STM32 互联型器件。
  * @param  RCC_OTGFSCLKSource: 指定 USB OTG FS 时钟源。
  *   该时钟由 PLL 输出派生。
  *   该参数可取以下值之一：
  *     @arg  RCC_OTGFSCLKSource_PLLVCO_Div3: 选择 PLL VCO 时钟除以 2 作为 USB OTG FS 时钟源
  *     @arg  RCC_OTGFSCLKSource_PLLVCO_Div2: 选择 PLL VCO 时钟除以 2 作为 USB OTG FS 时钟源
  * @retval 无
  */
void RCC_OTGFSCLKConfig(uint32_t RCC_OTGFSCLKSource)
{
  /* 检查参数 */
  assert_param(IS_RCC_OTGFSCLK_SOURCE(RCC_OTGFSCLKSource));

  *(__IO uint32_t *) CFGR_OTGFSPRE_BB = RCC_OTGFSCLKSource;
}
#endif /* STM32F10X_CL */ 

/**
  * @brief  配置 ADC 时钟（ADCCLK）。
  * @param  RCC_PCLK2: 定义 ADC 时钟分频系数。该时钟由 APB2 时钟（PCLK2）派生。
  *   该参数可取以下值之一：
  *     @arg RCC_PCLK2_Div2: ADC 时钟 = PCLK2/2
  *     @arg RCC_PCLK2_Div4: ADC 时钟 = PCLK2/4
  *     @arg RCC_PCLK2_Div6: ADC 时钟 = PCLK2/6
  *     @arg RCC_PCLK2_Div8: ADC 时钟 = PCLK2/8
  * @retval 无
  */
void RCC_ADCCLKConfig(uint32_t RCC_PCLK2)
{
  uint32_t tmpreg = 0;
  /* 检查参数 */
  assert_param(IS_RCC_ADCCLK(RCC_PCLK2));
  tmpreg = RCC->CFGR;
  /* 清除 ADCPRE[1:0] 位 */
  tmpreg &= CFGR_ADCPRE_Reset_Mask;
  /* 根据 RCC_PCLK2 值设置 ADCPRE[1:0] 位 */
  tmpreg |= RCC_PCLK2;
  /* 保存新值 */
  RCC->CFGR = tmpreg;
}

#ifdef STM32F10X_CL
/**
  * @brief  配置 I2S2 时钟源（I2S2CLK）。
  * @note
  *   - 本函数必须在使能 I2S2 APB 时钟之前调用。
  *   - 本函数仅适用于 STM32 互联型器件。
  * @param  RCC_I2S2CLKSource: 指定 I2S2 时钟源。
  *   该参数可取以下值之一：
  *     @arg RCC_I2S2CLKSource_SYSCLK: 选择系统时钟作为 I2S2 时钟输入
  *     @arg RCC_I2S2CLKSource_PLL3_VCO: 选择 PLL3 VCO 时钟作为 I2S2 时钟输入
  * @retval 无
  */
void RCC_I2S2CLKConfig(uint32_t RCC_I2S2CLKSource)
{
  /* 检查参数 */
  assert_param(IS_RCC_I2S2CLK_SOURCE(RCC_I2S2CLKSource));

  *(__IO uint32_t *) CFGR2_I2S2SRC_BB = RCC_I2S2CLKSource;
}

/**
  * @brief  配置 I2S3 时钟源（I2S2CLK）。
  * @note
  *   - 本函数必须在使能 I2S3 APB 时钟之前调用。
  *   - 本函数仅适用于 STM32 互联型器件。
  * @param  RCC_I2S3CLKSource: 指定 I2S3 时钟源。
  *   该参数可取以下值之一：
  *     @arg RCC_I2S3CLKSource_SYSCLK: 选择系统时钟作为 I2S3 时钟输入
  *     @arg RCC_I2S3CLKSource_PLL3_VCO: 选择 PLL3 VCO 时钟作为 I2S3 时钟输入
  * @retval 无
  */
void RCC_I2S3CLKConfig(uint32_t RCC_I2S3CLKSource)
{
  /* 检查参数 */
  assert_param(IS_RCC_I2S3CLK_SOURCE(RCC_I2S3CLKSource));

  *(__IO uint32_t *) CFGR2_I2S3SRC_BB = RCC_I2S3CLKSource;
}
#endif /* STM32F10X_CL */

/**
  * @brief  配置外部低速振荡器（LSE）。
  * @param  RCC_LSE: 指定 LSE 的新状态。
  *   该参数可取以下值之一：
  *     @arg RCC_LSE_OFF: LSE 振荡器关闭
  *     @arg RCC_LSE_ON: LSE 振荡器开启
  *     @arg RCC_LSE_Bypass: LSE 振荡器由外部时钟旁路
  * @retval 无
  */
void RCC_LSEConfig(uint8_t RCC_LSE)
{
  /* 检查参数 */
  assert_param(IS_RCC_LSE(RCC_LSE));
  /* 在配置 LSE 之前复位 LSEON 和 LSEBYP 位 ------------------*/
  /* 复位 LSEON 位 */
  *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_OFF;
  /* 复位 LSEBYP 位 */
  *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_OFF;
  /* 配置 LSE（RCC_LSE_OFF 已由上面的代码段涵盖） */
  switch(RCC_LSE)
  {
    case RCC_LSE_ON:
      /* 设置 LSEON 位 */
      *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_ON;
      break;
      
    case RCC_LSE_Bypass:
      /* 设置 LSEBYP 和 LSEON 位 */
      *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_Bypass | RCC_LSE_ON;
      break;            
      
    default:
      break;      
  }
}

/**
  * @brief  使能或关闭内部低速振荡器（LSI）。
  * @note   如果 IWDG 正在运行，则不能关闭 LSI。
  * @param  NewState: LSI 的新状态。该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void RCC_LSICmd(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CSR_LSION_BB = (uint32_t)NewState;
}

/**
  * @brief  配置 RTC 时钟（RTCCLK）。
  * @note   一旦选定了 RTC 时钟，除非复位备份域，否则无法更改。
  * @param  RCC_RTCCLKSource: 指定 RTC 时钟源。
  *   该参数可取以下值之一：
  *     @arg RCC_RTCCLKSource_LSE: 选择 LSE 作为 RTC 时钟
  *     @arg RCC_RTCCLKSource_LSI: 选择 LSI 作为 RTC 时钟
  *     @arg RCC_RTCCLKSource_HSE_Div128: 选择 HSE 时钟除以 128 作为 RTC 时钟
  * @retval 无
  */
void RCC_RTCCLKConfig(uint32_t RCC_RTCCLKSource)
{
  /* 检查参数 */
  assert_param(IS_RCC_RTCCLK_SOURCE(RCC_RTCCLKSource));
  /* 选择 RTC 时钟源 */
  RCC->BDCR |= RCC_RTCCLKSource;
}

/**
  * @brief  使能或关闭 RTC 时钟。
  * @note   本函数只能在使用 RCC_RTCCLKConfig 函数选择 RTC 时钟之后使用。
  * @param  NewState: RTC 时钟的新状态。该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void RCC_RTCCLKCmd(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) BDCR_RTCEN_BB = (uint32_t)NewState;
}

/**
  * @brief  返回各个片上时钟的频率。
  * @param  RCC_Clocks: 指向 RCC_ClocksTypeDef 结构的指针，
  *         该结构将保存各时钟频率。
  * @note   当 HSE 晶振使用小数分频值时，本函数的结果可能不正确。
  * @retval 无
  */
void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks)
{
  uint32_t tmp = 0, pllmull = 0, pllsource = 0, presc = 0;

#ifdef  STM32F10X_CL
  uint32_t prediv1source = 0, prediv1factor = 0, prediv2factor = 0, pll2mull = 0;
#endif /* STM32F10X_CL */

#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
  uint32_t prediv1factor = 0;
#endif
    
  /* 获取 SYSCLK 源 -------------------------------------------------------*/
  tmp = RCC->CFGR & CFGR_SWS_Mask;
  
  switch (tmp)
  {
    case 0x00:  /* HSI 用作系统时钟 */
      RCC_Clocks->SYSCLK_Frequency = HSI_VALUE;
      break;
    case 0x04:  /* HSE 用作系统时钟 */
      RCC_Clocks->SYSCLK_Frequency = HSE_VALUE;
      break;
    case 0x08:  /* PLL 用作系统时钟 */

      /* 获取 PLL 时钟源和倍频系数 ----------------------*/
      pllmull = RCC->CFGR & CFGR_PLLMull_Mask;
      pllsource = RCC->CFGR & CFGR_PLLSRC_Mask;
      
#ifndef STM32F10X_CL      
      pllmull = ( pllmull >> 18) + 2;
      
      if (pllsource == 0x00)
      {/* 选择 HSI 振荡器时钟除以 2 作为 PLL 时钟输入 */
        RCC_Clocks->SYSCLK_Frequency = (HSI_VALUE >> 1) * pllmull;
      }
      else
      {
 #if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
       prediv1factor = (RCC->CFGR2 & CFGR2_PREDIV1) + 1;
       /* 选择 HSE 振荡器时钟作为 PREDIV1 时钟输入 */
       RCC_Clocks->SYSCLK_Frequency = (HSE_VALUE / prediv1factor) * pllmull; 
 #else
        /* 选择 HSE 作为 PLL 时钟输入 */
        if ((RCC->CFGR & CFGR_PLLXTPRE_Mask) != (uint32_t)RESET)
        {/* HSE 振荡器时钟除以 2 */
          RCC_Clocks->SYSCLK_Frequency = (HSE_VALUE >> 1) * pllmull;
        }
        else
        {
          RCC_Clocks->SYSCLK_Frequency = HSE_VALUE * pllmull;
        }
 #endif
      }
#else
      pllmull = pllmull >> 18;
      
      if (pllmull != 0x0D)
      {
         pllmull += 2;
      }
      else
      { /* PLL 倍频系数 = PLL 输入时钟 * 6.5 */
        pllmull = 13 / 2; 
      }
            
      if (pllsource == 0x00)
      {/* 选择 HSI 振荡器时钟除以 2 作为 PLL 时钟输入 */
        RCC_Clocks->SYSCLK_Frequency = (HSI_VALUE >> 1) * pllmull;
      }
      else
      {/* 选择 PREDIV1 作为 PLL 时钟输入 */
        
        /* 获取 PREDIV1 时钟源和分频系数 */
        prediv1source = RCC->CFGR2 & CFGR2_PREDIV1SRC;
        prediv1factor = (RCC->CFGR2 & CFGR2_PREDIV1) + 1;
        
        if (prediv1source == 0)
        { /* 选择 HSE 振荡器时钟作为 PREDIV1 时钟输入 */
          RCC_Clocks->SYSCLK_Frequency = (HSE_VALUE / prediv1factor) * pllmull;          
        }
        else
        {/* 选择 PLL2 时钟作为 PREDIV1 时钟输入 */
          
          /* 获取 PREDIV2 分频系数和 PLL2 倍频系数 */
          prediv2factor = ((RCC->CFGR2 & CFGR2_PREDIV2) >> 4) + 1;
          pll2mull = ((RCC->CFGR2 & CFGR2_PLL2MUL) >> 8 ) + 2; 
          RCC_Clocks->SYSCLK_Frequency = (((HSE_VALUE / prediv2factor) * pll2mull) / prediv1factor) * pllmull;                         
        }
      }
#endif /* STM32F10X_CL */ 
      break;

    default:
      RCC_Clocks->SYSCLK_Frequency = HSI_VALUE;
      break;
  }

  /* 计算 HCLK、PCLK1、PCLK2 和 ADCCLK 时钟频率 ----------------*/
  /* 获取 HCLK 预分频器 */
  tmp = RCC->CFGR & CFGR_HPRE_Set_Mask;
  tmp = tmp >> 4;
  presc = APBAHBPrescTable[tmp];
  /* HCLK 时钟频率 */
  RCC_Clocks->HCLK_Frequency = RCC_Clocks->SYSCLK_Frequency >> presc;
  /* 获取 PCLK1 预分频器 */
  tmp = RCC->CFGR & CFGR_PPRE1_Set_Mask;
  tmp = tmp >> 8;
  presc = APBAHBPrescTable[tmp];
  /* PCLK1 时钟频率 */
  RCC_Clocks->PCLK1_Frequency = RCC_Clocks->HCLK_Frequency >> presc;
  /* 获取 PCLK2 预分频器 */
  tmp = RCC->CFGR & CFGR_PPRE2_Set_Mask;
  tmp = tmp >> 11;
  presc = APBAHBPrescTable[tmp];
  /* PCLK2 时钟频率 */
  RCC_Clocks->PCLK2_Frequency = RCC_Clocks->HCLK_Frequency >> presc;
  /* 获取 ADCCLK 预分频器 */
  tmp = RCC->CFGR & CFGR_ADCPRE_Set_Mask;
  tmp = tmp >> 14;
  presc = ADCPrescTable[tmp];
  /* ADCCLK 时钟频率 */
  RCC_Clocks->ADCCLK_Frequency = RCC_Clocks->PCLK2_Frequency / presc;
}

/**
  * @brief  使能或关闭 AHB 外设时钟。
  * @param  RCC_AHBPeriph: 指定要对其时钟进行门控的 AHB 外设。
  *
  *   对于 @b STM32_Connectivity_line_devices，该参数可取以下值的任意组合：
  *     @arg RCC_AHBPeriph_DMA1
  *     @arg RCC_AHBPeriph_DMA2
  *     @arg RCC_AHBPeriph_SRAM
  *     @arg RCC_AHBPeriph_FLITF
  *     @arg RCC_AHBPeriph_CRC
  *     @arg RCC_AHBPeriph_OTG_FS
  *     @arg RCC_AHBPeriph_ETH_MAC
  *     @arg RCC_AHBPeriph_ETH_MAC_Tx
  *     @arg RCC_AHBPeriph_ETH_MAC_Rx
  *
  *   对于 @b other_STM32_devices，该参数可取以下值的任意组合：
  *     @arg RCC_AHBPeriph_DMA1
  *     @arg RCC_AHBPeriph_DMA2
  *     @arg RCC_AHBPeriph_SRAM
  *     @arg RCC_AHBPeriph_FLITF
  *     @arg RCC_AHBPeriph_CRC
  *     @arg RCC_AHBPeriph_FSMC
  *     @arg RCC_AHBPeriph_SDIO
  *
  * @note SRAM 和 FLITF 时钟只能在睡眠模式下关闭。
  * @param  NewState: 指定外设时钟的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void RCC_AHBPeriphClockCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_RCC_AHB_PERIPH(RCC_AHBPeriph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    RCC->AHBENR |= RCC_AHBPeriph;
  }
  else
  {
    RCC->AHBENR &= ~RCC_AHBPeriph;
  }
}

/**
  * @brief  使能或关闭高速 APB（APB2）外设时钟。
  * @param  RCC_APB2Periph: 指定要对其时钟进行门控的 APB2 外设。
  *   该参数可取以下值的任意组合：
  *     @arg RCC_APB2Periph_AFIO, RCC_APB2Periph_GPIOA, RCC_APB2Periph_GPIOB,
  *          RCC_APB2Periph_GPIOC, RCC_APB2Periph_GPIOD, RCC_APB2Periph_GPIOE,
  *          RCC_APB2Periph_GPIOF, RCC_APB2Periph_GPIOG, RCC_APB2Periph_ADC1,
  *          RCC_APB2Periph_ADC2, RCC_APB2Periph_TIM1, RCC_APB2Periph_SPI1,
  *          RCC_APB2Periph_TIM8, RCC_APB2Periph_USART1, RCC_APB2Periph_ADC3,
  *          RCC_APB2Periph_TIM15, RCC_APB2Periph_TIM16, RCC_APB2Periph_TIM17,
  *          RCC_APB2Periph_TIM9, RCC_APB2Periph_TIM10, RCC_APB2Periph_TIM11
  * @param  NewState: 指定外设时钟的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->APB2ENR |= RCC_APB2Periph;
  }
  else
  {
    RCC->APB2ENR &= ~RCC_APB2Periph;
  }
}

/**
  * @brief  使能或关闭低速 APB（APB1）外设时钟。
  * @param  RCC_APB1Periph: 指定要对其时钟进行门控的 APB1 外设。
  *   该参数可取以下值的任意组合：
  *     @arg RCC_APB1Periph_TIM2, RCC_APB1Periph_TIM3, RCC_APB1Periph_TIM4,
  *          RCC_APB1Periph_TIM5, RCC_APB1Periph_TIM6, RCC_APB1Periph_TIM7,
  *          RCC_APB1Periph_WWDG, RCC_APB1Periph_SPI2, RCC_APB1Periph_SPI3,
  *          RCC_APB1Periph_USART2, RCC_APB1Periph_USART3, RCC_APB1Periph_USART4,
  *          RCC_APB1Periph_USART5, RCC_APB1Periph_I2C1, RCC_APB1Periph_I2C2,
  *          RCC_APB1Periph_USB, RCC_APB1Periph_CAN1, RCC_APB1Periph_BKP,
  *          RCC_APB1Periph_PWR, RCC_APB1Periph_DAC, RCC_APB1Periph_CEC,
  *          RCC_APB1Periph_TIM12, RCC_APB1Periph_TIM13, RCC_APB1Periph_TIM14
  * @param  NewState: 指定外设时钟的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_RCC_APB1_PERIPH(RCC_APB1Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->APB1ENR |= RCC_APB1Periph;
  }
  else
  {
    RCC->APB1ENR &= ~RCC_APB1Periph;
  }
}

#ifdef STM32F10X_CL
/**
  * @brief  强制或释放 AHB 外设复位。
  * @note   本函数仅适用于 STM32 互联型器件。
  * @param  RCC_AHBPeriph: 指定要复位的 AHB 外设。
  *   该参数可取以下值的任意组合：
  *     @arg RCC_AHBPeriph_OTG_FS
  *     @arg RCC_AHBPeriph_ETH_MAC
  * @param  NewState: 指定外设复位的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void RCC_AHBPeriphResetCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_RCC_AHB_PERIPH_RESET(RCC_AHBPeriph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    RCC->AHBRSTR |= RCC_AHBPeriph;
  }
  else
  {
    RCC->AHBRSTR &= ~RCC_AHBPeriph;
  }
}
#endif /* STM32F10X_CL */ 

/**
  * @brief  强制或释放高速 APB（APB2）外设复位。
  * @param  RCC_APB2Periph: 指定要复位的 APB2 外设。
  *   该参数可取以下值的任意组合：
  *     @arg RCC_APB2Periph_AFIO, RCC_APB2Periph_GPIOA, RCC_APB2Periph_GPIOB,
  *          RCC_APB2Periph_GPIOC, RCC_APB2Periph_GPIOD, RCC_APB2Periph_GPIOE,
  *          RCC_APB2Periph_GPIOF, RCC_APB2Periph_GPIOG, RCC_APB2Periph_ADC1,
  *          RCC_APB2Periph_ADC2, RCC_APB2Periph_TIM1, RCC_APB2Periph_SPI1,
  *          RCC_APB2Periph_TIM8, RCC_APB2Periph_USART1, RCC_APB2Periph_ADC3,
  *          RCC_APB2Periph_TIM15, RCC_APB2Periph_TIM16, RCC_APB2Periph_TIM17,
  *          RCC_APB2Periph_TIM9, RCC_APB2Periph_TIM10, RCC_APB2Periph_TIM11
  * @param  NewState: 指定外设复位的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void RCC_APB2PeriphResetCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->APB2RSTR |= RCC_APB2Periph;
  }
  else
  {
    RCC->APB2RSTR &= ~RCC_APB2Periph;
  }
}

/**
  * @brief  强制或释放低速 APB（APB1）外设复位。
  * @param  RCC_APB1Periph: 指定要复位的 APB1 外设。
  *   该参数可取以下值的任意组合：
  *     @arg RCC_APB1Periph_TIM2, RCC_APB1Periph_TIM3, RCC_APB1Periph_TIM4,
  *          RCC_APB1Periph_TIM5, RCC_APB1Periph_TIM6, RCC_APB1Periph_TIM7,
  *          RCC_APB1Periph_WWDG, RCC_APB1Periph_SPI2, RCC_APB1Periph_SPI3,
  *          RCC_APB1Periph_USART2, RCC_APB1Periph_USART3, RCC_APB1Periph_USART4,
  *          RCC_APB1Periph_USART5, RCC_APB1Periph_I2C1, RCC_APB1Periph_I2C2,
  *          RCC_APB1Periph_USB, RCC_APB1Periph_CAN1, RCC_APB1Periph_BKP,
  *          RCC_APB1Periph_PWR, RCC_APB1Periph_DAC, RCC_APB1Periph_CEC,
  *          RCC_APB1Periph_TIM12, RCC_APB1Periph_TIM13, RCC_APB1Periph_TIM14
  * @param  NewState: 指定外设时钟的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void RCC_APB1PeriphResetCmd(uint32_t RCC_APB1Periph, FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_RCC_APB1_PERIPH(RCC_APB1Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->APB1RSTR |= RCC_APB1Periph;
  }
  else
  {
    RCC->APB1RSTR &= ~RCC_APB1Periph;
  }
}

/**
  * @brief  强制或释放备份域复位。
  * @param  NewState: 备份域复位的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void RCC_BackupResetCmd(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) BDCR_BDRST_BB = (uint32_t)NewState;
}

/**
  * @brief  使能或关闭时钟安全系统。
  * @param  NewState: 时钟安全系统的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void RCC_ClockSecuritySystemCmd(FunctionalState NewState)
{
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CR_CSSON_BB = (uint32_t)NewState;
}

/**
  * @brief  选择在 MCO 引脚上输出的时钟源。
  * @param  RCC_MCO: 指定要输出的时钟源。
  *
  *   对于 @b STM32_Connectivity_line_devices，该参数可取以下值之一：
  *     @arg RCC_MCO_NoClock: 不选择时钟
  *     @arg RCC_MCO_SYSCLK: 选择系统时钟
  *     @arg RCC_MCO_HSI: 选择 HSI 振荡器时钟
  *     @arg RCC_MCO_HSE: 选择 HSE 振荡器时钟
  *     @arg RCC_MCO_PLLCLK_Div2: 选择 PLL 时钟除以 2
  *     @arg RCC_MCO_PLL2CLK: 选择 PLL2 时钟
  *     @arg RCC_MCO_PLL3CLK_Div2: 选择 PLL3 时钟除以 2
  *     @arg RCC_MCO_XT1: 选择外部 3-25 MHz 振荡器时钟
  *     @arg RCC_MCO_PLL3CLK: 选择 PLL3 时钟
  *
  *   对于 @b other_STM32_devices，该参数可取以下值之一：
  *     @arg RCC_MCO_NoClock: 不选择时钟
  *     @arg RCC_MCO_SYSCLK: 选择系统时钟
  *     @arg RCC_MCO_HSI: 选择 HSI 振荡器时钟
  *     @arg RCC_MCO_HSE: 选择 HSE 振荡器时钟
  *     @arg RCC_MCO_PLLCLK_Div2: 选择 PLL 时钟除以 2
  *
  * @retval 无
  */
void RCC_MCOConfig(uint8_t RCC_MCO)
{
  /* 检查参数 */
  assert_param(IS_RCC_MCO(RCC_MCO));

  /* 对 MCO 位执行字节访问以选择 MCO 源 */
  *(__IO uint8_t *) CFGR_BYTE4_ADDRESS = RCC_MCO;
}

/**
  * @brief  检查指定的 RCC 标志是否置位。
  * @param  RCC_FLAG: 指定要检查的标志。
  *
  *   对于 @b STM32_Connectivity_line_devices，该参数可取以下值之一：
  *     @arg RCC_FLAG_HSIRDY: HSI 振荡器时钟就绪
  *     @arg RCC_FLAG_HSERDY: HSE 振荡器时钟就绪
  *     @arg RCC_FLAG_PLLRDY: PLL 时钟就绪
  *     @arg RCC_FLAG_PLL2RDY: PLL2 时钟就绪
  *     @arg RCC_FLAG_PLL3RDY: PLL3 时钟就绪
  *     @arg RCC_FLAG_LSERDY: LSE 振荡器时钟就绪
  *     @arg RCC_FLAG_LSIRDY: LSI 振荡器时钟就绪
  *     @arg RCC_FLAG_PINRST: 引脚复位
  *     @arg RCC_FLAG_PORRST: POR/PDR 复位
  *     @arg RCC_FLAG_SFTRST: 软件复位
  *     @arg RCC_FLAG_IWDGRST: 独立看门狗复位
  *     @arg RCC_FLAG_WWDGRST: 窗口看门狗复位
  *     @arg RCC_FLAG_LPWRRST: 低功耗复位
  *
  *   对于 @b other_STM32_devices，该参数可取以下值之一：
  *     @arg RCC_FLAG_HSIRDY: HSI 振荡器时钟就绪
  *     @arg RCC_FLAG_HSERDY: HSE 振荡器时钟就绪
  *     @arg RCC_FLAG_PLLRDY: PLL 时钟就绪
  *     @arg RCC_FLAG_LSERDY: LSE 振荡器时钟就绪
  *     @arg RCC_FLAG_LSIRDY: LSI 振荡器时钟就绪
  *     @arg RCC_FLAG_PINRST: 引脚复位
  *     @arg RCC_FLAG_PORRST: POR/PDR 复位
  *     @arg RCC_FLAG_SFTRST: 软件复位
  *     @arg RCC_FLAG_IWDGRST: 独立看门狗复位
  *     @arg RCC_FLAG_WWDGRST: 窗口看门狗复位
  *     @arg RCC_FLAG_LPWRRST: 低功耗复位
  *
  * @retval RCC_FLAG 的新状态（SET 或 RESET）。
  */
FlagStatus RCC_GetFlagStatus(uint8_t RCC_FLAG)
{
  uint32_t tmp = 0;
  uint32_t statusreg = 0;
  FlagStatus bitstatus = RESET;
  /* 检查参数 */
  assert_param(IS_RCC_FLAG(RCC_FLAG));

  /* 获取 RCC 寄存器索引 */
  tmp = RCC_FLAG >> 5;
  if (tmp == 1)               /* 要检查的标志位于 CR 寄存器中 */
  {
    statusreg = RCC->CR;
  }
  else if (tmp == 2)          /* 要检查的标志位于 BDCR 寄存器中 */
  {
    statusreg = RCC->BDCR;
  }
  else                       /* 要检查的标志位于 CSR 寄存器中 */
  {
    statusreg = RCC->CSR;
  }

  /* 获取标志位置 */
  tmp = RCC_FLAG & FLAG_Mask;
  if ((statusreg & ((uint32_t)1 << tmp)) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }

  /* 返回标志状态 */
  return bitstatus;
}

/**
  * @brief  清除 RCC 复位标志。
  * @note   复位标志包括：RCC_FLAG_PINRST、RCC_FLAG_PORRST、RCC_FLAG_SFTRST、
  *   RCC_FLAG_IWDGRST、RCC_FLAG_WWDGRST、RCC_FLAG_LPWRRST
  * @param  None
  * @retval 无
  */
void RCC_ClearFlag(void)
{
  /* 设置 RMVF 位以清除复位标志 */
  RCC->CSR |= CSR_RMVF_Set;
}

/**
  * @brief  检查指定的 RCC 中断是否发生。
  * @param  RCC_IT: 指定要检查的 RCC 中断源。
  *
  *   对于 @b STM32_Connectivity_line_devices，该参数可取以下值之一：
  *     @arg RCC_IT_LSIRDY: LSI 就绪中断
  *     @arg RCC_IT_LSERDY: LSE 就绪中断
  *     @arg RCC_IT_HSIRDY: HSI 就绪中断
  *     @arg RCC_IT_HSERDY: HSE 就绪中断
  *     @arg RCC_IT_PLLRDY: PLL 就绪中断
  *     @arg RCC_IT_PLL2RDY: PLL2 就绪中断
  *     @arg RCC_IT_PLL3RDY: PLL3 就绪中断
  *     @arg RCC_IT_CSS: 时钟安全系统中断
  *
  *   对于 @b other_STM32_devices，该参数可取以下值之一：
  *     @arg RCC_IT_LSIRDY: LSI 就绪中断
  *     @arg RCC_IT_LSERDY: LSE 就绪中断
  *     @arg RCC_IT_HSIRDY: HSI 就绪中断
  *     @arg RCC_IT_HSERDY: HSE 就绪中断
  *     @arg RCC_IT_PLLRDY: PLL 就绪中断
  *     @arg RCC_IT_CSS: 时钟安全系统中断
  *
  * @retval RCC_IT 的新状态（SET 或 RESET）。
  */
ITStatus RCC_GetITStatus(uint8_t RCC_IT)
{
  ITStatus bitstatus = RESET;
  /* 检查参数 */
  assert_param(IS_RCC_GET_IT(RCC_IT));

  /* 检查指定 RCC 中断的状态 */
  if ((RCC->CIR & RCC_IT) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }

  /* 返回 RCC_IT 状态 */
  return  bitstatus;
}

/**
  * @brief  清除 RCC 的中断挂起位。
  * @param  RCC_IT: 指定要清除的中断挂起位。
  *
  *   对于 @b STM32_Connectivity_line_devices，该参数可取以下值的任意组合：
  *     @arg RCC_IT_LSIRDY: LSI 就绪中断
  *     @arg RCC_IT_LSERDY: LSE 就绪中断
  *     @arg RCC_IT_HSIRDY: HSI 就绪中断
  *     @arg RCC_IT_HSERDY: HSE 就绪中断
  *     @arg RCC_IT_PLLRDY: PLL 就绪中断
  *     @arg RCC_IT_PLL2RDY: PLL2 就绪中断
  *     @arg RCC_IT_PLL3RDY: PLL3 就绪中断
  *     @arg RCC_IT_CSS: 时钟安全系统中断
  *
  *   对于 @b other_STM32_devices，该参数可取以下值的任意组合：
  *     @arg RCC_IT_LSIRDY: LSI 就绪中断
  *     @arg RCC_IT_LSERDY: LSE 就绪中断
  *     @arg RCC_IT_HSIRDY: HSI 就绪中断
  *     @arg RCC_IT_HSERDY: HSE 就绪中断
  *     @arg RCC_IT_PLLRDY: PLL 就绪中断
  *
  *     @arg RCC_IT_CSS: 时钟安全系统中断
  * @retval 无
  */
void RCC_ClearITPendingBit(uint8_t RCC_IT)
{
  /* 检查参数 */
  assert_param(IS_RCC_CLEAR_IT(RCC_IT));

  /* 对 RCC_CIR[23:16] 位执行字节访问以清除所选的中断
     挂起位 */
  *(__IO uint8_t *) CIR_BYTE3_ADDRESS = RCC_IT;
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
