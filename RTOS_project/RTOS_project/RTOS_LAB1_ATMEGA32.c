/*
 * RTOS_LAB1_ATMEGA32.c
 *
 * Created: 1/27/2014 12:22:11 AM
 *  Author: Islam
 */ 

#include <avr/io.h>
#include "lcd_4bit.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "DIO_config.h"
#include "DIO_Init.h"
#include "KeyPad_int.h"
#include "PWM.h"

#include <util/delay.h>
#define F_CPU    8000000
#include <util/delay.h>

//#include "FreeRTOSConfig.h"

/* Define Tasks Priorities */
#define  TASK1_PRIORITY (1)
#define  TASK2_PRIORITY (1)

 /*tasks codes prototypes */
void task1_code(void*pvParamter);
void task2_code(void*pvParamter);
void keycheck_code(void*pvParamter);
void calc_code(void*pvParamter);
void LED_code(void*pvParamter);
void Normal_operation_code(void*pvParamter);
char KEYPAD_Scan(void);
//void task2_code(void*pvParamter);
xTaskHandle  welcome;
xTaskHandle  task2;
xTaskHandle  keycheck;
xTaskHandle  calc;
xTaskHandle  LED;

unsigned char op_value = 0; 

typedef enum{
	ON_C = 1,
	ZERO = 2,
	EQUAL = 3,
	ADD = 12,
	SUB = 13,
	MUL = 14,
	DIV = 15,
}SIGNS;
 
int main(void)
{
	char i = 0;
	/*initialize LCD*/
	DIO_voidInt();
	lcd_init();	
	lcd_clrScreen();	
// 	PWM0_Init();
// 	for(i;i<=100;i++){PWM0_SetDutyCycle(i);_delay_ms(10);}
// 	for(i=100;i>=0;i--){PWM0_SetDutyCycle(i);_delay_ms(10);}
	/*Create tasks*/
	xTaskCreate(task1_code,NULL,85,NULL,TASK1_PRIORITY,&welcome);
	/*start Scheduler */
	vTaskStartScheduler();

	return 0;
    
}



 void task1_code(void*pvParameters)
{
    static char x = 0;
    static char flag = 0;
	static char counter = 0;
    portTickType count = xTaskGetTickCount();
    while(1)
	{   
    vTaskDelayUntil(&count,50);
    if (x >= 0 && x<= 10 && flag == 0)
    {
	    lcd_clrScreen();
	    lcd_gotoxy(0,x);
	    lcd_dispString("welcome");
	    x++;
	    if (x>10) flag = 1;
    }
    else if (flag == 1)
    {
	    lcd_clrScreen();
	    lcd_gotoxy(0,x);
	    lcd_dispString("welcome");
	    x--;
	    if (x == 0) 
		{
			flag = 0;
			counter++;
		if (counter == 3)
		{	
		lcd_clrScreen();
		xTaskCreate(task2_code,NULL,85,NULL,TASK2_PRIORITY,&task2);
		xTaskCreate(keycheck_code,NULL,85,NULL,4,&keycheck);
		vTaskDelete(welcome);
		}
		}
    }
   } 
}

  void task2_code(void*pvParamter)
   {
		static char flag = 0,counter=0;
		portTickType count = xTaskGetTickCount(); 
	while(1)
	{
		vTaskDelayUntil(&count,250);
		if(counter == 40){
			xTaskCreate(LED_code,NULL,85,NULL,2,&LED);
			vTaskDelete(task2);
		}
		if ( flag < 2)
		{
			lcd_gotoxy(0,0);
			lcd_dispString("Press any Key to");
			lcd_gotoxy(1,1);
			lcd_dispString("Continue");
			flag++;
		}	
		if (flag == 2)
		{
			lcd_clrScreen();
			
			flag = 0;
			
		} 

		counter++; 
	}
  }
  void keycheck_code(void*pvParamter){
	  portTickType count = xTaskGetTickCount(); 
	  while (1)
	  {
		vTaskDelayUntil(&count,15);
		u8 temp = KeyPad_u8GetPressedKey();
		if(temp != NO_KEY_PRESSED )
		{
			xTaskCreate(calc_code,NULL,85,NULL,3,&calc);
			vTaskDelete(task2);
			vTaskDelete(keycheck);
		}
	  }  
  }

  
  void LED_code(void*pvParamter){
		PWM0_Init();
		portTickType count = xTaskGetTickCount(); 
		static int state = 0 ,duty = 0 ,counter = 0;
		while (1)
		{
			vTaskDelayUntil(&count,250);
			switch (state)
			{
				case 0:
					PWM0_Init();
					if(duty<=100)
					{
						PWM0_SetDutyCycle(duty);
						duty += 10;
					}
					else{
						duty = 0;
						state=1;
					}
				break;
				case 1:
					PWM0_Stop();
					if (counter == 10)
					{
						state = 2;	
					}
					else{
						counter++;
					}
				break;
				case 2:
					PWM0_Init();
					if(duty<=100)
					{
						PWM0_SetDutyCycle(100-duty);
						duty += 10;
					}
					else{
						duty = 0;
						state=3;
					}
				break;
				case 3:
					PWM0_Stop();
					if (counter == 0)
					{
						state = 0;
					}
					else{
						counter--;
					}
				break;
			}
		}
	  
  }
  void calc_code(void*pvParamter){
	  SIGNS op;
	  static char stk[16];
	  static unsigned char flag = 0,flag2=0;
	  static unsigned char counter = 0 , pos = 0;
	  static char val = NO_KEY_PRESSED,pre_key = NO_KEY_PRESSED; 
	  portTickType coun = xTaskGetTickCount();
	  while (1)
	  {
			vTaskDelayUntil(&coun,15);
			val = KeyPad_u8GetPressedKey();
			if(val == NO_KEY_PRESSED){pre_key = NO_KEY_PRESSED;continue;}
			if (val == pre_key){continue;}
			else {pre_key = val;}
			if (val >= 48 && val <= 57)
			{
					lcd_displayChar(val);
					stk[counter] = val;
					counter++;
			}
			else if(val == 61 && pos ){ // = is pressed
				flag = 1;
				counter = 0;
				lcd_clrScreen();
				lcd_displayChar(val);
			}
			else{
				pos = counter;
				lcd_clrScreen();
				switch (val)
				{
					case 42: //*
					op = MUL;
					break;
					case 43:
					op = ADD;
					break;
					case 45:
					op = SUB;
					break;
					case 47:
					op = DIV;
					break;	
					case 67:
					op = ON_C;
					break;									
				}
				//lcd_displayChar(val);
				flag2 =1;
			}
			if (flag)
			{
				lcd_clrScreen();
				switch (op){
					case ADD:
					lcd_displayChar('0'+ (stk[0]-'0')+(stk[pos]-'0'));
					break;
					case MUL:
					lcd_displayChar('0'+ ((stk[0]-'0')*(stk[pos]-'0')));
					break;
					case SUB:
					lcd_displayChar('0'+ ((stk[0]-'0')-(stk[pos]-'0')));
					break;
					case DIV:
					lcd_displayChar('0'+((stk[0]-'0')/(stk[pos]-'0')));
					break;															
	
				}
				flag=0;
				flag2=0;
				pos = 0;
				counter = 0;
			}
					
			
	  }
	  
  }
  void Normal_operation_code(void*pvParamter)
  {
	  portTickType coun = xTaskGetTickCount();
	  lcd_clrScreen();
	  static char finite = 0;
	  char temp = 0;
	  char data[20] = {0};
	  char data2[20] = {0};
	  char operations[20] = {0};
	  unsigned char num = 0;
	  unsigned char index = 0;
	  unsigned char index2 = 0;
	  unsigned char ind_op = 0;
	  unsigned char flag = 0;
	  unsigned char counter = 0;
	  while(1)
	  {
		  vTaskDelayUntil(&coun,5);
		  if (op_value>=48 && op_value<=57 && flag == 0)
		  {
			  finite = 0;
			  data[0] = op_value;
			  counter++;
		  }
		  if(counter != 1)
		  {
			  switch (finite)
			  {
				  case 0:
				  if(KeyPad_u8GetPressedKey() != NO_KEY_PRESSED)
				  {
					  temp = KeyPad_u8GetPressedKey();
					  num = temp;
					  flag = 2;
					  continue;
				  }
				  
				  if (num >= 48 && num <= 57 && flag == 2)
				  {
					  index++;
					  data[index] = num;
					  //lcd_gotoxy(0,index);
					  lcd_clrScreen();
					  lcd_displayChar(data[index]);
					  finite = 0;
					  flag = 1;
				  }
				  else if(num == 43 || num == 42 || num == 45 || num == 47 && flag == 2)
				  {
					  finite = 1;
					  operations[ind_op] = num;
					  //lcd_displayChar(num);
					  ind_op++;
					  flag = 1;
				  }
				  else {
					  finite = 0;
				  }
				  break;
				  
				  case 1:
				  if(KeyPad_u8GetPressedKey() != NO_KEY_PRESSED)
				  {
					  temp = KeyPad_u8GetPressedKey();
					  num = temp;
					  flag = 2;
					  continue;
				  }
				  if (num >= 48 && num <= 57 && flag == 2) //number
				  {
					  if (index2 == 0)
					  lcd_clrScreen();
					  data2[index2] = num;
					 // lcd_gotoxy(0,index2);
					 lcd_clrScreen();
					  lcd_displayChar(data2[index2]);
					  finite = 1;
					  index2++;
					  flag = 1;
					  lcd_displayChar(data[index]+ data2[index2]);
				  }
				  else if(num == 43 || num == 42 || num == 45 || num == 47 && flag == 2)
				  {
					  finite = 1;
					  operations[ind_op] = num;
					  //lcd_displayChar(num);
					  ind_op++;
					  flag = 1;
					  
				  }
				  break;
			  }
		  }
		  else
		  {
			  lcd_gotoxy(0,0);
			  //lcd_displayChar(data[0]+ data2[0]);
			  counter++;
			  flag = 1;
		  }
		  
	  }
	 }
	
	