#include "soft_uart.h"

// 微秒延时函数
void SoftUART_DelayUs(uint16_t us)
{
    __IO uint32_t cycles = (SystemCoreClock / 1000000L) * us;
    __IO uint32_t start = DWT->CYCCNT;
    while ((DWT->CYCCNT - start) < cycles);
}

// 初始化函数
void SoftUART_Init(void)
{
    // 启用DWT单元用于精确延时
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    
    // 配置RX引脚为上拉输入（在CubeMX中已配置，这里可省略）
}

// 接收函数（核心）
uint8_t SoftUART_Receive(void)
{
    uint8_t data = 0;
    
    // 1. 等待起始位（检测到低电平）
    while (HAL_GPIO_ReadPin(SOFT_UART_RX_PORT, SOFT_UART_RX_PIN) == GPIO_PIN_SET);
    
    // 2. 延时到数据位中间进行采样，提高准确性
    SoftUART_DelayUs(BIT_DELAY_US + (BIT_DELAY_US / 2));
    
    // 3. 采样8个数据位（LSB first）
    for (int i = 0; i < 8; i++)
    {
        data >>= 1;  // 先右移
        if (HAL_GPIO_ReadPin(SOFT_UART_RX_PORT, SOFT_UART_RX_PIN) == GPIO_PIN_SET)
        {
            data |= 0x80;  // 收到高电平，设置最高位
        }
        SoftUART_DelayUs(BIT_DELAY_US);  // 等待下一位
    }
    
    // 4. 跳过停止位
    SoftUART_DelayUs(BIT_DELAY_US);
    
    return data;
}
