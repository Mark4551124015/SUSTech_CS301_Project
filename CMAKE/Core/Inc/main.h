/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "myiic.h"
#include "stdio.h"
#include "touch.h"


/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
// EVENT
#define RETURN_BACK 0
#define RETURN_HOME 1
#define SHUTDOWN 2
#define EMOJI_SELECT 3
#define EMOJI_SELECTED 4
#define CREAT_CHAT 5
#define CHAT_SELECTED 6
#define CALC_SELECTED 7
#define PHOTO_SELECTED 8
#define REMOTE_KEY 9
#define SLIDE_LEFT 10
#define SLIDE_RIGHT 11
#define MESSAGE_VIEWD 12


//SCENE
#define NONE_SCENE 0
#define MAIN 1
#define CALC 2
#define CHAT_SELECT 3
#define CHAT_SCENE 4
#define ALBUM_SCENE 5

#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
#define PAout(n) BIT_ADDR(GPIOA_ODR_Addr, n)  // ���??
#define PAin(n) BIT_ADDR(GPIOA_IDR_Addr, n)   // ����

#define PBout(n) BIT_ADDR(GPIOB_ODR_Addr, n)  // ���??
#define PBin(n) BIT_ADDR(GPIOB_IDR_Addr, n)   // ����

#define PCout(n) BIT_ADDR(GPIOC_ODR_Addr, n)  // ���??
#define PCin(n) BIT_ADDR(GPIOC_IDR_Addr, n)   // ����

#define PDout(n) BIT_ADDR(GPIOD_ODR_Addr, n)  // ���??
#define PDin(n) BIT_ADDR(GPIOD_IDR_Addr, n)   // ����

#define PEout(n) BIT_ADDR(GPIOE_ODR_Addr, n)  // ���??
#define PEin(n) BIT_ADDR(GPIOE_IDR_Addr, n)   // ����

#define PFout(n) BIT_ADDR(GPIOF_ODR_Addr, n)  // ���??
#define PFin(n) BIT_ADDR(GPIOF_IDR_Addr, n)   // ����

#define PGout(n) BIT_ADDR(GPIOG_ODR_Addr, n)  // ���??
#define PGin(n) BIT_ADDR(GPIOG_IDR_Addr, n)   // ����

#define BITBAND(addr, bitnum) \
    ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define MEM_ADDR(addr) *((volatile unsigned long *)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))
// IO口地�??映射
// #define GPIOC_ODR_Addr (GPIOC_BASE + 12)      // 0x4001100C
// #define GPIOC_IDR_Addr (GPIOC_BASE + 8)       // 0x40011008




#define PCout(n) BIT_ADDR(GPIOC_ODR_Addr, n)  // 输出
#define PCin(n) BIT_ADDR(GPIOC_IDR_Addr, n)   // 输入
#define LCD_LED PCout(10)                     // LCD背光    	PC10

#define LCD_CS_SET GPIOC->BSRR = 1 << 9  // 片�?�端�??  		PC9
#define LCD_RS_SET GPIOC->BSRR = 1 << 8  // 数据/命令 		PC8
#define LCD_WR_SET GPIOC->BSRR = 1 << 7  // 写数�??			PC7
#define LCD_RD_SET GPIOC->BSRR = 1 << 6  // 读数�??			PC6

#define LCD_CS_CLR GPIOC->BRR = 1 << 9  // 片�?�端�??  		PC9
#define LCD_RS_CLR GPIOC->BRR = 1 << 8  // 数据/命令		PC8
#define LCD_WR_CLR GPIOC->BRR = 1 << 7  // 写数�??			PC7
#define LCD_RD_CLR GPIOC->BRR = 1 << 6  // 读数�??			PC6

// PB0~15,作为数据�??
#define DATAOUT(x) GPIOB->ODR = x;  // 数据输出
#define DATAIN GPIOB->IDR;          // 数据输入
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE;

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define IRQ_Pin GPIO_PIN_0
#define IRQ_GPIO_Port GPIOC
#define KEY_WK_Pin GPIO_PIN_0
#define KEY_WK_GPIO_Port GPIOA
#define KEY_WK_EXTI_IRQn EXTI0_IRQn
#define CS_Pin GPIO_PIN_1
#define CS_GPIO_Port GPIOA
#define KEY0_Pin GPIO_PIN_5
#define KEY0_GPIO_Port GPIOC
#define KEY0_EXTI_IRQn EXTI9_5_IRQn
#define LED0_Pin GPIO_PIN_8
#define LED0_GPIO_Port GPIOA
#define CE_Pin GPIO_PIN_15
#define CE_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
