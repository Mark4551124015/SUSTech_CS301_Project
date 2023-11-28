#include "led.h"
#include <stdio.h>

#include "main.h"

#define WAKE GPIO_PIN_RESET
#define DEAD GPIO_PIN_SET

LED::LED() {
    this->counter = 0;
    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, DEAD);
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, DEAD);
}

void LED::Init() {
    this->counter = 0;
    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, DEAD);
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, DEAD);

}
void LED::append(led_op operand) {
    switch (operand) {
        case BLINK_0:
            this->op.push(WAKE0);
            this->op.push(KILL0);
            break;
        case BLINK_1:
            this->op.push(WAKE1);
            this->op.push(KILL1);
            break;
        case BLINK_BOTH_TWICE:
            this->op.push(WAKEALL);
            this->op.push(KILLALL);
            this->op.push(WAKEALL);
            this->op.push(KILLALL);
            this->op.push(WAKEALL);
            this->op.push(KILLALL);
            break;
        case ERROR_ENCODE:
            this->op.push(WAKEALL);
            this->op.push(KILL0);
            this->op.push(WAKE0);
            this->op.push(KILL0);
            this->op.push(WAKE0);
            this->op.push(KILLALL);
            break;
        case ERROR_DECODE:
            this->op.push(WAKEALL);
            this->op.push(KILL1);
            this->op.push(WAKE1);
            this->op.push(KILL1);
            this->op.push(WAKE1);
            this->op.push(KILLALL);
            break;
        default:
            break;
    }
}
void LED::tick() {
    if (this->op.empty()) return;
    if (this->counter < TICK_MS) {
        this->counter++;
        return;
    } 
    switch (this->op.front()) {
        case WAKE0:
            HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, WAKE);
            break;
        case KILL0:
            HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, DEAD);
            break;
        case WAKE1:
            HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, WAKE);
            break;
        case KILL1:
            HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, DEAD);
            break;
        case WAKEALL:
            HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, WAKE);
            HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, WAKE);
            break;
        case KILLALL:
            HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, DEAD);
            HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, DEAD);
            break;
        default:
            break;
    }
    this->counter = 0;
    this->op.pop();
}