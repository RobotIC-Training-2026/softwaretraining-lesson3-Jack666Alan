#ifndef __SOFT_UART_H
#define __SOFT_UART_H

#include "main.h" // 包含STM32HAL库头文件

// 用户自定义的GPIO和引脚定义，根据CubeMX中的配置修改
#define SOFT_UART_TX_PORT GPIOA
#define SOFT_UART_TX_PIN GPIO_PIN_9

#define SOFT_UART_RX_PORT GPIOA
#define SOFT_UART_RX_PIN GPIO_PIN_10

// 定义波特率，这里使用9600
#define SOFT_UART_BAUDRATE 9600

// 计算一位的延时时间（微秒）。SystemCoreClock是系统主频，我们设为72MHz。
#define BIT_DELAY_US (1000000 / SOFT_UART_BAUDRATE) // 约104us
// 注意：我们使用HAL_Delay()的微秒延时版本，它依赖Systick。

// 函数声明
void SoftUART_Init(void);
void SoftUART_Transmit(uint8_t data);
uint8_t SoftUART_Receive(void);
void SoftUART_DelayUs(uint16_t us); // 一个简单的微秒级延时函数

#endif
