#include "relay.h"
#include "io.h"
#include "rs485.h"
#include "MyConfig.h"



GPIO_InitTypeDef RelayGPIO[2] = {0};

//uint8_t RelayStateBuffer[3] = {0};

uint8_t CurtainStatus = 0x00;     //0x00--Stop      0x01--Open   0x02--Close 


void RelayGPIOInit(void)
{
//Relay1GPIO = P2^5
	RelayGPIO[1].Port = GPIO_2;
  RelayGPIO[1].Pin = GPIO_Pin_7;	
	RelayGPIO[1].Mode =  GPIO_PullUp; 
  GPIO_Init(&RelayGPIO[1]);
	
//Relay2GPIO = P2^6  
	RelayGPIO[0].Port = GPIO_2;
  RelayGPIO[0].Pin = GPIO_Pin_6;		
	RelayGPIO[0].Mode =  GPIO_PullUp; 
  GPIO_Init(&RelayGPIO[0]);
	
//Relay3GPIO = P2^7  
//	RelayGPIO[0].Port = GPIO_2;
//  RelayGPIO[0].Pin = GPIO_Pin_7;		
//	RelayGPIO[0].Mode =  GPIO_PullUp; 
//  GPIO_Init(&RelayGPIO[0]);	
	
  Curtain_Stop();	
}


uint8_t RS485CmdControlRelay(uint8_t CtrlCmd)
{
  switch(CtrlCmd)
	{
	  case Stop:
      Curtain_Stop();
			break;
		
		case Open:
			Curtain_Open();
			break;
		
		case Close:
			Curtain_Close();
			break;
		
		default:
			return 0;
			break;
	}
  return 1;
}


void Curtain_Open(void)//0x01
{
	GPIO_ResetBits(RelayGPIO[0].Port,RelayGPIO[0].Pin);
	GPIO_ResetBits(RelayGPIO[1].Port,RelayGPIO[1].Pin);
	GPIO_SetBits(RelayGPIO[0].Port,RelayGPIO[0].Pin);	
  CurtainStatus = Open;	
}

void Curtain_Close(void)//0x02
{
	GPIO_ResetBits(RelayGPIO[1].Port,RelayGPIO[1].Pin);		
	GPIO_ResetBits(RelayGPIO[0].Port,RelayGPIO[0].Pin);
	GPIO_SetBits(RelayGPIO[1].Port,RelayGPIO[1].Pin);
  CurtainStatus = Close;	
}

void Curtain_Stop(void)//0x00
{
	GPIO_ResetBits(RelayGPIO[0].Port,RelayGPIO[0].Pin);
	GPIO_ResetBits(RelayGPIO[1].Port,RelayGPIO[1].Pin);
  CurtainStatus = Stop;	
}


//void Turn_On_Relay(uint8_t RelayNum)
//{
//	GPIO_SetBits(RelayGPIO[RelayNum].Port,RelayGPIO[RelayNum].Pin);	
//	RelayStateBuffer[RelayNum] = 0x02;		
//}

//void Turn_Off_Relay(uint8_t RelayNum)
//{
//	GPIO_ResetBits(RelayGPIO[RelayNum].Port,RelayGPIO[RelayNum].Pin); 	
//	RelayStateBuffer[RelayNum] = 0x01;			
//}


//void Toggle_Relay_State(uint8_t RelayNum)
//{
//	if(RelayStateBuffer[RelayNum] == 0x02)
//	{
//		GPIO_ResetBits(RelayGPIO[RelayNum].Port,RelayGPIO[RelayNum].Pin); 			
//		RelayStateBuffer[RelayNum] = 0x01;			
//	}
//	else if(RelayStateBuffer[RelayNum] == 0x01)
//	{
//		GPIO_SetBits(RelayGPIO[RelayNum].Port,RelayGPIO[RelayNum].Pin); 			
//		RelayStateBuffer[RelayNum] = 0x02;			
//	}		
//}


void OfflineGPIOControlRelay(uint8_t RelayNum)
{
	if(RelayNum == 0x00)
	{
	  if(CurtainStatus == Stop)
		{
		   Curtain_Close();				
		}
		else //if(CurtainStatus == Open)
		{
		   Curtain_Stop();		
		}
	}
	else if(RelayNum == 0x01)
	{
	  if(CurtainStatus == Stop)
		{
		   Curtain_Open();		
		}
		else //if(CurtainStatus == Close)
		{
		   Curtain_Stop();		
		}	
	}
}




void GPIO_SetBits(uint8_t Port,uint8_t GPIO_Pin)
{
  switch(Port)
	{
	  case GPIO_0:
      P0 |= (0x01 << GPIO_Pin);
			break;
	  case GPIO_1:
      P1 |= (0x01 << GPIO_Pin);
			break;
	  case GPIO_2:
      P2 |= (0x01 << GPIO_Pin);
			break;
	  case GPIO_3:
      P3 |= (0x01 << GPIO_Pin);
			break;
	  case GPIO_4:
      P4 |= (0x01 << GPIO_Pin);
			break;
	  case GPIO_5:
      P5 |= (0x01 << GPIO_Pin);
			break;
		
		default:
			break;	
	}  
}

void GPIO_ResetBits(uint8_t Port,uint8_t GPIO_Pin)
{
  switch(Port)
	{
	  case GPIO_0:
      P0 &= ~(0x01 << GPIO_Pin);
			break;
	  case GPIO_1:
      P1 &= ~(0x01 << GPIO_Pin);
			break;
	  case GPIO_2:
      P2 &= ~(0x01 << GPIO_Pin);
			break;
	  case GPIO_3:
      P3 &= ~(0x01 << GPIO_Pin);
			break;
	  case GPIO_4:
      P4 &= ~(0x01 << GPIO_Pin);
			break;
	  case GPIO_5:
      P5 &= ~(0x01 << GPIO_Pin);
			break;
		
		default:
			break;	
	}  
}