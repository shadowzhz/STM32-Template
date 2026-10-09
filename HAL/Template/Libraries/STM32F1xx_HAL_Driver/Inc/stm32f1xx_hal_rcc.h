/**
  ******************************************************************************
  * @file    stm32f1xx_hal_rcc.h
  * @author  MCD Application Team
  * @brief   HAL库 RCC（复位和时钟控制）模块头文件
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

/* 定义以防止递归 -------------------------------------*/
#ifndef __STM32F1xx_HAL_RCC_H
#define __STM32F1xx_HAL_RCC_H

#ifdef __cplusplus
extern "C" {
#endif

/* 头文件 ------------------------------------------------------------------*/
#include "stm32f1xx_hal_def.h"


/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @addtogroup RCC
  * @{
  */

/* 导出类型 ------------------------------------------------------------*/

/** @defgroup RCC_Exported_Types RCC 导出类型
  * @{
  */

/**
  * @brief  RCC 锁相环 配置结构定义
  */
typedef struct
{
  uint32_t PLLState;      /*!< PLLState: 锁相环新状态
                               参数取值于 @ref RCC_PLL_Config */

  uint32_t PLLSource;     /*!< PLLSource: 锁相环输入时钟源
                              参数必须取值于 @ref RCC_PLL_Clock_Source */

  uint32_t PLLMUL;        /*!< PLLMUL: 倍频系数
                              参数必须取值于 @ref RCCEx_PLL_Multiplication_Factor */
} RCC_PLLInitTypeDef;

/**
  * @brief  RCC System, AHB 和 APB 总线时钟配置结构定义
  */
typedef struct
{
  uint32_t ClockType;             /*!< 需要配置的时钟
                                        参数取值于 @ref RCC_System_Clock_Type */

  uint32_t SYSCLKSource;          /*!< 系统时钟 (SYSCLKS) 的时钟来源
                                        参数取值于 @ref RCC_System_Clock_Source */

  uint32_t AHBCLKDivider;         /*!< AHB总线时钟 (HCLK) 分频系数. 这个总线的时钟来源于系统时钟 (SYSCLK) 的分频.
                                        参数取值于 @ref RCC_AHB_Clock_Source */

  uint32_t APB1CLKDivider;        /*!< APB1总线时钟 (PCLK1) 分频系数. 这个总线的时钟来源于AHB时钟 (HCLK) 的分频.
                                        参数取值于 @ref RCC_APB1_APB2_Clock_Source */

  uint32_t APB2CLKDivider;        /*!< APB2总线时钟 (PCLK2) 分频系数. 这个总线的时钟来源于AHB时钟 (HCLK) 的分频.
                                        参数取值于 @ref RCC_APB1_APB2_Clock_Source */
} RCC_ClkInitTypeDef;

/**
  * @}
  */

/* 导出常量 --------------------------------------------------------*/
/** @defgroup RCC_Exported_Constants RCC 导出常量
  * @{
  */

/** @defgroup RCC_PLL_Clock_Source 锁相环时钟源
  * @{
  */

#define RCC_PLLSOURCE_HSI_DIV2      0x00000000U     /*!< HSI 2分频 作为锁相环 (PLL) 的时钟源 */
#define RCC_PLLSOURCE_HSE           RCC_CFGR_PLLSRC /*!< HSE 作为锁相环 (PLL) 的时钟源 */

/**
  * @}
  */

/** @defgroup RCC_Oscillator_Type 振荡器类型
  * @{
  */
#define RCC_OSCILLATORTYPE_NONE            0x00000000U  /* 无 */
#define RCC_OSCILLATORTYPE_HSE             0x00000001U  /* HSE 外部高速晶振 */
#define RCC_OSCILLATORTYPE_HSI             0x00000002U  /* HSI 内部高速RC振荡器 */
#define RCC_OSCILLATORTYPE_LSE             0x00000004U  /* LSE 外部低速晶振 */
#define RCC_OSCILLATORTYPE_LSI             0x00000008U  /* LSI 内部低速RC振荡器 */
/**
  * @}
  */

/** @defgroup RCC_HSE_Config HSE 配置
  * @{
  */
#define RCC_HSE_OFF                      0x00000000U                                /*!< HSE 时钟关闭 */
#define RCC_HSE_ON                       RCC_CR_HSEON                               /*!< HSE 时钟开启 */
#define RCC_HSE_BYPASS                   ((uint32_t)(RCC_CR_HSEBYP | RCC_CR_HSEON)) /*!< HSE 旁路（注意：是旁路内部相关驱动电路） */
/**
  * @}
  */

/** @defgroup RCC_LSE_Config LSE 配置
  * @{
  */
#define RCC_LSE_OFF                      0x00000000U                                    /*!< LSE 时钟关闭 */
#define RCC_LSE_ON                       RCC_BDCR_LSEON                                 /*!< LSE 时钟开启 */
#define RCC_LSE_BYPASS                   ((uint32_t)(RCC_BDCR_LSEBYP | RCC_BDCR_LSEON)) /*!< LSE 旁路 */

/**
  * @}
  */

/** @defgroup RCC_HSI_Config HSI 配置
  * @{
  */
#define RCC_HSI_OFF                      0x00000000U   /*!< HSI 时钟关闭 */
#define RCC_HSI_ON                       RCC_CR_HSION  /*!< HSI 时钟开启 */

#define RCC_HSICALIBRATION_DEFAULT       0x10U         /* HSI 默认校准微调值 */

/**
  * @}
  */

/** @defgroup RCC_LSI_Config LSI 配置
  * @{
  */
#define RCC_LSI_OFF                      0x00000000U              /*!< LSI 时钟关闭 */
#define RCC_LSI_ON                       RCC_CSR_LSION            /*!< LSI 时钟开启 */

/**
  * @}
  */

/** @defgroup RCC_PLL_Config PLL 配置
  * @{
  */
#define RCC_PLL_NONE                      0x00000000U  /*!< PLL 无配置 */
#define RCC_PLL_OFF                       0x00000001U  /*!< PLL 关闭 */
#define RCC_PLL_ON                        0x00000002U  /*!< PLL 开启 */

/**
  * @}
  */

/** @defgroup RCC_System_Clock_Type 时钟类型
  * @{
  */
#define RCC_CLOCKTYPE_SYSCLK             0x00000001U /*!< 配置 SYSCLK */
#define RCC_CLOCKTYPE_HCLK               0x00000002U /*!< 配置 HCLK */
#define RCC_CLOCKTYPE_PCLK1              0x00000004U /*!< 配置 PCLK1 */
#define RCC_CLOCKTYPE_PCLK2              0x00000008U /*!< 配置 PCLK2 */

/**
  * @}
  */

/** @defgroup RCC_System_Clock_Source 系统时钟的时钟源
  * @{
  */
#define RCC_SYSCLKSOURCE_HSI             RCC_CFGR_SW_HSI /*!< 选择 HSI 作为系统时钟 */
#define RCC_SYSCLKSOURCE_HSE             RCC_CFGR_SW_HSE /*!< 选择 HSE 作为系统时钟 */
#define RCC_SYSCLKSOURCE_PLLCLK          RCC_CFGR_SW_PLL /*!< 选择 PLL 作为系统时钟 */

/**
  * @}
  */

/** @defgroup RCC_System_Clock_Source_Status 系统时钟源状态
  * @{
  */
#define RCC_SYSCLKSOURCE_STATUS_HSI      RCC_CFGR_SWS_HSI            /*!< HSI 作为系统时钟 */
#define RCC_SYSCLKSOURCE_STATUS_HSE      RCC_CFGR_SWS_HSE            /*!< HSE 作为系统时钟 */
#define RCC_SYSCLKSOURCE_STATUS_PLLCLK   RCC_CFGR_SWS_PLL            /*!< PLL 作为系统时钟 */

/**
  * @}
  */

/** @defgroup RCC_AHB_Clock_Source AHB总线时钟源
  * @{
  */
#define RCC_SYSCLK_DIV1                  RCC_CFGR_HPRE_DIV1   /*!< 系统时钟（SYSCLK） 不分频 */
#define RCC_SYSCLK_DIV2                  RCC_CFGR_HPRE_DIV2   /*!< 系统时钟（SYSCLK） 2 分频 */
#define RCC_SYSCLK_DIV4                  RCC_CFGR_HPRE_DIV4   /*!< 系统时钟（SYSCLK） 4 分频 */
#define RCC_SYSCLK_DIV8                  RCC_CFGR_HPRE_DIV8   /*!< 系统时钟（SYSCLK） 8 分频 */
#define RCC_SYSCLK_DIV16                 RCC_CFGR_HPRE_DIV16  /*!< 系统时钟（SYSCLK） 16 分频 */
#define RCC_SYSCLK_DIV64                 RCC_CFGR_HPRE_DIV64  /*!< 系统时钟（SYSCLK） 64 分频 */
#define RCC_SYSCLK_DIV128                RCC_CFGR_HPRE_DIV128 /*!< 系统时钟（SYSCLK） 128 分频 */
#define RCC_SYSCLK_DIV256                RCC_CFGR_HPRE_DIV256 /*!< 系统时钟（SYSCLK） 256 分频 */
#define RCC_SYSCLK_DIV512                RCC_CFGR_HPRE_DIV512 /*!< 系统时钟（SYSCLK） 512 分频 */

/**
  * @}
  */

/** @defgroup RCC_APB1_APB2_Clock_Source APB1 APB2 总线时钟源
  * @{
  */
#define RCC_HCLK_DIV1                    RCC_CFGR_PPRE1_DIV1  /*!< AHB总线时钟（HCLK） 不分频  */
#define RCC_HCLK_DIV2                    RCC_CFGR_PPRE1_DIV2  /*!< AHB总线时钟（HCLK） 2 分频 */
#define RCC_HCLK_DIV4                    RCC_CFGR_PPRE1_DIV4  /*!< AHB总线时钟（HCLK） 4 分频 */
#define RCC_HCLK_DIV8                    RCC_CFGR_PPRE1_DIV8  /*!< AHB总线时钟（HCLK） 8 分频 */
#define RCC_HCLK_DIV16                   RCC_CFGR_PPRE1_DIV16 /*!< AHB总线时钟（HCLK） 16 分频 */

/**
  * @}
  */

/** @defgroup RCC_RTC_Clock_Source RTC 时钟源
  * @{
  */
#define RCC_RTCCLKSOURCE_NO_CLK          0x00000000U                 /*!< 无时钟 */
#define RCC_RTCCLKSOURCE_LSE             RCC_BDCR_RTCSEL_LSE         /*!< LSE 时钟作为 RTC 时钟 */
#define RCC_RTCCLKSOURCE_LSI             RCC_BDCR_RTCSEL_LSI         /*!< LSI 时钟作为 RTC 时钟 */
#define RCC_RTCCLKSOURCE_HSE_DIV128      RCC_BDCR_RTCSEL_HSE         /*!< HSE 时钟128分频作为 RTC 时钟 */
/**
  * @}
  */


/** @defgroup RCC_MCO_Index MCO 索引
  * @{
  */
#define RCC_MCO1                         0x00000000U
#define RCC_MCO                          RCC_MCO1               /*!< MCO1要与其他具有2个MCO的系列兼容*/

/**
  * @}
  */

/** @defgroup RCC_MCOx_Clock_Prescaler MCO 输出时钟预分频
  * @{
  */
#define RCC_MCODIV_1                    0x00000000U   /* 不分频 */

/**
  * @}
  */

/** @defgroup RCC_Interrupt 中断 
  * @{
  */
#define RCC_IT_LSIRDY                    ((uint8_t)RCC_CIR_LSIRDYF)   /*!< LSI 就绪中断标志 */
#define RCC_IT_LSERDY                    ((uint8_t)RCC_CIR_LSERDYF)   /*!< LSE 就绪中断标志 */
#define RCC_IT_HSIRDY                    ((uint8_t)RCC_CIR_HSIRDYF)   /*!< HSI 就绪中断标志 */
#define RCC_IT_HSERDY                    ((uint8_t)RCC_CIR_HSERDYF)   /*!< HSE 就绪中断标志 */
#define RCC_IT_PLLRDY                    ((uint8_t)RCC_CIR_PLLRDYF)   /*!< PLL 就绪中断标志 */
#define RCC_IT_CSS                       ((uint8_t)RCC_CIR_CSSF)      /*!< 时钟安全系统中断标志 */
/**
  * @}
  */

/** @defgroup RCC_Flag 标志
  *        元素值约定: XXXYYYYYb
  *           - YYYYY  : 寄存器中的标志位置
  *           - XXX  : 寄存器序号
  *                 - 001: CR（时钟控制寄存器） 寄存器
  *                 - 010: BDCR（备份域控制寄存器） 寄存器
  *                 - 011: CSR（控制/状态寄存器） 寄存器
  * @{
  */
/* CR（时钟控制寄存器） 寄存器中的标志 */
#define RCC_FLAG_HSIRDY                  ((uint8_t)((CR_REG_INDEX << 5U) | RCC_CR_HSIRDY_Pos)) /*!< Internal High Speed 时钟就绪 flag */
#define RCC_FLAG_HSERDY                  ((uint8_t)((CR_REG_INDEX << 5U) | RCC_CR_HSERDY_Pos)) /*!< External High Speed 时钟就绪 flag */
#define RCC_FLAG_PLLRDY                  ((uint8_t)((CR_REG_INDEX << 5U) | RCC_CR_PLLRDY_Pos)) /*!< PLL 时钟就绪 flag */

/* CSR（备份域控制寄存器） 寄存器中的标志 */
#define RCC_FLAG_LSIRDY                  ((uint8_t)((CSR_REG_INDEX << 5U) | RCC_CSR_LSIRDY_Pos))   /*!< Internal Low Speed oscillator Ready */
#define RCC_FLAG_PINRST                  ((uint8_t)((CSR_REG_INDEX << 5U) | RCC_CSR_PINRSTF_Pos))  /*!< PIN reset flag */
#define RCC_FLAG_PORRST                  ((uint8_t)((CSR_REG_INDEX << 5U) | RCC_CSR_PORRSTF_Pos))  /*!< POR/PDR reset flag */
#define RCC_FLAG_SFTRST                  ((uint8_t)((CSR_REG_INDEX << 5U) | RCC_CSR_SFTRSTF_Pos))  /*!< Software Reset flag */
#define RCC_FLAG_IWDGRST                 ((uint8_t)((CSR_REG_INDEX << 5U) | RCC_CSR_IWDGRSTF_Pos)) /*!< Independent Watchdog reset flag */
#define RCC_FLAG_WWDGRST                 ((uint8_t)((CSR_REG_INDEX << 5U) | RCC_CSR_WWDGRSTF_Pos)) /*!< Window watchdog reset flag */
#define RCC_FLAG_LPWRRST                 ((uint8_t)((CSR_REG_INDEX << 5U) | RCC_CSR_LPWRRSTF_Pos)) /*!< Low-Power reset flag */

/* BDCR（控制/状态寄存器） 寄存器中的标志 */
#define RCC_FLAG_LSERDY                  ((uint8_t)((BDCR_REG_INDEX << 5U) | RCC_BDCR_LSERDY_Pos)) /*!< External Low Speed oscillator Ready */

/**
  * @}
  */

/**
  * @}
  */

/* 导出宏 ------------------------------------------------------------*/

/** @defgroup RCC_Exported_Macros RCC 导出宏
  * @{
  */

/** @defgroup RCC_Peripheral_Clock_Enable_Disable AHB1总线上外设的时钟使能/不使能
  * @brief  启用或禁用AHB1总线上的外设时钟
  * @note   复位后，外设时钟(用于寄存器读/写访问)被禁用，应用软件在使用它之前必须启用这个时钟。
  * @{
  */
#define __HAL_RCC_DMA1_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHBENR, RCC_AHBENR_DMA1EN);\
                                        /* RCC外设时钟使能后的延迟 */\
                                        tmpreg = READ_BIT(RCC->AHBENR, RCC_AHBENR_DMA1EN);\
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_SRAM_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHBENR, RCC_AHBENR_SRAMEN);\
                                        /* RCC外设时钟使能后的延迟 */\
                                        tmpreg = READ_BIT(RCC->AHBENR, RCC_AHBENR_SRAMEN);\
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_FLITF_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHBENR, RCC_AHBENR_FLITFEN);\
                                        /* RCC外设时钟使能后的延迟 */\
                                        tmpreg = READ_BIT(RCC->AHBENR, RCC_AHBENR_FLITFEN);\
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_CRC_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHBENR, RCC_AHBENR_CRCEN);\
                                        /* RCC外设时钟使能后的延迟 */\
                                        tmpreg = READ_BIT(RCC->AHBENR, RCC_AHBENR_CRCEN);\
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_DMA1_CLK_DISABLE()      (RCC->AHBENR &= ~(RCC_AHBENR_DMA1EN))
#define __HAL_RCC_SRAM_CLK_DISABLE()      (RCC->AHBENR &= ~(RCC_AHBENR_SRAMEN))
#define __HAL_RCC_FLITF_CLK_DISABLE()     (RCC->AHBENR &= ~(RCC_AHBENR_FLITFEN))
#define __HAL_RCC_CRC_CLK_DISABLE()       (RCC->AHBENR &= ~(RCC_AHBENR_CRCEN))

/**
  * @}
  */

/** @defgroup RCC_AHB_Peripheral_Clock_Enable_Disable_Status AHB总线上外设时钟启用或禁用状态
  * @brief  获取AHB总线上外设时钟的启用或禁用状态。
  * @note   复位后，外设时钟(用于寄存器读/写访问)被禁用，应用软件在使用它之前必须启用这个时钟。
  * @{
  */

#define __HAL_RCC_DMA1_IS_CLK_ENABLED()       ((RCC->AHBENR & (RCC_AHBENR_DMA1EN)) != RESET)
#define __HAL_RCC_DMA1_IS_CLK_DISABLED()      ((RCC->AHBENR & (RCC_AHBENR_DMA1EN)) == RESET)
#define __HAL_RCC_SRAM_IS_CLK_ENABLED()       ((RCC->AHBENR & (RCC_AHBENR_SRAMEN)) != RESET)
#define __HAL_RCC_SRAM_IS_CLK_DISABLED()      ((RCC->AHBENR & (RCC_AHBENR_SRAMEN)) == RESET)
#define __HAL_RCC_FLITF_IS_CLK_ENABLED()       ((RCC->AHBENR & (RCC_AHBENR_FLITFEN)) != RESET)
#define __HAL_RCC_FLITF_IS_CLK_DISABLED()      ((RCC->AHBENR & (RCC_AHBENR_FLITFEN)) == RESET)
#define __HAL_RCC_CRC_IS_CLK_ENABLED()       ((RCC->AHBENR & (RCC_AHBENR_CRCEN)) != RESET)
#define __HAL_RCC_CRC_IS_CLK_DISABLED()      ((RCC->AHBENR & (RCC_AHBENR_CRCEN)) == RESET)

/**
  * @}
  */

/** @defgroup RCC_APB1_Clock_Enable_Disable APB1总线上外设的时钟使能/关闭
  * @brief  使能/关闭APB1 (低速APB)总线上的外设的时钟。
  * @note   复位后，外设时钟(用于寄存器读/写访问)被禁用，应用软件在使用它之前必须启用这个时钟。
  * @{
  */
#define __HAL_RCC_TIM2_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM2EN);\
                                        /* RCC外设时钟使能后的延迟 */\
                                        tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM2EN);\
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_TIM3_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM3EN);\
                                        /* RCC外设时钟使能后的延迟 */\
                                        tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM3EN);\
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_WWDG_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_WWDGEN);\
                                        /* RCC外设时钟使能后的延迟 */\
                                        tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_WWDGEN);\
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_USART2_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN);\
                                        /* RCC外设时钟使能后的延迟 */\
                                        tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN);\
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_I2C1_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_I2C1EN);\
                                        /* RCC外设时钟使能后的延迟 */\
                                        tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_I2C1EN);\
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_BKP_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_BKPEN);\
                                        /* RCC外设时钟使能后的延迟 */\
                                        tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_BKPEN);\
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_PWR_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN);\
                                        /* RCC外设时钟使能后的延迟 */\
                                        tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN);\
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_TIM2_CLK_DISABLE()      (RCC->APB1ENR &= ~(RCC_APB1ENR_TIM2EN))
#define __HAL_RCC_TIM3_CLK_DISABLE()      (RCC->APB1ENR &= ~(RCC_APB1ENR_TIM3EN))
#define __HAL_RCC_WWDG_CLK_DISABLE()      (RCC->APB1ENR &= ~(RCC_APB1ENR_WWDGEN))
#define __HAL_RCC_USART2_CLK_DISABLE()    (RCC->APB1ENR &= ~(RCC_APB1ENR_USART2EN))
#define __HAL_RCC_I2C1_CLK_DISABLE()      (RCC->APB1ENR &= ~(RCC_APB1ENR_I2C1EN))

#define __HAL_RCC_BKP_CLK_DISABLE()       (RCC->APB1ENR &= ~(RCC_APB1ENR_BKPEN))
#define __HAL_RCC_PWR_CLK_DISABLE()       (RCC->APB1ENR &= ~(RCC_APB1ENR_PWREN))

/**
  * @}
  */

/** @defgroup RCC_APB1_Peripheral_Clock_Enable_Disable_Status APB1总线上外设的时钟启用或禁用状态
  * @brief  获取APB1总线上外设时钟的启用或禁用状态。
  * @note   复位后，外设时钟(用于寄存器读/写访问)被禁用，应用软件在使用它之前必须启用这个时钟。
  * @{
  */

#define __HAL_RCC_TIM2_IS_CLK_ENABLED()       ((RCC->APB1ENR & (RCC_APB1ENR_TIM2EN)) != RESET)
#define __HAL_RCC_TIM2_IS_CLK_DISABLED()      ((RCC->APB1ENR & (RCC_APB1ENR_TIM2EN)) == RESET)
#define __HAL_RCC_TIM3_IS_CLK_ENABLED()       ((RCC->APB1ENR & (RCC_APB1ENR_TIM3EN)) != RESET)
#define __HAL_RCC_TIM3_IS_CLK_DISABLED()      ((RCC->APB1ENR & (RCC_APB1ENR_TIM3EN)) == RESET)
#define __HAL_RCC_WWDG_IS_CLK_ENABLED()       ((RCC->APB1ENR & (RCC_APB1ENR_WWDGEN)) != RESET)
#define __HAL_RCC_WWDG_IS_CLK_DISABLED()      ((RCC->APB1ENR & (RCC_APB1ENR_WWDGEN)) == RESET)
#define __HAL_RCC_USART2_IS_CLK_ENABLED()     ((RCC->APB1ENR & (RCC_APB1ENR_USART2EN)) != RESET)
#define __HAL_RCC_USART2_IS_CLK_DISABLED()    ((RCC->APB1ENR & (RCC_APB1ENR_USART2EN)) == RESET)
#define __HAL_RCC_I2C1_IS_CLK_ENABLED()       ((RCC->APB1ENR & (RCC_APB1ENR_I2C1EN)) != RESET)
#define __HAL_RCC_I2C1_IS_CLK_DISABLED()      ((RCC->APB1ENR & (RCC_APB1ENR_I2C1EN)) == RESET)
#define __HAL_RCC_BKP_IS_CLK_ENABLED()        ((RCC->APB1ENR & (RCC_APB1ENR_BKPEN)) != RESET)
#define __HAL_RCC_BKP_IS_CLK_DISABLED()       ((RCC->APB1ENR & (RCC_APB1ENR_BKPEN)) == RESET)
#define __HAL_RCC_PWR_IS_CLK_ENABLED()        ((RCC->APB1ENR & (RCC_APB1ENR_PWREN)) != RESET)
#define __HAL_RCC_PWR_IS_CLK_DISABLED()       ((RCC->APB1ENR & (RCC_APB1ENR_PWREN)) == RESET)

/**
  * @}
  */

/** @defgroup RCC_APB2_Clock_Enable_Disable APB2总线上外设的时钟启用或禁用
  * @brief  启用或禁用高速APB (APB2)总线上外设的时钟.
  * @note   复位后，外设时钟(用于寄存器读/写访问)被禁用，应用软件在使用它之前必须启用这个时钟。
  * @{
  */
#define __HAL_RCC_AFIO_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_AFIOEN);\
                                        /* RCC外设时钟使能后的延迟 */\
                                        tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_AFIOEN);\
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_GPIOA_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPAEN);\
                                        /* RCC外设时钟使能后的延迟 */\
                                        tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPAEN);\
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_GPIOB_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPBEN);\
                                        /* RCC外设时钟使能后的延迟 */\
                                        tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPBEN);\
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_GPIOC_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPCEN);\
                                        /* RCC外设时钟使能后的延迟 */\
                                        tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPCEN);\
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_GPIOD_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPDEN);\
                                        /* RCC外设时钟使能后的延迟 */\
                                        tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPDEN);\
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_ADC1_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC1EN);\
                                        /* RCC外设时钟使能后的延迟 */\
                                        tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC1EN);\
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_TIM1_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN);\
                                        /* RCC外设时钟使能后的延迟 */\
                                        tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN);\
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_SPI1_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN);\
                                        /* RCC外设时钟使能后的延迟 */\
                                        tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN);\
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_USART1_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN);\
                                        /* RCC外设时钟使能后的延迟 */\
                                        tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN);\
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_AFIO_CLK_DISABLE()      (RCC->APB2ENR &= ~(RCC_APB2ENR_AFIOEN))
#define __HAL_RCC_GPIOA_CLK_DISABLE()     (RCC->APB2ENR &= ~(RCC_APB2ENR_IOPAEN))
#define __HAL_RCC_GPIOB_CLK_DISABLE()     (RCC->APB2ENR &= ~(RCC_APB2ENR_IOPBEN))
#define __HAL_RCC_GPIOC_CLK_DISABLE()     (RCC->APB2ENR &= ~(RCC_APB2ENR_IOPCEN))
#define __HAL_RCC_GPIOD_CLK_DISABLE()     (RCC->APB2ENR &= ~(RCC_APB2ENR_IOPDEN))
#define __HAL_RCC_ADC1_CLK_DISABLE()      (RCC->APB2ENR &= ~(RCC_APB2ENR_ADC1EN))

#define __HAL_RCC_TIM1_CLK_DISABLE()      (RCC->APB2ENR &= ~(RCC_APB2ENR_TIM1EN))
#define __HAL_RCC_SPI1_CLK_DISABLE()      (RCC->APB2ENR &= ~(RCC_APB2ENR_SPI1EN))
#define __HAL_RCC_USART1_CLK_DISABLE()    (RCC->APB2ENR &= ~(RCC_APB2ENR_USART1EN))

/**
  * @}
  */

/** @defgroup RCC_APB2_Peripheral_Clock_Enable_Disable_Status APB2总线上外设的时钟状态
  * @brief  获取APB2总线上外设的时钟启用或禁用状态
  * @note   复位后，外设时钟(用于寄存器读/写访问)被禁用，应用软件在使用它之前必须启用这个时钟。
  * @{
  */

#define __HAL_RCC_AFIO_IS_CLK_ENABLED()       ((RCC->APB2ENR & (RCC_APB2ENR_AFIOEN)) != RESET)
#define __HAL_RCC_AFIO_IS_CLK_DISABLED()      ((RCC->APB2ENR & (RCC_APB2ENR_AFIOEN)) == RESET)
#define __HAL_RCC_GPIOA_IS_CLK_ENABLED()      ((RCC->APB2ENR & (RCC_APB2ENR_IOPAEN)) != RESET)
#define __HAL_RCC_GPIOA_IS_CLK_DISABLED()     ((RCC->APB2ENR & (RCC_APB2ENR_IOPAEN)) == RESET)
#define __HAL_RCC_GPIOB_IS_CLK_ENABLED()      ((RCC->APB2ENR & (RCC_APB2ENR_IOPBEN)) != RESET)
#define __HAL_RCC_GPIOB_IS_CLK_DISABLED()     ((RCC->APB2ENR & (RCC_APB2ENR_IOPBEN)) == RESET)
#define __HAL_RCC_GPIOC_IS_CLK_ENABLED()      ((RCC->APB2ENR & (RCC_APB2ENR_IOPCEN)) != RESET)
#define __HAL_RCC_GPIOC_IS_CLK_DISABLED()     ((RCC->APB2ENR & (RCC_APB2ENR_IOPCEN)) == RESET)
#define __HAL_RCC_GPIOD_IS_CLK_ENABLED()      ((RCC->APB2ENR & (RCC_APB2ENR_IOPDEN)) != RESET)
#define __HAL_RCC_GPIOD_IS_CLK_DISABLED()     ((RCC->APB2ENR & (RCC_APB2ENR_IOPDEN)) == RESET)
#define __HAL_RCC_ADC1_IS_CLK_ENABLED()       ((RCC->APB2ENR & (RCC_APB2ENR_ADC1EN)) != RESET)
#define __HAL_RCC_ADC1_IS_CLK_DISABLED()      ((RCC->APB2ENR & (RCC_APB2ENR_ADC1EN)) == RESET)
#define __HAL_RCC_TIM1_IS_CLK_ENABLED()       ((RCC->APB2ENR & (RCC_APB2ENR_TIM1EN)) != RESET)
#define __HAL_RCC_TIM1_IS_CLK_DISABLED()      ((RCC->APB2ENR & (RCC_APB2ENR_TIM1EN)) == RESET)
#define __HAL_RCC_SPI1_IS_CLK_ENABLED()       ((RCC->APB2ENR & (RCC_APB2ENR_SPI1EN)) != RESET)
#define __HAL_RCC_SPI1_IS_CLK_DISABLED()      ((RCC->APB2ENR & (RCC_APB2ENR_SPI1EN)) == RESET)
#define __HAL_RCC_USART1_IS_CLK_ENABLED()     ((RCC->APB2ENR & (RCC_APB2ENR_USART1EN)) != RESET)
#define __HAL_RCC_USART1_IS_CLK_DISABLED()    ((RCC->APB2ENR & (RCC_APB2ENR_USART1EN)) == RESET)

/**
  * @}
  */

/** @defgroup RCC_APB1_Force_Release_Reset APB1总线上外设强制/释放复位
  * @brief  强制/释放复位APB1总线上外设
  * @{
  */
#define __HAL_RCC_APB1_FORCE_RESET()       (RCC->APB1RSTR = 0xFFFFFFFFU)
#define __HAL_RCC_TIM2_FORCE_RESET()       (RCC->APB1RSTR |= (RCC_APB1RSTR_TIM2RST))
#define __HAL_RCC_TIM3_FORCE_RESET()       (RCC->APB1RSTR |= (RCC_APB1RSTR_TIM3RST))
#define __HAL_RCC_WWDG_FORCE_RESET()       (RCC->APB1RSTR |= (RCC_APB1RSTR_WWDGRST))
#define __HAL_RCC_USART2_FORCE_RESET()     (RCC->APB1RSTR |= (RCC_APB1RSTR_USART2RST))
#define __HAL_RCC_I2C1_FORCE_RESET()       (RCC->APB1RSTR |= (RCC_APB1RSTR_I2C1RST))

#define __HAL_RCC_BKP_FORCE_RESET()        (RCC->APB1RSTR |= (RCC_APB1RSTR_BKPRST))
#define __HAL_RCC_PWR_FORCE_RESET()        (RCC->APB1RSTR |= (RCC_APB1RSTR_PWRRST))

#define __HAL_RCC_APB1_RELEASE_RESET()      (RCC->APB1RSTR = 0x00)
#define __HAL_RCC_TIM2_RELEASE_RESET()       (RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM2RST))
#define __HAL_RCC_TIM3_RELEASE_RESET()       (RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM3RST))
#define __HAL_RCC_WWDG_RELEASE_RESET()       (RCC->APB1RSTR &= ~(RCC_APB1RSTR_WWDGRST))
#define __HAL_RCC_USART2_RELEASE_RESET()     (RCC->APB1RSTR &= ~(RCC_APB1RSTR_USART2RST))
#define __HAL_RCC_I2C1_RELEASE_RESET()       (RCC->APB1RSTR &= ~(RCC_APB1RSTR_I2C1RST))

#define __HAL_RCC_BKP_RELEASE_RESET()        (RCC->APB1RSTR &= ~(RCC_APB1RSTR_BKPRST))
#define __HAL_RCC_PWR_RELEASE_RESET()        (RCC->APB1RSTR &= ~(RCC_APB1RSTR_PWRRST))

/**
  * @}
  */

/** @defgroup RCC_APB2_Force_Release_Reset APB2总线上外设强制/释放复位
  * @brief  强制/释放复位APB2总线上的外设
  * @{
  */
#define __HAL_RCC_APB2_FORCE_RESET()       (RCC->APB2RSTR = 0xFFFFFFFFU)
#define __HAL_RCC_AFIO_FORCE_RESET()       (RCC->APB2RSTR |= (RCC_APB2RSTR_AFIORST))
#define __HAL_RCC_GPIOA_FORCE_RESET()      (RCC->APB2RSTR |= (RCC_APB2RSTR_IOPARST))
#define __HAL_RCC_GPIOB_FORCE_RESET()      (RCC->APB2RSTR |= (RCC_APB2RSTR_IOPBRST))
#define __HAL_RCC_GPIOC_FORCE_RESET()      (RCC->APB2RSTR |= (RCC_APB2RSTR_IOPCRST))
#define __HAL_RCC_GPIOD_FORCE_RESET()      (RCC->APB2RSTR |= (RCC_APB2RSTR_IOPDRST))
#define __HAL_RCC_ADC1_FORCE_RESET()       (RCC->APB2RSTR |= (RCC_APB2RSTR_ADC1RST))

#define __HAL_RCC_TIM1_FORCE_RESET()       (RCC->APB2RSTR |= (RCC_APB2RSTR_TIM1RST))
#define __HAL_RCC_SPI1_FORCE_RESET()       (RCC->APB2RSTR |= (RCC_APB2RSTR_SPI1RST))
#define __HAL_RCC_USART1_FORCE_RESET()     (RCC->APB2RSTR |= (RCC_APB2RSTR_USART1RST))

#define __HAL_RCC_APB2_RELEASE_RESET()      (RCC->APB2RSTR = 0x00)
#define __HAL_RCC_AFIO_RELEASE_RESET()       (RCC->APB2RSTR &= ~(RCC_APB2RSTR_AFIORST))
#define __HAL_RCC_GPIOA_RELEASE_RESET()      (RCC->APB2RSTR &= ~(RCC_APB2RSTR_IOPARST))
#define __HAL_RCC_GPIOB_RELEASE_RESET()      (RCC->APB2RSTR &= ~(RCC_APB2RSTR_IOPBRST))
#define __HAL_RCC_GPIOC_RELEASE_RESET()      (RCC->APB2RSTR &= ~(RCC_APB2RSTR_IOPCRST))
#define __HAL_RCC_GPIOD_RELEASE_RESET()      (RCC->APB2RSTR &= ~(RCC_APB2RSTR_IOPDRST))
#define __HAL_RCC_ADC1_RELEASE_RESET()       (RCC->APB2RSTR &= ~(RCC_APB2RSTR_ADC1RST))

#define __HAL_RCC_TIM1_RELEASE_RESET()       (RCC->APB2RSTR &= ~(RCC_APB2RSTR_TIM1RST))
#define __HAL_RCC_SPI1_RELEASE_RESET()       (RCC->APB2RSTR &= ~(RCC_APB2RSTR_SPI1RST))
#define __HAL_RCC_USART1_RELEASE_RESET()     (RCC->APB2RSTR &= ~(RCC_APB2RSTR_USART1RST))

/**
  * @}
  */

/** @defgroup RCC_HSI_Configuration HSI 配置
  * @{
  */

/** @brief  启用或禁用内部高速振荡器(HSI)的宏。
  * @note   当进入STOP和STANDBY模式时，HSI被硬件停止。
  * @note   HSI作为系统时钟源时不能停止。在这种情况下，您必须选择另一个系统时钟源，然后停止HSI。
  * @note   在使能HSI后，应用程序应当等待 HSIRDY 标志被置位表示 HSI 稳定，才可以被用于系统时钟源。
  * @note   当 HSI 被停止时，需要在6个HSI周期后，HSIRDY 标志才会被拉低（0）。
  */
#define __HAL_RCC_HSI_ENABLE()  (*(__IO uint32_t *) RCC_CR_HSION_BB = ENABLE)
#define __HAL_RCC_HSI_DISABLE() (*(__IO uint32_t *) RCC_CR_HSION_BB = DISABLE)

/** @brief  调整内部高速振荡器(HSI)校准值的宏。
  * @note   校准用于补偿由于电源和温度的变化对内部HSI RC震荡器的影响。
  * @param  _HSICALIBRATIONVALUE_ 指定校准微调值。
  *        (默认值为 RCC_HSICALIBRATION_DEFAULT).
  *         此参数的取值范围 0 到 0x1F.
  */
#define __HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(_HSICALIBRATIONVALUE_) \
          (MODIFY_REG(RCC->CR, RCC_CR_HSITRIM, (uint32_t)(_HSICALIBRATIONVALUE_) << RCC_CR_HSITRIM_Pos))

/**
  * @}
  */

/** @defgroup RCC_LSI_Configuration  LSI 配置
  * @{
  */

/** @brief 使能内部低速振荡器(LSI) 的宏.
  * @note   在使能LSI后，应用软件需要等待 LSIRDY 标志置位，表示LSI时钟稳定，才可以用于作为IWDG/RTC运行时钟。
  */
#define __HAL_RCC_LSI_ENABLE()  (*(__IO uint32_t *) RCC_CSR_LSION_BB = ENABLE)

/** @brief 失能内部低速振荡器(LSI) 的宏.
  * @note  当独立看门狗（IWDG）正在运行时，LSI不能被禁用。
  * @note  当LSI芯片停止工作时，经过6个LSI振荡器时钟周期后，LSIRDY标志位变为低（0）。
  */
#define __HAL_RCC_LSI_DISABLE() (*(__IO uint32_t *) RCC_CSR_LSION_BB = DISABLE)

/**
  * @}
  */

/** @defgroup RCC_HSE_Configuration HSE 配置
  * @{
  */

/**
  * @brief  配置外部高速振荡器(HSE) 的宏.
  * @note   此宏不支持将HSE Bypass转换为HSE On和将HSE On转换为HSE Bypass。
  *         应先切换到HSE Off，然后再切换到HSE On或HSE Bypass。
  * @note   在使能HSE (RCC_HSE_ON或RCC_HSE_Bypass)后，应用软件需要等待 HSERDY 标志设置，
  *         表示HSE时钟稳定，才可以用于锁相环（PLL）时钟源 / 系统时钟源。
  * @note   如果直接使用HSE 或 通过锁相环作为系统时钟，则无法直接改变HSE状态。
  *         在这种情况下，必须先选择另一个系统时钟源，然后才能更改HSE状态(例如禁用它)。
  * @note   在进入 STOP 和 STANDBY 模式后HSE硬件停止。
  * @note   这个函数重置CSSON位，所以如果以前启用了时钟安全系统(CSS)，则必须在调用该函数后再次启用它。
  * @param  __STATE__ 指定HSE的新状态。
  *          该参数可以是以下值之一:
  *            @arg @ref RCC_HSE_OFF    关闭 HSE 振荡器, HSERDY 标志在6个HSE震荡周期后置0
  *            @arg @ref RCC_HSE_ON     开启 HSE 振荡器
  *            @arg @ref RCC_HSE_BYPASS HSE振荡器使用外部时钟旁路
  */
#define __HAL_RCC_HSE_CONFIG(__STATE__)                                     \
                    do{                                                     \
                      if ((__STATE__) == RCC_HSE_ON)                        \
                      {                                                     \
                        SET_BIT(RCC->CR, RCC_CR_HSEON);                     \
                      }                                                     \
                      else if ((__STATE__) == RCC_HSE_OFF)                  \
                      {                                                     \
                        CLEAR_BIT(RCC->CR, RCC_CR_HSEON);                   \
                        CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);                  \
                      }                                                     \
                      else if ((__STATE__) == RCC_HSE_BYPASS)               \
                      {                                                     \
                        SET_BIT(RCC->CR, RCC_CR_HSEBYP);                    \
                        SET_BIT(RCC->CR, RCC_CR_HSEON);                     \
                      }                                                     \
                      else                                                  \
                      {                                                     \
                        CLEAR_BIT(RCC->CR, RCC_CR_HSEON);                   \
                        CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);                  \
                      }                                                     \
                    }while(0U)

/**
  * @}
  */

/** @defgroup RCC_LSE_Configuration LSE 配置
  * @{
  */

/**
  * @brief  配置外部低速振荡器(LSE) 的宏.
  * @note   这个宏不支持LSE Bypass到LSE On 和 LSE On到LSE Bypass的转换。
  * @note   由于LSE在备份域中，并且在复位后该域拒绝进行写访问，因此在配置LSE之前，
  *         必须使用 @ref HAL_PWR_EnableBkUpAccess() 函数启用写访问（在复位后执行一次）。
  * @note   使能LSE (RCC_LSE_ON或RCC_LSE_BYPASS)后，应用软件需要等待LSERDY标志设置，
  *         表示LSE时钟稳定，才可以用来作为RTC运行时钟。
  * @param  __STATE__ 指定LSE的新状态
  *         该参数可以是以下值之一:
  *            @arg @ref RCC_LSE_OFF    关闭 LSE 振荡器, LSERDY 标志在6个LSE震荡周期后置0
  *            @arg @ref RCC_LSE_ON     开启 LSE 振荡器
  *            @arg @ref RCC_LSE_BYPASS LSE振荡器与外部时钟旁路
  */
#define __HAL_RCC_LSE_CONFIG(__STATE__)                                     \
                    do{                                                     \
                      if ((__STATE__) == RCC_LSE_ON)                        \
                      {                                                     \
                        SET_BIT(RCC->BDCR, RCC_BDCR_LSEON);                   \
                      }                                                     \
                      else if ((__STATE__) == RCC_LSE_OFF)                  \
                      {                                                     \
                        CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEON);                 \
                        CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEBYP);                \
                      }                                                     \
                      else if ((__STATE__) == RCC_LSE_BYPASS)               \
                      {                                                     \
                        SET_BIT(RCC->BDCR, RCC_BDCR_LSEBYP);                  \
                        SET_BIT(RCC->BDCR, RCC_BDCR_LSEON);                   \
                      }                                                     \
                      else                                                  \
                      {                                                     \
                        CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEON);                 \
                        CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEBYP);                \
                      }                                                     \
                    }while(0U)

/**
  * @}
  */

/** @defgroup RCC_PLL_Configuration PLL 配置
  * @{
  */

/** @brief  使能主锁相环的宏。
  * @note   使能主锁相环后，应用软件需要等待PLLRDY标志设置，表示锁相环时钟稳定，才可以作为系统时钟源。
  * @note   当进入停止和待机模式时，主锁相环被硬件禁用。
  */
#define __HAL_RCC_PLL_ENABLE()          (*(__IO uint32_t *) RCC_CR_PLLON_BB = ENABLE)

/** @brief  关闭主锁相环的宏
  * @note   如果主锁相环用作系统时钟源，则不能关闭。
  */
#define __HAL_RCC_PLL_DISABLE()         (*(__IO uint32_t *) RCC_CR_PLLON_BB = DISABLE)

/** @brief  配置主锁相环时钟源和倍频系数的宏
  * @note   此函数只能在关闭主锁相环时使用。
  *
  * @param  __RCC_PLLSOURCE__ 指定锁相环输入时钟源
  *          该参数可以是以下值之一:
  *            @arg @ref RCC_PLLSOURCE_HSI_DIV2 HSI时钟 2 分频作为 PLL 的时钟源
  *            @arg @ref RCC_PLLSOURCE_HSE      HSE时钟不分频作为 PLL 的时钟源
  * @param  __PLLMUL__ 指定锁相环倍频系数
  *          该参数可以是以下值之一:
  *             @arg @ref RCC_PLL_MUL4   PLLVCO = PLL 输入时钟 x 4
  *             @arg @ref RCC_PLL_MUL6   PLLVCO = PLL 输入时钟 x 6
  @if STM32F105xC
  *             @arg @ref RCC_PLL_MUL6_5 PLLVCO = PLL 输入时钟 x 6.5
  @elseif STM32F107xC
  *             @arg @ref RCC_PLL_MUL6_5 PLLVCO = PLL 输入时钟 x 6.5
  @else
  *             @arg @ref RCC_PLL_MUL2   PLLVCO = PLL 输入时钟 x 2
  *             @arg @ref RCC_PLL_MUL3   PLLVCO = PLL 输入时钟 x 3
  *             @arg @ref RCC_PLL_MUL10  PLLVCO = PLL 输入时钟 x 10
  *             @arg @ref RCC_PLL_MUL11  PLLVCO = PLL 输入时钟 x 11
  *             @arg @ref RCC_PLL_MUL12  PLLVCO = PLL 输入时钟 x 12
  *             @arg @ref RCC_PLL_MUL13  PLLVCO = PLL 输入时钟 x 13
  *             @arg @ref RCC_PLL_MUL14  PLLVCO = PLL 输入时钟 x 14
  *             @arg @ref RCC_PLL_MUL15  PLLVCO = PLL 输入时钟 x 15
  *             @arg @ref RCC_PLL_MUL16  PLLVCO = PLL 输入时钟 x 16
  @endif
  *             @arg @ref RCC_PLL_MUL8   PLLVCO = PLL 输入时钟 x 8
  *             @arg @ref RCC_PLL_MUL9   PLLVCO = PLL 输入时钟 x 9
  *
  */
#define __HAL_RCC_PLL_CONFIG(__RCC_PLLSOURCE__, __PLLMUL__)\
          MODIFY_REG(RCC->CFGR, (RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL),((__RCC_PLLSOURCE__) | (__PLLMUL__) ))

/** @brief  获取PLL输入时钟源
  * @retval 用于锁相环输入的时钟源。返回值可以是下列值之一：
  *             @arg @ref RCC_PLLSOURCE_HSI_DIV2  HSI 振荡器时钟2分频作为PLL输入时钟
  *             @arg @ref RCC_PLLSOURCE_HSE       HSE 振荡器时钟作为PLL输入时钟
  */
#define __HAL_RCC_GET_PLL_OSCSOURCE() ((uint32_t)(READ_BIT(RCC->CFGR, RCC_CFGR_PLLSRC)))

/**
  * @}
  */

/** @defgroup RCC_Get_Clock_source 获取时钟源
  * @{
  */

/**
  * @brief  配置系统时钟源的宏。
  * @param  __SYSCLKSOURCE__ 指定系统时钟源
  *          该参数可以是以下值之一:
  *              @arg @ref RCC_SYSCLKSOURCE_HSI     HSI 震荡器被作为系统时钟源
  *              @arg @ref RCC_SYSCLKSOURCE_HSE     HSE 震荡器被作为系统时钟源
  *              @arg @ref RCC_SYSCLKSOURCE_PLLCLK  PLL 输出被作为系统时钟源
  */
#define __HAL_RCC_SYSCLK_CONFIG(__SYSCLKSOURCE__) \
                  MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, (__SYSCLKSOURCE__))

/** @brief  获取系统时钟的时钟源
  * @retval 作为系统时钟的时钟源。返回值可以是下列值之一:
  *             @arg @ref RCC_SYSCLKSOURCE_STATUS_HSI     HSI 被用作系统时钟
  *             @arg @ref RCC_SYSCLKSOURCE_STATUS_HSE     HSE 被用作系统时钟
  *             @arg @ref RCC_SYSCLKSOURCE_STATUS_PLLCLK  PLL 被用作系统时钟
  */
#define __HAL_RCC_GET_SYSCLK_SOURCE() ((uint32_t)(READ_BIT(RCC->CFGR,RCC_CFGR_SWS)))

/**
  * @}
  */

/** @defgroup RCCEx_MCOx_Clock_Config RCC扩展MCOx时钟输出配置
  * @{
  */

#if   defined(RCC_CFGR_MCO_3)
/** @brief  配置MCO时钟的宏
  * @param  __MCOCLKSOURCE__ 指定 MCO 时钟源.
  *         该参数可以是以下值之一:
  *            @arg @ref RCC_MCO1SOURCE_NOCLOCK      MCO 无时钟
  *            @arg @ref RCC_MCO1SOURCE_SYSCLK       系统时钟 (SYSCLK) 作为 MCO 时钟
  *            @arg @ref RCC_MCO1SOURCE_HSI          HSI 作为 MCO 时钟
  *            @arg @ref RCC_MCO1SOURCE_HSE          HSE 作为 MCO 时钟
  *            @arg @ref RCC_MCO1SOURCE_PLLCLK       PLL 时钟输出2分频作为 MCO 时钟
  *            @arg @ref RCC_MCO1SOURCE_PLL2CLK      PLL2 时钟输出2分频作为 MCO 时钟
  *            @arg @ref RCC_MCO1SOURCE_PLL3CLK_DIV2 PLL3 时钟输出2分频作为 MCO 时钟
  *            @arg @ref RCC_MCO1SOURCE_EXT_HSE XT1  外部3-25 MHz振荡器时钟被选为以太网MCO时钟
  *            @arg @ref RCC_MCO1SOURCE_PLL3CLK      PLL3 时钟作为以 MCO 时钟输出的时钟，用于以太网
  * @param  __MCODIV__ 指定 MCO 时钟预分频
  *         该参数可以是以下值之一:
  *            @arg @ref RCC_MCODIV_1 MCO时钟源上没有可应用的分频
  */
#else
/** @brief  配置MCO时钟的宏
  * @param  __MCOCLKSOURCE__ 指定 MCO 时钟源.
  *         该参数可以是以下值之一:
  *            @arg @ref RCC_MCO1SOURCE_NOCLOCK   MCO 无时钟
  *            @arg @ref RCC_MCO1SOURCE_SYSCLK    系统时钟 (SYSCLK) 作为 MCO 时钟
  *            @arg @ref RCC_MCO1SOURCE_HSI       HSI 作为 MCO 时钟
  *            @arg @ref RCC_MCO1SOURCE_HSE       HSE 作为 MCO 时钟
  *            @arg @ref RCC_MCO1SOURCE_PLLCLK    PLL 时钟输出2分频作为 MCO 时钟
  * @param  __MCODIV__ 指定 MCO 时钟预分频.
  *         该参数可以是以下值之一:
  *            @arg @ref RCC_MCODIV_1 MCO时钟源上没有可应用的分频
  */
#endif

#define __HAL_RCC_MCO1_CONFIG(__MCOCLKSOURCE__, __MCODIV__) \
                 MODIFY_REG(RCC->CFGR, RCC_CFGR_MCO, (__MCOCLKSOURCE__))


/**
  * @}
  */

/** @defgroup RCC_RTC_Clock_Configuration RCC RTC Clock 时钟配置
* @{
*/

/** @brief  配置 RTC 时钟源 (RTCCLK) 的宏.
  * @note   由于RTC时钟配置位在备份域中，复位后该域将拒绝写访问，因此在配置RTC时钟源之前，
  *         必须先使能备份域写访问（复位后需要重新配置一次）
  * @note   一旦配置了RTC时钟，它就不能被改变，除非使用 @ref __HAL_RCC_BACKUPRESET_FORCE() 宏重置备份域，
  *         或者通过上电复位重置(POR)。
  *
  * @param  __RTC_CLKSOURCE__ 指定 RTC 时钟源.
  *          该参数可以是以下值之一:
  *             @arg @ref RCC_RTCCLKSOURCE_NO_CLK       RTC 无时钟
  *             @arg @ref RCC_RTCCLKSOURCE_LSE          LSE 作为 RTC 时钟源
  *             @arg @ref RCC_RTCCLKSOURCE_LSI          LSI 作为 RTC 时钟源
  *             @arg @ref RCC_RTCCLKSOURCE_HSE_DIV128   HSE 128分频作为 RTC 时钟源
  * @note   当LSE或LSI作为RTC时钟源时，RTC可以继续工作在STOP和STANDBY模式，也可以作为唤醒源。
  *         但是，当使用HSE时钟作为RTC时钟源时，RTC不能在STOP和STANDBY模式下使用。
  * @note   RTC的最大输入时钟频率为1MHz(使用HSE作为RTC时钟源时)。
  */
#define __HAL_RCC_RTC_CONFIG(__RTC_CLKSOURCE__) MODIFY_REG(RCC->BDCR, RCC_BDCR_RTCSEL, (__RTC_CLKSOURCE__))

/** @brief  获取RTC的时钟源
  * @retval 时钟源可以是以下取值之一:
  *            @arg @ref RCC_RTCCLKSOURCE_NO_CLK      RTC 无时钟
  *            @arg @ref RCC_RTCCLKSOURCE_LSE         LSE 作为 RTC 时钟源
  *            @arg @ref RCC_RTCCLKSOURCE_LSI         LSI 作为 RTC 时钟源
  *            @arg @ref RCC_RTCCLKSOURCE_HSE_DIV128  HSE 128分频作为 RTC 时钟源
  */
#define __HAL_RCC_GET_RTC_SOURCE() (READ_BIT(RCC->BDCR, RCC_BDCR_RTCSEL))

/** @brief  启用RTC时钟的宏。
  * @note   此宏必须在选择RTC时钟源后使用。
  */
#define __HAL_RCC_RTC_ENABLE()          (*(__IO uint32_t *) RCC_BDCR_RTCEN_BB = ENABLE)

/** @brief 关闭RTC时钟的宏。
  * @note  此宏必须在选择RTC时钟源后使用。
  */
#define __HAL_RCC_RTC_DISABLE()         (*(__IO uint32_t *) RCC_BDCR_RTCEN_BB = DISABLE)

/** @brief  强制复位备份域的宏
  * @note   这个函数重置RTC外设(包括备份寄存器)和RCC_BDCR寄存器中的RTC时钟源选择。
  */
#define __HAL_RCC_BACKUPRESET_FORCE()   (*(__IO uint32_t *) RCC_BDCR_BDRST_BB = ENABLE)

/** @brief  释放复位备份域的宏
  */
#define __HAL_RCC_BACKUPRESET_RELEASE() (*(__IO uint32_t *) RCC_BDCR_BDRST_BB = DISABLE)

/**
  * @}
  */

/** @defgroup RCC_Flags_Interrupts_Management 中断标志管理
  * @brief 管理指定的RCC标志和中断的宏。
  * @{
  */

/** @brief  开启 RCC 中断
  * @param  __INTERRUPT__ 指定启用的RCC中断源
  *          该参数可以是以下值的任意组合:
  *            @arg @ref RCC_IT_LSIRDY  LSI 就绪中断
  *            @arg @ref RCC_IT_LSERDY  LSE 就绪中断
  *            @arg @ref RCC_IT_HSIRDY  HSI 就绪中断
  *            @arg @ref RCC_IT_HSERDY  HSE 就绪中断
  *            @arg @ref RCC_IT_PLLRDY  主 PLL 就绪中断
  @if STM32F105xx
  *            @arg @ref RCC_IT_PLL2RDY     主 PLL2 就绪中断.
  *            @arg @ref RCC_IT_PLLI2S2RDY  主 PLLI2S 就绪中断.
  @elsif STM32F107xx
  *            @arg @ref RCC_IT_PLL2RDY     主 PLL2 就绪中断.
  *            @arg @ref RCC_IT_PLLI2S2RDY  主 PLLI2S 就绪中断.
  @endif
  */
#define __HAL_RCC_ENABLE_IT(__INTERRUPT__) (*(__IO uint8_t *) RCC_CIR_BYTE1_ADDRESS |= (__INTERRUPT__))

/** @brief  关闭 RCC 中断
  * @param  __INTERRUPT__ 指定RCC中断源关闭
  *          该参数可以是以下值的任意组合:
  *            @arg @ref RCC_IT_LSIRDY  LSI 就绪中断
  *            @arg @ref RCC_IT_LSERDY  LSE 就绪中断
  *            @arg @ref RCC_IT_HSIRDY  HSI 就绪中断
  *            @arg @ref RCC_IT_HSERDY  HSE 就绪中断
  *            @arg @ref RCC_IT_PLLRDY  主 PLL 就绪中断
  @if STM32F105xx
  *            @arg @ref RCC_IT_PLL2RDY     主 PLL2 就绪中断.
  *            @arg @ref RCC_IT_PLLI2S2RDY  主 PLLI2S 就绪中断.
  @elsif STM32F107xx
  *            @arg @ref RCC_IT_PLL2RDY     主 PLL2 就绪中断.
  *            @arg @ref RCC_IT_PLLI2S2RDY  主 PLLI2S 就绪中断.
  @endif
  */
#define __HAL_RCC_DISABLE_IT(__INTERRUPT__) (*(__IO uint8_t *) RCC_CIR_BYTE1_ADDRESS &= (uint8_t)(~(__INTERRUPT__)))

/** @brief  清除RCC的中断挂起位
  * @param  __INTERRUPT__ 指定被清除的中断挂起位
  *          该参数可以是以下值的任意组合:
  *            @arg @ref RCC_IT_LSIRDY  LSI 就绪中断.
  *            @arg @ref RCC_IT_LSERDY  LSE 就绪中断.
  *            @arg @ref RCC_IT_HSIRDY  HSI 就绪中断.
  *            @arg @ref RCC_IT_HSERDY  HSE 就绪中断.
  *            @arg @ref RCC_IT_PLLRDY  主 PLL 就绪中断.
  @if STM32F105xx
  *            @arg @ref RCC_IT_PLL2RDY     主 PLL2 就绪中断.
  *            @arg @ref RCC_IT_PLLI2S2RDY  主 PLLI2S 就绪中断.
  @elsif STM32F107xx
  *            @arg @ref RCC_IT_PLL2RDY     主 PLL2 就绪中断.
  *            @arg @ref RCC_IT_PLLI2S2RDY  主 PLLI2S 就绪中断.
  @endif
  *            @arg @ref RCC_IT_CSS         时钟安全系统中断
  */
#define __HAL_RCC_CLEAR_IT(__INTERRUPT__) (*(__IO uint8_t *) RCC_CIR_BYTE2_ADDRESS = (__INTERRUPT__))

/** @brief  检查RCC的中断是否发生
  * @param  __INTERRUPT__ 指定被检查的RCC中断源
  *          该参数可以是以下值之一:
  *            @arg @ref RCC_IT_LSIRDY  LSI 就绪中断.
  *            @arg @ref RCC_IT_LSERDY  LSE 就绪中断.
  *            @arg @ref RCC_IT_HSIRDY  HSI 就绪中断.
  *            @arg @ref RCC_IT_HSERDY  HSE 就绪中断.
  *            @arg @ref RCC_IT_PLLRDY  主 PLL 就绪中断.
  @if STM32F105xx
  *            @arg @ref RCC_IT_PLL2RDY     主 PLL2 就绪中断.
  *            @arg @ref RCC_IT_PLLI2S2RDY  主 PLLI2S 就绪中断.
  @elsif STM32F107xx
  *            @arg @ref RCC_IT_PLL2RDY     主 PLL2 就绪中断.
  *            @arg @ref RCC_IT_PLLI2S2RDY  主 PLLI2S 就绪中断.
  @endif
  *            @arg @ref RCC_IT_CSS         时钟安全系统中断
  * @retval __INTERRUPT__ 状态 (TRUE 或 FALSE).
  */
#define __HAL_RCC_GET_IT(__INTERRUPT__) ((RCC->CIR & (__INTERRUPT__)) == (__INTERRUPT__))

/** @brief  设置 RMVF 位来清除复位源标志
  *         复位标志是 RCC_FLAG_PINRST, RCC_FLAG_PORRST, RCC_FLAG_SFTRST,
  *         RCC_FLAG_IWDGRST, RCC_FLAG_WWDGRST, RCC_FLAG_LPWRRST
  */
#define __HAL_RCC_CLEAR_RESET_FLAGS() (*(__IO uint32_t *)RCC_CSR_RMVF_BB = ENABLE)

/** @brief  检查RCC标志是否被置位
  * @param  __FLAG__ 指定需要检查的标志
  *          该参数可以是以下值之一:
  *            @arg @ref RCC_FLAG_HSIRDY  HSI 振荡器时钟就绪.
  *            @arg @ref RCC_FLAG_HSERDY  HSE 振荡器时钟就绪.
  *            @arg @ref RCC_FLAG_PLLRDY  主 PLL 时钟就绪.
  @if STM32F105xx
  *            @arg @ref RCC_FLAG_PLL2RDY     主 PLL2 时钟就绪.
  *            @arg @ref RCC_FLAG_PLLI2SRDY   主 PLLI2S 时钟就绪.
  @elsif STM32F107xx
  *            @arg @ref RCC_FLAG_PLL2RDY     主 PLL2 时钟就绪.
  *            @arg @ref RCC_FLAG_PLLI2SRDY   主 PLLI2S 时钟就绪.
  @endif
  *            @arg @ref RCC_FLAG_LSERDY      LSE 振荡器时钟就绪.
  *            @arg @ref RCC_FLAG_LSIRDY      LSI 振荡器时钟就绪.
  *            @arg @ref RCC_FLAG_PINRST      引脚复位.
  *            @arg @ref RCC_FLAG_PORRST      上电/掉电复位.
  *            @arg @ref RCC_FLAG_SFTRST      软件复位.
  *            @arg @ref RCC_FLAG_IWDGRST     独立看门狗复位.
  *            @arg @ref RCC_FLAG_WWDGRST     窗口看门狗复位.
  *            @arg @ref RCC_FLAG_LPWRRST     低功耗复位.
  * @retval __FLAG__状态 (TRUE 或 FALSE).
  */
#define __HAL_RCC_GET_FLAG(__FLAG__) (((((__FLAG__) >> 5U) == CR_REG_INDEX)?   RCC->CR   : \
                                      ((((__FLAG__) >> 5U) == BDCR_REG_INDEX)? RCC->BDCR : \
                                                                              RCC->CSR)) & (1U << ((__FLAG__) & RCC_FLAG_MASK)))

/**
  * @}
  */

/**
  * @}
  */

/* 引入 RCC HAL 扩展模块 */
#include "stm32f1xx_hal_rcc_ex.h"

/* 导出功能 --------------------------------------------------------*/
/** @addtogroup RCC_Exported_Functions
  * @{
  */

/** @addtogroup RCC_Exported_Functions_Group1
  * @{
  */

/* 初始化和反初始化函数  ******************************/

HAL_StatusTypeDef HAL_RCC_DeInit(void);
HAL_StatusTypeDef HAL_RCC_OscConfig(RCC_OscInitTypeDef  *RCC_OscInitStruct);
HAL_StatusTypeDef HAL_RCC_ClockConfig(RCC_ClkInitTypeDef  *RCC_ClkInitStruct, uint32_t FLatency);

/**
  * @}
  */

/** @addtogroup RCC_Exported_Functions_Group2
  * @{
  */

/* 外设控制功能  ************************************************/

void              HAL_RCC_MCOConfig(uint32_t RCC_MCOx, uint32_t RCC_MCOSource, uint32_t RCC_MCODiv);
void              HAL_RCC_EnableCSS(void);
void              HAL_RCC_DisableCSS(void);
uint32_t          HAL_RCC_GetSysClockFreq(void);
uint32_t          HAL_RCC_GetHCLKFreq(void);
uint32_t          HAL_RCC_GetPCLK1Freq(void);
uint32_t          HAL_RCC_GetPCLK2Freq(void);
void              HAL_RCC_GetOscConfig(RCC_OscInitTypeDef  *RCC_OscInitStruct);
void              HAL_RCC_GetClockConfig(RCC_ClkInitTypeDef  *RCC_ClkInitStruct, uint32_t *pFLatency);

/* CSS NMI 中断处理函数 */

void              HAL_RCC_NMI_IRQHandler(void);

/* 非阻塞模式（IT模式）下的用户回调函数 */

void              HAL_RCC_CSSCallback(void);

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup RCC_Private_Constants
  * @{
  */

/** @defgroup RCC_Timeout RCC Timeout
  * @{
  */

/* 禁用备份域写保护状态更改超时时间 */
#define RCC_DBP_TIMEOUT_VALUE          100U    /* 100 ms */
/* LSE 状态改变超时时间 */
#define RCC_LSE_TIMEOUT_VALUE          LSE_STARTUP_TIMEOUT
#define CLOCKSWITCH_TIMEOUT_VALUE      5000    /* 5 s    */
#define HSE_TIMEOUT_VALUE              HSE_STARTUP_TIMEOUT
#define HSI_TIMEOUT_VALUE              2U      /* 2 ms (minimum Tick + 1) */
#define LSI_TIMEOUT_VALUE              2U      /* 2 ms (minimum Tick + 1) */
#define PLL_TIMEOUT_VALUE              2U      /* 2 ms (minimum Tick + 1) */

/**
  * @}
  */

/** @defgroup RCC_Register_Offset 寄存器偏移
  * @{
  */
#define RCC_OFFSET                (RCC_BASE - PERIPH_BASE)
#define RCC_CR_OFFSET             0x00U
#define RCC_CFGR_OFFSET           0x04U
#define RCC_CIR_OFFSET            0x08U
#define RCC_BDCR_OFFSET           0x20U
#define RCC_CSR_OFFSET            0x24U

/**
  * @}
  */

/** @defgroup RCC_BitAddress_AliasRegion 别名区位地址
  * @brief RCC在别名区域注册位地址
  * @{
  */
#define RCC_CR_OFFSET_BB          (RCC_OFFSET + RCC_CR_OFFSET)
#define RCC_CFGR_OFFSET_BB        (RCC_OFFSET + RCC_CFGR_OFFSET)
#define RCC_CIR_OFFSET_BB         (RCC_OFFSET + RCC_CIR_OFFSET)
#define RCC_BDCR_OFFSET_BB        (RCC_OFFSET + RCC_BDCR_OFFSET)
#define RCC_CSR_OFFSET_BB         (RCC_OFFSET + RCC_CSR_OFFSET)

/* --- CR 寄存器 ---*/
/* HSION 位的别名地址 */
#define RCC_HSION_BIT_NUMBER      RCC_CR_HSION_Pos
#define RCC_CR_HSION_BB           ((uint32_t)(PERIPH_BB_BASE + (RCC_CR_OFFSET_BB * 32U) + (RCC_HSION_BIT_NUMBER * 4U)))
/* HSEON 位的别名地址 */
#define RCC_HSEON_BIT_NUMBER      RCC_CR_HSEON_Pos
#define RCC_CR_HSEON_BB           ((uint32_t)(PERIPH_BB_BASE + (RCC_CR_OFFSET_BB * 32U) + (RCC_HSEON_BIT_NUMBER * 4U)))
/* CSSON 位的别名地址 */
#define RCC_CSSON_BIT_NUMBER      RCC_CR_CSSON_Pos
#define RCC_CR_CSSON_BB           ((uint32_t)(PERIPH_BB_BASE + (RCC_CR_OFFSET_BB * 32U) + (RCC_CSSON_BIT_NUMBER * 4U)))
/* PLLON 位的别名地址 */
#define RCC_PLLON_BIT_NUMBER      RCC_CR_PLLON_Pos
#define RCC_CR_PLLON_BB           ((uint32_t)(PERIPH_BB_BASE + (RCC_CR_OFFSET_BB * 32U) + (RCC_PLLON_BIT_NUMBER * 4U)))

/* --- CSR 寄存器 ---*/
/* LSION 位的别名地址 */
#define RCC_LSION_BIT_NUMBER      RCC_CSR_LSION_Pos
#define RCC_CSR_LSION_BB          ((uint32_t)(PERIPH_BB_BASE + (RCC_CSR_OFFSET_BB * 32U) + (RCC_LSION_BIT_NUMBER * 4U)))

/* RMVF 位的别名地址 */
#define RCC_RMVF_BIT_NUMBER       RCC_CSR_RMVF_Pos
#define RCC_CSR_RMVF_BB           ((uint32_t)(PERIPH_BB_BASE + (RCC_CSR_OFFSET_BB * 32U) + (RCC_RMVF_BIT_NUMBER * 4U)))

/* --- BDCR 寄存器 ---*/
/* LSEON 位的别名地址 */
#define RCC_LSEON_BIT_NUMBER      RCC_BDCR_LSEON_Pos
#define RCC_BDCR_LSEON_BB          ((uint32_t)(PERIPH_BB_BASE + (RCC_BDCR_OFFSET_BB * 32U) + (RCC_LSEON_BIT_NUMBER * 4U)))

/* LSEBYP 位的别名地址 */
#define RCC_LSEBYP_BIT_NUMBER     RCC_BDCR_LSEBYP_Pos
#define RCC_BDCR_LSEBYP_BB         ((uint32_t)(PERIPH_BB_BASE + (RCC_BDCR_OFFSET_BB * 32U) + (RCC_LSEBYP_BIT_NUMBER * 4U)))

/* RTCEN 位的别名地址 */
#define RCC_RTCEN_BIT_NUMBER      RCC_BDCR_RTCEN_Pos
#define RCC_BDCR_RTCEN_BB          ((uint32_t)(PERIPH_BB_BASE + (RCC_BDCR_OFFSET_BB * 32U) + (RCC_RTCEN_BIT_NUMBER * 4U)))

/* BDRST 位的别名地址 */
#define RCC_BDRST_BIT_NUMBER      RCC_BDCR_BDRST_Pos
#define RCC_BDCR_BDRST_BB         ((uint32_t)(PERIPH_BB_BASE + (RCC_BDCR_OFFSET_BB * 32U) + (RCC_BDRST_BIT_NUMBER * 4U)))

/**
  * @}
  */

/* CR 寄存器 字节 2 (Bits[23:16]) 基地址 */
#define RCC_CR_BYTE2_ADDRESS          ((uint32_t)(RCC_BASE + RCC_CR_OFFSET + 0x02U))

/* CIR 寄存器 字节 1 (Bits[15:8]) 基地址 */
#define RCC_CIR_BYTE1_ADDRESS     ((uint32_t)(RCC_BASE + RCC_CIR_OFFSET + 0x01U))

/* CIR 寄存器 字节 2 (Bits[23:16]) 基地址 */
#define RCC_CIR_BYTE2_ADDRESS     ((uint32_t)(RCC_BASE + RCC_CIR_OFFSET + 0x02U))

/* 用于标志的定义 */
#define CR_REG_INDEX                     ((uint8_t)1)
#define BDCR_REG_INDEX                   ((uint8_t)2)
#define CSR_REG_INDEX                    ((uint8_t)3)

#define RCC_FLAG_MASK                    ((uint8_t)0x1F)

/**
  * @}
  */

/** @addtogroup RCC_Private_Macros
  * @{
  */
/** @defgroup RCC_Alias_For_Legacy 为遗留维护的别名定义
  * @{
  */
#define __HAL_RCC_SYSCFG_CLK_DISABLE    __HAL_RCC_AFIO_CLK_DISABLE
#define __HAL_RCC_SYSCFG_CLK_ENABLE     __HAL_RCC_AFIO_CLK_ENABLE
#define __HAL_RCC_SYSCFG_FORCE_RESET    __HAL_RCC_AFIO_FORCE_RESET
#define __HAL_RCC_SYSCFG_RELEASE_RESET  __HAL_RCC_AFIO_RELEASE_RESET
/**
  * @}
  */

#define IS_RCC_PLLSOURCE(__SOURCE__) (((__SOURCE__) == RCC_PLLSOURCE_HSI_DIV2) || \
                                      ((__SOURCE__) == RCC_PLLSOURCE_HSE))
#define IS_RCC_OSCILLATORTYPE(__OSCILLATOR__) (((__OSCILLATOR__) == RCC_OSCILLATORTYPE_NONE)                           || \
                                               (((__OSCILLATOR__) & RCC_OSCILLATORTYPE_HSE) == RCC_OSCILLATORTYPE_HSE) || \
                                               (((__OSCILLATOR__) & RCC_OSCILLATORTYPE_HSI) == RCC_OSCILLATORTYPE_HSI) || \
                                               (((__OSCILLATOR__) & RCC_OSCILLATORTYPE_LSI) == RCC_OSCILLATORTYPE_LSI) || \
                                               (((__OSCILLATOR__) & RCC_OSCILLATORTYPE_LSE) == RCC_OSCILLATORTYPE_LSE))
#define IS_RCC_HSE(__HSE__) (((__HSE__) == RCC_HSE_OFF) || ((__HSE__) == RCC_HSE_ON) || \
                             ((__HSE__) == RCC_HSE_BYPASS))
#define IS_RCC_LSE(__LSE__) (((__LSE__) == RCC_LSE_OFF) || ((__LSE__) == RCC_LSE_ON) || \
                             ((__LSE__) == RCC_LSE_BYPASS))
#define IS_RCC_HSI(__HSI__) (((__HSI__) == RCC_HSI_OFF) || ((__HSI__) == RCC_HSI_ON))
#define IS_RCC_CALIBRATION_VALUE(__VALUE__) ((__VALUE__) <= 0x1FU)
#define IS_RCC_LSI(__LSI__) (((__LSI__) == RCC_LSI_OFF) || ((__LSI__) == RCC_LSI_ON))
#define IS_RCC_PLL(__PLL__) (((__PLL__) == RCC_PLL_NONE) || ((__PLL__) == RCC_PLL_OFF) || \
                             ((__PLL__) == RCC_PLL_ON))

#define IS_RCC_CLOCKTYPE(CLK) ((((CLK) & RCC_CLOCKTYPE_SYSCLK) == RCC_CLOCKTYPE_SYSCLK) || \
                               (((CLK) & RCC_CLOCKTYPE_HCLK)   == RCC_CLOCKTYPE_HCLK)   || \
                               (((CLK) & RCC_CLOCKTYPE_PCLK1)  == RCC_CLOCKTYPE_PCLK1)  || \
                               (((CLK) & RCC_CLOCKTYPE_PCLK2)  == RCC_CLOCKTYPE_PCLK2))
#define IS_RCC_SYSCLKSOURCE(__SOURCE__) (((__SOURCE__) == RCC_SYSCLKSOURCE_HSI) || \
                                         ((__SOURCE__) == RCC_SYSCLKSOURCE_HSE) || \
                                         ((__SOURCE__) == RCC_SYSCLKSOURCE_PLLCLK))
#define IS_RCC_SYSCLKSOURCE_STATUS(__SOURCE__) (((__SOURCE__) == RCC_SYSCLKSOURCE_STATUS_HSI) || \
                                                ((__SOURCE__) == RCC_SYSCLKSOURCE_STATUS_HSE) || \
                                                ((__SOURCE__) == RCC_SYSCLKSOURCE_STATUS_PLLCLK))
#define IS_RCC_HCLK(__HCLK__) (((__HCLK__) == RCC_SYSCLK_DIV1) || ((__HCLK__) == RCC_SYSCLK_DIV2) || \
                               ((__HCLK__) == RCC_SYSCLK_DIV4) || ((__HCLK__) == RCC_SYSCLK_DIV8) || \
                               ((__HCLK__) == RCC_SYSCLK_DIV16) || ((__HCLK__) == RCC_SYSCLK_DIV64) || \
                               ((__HCLK__) == RCC_SYSCLK_DIV128) || ((__HCLK__) == RCC_SYSCLK_DIV256) || \
                               ((__HCLK__) == RCC_SYSCLK_DIV512))
#define IS_RCC_PCLK(__PCLK__) (((__PCLK__) == RCC_HCLK_DIV1) || ((__PCLK__) == RCC_HCLK_DIV2) || \
                               ((__PCLK__) == RCC_HCLK_DIV4) || ((__PCLK__) == RCC_HCLK_DIV8) || \
                               ((__PCLK__) == RCC_HCLK_DIV16))
#define IS_RCC_MCO(__MCO__)  ((__MCO__) == RCC_MCO)
#define IS_RCC_MCODIV(__DIV__) (((__DIV__) == RCC_MCODIV_1))
#define IS_RCC_RTCCLKSOURCE(__SOURCE__)  (((__SOURCE__) == RCC_RTCCLKSOURCE_NO_CLK) || \
                                          ((__SOURCE__) == RCC_RTCCLKSOURCE_LSE) || \
                                          ((__SOURCE__) == RCC_RTCCLKSOURCE_LSI) || \
                                          ((__SOURCE__) == RCC_RTCCLKSOURCE_HSE_DIV128))

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F1xx_HAL_RCC_H */


