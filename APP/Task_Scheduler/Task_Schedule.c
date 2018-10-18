//------------------------------------------------------------------------------------------
//	*copyright		: L
//	*FileName		: Task_Schedule
//	*Author			: Cosmos
//	*Version		: V1.0
//	*Date			: 2018.9.2
//	*Description	: ��ʼ���߳�����ִ���������
//	
//	*Others			:	
//	*Fun List		:
//	*History		:
//------------------------------------------------------------------------------------------
//Includes
//------------------------------------------------------------------------------------------
#include <stdio.h>
#include "Task_Scheduler/Task_Schedule.h"
#include "Drv_time/Drv_time.h"
#include "Drv_RGB/Drv_RGB.h"
#include "Drv_AT9/Drv_AT9.h"
#include "APP/Globle_Var/Globle_Var.h"
#include "Drv_MPU/Drv_MPU.h"
#include "APP/Control/Control.h"
#include "Drv_RGB/Drv_RGB.h"
#include "Drv_SPL06/Drv_SPL06.h"
//------------------------------------------------------------------------------------------
//	*Function	 :	Scheduler_Run()
//	*Descriptiton:	���ݵδ�ʱ���Ķ�ʱ���ж�ִ���ĸ��߳�����
//	*Parameter	 :	None
//	*Return		 :	None
//	*Others		 :
//------------------------------------------------------------------------------------------
void Scheduler_Run(void)
{
	uint32_t time_1ms = 0,time_2ms=0,time_5ms=0,time_10ms=0;
	uint32_t current_time = SysTick_GetTick();
	//1ms����
	if (current_time - time_1ms > 1)
	{
		time_1ms = current_time;
		RGB_Red();
	}
	//2ms����
	else if(current_time - time_2ms > 2)
	{
		time_2ms = current_time;
		AC_Data_Process();		//���ң��������
	}
	//5ms����
	else if (current_time - time_5ms > 5)
	{
		time_5ms = current_time;
	}
	//10ms����
	else if (current_time - time_10ms > 10)
	{
		time_10ms = current_time;
	}
	//for (index = 0; index < 8; index++)
	//{
	//	////��ȡϵͳ��ǰʱ�䣬��λMS
	//	//uint32_t tnow = SysTick_GetTick();
	//	///*�����жϣ������ǰʱ���ȥ��һ��ִ�е�ʱ�䣬���ڵ��ڸ��̵߳�ִ�����ڣ���ִ���߳�*/
	//	//if (tnow - sched_tasks[index].last_run >= sched_tasks[index].interval_ticks)
	//	//{

	//	//	//�����̵߳�ִ��ʱ�䣬������һ���ж�
	//	//	sched_tasks[index].last_run = tnow;
	//	//	//ִ���̺߳�����ʹ�õ��Ǻ���ָ��
	//	//	sched_tasks[index].task_func();
	//	//}
	//}


}
//------------------------------------------------------------------------------------------

