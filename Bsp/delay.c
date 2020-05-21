#include "delay.h"

#include "rs485.h"
//#include <string.h>
//#include <intrins.h>

uint16_t DelayTimes_Conut = 0;

void Timer1_Init(void)//1ms 自动重装载
{	
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x66;		//设置定时初值
	TH1 = 0xFC;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	ET1 = 1;
	TR1 = 1;		//定时器1开始计时	
	EA = 1;	
}

void Delay_ms(uint16_t Times)
{
  DelayTimes_Conut = Times;
	while(DelayTimes_Conut);
}

void Timer1() interrupt 3
{
  if(PFKDevStateTimerCount)
	{
	  PFKDevStateTimerCount--;
	}
	else
	{
	  PFKDevState = Offline;
	}
	
	if(DelayTimes_Conut)
	{
	  DelayTimes_Conut--;
	}
}
	
//void Delay1ms(uint16_t Num)		//@11.0592MHz
//{
//	uint16_t k = 0;
//	unsigned char i, j;	
//	for(k = 0;k<Num;k++)
//	{
//		_nop_();
//		_nop_();
//		_nop_();
//		i = 11;
//		j = 190;
//		do
//		{
//			while (--j);
//		} while (--i);	
//	}
//}
