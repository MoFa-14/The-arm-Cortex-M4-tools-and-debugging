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

// GPIO pin definitions for the wakeup button (B2)
#define BLUE_BUTTON_PIN                  GPIO_PIN_13
#define BLUE_BUTTON_GPIO_PORT            GPIOC
#define BLUE_BUTTON_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()
#define BLUE_BUTTON_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOC_CLK_DISABLE()

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
void LED2_Init(void);
void Blue_PB_Init(void);

int main(void)
{
    /* STM32L4xx HAL library initialization */
    HAL_Init();

    /* Configure the System clock to have a frequency of 80 MHz */
    SystemClock_Config();

    /* Initialize LED2 and the blue button */
    LED2_Init();
    Blue_PB_Init();

    /* Variable to track LED2 state */
    uint32_t greenledval = 0;

    /* Main loop */
    while (1)
    {
        /* Check if the blue button is pressed */
        if (HAL_GPIO_ReadPin(BLUE_BUTTON_GPIO_PORT, BLUE_BUTTON_PIN) == GPIO_PIN_RESET)
        {
            /* Toggle LED2 state */
            greenledval = 1 - greenledval;

            if (greenledval)
                HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_SET); // Turn LED2 ON
            else
                HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_RESET); // Turn LED2 OFF

            /* Wait for the button to be released (debouncing) */
            while (HAL_GPIO_ReadPin(BLUE_BUTTON_GPIO_PORT, BLUE_BUTTON_PIN) == GPIO_PIN_RESET);
        }
    }
}

/* System clock configuration */
static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Configure the main internal regulator output voltage */
    HAL_PWR_EnableBkUpAccess();
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Initializes the CPU, AHB, and APB busses clocks */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;  // 4 MHz
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 40;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        /* Initialization Error */
        while (1);
    }

    /* Initializes the CPU, AHB, and APB clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
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

    gpio_init_structure.Pin = LED2_PIN;
    gpio_init_structure.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_structure.Pull = GPIO_NOPULL;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(LED2_GPIO_PORT, &gpio_init_structure);
}

/* Initialize the blue button */
void Blue_PB_Init(void)
{
    GPIO_InitTypeDef gpio_init_structure;

    BLUE_BUTTON_GPIO_CLK_ENABLE(); // Enable the button clock

    gpio_init_structure.Pin = BLUE_BUTTON_PIN;
    gpio_init_structure.Mode = GPIO_MODE_INPUT;
    gpio_init_structure.Pull = GPIO_PULLUP;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(BLUE_BUTTON_GPIO_PORT, &gpio_init_structure);
}

