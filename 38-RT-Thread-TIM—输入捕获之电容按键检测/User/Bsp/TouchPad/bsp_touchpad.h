#ifndef __TOUCHPAD_H_
#define	__TOUCHPAD_H_

#include "stm32f4xx.h"
 
/******************** TPAD �������ò������� **************************/
#define TPAD_TIMx                   TIM2
#define TPAD_TIM_CLK                RCC_APB1Periph_TIM2

#define TPAD_TIM_Channel_X          TIM_Channel_1
#define TPAD_TIM_IT_CCx             TIM_IT_CC1
#define TPAD_TIM_GetCaptureX        TIM_GetCapture1
#define TPAD_TIM_PERIOD             0XFFFF
#define TPAD_TIM_PSC                (84-1)

//��ʱ��������ֵ
#define TPAD_ARR_MAX_VAL 	0XFFFF	

// TIM ���벶��ͨ��GPIO��غ궨��
#define TPAD_TIM_GPIO_CLK           RCC_AHB1Periph_GPIOA
#define TPAD_TIM_CH_PORT            GPIOA
#define TPAD_TIM_CH_PIN             GPIO_Pin_5
#define TPAD_TIM_AF                 GPIO_AF_TIM2
#define TPAD_TIM_SOURCE             GPIO_PinSource5

// ���ݰ��������µ�ʱ������ֵ����Ҫ���ݲ�ͬ��Ӳ��ʵ�ʲ��ԣ�
// ��С�������ֵ���������Ӧ�ٶ�
#define            TPAD_GATE_VAL              30

// ���ݰ������ص�ʱ���������С�ĳ��ʱ�䣬��ͬ��Ӳ����һ����
// F407���컢�ȶ���32
#define            TPAD_DEFAULT_VAL_MIN       30
#define            TPAD_DEFAULT_VAL_MAX       35

#define            TPAD_ON                     1
#define            TPAD_OFF                    0

/************************** TPAD ��������********************************/
void TPAD_TIM_Init(void);
uint8_t TPAD_Init(void);
uint16_t TPAD_Get_Val(void);
uint8_t TPAD_Scan(void);

#endif /* __TOUCHPAD_H_ */

