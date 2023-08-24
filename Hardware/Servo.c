#include "stm32f10x.h"                  // Device header
#include "PWM.h"

uint16_t data;

void Servo_Init(void)
{
	PWM_Init();
}

void Servo_SetAngle(int16_t Angle)
{
	
		data = ((Angle+180)*1000/360+400)%1000+1000;
		PWM_SetCompare1(data);
	
}
