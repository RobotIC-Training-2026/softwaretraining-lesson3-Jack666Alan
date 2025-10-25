#ifndef __SOFT_UART_H
#define __SOFT_UART_H

#include "main.h"

// 根据实际接线修改引脚定义
#define SOFT_UART_RX_PORT GPIOA
#define SOFT_UART_RX_PIN  GPIO_PIN_10

#define SOFT_UART_BAUDRATE 9600
#define BIT_DELAY_US (1000000 / SOFT_UART_BAUDRATE)

// 函数声明
void SoftUART_Init(void);
uint8_t SoftUART_Receive(void);
void SoftUART_DelayUs(uint16_t us);

#endif
