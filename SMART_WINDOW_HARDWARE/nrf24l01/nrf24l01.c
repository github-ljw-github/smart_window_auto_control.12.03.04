#include "nrf24l01.h"  
#include "stdio.h" 
const u8 TX_ADDRESS [TX_ADR_WIDTH]={0x60,0x20,0x30,0x40,0x50}; //���͵�ַ 

const u8 RX_ADDRESS0[RX_ADR_WIDTH]={0x10,0x20,0x30,0x40,0x50}; //���յ�ַ  
const u8 RX_ADDRESS1[RX_ADR_WIDTH]={0x20,0x20,0x30,0x40,0x50}; //���յ�ַ  
const u8 RX_ADDRESS2[1           ]={0x30}; //���յ�ַ  
const u8 RX_ADDRESS3[1           ]={0x40}; //���յ�ַ  
const u8 RX_ADDRESS4[1           ]={0x50}; //���յ�ַ  
const u8 RX_ADDRESS5[1           ]={0x60}; //���յ�ַ  

/*************************************************
* �������ƣ�InitNrf24L01
* ������������ʼNRF24L01��������IRQ���ŵĳ�ʼ��
* �����������
* �����������
* ����ֵ��  ����0�ɹ�������1ʧ��
**************************************************/ 
u8 InitNrf24L01(void)  
{   
    GPIO_InitTypeDef GPIO_InitStructure;  
	//ʹ��GPIO ��ʱ��              
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOG,ENABLE);    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;        //�������  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(GPIOB, &GPIO_InitStructure);   
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8;  
	GPIO_Init(GPIOG, &GPIO_InitStructure);   
    //
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //��������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  
    GPIO_Init(GPIOG, &GPIO_InitStructure);   
    //оƬʧ��
    NRF_CE_H;
    //û���ж�
    IRQ_H;
    //SPIʧ��
    CSN_H;
    //MUC��nrf���ж�����
    NrfIsrCfg();
    return Nrf24L01Check();
}  

/*************************************************
* �������ƣ�SpiReadWriteByte
* ���������������ӻ�����8λ����
* ���������- TxData���������͵�����
* �����������
* ����ֵ��  �ӻ����͵�����
**************************************************/  
u8 SpiReadWriteByte(u8 TxData)                                          
{         
    u8 i; 
    NRF_SCK_L;//����ʱ��
    for(i=0;i<8;i++)   
    {  
        if(TxData & 0x80)  
        MOSI_H;          
        else  
        MOSI_L;
        //�ƿ����λ���������λ   
        TxData = (TxData << 1);    
        //д���豸       
        NRF_SCK_H;  
        //�����豸       
        if(READ_MISO)   
        //�����ȡ������                
        TxData |= 0x01;         
        //����ʱ����.���һ�����ݽ�����SR��BUF��        
         NRF_SCK_L;                                
    }  
    return(TxData);                               
}  

/*************************************************
* �������ƣ�Nrf24l01ReadWriteBus
* ���������������ӻ���������ֽ�����
* ���������- cmd��ָ�
		    - data���������͵�����
* ���������- data���ӻ����͵�����
* ����ֵ��  ��
**************************************************/ 
void Nrf24l01ReadWriteBus(u8 cmd,u8 data[],u8 cnt)
{
	//1��ʹ��SPI����
	//2������ָ��
	//3�����͡���������
	//4��ʧ��SPI����
	u8 i;
	CSN_L;
	SpiReadWriteByte(cmd);
	for(i = 0; i < cnt; i++)
	{
		data[i] = SpiReadWriteByte(data[i]);
	}
	CSN_H;
}
/*************************************************
* �������ƣ�Nrf24L01Check
* �������������NRF24L01�Ƿ����
* �����������
* �����������
* ����ֵ��  ����0���ڣ�����1������
**************************************************/ 
u8 Nrf24L01Check(void)  
{  
    u8 a[5]={0XA0,0XA1,0XA2,0XA3,0XA4};  
    u8 b[5];  
    u8 i;          
    for(i=0;i<5;i++)b[i]=a[i];
	//д��5���ֽ�.    
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+TX_ADDR,a,5);
    //����д���5���ֽ�   
    Nrf24l01ReadWriteBus(SPI_READ_REG+TX_ADDR,a,5);  
    for(i=0;i<5;i++)
    {
        if(a[i]!=b[i]) break;    
    }    
    if(i!=5) return 1;                               //NRF24L01����λ        
    return 0;                                       //NRF24L01��λ  
}                             
/*************************************************
* �������ƣ�Nrf24L01SendPacket
* ��������������NRF24L01����һ������  
* ���������- tx_buf��Ҫ���͵�����
* �����������
* ����ֵ��  ��
**************************************************/ 
void Nrf24L01SendPacket(u8 *tx_buf)  
{  
    u8 result,data[TX_PLOAD_WIDTH];  
    //ʧ��оƬ����������ģʽ
    NRF_CE_L;  
    //���ݸ��ƣ���ϣ��ԭ���ݱ�����
    for(result=0;result<TX_PLOAD_WIDTH;result++)
		data[result]=tx_buf[result];
	//д���ݵ����ͻ�����TX BUF  32���ֽ�  
    Nrf24l01ReadWriteBus(WR_TX_PLOAD,data,TX_PLOAD_WIDTH);
	//ʹ��оƬ����������  
    NRF_CE_H;                                     
}  
                           
/*************************************************
* �������ƣ�Nrf24l01GetPacketHandler
* ����������NRF24L01���������жϴ�����    
* ���������- rx_buf���������ݵ�����
* ���������- rx_buf: ���յ�������
* ����ֵ��  ���յ�ͨ����
**************************************************/ 
u8 Nrf24l01GetPacketHandler(u8 *rx_buf)  
{  
    u8 result,channel;               
	//��ȡ״̬�Ĵ�����ֵ                                    
    Nrf24l01ReadWriteBus(SPI_READ_REG+STATUS,&result,1);
    //�õ����յ�ͨ����
    channel=((result&(7<<1))>>1);
    //��ȡ����  
    Nrf24l01ReadWriteBus(RD_RX_PLOAD,rx_buf,RX_PLOAD_WIDTH);
    //����жϱ�־λ
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+STATUS,&result,1);  
    //��ս���FIFO,��ȡ֮���Զ���գ�������ȡ��������գ�\
    ����nrf24l01�޷��������������ܲ����ж��ź�
    //Nrf24l01ReadWriteBus(FLUSH_RX,&result,1);   
    //���ؽ��յ�ͨ����
    return channel;
}  
                          
/*************************************************
* �������ƣ�Nrf24l01SendSuccessHandler
* �����������������ݳɹ��жϴ�����    
* �����������
* �����������
* ����ֵ��  ��
**************************************************/ 
void Nrf24l01SendSuccessHandler(void)
{
    u8 data;
    //��ȡ״̬�Ĵ���
    Nrf24l01ReadWriteBus(SPI_READ_REG+STATUS,&data,1);
    //��������жϱ�־λ
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+STATUS,&data,1);    
    //���TX FIFO�Ĵ���,���ͳɹ����Զ����   
    // Nrf24l01ReadWriteBus(FLUSH_TX,&data,1); 
}
                          
/*************************************************
* �������ƣ�Nrf24l01SendFailHandler
* ������������������ʧ���жϴ�����    
* �����������
* �����������
* ����ֵ��  ��
**************************************************/ 
void Nrf24l01SendFailHandler(void)
{
    u8 data;
    //��ȡ״̬�Ĵ���
    Nrf24l01ReadWriteBus(SPI_READ_REG+STATUS,&data,1);
    //��������жϱ�־λ
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+STATUS,&data,1);    
    //���TX FIFO�Ĵ���,����ʧ�ܣ������ֶ����������nrf24l01������������    
    Nrf24l01ReadWriteBus(FLUSH_TX,&data,1); 
}
                        
/*************************************************
* �������ƣ�Nrf24l01GetMode
* ��������������NRF24L01������ģʽ         
* �����������
* �����������
* ����ֵ��  ��
**************************************************/               
void Nrf24l01GetMode(void)  
{ 
	u8 data[RX_ADR_WIDTH],i;
    //оƬʧ�ܣ�ģ���������ģʽ
    NRF_CE_L;
    //дRX�ڵ��ַ��ע�⣬����д��λ��ַ�ģ�ͨ��0����ӵ�ж�����40λ��ַ��ͨ��1-5�����λ�ֽڵ�ַ��һ������������һ��
	for(i=0;i<RX_ADR_WIDTH;i++) 
		data[i]=RX_ADDRESS0[i];
	Nrf24l01ReadWriteBus(SPI_WRITE_REG+RX_ADDR_P0,data,RX_ADR_WIDTH);
	for(i=0;i<RX_ADR_WIDTH;i++) 
		data[i]=RX_ADDRESS1[i];
	Nrf24l01ReadWriteBus(SPI_WRITE_REG+RX_ADDR_P1,data,RX_ADR_WIDTH); 
	
	data[0]=RX_ADDRESS2[0];
	Nrf24l01ReadWriteBus(SPI_WRITE_REG+RX_ADDR_P2,data,1); 
	data[0]=RX_ADDRESS3[0];
	Nrf24l01ReadWriteBus(SPI_WRITE_REG+RX_ADDR_P3,data,1); 
	data[0]=RX_ADDRESS4[0];
	Nrf24l01ReadWriteBus(SPI_WRITE_REG+RX_ADDR_P4,data,1); 
	data[0]=RX_ADDRESS5[0];
	Nrf24l01ReadWriteBus(SPI_WRITE_REG+RX_ADDR_P5,data,1); 
	
    //ʹ��ͨ��0-5���Զ�Ӧ��,�����ֲ�Ĭ�϶����ǿ����ģ�ʵ�ʲ��Է��ֲ�û�У�������������������  
    data[0] = 0x3f;
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+EN_AA,data,1);
	
    //ʹ��ͨ��0-5�Ľ��յ�ַ��ͨ��0,1Ĭ�ϵĿ�����       
	Nrf24l01ReadWriteBus(SPI_WRITE_REG+EN_RXADDR,data,1);  
	data[0] = 0x3f;
	Nrf24l01ReadWriteBus(SPI_WRITE_REG+EN_RXADDR,data,1);
	
	//ѡ��ͨ��0����Ч���ݿ��         
	data[0] = RX_PLOAD_WIDTH;
	Nrf24l01ReadWriteBus(SPI_WRITE_REG+RX_PW_P0,data,1); 
	//ѡ��ͨ��1����Ч���ݿ��      
	data[0] = RX_PLOAD_WIDTH;
	Nrf24l01ReadWriteBus(SPI_WRITE_REG+RX_PW_P1,data,1);  
	//ѡ��ͨ��2����Ч���ݿ��        
	data[0] = RX_PLOAD_WIDTH;
	Nrf24l01ReadWriteBus(SPI_WRITE_REG+RX_PW_P2,data,1);  
	//ѡ��ͨ��3����Ч���ݿ��        
	data[0] = RX_PLOAD_WIDTH;
	Nrf24l01ReadWriteBus(SPI_WRITE_REG+RX_PW_P3,data,1);  
	//ѡ��ͨ��4����Ч���ݿ��       
	data[0] = RX_PLOAD_WIDTH;
	Nrf24l01ReadWriteBus(SPI_WRITE_REG+RX_PW_P4,data,1);  
	//ѡ��ͨ��5����Ч���ݿ��       
	data[0] = RX_PLOAD_WIDTH;
	Nrf24l01ReadWriteBus(SPI_WRITE_REG+RX_PW_P5,data,1);  
	
        
    //��Ƶͨ��0-6λ          
	data[0] = 0x4f;
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+RF_CH,data,1);        
    //0db����,2Mbps,���������濪��    
	data[0]=0;
    //bit3���������ʣ�1->2Mbps 0->1Mbps
    data[0]|=1<<3;
    //bit2-bit1:���书�ʣ�\
    00��-18dBm 01��-12dBm 10��-6dBm 11��0dBm
    data[0]|=0<<1;
    //bit0:�������Ŵ������棬1����0�ر�
    data[0]|=0<<0;
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+RF_SETUP,data,1);  
    data[0]=0;
    //bit6:�����жϣ�1���Σ�0������
	data[0]|=0<<6;
    //bit5:�����жϣ�1���Σ�0������
    data[0]|=0<<5;
    //bit4:�ﵽ����ط��жϣ�1���Σ�0������
	data[0]|=0<<4;
    //bit3:CRCʹ�ܣ�1ʹ�ܣ�0��ʹ��
    data[0]|=1<<3;
    //bit2:CRCģʽ��1->16λCRCУ�飬0->8λCRCУ��
    data[0]|=1<<2;
    //bit1:1�ϵ�0����
    data[0]|=1<<1;
    //bit0:1����ģʽ0����ģʽ
    data[0]|=1<<0;
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+CONFIG, data,1);   
    //��������ս���FIFO����nrf���ܲ����ж��ź�\
    �����ʱmcuʹ�ý����жϵķ�ʽ�������޷���\
    ���жϣ�IRQһֱ���ֵ͵�ƽ��ߵ�ƽ���������\
    ��־λ����һֱ���ָߵ�ƽ������һֱ���ֵ͵�\
    ƽ����ʹ�ò�ѯ��ʽ����ô����ִ����ս���\
    FIFO���п��ܵ�һ�ν��յ����������
    Nrf24l01ReadWriteBus(FLUSH_RX,data,1);
    //��ȡ״̬�Ĵ�����ֵ
    Nrf24l01ReadWriteBus(SPI_READ_REG+STATUS,data,1);
    //������еı�־λ��������빤��ģʽ��IRQһֱΪ��
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+STATUS,data,1);
    //оƬʹ��,�������ģʽ   
    NRF_CE_H;                                  
}             
                       
/*************************************************
* �������ƣ�Nrf24l01SendMode
* ��������������NRF24L01������ģʽ         
* �����������
* �����������
* ����ֵ��  ��
**************************************************/    
void Nrf24l01SendMode(void)  
{//      
	u8 data[TX_ADR_WIDTH],i;
    //��ַ���ƣ���ϣ��ԭ���ݱ�����
	for(i=0;i<TX_ADR_WIDTH;i++)
		data[i]=TX_ADDRESS[i];
    //ʧ��оƬ����������ģʽ
    NRF_CE_L;         
    //дTX�ڵ��ַ  
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+TX_ADDR,data,TX_ADR_WIDTH);      
    //����RX�ڵ��ַ,һ����ʹ��ͨ��0�ģ��ڷ��Ͷˣ��涨ʹ��ͨ��0�������ݺͽ���Ӧ���źţ��շ���ַ��һ���Ĳ�����ȷӦ��  
	for(i=0;i<TX_ADR_WIDTH;i++)
		data[i]=TX_ADDRESS[i];
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+RX_ADDR_P0,data,TX_ADR_WIDTH);      
    //ʹ��ͨ��0���Զ�Ӧ��
	data[0]=1;
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+EN_AA,data,1);      
    //ʹ��ͨ��0�Ľ��յ�ַ   
	data[0]=1;
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+EN_RXADDR,data,1);      
    //�����Զ��ط����ʱ��,����Զ��ط����� 
	data[0]=0x0;
    //bit7-bit4:�Զ��ط���ʱ��250*x+86us
    data[0]|=(0<<4);
    //bit3-bit0:�Զ��ط�����
    data[0]|=(15<<0);
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+SETUP_RETR,data,1);  
    //����RFͨ��bit0-bit6 ʵ��ͨ��Ƶ��=RF_CH+2400 (MHz)
	data[0]=0x4f;
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+RF_CH,data,1);   
    //0db����,2Mbps,���������濪��        
	data[0]=0;
    //bit3���������ʣ�1->2Mbps 0->1Mbps
    data[0]|=1<<3;
    //bit2-bit1:���书�ʣ�\
    00��-18dBm 01��-12dBm 10��-6dBm 11��0dBm
    data[0]|=0<<1;
    //bit0:�������Ŵ������棬1����0�ر�
    data[0]|=0<<0;
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+RF_SETUP,data,1);
    data[0]=0;
    //bit6:�����жϣ�1���Σ�0������
	data[0]|=0<<6;
    //bit5:�����жϣ�1���Σ�0������
    data[0]|=0<<5;
    //bit4:�ﵽ����ط��жϣ�1���Σ�0������
	data[0]|=0<<4;
    //bit3:CRCʹ�ܣ�1ʹ�ܣ�0��ʹ��
    data[0]|=1<<3;
    //bit2:CRCģʽ��1->16λCRCУ�飬0->8λCRCУ��
    data[0]|=1<<2;
    //bit1:1�ϵ�0����
    data[0]|=1<<1;
    //bit0:1����ģʽ0����ģʽ
    data[0]|=0<<0;
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+CONFIG,data,1);
    //��շ���FIFO
    Nrf24l01ReadWriteBus(FLUSH_TX,data,1);   
    //��ȡ״̬�Ĵ���
    Nrf24l01ReadWriteBus(SPI_READ_REG+STATUS,data,1);
    //��������жϱ�־λ
    Nrf24l01ReadWriteBus(SPI_WRITE_REG+STATUS,data,1);    
    //оƬʹ�ܣ����뷢��ģʽ  
    NRF_CE_H;                                    
}   
                       
/*************************************************
* �������ƣ�NrfIsrCfg
* ��������������NRF24L01 IRQ�����ж�io         
* �����������
* �����������
* ����ֵ��  ��
**************************************************/  
void NrfIsrCfg(void)
{
    //1��ʱ��ʹ�ܡ�
    //2��muc�ⲿ�ж����ŵ�����
    //3���ж����ȼ�������
    GPIO_InitTypeDef GPIO_InitStructure;  
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    //�������ù���ʱ�Ӻ�ioʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
    GPIO_Init(GPIOG,&GPIO_InitStructure);
    //�����ж�Դ
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource6);
    //�ⲿ�ж�������
    EXTI_InitStruct.EXTI_Line=EXTI_Line6;
    EXTI_InitStruct.EXTI_LineCmd=ENABLE;
    EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;//�½��ش���
    EXTI_Init( &EXTI_InitStruct);
    //���ȼ�����
    NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0x3;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
    NVIC_Init(&NVIC_InitStruct);  
}

