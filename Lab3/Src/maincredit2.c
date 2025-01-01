/**
  ******************************************************************************
  * @file    BSP/Src/main.c 
  * @author  MCD Application Team
  * @brief   Main program body
  ******************************************************************************

  * @brief  Main program
  * @param  None
  * @retval None
  */
  
#include "main.h"

// Data structure for LED
typedef struct {
    GPIO_TypeDef *port; // GPIO port
    uint16_t pin;       // GPIO pin
} LED_t;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
void LED_Init(LED_t *led);
void LED_On(LED_t *led);
void LED_Off(LED_t *led);
void LED_Toggle(LED_t *led);

int main(void)
{
    /* STM32L4xx HAL library initialization */
    HAL_Init();

    /* Configure the System clock to have a frequency of 80 MHz */
    SystemClock_Config();

    /* Define LED structures */
    LED_t LED1 = {GPIOA, GPIO_PIN_5};  // LED1 on GPIO Port A, Pin 5
    LED_t LED2 = {GPIOB, GPIO_PIN_14}; // LED2 on GPIO Port B, Pin 14

    /* Initialize both LEDs */
    LED_Init(&LED1);
    LED_Init(&LED2);

    /* Set the initial state of LEDs */
    LED_On(&LED1);   // Start with LED1 ON
    LED_Off(&LED2);  // Start with LED2 OFF

    /* Main loop toggling LED1 and LED2 alternately */
    while (1)
    {
        LED_Toggle(&LED1); // Toggle LED1
        LED_Toggle(&LED2); // Toggle LED2
        HAL_Delay(2000);   // Wait for 2 seconds
    }
}

/* Generic function to initialize an LED */
void LED_Init(LED_t *led)
{
    GPIO_InitTypeDef gpio_init_structure;

    /* Enable the clock for the LED's port */
    if (led->port == GPIOA) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    } else if (led->port == GPIOB) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    }

    gpio_init_structure.Pin = led->pin;
    gpio_init_structure.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_structure.Pull = GPIO_NOPULL;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(led->port, &gpio_init_structure);
}

/* Generic function to turn an LED on */
void LED_On(LED_t *led)
{
    HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_SET);
}

/* Generic function to turn an LED off */
void LED_Off(LED_t *led)
{
    HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_RESET);
}

/* Generic function to toggle an LED */
void LED_Toggle(LED_t *led)
{
    HAL_GPIO_TogglePin(led->port, led->pin);
}

/* System clock configuration */
static void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    /* MSI is enabled after System reset, activate PLL with MSI as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
    RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 40;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV4;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        /* Initialization Error */
        while (1);
    }

    /* Select PLL as system clock source and configure the HCLK, PCLK1, and PCLK2 clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
    {
        /* Initialization Error */
        while (1);
    }
}

