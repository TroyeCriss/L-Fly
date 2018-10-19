//------------------------------------------------------------------------------------------
//	*copyright		: L
//	*FileName		: Drv_SPL06
//	*Author			: Cosmos
//	*Version		: V1.0
//	*Date			: 2018.9.15
//	*Description	: ��ѹ�Ƴ�ʼ��������Ӧ�����ݴ���
//		
//	*Others			: 
//	*Fun List		:
//	*History		:
//------------------------------------------------------------------------------------------
//Includes
//------------------------------------------------------------------------------------------
#include "Drv_SPL06/Drv_SPL06.h"
#include "Drv_SPI/Drv_SPI.h"
#include "Drv_time/Drv_time.h"
//------------------------------------------------------------------------------------------
uint32_t _C0, _C1, _C00, _C10, _C01, _C11, _C20, _C21, _C30, _raw_temp, _raw_press;
float _kT, _kP;
float _Temp, _Press;
//��ѹ��������(sample/sec),Background ģʽʹ��
#define  PM_RATE_1          (0<<4)      //1 measurements pr. sec.
#define  PM_RATE_2          (1<<4)      //2 measurements pr. sec.
#define  PM_RATE_4          (2<<4)      //4 measurements pr. sec.           
#define  PM_RATE_8          (3<<4)      //8 measurements pr. sec.
#define  PM_RATE_16         (4<<4)      //16 measurements pr. sec.
#define  PM_RATE_32         (5<<4)      //32 measurements pr. sec.
#define  PM_RATE_64         (6<<4)      //64 measurements pr. sec.
#define  PM_RATE_128        (7<<4)      //128 measurements pr. sec.

//��ѹ�ز�������(times),Background ģʽʹ��
#define PM_PRC_1            0       //Sigle         kP=524288   ,3.6ms
#define PM_PRC_2            1       //2 times       kP=1572864  ,5.2ms
#define PM_PRC_4            2       //4 times       kP=3670016  ,8.4ms
#define PM_PRC_8            3       //8 times       kP=7864320  ,14.8ms
#define PM_PRC_16           4       //16 times      kP=253952   ,27.6ms
#define PM_PRC_32           5       //32 times      kP=516096   ,53.2ms
#define PM_PRC_64           6       //64 times      kP=1040384  ,104.4ms
#define PM_PRC_128          7       //128 times     kP=2088960  ,206.8ms

//�¶Ȳ�������(sample/sec),Background ģʽʹ��
#define  TMP_RATE_1         (0<<4)      //1 measurements pr. sec.
#define  TMP_RATE_2         (1<<4)      //2 measurements pr. sec.
#define  TMP_RATE_4         (2<<4)      //4 measurements pr. sec.           
#define  TMP_RATE_8         (3<<4)      //8 measurements pr. sec.
#define  TMP_RATE_16        (4<<4)      //16 measurements pr. sec.
#define  TMP_RATE_32        (5<<4)      //32 measurements pr. sec.
#define  TMP_RATE_64        (6<<4)      //64 measurements pr. sec.
#define  TMP_RATE_128       (7<<4)      //128 measurements pr. sec.

//�¶��ز�������(times),Background ģʽʹ��
#define TMP_PRC_1           0       //Sigle
#define TMP_PRC_2           1       //2 times
#define TMP_PRC_4           2       //4 times
#define TMP_PRC_8           3       //8 times
#define TMP_PRC_16          4       //16 times
#define TMP_PRC_32          5       //32 times
#define TMP_PRC_64          6       //64 times
#define TMP_PRC_128         7       //128 times

//SPL06_MEAS_CFG
#define MEAS_COEF_RDY       0x80
#define MEAS_SENSOR_RDY     0x40        //��������ʼ�����
#define MEAS_TMP_RDY        0x20        //���µ��¶�����
#define MEAS_PRS_RDY        0x10        //���µ���ѹ����

#define MEAS_CTRL_Standby               0x00    //����ģʽ
#define MEAS_CTRL_PressMeasure          0x01    //������ѹ����
#define MEAS_CTRL_TempMeasure           0x02    //�����¶Ȳ���
#define MEAS_CTRL_ContinuousPress       0x05    //������ѹ����
#define MEAS_CTRL_ContinuousTemp        0x06    //�����¶Ȳ���
#define MEAS_CTRL_ContinuousPressTemp   0x07    //������ѹ�¶Ȳ���

//FIFO_STS
#define SPL06_FIFO_FULL     0x02
#define SPL06_FIFO_EMPTY    0x01

//INT_STS
#define SPL06_INT_FIFO_FULL     0x04
#define SPL06_INT_TMP           0x02
#define SPL06_INT_PRS           0x01

//SPL06�Ĵ�����ַ
#define SPL06_CFG_T_SHIFT   0x08    //oversampling times>8ʱ����ʹ��
#define SPL06_CFG_P_SHIFT   0x04

#define SP06_PSR_B2     0x00        //��ѹֵ
#define SP06_PSR_B1     0x01
#define SP06_PSR_B0     0x02
#define SP06_TMP_B2     0x03        //�¶�ֵ
#define SP06_TMP_B1     0x04
#define SP06_TMP_B0     0x05

#define SP06_PSR_CFG    0x06        //��ѹ��������
#define SP06_TMP_CFG    0x07        //�¶Ȳ�������
#define SP06_MEAS_CFG   0x08        //����ģʽ����

#define SP06_CFG_REG    0x09
#define SP06_INT_STS    0x0A
#define SP06_FIFO_STS   0x0B

#define SP06_RESET      0x0C
#define SP06_ID         0x0D

#define SP06_COEF       0x10        //-0x21
#define SP06_COEF_SRCE  0x28
//------------------------------------------------------------------------------------------
//	*Function	 : SPL06_Start
//	*Descriptiton: ����������SPL06��ģʽ
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
static void SPL06_Start(uint8_t mode)
{
	SPI_Write(SP06_MEAS_CFG, mode);
}
//------------------------------------------------------------------------------------------
//	*Function	 : SPL06_Config_Temperature
//	*Descriptiton: SPL06�¶ȴ������Ĳ����ʣ����ֵ����
//	*Parameter	 : rate : ������		oversampling : �������
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
static void SPL06_Config_Temperature(uint8_t rate, uint8_t oversampling)
{
	switch (oversampling)
	{
	case TMP_PRC_1:
		_kT = 524288;
		break;
	case TMP_PRC_2:
		_kT = 1572864;
		break;
	case TMP_PRC_4:
		_kT = 3670016;
		break;
	case TMP_PRC_8:
		_kT = 7864320;
		break;
	case TMP_PRC_16:
		_kT = 253952;
		break;
	case TMP_PRC_32:
		_kT = 516096;
		break;
	case TMP_PRC_64:
		_kT = 1040384;
		break;
	case TMP_PRC_128:
		_kT = 2088960;
		break;
	}

	SPI_Write(SP06_TMP_CFG, rate | oversampling | 0x80);   //�¶�ÿ��128�β���һ��
	if (oversampling > TMP_PRC_8)
	{
		uint8_t temp = 0;
		SPI_Read(SP06_CFG_REG, 1, &temp);
		SPI_Write(SP06_CFG_REG, temp | SPL06_CFG_T_SHIFT);
	}
}
//------------------------------------------------------------------------------------------
//	*Function	 : SPL06_Config_Pressure
//	*Descriptiton: SPL06ѹ���������Ĳ����ʣ����ֵ����
//	*Parameter	 : rate : ������		oversampling : �������
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
static void SPL06_Config_Pressure(uint8_t rate, uint8_t oversampling)
{
	switch (oversampling)
	{
	case PM_PRC_1:
		_kP = 524288;
		break;
	case PM_PRC_2:
		_kP = 1572864;
		break;
	case PM_PRC_4:
		_kP = 3670016;
		break;
	case PM_PRC_8:
		_kP = 7864320;
		break;
	case PM_PRC_16:
		_kP = 253952;
		break;
	case PM_PRC_32:
		_kP = 516096;
		break;
	case PM_PRC_64:
		_kP = 1040384;
		break;
	case PM_PRC_128:
		_kP = 2088960;
		break;
	}

	SPI_Write(SP06_PSR_CFG, rate | oversampling);
	if (oversampling > PM_PRC_8)
	{
		uint8_t temp = 0;
		SPI_Read(SP06_CFG_REG, 1, &temp);
		SPI_Write(SP06_CFG_REG, temp | SPL06_CFG_P_SHIFT);
	}
}
//------------------------------------------------------------------------------------------
//	*Function	 : SPL06_Init
//	*Descriptiton: SPL06��ѹ�Ƴ�ʼ��
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void SPL06_Init(void)
{	
	uint8_t coef[18],ID;
	delay_ms(40);
	SPI_Write(SP06_RESET, 0x89);	//��λ������
	SPI_Read(SP06_ID, 1, &ID);
	SPI_Read(SP06_COEF, 18, coef);
	delay_ms(40);
	_C0 = ((int16_t)coef[0] << 4) + ((coef[1] & 0xF0) >> 4);
	_C0 = (_C0 & 0x0800) ? (0xF000 | _C0) : _C0;
	_C1 = ((int16_t)(coef[1] & 0x0F) << 8) + coef[2];
	_C1 = (_C1 & 0x0800) ? (0xF000 | _C1) : _C1;
	_C00 = ((int32_t)coef[3] << 12) + ((uint32_t)coef[4] << 4) + (coef[5] >> 4);
	_C10 = ((int32_t)(coef[5] & 0x0F) << 16) + ((uint32_t)coef[6] << 8) + coef[7];
	_C00 = (_C00 & 0x080000) ? (0xFFF00000 | _C00) : _C00;
	_C10 = (_C10 & 0x080000) ? (0xFFF00000 | _C10) : _C10;
	_C01 = ((int16_t)coef[8] << 8) + coef[9];
	_C11 = ((int16_t)coef[10] << 8) + coef[11];
	_C11 = (_C11 & 0x0800) ? (0xF000 | _C11) : _C11;
	_C20 = ((int16_t)coef[12] << 8) + coef[13];
	_C20 = (_C20 & 0x0800) ? (0xF000 | _C20) : _C20;
	_C21 = ((int16_t)coef[14] << 8) + coef[15];
	_C21 = (_C21 & 0x0800) ? (0xF000 | _C21) : _C21;
	_C30 = ((int16_t)coef[16] << 8) + coef[17];
	_C30 = (_C30 & 0x0800) ? (0xF000 | _C30) : _C30;

	SPL06_Config_Pressure(PM_RATE_128, PM_PRC_8);
	SPL06_Config_Temperature(PM_RATE_8, TMP_PRC_8);

	SPL06_Start(MEAS_CTRL_ContinuousPressTemp); //������������ѹ�¶Ȳ���
	delay_ms(20);

}
//------------------------------------------------------------------------------------------
//	*Function	 : SPL06_Get_Pressure_ADC
//	*Descriptiton: ���ѹ����������ԭʼֵ
//	*Parameter	 : None
//	*Return		 : ѹ��������ԭʼֵ
//	*Others		 :
//------------------------------------------------------------------------------------------
static int32_t SPL06_Get_Pressure_ADC()
{
	uint8_t buf[3];
	int32_t adc;

	SPI_Read(SP06_PSR_B2, 3, buf);
	adc = (int32_t)(buf[0] << 16) + (buf[1] << 8) + buf[2];
	adc = (adc & 0x800000) ? (0xFF000000 | adc) : adc;

	return adc;
}
//------------------------------------------------------------------------------------------
//	*Function	 : SPL06_Get_Temperature_ADC
//	*Descriptiton: ��ȡ�¶ȴ�����ԭʼֵ
//	*Parameter	 : None
//	*Return		 : �¶ȴ�������ֵ
//	*Others		 :
//------------------------------------------------------------------------------------------
static int32_t SPL06_Get_Temperature_ADC()
{
	uint8_t buf[3];
	int32_t adc;

	SPI_Read(SP06_TMP_B2, 3, buf);
	adc = (int32_t)(buf[0] << 16) + (buf[1] << 8) + buf[2];
	adc = (adc & 0x800000) ? (0xFF000000 | adc) : adc;

	return adc;
}
//------------------------------------------------------------------------------------------
//	*Function	 : SPL06_Update_Pressure
//	*Descriptiton: ��ԭʼֵ�����ʵ�ʵ��¶Ⱥ���ѹֵ
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
static void SPL06_Update_Pressure()
{
	float Traw_src, Praw_src;
	float qua2, qua3;

	Traw_src = _raw_temp/_kT;
	Praw_src =  _raw_press/_kP;

	//�����¶�
	_Temp = 0.5f*_C0 + Traw_src * _C1;

	//������ѹ
	qua2 = _C10 + Praw_src * (_C20 + Praw_src * _C30);
	qua3 = Traw_src * Praw_src * (_C11 + Praw_src * _C21);
	_Press = _C00 + Praw_src * qua2 + Traw_src * _C01 + qua3;
}
//------------------------------------------------------------------------------------------
//	*Function	 : SPL06_Update
//	*Descriptiton: �����¶Ⱥ�ѹ��ֵ
//	*Parameter	 : None
//	*Return		 : None 
//	*Others		 :
//------------------------------------------------------------------------------------------
void SPL06_Update()
{
	_raw_temp = SPL06_Get_Temperature_ADC();
	_raw_press = SPL06_Get_Pressure_ADC();
	SPL06_Update_Pressure();
}
//------------------------------------------------------------------------------------------
//	*Function	 : SPL06_Get_Temperature
//	*Descriptiton: ���ص�ǰ�¶�ֵ
//	*Parameter	 : None
//	*Return		 : �¶�ֵ
//	*Others		 :
//------------------------------------------------------------------------------------------
float SPL06_Get_Temperature()
{
	return _Temp;
}
//------------------------------------------------------------------------------------------
//	*Function	 : SPL06_Get_Pressure
//	*Descriptiton: ������ѹֵ
//	*Parameter	 : None
//	*Return		 : ��ѹֵ
//	*Others		 :
//------------------------------------------------------------------------------------------
float SPL06_Get_Pressure()
{
	return _Press;
}
//------------------------------------------------------------------------------------------
