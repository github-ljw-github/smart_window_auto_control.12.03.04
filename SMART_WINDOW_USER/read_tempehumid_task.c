#include "read_tempehumid_task.h"
void read_tempehumid_task(void *perameter)
{
	dp("read_tempehumid_task start");
	xQueueHandle queue_handle = (xQueueHandle)perameter;
	struct dht22msg msg;
	struct message _msg;
	int ok;
	_msg.type = E_DHT22;
	while(1)
	{
		taskENTER_CRITICAL();
		{
			ok = DHT22ReadTempeHumid(&(msg.tempe),&(msg.humid));
		}
		taskEXIT_CRITICAL();
		if(ok == 0)
		{	
			_msg.info = (void*)(&msg);
			//���¶�ʪ�����ݷ��ͳ�ȥ
			if(xQueueSendToBack(queue_handle,&_msg,100) == pdPASS)
			{
				dp("�¶�ʪ����Ϣ���ͳɹ�.");
			}
		}else
		{
			printf("��ȡ�¶�ʧ��");
		}
		vTaskDelay(pdMS_TO_TICKS(500));
		dp("��ʪ������������...");
	}
}

