#ifndef _RELAY_H
#define _RELAY_H

#include "STC8A8K64S4A12.h"
#include "io.h"

#define Relay_Keep   0x00
#define Relay_OFF    0x01
#define Relay_ON     0x02
#define Relay_Toggle 0x03

//extern uint8_t RelayStateBuffer[3];
extern GPIO_InitTypeDef RelayGPIO[2];
extern uint8_t CurtainStatus;

void RelayGPIOInit(void);


void Curtain_Open(void);//0x01
void Curtain_Close(void);//0x01
void Curtain_Stop(void);//0x01

//void Turn_On_Relay(uint8_t RelayNum);
//void Turn_Off_Relay(uint8_t RelayNum);
//void Toggle_Relay_State(uint8_t RelayNum);
void OfflineGPIOControlRelay(uint8_t RelayNum);
void GPIO_SetBits(uint8_t GPIOx,uint8_t GPIO_Pin);
void GPIO_ResetBits(uint8_t GPIOx,uint8_t GPIO_Pin);

uint8_t RS485CmdControlRelay(uint8_t CtrlCmd);
#endif
