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
uint8_t data[25]; 
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

static uint32_t ch_dog = 0;
//------------------------------------------------------------------------------------------
//	*Function	 : ch_feed_dog
//	*Descriptiton: ι��
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
static void ch_feed_dog(void)
{
	ch_dog = 0;
}
//------------------------------------------------------------------------------------------
//	*Function	 : ch_watch_dog
//	*Descriptiton: ���Ź����
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
static void ch_watch_dog(uint8_t dT_ms)
{
	uint8_t i = 0;
	if (ch_dog < 500)
	{
		ch_dog += dT_ms;
	}
	else
	{
		for (i = 0; i < 8; i++)
			ch_in[i] = 0;
	}
}
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

	///* ���� USART Ϊ�ж�Դ */
	//NVIC_InitStructure.NVIC_IRQChannel = AT9_USART_IRQ;
	///* �������ȼ�Ϊ 1 */
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	///* �����ȼ�Ϊ 1 */
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	///* ʹ���ж� */
	//NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	///* ��ʼ������ NVIC */
	//NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
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
	DMA_InitTypeDef DMA_InitStructure;
	/*����ʱ��ʹ��*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	/*USARTʱ��ʹ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	/*ʹ��DMA*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
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
	/* USART ģʽ���ƣ�ʹ�ܽ��� */
	USART_InitStructure.USART_Mode = USART_Mode_Rx;
	/* ��� USART ��ʼ������ */
	USART_Init(USART1, &USART_InitStructure);
	////ʹ�ܴ��ڽ����ж� 
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//ʹ�ܴ���
	USART_Cmd(USART1, ENABLE);

	//DMA�ж�
	NVIC_Configuration();
	/*����ͨ��*/
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;

	/*����DMAԴ���ڴ��ַ&�������ݼĴ�����ַ*/
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t) & (USART1->DR);

	/*�ڴ��ַ(Ҫ����ı�����ָ��)*/
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)data;

	/*���򣺴����赽�ڴ�*/
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;

	/*�����СDMA_BufferSize=SENDBUFF_SIZE*/

	DMA_InitStructure.DMA_BufferSize = 25;

	/*�����ַ����*/
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	/*�ڴ��ַ����*/
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

	/*�������ݵ�λ*/
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

	/*�ڴ����ݵ�λ 8bit*/
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;

	/*DMAģʽ��ѭ��*/
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;

	/*���ȼ����ǳ���*/
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;

	/*��ֹFIFO*/
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;

	/*ͻ������*/
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_INC16;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	DMA_Init(DMA2_Stream2, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream2, ENABLE);
	DMA_ITConfig(DMA2_Stream2, DMA_IT_TC, ENABLE); 
	USART_DMACmd(USART1,USART_DMAReq_Rx, ENABLE);
	
}
//------------------------------------------------------------------------------------------
//	*Function	 : DMA2_Stream2_IRQHandler
//	*Descriptiton: DMA2�ж�
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void DMA2_Stream2_IRQHandler(void)
{
	if (DMA_GetFlagStatus(DMA2_Stream2, DMA_FLAG_TCIF2)==SET)
	{
		DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);
		DMA_ClearITPendingBit(DMA2_Stream2, DMA_FLAG_TCIF2);
		if (data[0] == 0x0f && data[23] == 0 && data[24] == 0)
		{
			ch_in[0] = ((((data[1] | data[2] << 8) & 0x07FF)) - 1000) / 4;
			ch_in[1] = ((((data[2] >> 3 | data[3] << 5) & 0x07FF)) - 1000) / 4;
			ch_in[2] = ((((data[3] >> 6 | data[4] << 2 | data[5] << 10) & 0x07FF)) - 1000) / 4;
			ch_in[3] = ((((data[5] >> 1 | data[6] << 7) & 0x07FF)) - 1000) / 4;
			ch_in[4] = ((((data[6] >> 4 | data[7] << 4) & 0x07FF)) - 1000) / 4;
			ch_in[5] = ((((data[7] >> 7 | data[8] << 1 | data[9] << 9) & 0x07FF)) - 1000) / 4;
			ch_in[6] = ((((data[9] >> 2 | data[10] << 6) & 0x07FF)) - 1000) / 4;
			ch_in[7] = ((((data[10] >> 5 | data[11] << 3) & 0x07FF)) - 1000) / 4;
			data[0] = 1;
			data[23] = 1;
			data[24] = 1;
			ch_feed_dog();
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
	ch_watch_dog(2);
	if (ch_in[0]<-200&&ch_in[1]>200&&ch_in[2]>200&&ch_in[3]>200)
	{
		if (fly_flag.unclock == 0)
		{
			delay_ms(1000);
			if (ch_in[0]<-200 && ch_in[1]>200 && ch_in[2]>200 && ch_in[3]>200)
			{
				fly_flag.unclock = 1;
				RGB_State = 1;
				DTInit();
			}
		}
		else {
			delay_ms(1000);
			if (ch_in[0]<-200 && ch_in[1]>200 && ch_in[2]>200 && ch_in[3]>200)
			{
				fly_flag.unclock = 0;
				RGB_State = 2;
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
