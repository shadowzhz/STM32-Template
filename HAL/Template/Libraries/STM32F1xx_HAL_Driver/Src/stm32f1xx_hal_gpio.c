/**
  ******************************************************************************
  * @file    stm32f1xx_hal_gpio.c
  * @author  MCD Application Team
  * @brief   HAL库 GPIO模块驱动
  *          本文件提供固件函数，用于管理通用输入/输出(GPIO)外设的以下功能：
  *           + 初始化和反初始化函数
  *           + IO 操作函数
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
  @verbatim
  ==============================================================================
                    ##### GPIO 外设特性 #####
  ==============================================================================
  [..]
  根据数据手册中列出的每个I/O端口的特定硬件特性，
  通用输入/输出(GPIO)端口的每个端口位都可以被软件以几种模式进行单独配置:
  (+) 输入模式
  (+) 模拟模式
  (+) 输出模式
  (+) 复用模式
  (+) 外部中断/事件线

  [..]
  在复位期间和复位后，复用功能和外部中断线处于不激活状态，I/O 端口配置为浮空输入模式。

  [..]
  所有 GPIO 引脚都具有弱内部上拉和下拉电阻，可以激活或禁用。

  [..]
  在输出或复用模式下，每个 IO 可以配置为开漏或推挽类型，并且可以根据 VDD 值选择 IO 速度。

  [..]
  所有端口都具有外部中断/事件功能。要使用外部中断线，必须将端口配置为输入模式。
  所有可用的 GPIO 引脚都连接到 EXTI0 到 EXTI15 的 16 个外部中断/事件线。

  [..]
  外部中断/事件控制器由最多20个边沿检测器组成，在某些系列芯片中，或由19个边沿检测器组成，用于生成事件/中断请求。
  每个输入线都可以独立配置以选择类型(事件或中断)和相应的触发事件(上升沿或下降沿或双沿)。
  每条线路也可以独立屏蔽。
  一个挂起寄存器指示中断请求的状态行。

                     ##### 如何使用驱动 #####
  ==============================================================================
 [..]
   (#) 使用函数 __HAL_RCC_GPIOx_CLK_ENABLE() 启用GPIO APB2时钟: 。

   (#) 使用HAL_GPIO_Init()函数配置GPIO引脚。
       (++) 使用GPIO_InitTypeDef结构中的"Mode"成员配置IO模式。
       (++) 使用GPIO_InitTypeDef结构中的"Pull"成员激活上拉或下拉电阻。
       (++) 在选择输出或复用功能模式时，通过GPIO_InitTypeDef结构中的"Speed"成员配置IO速度。
       (++) 当一个引脚要用作ADC通道或DAC输出时，需要配置为模拟模式。
       (++) 在选择外部中断/事件时，使用GPIO_InitTypeDef结构中的"Mode"成员选择类型（中断或事件）和相应的触发事件（上升沿、下降沿或双沿）。

   (#) 在选择外部中断/事件模式时，使用HAL_NVIC_SetPriority()函数配置与EXTI线关联的NVIC IRQ优先级，
       并使用HAL_NVIC_EnableIRQ()函数使能它。

   (#) 若要获取配置为输入模式的引脚的电平，请使用HAL_GPIO_ReadPin()函数。

   (#) 若要设置/复位配置为输出模式的引脚的电平，请使用HAL_GPIO_WritePin()/HAL_GPIO_TogglePin()函数。

   (#) 若要锁定引脚配置直到下次复位，请使用HAL_GPIO_LockPin()函数。

   (#) 在复位期间和复位后，复用功能不处于活动状态，GPIO引脚配置为输入浮空模式（除了JTAG引脚）。

   (#) 当LSE振荡器关闭时，LSE振荡器引脚OSC32_IN和OSC32_OUT可以用作通用引脚(PC14和PC15)。 LSE优先于GPIO功能（也就是这两个引脚默认是LSE的引脚）。

   (#) 当HSE振荡器关闭时，HSE振荡器引脚OSC_IN/OSC_OUT可以用作通用引脚PD0和PD1。 HSE优先于GPIO功能（也就是这两个引脚默认是HSE的引脚）。

  @endverbatim
  ******************************************************************************
  */

/* 头文件 ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @defgroup GPIO GPIO
  * @brief GPIO HAL 模块驱动
  * @{
  */

#ifdef HAL_GPIO_MODULE_ENABLED

/* 私有类型 -----------------------------------------------------------*/
/* 私有定义 ------------------------------------------------------------*/
/** @addtogroup GPIO_Private_Constants GPIO 私有常量
  * @{
  */
#define GPIO_MODE             0x00000003u   /* GPIO模式 */
#define EXTI_MODE             0x10000000u   /* EXTI模式 */
#define GPIO_MODE_IT          0x00010000u   /* GPIO中断模式 */
#define GPIO_MODE_EVT         0x00020000u   /* GPIO事件模式 */
#define RISING_EDGE           0x00100000u   /* 上升沿 */
#define FALLING_EDGE          0x00200000u   /* 下降沿 */
#define GPIO_OUTPUT_TYPE      0x00000010u   /* GPIO输出类型 */

#define GPIO_NUMBER           16u   /* GPIO数量 */

/* CRL和CRH寄存器的位操作定义 */
#define  GPIO_CR_MODE_INPUT         0x00000000u /*!< 00: 输入模式 (复位状态)  */
#define  GPIO_CR_CNF_ANALOG         0x00000000u /*!< 00: 模拟模式  */
#define  GPIO_CR_CNF_INPUT_FLOATING 0x00000004u /*!< 01: 浮空输入模式 (复位状态)  */
#define  GPIO_CR_CNF_INPUT_PU_PD    0x00000008u /*!< 10: 上拉输入 / 下拉输入  */
#define  GPIO_CR_CNF_GP_OUTPUT_PP   0x00000000u /*!< 00: 通用推挽输出  */
#define  GPIO_CR_CNF_GP_OUTPUT_OD   0x00000004u /*!< 01: 通用开漏输出  */
#define  GPIO_CR_CNF_AF_OUTPUT_PP   0x00000008u /*!< 10: 复用推挽输出  */
#define  GPIO_CR_CNF_AF_OUTPUT_OD   0x0000000Cu /*!< 11: 复用开漏输出  */

/**
  * @}
  */
/* 私有宏 -------------------------------------------------------------*/
/* 私有变量 ---------------------------------------------------------*/
/* 私有函数原型 -----------------------------------------------*/
/* 私有函数 ---------------------------------------------------------*/
/* 导出函数 --------------------------------------------------------*/
/** @defgroup GPIO_Exported_Functions GPIO 导出函数
  * @{
  */

/** @defgroup GPIO_Exported_Functions_Group1 初始化和反初始化函数
 *  @brief    初始化和反初始化函数
 *
@verbatim
 ===============================================================================
              ##### 初始化和反初始化函数 #####
 ===============================================================================
  [..]
    本节提供的函数允许初始化和反初始化GPIO以准备使用。

@endverbatim
  * @{
  */


/**
  * @brief  GPIO初始化 根据GPIO_Init参数中指定的参数初始化GPIOx外设
  * @param  GPIOx: 其中x可以是（根据使用的设备）A..G中的一个，以选择GPIO外设。
  * @param  GPIO_Init: 指向GPIO_InitTypeDef结构的指针，该结构包含指定GPIO外设的配置信息。
  * @retval None
  */
void HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
  uint32_t position = 0x00u;
  uint32_t ioposition;
  uint32_t iocurrent;
  uint32_t temp;
  uint32_t config = 0x00u;
  __IO uint32_t *configregister; /* 根据引脚编号存储CRL或CRH寄存器的地址 */
  uint32_t registeroffset;       /* 计算CNF和MODE位在CRL或CRH寄存器内的放置位置时使用的偏移量 */

  /* 检查参数 */
  assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Init->Pin));
  assert_param(IS_GPIO_MODE(GPIO_Init->Mode));

  /* 配置端口引脚 */
  while (((GPIO_Init->Pin) >> position) != 0x00u)
  {
    /* 获取 IO 配置 */
    ioposition = (0x01uL << position);

    /* 获取当前 IO 配置 */
    iocurrent = (uint32_t)(GPIO_Init->Pin) & ioposition;

    if (iocurrent == ioposition)
    {
      /* 检查复用功能参数 */
      assert_param(IS_GPIO_AF_INSTANCE(GPIOx));

      /* 根据所需模式，使用MODEy [1：0]和CNFy [3：2]对应位填充配置变量 */
      switch (GPIO_Init->Mode)
      {
        /* 如果配置的引脚为推挽输出模式 */
        case GPIO_MODE_OUTPUT_PP:
          /* 检查GPIO速度参数 */
          assert_param(IS_GPIO_SPEED(GPIO_Init->Speed));
          config = GPIO_Init->Speed + GPIO_CR_CNF_GP_OUTPUT_PP;
          break;

        /* 如果配置的引脚为开漏输出模式 */
        case GPIO_MODE_OUTPUT_OD:
          /* 检查GPIO速度参数 */
          assert_param(IS_GPIO_SPEED(GPIO_Init->Speed));
          config = GPIO_Init->Speed + GPIO_CR_CNF_GP_OUTPUT_OD;
          break;

        /* 如果配置的引脚为复用推挽输出模式 */
        case GPIO_MODE_AF_PP:
          /* 检查GPIO速度参数 */
          assert_param(IS_GPIO_SPEED(GPIO_Init->Speed));
          config = GPIO_Init->Speed + GPIO_CR_CNF_AF_OUTPUT_PP;
          break;

        /* 如果配置的引脚为复用开漏输出模式 */
        case GPIO_MODE_AF_OD:
          /* 检查GPIO速度参数 */
          assert_param(IS_GPIO_SPEED(GPIO_Init->Speed));
          config = GPIO_Init->Speed + GPIO_CR_CNF_AF_OUTPUT_OD;
          break;

        /* 如果配置的引脚为输入模式（同时适用于中断/事件模式） */
        case GPIO_MODE_INPUT:
        case GPIO_MODE_IT_RISING:
        case GPIO_MODE_IT_FALLING:
        case GPIO_MODE_IT_RISING_FALLING:
        case GPIO_MODE_EVT_RISING:
        case GPIO_MODE_EVT_FALLING:
        case GPIO_MODE_EVT_RISING_FALLING:
          /* 检查GPIO上下拉参数 */
          assert_param(IS_GPIO_PULL(GPIO_Init->Pull));
          if (GPIO_Init->Pull == GPIO_NOPULL) /* 无上下拉 */
          {
            config = GPIO_CR_MODE_INPUT + GPIO_CR_CNF_INPUT_FLOATING;
          }
          else if (GPIO_Init->Pull == GPIO_PULLUP)  /* 上拉 */
          {
            config = GPIO_CR_MODE_INPUT + GPIO_CR_CNF_INPUT_PU_PD;

            /* 设置相应的ODR（端口输出寄存器）位（置对应引脚为1） */
            GPIOx->BSRR = ioposition;
          }
          else /* 下拉 */
          {
            config = GPIO_CR_MODE_INPUT + GPIO_CR_CNF_INPUT_PU_PD;

            /* 复位相应的ODR（端口输出寄存器）位（置对应引脚为0） */
            GPIOx->BRR = ioposition;
          }
          break;

        /* 如果配置引脚为模拟输入模式 */
        case GPIO_MODE_ANALOG:
          config = GPIO_CR_MODE_INPUT + GPIO_CR_CNF_ANALOG;
          break;

        /* 使用assert_param检查参数 */
        default:
          break;
      }

      /* 检查当前位是属于引脚计数的前半部分还是后半部分，以便寻址CRH或CRL寄存器 */
      configregister = (iocurrent < GPIO_PIN_8) ? &GPIOx->CRL     : &GPIOx->CRH;
      registeroffset = (iocurrent < GPIO_PIN_8) ? (position << 2u) : ((position - 8u) << 2u);

      /* 将引脚的新配置应用于寄存器 */
      MODIFY_REG((*configregister), ((GPIO_CRL_MODE0 | GPIO_CRL_CNF0) << registeroffset), (config << registeroffset));

      /*--------------------- EXTI 模式配置 ------------------------*/
      /* 为当前IO配置外部中断或事件 */
      if ((GPIO_Init->Mode & EXTI_MODE) == EXTI_MODE)
      {
        /* 启用AFIO时钟 */
        __HAL_RCC_AFIO_CLK_ENABLE();
        temp = AFIO->EXTICR[position >> 2u];
        CLEAR_BIT(temp, (0x0Fu) << (4u * (position & 0x03u)));
        SET_BIT(temp, (GPIO_GET_INDEX(GPIOx)) << (4u * (position & 0x03u)));
        AFIO->EXTICR[position >> 2u] = temp;


        /* 启用/禁用上升沿触发 */
        if ((GPIO_Init->Mode & RISING_EDGE) == RISING_EDGE)
        {
          SET_BIT(EXTI->RTSR, iocurrent);
        }
        else
        {
          CLEAR_BIT(EXTI->RTSR, iocurrent);
        }

        /* 启用/禁用下降沿触发 */
        if ((GPIO_Init->Mode & FALLING_EDGE) == FALLING_EDGE)
        {
          SET_BIT(EXTI->FTSR, iocurrent);
        }
        else
        {
          CLEAR_BIT(EXTI->FTSR, iocurrent);
        }

        /* 配置事件掩码 */
        if ((GPIO_Init->Mode & GPIO_MODE_EVT) == GPIO_MODE_EVT)
        {
          SET_BIT(EXTI->EMR, iocurrent);
        }
        else
        {
          CLEAR_BIT(EXTI->EMR, iocurrent);
        }

        /* 配置中断掩码 */
        if ((GPIO_Init->Mode & GPIO_MODE_IT) == GPIO_MODE_IT)
        {
          SET_BIT(EXTI->IMR, iocurrent);
        }
        else
        {
          CLEAR_BIT(EXTI->IMR, iocurrent);
        }
      }
    }

	position++;
  }
}

/**
  * @brief  将GPIOx外设寄存器反初始化为其默认重置值。
  * @param  GPIOx: 其中x可以是（根据使用的设备）A..G中的一个，以选择GPIO外设。
  * @param  GPIO_Pin: 指定要写入的端口位（引脚）。
  *        该参数可以是GPIO_PIN_x之一，其中x可以是（0..15）。
  * @retval None
  */
void HAL_GPIO_DeInit(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pin)
{
  uint32_t position = 0x00u;
  uint32_t iocurrent;
  uint32_t tmp;
  __IO uint32_t *configregister; /* 根据引脚号存储CRL或CRH寄存器的地址 */
  uint32_t registeroffset;

  /* 检查参数 */
  assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));

  /* 配置端口引脚 */
  while ((GPIO_Pin >> position) != 0u)
  {
    /* 获取当前引脚位置 */
    iocurrent = (GPIO_Pin) & (1uL << position);

    if (iocurrent)/* 由于此处，故此函数每次调用只能配置一个引脚 */
    {
      /*------------------------- EXTI 模式配置 --------------------*/
      /* 清除指定IO的外部中断/事件 */

      tmp = AFIO->EXTICR[position >> 2u];
      tmp &= 0x0FuL << (4u * (position & 0x03u));
      if (tmp == (GPIO_GET_INDEX(GPIOx) << (4u * (position & 0x03u))))
      {
        /* 清除EXTI线配置 */
        CLEAR_BIT(EXTI->IMR, (uint32_t)iocurrent);
        CLEAR_BIT(EXTI->EMR, (uint32_t)iocurrent);

        /* 清除上升下降沿配置 */
        CLEAR_BIT(EXTI->FTSR, (uint32_t)iocurrent);
        CLEAR_BIT(EXTI->RTSR, (uint32_t)iocurrent);
        
        tmp = 0x0FuL << (4u * (position & 0x03u));
        CLEAR_BIT(AFIO->EXTICR[position >> 2u], tmp);
      }
      /*------------------------- GPIO 模式配置 --------------------*/
      /* 检查当前位是属于引脚计数的前半部分还是后半部分，以便寻址CRH或CRL寄存器 */
      configregister = (iocurrent < GPIO_PIN_8) ? &GPIOx->CRL     : &GPIOx->CRH;
      registeroffset = (iocurrent < GPIO_PIN_8) ? (position << 2u) : ((position - 8u) << 2u);

      /* CRL/CRH默认值为浮动输入(0x04)移位到正确位置 */
      MODIFY_REG(*configregister, ((GPIO_CRL_MODE0 | GPIO_CRL_CNF0) << registeroffset), GPIO_CRL_CNF0_0 << registeroffset);

      /* ODR（端口输出寄存器）默认值 0 */
      CLEAR_BIT(GPIOx->ODR, iocurrent);
    }

    position++;
  }
}

/**
  * @}
  */

/** @defgroup GPIO_Exported_Functions_Group2 IO 操作函数
 *  @brief   GPIO 读和写
 *
@verbatim
 ===============================================================================
                       ##### IO 操作函数 #####
 ===============================================================================
  [..]
    本小节提供了一组用于管理GPIO的功能。

@endverbatim
  * @{
  */

/**
  * @brief  读取指定的输入端口引脚电平。
  * @param  GPIOx: 其中x可以是（根据使用的设备）A..G中的一个，以选择GPIO外设。
  * @param  GPIO_Pin: 指定读取的引脚号
  *         该参数可以是GPIO_PIN_x，其中x可以是(0..15)。
  * @retval 输入端口引脚的值
  *         该值是GPIO_PinState枚举值之一:
  *            @arg GPIO_PIN_RESET: 清除引脚（置0拉低）
  *            @arg GPIO_PIN_SET: 设置引脚（置1拉高）
  */
GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  GPIO_PinState bitstatus;

  /* 检查参数 */
  assert_param(IS_GPIO_PIN(GPIO_Pin));

  if ((GPIOx->IDR & GPIO_Pin) != (uint32_t)GPIO_PIN_RESET)
  {
    bitstatus = GPIO_PIN_SET;
  }
  else
  {
    bitstatus = GPIO_PIN_RESET;
  }
  return bitstatus;
}

/**
  * @brief  设置或清除所选数据端口位电平。
  *
  * @note   这个函数使用GPIOx_BSRR寄存器来允许读/修改访问。这样在读取和修改访问之间产生IRQ时不会发生风险。
  *
  * @param  GPIOx: 其中x可以是（根据使用的设备）A..G中的一个，以选择GPIO外设。
  * @param  GPIO_Pin: 指定要写入的端口位（引脚）。
  *         该参数可以是GPIO_PIN_x之一，其中x可以是（0..15）。
  * @param  PinState: 指定要写入的值。
  *          该参数可以是GPIO_PinState枚举值之一:
  *            @arg GPIO_PIN_RESET: 清除引脚（置0拉低）
  *            @arg GPIO_PIN_SET: 设置引脚（置1拉高）
  * @retval None
  */
void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
  /* 检查参数 */
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  assert_param(IS_GPIO_PIN_ACTION(PinState));

  if (PinState != GPIO_PIN_RESET)
  {
    GPIOx->BSRR = GPIO_Pin;
  }
  else
  {
    GPIOx->BSRR = (uint32_t)GPIO_Pin << 16u;
  }
}

/**
  * @brief  翻转指定GPIO引脚的状态
  * @param  GPIOx: 其中x可以是（根据使用的设备）A..G中的一个，以选择GPIO外设。
  * @param  GPIO_Pin: 指定被翻转的引脚
  * @retval None
  */
void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  uint32_t odr;

  /* 检查参数 */
  assert_param(IS_GPIO_PIN(GPIO_Pin));

  /* 获取当前输出数据寄存器值 */
  odr = GPIOx->ODR;

  /* 设置选中的引脚翻转 */
  GPIOx->BSRR = ((odr & GPIO_Pin) << GPIO_NUMBER) | (~odr & GPIO_Pin);
}

/**
* @brief  锁定GPIO引脚配置寄存器
* @note   锁定机制允许IO配置被冻结。当锁序列被应用于端口位时，在下一次复位之前，不可再修改端口位的值。
* @param  GPIOx: 其中x可以是（根据使用的设备）A..G中的一个，以选择GPIO外设。
* @param  GPIO_Pin: 指定锁定的引脚
*         该参数可以是GPIO_Pin_x的任意组合，其中x可以是(0..15)。
* @retval None
*/
HAL_StatusTypeDef HAL_GPIO_LockPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  __IO uint32_t tmp = GPIO_LCKR_LCKK;

  /* 检查参数 */
  assert_param(IS_GPIO_LOCK_INSTANCE(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));

  /* 应用锁键值写顺序 */
  SET_BIT(tmp, GPIO_Pin);
  /* 设置 LCKx bit(s): LCKK='1' + LCK[15-0] */
  GPIOx->LCKR = tmp;
  /* 复位 LCKx bit(s): LCKK='0' + LCK[15-0] */
  GPIOx->LCKR = GPIO_Pin;
  /* 设置 LCKx bit(s): LCKK='1' + LCK[15-0] */
  GPIOx->LCKR = tmp;
  /* 读取 LCKK 寄存器. 这个读取是必需的，以完成键值锁序列 */
  tmp = GPIOx->LCKR;

  /* 再次读取以确认锁定激活 */
  if ((uint32_t)(GPIOx->LCKR & GPIO_LCKR_LCKK))
  {
    return HAL_OK;
  }
  else
  {
    return HAL_ERROR;
  }
}

/**
  * @brief  GPIO EXTI中断请求公共处理函数。
  * @param  GPIO_Pin: 指定连接的EXTI线的引脚
  * @retval None
  */
void HAL_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin)
{
  /* EXTI 线中断检测 */
  if (__HAL_GPIO_EXTI_GET_IT(GPIO_Pin) != 0x00u)
  {
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
    HAL_GPIO_EXTI_Callback(GPIO_Pin);
  }
}

/**
  * @brief  GPIO EXTI 中断回调
  * @param  GPIO_Pin: 指定连接的引脚
  * @retval None
  */
__weak void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* 防止未使用参数的编译警告 */
  UNUSED(GPIO_Pin);
  /* NOTE: 当需要回调函数时，不应修改此函数，可以在用户文件中重写以覆盖此函数。
   */
}

/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_GPIO_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

 