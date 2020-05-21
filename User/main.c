#include "STC8A8K64S4A12.h"
#include "io.h"
#include "rs485.h"
#include "delay.h"
#include "relay.h"
#include "watchdog.h"
#include "MyConfig.h"

//Timer0---RS485Busy

//Timer1---进行精确延时ms
//Timer2---串口波特率

//Timer3---串口数据检查，接收和发送缓存；

void main(void)
{
  /*GPIO初始化*/	
	RS485AddrGPIOInit();
	InputGPIOEnableInit();
	InputGPIOInit();
	RelayGPIOInit();

	/*同步状态*/
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

