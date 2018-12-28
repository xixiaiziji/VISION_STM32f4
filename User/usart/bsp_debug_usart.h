#ifndef __DEBUG_USART_H
#define	__DEBUG_USART_H

#include "stm32f4xx.h"
#include <stdio.h>

//引脚定义
//#define USART1_DR_Base  0x40013804		// 0x40013800 + 0x04 = 0x40013804
//#define SENDBUFF_SIZE   5000
/************************串口1************************************/
#define DEBUG_USART1                             USART1
#define DEBUG_USART1_CLK                         RCC_APB2Periph_USART1
#define DEBUG_USART1_BAUDRATE                    115200

#define DEBUG_USART1_RX_GPIO_PORT                GPIOA
#define DEBUG_USART1_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define DEBUG_USART1_RX_PIN                      GPIO_Pin_10
#define DEBUG_USART1_RX_AF                       GPIO_AF_USART1
#define DEBUG_USART1_RX_SOURCE                   GPIO_PinSource10

#define DEBUG_USART1_TX_GPIO_PORT                GPIOA
#define DEBUG_USART1_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define DEBUG_USART1_TX_PIN                      GPIO_Pin_9
#define DEBUG_USART1_TX_AF                       GPIO_AF_USART1
#define DEBUG_USART1_TX_SOURCE                   GPIO_PinSource9

#define DEBUG_USART1_IRQHandler                  USART1_IRQHandler
#define DEBUG_USART1_IRQ                 				 USART1_IRQn

/**************************串口2**********************************/
#define DEBUG_USART2                             USART2
#define DEBUG_USART2_CLK                         RCC_APB1Periph_USART2
#define DEBUG_USART2_CLOCKCMD                    RCC_APB1PeriphClockCmd
#define DEBUG_USART2_BAUDRATE                    115200  //串口波特率

#define DEBUG_USART2_RX_GPIO_PORT                GPIOD
#define DEBUG_USART2_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOD
#define DEBUG_USART2_RX_PIN                      GPIO_Pin_6
#define DEBUG_USART2_RX_AF                       GPIO_AF_USART2
#define DEBUG_USART2_RX_SOURCE                   GPIO_PinSource6

#define DEBUG_USART2_TX_GPIO_PORT                GPIOD
#define DEBUG_USART2_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOD
#define DEBUG_USART2_TX_PIN                      GPIO_Pin_5
#define DEBUG_USART2_TX_AF                       GPIO_AF_USART2
#define DEBUG_USART2_TX_SOURCE                   GPIO_PinSource5

#define DEBUG_USART2_IRQHandler                  USART2_IRQHandler
#define DEBUG_USART2_IRQ                 				 USART2_IRQn

/************************************************************************/

void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);

void Debug_USART_Config(void);
//int fputc(int ch, FILE *f);
//int fgetc(FILE *f);
#endif /* __USART_H */
