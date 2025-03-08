/////////////////////////////////////////////////////////
//  Real Time OS for PIC16F87x & PIC18 family
//  Support Service Macro
//    Request_Task , Exit_Task
//    Trigger_Task ,
//    Suspend_Task , Resume_Task
//    Wait_Timer   , Get_Trigger
///////////////////////////////////////////////////////
#include "PIC_OS.h"

///////// System Global Variables ////////////
uint8_t Current_Task;
volatile bit Timer_Flag;

uint8_t TCB[2 * task_number + 2];

void OS_Task_Init(void)
{
	uint8_t i;
	for (i = 1; i <= task_number; i++)
	{
		TCB[2 * i] = 0;
		TCB[2 * i + 1] = 0;
	}
}

//////////// Request Task Service Macro ////
void OS_Request_Task(uint8_t taskid)
{
	di();
	if ((TCB[2 * taskid] & 0x80) == 0) // already Ready or Wait
	{
		TCB[2 * taskid] = TCB[2 * taskid] | 0x80; // Ready bit On
	}
	ei();
}

//////////// Trigger Task Service Macro ////
void OS_Trigger_Task(uint8_t taskid)
{
	di();
	if ((TCB[2 * taskid] & 0x80) == 0) // already Ready or Wait
	{
		TCB[2 * taskid] = TCB[2 * taskid] | 0x80; // Ready bit On
	}
	// 只能在中断中使用
}

/////////// Exit Task Service Macro ////
void OS_Exit_Task(void)
{
	di();
	TCB[2 * Current_Task] = 0;	   // Set dormant
	TCB[2 * Current_Task + 1] = 0; // clear parameter
	ei();
}

////////// Suspend Service Macro ////////
/*void OS_Suspend_Task(uint8_t taskid)
{
	di();
	if((TCB[2*Current_Task] & 0x40) == 0x00)	//already wait timer
	{
		TCB[2*Current_Task] = (TCB[2*Current_Task] & 0x80) | 0x40;
		TCB[2*Current_Task+1] = taskid;
	}
	ei();
}

//////// Resume Service Macro ///////
void OS_Resume_Task(void)
{
	int i;

	di();						//not exist check flag
	i = 0;
	while(i <= task_number)
	{
		///// Check wait and match task number  /////
		if((TCB[2*i] & 0x40) && (TCB[2*i+1] == Current_Task))
		{
			//TCB[2*i] = (TCB[2*i] & 0xA0) + 1;	//wake up and trigger flag set
			TCB[2*i] = TCB[2*i] & 0xA0;
			TCB[2*i+1] = 0;
			break;
		}
		else
			i++;							//next task
	}
	ei();
}

//////// Get Event Service Macro ///////
uint8_t OS_Get_Event(uint8_t *Data)
{
	uint8_t Event = 0;

	di();
	if (TCB[2*Current_Task] & 0x10)
	{
		TCB[2*Current_Task] = TCB[2*Current_Task] & 0xe0;
		Event = 1;
		*Data = TCB[2*Current_Task+1];
		TCB[2*Current_Task+1] = 0;
	}
	ei();
	return Event;
}

///// Send Event Service Macro ///////
void OS_Send_Event(uint8_t taskid,uint8_t Data)
{
	di();
	if((TCB[2*taskid] & 0x40) == 0)
	{
		TCB[2*taskid] = (TCB[2*taskid]&0x80)|0x10;
		TCB[2*taskid+1] = Data;
	}
	ei();
}


///////// Get Trigger Condition Service Macro ////////
uint8_t OS_Get_Trigger(void)
{
	return(TCB[2*Current_Task] & 0x0f);		//Trigger flag
}
*/

//////// Wait Timer Service Macro /////
void OS_Wait_Timer(uint8_t time)
{
	di();
	if ((TCB[2 * Current_Task] & 0x50) == 0)
	{
		TCB[2 * Current_Task] = (TCB[2 * Current_Task] & 0xd0) | 0x20;
		TCB[2 * Current_Task + 1] = time;
		Timer_Flag = 1;
	}
	ei();
}

////////// Timer update 需要放入节拍器中////////
void OS_Timer_Update(void)
{
	uint8_t i;

	if (Timer_Flag != 0) // no timer wait task
	{
		Timer_Flag = 0; // reset wait flag
		for (i = 1; i <= task_number; i++)
		{
			if (TCB[2 * i] & 0x20) // duling timer wait
			{
				TCB[2 * i + 1] = TCB[2 * i + 1] - 1; // wait counter -1
				if (TCB[2 * i + 1] == 0)			 // if time up,set ready
					// TCB[2*i] = (TCB[2*i] & 0xDD) +2;	//set trigger flag
					TCB[2 * i] = TCB[2 * i] & 0xd0; // set trigger flag
				else
					Timer_Flag = 1; // exist then set flag
			}
		}
	}
}
