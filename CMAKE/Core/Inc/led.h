#ifndef __LED_H
#define __LED_H

#include "stdlib.h"
#include <queue>
// #include "font.h"
#include <stdint.h>
#include <stdio.h>


#define TICK_MS 50
#define WAKE0 0
#define KILL0 1
#define WAKE1 2
#define KILL1 3
#define WAKEALL 4
#define KILLALL 5

#ifdef __cplusplus  // 使用C语言的方式编译方法名。
extern "C" {
#endif
enum led_op { BLINK_0, BLINK_1, BLINK_BOTH, ERROR_ENCODE, ERROR_DECODE };

class LED {
   public:
    std::queue<uint8_t> op;
    int counter;

   public:
    LED();
    void tick();
    void Init();
    void append(led_op operand);
    void update_index(int *p);
};


#ifdef __cplusplus  // 使用C语言的方式编译方法名。
}
#endif
#endif
