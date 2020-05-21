#include "watchdog.h"


void WatchDog_Init(void)
{
   WDT_CONTR = 0x3e;      //#0011 1110  启动看门狗
	                        //            看门狗清零位，当设置为1时，看门狗将重新计数，同时硬件立即将此位清零
                          //            看门狗IDLE空闲模式位，当设置为1时，WDT在空闲模式计数
	                        //            看门狗分频系数,[PS2 PS1 PS0]=110,分频系数为128
	                        //            看门狗溢出时间 = (12*分频系数*32768)/时钟频率
	                        //            分频系数 = 128，时钟频率 = 22.1184MHz ，则看门狗溢出时间=2.275s
}       

void WatchDog_Feed(void)
{
   WDT_CONTR = 0x3e;       //喂狗，同时复位WDT
}