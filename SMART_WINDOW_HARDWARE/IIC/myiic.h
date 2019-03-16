#ifndef __MYIIC_H
#define __MYIIC_H
#include "delay.h" 
#define IIC_PORT    GPIOC		    												   //IO ���ڵ���
#define IIC_SCL_BIT 	9															   //�ܽź�
#define IIC_SDA_BIT 	8															   //�ܽź�
#define IIC_PORT_CLOCK(opration) 	(RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,opration))//ʱ��ʹ��
#define IIC_DELAY_US(us) 			delay_us(us)		            					   //��ʱ����
#define IIC_SDA_MODE_IN  8
#define IIC_SDA_MODE_OUT 3
//IO��������
#if (IIC_SDA_BIT>8)
#define IIC_SDA_IN()  {IIC_PORT->CRH &= ~(0XF<<((IIC_SDA_BIT-8)*4));IIC_PORT->CRH|=(IIC_SDA_MODE_IN <<((IIC_SDA_BIT-8)*4));}
#define IIC_SDA_OUT() {IIC_PORT->CRH &= ~(0XF<<((IIC_SDA_BIT-8)*4));IIC_PORT->CRH|=(IIC_SDA_MODE_OUT<<((IIC_SDA_BIT-8)*4));}
#else
#define IIC_SDA_IN()  {IIC_PORT->CRL &= ~(0XF<<((IIC_SDA_BIT-0)*4));IIC_PORT->CRL|=(IIC_SDA_MODE_IN <<((IIC_SDA_BIT-0)*4));}
#define IIC_SDA_OUT() {IIC_PORT->CRL &= ~(0XF<<((IIC_SDA_BIT-0)*4));IIC_PORT->CRL|=(IIC_SDA_MODE_OUT<<((IIC_SDA_BIT-0)*4));}
#endif
//SCL
#define IIC_SCL_L (IIC_PORT->ODR &= ~(1<<IIC_SCL_BIT))
#define IIC_SCL_H (IIC_PORT->ODR |=  (1<<IIC_SCL_BIT))
//SDA	 
#define IIC_SDA_L (IIC_PORT->ODR &= ~(1<<IIC_SDA_BIT))
#define IIC_SDA_H (IIC_PORT->ODR |=  (1<<IIC_SDA_BIT))
#define READ_SDA  ((IIC_PORT->IDR&(1<<IIC_SDA_BIT))>>IIC_SDA_BIT) //����SDA 
//IIC���в�������
void IIC_Init(void);                                                           //��ʼ��IIC��IO��				 
void IIC_Start(void);				                                           //����IIC��ʼ�ź�
void IIC_Stop(void);	  			                                           //����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			                                           //IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);                                           //IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				                                           //IIC�ȴ�ACK�ź�
void IIC_Ack(void);					                                           //IIC����ACK�ź�
void IIC_NAck(void);				                                           //IIC������ACK�ź�
#endif
