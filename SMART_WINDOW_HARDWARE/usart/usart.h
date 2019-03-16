#ifndef __USART_H_
#define __USART_H_


#include "stdio.h"
#include "stm32f10x.h"
#define GSM_RXBUF_SIZE 100

void USART1_Init(u32 bound);//����1��ʼ������
void USART2_Init(u32 bound);//����2��ʼ������
void USART3_Init(u32 bound);//����3��ʼ������
void UART4_Init (u32 bound);//����4��ʼ������
void UART5_Init (u32 bound);//����5��ʼ������
//���ڷ���һ���ֽ�
void USART_SendByte(USART_TypeDef *USART_COM,u8 c);
//���ڷ������麯��
void USART_SendBuf(USART_TypeDef *USART_COM,unsigned char *buf,u16 len);
//���ڷ����ַ�������
void USART_SendString(USART_TypeDef *USART_COM,unsigned char *s);




















#endif


