#ifndef __OV2640_H
#define	__OV2640_H

/**
  ******************************************************************************
  * @file    DCMI/Camera/dcmi_ov2640.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    13-April-2012
  * @brief   Header for dcmi_ov2640.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DCMI_OV2640_H
#define __DCMI_OV2640_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "./systick/bsp_SysTick.h"
#include "./lcd/bsp_lcd.h"
#include "proess.h"
#define Delay(ms)  Delay_ms(ms)


/* Exported constants --------------------------------------------------------*/
#define DCMI_DR_ADDRESS       (uint32_t)0x50050028   //DCMI_BASE+0x28
#define FSMC_LCD_ADDRESS      LCD_FRAME_BUFFER

// ����SDRAM��3M�ֽ���Ϊ���ݻ��棬����ʹ���Դ�����Ŀռ䣬
// 0xD0800000-0x300000 = 0xD0500000
#define  IMAGE_FRAME_BUFFER  (uint32_t)0xD0500000
#define  IMGN_FRAME_BUFFER  (uint32_t)0xD0200000


/* Image Sizes enumeration */
typedef enum   
{
  BMP_QQVGA             =   0x00,	    /* BMP Image QQVGA 160x120 Size */
  BMP_QVGA              =   0x01,      /* BMP Image VGA 800*480 Size */
  JPEG_160x120          =   0x02,	    /* JPEG Image 160x120 Size */
  JPEG_176x144          =   0x03,	    /* JPEG Image 176x144 Size */
  JPEG_320x240          =   0x04,	    /* JPEG Image 320x240 Size */
  JPEG_352x288          =   0x05	    /* JPEG Image 352x288 Size */
}ImageFormat_TypeDef;


/*����ͷ�ӿ� */
//IIC SCCB
#define CAMERA_I2C                          I2C1
#define CAMERA_I2C_CLK                      RCC_APB1Periph_I2C1

#define CAMERA_I2C_SCL_PIN                  GPIO_Pin_6
#define CAMERA_I2C_SCL_GPIO_PORT            GPIOB
#define CAMERA_I2C_SCL_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define CAMERA_I2C_SCL_SOURCE               GPIO_PinSource6
#define CAMERA_I2C_SCL_AF                   GPIO_AF_I2C1

#define CAMERA_I2C_SDA_PIN                  GPIO_Pin_7
#define CAMERA_I2C_SDA_GPIO_PORT            GPIOB
#define CAMERA_I2C_SDA_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define CAMERA_I2C_SDA_SOURCE               GPIO_PinSource7
#define CAMERA_I2C_SDA_AF                   GPIO_AF_I2C1

//VSYNC
#define DCMI_VSYNC_GPIO_PORT        	GPIOI
#define DCMI_VSYNC_GPIO_CLK         	RCC_AHB1Periph_GPIOI
#define DCMI_VSYNC_GPIO_PIN         	GPIO_Pin_5
#define DCMI_VSYNC_PINSOURCE        	GPIO_PinSource5
#define DCMI_VSYNC_AF			          GPIO_AF_DCMI
// HSYNC
#define DCMI_HSYNC_GPIO_PORT        	GPIOA
#define DCMI_HSYNC_GPIO_CLK         	RCC_AHB1Periph_GPIOA
#define DCMI_HSYNC_GPIO_PIN         	GPIO_Pin_4
#define DCMI_HSYNC_PINSOURCE        	GPIO_PinSource4
#define DCMI_HSYNC_AF			          GPIO_AF_DCMI
//PIXCLK
#define DCMI_PIXCLK_GPIO_PORT        	GPIOA
#define DCMI_PIXCLK_GPIO_CLK         	RCC_AHB1Periph_GPIOA
#define DCMI_PIXCLK_GPIO_PIN         	GPIO_Pin_6
#define DCMI_PIXCLK_PINSOURCE        	GPIO_PinSource6
#define DCMI_PIXCLK_AF			          GPIO_AF_DCMI
//PWDN
#define DCMI_PWDN_GPIO_PORT        	GPIOG
#define DCMI_PWDN_GPIO_CLK         	RCC_AHB1Periph_GPIOG
#define DCMI_PWDN_GPIO_PIN         	GPIO_Pin_3

//�����ź���
#define DCMI_D0_GPIO_PORT        	GPIOH
#define DCMI_D0_GPIO_CLK         	RCC_AHB1Periph_GPIOH
#define DCMI_D0_GPIO_PIN         	GPIO_Pin_9
#define DCMI_D0_PINSOURCE        	GPIO_PinSource9
#define DCMI_D0_AF			          GPIO_AF_DCMI

#define DCMI_D1_GPIO_PORT        	GPIOH
#define DCMI_D1_GPIO_CLK         	RCC_AHB1Periph_GPIOH
#define DCMI_D1_GPIO_PIN         	GPIO_Pin_10
#define DCMI_D1_PINSOURCE        	GPIO_PinSource10
#define DCMI_D1_AF			          GPIO_AF_DCMI

#define DCMI_D2_GPIO_PORT        	GPIOH
#define DCMI_D2_GPIO_CLK         	RCC_AHB1Periph_GPIOH
#define DCMI_D2_GPIO_PIN         	GPIO_Pin_11
#define DCMI_D2_PINSOURCE        	GPIO_PinSource11
#define DCMI_D2_AF			          GPIO_AF_DCMI

#define DCMI_D3_GPIO_PORT        	GPIOH
#define DCMI_D3_GPIO_CLK         	RCC_AHB1Periph_GPIOH
#define DCMI_D3_GPIO_PIN         	GPIO_Pin_12
#define DCMI_D3_PINSOURCE        	GPIO_PinSource12
#define DCMI_D3_AF			          GPIO_AF_DCMI

#define DCMI_D4_GPIO_PORT        	GPIOH
#define DCMI_D4_GPIO_CLK         	RCC_AHB1Periph_GPIOH
#define DCMI_D4_GPIO_PIN         	GPIO_Pin_14
#define DCMI_D4_PINSOURCE        	GPIO_PinSource14
#define DCMI_D4_AF			          GPIO_AF_DCMI

#define DCMI_D5_GPIO_PORT        	GPIOD
#define DCMI_D5_GPIO_CLK         	RCC_AHB1Periph_GPIOD
#define DCMI_D5_GPIO_PIN         	GPIO_Pin_3
#define DCMI_D5_PINSOURCE        	GPIO_PinSource3
#define DCMI_D5_AF			          GPIO_AF_DCMI

#define DCMI_D6_GPIO_PORT        	GPIOI
#define DCMI_D6_GPIO_CLK         	RCC_AHB1Periph_GPIOI
#define DCMI_D6_GPIO_PIN         	GPIO_Pin_6
#define DCMI_D6_PINSOURCE        	GPIO_PinSource6
#define DCMI_D6_AF			          GPIO_AF_DCMI

#define DCMI_D7_GPIO_PORT        	GPIOI
#define DCMI_D7_GPIO_CLK         	RCC_AHB1Periph_GPIOI
#define DCMI_D7_GPIO_PIN         	GPIO_Pin_7
#define DCMI_D7_PINSOURCE        	GPIO_PinSource7
#define DCMI_D7_AF			          GPIO_AF_DCMI


/*debug*/

#define CAMERA_DEBUG_ON          1
#define CAMERA_DEBUG_ARRAY_ON   1
#define CAMERA_DEBUG_FUNC_ON    1
   
   
// Log define
#define CAMERA_INFO(fmt,arg...)           printf("<<-CAMERA-INFO->> "fmt"\n",##arg)
#define CAMERA_ERROR(fmt,arg...)          printf("<<-CAMERA-ERROR->> "fmt"\n",##arg)
#define CAMERA_DEBUG(fmt,arg...)        //  do{\
                                         if(CAMERA_DEBUG_ON)\
                                         printf("<<-CAMERA-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                       }while(0)
#define CAMERA_DEBUG_ARRAY(array, num)    do{\
                                         int32_t i;\
                                         uint32_t* a = array;\
                                         if(CAMERA_DEBUG_ARRAY_ON)\
                                         {\
                                            printf("<<-CAMERA-DEBUG-ARRAY->>\n");\
                                            for (i = 0; i < (num); i++)\
                                            {\
                                                printf("%04x   ", (a)[i]);\
                                                if ((i + 1 ) %10 == 0)\
                                                {\
                                                    printf("\n");\
                                                }\
                                            }\
                                            printf("\n");\
                                        }\
                                       }while(0)
#define CAMERA_DEBUG_FUNC()               do{\
                                         if(CAMERA_DEBUG_FUNC_ON)\
                                         printf("<<-CAMERA-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
                                       }while(0)


/* Exported types ------------------------------------------------------------*/
//�洢����ͷID�Ľṹ��
typedef struct
{
  uint8_t Manufacturer_ID1;
  uint8_t Manufacturer_ID2;
  uint8_t PIDH;
  uint8_t PIDL;
}OV2640_IDTypeDef;

/* Exported constants --------------------------------------------------------*/

/* Use this define to set the maximum delay timeout for the I2C DCMI_OV2640_SingleRandomWrite()
   and DCMI_OV2640_SingleRandomRead() operations. Exeeding this timeout delay, 
   the read/write functions will be aborted and return error code (0xFF).
   The period of the delay will depend on the system operating frequency. The following
   value has been set for system running at 168 MHz. */
#define DCMI_TIMEOUT_MAX               10000

#define OV2640_DEVICE_WRITE_ADDRESS    0x60
#define OV2640_DEVICE_READ_ADDRESS     0x61

/* OV2640 Registers definition when DSP bank selected (0xFF = 0x00) */
#define OV2640_DSP_R_BYPASS     0x05
#define OV2640_DSP_Qs           0x44
#define OV2640_DSP_CTRL         0x50
#define OV2640_DSP_HSIZE1       0x51
#define OV2640_DSP_VSIZE1       0x52
#define OV2640_DSP_XOFFL        0x53
#define OV2640_DSP_YOFFL        0x54
#define OV2640_DSP_VHYX         0x55
#define OV2640_DSP_DPRP         0x56
#define OV2640_DSP_TEST         0x57
#define OV2640_DSP_ZMOW         0x5A
#define OV2640_DSP_ZMOH         0x5B
#define OV2640_DSP_ZMHH         0x5C
#define OV2640_DSP_BPADDR       0x7C
#define OV2640_DSP_BPDATA       0x7D
#define OV2640_DSP_CTRL2        0x86
#define OV2640_DSP_CTRL3        0x87
#define OV2640_DSP_SIZEL        0x8C
#define OV2640_DSP_HSIZE2       0xC0
#define OV2640_DSP_VSIZE2       0xC1
#define OV2640_DSP_CTRL0        0xC2
#define OV2640_DSP_CTRL1        0xC3
#define OV2640_DSP_R_DVP_SP     0xD3
#define OV2640_DSP_IMAGE_MODE   0xDA
#define OV2640_DSP_RESET        0xE0
#define OV2640_DSP_MS_SP        0xF0
#define OV2640_DSP_SS_ID        0x7F
#define OV2640_DSP_SS_CTRL      0xF8
#define OV2640_DSP_MC_BIST      0xF9
#define OV2640_DSP_MC_AL        0xFA
#define OV2640_DSP_MC_AH        0xFB
#define OV2640_DSP_MC_D         0xFC
#define OV2640_DSP_P_STATUS     0xFE
#define OV2640_DSP_RA_DLMT      0xFF

/* OV2640 Registers definition when sensor bank selected (0xFF = 0x01) */
#define OV2640_SENSOR_GAIN       0x00
#define OV2640_SENSOR_COM1       0x03
#define OV2640_SENSOR_REG04      0x04
#define OV2640_SENSOR_REG08      0x08
#define OV2640_SENSOR_COM2       0x09
#define OV2640_SENSOR_PIDH       0x0A
#define OV2640_SENSOR_PIDL       0x0B
#define OV2640_SENSOR_COM3       0x0C
#define OV2640_SENSOR_COM4       0x0D
#define OV2640_SENSOR_AEC        0x10
#define OV2640_SENSOR_CLKRC      0x11
#define OV2640_SENSOR_COM7       0x12
#define OV2640_SENSOR_COM8       0x13
#define OV2640_SENSOR_COM9       0x14
#define OV2640_SENSOR_COM10      0x15
#define OV2640_SENSOR_HREFST     0x17
#define OV2640_SENSOR_HREFEND    0x18
#define OV2640_SENSOR_VSTART     0x19
#define OV2640_SENSOR_VEND       0x1A
#define OV2640_SENSOR_MIDH       0x1C
#define OV2640_SENSOR_MIDL       0x1D
#define OV2640_SENSOR_AEW        0x24
#define OV2640_SENSOR_AEB        0x25
#define OV2640_SENSOR_W          0x26
#define OV2640_SENSOR_REG2A      0x2A
#define OV2640_SENSOR_FRARL      0x2B
#define OV2640_SENSOR_ADDVSL     0x2D
#define OV2640_SENSOR_ADDVHS     0x2E
#define OV2640_SENSOR_YAVG       0x2F
#define OV2640_SENSOR_REG32      0x32
#define OV2640_SENSOR_ARCOM2     0x34
#define OV2640_SENSOR_REG45      0x45
#define OV2640_SENSOR_FLL        0x46
#define OV2640_SENSOR_FLH        0x47
#define OV2640_SENSOR_COM19      0x48
#define OV2640_SENSOR_ZOOMS      0x49
#define OV2640_SENSOR_COM22      0x4B
#define OV2640_SENSOR_COM25      0x4E
#define OV2640_SENSOR_BD50       0x4F
#define OV2640_SENSOR_BD60       0x50
#define OV2640_SENSOR_REG5D      0x5D
#define OV2640_SENSOR_REG5E      0x5E
#define OV2640_SENSOR_REG5F      0x5F
#define OV2640_SENSOR_REG60      0x60
#define OV2640_SENSOR_HISTO_LOW  0x61
#define OV2640_SENSOR_HISTO_HIGH 0x62
#define img_buf_size  480*800
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern uint16_t img_width,img_height;
extern uint16_t image[img_buf_size];
extern uint16_t imgn[480][800];
extern uint16_t image2[480][800];
extern uint16_t m[480][800];
extern int16_t  o[480][800];
extern uint16_t image_flag,frame_flag,proess_ok,num;
void OV2640_HW_Init(void);
void OV2640_Reset(void);
void OV2640_ReadID(OV2640_IDTypeDef *OV2640ID);
void OV2640_DCMI_Init(void);
void OV2640_NVIT_Config(void);
void OV2640_QQVGAConfig(void);
void OV2640_UXGAConfig(void);
void OV2640_JPEGConfig(ImageFormat_TypeDef ImageFormat);
void OV2640_RGB565_Mode(void);
void OV2640_UXGA_YUV422_Mode(void);
void OV2640_BrightnessConfig(uint8_t Brightness);
void OV2640_ContrastConfig(uint8_t value1, uint8_t value2);
void OV2640_BandWConfig(uint8_t BlackWhite);
void OV2640_ColorEffectsConfig(uint8_t value1, uint8_t value2);
uint8_t OV2640_WriteReg(uint16_t Addr, uint8_t Data);
uint8_t OV2640_ReadReg(uint16_t Addr);


void OV2640_DMA_Buffer1_Config(void);
void OV2640_DMA_Buffer2_Config(void);
void OV2640_DMA_Buffer3_Config(void);
void OV2640_DMAvtolcd_Config(uint32_t v_Addr,uint32_t lcd_Addr,uint16_t DMA_BufferSize);
void OV2640_DMAtolcd_Config(uint32_t DMA_Memory0BaseAddr,uint16_t DMA_BufferSize);
void OV2640_DMAlcdtov_Config(uint32_t lcd_Addr,uint32_t v_Addr,uint16_t DMA_BufferSize);
void OV2640_SpecialEffects(uint8_t mode);
void OV2640_LightMode(uint8_t mode);


#endif /* __DCMI_OV2640_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


#endif /* __LED_H */
