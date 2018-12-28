/**
  ******************************************************************************
  * @file    bsp_debug_usart.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   �ض���c��printf������usart�˿�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "./usart/bsp_debug_usart.h"

 /**
  * @brief  DEBUG_USART GPIO ����,����ģʽ���á�115200 8-N-1
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART1_IRQ;
  /* �������ȼ�Ϊ1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ�Ϊ1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

void Debug_USART_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd( DEBUG_USART1_RX_GPIO_CLK|DEBUG_USART1_TX_GPIO_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(DEBUG_USART1_CLK, ENABLE); /* ʹ�� UART ʱ�� */
  
/**********************  �������GPIO���� **********************/ 
  GPIO_PinAFConfig(DEBUG_USART1_RX_GPIO_PORT,DEBUG_USART1_RX_SOURCE, DEBUG_USART1_RX_AF);/* �������ž��帴�ù���Ϊ����1��������*/
  GPIO_PinAFConfig(DEBUG_USART1_TX_GPIO_PORT,DEBUG_USART1_TX_SOURCE,DEBUG_USART1_TX_AF); /* �������ž��帴�ù���Ϊ����1��������*/

  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;/* ����Tx����Ϊ���ù���  */
  GPIO_InitStructure.GPIO_Pin = DEBUG_USART1_TX_PIN;
  GPIO_Init(DEBUG_USART1_TX_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;/* ����Rx����Ϊ���ù��� */
  GPIO_InitStructure.GPIO_Pin = DEBUG_USART1_RX_PIN;
  GPIO_Init(DEBUG_USART1_RX_GPIO_PORT, &GPIO_InitStructure);

/*************************  ��������  ***************************/ 			
	USART_InitStructure.USART_BaudRate = DEBUG_USART1_BAUDRATE;
  /* ���������ã�DEBUG_USART_BAUDRATE */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  /* �ֳ�(����λ+У��λ)��8 */
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  /* ֹͣλ��1��ֹͣλ */
  USART_InitStructure.USART_Parity = USART_Parity_No ;
 /* У��λѡ�񣺲�ʹ��У�� */
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  /* Ӳ�������ƣ���ʹ��Ӳ���� */
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* USARTģʽ���ƣ�ͬʱʹ�ܽ��պͷ��� */
	USART_Init(DEBUG_USART1, &USART_InitStructure);  
  /* ���USART��ʼ������ */
	
	NVIC_Configuration();/* Ƕ�������жϿ�����NVIC���� */
	/* ʹ�ܴ��ڽ����ж� */
	USART_ITConfig(DEBUG_USART1, USART_IT_RXNE, ENABLE);
  /* ʹ�ܴ��� */
  USART_Cmd(DEBUG_USART1, ENABLE);
}

/*****************  ����һ���ַ� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/*****************  �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* �ȴ�������� */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/*****************  ����һ��16λ�� **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* ȡ���߰�λ */
	temp_h = (ch&0XFF00)>>8;
	/* ȡ���Ͱ�λ */
	temp_l = ch&0XFF;
	
	/* ���͸߰�λ */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* ���͵Ͱ�λ */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

///�ض���c�⺯��printf������DEBUG_USART���ض�����ʹ��printf����
//��Դ���DEBUG_USART1 
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�����DEBUG_USART1 */
		USART_SendData(DEBUG_USART1, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(DEBUG_USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf������DEBUG_USART1����д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(DEBUG_USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USART1);
}
/*********************************************END OF FILE**********************/
