//------------------------------------------------------------------------------------------
//	*copyright		: L
//	*FileName		: Drv_uart
//	*Author			: Cosmos
//	*Version		: V1.0
//	*Date			: 2018.9.16	 
//	*Description	: uart����Ӳ����Դ
//
//	*Others			:
//	*Fun List		:
//	*History		:
//------------------------------------------------------------------------------------------
//Includes
//------------------------------------------------------------------------------------------
#include "Drv_uart/Drv_uart.h"
#include <stdio.h>
//���Ŷ���
/*******************************************************/
#define USARTx USART1
/* ��ͬ�Ĵ��ڹ��ص����߲�һ����ʱ��ʹ�ܺ���Ҳ��һ������ֲʱҪע��
* ���� 1 �� 6 �� RCC_APB2PeriphClockCmd
* ���� 2/3/4/5/7 �� RCC_APB1PeriphClockCmd
*/
#define USARTx_CLK				RCC_APB2Periph_USART1
#define USARTx_CLOCKCMD			RCC_APB2PeriphClockCmd
#define USARTx_BAUDRATE			100000 //���ڲ�����

#define USARTx_RX_GPIO_PORT		GPIOA
#define USARTx_RX_GPIO_CLK		RCC_AHB1Periph_GPIOA
#define USARTx_RX_PIN			GPIO_Pin_10
#define USARTx_RX_AF			GPIO_AF_USART1
#define USARTx_RX_SOURCE		GPIO_PinSource10

#define USARTx_TX_GPIO_PORT		GPIOA
#define USARTx_TX_GPIO_CLK		RCC_AHB1Periph_GPIOA
#define USARTx_TX_PIN			GPIO_Pin_9
#define USARTx_TX_AF			GPIO_AF_USART1
#define USARTx_TX_SOURCE		GPIO_PinSource9
//------------------------------------------------------------------------------------------
//	*Function	 : USART_init
//	*Descriptiton: UART��ʼ��
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 : ��λ��ͨ�Ŵ���
//------------------------------------------------------------------------------------------
void USART_init(uint32_t baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;   //����GPIO�ṹ�����
	USART_InitTypeDef USART_InitStructure;   //���崮�ڽṹ�����

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);   //ʹ��GPIOC��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);   //ʹ��USART3��ʱ��

															 //����3��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3); //GPIOC10����ΪUSART3_tx
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3); //GPIOC11����ΪUSART3_rx

															   //USART3�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOC10��GPIOC11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ��PC10��PC11

										   //UART4 ��ʼ������
	USART_InitStructure.USART_BaudRate = baudrate;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure); //��ʼ������3

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);		//���ڽ����ж�
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);		//���ڿ����ж�

	USART_Cmd(USART3, ENABLE);   //ʹ��USART3
}
//------------------------------------------------------------------------------------------
//	*Function	 : USART_Config
//	*Descriptiton: UART��ʼ��
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/*����ʱ��ʹ��*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA ,ENABLE);
	/*USARTʱ��ʹ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* ���� PXx �� USARTx_Tx*/
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	/* ���� PXx �� USARTx__Rx*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);

	/* GPIO ��ʼ�� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	/* ���� Tx ����Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* ���� Rx ����Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ���ô� DEBUG_USART ģʽ */
	/* ���������ã� DEBUG_USART_BAUDRATE */
	USART_InitStructure.USART_BaudRate =115200;
	/* �ֳ�(����λ+У��λ)�� 8 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	/* ֹͣλ�� 1 ��ֹͣλ */
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	/* У��λѡ��żУ�� */
	USART_InitStructure.USART_Parity = USART_Parity_No;
	/* Ӳ�������ƣ���ʹ��Ӳ���� */
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
	/* USART ģʽ���ƣ�ͬʱʹ�ܽ��պͷ��� */
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	/* ��� USART ��ʼ������ */
	USART_Init(USART1, &USART_InitStructure);
	/* ʹ�ܴ��� */
	USART_Cmd(USART1, ENABLE);
}
//------------------------------------------------------------------------------------------
//	*Function	 : fputc
//	*Descriptiton: �ض��� c �⺯�� printf �����ڣ��ض�����ʹ�� printf ����
//	*Parameter	 : 
//	*Return		 : 
//	*Others		 :
//------------------------------------------------------------------------------------------
//�ض��� c �⺯�� printf �����ڣ��ض�����ʹ�� printf ����
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ����� */
	USART_SendData(USART1, (uint8_t)ch);
	/* �ȴ�������� */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	return (ch);
}
//�ض��� c �⺯�� scanf �����ڣ���д����ʹ�� scanf�� getchar �Ⱥ���
int fgetc(FILE *f)
{
	/* �ȴ������������� */
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	return (int)USART_ReceiveData(USART1);
}
//------------------------------------------------------------------------------------------
