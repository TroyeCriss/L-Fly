//------------------------------------------------------------------------------------------
//	*copyright		: L
//	*FileName		: Drv_time
//	*Author			: Cosmos
//	*Version		: V1.0
//	*Date			: 2018.9.2
//	*Description	: ϵͳʱ�ӳ�ʼ����
//					  ��ȡ��ǰϵͳִ��ʱ�䡣
//					  ���ø߾���us��ms�����ӳ�
//	*Others			:
//	*Fun List		:
//	*History		:
//------------------------------------------------------------------------------------------
//Includes
//------------------------------------------------------------------------------------------
#include "./BSP/Drv_time/Drv_time.h"
//------------------------------------------------------------------------------------------
volatile uint32_t sysTickUptime = 0;
//------------------------------------------------------------------------------------------
//	*Function	 : SysTick_Configuration
//	*Descriptiton: �δ�ʱ����ʼ��
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void SysTick_Configuration()
{
	if (SysTick_Config(SystemCoreClock / 1000))
	{
		while (1);
	}
}
//------------------------------------------------------------------------------------------
//	*Function	 : SysTick_Handler
//	*Descriptiton: ��ʱ���жϣ�ÿ1ms����һ�Σ�ms��ʱ������1
//	*Parameter	 : None
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void SysTick_Handler(void)
{
	sysTickUptime++;
}
//------------------------------------------------------------------------------------------
//	*Function	 : SysTick_GetTick
//	*Descriptiton: ��ȡ��ǰϵͳִ��ʱ��(��λms)
//	*Parameter	 : None
//	*Return		 : ��ǰϵͳʱ��(��λms)
//	*Others		 :
//------------------------------------------------------------------------------------------
uint32_t SysTick_GetTick(void)
{
	return sysTickUptime;
}
//------------------------------------------------------------------------------------------
//	*Function	 : GetSysTime_us
//	*Descriptiton: ��ȡ��ǰϵͳִ��ʱ��(��λus)
//	*Parameter	 : None
//	*Return		 : ��ǰϵͳʱ��(��λus)
//	*Others		 :
//------------------------------------------------------------------------------------------
uint64_t GetSysTime_us(void)
{
	volatile uint32_t ms;
	uint32_t value;
	ms = sysTickUptime;
	value = ms*TICK_US+(SysTick->LOAD-SysTick->VAL)*TICK_US/SysTick->LOAD;
	return value;
}
//------------------------------------------------------------------------------------------
//	*Function	 : delay_us
//	*Descriptiton: us�߾�����ʱ
//	*Parameter	 : us:������ʱʱ��(1=1us)
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void delay_us(uint32_t us)
{
	uint64_t now = GetSysTime_us();
	while ((GetSysTime_us() - now)< us);
}
//------------------------------------------------------------------------------------------
//	*Function	 : delay_ms
//	*Descriptiton: ms�߾�����ʱ
//	*Parameter	 : ms:������ʱʱ��(1=1ms)
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
void delay_ms(uint32_t ms)
{
	delay_us(ms * 1000);
}
//------------------------------------------------------------------------------------------
//	*Function	 : delay_ms
//	*Descriptiton: ms�߾�����ʱ
//	*Parameter	 : ms:������ʱʱ��(1=1ms)
//	*Return		 : None
//	*Others		 :
//------------------------------------------------------------------------------------------
uint32_t Get_Time(void)
{
	return sysTickUptime;
}
//------------------------------------------------------------------------------------------
void HSI_SetSysClock(uint32_t m, uint32_t n, uint32_t p, uint32_t q)
{
	__IO uint32_t HSIStartUpStatus = 0;
	// �� RCC �����ʼ���ɸ�λ״̬
	RCC_DeInit();
	//ʹ�� HSI, HSI=16M
	RCC_HSICmd(ENABLE);
	// �ȴ� HSI ����
	HSIStartUpStatus = RCC->CR & RCC_CR_HSIRDY;
	// ֻ�� HSI ����֮�����������ִ��
	if (HSIStartUpStatus == RCC_CR_HSIRDY) {
		// ��ѹ����ѹ�����������Ϊ 1���Ա�������Ϊ���Ƶ��
		// ����ʱʹ���ܺ͹���ʵ��ƽ��
		RCC->APB1ENR |= RCC_APB1ENR_PWREN;
		PWR->CR |= PWR_CR_VOS;
		// HCLK = SYSCLK / 1
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		// PCLK2 = HCLK / 2
		RCC_PCLK2Config(RCC_HCLK_Div2);
		// PCLK1 = HCLK / 4
		RCC_PCLK1Config(RCC_HCLK_Div4);
		// ���Ҫ��Ƶ�͵�������������
		// ���� PLL ��Դʱ�ӣ����� VCO ��Ƶ���� m������ VCO ��Ƶ���� n��
		// ����ϵͳʱ�ӷ�Ƶ���� p������ OTG FS,SDIO,RNG ��Ƶ���� q
		RCC_PLLConfig(RCC_PLLSource_HSI, m, n, p, q);
		// ʹ�� PLL
		RCC_PLLCmd(ENABLE);
		// �ȴ� PLL �ȶ�
		while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {
		}
		/*-----------------------------------------------------*/
		// ���� FLASH Ԥȡָ,ָ���,���ݻ���͵ȴ�״̬
		FLASH->ACR = FLASH_ACR_PRFTEN
			| FLASH_ACR_ICEN
			| FLASH_ACR_DCEN
			| FLASH_ACR_LATENCY_5WS;
		/*-----------------------------------------------------*/
		// �� PLL �ȶ�֮�󣬰� PLL ʱ���л�Ϊϵͳʱ�� SYSCLK
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		// ��ȡʱ���л�״̬λ��ȷ�� PLLCLK ��ѡΪϵͳʱ��
		while (RCC_GetSYSCLKSource() != 0x08) {
		}
	}
	else {
		// HSI ����������
		while (1) {
		}
	}
}
//------------------------------------------------------------------------------------------