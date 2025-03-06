#ifndef _PIC_OS_H

#include "Main.h"

///// Define Command for Task Dispatch Table /////
#define OS_Dispatch(i,task)	case i :{ ei(); task; break;}

extern uint8_t  TCB[2*task_number+2];

extern uint8_t	Current_Task;


void OS_Task_Init(void);
void OS_Request_Task(uint8_t taskid);
void OS_Trigger_Task(uint8_t taskid);
void OS_Exit_Task(void);
//void OS_Suspend_Task(uint8_t taskid);
//void OS_Resume_Task(void);
//uint8_t OS_Get_Trigger(void);
//uint8_t OS_Get_Event(uint8_t *Data);
//void OS_Send_Event(uint8_t taskid,uint8_t Data);
void OS_Wait_Timer(uint8_t time);
void OS_Timer_Update(void);

#endif