/* ���ļ�ͳһ���ڴ���жϷ����� */
#include "stm32f4xx.h"
#include "board.h" 

__IO uint16_t IC2Value = 0;
__IO uint16_t IC1Value = 0;
__IO float DutyCycle = 0;
__IO float Frequency = 0;

void  ADVANCE_TIM_IRQHandler (void)
{
  /* �����ʱ������/�Ƚ�1�ж� */
  TIM_ClearITPendingBit(ADVANCE_TIM, TIM_IT_CC1);

  /* ��ȡ���벶��ֵ */
  IC1Value = TIM_GetCapture1(ADVANCE_TIM);
  IC2Value = TIM_GetCapture2(ADVANCE_TIM);	
  //printf("IC1Value = %d  IC2Value = %d ",IC1Value,IC2Value);
	
  // ע�⣺����Ĵ���CCR1��CCR2��ֵ�ڼ���ռ�ձȺ�Ƶ�ʵ�ʱ������1
	if (IC1Value != 0)
  {
    /* ռ�ձȼ��� */
    DutyCycle = (float)((IC2Value+1) * 100) / (IC1Value+1);

    /* Ƶ�ʼ��� */
    Frequency = 168000000/(ADVANCE_TIM_PSC+1)/(float)(IC1Value+1);
		printf("ռ�ձȣ�%0.2f%%   Ƶ�ʣ�%0.2fHz\n",DutyCycle,Frequency);
  }
  else
  {
    DutyCycle = 0;
    Frequency = 0;
  }
}


