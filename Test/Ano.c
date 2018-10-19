//------------------------------------------------------------------------------------------
//	*copyright		: L
//	*FileName		: Ano.c
//	*Author			: Cosmos
//	*Version		: V1.0
//	*Date			: 2018.9.23
//	*Description	: ������λ�����ͳ���
//		
//	*Others			: 
//	*Fun List		:
//	*History		:
//------------------------------------------------------------------------------------------
//Includes
//------------------------------------------------------------------------------------------
#include "Test/Ano.h"

//���ݲ�ֺ궨�壬�ڷ��ʹ���1�ֽڵ���������ʱ������int16��float�ȣ���Ҫ�����ݲ�ֳɵ����ֽڽ��з���
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)		) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )
uint8_t data_to_send[50];	//�������ݻ���

//------------------------------------------------------------------------------------------
//	*Function	 : ANO_DT_Send_Data
//	*Descriptiton: ����һ֡����
//	*Parameter	 : data : ����Ҫ���͵�����			len : ���ݳ���
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void ANO_DT_Send_Data(uint8_t *data , uint8_t len)
{
	uint8_t i;
	for (i = 0; i<len; i++)
	{
		/*USART_SendData(USART1, *(data + i));
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);*/
		USART_SendData(USART3, *(data + i));
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
	}
}
//------------------------------------------------------------------------------------------
//	*Function	 : ANO_DT_Send_Status
//	*Descriptiton: �ɻ���̬���߶�һ�����ݷ��ͣ�����������λ���۲�3Dͼ��
//	*Parameter	 : angle_rol  angle_pit angle_yaw alt FLY_ENABLEl(��������) armed(����ģʽ)
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void ANO_DT_Send_Status(float angle_rol, float angle_pit, float angle_yaw, s32 alt, uint8_t FLY_ENABLEl, uint8_t armed)
{
	uint8_t _cnt = 0, sum = 0, i;
	vs16 _temp;
	vs32 _temp2 = alt;

	data_to_send[_cnt++] = 0xAA;
	data_to_send[_cnt++] = 0x05;
	data_to_send[_cnt++] = 0xAF;
	data_to_send[_cnt++] = 0x01;
	data_to_send[_cnt++] = 0;

	_temp = (int)(angle_rol * 100);
	data_to_send[_cnt++] = BYTE1(_temp);
	data_to_send[_cnt++] = BYTE0(_temp);
	_temp = (int)(angle_pit * 100);
	data_to_send[_cnt++] = BYTE1(_temp);
	data_to_send[_cnt++] = BYTE0(_temp);
	_temp = (int)(angle_yaw * 100);
	data_to_send[_cnt++] = BYTE1(_temp);
	data_to_send[_cnt++] = BYTE0(_temp);

	data_to_send[_cnt++] = BYTE3(_temp2);
	data_to_send[_cnt++] = BYTE2(_temp2);
	data_to_send[_cnt++] = BYTE1(_temp2);
	data_to_send[_cnt++] = BYTE0(_temp2);

	data_to_send[_cnt++] = FLY_ENABLEl;

	data_to_send[_cnt++] = armed;

	data_to_send[4] = _cnt - 5;

	for (i = 0; i<_cnt; i++)
		sum += data_to_send[i];
	data_to_send[_cnt++] = sum;

	ANO_DT_Send_Data(data_to_send, _cnt);
}
//------------------------------------------------------------------------------------------
