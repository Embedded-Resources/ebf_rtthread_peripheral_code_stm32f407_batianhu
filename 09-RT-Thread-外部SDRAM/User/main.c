/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   RT-Thread 3.0 + STM32 ����ģ��
  *********************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-���컢 STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  **********************************************************************
  */ 
 
/*
*************************************************************************
*                             ������ͷ�ļ�
*************************************************************************
*/ 
#include "board.h"
#include "rtthread.h"


/*
*************************************************************************
*                               ����
*************************************************************************
*/
/* �����߳̿��ƿ� */
static rt_thread_t led1_thread = RT_NULL;


typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* ��ȡ�������ĳ��� */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define  BufferSize (countof(Tx_Buffer)-1)

#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress

   
/*���Զ�λ��ʽ����SRAM,���ַ�ʽ���붨���ȫ�ֱ���*/
uint8_t testValue __attribute__((at(Bank1_SRAM4_ADDR)));

/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void led1_thread_entry(void* parameter);


/*
*************************************************************************
*                             main ����
*************************************************************************
*/
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{	
    /* 
	 * ������Ӳ����ʼ����RTTϵͳ��ʼ���Ѿ���main����֮ǰ��ɣ�
	 * ����component.c�ļ��е�rtthread_startup()����������ˡ�
	 * ������main�����У�ֻ��Ҫ�����̺߳������̼߳��ɡ�
	 */
	
	led1_thread =                          /* �߳̿��ƿ�ָ�� */
    rt_thread_create( "led1",              /* �߳����� */
                      led1_thread_entry,   /* �߳���ں��� */
                      RT_NULL,             /* �߳���ں������� */
                      512,                 /* �߳�ջ��С */
                      3,                   /* �̵߳����ȼ� */
                      20);                 /* �߳�ʱ��Ƭ */
                   
    /* �����̣߳��������� */
   if (led1_thread != RT_NULL)
        rt_thread_startup(led1_thread);
    else
        return -1;
}

/*
*************************************************************************
*                             �̶߳���
*************************************************************************
*/

static void led1_thread_entry(void* parameter)
{	
	rt_kprintf ( "\r\n�����ⲿ SRAM ����\r\n" );
	
  /*����������ʾ���ڶ�дSRAM����*/
  LED_BLUE;
  
  /*��SRAM���ж�д���ԣ����SRAM�Ƿ�����*/
  if(SRAM_Test()==1)
  {
		//�������� �̵���
    LED_GREEN;			  
  }
	else
	{
		//����ʧ�� �����
		LED_RED;
	}
	
	/*ָ�뷽ʽ����SRAM*/
	{	
	 uint32_t temp;
	
	 rt_kprintf("\r\nָ�뷽ʽ����SRAM\r\n");
	/*��SRAMд��8λ����*/
	 *( uint8_t*) (Bank1_SRAM4_ADDR ) = (uint8_t)0xAA;
	 rt_kprintf("\r\nָ�����SRAM��д������0xAA \r\n");

	 /*��SRAM��ȡ����*/
	 temp =  *( uint8_t*) (Bank1_SRAM4_ADDR );
	 rt_kprintf("��ȡ���ݣ�0x%X \r\n",temp);

	 /*д/�� 16λ����*/
	 *( uint16_t*) (Bank1_SRAM4_ADDR+10 ) = (uint16_t)0xBBBB;
	 rt_kprintf("ָ�����SRAM��д������0xBBBB \r\n");
	 
	 temp =  *( uint16_t*) (Bank1_SRAM4_ADDR+10 );
	 rt_kprintf("��ȡ���ݣ�0x%X \r\n",temp);


	 /*д/�� 32λ����*/
	 *( uint32_t*) (Bank1_SRAM4_ADDR+20 ) = (uint32_t)0xCCCCCCCC;
	 rt_kprintf("ָ�����SRAM��д������0xCCCCCCCC \r\n");	 
	 temp =  *( uint32_t*) (Bank1_SRAM4_ADDR+20 );
	 rt_kprintf("��ȡ���ݣ�0x%X \r\n",temp);

	}
	
	/*���Զ�λ��ʽ����SRAM,���ַ�ʽ���붨���ȫ�ֱ���*/
	{
		testValue = 0xDD;
		rt_kprintf("\r\n���Զ�λ����SRAM��д������0xDD,��������0x%X,������ַΪ%X\r\n",testValue,(uint32_t )&testValue);	 
	}
  
  while (1)
  {
    LED1_ON;
    rt_thread_delay(500);   /* ��ʱ500��tick */
    rt_kprintf("led1_thread running,LED1_ON\r\n");
    
    LED1_OFF;     
    rt_thread_delay(500);   /* ��ʱ500��tick */		 		
    rt_kprintf("led1_thread running,LED1_OFF\r\n");
  }
}

/********************************END OF FILE****************************/
