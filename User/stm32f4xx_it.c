/**
  ******************************************************************************
  * @file    FMC_SDRAM/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Main Interrupt Service Routines.
  *         This file provides template for all exceptions handler and
  *         peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "./camera/bsp_ov2640.h"
#include "./systick/bsp_SysTick.h"
#include "./key/bsp_exti.h"
#include "sdio/bsp_sdio_sd.h"
#include "./usart/bsp_debug_usart.h"

extern unsigned int Task_Delay[];

/** @addtogroup STM32F429I_DISCOVERY_Examples
  * @{
  */

/** @addtogroup FMC_SDRAM
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{}

	
extern uint32_t Task_Delay[];
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	uint8_t i;
	
  TimingDelay_Decrement();
	
	for(i=0;i<NumOfTask;i++)
	{
		if(Task_Delay[i])
		{
			Task_Delay[i]--;
		}
	}
}
void USART1_IRQHandler(void)//将接收的数据再发回去
{
  uint8_t ucTemp;
	if(USART_GetITStatus(DEBUG_USART1,USART_IT_RXNE)!=RESET)
	{		
		ucTemp = USART_ReceiveData( DEBUG_USART1 );
    USART_SendData(DEBUG_USART1,ucTemp);    
	}	 
}	
void USART2_IRQHandler(void)
{
  uint8_t ucTemp;
	if(USART_GetITStatus(DEBUG_USART2,USART_IT_RXNE)!=RESET)
	{		
		ucTemp = USART_ReceiveData( DEBUG_USART2 );
    USART_SendData(DEBUG_USART2,ucTemp);    
	}	 
}	

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f429_439xx.s).                         */
/******************************************************************************/



//记录传输了多少行
static uint16_t line_num =0;
uint16_t image_flag=0,frame_flag=0,proess_ok=0;

void DMA2_Stream1_IRQHandler(void)
{
  if( DMA_GetITStatus(DMA2_Stream1,DMA_IT_TCIF1) == SET )    
  {
		
    line_num++; /*行计数*/
    if(line_num==img_height)
		{
			line_num=0;	/*传输完一帧,计数复位*/
		}	
		/*DMA 一行一行传输*/
   	OV2640_DMAtolcd_Config(FSMC_LCD_ADDRESS+(lcd_height-line_num-1)*(lcd_width)*2,img_width*2/4);
		DMA_ClearITPendingBit(DMA2_Stream1,DMA_IT_TCIF1); /* 清除DMA数据流传输完成标志位 */
		//DCMI_CaptureCmd(ENABLE); 
	}
		
}
void DMA2_Stream0_IRQHandler(void)
{
   if( DMA_GetITStatus(DMA2_Stream0,DMA_IT_TCIF0) == SET )    
  {
		image_flag++;
		if(image_flag==img_height)
		{
			image_flag=1;
		}	
		
		DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_TCIF0); /* 清除DMA数据流传输完成标志位 */
	}
}

//使用帧中断重置line_num,可防止有时掉数据的时候DMA传送行数出现偏移
void DCMI_IRQHandler(void)
{ 
	if(  DCMI_GetITStatus (DCMI_IT_FRAME) == SET )    
	{
		frame_flag=1;
		fps++; //帧率计数
		/*传输完一帧，计数复位*/
	  line_num=0;
		DCMI_ClearITPendingBit(DCMI_IT_FRAME); 
		
	}
}

void KEY2_IRQHandler(void)
{
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET) 
	{
		flag=1;

    //清除中断标志位
		EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);     
	}  
}

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
