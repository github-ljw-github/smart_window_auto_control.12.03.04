#include "stm32f10x.h"
#include "pwm.h"
#include "delay.h"
#include "dp.h"
void pwm_init(void)
{
	GPIO_InitTypeDef PWM_IO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);
	
	PWM_IO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//���ù���
	PWM_IO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	PWM_IO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, & PWM_IO_InitStruct);
		
	PWM_IO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_Init( GPIOC, & PWM_IO_InitStruct);
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,  ENABLE);//��ӳ��
	
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;//���¼���ģʽ
	TIM_TimeBaseInitStruct.TIM_Period = 0xFFFF;//�����趨�����
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72-1;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//�������ֵ��������pwm������Ϊ�����ʧ��
	
	TIM_TimeBaseInit( TIM2, & TIM_TimeBaseInitStruct);
	TIM_TimeBaseInit( TIM3, & TIM_TimeBaseInitStruct);

	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;//������¼���
	
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OCInitStruct.TIM_OutputState = ENABLE;
	TIM_OCInitStruct.TIM_OutputNState = DISABLE;
	/*TIM_SetCompare1( TIM2, 1100+900);
		TIM_SetCompare2( TIM2, 550);
		
		TIM_SetCompare3( TIM2, 520);
		TIM_SetCompare4( TIM2,  500+900);
	 */
	TIM_OCInitStruct.TIM_Pulse = 2000;//��׼����
	TIM_OC1Init( TIM2, & TIM_OCInitStruct);//a0
	TIM_OCInitStruct.TIM_Pulse = 550;//��׼����
	TIM_OC2Init( TIM2, & TIM_OCInitStruct);//a1
	TIM_OCInitStruct.TIM_Pulse = 520;//��׼����
	TIM_OC3Init( TIM2, & TIM_OCInitStruct);//a2
	TIM_OCInitStruct.TIM_Pulse = 500+900;//��׼����
	TIM_OC4Init( TIM2, & TIM_OCInitStruct);//a3
	
	TIM_OCInitStruct.TIM_Pulse = 860;//��׼����
	TIM_OC1Init( TIM3, & TIM_OCInitStruct);//c6
	TIM_OCInitStruct.TIM_Pulse = 1800;//��׼����
	TIM_OC2Init( TIM3, & TIM_OCInitStruct);//c7

	TIM_OC3Init( TIM3, & TIM_OCInitStruct);//c8
	TIM_OC4Init( TIM3, & TIM_OCInitStruct);//c9
	
	TIM_Cmd( TIM2, ENABLE);
	TIM_Cmd( TIM3, ENABLE);
	delay_ms(1000);
	dp("�����λ");
	delay_ms(1000);
	TIM_Cmd( TIM2, DISABLE);
	TIM_Cmd( TIM3, DISABLE);
	
	////////////////////// ���ϵ������˼���趨���ںܴ�pwm��ʹ�ö��������λ����һ���ӹ�λŪ������/////////////////////////////////////////////////
	/////////////����ȥ��������Ҫ���õ�ֵ//////////////////////////////////////////
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;//���¼���ģʽ
	TIM_TimeBaseInitStruct.TIM_Period = 10000-1;//���ڿ��ƶ�������һЩ�����ٶ�Ҫ��û��ô�ߣ�̫СҲ��̫�ã�������������Ҳ���׿���
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72-1;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//�������ֵ��������pwm������Ϊ�����ʧ��
	
	TIM_TimeBaseInit( TIM2, & TIM_TimeBaseInitStruct);
	TIM_TimeBaseInit( TIM3, & TIM_TimeBaseInitStruct);

	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;
	
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OCInitStruct.TIM_OutputState = ENABLE;
	TIM_OCInitStruct.TIM_OutputNState = DISABLE;
	/*
	TIM_SetCompare1( TIM2, 1100+900);
		TIM_SetCompare2( TIM2, 550);
		
		TIM_SetCompare3( TIM2, 520);
		TIM_SetCompare4( TIM2,  500+900);
		
		TIM_SetCompare1( TIM2, 1100+900);
		TIM_SetCompare2( TIM2, 550);
		
		TIM_SetCompare3( TIM2, 520);
		TIM_SetCompare4( TIM2,  500+900);
	
	*/
	TIM_OCInitStruct.TIM_Pulse = 1100+900;//��׼����
	TIM_OC1Init( TIM2, & TIM_OCInitStruct);//a0
	TIM_OCInitStruct.TIM_Pulse = 550;//��׼����
	TIM_OC2Init( TIM2, & TIM_OCInitStruct);//a1
	TIM_OCInitStruct.TIM_Pulse = 520;//��׼����
	TIM_OC3Init( TIM2, & TIM_OCInitStruct);//a2
	TIM_OCInitStruct.TIM_Pulse = 500+900;//��׼����
	TIM_OC4Init( TIM2, & TIM_OCInitStruct);//a3
	
	TIM_OCInitStruct.TIM_Pulse = 860;//���ͨ���趨�ĳ�ʼ����
	TIM_OC1Init( TIM3, & TIM_OCInitStruct);//c6	
	TIM_OCInitStruct.TIM_Pulse = 1800;//���ͨ���趨�ĳ�ʼ����
	TIM_OC2Init( TIM3, & TIM_OCInitStruct);//c7
	TIM_OCInitStruct.TIM_Pulse = 0;//��׼���� 
	TIM_OC3Init( TIM3, & TIM_OCInitStruct);//c8
	TIM_OC4Init( TIM3, & TIM_OCInitStruct);//c9
	
	TIM_Cmd( TIM2, ENABLE);
	TIM_Cmd( TIM3, ENABLE);
}



