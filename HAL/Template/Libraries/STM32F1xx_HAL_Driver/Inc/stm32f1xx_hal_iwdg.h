/**
  ******************************************************************************
  * @file    stm32f1xx_hal_iwdg.h
  * @author  MCD Application Team
  * @brief   Header file of IWDG HAL module.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32F1xx_HAL_IWDG_H
#define STM32F1xx_HAL_IWDG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal_def.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @defgroup IWDG IWDG
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup IWDG_Exported_Types IWDG Exported Types
  * @{
  */

/**
  * @brief  IWDG Init structure definition
  */
typedef struct
{
  uint32_t Prescaler;  /*!< Select the prescaler of the IWDG.
                            This parameter can be a value of @ref IWDG_Prescaler */

  uint32_t Reload;     /*!< Specifies the IWDG down-counter reload value.
                            This parameter must be a number between Min_Data = 0 and Max_Data = 0x0FFF */

} IWDG_InitTypeDef;

/**
  * @brief  IWDG Handle Structure definition
  */
typedef struct
{
  IWDG_TypeDef                 *Instance;  /*!< Register base address    */

  IWDG_InitTypeDef             Init;       /*!< IWDG required parameters */
} IWDG_HandleTypeDef;


/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup IWDG_Exported_Constants IWDG Exported Constants
  * @{
  */

/** @defgroup IWDG_Prescaler IWDG Prescaler
  * @{
  */
#define IWDG_PRESCALER_4                0x00000000u                                     /*!< IWDG prescaler set to 4   */
#define IWDG_PRESCALER_8                IWDG_PR_PR_0                                    /*!< IWDG prescaler set to 8   */
#define IWDG_PRESCALER_16               IWDG_PR_PR_1                                    /*!< IWDG prescaler set to 16  */
#define IWDG_PRESCALER_32               (IWDG_PR_PR_1 | IWDG_PR_PR_0)                   /*!< IWDG prescaler set to 32  */
#define IWDG_PRESCALER_64               IWDG_PR_PR_2                                    /*!< IWDG prescaler set to 64  */
#define IWDG_PRESCALER_128              (IWDG_PR_PR_2 | IWDG_PR_PR_0)                   /*!< IWDG prescaler set to 128 */
#define IWDG_PRESCALER_256              (IWDG_PR_PR_2 | IWDG_PR_PR_1)                   /*!< IWDG prescaler set to 256 */
/**
  * @}
  */

/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/
/** @defgroup IWDG_Exported_Macros IWDG Exported Macros
  * @{
  */

/**
  * @brief  Enable the IWDG peripheral.
  * @param  __HANDLE__  IWDG handle
  * @retval None
  */
#define __HAL_IWDG_START(__HANDLE__)                WRITE_REG((__HANDLE__)->Instance->KR, IWDG_KEY_ENABLE)

/**
  * @brief  Reload IWDG counter with value defined in the reload register
  *         (write access to IWDG_PR and IWDG_RLR registers disabled).
  * @param  __HANDLE__  IWDG handle
  * @retval None
  */
#define __HAL_IWDG_RELOAD_COUNTER(__HANDLE__)       WRITE_REG((__HANDLE__)->Instance->KR, IWDG_KEY_RELOAD)

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup IWDG_Exported_Functions  IWDG Exported Functions
  * @{
  */

/** @defgroup IWDG_Exported_Functions_Group1 Initialization and Start functions
  * @{
  */
/* Initialization/Start functions  ********************************************/

/**
  * @brief  初始化独立看门狗（IWDG）并立即启动硬件计数
  * @param  hiwdg: IWDG 句柄指针
  * @note   【超时时间估算公式】
  *         IWDG 由芯片内部专用的低速 RC 振荡器（LSI，标称 40kHz，实际约 30~60kHz）独立供电驱动：
  *         溢出超时时间：Tout = (4 * 2^PR * (Reload + 1)) / 40000 秒
  *         例如：Prescaler = IWDG_PRESCALER_64, Reload = 624 对应超时时间约为 1.0 秒。
  * @warning 【不可逆启动警告】
  *          在 STM32 硬件设计中，独立看门狗一旦被初始化启动，【在本次上电运行期间任何软件指令都无法将其关闭】！
  *          系统必须定期调用 HAL_IWDG_Refresh() 喂狗，否则看门狗计数归零将触发全片硬件复位。
  * @retval HAL 状态
  */
HAL_StatusTypeDef     HAL_IWDG_Init(IWDG_HandleTypeDef *hiwdg);
/**
  * @}
  */

/** @defgroup IWDG_Exported_Functions_Group2 IO operation functions
  * @{
  */
/* I/O operation functions ****************************************************/

/**
  * @brief  刷新/重载独立看门狗计数器（即常说的“喂狗”）
  * @param  hiwdg: IWDG 句柄指针
  * @note   向 IWDG_KR 寄存器写入键值 0xAAAA，使计数器重新装载初始设定值，防止芯片复位。
  * @warning 喂狗操作应置于主循环最外层或核心健康监控任务中，严禁在定时器中断中机械喂狗（否则主循环卡死也无法检测）。
  * @retval HAL 状态
  */
HAL_StatusTypeDef     HAL_IWDG_Refresh(IWDG_HandleTypeDef *hiwdg);
/**
  * @}
  */

/**
  * @}
  */

/* Private constants ---------------------------------------------------------*/
/** @defgroup IWDG_Private_Constants IWDG Private Constants
  * @{
  */

/**
  * @brief  IWDG Key Register BitMask
  */
#define IWDG_KEY_RELOAD                 0x0000AAAAu  /*!< IWDG Reload Counter Enable   */
#define IWDG_KEY_ENABLE                 0x0000CCCCu  /*!< IWDG Peripheral Enable       */
#define IWDG_KEY_WRITE_ACCESS_ENABLE    0x00005555u  /*!< IWDG KR Write Access Enable  */
#define IWDG_KEY_WRITE_ACCESS_DISABLE   0x00000000u  /*!< IWDG KR Write Access Disable */

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @defgroup IWDG_Private_Macros IWDG Private Macros
  * @{
  */

/**
  * @brief  Enable write access to IWDG_PR and IWDG_RLR registers.
  * @param  __HANDLE__  IWDG handle
  * @retval None
  */
#define IWDG_ENABLE_WRITE_ACCESS(__HANDLE__)  WRITE_REG((__HANDLE__)->Instance->KR, IWDG_KEY_WRITE_ACCESS_ENABLE)

/**
  * @brief  Disable write access to IWDG_PR and IWDG_RLR registers.
  * @param  __HANDLE__  IWDG handle
  * @retval None
  */
#define IWDG_DISABLE_WRITE_ACCESS(__HANDLE__) WRITE_REG((__HANDLE__)->Instance->KR, IWDG_KEY_WRITE_ACCESS_DISABLE)

/**
  * @brief  Check IWDG prescaler value.
  * @param  __PRESCALER__  IWDG prescaler value
  * @retval None
  */
#define IS_IWDG_PRESCALER(__PRESCALER__)      (((__PRESCALER__) == IWDG_PRESCALER_4)  || \
                                               ((__PRESCALER__) == IWDG_PRESCALER_8)  || \
                                               ((__PRESCALER__) == IWDG_PRESCALER_16) || \
                                               ((__PRESCALER__) == IWDG_PRESCALER_32) || \
                                               ((__PRESCALER__) == IWDG_PRESCALER_64) || \
                                               ((__PRESCALER__) == IWDG_PRESCALER_128)|| \
                                               ((__PRESCALER__) == IWDG_PRESCALER_256))

/**
  * @brief  Check IWDG reload value.
  * @param  __RELOAD__  IWDG reload value
  * @retval None
  */
#define IS_IWDG_RELOAD(__RELOAD__)            ((__RELOAD__) <= IWDG_RLR_RL)



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

#endif /* STM32F1xx_HAL_IWDG_H */
