#ifndef EASY_TRACER_H
#define EASY_TRACER_H

#define IMG_X 0	           //ͼƬx����
#define IMG_Y 0	           //ͼƬy����
#define IMG_W 400          //ͼƬ���
#define IMG_H 240         //ͼƬ�߶�

#define ALLOW_FAIL_PER 3   //�ݴ��ʣ�ÿ1<<ALLOW_FAIL_PER�����������һ������㣬�ݴ���Խ��Խ����ʶ�𣬵�������Խ��
#define ITERATE_NUM    8   //������������������Խ��ʶ��Խ��ȷ����������Խ��
#include "stm32f4xx.h"

typedef struct             //�ж�Ϊ��Ŀ������
{
	uint8_t  H_MIN;    //Ŀ����Сɫ��
	uint8_t  H_MAX;    //Ŀ�����ɫ��	
	
	uint8_t  S_MIN;    //Ŀ����С���Ͷ�  
	uint8_t  S_MAX;    //Ŀ����󱥺Ͷ�

	uint8_t  L_MIN;    //Ŀ����С����  
	uint8_t  L_MAX;    //Ŀ���������

	uint16_t   WIDTH_MIN; //Ŀ����С���
	uint16_t   HIGHT_MIN; //Ŀ����С�߶�

	uint16_t   WIDTH_MAX; //Ŀ�������
	uint16_t   HIGHT_MAX; //Ŀ�����߶�
}TARGET_CONDI;

typedef struct             //ʶ����
{
	uint16_t x;         //Ŀ���x����
	uint16_t y;         //Ŀ���y����
	uint16_t w;         //Ŀ��Ŀ��
	uint16_t h;         //Ŀ��ĸ߶�
}RESULT;


//-----------------------------------------------------------------------------------------
//Ψһ��API���û���ʶ������д��Conditionָ��Ľṹ���У��ú���������Ŀ���x��y����ͳ���
//����1ʶ��ɹ�������1ʶ��ʧ��
//-----------------------------------------------------------------------------------------
int Trace(const TARGET_CONDI *Condition,RESULT *Resu);

#endif
