/**
  ******************************************************************************
  * @file    stm32f10x_flash.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件提供所有 FLASH 固件函数。
  ******************************************************************************
  * @attention
  *
  * 本固件仅供参考，旨在为客户提供有关其产品的编码信息，以便客户节省时间。
  * 因此，对于因本固件的内容和/或客户将本文所含编码信息用于其产品
  * 而产生的任何索赔所导致的任何直接、间接或后果性损害，
  * 意法半导体概不承担责任。
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f10x_flash.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @defgroup FLASH
  * @brief FLASH 驱动模块
  * @{
  */ 

/** @defgroup FLASH_Private_TypesDefinitions   FLASH 私有类型定义
  * @{
  */

/**
  * @}
  */ 

/** @defgroup FLASH_Private_Defines   FLASH 私有宏定义
  * @{
  */ 

/* 闪存访问控制寄存器位 */
#define ACR_LATENCY_Mask         ((uint32_t)0x00000038)
#define ACR_HLFCYA_Mask          ((uint32_t)0xFFFFFFF7)
#define ACR_PRFTBE_Mask          ((uint32_t)0xFFFFFFEF)

/* 闪存访问控制寄存器位 */
#define ACR_PRFTBS_Mask          ((uint32_t)0x00000020) 

/* 闪存控制寄存器位 */
#define CR_PG_Set                ((uint32_t)0x00000001)
#define CR_PG_Reset              ((uint32_t)0x00001FFE) 
#define CR_PER_Set               ((uint32_t)0x00000002)
#define CR_PER_Reset             ((uint32_t)0x00001FFD)
#define CR_MER_Set               ((uint32_t)0x00000004)
#define CR_MER_Reset             ((uint32_t)0x00001FFB)
#define CR_OPTPG_Set             ((uint32_t)0x00000010)
#define CR_OPTPG_Reset           ((uint32_t)0x00001FEF)
#define CR_OPTER_Set             ((uint32_t)0x00000020)
#define CR_OPTER_Reset           ((uint32_t)0x00001FDF)
#define CR_STRT_Set              ((uint32_t)0x00000040)
#define CR_LOCK_Set              ((uint32_t)0x00000080)

/* FLASH 掩码 */
#define RDPRT_Mask               ((uint32_t)0x00000002)
#define WRP0_Mask                ((uint32_t)0x000000FF)
#define WRP1_Mask                ((uint32_t)0x0000FF00)
#define WRP2_Mask                ((uint32_t)0x00FF0000)
#define WRP3_Mask                ((uint32_t)0xFF000000)
#define OB_USER_BFB2             ((uint16_t)0x0008)

/* FLASH 密钥 */
#define RDP_Key                  ((uint16_t)0x00A5)
#define FLASH_KEY1               ((uint32_t)0x45670123)
#define FLASH_KEY2               ((uint32_t)0xCDEF89AB)

/* FLASH BANK 地址 */
#define FLASH_BANK1_END_ADDRESS   ((uint32_t)0x807FFFF)

/* 延时定义 */   
#define EraseTimeout          ((uint32_t)0x000B0000)
#define ProgramTimeout        ((uint32_t)0x00002000)
/**
  * @}
  */ 

/** @defgroup FLASH_Private_Macros   FLASH 私有宏
  * @{
  */

/**
  * @}
  */ 

/** @defgroup FLASH_Private_Variables   FLASH 私有变量
  * @{
  */

/**
  * @}
  */ 

/** @defgroup FLASH_Private_FunctionPrototypes   FLASH 私有函数原型
  * @{
  */
  
/**
  * @}
  */

/** @defgroup FLASH_Private_Functions   FLASH 私有函数
  * @{
  */

/**
@code

 本驱动提供用于配置和编程所有 STM32F10x 器件（包括最新的 STM32F10x_XL
 密度器件）Flash 存储器的函数。

 STM32F10x_XL 器件具有高达 1 Mbyte 的容量，并采用双 Bank 架构以具备
 边读边写 (RWW) 能力：
    - bank1: 固定大小为 512 Kbytes（256 页，每页 2 Kbytes）
    - bank2: 高达 512 Kbytes（最多 256 页，每页 2 Kbytes）
 而其他 STM32F10x 器件只有一个 Bank，存储器容量最高为 512 Kbytes。

 在 V3.3.0 版本中，更新了一些函数并新增了一些函数以支持
 STM32F10x_XL 器件。因此，有些函数管理所有器件，而有些函数
 则专用于 XL 器件。

 下表列出了根据所使用的 STM32F10x 器件可用的函数清单。
 （以下为等宽 ASCII 对照表，为保持列对齐原样保留英文内容）

   ***************************************************
   * Legacy functions used for all STM32F10x devices *
   ***************************************************
   +----------------------------------------------------------------------------------------------------------------------------------+
   |       Functions prototypes         |STM32F10x_XL|Other STM32F10x|    Comments                                                    |
   |                                    |   devices  |  devices      |                                                                |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_SetLatency                    |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_HalfCycleAccessCmd            |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_PrefetchBufferCmd             |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_Unlock                        |    Yes     |      Yes      | - For STM32F10X_XL devices: unlock Bank1 and Bank2.            |
   |                                    |            |               | - For other devices: unlock Bank1 and it is equivalent         |
   |                                    |            |               |   to FLASH_UnlockBank1 function.                               |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_Lock                          |    Yes     |      Yes      | - For STM32F10X_XL devices: lock Bank1 and Bank2.              |
   |                                    |            |               | - For other devices: lock Bank1 and it is equivalent           |
   |                                    |            |               |   to FLASH_LockBank1 function.                                 |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_ErasePage                     |    Yes     |      Yes      | - For STM32F10x_XL devices: erase a page in Bank1 and Bank2    |
   |                                    |            |               | - For other devices: erase a page in Bank1                     |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_EraseAllPages                 |    Yes     |      Yes      | - For STM32F10x_XL devices: erase all pages in Bank1 and Bank2 |
   |                                    |            |               | - For other devices: erase all pages in Bank1                  |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_EraseOptionBytes              |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_ProgramWord                   |    Yes     |      Yes      | Updated to program up to 1MByte (depending on the used device) |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_ProgramHalfWord               |    Yes     |      Yes      | Updated to program up to 1MByte (depending on the used device) |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_ProgramOptionByteData         |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_EnableWriteProtection         |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_ReadOutProtection             |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_UserOptionByteConfig          |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_GetUserOptionByte             |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_GetWriteProtectionOptionByte  |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_GetReadOutProtectionStatus    |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_GetPrefetchBufferStatus       |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_ITConfig                      |    Yes     |      Yes      | - For STM32F10x_XL devices: enable Bank1 and Bank2's interrupts|
   |                                    |            |               | - For other devices: enable Bank1's interrupts                 |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_GetFlagStatus                 |    Yes     |      Yes      | - For STM32F10x_XL devices: return Bank1 and Bank2's flag status|
   |                                    |            |               | - For other devices: return Bank1's flag status                |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_ClearFlag                     |    Yes     |      Yes      | - For STM32F10x_XL devices: clear Bank1 and Bank2's flag       |
   |                                    |            |               | - For other devices: clear Bank1's flag                        |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_GetStatus                     |    Yes     |      Yes      | - Return the status of Bank1 (for all devices)                 |
   |                                    |            |               |   equivalent to FLASH_GetBank1Status function                  |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_WaitForLastOperation          |    Yes     |      Yes      | - Wait for Bank1 last operation (for all devices)              |
   |                                    |            |               |   equivalent to: FLASH_WaitForLastBank1Operation function      |
   +----------------------------------------------------------------------------------------------------------------------------------+

   ************************************************************************************************************************
   * New functions used for all STM32F10x devices to manage Bank1:                                                        *
   *   - These functions are mainly useful for STM32F10x_XL density devices, to have separate control for Bank1 and bank2 *
   *   - For other devices, these functions are optional (covered by functions listed above)                              *
   ************************************************************************************************************************
   +----------------------------------------------------------------------------------------------------------------------------------+
   |       Functions prototypes         |STM32F10x_XL|Other STM32F10x|    Comments                                                    |
   |                                    |   devices  |  devices      |                                                                |
   |----------------------------------------------------------------------------------------------------------------------------------|
   | FLASH_UnlockBank1                  |    Yes     |      Yes      | - Unlock Bank1                                                 |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_LockBank1                     |    Yes     |      Yes      | - Lock Bank1                                                   |
   |----------------------------------------------------------------------------------------------------------------------------------|
   | FLASH_EraseAllBank1Pages           |    Yes     |      Yes      | - Erase all pages in Bank1                                     |
   |----------------------------------------------------------------------------------------------------------------------------------|
   | FLASH_GetBank1Status               |    Yes     |      Yes      | - Return the status of Bank1                                   |
   |----------------------------------------------------------------------------------------------------------------------------------|
   | FLASH_WaitForLastBank1Operation    |    Yes     |      Yes      | - Wait for Bank1 last operation                                |
   +----------------------------------------------------------------------------------------------------------------------------------+

   *****************************************************************************
   * New Functions used only with STM32F10x_XL density devices to manage Bank2 *
   *****************************************************************************
   +----------------------------------------------------------------------------------------------------------------------------------+
   |       Functions prototypes         |STM32F10x_XL|Other STM32F10x|    Comments                                                    |
   |                                    |   devices  |  devices      |                                                                |
   |----------------------------------------------------------------------------------------------------------------------------------|
   | FLASH_UnlockBank2                  |    Yes     |      No       | - Unlock Bank2                                                 |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_LockBank2                     |    Yes     |      No       | - Lock Bank2                                                   |
   |----------------------------------------------------------------------------------------------------------------------------------|
   | FLASH_EraseAllBank2Pages           |    Yes     |      No       | - Erase all pages in Bank2                                     |
   |----------------------------------------------------------------------------------------------------------------------------------|
   | FLASH_GetBank2Status               |    Yes     |      No       | - Return the status of Bank2                                   |
   |----------------------------------------------------------------------------------------------------------------------------------|
   | FLASH_WaitForLastBank2Operation    |    Yes     |      No       | - Wait for Bank2 last operation                                |
   |----------------------------------------------------------------------------------------------------------------------------------|
   | FLASH_BootConfig                   |    Yes     |      No       | - Configure to boot from Bank1 or Bank2                        |
   +----------------------------------------------------------------------------------------------------------------------------------+
@endcode
*/


/**
  * @brief  设置代码延时值 (Latency)。
  * @note   本函数可用于所有 STM32F10x 器件。
  * @param  FLASH_Latency: 指定 FLASH 延时值。
  *   该参数可取以下值之一：
  *     @arg FLASH_Latency_0: FLASH 零延时周期
  *     @arg FLASH_Latency_1: FLASH 一个延时周期
  *     @arg FLASH_Latency_2: FLASH 两个延时周期
  * @retval 无
  */
void FLASH_SetLatency(uint32_t FLASH_Latency)
{
  uint32_t tmpreg = 0;
  
  /* 检查参数 */
  assert_param(IS_FLASH_LATENCY(FLASH_Latency));
  
  /* 读取 ACR 寄存器 */
  tmpreg = FLASH->ACR;  
  
  /* 设置 Latency 值 */
  tmpreg &= ACR_LATENCY_Mask;
  tmpreg |= FLASH_Latency;
  
  /* 写入 ACR 寄存器 */
  FLASH->ACR = tmpreg;
}

/**
  * @brief  使能或关闭半周期闪存访问。
  * @note   本函数可用于所有 STM32F10x 器件。
  * @param  FLASH_HalfCycleAccess: 指定 FLASH 半周期访问模式。
  *   该参数可取以下值之一：
  *     @arg FLASH_HalfCycleAccess_Enable: FLASH 半周期使能
  *     @arg FLASH_HalfCycleAccess_Disable: FLASH 半周期关闭
  * @retval 无
  */
void FLASH_HalfCycleAccessCmd(uint32_t FLASH_HalfCycleAccess)
{
  /* 检查参数 */
  assert_param(IS_FLASH_HALFCYCLEACCESS_STATE(FLASH_HalfCycleAccess));
  
  /* 使能或关闭半周期访问 */
  FLASH->ACR &= ACR_HLFCYA_Mask;
  FLASH->ACR |= FLASH_HalfCycleAccess;
}

/**
  * @brief  使能或关闭预取缓冲区。
  * @note   本函数可用于所有 STM32F10x 器件。
  * @param  FLASH_PrefetchBuffer: 指定预取缓冲区的状态。
  *   该参数可取以下值之一：
  *     @arg FLASH_PrefetchBuffer_Enable: FLASH 预取缓冲区使能
  *     @arg FLASH_PrefetchBuffer_Disable: FLASH 预取缓冲区关闭
  * @retval 无
  */
void FLASH_PrefetchBufferCmd(uint32_t FLASH_PrefetchBuffer)
{
  /* 检查参数 */
  assert_param(IS_FLASH_PREFETCHBUFFER_STATE(FLASH_PrefetchBuffer));
  
  /* 使能或关闭预取缓冲区 */
  FLASH->ACR &= ACR_PRFTBE_Mask;
  FLASH->ACR |= FLASH_PrefetchBuffer;
}

/**
  * @brief  解锁 FLASH 编程擦除控制器。
  * @note   本函数可用于所有 STM32F10x 器件。
  *         - 对于 STM32F10X_XL 器件，本函数解锁 Bank1 和 Bank2。
  *         - 对于所有其他器件，本函数解锁 Bank1，等效于
  *           FLASH_UnlockBank1 函数。。
  * @param  无
  * @retval 无
  */
void FLASH_Unlock(void)
{
  /* 授权访问 Bank1 的 FPEC */
  FLASH->KEYR = FLASH_KEY1;
  FLASH->KEYR = FLASH_KEY2;

#ifdef STM32F10X_XL
  /* 授权访问 Bank2 的 FPEC */
  FLASH->KEYR2 = FLASH_KEY1;
  FLASH->KEYR2 = FLASH_KEY2;
#endif /* STM32F10X_XL */
}
/**
  * @brief  解锁 FLASH Bank1 编程擦除控制器。
  * @note   本函数可用于所有 STM32F10x 器件。
  *         - 对于 STM32F10X_XL 器件，本函数解锁 Bank1。
  *         - 对于所有其他器件，本函数解锁 Bank1，等效于
  *           FLASH_Unlock 函数。
  * @param  无
  * @retval 无
  */
void FLASH_UnlockBank1(void)
{
  /* 授权访问 Bank1 的 FPEC */
  FLASH->KEYR = FLASH_KEY1;
  FLASH->KEYR = FLASH_KEY2;
}

#ifdef STM32F10X_XL
/**
  * @brief  解锁 FLASH Bank2 编程擦除控制器。
  * @note   本函数仅可用于 STM32F10X_XL 密度器件。
  * @param  无
  * @retval 无
  */
void FLASH_UnlockBank2(void)
{
  /* 授权访问 Bank2 的 FPEC */
  FLASH->KEYR2 = FLASH_KEY1;
  FLASH->KEYR2 = FLASH_KEY2;

}
#endif /* STM32F10X_XL */

/**
  * @brief  锁定 FLASH 编程擦除控制器。
  * @note   本函数可用于所有 STM32F10x 器件。
  *         - 对于 STM32F10X_XL 器件，本函数锁定 Bank1 和 Bank2。
  *         - 对于所有其他器件，本函数锁定 Bank1，等效于
  *           FLASH_LockBank1 函数。
  * @param  无
  * @retval 无
  */
void FLASH_Lock(void)
{
  /* 置位锁定位以锁定 Bank1 的 FPEC 和 CR */
  FLASH->CR |= CR_LOCK_Set;

#ifdef STM32F10X_XL
  /* 置位锁定位以锁定 Bank2 的 FPEC 和 CR */
  FLASH->CR2 |= CR_LOCK_Set;
#endif /* STM32F10X_XL */
}

/**
  * @brief  锁定 FLASH Bank1 编程擦除控制器。
  * @note   本函数可用于所有 STM32F10x 器件。
  *         - 对于 STM32F10X_XL 器件，本函数锁定 Bank1。
  *         - 对于所有其他器件，本函数锁定 Bank1，等效于
  *           FLASH_Lock 函数。
  * @param  无
  * @retval 无
  */
void FLASH_LockBank1(void)
{
  /* 置位锁定位以锁定 Bank1 的 FPEC 和 CR */
  FLASH->CR |= CR_LOCK_Set;
}

#ifdef STM32F10X_XL
/**
  * @brief  锁定 FLASH Bank2 编程擦除控制器。
  * @note   本函数仅可用于 STM32F10X_XL 密度器件。
  * @param  无
  * @retval 无
  */
void FLASH_LockBank2(void)
{
  /* 置位锁定位以锁定 Bank2 的 FPEC 和 CR */
  FLASH->CR2 |= CR_LOCK_Set;
}
#endif /* STM32F10X_XL */

/**
  * @brief  擦除指定的 FLASH 页。
  * @note   本函数可用于所有 STM32F10x 器件。
  * @param  Page_Address: 要擦除的页地址。
  * @retval FLASH 状态：返回值可以是 FLASH_BUSY、FLASH_ERROR_PG、
  *         FLASH_ERROR_WRP、FLASH_COMPLETE 或 FLASH_TIMEOUT。
  */
FLASH_Status FLASH_ErasePage(uint32_t Page_Address)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* 检查参数 */
  assert_param(IS_FLASH_ADDRESS(Page_Address));

#ifdef STM32F10X_XL
  if(Page_Address < FLASH_BANK1_END_ADDRESS)  
  {
    /* 等待上一次操作完成 */
    status = FLASH_WaitForLastBank1Operation(EraseTimeout);
    if(status == FLASH_COMPLETE)
    { 
      /* 若上一次操作已完成，则继续擦除该页 */
      FLASH->CR|= CR_PER_Set;
      FLASH->AR = Page_Address; 
      FLASH->CR|= CR_STRT_Set;
    
      /* 等待上一次操作完成 */
      status = FLASH_WaitForLastBank1Operation(EraseTimeout);

      /* 关闭 PER 位 */
      FLASH->CR &= CR_PER_Reset;
    }
  }
  else
  {
    /* 等待上一次操作完成 */
    status = FLASH_WaitForLastBank2Operation(EraseTimeout);
    if(status == FLASH_COMPLETE)
    { 
      /* 若上一次操作已完成，则继续擦除该页 */
      FLASH->CR2|= CR_PER_Set;
      FLASH->AR2 = Page_Address; 
      FLASH->CR2|= CR_STRT_Set;
    
      /* 等待上一次操作完成 */
      status = FLASH_WaitForLastBank2Operation(EraseTimeout);
      
      /* 关闭 PER 位 */
      FLASH->CR2 &= CR_PER_Reset;
    }
  }
#else
  /* 等待上一次操作完成 */
  status = FLASH_WaitForLastOperation(EraseTimeout);
  
  if(status == FLASH_COMPLETE)
  { 
    /* 若上一次操作已完成，则继续擦除该页 */
    FLASH->CR|= CR_PER_Set;
    FLASH->AR = Page_Address; 
    FLASH->CR|= CR_STRT_Set;
    
    /* 等待上一次操作完成 */
    status = FLASH_WaitForLastOperation(EraseTimeout);
    
    /* 关闭 PER 位 */
    FLASH->CR &= CR_PER_Reset;
  }
#endif /* STM32F10X_XL */

  /* 返回擦除状态 */
  return status;
}

/**
  * @brief  擦除所有 FLASH 页。
  * @note   本函数可用于所有 STM32F10x 器件。
  * @param  无
  * @retval FLASH 状态：返回值可以是 FLASH_ERROR_PG、
  *         FLASH_ERROR_WRP、FLASH_COMPLETE 或 FLASH_TIMEOUT。
  */
FLASH_Status FLASH_EraseAllPages(void)
{
  FLASH_Status status = FLASH_COMPLETE;

#ifdef STM32F10X_XL
  /* 等待上一次操作完成 */
  status = FLASH_WaitForLastBank1Operation(EraseTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* 若上一次操作已完成，则继续擦除所有页 */
     FLASH->CR |= CR_MER_Set;
     FLASH->CR |= CR_STRT_Set;
    
    /* 等待上一次操作完成 */
    status = FLASH_WaitForLastBank1Operation(EraseTimeout);
    
    /* 关闭 MER 位 */
    FLASH->CR &= CR_MER_Reset;
  }    
  if(status == FLASH_COMPLETE)
  {
    /* 若上一次操作已完成，则继续擦除所有页 */
     FLASH->CR2 |= CR_MER_Set;
     FLASH->CR2 |= CR_STRT_Set;
    
    /* 等待上一次操作完成 */
    status = FLASH_WaitForLastBank2Operation(EraseTimeout);
    
    /* 关闭 MER 位 */
    FLASH->CR2 &= CR_MER_Reset;
  }
#else
  /* 等待上一次操作完成 */
  status = FLASH_WaitForLastOperation(EraseTimeout);
  if(status == FLASH_COMPLETE)
  {
    /* 若上一次操作已完成，则继续擦除所有页 */
     FLASH->CR |= CR_MER_Set;
     FLASH->CR |= CR_STRT_Set;
    
    /* 等待上一次操作完成 */
    status = FLASH_WaitForLastOperation(EraseTimeout);

    /* 关闭 MER 位 */
    FLASH->CR &= CR_MER_Reset;
  }
#endif /* STM32F10X_XL */

  /* 返回擦除状态 */
  return status;
}

/**
  * @brief  擦除所有 Bank1 FLASH 页。
  * @note   本函数可用于所有 STM32F10x 器件。
  *         - 对于 STM32F10X_XL 器件，本函数擦除所有 Bank1 页。
  *         - 对于所有其他器件，本函数擦除所有 Bank1 页，等效于
  *           FLASH_EraseAllPages 函数。
  * @param  无
  * @retval FLASH 状态：返回值可以是 FLASH_ERROR_PG、
  *         FLASH_ERROR_WRP、FLASH_COMPLETE 或 FLASH_TIMEOUT。
  */
FLASH_Status FLASH_EraseAllBank1Pages(void)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* 等待上一次操作完成 */
  status = FLASH_WaitForLastBank1Operation(EraseTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* 若上一次操作已完成，则继续擦除所有页 */
     FLASH->CR |= CR_MER_Set;
     FLASH->CR |= CR_STRT_Set;
    
    /* 等待上一次操作完成 */
    status = FLASH_WaitForLastBank1Operation(EraseTimeout);
    
    /* 关闭 MER 位 */
    FLASH->CR &= CR_MER_Reset;
  }    
  /* 返回擦除状态 */
  return status;
}

#ifdef STM32F10X_XL
/**
  * @brief  擦除所有 Bank2 FLASH 页。
  * @note   本函数仅可用于 STM32F10x_XL 密度器件。
  * @param  无
  * @retval FLASH 状态：返回值可以是 FLASH_ERROR_PG、
  *         FLASH_ERROR_WRP、FLASH_COMPLETE 或 FLASH_TIMEOUT。
  */
FLASH_Status FLASH_EraseAllBank2Pages(void)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* 等待上一次操作完成 */
  status = FLASH_WaitForLastBank2Operation(EraseTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* 若上一次操作已完成，则继续擦除所有页 */
     FLASH->CR2 |= CR_MER_Set;
     FLASH->CR2 |= CR_STRT_Set;
    
    /* 等待上一次操作完成 */
    status = FLASH_WaitForLastBank2Operation(EraseTimeout);

    /* 关闭 MER 位 */
    FLASH->CR2 &= CR_MER_Reset;
  }    
  /* 返回擦除状态 */
  return status;
}
#endif /* STM32F10X_XL */

/**
  * @brief  擦除 FLASH 选项字节。
  * @note   本函数擦除除读保护 (RDP) 之外的所有选项字节。
  * @note   本函数可用于所有 STM32F10x 器件。
  * @param  无
  * @retval FLASH 状态：返回值可以是 FLASH_ERROR_PG、
  *         FLASH_ERROR_WRP、FLASH_COMPLETE 或 FLASH_TIMEOUT。
  */
FLASH_Status FLASH_EraseOptionBytes(void)
{
  uint16_t rdptmp = RDP_Key;

  FLASH_Status status = FLASH_COMPLETE;

  /* 获取实际的读保护选项字节值 */ 
  if(FLASH_GetReadOutProtectionStatus() != RESET)
  {
    rdptmp = 0x00;  
  }

  /* 等待上一次操作完成 */
  status = FLASH_WaitForLastOperation(EraseTimeout);
  if(status == FLASH_COMPLETE)
  {
    /* 授权小信息块编程 */
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;
    
    /* 若上一次操作已完成，则继续擦除选项字节 */
    FLASH->CR |= CR_OPTER_Set;
    FLASH->CR |= CR_STRT_Set;
    /* 等待上一次操作完成 */
    status = FLASH_WaitForLastOperation(EraseTimeout);
    
    if(status == FLASH_COMPLETE)
    {
      /* 若擦除操作已完成，则关闭 OPTER 位 */
      FLASH->CR &= CR_OPTER_Reset;
       
      /* 使能选项字节编程操作 */
      FLASH->CR |= CR_OPTPG_Set;
      /* 恢复上一次的读保护选项字节值 */
      OB->RDP = (uint16_t)rdptmp; 
      /* 等待上一次操作完成 */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
 
      if(status != FLASH_TIMEOUT)
      {
        /* 若编程操作已完成，则关闭 OPTPG 位 */
        FLASH->CR &= CR_OPTPG_Reset;
      }
    }
    else
    {
      if (status != FLASH_TIMEOUT)
      {
        /* 关闭 OPTPG 位 */
        FLASH->CR &= CR_OPTPG_Reset;
      }
    }  
  }
  /* 返回擦除状态 */
  return status;
}

/**
  * @brief  在指定地址编程一个字。
  * @note   本函数可用于所有 STM32F10x 器件。
  * @param  Address: 指定要编程的地址。
  * @param  Data: 指定要编程的数据。
  * @retval FLASH 状态：返回值可以是 FLASH_ERROR_PG、
  *         FLASH_ERROR_WRP、FLASH_COMPLETE 或 FLASH_TIMEOUT。
  */
FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  __IO uint32_t tmp = 0;

  /* 检查参数 */
  assert_param(IS_FLASH_ADDRESS(Address));

#ifdef STM32F10X_XL
  if(Address < FLASH_BANK1_END_ADDRESS - 2)
  { 
    /* 等待上一次操作完成 */
    status = FLASH_WaitForLastBank1Operation(ProgramTimeout); 
    if(status == FLASH_COMPLETE)
    {
      /* 若上一次操作已完成，则继续编程新的第一个
       半字 */
      FLASH->CR |= CR_PG_Set;
  
      *(__IO uint16_t*)Address = (uint16_t)Data;
      /* 等待上一次操作完成 */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
 
      if(status == FLASH_COMPLETE)
      {
        /* 若上一次操作已完成，则继续编程新的第二个
       半字 */
        tmp = Address + 2;

        *(__IO uint16_t*) tmp = Data >> 16;
    
        /* 等待上一次操作完成 */
        status = FLASH_WaitForLastOperation(ProgramTimeout);
        
        /* 关闭 PG 位 */
        FLASH->CR &= CR_PG_Reset;
      }
      else
      {
        /* 关闭 PG 位 */
        FLASH->CR &= CR_PG_Reset;
       }
    }
  }
  else if(Address == (FLASH_BANK1_END_ADDRESS - 1))
  {
    /* 等待上一次操作完成 */
    status = FLASH_WaitForLastBank1Operation(ProgramTimeout);

    if(status == FLASH_COMPLETE)
    {
      /* 若上一次操作已完成，则继续编程新的第一个
       半字 */
      FLASH->CR |= CR_PG_Set;
  
      *(__IO uint16_t*)Address = (uint16_t)Data;

      /* 等待上一次操作完成 */
      status = FLASH_WaitForLastBank1Operation(ProgramTimeout);
      
	  /* 关闭 PG 位 */
      FLASH->CR &= CR_PG_Reset;
    }
    else
    {
      /* 关闭 PG 位 */
      FLASH->CR &= CR_PG_Reset;
    }

    /* 等待上一次操作完成 */
    status = FLASH_WaitForLastBank2Operation(ProgramTimeout);

    if(status == FLASH_COMPLETE)
    {
      /* 若上一次操作已完成，则继续编程新的第二个
      半字 */
      FLASH->CR2 |= CR_PG_Set;
      tmp = Address + 2;

      *(__IO uint16_t*) tmp = Data >> 16;
    
      /* 等待上一次操作完成 */
      status = FLASH_WaitForLastBank2Operation(ProgramTimeout);
        
      /* 关闭 PG 位 */
      FLASH->CR2 &= CR_PG_Reset;
    }
    else
    {
      /* 关闭 PG 位 */
      FLASH->CR2 &= CR_PG_Reset;
    }
  }
  else
  {
    /* 等待上一次操作完成 */
    status = FLASH_WaitForLastBank2Operation(ProgramTimeout);

    if(status == FLASH_COMPLETE)
    {
      /* 若上一次操作已完成，则继续编程新的第一个
       半字 */
      FLASH->CR2 |= CR_PG_Set;
  
      *(__IO uint16_t*)Address = (uint16_t)Data;
      /* 等待上一次操作完成 */
      status = FLASH_WaitForLastBank2Operation(ProgramTimeout);
 
      if(status == FLASH_COMPLETE)
      {
        /* 若上一次操作已完成，则继续编程新的第二个
       半字 */
        tmp = Address + 2;

        *(__IO uint16_t*) tmp = Data >> 16;
    
        /* 等待上一次操作完成 */
        status = FLASH_WaitForLastBank2Operation(ProgramTimeout);
        
        /* 关闭 PG 位 */
        FLASH->CR2 &= CR_PG_Reset;
      }
      else
      {
        /* 关闭 PG 位 */
        FLASH->CR2 &= CR_PG_Reset;
      }
    }
  }
#else
  /* 等待上一次操作完成 */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* 若上一次操作已完成，则继续编程新的第一个
    半字 */
    FLASH->CR |= CR_PG_Set;
  
    *(__IO uint16_t*)Address = (uint16_t)Data;
    /* 等待上一次操作完成 */
    status = FLASH_WaitForLastOperation(ProgramTimeout);
 
    if(status == FLASH_COMPLETE)
    {
      /* 若上一次操作已完成，则继续编程新的第二个
      半字 */
      tmp = Address + 2;

      *(__IO uint16_t*) tmp = Data >> 16;
    
      /* 等待上一次操作完成 */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
        
      /* 关闭 PG 位 */
      FLASH->CR &= CR_PG_Reset;
    }
    else
    {
      /* 关闭 PG 位 */
      FLASH->CR &= CR_PG_Reset;
    }
  }         
#endif /* STM32F10X_XL */
   
  /* 返回编程状态 */
  return status;
}

/**
  * @brief  在指定地址编程一个半字。
  * @note   本函数可用于所有 STM32F10x 器件。
  * @param  Address: 指定要编程的地址。
  * @param  Data: 指定要编程的数据。
  * @retval FLASH 状态：返回值可以是 FLASH_ERROR_PG、
  *         FLASH_ERROR_WRP、FLASH_COMPLETE 或 FLASH_TIMEOUT。
  */
FLASH_Status FLASH_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* 检查参数 */
  assert_param(IS_FLASH_ADDRESS(Address));

#ifdef STM32F10X_XL
  /* 等待上一次操作完成 */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(Address < FLASH_BANK1_END_ADDRESS)
  {
    if(status == FLASH_COMPLETE)
    {
      /* 若上一次操作已完成，则继续编程新数据 */
      FLASH->CR |= CR_PG_Set;
  
      *(__IO uint16_t*)Address = Data;
      /* 等待上一次操作完成 */
      status = FLASH_WaitForLastBank1Operation(ProgramTimeout);

      /* 关闭 PG 位 */
      FLASH->CR &= CR_PG_Reset;
    }
  }
  else
  {
    if(status == FLASH_COMPLETE)
    {
      /* 若上一次操作已完成，则继续编程新数据 */
      FLASH->CR2 |= CR_PG_Set;
  
      *(__IO uint16_t*)Address = Data;
      /* 等待上一次操作完成 */
      status = FLASH_WaitForLastBank2Operation(ProgramTimeout);

      /* 关闭 PG 位 */
      FLASH->CR2 &= CR_PG_Reset;
    }
  }
#else
  /* 等待上一次操作完成 */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* 若上一次操作已完成，则继续编程新数据 */
    FLASH->CR |= CR_PG_Set;
  
    *(__IO uint16_t*)Address = Data;
    /* 等待上一次操作完成 */
    status = FLASH_WaitForLastOperation(ProgramTimeout);
    
    /* 关闭 PG 位 */
    FLASH->CR &= CR_PG_Reset;
  } 
#endif  /* STM32F10X_XL */
  
  /* 返回编程状态 */
  return status;
}

/**
  * @brief  在指定的选项字节数据地址编程一个半字。
  * @note   本函数可用于所有 STM32F10x 器件。
  * @param  Address: 指定要编程的地址。
  *   该参数可以是 0x1FFFF804 或 0x1FFFF806。
  * @param  Data: 指定要编程的数据。
  * @retval FLASH 状态：返回值可以是 FLASH_ERROR_PG、
  *         FLASH_ERROR_WRP、FLASH_COMPLETE 或 FLASH_TIMEOUT。
  */
FLASH_Status FLASH_ProgramOptionByteData(uint32_t Address, uint8_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* 检查参数 */
  assert_param(IS_OB_DATA_ADDRESS(Address));
  status = FLASH_WaitForLastOperation(ProgramTimeout);

  if(status == FLASH_COMPLETE)
  {
    /* 授权小信息块编程 */
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;
    /* 使能选项字节编程操作 */
    FLASH->CR |= CR_OPTPG_Set; 
    *(__IO uint16_t*)Address = Data;
    
    /* 等待上一次操作完成 */
    status = FLASH_WaitForLastOperation(ProgramTimeout);
    if(status != FLASH_TIMEOUT)
    {
      /* 若编程操作已完成，则关闭 OPTPG 位 */
      FLASH->CR &= CR_OPTPG_Reset;
    }
  }
  /* 返回选项字节数据编程状态 */
  return status;
}

/**
  * @brief  对所需页进行写保护
  * @note   本函数可用于所有 STM32F10x 器件。
  * @param  FLASH_Pages: 指定要写保护的页的地址。
  *   该参数可以是：
  *     @arg 对于 @b STM32_Low-density_devices：取值介于 FLASH_WRProt_Pages0to3 和 FLASH_WRProt_Pages28to31 之间
  *     @arg 对于 @b STM32_Medium-density_devices：取值介于 FLASH_WRProt_Pages0to3
  *       和 FLASH_WRProt_Pages124to127 之间
  *     @arg 对于 @b STM32_High-density_devices：取值介于 FLASH_WRProt_Pages0to1 和
  *       FLASH_WRProt_Pages60to61 或 FLASH_WRProt_Pages62to255 之间
  *     @arg 对于 @b STM32_Connectivity_line_devices：取值介于 FLASH_WRProt_Pages0to1 和
  *       FLASH_WRProt_Pages60to61 或 FLASH_WRProt_Pages62to127 之间
  *     @arg 对于 @b STM32_XL-density_devices：取值介于 FLASH_WRProt_Pages0to1 和
  *       FLASH_WRProt_Pages60to61 或 FLASH_WRProt_Pages62to511 之间
  *     @arg FLASH_WRProt_AllPages
  * @retval FLASH 状态：返回值可以是 FLASH_ERROR_PG、
  *         FLASH_ERROR_WRP、FLASH_COMPLETE 或 FLASH_TIMEOUT。
  */
FLASH_Status FLASH_EnableWriteProtection(uint32_t FLASH_Pages)
{
  uint16_t WRP0_Data = 0xFFFF, WRP1_Data = 0xFFFF, WRP2_Data = 0xFFFF, WRP3_Data = 0xFFFF;
  
  FLASH_Status status = FLASH_COMPLETE;
  
  /* 检查参数 */
  assert_param(IS_FLASH_WRPROT_PAGE(FLASH_Pages));
  
  FLASH_Pages = (uint32_t)(~FLASH_Pages);
  WRP0_Data = (uint16_t)(FLASH_Pages & WRP0_Mask);
  WRP1_Data = (uint16_t)((FLASH_Pages & WRP1_Mask) >> 8);
  WRP2_Data = (uint16_t)((FLASH_Pages & WRP2_Mask) >> 16);
  WRP3_Data = (uint16_t)((FLASH_Pages & WRP3_Mask) >> 24);
  
  /* 等待上一次操作完成 */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* 授权小信息块编程 */
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;
    FLASH->CR |= CR_OPTPG_Set;
    if(WRP0_Data != 0xFF)
    {
      OB->WRP0 = WRP0_Data;
      
      /* 等待上一次操作完成 */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
    }
    if((status == FLASH_COMPLETE) && (WRP1_Data != 0xFF))
    {
      OB->WRP1 = WRP1_Data;
      
      /* 等待上一次操作完成 */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
    }
    if((status == FLASH_COMPLETE) && (WRP2_Data != 0xFF))
    {
      OB->WRP2 = WRP2_Data;
      
      /* 等待上一次操作完成 */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
    }
    
    if((status == FLASH_COMPLETE)&& (WRP3_Data != 0xFF))
    {
      OB->WRP3 = WRP3_Data;
     
      /* 等待上一次操作完成 */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
    }
          
    if(status != FLASH_TIMEOUT)
    {
      /* 若编程操作已完成，则关闭 OPTPG 位 */
      FLASH->CR &= CR_OPTPG_Reset;
    }
  } 
  /* 返回写保护操作状态 */
  return status;       
}

/**
  * @brief  使能或关闭读保护。
  * @note   若用户在调用本函数之前已编程了其他选项字节，
  *   则必须重新编程这些字节，因为本函数会擦除所有选项字节。
  * @note   本函数可用于所有 STM32F10x 器件。
  * @param  Newstate: 读保护的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval FLASH 状态：返回值可以是 FLASH_ERROR_PG、
  *         FLASH_ERROR_WRP、FLASH_COMPLETE 或 FLASH_TIMEOUT。
  */
FLASH_Status FLASH_ReadOutProtection(FunctionalState NewState)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* 检查参数 */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  status = FLASH_WaitForLastOperation(EraseTimeout);
  if(status == FLASH_COMPLETE)
  {
    /* 授权小信息块编程 */
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;
    FLASH->CR |= CR_OPTER_Set;
    FLASH->CR |= CR_STRT_Set;
    /* 等待上一次操作完成 */
    status = FLASH_WaitForLastOperation(EraseTimeout);
    if(status == FLASH_COMPLETE)
    {
      /* 若擦除操作已完成，则关闭 OPTER 位 */
      FLASH->CR &= CR_OPTER_Reset;
      /* 使能选项字节编程操作 */
      FLASH->CR |= CR_OPTPG_Set; 
      if(NewState != DISABLE)
      {
        OB->RDP = 0x00;
      }
      else
      {
        OB->RDP = RDP_Key;  
      }
      /* 等待上一次操作完成 */
      status = FLASH_WaitForLastOperation(EraseTimeout); 
    
      if(status != FLASH_TIMEOUT)
      {
        /* 若编程操作已完成，则关闭 OPTPG 位 */
        FLASH->CR &= CR_OPTPG_Reset;
      }
    }
    else 
    {
      if(status != FLASH_TIMEOUT)
      {
        /* 关闭 OPTER 位 */
        FLASH->CR &= CR_OPTER_Reset;
      }
    }
  }
  /* 返回保护操作状态 */
  return status;       
}

/**
  * @brief  编程 FLASH 用户选项字节：IWDG_SW / RST_STOP / RST_STDBY。
  * @note   本函数可用于所有 STM32F10x 器件。
  * @param  OB_IWDG: 选择 IWDG 模式
  *   该参数可取以下值之一：
  *     @arg OB_IWDG_SW: 选择软件 IWDG
  *     @arg OB_IWDG_HW: 选择硬件 IWDG
  * @param  OB_STOP: 进入 STOP 模式时的复位事件。
  *   该参数可取以下值之一：
  *     @arg OB_STOP_NoRST: 进入 STOP 时不产生复位
  *     @arg OB_STOP_RST: 进入 STOP 时产生复位
  * @param  OB_STDBY: 进入待机模式时的复位事件。
  *   该参数可取以下值之一：
  *     @arg OB_STDBY_NoRST: 进入 STANDBY 时不产生复位
  *     @arg OB_STDBY_RST: 进入 STANDBY 时产生复位
  * @retval FLASH 状态：返回值可以是 FLASH_ERROR_PG、
  *         FLASH_ERROR_WRP、FLASH_COMPLETE 或 FLASH_TIMEOUT。
  */
FLASH_Status FLASH_UserOptionByteConfig(uint16_t OB_IWDG, uint16_t OB_STOP, uint16_t OB_STDBY)
{
  FLASH_Status status = FLASH_COMPLETE; 

  /* 检查参数 */
  assert_param(IS_OB_IWDG_SOURCE(OB_IWDG));
  assert_param(IS_OB_STOP_SOURCE(OB_STOP));
  assert_param(IS_OB_STDBY_SOURCE(OB_STDBY));

  /* 授权小信息块编程 */
  FLASH->OPTKEYR = FLASH_KEY1;
  FLASH->OPTKEYR = FLASH_KEY2;
  
  /* 等待上一次操作完成 */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {  
    /* 使能选项字节编程操作 */
    FLASH->CR |= CR_OPTPG_Set; 
           
    OB->USER = OB_IWDG | (uint16_t)(OB_STOP | (uint16_t)(OB_STDBY | ((uint16_t)0xF8))); 
  
    /* 等待上一次操作完成 */
    status = FLASH_WaitForLastOperation(ProgramTimeout);
    if(status != FLASH_TIMEOUT)
    {
      /* 若编程操作已完成，则关闭 OPTPG 位 */
      FLASH->CR &= CR_OPTPG_Reset;
    }
  }    
  /* 返回选项字节编程状态 */
  return status;
}

#ifdef STM32F10X_XL
/**
  * @brief  配置从 Bank1 或 Bank2 启动。
  * @note   本函数仅可用于 STM32F10x_XL 密度器件。
  * @param  FLASH_BOOT: 选择要启动的 FLASH Bank。
  *   该参数可取以下值之一：
  *     @arg FLASH_BOOT_Bank1: 启动时，若启动引脚设置为从用户 Flash 启动
  *        位置且选择该参数，则器件将从 Bank1 启动（默认）。
  *     @arg FLASH_BOOT_Bank2: 启动时，若启动引脚设置为从用户 Flash 启动
  *        位置且选择该参数，则器件将从 Bank2 或 Bank1 启动，
  *        具体取决于 Bank 的激活情况。活动 Bank 的检查顺序为：
  *        先 Bank2，然后 Bank1。
  *        活动 Bank 由各 Bank 基地址处编程的值来识别
  *        （对应于中断向量表中的初始堆栈指针值）。
  *        更多信息请参阅 www.st.com 上的 AN2606。
  * @retval FLASH 状态：返回值可以是 FLASH_ERROR_PG、
  *         FLASH_ERROR_WRP、FLASH_COMPLETE 或 FLASH_TIMEOUT。
  */
FLASH_Status FLASH_BootConfig(uint16_t FLASH_BOOT)
{ 
  FLASH_Status status = FLASH_COMPLETE; 
  assert_param(IS_FLASH_BOOT(FLASH_BOOT));
  /* 授权小信息块编程 */
  FLASH->OPTKEYR = FLASH_KEY1;
  FLASH->OPTKEYR = FLASH_KEY2;
  
  /* 等待上一次操作完成 */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {  
    /* 使能选项字节编程操作 */
    FLASH->CR |= CR_OPTPG_Set; 

    if(FLASH_BOOT == FLASH_BOOT_Bank1)
    {
      OB->USER |= OB_USER_BFB2;
    }
    else
    {
      OB->USER &= (uint16_t)(~(uint16_t)(OB_USER_BFB2));
    }
    /* 等待上一次操作完成 */
    status = FLASH_WaitForLastOperation(ProgramTimeout);
    if(status != FLASH_TIMEOUT)
    {
      /* 若编程操作已完成，则关闭 OPTPG 位 */
      FLASH->CR &= CR_OPTPG_Reset;
    }
  }    
  /* 返回选项字节编程状态 */
  return status;
}
#endif /* STM32F10X_XL */

/**
  * @brief  返回 FLASH 用户选项字节值。
  * @note   本函数可用于所有 STM32F10x 器件。
  * @param  无
  * @retval FLASH 用户选项字节值：IWDG_SW(Bit0)、RST_STOP(Bit1)
  *         和 RST_STDBY(Bit2)。
  */
uint32_t FLASH_GetUserOptionByte(void)
{
  /* 返回用户选项字节 */
  return (uint32_t)(FLASH->OBR >> 2);
}

/**
  * @brief  返回 FLASH 写保护选项字节寄存器值。
  * @note   本函数可用于所有 STM32F10x 器件。
  * @param  无
  * @retval FLASH 写保护选项字节寄存器值
  */
uint32_t FLASH_GetWriteProtectionOptionByte(void)
{
  /* 返回 Flash 写保护寄存器值 */
  return (uint32_t)(FLASH->WRPR);
}

/**
  * @brief  检查 FLASH 读保护状态是否被置位。
  * @note   本函数可用于所有 STM32F10x 器件。
  * @param  无
  * @retval FLASH 读保护状态 (SET 或 RESET)
  */
FlagStatus FLASH_GetReadOutProtectionStatus(void)
{
  FlagStatus readoutstatus = RESET;
  if ((FLASH->OBR & RDPRT_Mask) != (uint32_t)RESET)
  {
    readoutstatus = SET;
  }
  else
  {
    readoutstatus = RESET;
  }
  return readoutstatus;
}

/**
  * @brief  检查 FLASH 预取缓冲区状态是否被置位。
  * @note   本函数可用于所有 STM32F10x 器件。
  * @param  无
  * @retval FLASH 预取缓冲区状态 (SET 或 RESET)。
  */
FlagStatus FLASH_GetPrefetchBufferStatus(void)
{
  FlagStatus bitstatus = RESET;
  
  if ((FLASH->ACR & ACR_PRFTBS_Mask) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  /* 返回 FLASH 预取缓冲区状态的新状态 (SET 或 RESET) */
  return bitstatus; 
}

/**
  * @brief  使能或关闭指定的 FLASH 中断。
  * @note   本函数可用于所有 STM32F10x 器件。
  *         - 对于 STM32F10X_XL 器件，使能或关闭 Bank1 和 Bank2 的
  *           指定 FLASH 中断。
  *         - 对于其他器件，使能或关闭 Bank1 的指定 FLASH 中断。
  * @param  FLASH_IT: 指定要使能或关闭的 FLASH 中断源。
  *   该参数可以是以下值的任意组合：
  *     @arg FLASH_IT_ERROR: FLASH 错误中断
  *     @arg FLASH_IT_EOP: FLASH 操作结束中断
  * @param  NewState: 指定 Flash 中断的新状态。
  *   该参数可取：ENABLE 或 DISABLE。
  * @retval 无
  */
void FLASH_ITConfig(uint32_t FLASH_IT, FunctionalState NewState)
{
#ifdef STM32F10X_XL
  /* 检查参数 */
  assert_param(IS_FLASH_IT(FLASH_IT)); 
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if((FLASH_IT & 0x80000000) != 0x0)
  {
    if(NewState != DISABLE)
    {
      /* 使能中断源 */
      FLASH->CR2 |= (FLASH_IT & 0x7FFFFFFF);
    }
    else
    {
      /* 关闭中断源 */
      FLASH->CR2 &= ~(uint32_t)(FLASH_IT & 0x7FFFFFFF);
    }
  }
  else
  {
    if(NewState != DISABLE)
    {
      /* 使能中断源 */
      FLASH->CR |= FLASH_IT;
    }
    else
    {
      /* 关闭中断源 */
      FLASH->CR &= ~(uint32_t)FLASH_IT;
    }
  }
#else
  /* 检查参数 */
  assert_param(IS_FLASH_IT(FLASH_IT)); 
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if(NewState != DISABLE)
  {
    /* 使能中断源 */
    FLASH->CR |= FLASH_IT;
  }
  else
  {
    /* 关闭中断源 */
    FLASH->CR &= ~(uint32_t)FLASH_IT;
  }
#endif /* STM32F10X_XL */
}

/**
  * @brief  检查指定的 FLASH 标志是否被置位。
  * @note   本函数可用于所有 STM32F10x 器件。
  *         - 对于 STM32F10X_XL 器件，本函数检查指定的
  *           Bank1 或 Bank2 标志是否被置位。
  *         - 对于其他器件，本函数检查指定的 Bank1 标志
  *           是否被置位。
  * @param  FLASH_FLAG: 指定要检查的 FLASH 标志。
  *   该参数可取以下值之一：
  *     @arg FLASH_FLAG_BSY: FLASH 忙标志
  *     @arg FLASH_FLAG_PGERR: FLASH 编程错误标志
  *     @arg FLASH_FLAG_WRPRTERR: FLASH 写保护错误标志
  *     @arg FLASH_FLAG_EOP: FLASH 操作结束标志
  *     @arg FLASH_FLAG_OPTERR:  FLASH 选项字节错误标志
  * @retval FLASH_FLAG 的新状态 (SET 或 RESET)。
  */
FlagStatus FLASH_GetFlagStatus(uint32_t FLASH_FLAG)
{
  FlagStatus bitstatus = RESET;

#ifdef STM32F10X_XL
  /* 检查参数 */
  assert_param(IS_FLASH_GET_FLAG(FLASH_FLAG)) ;
  if(FLASH_FLAG == FLASH_FLAG_OPTERR) 
  {
    if((FLASH->OBR & FLASH_FLAG_OPTERR) != (uint32_t)RESET)
    {
      bitstatus = SET;
    }
    else
    {
      bitstatus = RESET;
    }
  }
  else
  {
    if((FLASH_FLAG & 0x80000000) != 0x0)
    {
      if((FLASH->SR2 & FLASH_FLAG) != (uint32_t)RESET)
      {
        bitstatus = SET;
      }
      else
      {
        bitstatus = RESET;
      }
    }
    else
    {
      if((FLASH->SR & FLASH_FLAG) != (uint32_t)RESET)
      {
        bitstatus = SET;
      }
      else
      {
        bitstatus = RESET;
      }
    }
  }
#else
  /* 检查参数 */
  assert_param(IS_FLASH_GET_FLAG(FLASH_FLAG)) ;
  if(FLASH_FLAG == FLASH_FLAG_OPTERR) 
  {
    if((FLASH->OBR & FLASH_FLAG_OPTERR) != (uint32_t)RESET)
    {
      bitstatus = SET;
    }
    else
    {
      bitstatus = RESET;
    }
  }
  else
  {
   if((FLASH->SR & FLASH_FLAG) != (uint32_t)RESET)
    {
      bitstatus = SET;
    }
    else
    {
      bitstatus = RESET;
    }
  }
#endif /* STM32F10X_XL */

  /* 返回 FLASH_FLAG 的新状态 (SET 或 RESET) */
  return bitstatus;
}

/**
  * @brief  清除 FLASH 的挂起标志。
  * @note   本函数可用于所有 STM32F10x 器件。
  *         - 对于 STM32F10X_XL 器件，本函数清除 Bank1 或 Bank2 的挂起标志
  *         - 对于其他器件，本函数清除 Bank1 的挂起标志。
  * @param  FLASH_FLAG: 指定要清除的 FLASH 标志。
  *   该参数可以是以下值的任意组合：
  *     @arg FLASH_FLAG_PGERR: FLASH 编程错误标志
  *     @arg FLASH_FLAG_WRPRTERR: FLASH 写保护错误标志
  *     @arg FLASH_FLAG_EOP: FLASH 操作结束标志
  * @retval 无
  */
void FLASH_ClearFlag(uint32_t FLASH_FLAG)
{
#ifdef STM32F10X_XL
  /* 检查参数 */
  assert_param(IS_FLASH_CLEAR_FLAG(FLASH_FLAG)) ;

  if((FLASH_FLAG & 0x80000000) != 0x0)
  {
    /* 清除标志 */
    FLASH->SR2 = FLASH_FLAG;
  }
  else
  {
    /* 清除标志 */
    FLASH->SR = FLASH_FLAG;
  }  

#else
  /* 检查参数 */
  assert_param(IS_FLASH_CLEAR_FLAG(FLASH_FLAG)) ;
  
  /* 清除标志 */
  FLASH->SR = FLASH_FLAG;
#endif /* STM32F10X_XL */
}

/**
  * @brief  返回 FLASH 状态。
  * @note   本函数可用于所有 STM32F10x 器件，等效于
  *         FLASH_GetBank1Status 函数。
  * @param  无
  * @retval FLASH 状态：返回值可以是 FLASH_BUSY、FLASH_ERROR_PG、
  *         FLASH_ERROR_WRP 或 FLASH_COMPLETE
  */
FLASH_Status FLASH_GetStatus(void)
{
  FLASH_Status flashstatus = FLASH_COMPLETE;
  
  if((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY) 
  {
    flashstatus = FLASH_BUSY;
  }
  else 
  {  
    if((FLASH->SR & FLASH_FLAG_PGERR) != 0)
    { 
      flashstatus = FLASH_ERROR_PG;
    }
    else 
    {
      if((FLASH->SR & FLASH_FLAG_WRPRTERR) != 0 )
      {
        flashstatus = FLASH_ERROR_WRP;
      }
      else
      {
        flashstatus = FLASH_COMPLETE;
      }
    }
  }
  /* 返回 Flash 状态 */
  return flashstatus;
}

/**
  * @brief  返回 FLASH Bank1 状态。
  * @note   本函数可用于所有 STM32F10x 器件，等效于
  *         FLASH_GetStatus 函数。
  * @param  无
  * @retval FLASH 状态：返回值可以是 FLASH_BUSY、FLASH_ERROR_PG、
  *         FLASH_ERROR_WRP 或 FLASH_COMPLETE
  */
FLASH_Status FLASH_GetBank1Status(void)
{
  FLASH_Status flashstatus = FLASH_COMPLETE;
  
  if((FLASH->SR & FLASH_FLAG_BANK1_BSY) == FLASH_FLAG_BSY) 
  {
    flashstatus = FLASH_BUSY;
  }
  else 
  {  
    if((FLASH->SR & FLASH_FLAG_BANK1_PGERR) != 0)
    { 
      flashstatus = FLASH_ERROR_PG;
    }
    else 
    {
      if((FLASH->SR & FLASH_FLAG_BANK1_WRPRTERR) != 0 )
      {
        flashstatus = FLASH_ERROR_WRP;
      }
      else
      {
        flashstatus = FLASH_COMPLETE;
      }
    }
  }
  /* 返回 Flash 状态 */
  return flashstatus;
}

#ifdef STM32F10X_XL
/**
  * @brief  返回 FLASH Bank2 状态。
  * @note   本函数可用于 STM32F10x_XL 密度器件。
  * @param  无
  * @retval FLASH 状态：返回值可以是 FLASH_BUSY、FLASH_ERROR_PG、
  *        FLASH_ERROR_WRP 或 FLASH_COMPLETE
  */
FLASH_Status FLASH_GetBank2Status(void)
{
  FLASH_Status flashstatus = FLASH_COMPLETE;
  
  if((FLASH->SR2 & (FLASH_FLAG_BANK2_BSY & 0x7FFFFFFF)) == (FLASH_FLAG_BANK2_BSY & 0x7FFFFFFF)) 
  {
    flashstatus = FLASH_BUSY;
  }
  else 
  {  
    if((FLASH->SR2 & (FLASH_FLAG_BANK2_PGERR & 0x7FFFFFFF)) != 0)
    { 
      flashstatus = FLASH_ERROR_PG;
    }
    else 
    {
      if((FLASH->SR2 & (FLASH_FLAG_BANK2_WRPRTERR & 0x7FFFFFFF)) != 0 )
      {
        flashstatus = FLASH_ERROR_WRP;
      }
      else
      {
        flashstatus = FLASH_COMPLETE;
      }
    }
  }
  /* 返回 Flash 状态 */
  return flashstatus;
}
#endif /* STM32F10X_XL */
/**
  * @brief  等待 Flash 操作完成或发生超时。
  * @note   本函数可用于所有 STM32F10x 器件，
  *         它等效于 FLASH_WaitForLastBank1Operation。
  *         - 对于 STM32F10X_XL 器件，本函数等待 Bank1 Flash 操作
  *           完成或发生超时。
  *         - 对于所有其他器件，本函数等待 Flash 操作完成
  *           或发生超时。
  * @param  Timeout: FLASH 编程超时时间
  * @retval FLASH 状态：返回值可以是 FLASH_ERROR_PG、
  *         FLASH_ERROR_WRP、FLASH_COMPLETE 或 FLASH_TIMEOUT。
  */
FLASH_Status FLASH_WaitForLastOperation(uint32_t Timeout)
{ 
  FLASH_Status status = FLASH_COMPLETE;
   
  /* 检查 Flash 状态 */
  status = FLASH_GetBank1Status();
  /* 等待 Flash 操作完成或发生超时 */
  while((status == FLASH_BUSY) && (Timeout != 0x00))
  {
    status = FLASH_GetBank1Status();
    Timeout--;
  }
  if(Timeout == 0x00 )
  {
    status = FLASH_TIMEOUT;
  }
  /* 返回操作状态 */
  return status;
}

/**
  * @brief  等待 Bank1 上的 Flash 操作完成或发生超时。
  * @note   本函数可用于所有 STM32F10x 器件，
  *         它等效于 FLASH_WaitForLastOperation。
  * @param  Timeout: FLASH 编程超时时间
  * @retval FLASH 状态：返回值可以是 FLASH_ERROR_PG、
  *         FLASH_ERROR_WRP、FLASH_COMPLETE 或 FLASH_TIMEOUT。
  */
FLASH_Status FLASH_WaitForLastBank1Operation(uint32_t Timeout)
{ 
  FLASH_Status status = FLASH_COMPLETE;
   
  /* 检查 Flash 状态 */
  status = FLASH_GetBank1Status();
  /* 等待 Flash 操作完成或发生超时 */
  while((status == FLASH_FLAG_BANK1_BSY) && (Timeout != 0x00))
  {
    status = FLASH_GetBank1Status();
    Timeout--;
  }
  if(Timeout == 0x00 )
  {
    status = FLASH_TIMEOUT;
  }
  /* 返回操作状态 */
  return status;
}

#ifdef STM32F10X_XL
/**
  * @brief  等待 Bank2 上的 Flash 操作完成或发生超时。
  * @note   本函数仅可用于 STM32F10x_XL 密度器件。
  * @param  Timeout: FLASH 编程超时时间
  * @retval FLASH 状态：返回值可以是 FLASH_ERROR_PG、
  *         FLASH_ERROR_WRP、FLASH_COMPLETE 或 FLASH_TIMEOUT。
  */
FLASH_Status FLASH_WaitForLastBank2Operation(uint32_t Timeout)
{ 
  FLASH_Status status = FLASH_COMPLETE;
   
  /* 检查 Flash 状态 */
  status = FLASH_GetBank2Status();
  /* 等待 Flash 操作完成或发生超时 */
  while((status == (FLASH_FLAG_BANK2_BSY & 0x7FFFFFFF)) && (Timeout != 0x00))
  {
    status = FLASH_GetBank2Status();
    Timeout--;
  }
  if(Timeout == 0x00 )
  {
    status = FLASH_TIMEOUT;
  }
  /* 返回操作状态 */
  return status;
}
#endif /* STM32F10X_XL */

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
