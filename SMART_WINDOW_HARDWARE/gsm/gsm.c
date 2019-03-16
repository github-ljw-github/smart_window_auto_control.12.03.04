#include "gsm.h"

extern u8 gsm_rxbuf[];

extern u8 gsm_index;

void gsm_init(void)
{
	GPIO_InitTypeDef  io;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	io.GPIO_Pin = GPIO_Pin_12;				      
	io.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	io.GPIO_Speed = GPIO_Speed_50MHz;		
	//gsm ��λ����
	GPIO_Init(GPIOB, &io);	
	GPIOB->ODR &= ~(1<<12);
	
	USART3_Init(115200);
}

/************************************************************************************
 * �������ƣ�gsm_send_at
 * ����������gsmģ�鷢��ָ��
 * ���������- cmd��ָ������س����У�
 *	    - response��������Ӧ�ַ���
 *	    - wait_ms���ȴ���Ӧ��ʱ�� ��λ/����
 * �����������
 * �������أ�����ָ��ɹ�true ����false
 ************************************************************************************/
bool gsm_send_at(u8 cmd[],u8 response[],u32 wait_ms)
{
	//���ô��ڽ�����
	memset(gsm_rxbuf,'\0',GSM_RXBUF_SIZE);
	gsm_index 	= 0;
	//��������
	gsm_send_str(cmd);
	//��������Ƿ�ɹ�ִ��
	while(wait_ms != 0)
	{
		if(strstr((char*)gsm_rxbuf,(char*)response) != NULL)
		{
			return true;
		}
		gsm_delay_ms(2);
		wait_ms--;
	}
	return false;
}
/************************************************************************************
 * �������ƣ�gsm_send_message
 * ����������gsmģ�鷢����Ϣ
 * ���������- msg����Ϣ���ݣ�Ӣ�ģ�
 *	    - phone��Ŀ��绰����
 * �����������
 * �������أ����ͳɹ�true ����false
 ************************************************************************************/
s8 gsm_send_message(u8 *msg,u8 *phone)
{
	u8 end[2]={0x1A,0};//�������Ͷ���
	u8 tmp[50];

	//����ΪTEXTģʽ,����Ӣ�Ķ���
	if(gsm_send_at("AT+CMGF=1\r\n","OK",49) == false)
		return -1;
	
	//�����ַ���ʽ
	if(gsm_send_at("AT+CSCS=\"GSM\"\r\n","OK",49) == false)
		return -2;
	
	//���������˺���
	sprintf((char*)tmp,"AT+CMGS=\"%s\"\r\n",phone);
	if(gsm_send_at(tmp,">",49) == false)
		return -3;
	
	//д��������
	gsm_send_str(msg);
	
	//��������
	if(gsm_send_at(end,"OK",49) == false)
		return -4;
	return 0;
}
/************************************************************************************
 * �������ƣ�gsm_set_namecard
 * ����������������Ƭ��Ϣ
 * ���������- phone���绰����
 *	    - name������
 *	    - index���洢����
 * �����������
 * �������أ����óɹ�true ����false
 ************************************************************************************/
s8 gsm_set_namecard(u8 phone[],u8 name[],u32 index)
{
	if(gsm_send_at("AT+CPBS=\"SM\"\r\n","OK",49) != true)
	{
		//printf("buf is %s\r\n",usart3_rxbuf);
		//printf("fail at AT+CPBS.\n");
		return -1;
	}
	u8 buf[50];
	sprintf((char*)buf,"AT+CPBW=%d,\"%s\",129,\"%s\"\r\n",index,phone,name);
	
	//printf("dddddddddddddddddddddd---->%s\r\n",buf);
	if(gsm_send_at(buf,"OK",49) != true)
		return -2;
	return 0;
}
/************************************************************************************
 * �������ƣ�gsm_get_namecard
 * ������������ȡ��Ƭ��Ϣ
 * ���������- index���洢���� 
 * ���������- phone���绰����
 *	    - name������
 * �������أ���ȡ�ɹ�true ����false
 ************************************************************************************/
s8 gsm_get_namecard(u8 phone[],u8 name[],u32 index)
{
	u8 *s,buf[50],i;
	//ѡ��洢λ��
	if(gsm_send_at("AT+CPBS=\"SM\"\r\n","OK",49) != true)
	{
		//printf("fail at AT+CPBS\r\n");
		return -1;
	}
	//λ��
	sprintf((char*)buf,"AT+CPBR=%d\r\n",index);
	if(gsm_send_at(buf,"\",",49) != true)
	{
		//printf("buf is %s\r\n",usart3_rxbuf);
		//printf("fail at AT+CPBR");
		return -2;
	}
	//��õ绰���� ����
	s = (u8*)strstr((char*)gsm_rxbuf,",\"");
	if(s != NULL)
	{
		//printf("dddddd---------%s\r\n",s);
		s += 2;//�绰�����˵�ƫ��������ӡ��������֪����ô����
		for(i=0; i<11; i++)
			phone[i] = *s++;
		//printf("phone:%s----\r\n",phone);
		phone[12] = 0;
		//printf("phone:%s----\r\n",phone);
		//return true;
		gsm_delay_ms(10);//�ȴ�����ɣ�gsmģ�鷢����Ƭ��Ϣ����Ƭ����Ҫʱ�䣬���ȴ����ܵò�������
		s = (u8*)strstr((char*)s,(char*)",\"");
		s += 2;
		while(*s != '\"')
			*name++ = *s++;
		//printf("phone:%s----\r\n",phone);
		return 0;
	}
	
	return 0;
}
s8 gsm_call(s8 phone[])
{
	char buf[50];
	sprintf(buf,"ATD%s\r\n",phone);
	bool ok = gsm_send_at((u8*)buf,"OK",49);
	if(ok)
		return 0;
	return -1;
}
/*///////////////////////////////////////////////////////////////
AT+CPBS=? ��ѯ֧�ֵĴ洢λ��
AT+CPBS="SM" ѡ��洢λ��"SM"
AT+CPBW=? ��ѯ֧��д��Ƭ��λ��
AT+CPBW=1,"13570574320",129,"LJW" д��Ƭ��1��λ�ã�129������ڵ绰
AT+CPBR=? ��ѯ֧�ֵ�λ�ã����λ�ô������Ƭ
AT+CPBR=1 ��ѯ1��λ�õ���Ƭ��Ϣ
AT+CPBF="LJW" �ҹؼ�����"LJW"����Ƭ��Ϣ
///////////////////////////////////////////////////////////////
AT+CSCS=? �鿴֧�ֵ��ַ���
AT+CSCS="GSM" ����Ϊ"GSM"��ʽ 
AT+CSCS="UCS32"
AT+CMGF=1���ö��Ÿ�ʽ
AT+CMGR=1��ȡ����
///////////////////////////////
ATD10086 ����10086
ATH �Ҷϵ绰
*/

