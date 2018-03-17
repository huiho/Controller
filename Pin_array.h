#include "all_header.h"

//조이스틱 수신값 타이머에 기생하여 제는 시간 재기 변수
int time_cal_pin_array[9]={0,};
int time_start_pin_array[9]={0.};
int time_end_pin_array[9]={-1,};
int pin_array_off[9]={0,};
int pin_array_i;

//핀어레이 각각의 주파수를 측정하는 변수
int Pin_f[9] = {0,};          //현재 주파수
int Pin_Hz[9] = {0,};       //현재 TIM7 주파수
int one_Hz = 1;
int pin_i[9]={0,};


//테스트용 (LED)
 int j;
 int j_flag=0;

void changeTimerXperiod7( uint32_t TIMER_FREQ ){//타이머 주기 실시간 변화
  uint16_t PrescalerValue;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//Timer관련 구조체
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
  SystemCoreClockUpdate();
  PrescalerValue = (uint16_t) (SystemCoreClock / 2 /TIMER_PRESCALER_FREQ)*220;
  TIM_TimeBaseStructure.TIM_Period = TIMER_FREQ;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);    
  TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM7, ENABLE);
}
void Init_Timer7()
{
  uint16_t PrescalerValue;    //uint16_t 자료형의 변수 선언.      
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  //Timer관련 구조체
  NVIC_InitTypeDef NVIC_InitStructure;     //NVIC관련 구조체
  NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;        //타이머 2번에 대한 인터럽트 등록
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f; //우선순위
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0f;    //서브 우선순위
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //인터럽트 채널 사용가능 하도록함.
  NVIC_Init(&NVIC_InitStructure);     //NVIC 초기화 (인터럽트 요청준비단계 끝)=>이제 밥을 하기위한 핸들러를 만들어야함.
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);    //TIM2번을 사용가능하도록 TM
  SystemCoreClockUpdate();  //체제에 따라
  PrescalerValue = (uint16_t) (SystemCoreClock /2/ TIMER_PRESCALER_FREQ)*10; // => 지금은 83 ★?
  TIM_TimeBaseStructure.TIM_Period = 100;//-1은 오버플로우 방지★? 4545
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue; TIM_TimeBaseStructure.TIM_ClockDivision = 0;  //중요하지 X
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;    //타이머를 세는 모드
  TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);    //타이머 2번을 초기화
  /* TIM IT enable */
  TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);    //타이머를 쓸것이냐 안쓸것이냐 하는 것
  /* TIM3 enable counter */
  TIM_Cmd(TIM7, ENABLE);    //타이머 2번을 사용하겠다는 결정
}

void Init_GPIO_OUT1()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO셋팅(Pin, Mode, PuPd, OType, Speed)과 관련된 구조체
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control의 약자, ClockSetting 또는 Reset을 할 때 사용됨.    //각 GPIO 포트(A,B,C,D,E)들은 AHB(Advanced High-performance Bus)라는 버스에 연결되어 있음.    //즉 위의 함수를 통해,  AHB1의 GPIOA를 사용 가능하게 하겠다(Enable)라는 의미.
  //앞에서 RCC함수를 통하여 GPIOA를 초기화시켰고, 이 이후로는 그 포트의 어떤 핀을 어떤 모드로 사용할 것인가   에 대한 셋팅을 함.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //GPIO Pin 15번을 사용
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out모드를 사용, In으로 바꾸면 Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down을 설정    // GPIO_PuPd_UP(Pull Up 상태로 만들기), GPIO_PuPd_DOWN(Pull Down 상태 만들기)    // Default Pull-Up Mode로 설정되어있음,  Pull-Down 모드로 셋팅하세요
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type 설정, PP는 Push Pull, OD는 Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed를 설정. 100MHz 최대
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // E포트의 GPIO 설정을, GPIO_InitStructre와 같이 초기화시킴
}

void Init_GPIO_OUT2()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO셋팅(Pin, Mode, PuPd, OType, Speed)과 관련된 구조체
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control의 약자, ClockSetting 또는 Reset을 할 때 사용됨.    //각 GPIO 포트(A,B,C,D,E)들은 AHB(Advanced High-performance Bus)라는 버스에 연결되어 있음.    //즉 위의 함수를 통해,  AHB1의 GPIOA를 사용 가능하게 하겠다(Enable)라는 의미.
  //앞에서 RCC함수를 통하여 GPIOA를 초기화시켰고, 이 이후로는 그 포트의 어떤 핀을 어떤 모드로 사용할 것인가   에 대한 셋팅을 함.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //GPIO Pin 15번을 사용
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out모드를 사용, In으로 바꾸면 Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down을 설정    // GPIO_PuPd_UP(Pull Up 상태로 만들기), GPIO_PuPd_DOWN(Pull Down 상태 만들기)    // Default Pull-Up Mode로 설정되어있음,  Pull-Down 모드로 셋팅하세요
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type 설정, PP는 Push Pull, OD는 Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed를 설정. 100MHz 최대
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A포트의 GPIO 설정을, GPIO_InitStructre와 같이 초기화시킴
}

void Init_GPIO_OUT3()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO셋팅(Pin, Mode, PuPd, OType, Speed)과 관련된 구조체
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control의 약자, ClockSetting 또는 Reset을 할 때 사용됨.    //각 GPIO 포트(A,B,C,D,E)들은 AHB(Advanced High-performance Bus)라는 버스에 연결되어 있음.    //즉 위의 함수를 통해,  AHB1의 GPIOA를 사용 가능하게 하겠다(Enable)라는 의미.
  //앞에서 RCC함수를 통하여 GPIOA를 초기화시켰고, 이 이후로는 그 포트의 어떤 핀을 어떤 모드로 사용할 것인가   에 대한 셋팅을 함.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //GPIO Pin 15번을 사용
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out모드를 사용, In으로 바꾸면 Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down을 설정    // GPIO_PuPd_UP(Pull Up 상태로 만들기), GPIO_PuPd_DOWN(Pull Down 상태 만들기)    // Default Pull-Up Mode로 설정되어있음,  Pull-Down 모드로 셋팅하세요
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type 설정, PP는 Push Pull, OD는 Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed를 설정. 100MHz 최대
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A포트의 GPIO 설정을, GPIO_InitStructre와 같이 초기화시킴
}

void Init_GPIO_OUT4()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO셋팅(Pin, Mode, PuPd, OType, Speed)과 관련된 구조체
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control의 약자, ClockSetting 또는 Reset을 할 때 사용됨.    //각 GPIO 포트(A,B,C,D,E)들은 AHB(Advanced High-performance Bus)라는 버스에 연결되어 있음.    //즉 위의 함수를 통해,  AHB1의 GPIOA를 사용 가능하게 하겠다(Enable)라는 의미.
  //앞에서 RCC함수를 통하여 GPIOA를 초기화시켰고, 이 이후로는 그 포트의 어떤 핀을 어떤 모드로 사용할 것인가   에 대한 셋팅을 함.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //GPIO Pin 15번을 사용
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out모드를 사용, In으로 바꾸면 Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down을 설정    // GPIO_PuPd_UP(Pull Up 상태로 만들기), GPIO_PuPd_DOWN(Pull Down 상태 만들기)    // Default Pull-Up Mode로 설정되어있음,  Pull-Down 모드로 셋팅하세요
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type 설정, PP는 Push Pull, OD는 Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed를 설정. 100MHz 최대
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A포트의 GPIO 설정을, GPIO_InitStructre와 같이 초기화시킴
}

void Init_GPIO_OUT5()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO셋팅(Pin, Mode, PuPd, OType, Speed)과 관련된 구조체
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control의 약자, ClockSetting 또는 Reset을 할 때 사용됨.    //각 GPIO 포트(A,B,C,D,E)들은 AHB(Advanced High-performance Bus)라는 버스에 연결되어 있음.    //즉 위의 함수를 통해,  AHB1의 GPIOA를 사용 가능하게 하겠다(Enable)라는 의미.
  //앞에서 RCC함수를 통하여 GPIOA를 초기화시켰고, 이 이후로는 그 포트의 어떤 핀을 어떤 모드로 사용할 것인가   에 대한 셋팅을 함.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; //GPIO Pin 15번을 사용
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out모드를 사용, In으로 바꾸면 Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down을 설정    // GPIO_PuPd_UP(Pull Up 상태로 만들기), GPIO_PuPd_DOWN(Pull Down 상태 만들기)    // Default Pull-Up Mode로 설정되어있음,  Pull-Down 모드로 셋팅하세요
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type 설정, PP는 Push Pull, OD는 Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed를 설정. 100MHz 최대
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A포트의 GPIO 설정을, GPIO_InitStructre와 같이 초기화시킴
}

void Init_GPIO_OUT6()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO셋팅(Pin, Mode, PuPd, OType, Speed)과 관련된 구조체
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control의 약자, ClockSetting 또는 Reset을 할 때 사용됨.    //각 GPIO 포트(A,B,C,D,E)들은 AHB(Advanced High-performance Bus)라는 버스에 연결되어 있음.    //즉 위의 함수를 통해,  AHB1의 GPIOA를 사용 가능하게 하겠다(Enable)라는 의미.
  //앞에서 RCC함수를 통하여 GPIOA를 초기화시켰고, 이 이후로는 그 포트의 어떤 핀을 어떤 모드로 사용할 것인가   에 대한 셋팅을 함.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //GPIO Pin 15번을 사용
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out모드를 사용, In으로 바꾸면 Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down을 설정    // GPIO_PuPd_UP(Pull Up 상태로 만들기), GPIO_PuPd_DOWN(Pull Down 상태 만들기)    // Default Pull-Up Mode로 설정되어있음,  Pull-Down 모드로 셋팅하세요
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type 설정, PP는 Push Pull, OD는 Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed를 설정. 100MHz 최대
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A포트의 GPIO 설정을, GPIO_InitStructre와 같이 초기화시킴
}

void Init_GPIO_OUT7()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO셋팅(Pin, Mode, PuPd, OType, Speed)과 관련된 구조체
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control의 약자, ClockSetting 또는 Reset을 할 때 사용됨.    //각 GPIO 포트(A,B,C,D,E)들은 AHB(Advanced High-performance Bus)라는 버스에 연결되어 있음.    //즉 위의 함수를 통해,  AHB1의 GPIOA를 사용 가능하게 하겠다(Enable)라는 의미.
  //앞에서 RCC함수를 통하여 GPIOA를 초기화시켰고, 이 이후로는 그 포트의 어떤 핀을 어떤 모드로 사용할 것인가   에 대한 셋팅을 함.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //GPIO Pin 15번을 사용
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out모드를 사용, In으로 바꾸면 Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down을 설정    // GPIO_PuPd_UP(Pull Up 상태로 만들기), GPIO_PuPd_DOWN(Pull Down 상태 만들기)    // Default Pull-Up Mode로 설정되어있음,  Pull-Down 모드로 셋팅하세요
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type 설정, PP는 Push Pull, OD는 Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed를 설정. 100MHz 최대
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A포트의 GPIO 설정을, GPIO_InitStructre와 같이 초기화시킴
}

void Init_GPIO_OUT8()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO셋팅(Pin, Mode, PuPd, OType, Speed)과 관련된 구조체
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control의 약자, ClockSetting 또는 Reset을 할 때 사용됨.    //각 GPIO 포트(A,B,C,D,E)들은 AHB(Advanced High-performance Bus)라는 버스에 연결되어 있음.    //즉 위의 함수를 통해,  AHB1의 GPIOA를 사용 가능하게 하겠다(Enable)라는 의미.
  //앞에서 RCC함수를 통하여 GPIOA를 초기화시켰고, 이 이후로는 그 포트의 어떤 핀을 어떤 모드로 사용할 것인가   에 대한 셋팅을 함.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //GPIO Pin 15번을 사용
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out모드를 사용, In으로 바꾸면 Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down을 설정    // GPIO_PuPd_UP(Pull Up 상태로 만들기), GPIO_PuPd_DOWN(Pull Down 상태 만들기)    // Default Pull-Up Mode로 설정되어있음,  Pull-Down 모드로 셋팅하세요
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type 설정, PP는 Push Pull, OD는 Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed를 설정. 100MHz 최대
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A포트의 GPIO 설정을, GPIO_InitStructre와 같이 초기화시킴
}

void Init_GPIO_OUT9()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO셋팅(Pin, Mode, PuPd, OType, Speed)과 관련된 구조체
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control의 약자, ClockSetting 또는 Reset을 할 때 사용됨.    //각 GPIO 포트(A,B,C,D,E)들은 AHB(Advanced High-performance Bus)라는 버스에 연결되어 있음.    //즉 위의 함수를 통해,  AHB1의 GPIOA를 사용 가능하게 하겠다(Enable)라는 의미.
  //앞에서 RCC함수를 통하여 GPIOA를 초기화시켰고, 이 이후로는 그 포트의 어떤 핀을 어떤 모드로 사용할 것인가   에 대한 셋팅을 함.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //GPIO Pin 15번을 사용
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out모드를 사용, In으로 바꾸면 Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down을 설정    // GPIO_PuPd_UP(Pull Up 상태로 만들기), GPIO_PuPd_DOWN(Pull Down 상태 만들기)    // Default Pull-Up Mode로 설정되어있음,  Pull-Down 모드로 셋팅하세요
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type 설정, PP는 Push Pull, OD는 Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed를 설정. 100MHz 최대
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A포트의 GPIO 설정을, GPIO_InitStructre와 같이 초기화시킴
}

void Init_GPIO_OUT10()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO셋팅(Pin, Mode, PuPd, OType, Speed)과 관련된 구조체
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control의 약자, ClockSetting 또는 Reset을 할 때 사용됨.    //각 GPIO 포트(A,B,C,D,E)들은 AHB(Advanced High-performance Bus)라는 버스에 연결되어 있음.    //즉 위의 함수를 통해,  AHB1의 GPIOA를 사용 가능하게 하겠다(Enable)라는 의미.
  //앞에서 RCC함수를 통하여 GPIOA를 초기화시켰고, 이 이후로는 그 포트의 어떤 핀을 어떤 모드로 사용할 것인가   에 대한 셋팅을 함.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //GPIO Pin 15번을 사용
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out모드를 사용, In으로 바꾸면 Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down을 설정    // GPIO_PuPd_UP(Pull Up 상태로 만들기), GPIO_PuPd_DOWN(Pull Down 상태 만들기)    // Default Pull-Up Mode로 설정되어있음,  Pull-Down 모드로 셋팅하세요
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type 설정, PP는 Push Pull, OD는 Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed를 설정. 100MHz 최대
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A포트의 GPIO 설정을, GPIO_InitStructre와 같이 초기화시킴
}

void Init_GPIO_OUT11()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO셋팅(Pin, Mode, PuPd, OType, Speed)과 관련된 구조체
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control의 약자, ClockSetting 또는 Reset을 할 때 사용됨.    //각 GPIO 포트(A,B,C,D,E)들은 AHB(Advanced High-performance Bus)라는 버스에 연결되어 있음.    //즉 위의 함수를 통해,  AHB1의 GPIOA를 사용 가능하게 하겠다(Enable)라는 의미.
  //앞에서 RCC함수를 통하여 GPIOA를 초기화시켰고, 이 이후로는 그 포트의 어떤 핀을 어떤 모드로 사용할 것인가   에 대한 셋팅을 함.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //GPIO Pin 15번을 사용
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out모드를 사용, In으로 바꾸면 Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down을 설정    // GPIO_PuPd_UP(Pull Up 상태로 만들기), GPIO_PuPd_DOWN(Pull Down 상태 만들기)    // Default Pull-Up Mode로 설정되어있음,  Pull-Down 모드로 셋팅하세요
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type 설정, PP는 Push Pull, OD는 Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed를 설정. 100MHz 최대
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A포트의 GPIO 설정을, GPIO_InitStructre와 같이 초기화시킴
}

void Init_GPIO_OUT12()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO셋팅(Pin, Mode, PuPd, OType, Speed)과 관련된 구조체
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control의 약자, ClockSetting 또는 Reset을 할 때 사용됨.    //각 GPIO 포트(A,B,C,D,E)들은 AHB(Advanced High-performance Bus)라는 버스에 연결되어 있음.    //즉 위의 함수를 통해,  AHB1의 GPIOA를 사용 가능하게 하겠다(Enable)라는 의미.
  //앞에서 RCC함수를 통하여 GPIOA를 초기화시켰고, 이 이후로는 그 포트의 어떤 핀을 어떤 모드로 사용할 것인가   에 대한 셋팅을 함.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //GPIO Pin 15번을 사용
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out모드를 사용, In으로 바꾸면 Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down을 설정    // GPIO_PuPd_UP(Pull Up 상태로 만들기), GPIO_PuPd_DOWN(Pull Down 상태 만들기)    // Default Pull-Up Mode로 설정되어있음,  Pull-Down 모드로 셋팅하세요
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type 설정, PP는 Push Pull, OD는 Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed를 설정. 100MHz 최대
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A포트의 GPIO 설정을, GPIO_InitStructre와 같이 초기화시킴
}

void Init_GPIO_OUT13()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO셋팅(Pin, Mode, PuPd, OType, Speed)과 관련된 구조체
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control의 약자, ClockSetting 또는 Reset을 할 때 사용됨.    //각 GPIO 포트(A,B,C,D,E)들은 AHB(Advanced High-performance Bus)라는 버스에 연결되어 있음.    //즉 위의 함수를 통해,  AHB1의 GPIOA를 사용 가능하게 하겠다(Enable)라는 의미.
  //앞에서 RCC함수를 통하여 GPIOA를 초기화시켰고, 이 이후로는 그 포트의 어떤 핀을 어떤 모드로 사용할 것인가   에 대한 셋팅을 함.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //GPIO Pin 15번을 사용
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out모드를 사용, In으로 바꾸면 Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down을 설정    // GPIO_PuPd_UP(Pull Up 상태로 만들기), GPIO_PuPd_DOWN(Pull Down 상태 만들기)    // Default Pull-Up Mode로 설정되어있음,  Pull-Down 모드로 셋팅하세요
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type 설정, PP는 Push Pull, OD는 Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed를 설정. 100MHz 최대
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A포트의 GPIO 설정을, GPIO_InitStructre와 같이 초기화시킴
}

void Init_GPIO_OUT14()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO셋팅(Pin, Mode, PuPd, OType, Speed)과 관련된 구조체
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control의 약자, ClockSetting 또는 Reset을 할 때 사용됨.    //각 GPIO 포트(A,B,C,D,E)들은 AHB(Advanced High-performance Bus)라는 버스에 연결되어 있음.    //즉 위의 함수를 통해,  AHB1의 GPIOA를 사용 가능하게 하겠다(Enable)라는 의미.
  //앞에서 RCC함수를 통하여 GPIOA를 초기화시켰고, 이 이후로는 그 포트의 어떤 핀을 어떤 모드로 사용할 것인가   에 대한 셋팅을 함.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; //GPIO Pin 15번을 사용
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out모드를 사용, In으로 바꾸면 Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down을 설정    // GPIO_PuPd_UP(Pull Up 상태로 만들기), GPIO_PuPd_DOWN(Pull Down 상태 만들기)    // Default Pull-Up Mode로 설정되어있음,  Pull-Down 모드로 셋팅하세요
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type 설정, PP는 Push Pull, OD는 Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed를 설정. 100MHz 최대
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A포트의 GPIO 설정을, GPIO_InitStructre와 같이 초기화시킴
}

void Init_GPIO_OUT15()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO셋팅(Pin, Mode, PuPd, OType, Speed)과 관련된 구조체
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control의 약자, ClockSetting 또는 Reset을 할 때 사용됨.    //각 GPIO 포트(A,B,C,D,E)들은 AHB(Advanced High-performance Bus)라는 버스에 연결되어 있음.    //즉 위의 함수를 통해,  AHB1의 GPIOA를 사용 가능하게 하겠다(Enable)라는 의미.
  //앞에서 RCC함수를 통하여 GPIOA를 초기화시켰고, 이 이후로는 그 포트의 어떤 핀을 어떤 모드로 사용할 것인가   에 대한 셋팅을 함.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; //GPIO Pin 15번을 사용
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out모드를 사용, In으로 바꾸면 Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down을 설정    // GPIO_PuPd_UP(Pull Up 상태로 만들기), GPIO_PuPd_DOWN(Pull Down 상태 만들기)    // Default Pull-Up Mode로 설정되어있음,  Pull-Down 모드로 셋팅하세요
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type 설정, PP는 Push Pull, OD는 Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed를 설정. 100MHz 최대
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A포트의 GPIO 설정을, GPIO_InitStructre와 같이 초기화시킴
}

void Init_GPIO_OUT16()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO셋팅(Pin, Mode, PuPd, OType, Speed)과 관련된 구조체
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control의 약자, ClockSetting 또는 Reset을 할 때 사용됨.    //각 GPIO 포트(A,B,C,D,E)들은 AHB(Advanced High-performance Bus)라는 버스에 연결되어 있음.    //즉 위의 함수를 통해,  AHB1의 GPIOA를 사용 가능하게 하겠다(Enable)라는 의미.
  //앞에서 RCC함수를 통하여 GPIOA를 초기화시켰고, 이 이후로는 그 포트의 어떤 핀을 어떤 모드로 사용할 것인가   에 대한 셋팅을 함.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //GPIO Pin 15번을 사용
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out모드를 사용, In으로 바꾸면 Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down을 설정    // GPIO_PuPd_UP(Pull Up 상태로 만들기), GPIO_PuPd_DOWN(Pull Down 상태 만들기)    // Default Pull-Up Mode로 설정되어있음,  Pull-Down 모드로 셋팅하세요
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type 설정, PP는 Push Pull, OD는 Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed를 설정. 100MHz 최대
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A포트의 GPIO 설정을, GPIO_InitStructre와 같이 초기화시킴
}

void Init_GPIO_OUT17()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO셋팅(Pin, Mode, PuPd, OType, Speed)과 관련된 구조체
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);    //Reset and Clock Control의 약자, ClockSetting 또는 Reset을 할 때 사용됨.    //각 GPIO 포트(A,B,C,D,E)들은 AHB(Advanced High-performance Bus)라는 버스에 연결되어 있음.    //즉 위의 함수를 통해,  AHB1의 GPIOA를 사용 가능하게 하겠다(Enable)라는 의미.
  //앞에서 RCC함수를 통하여 GPIOA를 초기화시켰고, 이 이후로는 그 포트의 어떤 핀을 어떤 모드로 사용할 것인가   에 대한 셋팅을 함.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //GPIO Pin 15번을 사용
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out모드를 사용, In으로 바꾸면 Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down을 설정    // GPIO_PuPd_UP(Pull Up 상태로 만들기), GPIO_PuPd_DOWN(Pull Down 상태 만들기)    // Default Pull-Up Mode로 설정되어있음,  Pull-Down 모드로 셋팅하세요
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type 설정, PP는 Push Pull, OD는 Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed를 설정. 100MHz 최대
  GPIO_Init(GPIOB, &GPIO_InitStructure);  // A포트의 GPIO 설정을, GPIO_InitStructre와 같이 초기화시킴
}

void Init_GPIO_OUT18()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO셋팅(Pin, Mode, PuPd, OType, Speed)과 관련된 구조체
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);    //Reset and Clock Control의 약자, ClockSetting 또는 Reset을 할 때 사용됨.    //각 GPIO 포트(A,B,C,D,E)들은 AHB(Advanced High-performance Bus)라는 버스에 연결되어 있음.    //즉 위의 함수를 통해,  AHB1의 GPIOA를 사용 가능하게 하겠다(Enable)라는 의미.
  //앞에서 RCC함수를 통하여 GPIOA를 초기화시켰고, 이 이후로는 그 포트의 어떤 핀을 어떤 모드로 사용할 것인가   에 대한 셋팅을 함.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //GPIO Pin 15번을 사용
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out모드를 사용, In으로 바꾸면 Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down을 설정    // GPIO_PuPd_UP(Pull Up 상태로 만들기), GPIO_PuPd_DOWN(Pull Down 상태 만들기)    // Default Pull-Up Mode로 설정되어있음,  Pull-Down 모드로 셋팅하세요
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type 설정, PP는 Push Pull, OD는 Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed를 설정. 100MHz 최대
  GPIO_Init(GPIOB, &GPIO_InitStructure);  // A포트의 GPIO 설정을, GPIO_InitStructre와 같이 초기화시킴
}

//테스트용
void Init_GPIO_OUT19()
{
    GPIO_InitTypeDef GPIO_InitStructure; //GPIO셋팅(Pin, Mode, PuPd, OType, Speed)과 관련된 구조체
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);    //Reset and Clock Control의 약자, ClockSetting 또는 Reset을 할 때 사용됨.    //각 GPIO 포트(A,B,C,D,E)들은 AHB(Advanced High-performance Bus)라는 버스에 연결되어 있음.    //즉 위의 함수를 통해,  AHB1의 GPIOA를 사용 가능하게 하겠다(Enable)라는 의미.
    //앞에서 RCC함수를 통하여 GPIOA를 초기화시켰고, 이 이후로는 그 포트의 어떤 핀을 어떤 모드로 사용할 것인가   에 대한 셋팅을 함.
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //GPIO Pin 15번을 사용
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out모드를 사용, In으로 바꾸면 Read
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down을 설정    // GPIO_PuPd_UP(Pull Up 상태로 만들기), GPIO_PuPd_DOWN(Pull Down 상태 만들기)    // Default Pull-Up Mode로 설정되어있음,  Pull-Down 모드로 셋팅하세요
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type 설정, PP는 Push Pull, OD는 Open-Drain
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed를 설정. 100MHz 최대
    GPIO_Init(GPIOA, &GPIO_InitStructure);  // A포트의 GPIO 설정을, GPIO_InitStructre와 같이 초기화시킴
}