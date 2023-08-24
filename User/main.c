#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "Key.h"
#include "MPU6050.h"
#include "HMC5883L.h"
#include "Servo.h"
#include "PWM.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

uint8_t RxData;
double Angle0 = 0;
double AngleChange;

int main(void)
{
	OLED_Init();

	OLED_ShowString(1,1,"MPU:");
	OLED_ShowString(1,8,"HMC:");
	Servo_Init();
	Key_Init();
	
	Serial_Init();
	MPU6050_Init();
	HMC5883L_Init();
	
	
	while (1)
	{
		int16_t mpuX,mpuY,mpuZ;
		MPU6050_GetData(&mpuX,&mpuY,&mpuZ);
		OLED_ShowSignedNum(2,1,mpuX,5);
		OLED_ShowSignedNum(3,1,mpuY,5);
		OLED_ShowSignedNum(4,1,mpuZ,5);
		
		int16_t hmcX,hmcY,hmcZ;
		HMC5883L_GetData(&hmcX,&hmcY,&hmcZ);
		hmcX=(double)((hmcX+150)*1.2+90-600);
		hmcZ=(double)((hmcZ-265)*1.32-10-700+615);
		hmcY=0;
//		printf("%d ",hmcX);
//		printf("%d ",hmcY);
//		printf("%d\r\n",hmcZ);
		OLED_ShowSignedNum(2,8,hmcX,5);
		OLED_ShowSignedNum(3,8,hmcY,5);
		OLED_ShowSignedNum(4,8,hmcZ,5);
		
		
		double ArcTan;
		ArcTan=(double)hmcZ/(double)hmcX;

		int Angle;
		Angle=atan(ArcTan)*57.3+90;
		int16_t data;
		if(hmcX>0)
		{
			data = ((Angle-30)*1000/360+400)%1000+1000;
			PWM_SetCompare1(data);
			Delay_ms(30);
		}
		else if(-300<hmcX<1&&-320<hmcZ<-200)
		{
		Servo_SetAngle((Angle+120)%360);
		Delay_ms(30);
		}else if(hmcX<0)
		{
		Servo_SetAngle(Angle-60);
		Delay_ms(30);
		}
		printf("%d %d %d\r\n",hmcX,hmcY,hmcZ);
//		printf("%d,%d\r\n",Angle,data);		
		Key_GetNum();
	}
}
