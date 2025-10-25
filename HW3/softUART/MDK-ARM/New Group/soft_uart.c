#include "soft_uart.h"

void SoftUART_DelayUs(uint16_t us)
{
    // 根据系统时钟频率调整这个循环次数。对于72MHz，这个值大致准确，可能需要校准。
    __IO uint32_t cycles = (SystemCoreClock / 1000000L) * us;
    __IO uint32_t start = DWT->CYCCNT;
    while ((DWT->CYCCNT - start) < cycles);
}

// 在初始化中需要启用DWT（Data Watchpoint and Trace）单元以使用CYCCNT计数器
void SoftUART_Init(void) {
    // 启用DWT单元，用于微秒延时
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}
void SoftUART_Transmit(uint8_t data)
{
    // 1. 发送起始位 (低电平)
    HAL_GPIO_WritePin(SOFT_UART_TX_PORT, SOFT_UART_TX_PIN, GPIO_PIN_RESET);
    SoftUART_DelayUs(BIT_DELAY_US);

    // 2. 依次发送8个数据位 (LSB first，即先发最低位)
    for (int i = 0; i < 8; i++)
    {
        if (data & 0x01) // 检查当前最低位
        {
            HAL_GPIO_WritePin(SOFT_UART_TX_PORT, SOFT_UART_TX_PIN, GPIO_PIN_SET); // 发1
        }
        else
        {
            HAL_GPIO_WritePin(SOFT_UART_TX_PORT, SOFT_UART_TX_PIN, GPIO_PIN_RESET); // 发0
        }
        SoftUART_DelayUs(BIT_DELAY_US);
        data >>= 1; // 右移，准备发送下一位
    }

    // 3. 发送停止位 (高电平)
    HAL_GPIO_WritePin(SOFT_UART_TX_PORT, SOFT_UART_TX_PIN, GPIO_PIN_SET);
    SoftUART_DelayUs(BIT_DELAY_US);
}
uint8_t SoftUART_Receive(void)
{
    uint8_t data = 0;

    // 1. 等待起始位（检测到低电平）
    while (HAL_GPIO_ReadPin(SOFT_UART_RX_PORT, SOFT_UART_RX_PIN) == GPIO_PIN_SET);

    // 2. 延时到一位的中间位置进行采样，以避开边沿不稳定期
    SoftUART_DelayUs(BIT_DELAY_US + (BIT_DELAY_US / 2)); // 延时1.5个位时间

    // 3. 采样8个数据位
    for (int i = 0; i < 8; i++)
    {
        data >>= 1; // 先右移，LSB first
        if (HAL_GPIO_ReadPin(SOFT_UART_RX_PORT, SOFT_UART_RX_PIN) == GPIO_PIN_SET)
        {
            data |= 0x80; // 如果读到高电平，将最高位置1
        }
        SoftUART_DelayUs(BIT_DELAY_US); // 延时一位时间，等待下一个数据位
    }

    // 4. 这里本应检测停止位，但作业要求简单，可以忽略
    // SoftUART_DelayUs(BIT_DELAY_US); // 跳过停止位

    return data;
}
