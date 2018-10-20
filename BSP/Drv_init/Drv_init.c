//------------------------------------------------------------------------------------------
//	*copyright		: L
//	*FileName		: Drv_init
//	*Author			: Cosmos
//	*Version		: v1.0
//	*Date			: 2018.9.3
//	*Description	: ��Ӳ����Դ��ʼ��
//		
//	*Others			:
//	*Fun List		:
//	*History		:
//------------------------------------------------------------------------------------------
//Includes
//------------------------------------------------------------------------------------------
#include "Drv_init/Drv_init.h"
#include "Drv_time/Drv_time.h"
#include "Drv_RGB/Drv_RGB.h"
#include "Drv_AT9/Drv_AT9.h"
#include "Drv_MPU/Drv_MPU.h"
#include "APP/Control/Control.h"
#include "Drv_PWM/Drv_PWM.h"
#include "Drv_SPL06/Drv_SPL06.h"
#include "Drv_SPI/Drv_SPI.h"
#include "Drv_I2C/Drv_I2C.h"
#include "Drv_uart/Drv_uart.h"
#include "Drv_DMP/inv_mpu.h"
#include "Drv_DMP/inv_mpu_dmp_motion_driver.h"
#include "Drv_DMP/dmpmap.h"
//------------------------------------------------------------------------------------------
//	*Function	 :	all_init
//	*Descriptiton:	Ӳ����Դ��ʼ��
//	*Parameter	 :	None
//	*Return		 :	��ʼ���ɹ�����1
//	*Others		 :
//------------------------------------------------------------------------------------------
uint8_t all_init(void)
{
	//�δ�ʱ��
	SysTick_Configuration();
	////RGB��
	RGB_Init();
	//I2C��ΪMPU�ṩͨ��
	I2C_My_Init();
	//����MPU
	if (MPU_Init())
		return 0;
	if (mpu_dmp_init())
		return 0;
	//ң�����źŽ�����
	AT9_Init();
	//Pwm��ʼ��
	Pwm_Init();
	//������λ��
	USART_init(500000);
	//SPI
	//SPI_My_Init();
	//��ѹ��
	//SPL06_Init();
	//PID��ʼ��
	/*PID_Init();*/
	return 1;
}
//------------------------------------------------------------------------------------------
