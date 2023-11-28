#ifndef __DELAY_H
#define __DELAY_H 			   
// #include "sys.h"

#include "stm32f1xx.h"
#include "stdint.h"  
typedef uint32_t  u32; 
typedef uint16_t u16;
typedef uint8_t  u8;

#ifdef __cplusplus   //使用C语言的方式编译方法名。
extern "C" {
#endif

void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#ifdef __cplusplus   //使用C语言的方式编译方法名。
}
#endif

#endif





























