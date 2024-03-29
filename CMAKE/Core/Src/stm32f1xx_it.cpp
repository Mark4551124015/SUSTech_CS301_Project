/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32f1xx_it.c
 * @brief   Interrupt Service Routines.
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
#include "stm32f1xx_it.h"

#include <cstdio>

#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "led.h"
#include "scene.h"
#include "string.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
extern string RX_DATA;
unsigned char rxBuffer[20];
// extern LED leddev;
bool rx_flag = false;
extern u8 RmtSta;
extern u16 Dval;    // 下降沿时计数器的�?
extern u32 RmtRec;  // 红外接收到的数据
extern u8 RmtCnt;   // 按键按下的次�?

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c1;
extern RTC_HandleTypeDef hrtc;
extern DMA_HandleTypeDef hdma_sdio;
extern SD_HandleTypeDef hsd;
extern TIM_HandleTypeDef htim3;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern UART_HandleTypeDef huart1;
/* USER CODE BEGIN EV */
extern chat_scene_main chat_sc;
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void) {
    /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

    /* USER CODE END NonMaskableInt_IRQn 0 */
    /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
    while (1) {
    }
    /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void) {
    /* USER CODE BEGIN HardFault_IRQn 0 */

    /* USER CODE END HardFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_HardFault_IRQn 0 */
        /* USER CODE END W1_HardFault_IRQn 0 */
    }
}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void) {
    /* USER CODE BEGIN MemoryManagement_IRQn 0 */

    /* USER CODE END MemoryManagement_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
        /* USER CODE END W1_MemoryManagement_IRQn 0 */
    }
}

/**
 * @brief This function handles Prefetch fault, memory access fault.
 */
void BusFault_Handler(void) {
    /* USER CODE BEGIN BusFault_IRQn 0 */

    /* USER CODE END BusFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_BusFault_IRQn 0 */
        /* USER CODE END W1_BusFault_IRQn 0 */
    }
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void) {
    /* USER CODE BEGIN UsageFault_IRQn 0 */

    /* USER CODE END UsageFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
        /* USER CODE END W1_UsageFault_IRQn 0 */
    }
}

/**
 * @brief This function handles System service call via SWI instruction.
 */
void SVC_Handler(void) {
    /* USER CODE BEGIN SVCall_IRQn 0 */

    /* USER CODE END SVCall_IRQn 0 */
    /* USER CODE BEGIN SVCall_IRQn 1 */

    /* USER CODE END SVCall_IRQn 1 */
}

/**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler(void) {
    /* USER CODE BEGIN DebugMonitor_IRQn 0 */

    /* USER CODE END DebugMonitor_IRQn 0 */
    /* USER CODE BEGIN DebugMonitor_IRQn 1 */

    /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
 * @brief This function handles Pendable request for system service.
 */
void PendSV_Handler(void) {
    /* USER CODE BEGIN PendSV_IRQn 0 */

    /* USER CODE END PendSV_IRQn 0 */
    /* USER CODE BEGIN PendSV_IRQn 1 */

    /* USER CODE END PendSV_IRQn 1 */
}

/**
 * @brief This function handles System tick timer.
 */
void SysTick_Handler(void) {
    /* USER CODE BEGIN SysTick_IRQn 0 */

    /* USER CODE END SysTick_IRQn 0 */
    HAL_IncTick();
    /* USER CODE BEGIN SysTick_IRQn 1 */

    /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
 * @brief This function handles RTC global interrupt.
 */
void RTC_IRQHandler(void) {
    /* USER CODE BEGIN RTC_IRQn 0 */

    /* USER CODE END RTC_IRQn 0 */
    HAL_RTCEx_RTCIRQHandler(&hrtc);
    /* USER CODE BEGIN RTC_IRQn 1 */

    /* USER CODE END RTC_IRQn 1 */
}

/**
 * @brief This function handles EXTI line0 interrupt.
 */
void EXTI0_IRQHandler(void) {
    /* USER CODE BEGIN EXTI0_IRQn 0 */

    /* USER CODE END EXTI0_IRQn 0 */
    HAL_GPIO_EXTI_IRQHandler(KEY_WK_Pin);
    /* USER CODE BEGIN EXTI0_IRQn 1 */

    /* USER CODE END EXTI0_IRQn 1 */
}

/**
 * @brief This function handles DMA1 channel4 global interrupt.
 */
void DMA1_Channel4_IRQHandler(void) {
    /* USER CODE BEGIN DMA1_Channel4_IRQn 0 */

    /* USER CODE END DMA1_Channel4_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_usart1_tx);
    /* USER CODE BEGIN DMA1_Channel4_IRQn 1 */

    /* USER CODE END DMA1_Channel4_IRQn 1 */
}

/**
 * @brief This function handles DMA1 channel5 global interrupt.
 */
void DMA1_Channel5_IRQHandler(void) {
    /* USER CODE BEGIN DMA1_Channel5_IRQn 0 */

    /* USER CODE END DMA1_Channel5_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_usart1_rx);
    /* USER CODE BEGIN DMA1_Channel5_IRQn 1 */

    /* USER CODE END DMA1_Channel5_IRQn 1 */
}

/**
 * @brief This function handles EXTI line[9:5] interrupts.
 */
void EXTI9_5_IRQHandler(void) {
    /* USER CODE BEGIN EXTI9_5_IRQn 0 */

    /* USER CODE END EXTI9_5_IRQn 0 */
    HAL_GPIO_EXTI_IRQHandler(KEY0_Pin);
    /* USER CODE BEGIN EXTI9_5_IRQn 1 */

    /* USER CODE END EXTI9_5_IRQn 1 */
}

/**
 * @brief This function handles TIM3 global interrupt.
 */
void TIM3_IRQHandler(void) {
    /* USER CODE BEGIN TIM3_IRQn 0 */

    /* USER CODE END TIM3_IRQn 0 */
    HAL_TIM_IRQHandler(&htim3);
    /* USER CODE BEGIN TIM3_IRQn 1 */

    /* USER CODE END TIM3_IRQn 1 */
}

/**
 * @brief This function handles I2C1 event interrupt.
 */
void I2C1_EV_IRQHandler(void) {
    /* USER CODE BEGIN I2C1_EV_IRQn 0 */

    /* USER CODE END I2C1_EV_IRQn 0 */
    HAL_I2C_EV_IRQHandler(&hi2c1);
    /* USER CODE BEGIN I2C1_EV_IRQn 1 */

    /* USER CODE END I2C1_EV_IRQn 1 */
}

/**
 * @brief This function handles I2C1 error interrupt.
 */
void I2C1_ER_IRQHandler(void) {
    /* USER CODE BEGIN I2C1_ER_IRQn 0 */

    /* USER CODE END I2C1_ER_IRQn 0 */
    HAL_I2C_ER_IRQHandler(&hi2c1);
    /* USER CODE BEGIN I2C1_ER_IRQn 1 */

    /* USER CODE END I2C1_ER_IRQn 1 */
}

/**
 * @brief This function handles USART1 global interrupt.
 */
void USART1_IRQHandler(void) {
    /* USER CODE BEGIN USART1_IRQn 0 */

    /* USER CODE END USART1_IRQn 0 */
    HAL_UART_IRQHandler(&huart1);
    /* USER CODE BEGIN USART1_IRQn 1 */
    HAL_UART_Receive_IT(&huart1, (uint8_t *)rxBuffer, 1);
    /* USER CODE END USART1_IRQn 1 */
}

/**
 * @brief This function handles SDIO global interrupt.
 */
void SDIO_IRQHandler(void) {
    /* USER CODE BEGIN SDIO_IRQn 0 */
    /* USER CODE END SDIO_IRQn 0 */
    HAL_SD_IRQHandler(&hsd);
    /* USER CODE BEGIN SDIO_IRQn 1 */

    /* USER CODE END SDIO_IRQn 1 */
}

/**
 * @brief This function handles DMA2 channel4 and channel5 global interrupts.
 */
void DMA2_Channel4_5_IRQHandler(void) {
    /* USER CODE BEGIN DMA2_Channel4_5_IRQn 0 */

    /* USER CODE END DMA2_Channel4_5_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_sdio);
    /* USER CODE BEGIN DMA2_Channel4_5_IRQn 1 */

    /* USER CODE END DMA2_Channel4_5_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        // static char uLength = 0;
        if (rxBuffer[0] == '\n') {
            // uLength = 0;
            int str_len = RX_DATA.length();
            RX_DATA.erase(str_len - 1);
            rx_flag = 1;
            printf("%s", RX_DATA.c_str());
        } else {
            RX_DATA += rxBuffer[0];
            // uLength++;
        }
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM3) {
        // leddev.tick();
    }
    if (htim->Instance == TIM5) {
        if (RmtSta & 0x80)  // 上次有数据被接收到了
        {
            RmtSta &= ~0X10;  // 取消上升沿已经被捕获标记
            if ((RmtSta & 0X0F) == 0X00)
                RmtSta |= 1 << 6;  // 标记已经完成�?次按键的键�?�信息采�?
            if ((RmtSta & 0X0F) < 14)
                RmtSta++;
            else {
                RmtSta &= ~(1 << 7);  // 清空引导标识
                RmtSta &= 0XF0;       // 清空计数�?
            }
        }
    }
}
//  void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
// {
// 	HAL_Delay(500);

// 	POINT_COLOR = RED;
//     char output[50];
// 	switch (GPIO_Pin) {
// 	case KEY0_Pin:
// 			HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
// 			 sprintf(output, "KEY0 Pressed");
// 			HAL_UART_Transmit(&huart1, output, strlen(output),
// 0xffff); 		break; 	case KEY1_Pin:
//             sprintf(output, "KEY1 Pressed");
// 			HAL_UART_Transmit(&huart1, output, strlen(output),
// 0xffff); 		break; 	case KEY_WK_Pin:
// HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
// HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
// HAL_Delay(100); 			HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
// 			HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
// 			HAL_Delay(100);
// 			HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
// 			HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
// 			HAL_Delay(100);
// 			HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
//			HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
// 		break;
// 	default:
// 		break;

// 	}
// }
/* USER CODE END 1 */
