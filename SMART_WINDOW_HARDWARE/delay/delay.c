//
#include "delay.h"
/**************************************************************
 * �������ƣ�delay_init
 * �������������ö�ʱ��������ʱ����
 * �����������ʱ��ʱ������Ƶ�ʣ�MHz��
 * �����������
 * ����ֵ  ����
 **************************************************************/
static u8 __SYSCLK_MHz = 72;//��ʱ������ʼ������ϵͳʱ�Ӵ�С����λMHz
extern uint32_t SystemCoreClock;
void delay_init(void)
{
	__SYSCLK_MHz = SystemCoreClock/1000000;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1, ENABLE );
	TIM_TimeBaseStructInit( &TIM_TimeBaseStructure );
	TIM_TimeBaseStructure.TIM_Period = 100;//Ĭ�ϼ���ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = __SYSCLK_MHz;//��Ƶ��΢�
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;//���¼����������ʱ����ʹ��
	TIM_TimeBaseInit( TIM1, &TIM_TimeBaseStructure );
	TIM_Cmd( TIM1, ENABLE );
}
/**************************************************************
 * �������ƣ�delay_us
 * ����������΢����ʱ����
 * �����������ʱ΢����
 * �����������
 * ����ֵ  ����
 **************************************************************/
void delay_us(unsigned int us)
{
	if(us <= 0) return;
	TIM1->PSC = __SYSCLK_MHz;
	TIM1->ARR = us;
	TIM1->CNT = us;
	while(TIM1->CNT);//ע����������������¼���ģʽ
}
/**************************************************************
 * �������ƣ�delay_ms
 * ����������������ʱ����
 * �����������ʱ������
 * �����������
 * ����ֵ  ����
 **************************************************************/
void delay_ms(unsigned int ms)
{	
	TIM1->PSC = __SYSCLK_MHz;
	TIM1->ARR = 1000;
	while(ms--)
	{	
		TIM1->CNT = 1000;
		while(TIM1->CNT);//ע����������������¼���ģʽ
	}
}

