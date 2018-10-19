//------------------------------------------------------------------------------------------
//	*copyright		: L
//	*FileName		: control
//	*Author			: Cosmos
//	*Version		: V1.0
//	*Date			: 2018.9.12
//	*Description	: ��������̬PID����
//		
//	*Others			:
//	*Fun List		:
//	*History		:
//------------------------------------------------------------------------------------------
//Includes
//------------------------------------------------------------------------------------------
#include "APP/Control/Control.h"
#include "APP/Globle_Var/Globle_Var.h"
#include "Drv_PWM/Drv_PWM.h"
//------------------------------------------------------------------------------------------
PID_Typedef pitch_angle_PID, pitch_speed_PID;		//�����Ƕȣ����ٶȿ��ƻ�
PID_Typedef roll_angle_PID, roll_speed_PID;			//�����Ƕȣ����ٶȿ��ƻ�
PID_Typedef	yaw_angle_PID, yaw_speed_PID;			//ƫ���Ƕȣ����ٶȿ��ƻ�
PID_Typedef height_PID;
#define		ERROR_ANGLE_PITCH_MAX			100
#define		ERROR_ANGLE_ROLL_MAX			100
#define		ERROR_ANGLE_YAW_MAX				100
#define		ERROR_SPEED_PITCH_MAX			100
#define		ERROR_SPEED_ROLL_MAX			100
#define		ERROR_SPEED_YAW_MAX				100
int16_t thr,pitch, roll, yaw;
//------------------------------------------------------------------------------------------
//	*Function	 : PID_Caculate
//	*Descriptiton: ����PID���
//	*Parameter	 : PID : ��Ҫ�����PID�ṹ��
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
float PID_Caculate(PID_Typedef PID)
{
	return PID.P*PID.Error + PID.I*PID.Integ + PID.D*PID.Deriv;
}
//------------------------------------------------------------------------------------------
//	*Function	 : PID_Init
//	*Descriptiton: ������΢�֣�����ϵ����ʼ��
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void PID_Init(void)
{
	//����PIDϵ����ʼ��
	pitch_angle_PID.P = 4;
	pitch_angle_PID.I = 3;
	pitch_angle_PID.D = 1;

	pitch_speed_PID.P = 0.45;
	pitch_speed_PID.I = 0;
	pitch_speed_PID.D = 1.8;
	//����PIDϵ����ʼ��
	roll_angle_PID.P = 4;
	roll_angle_PID.I = 3;
	roll_angle_PID.D = 1;

	roll_speed_PID.P = 0.4;
	roll_speed_PID.I = 0;
	roll_speed_PID.D = 1.2;
	//ƫ��PIDϵ����ʼ��
	yaw_angle_PID.P = 3;
	yaw_angle_PID.I = 0;
	yaw_angle_PID.D = 0;

	yaw_speed_PID.P = 3;
	yaw_speed_PID.I = 0;
	yaw_speed_PID.D = 0;
}
//------------------------------------------------------------------------------------------
//	*Function	 : control_angle
//	*Descriptiton: �Ƕȿ��ƻ�
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void control_angle(void)
{
	//�Ƕ����
	pitch_angle_PID.Error = RC_Data.pitch - MPU_Data.pitch;
	roll_angle_PID.Error = RC_Data.roll - MPU_Data.roll;
	yaw_angle_PID.Error = RC_Data.yaw - MPU_Data.yaw;
	//�ǶȻ���
	pitch_angle_PID.Integ += pitch_angle_PID.Error;
	roll_angle_PID.Integ += roll_angle_PID.Error;
	yaw_angle_PID.Integ += yaw_angle_PID.Error;
	//���ַ�ֵ�޶�
	if (pitch_angle_PID.Integ > ERROR_ANGLE_PITCH_MAX)
		pitch_angle_PID.Integ = ERROR_ANGLE_PITCH_MAX;
	if (roll_angle_PID.Integ > ERROR_ANGLE_ROLL_MAX)
		roll_angle_PID.Integ = ERROR_ANGLE_ROLL_MAX;
	if (yaw_angle_PID.Integ > ERROR_ANGLE_YAW_MAX)
		yaw_angle_PID.Integ = ERROR_ANGLE_YAW_MAX;
	//�Ƕ�΢��
	pitch_angle_PID.Deriv = pitch_angle_PID.Error - pitch_angle_PID.PreError;
	roll_angle_PID.Deriv = roll_angle_PID.Error - roll_angle_PID.PreError;
	yaw_angle_PID.Deriv = yaw_angle_PID.Error - yaw_angle_PID.PreError;
	//�Ƕ�PID���---�⻷���
	pitch_angle_PID.Output = PID_Caculate(pitch_angle_PID);
	roll_angle_PID.Output = PID_Caculate(roll_angle_PID);
	yaw_angle_PID.Output = PID_Caculate(yaw_angle_PID);
	//�������
	pitch_angle_PID.PreError = pitch_angle_PID.Error;
	roll_angle_PID.PreError = roll_angle_PID.Error;
	yaw_angle_PID.PreError = yaw_angle_PID.Error;
}
//------------------------------------------------------------------------------------------
//	*Function	 : control_speed
//	*Descriptiton: ���ٶȿ��ƻ�
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void control_speed(void)
{
	//���ٶ����
	roll_speed_PID.Error = gyro.x - roll_angle_PID.Output;
	pitch_speed_PID.Error = gyro.y - pitch_speed_PID.Output;
	yaw_speed_PID.Error = gyro.z - yaw_speed_PID.Output;
	//�ǶȻ���
	roll_speed_PID.Integ = (roll_speed_PID.Integ + roll_speed_PID.Error)/1000;
	pitch_speed_PID.Integ = (pitch_speed_PID.Integ+pitch_speed_PID.Error)/1000;
	yaw_speed_PID.Integ = (yaw_speed_PID.Integ+yaw_speed_PID.Error)/1000;
	//���ַ�ֵ�޶�
	if (pitch_speed_PID.Integ > ERROR_SPEED_PITCH_MAX)
		pitch_speed_PID.Integ = ERROR_SPEED_PITCH_MAX;
	if (roll_speed_PID.Integ > ERROR_SPEED_ROLL_MAX)
		roll_speed_PID.Integ = ERROR_SPEED_ROLL_MAX;
	if (yaw_speed_PID.Integ > ERROR_SPEED_YAW_MAX)
		yaw_speed_PID.Integ = ERROR_SPEED_YAW_MAX;
	//���ٶ�΢��
	pitch_speed_PID.Deriv = pitch_speed_PID.Error - pitch_speed_PID.PreError;
	roll_speed_PID.Deriv = roll_speed_PID.Error - roll_speed_PID.PreError;
	yaw_speed_PID.Deriv = yaw_speed_PID.Error - yaw_speed_PID.PreError;
	//���ٶ�PID���---�⻷���
	pitch=pitch_speed_PID.Output = PID_Caculate(pitch_speed_PID);
	roll=roll_speed_PID.Output = PID_Caculate(roll_speed_PID);
	yaw=yaw_speed_PID.Output = PID_Caculate(yaw_speed_PID);
	//�������
	pitch_speed_PID.PreError = pitch_speed_PID.Error;
	roll_speed_PID.PreError = roll_speed_PID.Error;
	yaw_speed_PID.PreError = yaw_speed_PID.Error;

}
//------------------------------------------------------------------------------------------
//	*Function	 : control_height
//	*Descriptiton: �߶ȿ��ƻ�
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void control_height(void)
{

}
//------------------------------------------------------------------------------------------
//	*Function	 : control
//	*Descriptiton: ��������̬����
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void control(void)
{
	int16_t motor[4];
	if (fly_flag.unclock)
	{
		control_angle();
		control_speed();
		//if (fly_flag.height_mod)
		//{
		//	control_height();
		//	//�޸�����ֵ
		//}
		//else
		//{
		//	thr = RC_Data.thr;
		//}
		motor[2] = (int16_t)(thr + pitch - roll - yaw);
		motor[0] = (int16_t)(thr - pitch + roll - yaw);
		motor[3] = (int16_t)(thr + pitch + roll + yaw);
		motor[1] = (int16_t)(thr - pitch - roll + yaw);
		MotorFlash(motor[0], motor[1], motor[2], motor[3]);
	}
	else
	{
		MotorFlash(0, 0, 0, 0);
	}
}
//------------------------------------------------------------------------------------------
