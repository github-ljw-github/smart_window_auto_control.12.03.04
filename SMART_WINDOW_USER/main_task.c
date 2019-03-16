#include "main_task.h"

void main_task(void*paremeter)
{
	//����
	xQueueHandle queue_handle;
	TimerHandle_t timer;
	BaseType_t ok;
	timer =xTimerCreate("timer1_callback",
		pdMS_TO_TICKS(600) ,//period
		pdTRUE,		    //atuo reload 
		NULL,
		timer1_callback ); 
	if(timer != NULL)
		xTimerStart(timer,pdMS_TO_TICKS(100));

	//����һ����������������ͨ��
	queue_handle = xQueueCreate(100,sizeof(struct message));
	configASSERT(queue_handle != NULL);
	#if 1
	//������ȡ�¶�ʪ������
	ok = xTaskCreate( 
		read_tempehumid_task, //������
		"read_tempehumid_task",
		500, //ջ��� 
		queue_handle,
		1, 
		NULL );
	configASSERT(ok == pdPASS);
	#endif
	printf("read_tempehumid_task ok\r\n");
	//����ɨ���������
	ok = xTaskCreate( 
		scankey_task, 
		"scankey_task",
		500, 
		NULL,
		1, 
		&scankey_task_handle );
	configASSERT(ok == pdPASS); 
	printf("scankey_task ok\r\n");
	//������ȫ״̬���Ź�����
	ok = xTaskCreate( 
		watchdog_for_secure_state_task, 
		"watchdog_for_secure_state_task",
		128, 
		NULL,
		1, 
		&watchdog_for_secure_state_task_handle );
	configASSERT(ok == pdPASS);
	printf("watchdog_for_secure_state_task ok\r\n");
	#if 1
	//������������
	ok = xTaskCreate( 
		stealing_task, 
		"stealing_task",
		200, 
		NULL,
		1, 
		&stealing_task_handle );
	configASSERT(ok == pdPASS);
	printf("stealing_task_handle ok\r\n");
	#endif
	//�����������
	ok = xTaskCreate( 
		rainwiper_task, 
		"rainwiper_task",
		200, 
		NULL,
		1, 
		&rainwiper_task_handle );
	configASSERT(ok == pdPASS);
	vTaskSuspend( rainwiper_task_handle );
	printf("rainwiper_task_handle ok\r\n");
	//���ⱨ��
	ok = xTaskCreate( 
		flash_alarm_task, 
		"flash_alarm_task",
		100, 
		NULL,
		1, 
		&flash_alarm_task_handle );
	configASSERT(ok == pdPASS);
	vTaskSuspend( flash_alarm_task_handle );
	printf("flash_alarm_task_handle ok\r\n");
	printf("all task ok\r\n");
	
	while(1)
	{
		vTaskDelay(pdMS_TO_TICKS(10));
#if 1
		dp("main task.\r\n");
		static u32 toxicval;
		toxicval= toxicgas_getad(5);//adc1 a5
		if(is_toxicgas(toxicval) == true)//�����ж�����
		{
			set_flag(1);
			dp("�����ж�����");
			dp("�ж�����ֵ%d",toxicval);
			//���ѱ���
			BEEP_ON;
			set_toxic_beep_alarm_flag();//�ж����徯����־ 
			//�жϲ���������û��
			if(get_windowstatus() != WIN_OPEN)
			{
				//�򿪴���
				open_window();
			}
			/*�ȴ��򿪴����ɹ�*/
			while(get_windowstatus() != WIN_OPEN)
			{
				vTaskDelay(pdMS_TO_TICKS(10));
			}
			/*ֹͣ���*/
			stop_window();
			
			
			//��ɴû�д�
			if(get_dustwinstatus() != DUSTWIN_OPEN)
			{
				//��ɴ��
				open_preventdustwin(0);
				dp("�رշ���ɴ��...\n");
			}
			//�ȴ��жϴ���û��
			while(get_dustwinstatus() != DUSTWIN_OPEN)
			{
				vTaskDelay(pdMS_TO_TICKS(10));
			}
			/*ֹͣ���*/
			stop_preventdustwin();
			set_flag(0);
			
		}else
		{
			reset_toxic_beep_alarm_flag();//���ж�����
			if(without_beep_alarm_event() == true)//���κξ���
			{
				BEEP_OFF;
			}
		}
		static double dustad;
		taskENTER_CRITICAL();
		{
			dustad = dust_get_advalue();
		}
		taskEXIT_CRITICAL();
		dp("dustval%lf\r\n",dustad);
		static double dustug;
		dustug = dust_get(dustad)*1000;
		static int dustovertime = 0;
		if(is_airpolution(dustug) == true)//���ֿ�������
			dustovertime++;
		else
			dustovertime = 0;
		/*dustovertime����˼��˵�����ķ��ֿ�����Ũ�ȳ���ʱ����Ϊ�ǳ�����*/
		if(dustovertime > 50)//���ֿ�������
		{
			dp("���ֿ�������");
			//��ɴû�й�
			if(get_dustwinstatus() != DUSTWIN_CLOSE)
			{
				//�ط�������ɴ
				close_preventdustwin(0);
				dp("�رշ���ɴ��...\n");
			}
			//�ȴ��жϹر������û��
			while(get_dustwinstatus() != DUSTWIN_CLOSE)
			{
				vTaskDelay(pdMS_TO_TICKS(10));
			}
			/*ֹͣ���*/
			stop_preventdustwin();
		}       
		static struct message msg;
		static struct dht22msg thmsg;
		if(xQueueReceive(queue_handle,&msg,0) == pdPASS)//������linux�е�poll����
		{
			switch(msg.type)
			{
			case E_DHT22:// ��ʪ����Ϣ
	
				thmsg = *(struct dht22msg*)msg.info; 
					
				if(is_operatable_screen())//��Ļ��д
				{
					dp("��Ļ��д");
					show_tempe_humid_info(thmsg.tempe,thmsg.humid);
					show_dust_value(dustug);
				}else
				{
					dp("��Ļ����д");
				}
				if((is_tempeoverpass(thmsg.tempe) == true))//�¶ȳ����趨ֵ
				{
					dp("�¶ȱ���...");
					set_tempe_beep_alarm_flag();
					BEEP_ON;
				}else
				{
					reset_tempe_beep_alarm_flag();//�¶ȱ���λȡ��
					//�¶Ȼָ�����
					if(without_beep_alarm_event() == true)//���ж�������¶ȱ���
					{
						dp("�޾���");
						BEEP_OFF;	
					}
				}
			break;
			}
		}	
		static int32_t rain_val;
		rain_val = rain_getad(2);//adc1 ͨ��2
		printf("����ֵ%d\r\n",rain_val);
		if((is_rain( rain_val) == true) && (is_toxic_beep_alarm()==false))//������ ��û���ж�����
		{
			set_flag(1);
			dp("������");
			dp("����ֵ%d",  rain_val);
			/*�رմ���*/
			if(get_windowstatus() != WIN_CLOSE)
			{
				//�رմ���
				close_window();
			}
			//�ȴ��жϹر������û��
			while(get_windowstatus() != WIN_CLOSE)
			{
				printf("�ȴ������رճɹ�\r\n");
				vTaskDelay(pdMS_TO_TICKS(10));
			}
			/*ֹͣ���*/
			stop_window();
			
			
			if(is_task_suspended(rainwiper_task_handle) == true)
			{
				dp("�����������");
				//�����������
				vTaskResume( rainwiper_task_handle  );
			}
			dp("���������Ĵ�С������ε�Ƶ��");
			//���������Ĵ�С������ε�Ƶ��
			setrainwiper_break(rain_val);
			dp("�������Ƶ��Ϊ%d\r\n",rain_val);
		}else 
		{ 
			if(is_task_suspended(rainwiper_task_handle) != true)
			{
				dp("����������񣬹ر���ˢ����λ");
				
				//����������񣬹ر���ˢ����λ
				vTaskSuspend( rainwiper_task_handle );
				//�ɹ�����Ȼ��λ
				if(is_task_suspended(rainwiper_task_handle) == true)
				{
					dp("����������񣬹ر���ˢ����λ"); 
					rainwiper_reset();
					dp("��λ�ɹ�\r\n");
				}
					set_flag(0);
			}
		}
		
	#endif
	}
}
void timer1_callback(void*parameter)
{
	dp("timer1_callback start");
	static s8 f;
	f++;
	taskENTER_CRITICAL();
	{
		if(f&1)
			LcdWriteString(0,0," SmartWindow   ");
		else
			LcdWriteString(0,0,"             ");
	}
	taskEXIT_CRITICAL();
	LED0_TURN;                                 
}
bool is_airpolution(double valug)
{
	return (valug >= 75);//
} 
void set_alarm_tempe(u32 val)
{
	taskENTER_CRITICAL();
	alarm_tempe = val;
	taskEXIT_CRITICAL();
}
bool is_tempeoverpass(int tempe)
{
	return (tempe > alarm_tempe);
}
bool is_toxicgas(u32 val)
{
	return (val > 1024);	//1024/4096���̵�1/4��������
}
bool is_rain(int val)
{
	return (val < 1000);//����ʱ�͵�ƽ�����
}
void show_tempe_humid_info(u32 tempe,u32 humid)
{
	char str[20];
	taskENTER_CRITICAL();
	{
		LcdWriteString(0,4,"                 "); 	
		sprintf(str,"tempe:%d.%d     ",tempe/10,tempe%10);
		LcdWriteString(0,1,(u8*)str); 
		sprintf(str,"humid:%d.%d     ",humid/10,humid%10);
		LcdWriteString(0,2,(u8*)str); 
	}
	taskEXIT_CRITICAL();
}
void show_dust_value(double val)
{
	taskENTER_CRITICAL();
	{
		char str[20];
		sprintf(str,"dustv:%.0lf       ",val);
		LcdWriteString(0,3,(u8*)str);
	}
	taskEXIT_CRITICAL();
}
bool is_task_suspended(TaskHandle_t xTask)
{
	return (eTaskGetState( xTask ) == eSuspended);
}



