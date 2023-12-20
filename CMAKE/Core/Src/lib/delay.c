#include "delay.h"


static u32 fac_us=0;							//us延时倍乘数

			   
//初始化延迟函数
//当使用ucos的时候,此函数会初始化ucos的时钟节拍
//SYSTICK的时钟固定为AHB时钟
//SYSCLK:系统时钟频率




//延时nus
//nus为要延时的us数.	
//nus:0~190887435(最大值即2^32/fac_us@fac_us=22.5)	 
void delay_us(u32 nus)
{		

}

//延时nms
//nms:要延时的ms数
void delay_ms(u16 nms)
{

}


