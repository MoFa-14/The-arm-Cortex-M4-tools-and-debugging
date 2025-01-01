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

// GPIO pin definitions for LED2 (Green LED)
#define LED2_PIN                         GPIO_PIN_14
#define LED2_GPIO_PORT                   GPIOB
#define LED2_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOB_CLK_DISABLE()

// GPIO pin definitions for LED1 (Blue LED)
#define LED1_PIN                         GPIO_PIN_5
#define LED1_GPIO_PORT                   GPIOA
#define LED1_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()
#define LED1_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
void LED2_Init(void);
void LED2_On(void);
void LED2_Off(void);
void LED2_DeInit(void);
void LED2_Toggle(void);

void LED1_Init(void);
void LED1_On(void);
void LED1_Off(void);
void LED1_DeInit(void);
void LED1_Toggle(void);

int main(void)
{
    /* STM32L4xx HAL library initialization */
    HAL_Init();

    /* Configure the System clock to have a frequency of 80 MHz */
    SystemClock_Config();

    /* Initialize both LEDs */
    LED1_Init();
    LED2_Init();

    /* Set the initial state of LEDs */
    LED1_On();   // Start with LED1 ON
    LED2_Off();  // Start with LED2 OFF

    /* Main loop toggling LED1 and LED2 alternately */
    while (1)
    {
        LED1_Toggle();   // Toggle LED1 state
        LED2_Toggle();   // Toggle LED2 state
        HAL_Delay(2000); // Wait for 2 seconds
    }
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
    RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLP = 7;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        /* Initialization Error */
        while (1);
    }

    /* Select PLL as system clock source and configure the HCLK, PCLK1, and PCLK2 clocks */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
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

/* Initialize LED2 */
void LED2_Init(void)
{
    GPIO_InitTypeDef gpio_init_structure;
  
    LED2_GPIO_CLK_ENABLE(); // Enable the clock for GPIO Port B

    gpio_init_structure.Pin   = LED2_PIN;
    gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init_structure.Pull  = GPIO_NOPULL;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(LED2_GPIO_PORT, &gpio_init_structure);
}

/* De-initialize LED2 */
void LED2_DeInit(void)
{
    GPIO_InitTypeDef gpio_init_structure;

    gpio_init_structure.Pin = LED2_PIN;

    HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_RESET); // Turn off LED2
    HAL_GPIO_DeInit(LED2_GPIO_PORT, gpio_init_structure.Pin);    // De-initialize GPIO
}

/* Turn LED2 on */
void LED2_On(void)
{
    HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_SET);
}

/* Turn LED2 off */
void LED2_Off(void)
{
    HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_RESET);
}

/* Toggle LED2 */
void LED2_Toggle(void)
{
    HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
}

/* Initialize LED1 */
void LED1_Init(void)
{
    GPIO_InitTypeDef gpio_init_structure;

    LED1_GPIO_CLK_ENABLE(); // Enable the clock for GPIO Port A

    gpio_init_structure.Pin   = LED1_PIN;
    gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init_structure.Pull  = GPIO_NOPULL;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(LED1_GPIO_PORT, &gpio_init_structure);
}

/* De-initialize LED1 */
void LED1_DeInit(void)
{
    GPIO_InitTypeDef gpio_init_structure;

    gpio_init_structure.Pin = LED1_PIN;

    HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_RESET); // Turn off LED1
    HAL_GPIO_DeInit(LED1_GPIO_PORT, gpio_init_structure.Pin);    // De-initialize GPIO
}

/* Turn LED1 on */
void LED1_On(void)
{
    HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_SET);
}

/* Turn LED1 off */
void LED1_Off(void)
{
    HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_RESET);
}

/* Toggle LED1 */
void LED1_Toggle(void)
{
    HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);
}

