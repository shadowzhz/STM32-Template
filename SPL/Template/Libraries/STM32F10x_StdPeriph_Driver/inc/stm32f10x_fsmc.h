/**
  ******************************************************************************
  * @file    stm32f10x_fsmc.h
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   本文件包含 FSMC 固件库的所有函数原型。
  ******************************************************************************
  * @attention
  *
  * 本固件仅供指引之用，旨在为客户提供有关其产品的编码信息，
  * 以便客户节省时间。因此，对于因本固件内容及/或客户将本文
  * 所含编码信息用于其产品而产生的任何索赔所导致的任何直接、
  * 间接或后果性损害，STMicroelectronics 概不负责。
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* 定义以下宏，以防止本头文件被递归包含 -------------------------------------*/
#ifndef __STM32F10x_FSMC_H
#define __STM32F10x_FSMC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* 头文件包含 ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup STM32F10x_StdPeriph_Driver   STM32F10x 标准外设驱动
  * @{
  */

/** @addtogroup FSMC  FSMC 驱动模块
  * @{
  */

/** @defgroup FSMC_Exported_Types   FSMC 导出类型
  * @{
  */

/**
  * @brief  NOR/SRAM Bank 的时序参数
  */

typedef struct
{
  uint32_t FSMC_AddressSetupTime;       /*!< 定义用于配置地址建立时间持续时间的
                                             HCLK 周期数。
                                             该参数可取 0 到 0xF 之间的值。
                                             @note: 该参数不用于同步 NOR Flash 存储器。 */

  uint32_t FSMC_AddressHoldTime;        /*!< 定义用于配置地址保持时间持续时间的
                                             HCLK 周期数。
                                             该参数可取 0 到 0xF 之间的值。
                                             @note: 该参数不用于同步 NOR Flash 存储器。*/

  uint32_t FSMC_DataSetupTime;          /*!< 定义用于配置数据建立时间持续时间的
                                             HCLK 周期数。
                                             该参数可取 0 到 0xFF 之间的值。
                                             @note: 该参数用于 SRAM、ROM 和异步复用 NOR Flash 存储器。 */

  uint32_t FSMC_BusTurnAroundDuration;  /*!< 定义用于配置总线周转持续时间的
                                             HCLK 周期数。
                                             该参数可取 0 到 0xF 之间的值。
                                             @note: 该参数仅用于复用 NOR Flash 存储器。 */

  uint32_t FSMC_CLKDivision;            /*!< 定义 CLK 时钟输出信号的周期，以 HCLK 周期数表示。
                                             该参数可取 1 到 0xF 之间的值。
                                             @note: 该参数不用于异步 NOR Flash、SRAM 或 ROM 访问。 */

  uint32_t FSMC_DataLatency;            /*!< 定义在获取第一个数据之前向存储器发出的
                                             存储器时钟周期数。
                                             该参数的值取决于存储器类型，如下所示：
                                              - 对于 CRAM，该参数必须设置为 0
                                              - 在异步 NOR、SRAM 或 ROM 访问中该参数无关紧要
                                              - 在使能同步突发模式的 NOR Flash 存储器中
                                                该参数可取 0 到 0xF 之间的值 */

  uint32_t FSMC_AccessMode;             /*!< 指定异步访问模式。
                                             该参数可取 @ref FSMC_Access_Mode 的值 */
}FSMC_NORSRAMTimingInitTypeDef;

/**
  * @brief  FSMC NOR/SRAM 初始化结构定义
  */

typedef struct
{
  uint32_t FSMC_Bank;                /*!< 指定将要使用的 NOR/SRAM 存储块。
                                          该参数可取 @ref FSMC_NORSRAM_Bank 的值 */

  uint32_t FSMC_DataAddressMux;      /*!< 指定地址和数据值是否
                                          在数据总线上复用。
                                          该参数可取 @ref FSMC_Data_Address_Bus_Multiplexing 的值 */

  uint32_t FSMC_MemoryType;          /*!< 指定连接到相应存储块的
                                          外部存储器类型。
                                          该参数可取 @ref FSMC_Memory_Type 的值 */

  uint32_t FSMC_MemoryDataWidth;     /*!< 指定外部存储器件的宽度。
                                          该参数可取 @ref FSMC_Data_Width 的值 */

  uint32_t FSMC_BurstAccessMode;     /*!< 使能或关闭 Flash 存储器的突发访问模式，
                                          仅对同步突发 Flash 存储器有效。
                                          该参数可取 @ref FSMC_Burst_Access_Mode 的值 */
                                       
  uint32_t FSMC_AsynchronousWait;     /*!< 使能或关闭异步传输期间的等待信号，
                                          仅对异步 Flash 存储器有效。
                                          该参数可取 @ref FSMC_AsynchronousWait 的值 */

  uint32_t FSMC_WaitSignalPolarity;  /*!< 指定等待信号极性，仅在突发模式下
                                          访问 Flash 存储器时有效。
                                          该参数可取 @ref FSMC_Wait_Signal_Polarity 的值 */

  uint32_t FSMC_WrapMode;            /*!< 使能或关闭 Flash 存储器的回绕突发访问模式，
                                          仅在突发模式下访问 Flash 存储器时有效。
                                          该参数可取 @ref FSMC_Wrap_Mode 的值 */

  uint32_t FSMC_WaitSignalActive;    /*!< 指定等待信号是由存储器在等待状态前
                                          一个时钟周期还是在等待状态期间置起，
                                          仅在突发模式下访问存储器时有效。
                                          该参数可取 @ref FSMC_Wait_Timing 的值 */

  uint32_t FSMC_WriteOperation;      /*!< 使能或关闭 FSMC 在所选 bank 中的写操作。
                                          该参数可取 @ref FSMC_Write_Operation 的值 */

  uint32_t FSMC_WaitSignal;          /*!< 使能或关闭通过等待信号插入等待状态，
                                          对突发模式下的 Flash 存储器访问有效。
                                          该参数可取 @ref FSMC_Wait_Signal 的值 */

  uint32_t FSMC_ExtendedMode;        /*!< 使能或关闭扩展模式。
                                          该参数可取 @ref FSMC_Extended_Mode 的值 */

  uint32_t FSMC_WriteBurst;          /*!< 使能或关闭写突发操作。
                                          该参数可取 @ref FSMC_Write_Burst 的值 */ 

  FSMC_NORSRAMTimingInitTypeDef* FSMC_ReadWriteTimingStruct; /*!< 未使用 ExtendedMode 时读写访问的时序参数*/  

  FSMC_NORSRAMTimingInitTypeDef* FSMC_WriteTimingStruct;     /*!< 使用 ExtendedMode 时写访问的时序参数*/      
}FSMC_NORSRAMInitTypeDef;

/**
  * @brief  FSMC NAND 和 PCCARD Bank 的时序参数
  */

typedef struct
{
  uint32_t FSMC_SetupTime;      /*!< 定义对 common/Attribute 或 I/O 存储空间（取决于
                                     要配置的存储空间时序）进行 NAND-Flash 读或写访问时，
                                     在命令置起之前建立地址所需的
                                     HCLK 周期数。
                                     该参数可取 0 到 0xFF 之间的值。*/

  uint32_t FSMC_WaitSetupTime;  /*!< 定义对 common/Attribute 或 I/O 存储空间（取决于
                                     要配置的存储空间时序）进行 NAND-Flash 读或写访问时
                                     置起命令所需的最少 HCLK 周期数。
                                     该参数可取 0x00 到 0xFF 之间的数值 */

  uint32_t FSMC_HoldSetupTime;  /*!< 定义对 common/Attribute 或 I/O 存储空间（取决于
                                     要配置的存储空间时序）进行 NAND-Flash 读或写访问时，
                                     在命令撤销之后保持地址（写访问时还包括数据）
                                     所需的 HCLK 时钟周期数。
                                     该参数可取 0x00 到 0xFF 之间的数值 */

  uint32_t FSMC_HiZSetupTime;   /*!< 定义对 common/Attribute 或 I/O 存储空间（取决于
                                     要配置的存储空间时序）开始 NAND-Flash 写访问之后，
                                     数据总线保持高阻态的 HCLK 时钟周期数。
                                     该参数可取 0x00 到 0xFF 之间的数值 */
}FSMC_NAND_PCCARDTimingInitTypeDef;

/**
  * @brief  FSMC NAND 初始化结构定义
  */

typedef struct
{
  uint32_t FSMC_Bank;              /*!< 指定将要使用的 NAND 存储块。
                                      该参数可取 @ref FSMC_NAND_Bank 的值 */

  uint32_t FSMC_Waitfeature;      /*!< 使能或关闭 NAND 存储块的等待功能。
                                       该参数可取 @ref FSMC_Wait_feature 的任意值 */

  uint32_t FSMC_MemoryDataWidth;  /*!< 指定外部存储器件的宽度。
                                       该参数可取 @ref FSMC_Data_Width 的任意值 */

  uint32_t FSMC_ECC;              /*!< 使能或关闭 ECC 计算。
                                       该参数可取 @ref FSMC_ECC 的任意值 */

  uint32_t FSMC_ECCPageSize;      /*!< 定义扩展 ECC 的页大小。
                                       该参数可取 @ref FSMC_ECC_Page_Size 的任意值 */

  uint32_t FSMC_TCLRSetupTime;    /*!< 定义用于配置 CLE 为低与 RE 为低之间
                                       延迟的 HCLK 周期数。
                                       该参数可取 0 到 0xFF 之间的值。 */

  uint32_t FSMC_TARSetupTime;     /*!< 定义用于配置 ALE 为低与 RE 为低之间
                                       延迟的 HCLK 周期数。
                                       该参数可取 0x0 到 0xFF 之间的数值 */ 

  FSMC_NAND_PCCARDTimingInitTypeDef*  FSMC_CommonSpaceTimingStruct;   /*!< FSMC 公共空间时序 */ 

  FSMC_NAND_PCCARDTimingInitTypeDef*  FSMC_AttributeSpaceTimingStruct; /*!< FSMC 属性空间时序 */
}FSMC_NANDInitTypeDef;

/**
  * @brief  FSMC PCCARD 初始化结构定义
  */

typedef struct
{
  uint32_t FSMC_Waitfeature;    /*!< 使能或关闭存储块的等待功能。
                                    该参数可取 @ref FSMC_Wait_feature 的任意值 */

  uint32_t FSMC_TCLRSetupTime;  /*!< 定义用于配置 CLE 为低与 RE 为低之间
                                     延迟的 HCLK 周期数。
                                     该参数可取 0 到 0xFF 之间的值。 */

  uint32_t FSMC_TARSetupTime;   /*!< 定义用于配置 ALE 为低与 RE 为低之间
                                     延迟的 HCLK 周期数。
                                     该参数可取 0x0 到 0xFF 之间的数值 */ 

  
  FSMC_NAND_PCCARDTimingInitTypeDef*  FSMC_CommonSpaceTimingStruct; /*!< FSMC 公共空间时序 */

  FSMC_NAND_PCCARDTimingInitTypeDef*  FSMC_AttributeSpaceTimingStruct;  /*!< FSMC 属性空间时序 */ 
  
  FSMC_NAND_PCCARDTimingInitTypeDef*  FSMC_IOSpaceTimingStruct; /*!< FSMC IO 空间时序 */  
}FSMC_PCCARDInitTypeDef;

/**
  * @}
  */

/** @defgroup FSMC_Exported_Constants   FSMC 导出常量
  * @{
  */

/** @defgroup FSMC_NORSRAM_Bank   FSMC NOR/SRAM Bank
  * @{
  */
#define FSMC_Bank1_NORSRAM1                             ((uint32_t)0x00000000)
#define FSMC_Bank1_NORSRAM2                             ((uint32_t)0x00000002)
#define FSMC_Bank1_NORSRAM3                             ((uint32_t)0x00000004)
#define FSMC_Bank1_NORSRAM4                             ((uint32_t)0x00000006)
/**
  * @}
  */

/** @defgroup FSMC_NAND_Bank   FSMC NAND Bank
  * @{
  */  
#define FSMC_Bank2_NAND                                 ((uint32_t)0x00000010)
#define FSMC_Bank3_NAND                                 ((uint32_t)0x00000100)
/**
  * @}
  */

/** @defgroup FSMC_PCCARD_Bank   FSMC PCCARD Bank
  * @{
  */    
#define FSMC_Bank4_PCCARD                               ((uint32_t)0x00001000)
/**
  * @}
  */

#define IS_FSMC_NORSRAM_BANK(BANK) (((BANK) == FSMC_Bank1_NORSRAM1) || \
                                    ((BANK) == FSMC_Bank1_NORSRAM2) || \
                                    ((BANK) == FSMC_Bank1_NORSRAM3) || \
                                    ((BANK) == FSMC_Bank1_NORSRAM4))

#define IS_FSMC_NAND_BANK(BANK) (((BANK) == FSMC_Bank2_NAND) || \
                                 ((BANK) == FSMC_Bank3_NAND))

#define IS_FSMC_GETFLAG_BANK(BANK) (((BANK) == FSMC_Bank2_NAND) || \
                                    ((BANK) == FSMC_Bank3_NAND) || \
                                    ((BANK) == FSMC_Bank4_PCCARD))

#define IS_FSMC_IT_BANK(BANK) (((BANK) == FSMC_Bank2_NAND) || \
                               ((BANK) == FSMC_Bank3_NAND) || \
                               ((BANK) == FSMC_Bank4_PCCARD))

/** @defgroup NOR_SRAM_Controller   NOR/SRAM 控制器
  * @{
  */

/** @defgroup FSMC_Data_Address_Bus_Multiplexing   FSMC 数据地址总线复用
  * @{
  */

#define FSMC_DataAddressMux_Disable                       ((uint32_t)0x00000000)
#define FSMC_DataAddressMux_Enable                        ((uint32_t)0x00000002)
#define IS_FSMC_MUX(MUX) (((MUX) == FSMC_DataAddressMux_Disable) || \
                          ((MUX) == FSMC_DataAddressMux_Enable))

/**
  * @}
  */

/** @defgroup FSMC_Memory_Type   FSMC 存储器类型
  * @{
  */

#define FSMC_MemoryType_SRAM                            ((uint32_t)0x00000000)
#define FSMC_MemoryType_PSRAM                           ((uint32_t)0x00000004)
#define FSMC_MemoryType_NOR                             ((uint32_t)0x00000008)
#define IS_FSMC_MEMORY(MEMORY) (((MEMORY) == FSMC_MemoryType_SRAM) || \
                                ((MEMORY) == FSMC_MemoryType_PSRAM)|| \
                                ((MEMORY) == FSMC_MemoryType_NOR))

/**
  * @}
  */

/** @defgroup FSMC_Data_Width   FSMC 数据宽度
  * @{
  */

#define FSMC_MemoryDataWidth_8b                         ((uint32_t)0x00000000)
#define FSMC_MemoryDataWidth_16b                        ((uint32_t)0x00000010)
#define IS_FSMC_MEMORY_WIDTH(WIDTH) (((WIDTH) == FSMC_MemoryDataWidth_8b) || \
                                     ((WIDTH) == FSMC_MemoryDataWidth_16b))

/**
  * @}
  */

/** @defgroup FSMC_Burst_Access_Mode   FSMC 突发访问模式
  * @{
  */

#define FSMC_BurstAccessMode_Disable                    ((uint32_t)0x00000000) 
#define FSMC_BurstAccessMode_Enable                     ((uint32_t)0x00000100)
#define IS_FSMC_BURSTMODE(STATE) (((STATE) == FSMC_BurstAccessMode_Disable) || \
                                  ((STATE) == FSMC_BurstAccessMode_Enable))
/**
  * @}
  */
  
/** @defgroup FSMC_AsynchronousWait   FSMC 异步等待
  * @{
  */
#define FSMC_AsynchronousWait_Disable                   ((uint32_t)0x00000000)
#define FSMC_AsynchronousWait_Enable                    ((uint32_t)0x00008000)
#define IS_FSMC_ASYNWAIT(STATE) (((STATE) == FSMC_AsynchronousWait_Disable) || \
                                 ((STATE) == FSMC_AsynchronousWait_Enable))

/**
  * @}
  */
  
/** @defgroup FSMC_Wait_Signal_Polarity   FSMC 等待信号极性
  * @{
  */

#define FSMC_WaitSignalPolarity_Low                     ((uint32_t)0x00000000)
#define FSMC_WaitSignalPolarity_High                    ((uint32_t)0x00000200)
#define IS_FSMC_WAIT_POLARITY(POLARITY) (((POLARITY) == FSMC_WaitSignalPolarity_Low) || \
                                         ((POLARITY) == FSMC_WaitSignalPolarity_High)) 

/**
  * @}
  */

/** @defgroup FSMC_Wrap_Mode   FSMC 回绕模式
  * @{
  */

#define FSMC_WrapMode_Disable                           ((uint32_t)0x00000000)
#define FSMC_WrapMode_Enable                            ((uint32_t)0x00000400) 
#define IS_FSMC_WRAP_MODE(MODE) (((MODE) == FSMC_WrapMode_Disable) || \
                                 ((MODE) == FSMC_WrapMode_Enable))

/**
  * @}
  */

/** @defgroup FSMC_Wait_Timing   FSMC 等待时序
  * @{
  */

#define FSMC_WaitSignalActive_BeforeWaitState           ((uint32_t)0x00000000)
#define FSMC_WaitSignalActive_DuringWaitState           ((uint32_t)0x00000800) 
#define IS_FSMC_WAIT_SIGNAL_ACTIVE(ACTIVE) (((ACTIVE) == FSMC_WaitSignalActive_BeforeWaitState) || \
                                            ((ACTIVE) == FSMC_WaitSignalActive_DuringWaitState))

/**
  * @}
  */

/** @defgroup FSMC_Write_Operation   FSMC 写操作
  * @{
  */

#define FSMC_WriteOperation_Disable                     ((uint32_t)0x00000000)
#define FSMC_WriteOperation_Enable                      ((uint32_t)0x00001000)
#define IS_FSMC_WRITE_OPERATION(OPERATION) (((OPERATION) == FSMC_WriteOperation_Disable) || \
                                            ((OPERATION) == FSMC_WriteOperation_Enable))
                              
/**
  * @}
  */

/** @defgroup FSMC_Wait_Signal   FSMC 等待信号
  * @{
  */

#define FSMC_WaitSignal_Disable                         ((uint32_t)0x00000000)
#define FSMC_WaitSignal_Enable                          ((uint32_t)0x00002000) 
#define IS_FSMC_WAITE_SIGNAL(SIGNAL) (((SIGNAL) == FSMC_WaitSignal_Disable) || \
                                      ((SIGNAL) == FSMC_WaitSignal_Enable))
/**
  * @}
  */

/** @defgroup FSMC_Extended_Mode   FSMC 扩展模式
  * @{
  */

#define FSMC_ExtendedMode_Disable                       ((uint32_t)0x00000000)
#define FSMC_ExtendedMode_Enable                        ((uint32_t)0x00004000)

#define IS_FSMC_EXTENDED_MODE(MODE) (((MODE) == FSMC_ExtendedMode_Disable) || \
                                     ((MODE) == FSMC_ExtendedMode_Enable)) 

/**
  * @}
  */

/** @defgroup FSMC_Write_Burst   FSMC 写突发
  * @{
  */

#define FSMC_WriteBurst_Disable                         ((uint32_t)0x00000000)
#define FSMC_WriteBurst_Enable                          ((uint32_t)0x00080000) 
#define IS_FSMC_WRITE_BURST(BURST) (((BURST) == FSMC_WriteBurst_Disable) || \
                                    ((BURST) == FSMC_WriteBurst_Enable))
/**
  * @}
  */

/** @defgroup FSMC_Address_Setup_Time   FSMC 地址建立时间
  * @{
  */

#define IS_FSMC_ADDRESS_SETUP_TIME(TIME) ((TIME) <= 0xF)

/**
  * @}
  */

/** @defgroup FSMC_Address_Hold_Time   FSMC 地址保持时间
  * @{
  */

#define IS_FSMC_ADDRESS_HOLD_TIME(TIME) ((TIME) <= 0xF)

/**
  * @}
  */

/** @defgroup FSMC_Data_Setup_Time   FSMC 数据建立时间
  * @{
  */

#define IS_FSMC_DATASETUP_TIME(TIME) (((TIME) > 0) && ((TIME) <= 0xFF))

/**
  * @}
  */

/** @defgroup FSMC_Bus_Turn_around_Duration   FSMC 总线周转持续时间
  * @{
  */

#define IS_FSMC_TURNAROUND_TIME(TIME) ((TIME) <= 0xF)

/**
  * @}
  */

/** @defgroup FSMC_CLK_Division   FSMC CLK 分频
  * @{
  */

#define IS_FSMC_CLK_DIV(DIV) ((DIV) <= 0xF)

/**
  * @}
  */

/** @defgroup FSMC_Data_Latency   FSMC 数据等待周期
  * @{
  */

#define IS_FSMC_DATA_LATENCY(LATENCY) ((LATENCY) <= 0xF)

/**
  * @}
  */

/** @defgroup FSMC_Access_Mode   FSMC 访问模式
  * @{
  */

#define FSMC_AccessMode_A                               ((uint32_t)0x00000000)
#define FSMC_AccessMode_B                               ((uint32_t)0x10000000) 
#define FSMC_AccessMode_C                               ((uint32_t)0x20000000)
#define FSMC_AccessMode_D                               ((uint32_t)0x30000000)
#define IS_FSMC_ACCESS_MODE(MODE) (((MODE) == FSMC_AccessMode_A) || \
                                   ((MODE) == FSMC_AccessMode_B) || \
                                   ((MODE) == FSMC_AccessMode_C) || \
                                   ((MODE) == FSMC_AccessMode_D)) 

/**
  * @}
  */

/**
  * @}
  */
  
/** @defgroup NAND_PCCARD_Controller   NAND/PCCARD 控制器
  * @{
  */

/** @defgroup FSMC_Wait_feature   FSMC 等待功能
  * @{
  */

#define FSMC_Waitfeature_Disable                        ((uint32_t)0x00000000)
#define FSMC_Waitfeature_Enable                         ((uint32_t)0x00000002)
#define IS_FSMC_WAIT_FEATURE(FEATURE) (((FEATURE) == FSMC_Waitfeature_Disable) || \
                                       ((FEATURE) == FSMC_Waitfeature_Enable))

/**
  * @}
  */


/** @defgroup FSMC_ECC   FSMC ECC
  * @{
  */

#define FSMC_ECC_Disable                                ((uint32_t)0x00000000)
#define FSMC_ECC_Enable                                 ((uint32_t)0x00000040)
#define IS_FSMC_ECC_STATE(STATE) (((STATE) == FSMC_ECC_Disable) || \
                                  ((STATE) == FSMC_ECC_Enable))

/**
  * @}
  */

/** @defgroup FSMC_ECC_Page_Size   FSMC ECC 页大小
  * @{
  */

#define FSMC_ECCPageSize_256Bytes                       ((uint32_t)0x00000000)
#define FSMC_ECCPageSize_512Bytes                       ((uint32_t)0x00020000)
#define FSMC_ECCPageSize_1024Bytes                      ((uint32_t)0x00040000)
#define FSMC_ECCPageSize_2048Bytes                      ((uint32_t)0x00060000)
#define FSMC_ECCPageSize_4096Bytes                      ((uint32_t)0x00080000)
#define FSMC_ECCPageSize_8192Bytes                      ((uint32_t)0x000A0000)
#define IS_FSMC_ECCPAGE_SIZE(SIZE) (((SIZE) == FSMC_ECCPageSize_256Bytes) || \
                                    ((SIZE) == FSMC_ECCPageSize_512Bytes) || \
                                    ((SIZE) == FSMC_ECCPageSize_1024Bytes) || \
                                    ((SIZE) == FSMC_ECCPageSize_2048Bytes) || \
                                    ((SIZE) == FSMC_ECCPageSize_4096Bytes) || \
                                    ((SIZE) == FSMC_ECCPageSize_8192Bytes))

/**
  * @}
  */

/** @defgroup FSMC_TCLR_Setup_Time   FSMC TCLR 建立时间
  * @{
  */

#define IS_FSMC_TCLR_TIME(TIME) ((TIME) <= 0xFF)

/**
  * @}
  */

/** @defgroup FSMC_TAR_Setup_Time   FSMC TAR 建立时间
  * @{
  */

#define IS_FSMC_TAR_TIME(TIME) ((TIME) <= 0xFF)

/**
  * @}
  */

/** @defgroup FSMC_Setup_Time   FSMC 建立时间
  * @{
  */

#define IS_FSMC_SETUP_TIME(TIME) ((TIME) <= 0xFF)

/**
  * @}
  */

/** @defgroup FSMC_Wait_Setup_Time   FSMC 等待建立时间
  * @{
  */

#define IS_FSMC_WAIT_TIME(TIME) ((TIME) <= 0xFF)

/**
  * @}
  */

/** @defgroup FSMC_Hold_Setup_Time   FSMC 保持建立时间
  * @{
  */

#define IS_FSMC_HOLD_TIME(TIME) ((TIME) <= 0xFF)

/**
  * @}
  */

/** @defgroup FSMC_HiZ_Setup_Time   FSMC 高阻态建立时间
  * @{
  */

#define IS_FSMC_HIZ_TIME(TIME) ((TIME) <= 0xFF)

/**
  * @}
  */

/** @defgroup FSMC_Interrupt_sources   FSMC 中断源
  * @{
  */

#define FSMC_IT_RisingEdge                              ((uint32_t)0x00000008)
#define FSMC_IT_Level                                   ((uint32_t)0x00000010)
#define FSMC_IT_FallingEdge                             ((uint32_t)0x00000020)
#define IS_FSMC_IT(IT) ((((IT) & (uint32_t)0xFFFFFFC7) == 0x00000000) && ((IT) != 0x00000000))
#define IS_FSMC_GET_IT(IT) (((IT) == FSMC_IT_RisingEdge) || \
                            ((IT) == FSMC_IT_Level) || \
                            ((IT) == FSMC_IT_FallingEdge)) 
/**
  * @}
  */

/** @defgroup FSMC_Flags   FSMC 标志
  * @{
  */

#define FSMC_FLAG_RisingEdge                            ((uint32_t)0x00000001)
#define FSMC_FLAG_Level                                 ((uint32_t)0x00000002)
#define FSMC_FLAG_FallingEdge                           ((uint32_t)0x00000004)
#define FSMC_FLAG_FEMPT                                 ((uint32_t)0x00000040)
#define IS_FSMC_GET_FLAG(FLAG) (((FLAG) == FSMC_FLAG_RisingEdge) || \
                                ((FLAG) == FSMC_FLAG_Level) || \
                                ((FLAG) == FSMC_FLAG_FallingEdge) || \
                                ((FLAG) == FSMC_FLAG_FEMPT))

#define IS_FSMC_CLEAR_FLAG(FLAG) ((((FLAG) & (uint32_t)0xFFFFFFF8) == 0x00000000) && ((FLAG) != 0x00000000))

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup FSMC_Exported_Macros   FSMC 导出宏
  * @{
  */

/**
  * @}
  */

/** @defgroup FSMC_Exported_Functions   FSMC 导出函数
  * @{
  */

void FSMC_NORSRAMDeInit(uint32_t FSMC_Bank);
void FSMC_NANDDeInit(uint32_t FSMC_Bank);
void FSMC_PCCARDDeInit(void);
void FSMC_NORSRAMInit(FSMC_NORSRAMInitTypeDef* FSMC_NORSRAMInitStruct);
void FSMC_NANDInit(FSMC_NANDInitTypeDef* FSMC_NANDInitStruct);
void FSMC_PCCARDInit(FSMC_PCCARDInitTypeDef* FSMC_PCCARDInitStruct);
void FSMC_NORSRAMStructInit(FSMC_NORSRAMInitTypeDef* FSMC_NORSRAMInitStruct);
void FSMC_NANDStructInit(FSMC_NANDInitTypeDef* FSMC_NANDInitStruct);
void FSMC_PCCARDStructInit(FSMC_PCCARDInitTypeDef* FSMC_PCCARDInitStruct);
void FSMC_NORSRAMCmd(uint32_t FSMC_Bank, FunctionalState NewState);
void FSMC_NANDCmd(uint32_t FSMC_Bank, FunctionalState NewState);
void FSMC_PCCARDCmd(FunctionalState NewState);
void FSMC_NANDECCCmd(uint32_t FSMC_Bank, FunctionalState NewState);
uint32_t FSMC_GetECC(uint32_t FSMC_Bank);
void FSMC_ITConfig(uint32_t FSMC_Bank, uint32_t FSMC_IT, FunctionalState NewState);
FlagStatus FSMC_GetFlagStatus(uint32_t FSMC_Bank, uint32_t FSMC_FLAG);
void FSMC_ClearFlag(uint32_t FSMC_Bank, uint32_t FSMC_FLAG);
ITStatus FSMC_GetITStatus(uint32_t FSMC_Bank, uint32_t FSMC_IT);
void FSMC_ClearITPendingBit(uint32_t FSMC_Bank, uint32_t FSMC_IT);

#ifdef __cplusplus
}
#endif

#endif /*__STM32F10x_FSMC_H */
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
