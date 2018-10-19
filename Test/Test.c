//------------------------------------------------------------------------------------------
//	*copyright		: L
//	*FileName		: Test.c
//	*Author			: Cosmos
//	*Version		: V1.3
//	*Date			: 2018.9.4
//	*Description	: Ӳ��ģ�����
//		
//	*Others			:
//	*Fun List		:
//	*History		: 2018.9.20
//					  PWM,Stick���Գɹ� 
//					  2018.9.24
//					  MPU���Գɹ�
//					  2018.9.26
//					  RGB���Գɹ�
//------------------------------------------------------------------------------------------
//Includes
//------------------------------------------------------------------------------------------
#include <stdio.h>
#include "include.h"
#include "Test/Test.h"
#include "Drv_time/Drv_time.h"
#include "Drv_RGB/Drv_RGB.h"
#include "Drv_AT9/Drv_AT9.h"
#include "Drv_Pwm/Drv_PWM.h"
#include "Drv_uart/Drv_uart.h"
#include "Drv_MPU/Drv_MPU.h"
#include "Drv_I2C/Drv_I2C.h"
#include "Drv_SPI/Drv_SPI.h"
#include "Drv_DMP/inv_mpu.h"
#include "Drv_DMP/inv_mpu_dmp_motion_driver.h"
#include "APP/Globle_Var/Globle_Var.h"
#include "Drv_SPL06/Drv_SPL06.h"
#include "Test/Ano.h"
#define THR_MAX		500
float press, temp;
//------------------------------------------------------------------------------------------
//	*Function	 : Test_time
//	*Descriptiton: ����ϵͳʱ�ӣ���Keil����˵�������ʱ��
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void Test_time(void)
{
	//������ʱus
	delay_us(100);
	//������ʱms
	delay_ms(100);
}
//------------------------------------------------------------------------------------------
//	*Function	 : Test_RGB
//	*Descriptiton: �����ź�1���ź�0�Ƿ���ȷ������RGB�Ƿ���Ե������ź�1��0��Keil�й۲�ִ��ʱ�䡣
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void Test_RGB(void)
{
	RGB_Init();
	while (1)
	{
		RGB_Rand();
		delay_ms(500);
	}
}
//------------------------------------------------------------------------------------------
//	*Function	 : Test_Pwm
//	*Descriptiton: ����PWM���Ƿ�ɰ��趨Ƶ�ʣ�ռ�ձ����
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void Test_Pwm()
{
	DTInit();
	delay_ms(5);
	/*MotorFlash(THR_MAX, THR_MAX, THR_MAX, THR_MAX);*/
	while (1)
	{
	}
}
//------------------------------------------------------------------------------------------
//	*Function	 : Test_AT9
//	*Descriptiton: ���Խ������Ƿ���������������
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void Test_AT9()
{
	AT9_Init();
	while (1)
	{
		/*delay_ms(100);
		USART_SendData(USART2, 'a');*/
	}
}
//------------------------------------------------------------------------------------------
//	*Function	 : Test_MPU
//	*Descriptiton: ����MPU��̬����ģ�飬�������ݵ���λ��
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void Test_MPU(void)
{
	float roll = 0, yaw=0, pitch = 0;
	/*USART_Config();*/
	USART_init(115200);
	delay_ms(1);
	MPU_Init();
	while(mpu_dmp_init());
	delay_ms(5);
	while(1) 
	{ 
		mpu_dmp_get_data(&pitch, &roll, &yaw);
		ANO_DT_Send_Status(roll, -pitch, -yaw, 10, 1, 1);
		delay_ms(2);
	}
}
//------------------------------------------------------------------------------------------
//	*Function	 : Test_I2C
//	*Descriptiton: ����I2C
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void Test_I2C(void)
{
	I2C_My_Init();
	while (1)
	{
		MPU_Read_Byte(0x75);
		delay_ms(10);
	}
}
//------------------------------------------------------------------------------------------
//	*Function	 : Test_SPI
//	*Descriptiton: ����SPI
//	*Parameter	 :
//	*Return		 :	
//	*Others		 :
//------------------------------------------------------------------------------------------
void Test_SPI(void)
{
	SPI_My_Init();
	while (1)
	{
		SPI_Write(0xcc, 0xaa);
		delay_ms(5);
	}
}
//------------------------------------------------------------------------------------------
//	*Function	 : Test_SPL06
//	*Descriptiton: ������ѹ��
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void Test_SPL06(void)
{
	SPI_My_Init();
	SPL06_Init();
	while (1)
	{
		SPL06_Update();
		press = SPL06_Get_Pressure();
		temp = SPL06_Get_Temperature();
		delay_ms(20);
	}
}
//------------------------------------------------------------------------------------------
//	*Function	 : Test_delay
//	*Descriptiton: ������ʱʱ��
//	*Parameter	 : None	
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void delay_ns(uint8_t ns)
{
	while (ns--);
}
void Test_delay(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	while (1)
	{
		PCout(10) = 1;
		delay_ms(5);
		PCout(10) = 0;
		delay_ms(5);
	}
}
//------------------------------------------------------------------------------------------
//	*Function	 : Test_uart
//	*Descriptiton: ����Uartͨ��
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void Test_uart(void)
{
	USART_Config();
	while (1)
	{
		delay_ms(100);
		USART_SendData(USART1, 'a');
	}
}
//------------------------------------------------------------------------------------------
