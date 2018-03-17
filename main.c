#include "bluetooth.h"
#include "joystick.h"
#include "lra.h"
#include "pin_array.h"
#include "button.h"

//핀결정
int pin_s[8];
int pin_s2[8];

void Pin_Array_All(int pin1, int pin2, int time_pin, int fre);
void Joy_stick_period_cal(uint32_t time_lra, uint32_t fre, uint32_t du);
void vib_one(int pin1, int pin2, int time_lra, uint32_t du);
void pin_sel(int num, int arr[]);
uint32_t num(uint32_t fre);
uint32_t Duty(uint32_t du);

int main(){
  

  //LRA  
  setup_pwm_timer3();
  setup_pwm3();
  setup_lras3();
  setup_pwm_timer4();
  setup_pwm4();
  setup_lras4();
  setup_pwm_timer12();
  setup_pwm12();
  setup_lras12();
  
  //파싱 데이터 전송 (device -> pc)
  Init_Timer7();          //pin-array 제어

  //조이스틱 입력값 받기 x,y
  Init_ADC2();   
  Init_ADC3();
  Init_TimerforADC();
  
  //조이스틱 MR Actuator
  Init_PWM_Joystick_xy();
  //블루투스 USART
  Init_USART2_BT();
  
  //핀어레이
  Init_GPIO_OUT1();
  Init_GPIO_OUT2();
  Init_GPIO_OUT3();
  Init_GPIO_OUT4();
  Init_GPIO_OUT5();
  Init_GPIO_OUT6();
  Init_GPIO_OUT7();
  Init_GPIO_OUT8();
  Init_GPIO_OUT9();
  Init_GPIO_OUT10();
  Init_GPIO_OUT11();
  Init_GPIO_OUT12();
  Init_GPIO_OUT13();
  Init_GPIO_OUT14();
  Init_GPIO_OUT15();
  Init_GPIO_OUT16();
  Init_GPIO_OUT17();
  Init_GPIO_OUT18();
  
  
  //버튼
  Init_GPIO_IN1();
  Init_GPIO_IN2();
  Init_GPIO_IN3();
  Init_GPIO_IN4();
  Init_GPIO_IN5();
  Init_GPIO_IN6();
  
  
  //테스트용
  Init_GPIO_OUT19();
  
  while(1){
    
  }
}

void left(int du)
{
  if(du > 100)
     du = 100;
  if(du < 0)
    du = 0;
   
 // PWMChannel_Control1(Duty(du));
  PWMChannel_Control4(Duty(du));
 // PWMChannel_Control7(Duty(du));
  
}
void right(int du)
{
  if(du > 100)
     du = 100;
  if(du < 0)
    du = 0;
  PWMChannel_Control3(Duty(du));
  PWMChannel_Control6(Duty(du));
  PWMChannel_Control9(Duty(du));
}
void center(int du)
{
  if(du > 100)
     du = 100;
  if(du < 0)
    du = 0;
   PWMChannel_Control2(Duty(du));
  PWMChannel_Control5(Duty(du));
  PWMChannel_Control8(Duty(du));
}

void left_LRA(int sp)
{
  if(sp>=5)
    sp= 5;
  else if(sp<=0)
    sp = 0;
  
  if(left_on==1)
    {
      if(left_move>=(de/3))
      {
        left(left_move);
        if(move_time == 2)
          left_move-=sp*2+de/10;
      }    
      else if(right_move >= (de-(de/15)))
      {
        right(0);
        right_move=0;
        left_move=0;
        left_on=0;
      }
      else if((de-(de/15))>=right_move)
      {
        left(left_move);
        right(right_move);
        center(right_move+de/8);
        if(move_time == 2)
        {
          right_move+=sp*3;
          left_move-=sp*3;
          
        }
      }
      else if((de/3)<=right_move)
      {
        right(right_move);
        if(move_time == 2)
        {
          right_move+=sp*3;
        }
        left(0);
      }    
      
    }
}


void pin_sel(int num, int arr[])
{
  int i;
  
  for(i=0; i<8; i++)
  {
    arr[i] = ((num >> i) & 1);
  }
}
uint32_t num(uint32_t fre)
{
  float number = ( 0.5/fre );
  float cal =number*9090;
  uint32_t per = (uint32_t) cal;
  
  return per;
}

//듀티 제어 함수
uint32_t Duty(unsigned int du)
{
  uint32_t Duty= du*10;
  
  return Duty;
}


            
//핀어레이 제어 함수(H/L, 주기, 시간) ( H/L는 제어 아직 안함 )
void Pin_Array_All(int pin1, int pin2, int time_pin, int fre){
  
  int i;

  pin_sel(pin1, pin_s);
  pin_sel(pin2, pin_s2);
  
  //time_end값 정하기, 시간 측정 시작 변수 on
    if(pin_s[0]==1){
      time_start_pin_array[8]=1;
      time_cal_pin_array[8]=0;
      time_end_pin_array[8]=time_pin*40;
      Pin_f[8] = fre;
    }

   for(i=0; i<8; i++){
    if(pin_s2[i]==1){
      time_start_pin_array[i]=1;
      time_cal_pin_array[i]=0;
      time_end_pin_array[i]=time_pin*40;
      Pin_f[i] = fre;
    }
  }
  
  //핀어레이 작동
  for(i=0; i<9; i++)
  {
    pin_i[i]=0;
    if(time_start_pin_array[i]==1){
    pin_array_off[i]=0;
    }
  }
}

//조이스틱 제어 함수(시간, 주기, 듀티)
void Joy_stick_period_cal(uint32_t time_joy_stick, uint32_t fre, uint32_t du){
  //주파수 계산
  uint32_t fr = num(fre);
  //듀티 게산
  uint32_t dut = Duty(du);
  
  //time을 on/off기능 처럼  0보다 크면 작동
  if(time_joy_stick>0){
   Init_PWM4_Joystick(fr,dut);
  }
  //time이 0보다 작거나 같으면 작동하지 않음
  else if(time_joy_stick <= 0 ){
     Init_PWM4_Joystick(0,0);
  }


}

void vib_one(int pin1, int pin2, int time_lra, uint32_t du)
{
  int i;
  du = Duty(du);
  
  pin_sel(pin1, pin_s);
  pin_sel(pin2, pin_s2);

  //시간 측정 함수
  if(pin_s[0]==1){
      time_start_LRA[8]=1;
      time_cal_LRA[8]=0;
      LRA_off[8]=0;
      time_end_LRA[8]=time_lra*40;
    }

   for(i=0; i<8; i++){
    if(pin_s2[i]==1){
      time_start_LRA[i]=1;
      time_cal_LRA[i]=0;
      LRA_off[i]=0;
      time_end_LRA[i]=time_lra*40;
    }
  }
 
  if(time_start_LRA[0])
  {
    PWMChannel_Control1(du);
  }
  if(time_start_LRA[1])
  {
    PWMChannel_Control2(du);
  }
  if(time_start_LRA[2])
  {
    PWMChannel_Control3(du);
  }
  if(time_start_LRA[3])
  {
    PWMChannel_Control4(du);
  }
  if(time_start_LRA[4])
  {
    PWMChannel_Control5(du);
  }
  if(time_start_LRA[5])
  {
    PWMChannel_Control6(du);
  }
  if(time_start_LRA[6])
  {
    PWMChannel_Control7(du);
  }
  if(time_start_LRA[7])
  {
    PWMChannel_Control8(du);
  }
  if(time_start_LRA[8])
  {
    PWMChannel_Control9(du);
  }
}


void vib_one2(int pin1, int pin2, int time_lra, uint32_t du)
{
  int i;
  du = Duty(du);
  
  pin_sel(pin1, pin_s);
  pin_sel(pin2, pin_s2);

  //시간 측정 함수
 
 
    PWMChannel_Control1(du);
    PWMChannel_Control2(du);
    PWMChannel_Control3(du);
    PWMChannel_Control4(du);
    PWMChannel_Control5(du);
    PWMChannel_Control6(du);
    PWMChannel_Control7(du);
    PWMChannel_Control8(du);
    PWMChannel_Control9(du);
  
}



void USART2_IRQHandler(void)
{
  
  if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET) // RX에 무슨 값이 들어왔는가?를 확인하는 방법.
  {
    //   csw = USART_ReceiveData(USART2);//마이크로 컨트롤러 내부의 RX에 일정 값이 들어모녀 한글자, 한글자씩 저장함.
    //     USART_SendData(USART1, csw);
    
    if(p < 1)
    {
      parsing[p] = USART_ReceiveData(USART2);
      start = parsing[p];      
    }
    else if(p < 2)
    {
      parsing[p] = USART_ReceiveData(USART2);
      com =  parsing[p];
    }
    else if(p < 3)
    {
      parsing[p] = USART_ReceiveData(USART2);
      pin1 = parsing[p];
      
    }
    else if(p < 4)
    {
      parsing[p] = USART_ReceiveData(USART2);
      pin2 = parsing[p];
      
    }
    else if(p < 5)
    {
      parsing[p] = USART_ReceiveData(USART2);
      time = parsing[p];
    }
    else if(p < 6)
    {
      parsing[p] = USART_ReceiveData(USART2);
      fre = parsing[p];
    }
    else if(p < 7)
    {
      parsing[p] = USART_ReceiveData(USART2);
      du = parsing[p];
    }
    else if(p < 8)
    {
      parsing[p] = USART_ReceiveData(USART2);
      stop = parsing[p];
    }
    
    if(start == 250)
    {   
      p++;
    }
    
    if(stop == 251)
    {
      
      if(com==1){

        vib_one(pin1, pin2, time, du);    
      }
      else if(com==2){
        Joy_stick_period_cal(time, fre, du);
      }
      else if(com==3){
        //pin_array 주기, 시간 제어 성공 ( H/L는 제어 아직 안함 )
        Pin_Array_All(pin1, pin2, time, fre);
        j_flag=time;       
      }
      else if(com==4)
      {
        left_on = 1;
        move_on = 1;
        de = du;
        left_move = de;
        right_move = 0;
        center_move=0;
      }
      else if(com==5){
         GPIOA->BSRRL = GPIO_Pin_3;
      }
      else if(com==6){
         GPIOA->BSRRH = GPIO_Pin_3;
      }
      
      
      start = -1;
      stop = -1;
      p=0;
    }
    if(p>7)
    {
      p=0;
      start = -1;
    }
  }                          
}
//Pin_Array핸들러
void TIM7_IRQHandler(void)//타이머 인터럽트 핸들러
{
  if (TIM_GetITStatus(TIM7, TIM_IT_Update) == SET)    
  {
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);    
    {
      
      for(int i = 0; i < 9; i++)
      {
        if(pin_array_off[i]==0){
          Pin_Hz[i] ++;

        }
      }
      
      
      if(pin_array_off[0]==1){
        GPIOE->BSRRH = GPIO_Pin_0;     
        GPIOE->BSRRH = GPIO_Pin_1;
      }
      if(pin_array_off[1]==1){
        GPIOE->BSRRH = GPIO_Pin_2;
        GPIOE->BSRRH = GPIO_Pin_3;
      }
      if(pin_array_off[2]==1){
        GPIOE->BSRRH = GPIO_Pin_4;
        GPIOE->BSRRH = GPIO_Pin_5;
      }
      if(pin_array_off[3]==1){
        GPIOE->BSRRH = GPIO_Pin_6;
        GPIOE->BSRRH = GPIO_Pin_7;
      }
      if(pin_array_off[4]==1){
        GPIOE->BSRRH = GPIO_Pin_8;
        GPIOE->BSRRH = GPIO_Pin_9;
      }
      if(pin_array_off[5]==1){
        GPIOE->BSRRH = GPIO_Pin_10;
        GPIOE->BSRRH = GPIO_Pin_11;
      }
      if(pin_array_off[6]==1){
        GPIOE->BSRRH = GPIO_Pin_12;
        GPIOE->BSRRH = GPIO_Pin_13;
      }
      if(pin_array_off[7]==1){
        GPIOE->BSRRH = GPIO_Pin_14;
        GPIOE->BSRRH = GPIO_Pin_15;
      }    
      if(pin_array_off[8]==1){
        GPIOB->BSRRH = GPIO_Pin_8;
        GPIOB->BSRRH = GPIO_Pin_9;
      }
   
    
   if(  ((Pin_Hz[0]%(1000/Pin_f[0]))) == 0 ){
      if(pin_array_off[0]==0)
      {        
        if(pin_i[0]%2==0 )
          GPIOE->BSRRL = GPIO_Pin_0;
        else
          GPIOE->BSRRH = GPIO_Pin_0;
        
        if(pin_i[0]%2==0  )
          GPIOE->BSRRH = GPIO_Pin_1;
        else
          GPIOE->BSRRL = GPIO_Pin_1;
        pin_i[0] = 1-pin_i[0];
            
      }
    }
    if(  (Pin_Hz[1]%(1000/Pin_f[1])) == 0  ){
      if(pin_array_off[1]==0)
      {
        if(pin_i[1]%2==0)
          GPIOE->BSRRL = GPIO_Pin_2;
        else
          GPIOE->BSRRH = GPIO_Pin_2;
        
        if(pin_i[1]%2==0)
          GPIOE->BSRRH = GPIO_Pin_3;
        else
          GPIOE->BSRRL = GPIO_Pin_3;
        
        pin_i[1] = 1-pin_i[1];
            
      }
 }
   if( (Pin_Hz[2]%(1000/Pin_f[2])) == 0  ){
      if(pin_array_off[2]==0)
      {
        if(pin_i[2]%2==0)
          GPIOE->BSRRL = GPIO_Pin_4;
        else
          GPIOE->BSRRH = GPIO_Pin_4;
        
        if(pin_i[2]%2==0)
          GPIOE->BSRRH = GPIO_Pin_5;
        else
          GPIOE->BSRRL = GPIO_Pin_5;      
             
       pin_i[2] = 1-pin_i[2];
      }
   }
    if( (Pin_Hz[3]%(1000/Pin_f[3])) == 0  ){
      if(pin_array_off[3]==0)
      {
        if(pin_i[3]%2==0)
          GPIOE->BSRRL = GPIO_Pin_6;
        else
          GPIOE->BSRRH = GPIO_Pin_6;  
        
        if(pin_i[3]%2==0)
          GPIOE->BSRRH = GPIO_Pin_7;
        else
          GPIOE->BSRRL = GPIO_Pin_7;
        pin_i[3] = 1-pin_i[3];
      }
    }
    if( (Pin_Hz[4]%(1000/Pin_f[4]))  == 0 ){
      if(pin_array_off[4]==0)
      {                        
        if(pin_i[4]%2==0)
          GPIOE->BSRRL = GPIO_Pin_8;
        else
          GPIOE->BSRRH = GPIO_Pin_8;
        
        if(pin_i[4]%2==0)
          GPIOE->BSRRH = GPIO_Pin_9;
        else
          GPIOE->BSRRL = GPIO_Pin_9;
        pin_i[4] = 1-pin_i[4];
      }
   }
    if( (Pin_Hz[5]%(1000/Pin_f[5]))  == 0 ){
      if(pin_array_off[5]==0)
      {       
        if(pin_i[5]%2==0)
          GPIOE->BSRRL = GPIO_Pin_10;
        else
          GPIOE->BSRRH = GPIO_Pin_10;
        
        if(pin_i[5]%2==0)
          GPIOE->BSRRH = GPIO_Pin_11;
        else
          GPIOE->BSRRL = GPIO_Pin_11;
        pin_i[5] = 1-pin_i[5];
      }
    }
    if( (Pin_Hz[6]%(1000/Pin_f[6]))  == 0  ){
      if(pin_array_off[6]==0)
      {        
        if(pin_i[6]%2==0)
          GPIOE->BSRRL = GPIO_Pin_12;
        else
          GPIOE->BSRRH = GPIO_Pin_12;
        
        if(pin_i[6]%2==0)
          GPIOE->BSRRH = GPIO_Pin_13;
        else
          GPIOE->BSRRL = GPIO_Pin_13;
         pin_i[6] = 1-pin_i[6];
      }     
   }
   if( (Pin_Hz[7]%(1000/Pin_f[7])) == 0  ){
      if(pin_array_off[7]==0)
      {
        if(pin_i[7]%2==0)
          GPIOE->BSRRL = GPIO_Pin_14;
        else
          GPIOE->BSRRH = GPIO_Pin_14;
        
        if(pin_i[7]%2==0)
          GPIOE->BSRRH = GPIO_Pin_15;
        else
          GPIOE->BSRRL = GPIO_Pin_15;
        pin_i[7] = 1-pin_i[7];        
      }
    }
    if( (Pin_Hz[8]%(1000/Pin_f[8])) == 0  ){
      if(pin_array_off[8]==0)
      {        
        if(pin_i[8]%2==0)
          GPIOB->BSRRL = GPIO_Pin_8;
        else
          GPIOB->BSRRH = GPIO_Pin_8;
        
        if(pin_i[8]%2==0)
          GPIOB->BSRRH = GPIO_Pin_9;
        else
          GPIOB->BSRRL = GPIO_Pin_9;
        pin_i[8] = 1-pin_i[8]; 
      }
    }
    i = 1-i;
    }
  }
}

//조이스틱 핸들러(ADC)
void TIM2_IRQHandler(void){
  //TIM2 ADC인터럽터
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    
    ADC_SoftwareStartConv(ADC3);
    ADC_SoftwareStartConv(ADC2);  
    
    if (ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC) == SET)    
    {
      x = ADC_GetConversionValue(ADC3);
    }
    if (ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC) == SET)    
    {
      y = ADC_GetConversionValue(ADC2);
    }
    
    mx=(int)(x/16.33);
    my=(int)(y/16.33);

    
    //버튼이 눌러졌는지 확인 후 버튼배열에 값 넣기
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10)){
      button_input[0]=1;
    }
    else{
      button_input[0]=0;
    }
      
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9)){
      button_input[1]=2;
    }
    else{
      button_input[1]=0;
    }
      
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)){
      button_input[2]=4;
    }
    else{
      button_input[2]=0;
    }
      
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_13)){
      button_input[3]=8;
    }
    else{
      button_input[3]=0;
    } 
      
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_14)){
      button_input[4]=16;
    }
    else{
      button_input[4]=0;
    }  
      
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)){
      button_input[5]=32;
    }
    else{
      button_input[5]=0;
    }
      
    //파싱에 보내기 위해 배열에 값들을 button_value에  연산하여 대입
    for(button_i=0; button_i<6; button_i++){
          button_value += button_input[button_i];      
    }
    
    buffer[0]=0XFA;
    buffer[1]=button_input[1];
    buffer[2]=0;
    buffer[3]=0;
    buffer[4]=mx;
    buffer[5]=my;
    buffer[6]=0XFB;
    
    USART_SendData(USART2,buffer[Parsing_i]); 
    
    Parsing_i++;
    if(Parsing_i==7){
      Parsing_i=0;
      button_value=0;
    }
    
    //핀어레이 시간 측정 시작
    for(pin_array_i=0; pin_array_i<9; pin_array_i++){
         if(time_start_pin_array[pin_array_i]==1){
            time_cal_pin_array[pin_array_i]++;
         }
    }  
    
    //핀어레이  시간 완료 확인
     for(pin_array_i=0; pin_array_i<9; pin_array_i++){
          if(time_cal_pin_array[pin_array_i]>=time_end_pin_array[pin_array_i]){
            time_end_pin_array[pin_array_i]=0;
            time_cal_pin_array[pin_array_i]=0;
            time_start_pin_array[pin_array_i]=0;
            pin_array_off[pin_array_i]=1;
            Pin_f[8] = 0;
            Pin_Hz[8] = 0;
          }
    }
 
    //LRA 시간 측정 시작
    for(LRA_i=0; LRA_i<9; LRA_i++){
         if(time_start_LRA[LRA_i]==1){
          time_cal_LRA[LRA_i]++;
          
         }
    } 
    
    //LRA 시간 측정 확인
    for(LRA_i=0; LRA_i<9; LRA_i++){
      if(time_cal_LRA[LRA_i]>=time_end_LRA[LRA_i]){
        time_end_LRA[LRA_i]=-1;
        time_cal_LRA[LRA_i]=0;
        time_start_LRA[LRA_i]=0;
        LRA_off[LRA_i]=1;
      }
    }
      
    
    
    if(LRA_off[0]==1){
      PWMChannel_Control1(0);
    }
    if(LRA_off[1]==1){
      PWMChannel_Control2(0);
    }   
    if(LRA_off[2]==1){
      PWMChannel_Control3(0);
    }  
    if(LRA_off[3]==1){
      PWMChannel_Control4(0);
    }   
    if(LRA_off[4]==1){
      PWMChannel_Control5(0);
    }    
    if(LRA_off[5]==1){
      PWMChannel_Control6(0);
    }
    if(LRA_off[6]==1){
      PWMChannel_Control7(0);
    }
    if(LRA_off[7]==1){
      PWMChannel_Control8(0);
    }
    if(LRA_off[8]==1){
      PWMChannel_Control9(0);
    }

    if(move_on == 1)
    {
      move_time++;
      left_LRA(fre);
      if(move_time == 2)
      {
        move_time = 0;
      }
    }
  
       
  }
}

//LRA핸들러 TIM3
void TIM3_IRQHandler(void){
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);          
    int duty=1;//Duty 비율 설정
    TIM_SetCompare4(TIM3, duty);//PWM 발생(입력한 Duty에 좌우됨)
    //B포트 1번핀에서 PWM이 발생함.
  }
}
//LRA핸들러 TIM4
void TIM4_IRQHandler(void){
  if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
  {
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);          
    int duty=1;//Duty 비율 설정
    TIM_SetCompare4(TIM4, duty);//PWM 발생(입력한 Duty에 좌우됨)
    //B포트 2번핀에서 PWM이 발생함.
  }
}
//LRA핸들러 TIM12
void TIM12_IRQHandler(void){
  if (TIM_GetITStatus(TIM12, TIM_IT_Update) == SET)
  {
    TIM_ClearITPendingBit(TIM12, TIM_IT_Update);          
    int duty=1;//Duty 비율 설정
    TIM_SetCompare4(TIM12, duty);//PWM 발생(입력한 Duty에 좌우됨)
    //B포트 2번핀에서 PWM이 발생함.
  }
}