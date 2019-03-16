#ifndef __LCD5110_H__
#define __LCD5110_H__
#include "string.h"
#include "stm32f10x.h"
#include "delay.h"
///////////////////////////
////��Ļ84*48 ��*��         
////                       
////�ַ�6*8 ��*��          
////                       
////һ��������ʾ14*6���ַ�  
///////////////////////////
//����ʽȡģ����λ��ǰ
// LCD_RST

//RST	CE	DC	DIN	CLK	VCC	BL	GND
//D9	E15	E13	E11	E9	E7	D0	D14

#define RST_L 	  GPIO_WriteBit( GPIOD,  GPIO_Pin_9, Bit_RESET )
#define RST_H     GPIO_WriteBit( GPIOD,  GPIO_Pin_9, Bit_SET )
// LCD_CE
#define CE_L  	  GPIO_WriteBit( GPIOE,  GPIO_Pin_15, Bit_RESET )
#define CE_H  	  GPIO_WriteBit( GPIOE,  GPIO_Pin_15, Bit_SET )
// LCD_DC
#define DC_L  	  GPIO_WriteBit( GPIOE,  GPIO_Pin_13, Bit_RESET )
#define DC_H  	  GPIO_WriteBit( GPIOE,  GPIO_Pin_13, Bit_SET )
// LCD_DI
#define DIN_L  	  GPIO_WriteBit( GPIOE,  GPIO_Pin_11, Bit_RESET )
#define DIN_H  	  GPIO_WriteBit( GPIOE,  GPIO_Pin_11, Bit_SET )
// LCD_CLK
#define SCK_L  	  GPIO_WriteBit( GPIOE,  GPIO_Pin_9, Bit_RESET )
#define SCK_H  	  GPIO_WriteBit( GPIOE,  GPIO_Pin_9, Bit_SET )

//#define VCC	  GPIO_WriteBit( GPIOE,  GPIO_Pin_7, Bit_SET )

// LCD_BL
#define BL_L  	  GPIO_WriteBit( GPIOD,  GPIO_Pin_0, Bit_RESET )
#define BL_H  	  GPIO_WriteBit( GPIOD,  GPIO_Pin_0, Bit_SET )

//#define GND	  GPIO_WriteBit( GPIOD,  GPIO_Pin_14, Bit_RESET )
////////////////////////////////////////////////////
#define WIDTH    6//�ַ����
#define HEIGHT   8//�ַ��߶�

void LcdIoInit(void);

void LcdReset(void);

void LcdInit(void);

void LcdWriteByte(u8 data, u8 cmd);

void LcdSetXY(u8 X, u8 Y);

void LcdClear(void);

void LcdWriteChar(u8 X,u8 Y,u8 c);

void LcdWriteString(u8 X,u8 Y,u8 *s);
 
void LcdWriteNumber(u8 x,u8 y, unsigned long number);

void LcdMoveCharRight(u8 Y,u8 *s);

void LcdMoveCharLeft(u8 Y,u8 *s);

void LcdMoveUpToDowm(u8 x,u8 *s);

void LcdShowChineseLeftToRight(u8 x,u8 y,u8 c,u8 h,u8 w,u8 chineseString[]);

void LcdShowChineseUpToDown(u8 x,u8 y,u8 c,u8 h,u8 w,u8 chineseString[]);


#endif


