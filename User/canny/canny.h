#ifndef __canny_H
#define	__canny_H

#include "stm32f4xx.h"

int Robert(uint16_t *pGryImg,int width, int height,int threshold);
int Sobel(uint16_t *pGryImg,int width,int height,int threshold);
int Laplace(uint16_t *pGryImg,int width, int height);
int Prewitt(uint16_t *pGryImg,int width, int height,int threshold);
int Kirsch(uint16_t *pGryImg,int width,int height,int threshold);
int Gradient(uint16_t *pGryImg,uint16_t width,uint16_t height,uint16_t threshold);




#endif


