//------------------------------------------------------------------------------------------
//	*copyright		: L
//	*FileName		: Drv_MPU
//	*Author			: Cosmos
//	*Version		: V1.0
//	*Date			: 2018.9.6	 
//	*Description	: �����ʼ��
//		
//	*Others			:
//	*Fun List		:
//	*History		: 2018.9.24 ���Գɹ�
//------------------------------------------------------------------------------------------
//Includes
//------------------------------------------------------------------------------------------
#include "Drv_MPU/Drv_MPU.h"
#include "Drv_I2C/Drv_I2C.h"
#include "Drv_DMP/inv_mpu.h"
#include "Drv_DMP/inv_mpu_dmp_motion_driver.h"
#include "Drv_time/Drv_time.h"
#include <math.h>
//------------------------------------------------------------------------------------------
//	*Function	 : MPU_Init
//	*Descriptiton: ��ʼ��MPU6050
//	*Parameter	 : None 
//	*Return		 : 0 : �ɹ�    ����,�������
//	*Others		 :
//------------------------------------------------------------------------------------------
uint8_t MPU_Init(void)
{
	uint8_t res;
	I2C_My_Init();															//��ʼ��IIC����
	MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X80);	//��λMPU6050
	delay_ms(100);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X00);	//����MPU6050 
	MPU_Set_Gyro_Fsr(3);										//�����Ǵ�����,��2000dps
	MPU_Set_Accel_Fsr(0);										//���ٶȴ�����,��2g
	MPU_Set_Rate(50);												//���ò�����50Hz
	MPU_Write_Byte(MPU_INT_EN_REG, 0X00);		//�ر������ж�
	MPU_Write_Byte(MPU_USER_CTRL_REG, 0X00);	//I2C��ģʽ�رգ���MPU6050����Ϊ��ģʽ��
	MPU_Write_Byte(MPU_FIFO_EN_REG, 0X00);		//�ر�FIFO
	MPU_Write_Byte(MPU_INTBP_CFG_REG, 0X80);	//INT���ŵ͵�ƽ��Ч
	res = MPU_Read_Byte(MPU_DEVICE_ID_REG);
	if (res == MPU_ADDR)												//����ID��ȷ
	{
		MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X01);	//����CLKSEL,PLL X��Ϊ�ο�
		MPU_Write_Byte(MPU_PWR_MGMT2_REG, 0X00);	//���ٶ��������Ƕ�����
		MPU_Set_Rate(50);												//���ò�����Ϊ50Hz���Ƿ���֮ǰ���ظ�����
	}
	else return 1;
	return 0;
}
//------------------------------------------------------------------------------------------
//	*Function	 : MPU_Set_Gyro_Fsr
//	*Descriptiton: ����MPU6050�����Ǵ����������̷�Χ
//	*Parameter	 : fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
//	*Return		 : 0 : ���óɹ�    ����,����ʧ�� 
//	*Others		 :
//------------------------------------------------------------------------------------------
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG, fsr << 3);//���������������̷�Χ  
}
//------------------------------------------------------------------------------------------
//	*Function	 : MPU_Set_Accel_Fsr
//	*Descriptiton: ����MPU6050���ٶȴ����������̷�Χ
//	*Parameter	 : fsr:0,��2g;1,��4g;2,��8g;3,��16g
//	*Return		 : 0 : ���óɹ�        ����,����ʧ�� 
//	*Others		 :
//------------------------------------------------------------------------------------------
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG, fsr << 3);//���ü��ٶȴ����������̷�Χ  
}
//------------------------------------------------------------------------------------------
//	*Function	 : MPU_Set_LPF
//	*Descriptiton: ����MPU6050�����ֵ�ͨ�˲���
//	*Parameter	 : lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
//	*Return		 : 0 : ���óɹ�			����,����ʧ�� 
//	*Others		 :
//------------------------------------------------------------------------------------------
uint8_t MPU_Set_LPF(u16 lpf)
{
	uint8_t data = 0;
	if (lpf >= 188)data = 1;
	else if (lpf >= 98)data = 2;
	else if (lpf >= 42)data = 3;
	else if (lpf >= 20)data = 4;
	else if (lpf >= 10)data = 5;
	else data = 6;
	return MPU_Write_Byte(MPU_CFG_REG, data);//�������ֵ�ͨ�˲���  
}
//------------------------------------------------------------------------------------------
//	*Function	 : MPU_Set_Rate
//	*Descriptiton: ����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
//	*Parameter	 : rate:4~1000(Hz)
//	*Return		 : 0 : ���óɹ�		����,����ʧ��
//	*Others		 :
//------------------------------------------------------------------------------------------
uint8_t MPU_Set_Rate(u16 rate)
{
	uint8_t data;
	if (rate>1000)rate = 1000;
	if (rate<4)rate = 4;
	data = 1000 / rate - 1;
	data = MPU_Write_Byte(MPU_SAMPLE_RATE_REG, data);	//�������ֵ�ͨ�˲���
	return MPU_Set_LPF(rate / 2);	//�Զ�����LPFΪ�����ʵ�һ��
}
//------------------------------------------------------------------------------------------
//	*Function	 : MPU_Get_Temperature
//	*Descriptiton: �õ��¶�ֵ
//	*Parameter	 : None
//	*Return		 : �¶�ֵ(������100��)
//	*Others		 :
//------------------------------------------------------------------------------------------
short MPU_Get_Temperature(void)
{
	uint8_t buf[2];
	short raw;
	float temp;
	MPU_Read_Len(MPU_ADDR, MPU_TEMP_OUTH_REG, 2, buf);
	raw = ((u16)buf[0] << 8) | buf[1];
	temp = 36.53 + ((double)raw) / 340;
	return temp * 100;;
}
//------------------------------------------------------------------------------------------
//	*Function	 : MPU_Get_Gyroscope
//	*Descriptiton: �õ�������ֵ(ԭʼֵ)
//	*Parameter	 : gx,gy,gz:������x,y,z���ԭʼ����(������)
//	*Return		 : 0 : ���óɹ�		����,����ʧ��
//	*Others		 :
//------------------------------------------------------------------------------------------
uint8_t MPU_Get_Gyroscope(short *gx, short *gy, short *gz)
{
	uint8_t buf[6], res;
	res = MPU_Read_Len(MPU_ADDR, MPU_GYRO_XOUTH_REG, 6, buf);
	if (res == 0)
	{
		*gx = ((u16)buf[0] << 8) | buf[1];
		*gy = ((u16)buf[2] << 8) | buf[3];
		*gz = ((u16)buf[4] << 8) | buf[5];
	}
	return res;;
}
//------------------------------------------------------------------------------------------
//	*Function	 : MPU_Get_Accelerometer
//	*Descriptiton: �õ����ٶ�ֵ(ԭʼֵ)
//	*Parameter	 : ax,ay,az:���ٶȼ�x,y,z���ԭʼ����(������)
//	*Return		 : 0 : ���óɹ�		����,����ʧ��
//	*Others		 :
//------------------------------------------------------------------------------------------
uint8_t MPU_Get_Accelerometer(short *ax, short *ay, short *az)
{
	uint8_t buf[6], res;
	res = MPU_Read_Len(MPU_ADDR, MPU_ACCEL_XOUTH_REG, 6, buf);
	if (res == 0)
	{
		*ax = ((u16)buf[0] << 8) | buf[1];
		*ay = ((u16)buf[2] << 8) | buf[3];
		*az = ((u16)buf[4] << 8) | buf[5];
	}
	return res;;
}
//------------------------------------------------------------------------------------------
//	*Function	 : MPU_Write_Len
//	*Descriptiton: IIC����д
//	*Parameter	 : addr:������ַ reg:�Ĵ�����ַ len:д�볤�� buf:������
//	*Return		 : 0 : �ɹ�		����,ʧ��
//	*Others		 :
//------------------------------------------------------------------------------------------
uint8_t MPU_Write_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
	uint8_t i;
	I2C_Start();
	I2C_Send_Byte((addr << 1) | 0);	//����������ַ+д����	
	if (I2C_Wait_Ack())					//�ȴ�Ӧ��
	{
		I2C_Stop();
		return 1;
	}
	I2C_Send_Byte(reg);				//д�Ĵ�����ַ
	I2C_Wait_Ack();						//�ȴ�Ӧ��
	for (i = 0; i<len; i++)
	{
		I2C_Send_Byte(buf[i]);		//��������
		if (I2C_Wait_Ack())				//�ȴ�ACK
		{
			I2C_Stop();
			return 1;
		}
	}
	I2C_Stop();
	return 0;
}
//------------------------------------------------------------------------------------------
//	*Function	 : MPU_Read_Len
//	*Descriptiton: I2C������
//	*Parameter	 : reg:Ҫ��ȡ�ļĴ�����ַ len:Ҫ��ȡ�ĳ��� buf:��ȡ�������ݴ洢��
//	*Return		 : 0 : �ɹ�		����,ʧ��
//	*Others		 :
//------------------------------------------------------------------------------------------
uint8_t MPU_Read_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
	I2C_Start();
	I2C_Send_Byte((addr << 1) | 0);	//����������ַ+д����	
	if (I2C_Wait_Ack())					//�ȴ�Ӧ��
	{
		I2C_Stop();
		return 1;
	}
	I2C_Send_Byte(reg);				//д�Ĵ�����ַ
	I2C_Wait_Ack();						//�ȴ�Ӧ��
	I2C_Start();
	I2C_Send_Byte((addr << 1) | 1);	//����������ַ+������	
	I2C_Wait_Ack();						//�ȴ�Ӧ�� 
	while (len)
	{
		if (len == 1)*buf = I2C_Read_Byte(0);//������,����nACK 
		else *buf = I2C_Read_Byte(1);			//������,����ACK  
		len--;
		buf++;
	}
	I2C_Stop();	//����һ��ֹͣ���� 
	return 0;
}
//------------------------------------------------------------------------------------------
//	*Function	 : MPU_Write_Byte
//	*Descriptiton: IICдһ���ֽ� 
//	*Parameter	 : reg:�Ĵ�����ַ data:����
//	*Return		 : 0 : �ɹ�		����,ʧ��
//	*Others		 :
//------------------------------------------------------------------------------------------
uint8_t MPU_Write_Byte(uint8_t reg, uint8_t data)
{
	I2C_Start();
	I2C_Send_Byte((MPU_ADDR << 1) | 0);//����������ַ+д����	
	if (I2C_Wait_Ack())		//�ȴ�Ӧ��
	{
		I2C_Stop();
		return 1;
	}
	I2C_Send_Byte(reg);			//д�Ĵ�����ַ
	I2C_Wait_Ack();				//�ȴ�Ӧ�� 
	I2C_Send_Byte(data);		//��������
	if (I2C_Wait_Ack())		//�ȴ�ACK
	{
		I2C_Stop();
		return 1;
	}
	I2C_Stop();
	return 0;
}
//------------------------------------------------------------------------------------------
//	*Function	 : MPU_Read_Byte
//	*Descriptiton: IIC��һ���ֽ�  
//	*Parameter	 : reg:�Ĵ�����ַ 
//	*Return		 : 0 : �ɹ�		����,ʧ��
//	*Others		 :
//------------------------------------------------------------------------------------------
uint8_t MPU_Read_Byte(uint8_t reg)
{
	uint8_t res;
	I2C_Start();
	I2C_Send_Byte((MPU_ADDR << 1) | 0);//����������ַ+д����	
	I2C_Wait_Ack();		//�ȴ�Ӧ�� 
	I2C_Send_Byte(reg);	//д�Ĵ�����ַ
	I2C_Wait_Ack();		//�ȴ�Ӧ��
	I2C_Start();
	I2C_Send_Byte((MPU_ADDR << 1) | 1);//����������ַ+������	
	I2C_Wait_Ack();		//�ȴ�Ӧ�� 
	res = I2C_Read_Byte(0);//��ȡ����,����nACK 
	I2C_Stop();			//����һ��ֹͣ���� 
	return res;
}
//------------------------------------------------------------------------------------------
