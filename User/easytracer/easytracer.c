#include "stm32f4xx.h"
#include "easytracer.h"
#include "./camera/bsp_ov2640.h"

#define min3v(v1, v2, v3)   ((v1)>(v2)? ((v2)>(v3)?(v3):(v2)):((v1)>(v3)?(v3):(v1)))
#define max3v(v1, v2, v3)   ((v1)<(v2)? ((v2)<(v3)?(v3):(v2)):((v1)<(v3)?(v3):(v1)))

typedef struct                      //RGB��ʽ��ɫ
{
    uint8_t  red;             // [0,255]
    uint8_t  green;           // [0,255]
    uint8_t  blue;            // [0,255]
}COLOR_RGB;

typedef struct                      //HSL��ʽ��ɫ
{
    uint8_t hue;              // [0,240]
    uint8_t saturation;       // [0,240]
    uint8_t luminance;        // [0,240]
}COLOR_HSL;

typedef struct                      //����
{
	uint16_t X_Start;              
	uint16_t X_End;
	uint16_t Y_Start;              
	uint16_t Y_End;
}SEARCH_AREA;



//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//��ȡRBG��ʽ��ɫ��Ψһ��Ҫ��ֲ�ĺ���
//----------------------------------------------------------------------------
//extern unsigned short GUI_ReadBit16Point(unsigned short x,unsigned short y);
#define GUI_ReadBit16Point   ILI9341_GetPointPixel


//============================================================================
//��ȡ����㴦����ɫ
//============================================================================
static void ReadColor(unsigned int x,unsigned int y,COLOR_RGB *Rgb)
{
	unsigned short C16;

	C16 = image2[x][y];                   //�������ɫ

	Rgb->red   =	 (unsigned char)((C16&0xf800)>>8);
	Rgb->green =	 (unsigned char)((C16&0x07e0)>>3);
	Rgb->blue  =   (unsigned char)((C16&0x001f));
}



//============================================================================
//RGBתHSL
//============================================================================
static void RGBtoHSL(const COLOR_RGB *Rgb, COLOR_HSL *Hsl)
{
  int h,s,l,maxVal,minVal,difVal;
	int r  = Rgb->red;
	int g  = Rgb->green;
    int b  = Rgb->blue;
	
	maxVal = max3v(r, g, b);
	minVal = min3v(r, g, b);
	
	difVal = maxVal-minVal;
	
	//��������
    l = (maxVal+minVal)*240/255/2;
	
	if(maxVal == minVal)//��r=g=b
	{
		h = 0; 
		s = 0;
	}
	else
	{
		//����ɫ��
		if(maxVal==r)
		{
			if(g>=b)
				h = 40*(g-b)/(difVal);
			else
				h = 40*(g-b)/(difVal) + 240;
		}
		else if(maxVal==g)
			h = 40*(b-r)/(difVal) + 80;
		else if(maxVal==b)
			h = 40*(r-g)/(difVal) + 160;
		//���㱥�Ͷ�
		if(l == 0)
			s = 0;
		else if(l<=120)
			s = (difVal)*240/(maxVal+minVal);
		else
			s = (difVal)*240/(480 - (maxVal+minVal));
	}
    Hsl->hue =        (unsigned char)(((h>240)? 240 : ((h<0)?0:h)));
    Hsl->saturation = (unsigned char)(((s>240)? 240 : ((s<0)?0:s)));
    Hsl->luminance =  (unsigned char)(((l>240)? 240 : ((l<0)?0:l)));
}

//============================================================================
//ƥ����ɫ
//============================================================================
static int ColorMatch(const COLOR_HSL *Hsl,const TARGET_CONDI *Condition)
{
	if( 
		Hsl->hue		>	Condition->H_MIN &&
		Hsl->hue		<	Condition->H_MAX &&
		Hsl->saturation	>	Condition->S_MIN &&
		Hsl->saturation	<   Condition->S_MAX &&
		Hsl->luminance	>	Condition->L_MIN &&
		Hsl->luminance	<   Condition->L_MAX 
    )
		return 1;
	else
		return 0;
}

//============================================================================
//������ʴ����
//============================================================================
static int SearchCentre(uint16_t *x,uint16_t *y,const TARGET_CONDI *Condition,const SEARCH_AREA *Area)
{
	uint16_t SpaceX,SpaceY,i,j,k,FailCount=0;
	COLOR_RGB Rgb;
	COLOR_HSL Hsl;
	
	SpaceX = Condition->WIDTH_MIN/3;
	SpaceY = Condition->HIGHT_MIN/3;//�����ռ䣿����

	for(i=Area->Y_Start;i<Area->Y_End;i+=SpaceY)
	{
		for(j=Area->X_Start;j<Area->X_End;j+=SpaceX)
		{
			FailCount=0;
			for(k=0;k<SpaceX+SpaceY;k++)
			{
				if(k<SpaceX)
					ReadColor(j+k,i+SpaceY/2,&Rgb);
				else
					ReadColor(j+SpaceX/2,i+(k-SpaceX),&Rgb);
				  RGBtoHSL(&Rgb,&Hsl);
				
				if(!ColorMatch(&Hsl,Condition))
					FailCount++;
				if(FailCount>((SpaceX+SpaceY)>>ALLOW_FAIL_PER))
					break;
			}
			if(k == SpaceX+SpaceY)
			{
				*x = j+SpaceX/2;
				*y = i+SpaceY/2;
				return 1;
			}
		}
	}
	
	return 0;
	
}

//============================================================================
//�Ӹ�ʴ�������ⸯʴ���õ��µĸ�ʴ����
//============================================================================
static int Corrode(uint16_t oldx,uint16_t oldy,const TARGET_CONDI *Condition,RESULT *Resu)
{
	uint16_t Xmin,Xmax,Ymin,Ymax,i,FailCount=0;
	COLOR_RGB Rgb;
	COLOR_HSL Hsl;
	
	for(i=oldx;i>IMG_X;i--)
	{
		ReadColor(i,oldy,&Rgb);
		RGBtoHSL(&Rgb,&Hsl);
		if(!ColorMatch(&Hsl,Condition))
			FailCount++;
		if(FailCount>(((Condition->WIDTH_MIN+Condition->WIDTH_MAX)>>2)>>ALLOW_FAIL_PER))
			break;	
	}
	Xmin=i;
	FailCount=0;
	
	for(i=oldx;i<IMG_X+IMG_W;i++)
	{
		ReadColor(i,oldy,&Rgb);
		RGBtoHSL(&Rgb,&Hsl);
		if(!ColorMatch(&Hsl,Condition))
			FailCount++;
		if(FailCount>(((Condition->WIDTH_MIN+Condition->WIDTH_MAX)>>2)>>ALLOW_FAIL_PER))
			break;	
	}
	Xmax=i;
	FailCount=0;
	
	for(i=oldy;i>IMG_Y;i--)
	{
		ReadColor(oldx,i,&Rgb);
		RGBtoHSL(&Rgb,&Hsl);
		if(!ColorMatch(&Hsl,Condition))
			FailCount++;
		if(FailCount>(((Condition->HIGHT_MIN+Condition->HIGHT_MAX)>>2)>>ALLOW_FAIL_PER))
			break;	
	}
	Ymin=i;
	FailCount=0;
	
	for(i=oldy;i<IMG_Y+IMG_H;i++)
	{
		ReadColor(oldx,i,&Rgb);
		RGBtoHSL(&Rgb,&Hsl);
		if(!ColorMatch(&Hsl,Condition))
			FailCount++;
		if(FailCount>(((Condition->HIGHT_MIN+Condition->HIGHT_MAX)>>2)>>ALLOW_FAIL_PER))
			break;	
	}
	Ymax=i;
	FailCount=0;
	
	Resu->x	= (Xmin+Xmax)/2;
	Resu->y	= (Ymin+Ymax)/2;
	Resu->w	= Xmax-Xmin;
	Resu->h	= Ymax-Ymin;

	if(((Xmax-Xmin)>(Condition->WIDTH_MIN)) && ((Ymax-Ymin)>(Condition->HIGHT_MIN)) &&\
	   ((Xmax-Xmin)<(Condition->WIDTH_MAX)) && ((Ymax-Ymin)<(Condition->HIGHT_MAX)) )
		return 1;	
	else
		return 0;	
}


//============================================================================
//Ψһ��API���û���ʶ������д��Conditionָ��Ľṹ���У��ú���������Ŀ���x��y����ͳ���
//����1ʶ��ɹ�������0ʶ��ʧ��
//============================================================================
int Trace(const TARGET_CONDI *Condition,RESULT *Resu)
{
	uint16_t i;
	uint16_t x0,y0,flag_=0;
	static SEARCH_AREA Area={IMG_X,
	                         IMG_X+IMG_W,
	                         IMG_Y,
	                         IMG_Y+IMG_H};  //ʶ�����Ч��Χ  ���ﶨ��Ϊ400*240
	RESULT Result;	                                                //����ʶ����
	
	if(flag_ == 0)
	{
		if(SearchCentre(&x0,&y0,Condition,&Area))   //������ʴ����
			flag_ = 1;
		else
		{
			Area.X_Start= IMG_X	;
			Area.X_End  = IMG_X+IMG_W  ;
			Area.Y_Start= IMG_Y		;
			Area.Y_End  = IMG_Y+IMG_H;

			if(SearchCentre(&x0,&y0,Condition,&Area))	
			{
				flag_ =0;
				return 0;
			}	
		}
	}
	Result.x = x0;
	Result.y = y0;
	
	for(i=0;i<ITERATE_NUM;i++)
		Corrode(Result.x,Result.y,Condition,&Result);   //�Ӹ�ʴ�������ⸯʴ���õ��µĸ�ʴ����
		
	if(Corrode(Result.x,Result.y,Condition,&Result))
	{
		x0      = Result.x;
		y0      = Result.y;
		Resu->x = Result.x;
		Resu->y = Result.y;
		Resu->w = Result.w;
		Resu->h = Result.h;
		flag_   = 1;

		Area.X_Start= Result.x - ((Result.w)>>1);
		Area.X_End  = Result.x + ((Result.w)>>1);
		Area.Y_Start= Result.y - ((Result.h)>>1);
		Area.Y_End  = Result.y + ((Result.h)>>1);

		return 1;
	}
	else
	{
		flag_  = 0;
		return 0;
	}

}

//ʹ�þ���
//RESULT Resu;
//TARGET_CONDI Condition={60,100,20,120,10,160,40,40,320,240};
//Trace(&Condition,&Resu);

