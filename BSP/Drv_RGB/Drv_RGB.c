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
//	*Function	 : RGB_Rand
//	*Descriptiton: �����ʾRGB�Ƶ���ɫ
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void RGB_Rand(void)
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
	reset();
}
//------------------------------------------------------------------------------------------
//	*Function	 : RGB_Show
//	*Descriptiton: �ĸ�RGB����ʾͬһ����ɫ
//	*Parameter	 : red ,green , blue RGB255��ʽ����
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void RGB_Show(uint8_t red,uint8_t green, uint8_t blue)
{
	uint8_t i = 4;
	while (i--)
	{
		RGB_Colorset(green, red, blue);
	}
	reset();
}
//------------------------------------------------------------------------------------------
//	*Function	 : RGB_Breath
//	*Descriptiton: ������
//	*Parameter	 : color_mod : ��������ɫ���� 1:�� 2:�� 3:�� 4:�� 5:�� 6:��
//				   T : ����(ms)
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void RGB_Breath(uint8_t color_mod, uint16_t T)
{
	uint8_t change_time = T / 30;//���ȸı�ʱ��
	static uint8_t base_light = 0xff; //��������
	static uint8_t gap_time = 0; //����һ�����ȸı��ʱ��
	static uint8_t flag = 0; //�������ӻ��Ǽ��ٵı�־λ  0:���� 1:����
	gap_time++;
	//�ﵽ�ı�ʱ��
	if (gap_time >= change_time)
	{
		gap_time = 0;
		//���ȼ���
		if (flag == 0)
		{
			base_light -= 0x11;
			//�����ȴﵽ��Сʱ���ı�flag
			if (base_light <= 0x11)
				flag = 1;
		}
		//��������
		else {
			base_light += 0x11;
			//�����ȴﵽ���ʱ���ı�flag
			if (base_light >= 0xff)
				flag = 0;
		}
		//�ı�����
		switch (color_mod)
		{
			//��ɫ
		case 1:
			RGB_Show(base_light, 0, 0);
			break;
			//��ɫ
		case 2:
			RGB_Show(0, base_light, 0);
			break;
			//��ɫ
		case 3:
			RGB_Show(0, 0, base_light);
			break;
			//��ɫ=��ɫ+��ɫ
		case 4:
			RGB_Show(base_light, base_light, 0);
			break;
			//��ɫ=��ɫ+��ɫ
		case 5:
			RGB_Show(base_light, 0, base_light);
			break;
			//��ɫ=��ɫ+��ɫ
		case 6:
			RGB_Show(0, base_light, base_light);
			break;
		}
	}
}
//------------------------------------------------------------------------------------------
//	*Function	 : RGB_Breath
//	*Descriptiton: ������
//	*Parameter	 : color_mod : ��������ɫ���� 1:�� 2:�� 3:�� 4:�� 5:�� 6:��
//				   T : ����(ms)
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void RGB_flash(uint8_t color_mod, uint16_t turn_on_time, uint16_t turn_off_time, uint16_t nums_in_group, uint16_t time_of_groups)
{
	static uint8_t base_light = 0xff;
	static uint16_t current_time = 0;
	static uint8_t nums = 0;
	current_time++;
	if (nums <= nums_in_group)
	{
		if (current_time <= turn_on_time)
			base_light = 0xff;
		else if (current_time <= turn_off_time+turn_on_time)
			base_light = 0;
		else
			nums++, current_time = 0;
	}
	else if (current_time >= time_of_groups)
		nums = 0, current_time = 0;
	switch (color_mod)
	{
		//��ɫ
	case 1:
		RGB_Show(base_light, 0, 0);
		break;
		//��ɫ
	case 2:
		RGB_Show(0, base_light, 0);
		break;
		//��ɫ
	case 3:
		RGB_Show(0, 0, base_light);
		break;
		//��ɫ=��ɫ+��ɫ
	case 4:
		RGB_Show(base_light, base_light, 0);
		break;
		//��ɫ=��ɫ+��ɫ
	case 5:
		RGB_Show(base_light, 0, base_light);
		break;
		//��ɫ=��ɫ+��ɫ
	case 6:
		RGB_Show(0, base_light, base_light);
		break;
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
		RGB_flash(3, 100, 100, 5, 500);
		break;
	case 1:
		RGB_Breath(1, 500);
		break;
	case 2:
		RGB_Breath(4, 400);
		break;
	case 3:
		RGB_Breath(5, 400);
		break;
	case 4:
		RGB_Breath(6, 400);
		break;
	default :
		break;
	}
}
//------------------------------------------------------------------------------------------
