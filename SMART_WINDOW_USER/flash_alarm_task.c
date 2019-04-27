
#include "flash_alarm_task.h"
u8 alarm = 0;//λ0�¶ȱ�����־��λ1�ж����屨��,Ϊ2���ⱨ����־
TaskHandle_t flash_alarm_task_handle;
extern bool get_invade_flag(void);
void flash_alarm_task(void*parameter)
{
	static int i = 0;
	while(1)
	{
		vTaskDelay(pdMS_TO_TICKS(10));
		if(get_invade_flag())
		{
			LED0_TURN;
			BEEP_TURN;
			//����������
			VOICE_ON;
			printf("flash_alarm_task::��������\r\n");
			i++;
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
			}
		}else
		{
			/*��λ��������*/
			reset_invade_flag();
			//��λ���ⱨ����־
			reset_flash_beep_alarm_flag();
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
