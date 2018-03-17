#include "all_header.h"

char flag = 0;
int mx = 0;
int my = 0;
int m = 0;
uint16_t x ,y = 0;
char buffer[7];


void Init_PWM_Joystick_xy(){
  //PWM �� General Purpose Timer�̶�� �̾߱��ߴ� TIM2, TIM3, TIM4, TIM5, TIM9, TIM12, �� ���Ͽ� �޽��� generating �ϰ� �ȴ�.
  //PWM 1�� PWM8�� edge-or Center -aligned mode��
  //��ũ :
  //http://microchip.wdfiles.com/local--files/pwr3101:pwm-edge-center-aligned-modes/edge-aligned-pwm.png
  //PWM �� ����� PC6, PC7, PB0 and PB1�� ��������. �� ��� ����
  uint16_t PrescalerValue;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  /* TIM4, GPIOB clock enable */
  //TIM4�� ���Ͽ� PWM�� ������.
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
  //TIM4�� B����Ʈ 1���ɿ� �ִ� ch4�� ����ϵ��� ��.
  //�� �� ������ PWM���� ���۽�Ű�� �޽��� ������ ��.
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);    
  //AFConfig�� Alternative Function�̶�� �Ͽ� ���� �ɿ� �������� Peripheral���� ���ִ°��� �����ϱ� ���Ͽ� ����ϴ� Function�̴�.
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
  //PWM ��� ����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       //pin0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* Compute the prescaler value */
  //TIMER�� ���������� PRESCALE�� �����Ѵ�.
  SystemCoreClockUpdate();
  PrescalerValue = (uint16_t) (SystemCoreClock / 2 / TIMER_PRESCALER_FREQ)*220;     // 1kHz timer //84hz
  TIM_TimeBaseStructure.TIM_Period = 4545 ;//-1�� �����÷ο� ������?
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
  //�̺κ��� PWM���� ����ü�� �ʱ�ȭ ��Ű�� �κ��̴�.
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  //OC�� Output Compare�� ����,
  //OCMode�� PWM1�� PWM2 �ΰ��� �����µ�, HighState���� Compare Interrupt�� �ǽ��Ұ�����, LowState���� CompareInturrupt�� �ǽ��Ұ����� Ȯ�����ش�. PWM1�� PWM2�� ������ �ݴ��̴�.
  // ���� �̹��� : http://cfile28.uf.tistory.com/image/210BF93753BE7AB609013E
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  //Output�� Enable�Ѵ�.
  TIM_OCInitStructure.TIM_Pulse = DUTY_IDLE;       // default 50%, DUTY_IDLE�� ���� define�Ǿ��ִ�.
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  //��ȣ�� High���·� ���۽�Ų��.
  TIM_OC1Init(TIM5, &TIM_OCInitStructure);
  //Output Camture4�� �ʱ�ȭ������.    
  TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Enable);
  //OC�� 1~4���� ������, TIMx�� CCRx�� ���߾� �����ؾ��Ѵ�.
  /* TIM4 enable counter */
  TIM_Cmd(TIM5, ENABLE);    
}


//���̽�ƽ ���ļ� ���� �ڵ�
void Init_PWM4_Joystick(uint32_t TIMER_FREQ, uint32_t  DUTY){
  //PWM �� General Purpose Timer�̶�� �̾߱��ߴ� TIM2, TIM3, TIM4, TIM5, TIM9, TIM12, �� ���Ͽ� �޽��� generating �ϰ� �ȴ�.
  //PWM 1�� PWM8�� edge-or Center -aligned mode��
  //��ũ :
  //http://microchip.wdfiles.com/local--files/pwr3101:pwm-edge-center-aligned-modes/edge-aligned-pwm.png
  //PWM �� ����� PC6, PC7, PB0 and PB1�� ��������. �� ��� ����
  
  uint16_t PrescalerValue;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  /* TIM4, GPIOB clock enable */
  //TIM4�� ���Ͽ� PWM�� ������.
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
  //TIM4�� B����Ʈ 1���ɿ� �ִ� ch4�� ����ϵ��� ��.
  //�� �� ������ PWM���� ���۽�Ű�� �޽��� ������ ��.
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);    
  //AFConfig�� Alternative Function�̶�� �Ͽ� ���� �ɿ� �������� Peripheral���� ���ִ°��� �����ϱ� ���Ͽ� ����ϴ� Function�̴�.
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM5);
  //PWM ��� ����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;       //pin0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* Compute the prescaler value */
  //TIMER�� ���������� PRESCALE�� �����Ѵ�.
  SystemCoreClockUpdate();
  PrescalerValue = (uint16_t) (SystemCoreClock / 2 / TIMER_PRESCALER_FREQ)*220;     // 1kHz timer
  
  TIM_TimeBaseStructure.TIM_Period = TIMER_FREQ ;//-1�� �����÷ο� ������?
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
  //�̺κ��� PWM���� ����ü�� �ʱ�ȭ ��Ű�� �κ��̴�.
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  //OC�� Output Compare�� ����,
  //OCMode�� PWM1�� PWM2 �ΰ��� �����µ�, HighState���� Compare Interrupt�� �ǽ��Ұ�����, LowState���� CompareInturrupt�� �ǽ��Ұ����� Ȯ�����ش�. PWM1�� PWM2�� ������ �ݴ��̴�.
  // ���� �̹��� : http://cfile28.uf.tistory.com/image/210BF93753BE7AB609013E
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  //Output�� Enable�Ѵ�.
  TIM_OCInitStructure.TIM_Pulse = DUTY;       // default 50%, DUTY_IDLE�� ���� define�Ǿ��ִ�.
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  //��ȣ�� High���·� ���۽�Ų��.
  TIM_OC4Init(TIM5, &TIM_OCInitStructure);
  //Output Camture4�� �ʱ�ȭ������.    
  TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);
  //OC�� 1~4���� ������, TIMx�� CCRx�� ���߾� �����ؾ��Ѵ�.
  /* TIM4 enable counter */
  TIM_Cmd(TIM5, ENABLE);    
}

void Init_ADC3(){
  GPIO_InitTypeDef GPIO_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC3, &ADC_InitStructure);
  ADC_RegularChannelConfig(ADC3, ADC_Channel_13, 1, ADC_SampleTime_3Cycles);
  ADC_Cmd(ADC3, ENABLE);
}
void Init_ADC2(){
  GPIO_InitTypeDef GPIO_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
  ADC_RegularChannelConfig(ADC2, ADC_Channel_12, 1, ADC_SampleTime_3Cycles);
  ADC_Cmd(ADC2, ENABLE);
}
void Init_TimerforADC()
{       //�Ϸ�  TIM2
  // ���ܼ� ������ Ÿ�̸� ���ͷ�Ʈ(���� 120Hz�� �����)
  uint16_t PrescalerValue;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0f;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  SystemCoreClockUpdate();
  PrescalerValue = (uint16_t) (( SystemCoreClock /2) / TIMER_PRESCALER_FREQ) *220; //119, -1�� �����÷� ����
  TIM_TimeBaseStructure.TIM_Period = 113 ;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  /* TIM2 IT enable */
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  /* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);
}