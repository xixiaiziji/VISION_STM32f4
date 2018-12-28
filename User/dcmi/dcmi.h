#ifndef _DCMI_H
#define _DCMI_H
#include "stm32f4xx.h"									
extern void jpeg_data_process(void);	//JPEG数据处理函数	
extern u8 ov_frame;  						//帧率

void My_DCMI_Init(void);
void DCMI_DMA_Init(u32 DMA_Memory0BaseAddr,u16 DMA_BufferSize,u32 DMA_MemoryDataSize,u32 DMA_MemoryInc);
void DCMI_Start(void);
void DCMI_Stop(void);

void DCMI_Set_Window(u16 sx,u16 sy,u16 width,u16 height);
void DCMI_CR_Set(u8 pclk,u8 hsync,u8 vsync);

#endif

