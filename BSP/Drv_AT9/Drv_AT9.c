//------------------------------------------------------------------------------------------
//	*copyright		: L
//	*FileName		: Drv_AT9
//	*Author			: Cosmos
//	*Version		: V1.0
//	*Date			: 2018.9.4	 
//	*Description	: ��ʼ����������������ͨ��ֵ
//		
//	*Others			:
//	*Fun List		:
//	*History		:
//Includes
//------------------------------------------------------------------------------------------
#include "Drv_AT9/Drv_AT9.h"
#include "APP/Globle_Var/Globle_Var.h"
#include "Drv_Pwm/Drv_PWM.h"
#include "Drv_time/Drv_time.h"
//------------------------------------------------------------------------------------------
#define AT9_USART						USART1
#define AT9_USART_CLK					RCC_APB2Periph_USART1
#define AT9_USART_BAUDRATE				99000 //���ڲ�����

#define AT9_USART_RX_GPIO_PORT			GPIOA
#define AT9_USART_RX_GPIO_CLK			RCC_AHB1Periph_GPIOA
#define AT9_USART_RX_PIN				GPIO_Pin_9
#define AT9_USART_RX_AF					GPIO_AF_USART1
#define AT9_USART_RX_SOURCE				GPIO_PinSource9

#define AT9_USART_TX_GPIO_PORT			GPIOA
#define AT9_USART_TX_GPIO_CLK			RCC_AHB1Periph_GPIOA
#define AT9_USART_TX_PIN				GPIO_Pin_10
#define AT9_USART_TX_AF					GPIO_AF_USART1
#define AT9_USART_TX_SOURCE				GPIO_PinSource10

#define AT9_USART_IRQHandler			USART1_IRQHandler
#define AT9_USART_IRQ					USART1_IRQn

#define JUDGE_ENDBYTE(data)				((data==0x04)|(data==0x14)|(data==0x24)|(data==0x34))
//------------------------------------------------------------------------------------------
//	*Function	 : NVIC_Configuration
//	*Descriptiton: �����жϴ���
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Ƕ�������жϿ�������ѡ�� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* ���� USART Ϊ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = AT9_USART_IRQ;
	/* �������ȼ�Ϊ 1 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	/* �����ȼ�Ϊ 1 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	/* ʹ���ж� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* ��ʼ������ NVIC */
	NVIC_Init(&NVIC_InitStructure);
}
//------------------------------------------------------------------------------------------
//	*Function	 : AT9_Init
//	*Descriptiton: ��ʼ��������
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void AT9_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/*����ʱ��ʹ��*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	/*USARTʱ��ʹ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* ���� PXx �� USARTx_Tx*/
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

	/* GPIO ��ʼ�� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	/* ���� Rx ����Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* ���ô� DEBUG_USART ģʽ */
	/* ���������ã� DEBUG_USART_BAUDRATE */
	USART_InitStructure.USART_BaudRate =99000;
	/* �ֳ�(����λ+У��λ)�� 8 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	/* ֹͣλ�� 1 ��ֹͣλ */
	USART_InitStructure.USART_StopBits = USART_StopBits_2;
	/* У��λѡ��żУ�� */
	USART_InitStructure.USART_Parity = USART_Parity_Even;
	/* Ӳ�������ƣ���ʹ��Ӳ���� */
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	/* USART ģʽ���ƣ�ͬʱʹ�ܽ��պͷ��� */
	USART_InitStructure.USART_Mode = USART_Mode_Rx;
	/* ��� USART ��ʼ������ */
	USART_Init(USART1, &USART_InitStructure);

	NVIC_Configuration();

	//ʹ�ܴ��ڽ����ж� 
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//ʹ�ܴ���
	USART_Cmd(USART1, ENABLE);
}
//------------------------------------------------------------------------------------------
//	*Function	 : AT9_CH_DataProcess
//	*Descriptiton: ��AT9���͵����ݣ�����Ϊ����ͨ������ֵ(ÿ��ͨ��Ϊ11λ����)
//	*Parameter	 : data: AT9���͵���������
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void AT9_CH_DataProcess(uint16_t *data)
{
	//ֱ������
	ch_in[0] = ((((data[1] | data[2] << 8) & 0x07FF))-1000)/4;
	ch_in[1] = ((((data[2] >> 3 | data[3] << 5) & 0x07FF))-1000)/4;
	ch_in[2] = ((((data[3] >> 6 | data[4] << 2 | data[5] << 10) & 0x07FF)) - 1000) / 4;
	ch_in[3] = ((((data[5] >> 1 | data[6] << 7) & 0x07FF)) - 1000) / 4; 
	ch_in[4] = ((((data[6] >> 4 | data[7] << 4) & 0x07FF)) - 1000) / 4;
	ch_in[5] = ((((data[7] >> 7 | data[8] << 1 | data[9] << 9) & 0x07FF)) - 1000) / 4;
	ch_in[6] = ((((data[9] >> 2 | data[10] << 6) & 0x07FF)) - 1000) / 4;
	ch_in[7] = ((((data[10] >> 5 | data[11] << 3) & 0x07FF)) - 1000) / 4;
	//ch_in[8] = ((data[12] | data[13] << 8) & 0x07FF);
	//ch_in[9] = ((data[13] >> 3 | data[14] << 5) & 0x07FF);
	//ch_in[10] = ((data[14] >> 6 | data[15] << 2 | data[16] << 10) & 0x07FF);
	//ch_in[11] = ((data[16] >> 1 | data[17] << 7) & 0x07FF);
	//ch_in[12] = ((data[17] >> 4 | data[18] << 4) & 0x07FF);
	//ch_in[13] = ((data[18] >> 7 | data[19] << 1 | data[20] << 9) & 0x07FF);
	//ch_in[14] = ((data[20] >> 2 | data[21] << 6) & 0x07FF);
	//ch_in[15] = ((data[21] >> 5 | data[22] << 3) & 0x07FF);
}
//------------------------------------------------------------------------------------------
//	*Function	 : AT9_USART_IRQHandler
//	*Descriptiton: USART�����жϣ���ȡң����ͨ����ֵ
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void AT9_USART_IRQHandler(void)
{
	uint16_t data[25];   //����֡
	static uint8_t index=0;  //�±�
	uint8_t temp;   
	static uint8_t flag=0; //������ݴ��俪ʼ

	if (USART_GetITStatus(AT9_USART, USART_IT_RXNE) != RESET) {
		temp = USART_ReceiveData(AT9_USART);
		if (flag)
		{
			//һ֡������
			index++;
			data[index] = temp;
			//�������
			if (index == 24)
			{
				flag = 0;		//��־λ���㣬����һ�δ������
				index = 0;		//�±����㣬Ϊ��һ�δ�����׼��
				//�жϴ�֡�����Ƿ�����
				if (data[24] == 0 && data[23] == 0)
				{
					data[23] = 1;
					data[24] = 1;
					AT9_CH_DataProcess(data);
				}
			}
		}
		//�ж���ʼλ
		else if (temp == 0x0f && index == 0)
		{
			flag = 1;
		}
	}
}
//------------------------------------------------------------------------------------------
//	*Function	 : AC_Data_Process
//	*Descriptiton: ����ң�����ź�
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void AC_Data_Process(void)
{
	//����,��̬���ݻ�ȡ
	RC_Data.thr = ch_in[0] * 1.5;
	RC_Data.pitch = ch_in[1] * 0.08;
	RC_Data.roll = ch_in[2] * 0.08;
	RC_Data.yaw = ch_in[3] * 0.08;
	//����
	if (ch_in[0]<-150&&ch_in[1]>150&&ch_in[2]>150&&ch_in[3]>150)
	{
		if (fly_flag.unclock == 0)
		{
			delay_ms(500);
			if (ch_in[0] < -150 && ch_in[1] >150 && ch_in[2]>150 && ch_in[3]>150)
			{
				fly_flag.unclock = 1;
				RGB_State = 1;
				DTInit();
			}
		}
		else {
			delay_ms(200);
			if (ch_in[0] < -150 && ch_in[1] >150 && ch_in[2]>150 && ch_in[3]>150)
			{
				fly_flag.unclock = 0;
				RGB_State = 3;
				CloseDT();
			}
		}
	}
	//����ģʽ
	if (ch_in[5] > 300)
	{
		fly_flag.height_mod = 1;
		RGB_State = 2;
	}
}
//------------------------------------------------------------------------------------------
