#include "anti_steal.h"
#include "stm32f10x.h"
/*��ʼ����F9���ڷ����ҵ��Թ���*/
void anti_steal_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				      
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;/*����*/ 		//MODIFY BY YPL ��Ϊ���� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOF, &GPIO_InitStructure);						
}
bool anti_steal_is_danger_signal(void)
{
	return (GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_9) == Bit_SET);/*�ſؿ�������ߵ�ƽ��������*/  
}

