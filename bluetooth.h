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
        GPIO_InitTypeDef GPIO_InitStructure; //GPIO 초기화용 Structure를 선언
        USART_InitTypeDef USART_InitStructure; // USART 초기화용  Structure를 선언
        NVIC_InitTypeDef NVIC_InitStructure; // NVIC 초기화용 Structure 선언
        // Enable peripheral
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //GPIOA를 Enable시킴 GPIOA는 AHB1에 위치
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // USART1을 Enalble 시킴 USART1은 APB2에 위치
        // Configure USART Interrupt
        NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; // UART Interrupt을 등록함.핸들러는 TImer처럼 고정되어있음
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f; // Preemption 인터럽트 우선순위 설정
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0f; // Sub 인터럽트 우선순위 설정
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // IRQ채널의 cmd를 Enable함.
        NVIC_Init(&NVIC_InitStructure); // NVIC 초기화
        // GPIO AF config
        GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
        GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5; // USART(또는 UART)의 TX 핀번호
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOD, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6; // USART(또는 UART)의 RX 핀번호
        GPIO_Init(GPIOD, &GPIO_InitStructure);
        // Configure UART peripheral
        USART_InitStructure.USART_BaudRate   = 115200; //Baudrate설정.
        USART_InitStructure.USART_WordLength = USART_WordLength_8b; // 전송하는 패킷의 길이는 8bit씩 전송함을 명시
        USART_InitStructure.USART_StopBits   = USART_StopBits_1; // stopbit는 1개임을 의미
        USART_InitStructure.USART_Parity     = USART_Parity_No ; // 패리티 비트는 없다. 패리티 비트는 단지 에러가 있음을 알려줄뿐, 에러를 직접 고치지 못하기 때문에, 이는 UART데이터 전송속도에 안좋은 영향을 끼칠 것.(매우 긴 패킷에서만..)
        USART_InitStructure.USART_HardwareFlowControl= USART_HardwareFlowControl_None; // HardwareFlowControl을 없다고 설정
        USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx; //USART_Mode에서 RX, TX를 설정
        USART_Init(USART2, &USART_InitStructure); //  USART초기화
        // Enable USART receive interrupt
        USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
        USART_Cmd(USART2, ENABLE);
}