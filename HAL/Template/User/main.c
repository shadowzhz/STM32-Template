#include "stm32f1xx_hal.h"
#include "stm32f1xx_it.h"

/* 用 LL 时直接包含需要的 LL 头文件, LL 大部分函数在头文件里内联 */
/* #include "stm32f1xx_ll_bus.h" */
/* #include "stm32f1xx_ll_gpio.h" */
/* #include "stm32f1xx_ll_utils.h" */

#define LED_Pin       GPIO_PIN_13
#define LED_GPIO_Port GPIOC

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void Error_Handler(void);

int main(void)
{
    HAL_Init();

    SystemClock_Config();

    MX_GPIO_Init();

    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);  // LED灭(低电平点亮)

    while (1)
    {
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        HAL_Delay(500);
    }
}

/**
  * @brief  系统时钟: HSE(8MHz) -> PLLx9 -> SYSCLK 72MHz
  *         AHB=72MHz, APB1=36MHz(max), APB2=72MHz, ADCCLK=12MHz(<=14MHz)
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL     = RCC_PLL_MUL9;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK   | RCC_CLOCKTYPE_SYSCLK
                                     | RCC_CLOCKTYPE_PCLK1  | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOC_CLK_ENABLE();

    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

    GPIO_InitStruct.Pin   = LED_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

    /* LL 等价写法(取消注释并屏蔽上面的 HAL 初始化即可体验):
     *
     * LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
     * LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_13, LL_GPIO_MODE_OUTPUT);
     * LL_GPIO_SetPinOutputType(GPIOC, LL_GPIO_PIN_13, LL_GPIO_OUTPUT_PUSHPULL);
     * LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_13, LL_GPIO_SPEED_FREQ_HIGH);
     *
     * 主循环里延时用: LL_mDelay(500);
     * 翻转用:        LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
     *
     * 注意: LL_mDelay 依赖全局变量 SystemCoreClock,
     *       时钟配置完成后调用一次 LL_SetSystemCoreClock(72000000);
     */
}

/**
  * @brief  出错兜底: 关中断死循环, 方便断点定位
  */
void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    (void)file;
    (void)line;
}
#endif /* USE_FULL_ASSERT */
