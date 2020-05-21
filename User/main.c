#include "STC8A8K64S4A12.h"
#include "io.h"
#include "rs485.h"
#include "delay.h"
#include "relay.h"
#include "watchdog.h"
#include "MyConfig.h"

//Timer0---RS485Busy

//Timer1---���о�ȷ��ʱms
//Timer2---���ڲ�����

//Timer3---�������ݼ�飬���պͷ��ͻ��棻

void main(void)
{
  /*GPIO��ʼ��*/	
	RS485AddrGPIOInit();
	InputGPIOEnableInit();
	InputGPIOInit();
	RelayGPIOInit();

	/*ͬ��״̬*/
  Synchronization_InputGPIO_Status();	
	
  Timer1_Init();
  Timer0_Init();		
	RS485_Init();	
	WatchDog_Init();	
	while (1)	
	{
		RS485AddrCheck();
		InputEnableGPIOCheck();
		InputGPIOStateCheck();
		WatchDog_Feed();		
	}
}

