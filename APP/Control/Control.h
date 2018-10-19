#ifndef __CONTROL_H__
#define __CONTROL_H__

typedef struct
{
	float P;			//����ϵ��
	float I;			//����ϵ��
	float D;			//΢��ϵ��
	//float Desired;		
	float Error;		//��ǰ���
	float PreError;		//ǰ�����
	double Integ;		//������
	float Deriv;		//���΢��
	float Output;		//PID���
}PID_Typedef;

void PID_Init(void);
void control(void);
#endif // !__CONTROL_H__
