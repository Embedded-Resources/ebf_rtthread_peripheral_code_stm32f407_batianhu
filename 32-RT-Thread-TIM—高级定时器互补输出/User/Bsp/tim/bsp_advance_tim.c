/**
  ******************************************************************************
  * @file    bsp_advance_tim.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   �߼����ƶ�ʱ�������������
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

__IO uint16_t ChannelPulse = 127;
/**
  * @brief  ����TIM�������PWMʱ�õ���I/O
  * @param  ��
  * @retval ��
  */
static void TIMx_GPIO_Config(void) 
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*������ʱ����ص�GPIO����ʱ��*/
	RCC_AHB1PeriphClockCmd (ADVANCE_OCPWM_GPIO_CLK |
            	            ADVANCE_OCNPWM_GPIO_CLK| 
	                        ADVANCE_BKIN_GPIO_CLK, 
	                        ENABLE); 
  /* ָ�����Ÿ��ù��� */
	GPIO_PinAFConfig(ADVANCE_OCPWM_GPIO_PORT,
	                 ADVANCE_OCPWM_PINSOURCE, 
	                 ADVANCE_OCPWM_AF); 
	GPIO_PinAFConfig(ADVANCE_OCNPWM_GPIO_PORT,
	                 ADVANCE_OCNPWM_PINSOURCE,
	                 ADVANCE_OCNPWM_AF); 
	GPIO_PinAFConfig(ADVANCE_BKIN_GPIO_PORT,
	                 ADVANCE_BKIN_PINSOURCE,
	                 ADVANCE_BKIN_AF); 
	
	/* ��ʱ���������ų�ʼ�� */															   
	GPIO_InitStructure.GPIO_Pin = ADVANCE_OCPWM_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_Init(ADVANCE_OCPWM_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ADVANCE_OCNPWM_PIN;	
	GPIO_Init(ADVANCE_OCNPWM_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ADVANCE_BKIN_PIN;	
	GPIO_Init(ADVANCE_BKIN_GPIO_PORT, &GPIO_InitStructure);
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
static void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
  /* ================== ʱ���ṹ���ʼ��=================== */	
	// ������ʱ��ʱ��,���ڲ�ʱ��CK_INT,����168M
  RCC_APB2PeriphClockCmd(ADVANCE_TIM_CLK, ENABLE); 

	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Period = 1024-1;
	// ����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler = 1680-1;	
	// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  // ������ʽ
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	// �ظ���������ֵ��û�õ����ù�
  TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);
	
	/*--------------------����ȽϽṹ���ʼ��-------------------*/
	// ����ΪPWMģʽ1��������ߵ�ƽ���ﵽ�Ƚ�ֵ��ʱ���ٸı��ƽ
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// �����ʹ��
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  // �������ʹ��	
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	// ����ռ�ձȴ�С
  TIM_OCInitStructure.TIM_Pulse = ChannelPulse;
	// ������ߵ�ƽ��Ч
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	// ��������ߵ�ƽ��Ч
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	// ������ڱ���ֹʱΪ�ߵ�ƽ
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	// ��������ڱ���ֹʱΪ�͵�ƽ
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
  // ͨ����ʼ��
  TIM_OC1Init(ADVANCE_TIM, &TIM_OCInitStructure);  
	// ʹ��ͨ����װ��
	TIM_OC1PreloadConfig(ADVANCE_TIM, TIM_OCPreload_Enable);
	
	/*-------------------ɲ���������ṹ���ʼ��-------------------*/
	// �й�ɲ���������ṹ��ĳ�Ա����ɲο�BDTR�Ĵ���������
  TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
  TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
  TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
	// ����Ƚ��ź�����ʱ�����ã�������μ���ɲο� BDTR:UTG[7:0]������
  TIM_BDTRInitStructure.TIM_DeadTime = 11;
  TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
	// ��BKIN���ż�⵽�ߵ�ƽ��ʱ������Ƚ��źű���ֹ���ͺ�����ɲ��һ��
  TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_Low;
  TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
  TIM_BDTRConfig(ADVANCE_TIM, &TIM_BDTRInitStructure);
	
	// ʹ�ܶ�ʱ��
	TIM_Cmd(ADVANCE_TIM, ENABLE);	
	
	// �����ʹ�ܣ���ʹ�õ���ͨ�ö�ʱ��ʱ����䲻��Ҫ 
  TIM_CtrlPWMOutputs(ADVANCE_TIM, ENABLE);
}

/**
  * @brief  ��ʼ���߼����ƶ�ʱ��
  * @param  ��
  * @retval ��
  */
void TIMx_Configuration(void)
{  
	TIMx_GPIO_Config();
	
  TIM_Mode_Config();
}

/*********************************************END OF FILE**********************/
