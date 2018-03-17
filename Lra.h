#include "all_header.h"

int time_cal_LRA[9]={0,};
int time_start_LRA[9]={0,};
int time_end_LRA[9]={-1,};
int LRA_off[9]={0,};
int LRA_i;

int left_on= 0;
int left_move = 0;
int right_move = 0;
int right_on=0;
int center_move = 0;
int center_on = 0;
int de = 0;
int move_time = 0;
int move_time2 = 0;
int move_on = 0;

//LRA PWM 타이머3 초기화
static void setup_pwm_timer3(void) {
  uint16_t PrescalerValue;                // 추가
  /* Structure storing the information of Timer 3. */
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  /* Enable Timer 3 clock. */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);    //V
  PrescalerValue = (uint16_t) (SystemCoreClock / 2 / TIMER_PRESCALER_FREQ);
  /* Set the timing clock prescaler. */
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;    //v
  /* Set the timer count up. */
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;    //v
  /* Set the timer's top counting value. */
  TIM_TimeBaseStructure.TIM_Period = 4255;                //v
  /* Set the internal clock division. */
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;            //v
  /* No repetition counter for Timer 3. */
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;            // 검색필요
  /* Write this data into memory at the address mapped to Timer 3. */
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);        //v
  /* Enable Timer 3's counter. */
  TIM_Cmd(TIM3, ENABLE);                //v
}

// PWM3 채널 설정
static void setup_pwm3(void) {
  /* Structure storing the information of output capture. */
  TIM_OCInitTypeDef TIM_OCInitStructure = {0,};        //V
  /* Set output capture as PWM mode. */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;        //v
  /* Enable output compare to the correspond output pin.*/
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //v
  /* Set output compare active as high. */
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;        //v
  /* Set the PWM duty cycle. */
  TIM_OCInitStructure.TIM_Pulse = 0;  /* 0% duty cycle */    //v =>예비 수정
  /* Write this data into memory at the address mapped
  * to output capture 1~4. */
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
  TIM_OC3Init(TIM3, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
  TIM_OC4Init(TIM3, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
}

// PWM3 GPIO 초기화(PB4, PB5, PC8, PC9) 
static void setup_lras3(void) {
  /* Structure storing the information of GPIO Port B, C. */
    GPIO_InitTypeDef GPIO_InitStructure;            //V
    GPIO_InitTypeDef GPIO_InitStructure2;            //V
    GPIO_InitTypeDef GPIO_InitStructure3;            //V
    GPIO_InitTypeDef GPIO_InitStructure4;            //V   
    /* Enable the GPIOB,C peripheral clock. */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);    //V
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);    //V
    /* Set the alternative function for the LRAs' pins. */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);    //V
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);
    /* Pin numbers of LRAs are mapped to bits number. */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;
    /* Pins in alternative function mode. */
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;    //V
    /* Sampling rate for the selected pins. */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;        //V
    /* Operating output type for the selected pins. */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        //V
    /* Operating Pull-up/Pull down for the selected pins. */
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;    //V
    GPIO_InitStructure2.GPIO_Pin   = GPIO_Pin_5;
    GPIO_InitStructure2.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure2.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure2.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStructure3.GPIO_Pin   = GPIO_Pin_8;
    GPIO_InitStructure3.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure3.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure3.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure3.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStructure4.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure4.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure4.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure4.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure4.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    /* Write this data into memory at the address
     * mapped to GPIO device port B,C, where the LRA pins
     * are connected */
    GPIO_Init(GPIOB, &GPIO_InitStructure);        //V
    GPIO_Init(GPIOB, &GPIO_InitStructure2);        //V
    GPIO_Init(GPIOC, &GPIO_InitStructure3);        //V
    GPIO_Init(GPIOC, &GPIO_InitStructure4);        //V
}

//LRA PWM 타이머4 초기화
static void setup_pwm_timer4(void) {
    uint16_t PrescalerValue;                // 추가
    /* Structure storing the information of Timer 4. */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    /* Enable Timer 4 clock. */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);    //V
    PrescalerValue = (uint16_t) (SystemCoreClock / 2 / TIMER_PRESCALER_FREQ);     // 추가(다 다르게 해야함)
    /* Set the timing clock prescaler. */
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;    //v
    /* Set the timer count up. */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;    //v
    /* Set the timer's top counting value. */
    TIM_TimeBaseStructure.TIM_Period = 4255;                //v
    /* Set the internal clock division. */
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;            //v
    /* No repetition counter for Timer 4. */
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;            // 검색필요
    /* Write this data into memory at the address mapped to Timer 4. */
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);        //v
    /* Enable Timer 4's counter. */
    TIM_Cmd(TIM4, ENABLE);                //v
}

// PWM4 채널 설정
static void setup_pwm4(void) {
    /* Structure storing the information of output capture. */
    TIM_OCInitTypeDef TIM_OCInitStructure = {0,};        //V
    /* Set output capture as PWM mode. */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;        //v
    /* Enable output compare to the correspond output pin.*/
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //v
    /* Set output compare active as high. */
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;        //v
    /* Set the PWM duty cycle. */
    TIM_OCInitStructure.TIM_Pulse = 0;  /* 0% duty cycle */    //v =>예비 수정
    
    
    /* Write this data into memory at the address mapped
     * to output capture 1~4. */
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
}

// PWM4 GPIO 초기화(PD12, PD13, PD14, PD15)
static void setup_lras4(void) {
    /* Structure storing the information of GPIO Port D. */
    GPIO_InitTypeDef GPIO_InitStructure;            //V
    GPIO_InitTypeDef GPIO_InitStructure2;
    GPIO_InitTypeDef GPIO_InitStructure3;
    GPIO_InitTypeDef GPIO_InitStructure4;
    /* Enable the GPIOD peripheral clock. */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);    //V
    /* Set the alternative function for the LRAs' pins. */
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);    //V
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);    //V
    /* Pin numbers of LRAs are mapped to bits number. */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
    /* Pins in alternative function mode. */
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;    //V
    /* Sampling rate for the selected pins. */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;        //V
    /* Operating output type for the selected pins. */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        //V
    /* Operating Pull-up/Pull down for the selected pins. */
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;    //V
    GPIO_InitStructure2.GPIO_Pin   = GPIO_Pin_13;
    GPIO_InitStructure2.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure2.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure2.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStructure3.GPIO_Pin   = GPIO_Pin_14;
    GPIO_InitStructure3.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure3.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure3.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure3.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStructure4.GPIO_Pin   = GPIO_Pin_15;
    GPIO_InitStructure4.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure4.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure4.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure4.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    
    /* Write this data into memory at the address
     * mapped to GPIO device port D, where the LRA pins
     * are connected */
    GPIO_Init(GPIOD, &GPIO_InitStructure);        //V
    
    GPIO_Init(GPIOD, &GPIO_InitStructure2);        //V
    
    GPIO_Init(GPIOD, &GPIO_InitStructure3);        //V
    
    GPIO_Init(GPIOD, &GPIO_InitStructure4);        //V
}

//LRA PWM 타이머12 초기화
static void setup_pwm_timer12(void) {
    uint16_t PrescalerValue;                // 추가
    /* Structure storing the information of Timer 12. */
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    /* Enable Timer 12 clock. */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);    //V
    PrescalerValue = (uint16_t) (SystemCoreClock / 2 / TIMER_PRESCALER_FREQ);     // 추가(다 다르게 해야함)
    /* Set the timing clock prescaler. */
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;    //v
    /* Set the timer count up. */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;    //v
    /* Set the timer's top counting value. */
    TIM_TimeBaseStructure.TIM_Period = 4255;                //v
    /* Set the internal clock division. */
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;            //v
    /* No repetition counter for Timer 12. */
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;            // 검색필요
    /* Write this data into memory at the address mapped to Timer 12. */
    TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStructure);        //v
    /* Enable Timer 12's counter. */
    TIM_Cmd(TIM12, ENABLE);                //v
}

// PWM12 채널 설정
static void setup_pwm12(void) {
    /* Structure storing the information of output capture. */
    TIM_OCInitTypeDef TIM_OCInitStructure = {0,};        //V
    /* Set output capture as PWM mode. */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;        //v
    /* Enable output compare to the correspond output pin.*/
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //v
    /* Set output compare active as high. */
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;        //v
    /* Set the PWM duty cycle. */
    TIM_OCInitStructure.TIM_Pulse = 0;  /* 0% duty cycle */    //v =>예비 수정
    /* Write this data into memory at the address mapped
     * to output capture 1~4. */
    TIM_OC2Init(TIM12, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM12, TIM_OCPreload_Enable);
}

// PWM12 GPIO 초기화(PB15)
static void setup_lras12(void) {
    /* Structure storing the information of GPIO Port B. */
    GPIO_InitTypeDef GPIO_InitStructure;
    /* Enable the GPIOB peripheral clock. */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);    //V
    /* Set the alternative function for the LRAs' pins. */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_TIM12);    //V
    /* Pin numbers of LRAs are mapped to bits number. */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;
    /* Pins in alternative function mode. */
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;    //V
    /* Sampling rate for the selected pins. */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;        //V
    /* Operating output type for the selected pins. */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        //V
    /* Operating Pull-up/Pull down for the selected pins. */
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;    //V
    /* Write this data into memory at the address
     * mapped to GPIO device port D, where the LRA pins
     * are connected */
    GPIO_Init(GPIOB, &GPIO_InitStructure);        //V
}

// 1번 LRA duty 변경 함수
void PWMChannel_Control1(uint32_t du)
{
    TIM_OCInitTypeDef outputChannelInit = {0,};
    outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
      outputChannelInit.TIM_Pulse = du;
    outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
    outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM3, &outputChannelInit);
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);
}

// 2번 LRA duty 변경 함수
void PWMChannel_Control2(uint32_t du)
{
    TIM_OCInitTypeDef outputChannelInit = {0,};
    outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
      outputChannelInit.TIM_Pulse = du;
    
    outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
    outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC2Init(TIM3, &outputChannelInit);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3);
}

// 3번 LRA duty 변경 함수
void PWMChannel_Control3(uint32_t du)
{
    TIM_OCInitTypeDef outputChannelInit = {0,};
    outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
    outputChannelInit.TIM_Pulse = du;
    outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
    outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC3Init(TIM3, &outputChannelInit);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
}

// 4번 LRA duty 변경 함수
void PWMChannel_Control4(uint32_t du)
{
    TIM_OCInitTypeDef outputChannelInit = {0,};
    outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
    outputChannelInit.TIM_Pulse = du;
    outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
    outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC4Init(TIM3, &outputChannelInit);
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);
}

// 5번 LRA duty 변경 함수
void PWMChannel_Control5(uint32_t du)
{
    TIM_OCInitTypeDef outputChannelInit = {0,};
    outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
    outputChannelInit.TIM_Pulse = du;
    outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
    outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM4, &outputChannelInit);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
}

// 6번 LRA duty 변경 함수
void PWMChannel_Control6(uint32_t du)
{
    TIM_OCInitTypeDef outputChannelInit = {0,};
    outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
    outputChannelInit.TIM_Pulse = du;
    outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
    outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC2Init(TIM4, &outputChannelInit);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
}

// 7번 LRA duty 변경 함수
void PWMChannel_Control7(uint32_t du)
{
    TIM_OCInitTypeDef outputChannelInit = {0,};
    outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
    outputChannelInit.TIM_Pulse = du;
    outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
    outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC3Init(TIM4, &outputChannelInit);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
}

// 8번 LRA duty 변경 함수
void PWMChannel_Control8(uint32_t du)
{
    TIM_OCInitTypeDef outputChannelInit = {0,};
    outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
    outputChannelInit.TIM_Pulse = du;
    outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
    outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC4Init(TIM4, &outputChannelInit);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
}

// 9번 LRA duty 변경 함수
void PWMChannel_Control9(uint32_t du)
{
    TIM_OCInitTypeDef outputChannelInit = {0,};
    outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
    outputChannelInit.TIM_Pulse = du;
    outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
    outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC2Init(TIM12, &outputChannelInit);
    TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_TIM12);
}

// TIM3 주파수 변경 함수
void changeTimerXperiod3( uint32_t TIMER_FREQ ){//타이머 주기 실시간 변화
       uint16_t PrescalerValue;
       TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//Timer관련 구조체
       RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
       SystemCoreClockUpdate();
       PrescalerValue = (uint16_t) (SystemCoreClock / 2 /TIMER_PRESCALER_FREQ) *220;
       TIM_TimeBaseStructure.TIM_Period = TIMER_FREQ;
       TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
       TIM_TimeBaseStructure.TIM_ClockDivision = 0;
       TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
       TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);    
       TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
       TIM_Cmd(TIM3, ENABLE);
}

// TIM4 주파수 변경 함수
void changeTimerXperiod4( uint32_t TIMER_FREQ ){//타이머 주기 실시간 변화
       uint16_t PrescalerValue;
       TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//Timer관련 구조체
       RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
       SystemCoreClockUpdate();
       PrescalerValue = (uint16_t) (SystemCoreClock / 2 /TIMER_PRESCALER_FREQ) *220;
       TIM_TimeBaseStructure.TIM_Period = TIMER_FREQ;
       TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
       TIM_TimeBaseStructure.TIM_ClockDivision = 0;
       TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
       TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);    
       TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
       TIM_Cmd(TIM4, ENABLE);
}

// TIM12 주파수 변경 함수
void changeTimerXperiod12( uint32_t TIMER_FREQ ){//타이머 주기 실시간 변화
       uint16_t PrescalerValue;
       TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//Timer관련 구조체
       RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
       SystemCoreClockUpdate();
       PrescalerValue = (uint16_t) (SystemCoreClock / 2 /TIMER_PRESCALER_FREQ) *220;
       TIM_TimeBaseStructure.TIM_Period = TIMER_FREQ;
       TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
       TIM_TimeBaseStructure.TIM_ClockDivision = 0;
       TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
       TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStructure);    
       TIM_ITConfig(TIM12, TIM_IT_Update, ENABLE);
       TIM_Cmd(TIM12, ENABLE);
}
/*
void Frequency3(unsigned char fre)
{
  int per = num(fre);
  changeTimerXperiod3(per);
}
void Frequency4(unsigned char fre)
{
  int per = num(fre);
  changeTimerXperiod4(per);
}
void Frequency12(unsigned char fre)
{
  int per = num(fre);
  changeTimerXperiod12(per);
}
*/
