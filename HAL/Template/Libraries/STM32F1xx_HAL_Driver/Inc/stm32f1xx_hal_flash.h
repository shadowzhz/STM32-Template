/**
  ******************************************************************************
  * @file    stm32f1xx_hal_flash.h
  * @author  MCD Application Team
  * @brief   Header file of Flash HAL module.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file in
  * the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F1xx_HAL_FLASH_H
#define __STM32F1xx_HAL_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal_def.h"
   
/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @addtogroup FLASH
  * @{
  */
  
/** @addtogroup FLASH_Private_Constants
  * @{
  */
#define FLASH_TIMEOUT_VALUE              50000U /* 50 s */
/**
  * @}
  */

/** @addtogroup FLASH_Private_Macros
  * @{
  */

#define IS_FLASH_TYPEPROGRAM(VALUE)  (((VALUE) == FLASH_TYPEPROGRAM_HALFWORD) || \
                                      ((VALUE) == FLASH_TYPEPROGRAM_WORD)     || \
                                      ((VALUE) == FLASH_TYPEPROGRAM_DOUBLEWORD))  

#if   defined(FLASH_ACR_LATENCY)
#define IS_FLASH_LATENCY(__LATENCY__) (((__LATENCY__) == FLASH_LATENCY_0) || \
                                       ((__LATENCY__) == FLASH_LATENCY_1) || \
                                       ((__LATENCY__) == FLASH_LATENCY_2))

#else
#define IS_FLASH_LATENCY(__LATENCY__)   ((__LATENCY__) == FLASH_LATENCY_0)
#endif /* FLASH_ACR_LATENCY */
/**
  * @}
  */  

/* Exported types ------------------------------------------------------------*/ 
/** @defgroup FLASH_Exported_Types FLASH Exported Types
  * @{
  */  

/**
  * @brief  FLASH Procedure structure definition
  */
typedef enum 
{
  FLASH_PROC_NONE              = 0U, 
  FLASH_PROC_PAGEERASE         = 1U,
  FLASH_PROC_MASSERASE         = 2U,
  FLASH_PROC_PROGRAMHALFWORD   = 3U,
  FLASH_PROC_PROGRAMWORD       = 4U,
  FLASH_PROC_PROGRAMDOUBLEWORD = 5U
} FLASH_ProcedureTypeDef;

/** 
  * @brief  FLASH handle Structure definition  
  */
typedef struct
{
  __IO FLASH_ProcedureTypeDef ProcedureOnGoing; /*!< Internal variable to indicate which procedure is ongoing or not in IT context */
  
  __IO uint32_t               DataRemaining;    /*!< Internal variable to save the remaining pages to erase or half-word to program in IT context */

  __IO uint32_t               Address;          /*!< Internal variable to save address selected for program or erase */

  __IO uint64_t               Data;             /*!< Internal variable to save data to be programmed */

  HAL_LockTypeDef             Lock;             /*!< FLASH locking object                */

  __IO uint32_t               ErrorCode;        /*!< FLASH error code                    
                                                     This parameter can be a value of @ref FLASH_Error_Codes  */
} FLASH_ProcessTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup FLASH_Exported_Constants FLASH Exported Constants
  * @{
  */  

/** @defgroup FLASH_Error_Codes FLASH Error Codes
  * @{
  */

#define HAL_FLASH_ERROR_NONE      0x00U  /*!< No error */
#define HAL_FLASH_ERROR_PROG      0x01U  /*!< Programming error */
#define HAL_FLASH_ERROR_WRP       0x02U  /*!< Write protection error */
#define HAL_FLASH_ERROR_OPTV      0x04U  /*!< Option validity error */

/**
  * @}
  */

/** @defgroup FLASH_Type_Program FLASH Type Program
  * @{
  */ 
#define FLASH_TYPEPROGRAM_HALFWORD             0x01U  /*!<Program a half-word (16-bit) at a specified address.*/
#define FLASH_TYPEPROGRAM_WORD                 0x02U  /*!<Program a word (32-bit) at a specified address.*/
#define FLASH_TYPEPROGRAM_DOUBLEWORD           0x03U  /*!<Program a double word (64-bit) at a specified address*/

/**
  * @}
  */

#if   defined(FLASH_ACR_LATENCY)
/** @defgroup FLASH_Latency FLASH Latency
  * @{
  */
#define FLASH_LATENCY_0            0x00000000U               /*!< FLASH Zero Latency cycle */
#define FLASH_LATENCY_1            FLASH_ACR_LATENCY_0       /*!< FLASH One Latency cycle */
#define FLASH_LATENCY_2            FLASH_ACR_LATENCY_1       /*!< FLASH Two Latency cycles */

/**
  * @}
  */

#else
/** @defgroup FLASH_Latency FLASH Latency
  * @{
  */
#define FLASH_LATENCY_0            0x00000000U    /*!< FLASH Zero Latency cycle */

/**
  * @}
  */

#endif /* FLASH_ACR_LATENCY */
/**
  * @}
  */  
  
/* Exported macro ------------------------------------------------------------*/

/** @defgroup FLASH_Exported_Macros FLASH Exported Macros
 *  @brief macros to control FLASH features 
 *  @{
 */
 
/** @defgroup FLASH_Half_Cycle FLASH Half Cycle
 *  @brief macros to handle FLASH half cycle
 * @{
 */

/**
  * @brief  Enable the FLASH half cycle access.
  * @note   half cycle access can only be used with a low-frequency clock of less than
            8 MHz that can be obtained with the use of HSI or HSE but not of PLL.
  * @retval None
  */
#define __HAL_FLASH_HALF_CYCLE_ACCESS_ENABLE()  (FLASH->ACR |= FLASH_ACR_HLFCYA)

/**
  * @brief  Disable the FLASH half cycle access.
  * @note   half cycle access can only be used with a low-frequency clock of less than
            8 MHz that can be obtained with the use of HSI or HSE but not of PLL.
  * @retval None
  */
#define __HAL_FLASH_HALF_CYCLE_ACCESS_DISABLE() (FLASH->ACR &= (~FLASH_ACR_HLFCYA))

/**
  * @}
  */

#if defined(FLASH_ACR_LATENCY)
/** @defgroup FLASH_EM_Latency FLASH Latency
 *  @brief macros to handle FLASH Latency
 * @{
 */ 
  
/**
  * @brief  Set the FLASH Latency.
  * @param  __LATENCY__ FLASH Latency                   
  *         The value of this parameter depend on device used within the same series
  * @retval None
  */ 
#define __HAL_FLASH_SET_LATENCY(__LATENCY__)    (FLASH->ACR = (FLASH->ACR&(~FLASH_ACR_LATENCY)) | (__LATENCY__))


/**
  * @brief  Get the FLASH Latency.
  * @retval FLASH Latency                   
  *         The value of this parameter depend on device used within the same series
  */ 
#define __HAL_FLASH_GET_LATENCY()     (READ_BIT((FLASH->ACR), FLASH_ACR_LATENCY))

/**
  * @}
  */

#endif /* FLASH_ACR_LATENCY */
/** @defgroup FLASH_Prefetch FLASH Prefetch
 *  @brief macros to handle FLASH Prefetch buffer
 * @{
 */   
/**
  * @brief  Enable the FLASH prefetch buffer.
  * @retval None
  */ 
#define __HAL_FLASH_PREFETCH_BUFFER_ENABLE()    (FLASH->ACR |= FLASH_ACR_PRFTBE)

/**
  * @brief  Disable the FLASH prefetch buffer.
  * @retval None
  */
#define __HAL_FLASH_PREFETCH_BUFFER_DISABLE()   (FLASH->ACR &= (~FLASH_ACR_PRFTBE))

/**
  * @}
  */
  
/**
  * @}
  */ 

/* Include FLASH HAL Extended module */
#include "stm32f1xx_hal_flash_ex.h"  

/* Exported functions --------------------------------------------------------*/
/** @addtogroup FLASH_Exported_Functions
  * @{
  */
  
/** @addtogroup FLASH_Exported_Functions_Group1
  * @{
  */
/* IO operation functions *****************************************************/

/**
  * @brief  向片上指定 Flash 地址写入半字（16位）或双字数据（掉电存储参数）
  * @param  TypeProgram: 写入类型（FLASH_TYPEPROGRAM_HALFWORD 或 FLASH_TYPEPROGRAM_DOUBLEWORD）
  * @param  Address: 目标 Flash 物理地址（STM32F1 起始地址为 0x08000000）
  * @param  Data: 待写入的数据内容
  * @note   【掉电参数存储实战规范】
  *         1. 存储地址请选用 Flash 靠后的末尾页（如 F103C8T6 最后一页 0x0800FC00），严禁覆盖代码运行段。
  *         2. 写入前【必须先将目标扇区/页擦除为全 0xFFFF】！直接覆盖已写入非 0xFFFF 的区域会报 PGERR 编程错误。
  *         3. 标准三步走：
  *            @code
  *            HAL_FLASH_Unlock();
  *            // 必须先通过 HAL_FLASHEx_Erase 擦除对应页...
  *            HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, addr, data);
  *            HAL_FLASH_Lock(); // 务必重新上锁
  *            @endcode
  * @warning 写入期间 CPU 总线会被硬件暂时挂起（约几十微秒），在此期间无法执行 Flash 中的代码。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint64_t Data);
HAL_StatusTypeDef HAL_FLASH_Program_IT(uint32_t TypeProgram, uint32_t Address, uint64_t Data);

/* FLASH IRQ handler function */
void       HAL_FLASH_IRQHandler(void);
/* Callbacks in non blocking modes */ 
void       HAL_FLASH_EndOfOperationCallback(uint32_t ReturnValue);
void       HAL_FLASH_OperationErrorCallback(uint32_t ReturnValue);

/**
  * @}
  */

/** @addtogroup FLASH_Exported_Functions_Group2
  * @{
  */
/* Peripheral Control functions ***********************************************/

/**
  * @brief  解锁 Flash 控制寄存器访问权限
  * @note   复位后 Flash 控制寄存器默认处于写保护状态以防止程序跑飞误改写代码。
  *         写入擦除或编程 Flash 前必须先调用本函数。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_FLASH_Unlock(void);

/**
  * @brief  重新对 Flash 控制寄存器上锁（使能写保护）
  * @note   完成 Flash 擦写后【强烈建议必须立即调用本函数】，防止野指针或异常断电误损坏固件代码。
  * @retval HAL 状态
  */
HAL_StatusTypeDef HAL_FLASH_Lock(void);
HAL_StatusTypeDef HAL_FLASH_OB_Unlock(void);
HAL_StatusTypeDef HAL_FLASH_OB_Lock(void);
void HAL_FLASH_OB_Launch(void);

/**
  * @}
  */

/** @addtogroup FLASH_Exported_Functions_Group3
  * @{
  */
/* Peripheral State and Error functions ***************************************/
uint32_t HAL_FLASH_GetError(void);

/**
  * @}
  */

/**
  * @}
  */

/* Private function -------------------------------------------------*/
/** @addtogroup FLASH_Private_Functions
 * @{
 */
HAL_StatusTypeDef       FLASH_WaitForLastOperation(uint32_t Timeout);
#if defined(FLASH_BANK2_END)
HAL_StatusTypeDef       FLASH_WaitForLastOperationBank2(uint32_t Timeout);
#endif /* FLASH_BANK2_END */

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

#endif /* __STM32F1xx_HAL_FLASH_H */


