///////////////////////////////////////////////////////////////////////////////
//Code Name : IWON TEMP - NON CONTACT PROJECT
//
// For STM8L0
//
// 2020/04/18 I-WON SOLUTION CO LTD
// 2020/04/18 v1.0 by KGY & KHJ
///////////////////////////////////////////////////////////////////////////////



#include "stm8l15x.h"
#include "lcd_driver.h"

#include "IWON_TASK.h"

// 아이원 온도계 테스크 클래스
IWON_TEMP_TASK *IWonTask = NULL;

#define GPIO_HIGH(a,b) 		a->ODR|=b
#define GPIO_LOW(a,b)		a->ODR&=~b
#define GPIO_TOGGLE(a,b) 	a->ODR^=b

#define SW_PWR_ON !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)
#define SW_LEFT_ON !GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)
#define SW_RIGHT_ON !GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7)


/************************************************************************/
/**
  * 1 msec 마다 호출되는 인터럽트이므로 응용해서 사용해도 좋다.
  */
INTERRUPT_HANDLER(TIM4_UPD_OVF_TRG_IRQHandler,25) {
  
	IWonTask->Time();
		
	//
	// 필요한 경우 여기에 코드를 넣어서 사용하세요.
	//        
         
	// Clear TIM4 interrupt
	TIM4_ClearITPendingBit(TIM4_IT_Update);
}
/************************************************************************/


#define RED  1
#define GREEN  2
#define BLUE  3

void displayRGB(int color)
{  
  if(color == RED)
  {
    GPIO_LOW(GPIOD, GPIO_Pin_4 );
    GPIO_HIGH(GPIOF, GPIO_Pin_0 );
    GPIO_HIGH(GPIOB, GPIO_Pin_7 );
  }
  
  
  else if(color == GREEN)
  {
    GPIO_HIGH(GPIOD, GPIO_Pin_4 );
    GPIO_LOW(GPIOF, GPIO_Pin_0 );
    GPIO_HIGH(GPIOB, GPIO_Pin_7 );
  }
  
  
  else if(color == BLUE)
  {
    GPIO_HIGH(GPIOD, GPIO_Pin_4 );
    GPIO_HIGH(GPIOF, GPIO_Pin_0 );
    GPIO_LOW(GPIOB, GPIO_Pin_7 );
  }
}

void displayLOW(void)
{
	NUMBER_CLEAR(1);
	NUMBER_CLEAR(2);
	NUMBER_CLEAR(3);
	NUMBER_CLEAR(4);

	LCD->F1 = 1; // L
	LCD->E1 = 1;
	LCD->D1 = 1;

	LCD->A2 = 1; // O
	LCD->B2 = 1;
	LCD->C2 = 1;
	LCD->D2 = 1;
	LCD->E2 = 1;
	LCD->F2 = 1;
}


void displayHIGH(void)
{ 
	NUMBER_CLEAR(1);
	NUMBER_CLEAR(2);
	NUMBER_CLEAR(3);
	NUMBER_CLEAR(4);

	LCD->F1 = 1; // H
	LCD->E1 = 1;
	LCD->G1 = 1;
	LCD->G1 = 1;
	LCD->B1 = 1;
	LCD->C1 = 1;

	LCD->F2 = 1; // I
	LCD->E2 = 1;
  
}

/************************************************************************/

void delay_10us(int us) {
  us *= 16;
  for(int i=0;i<us;i++) {
	us = us;
  }
}
void delay_ms(int ms) {
  for(int i=0;i<ms;i++) {
	delay_10us(200);
  }
}
/************************************************************************/

void Beep(int length)
{
  if(buzzerState_p)
  for(int i=0; i<length; i++) {
    GPIO_HIGH(GPIOD, GPIO_Pin_5);
    delay_10us(20);
    GPIO_LOW(GPIOD, GPIO_Pin_5);
    delay_10us(20);
  }
}
void Beep()
{
  Beep(500);
}

void GPIO_init()
{
  GPIO_Init(GPIOD, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast); //RED 
  GPIO_Init(GPIOF, GPIO_Pin_0, GPIO_Mode_Out_PP_High_Fast); //GREEN
  GPIO_Init(GPIOB, GPIO_Pin_7, GPIO_Mode_Out_PP_High_Fast); //BLUE
  
  GPIO_Init(GPIOC, GPIO_Pin_5, GPIO_Mode_In_FL_No_IT); //SW_PWR
  GPIO_Init(GPIOE, GPIO_Pin_6, GPIO_Mode_In_FL_No_IT); //SW_LEFT
  GPIO_Init(GPIOE, GPIO_Pin_7, GPIO_Mode_In_FL_No_IT); //SW_RIGHT
  
  GPIO_Init(GPIOD, GPIO_Pin_5, GPIO_Mode_Out_PP_Low_Fast); //BUZZER
  
  GPIO_Init(GPIOD, GPIO_Pin_7, GPIO_Mode_Out_PP_High_Fast); //PWR_STATE
  
  GPIO_HIGH(GPIOD, GPIO_Pin_7);
}


/*********************************************/

void tempLogDataTask()
{
   (memNumber_p)++;
  
   if(memNumber_p > 32) memNumber_p = 1;
 
   memNumberDisplay(memNumber_p); 
   memTempDataDisplay(memNumber_p, __EEPROM->memTempData[memNumber_p]);
}

void buzzerStateTask()
{
   buzzerState_p ^= 1;
        
   buzzerCMD(buzzerState_p);
}

void measureModeTask()
{
   measureMode_p ^= 1;
                 
   measureModeSet(measureMode_p);
}

void tempUnitTask()
{
   tempUnit_p ^= 1;
    
  tempUnitSet(tempUnit_p);  
}

/*********************************************/

void keyScan()
{
  if(SW_LEFT_ON) // SW_LEFT
  {
	IWonTask->ClearPowerDown();
	
	delay_ms(20);
	int delayCount = 0;
        
	while(SW_LEFT_ON)
	{
		delayCount++;
		delay_ms(15);
		if(delayCount == 40)// LONG_PRESS 
		{
			Beep();
			measureModeTask(); // measure mode Set
		}
	}

	if(delayCount < 40) // SHORT_PRESS 
	{
		Beep();
		tempLogDataTask(); // memory Data
	}
	delay_ms(10);
  }
  
  if(SW_RIGHT_ON) // SW_RIGHT
  {
	IWonTask->ClearPowerDown();

	delay_ms(20);
	int delayCount = 0;   
	
	while(SW_RIGHT_ON)
	{
	  delayCount++;
	  delay_ms(15);
	  
	  if(delayCount == 40)// LONG_PRESS 
	  {  
		Beep();
		tempUnitTask(); // temp Unit set
	  }
	}
	
	  if(delayCount < 40) // SHORT_PRESS 
	  {
		buzzerStateTask(); // buzzer On / Off
		Beep();
	  }
	delay_ms(10);
  }
}


/*********************************************/
/*****************  MAIN  ********************/
/*********************************************/

int main( void )
{
  GPIO_init();
  EEPROM_init();  
  LCD_Display_init();
    
  GPIO_LOW(GPIOD, GPIO_Pin_4);
 
  IWonTask = new IWON_TEMP_TASK(10);	// 온도를 10개 합산해서 평균낸다.

  //읽어서...
  IWonTask->Set_AdjValue(0);	// <= 이 값을 저장하고 읽어서 여기에 적용 하세요.
 
  Beep();

  BOOL Measuring = false;
  BOOL Measured = false;
  INT16 MeasredTemp = -999;
  INT8 MeasredCount1 = 0;
  INT8 MeasredCount2 = 0;
  while(IWonTask->NeedPowerDown()==false)
  {
	
	if(Measuring==false) {
		keyScan();
	}
	
	if(Measuring==false && Measured==false && MeasredTemp!=-100 && SW_PWR_ON) {
		delay_ms(20);
		if(SW_PWR_ON) {
			IWonTask->ClearPowerDown();

			Beep();
														
			MeasredTemp = -100;	// 온도측정하라는 값
			IWonTask->Clear_AVG();
		}
	}
	if(Measuring==false && Measured && SW_PWR_ON==false) {
	  Measured = false;
	}
	
	if(IWonTask->Task()) {
	  if(Measured==false && Measuring==false && MeasredTemp==-100) {	// 온도 측정 시작
		Measuring = true;
		Measured = false;
		MeasredCount1 = 0;
		MeasredCount2 = 0;
	  } 
	  else
	  if(Measuring) {	// 온도 측정
		  INT32 TEMP = IWonTask->Get_AMB_TEMP();	// -1 : LOW , -2 : HIGH
		  
		  if(TEMP < 0 || 500 < TEMP) { // 사용 환경의 온도가 0 도 보다 낮고 50 도 보다 높으면 에러를 발생한다.
				displayRGB(RED);
				//displayERR();
				
				tempValueDisplay((int16_t)TEMP);
				
				MeasredTemp = 0;
				
				Measuring = false;
				Measured = true;
				MeasredCount1 = 0;
				MeasredCount2 = 0;				
				
				delay_ms(50);
				Beep(1000);
				delay_ms(40);
				Beep(1000);
				delay_ms(40);	
				Beep(1000);
				delay_ms(40);	
				
		  } else
		  if(TEMP>0) {
			if(measureMode_p==1) {
			  	// 인체 측정
				TEMP = IWonTask->Get_BDY_TEMP();	// -1 : LOW , -2 : HIGH
				if(TEMP==-1) {
				  //tempValueDisplay((int16_t)TEMP);				  
					displayRGB(BLUE);
					displayLOW();
				} else
				if(TEMP==-2) {
				  	//tempValueDisplay((int16_t)TEMP);
					displayRGB(RED);
					displayHIGH();
				} else {
					if(MeasredCount1>0 && MeasredCount2<50 && (TEMP-MeasredTemp>2 || TEMP-MeasredTemp<-2)) {
						MeasredTemp = (int16_t)TEMP;
						MeasredCount1 = 1;						
						displayRGB(GREEN);						
					  } else {
						MeasredTemp = (int16_t)TEMP;
						MeasredCount1++;
						MeasredCount2++;
						if(MeasredCount1>20 || MeasredCount2>=50) {
						  tempValueDisplay((int16_t)TEMP);
                                                  
						   memNumber_p++;
                                                   
                           if(memNumber_p > 32) memNumber_p = 1;
						  
						  __EEPROM->memTempData[memNumber_p] = TEMP;    // save Temp
						  
						  memNumberDisplay(memNumber_p); 
						  memTempDataDisplay(memNumber_p, __EEPROM->memTempData[memNumber_p]);
						  
						  if(TEMP>380) {
							  displayRGB(RED);
							  Beep(1500);
							  delay_ms(30);
                                                          Beep(1500);
							  delay_ms(30);
                                                          Beep(1500);
							  delay_ms(30);	
						  } else
						  if(TEMP>370) {
							  displayRGB(GREEN);
							  Beep(400);
							  delay_ms(30);							  
							  Beep(400);
							  delay_ms(30);
                                                          Beep(400);
							  delay_ms(30);	
						  } else
						  if(TEMP<330) {
							  displayRGB(BLUE);
							  Beep(400);
						  } else {
						  	  displayRGB(BLUE);
							  Beep(500);
						  }						  
											  
						  Measuring = false;
						  Measured = true;
						  MeasredCount1 = 0;
						  MeasredCount2 = 0;
						}
					  }				
				}
			} else {			  
			  	// 사물 측정
				TEMP = IWonTask->Get_OBJ_TEMP();
				if(MeasredCount1>0 && (TEMP-MeasredTemp>3 || TEMP-MeasredTemp<-3)) {
					displayRGB(BLUE);
					
					MeasredTemp = (int16_t)TEMP;
					MeasredCount1 = 1;
				  } else {
					MeasredTemp = (int16_t)TEMP;
					MeasredCount1++;
					MeasredCount2++;
					if(MeasredCount1>15 || MeasredCount2>25) {
					  displayRGB(GREEN);
					  
					  tempValueDisplay((int16_t)TEMP);
                                          
					   memNumber_p++;
					
                                           if(memNumber_p > 32) memNumber_p = 1;
                                           
					   __EEPROM->memTempData[memNumber_p] = TEMP;    // save Temp
							  
					   memNumberDisplay(memNumber_p); 
				           memTempDataDisplay(memNumber_p, __EEPROM->memTempData[memNumber_p]);
                                          
                                          
					  Beep(850);
					  Measuring = false;
					  Measured = true;
					  MeasredCount1 = 0;
					  MeasredCount2 = 0;
					}
				  }
			}
		  }
	  }
	}
  }
  
  GPIO_LOW(GPIOD, GPIO_Pin_7);
 
}


/*********************************************/
/*********************************************/
