#ifndef __GSM_H__
#include "string.h"
#include "usart.h"
//gsmģ��ʹ�ã����ڸոճ�ʼ��Ҫ��һ��ʱ��ſ���ʹ�ã�Ӳ����ʼ����Ҫʱ��
#define gsm_send_str(s) USART_SendString(USART3,s)

#define gsm_delay_ms(ms) delay_ms(ms)

#include "stm32f10x.h"

#include "stdbool.h"

#include "delay.h"

#define GSM_RESET	do{GPIOB->ODR |= (1<<12);GPIOB->ODR &= ~(1<<12);}while(0)

void gsm_init(void);

bool gsm_send_at(u8 cmd[],u8 response[], u32 wait_ms);

s8 gsm_send_message(u8 *msg,u8 *phone);

s8 gsm_set_namecard(u8 phone[],u8 name[],u32 index);

s8 gsm_get_namecard(u8 phone[],u8 name[],u32 index);

s8 gsm_call(s8 phone[]);

#endif//__GSM_MODULE_H__
