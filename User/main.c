#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./sdram/bsp_sdram.h"
#include "./lcd/bsp_lcd.h"
#include "./camera/bsp_ov2640.h"
#include "./systick/bsp_SysTick.h"
#include "stm32f4xx_it.h"
#include "proess.h"
#include "./easytracer/easytracer.h"
#include "sdio/bsp_sdio_sd.h"
#include "./FATFS/ff.h"
#include "./key/bsp_exti.h"

static void BL8782_PDN_INIT(void)
{
  /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOG, ENABLE); 							   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
  GPIO_Init(GPIOG, &GPIO_InitStructure);	
  
  GPIO_ResetBits(GPIOG,GPIO_Pin_9);  //����WiFiģ��
}

/*���������*/
uint32_t Task_Delay[NumOfTask];
uint16_t thre;                      //��ֵ����
uint16_t ratio,center_x,center_y,cnt;
uint8_t dispBuf[100];
OV2640_IDTypeDef OV2640_Camera_ID;
uint16_t i=0,j=0,num=0,flag=0;
uint8_t fps=0;                       //֡�ʼ���
uint16_t d;                         //�������
uint16_t p[4];                    //��С��Ӿ���4����
uint16_t dig;                     //�ǶȲ���


extern uint16_t line_num;

int main(void)
{
  uint8_t file_name[20];
	uint8_t num = 0;
	uint8_t res=0;
	
	/* ����WiFiģ�� */
	BL8782_PDN_INIT();
	
	/*GPIO ����,����ģʽ���á�115200 8-N-1���жϽ���*/
  Debug_USART_Config();   
	
	/* ����SysTick Ϊ1ms�ж�һ��,ʱ�䵽�󴥷���ʱ�жϣ�
	*����stm32fxx_it.c�ļ���SysTick_Handler����ͨ�����жϴ�����ʱ
	*/
	SysTick_Init();
  
	
	/*��ʼ��Һ����*/
 
	LCD_Init();
  LCD_LayerInit();
  LTDC_Cmd(ENABLE);
	
	/*�ѱ�����ˢ��ɫ*/
 
  LCD_SetLayer(LCD_BACKGROUND_LAYER);  
	LCD_SetTransparency(0xFF);
	LCD_Clear(LCD_COLOR_WHITE);
	
  /*��ʼ����Ĭ��ʹ��ǰ����*/
	LCD_SetLayer(LCD_FOREGROUND_LAYER); 
	
	/*�ú�������Ϊ��͸���ȣ���Χ 0-0xff ��0Ϊȫ͸����0xffΪ��͸��*/
  
	LCD_SetTransparency(0xff);
	LCD_Clear(TRANSPARENCY);//����ǰ��͸��
	LCD_SetColors(LCD_COLOR_YELLOW,TRANSPARENCY);  
	LCD_ClearLine(LINE(19));
  LCD_DisplayStringLine_EN_CH(LINE(19),(uint8_t* )"UXGA:480*800");
 
  
  /* ��ʼ������ͷGPIO��IIC */
  OV2640_HW_Init();   

  /* ��ȡ����ͷоƬID��ȷ������ͷ�������� */
  OV2640_ReadID(&OV2640_Camera_ID);

   if(OV2640_Camera_ID.PIDH  == 0x26)
  {
    //sprintf((char*)dispBuf, "              OV2640 ����ͷ,ID:0x%x", OV2640_Camera_ID.PIDH);
		//LCD_DisplayStringLine_EN_CH(LINE(0),(uint8_t*)dispBuf);
    CAMERA_DEBUG("%x %x",OV2640_Camera_ID.Manufacturer_ID1 ,OV2640_Camera_ID.Manufacturer_ID2);

  }
  else
  {
    LCD_SetTextColor(LCD_COLOR_RED);
    LCD_DisplayStringLine_EN_CH(LINE(0),(uint8_t*) "         û�м�⵽OV2640�������¼�����ӡ�");
    CAMERA_DEBUG("û�м�⵽OV2640����ͷ�������¼�����ӡ�");

    while(1);  
  }

	 OV2640_UXGAConfig();//��ʼ��Ov2640
   OV2640_DCMI_Init(); //����dcmi��ʼ���ṹ��
   DCMI_Cmd(ENABLE);  //ʹ��DCMI�ɼ�����
	 OV2640_NVIT_Config();
	 Delay_ms(600);//�����ϵ���ȶ���ͼ������
	
	// SD_Test();  //ʵ��ʱ����Ҫ
   EXTI_Key_Config(); 
	
	//memset((uint32_t *)IMAGE_FRAME_BUFFER,0x00,0x00300000); /* ��ʼ���黺3m�ڴ�����*/
	  
	while(1)
	{
		
	 DCMI_CaptureCmd(ENABLE); //��ʼ��׽ͼ��
	//DMA���ɼ��������ݰᵽlcd��ʾ
	 OV2640_DMAtolcd_Config(FSMC_LCD_ADDRESS+(lcd_height-1)*(lcd_width)*2,img_width*2/4);  
	 Delay_ms(300);    //���ɵ���200������
	
  //����SDRAM�ռ�洢�Ҷ�ͼƬ
	//�����Դ����ݵ����飨�����׵�ַ�Ѷ�����SDROM�У�
	 for(i=0;i<img_height;i++)
	{
	OV2640_DMAlcdtov_Config(FSMC_LCD_ADDRESS+i*(lcd_width)*2,(uint32_t)image2[i],img_width);
	Delay_ms(1);
	}
	  
/*-------------------ͼ��Ԥ����----------------------------*/	

   //�Ҷ�Ԥ����
	 for(j=0;j<img_height;j++)
	 {
	   for(i=0;i<img_width;i++)
	  {
		 image2[j][i]=RGB565_to_Gray(image2[j][i]);
		 }
 	 }	   
	   
  
	 MeanFilterOper();         //��Ȩ��ֵ�˲���������Ч���Ϻ�
	 //MedianFilterOper();    //��ֵ�˲�----����ƽ������һЩ��Ե��Ĩ��
	 // KNNFilterOper();      //knn�˲�
	 //Hist_plane();          //ֱ��ͼ���⻯
	  
	 //lcd��ʾ
	//  for(i=1;i<=img_height;i++)
	// {
	//	OV2640_DMAvtolcd_Config((uint32_t)image2[i],FSMC_LCD_ADDRESS+i*(lcd_width)*2,img_width);
	//	 Delay_ms(1);
	// }
    
/*-------------------ͼ��ָ��Ե���----------------------------*/	 
    
	  thre=otsuThreshold();    //����1�Զ���ֵѡȡ
	   // thre=OET();          //����2
  	get_newimage(thre);      //ͼ��ָ�ں���̬ѧ�˲���
	  
	  //log_process(thre);//��Ե���
    //canny_process(thre);//�����log��Ե���canny����ʱ����Ч�ʵͣ�����Ч���ӽ� 
	 
	 //lcd��ʾ
	//  for(i=1;i<=img_height;i++)
	// {
	//	OV2640_DMAvtolcd_Config((uint32_t)image2[i],FSMC_LCD_ADDRESS+i*(lcd_width)*2,img_width);
	//	 Delay_ms(1);
	// }
  //  Delay_ms(300);
	 	  
	 
/*-----------�����ǡ�Ŀ����ȡ��������ֵ��Ч������ʱ�����Ӵ���ʱ�䣩-------------------------------*/		 
	  
	   LCD_SetTextColor(LCD_COLOR_BLACK);  //���ľֲ�������
	   LCD_DrawRect(200,20,400,440);	 
	 
	 //  cnt=mylabel();                       //������
	 //  label_extract(cnt,1000);             //Ŀ����ȡ
	  
  	  //lcd��ʾ
		// for(i=1;i<=img_height;i++)
		//	 {
		//		OV2640_DMAvtolcd_Config((uint32_t)image2[i],FSMC_LCD_ADDRESS+i*(lcd_width)*2,img_width);
		//		 Delay_ms(1);
		//	 }
		//		Delay_ms(300);
	 

/*-------------------�������-������ʾ-�������----------------------------*/			      
		 
		  point(p);                           //���Ե����������
	   
	    LCD_SetTextColor(LCD_COLOR_RED);  
	    LCD_DrawRect(p[3],p[1],p[2]-p[3],p[0]-p[1]);	  //��С����
	   
  	  d=distance(0.015,0.4,15,p[2],p[3]);
	   //��������ֵ(����Ϊ��Ԫ�ߴ硢���ࡢ��ࡢ�����С������)
     
			LCD_SetColors(LCD_COLOR_RED,TRANSPARENCY);
			LCD_ClearLine(LINE(17));
			sprintf((char*)dispBuf, "��ֱ����:%.2fcm",(float)d);
			LCD_DisplayStringLine_EN_CH(LINE(17),dispBuf);
     
		 /*����printf��������Ϊ�ض�����fputc��printf�����ݻ����������*/
			//printf("%d",(int)d);
			Usart_SendHalfWord(DEBUG_USART1,(uint16_t)d);
/*-------------------�Ƕȼ���-�Ƕ���ʾ-�������----------------------------*/			      
		      
		/*	if (d<=40)
			{
			 dig=diagonal(p,1.4,0.155);
			 LCD_SetColors(LCD_COLOR_RED,TRANSPARENCY);
			 LCD_ClearLine(LINE(15));
			 sprintf((char*)dispBuf, "ץȡ�Ƕ�:%.2fcm",(float)dig);
			 LCD_DisplayStringLine_EN_CH(LINE(17),dispBuf);
			// printf("%d",(int)dig);
			 Usart_SendHalfWord(DEBUG_USART1,(uint16_t)d);
			}
    */
/*-------------------------��Ƭ�洢----------------------------*/			 
		  
		/*	res=1;
			sprintf((char *)&file_name,"/camera%d",num++);  
		  res=Screen_shot(0,0,800,480,(unsigned char *)file_name);
			if(res==0)
				printf("\r\ntake photos success!\r\n");
	    frame_flag=0;
	  */
   } 
    
 }

/*********************************************END OF FILE**********************/

