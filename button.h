#include "all_header.h"

int button_input[6]=0;
int button_value=0;
int button_i=0;

//pin_array button
void Init_GPIO_IN1()
{
    GPIO_InitTypeDef GPIO_InitStructure; //GPIO����(Pin, Mode, PuPd, OType, Speed)�� ���õ� ����ü

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//Reset and Clock Control�� ����, ClockSetting �Ǵ� Reset�� �� �� ����;    //�� GPIO ��Ʈ(A,B,C,D,E)���� AHB(Advanced High-performance Bus)��� ������ ����Ǿ� ����.    //�� ���� �Լ��� ����,  AHB1�� GPIOA�� ��� �����ϰ� �ϰڴ�(Enable)��� �ǹ�.
 
    //�տ��� RCC�Լ��� ���Ͽ� GPIOA�� �ʱ�ȭ���װ�, �� ���ķδ� �� ��Ʈ�� � ���� � ���� ����� ����   �� ���� ������ ��.
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //GPIO Pin 1���� ���
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //In��带 ���

    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down�� ����// GPIO_PuPd_UP(Pull Up ���·� �����), GPIO_PuPd_DOWN(Pull Down ���� �����)// NOPULL���·� ����
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type ����, PP�� Push Pull, OD�� Open-Drain

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed�� ����. 100MHz �ִ�

    GPIO_Init(GPIOA, &GPIO_InitStructure);  // A��Ʈ�� GPIO ������, GPIO_InitStructre�� ���� �ʱ�ȭ��Ŵ
}

//joy_stick button
void Init_GPIO_IN2()
{
    GPIO_InitTypeDef GPIO_InitStructure; //GPIO����(Pin, Mode, PuPd, OType, Speed)�� ���õ� ����ü

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//Reset and Clock Control�� ����, ClockSetting �Ǵ� Reset�� �� �� ����;    //�� GPIO ��Ʈ(A,B,C,D,E)���� AHB(Advanced High-performance Bus)��� ������ ����Ǿ� ����.    //�� ���� �Լ��� ����,  AHB1�� GPIOA�� ��� �����ϰ� �ϰڴ�(Enable)��� �ǹ�.
 
    //�տ��� RCC�Լ��� ���Ͽ� GPIOA�� �ʱ�ȭ���װ�, �� ���ķδ� �� ��Ʈ�� � ���� � ���� ����� ����   �� ���� ������ ��.
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //GPIO Pin 1���� ���
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //In��带 ���

    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down�� ����// GPIO_PuPd_UP(Pull Up ���·� �����), GPIO_PuPd_DOWN(Pull Down ���� �����)// NOPULL���·� ����
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type ����, PP�� Push Pull, OD�� Open-Drain

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed�� ����. 100MHz �ִ�

    GPIO_Init(GPIOA, &GPIO_InitStructure);  // A��Ʈ�� GPIO ������, GPIO_InitStructre�� ���� �ʱ�ȭ��Ŵ
}

//general button-------------------------------------------------------------------------------------
void Init_GPIO_IN3()
{
    GPIO_InitTypeDef GPIO_InitStructure; //GPIO����(Pin, Mode, PuPd, OType, Speed)�� ���õ� ����ü

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//Reset and Clock Control�� ����, ClockSetting �Ǵ� Reset�� �� �� ����;    //�� GPIO ��Ʈ(A,B,C,D,E)���� AHB(Advanced High-performance Bus)��� ������ ����Ǿ� ����.    //�� ���� �Լ��� ����,  AHB1�� GPIOA�� ��� �����ϰ� �ϰڴ�(Enable)��� �ǹ�.
 
    //�տ��� RCC�Լ��� ���Ͽ� GPIOA�� �ʱ�ȭ���װ�, �� ���ķδ� �� ��Ʈ�� � ���� � ���� ����� ����   �� ���� ������ ��.
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //GPIO Pin 1���� ���
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //In��带 ���

    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down�� ����// GPIO_PuPd_UP(Pull Up ���·� �����), GPIO_PuPd_DOWN(Pull Down ���� �����)// NOPULL���·� ����
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type ����, PP�� Push Pull, OD�� Open-Drain

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed�� ����. 100MHz �ִ�

    GPIO_Init(GPIOA, &GPIO_InitStructure);  // A��Ʈ�� GPIO ������, GPIO_InitStructre�� ���� �ʱ�ȭ��Ŵ
}


void Init_GPIO_IN4()
{
    GPIO_InitTypeDef GPIO_InitStructure; //GPIO����(Pin, Mode, PuPd, OType, Speed)�� ���õ� ����ü

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//Reset and Clock Control�� ����, ClockSetting �Ǵ� Reset�� �� �� ����;    //�� GPIO ��Ʈ(A,B,C,D,E)���� AHB(Advanced High-performance Bus)��� ������ ����Ǿ� ����.    //�� ���� �Լ��� ����,  AHB1�� GPIOA�� ��� �����ϰ� �ϰڴ�(Enable)��� �ǹ�.
 
    //�տ��� RCC�Լ��� ���Ͽ� GPIOA�� �ʱ�ȭ���װ�, �� ���ķδ� �� ��Ʈ�� � ���� � ���� ����� ����   �� ���� ������ ��.
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; //GPIO Pin 1���� ���
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //In��带 ���

    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down�� ����// GPIO_PuPd_UP(Pull Up ���·� �����), GPIO_PuPd_DOWN(Pull Down ���� �����)// NOPULL���·� ����
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type ����, PP�� Push Pull, OD�� Open-Drain

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed�� ����. 100MHz �ִ�

    GPIO_Init(GPIOA, &GPIO_InitStructure);  // A��Ʈ�� GPIO ������, GPIO_InitStructre�� ���� �ʱ�ȭ��Ŵ
}


void Init_GPIO_IN5()
{
    GPIO_InitTypeDef GPIO_InitStructure; //GPIO����(Pin, Mode, PuPd, OType, Speed)�� ���õ� ����ü

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//Reset and Clock Control�� ����, ClockSetting �Ǵ� Reset�� �� �� ����;    //�� GPIO ��Ʈ(A,B,C,D,E)���� AHB(Advanced High-performance Bus)��� ������ ����Ǿ� ����.    //�� ���� �Լ��� ����,  AHB1�� GPIOA�� ��� �����ϰ� �ϰڴ�(Enable)��� �ǹ�.
 
    //�տ��� RCC�Լ��� ���Ͽ� GPIOA�� �ʱ�ȭ���װ�, �� ���ķδ� �� ��Ʈ�� � ���� � ���� ����� ����   �� ���� ������ ��.
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; //GPIO Pin 1���� ���
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //In��带 ���

    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down�� ����// GPIO_PuPd_UP(Pull Up ���·� �����), GPIO_PuPd_DOWN(Pull Down ���� �����)// NOPULL���·� ����
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type ����, PP�� Push Pull, OD�� Open-Drain

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed�� ����. 100MHz �ִ�

    GPIO_Init(GPIOA, &GPIO_InitStructure);  // A��Ʈ�� GPIO ������, GPIO_InitStructre�� ���� �ʱ�ȭ��Ŵ
}

void Init_GPIO_IN6()
{
    GPIO_InitTypeDef GPIO_InitStructure; //GPIO����(Pin, Mode, PuPd, OType, Speed)�� ���õ� ����ü

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//Reset and Clock Control�� ����, ClockSetting �Ǵ� Reset�� �� �� ����;    //�� GPIO ��Ʈ(A,B,C,D,E)���� AHB(Advanced High-performance Bus)��� ������ ����Ǿ� ����.    //�� ���� �Լ��� ����,  AHB1�� GPIOA�� ��� �����ϰ� �ϰڴ�(Enable)��� �ǹ�.
 
    //�տ��� RCC�Լ��� ���Ͽ� GPIOA�� �ʱ�ȭ���װ�, �� ���ķδ� �� ��Ʈ�� � ���� � ���� ����� ����   �� ���� ������ ��.
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //GPIO Pin 1���� ���
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //In��带 ���

    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull-up, Pull-Down�� ����// GPIO_PuPd_UP(Pull Up ���·� �����), GPIO_PuPd_DOWN(Pull Down ���� �����)// NOPULL���·� ����
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Output Type ����, PP�� Push Pull, OD�� Open-Drain

    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // Speed�� ����. 100MHz �ִ�

    GPIO_Init(GPIOA, &GPIO_InitStructure);  // A��Ʈ�� GPIO ������, GPIO_InitStructre�� ���� �ʱ�ȭ��Ŵ
}
//general button-------------------------------------------------------------------------------------