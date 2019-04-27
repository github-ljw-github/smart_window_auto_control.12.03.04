#include "watchdog_for_secure_state_task.h"
extern bool get_invade_flag();
extern TaskHandle_t scankey_task_handle;
TaskHandle_t watchdog_for_secure_state_task_handle;

#define WATCHDOG_TASK_SET_WINDOW_SELF_OPERATION (0x1<<2)
extern void set_windows_self_operation_flag(uint8_t val);
extern void reset_windows_self_operation_flag(uint8_t val);
void watchdog_for_secure_state_task(void *p)/*������������ǽ���ʱ�䴦�ڽ���״̬�����ް��������ļ���������ͬʱ����������*/
{
	while(1)
	{
		vTaskDelay(pdMS_TO_TICKS(100));
	}
	dp("watchdog_for_secure_state_task start");
	/* The rate at which the periodic task generates software interrupts. */
	const TickType_t xMaxExpectedBlockTime = pdMS_TO_TICKS( 8000UL );
	uint32_t ulEventsToProcess;
	eTaskState task_state;
	/*��ɨ��������񴴽��ɹ���ż���ִ��*/
	while((eTaskGetState( scankey_task_handle ) != eReady))
	{
		dp("��ɨ��������񴴽��ɹ���ż���ִ��.\r\n");
		vTaskDelay(pdMS_TO_TICKS(100));
	}
	while(1)
	{	
		ulEventsToProcess = ulTaskNotifyTake( pdTRUE, xMaxExpectedBlockTime );
		if( ulEventsToProcess == 0 )
		{
			if(keyboard_is_locked()==false)/*���̴��ڽ���״̬*/
			{
				if(get_invade_flag()==false)/*û��������������*/
				{
					dp("����\r\n");
					dp("kill scankey_task");
					/*	�����ｫ���ܴ��ָ�����ȫ״̬	
						����ɱ��ɨ���������Ȼ����
						���´�����������
					*/
					vTaskDelete( scankey_task_handle );
					/*ȷ��������ȷɾ����*/
					task_state = eTaskGetState(  scankey_task_handle );
					if( (task_state != eReady		) && \
						(task_state != eBlocked		) && \
						(task_state != eSuspended	) && \
						(task_state != eRunning		)	 \
					  )
					{
						dp("ɾ������ɹ�\r\n");
						if(get_windowstatus() != WIN_CLOSE)
						{
							set_windows_self_operation_flag(WATCHDOG_TASK_SET_WINDOW_SELF_OPERATION);
							{	
								/*�Ѵ����ر�*/
								close_window();
								/*�ȴ������ر����*/
								while(get_windowstatus() != WIN_CLOSE)        
								{
									vTaskDelay(pdMS_TO_TICKS(10));
								}
								stop_window();
							}
							reset_windows_self_operation_flag(~WATCHDOG_TASK_SET_WINDOW_SELF_OPERATION);
						}
						//����ɨ���������
						xTaskCreate( 
							scankey_task, 
							"scankey_task",
							500, 
							NULL,
							1, 
							NULL );
						dp("����scankey_task�ɹ�\r\n");
					}else
					{
						dp("ɾ������ʧ��\r\n");
					}
				}
			}
		}
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}
void feed_dog_for_secure_state(void)
{
	/*��ֵ֪ͨ��һ����*/
	xTaskNotifyGive( watchdog_for_secure_state_task_handle);
}
