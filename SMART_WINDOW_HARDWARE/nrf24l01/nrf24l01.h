#ifndef _NRF24L01_H  
#define _NRF24L01_H  
#include "stm32f10x.h" 
/****************************����************************************************************************/  
#define RD_RX_PLOAD     0x61  //��RX��Ч����,1~32�ֽ�  
#define WR_TX_PLOAD     0xA0  //дTX��Ч����,1~32�ֽ�  
#define REUSE_TX_PL     0xE3  //����ʹ����һ������,CEΪ��,���ݰ������Ϸ���.  
#define NOP             0xFF  //�ղ���,����������״̬�Ĵ���
#define FLUSH_TX        0xE1  //���TX FIFO�Ĵ���.����ģʽ����  
#define FLUSH_RX        0xE2  //���RX FIFO�Ĵ���.����ģʽ����  
 
#define SPI_READ_REG    0x00  //���Ĵ���,��5λΪ�Ĵ�����ַ  
#define SPI_WRITE_REG   0x20  //д�Ĵ���,��5λΪ�Ĵ�����ַ  
 
//SPI(NRF24L01)�Ĵ�����ַ  
#define CONFIG          0x00  //���üĴ�����ַ;bit0:1����ģʽ,0����ģʽ;bit1:�ϵ�ѡ��;bit2:CRCģʽ;bit3:CRCʹ��;  
                              //bit4:�ж�MAX_RT(�ﵽ����ط������ж�)ʹ��;bit5:�ж�TX_DSʹ��;bit6:�ж�RX_DRʹ��  
#define EN_AA           0x01  //ʹ���Զ�Ӧ����  bit0~5,��Ӧͨ��0~5  
#define EN_RXADDR       0x02  //���յ�ַ����,bit0~5,��Ӧͨ��0~5  
#define SETUP_AW        0x03  //���õ�ַ���(��������ͨ��):bit1,0:00,3�ֽ�;01,4�ֽ�;02,5�ֽ�;  
#define SETUP_RETR      0x04  //�����Զ��ط�;bit3:0,�Զ��ط�������;bit7:4,�Զ��ط���ʱ 250*x+86us  
#define RF_CH           0x05  //RFͨ��,bit6:0,����ͨ��Ƶ��;  
#define RF_SETUP        0x06  //RF�Ĵ���;bit3:��������(0:1Mbps,1:2Mbps);bit2:1,���书��;bit0:�������Ŵ�������  
#define STATUS          0x07  //״̬�Ĵ���;bit0:TX FIFO����־;bit3:1,��������ͨ����(���:6);bit4,�ﵽ�����ط�  
                              //bit5:���ݷ�������ж�;bit6:���������ж�;  
  
#define OBSERVE_TX      0x08  //���ͼ��Ĵ���,bit7:4,���ݰ���ʧ������;bit3:0,�ط�������  
#define CD              0x09  //�ز����Ĵ���,bit0,�ز����;  
#define RX_ADDR_P0      0x0A  //����ͨ��0���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ  
#define RX_ADDR_P1      0x0B  //����ͨ��1���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ  
#define RX_ADDR_P2      0x0C  //����ͨ��2���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;  
#define RX_ADDR_P3      0x0D  //����ͨ��3���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;  
#define RX_ADDR_P4      0x0E  //����ͨ��4���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;  
#define RX_ADDR_P5      0x0F  //����ͨ��5���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;  
#define TX_ADDR         0x10  //���͵�ַ(���ֽ���ǰ),ShockBurstTMģʽ��,RX_ADDR_P0��˵�ַ���  
#define RX_PW_P0        0x11  //��������ͨ��0��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�  
#define RX_PW_P1        0x12  //��������ͨ��1��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�  
#define RX_PW_P2        0x13  //��������ͨ��2��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�  
#define RX_PW_P3        0x14  //��������ͨ��3��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�  
#define RX_PW_P4        0x15  //��������ͨ��4��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�  
#define RX_PW_P5        0x16  //��������ͨ��5��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�  
#define FIFO_STATUS     0x17  //FIFO״̬�Ĵ���;bit0,RX FIFO�Ĵ����ձ�־;bit1,RX FIFO����־;bit2,3,����  
                              //bit4,TX FIFO�ձ�־;bit5,TX FIFO����־;bit6,1,ѭ��������һ���ݰ�.0,��ѭ��;  
//һЩ��־λ
#define MAX_TX          (1<<4)//0x10  //�ﵽ����ʹ����ж�  
#define TX_OK           (1<<5)//0x20  //TX��������ж�  
#define RX_OK           (1<<6)//0x40  //���յ������ж� 
/**********************************************************************************************************/  
  
//NRF2401Ƭѡ�ź�  RX��TXģʽ����
#define         NRF_CE_L      GPIO_WriteBit(GPIOG, GPIO_Pin_8,Bit_RESET)  
#define         NRF_CE_H      GPIO_WriteBit(GPIOG, GPIO_Pin_8,Bit_SET)  
  
//SPIƬѡ�ź�     
#define         CSN_L         GPIO_WriteBit(GPIOG, GPIO_Pin_7,Bit_RESET)  
#define         CSN_H         GPIO_WriteBit(GPIOG, GPIO_Pin_7,Bit_SET)  
  
//SPIʱ��  
#define         NRF_SCK_L     GPIO_WriteBit(GPIOB , GPIO_Pin_13,Bit_RESET)  
#define         NRF_SCK_H     GPIO_WriteBit(GPIOB , GPIO_Pin_13,Bit_SET)  
  
//SPI���  
#define         MOSI_L        GPIO_WriteBit(GPIOB , GPIO_Pin_15,Bit_RESET)  
#define         MOSI_H        GPIO_WriteBit(GPIOB , GPIO_Pin_15,Bit_SET)  
  
//SPI����  
#define         READ_MISO     GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)  
     
//IRQ�жϽ�   
#define         IRQ_L         GPIO_WriteBit(GPIOG,GPIO_Pin_6,Bit_RESET)   
#define         IRQ_H         GPIO_WriteBit(GPIOG,GPIO_Pin_6,Bit_SET)  
  
#define         READ_IRQ      GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_6)  
  
//NRF24L01���ͽ������ݿ�ȶ���  
#define TX_ADR_WIDTH    5                   //5�ֽڵĵ�ַ���  
#define RX_ADR_WIDTH    5                   //5�ֽڵĵ�ַ���  
#define TX_PLOAD_WIDTH  32                  //���ݿ��  
#define RX_PLOAD_WIDTH  32                  //���ݿ��  
                                             
u8 InitNrf24L01(void);    

u8 SpiReadWriteByte(u8 TxData) ;   

u8 Nrf24L01Check(void); 

void Nrf24l01ReadWriteBus(u8 cmd,u8 data[],u8 cnt);
  
void Nrf24l01GetMode(void);            
               
void Nrf24l01SendMode(void);        
                            
void Nrf24L01SendPacket(u8 *txbuf);    
                       
u8 Nrf24l01GetPacketHandler(u8 *rxbuf);

void Nrf24l01SendSuccessHandler(void);

void Nrf24l01SendFailHandler(void);

void NrfIsrCfg(void);

#endif  

