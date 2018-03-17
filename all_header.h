#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"
#define TIMER_PRESCALER_FREQ        1000000          // timer 입력 클력 1MHz
#define TIMER_FREQ_2                 100//timer 2 주파수 1000Hz      //신호 제어  
#define TIMER_FREQ_3    100
#define TIMER_FREQ_5     1//1Hz            //둘다 Period  

#define PWM_FREQ                    1000                // PWM 반복 주기 1ms = 1kHz
#define PWM_WIDTH                   (TIMER_PRESCALER_FREQ / PWM_FREQ)
#define DUTY_IDLE                   (PWM_WIDTH / 2)     // 50% duty
#define DUTY_MAX                    (PWM_WIDTH - 1)     // 최대 duty 값.






