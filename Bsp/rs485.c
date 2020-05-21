#include "rs485.h"
#include "delay.h"
#include "IO.h"
#include "relay.h"
#include "STC8A8K64S4A12.h"
#include <string.h>
#include <stdio.h>
#include <intrins.h>
#include "MyConfig.h"


uint8_t Rx1_Buffer[20] = {0};      //串口中断接收数据的Buffer
uint8_t Rx1_Cnt = 0;               //串口中断接收数据的数量
//uint8_t Rx2_Buffer[20] = {0};      //串口中断接收数据的Buffer
//uint8_t Rx2_Cnt = 0;               //串口中断接收数据的数量

uint8_t Bus2SendOK = 0;
//uint8_t AcktiveFlag = 0;

uint8_t  PFKDevState = Offline;
uint16_t PFKDevStateTimerCount = 0;
uint8_t RS485Bus2Busy = 0;
uint8_t RS485Bus2DelayCont = 0;

uint8_t Count = 0;
uint8_t DataTemp = 0;



/******************************************************************************
 函数: void RS485_Init(void)
 描述: RS485_usart初始化函数
 参数: 无
 返回: 无
******************************************************************************/
void RS485_Init(void)
{ 
  GPIO_InitTypeDef	GPIO_InitStructure;	
	
  //Bus1--RXD2--P1.0
	GPIO_InitStructure.Port = GPIO_1;
	GPIO_InitStructure.Pin = GPIO_Pin_0;
	GPIO_InitStructure.Mode = GPIO_PullUp;
	GPIO_Init(&GPIO_InitStructure);
	//Bus1--TXD2--P1.1
	GPIO_InitStructure.Port = GPIO_1;
	GPIO_InitStructure.Pin = GPIO_Pin_1;
	GPIO_InitStructure.Mode = GPIO_PullUp;
	GPIO_Init(&GPIO_InitStructure);	
		
	P_SW2 &= ~0x01; //串口2使用引脚 RXD2--P1.0  TXD2--P1.1
	S2CON = 0x50;
	

	
  //Bus2--RXD3--P0.0
	GPIO_InitStructure.Port = GPIO_0;
	GPIO_InitStructure.Pin = GPIO_Pin_0;
	GPIO_InitStructure.Mode = GPIO_PullUp;
	GPIO_Init(&GPIO_InitStructure);
	//Bus2--TXD3--P0.1
	GPIO_InitStructure.Port = GPIO_0;
	GPIO_InitStructure.Pin = GPIO_Pin_1;
	GPIO_InitStructure.Mode = GPIO_PullUp;
	GPIO_Init(&GPIO_InitStructure);	

	P_SW2 &= ~0x02 ;//串口3使用引脚 RXD3--P0.0	TXD3--P0.1
	S3CON |= 0x10; 	
	S3CON &= 0x30;		//8位数据,可变波特率,串口3选择定时器2为波特率发生器	



	AUXR &= 0xF7;     //#xxxx 0xxx  定时器T2用作定时器功能；	
	AUXR |= 0x04;     // #xxxx x1xx   定时器T2不分频，即工作在1T模式；	                                 
	AUXR &= 0xF7;     //#xxxx 0xxx  定时器T2用作定时器功能；

	T2L = 0xE0;		//设定定时初值
	T2H = 0xFE;		//设定定时初值
	
	AUXR |= 0x10;     //启动定时器T2



	IE2 |= 0x01;     // 串口2中断打开
	IE2 |= 0x08;     // 串口3中断打开
	EA = 1;	 	
}


//Bus1--UART2--9600
void UART2(void) interrupt 8
{ 
	if(S2CON & 0x01)
	{
		S2CON &= ~0x01;						                         //清零接收标志位	
		
		Rx1_Buffer[Rx1_Cnt++] = S2BUF;				            	//读取缓冲区的数据
   		
		if(Rx1_Buffer[0] == 0xad)
		{
			if(Rx1_Buffer[1] == 0xda)
			{
				if(Rx1_Cnt == Rx1_Buffer[2]+4)	
				{	
					IE2 &= ~0x01;	//"关闭"串口2接收中断，进行数据处理
          RS485ReceiveDataParase(Rx1_Buffer,Rx1_Cnt);	
					IE2 |= 0x01;	//"打开"串口2接收中断，准备接收数据				
					Rx1_Cnt = 0;				
				}
				if(Rx1_Cnt > (Rx1_Buffer[2]+4))
				{
					Rx1_Cnt = 0;						
				}				
			}
			else if(Rx1_Cnt > 1)
			{
				Rx1_Cnt = 0;				
			}
		}
		else
		{
			Rx1_Cnt = 0;
		}			
	}
}





void RS485Bus1SendDataPacket(uint8_t *DataBuffer,uint8_t DataNum)
{
	uint8_t Cnt;
  
	for(Cnt =0;Cnt < DataNum;Cnt++)
	{	
     S2BUF = DataBuffer[Cnt]; 				       	//发送数据
     while(!(S2CON & 0x02)); 					        //等待发送完成
     S2CON &= ~0x02; 							          //清零发送标志位
  }
}

void RS485Bus2SendDataPacket(uint8_t *DataBuffer,uint8_t DataNum)
{
	uint8_t Cnt;
	
	for(Cnt =0;Cnt < DataNum;Cnt++)
	{
     IE2 &= ~0x08;		
     S3BUF = DataBuffer[Cnt]; 				       	//发送数据
     while(!(S3CON & 0x02)); 					        //等待发送完成
     S3CON &= ~0x02; 							          //清零发送标志位
		 IE2 |= 0x08;
  }
}


//ad da num cmd (addr) crc

uint8_t RS485ReceiveDataParase(uint8_t *DataBuffer,uint8_t Num)
{
  if((Calculate_DataPackect_CRCValue(DataBuffer,Num - 1) != DataBuffer[Num - 1]))
		return 0;


	/*对于双总线模块而言，只要Bus1的RS485口收到CRC校验通过的数据，则表明模块在线；
	  对于单总线模块而言，只有RS485口收到符合自己Addr的CRC校验通过的查询数据，因为单总线需要网关不停发查询指令，才能知道单总线模块的状态，才能说明自己在线.*/
		PFKDevState = Online;
		PFKDevStateTimerCount = 15000;//15s内如果没有收到心跳则进入离线模式			
	
	

	if(DataBuffer[3] == ModuleAddr)
	{
		//ad da len addr cmd crc
	  //ad da  02 addr 20  crc
		if(DataBuffer[4] == 0x20)//query_state
		{
			Get_Device_State();			 
			                              //Cmd = 0x20
			RS485RecviceDataPacketRespond(DataBuffer[4]);		
		}
		//ad da len addr cmd channel state crc
		//ad da 04   00   60    01     03  83
		else if(DataBuffer[4] == 0x60)//ctrl
		{
			if(DataBuffer[5] == 0x01)//Channel
			{
				if(RS485CmdControlRelay(DataBuffer[6]))//State			
				{                              //Cmd = 0x60
					RS485RecviceDataPacketRespond(DataBuffer[4]);						
				}						
			}
		}	
	}
	else if((DataBuffer[3] == 0xff) && (DataBuffer[4] == 0xf0))
	{
    RS485RecviceDataPacketRespond(DataBuffer[4]);			
	}
	else 
	{
	  return 0;
	}
  return 1;
}



//0x20-------ad da len addr cmd type io_h io_l relay crc
           //AD DA  06  00   20  A2   00   08   05   1F  
//0x60-------ad da len addr  cmd type channle  state crc
           //AD DA  05  00   60   A2    02      01   E6 
//0xf0-------ad da len  addr cmd type crc
          // AD DA  03  00   f0  A2  19 
void RS485RecviceDataPacketRespond(uint8_t Cmd)
{
  uint8_t Buffer[10] = {0},Num = 0;
	
	Buffer[Num++] = 0xad;//0 - 1
	Buffer[Num++] = 0xda;//1 - 2
	Buffer[Num++] = 0x03;//2 - 3
	
	if(Cmd == 0x20)
	{					
		Buffer[Num++] = ModuleAddr;//addr 3 - 4		
		Buffer[Num++] = Cmd;//cmd 4 - 5
		Buffer[Num++] = ModuleType;// 5 - 6			
		Buffer[Num++] = ModuleStatus[1];// 6 - 7
		Buffer[Num++] = ModuleStatus[0];// 6 - 7
		Buffer[Num++] = CurtainStatus;// 6 - 7				
	}	
	//ad da 04 00 60 01 03 83	
	else if(Cmd == 0x60)
	{					
		Buffer[Num++] = ModuleAddr;//addr 3 - 4		
		Buffer[Num++] = Cmd;//cmd 4 - 5
		Buffer[Num++] = ModuleType;// 5 - 6				
		Buffer[Num++] = 0x01;// Channel6 - 7
		Buffer[Num++] = CurtainStatus;// 6 - 7			
	}
	else
	{		
		Buffer[Num++] = ModuleAddr;//addr 3 - 4	
		Buffer[Num++] = 0xff;//cmd 4 - 5
		Buffer[Num++] = 0xf0;//cmd 4 - 5		
		Buffer[Num++] = ModuleType;// 5 - 6	
	}	
		
	Buffer[2] = Num - 3;
	Buffer[Num] = Calculate_DataPackect_CRCValue(Buffer,Num);
	Num++;
  RS485Bus1SendDataPacket(Buffer,Num);
}

//0x02--ad da len type addr cmd channel state crc
//      ad da len addr cmd type channle  state crc
void OnlineInputGPIOTriggeredReport(uint8_t Channel,uint8_t State)
{
  uint8_t ReportBuffer[10] = {0},ReportNum = 0;	
	
	ReportBuffer[ReportNum++] = 0xad;//0 - 1
	ReportBuffer[ReportNum++] = 0xda;//1 - 2
	ReportBuffer[ReportNum++] = 0x03;//2 - 3
	ReportBuffer[ReportNum++] = ModuleAddr;//addr 3 - 4	
	ReportBuffer[ReportNum++] = 0x02;//cmd 4 - 5
	ReportBuffer[ReportNum++] = ModuleType;// 5 - 6		
	ReportBuffer[ReportNum++] = Channel + 1;// 5 - 6
	ReportBuffer[ReportNum++] = State;// 6 - 7	
	ReportBuffer[2] = ReportNum - 3;
	ReportBuffer[ReportNum] = CRC8_Check(ReportNum,ReportBuffer);
	ReportNum++;
	
  Bus2SendOK = 0;	
  RS485Bus2Busy = 0;	
//	TL0 = 0xCD;		//设置定时初值
//	TH0 = 0xF8;		//设置定时初值
	TL0 = 0xA4;		//设置定时初值100us
	TH0 = 0xFF;		//设置定时初值	
	TR0 = 1;	
	while(!Bus2SendOK);		
	RS485Bus2SendDataPacket(ReportBuffer,ReportNum);
//	AcktiveFlag = 1;
}


void Uart3Isr() interrupt 17
{
	if (S3CON & 0x01)
	{
		S3CON &= ~0x01;
		RS485Bus2Busy = 1;			
	}
}


void Timer0_Init(void)//2ms 12T 自动重装载
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
//	TL0 = 0xCD;		//设置定时初值2ms
//	TH0 = 0xF8;		//设置定时初值

	TL0 = 0xA4;		//设置定时初值100us
	TH0 = 0xFF;		//设置定时初值

	
	ET0 = 1;
	TR0 = 0;		//定时器0开始计时	stop
	EA = 1;	
}

uint8_t CoutiunsFlag = 0;
uint8_t RS485Bus2FreeCont = 0;
void Timer0() interrupt 1
{
	TR0 = 0;		
	
  if(RS485Bus2Busy == 0)
	{
	  if(RS485Bus2FreeCont > 30)
		{
			RS485Bus2FreeCont = 0;		
      Bus2SendOK = 1; 				
		}
		else
		{
		  RS485Bus2FreeCont++;			
//			TL0 = 0xCD;		//设置定时初值 2ms
//			TH0 = 0xF8;		//设置定时初值
				TL0 = 0xA4;		//设置定时初值100us
				TH0 = 0xFF;		//设置定时初值			
			TR0 = 1;	
			RS485Bus2Busy = 0;						
		}
	}
	else
	{
		  RS485Bus2FreeCont = 0;			
//			TL0 = 0xCD;		//设置定时初值 2ms
//			TH0 = 0xF8;		//设置定时初值
				TL0 = 0xA4;		//设置定时初值100us
				TH0 = 0xFF;		//设置定时初值		
			TR0 = 1;	
			RS485Bus2Busy = 0;		
	}
	
}



uint8_t Calculate_DataPackect_CRCValue(uint8_t *DataPacket,uint8_t Len)
{
    uint8_t num_i = 0,num_j = 0,crc = 0,middle_byte = 0;
    for(num_i = 0 ; num_i < Len-3; num_i++)
    {
        middle_byte = DataPacket[num_i+3]; 
        for( num_j = 0 ; num_j < 8; num_j++)
			  {
            if(((crc^middle_byte)&0x01) == 0 )
						{
                crc >>=1;
            }
            else
						{
                crc^= 0x18;
                crc >>=1;
                crc |=0x80;
            }
            middle_byte >>=1;
        }
    }
    return crc ;
}

uint8_t CRC8_Check(uint8_t len,const uint8_t *buf)
{
    uint8_t num_i = 0,num_j = 0,crc = 0,middle_byte = 0;
    for(num_i = 0 ; num_i < len-3; num_i++)
    {
        middle_byte = buf[num_i+3]; 
        for( num_j = 0 ; num_j < 8; num_j++)
			  {
            if(((crc^middle_byte)&0x01) == 0 )
						{
                crc >>=1;
            }
            else
						{
                crc^= 0x18;
                crc >>=1;
                crc |=0x80;
            }
            middle_byte >>=1;
        }
    }
    return crc ;
}










/**********************************THE END**********************************/ 

 