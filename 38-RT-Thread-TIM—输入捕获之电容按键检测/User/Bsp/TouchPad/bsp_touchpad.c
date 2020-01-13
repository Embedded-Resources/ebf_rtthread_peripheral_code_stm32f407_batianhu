/**
  ******************************************************************************
  * @file    bsp_touchpad.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ���ݰ���Ӧ�ú����ӿ�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F407 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "board.h"
#include "rtthread.h"

// ���ݰ������ص�ʱ����ʱ��
volatile uint16_t tpad_default_val=0;


static void TPAD_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	// ʹ��ͨ������ʱ��	
	RCC_AHB1PeriphClockCmd(TPAD_TIM_GPIO_CLK, ENABLE); 	
	// ָ�����Ÿ���
	GPIO_PinAFConfig(TPAD_TIM_CH_PORT,TPAD_TIM_SOURCE,TPAD_TIM_AF); 
	
	// �˿�����
	GPIO_InitStructure.GPIO_Pin = TPAD_TIM_CH_PIN;	
	// ���ù���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	//���������� 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(TPAD_TIM_CH_PORT, &GPIO_InitStructure);
}
/***********************************
 *
 * ��ʱ�����벶������
 *
 ***********************************/
static void TPAD_TIM_Mode_Config(void)
{ 
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	
	// ʹ��TIMʱ��,TIMx_CLK = 2 * PCLK1 = 84M
	RCC_APB1PeriphClockCmd(TPAD_TIM_CLK,ENABLE);
/*--------------------ʱ���ṹ���ʼ��----------------------*/
  // �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Period = TPAD_TIM_PERIOD;
	// ����CNT��������ʱ�� = TIMx_CLK/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler = TPAD_TIM_PSC;
	// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ������ﲻ��Ҫ	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	// ����������ģʽ	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(TPAD_TIMx, &TIM_TimeBaseStructure); 
/*--------------------���벶��ṹ���ʼ��-------------------*/	
	// �������벶���ͨ������Ҫ���ݾ����GPIO������
	TIM_ICInitStructure.TIM_Channel = TPAD_TIM_Channel_X; 
	// ���벶���źŵļ�������
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	
	// ����ͨ���Ͳ���ͨ����ӳ���ϵ����ֱ���ͷ�ֱ������
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
	//���������Ƶ,����Ƶ 
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	
	//���������˲��� ���˲�
	TIM_ICInitStructure.TIM_ICFilter = 0x00;
	// ��ʱ�����벶���ʼ��
	TIM_ICInit(TPAD_TIMx, &TIM_ICInitStructure);
	
	// ʹ�ܼ�����
	TIM_Cmd ( TPAD_TIMx, ENABLE ); 
}

/*===========================================================================================*/
/**
  * @brief  ��λ���ݰ������ŵ磬���³��
  * @param  ��
  * @retval ��
  * ˵����
  * �������ϵ�֮�󣬵��ݰ���Ĭ���Ѿ������˵磬Ҫ���õ��ݰ����ĳ��ʱ��
  * �ͱ����Ȱѵ��ݰ����ĵ�ŵ�������Ϊ�ýӵ��ݰ�����IO����͵�ƽ����
  * �ŵ����֮���ٰ����ӵ��ݰ�����IO����Ϊ���룬Ȼ��ͨ�����벶��ķ���
  * �������ݰ����ĳ��ʱ�䣬������ʱ����û����ָ����������µĳ��ʱ��
  * ����������صĳ��ʱ��ǳ��ȶ�����Ϊ��·���Ӳ���Ѿ�ȷ����
  * 
  * ������ָ����������£����ʱ���䳤������ֻ��Ҫ�Ա�������ʱ��Ϳ���
  * ֪�����ݰ����Ƿ�����ָ����
  */
static void TPAD_Reset(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	// ʹ��ͨ������ʱ��	
	RCC_AHB1PeriphClockCmd(TPAD_TIM_GPIO_CLK, ENABLE); 
	
	//��������Ϊ��ͨ�������
 	GPIO_InitStructure.GPIO_Pin = TPAD_TIM_CH_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; 
 	GPIO_Init(TPAD_TIM_CH_PORT, &GPIO_InitStructure);
	
	// ����TPAD��IO����Ϊ�����Ȼ������͵�ƽ����ʱһ�ᣬȷ�����ݰ����ŵ����
 	GPIO_ResetBits ( TPAD_TIM_CH_PORT, TPAD_TIM_CH_PIN );						 
  // �ŵ��Ǻܿ�ģ�һ����us����
	//Delay_ms(5);
	rt_thread_delay(5);	
	
	// ����TPAD��IO����Ϊ���룬�������벶��
	GPIO_InitStructure.GPIO_Pin = TPAD_TIM_CH_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(TPAD_TIM_CH_PORT,&GPIO_InitStructure);
}

/**
  * @brief  ��ȡ��ʱ������ֵ
  * @param  ��
  * @retval ��ʱ������ֵ�������ʱ,��ֱ�ӷ��ض�ʱ���ļ���ֵ��
  */
uint16_t TPAD_Get_Val(void)
{		
  // ÿ�β����ʱ�򣬱����ȸ�λ�ŵ�	
	TPAD_Reset();
	
	// �����ݰ�����λ�ŵ�֮�󣬼�������0��ʼ����
	TIM_SetCounter(TPAD_TIMx,0);
	
	// �����صı�־λ
	TIM_ClearITPendingBit(TPAD_TIMx, TPAD_TIM_IT_CCx|TIM_IT_Update);
	
	// �ȴ����������أ������ݰ�����絽1.8V���ҵ�ʱ�򣬾ͻᱻ��Ϊ��������
	while(TIM_GetFlagStatus ( TPAD_TIMx, TPAD_TIM_IT_CCx ) == RESET)
	{
		// �����ʱ��,ֱ�ӷ���CNT��ֵ
		// һ����ʱ�䶼����ms�������ڣ����ٻᳬ����ʱ����������ֵ
		if(TIM_GetCounter(TPAD_TIMx)>TPAD_ARR_MAX_VAL-500)
		{
			return TIM_GetCounter(TPAD_TIMx);
		}
			
	}
	// ��ȡ����ȽϼĴ�����ֵ
	return TPAD_TIM_GetCaptureX(TPAD_TIMx );	  
} 	

void TPAD_TIM_Init(void)
{
	TPAD_TIM_GPIO_Config();
	TPAD_TIM_Mode_Config();		
}

/**
  * @brief  ��ʼ��������������ÿ��ص�ʱ���������ĳ��ʱ��
  * @param  ��
  * @retval 0���ɹ���1��ʧ��
  * @note   ����ֵһ����ȶ�����Ӳ����·�������ú���ֻ��Ҫ����һ�μ���
  *         ����������صĳ��ʱ��ÿ��Ӳ������һ�������ʵ�ʲ�����
  */
uint8_t TPAD_Init(void)
{
	uint16_t temp;
	
	// ���ݰ����õ������벶���IO�Ͳ���ģʽ������ʼ��
	TPAD_TIM_Init();
	
	temp = TPAD_Get_Val();
	
	// ���Ե�ʱ����԰Ѳ����ֵ��ӡ����������Ĭ�ϵĳ��ʱ���Ƕ���
	printf("���ݰ���Ĭ�ϳ��ʱ��Ϊ: %d us\n",temp);
	
	// ���ݰ������صĳ��ʱ��ǳ��ȶ�����ͬ��Ӳ�����ʱ�䲻һ��
	// ��Ҫʵ�ʲ������ã�F407-���컢�ϵĵ��ݰ������س��ʱ���ȶ���32us
	// �������õ��β�����׼ȷ������Զ�β���Ȼ��ȡ��ƽ��ֵ
	if( (TPAD_DEFAULT_VAL_MIN<temp) && (temp<TPAD_DEFAULT_VAL_MAX) )
	{
		tpad_default_val = temp;
		return 0;  // �ɹ�
	}
	else
  {
		return 1; // ʧ��
  }
}
 
/**
  * @brief  ��ȡ���ɴζ�ʱ������ֵ�����������ֵ��
  * @param  num ����ȡ����
  * @retval ��ȡ�������ʱ������ֵ
  */
uint16_t TPAD_Get_MaxVal( uint8_t num )
{
	uint16_t temp=0, res=0;
	
	while(num--)
	{
		temp = TPAD_Get_Val();
		if( temp > res ) 
			res = temp;
	}	
	return res;	
}


/**
  * @brief  ����ɨ�躯��
  * @param  ��
  * @retval 1��������Ч��0��������Ч
  */
uint8_t TPAD_Scan(void)
{
	// keyen���������ʹ�ܱ�־
	// 0�����Կ�ʼ���
	// >0�������ܿ�ʼ��⣬��ʾ����һֱ������
	// ע�⣺keytn ��������� static ���Σ��൱��һ��ȫ�ֱ�����������Ϊ���ں����ڲ����壬
	// �������൱�����������ȫ�ֱ�����ÿ���޸�֮ǰ����������һ�ε�ֵ
	static uint8_t keyen=0;	

	uint8_t res=0,sample=3; 
	uint16_t scan_val;		

	// ����sampleֵ������Σ���ȡ���ֵ��С��һ���Ǹ��Ż���������
	scan_val = TPAD_Get_MaxVal(sample); 
	
  // ��ɨ���ֵ���ڿ���ֵ����Ĭ�ϵ�����ֵ֮�󣬱�ʾ��������
	// ���TPAD_GATE_VAL����Ӳ����������Ҫʵ�ʲ���
	if(scan_val > (tpad_default_val+TPAD_GATE_VAL))
	{						
    // �ٴμ�⣬�����ڻ�е������ȥ��
		scan_val = TPAD_Get_MaxVal(sample);		
		if( ( keyen == 0 )&& (scan_val > (tpad_default_val+TPAD_GATE_VAL)))
				res = 1;	// ��Ч�İ���

		// �������һֱ�����£�keyen��ֵ��һֱ��keyen�ĳ�ʼֵ��keyen-1֮��ѭ������Զ�������0
		keyen = 2;
	}
	
	// ������û�б����»���keyen>0ʱ����ִ��keyen--
	if( keyen > 0)
		keyen--;
  
	return res;
}


/*********************************************END OF FILE**********************/
