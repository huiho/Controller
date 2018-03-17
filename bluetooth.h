#include "all_header.h"
unsigned char parsing[8];
unsigned char start = -1;
unsigned char stop = -1;
unsigned char com = -1;
unsigned char pin1 = -1;
unsigned char pin2 = -1;
unsigned char time = -1;
unsigned char fre = -1;
unsigned char du = -1;
int Parsing_i=0;
int p = 0;
int i = 0;

void Init_USART2_BT(void){
        GPIO_InitTypeDef GPIO_InitStructure; //GPIO �ʱ�ȭ�� Structure�� ����
        USART_InitTypeDef USART_InitStructure; // USART �ʱ�ȭ��  Structure�� ����
        NVIC_InitTypeDef NVIC_InitStructure; // NVIC �ʱ�ȭ�� Structure ����
        // Enable peripheral
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //GPIOA�� Enable��Ŵ GPIOA�� AHB1�� ��ġ
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // USART1�� Enalble ��Ŵ USART1�� APB2�� ��ġ
        // Configure USART Interrupt
        NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; // UART Interrupt�� �����.�ڵ鷯�� TImeró�� �����Ǿ�����
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f; // Preemption ���ͷ�Ʈ �켱���� ����
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0f; // Sub ���ͷ�Ʈ �켱���� ����
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // IRQä���� cmd�� Enable��.
        NVIC_Init(&NVIC_InitStructure); // NVIC �ʱ�ȭ
        // GPIO AF config
        GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
        GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5; // USART(�Ǵ� UART)�� TX �ɹ�ȣ
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOD, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6; // USART(�Ǵ� UART)�� RX �ɹ�ȣ
        GPIO_Init(GPIOD, &GPIO_InitStructure);
        // Configure UART peripheral
        USART_InitStructure.USART_BaudRate   = 115200; //Baudrate����.
        USART_InitStructure.USART_WordLength = USART_WordLength_8b; // �����ϴ� ��Ŷ�� ���̴� 8bit�� �������� ���
        USART_InitStructure.USART_StopBits   = USART_StopBits_1; // stopbit�� 1������ �ǹ�
        USART_InitStructure.USART_Parity     = USART_Parity_No ; // �и�Ƽ ��Ʈ�� ����. �и�Ƽ ��Ʈ�� ���� ������ ������ �˷��ٻ�, ������ ���� ��ġ�� ���ϱ� ������, �̴� UART������ ���ۼӵ��� ������ ������ ��ĥ ��.(�ſ� �� ��Ŷ������..)
        USART_InitStructure.USART_HardwareFlowControl= USART_HardwareFlowControl_None; // HardwareFlowControl�� ���ٰ� ����
        USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx; //USART_Mode���� RX, TX�� ����
        USART_Init(USART2, &USART_InitStructure); //  USART�ʱ�ȭ
        // Enable USART receive interrupt
        USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
        USART_Cmd(USART2, ENABLE);
}