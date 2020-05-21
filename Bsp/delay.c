#include "delay.h"

#include "rs485.h"
//#include <string.h>
//#include <intrins.h>

uint16_t DelayTimes_Conut = 0;

void Timer1_Init(void)//1ms �Զ���װ��
{	
	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0x66;		//���ö�ʱ��ֵ
	TH1 = 0xFC;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	ET1 = 1;
	TR1 = 1;		//��ʱ��1��ʼ��ʱ	
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
