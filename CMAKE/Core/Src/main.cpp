/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.cpp
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

// #include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>

#include <cstdint>
#include <cstdio>
#include <string>
#include <utility>

#include "24l01.h"
#include "framework.h"
#include "led.h"
#include "mmc_sd.h"
#include "piclib.h"
#include "scene.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MAIN 1
#define CALC 2
#define CHAT_SELECT 3
#define CHAT_SCENE 4
#define EMOJI_SCENE 5
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

SD_HandleTypeDef hsd;
DMA_HandleTypeDef hdma_sdio;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM3_Init(void);
static void MX_SDIO_SD_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
string RX_DATA;
char uLength = 0;
extern unsigned char rxBuffer[1024];
extern bool rx_flag;
pii touch;
bool button_click[8];
// LED leddev = LED();  // led_dev
char *tmp = new char[1];
vtext *choosed;
uint8_t EVENT[8];
bool fly = false;
string users[3] = {(char *)"User0", (char *)"User1", (char *)""};
int SCENE = MAIN;
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */

int main(void) {
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU
     * Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the
     * Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */
    NRF24L01_Init();
    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */
    delay_ms(100);
    // MX_I2C1_Init();

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_USART1_UART_Init();
    MX_I2C1_Init();
    MX_TIM3_Init();
    // MX_SDIO_SD_Init();
    // MX_FATFS_Init();
    MX_SPI1_Init();
    /* USER CODE BEGIN 2 */
    //   printf("LCD shape %u %u", lcddev.height, lcddev.width);
    // leddev.Init();
    LCD_Init();
    // piclib_init();
    LCD_Clear(WHITE);
    HAL_TIM_Base_Start_IT(&htim3);

    tp_dev.init();
    // leddev.append(BLINK_BOTH);
    // leddev.append(BLINK_BOTH);

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    dpo canvas = dpo("canvas", {lcddev.width / 2, lcddev.height / 2},
                     {lcddev.width, lcddev.height});
    printf("here");

    printf("Done Creating\n");
    bar *bottom_bar = new bar("bar1", {0, 140}, {lcddev.width, 40});
    dpo *window_view = new dpo("window_view", {0, -20}, {lcddev.width, 280});
    // calc_main *cal_sc = new calc_main("calc_main", {0, 0}, {lcddev.width,
    // 280});

    // chat_select_main *chat_sel_sc =
    //     new chat_select_main("chat_select_main", {0, 0}, {lcddev.width,
    //     280});
    // chat_scene_main *chat_sc = new chat_scene_main("chat_scene_main", {0, 0},
    //                                            {lcddev.width, 280}, users);
    // canvas.add_son(&m);

    // emoji_scene_main *emoji_sc =
    //     new emoji_scene_main("emoji_scene_main", {0, 0}, {lcddev.width,
    //     280});
    calc_main *cal_sc = nullptr;
    chat_select_main *chat_sel_sc = nullptr;
    emoji_scene_main *emoji_sc = nullptr;
    chat_scene_main *chat_sc = nullptr;

    canvas.add_son(bottom_bar);
    canvas.add_son(window_view);
    // window_view.add_son(&cal_sc);
    // cal_sc.setVisbility(false);
    printf("Start\n");
    // window_view.add_son(&emoji_sc);
    // emoji_sc.setVisbility(false);
    // window_view.add_son(&chat_sc);
    // chat_sc.setVisbility(true);

    HAL_UART_Receive_IT(&huart1, (uint8_t *)rxBuffer, 1);
    u32 total, free;

    mem_init();
    exfuns_init();
    SD_Init();                // 检测不到SD卡
    f_mount(fs[0], "0:", 1);  // 挂载SD卡
    f_mount(fs[1], "1:", 1);  // 挂载FLASH.

    while (1) {
        // LCD_ShowString(2, 2, 160, 16, 16, (uint8_t *)"Mem");
        tp_dev.scan(0);
        touch = {(int)tp_dev.x[0], (int)tp_dev.y[0]};
        fly = equal_pii(touch, {65535, 65535});
        canvas.update(nullptr, {0, 0});

        POINT_COLOR = BLUE;  // 设置字体为蓝色
        // LCD_ShowString(30, 150, 200, 16, 16, (uint8_t *)"FATFS OK!");
        // LCD_ShowString(30, 170, 200, 16, 16,
        //                (uint8_t *)"SD Total Size:     MB");
        // LCD_ShowString(30, 190, 200, 16, 16,
        //                (uint8_t *)"SD  Free Size:     MB");

        // exf_getfree((uint8_t *)"0:", &total, &free);
        // LCD_ShowNum(30 + 8 * 14, 170, total >> 10, 5, 16);  // 显示SD卡总容量 MB
        // LCD_ShowNum(30 + 8 * 14, 190, free >> 10, 5,
        //             16);  // 显示SD卡剩余容量 MB
        
        if (!fly)
            HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
        else
            HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
        if (EVENT[RETURN_BACK]) {
            printf("[EVENT] Press Back\n");
            SCENE = CHAT_SCENE;
            EVENT[RETURN_BACK] = 0;
        }
        if (EVENT[RETURN_HOME]) {
            printf("[EVENT] Press Home\n");
            SCENE = CALC;
            EVENT[RETURN_HOME] = 0;
        }
        if (EVENT[EMOJI_SELECT]) {
            printf("[EVENT] Press Emoji\n");

            // chat_sc->setVisbility(false);
            // emoji_sc->setVisbility(true);
            SCENE = EMOJI_SCENE;
            EVENT[EMOJI_SELECT] = 0;
            //canvas.need_render = true;
        }
        if (EVENT[EMOJI_SELECTED]) {
            printf("[EVENT] Emoji Selected\n");
            // emoji_sc->setVisbility(false);
            // chat_sc->setVisbility(true);
            SCENE = CHAT_SCENE;
            //chat_sc->addImageToPage(emoji_sc->emoji_num, 1);
            printf("emoji name: %d\n", emoji_sc->emoji_num);
            EVENT[EMOJI_SELECTED] = 0;
            canvas.need_render = true;
        }

        if (SCENE == CALC) {
            LCD_Clear(WHITE);
            canvas.need_render = true;
            if (chat_sc != nullptr) delete (chat_sc), chat_sc = nullptr;
            if (emoji_sc != nullptr) delete (emoji_sc), emoji_sc = nullptr;
            if (chat_sel_sc != nullptr)
                delete (chat_sel_sc), chat_sel_sc = nullptr;
            if (cal_sc == nullptr) {
                cal_sc =
                    new calc_main("calc_main", {0, 0}, {lcddev.width, 280});
                window_view->sub_object_cnt = 0;
                window_view->add_son(cal_sc);
                cal_sc->setVisbility(true);
            }
            SCENE = 0;
        }

        // if (SCENE == CHAT_SELECT) {
        //     if (chat_sc!=nullptr) delete (chat_sc), chat_sc = nullptr;
        //     if (emoji_sc!=nullptr) delete (emoji_sc), emoji_sc = nullptr;
        //     if (cal_sc!=nullptr) delete (cal_sc), cal_sc = nullptr;
        //     if (chat_sel_sc==nullptr) chat_sel_sc = new
        //     chat_select_main("chat_select_main", {0, 0}, {lcddev.width,
        //     280}); window_view->add_son(chat_sel_sc);
        //     chat_sel_sc->setVisbility(true);
        // }

        if (SCENE == EMOJI_SCENE) {
            LCD_Clear(WHITE);
            canvas.need_render = true;
            if (chat_sel_sc != nullptr)
                delete (chat_sel_sc), chat_sel_sc = nullptr;
            if (chat_sc != nullptr) 
                delete (chat_sc), chat_sc = nullptr;
            if (cal_sc != nullptr) delete (cal_sc), cal_sc = nullptr;
            if (emoji_sc == nullptr) {
                emoji_sc = new emoji_scene_main("emoji_scene_main", {0, 0},
                                                {lcddev.width, 280});
                window_view->sub_object_cnt = 0;
                window_view->add_son(emoji_sc);
            }
            SCENE = 0;
        }

        if (SCENE == CHAT_SCENE) {
            LCD_Clear(WHITE);
            canvas.need_render = true;
            int emoji_num = 0;
            if (chat_sel_sc != nullptr)
                delete (chat_sel_sc), chat_sel_sc = nullptr;
            if (emoji_sc != nullptr) {
                emoji_num = emoji_sc->emoji_num;
                delete (emoji_sc), emoji_sc = nullptr;
            }
            if (cal_sc != nullptr) delete (cal_sc), cal_sc = nullptr;
            if (chat_sc == nullptr) {
                chat_sc = new chat_scene_main("chat_scene_main", {0, 0},
                                              {lcddev.width, 280}, users);
                window_view->sub_object_cnt = 0;
                window_view->add_son(chat_sc);
                printf("Done creating\n");
            }
            chat_sc->setVisbility(true);
            printf("emoji_num:%d\n", emoji_num);
            if(emoji_num)
                chat_sc->addImageToPage(emoji_num, 1);
            SCENE = 0;
        }
        if (rx_flag && chat_sc != nullptr) {
            printf("executed");
            chat_sc->addMessageToPage(RX_DATA, 1);
            rx_flag = 0;
        }

        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {
    /* USER CODE BEGIN I2C1_Init 0 */

    /* USER CODE END I2C1_Init 0 */

    /* USER CODE BEGIN I2C1_Init 1 */

    /* USER CODE END I2C1_Init 1 */
    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN I2C1_Init 2 */

    /* USER CODE END I2C1_Init 2 */
}

/**
 * @brief SDIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_SDIO_SD_Init(void) {
    /* USER CODE BEGIN SDIO_Init 0 */

    /* USER CODE END SDIO_Init 0 */

    /* USER CODE BEGIN SDIO_Init 1 */

    /* USER CODE END SDIO_Init 1 */
    hsd.Instance = SDIO;
    hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
    hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
    hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
    hsd.Init.BusWide = SDIO_BUS_WIDE_4B;
    hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
    hsd.Init.ClockDiv = 36;
    /* USER CODE BEGIN SDIO_Init 2 */

    /* USER CODE END SDIO_Init 2 */
}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void) {
    /* USER CODE BEGIN SPI1_Init 0 */

    /* USER CODE END SPI1_Init 0 */

    /* USER CODE BEGIN SPI1_Init 1 */

    /* USER CODE END SPI1_Init 1 */
    /* SPI1 parameter configuration*/
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_ENABLE;
    hspi1.Init.CRCPolynomial = 7;
    if (HAL_SPI_Init(&hspi1) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN SPI1_Init 2 */

    /* USER CODE END SPI1_Init 2 */
}

/**
 * @brief TIM3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM3_Init(void) {
    /* USER CODE BEGIN TIM3_Init 0 */

    /* USER CODE END TIM3_Init 0 */

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* USER CODE BEGIN TIM3_Init 1 */

    /* USER CODE END TIM3_Init 1 */
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 7199;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 9;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim3) != HAL_OK) {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK) {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) !=
        HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM3_Init 2 */

    /* USER CODE END TIM3_Init 2 */
}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void) {
    /* USER CODE BEGIN USART1_Init 0 */

    /* USER CODE END USART1_Init 0 */

    /* USER CODE BEGIN USART1_Init 1 */

    /* USER CODE END USART1_Init 1 */
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN USART1_Init 2 */

    /* USER CODE END USART1_Init 2 */
}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void) {
    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();
    __HAL_RCC_DMA2_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA1_Channel4_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
    /* DMA1_Channel5_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
    /* DMA2_Channel4_5_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Channel4_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Channel4_5_IRQn);
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* USER CODE BEGIN MX_GPIO_Init_1 */
    /* USER CODE END MX_GPIO_Init_1 */

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, CS_Pin | CE_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin : IRQ_Pin */
    GPIO_InitStruct.Pin = IRQ_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(IRQ_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : KEY_WK_Pin */
    GPIO_InitStruct.Pin = KEY_WK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(KEY_WK_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : CS_Pin CE_Pin */
    GPIO_InitStruct.Pin = CS_Pin | CE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : PA3 */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : KEY0_Pin */
    GPIO_InitStruct.Pin = KEY0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY0_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : LED0_Pin */
    GPIO_InitStruct.Pin = LED0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED0_GPIO_Port, &GPIO_InitStruct);

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    /* USER CODE BEGIN MX_GPIO_Init_2 */
    /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

PUTCHAR_PROTOTYPE {
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state
     */
    __disable_irq();
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line) {
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line
       number, ex: printf("Wrong parameters value: file %s on line %d\r\n",
       file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
