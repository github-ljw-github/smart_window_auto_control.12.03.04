#ifndef __DHT22_H__
#define __DHT22_H__
#include "stm32f10x.h"
//#include "timer_delay.h"
#include "delay.h"
#include "freertos.h"
/********************************************************************************************************
 * �����޸�.c�ļ���ֻ���޸�.h�ļ�����
 *******************************************************************************************************/
typedef unsigned char u8;
#define DHT22_PORT    GPIOF	//stm32����һ��˿�	    												   //IO ���ڵ���
#define DHT22_DQ_BIT 	3	//����˿ڵ���һλ														   //�ܽź�
#define DHT22_PORT_CLOCK(opration) 	(RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,opration))//ioʱ��ʹ��
#define DHT22_DELAY_MS(ms) 			delay_ms(ms)	            					   //��ʱ����
#define DHT22_DELAY_US(us) 			delay_us(us)	            					   //��ʱ����
#define DHT22_DQ_MODE_IN  8//��Ҫ��
#define DHT22_DQ_MODE_OUT 3//��Ҫ��
//IO��������
#if (DHT22_DQ_BIT>8)
#define DHT22_DQ_IN()  do{DHT22_PORT->CRH &= ~(0XF<<((DHT22_DQ_BIT-8)*4));DHT22_PORT->CRH|=(DHT22_DQ_MODE_IN <<((DHT22_DQ_BIT-8)*4));}while(0)
#define DHT22_DQ_OUT() do{DHT22_PORT->CRH &= ~(0XF<<((DHT22_DQ_BIT-8)*4));DHT22_PORT->CRH|=(DHT22_DQ_MODE_OUT<<((DHT22_DQ_BIT-8)*4));}while(0)
#else
#define DHT22_DQ_IN()  do{DHT22_PORT->CRL &= ~(0XF<<((DHT22_DQ_BIT-0)*4));DHT22_PORT->CRL|=(DHT22_DQ_MODE_IN <<((DHT22_DQ_BIT-0)*4));}while(0)
#define DHT22_DQ_OUT() do{DHT22_PORT->CRL &= ~(0XF<<((DHT22_DQ_BIT-0)*4));DHT22_PORT->CRL|=(DHT22_DQ_MODE_OUT<<((DHT22_DQ_BIT-0)*4));}while(0)
#endif
//DQ 
#define DHT22_DQ_L (DHT22_PORT->ODR &= ~(1<<DHT22_DQ_BIT))
#define DHT22_DQ_H (DHT22_PORT->ODR |=  (1<<DHT22_DQ_BIT))
#define READ_DQ  ((DHT22_PORT->IDR&(1<<DHT22_DQ_BIT))>>DHT22_DQ_BIT) //���� 


void DHT22Init(void);

u8 DHT22ReadTempeHumid(int *temp,int *humi);
 
#endif//__DHT22_H__
