
#include "dht22.h" 
static void DHT22Start(void); 

static u8 DHT22_Response(void); 	
 
static u8 DHT22ReadOneBit(void);			 

static u8 DHT22ReadOneByte(void);   

/***********************************************
 * �������ƣ�DHT22Start
 * ������������λDHT22
 * �����������
 * �����������
 * ����ֵ��  ��
************************************************/
static void DHT22Start(void)	   
{                 
	DHT22_DQ_OUT();//SET OUTPUT
	DHT22_DQ_L;//����DQ
	DHT22_DELAY_MS(1);   //����ʱ��
	DHT22_DQ_H;//�ͷ�DQ
	DHT22_DQ_IN(); //SET INPUT
}
/***********************************************
 * �������ƣ�DHT22_Response
 * �������������DHT22�Ƿ���Ӧ
 * �����������
 * �����������
 * ����ֵ��  - ����0��������Ӧ
 *		    - ����1���쳣��Ӧ
 ************************************************/
static u8 DHT22_Response(void) 	   
{   
	u8 retry=0;
	DHT22_DQ_IN();//SET INPUT	 
	while (READ_DQ&&retry<100)//DHT22������40~80us
	{
		retry++;
		DHT22_DELAY_US(1);
	}	 
	if(retry>=100)return 1;
	else retry=0;
	while (!READ_DQ&&retry<100)//DHT22���ͺ���ٴ�����40~80us
	{
		retry++;
		DHT22_DELAY_US(1);
	}
	if(retry >= 100)return 1;	    
	return 0;
}

/***********************************************
* �������ƣ�DHT22ReadOneBit
* ������������DHT22��ȡһ��λ
* �����������
* �����������
* ����ֵ��  ��ȡ��ֵ��0��1
************************************************/
static u8 DHT22ReadOneBit(void) 			 
{
 	u8 retry=0;
	while(READ_DQ&&(retry<100))//�ȴ���Ϊ�͵�ƽ
	{
		retry++;
		DHT22_DELAY_US(1);
	}
	retry=0;
	while(!READ_DQ&&(retry<100))//�ȴ���ߵ�ƽ
	{
		retry++;
		DHT22_DELAY_US(1);
	}
	DHT22_DELAY_US(40);//�ȴ�40us
	if(READ_DQ)return 1;
	else return 0;		   
}
/***********************************************
* �������ƣ�DHT22ReadOneByte
* ������������DHT22��ȡһ���ֽ�
* �����������
* �����������
* ����ֵ��  ��ȡ���ֽ�
************************************************/
static u8 DHT22ReadOneByte(void)    
{        
	u8 i,dat=0;
	for (i=0;i<8;i++) 
	{
		dat<<=1; //ע�⣺����λ
		dat|=DHT22ReadOneBit();
	}						    
	return dat;
}
/***********************************************
* �������ƣ�DHT22ReadTempeHumid
* ������������DHT22��ȡһ���ֽ�
* ���������- temp�������¶�ֵ��ַ
*			- humi������ʪ��ֵ��ַ
* ���������- tmep�������¶�ֵ��ַ
*			- humi������ʪ��ֵ��ַ
* ����ֵ��  ����0��ȡ�ɹ���1��ȡʧ��
************************************************/
u8 DHT22ReadTempeHumid(int *temp,int *humi)    
{        
 	u8 b[6];
	int i;
	DHT22Start();//���Ϳ�ʼ�ź�
	if(DHT22_Response()==0)//�ȴ���Ӧ
	{
		for(i=0;i<5;i++)//��ȡ40λ����
		{
			b[i]=DHT22ReadOneByte();
		}
		
		//У������
		b[5] = (b[0]+b[1]+b[2]+b[3]);
		if(b[5]!=b[4])return 1;// У�����
		
		*humi = (int)((b[0]<<8)|b[1]);
		*temp = (int)((b[2]<<8)|b[3]);
		return 0;
	}
	
	return 1;
}
/***********************************************
* �������ƣ�DHT22Init
* ������������ʼDHT22
* �����������
* �����������
* ����ֵ��  ��
************************************************/
void DHT22Init(void)
{
	GPIO_InitTypeDef DQ_IO_InitStruct;
	DHT22_PORT_CLOCK(ENABLE);
	DQ_IO_InitStruct.GPIO_Pin =  (1<<DHT22_DQ_BIT);
	DQ_IO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	DQ_IO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( DHT22_PORT, &DQ_IO_InitStruct );	
	DHT22_DQ_H;//DQ�߸ߵ�ƽ������״̬
}

