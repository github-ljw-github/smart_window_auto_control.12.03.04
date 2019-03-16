
#include "flash_alarm_task.h"
#include "window.h"
u8 alarm = 0;//λ0�¶ȱ�����־��λ1�ж����屨��,Ϊ2���ⱨ����־
TaskHandle_t flash_alarm_task_handle;
void flash_alarm_task(void*parameter)
{
	static int i = 0;
	while(1)
	{
		LED0_TURN;
		BEEP_TURN;
		//����������
		VOICE_ON;
		vTaskDelay(pdMS_TO_TICKS(200));
		printf("flash_alarm_task::��������\r\n");
		i++;
		//if(i > 1*30*1000/200)
		if(i > 1*10*1000/200)
		{
			i = 0;
			//����������
			VOICE_OFF;
			LED0_OFF;
			BEEP_OFF;
			//��λ���ⱨ����־
			reset_flash_beep_alarm_flag();
			/*��λ��������*/
			reset_invade_flag();
			//�����Լ�
			vTaskSuspend( flash_alarm_task_handle );
			
			
			
			/*�Ѵ����ر�*/
			close_window();
			/*�ȴ������ر����*/
			while(get_windowstatus() != WIN_CLOSE)        
			{
				vTaskDelay(pdMS_TO_TICKS(10));
			}
			stop_window();
			/*�ȶ�һ�£���ֹ�����ִ�������*/
			vTaskDelay(pdMS_TO_TICKS(1000));
		}
	}
}

bool without_beep_alarm_event(void)
{
	bool ok;
	taskENTER_CRITICAL();
	if((alarm&ALARM_MASK) == 0)
		ok = true;
	else 
		ok = false;
	taskEXIT_CRITICAL();
	return ok;
}
void set_flash_beep_alarm_flag(void)
{
	taskENTER_CRITICAL();
	alarm |= FLASH_ALARM_MASK;
	taskEXIT_CRITICAL();
}
void reset_flash_beep_alarm_flag(void)
{
	taskENTER_CRITICAL();
	alarm &= ~FLASH_ALARM_MASK;
	taskEXIT_CRITICAL();
}

void set_toxic_beep_alarm_flag(void)
{
	taskENTER_CRITICAL();
	alarm |= TOXIC_ALARM_MASK;
	taskEXIT_CRITICAL();
}
bool is_toxic_beep_alarm(void)
{
	bool ok = false;
	taskENTER_CRITICAL();
	if((alarm & TOXIC_ALARM_MASK) != 0)
		ok = true;
	taskEXIT_CRITICAL();
	return ok;
}
void set_tempe_beep_alarm_flag(void)
{
	taskENTER_CRITICAL();
	alarm |= TEMPE_ALARM_MASK;
	taskEXIT_CRITICAL();
}
void reset_toxic_beep_alarm_flag(void)
{
	taskENTER_CRITICAL();
	alarm &= ~TOXIC_ALARM_MASK;
	taskEXIT_CRITICAL();
}
void reset_tempe_beep_alarm_flag(void)
{
	taskENTER_CRITICAL();
	alarm &= ~TEMPE_ALARM_MASK;
	taskEXIT_CRITICAL();
}
