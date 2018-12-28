#include "canny.h"
#include "math.h"
int Robert(uint16_t *pGryImg,int width, int height,int threshold)
{	
	uint16_t *lpSrc;
	uint16_t *lpDst;
	uint16_t *NewpGryImg;
  uint16_t *hNewGryImg;	
	int i,j;
	float result;
	uint8_t pixel[4];
	hNewGryImg = new BYTE[width * height];
	if (hNewGryImg == NULL)
	{
		return 0;
	}
	NewpGryImg = hNewGryImg;
	lpDst = NewpGryImg;
	memset(lpDst, (uint8_t)255, width * height);
	for(j = height-1; j > 0; j--)
	{
		for(i = 0;i <width-1; i++)
		{		
			lpSrc = pGryImg + width * j + i;		
			lpDst = NewpGryImg + width * j + i;
			pixel[0] = (uint8_t)*lpSrc;
			pixel[1] = (uint8_t)*(lpSrc + 1);
			pixel[2] = (uint8_t)*(lpSrc - width);
			pixel[3] = (uint8_t)*(lpSrc - width + 1);
			result = sqrt(( pixel[0] - pixel[3] )*( pixel[0] - pixel[3] ) + ( pixel[1] - pixel[2] )*( pixel[1] - pixel[2] ));
			 if (result>threshold)
			 {
				 result=255;
			 }
			 if (result<threshold)
			 {
				 result=0;
			 }
			*lpDst = (uint8_t)result;		
		}
	}
	
// 复制经过模板运算后的图像到原图像
	memcpy(pGryImg, NewpGryImg, width * height);
	return 1;
}

int Sobel(uint16_t *pGryImg,int width,int height,int threshold)
{	
	uint16_t *lpDst1;
	uint16_t *lpDst2;		
	uint16_t *NewpGryImg1;
	uint16_t *hNewGryImg1;
	uint16_t *NewpGryImg2;
	uint16_t *hNewGryImg2;
	long i,j;	
	int	tempH;	
	int	tempW;	
	float tempR;
	int	tempX;
	int	tempY;		
	float aTemplate[9];	

	hNewGryImg1 = new BYTE[width * height];
	if (hNewGryImg1 == NULL)
	{
		return 0;
	}
	NewpGryImg1 = hNewGryImg1;
	hNewGryImg2 = new BYTE[width * height];
	if (hNewGryImg2 == NULL)
	{
		return 0;
	}
	NewpGryImg2 = hNewGryImg2;
	lpDst1 = NewpGryImg1;
	memcpy(NewpGryImg1, pGryImg, width * height);
	lpDst2 = NewpGryImg2;
	memcpy(NewpGryImg2, pGryImg, width * height);
	//设置Sobel模板参数
	tempW = 3;
	tempH = 3;
	tempR = 1.0;
	tempX = 1;
	tempY = 1;
	aTemplate[0] = -1.0;
	aTemplate[1] = -2.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 0.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 0.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 2.0;
	aTemplate[8] = 1.0;
	if (!Template(NewpGryImg1, width, height, tempH, tempW, tempX, tempY, aTemplate, tempR))
	{
		return 0;
	}
	//设置Sobel模板参数
	aTemplate[0] = -1.0;
	aTemplate[1] = 0.0;
	aTemplate[2] = 1.0;
	aTemplate[3] = -2.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 2.0;
	aTemplate[6] = -1.0;
	aTemplate[7] = 0.0;
	aTemplate[8] = 1.0;
	if (!Template(NewpGryImg2, width, height, tempH, tempW, tempX, tempY, aTemplate, tempR))
	{
		return 0;
	}
	for(j = 0; j <height; j++)
	{
		for(i = 0;i <width-1; i++)
		{			
			lpDst1 = NewpGryImg1 + width * j + i;		
			lpDst2 = NewpGryImg2 + width * j + i;
			if(*lpDst2 > *lpDst1)
			{
				*lpDst1 = *lpDst2;
			}
			if (*lpDst1>threshold)
			{
				*lpDst1=255;
			}
			if (*lpDst1<threshold)
			{
				*lpDst1=0;
			}
		}
	}
	memcpy(pGryImg, NewpGryImg1, width * height);
	return 1;
}
int Laplace(uint16_t *pGryImg,int width, int height)
{		
	uint16_t *lpDst;	
	uint16_t *NewpGryImg;
	uint16_t *hNewGryImg;
	long i,j;
	int	tempH;	
	int	tempW;		
	int	tempX;		
	int	tempY;		
	float aTemplate[9];	
	hNewGryImg = new BYTE[width * height];
	if (hNewGryImg == NULL)
	{
		return 0;
	}
	NewpGryImg = hNewGryImg;
	lpDst = NewpGryImg;
	memcpy(NewpGryImg, pGryImg, width * height);
	// 设置Laplace模板参数
	tempW = 3;
	tempH = 3;
	tempX = 1;
	tempY = 1;
	aTemplate[0] = -1.0;
	aTemplate[1] = -1.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = -1.0;
	aTemplate[4] = 8.0;
	aTemplate[5] = -1.0;
	aTemplate[6] = -1.0;
	aTemplate[7] = -1.0;
	aTemplate[8] = -1.0;
	if (!TempLap(NewpGryImg,width,height,tempH,tempW,tempX,tempY,aTemplate))
	{
		return 0;
	}
	memcpy(pGryImg,NewpGryImg,width*height);
	return 1;
}
int Prewitt(uint16_t *pGryImg,int width, int height,int threshold)
{		
	uint16_t *lpDst1;			
	uint16_t *lpDst2;	
	uint16_t *NewpGryImg1;	
	uint16_t *hNewGryImg1;
	uint16_t *NewpGryImg2;
	uint16_t *hNewGryImg2;
	
	long i,j;					
	int		tempH;					
	int		tempW;				
	float	tempR;			
	int		tempX;			
	int		tempY;		
	float aTemplate[9];
	// ??????,??????
	hNewGryImg1 = new BYTE[width * height];
	if (hNewGryImg1 == NULL)
	{
		return 0;
	}
	
	NewpGryImg1 = hNewGryImg1;

	// ??????,??????
	hNewGryImg2 = new BYTE[width * height];
	if (hNewGryImg2 == NULL)
	{
		return 0;
	}

	NewpGryImg2 = hNewGryImg2;
	// ???????????
	lpDst1 = NewpGryImg1;
	memcpy(NewpGryImg1, pGryImg, width * height);
	lpDst2 = NewpGryImg2;
	memcpy(NewpGryImg2, pGryImg, width * height);

	//设置Prewitt模板1参数
	tempW = 3;
	tempH = 3;
	tempR = 1.0;
	tempX = 1;
	tempY = 1;
	aTemplate[0] = -1.0;
	aTemplate[1] = -1.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 0.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 0.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 1.0;
	aTemplate[8] = 1.0;

	if (!Template(NewpGryImg1, width, height, tempH, tempW, tempX, tempY, aTemplate, tempR))
	{
		return 0;
	}

	// 设置Prewitt模板2参数
	aTemplate[0] = 1.0;
	aTemplate[1] = 0.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 1.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -1.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 0.0;
	aTemplate[8] = -1.0;

	if (!Template(NewpGryImg2, width, height, tempH, tempW, tempX, tempY, aTemplate, tempR))
	{
		return 0;
	}

	////求两幅图像最大值
	for(j = 0; j <height; j++)
	{
		for(i = 0;i <width-1; i++)
		{
			// 指向图像1倒数第j行第i个像素指针	
			lpDst1 = NewpGryImg1 + width * j + i;
			// 指向图像2倒数第j行第i个像素指针
			lpDst2 = NewpGryImg2 + width * j + i;
			
			if(*lpDst2 > *lpDst1)
			{
				*lpDst1 = *lpDst2;
			}
		}
	}

	memcpy(pGryImg, NewpGryImg1, width * height);
	return 1;
}

int Kirsch(uint16_t *pGryImg,int width,int height,int threshold)
{		
	uint16_t *lpDst1;			// 指向缓存图像指针
	uint16_t *lpDst2;
	uint16_t *NewpGryImg1;	// 指向缓存图像指针
	uint16_t *hNewGryImg1;
	uint16_t *NewpGryImg2;
	uint16_t *hNewGryImg2;
	long i,j;
	int	tempH;		// 模板宽度
	int	tempW;		// 模板高度
	float tempR;	// 模板系数
	int	tempX;		// 模板中心元素x坐标	
	int	tempY;		// 模板中心元素y坐标	
	float aTemplate[9];		// 模板数组

	hNewGryImg1 = new BYTE[width * height];
	if (hNewGryImg1 == NULL)
	{
		return 0;
	}
	
	NewpGryImg1 = hNewGryImg1;
	hNewGryImg2 = new BYTE[width * height];
	if (hNewGryImg2 == NULL)
	{
		return 0;
	}
	
	NewpGryImg2 = hNewGryImg2;

	// 拷贝原图到缓存图
	lpDst1 = NewpGryImg1;
	memcpy(NewpGryImg1, pGryImg, width * height);
	lpDst2 = NewpGryImg2;
	memcpy(NewpGryImg2, pGryImg, width * height);

	// 设置Kirsch模板1参数
	tempW = 3;
	tempH = 3;
	tempR = 1.0;
	tempX = 1;
	tempY = 1;
	aTemplate[0] = 5.0;
	aTemplate[1] = 5.0;
	aTemplate[2] = 5.0;
	aTemplate[3] = -3.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -3.0;
	aTemplate[6] = -3.0;
	aTemplate[7] = -3.0;
	aTemplate[8] = -3.0;

	if (!Template(NewpGryImg1, width, height, tempH, tempW, tempX, tempY, aTemplate, tempR))
	{
		return 0;
	}

	//设置Kirsch模板2参数
	aTemplate[0] = -3.0;
	aTemplate[1] = 5.0;
	aTemplate[2] = 5.0;
	aTemplate[3] = -3.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 5.0;
	aTemplate[6] = -3.0;
	aTemplate[7] = -3.0;
	aTemplate[8] = -3.0;

	if (!Template(NewpGryImg2, width, height, tempH, tempW, tempX, tempY, aTemplate, tempR))
	{
		return 0;
	}

	//求两幅图像最大值
	for(j = 0; j <height; j++)
	{
		for(i = 0;i <width-1; i++)
		{
			// 指向图像1倒数第j行第i个像素指针		
			lpDst1 = NewpGryImg1 + width * j + i;
			// 指向图像2倒数第j行第i个像素指针		
			lpDst2 = NewpGryImg2 + width * j + i;
			
			if(*lpDst2 > *lpDst1)
			{
				*lpDst1 = *lpDst2;
			}
		}
	}
	memcpy(NewpGryImg2, pGryImg, width * height);

	//设置Kirsch模板3参数
	aTemplate[0] = -3.0;
	aTemplate[1] = -3.0;
	aTemplate[2] = 5.0;
	aTemplate[3] = -3.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 5.0;
	aTemplate[6] = -3.0;
	aTemplate[7] = -3.0;
	aTemplate[8] = 5.0;

	if (!Template(NewpGryImg2, width, height, tempH, tempW, tempX, tempY, aTemplate, tempR))
	{
		return 0;
	}

	//求两幅图像最大值
	for(j = 0; j <height; j++)
	{
		for(i = 0;i <width-1; i++)
		{
			// 指向图像1倒数第j行第i个像素指针	
			lpDst1 = NewpGryImg1 + width * j + i;
			// 指向图像2倒数第j行第i个像素指针	
			lpDst2 = NewpGryImg2 + width * j + i;
			
			if(*lpDst2 > *lpDst1)
			{
				*lpDst1 = *lpDst2;
			}
		}
	}
	memcpy(NewpGryImg2, pGryImg, width * height);
	
	//设置Kirsch模4参数
	aTemplate[0] = -3.0;
	aTemplate[1] = -3.0;
	aTemplate[2] = -3.0;
	aTemplate[3] = -3.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 5.0;
	aTemplate[6] = -3.0;
	aTemplate[7] = 5.0;
	aTemplate[8] = 5.0;

	if (!Template(NewpGryImg2, width, height, tempH, tempW, tempX, tempY, aTemplate, tempR))
	{
		return 0;
	}

	//求两幅图像最大值
	for(j = 0; j <height; j++)
	{
		for(i = 0;i <width-1; i++)
		{
			// 指向图像1倒数第j行第i个像素指针		
			lpDst1 = NewpGryImg1 + width * j + i;
			// 指向图像2倒数第j行第i个像素指针			
			lpDst2 = NewpGryImg2 + width * j + i;
			
			if(*lpDst2 > *lpDst1)
			{
				*lpDst1 = *lpDst2;
			}
		}
	}
	memcpy(NewpGryImg2, pGryImg, width * height);
	
	//设置Kirsch模板5参数
	aTemplate[0] = -3.0;
	aTemplate[1] = -3.0;
	aTemplate[2] = -3.0;
	aTemplate[3] = -3.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -3.0;
	aTemplate[6] = 5.0;
	aTemplate[7] = 5.0;
	aTemplate[8] = 5.0;

	if (!Template(NewpGryImg2, width, height, tempH, tempW, tempX, tempY, aTemplate, tempR))
	{
		return 0;
	}
	memcpy(NewpGryImg2, pGryImg, width * height);

	//求两幅图像最大值
	for(j = 0; j <height; j++)
	{
		for(i = 0;i <width-1; i++)
		{
			// 指向图像1倒数第j行第i个像素指针			
			lpDst1 = NewpGryImg1 + width * j + i;
			// 指向图像2倒数第j行第i个像素指针		
			lpDst2 = NewpGryImg2 + width * j + i;
			
			if(*lpDst2 > *lpDst1)
			{
				*lpDst1 = *lpDst2;
			}
		}
	}
	memcpy(NewpGryImg2, pGryImg, width * height);
	
	//设置Kirsch模板6参数
	aTemplate[0] = -3.0;
	aTemplate[1] = -3.0;
	aTemplate[2] = -3.0;
	aTemplate[3] = 5.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -3.0;
	aTemplate[6] = 5.0;
	aTemplate[7] = 5.0;
	aTemplate[8] = -3.0;

	if (!Template(NewpGryImg2, width, height, tempH, tempW, tempX, tempY, aTemplate, tempR))
	{
		return 0;
	}

	//求两幅图像最大值
	for(j = 0; j <height; j++)
	{
		for(i = 0;i <width-1; i++)
		{
			// 指向图像1倒数第j行第i个像素指针		
			lpDst1 = NewpGryImg1 + width * j + i;
			// 指向图像2倒数第j行第i个像素指针			
			lpDst2 = NewpGryImg2 + width * j + i;
			
			if(*lpDst2 > *lpDst1)
			{
				*lpDst1 = *lpDst2;
			}
		}
	}
	memcpy(NewpGryImg2, pGryImg, width * height);
	
	// 设置Kirsch模板7参数
	aTemplate[0] = 5.0;
	aTemplate[1] = -3.0;
	aTemplate[2] = -3.0;
	aTemplate[3] = 5.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -3.0;
	aTemplate[6] = 5.0;
	aTemplate[7] = -3.0;
	aTemplate[8] = -3.0;

	if (!Template(NewpGryImg2, width, height, tempH, tempW, tempX, tempY, aTemplate, tempR))
	{
		return 0;
	}

	//求两幅图像最大值
	for(j = 0; j <height; j++)
	{
		for(i = 0;i <width-1; i++)
		{
			// 指向图像1倒数第j行第i个像素指针			
			lpDst1 = NewpGryImg1 + width * j + i;
			// 指向图像2倒数第j行第i个像素指针			
			lpDst2 = NewpGryImg2 + width * j + i;
			
			if(*lpDst2 > *lpDst1)
			{
				*lpDst1 = *lpDst2;
			}
		}
	}
	memcpy(NewpGryImg2, pGryImg, width * height);
	
	// 设置Kirsch模板8参数
	aTemplate[0] = 5.0;
	aTemplate[1] = 5.0;
	aTemplate[2] = -3.0;
	aTemplate[3] = 5.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -3.0;
	aTemplate[6] = -3.0;
	aTemplate[7] = -3.0;
	aTemplate[8] = -3.0;

	if (!Template(NewpGryImg2, width, height, tempH, tempW, tempX, tempY, aTemplate, tempR))
	{
		return 0;
	}

	//求两幅图像最大值
	for(j = 0; j <height; j++)
	{
		for(i = 0;i <width-1; i++)
		{
			// 指向图像1倒数第j行第i个像素指针			
			lpDst1 = NewpGryImg1 + width * j + i;
			// 指向图像2倒数第j行第i个像素指针			
			lpDst2 = NewpGryImg2 + width * j + i;
			
			if(*lpDst2 > *lpDst1)
			{
				*lpDst1 = *lpDst2;
			}
		}
	}
	memcpy(pGryImg, NewpGryImg1, width * height);

	return 1;
}

int Gradient(uint16_t *pGryImg,uint16_t width,uint16_t height,uint16_t threshold)
{	
	uint16_t *lpSrc;
	uint16_t *lpDst;		
	uint16_t *NewpGryImg;		
	uint16_t *hNewGryImg;
	uint16_t i,j;				
	double result;
	uint8_t pixel[4];

	hNewGryImg = new uint16_t[width*height];
	if (hNewGryImg == NULL)
	{
		return 0;
	}
	NewpGryImg = hNewGryImg;
	lpDst = NewpGryImg;
	memset(lpDst, (uint8_t)255, width*height*2);
	for(j = height-1; j > 0; j--)
	{
		for(i = 1;i <=width-1; i++)
		{
			lpSrc = pGryImg + width * j + i;			
			lpDst = NewpGryImg + width * j + i;
			pixel[0] = (uint8_t)*lpSrc;
			pixel[1] = (uint8_t)*(lpSrc - 1);
			pixel[2] = (uint8_t)*(lpSrc - width);
			 result = sqrt(( pixel[0] - pixel[1] )*( pixel[0] - pixel[1] ) + ( pixel[0] - pixel[2] )*( pixel[0] - pixel[2] ));
			 if (result>threshold)
			 {
				 result=255;
			 }
			 if (result<threshold)
			 {
				 result=0;
			 }
			*lpDst = (uint8_t)result;		
		}
	}
	memcpy(pGryImg,NewpGryImg,width*height);
	return 1;
}