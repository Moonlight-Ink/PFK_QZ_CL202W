#ifndef _rs485_H_
#define _rs485_H_

#include "STC8A8K64S4A12.h"
//#include "MyConfig.h"




#define Offline 0x00
#define Online  0x01



typedef struct
{
  uint8_t Cmd;
	uint8_t Channel;
	uint8_t State;
}AckMsg;

extern uint8_t PFKDevState;
extern uint16_t  PFKDevStateTimerCount;
extern uint8_t Bus2SendOK;
//extern uint8_t AcktiveFlag;
//void RS485DataCacheBufferInit(void);
//void DataIn_RS485RecvDataCacheBuffer(uint8_t *Data,uint8_t Num);
//void DataOut_RS485RecvDataCacheBuffer(void);
//void DataIn_RS485SendDataCacheBuffer(uint8_t *PushData,uint8_t PushCount);
//uint8_t DataOut_RS485SendDataCacheBuffer(void);

//void Timer0_Init(void);
//void RS485SendDataPacket(uint8_t *DataBuffer,uint8_t DataNum);



void Timer0_Init(void);
void RS485_Init(void);					           //串口485初始化设置


//void  Analyse_Received_data(uint8_t *Analy_Buffer,uint8_t Analy_Cnt);
//void Response_IO_Relay_State(uint8_t cmd);
//void UART_Cmd_Control_Relay(uint8_t *Cmd_Temp);
//void Response_Cmd_Control_Msg(uint8_t cmd);
//void Response_RS485_Adrress(uint8_t addr,uint8_t cmd);






uint8_t RS485ReceiveDataParase(uint8_t *DataBuffer,uint8_t Num);
void RS485RecviceDataPacketRespond(uint8_t Cmd);

void OnlineInputGPIOTriggeredReport(uint8_t Channel,uint8_t State);

void RS485Bus1SendDataPacket(uint8_t *DataPacket,uint8_t DataNum);
void RS485Bus2SendDataPacket(uint8_t *DataBuffer,uint8_t DataNum);

uint8_t Calculate_DataPackect_CRCValue(uint8_t *DataPacket,uint8_t Len);


uint8_t CRC8_Check(uint8_t len,const uint8_t *buf);
//void Respond_RS485Cmd(AckMsg Data);


//void Send_data(uint8_t *Send_Buffer,uint8_t Send_Cnt);

//void Send2_data(uint8_t *Send_Buffer,uint8_t Send_Cnt);
#endif


