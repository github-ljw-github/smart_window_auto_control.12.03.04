#include "toxicgas.h"
/*********************************************************************************************************************
 * �������ƣ�toxicgas_init
 * ������������ʼ���ж�������
 * �����������
 * �����������
 * �������أ���
 *********************************************************************************************************************/
void toxicgas_init(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef io;	
	//adcʱ�ӷ�Ƶ
	RCC_ADCCLKConfig(RCC_PCLK2_Div2); 
	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//AD ����
	io.GPIO_Pin = GPIO_Pin_5;
	io.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &io);
	
	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ADC1���С�ADC2\ADC3��������
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ͨ��
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//������ת������Ҫһ�δ���һ�μ���
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//���ⲿ����
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//����λ�Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);
	/* Enable ADC1 reset calibration register */   
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));
	/* Start ADC1 calibration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));
	/* Start ADC1 Software Conversion */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);  
}
u32 toxicgas_getad(u8 ch)
{
	int i = 2;
	u16 t;
	while(i--)//��֪��ΪʲôҪ�����������β���
	{
		ADC_RegularChannelConfig(ADC1,ch,1, ADC_SampleTime_239Cycles5);
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);  
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������ 
		t = ADC_GetConversionValue(ADC1);//�������һ�� ADC1 �������ת�����  ;
	}
	return t;
}
