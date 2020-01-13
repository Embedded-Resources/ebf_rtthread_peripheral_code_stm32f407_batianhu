/**
  ******************************************************************************
  * @file    bsp_advance_tim.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ͨ�ö�ʱ����ʱ����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F407 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./tim/bsp_advance_tim.h"


 /**
  * @brief  �߼����ƶ�ʱ�� TIMx,x[1,8]�ж����ȼ�����
  * @param  ��
  * @retval ��
  */
void TIMx_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = ADVANCE_TIM_IRQn; 	
		// ������ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // ���������ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         ����
 * TIM_CounterMode			 TIMx,x[6,7]û�У��������У�������ʱ����
 * TIM_Period            ����
 * TIM_ClockDivision     TIMx,x[6,7]û�У���������(������ʱ��)
 * TIM_RepetitionCounter TIMx,x[1,8]����(�߼���ʱ��)
 *-----------------------------------------------------------------------------
 */
void TIM_PWMOUTPUT_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*--------------------GPIO�ṹ���ʼ��-------------------------*/
	// ��ʼGPIO�˿�ʱ��
	RCC_AHB1PeriphClockCmd (GENERAL_OCPWM_GPIO_CLK, ENABLE); 	

	// ͨ�ö�ʱ����������
	GPIO_PinAFConfig(GENERAL_OCPWM_GPIO_PORT,GENERAL_OCPWM_PINSOURCE,GENERAL_OCPWM_AF);
	
	// ͨ�ö�ʱ��PWM�������															   
	GPIO_InitStructure.GPIO_Pin = GENERAL_OCPWM_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_Init(GENERAL_OCPWM_GPIO_PORT, &GPIO_InitStructure);
	
	/*--------------------ʱ���ṹ���ʼ��-------------------------*/
	// �������ڣ���������Ϊ100K
	
	// ����TIMx_CLK
	// ͨ�ö�ʱ��ʱ��ԴTIMxCLK = 2*PCLK1=84MHz 
  RCC_APB1PeriphClockCmd(GENERAL_TIM_CLK, ENABLE); 

  // �ۼ� TIM_Period�������һ�����»����ж�*/
  TIM_TimeBaseStructure.TIM_Period = GENERAL_TIM_PERIOD;
	// ����CNT��������ʱ�� = TIMxCLK/(psc+1)
  TIM_TimeBaseStructure.TIM_Prescaler = GENERAL_TIM_PSC;	
  // ����ʱ�ӷ�Ƶ
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  // ������ʽ
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	
	// ��ʼ����ʱ��TIMx
	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);
	
	/*--------------------����ȽϽṹ���ʼ��-------------------*/
	// ����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	
  // ���ʹ��	
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// ���ͨ����ƽ��������
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	// ����Ƚ�ֵ����
  TIM_OCInitStructure.TIM_Pulse = GENERAL_TIM_CCR1;
	// ʹ��ͨ��
  TIM_OC1Init(GENERAL_TIM, &TIM_OCInitStructure);	
	// ʹ��ͨ������
	TIM_OC1PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
	
	// ʹ�ܶ�ʱ��
	TIM_Cmd(GENERAL_TIM, ENABLE);	
}

void TIM_PWMINPUT_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*--------------------GPIO�ṹ���ʼ��-------------------------*/
	// ��ʼGPIO�˿�ʱ��
	RCC_AHB1PeriphClockCmd (ADVANCE_ICPWM_GPIO_CLK, ENABLE); 	

	// ��ʱ����������
	GPIO_PinAFConfig(ADVANCE_ICPWM_GPIO_PORT,ADVANCE_ICPWM_PINSOURCE,ADVANCE_ICPWM_AF); 
	
	// �߼����ƶ�ʱ��PWM���벶������															   
	GPIO_InitStructure.GPIO_Pin = ADVANCE_ICPWM_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_Init(ADVANCE_ICPWM_GPIO_PORT, &GPIO_InitStructure);
	
	// ����TIMx_CLK
	// �߼���ʱ��ʱ��ԴTIMxCLK = 2*PCLK2=168MHz 
  RCC_APB2PeriphClockCmd(ADVANCE_TIM_CLK, ENABLE); 
	
  /*--------------------ʱ���ṹ���ʼ��----------------------*/
	// �ۼ� TIM_Period�������һ�����»����ж�*/
  TIM_TimeBaseStructure.TIM_Period = ADVANCE_TIM_PERIOD;
  // ����CNT��������ʱ�� = TIMxCLK/(psc+1)
  TIM_TimeBaseStructure.TIM_Prescaler = ADVANCE_TIM_PSC;
	// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;	
  // ������ʽ
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	
	// �ظ���������ֵ��û�õ����ù�
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);
	
  /*--------------------���벶��ṹ���ʼ��-------------------*/
	// ʹ��PWM����ģʽʱ����Ҫռ����������Ĵ�����һ�������ڣ�����һ����ռ�ձ�
	// ����ͨ��IC1����
	// ѡ�񲶻�ͨ��
  TIM_ICInitStructure.TIM_Channel = ADVANCE_IC1PWM_CHANNEL;
	// ���ò���ı���
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	// ���ò���ͨ�����ź��������ĸ�����ͨ������ֱ���ͷ�ֱ������
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// 1��Ƶ���������źŵ�ÿ����Ч���ض�����
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// ���˲�
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
	// ��ʼ��PWM����ģʽ
  TIM_PWMIConfig(ADVANCE_TIM, &TIM_ICInitStructure);
	
  // ��������PWM����ģʽʱ,ֻ��Ҫ���ô����źŵ���һ·���ɣ����ڲ������ڣ�
  // ����һ·�����ڲ���ռ�ձȣ�����Ӳ���Դ����ã�����Ҫ������
	
	// ѡ�����벶��Ĵ����ź�
  TIM_SelectInputTrigger(ADVANCE_TIM, TIM_TS_TI1FP1);		

  // ѡ���ģʽ: ��λģʽ
	// PWM����ģʽʱ,��ģʽ���빤���ڸ�λģʽ��������ʼʱ,������CNT�ᱻ��λ
  TIM_SelectSlaveMode(ADVANCE_TIM, TIM_SlaveMode_Reset);
  TIM_SelectMasterSlaveMode(ADVANCE_TIM,TIM_MasterSlaveMode_Enable);
	
	// ʹ�ܲ����ж�,����ж���Ե���������ͨ�������������Ǹ���
	TIM_ITConfig(ADVANCE_TIM, TIM_IT_CC1, ENABLE);
	TIM_ClearITPendingBit(ADVANCE_TIM, TIM_IT_CC1);
	
  // ʹ�ܸ߼����ƶ�ʱ������������ʼ����
  TIM_Cmd(ADVANCE_TIM, ENABLE);  
}


/*********************************************END OF FILE**********************/
