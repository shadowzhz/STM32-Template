/**
  ******************************************************************************
  * @file    stm32f1xx_hal_cortex.h
  * @author  MCD Application Team
  * @brief   HAL库 CORTEX 模块头文件
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

/* 定义防止递归 -------------------------------------*/
#ifndef __STM32F1xx_HAL_CORTEX_H
#define __STM32F1xx_HAL_CORTEX_H

#ifdef __cplusplus
 extern "C" {
#endif

/* 头文件 ------------------------------------------------------------------*/
#include "stm32f1xx_hal_def.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @addtogroup CORTEX
  * @{
  */ 
/* 导出类型 ------------------------------------------------------------*/
/** @defgroup CORTEX_Exported_Types Cortex 导出类型
  * @{
  */

#if (__MPU_PRESENT == 1U)
/** @defgroup CORTEX_MPU_Region_Initialization_Structure_definition MPU 区域初始化结构体定义
  * @brief  MPU 区域初始化结构体
  * @{
  */
typedef struct
{
  uint8_t                Enable;                /*!< 该参数指定了区域的状态
                                                     这个参数可以是 @ref CORTEX_MPU_Region_Enable 值之一                 */
  uint8_t                Number;                /*!< 指定要保护的区域编号
                                                     此参数可以是 @ref CORTEX_MPU_Region_Number 中的值                 */
  uint32_t               BaseAddress;           /*!< 指定要保护的区域的基地址                           */
  uint8_t                Size;                  /*!< 指定要保护的区域的大小
                                                     此参数可以是 @ref CORTEX_MPU_Region_Size 中的值                   */
  uint8_t                SubRegionDisable;      /*!< 指定要禁用的子区域保护的编号 
                                                     此参数必须是介于 Min_Data = 0x00 和 Max_Data = 0xFF 之间的数字    */         
  uint8_t                TypeExtField;          /*!< 此参数用于指定TEX字段级别
                                                     可以选择的值取决于 @ref CORTEX_MPU_TEX_Level                    */                 
  uint8_t                AccessPermission;      /*!< 指定区域访问权限类型
                                                     此参数可以是 @ref CORTEX_MPU_Region_Permission_Attributes 中的值  */
  uint8_t                DisableExec;           /*!< 指定指令访问状态 
                                                     此参数可以是 @ref CORTEX_MPU_Instruction_Access中的值之一            */
  uint8_t                IsShareable;           /*!< 指定受保护区域的共享状态 
                                                     这个参数可以是 @ref CORTEX_MPU_Access_Shareable中的值之一              */
  uint8_t                IsCacheable;           /*!< 指定受保护区域的可缓存状态
                                                     此参数可以是 @ref CORTEX_MPU_Access_Cacheable 中的一个值              */
  uint8_t                IsBufferable;          /*!< 指定受保护区域的可缓存状态 
                                                     此参数可以是 @ref CORTEX_MPU_Access_Bufferable中的值之一             */
}MPU_Region_InitTypeDef;
/**
  * @}
  */
#endif /* __MPU_PRESENT */

/**
  * @}
  */

/* 导出常量 --------------------------------------------------------*/

/** @defgroup CORTEX_Exported_Constants CORTEX 导出常量
  * @{
  */

/** @defgroup CORTEX_Preemption_Priority_Group CORTEX 抢占优先级组
  * @{
  */
#define NVIC_PRIORITYGROUP_0         0x00000007U /*!< 0 位抢占优先级
                                                      4 子优先级 */
#define NVIC_PRIORITYGROUP_1         0x00000006U /*!< 1 位抢占优先级
                                                      3 子优先级 */
#define NVIC_PRIORITYGROUP_2         0x00000005U /*!< 2 位抢占优先级
                                                      2 子优先级 */
#define NVIC_PRIORITYGROUP_3         0x00000004U /*!< 3 位抢占优先级
                                                      1 子优先级 */
#define NVIC_PRIORITYGROUP_4         0x00000003U /*!< 4 位抢占优先级
                                                      0 子优先级 */
/**
  * @}
  */

/** @defgroup CORTEX_SysTick_clock_source CORTEX _SysTick 时钟源
  * @{
  */
#define SYSTICK_CLKSOURCE_HCLK_DIV8    0x00000000U  /* HCLK 8分频 */
#define SYSTICK_CLKSOURCE_HCLK         0x00000004U  /* HCLK */

/**
  * @}
  */

#if (__MPU_PRESENT == 1)
/** @defgroup CORTEX_MPU_HFNMI_PRIVDEF_Control MPU HFNMI 和 PRIVILEGED 访问控制
  * @{
  */
#define  MPU_HFNMI_PRIVDEF_NONE           0x00000000U
#define  MPU_HARDFAULT_NMI                MPU_CTRL_HFNMIENA_Msk
#define  MPU_PRIVILEGED_DEFAULT           MPU_CTRL_PRIVDEFENA_Msk
#define  MPU_HFNMI_PRIVDEF               (MPU_CTRL_HFNMIENA_Msk | MPU_CTRL_PRIVDEFENA_Msk)

/**
  * @}
  */

/** @defgroup CORTEX_MPU_Region_Enable CORTEX MPU 区域使能
  * @{
  */
#define  MPU_REGION_ENABLE     ((uint8_t)0x01)
#define  MPU_REGION_DISABLE    ((uint8_t)0x00)
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Instruction_Access CORTEX MPU 指令访问
  * @{
  */
#define  MPU_INSTRUCTION_ACCESS_ENABLE      ((uint8_t)0x00)
#define  MPU_INSTRUCTION_ACCESS_DISABLE     ((uint8_t)0x01)
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Access_Shareable CORTEX MPU 指令访问可共享
  * @{
  */
#define  MPU_ACCESS_SHAREABLE        ((uint8_t)0x01)
#define  MPU_ACCESS_NOT_SHAREABLE    ((uint8_t)0x00)
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Access_Cacheable CORTEX MPU 指令访问可缓存
  * @{
  */
#define  MPU_ACCESS_CACHEABLE         ((uint8_t)0x01)
#define  MPU_ACCESS_NOT_CACHEABLE     ((uint8_t)0x00)
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Access_Bufferable CORTEX MPU 指令访问可以缓存
  * @{
  */
#define  MPU_ACCESS_BUFFERABLE         ((uint8_t)0x01)
#define  MPU_ACCESS_NOT_BUFFERABLE     ((uint8_t)0x00)
/**
  * @}
  */

/** @defgroup CORTEX_MPU_TEX_Levels MPU TEX 等级
  * @{
  */
#define  MPU_TEX_LEVEL0    ((uint8_t)0x00)
#define  MPU_TEX_LEVEL1    ((uint8_t)0x01)
#define  MPU_TEX_LEVEL2    ((uint8_t)0x02)
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Region_Size CORTEX MPU 区域大小
  * @{
  */
#define   MPU_REGION_SIZE_32B      ((uint8_t)0x04)
#define   MPU_REGION_SIZE_64B      ((uint8_t)0x05)
#define   MPU_REGION_SIZE_128B     ((uint8_t)0x06)
#define   MPU_REGION_SIZE_256B     ((uint8_t)0x07)
#define   MPU_REGION_SIZE_512B     ((uint8_t)0x08)
#define   MPU_REGION_SIZE_1KB      ((uint8_t)0x09)
#define   MPU_REGION_SIZE_2KB      ((uint8_t)0x0A)
#define   MPU_REGION_SIZE_4KB      ((uint8_t)0x0B)
#define   MPU_REGION_SIZE_8KB      ((uint8_t)0x0C)
#define   MPU_REGION_SIZE_16KB     ((uint8_t)0x0D)
#define   MPU_REGION_SIZE_32KB     ((uint8_t)0x0E)
#define   MPU_REGION_SIZE_64KB     ((uint8_t)0x0F)
#define   MPU_REGION_SIZE_128KB    ((uint8_t)0x10)
#define   MPU_REGION_SIZE_256KB    ((uint8_t)0x11)
#define   MPU_REGION_SIZE_512KB    ((uint8_t)0x12)
#define   MPU_REGION_SIZE_1MB      ((uint8_t)0x13)
#define   MPU_REGION_SIZE_2MB      ((uint8_t)0x14)
#define   MPU_REGION_SIZE_4MB      ((uint8_t)0x15)
#define   MPU_REGION_SIZE_8MB      ((uint8_t)0x16)
#define   MPU_REGION_SIZE_16MB     ((uint8_t)0x17)
#define   MPU_REGION_SIZE_32MB     ((uint8_t)0x18)
#define   MPU_REGION_SIZE_64MB     ((uint8_t)0x19)
#define   MPU_REGION_SIZE_128MB    ((uint8_t)0x1A)
#define   MPU_REGION_SIZE_256MB    ((uint8_t)0x1B)
#define   MPU_REGION_SIZE_512MB    ((uint8_t)0x1C)
#define   MPU_REGION_SIZE_1GB      ((uint8_t)0x1D)
#define   MPU_REGION_SIZE_2GB      ((uint8_t)0x1E)
#define   MPU_REGION_SIZE_4GB      ((uint8_t)0x1F)
/**
  * @}
  */
   
/** @defgroup CORTEX_MPU_Region_Permission_Attributes CORTEX MPU区域权限属性
  * @{
  */
#define  MPU_REGION_NO_ACCESS      ((uint8_t)0x00)
#define  MPU_REGION_PRIV_RW        ((uint8_t)0x01)
#define  MPU_REGION_PRIV_RW_URO    ((uint8_t)0x02)
#define  MPU_REGION_FULL_ACCESS    ((uint8_t)0x03)
#define  MPU_REGION_PRIV_RO        ((uint8_t)0x05)
#define  MPU_REGION_PRIV_RO_URO    ((uint8_t)0x06)
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Region_Number CORTEX MPU 区域编号
  * @{
  */
#define  MPU_REGION_NUMBER0    ((uint8_t)0x00)
#define  MPU_REGION_NUMBER1    ((uint8_t)0x01)
#define  MPU_REGION_NUMBER2    ((uint8_t)0x02)
#define  MPU_REGION_NUMBER3    ((uint8_t)0x03)
#define  MPU_REGION_NUMBER4    ((uint8_t)0x04)
#define  MPU_REGION_NUMBER5    ((uint8_t)0x05)
#define  MPU_REGION_NUMBER6    ((uint8_t)0x06)
#define  MPU_REGION_NUMBER7    ((uint8_t)0x07)
/**
  * @}
  */
#endif /* __MPU_PRESENT */

/**
  * @}
  */


/* 导出宏 -----------------------------------------------------------*/

/* 导出函数 --------------------------------------------------------*/
/** @addtogroup CORTEX_Exported_Functions
  * @{
  */
  
/** @addtogroup CORTEX_Exported_Functions_Group1
  * @{
  */
/* 初始化和反初始化函数 *****************************/

void HAL_NVIC_SetPriorityGrouping(uint32_t PriorityGroup);
void HAL_NVIC_SetPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority);
void HAL_NVIC_EnableIRQ(IRQn_Type IRQn);
void HAL_NVIC_DisableIRQ(IRQn_Type IRQn);
void HAL_NVIC_SystemReset(void);
uint32_t HAL_SYSTICK_Config(uint32_t TicksNumb);
/**
  * @}
  */

/** @addtogroup CORTEX_Exported_Functions_Group2
  * @{
  */
/* 外设控制函数 ***********************************************/

uint32_t HAL_NVIC_GetPriorityGrouping(void);
void HAL_NVIC_GetPriority(IRQn_Type IRQn, uint32_t PriorityGroup, uint32_t* pPreemptPriority, uint32_t* pSubPriority);
uint32_t HAL_NVIC_GetPendingIRQ(IRQn_Type IRQn);
void HAL_NVIC_SetPendingIRQ(IRQn_Type IRQn);
void HAL_NVIC_ClearPendingIRQ(IRQn_Type IRQn);
uint32_t HAL_NVIC_GetActive(IRQn_Type IRQn);
void HAL_SYSTICK_CLKSourceConfig(uint32_t CLKSource);
void HAL_SYSTICK_IRQHandler(void);
void HAL_SYSTICK_Callback(void);

#if (__MPU_PRESENT == 1U)
void HAL_MPU_Enable(uint32_t MPU_Control);
void HAL_MPU_Disable(void);
void HAL_MPU_ConfigRegion(MPU_Region_InitTypeDef *MPU_Init);
#endif /* __MPU_PRESENT */
/**
  * @}
  */

/**
  * @}
  */

/* 私有类型 -------------------------------------------------------------*/
/* 私有变量 ---------------------------------------------------------*/
/* 私有常量 ---------------------------------------------------------*/
/* 私有宏 ------------------------------------------------------------*/
/** @defgroup CORTEX_Private_Macros CORTEX 私有宏
  * @{
  */
#define IS_NVIC_PRIORITY_GROUP(GROUP) (((GROUP) == NVIC_PRIORITYGROUP_0) || \
                                       ((GROUP) == NVIC_PRIORITYGROUP_1) || \
                                       ((GROUP) == NVIC_PRIORITYGROUP_2) || \
                                       ((GROUP) == NVIC_PRIORITYGROUP_3) || \
                                       ((GROUP) == NVIC_PRIORITYGROUP_4))

#define IS_NVIC_PREEMPTION_PRIORITY(PRIORITY)  ((PRIORITY) < 0x10U)

#define IS_NVIC_SUB_PRIORITY(PRIORITY)         ((PRIORITY) < 0x10U)

#define IS_NVIC_DEVICE_IRQ(IRQ)                ((IRQ) >= (IRQn_Type)0x00U)

#define IS_SYSTICK_CLK_SOURCE(SOURCE) (((SOURCE) == SYSTICK_CLKSOURCE_HCLK) || \
                                       ((SOURCE) == SYSTICK_CLKSOURCE_HCLK_DIV8))

#if (__MPU_PRESENT == 1U)
#define IS_MPU_REGION_ENABLE(STATE) (((STATE) == MPU_REGION_ENABLE) || \
                                     ((STATE) == MPU_REGION_DISABLE))

#define IS_MPU_INSTRUCTION_ACCESS(STATE) (((STATE) == MPU_INSTRUCTION_ACCESS_ENABLE) || \
                                          ((STATE) == MPU_INSTRUCTION_ACCESS_DISABLE))

#define IS_MPU_ACCESS_SHAREABLE(STATE)   (((STATE) == MPU_ACCESS_SHAREABLE) || \
                                          ((STATE) == MPU_ACCESS_NOT_SHAREABLE))

#define IS_MPU_ACCESS_CACHEABLE(STATE)   (((STATE) == MPU_ACCESS_CACHEABLE) || \
                                          ((STATE) == MPU_ACCESS_NOT_CACHEABLE))

#define IS_MPU_ACCESS_BUFFERABLE(STATE)   (((STATE) == MPU_ACCESS_BUFFERABLE) || \
                                          ((STATE) == MPU_ACCESS_NOT_BUFFERABLE))

#define IS_MPU_TEX_LEVEL(TYPE) (((TYPE) == MPU_TEX_LEVEL0)  || \
                                ((TYPE) == MPU_TEX_LEVEL1)  || \
                                ((TYPE) == MPU_TEX_LEVEL2))

#define IS_MPU_REGION_PERMISSION_ATTRIBUTE(TYPE) (((TYPE) == MPU_REGION_NO_ACCESS)   || \
                                                  ((TYPE) == MPU_REGION_PRIV_RW)     || \
                                                  ((TYPE) == MPU_REGION_PRIV_RW_URO) || \
                                                  ((TYPE) == MPU_REGION_FULL_ACCESS) || \
                                                  ((TYPE) == MPU_REGION_PRIV_RO)     || \
                                                  ((TYPE) == MPU_REGION_PRIV_RO_URO))

#define IS_MPU_REGION_NUMBER(NUMBER)    (((NUMBER) == MPU_REGION_NUMBER0) || \
                                         ((NUMBER) == MPU_REGION_NUMBER1) || \
                                         ((NUMBER) == MPU_REGION_NUMBER2) || \
                                         ((NUMBER) == MPU_REGION_NUMBER3) || \
                                         ((NUMBER) == MPU_REGION_NUMBER4) || \
                                         ((NUMBER) == MPU_REGION_NUMBER5) || \
                                         ((NUMBER) == MPU_REGION_NUMBER6) || \
                                         ((NUMBER) == MPU_REGION_NUMBER7))

#define IS_MPU_REGION_SIZE(SIZE)    (((SIZE) == MPU_REGION_SIZE_32B)   || \
                                     ((SIZE) == MPU_REGION_SIZE_64B)   || \
                                     ((SIZE) == MPU_REGION_SIZE_128B)  || \
                                     ((SIZE) == MPU_REGION_SIZE_256B)  || \
                                     ((SIZE) == MPU_REGION_SIZE_512B)  || \
                                     ((SIZE) == MPU_REGION_SIZE_1KB)   || \
                                     ((SIZE) == MPU_REGION_SIZE_2KB)   || \
                                     ((SIZE) == MPU_REGION_SIZE_4KB)   || \
                                     ((SIZE) == MPU_REGION_SIZE_8KB)   || \
                                     ((SIZE) == MPU_REGION_SIZE_16KB)  || \
                                     ((SIZE) == MPU_REGION_SIZE_32KB)  || \
                                     ((SIZE) == MPU_REGION_SIZE_64KB)  || \
                                     ((SIZE) == MPU_REGION_SIZE_128KB) || \
                                     ((SIZE) == MPU_REGION_SIZE_256KB) || \
                                     ((SIZE) == MPU_REGION_SIZE_512KB) || \
                                     ((SIZE) == MPU_REGION_SIZE_1MB)   || \
                                     ((SIZE) == MPU_REGION_SIZE_2MB)   || \
                                     ((SIZE) == MPU_REGION_SIZE_4MB)   || \
                                     ((SIZE) == MPU_REGION_SIZE_8MB)   || \
                                     ((SIZE) == MPU_REGION_SIZE_16MB)  || \
                                     ((SIZE) == MPU_REGION_SIZE_32MB)  || \
                                     ((SIZE) == MPU_REGION_SIZE_64MB)  || \
                                     ((SIZE) == MPU_REGION_SIZE_128MB) || \
                                     ((SIZE) == MPU_REGION_SIZE_256MB) || \
                                     ((SIZE) == MPU_REGION_SIZE_512MB) || \
                                     ((SIZE) == MPU_REGION_SIZE_1GB)   || \
                                     ((SIZE) == MPU_REGION_SIZE_2GB)   || \
                                     ((SIZE) == MPU_REGION_SIZE_4GB))

#define IS_MPU_SUB_REGION_DISABLE(SUBREGION)  ((SUBREGION) < (uint16_t)0x00FF)
#endif /* __MPU_PRESENT */

/**                                                                          
  * @}                                                                  
  */

/* 私有函数 ---------------------------------------------------------*/

/**
  * @}
  */ 

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __STM32F1xx_HAL_CORTEX_H */


