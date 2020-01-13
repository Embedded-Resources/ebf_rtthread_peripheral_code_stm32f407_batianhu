#ifndef __BREATH_LED_H
#define	__BREATH_LED_H

#include "stm32f4xx.h"

/*��ѹ��ֵ�ȼ���*/
#define AMPLITUDE_CLASS 256

//����������ε�Ƶ��
extern __IO uint16_t period_class ;

/*PWM���еĵ���*/
extern uint16_t POINT_NUM;



/********************��ʱ��ͨ��**************************/



//R ��ɫ��
#define BRE_RED_TIM           							TIM10
#define BRE_RED_TIM_CLK       						RCC_APB2Periph_TIM10
#define BRE_TIM_GPIO_CLK              		(RCC_AHB1Periph_GPIOF)
#define BRE_RED_TIM_APBxClock_FUN        RCC_APB2PeriphClockCmd

#define BRE_RED_TIM_IRQn								TIM1_UP_TIM10_IRQn
#define BRE_RED_TIM_IRQHandler 				TIM1_UP_TIM10_IRQHandler

/*����˵����c�ļ�*/
/*����ͨ�ö�ʱ����ʱ��ΪHCLK/4������ΪHCLK/2*/
#define BRE_RED_TIM_PRESCALER						(22-1)


#define BRE_RED_PIN                  GPIO_Pin_6                 
#define BRE_RED_GPIO_PORT            GPIOF                      
#define BRE_RED_PINSOURCE							GPIO_PinSource6
#define BRE_RED_AF										GPIO_AF_TIM10

//ͨ���ȽϼĴ�������TIMx->CCRx��ʽ�ɷ��ʸüĴ����������µıȽ�ֵ������ռ�ձ�
//�Ժ��װ��ʹ��������ʽ��BRE_TIMx->BRE_LED_CCRx ���ɷ��ʸ�ͨ���ıȽϼĴ���
#define BRE_RED_CCRx									CCR1
#define BRE_RED_TIM_CHANNEL					TIM_Channel_1

#define  BRE_RED_TIM_OCxInit              TIM_OC1Init            //ͨ��ѡ��1~4
#define  BRE_RED_TIM_OCxPreloadConfig    TIM_OC1PreloadConfig 


//G ��ɫ��
#define BRE_GREEN_TIM           							TIM11
#define BRE_GREEN_TIM_CLK       						RCC_APB2Periph_TIM11
#define BRE_GREEN_TIM_APBxClock_FUN        RCC_APB2PeriphClockCmd

#define BRE_GREEN_TIM_IRQn								TIM1_TRG_COM_TIM11_IRQn
#define BRE_GREEN_TIM_IRQHandler 				TIM1_TRG_COM_TIM11_IRQHandler

/*����˵����c�ļ�*/
/*����ͨ�ö�ʱ����ʱ��ΪHCLK/4������ΪHCLK/2*/
#define BRE_GREEN_TIM_PRESCALER						(22-1)


#define BRE_GREEN_PIN                  GPIO_Pin_7                 
#define BRE_GREEN_GPIO_PORT            GPIOF                      
#define BRE_GREEN_PINSOURCE							GPIO_PinSource7
#define BRE_GREEN_AF										GPIO_AF_TIM11

//ͨ���ȽϼĴ�������TIMx->CCRx��ʽ�ɷ��ʸüĴ����������µıȽ�ֵ������ռ�ձ�
//�Ժ��װ��ʹ��������ʽ��BRE_TIM->BRE_LED_CCRx ���ɷ��ʸ�ͨ���ıȽϼĴ���
#define BRE_GREEN_CCRx									CCR1
#define BRE_GREEN_TIM_CHANNEL					TIM_Channel_1

#define  BRE_GREEN_TIM_OCxInit              TIM_OC1Init            //ͨ��ѡ��1~4
#define  BRE_GREEN_TIM_OCxPreloadConfig    TIM_OC1PreloadConfig 

//B ��ɫ��
#define BRE_BLUE_TIM           							TIM13
#define BRE_BLUE_TIM_CLK       						RCC_APB1Periph_TIM13
#define BRE_BLUE_TIM_APBxClock_FUN        RCC_APB1PeriphClockCmd

#define BRE_BLUE_TIM_IRQn								TIM8_UP_TIM13_IRQn
#define BRE_BLUE_TIM_IRQHandler 				TIM8_UP_TIM13_IRQHandler

/*����˵����c�ļ�*/
/*����ͨ�ö�ʱ����ʱ��ΪHCLK/4������ΪHCLK/2*/
#define BRE_BLUE_TIM_PRESCALER						(11-1)


#define BRE_BLUE_PIN                  GPIO_Pin_8                 
#define BRE_BLUE_GPIO_PORT            GPIOF                      
#define BRE_BLUE_PINSOURCE							GPIO_PinSource8
#define BRE_BLUE_AF										GPIO_AF_TIM13

//ͨ���ȽϼĴ�������TIMx->CCRx��ʽ�ɷ��ʸüĴ����������µıȽ�ֵ������ռ�ձ�
//�Ժ��װ��ʹ��������ʽ��BRE_TIM->BRE_LED_CCRx ���ɷ��ʸ�ͨ���ıȽϼĴ���
#define BRE_BLUE_CCRx									CCR1
#define BRE_BLUE_TIM_CHANNEL					TIM_Channel_1

#define  BRE_BLUE_TIM_OCxInit              TIM_OC1Init            //ͨ��ѡ��1~4
#define  BRE_BLUE_TIM_OCxPreloadConfig    TIM_OC1PreloadConfig 


void BreathLED_Config(void);

#endif /* __BREATH_LED_H */
