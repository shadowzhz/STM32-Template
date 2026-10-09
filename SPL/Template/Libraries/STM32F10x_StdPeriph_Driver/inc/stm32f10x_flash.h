/**
  ******************************************************************************
  * @file    stm32f10x_flash.h
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件包含 FLASH 固件库的所有函数原型。
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

/* 定义以下宏，以防止本头文件被递归包含 -------------------------------------*/
#ifndef __STM32F10x_FLASH_H
#define __STM32F10x_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @addtogroup FLASH   FLASH 驱动模块
  * @{
  */

/** @defgroup FLASH_Exported_Types   FLASH 导出类型
  * @{
  */

/**
  * @brief  FLASH 状态
  */

typedef enum
{ 
  FLASH_BUSY = 1,
  FLASH_ERROR_PG,
  FLASH_ERROR_WRP,
  FLASH_COMPLETE,
  FLASH_TIMEOUT
}FLASH_Status;

/**
  * @}
  */

/** @defgroup FLASH_Exported_Constants   FLASH 导出常量
  * @{
  */

/** @defgroup Flash_Latency   Flash 延时
  * @{
  */

#define FLASH_Latency_0                ((uint32_t)0x00000000)  /*!< FLASH 零延时周期 */
#define FLASH_Latency_1                ((uint32_t)0x00000001)  /*!< FLASH 一个延时周期 */
#define FLASH_Latency_2                ((uint32_t)0x00000002)  /*!< FLASH 两个延时周期 */
#define IS_FLASH_LATENCY(LATENCY) (((LATENCY) == FLASH_Latency_0) || \
                                   ((LATENCY) == FLASH_Latency_1) || \
                                   ((LATENCY) == FLASH_Latency_2))
/**
  * @}
  */

/** @defgroup Half_Cycle_Enable_Disable   半周期使能关闭
  * @{
  */

#define FLASH_HalfCycleAccess_Enable   ((uint32_t)0x00000008)  /*!< FLASH 半周期使能 */
#define FLASH_HalfCycleAccess_Disable  ((uint32_t)0x00000000)  /*!< FLASH 半周期关闭 */
#define IS_FLASH_HALFCYCLEACCESS_STATE(STATE) (((STATE) == FLASH_HalfCycleAccess_Enable) || \
                                               ((STATE) == FLASH_HalfCycleAccess_Disable)) 
/**
  * @}
  */

/** @defgroup Prefetch_Buffer_Enable_Disable   预取缓冲区使能关闭
  * @{
  */

#define FLASH_PrefetchBuffer_Enable    ((uint32_t)0x00000010)  /*!< FLASH 预取缓冲区使能 */
#define FLASH_PrefetchBuffer_Disable   ((uint32_t)0x00000000)  /*!< FLASH 预取缓冲区关闭 */
#define IS_FLASH_PREFETCHBUFFER_STATE(STATE) (((STATE) == FLASH_PrefetchBuffer_Enable) || \
                                              ((STATE) == FLASH_PrefetchBuffer_Disable)) 
/**
  * @}
  */

/** @defgroup Option_Bytes_Write_Protection   选项字节写保护
  * @{
  */

/* 用于 STM32 小容量和中密度器件的取值 */
#define FLASH_WRProt_Pages0to3         ((uint32_t)0x00000001) /*!< STM32 小容量和中密度器件：页 0 到 3 的写保护 */
#define FLASH_WRProt_Pages4to7         ((uint32_t)0x00000002) /*!< STM32 小容量和中密度器件：页 4 到 7 的写保护 */
#define FLASH_WRProt_Pages8to11        ((uint32_t)0x00000004) /*!< STM32 小容量和中密度器件：页 8 到 11 的写保护 */
#define FLASH_WRProt_Pages12to15       ((uint32_t)0x00000008) /*!< STM32 小容量和中密度器件：页 12 到 15 的写保护 */
#define FLASH_WRProt_Pages16to19       ((uint32_t)0x00000010) /*!< STM32 小容量和中密度器件：页 16 到 19 的写保护 */
#define FLASH_WRProt_Pages20to23       ((uint32_t)0x00000020) /*!< STM32 小容量和中密度器件：页 20 到 23 的写保护 */
#define FLASH_WRProt_Pages24to27       ((uint32_t)0x00000040) /*!< STM32 小容量和中密度器件：页 24 到 27 的写保护 */
#define FLASH_WRProt_Pages28to31       ((uint32_t)0x00000080) /*!< STM32 小容量和中密度器件：页 28 到 31 的写保护 */

/* 用于 STM32 中密度器件的取值 */
#define FLASH_WRProt_Pages32to35       ((uint32_t)0x00000100) /*!< STM32 中密度器件：页 32 到 35 的写保护 */
#define FLASH_WRProt_Pages36to39       ((uint32_t)0x00000200) /*!< STM32 中密度器件：页 36 到 39 的写保护 */
#define FLASH_WRProt_Pages40to43       ((uint32_t)0x00000400) /*!< STM32 中密度器件：页 40 到 43 的写保护 */
#define FLASH_WRProt_Pages44to47       ((uint32_t)0x00000800) /*!< STM32 中密度器件：页 44 到 47 的写保护 */
#define FLASH_WRProt_Pages48to51       ((uint32_t)0x00001000) /*!< STM32 中密度器件：页 48 到 51 的写保护 */
#define FLASH_WRProt_Pages52to55       ((uint32_t)0x00002000) /*!< STM32 中密度器件：页 52 到 55 的写保护 */
#define FLASH_WRProt_Pages56to59       ((uint32_t)0x00004000) /*!< STM32 中密度器件：页 56 到 59 的写保护 */
#define FLASH_WRProt_Pages60to63       ((uint32_t)0x00008000) /*!< STM32 中密度器件：页 60 到 63 的写保护 */
#define FLASH_WRProt_Pages64to67       ((uint32_t)0x00010000) /*!< STM32 中密度器件：页 64 到 67 的写保护 */
#define FLASH_WRProt_Pages68to71       ((uint32_t)0x00020000) /*!< STM32 中密度器件：页 68 到 71 的写保护 */
#define FLASH_WRProt_Pages72to75       ((uint32_t)0x00040000) /*!< STM32 中密度器件：页 72 到 75 的写保护 */
#define FLASH_WRProt_Pages76to79       ((uint32_t)0x00080000) /*!< STM32 中密度器件：页 76 到 79 的写保护 */
#define FLASH_WRProt_Pages80to83       ((uint32_t)0x00100000) /*!< STM32 中容量器件：第 80 至 83 页的写保护 */
#define FLASH_WRProt_Pages84to87       ((uint32_t)0x00200000) /*!< STM32 中容量器件：第 84 至 87 页的写保护 */
#define FLASH_WRProt_Pages88to91       ((uint32_t)0x00400000) /*!< STM32 中容量器件：第 88 至 91 页的写保护 */
#define FLASH_WRProt_Pages92to95       ((uint32_t)0x00800000) /*!< STM32 中容量器件：第 92 至 95 页的写保护 */
#define FLASH_WRProt_Pages96to99       ((uint32_t)0x01000000) /*!< STM32 中容量器件：第 96 至 99 页的写保护 */
#define FLASH_WRProt_Pages100to103     ((uint32_t)0x02000000) /*!< STM32 中容量器件：第 100 至 103 页的写保护 */
#define FLASH_WRProt_Pages104to107     ((uint32_t)0x04000000) /*!< STM32 中容量器件：第 104 至 107 页的写保护 */
#define FLASH_WRProt_Pages108to111     ((uint32_t)0x08000000) /*!< STM32 中容量器件：第 108 至 111 页的写保护 */
#define FLASH_WRProt_Pages112to115     ((uint32_t)0x10000000) /*!< STM32 中容量器件：第 112 至 115 页的写保护 */
#define FLASH_WRProt_Pages116to119     ((uint32_t)0x20000000) /*!< STM32 中容量器件：第 115 至 119 页的写保护 */
#define FLASH_WRProt_Pages120to123     ((uint32_t)0x40000000) /*!< STM32 中容量器件：第 120 至 123 页的写保护 */
#define FLASH_WRProt_Pages124to127     ((uint32_t)0x80000000) /*!< STM32 中容量器件：第 124 至 127 页的写保护 */

/* 用于 STM32 高容量和 STM32F10X 互联型器件的取值 */
#define FLASH_WRProt_Pages0to1         ((uint32_t)0x00000001) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 0 至 1 页的写保护 */
#define FLASH_WRProt_Pages2to3         ((uint32_t)0x00000002) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 2 至 3 页的写保护 */
#define FLASH_WRProt_Pages4to5         ((uint32_t)0x00000004) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 4 至 5 页的写保护 */
#define FLASH_WRProt_Pages6to7         ((uint32_t)0x00000008) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 6 至 7 页的写保护 */
#define FLASH_WRProt_Pages8to9         ((uint32_t)0x00000010) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 8 至 9 页的写保护 */
#define FLASH_WRProt_Pages10to11       ((uint32_t)0x00000020) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 10 至 11 页的写保护 */
#define FLASH_WRProt_Pages12to13       ((uint32_t)0x00000040) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 12 至 13 页的写保护 */
#define FLASH_WRProt_Pages14to15       ((uint32_t)0x00000080) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 14 至 15 页的写保护 */
#define FLASH_WRProt_Pages16to17       ((uint32_t)0x00000100) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 16 至 17 页的写保护 */
#define FLASH_WRProt_Pages18to19       ((uint32_t)0x00000200) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 18 至 19 页的写保护 */
#define FLASH_WRProt_Pages20to21       ((uint32_t)0x00000400) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 20 至 21 页的写保护 */
#define FLASH_WRProt_Pages22to23       ((uint32_t)0x00000800) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 22 至 23 页的写保护 */
#define FLASH_WRProt_Pages24to25       ((uint32_t)0x00001000) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 24 至 25 页的写保护 */
#define FLASH_WRProt_Pages26to27       ((uint32_t)0x00002000) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 26 至 27 页的写保护 */
#define FLASH_WRProt_Pages28to29       ((uint32_t)0x00004000) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 28 至 29 页的写保护 */
#define FLASH_WRProt_Pages30to31       ((uint32_t)0x00008000) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 30 至 31 页的写保护 */
#define FLASH_WRProt_Pages32to33       ((uint32_t)0x00010000) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 32 至 33 页的写保护 */
#define FLASH_WRProt_Pages34to35       ((uint32_t)0x00020000) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 34 至 35 页的写保护 */
#define FLASH_WRProt_Pages36to37       ((uint32_t)0x00040000) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 36 至 37 页的写保护 */
#define FLASH_WRProt_Pages38to39       ((uint32_t)0x00080000) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 38 至 39 页的写保护 */
#define FLASH_WRProt_Pages40to41       ((uint32_t)0x00100000) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 40 至 41 页的写保护 */
#define FLASH_WRProt_Pages42to43       ((uint32_t)0x00200000) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 42 至 43 页的写保护 */
#define FLASH_WRProt_Pages44to45       ((uint32_t)0x00400000) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 44 至 45 页的写保护 */
#define FLASH_WRProt_Pages46to47       ((uint32_t)0x00800000) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 46 至 47 页的写保护 */
#define FLASH_WRProt_Pages48to49       ((uint32_t)0x01000000) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 48 至 49 页的写保护 */
#define FLASH_WRProt_Pages50to51       ((uint32_t)0x02000000) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 50 至 51 页的写保护 */
#define FLASH_WRProt_Pages52to53       ((uint32_t)0x04000000) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 52 至 53 页的写保护 */
#define FLASH_WRProt_Pages54to55       ((uint32_t)0x08000000) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 54 至 55 页的写保护 */
#define FLASH_WRProt_Pages56to57       ((uint32_t)0x10000000) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 56 至 57 页的写保护 */
#define FLASH_WRProt_Pages58to59       ((uint32_t)0x20000000) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 58 至 59 页的写保护 */
#define FLASH_WRProt_Pages60to61       ((uint32_t)0x40000000) /*!< STM32 高容量、超大容量和互联型器件：
                                                                   第 60 至 61 页的写保护 */
#define FLASH_WRProt_Pages62to127      ((uint32_t)0x80000000) /*!< STM32 互联型器件：第 62 至 127 页的写保护 */
#define FLASH_WRProt_Pages62to255      ((uint32_t)0x80000000) /*!< STM32 中容量器件：第 62 至 255 页的写保护 */
#define FLASH_WRProt_Pages62to511      ((uint32_t)0x80000000) /*!< STM32 超大容量器件：第 62 至 511 页的写保护 */

#define FLASH_WRProt_AllPages          ((uint32_t)0xFFFFFFFF) /*!< 所有页的写保护 */

#define IS_FLASH_WRPROT_PAGE(PAGE) (((PAGE) != 0x00000000))

#define IS_FLASH_ADDRESS(ADDRESS) (((ADDRESS) >= 0x08000000) && ((ADDRESS) < 0x080FFFFF))

#define IS_OB_DATA_ADDRESS(ADDRESS) (((ADDRESS) == 0x1FFFF804) || ((ADDRESS) == 0x1FFFF806))

/**
  * @}
  */

/** @defgroup Option_Bytes_IWatchdog   独立看门狗选项字节
  * @{
  */

#define OB_IWDG_SW                     ((uint16_t)0x0001)  /*!< 选择软件 IWDG */
#define OB_IWDG_HW                     ((uint16_t)0x0000)  /*!< 选择硬件 IWDG */
#define IS_OB_IWDG_SOURCE(SOURCE) (((SOURCE) == OB_IWDG_SW) || ((SOURCE) == OB_IWDG_HW))

/**
  * @}
  */

/** @defgroup Option_Bytes_nRST_STOP   STOP 模式复位选项字节
  * @{
  */

#define OB_STOP_NoRST                  ((uint16_t)0x0002) /*!< 进入 STOP 模式时不产生复位 */
#define OB_STOP_RST                    ((uint16_t)0x0000) /*!< 进入 STOP 模式时产生复位 */
#define IS_OB_STOP_SOURCE(SOURCE) (((SOURCE) == OB_STOP_NoRST) || ((SOURCE) == OB_STOP_RST))

/**
  * @}
  */

/** @defgroup Option_Bytes_nRST_STDBY   STANDBY 模式复位选项字节
  * @{
  */

#define OB_STDBY_NoRST                 ((uint16_t)0x0004) /*!< 进入 STANDBY 模式时不产生复位 */
#define OB_STDBY_RST                   ((uint16_t)0x0000) /*!< 进入 STANDBY 模式时产生复位 */
#define IS_OB_STDBY_SOURCE(SOURCE) (((SOURCE) == OB_STDBY_NoRST) || ((SOURCE) == OB_STDBY_RST))

#ifdef STM32F10X_XL
/**
  * @}
  */
/** @defgroup FLASH_Boot  启动配置
  * @{
  */
#define FLASH_BOOT_Bank1  ((uint16_t)0x0000) /*!< 启动时，若 BOOT 引脚配置为从用户 Flash 启动，
                                                  且选择该参数，则器件将从 Bank1 启动（默认） */
#define FLASH_BOOT_Bank2  ((uint16_t)0x0001) /*!< 启动时，若 BOOT 引脚配置为从用户 Flash 启动，
                                                  且选择该参数，则器件将从 Bank 2 或 Bank 1 启动，
                                                  具体取决于 bank 的激活情况 */
#define IS_FLASH_BOOT(BOOT) (((BOOT) == FLASH_BOOT_Bank1) || ((BOOT) == FLASH_BOOT_Bank2))
#endif
/**
  * @}
  */
/** @defgroup FLASH_Interrupts   FLASH 中断
  * @{
  */
#ifdef STM32F10X_XL
#define FLASH_IT_BANK2_ERROR                 ((uint32_t)0x80000400)  /*!< FPEC BANK2 错误中断源 */
#define FLASH_IT_BANK2_EOP                   ((uint32_t)0x80001000)  /*!< FLASH BANK2 操作结束中断源 */

#define FLASH_IT_BANK1_ERROR                 FLASH_IT_ERROR          /*!< FPEC BANK1 错误中断源 */
#define FLASH_IT_BANK1_EOP                   FLASH_IT_EOP            /*!< FLASH BANK1 操作结束中断源 */

#define FLASH_IT_ERROR                 ((uint32_t)0x00000400)  /*!< FPEC BANK1 错误中断源 */
#define FLASH_IT_EOP                   ((uint32_t)0x00001000)  /*!< FLASH BANK1 操作结束中断源 */
#define IS_FLASH_IT(IT) ((((IT) & (uint32_t)0x7FFFEBFF) == 0x00000000) && (((IT) != 0x00000000)))
#else
#define FLASH_IT_ERROR                 ((uint32_t)0x00000400)  /*!< FPEC 错误中断源 */
#define FLASH_IT_EOP                   ((uint32_t)0x00001000)  /*!< FLASH 操作结束中断源 */
#define FLASH_IT_BANK1_ERROR           FLASH_IT_ERROR          /*!< FPEC BANK1 错误中断源 */
#define FLASH_IT_BANK1_EOP             FLASH_IT_EOP            /*!< FLASH BANK1 操作结束中断源 */

#define IS_FLASH_IT(IT) ((((IT) & (uint32_t)0xFFFFEBFF) == 0x00000000) && (((IT) != 0x00000000)))
#endif

/**
  * @}
  */

/** @defgroup FLASH_Flags   FLASH 标志
  * @{
  */
#ifdef STM32F10X_XL
#define FLASH_FLAG_BANK2_BSY                 ((uint32_t)0x80000001)  /*!< FLASH BANK2 忙标志 */
#define FLASH_FLAG_BANK2_EOP                 ((uint32_t)0x80000020)  /*!< FLASH BANK2 操作结束标志 */
#define FLASH_FLAG_BANK2_PGERR               ((uint32_t)0x80000004)  /*!< FLASH BANK2 编程错误标志 */
#define FLASH_FLAG_BANK2_WRPRTERR            ((uint32_t)0x80000010)  /*!< FLASH BANK2 写保护错误标志 */

#define FLASH_FLAG_BANK1_BSY                 FLASH_FLAG_BSY       /*!< FLASH BANK1 忙标志*/
#define FLASH_FLAG_BANK1_EOP                 FLASH_FLAG_EOP       /*!< FLASH BANK1 操作结束标志 */
#define FLASH_FLAG_BANK1_PGERR               FLASH_FLAG_PGERR     /*!< FLASH BANK1 编程错误标志 */
#define FLASH_FLAG_BANK1_WRPRTERR            FLASH_FLAG_WRPRTERR  /*!< FLASH BANK1 写保护错误标志 */

#define FLASH_FLAG_BSY                 ((uint32_t)0x00000001)  /*!< FLASH 忙标志 */
#define FLASH_FLAG_EOP                 ((uint32_t)0x00000020)  /*!< FLASH 操作结束标志 */
#define FLASH_FLAG_PGERR               ((uint32_t)0x00000004)  /*!< FLASH 编程错误标志 */
#define FLASH_FLAG_WRPRTERR            ((uint32_t)0x00000010)  /*!< FLASH 写保护错误标志 */
#define FLASH_FLAG_OPTERR              ((uint32_t)0x00000001)  /*!< FLASH 选项字节错误标志 */
 
#define IS_FLASH_CLEAR_FLAG(FLAG) ((((FLAG) & (uint32_t)0x7FFFFFCA) == 0x00000000) && ((FLAG) != 0x00000000))
#define IS_FLASH_GET_FLAG(FLAG)  (((FLAG) == FLASH_FLAG_BSY) || ((FLAG) == FLASH_FLAG_EOP) || \
                                  ((FLAG) == FLASH_FLAG_PGERR) || ((FLAG) == FLASH_FLAG_WRPRTERR) || \
                                  ((FLAG) == FLASH_FLAG_OPTERR)|| \
                                  ((FLAG) == FLASH_FLAG_BANK1_BSY) || ((FLAG) == FLASH_FLAG_BANK1_EOP) || \
                                  ((FLAG) == FLASH_FLAG_BANK1_PGERR) || ((FLAG) == FLASH_FLAG_BANK1_WRPRTERR) || \
                                  ((FLAG) == FLASH_FLAG_BANK2_BSY) || ((FLAG) == FLASH_FLAG_BANK2_EOP) || \
                                  ((FLAG) == FLASH_FLAG_BANK2_PGERR) || ((FLAG) == FLASH_FLAG_BANK2_WRPRTERR))
#else
#define FLASH_FLAG_BSY                 ((uint32_t)0x00000001)  /*!< FLASH 忙标志 */
#define FLASH_FLAG_EOP                 ((uint32_t)0x00000020)  /*!< FLASH 操作结束标志 */
#define FLASH_FLAG_PGERR               ((uint32_t)0x00000004)  /*!< FLASH 编程错误标志 */
#define FLASH_FLAG_WRPRTERR            ((uint32_t)0x00000010)  /*!< FLASH 写保护错误标志 */
#define FLASH_FLAG_OPTERR              ((uint32_t)0x00000001)  /*!< FLASH 选项字节错误标志 */

#define FLASH_FLAG_BANK1_BSY                 FLASH_FLAG_BSY       /*!< FLASH BANK1 忙标志*/
#define FLASH_FLAG_BANK1_EOP                 FLASH_FLAG_EOP       /*!< FLASH BANK1 操作结束标志 */
#define FLASH_FLAG_BANK1_PGERR               FLASH_FLAG_PGERR     /*!< FLASH BANK1 编程错误标志 */
#define FLASH_FLAG_BANK1_WRPRTERR            FLASH_FLAG_WRPRTERR  /*!< FLASH BANK1 写保护错误标志 */
 
#define IS_FLASH_CLEAR_FLAG(FLAG) ((((FLAG) & (uint32_t)0xFFFFFFCA) == 0x00000000) && ((FLAG) != 0x00000000))
#define IS_FLASH_GET_FLAG(FLAG)  (((FLAG) == FLASH_FLAG_BSY) || ((FLAG) == FLASH_FLAG_EOP) || \
                                  ((FLAG) == FLASH_FLAG_PGERR) || ((FLAG) == FLASH_FLAG_WRPRTERR) || \
								  ((FLAG) == FLASH_FLAG_BANK1_BSY) || ((FLAG) == FLASH_FLAG_BANK1_EOP) || \
                                  ((FLAG) == FLASH_FLAG_BANK1_PGERR) || ((FLAG) == FLASH_FLAG_BANK1_WRPRTERR) || \
                                  ((FLAG) == FLASH_FLAG_OPTERR))
#endif

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup FLASH_Exported_Macros   FLASH 导出宏
  * @{
  */

/**
  * @}
  */

/** @defgroup FLASH_Exported_Functions   FLASH 导出函数
  * @{
  */

/*------------ 适用于所有 STM32F10x 器件的函数 -----*/
void FLASH_SetLatency(uint32_t FLASH_Latency);
void FLASH_HalfCycleAccessCmd(uint32_t FLASH_HalfCycleAccess);
void FLASH_PrefetchBufferCmd(uint32_t FLASH_PrefetchBuffer);
void FLASH_Unlock(void);
void FLASH_Lock(void);
FLASH_Status FLASH_ErasePage(uint32_t Page_Address);
FLASH_Status FLASH_EraseAllPages(void);
FLASH_Status FLASH_EraseOptionBytes(void);
FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data);
FLASH_Status FLASH_ProgramHalfWord(uint32_t Address, uint16_t Data);
FLASH_Status FLASH_ProgramOptionByteData(uint32_t Address, uint8_t Data);
FLASH_Status FLASH_EnableWriteProtection(uint32_t FLASH_Pages);
FLASH_Status FLASH_ReadOutProtection(FunctionalState NewState);
FLASH_Status FLASH_UserOptionByteConfig(uint16_t OB_IWDG, uint16_t OB_STOP, uint16_t OB_STDBY);
uint32_t FLASH_GetUserOptionByte(void);
uint32_t FLASH_GetWriteProtectionOptionByte(void);
FlagStatus FLASH_GetReadOutProtectionStatus(void);
FlagStatus FLASH_GetPrefetchBufferStatus(void);
void FLASH_ITConfig(uint32_t FLASH_IT, FunctionalState NewState);
FlagStatus FLASH_GetFlagStatus(uint32_t FLASH_FLAG);
void FLASH_ClearFlag(uint32_t FLASH_FLAG);
FLASH_Status FLASH_GetStatus(void);
FLASH_Status FLASH_WaitForLastOperation(uint32_t Timeout);

/*------------ 适用于所有 STM32F10x 器件的新函数 -----*/
void FLASH_UnlockBank1(void);
void FLASH_LockBank1(void);
FLASH_Status FLASH_EraseAllBank1Pages(void);
FLASH_Status FLASH_GetBank1Status(void);
FLASH_Status FLASH_WaitForLastBank1Operation(uint32_t Timeout);

#ifdef STM32F10X_XL
/*---- 仅用于 STM32F10x_XL 密度器件的新函数 -----*/
void FLASH_UnlockBank2(void);
void FLASH_LockBank2(void);
FLASH_Status FLASH_EraseAllBank2Pages(void);
FLASH_Status FLASH_GetBank2Status(void);
FLASH_Status FLASH_WaitForLastBank2Operation(uint32_t Timeout);
FLASH_Status FLASH_BootConfig(uint16_t FLASH_BOOT);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_FLASH_H */
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
