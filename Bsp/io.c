#include "io.h"
#include "delay.h"
#include "relay.h"

#include "rs485.h"
#include <string.h>
#include "MyConfig.h"


GPIO_InitTypeDef RS485AddrGPIO[6] = {0};
GPIO_InitTypeDef InputGPIO[6] = {0};
GPIO_InitTypeDef InputEnableGPIO[6] = {0};


uint8_t RS485AddrBuffer[6] = {0};//0-5--pin
uint8_t InputGPIOStateBuffer[6] = {0};
uint8_t InputGPIOStateBufferLock[6] = {0};
uint8_t InputGPIOEnableBuffer[6] = {0};

uint8_t ModuleAddr = 0;
//uint8_t PFKDevAddr = 0;
uint8_t ModuleStatus[2] = {0};


void RS485AddrGPIOInit(void)
{
    /*RS485Addr1 = P2^0*/
  RS485AddrGPIO[0].Port = GPIO_2;
	RS485AddrGPIO[0].Pin = GPIO_Pin_0;
	RS485AddrGPIO[0].Mode =  GPIO_PullUp; 
  GPIO_Init(&RS485AddrGPIO[0]);	
	
    /*RS485Addr2 = P4^2*/
  RS485AddrGPIO[1].Port = GPIO_4;
	RS485AddrGPIO[1].Pin = GPIO_Pin_2;
	RS485AddrGPIO[1].Mode =  GPIO_PullUp; 
  GPIO_Init(&RS485AddrGPIO[1]);	
	
    /*RS485Addr3 = P2^1*/
  RS485AddrGPIO[2].Port = GPIO_2;
	RS485AddrGPIO[2].Pin = GPIO_Pin_1;
	RS485AddrGPIO[2].Mode =  GPIO_PullUp; 
  GPIO_Init(&RS485AddrGPIO[2]);	
	
    /*RS485Addr4 = P2^2*/
  RS485AddrGPIO[3].Port = GPIO_2;
	RS485AddrGPIO[3].Pin = GPIO_Pin_2;
	RS485AddrGPIO[3].Mode =  GPIO_PullUp; 
  GPIO_Init(&RS485AddrGPIO[3]);	

    /*sbit Addr5 = P2^3*/
  RS485AddrGPIO[4].Port = GPIO_2;
	RS485AddrGPIO[4].Pin = GPIO_Pin_3;
	RS485AddrGPIO[4].Mode =  GPIO_PullUp; 
  GPIO_Init(&RS485AddrGPIO[4]);	

    /*RS485Addr6 = P2^4*/
  RS485AddrGPIO[5].Port = GPIO_2;
	RS485AddrGPIO[5].Pin = GPIO_Pin_4;
	RS485AddrGPIO[5].Mode =  GPIO_PullUp; 
  GPIO_Init(&RS485AddrGPIO[5]);		
}

void InputGPIOInit(void)
{
    /*InputGPIO_1 = P1^2*/
  InputGPIO[5].Port = GPIO_1;
	InputGPIO[5].Pin = GPIO_Pin_2;	
	InputGPIO[5].Mode =  GPIO_PullUp; 
  GPIO_Init(&InputGPIO[5]);		
	
    /*InputGPIO_2 = P1^3*/
	InputGPIO[4].Port = GPIO_1;
	InputGPIO[4].Pin = GPIO_Pin_3;	
	InputGPIO[4].Mode =  GPIO_PullUp; 
 GPIO_Init(&InputGPIO[4]);	
	
    /*InputGPIO_3 = P1^4*/
	InputGPIO[3].Port = GPIO_1;
	InputGPIO[3].Pin = GPIO_Pin_4;	
	InputGPIO[3].Mode =  GPIO_PullUp; 
 GPIO_Init(&InputGPIO[3]);	
	
    /*InputGPIO_4 = P4^4*/
	InputGPIO[2].Port = GPIO_4;
	InputGPIO[2].Pin = GPIO_Pin_4;	
	InputGPIO[2].Mode =  GPIO_PullUp; 
  GPIO_Init(&InputGPIO[2]);	
	
    /*InputGPIO_5 = P1^5*/
	InputGPIO[1].Port = GPIO_1;
	InputGPIO[1].Pin = GPIO_Pin_5;	
	InputGPIO[1].Mode =  GPIO_PullUp; 
  GPIO_Init(&InputGPIO[1]);	
	
    /*InputGPIO_6 = P1^6*/
  InputGPIO[0].Port = GPIO_1;
	InputGPIO[0].Pin = GPIO_Pin_6;	
	InputGPIO[0].Mode =  GPIO_PullUp; 
  GPIO_Init(&InputGPIO[0]);		
}


void InputGPIOEnableInit(void)
{
    /*InputGPIOEnable_1 = P3^7*/
  InputEnableGPIO[0].Port = GPIO_3;
	InputEnableGPIO[0].Pin = GPIO_Pin_7;	
	InputEnableGPIO[0].Mode =  GPIO_PullUp; 
  GPIO_Init(&InputEnableGPIO[0]);			
	
    /*InputGPIOEnable_2 = P3^6*/
  InputEnableGPIO[1].Port = GPIO_3;
	InputEnableGPIO[1].Pin = GPIO_Pin_6;	
	InputEnableGPIO[1].Mode =  GPIO_PullUp; 
  GPIO_Init(&InputEnableGPIO[1]);
	
    /*InputGPIOEnable_3 = P5^1*/
  InputEnableGPIO[2].Port = GPIO_5;
	InputEnableGPIO[2].Pin = GPIO_Pin_1;	
	InputEnableGPIO[2].Mode =  GPIO_PullUp; 
  GPIO_Init(&InputEnableGPIO[2]);	
	
    /*InputGPIOEnable_4 = P5^0*/
  InputEnableGPIO[3].Port = GPIO_5;
	InputEnableGPIO[3].Pin = GPIO_Pin_0;	
	InputEnableGPIO[3].Mode =  GPIO_PullUp; 
  GPIO_Init(&InputEnableGPIO[3]);

    /*InputGPIOEnable_5 = P3^5*/
  InputEnableGPIO[4].Port = GPIO_3;
	InputEnableGPIO[4].Pin = GPIO_Pin_5;	
	InputEnableGPIO[4].Mode =  GPIO_PullUp; 
  GPIO_Init(&InputEnableGPIO[4]);

    /*InputGPIOEnable_6 = P3^4*/
  InputEnableGPIO[5].Port = GPIO_3;
	InputEnableGPIO[5].Pin = GPIO_Pin_4;
	InputEnableGPIO[5].Mode =  GPIO_PullUp; 
  GPIO_Init(&InputEnableGPIO[5]);	
}

void Synchronization_InputGPIO_Status(void)
{
  uint8_t Num = 0;
	for(Num = 0;Num < 6;Num++)
	{	
		InputGPIOEnableBuffer[Num] = GPIO_ReadInputDataBit(InputEnableGPIO[Num].Port,InputEnableGPIO[Num].Pin);	
		
		InputGPIOStateBuffer[Num] = GPIO_ReadInputDataBit(InputGPIO[Num].Port,InputGPIO[Num].Pin);
		
		RS485AddrBuffer[Num] = GPIO_ReadInputDataBit(RS485AddrGPIO[Num].Port,RS485AddrGPIO[Num].Pin);
		if(RS485AddrBuffer[Num])
		{
			ModuleAddr |= (0x01 << Num);  		
		}
		else
		{
			ModuleAddr &= ~(0x01 << Num);		
		}			
	}  
}


void RS485AddrCheck(void)
{
  uint8_t Num = 0;
	for(Num = 0;Num < 6;Num++)
	{	
		RS485AddrBuffer[Num] = GPIO_ReadInputDataBit(RS485AddrGPIO[Num].Port,RS485AddrGPIO[Num].Pin);
		
		if(RS485AddrBuffer[Num])
		{
			ModuleAddr |= (0x01 << Num);   
		}
		else
		{
			ModuleAddr &= ~(0x01 << Num);   
		}		
	}
}

void InputEnableGPIOCheck(void)
{
  uint8_t Num = 0;
	for(Num = 0;Num < 6;Num++)
	{	
		InputGPIOEnableBuffer[Num] = GPIO_ReadInputDataBit(InputEnableGPIO[Num].Port,InputEnableGPIO[Num].Pin);	
	}
}








void InputGPIOStateCheck(void)
{
  uint8_t Num = 0,Tempt[2] = {0};
	for(Num = 0;Num < 6;Num++)
	{	
		Tempt[0] = GPIO_ReadInputDataBit(InputGPIO[Num].Port,InputGPIO[Num].Pin);
		Delay_ms(10);//消抖延时
		Tempt[1] = GPIO_ReadInputDataBit(InputGPIO[Num].Port,InputGPIO[Num].Pin);

		if((Tempt[0] == Tempt[1]) && (InputGPIOStateBuffer[Num] != Tempt[0]))
		{
		  if(PFKDevState == Offline)
			{				
				if((Num < 2) && (InputGPIOEnableBuffer[Num]))//Channel = 0/1 && Enable
				{
					OfflineGPIOControlRelay(Num);
				}
/*********************************************************************/					
			/*以下两行代码用来区分自复位开关和翘板开关，具体延时的值需要经过实际测试确定*/
			/*这个代码本质上是针对自复位开关做出的*/
			/*注解如下：
			  先进行"一定时间"的"延时"，然后再将InputGPIO的状态进行同步，
			  1.如果在这个”延时时间“内，inputGPIO再次被触发，则不进行理会，直接无视(就像自复位开关，按下后返回的时间很短，因此返回的触发不被响应，因而人为造成一次触发)，然后延时结束后进行状态同步；
			  2.如果在这个”延时时间“内，inputGPIO未被被触发，则再次触发，可以被检测到，接着会进行状态同步*/
				Delay_ms(200);
				InputGPIOStateBuffer[Num] = GPIO_ReadInputDataBit(InputGPIO[Num].Port,InputGPIO[Num].Pin);
/*********************************************************************/						
			}
			else
			{
				InputGPIOStateBuffer[Num] = Tempt[0];		
				if(InputGPIOEnableBuffer[Num])
				{					
					OnlineInputGPIOTriggeredReport(Num,Tempt[0]);	
				}																				
			}		
		}	
	}	
}

uint8_t GPIO_ReadInputDataBit(uint8_t GPIOx,uint8_t GPIO_Pin)
{
	uint8_t Tempt = 0;
	
  switch(GPIOx)
	{
	  case GPIO_0:
			Tempt = ((P0 >> GPIO_Pin) & 0x01);
			break;
	  case GPIO_1:
			Tempt = ((P1 >> GPIO_Pin) & 0x01);
			break;
	  case GPIO_2:
			Tempt = ((P2 >> GPIO_Pin) & 0x01);
			break;
	  case GPIO_3:
			Tempt = ((P3 >> GPIO_Pin) & 0x01);
			break;
	  case GPIO_4:
			Tempt = ((P4 >> GPIO_Pin) & 0x01);
			break;
	  case GPIO_5:
			Tempt = ((P5 >> GPIO_Pin) & 0x01);
			break;
		
		default:
			break;	
	}
	return Tempt = (Tempt?0x00:0x01);//此处对读取的IO值Tempt进行取反处理，和板子硬件接线有关
}


void Get_Device_State(void)
{
	
  ModuleStatus[0] = (((InputGPIOStateBuffer[0]==0x01?0x01:0x00))
	                  |((InputGPIOEnableBuffer[0]==0x01?0x01:0x00)<<1)
	                  |((InputGPIOStateBuffer[1]==0x01?0x01:0x00)<<2)
	                  |((InputGPIOEnableBuffer[1]==0x01?0x01:0x00)<<3)
	                  |((InputGPIOStateBuffer[2]==0x01?0x01:0x00)<<4)
	                  |((InputGPIOEnableBuffer[2]==0x01?0x01:0x00)<<5)
	                  |((InputGPIOStateBuffer[3]==0x01?0x01:0x00)<<6)
		                |((InputGPIOEnableBuffer[3]==0x01?0x01:0x00)<<7));	
	
  ModuleStatus[1] = (((InputGPIOStateBuffer[4]==0x01?0x01:0x00))
	                  |((InputGPIOEnableBuffer[4]==0x01?0x01:0x00)<<1)
	                  |((InputGPIOStateBuffer[5]==0x01?0x01:0x00)<<2)
	                  |((InputGPIOEnableBuffer[5]==0x01?0x01:0x00)<<3));
}

uint8_t GPIO_Init(GPIO_InitTypeDef *GPIOx)
{
  if((GPIOx->Port > GPIO_5) || (GPIOx->Pin > 7) ||(GPIOx->Mode > GPIO_OUT_PP))
	{
	  return 0;
	}
	 
	switch(GPIOx->Port)
	{
	  case GPIO_0:
			if(GPIOx->Mode == GPIO_PullUp)//上拉准双向口	
			{
			  P0M1 &= ~GPIOx->Pin,	P0M0 &= ~GPIOx->Pin;	
			}
			else if(GPIOx->Mode == GPIO_HighZ)	//浮空输入	
			{
				P0M1 |=  GPIOx->Pin,	P0M0 &= ~GPIOx->Pin;	
			}
      else if(GPIOx->Mode == GPIO_OUT_OD)		
			{
				P0M1 |=  GPIOx->Pin,	P0M0 |=  GPIOx->Pin;	 //开漏输出
			}
			else                     //推挽输出
			{
			  P0M1 &= ~GPIOx->Pin,	P0M0 |=  GPIOx->Pin;
			}			
			break;
	  case GPIO_1:
			if(GPIOx->Mode == GPIO_PullUp)//上拉准双向口	
			{
			  P1M1 &= ~GPIOx->Pin,	P1M0 &= ~GPIOx->Pin;	
			}
			else if(GPIOx->Mode == GPIO_HighZ)	//浮空输入	
			{
				P1M1 |=  GPIOx->Pin,	P1M0 &= ~GPIOx->Pin;	
			}
      else if(GPIOx->Mode == GPIO_OUT_OD)	//开漏输出	
			{
				P1M1 |=  GPIOx->Pin,	P1M0 |=  GPIOx->Pin;	 
			}
			else                     //推挽输出
			{
			  P1M1 &= ~GPIOx->Pin,	P1M0 |=  GPIOx->Pin;
			}		
			break;
	  case GPIO_2:
			if(GPIOx->Mode == GPIO_PullUp)//上拉准双向口	
			{
			  P2M1 &= ~GPIOx->Pin,	P2M0 &= ~GPIOx->Pin;	
			}
			else if(GPIOx->Mode == GPIO_HighZ)	//浮空输入	
			{
				P2M1 |=  GPIOx->Pin,	P2M0 &= ~GPIOx->Pin;	
			}
      else if(GPIOx->Mode == GPIO_OUT_OD)	//开漏输出	
			{
				P2M1 |=  GPIOx->Pin,	P2M0 |=  GPIOx->Pin;	 
			}
			else                     //推挽输出
			{
			  P2M1 &= ~GPIOx->Pin,	P2M0 |=  GPIOx->Pin;
			}		
			break;
	  case GPIO_3:
			if(GPIOx->Mode == GPIO_PullUp)//上拉准双向口	
			{
			  P3M1 &= ~GPIOx->Pin,	P3M0 &= ~GPIOx->Pin;	
			}
			else if(GPIOx->Mode == GPIO_HighZ)	//浮空输入	
			{
				P3M1 |=  GPIOx->Pin,	P3M0 &= ~GPIOx->Pin;	
			}
      else if(GPIOx->Mode == GPIO_OUT_OD)//开漏输出		
			{
				P3M1 |=  GPIOx->Pin,	P3M0 |=  GPIOx->Pin;	 
			}
			else                     //推挽输出
			{
			  P3M1 &= ~GPIOx->Pin,	P3M0 |=  GPIOx->Pin;
			}		
			break;
	  case GPIO_4:
			if(GPIOx->Mode == GPIO_PullUp)//上拉准双向口	
			{
			  P4M1 &= ~GPIOx->Pin,	P4M0 &= ~GPIOx->Pin;	
			}
			else if(GPIOx->Mode == GPIO_HighZ)	//浮空输入	
			{
				P4M1 |=  GPIOx->Pin,	P4M0 &= ~GPIOx->Pin;	
			}
      else if(GPIOx->Mode == GPIO_OUT_OD)	//开漏输出	
			{
				P4M1 |=  GPIOx->Pin,	P4M0 |=  GPIOx->Pin;	 
			}
			else                     //推挽输出
			{
			  P4M1 &= ~GPIOx->Pin,	P4M0 |=  GPIOx->Pin;
			}					
			break;
	  case GPIO_5:
			if(GPIOx->Mode == GPIO_PullUp)//上拉准双向口	
			{
			  P5M1 &= ~GPIOx->Pin,	P5M0 &= ~GPIOx->Pin;	
			}
			else if(GPIOx->Mode == GPIO_HighZ)	//浮空输入	
			{
				P5M1 |=  GPIOx->Pin,	P5M0 &= ~GPIOx->Pin;	
			}
      else if(GPIOx->Mode == GPIO_OUT_OD)//开漏输出		
			{
				P5M1 |=  GPIOx->Pin,	P5M0 |=  GPIOx->Pin;	 
			}
			else                     //推挽输出
			{
			  P5M1 &= ~GPIOx->Pin,	P5M0 |=  GPIOx->Pin;
			}		
			break;
		
		default:
			break;
	}
	
  return 1;
}
					
//					Delay_ms(50);
//					if(InputGPIOStateBuffer[Num] != GPIO_ReadInputDataBit(InputGPIO[Num].Port,InputGPIO[Num].Pin))
//					{
//            InputGPIOStateBuffer[Num] = GPIO_ReadInputDataBit(InputGPIO[Num].Port,InputGPIO[Num].Pin);
//						if(InputGPIOEnableBuffer[Num])
//						{
//							Bus2SendOK = 0;				
//							OnlineInputGPIOTriggeredReport(Num,Tempt[0]);
//						}
//						while(!Bus2SendOK);											
//					}		

						
//			if(PFKDevState == Offline)
//			{
//				//对于自复位开关，网关需要所有的状态，所以GPIO的“1”和“0”状态在“在线”状态下都要上报，所以delay时间短，需要检测到“1”和“0”状态
//				Delay_ms(200);
//				InputGPIOStateBuffer[Num] = GPIO_ReadInputDataBit(InputGPIO[Num].Port,InputGPIO[Num].Pin);	 			
//			}
//			else
//			{
//				//离线状态下，对于自复位开关，只需检测"1"的触发状态即可，所以delay时间长，滤除掉"0"状态
//			  Delay_ms(200);
//			  //InputGPIOStateBuffer[Num] = GPIO_ReadInputDataBit(InputGPIO[Num].Port,InputGPIO[Num].Pin);			
//			}	
