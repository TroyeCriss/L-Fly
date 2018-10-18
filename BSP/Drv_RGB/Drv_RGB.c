//------------------------------------------------------------------------------------------
//	*copyright		: L	 
//	*FileName		: Drv_LED
//	*Author			: Cosmos
//	*Version		: V1.0
//	*Date			: 2018.9.3
//	*Description	: �������źŵ��Լ��ź�ͨ��Э��
//		
//	*Others			:
//	*Fun List		:
//	*History		:
//------------------------------------------------------------------------------------------
//Includes
//------------------------------------------------------------------------------------------
#include "Drv_RGB/Drv_RGB.h"
#include "APP/Globle_Var/Globle_Var.h"
#include "Drv_time/Drv_time.h"
#include  <stdlib.h>
//------------------------------------------------------------------------------------------
#define RGB_PIN			GPIO_Pin_9
#define RGB_GPIO_PORT	GPIOC
#define RGB_GPIO_CLK	RCC_AHB1Periph_GPIOC
#define RGB_H			RGB_GPIO_PORT->BSRRL=RGB_PIN
#define RGB_L			RGB_GPIO_PORT->BSRRH=RGB_PIN
//------------------------------------------------------------------------------------------
//	*Function	 : LED_Init
//	*Descriptiton: RGB�������ų�ʼ��
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void RGB_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RGB_GPIO_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = RGB_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(RGB_GPIO_PORT, &GPIO_InitStructure);
}
//------------------------------------------------------------------------------------------
//	*Function	 : send_high_bit
//	*Descriptiton: RGBͨ���У������ź�1
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 : �ߵ�ƽ600ns   �͵�ƽ600ns
//------------------------------------------------------------------------------------------
void send_high_bit()
{
	uint8_t i = 14, j = 14;
	RGB_H;
	while (i--)
		__nop();
	RGB_L;
	while (j--)
		__nop();
}
//------------------------------------------------------------------------------------------
//	*Function	 : send_low_bit
//	*Descriptiton: RGBͨ���У������ź�0
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 : �ߵ�ƽ300ns �͵�ƽ900ns
//------------------------------------------------------------------------------------------
void send_low_bit(void)
{
	uint8_t i = 6, j = 20;
	RGB_H;
	while (i--)
		__nop();
	RGB_L;
	while (j--)
		__nop();
}
//------------------------------------------------------------------------------------------
//	*Function	 : reset
//	*Descriptiton: RGBͨ�����ã�����һ��ͨ�Ž�������һ�ο�ʼ
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
static void reset(void)
{
	delay_us(80);
}
//------------------------------------------------------------------------------------------
//	*Function	 : send_one_byte
//	*Descriptiton: ����8λ��Ϣ
//	*Parameter	 : byte: ��Ҫ���͵�8λ��Ϣ
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
static void send_one_byte(uint8_t byte)
{
	uint8_t i = 8;
	while(i--)
	{
		if (byte & 0x80)
			send_high_bit();
		else
			send_low_bit();
		byte <<= 1;
	}
}
//------------------------------------------------------------------------------------------
//	*Function	 : RGB_Colorser
//	*Descriptiton: ����RGB�Ƶ���ɫ
//	*Parameter	 : green: ����RGB���У���ɫ������(0-0xFF)
//				   red  : ����RGB���У���ɫ������(0-0xFF)
//				   blue : ����RGB���У���ɫ������(0-0xFF)
//	*Return		 :	
//	*Others		 :
//------------------------------------------------------------------------------------------
void RGB_Colorset(uint8_t green, uint8_t red, uint8_t blue)
{
	send_one_byte(green);
	send_one_byte(red);
	send_one_byte(blue);
}
//------------------------------------------------------------------------------------------
//	*Function	 : 
//	*Descriptiton: 
//	*Parameter	 : 
//				   
//				   
//	*Return		 :	
//	*Others		 :
//------------------------------------------------------------------------------------------
void RGB_Red(void)
{
	uint8_t i = 0;
	uint8_t green, red, blue;
	for (i = 0; i < 4; i++)
	{
		green = rand() % 18 + 2;
		red = rand() % 18 + 2;
		blue = rand() % 18 + 2;
		RGB_Colorset(green,red,blue);
	}
}
void RGB_Show(uint8_t green, uint8_t red, uint8_t blue)
{
	uint8_t i = 4;
	while (i--)
	{
		RGB_Colorset(green, red, blue);
	}
}
//------------------------------------------------------------------------------------------
//	*Function	 : RGB_Infor_Show
//	*Descriptiton: �ɵ�ǰ�ɻ�״̬����ʾ��Ӧ��ͨ���ź�
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void RGB_Infor_Show(void)
{
	switch (RGB_State)
	{
	case 0:
		break;
	case 1:
		break;
	case 3:
		break;
	case 4:
		break;
	default :
		break;
	}
}
//------------------------------------------------------------------------------------------
