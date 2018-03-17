#include "all_header.h"

int button_input[6]=0;
int button_value=0;
int button_i=0;

//pin_array button
void Init_GPIO_IN1()
{
    GPIO_InitTypeDef GPIO_InitStructure; //GPIO셋팅(Pin, Mode, PuPd, OType, Speed)과 관련된 구조체

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//Reset and Clock Control의 약자, ClockSetting 또는 Reset을 할 때 사용됨;    //각 GPIO 포트(A,B,C,D,E)들은 AHB(Advanced High-performance Bus)라는 버스에 연결되어 있음.    //즉 위의 함수를 통해,  AHB1의 GPIOA를 사용 가능하게 하겠다(Enable)라는 의미.
 
    //앞에서 RCC함수를 통하여 GPIOA를 초기화시켰고, 이 이후로는 그 포트의 어떤 핀을 어떤 모드로 사용할 것인   에 대한 셋팅을 함.
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //GPIO Pin 1번을 사용
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //In모드를 사용

    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down을 설정// GPIO_PuPd_UP(Pull Up 상태로 만들기), GPIO_PuPd_DOWN(Pull Down 상태 만들기)// NOPULL상태로 셋팅
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type 설정, PP는 Push Pull, OD는 Open-Drain

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed를 설정. 100MHz 최대

    GPIO_Init(GPIOA, &GPIO_InitStructure);  // A포트의 GPIO 설정을, GPIO_InitStructre와 같이 초기화시킴
}

//joy_stick button
void Init_GPIO_IN2()
{
    GPIO_InitTypeDef GPIO_InitStructure; //GPIO셋팅(Pin, Mode, PuPd, OType, Speed)과 관련된 구조체

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//Reset and Clock Control의 약자, ClockSetting 또는 Reset을 할 때 사용됨;    //각 GPIO 포트(A,B,C,D,E)들은 AHB(Advanced High-performance Bus)라는 버스에 연결되어 있음.    //즉 위의 함수를 통해,  AHB1의 GPIOA를 사용 가능하게 하겠다(Enable)라는 의미.
 
    //앞에서 RCC함수를 통하여 GPIOA를 초기화시켰고, 이 이후로는 그 포트의 어떤 핀을 어떤 모드로 사용할 것인   에 대한 셋팅을 함.
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //GPIO Pin 1번을 사용
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //In모드를 사용

    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down을 설정// GPIO_PuPd_UP(Pull Up 상태로 만들기), GPIO_PuPd_DOWN(Pull Down 상태 만들기)// NOPULL상태로 셋팅
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type 설정, PP는 Push Pull, OD는 Open-Drain

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed를 설정. 100MHz 최대

    GPIO_Init(GPIOA, &GPIO_InitStructure);  // A포트의 GPIO 설정을, GPIO_InitStructre와 같이 초기화시킴
}

//general button-------------------------------------------------------------------------------------
void Init_GPIO_IN3()
{
    GPIO_InitTypeDef GPIO_InitStructure; //GPIO셋팅(Pin, Mode, PuPd, OType, Speed)과 관련된 구조체

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//Reset and Clock Control의 약자, ClockSetting 또는 Reset을 할 때 사용됨;    //각 GPIO 포트(A,B,C,D,E)들은 AHB(Advanced High-performance Bus)라는 버스에 연결되어 있음.    //즉 위의 함수를 통해,  AHB1의 GPIOA를 사용 가능하게 하겠다(Enable)라는 의미.
 
    //앞에서 RCC함수를 통하여 GPIOA를 초기화시켰고, 이 이후로는 그 포트의 어떤 핀을 어떤 모드로 사용할 것인   에 대한 셋팅을 함.
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //GPIO Pin 1번을 사용
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //In모드를 사용

    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down을 설정// GPIO_PuPd_UP(Pull Up 상태로 만들기), GPIO_PuPd_DOWN(Pull Down 상태 만들기)// NOPULL상태로 셋팅
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type 설정, PP는 Push Pull, OD는 Open-Drain

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed를 설정. 100MHz 최대

    GPIO_Init(GPIOA, &GPIO_InitStructure);  // A포트의 GPIO 설정을, GPIO_InitStructre와 같이 초기화시킴
}


void Init_GPIO_IN4()
{
    GPIO_InitTypeDef GPIO_InitStructure; //GPIO셋팅(Pin, Mode, PuPd, OType, Speed)과 관련된 구조체

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//Reset and Clock Control의 약자, ClockSetting 또는 Reset을 할 때 사용됨;    //각 GPIO 포트(A,B,C,D,E)들은 AHB(Advanced High-performance Bus)라는 버스에 연결되어 있음.    //즉 위의 함수를 통해,  AHB1의 GPIOA를 사용 가능하게 하겠다(Enable)라는 의미.
 
    //앞에서 RCC함수를 통하여 GPIOA를 초기화시켰고, 이 이후로는 그 포트의 어떤 핀을 어떤 모드로 사용할 것인   에 대한 셋팅을 함.
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; //GPIO Pin 1번을 사용
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //In모드를 사용

    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down을 설정// GPIO_PuPd_UP(Pull Up 상태로 만들기), GPIO_PuPd_DOWN(Pull Down 상태 만들기)// NOPULL상태로 셋팅
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type 설정, PP는 Push Pull, OD는 Open-Drain

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed를 설정. 100MHz 최대

    GPIO_Init(GPIOA, &GPIO_InitStructure);  // A포트의 GPIO 설정을, GPIO_InitStructre와 같이 초기화시킴
}


void Init_GPIO_IN5()
{
    GPIO_InitTypeDef GPIO_InitStructure; //GPIO셋팅(Pin, Mode, PuPd, OType, Speed)과 관련된 구조체

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//Reset and Clock Control의 약자, ClockSetting 또는 Reset을 할 때 사용됨;    //각 GPIO 포트(A,B,C,D,E)들은 AHB(Advanced High-performance Bus)라는 버스에 연결되어 있음.    //즉 위의 함수를 통해,  AHB1의 GPIOA를 사용 가능하게 하겠다(Enable)라는 의미.
 
    //앞에서 RCC함수를 통하여 GPIOA를 초기화시켰고, 이 이후로는 그 포트의 어떤 핀을 어떤 모드로 사용할 것인   에 대한 셋팅을 함.
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; //GPIO Pin 1번을 사용
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //In모드를 사용

    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down을 설정// GPIO_PuPd_UP(Pull Up 상태로 만들기), GPIO_PuPd_DOWN(Pull Down 상태 만들기)// NOPULL상태로 셋팅
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type 설정, PP는 Push Pull, OD는 Open-Drain

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed를 설정. 100MHz 최대

    GPIO_Init(GPIOA, &GPIO_InitStructure);  // A포트의 GPIO 설정을, GPIO_InitStructre와 같이 초기화시킴
}

void Init_GPIO_IN6()
{
    GPIO_InitTypeDef GPIO_InitStructure; //GPIO셋팅(Pin, Mode, PuPd, OType, Speed)과 관련된 구조체

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//Reset and Clock Control의 약자, ClockSetting 또는 Reset을 할 때 사용됨;    //각 GPIO 포트(A,B,C,D,E)들은 AHB(Advanced High-performance Bus)라는 버스에 연결되어 있음.    //즉 위의 함수를 통해,  AHB1의 GPIOA를 사용 가능하게 하겠다(Enable)라는 의미.
 
    //앞에서 RCC함수를 통하여 GPIOA를 초기화시켰고, 이 이후로는 그 포트의 어떤 핀을 어떤 모드로 사용할 것인   에 대한 셋팅을 함.
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //GPIO Pin 1번을 사용
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //In모드를 사용

    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down을 설정// GPIO_PuPd_UP(Pull Up 상태로 만들기), GPIO_PuPd_DOWN(Pull Down 상태 만들기)// NOPULL상태로 셋팅
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type 설정, PP는 Push Pull, OD는 Open-Drain

    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed를 설정. 100MHz 최대

    GPIO_Init(GPIOA, &GPIO_InitStructure);  // A포트의 GPIO 설정을, GPIO_InitStructre와 같이 초기화시킴
}
//general button-------------------------------------------------------------------------------------