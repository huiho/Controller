#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"
#define TIMER_PRESCALER_FREQ        1000000          // timer �Է� Ŭ�� 1MHz
#define TIMER_FREQ_2                 100//timer 2 ���ļ� 1000Hz      //��ȣ ����  
#define TIMER_FREQ_3    100
#define TIMER_FREQ_5     1//1Hz            //�Ѵ� Period  

#define PWM_FREQ                    1000                // PWM �ݺ� �ֱ� 1ms = 1kHz
#define PWM_WIDTH                   (TIMER_PRESCALER_FREQ / PWM_FREQ)
#define DUTY_IDLE                   (PWM_WIDTH / 2)     // 50% duty
#define DUTY_MAX                    (PWM_WIDTH - 1)     // �ִ� duty ��.






