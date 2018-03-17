#include "all_header.h"

//���̽�ƽ ���Ű� Ÿ�̸ӿ� ����Ͽ� ���� �ð� ��� ����
int time_cal_pin_array[9]={0,};
int time_start_pin_array[9]={0.};
int time_end_pin_array[9]={-1,};
int pin_array_off[9]={0,};
int pin_array_i;

//�ɾ�� ������ ���ļ��� �����ϴ� ����
int Pin_f[9] = {0,};          //���� ���ļ�
int Pin_Hz[9] = {0,};       //���� TIM7 ���ļ�
int one_Hz = 1;
int pin_i[9]={0,};


//�׽�Ʈ�� (LED)
 int j;
 int j_flag=0;

void changeTimerXperiod7( uint32_t TIMER_FREQ ){//Ÿ�̸� �ֱ� �ǽð� ��ȭ
  uint16_t PrescalerValue;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//Timer���� ����ü
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
  uint16_t PrescalerValue;    //uint16_t �ڷ����� ���� ����.      
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  //Timer���� ����ü
  NVIC_InitTypeDef NVIC_InitStructure;     //NVIC���� ����ü
  NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;        //Ÿ�̸� 2���� ���� ���ͷ�Ʈ ���
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f; //�켱����
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0f;    //���� �켱����
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //���ͷ�Ʈ ä�� ��밡�� �ϵ�����.
  NVIC_Init(&NVIC_InitStructure);     //NVIC �ʱ�ȭ (���ͷ�Ʈ ��û�غ�ܰ� ��)=>���� ���� �ϱ����� �ڵ鷯�� ��������.
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);    //TIM2���� ��밡���ϵ��� TM
  SystemCoreClockUpdate();  //ü���� ����
  PrescalerValue = (uint16_t) (SystemCoreClock /2/ TIMER_PRESCALER_FREQ)*10; // => ������ 83 ��?
  TIM_TimeBaseStructure.TIM_Period = 100;//-1�� �����÷ο� ������? 4545
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue; TIM_TimeBaseStructure.TIM_ClockDivision = 0;  //�߿����� X
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;    //Ÿ�̸Ӹ� ���� ���
  TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);    //Ÿ�̸� 2���� �ʱ�ȭ
  /* TIM IT enable */
  TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);    //Ÿ�̸Ӹ� �����̳� �Ⱦ����̳� �ϴ� ��
  /* TIM3 enable counter */
  TIM_Cmd(TIM7, ENABLE);    //Ÿ�̸� 2���� ����ϰڴٴ� ����
}

void Init_GPIO_OUT1()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO����(Pin, Mode, PuPd, OType, Speed)�� ���õ� ����ü
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control�� ����, ClockSetting �Ǵ� Reset�� �� �� ����.    //�� GPIO ��Ʈ(A,B,C,D,E)���� AHB(Advanced High-performance Bus)��� ������ ����Ǿ� ����.    //�� ���� �Լ��� ����,  AHB1�� GPIOA�� ��� �����ϰ� �ϰڴ�(Enable)��� �ǹ�.
  //�տ��� RCC�Լ��� ���Ͽ� GPIOA�� �ʱ�ȭ���װ�, �� ���ķδ� �� ��Ʈ�� � ���� � ���� ����� ���ΰ�   �� ���� ������ ��.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //GPIO Pin 15���� ���
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out��带 ���, In���� �ٲٸ� Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down�� ����    // GPIO_PuPd_UP(Pull Up ���·� �����), GPIO_PuPd_DOWN(Pull Down ���� �����)    // Default Pull-Up Mode�� �����Ǿ�����,  Pull-Down ���� �����ϼ���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type ����, PP�� Push Pull, OD�� Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed�� ����. 100MHz �ִ�
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // E��Ʈ�� GPIO ������, GPIO_InitStructre�� ���� �ʱ�ȭ��Ŵ
}

void Init_GPIO_OUT2()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO����(Pin, Mode, PuPd, OType, Speed)�� ���õ� ����ü
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control�� ����, ClockSetting �Ǵ� Reset�� �� �� ����.    //�� GPIO ��Ʈ(A,B,C,D,E)���� AHB(Advanced High-performance Bus)��� ������ ����Ǿ� ����.    //�� ���� �Լ��� ����,  AHB1�� GPIOA�� ��� �����ϰ� �ϰڴ�(Enable)��� �ǹ�.
  //�տ��� RCC�Լ��� ���Ͽ� GPIOA�� �ʱ�ȭ���װ�, �� ���ķδ� �� ��Ʈ�� � ���� � ���� ����� ���ΰ�   �� ���� ������ ��.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //GPIO Pin 15���� ���
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out��带 ���, In���� �ٲٸ� Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down�� ����    // GPIO_PuPd_UP(Pull Up ���·� �����), GPIO_PuPd_DOWN(Pull Down ���� �����)    // Default Pull-Up Mode�� �����Ǿ�����,  Pull-Down ���� �����ϼ���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type ����, PP�� Push Pull, OD�� Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed�� ����. 100MHz �ִ�
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A��Ʈ�� GPIO ������, GPIO_InitStructre�� ���� �ʱ�ȭ��Ŵ
}

void Init_GPIO_OUT3()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO����(Pin, Mode, PuPd, OType, Speed)�� ���õ� ����ü
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control�� ����, ClockSetting �Ǵ� Reset�� �� �� ����.    //�� GPIO ��Ʈ(A,B,C,D,E)���� AHB(Advanced High-performance Bus)��� ������ ����Ǿ� ����.    //�� ���� �Լ��� ����,  AHB1�� GPIOA�� ��� �����ϰ� �ϰڴ�(Enable)��� �ǹ�.
  //�տ��� RCC�Լ��� ���Ͽ� GPIOA�� �ʱ�ȭ���װ�, �� ���ķδ� �� ��Ʈ�� � ���� � ���� ����� ���ΰ�   �� ���� ������ ��.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //GPIO Pin 15���� ���
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out��带 ���, In���� �ٲٸ� Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down�� ����    // GPIO_PuPd_UP(Pull Up ���·� �����), GPIO_PuPd_DOWN(Pull Down ���� �����)    // Default Pull-Up Mode�� �����Ǿ�����,  Pull-Down ���� �����ϼ���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type ����, PP�� Push Pull, OD�� Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed�� ����. 100MHz �ִ�
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A��Ʈ�� GPIO ������, GPIO_InitStructre�� ���� �ʱ�ȭ��Ŵ
}

void Init_GPIO_OUT4()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO����(Pin, Mode, PuPd, OType, Speed)�� ���õ� ����ü
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control�� ����, ClockSetting �Ǵ� Reset�� �� �� ����.    //�� GPIO ��Ʈ(A,B,C,D,E)���� AHB(Advanced High-performance Bus)��� ������ ����Ǿ� ����.    //�� ���� �Լ��� ����,  AHB1�� GPIOA�� ��� �����ϰ� �ϰڴ�(Enable)��� �ǹ�.
  //�տ��� RCC�Լ��� ���Ͽ� GPIOA�� �ʱ�ȭ���װ�, �� ���ķδ� �� ��Ʈ�� � ���� � ���� ����� ���ΰ�   �� ���� ������ ��.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //GPIO Pin 15���� ���
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out��带 ���, In���� �ٲٸ� Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down�� ����    // GPIO_PuPd_UP(Pull Up ���·� �����), GPIO_PuPd_DOWN(Pull Down ���� �����)    // Default Pull-Up Mode�� �����Ǿ�����,  Pull-Down ���� �����ϼ���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type ����, PP�� Push Pull, OD�� Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed�� ����. 100MHz �ִ�
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A��Ʈ�� GPIO ������, GPIO_InitStructre�� ���� �ʱ�ȭ��Ŵ
}

void Init_GPIO_OUT5()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO����(Pin, Mode, PuPd, OType, Speed)�� ���õ� ����ü
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control�� ����, ClockSetting �Ǵ� Reset�� �� �� ����.    //�� GPIO ��Ʈ(A,B,C,D,E)���� AHB(Advanced High-performance Bus)��� ������ ����Ǿ� ����.    //�� ���� �Լ��� ����,  AHB1�� GPIOA�� ��� �����ϰ� �ϰڴ�(Enable)��� �ǹ�.
  //�տ��� RCC�Լ��� ���Ͽ� GPIOA�� �ʱ�ȭ���װ�, �� ���ķδ� �� ��Ʈ�� � ���� � ���� ����� ���ΰ�   �� ���� ������ ��.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; //GPIO Pin 15���� ���
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out��带 ���, In���� �ٲٸ� Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down�� ����    // GPIO_PuPd_UP(Pull Up ���·� �����), GPIO_PuPd_DOWN(Pull Down ���� �����)    // Default Pull-Up Mode�� �����Ǿ�����,  Pull-Down ���� �����ϼ���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type ����, PP�� Push Pull, OD�� Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed�� ����. 100MHz �ִ�
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A��Ʈ�� GPIO ������, GPIO_InitStructre�� ���� �ʱ�ȭ��Ŵ
}

void Init_GPIO_OUT6()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO����(Pin, Mode, PuPd, OType, Speed)�� ���õ� ����ü
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control�� ����, ClockSetting �Ǵ� Reset�� �� �� ����.    //�� GPIO ��Ʈ(A,B,C,D,E)���� AHB(Advanced High-performance Bus)��� ������ ����Ǿ� ����.    //�� ���� �Լ��� ����,  AHB1�� GPIOA�� ��� �����ϰ� �ϰڴ�(Enable)��� �ǹ�.
  //�տ��� RCC�Լ��� ���Ͽ� GPIOA�� �ʱ�ȭ���װ�, �� ���ķδ� �� ��Ʈ�� � ���� � ���� ����� ���ΰ�   �� ���� ������ ��.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //GPIO Pin 15���� ���
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out��带 ���, In���� �ٲٸ� Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down�� ����    // GPIO_PuPd_UP(Pull Up ���·� �����), GPIO_PuPd_DOWN(Pull Down ���� �����)    // Default Pull-Up Mode�� �����Ǿ�����,  Pull-Down ���� �����ϼ���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type ����, PP�� Push Pull, OD�� Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed�� ����. 100MHz �ִ�
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A��Ʈ�� GPIO ������, GPIO_InitStructre�� ���� �ʱ�ȭ��Ŵ
}

void Init_GPIO_OUT7()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO����(Pin, Mode, PuPd, OType, Speed)�� ���õ� ����ü
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control�� ����, ClockSetting �Ǵ� Reset�� �� �� ����.    //�� GPIO ��Ʈ(A,B,C,D,E)���� AHB(Advanced High-performance Bus)��� ������ ����Ǿ� ����.    //�� ���� �Լ��� ����,  AHB1�� GPIOA�� ��� �����ϰ� �ϰڴ�(Enable)��� �ǹ�.
  //�տ��� RCC�Լ��� ���Ͽ� GPIOA�� �ʱ�ȭ���װ�, �� ���ķδ� �� ��Ʈ�� � ���� � ���� ����� ���ΰ�   �� ���� ������ ��.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //GPIO Pin 15���� ���
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out��带 ���, In���� �ٲٸ� Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down�� ����    // GPIO_PuPd_UP(Pull Up ���·� �����), GPIO_PuPd_DOWN(Pull Down ���� �����)    // Default Pull-Up Mode�� �����Ǿ�����,  Pull-Down ���� �����ϼ���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type ����, PP�� Push Pull, OD�� Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed�� ����. 100MHz �ִ�
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A��Ʈ�� GPIO ������, GPIO_InitStructre�� ���� �ʱ�ȭ��Ŵ
}

void Init_GPIO_OUT8()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO����(Pin, Mode, PuPd, OType, Speed)�� ���õ� ����ü
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control�� ����, ClockSetting �Ǵ� Reset�� �� �� ����.    //�� GPIO ��Ʈ(A,B,C,D,E)���� AHB(Advanced High-performance Bus)��� ������ ����Ǿ� ����.    //�� ���� �Լ��� ����,  AHB1�� GPIOA�� ��� �����ϰ� �ϰڴ�(Enable)��� �ǹ�.
  //�տ��� RCC�Լ��� ���Ͽ� GPIOA�� �ʱ�ȭ���װ�, �� ���ķδ� �� ��Ʈ�� � ���� � ���� ����� ���ΰ�   �� ���� ������ ��.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //GPIO Pin 15���� ���
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out��带 ���, In���� �ٲٸ� Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down�� ����    // GPIO_PuPd_UP(Pull Up ���·� �����), GPIO_PuPd_DOWN(Pull Down ���� �����)    // Default Pull-Up Mode�� �����Ǿ�����,  Pull-Down ���� �����ϼ���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type ����, PP�� Push Pull, OD�� Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed�� ����. 100MHz �ִ�
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A��Ʈ�� GPIO ������, GPIO_InitStructre�� ���� �ʱ�ȭ��Ŵ
}

void Init_GPIO_OUT9()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO����(Pin, Mode, PuPd, OType, Speed)�� ���õ� ����ü
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control�� ����, ClockSetting �Ǵ� Reset�� �� �� ����.    //�� GPIO ��Ʈ(A,B,C,D,E)���� AHB(Advanced High-performance Bus)��� ������ ����Ǿ� ����.    //�� ���� �Լ��� ����,  AHB1�� GPIOA�� ��� �����ϰ� �ϰڴ�(Enable)��� �ǹ�.
  //�տ��� RCC�Լ��� ���Ͽ� GPIOA�� �ʱ�ȭ���װ�, �� ���ķδ� �� ��Ʈ�� � ���� � ���� ����� ���ΰ�   �� ���� ������ ��.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //GPIO Pin 15���� ���
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out��带 ���, In���� �ٲٸ� Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down�� ����    // GPIO_PuPd_UP(Pull Up ���·� �����), GPIO_PuPd_DOWN(Pull Down ���� �����)    // Default Pull-Up Mode�� �����Ǿ�����,  Pull-Down ���� �����ϼ���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type ����, PP�� Push Pull, OD�� Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed�� ����. 100MHz �ִ�
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A��Ʈ�� GPIO ������, GPIO_InitStructre�� ���� �ʱ�ȭ��Ŵ
}

void Init_GPIO_OUT10()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO����(Pin, Mode, PuPd, OType, Speed)�� ���õ� ����ü
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control�� ����, ClockSetting �Ǵ� Reset�� �� �� ����.    //�� GPIO ��Ʈ(A,B,C,D,E)���� AHB(Advanced High-performance Bus)��� ������ ����Ǿ� ����.    //�� ���� �Լ��� ����,  AHB1�� GPIOA�� ��� �����ϰ� �ϰڴ�(Enable)��� �ǹ�.
  //�տ��� RCC�Լ��� ���Ͽ� GPIOA�� �ʱ�ȭ���װ�, �� ���ķδ� �� ��Ʈ�� � ���� � ���� ����� ���ΰ�   �� ���� ������ ��.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //GPIO Pin 15���� ���
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out��带 ���, In���� �ٲٸ� Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down�� ����    // GPIO_PuPd_UP(Pull Up ���·� �����), GPIO_PuPd_DOWN(Pull Down ���� �����)    // Default Pull-Up Mode�� �����Ǿ�����,  Pull-Down ���� �����ϼ���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type ����, PP�� Push Pull, OD�� Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed�� ����. 100MHz �ִ�
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A��Ʈ�� GPIO ������, GPIO_InitStructre�� ���� �ʱ�ȭ��Ŵ
}

void Init_GPIO_OUT11()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO����(Pin, Mode, PuPd, OType, Speed)�� ���õ� ����ü
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control�� ����, ClockSetting �Ǵ� Reset�� �� �� ����.    //�� GPIO ��Ʈ(A,B,C,D,E)���� AHB(Advanced High-performance Bus)��� ������ ����Ǿ� ����.    //�� ���� �Լ��� ����,  AHB1�� GPIOA�� ��� �����ϰ� �ϰڴ�(Enable)��� �ǹ�.
  //�տ��� RCC�Լ��� ���Ͽ� GPIOA�� �ʱ�ȭ���װ�, �� ���ķδ� �� ��Ʈ�� � ���� � ���� ����� ���ΰ�   �� ���� ������ ��.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //GPIO Pin 15���� ���
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out��带 ���, In���� �ٲٸ� Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down�� ����    // GPIO_PuPd_UP(Pull Up ���·� �����), GPIO_PuPd_DOWN(Pull Down ���� �����)    // Default Pull-Up Mode�� �����Ǿ�����,  Pull-Down ���� �����ϼ���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type ����, PP�� Push Pull, OD�� Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed�� ����. 100MHz �ִ�
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A��Ʈ�� GPIO ������, GPIO_InitStructre�� ���� �ʱ�ȭ��Ŵ
}

void Init_GPIO_OUT12()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO����(Pin, Mode, PuPd, OType, Speed)�� ���õ� ����ü
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control�� ����, ClockSetting �Ǵ� Reset�� �� �� ����.    //�� GPIO ��Ʈ(A,B,C,D,E)���� AHB(Advanced High-performance Bus)��� ������ ����Ǿ� ����.    //�� ���� �Լ��� ����,  AHB1�� GPIOA�� ��� �����ϰ� �ϰڴ�(Enable)��� �ǹ�.
  //�տ��� RCC�Լ��� ���Ͽ� GPIOA�� �ʱ�ȭ���װ�, �� ���ķδ� �� ��Ʈ�� � ���� � ���� ����� ���ΰ�   �� ���� ������ ��.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //GPIO Pin 15���� ���
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out��带 ���, In���� �ٲٸ� Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down�� ����    // GPIO_PuPd_UP(Pull Up ���·� �����), GPIO_PuPd_DOWN(Pull Down ���� �����)    // Default Pull-Up Mode�� �����Ǿ�����,  Pull-Down ���� �����ϼ���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type ����, PP�� Push Pull, OD�� Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed�� ����. 100MHz �ִ�
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A��Ʈ�� GPIO ������, GPIO_InitStructre�� ���� �ʱ�ȭ��Ŵ
}

void Init_GPIO_OUT13()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO����(Pin, Mode, PuPd, OType, Speed)�� ���õ� ����ü
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control�� ����, ClockSetting �Ǵ� Reset�� �� �� ����.    //�� GPIO ��Ʈ(A,B,C,D,E)���� AHB(Advanced High-performance Bus)��� ������ ����Ǿ� ����.    //�� ���� �Լ��� ����,  AHB1�� GPIOA�� ��� �����ϰ� �ϰڴ�(Enable)��� �ǹ�.
  //�տ��� RCC�Լ��� ���Ͽ� GPIOA�� �ʱ�ȭ���װ�, �� ���ķδ� �� ��Ʈ�� � ���� � ���� ����� ���ΰ�   �� ���� ������ ��.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //GPIO Pin 15���� ���
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out��带 ���, In���� �ٲٸ� Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down�� ����    // GPIO_PuPd_UP(Pull Up ���·� �����), GPIO_PuPd_DOWN(Pull Down ���� �����)    // Default Pull-Up Mode�� �����Ǿ�����,  Pull-Down ���� �����ϼ���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type ����, PP�� Push Pull, OD�� Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed�� ����. 100MHz �ִ�
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A��Ʈ�� GPIO ������, GPIO_InitStructre�� ���� �ʱ�ȭ��Ŵ
}

void Init_GPIO_OUT14()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO����(Pin, Mode, PuPd, OType, Speed)�� ���õ� ����ü
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control�� ����, ClockSetting �Ǵ� Reset�� �� �� ����.    //�� GPIO ��Ʈ(A,B,C,D,E)���� AHB(Advanced High-performance Bus)��� ������ ����Ǿ� ����.    //�� ���� �Լ��� ����,  AHB1�� GPIOA�� ��� �����ϰ� �ϰڴ�(Enable)��� �ǹ�.
  //�տ��� RCC�Լ��� ���Ͽ� GPIOA�� �ʱ�ȭ���װ�, �� ���ķδ� �� ��Ʈ�� � ���� � ���� ����� ���ΰ�   �� ���� ������ ��.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; //GPIO Pin 15���� ���
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out��带 ���, In���� �ٲٸ� Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down�� ����    // GPIO_PuPd_UP(Pull Up ���·� �����), GPIO_PuPd_DOWN(Pull Down ���� �����)    // Default Pull-Up Mode�� �����Ǿ�����,  Pull-Down ���� �����ϼ���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type ����, PP�� Push Pull, OD�� Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed�� ����. 100MHz �ִ�
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A��Ʈ�� GPIO ������, GPIO_InitStructre�� ���� �ʱ�ȭ��Ŵ
}

void Init_GPIO_OUT15()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO����(Pin, Mode, PuPd, OType, Speed)�� ���õ� ����ü
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control�� ����, ClockSetting �Ǵ� Reset�� �� �� ����.    //�� GPIO ��Ʈ(A,B,C,D,E)���� AHB(Advanced High-performance Bus)��� ������ ����Ǿ� ����.    //�� ���� �Լ��� ����,  AHB1�� GPIOA�� ��� �����ϰ� �ϰڴ�(Enable)��� �ǹ�.
  //�տ��� RCC�Լ��� ���Ͽ� GPIOA�� �ʱ�ȭ���װ�, �� ���ķδ� �� ��Ʈ�� � ���� � ���� ����� ���ΰ�   �� ���� ������ ��.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; //GPIO Pin 15���� ���
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out��带 ���, In���� �ٲٸ� Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down�� ����    // GPIO_PuPd_UP(Pull Up ���·� �����), GPIO_PuPd_DOWN(Pull Down ���� �����)    // Default Pull-Up Mode�� �����Ǿ�����,  Pull-Down ���� �����ϼ���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type ����, PP�� Push Pull, OD�� Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed�� ����. 100MHz �ִ�
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A��Ʈ�� GPIO ������, GPIO_InitStructre�� ���� �ʱ�ȭ��Ŵ
}

void Init_GPIO_OUT16()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO����(Pin, Mode, PuPd, OType, Speed)�� ���õ� ����ü
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);    //Reset and Clock Control�� ����, ClockSetting �Ǵ� Reset�� �� �� ����.    //�� GPIO ��Ʈ(A,B,C,D,E)���� AHB(Advanced High-performance Bus)��� ������ ����Ǿ� ����.    //�� ���� �Լ��� ����,  AHB1�� GPIOA�� ��� �����ϰ� �ϰڴ�(Enable)��� �ǹ�.
  //�տ��� RCC�Լ��� ���Ͽ� GPIOA�� �ʱ�ȭ���װ�, �� ���ķδ� �� ��Ʈ�� � ���� � ���� ����� ���ΰ�   �� ���� ������ ��.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //GPIO Pin 15���� ���
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out��带 ���, In���� �ٲٸ� Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down�� ����    // GPIO_PuPd_UP(Pull Up ���·� �����), GPIO_PuPd_DOWN(Pull Down ���� �����)    // Default Pull-Up Mode�� �����Ǿ�����,  Pull-Down ���� �����ϼ���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type ����, PP�� Push Pull, OD�� Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed�� ����. 100MHz �ִ�
  GPIO_Init(GPIOE, &GPIO_InitStructure);  // A��Ʈ�� GPIO ������, GPIO_InitStructre�� ���� �ʱ�ȭ��Ŵ
}

void Init_GPIO_OUT17()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO����(Pin, Mode, PuPd, OType, Speed)�� ���õ� ����ü
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);    //Reset and Clock Control�� ����, ClockSetting �Ǵ� Reset�� �� �� ����.    //�� GPIO ��Ʈ(A,B,C,D,E)���� AHB(Advanced High-performance Bus)��� ������ ����Ǿ� ����.    //�� ���� �Լ��� ����,  AHB1�� GPIOA�� ��� �����ϰ� �ϰڴ�(Enable)��� �ǹ�.
  //�տ��� RCC�Լ��� ���Ͽ� GPIOA�� �ʱ�ȭ���װ�, �� ���ķδ� �� ��Ʈ�� � ���� � ���� ����� ���ΰ�   �� ���� ������ ��.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //GPIO Pin 15���� ���
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out��带 ���, In���� �ٲٸ� Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down�� ����    // GPIO_PuPd_UP(Pull Up ���·� �����), GPIO_PuPd_DOWN(Pull Down ���� �����)    // Default Pull-Up Mode�� �����Ǿ�����,  Pull-Down ���� �����ϼ���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type ����, PP�� Push Pull, OD�� Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed�� ����. 100MHz �ִ�
  GPIO_Init(GPIOB, &GPIO_InitStructure);  // A��Ʈ�� GPIO ������, GPIO_InitStructre�� ���� �ʱ�ȭ��Ŵ
}

void Init_GPIO_OUT18()
{
  GPIO_InitTypeDef GPIO_InitStructure; //GPIO����(Pin, Mode, PuPd, OType, Speed)�� ���õ� ����ü
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);    //Reset and Clock Control�� ����, ClockSetting �Ǵ� Reset�� �� �� ����.    //�� GPIO ��Ʈ(A,B,C,D,E)���� AHB(Advanced High-performance Bus)��� ������ ����Ǿ� ����.    //�� ���� �Լ��� ����,  AHB1�� GPIOA�� ��� �����ϰ� �ϰڴ�(Enable)��� �ǹ�.
  //�տ��� RCC�Լ��� ���Ͽ� GPIOA�� �ʱ�ȭ���װ�, �� ���ķδ� �� ��Ʈ�� � ���� � ���� ����� ���ΰ�   �� ���� ������ ��.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //GPIO Pin 15���� ���
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out��带 ���, In���� �ٲٸ� Read
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down�� ����    // GPIO_PuPd_UP(Pull Up ���·� �����), GPIO_PuPd_DOWN(Pull Down ���� �����)    // Default Pull-Up Mode�� �����Ǿ�����,  Pull-Down ���� �����ϼ���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type ����, PP�� Push Pull, OD�� Open-Drain
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed�� ����. 100MHz �ִ�
  GPIO_Init(GPIOB, &GPIO_InitStructure);  // A��Ʈ�� GPIO ������, GPIO_InitStructre�� ���� �ʱ�ȭ��Ŵ
}

//�׽�Ʈ��
void Init_GPIO_OUT19()
{
    GPIO_InitTypeDef GPIO_InitStructure; //GPIO����(Pin, Mode, PuPd, OType, Speed)�� ���õ� ����ü
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);    //Reset and Clock Control�� ����, ClockSetting �Ǵ� Reset�� �� �� ����.    //�� GPIO ��Ʈ(A,B,C,D,E)���� AHB(Advanced High-performance Bus)��� ������ ����Ǿ� ����.    //�� ���� �Լ��� ����,  AHB1�� GPIOA�� ��� �����ϰ� �ϰڴ�(Enable)��� �ǹ�.
    //�տ��� RCC�Լ��� ���Ͽ� GPIOA�� �ʱ�ȭ���װ�, �� ���ķδ� �� ��Ʈ�� � ���� � ���� ����� ���ΰ�   �� ���� ������ ��.
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //GPIO Pin 15���� ���
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //Out��带 ���, In���� �ٲٸ� Read
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down�� ����    // GPIO_PuPd_UP(Pull Up ���·� �����), GPIO_PuPd_DOWN(Pull Down ���� �����)    // Default Pull-Up Mode�� �����Ǿ�����,  Pull-Down ���� �����ϼ���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type ����, PP�� Push Pull, OD�� Open-Drain
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed�� ����. 100MHz �ִ�
    GPIO_Init(GPIOA, &GPIO_InitStructure);  // A��Ʈ�� GPIO ������, GPIO_InitStructre�� ���� �ʱ�ȭ��Ŵ
}