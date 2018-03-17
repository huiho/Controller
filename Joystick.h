#include "all_header.h"

char flag = 0;
int mx = 0;
int my = 0;
int m = 0;
uint16_t x ,y = 0;
char buffer[7];


void Init_PWM_Joystick_xy(){
  //PWM 은 General Purpose Timer이라고 이야기했던 TIM2, TIM3, TIM4, TIM5, TIM9, TIM12, 를 통하여 펄스를 generating 하게 된다.
  //PWM 1과 PWM8은 edge-or Center -aligned mode임
  //링크 :
  //http://microchip.wdfiles.com/local--files/pwr3101:pwm-edge-center-aligned-modes/edge-aligned-pwm.png
  //PWM 의 출력은 PC6, PC7, PB0 and PB1에 묶여있음. 핀 헤더 참조
  uint16_t PrescalerValue;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  /* TIM4, GPIOB clock enable */
  //TIM4를 통하여 PWM을 생성함.
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
  //TIM4는 Bㅠ포트 1번핀에 있는 ch4을 사용하도록 함.
  //즉 이 핀으로 PWM으로 동작시키는 펄스가 나오게 됨.
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);    
  //AFConfig는 Alternative Function이라고 하여 같은 핀에 여러개의 Peripheral들이 모여있는것을 구분하기 위하여 사용하는 Function이다.
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
  //PWM 모드 설정
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       //pin0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* Compute the prescaler value */
  //TIMER와 마찬가지로 PRESCALE을 설정한다.
  SystemCoreClockUpdate();
  PrescalerValue = (uint16_t) (SystemCoreClock / 2 / TIMER_PRESCALER_FREQ)*220;     // 1kHz timer //84hz
  TIM_TimeBaseStructure.TIM_Period = 4545 ;//-1은 오버플로우 방지★?
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
  //이부분이 PWM관련 구조체를 초기화 시키는 부분이다.
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  //OC는 Output Compare의 약자,
  //OCMode는 PWM1과 PWM2 두개로 나뉘는데, HighState에서 Compare Interrupt를 실시할것인지, LowState에서 CompareInturrupt를 실시할것인지 확인해준다. PWM1과 PWM2는 위상이 반대이다.
  // 관련 이미지 : http://cfile28.uf.tistory.com/image/210BF93753BE7AB609013E
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  //Output을 Enable한다.
  TIM_OCInitStructure.TIM_Pulse = DUTY_IDLE;       // default 50%, DUTY_IDLE은 위에 define되어있다.
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  //신호를 High상태로 동작시킨다.
  TIM_OC1Init(TIM5, &TIM_OCInitStructure);
  //Output Camture4를 초기화시켜줌.    
  TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Enable);
  //OC는 1~4까지 있으며, TIMx의 CCRx에 맞추어 셋팅해야한다.
  /* TIM4 enable counter */
  TIM_Cmd(TIM5, ENABLE);    
}


//조이스틱 주파수 변경 코딩
void Init_PWM4_Joystick(uint32_t TIMER_FREQ, uint32_t  DUTY){
  //PWM 은 General Purpose Timer이라고 이야기했던 TIM2, TIM3, TIM4, TIM5, TIM9, TIM12, 를 통하여 펄스를 generating 하게 된다.
  //PWM 1과 PWM8은 edge-or Center -aligned mode임
  //링크 :
  //http://microchip.wdfiles.com/local--files/pwr3101:pwm-edge-center-aligned-modes/edge-aligned-pwm.png
  //PWM 의 출력은 PC6, PC7, PB0 and PB1에 묶여있음. 핀 헤더 참조
  
  uint16_t PrescalerValue;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  /* TIM4, GPIOB clock enable */
  //TIM4를 통하여 PWM을 생성함.
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
  //TIM4는 Bㅠ포트 1번핀에 있는 ch4을 사용하도록 함.
  //즉 이 핀으로 PWM으로 동작시키는 펄스가 나오게 됨.
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);    
  //AFConfig는 Alternative Function이라고 하여 같은 핀에 여러개의 Peripheral들이 모여있는것을 구분하기 위하여 사용하는 Function이다.
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM5);
  //PWM 모드 설정
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;       //pin0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* Compute the prescaler value */
  //TIMER와 마찬가지로 PRESCALE을 설정한다.
  SystemCoreClockUpdate();
  PrescalerValue = (uint16_t) (SystemCoreClock / 2 / TIMER_PRESCALER_FREQ)*220;     // 1kHz timer
  
  TIM_TimeBaseStructure.TIM_Period = TIMER_FREQ ;//-1은 오버플로우 방지★?
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
  //이부분이 PWM관련 구조체를 초기화 시키는 부분이다.
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  //OC는 Output Compare의 약자,
  //OCMode는 PWM1과 PWM2 두개로 나뉘는데, HighState에서 Compare Interrupt를 실시할것인지, LowState에서 CompareInturrupt를 실시할것인지 확인해준다. PWM1과 PWM2는 위상이 반대이다.
  // 관련 이미지 : http://cfile28.uf.tistory.com/image/210BF93753BE7AB609013E
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  //Output을 Enable한다.
  TIM_OCInitStructure.TIM_Pulse = DUTY;       // default 50%, DUTY_IDLE은 위에 define되어있다.
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  //신호를 High상태로 동작시킨다.
  TIM_OC4Init(TIM5, &TIM_OCInitStructure);
  //Output Camture4를 초기화시켜줌.    
  TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);
  //OC는 1~4까지 있으며, TIMx의 CCRx에 맞추어 셋팅해야한다.
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
{       //완료  TIM2
  // 적외선 센서용 타이머 인터럽트(보통 120Hz면 충분함)
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
  PrescalerValue = (uint16_t) (( SystemCoreClock /2) / TIMER_PRESCALER_FREQ) *220; //119, -1은 오버플로 방지
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