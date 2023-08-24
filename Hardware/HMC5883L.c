#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "HMC5883L_Reg.h"

#define HMC5883L_ADDRESS 0x1E
#define HMC5883L_READ_ADDRESS 0x3D
#define HMC5883L_WRITE_ADDRESS 0x3C


void HMC5883L_WriteReg(uint8_t RegAddress, uint8_t Data)
{
		MyI2C_Start();
		MyI2C_SendByte(HMC5883L_WRITE_ADDRESS);
		MyI2C_ReceiveAck();
		MyI2C_SendByte(RegAddress);
		MyI2C_ReceiveAck();
		MyI2C_SendByte(Data);
		MyI2C_ReceiveAck();
		MyI2C_Stop();
}

uint8_t HMC5883L_ReadReg(uint8_t RegAddress)
{
		uint8_t Data;
		
		MyI2C_Start();
		MyI2C_SendByte(HMC5883L_WRITE_ADDRESS);
		MyI2C_ReceiveAck();
		MyI2C_SendByte(RegAddress);
		MyI2C_ReceiveAck();
		
		MyI2C_Start();
		MyI2C_SendByte(HMC5883L_READ_ADDRESS | 0x01);
		MyI2C_ReceiveAck();
		Data = MyI2C_ReceiveByte();
		MyI2C_SendAck(1);
		MyI2C_Stop();
		
		
		return Data;
}


void HMC5883L_Init(void)
{
	MyI2C_Init();
	
	HMC5883L_WriteReg(HMC5883L_CON_REG_A,0x70);
	HMC5883L_WriteReg(HMC5883L_CON_REG_B,0x40);
	HMC5883L_WriteReg(HMC5883L_MODE_REG	,0x00);
}

void HMC5883L_GetData(int16_t*AccX,int16_t*AccY,int16_t*AccZ)
{
	uint8_t DataM,DataL;
	
	DataM = HMC5883L_ReadReg(HMC5883L_DATA_OUTPUT_X_MSB_REG);
	DataL = HMC5883L_ReadReg(HMC5883L_DATA_OUTPUT_X_LSB_REG);
	*AccX = (DataM << 8 | DataL);
	
	DataM = HMC5883L_ReadReg(HMC5883L_DATA_OUTPUT_Y_MSB_REG);
	DataL = HMC5883L_ReadReg(HMC5883L_DATA_OUTPUT_Y_LSB_REG);
	*AccY = (DataM << 8 | DataL);
	
	
	DataM = HMC5883L_ReadReg(HMC5883L_DATA_OUTPUT_Z_MSB_REG);
	DataL = HMC5883L_ReadReg(HMC5883L_DATA_OUTPUT_Z_LSB_REG);
	*AccZ = (DataM << 8 | DataL);

	
	
	
}


