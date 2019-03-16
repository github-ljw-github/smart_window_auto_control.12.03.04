#include <flash.h>
static uint8_t buf[FLASH_PAGE_SIZE];
void flash_write(uint32_t start_addr, uint8_t out[], uint32_t size)
{
	/*У���ַ�Ƿ�Ϸ�*/
	if((FLASH_PAGE_255_START_ADDR<=start_addr) && (start_addr<=FLASH_PAGE_255_END_ADDR))
	{
		/*��ȡ����*/
		flash_read(FLASH_PAGE_255_START_ADDR, buf, FLASH_PAGE_SIZE);
		/*���д����*/
		FLASH_Unlock();
		/* Clear All pending flags */
		FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
		/*�������һҳ*/
		FLASH_ErasePage(FLASH_PAGE_255_START_ADDR);	
		/*����д��λ���������е�ƫ����*/
		uint32_t byte_offset = start_addr - FLASH_PAGE_255_START_ADDR;
		/*д������*/
		for(int i=0; (i<FLASH_PAGE_SIZE)&&(i<size); i++)
		{
			buf[i+byte_offset] = out[i];
		}
		uint32_t data;
		/*д��flash*/
		for(int i=0; i<FLASH_PAGE_SIZE; i+=4)
		{
			data = (buf[i+3]<<24)|(buf[i+2]<<16)|(buf[i+1]<<8)|(buf[i+0]<<0);
			FLASH_ProgramWord(FLASH_PAGE_255_START_ADDR+i, data);
		}
	}
}
void flash_read(uint32_t start_addr, uint8_t out[], uint32_t size)            
{
	for(int i=0; i<size; i++)
	{
		out[i] = *(((__IO uint8_t*)start_addr)+i);
	}
}




