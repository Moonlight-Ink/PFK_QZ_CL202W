#ifndef _IO_H
#define _IO_H

#include "STC8A8K64S4A12.h"


typedef struct
{
  uint8_t Port;
	uint8_t Pin;
	uint8_t Mode;
}GPIO_InitTypeDef;


#define  GPIO_PullUp    0	//上拉准双向口
#define  GPIO_HighZ     1	//浮空输入
#define  GPIO_OUT_OD    2	//开漏输出
#define  GPIO_OUT_PP    3	//推挽输出

#define GPIO_0 0x00
#define GPIO_1 0x01
#define GPIO_2 0x02
#define GPIO_3 0x03
#define GPIO_4 0x04
#define GPIO_5 0x05


#define GPIO_Pin_0 0x00
#define GPIO_Pin_1 0x01
#define GPIO_Pin_2 0x02
#define GPIO_Pin_3 0x03
#define GPIO_Pin_4 0x04
#define GPIO_Pin_5 0x05
#define GPIO_Pin_6 0x06
#define GPIO_Pin_7 0x07



extern uint8_t RS485AddrBuffer[6];
extern uint8_t InputGPIOEnableBuffer[6];
extern uint8_t InputGPIOStateBuffer[6];
extern uint8_t InputGPIOStateBufferLock[6];
//extern uint8_t StateBuffer[3];
extern uint8_t ModuleAddr;
extern uint8_t ModuleStatus[2];


void InputGPIOInit(void);
void RS485AddrGPIOInit(void);
void InputGPIOEnableInit(void);
void Synchronization_InputGPIO_Status(void);

void RS485AddrCheck(void);
void InputEnableGPIOCheck(void);
void InputGPIOStateCheck(void);


void Get_Device_State(void);

uint8_t GPIO_ReadInputDataBit(uint8_t GPIOx,uint8_t GPIO_Pin);
uint8_t GPIO_Init(GPIO_InitTypeDef *GPIOx);
#endif
