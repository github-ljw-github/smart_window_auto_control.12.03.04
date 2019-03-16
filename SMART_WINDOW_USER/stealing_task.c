#include "stealing_task.h"
#include "flash_alarm_task.h"
static bool invade_flag = false;
static int flag  = 0;

void set_flag(int i)
{
	flag = i;
}

int get_flag()
{
	return flag;
}
	

bool get_invade_flag()/*watchdog_for_secure_state_task�е���*/
{
	taskENTER_CRITICAL();
	bool ok =  invade_flag;
	taskEXIT_CRITICAL();
	return ok;
}
void set_invade_flag()/*stealing_task������*/
{
	taskENTER_CRITICAL();
	invade_flag = true;
	taskEXIT_CRITICAL();
}
void reset_invade_flag()/*flash_alarm_task�и�λ*/
{
	taskENTER_CRITICAL();
	invade_flag = false;
	taskEXIT_CRITICAL();
}
TaskHandle_t stealing_task_handle;
/*�ſؿ��ط������� */
void stealing_task(void *p)
{
	dp("stealing_task start");
	int8_t phone[12]={0};
	/*ѭ���ж��Ƿ���������*/
	while(1)
	{
		/*�ó�CPU 10����*/
		vTaskDelay(pdMS_TO_TICKS(10));
		dp("stealing_task running...\r\n");
		
		if(get_flag())continue;
		
		/*�ж��Ƿ��������ҵ��Զ���*/
		if(is_invade_cond())            
		{
			printf("�ж��Ƿ��������ҵ��Զ���\r\n");
			set_invade_flag();
			printf("stealing_task::is_invade_cond\r\n");
			
			/*������ⱨ������������״̬*/
			if(is_task_suspended(flash_alarm_task_handle))
			{
				/*�������ⱨ������*/
				vTaskResume( flash_alarm_task_handle);
				taskENTER_CRITICAL();
				{
					/*�绰����*/
					//taskENTER_CRITICAL();
					{
						flash_read(FLAHS_SAVE_ALARM_PHOME_ADDR, (uint8_t*)phone, 11);    
					}
					//taskEXIT_CRITICAL();
					if(gsm_call(phone) != 0)
					{
						if(gsm_call(phone) != 0)
						{
							if(gsm_call(phone) != 0)
							{
								dp("�绰����ʧ��, ���Զ��ű���\r\n");
								if(gsm_send_message((u8*)"Your home is dangerous.", (u8*)phone) != 0)
								{
									if(gsm_send_message((u8*)"Your home is dangerous.", (u8*)phone) != 0)
									{
										if(gsm_send_message((u8*)"Your home is dangerous.", (u8*)phone) != 0)
										{
											dp("����ʧ��\r\n");
										}else
										{
											dp("�����ɹ�\r\n");
										}
									}		
								}								
							}		
						}		
					}	
				}		
				reset_invade_flag();				
				taskEXIT_CRITICAL();

			}
		}else
		{
			
		}
	}
	/*���Բ������е�������һ��Ҫ�������ڴ˴�ɾ��������񣬷���CPU���ܷ���*/
}

/*�ж��Ƿ��������֣��Ƿ��򿪴���*/
bool is_invade_cond(void)
{
	/*	��ô�ж��ǺϷ����ǷǷ�������������δ����ʱ���ǺϷ��ģ������ǷǷ���	
		����û������˼��̣���������������ͳ���ȥ�ˣ���ʱ���������ҵ�����
		ô�������̽��������ܴ����ڷǰ�ȫ״̬������״̬�²��ᴥ���ƴ���
		��ı��������������Ҫ���ÿ��Ź���˼�룺��ʱι�������򹷽У�������
		�̣��ָ�����ȫ״̬������ζ�ʱι�����ڼ��������е�ÿһ��whileѭ��
		�ж���һ��ι����������ÿ��ι����ʱ��С�ڵ��ڶ�ʱι��ʱ�伴�ɣ�����
		�������ڲ���ϵͳ�ṩ������֪ͨ���������,
		watchdog_for_secure_state_task�������ڼ�����ܴ��Ƿ�ʱ�䴦
		�ڷǰ�ȫ״̬,feed_dog_for_secure_state��������ι����
	*/
	
	/*�жϴſؿ���������Ӧ����*/
	if(anti_steal_is_danger_signal())
	{
		return is_operatable_screen();
	}else
	{
		return false;
	}
}
