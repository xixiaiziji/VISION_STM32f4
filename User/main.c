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
  /*定义一个GPIO_InitTypeDef类型的结构体*/
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOG, ENABLE); 							   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
  GPIO_Init(GPIOG, &GPIO_InitStructure);	
  
  GPIO_ResetBits(GPIOG,GPIO_Pin_9);  //禁用WiFi模块
}

/*简单任务管理*/
uint32_t Task_Delay[NumOfTask];
uint16_t thre;                      //阈值参数
uint16_t ratio,center_x,center_y,cnt;
uint8_t dispBuf[100];
OV2640_IDTypeDef OV2640_Camera_ID;
uint16_t i=0,j=0,num=0,flag=0;
uint8_t fps=0;                       //帧率计数
uint16_t d;                         //距离参数
uint16_t p[4];                    //最小外接矩形4个点
uint16_t dig;                     //角度参数


extern uint16_t line_num;

int main(void)
{
  uint8_t file_name[20];
	uint8_t num = 0;
	uint8_t res=0;
	
	/* 禁用WiFi模块 */
	BL8782_PDN_INIT();
	
	/*GPIO 配置,工作模式配置。115200 8-N-1，中断接收*/
  Debug_USART_Config();   
	
	/* 配置SysTick 为1ms中断一次,时间到后触发定时中断，
	*进入stm32fxx_it.c文件的SysTick_Handler处理，通过数中断次数计时
	*/
	SysTick_Init();
  
	
	/*初始化液晶屏*/
 
	LCD_Init();
  LCD_LayerInit();
  LTDC_Cmd(ENABLE);
	
	/*把背景层刷黑色*/
 
  LCD_SetLayer(LCD_BACKGROUND_LAYER);  
	LCD_SetTransparency(0xFF);
	LCD_Clear(LCD_COLOR_WHITE);
	
  /*初始化后默认使用前景层*/
	LCD_SetLayer(LCD_FOREGROUND_LAYER); 
	
	/*该函数参数为不透明度，范围 0-0xff ，0为全透明，0xff为不透明*/
  
	LCD_SetTransparency(0xff);
	LCD_Clear(TRANSPARENCY);//设置前景透明
	LCD_SetColors(LCD_COLOR_YELLOW,TRANSPARENCY);  
	LCD_ClearLine(LINE(19));
  LCD_DisplayStringLine_EN_CH(LINE(19),(uint8_t* )"UXGA:480*800");
 
  
  /* 初始化摄像头GPIO及IIC */
  OV2640_HW_Init();   

  /* 读取摄像头芯片ID，确定摄像头正常连接 */
  OV2640_ReadID(&OV2640_Camera_ID);

   if(OV2640_Camera_ID.PIDH  == 0x26)
  {
    //sprintf((char*)dispBuf, "              OV2640 摄像头,ID:0x%x", OV2640_Camera_ID.PIDH);
		//LCD_DisplayStringLine_EN_CH(LINE(0),(uint8_t*)dispBuf);
    CAMERA_DEBUG("%x %x",OV2640_Camera_ID.Manufacturer_ID1 ,OV2640_Camera_ID.Manufacturer_ID2);

  }
  else
  {
    LCD_SetTextColor(LCD_COLOR_RED);
    LCD_DisplayStringLine_EN_CH(LINE(0),(uint8_t*) "         没有检测到OV2640，请重新检查连接。");
    CAMERA_DEBUG("没有检测到OV2640摄像头，请重新检查连接。");

    while(1);  
  }

	 OV2640_UXGAConfig();//初始化Ov2640
   OV2640_DCMI_Init(); //配置dcmi初始化结构体
   DCMI_Cmd(ENABLE);  //使能DCMI采集数据
	 OV2640_NVIT_Config();
	 Delay_ms(600);//丢弃上电后不稳定的图像数据
	
	// SD_Test();  //实测时不需要
   EXTI_Key_Config(); 
	
	//memset((uint32_t *)IMAGE_FRAME_BUFFER,0x00,0x00300000); /* 初始数组缓3m内存区域*/
	  
	while(1)
	{
		
	 DCMI_CaptureCmd(ENABLE); //开始捕捉图像
	//DMA将采集到的数据搬到lcd显示
	 OV2640_DMAtolcd_Config(FSMC_LCD_ADDRESS+(lcd_height-1)*(lcd_width)*2,img_width*2/4);  
	 Delay_ms(300);    //不可低于200否则花屏
	
  //开辟SDRAM空间存储灰度图片
	//搬运显存数据到数组（数组首地址已定义在SDROM中）
	 for(i=0;i<img_height;i++)
	{
	OV2640_DMAlcdtov_Config(FSMC_LCD_ADDRESS+i*(lcd_width)*2,(uint32_t)image2[i],img_width);
	Delay_ms(1);
	}
	  
/*-------------------图像预处理----------------------------*/	

   //灰度预处理
	 for(j=0;j<img_height;j++)
	 {
	   for(i=0;i<img_width;i++)
	  {
		 image2[j][i]=RGB565_to_Gray(image2[j][i]);
		 }
 	 }	   
	   
  
	 MeanFilterOper();         //加权均值滤波————效果较好
	 //MedianFilterOper();    //中值滤波----过度平滑导致一些边缘被抹除
	 // KNNFilterOper();      //knn滤波
	 //Hist_plane();          //直方图均衡化
	  
	 //lcd显示
	//  for(i=1;i<=img_height;i++)
	// {
	//	OV2640_DMAvtolcd_Config((uint32_t)image2[i],FSMC_LCD_ADDRESS+i*(lcd_width)*2,img_width);
	//	 Delay_ms(1);
	// }
    
/*-------------------图像分割及边缘检测----------------------------*/	 
    
	  thre=otsuThreshold();    //方法1自动阈值选取
	   // thre=OET();          //方法2
  	get_newimage(thre);      //图像分割（内含形态学滤波）
	  
	  //log_process(thre);//边缘检测
    //canny_process(thre);//相对于log边缘检测canny运行时间慢效率低，处理效果接近 
	 
	 //lcd显示
	//  for(i=1;i<=img_height;i++)
	// {
	//	OV2640_DMAvtolcd_Config((uint32_t)image2[i],FSMC_LCD_ADDRESS+i*(lcd_width)*2,img_width);
	//	 Delay_ms(1);
	// }
  //  Delay_ms(300);
	 	  
	 
/*-----------区域标记、目标提取（用于阈值化效果不好时会增加处理时间）-------------------------------*/		 
	  
	   LCD_SetTextColor(LCD_COLOR_BLACK);  //中心局部计算区
	   LCD_DrawRect(200,20,400,440);	 
	 
	 //  cnt=mylabel();                       //区域标记
	 //  label_extract(cnt,1000);             //目标提取
	  
  	  //lcd显示
		// for(i=1;i<=img_height;i++)
		//	 {
		//		OV2640_DMAvtolcd_Config((uint32_t)image2[i],FSMC_LCD_ADDRESS+i*(lcd_width)*2,img_width);
		//		 Delay_ms(1);
		//	 }
		//		Delay_ms(300);
	 

/*-------------------距离计算-距离显示-串口输出----------------------------*/			      
		 
		  point(p);                           //求绝缘子两端最大点
	   
	    LCD_SetTextColor(LCD_COLOR_RED);  
	    LCD_DrawRect(p[3],p[1],p[2]-p[3],p[0]-p[1]);	  //最小矩形
	   
  	  d=distance(0.015,0.4,15,p[2],p[3]);
	   //测量距离值(参数为像元尺寸、焦距、物距、最大最小横坐标)
     
			LCD_SetColors(LCD_COLOR_RED,TRANSPARENCY);
			LCD_ClearLine(LINE(17));
			sprintf((char*)dispBuf, "垂直距离:%.2fcm",(float)d);
			LCD_DisplayStringLine_EN_CH(LINE(17),dispBuf);
     
		 /*调用printf函数，因为重定向了fputc，printf的内容会输出到串口*/
			//printf("%d",(int)d);
			Usart_SendHalfWord(DEBUG_USART1,(uint16_t)d);
/*-------------------角度计算-角度显示-串口输出----------------------------*/			      
		      
		/*	if (d<=40)
			{
			 dig=diagonal(p,1.4,0.155);
			 LCD_SetColors(LCD_COLOR_RED,TRANSPARENCY);
			 LCD_ClearLine(LINE(15));
			 sprintf((char*)dispBuf, "抓取角度:%.2fcm",(float)dig);
			 LCD_DisplayStringLine_EN_CH(LINE(17),dispBuf);
			// printf("%d",(int)dig);
			 Usart_SendHalfWord(DEBUG_USART1,(uint16_t)d);
			}
    */
/*-------------------------照片存储----------------------------*/			 
		  
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

