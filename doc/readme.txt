/***************************************Home_Control*******************************************/

1.��"MyConfig.h",һ��Ҫ����ǰ��ģ�����������	

2.����Ƶ��λ 11.0592MHz 
            
  ������Ƶ�ʲ��ԣ���ʱʱ�䡢���ڲ����ʡ����Ź����ʱ�䶼������쳣��
    
3.�ο��鼮��51��Ƭ����������---����STC15W4Kϵ��(C���԰�)   ����ȫ ������	
  PCON�Ĵ���---Page.134



4.��ʱ����ֵ���㣺
   1T ��Ϊ T(us) = (65536 - Value)/SYSclk --->T/1000(ms) = (65536 - Value)/SYSclk
                                          --->Value = 65536 - SYSclk * (T/1000)
										  
   12T ��Ϊ T(us) = (65536 - Value)/(SYSclk/12) --->T(ms)/1000 = (65536 - Value)/(SYSclk/12)
                                                --->Value = 65536 - (SYSclk/12) * (T/1000)   
   �ܽ�,������ʽ Value = 65536 - SYSclk*T,����TΪus��SYSclkΪ1Tģʽ
   1)1T,1us--->Value = 65536 - SYSclk*T
   2)1T.1ms--->Value = 65536 - SYSclk*(T/100),��Value = 65536 - SYSclk*T/100;
   3)12T,1us--->Value = 65536 - (SYSclk/12)*T,��Value = 65536 - SYSclk*T/12
   4)12T,1ms--->Value = 65536 - (SYSclk/12)*(T/1000),��Value = 65536 - SYSclk*T/12/1000    
   



//Э��
1.����(���15s)--cmd:0x01
--->  // ad da len addr cmd crc	
	  // ad da  02  ff   01 crc

2.״̬��ѯ---cmd:0x20  	
 --->  //ad da len addr cmd crc
	   //ad da  02 addr 20 crc
	  
<---  //ad da len addr cmd type io_h io_l relay crc
      //AD DA  06  00   20  A2   00   08   05   1F  
	 
3.�̵�������--cmd:0x60
                       channel:1����2
			           state:00--off,01--on,02--keep,03--toggle
--->//ad da len addr cmd channel state crc
	//ad da 04   00   60    01     03  83			 
	 
	                                channel:1����2
                                    state:00--off,01--on, 
<--- //ad da len addr  cmd type channle  state crc
     //AD DA  05  00   60   A2    02      01   E6 	   

	 
4.��ַ��ѯ--cmd:0xf0
---> //ad da len  addr cmd crc
	 //ad da  02  ff   f0 crc
	 
<----//ad da len  addr cmd type crc
     //AD DA  03   00   f0  A2  19 
	 
5.GPIO�����������ϱ�--cmd:0x02	state:00--Low,01--High 
<--- //ad da len addr cmd type channle  state crc
     //AD DA  05  00   02  A2     00      01  E0 
	 
	 

	 
/*�Զ��建����*/	 	 
//uint8_t RS485RecvDataCacheBuffer[5][10] = {0};
//uint8_t *RS485RecvDataCacheBufferInPtr = NULL;
//uint8_t *RS485RecvDataCacheBufferOutPtr = NULL;
//uint8_t *RS485RecvDataCacheBufferEndPtr = NULL;

//uint8_t RS485SendDataCacheBuffer[4][10] = {0};
//uint8_t *RS485SendDataCacheBufferInPtr = NULL;
//uint8_t *RS485SendDataCacheBufferOutPtr = NULL;
//uint8_t *RS485SendDataCacheBufferEndPtr = NULL;	 
	 
//void RS485DataCacheBufferInit(void)
//{
//  RS485RecvDataCacheBufferInPtr = RS485RecvDataCacheBuffer[0];
//	RS485RecvDataCacheBufferOutPtr = RS485RecvDataCacheBufferInPtr;
//	RS485RecvDataCacheBufferEndPtr = RS485RecvDataCacheBuffer[5];

//	RS485SendDataCacheBufferInPtr = RS485SendDataCacheBuffer[0];
//	RS485SendDataCacheBufferOutPtr = RS485SendDataCacheBufferInPtr; 
//	RS485SendDataCacheBufferEndPtr = RS485SendDataCacheBuffer[4];
//}


//void DataIn_RS485RecvDataCacheBuffer(uint8_t *Data,uint8_t Num)
//{
//  RS485RecvDataCacheBufferInPtr[0] = Num;
//	memcpy(&RS485RecvDataCacheBufferInPtr[1],Data,Num);
//	RS485RecvDataCacheBufferInPtr += 10;
//	
//	if(RS485RecvDataCacheBufferInPtr >= RS485RecvDataCacheBufferEndPtr)
//	{
//	  RS485RecvDataCacheBufferInPtr = RS485RecvDataCacheBuffer[0];
//	}
//}

//void DataOut_RS485RecvDataCacheBuffer(void)
//{
//  if(RS485RecvDataCacheBufferInPtr != RS485RecvDataCacheBufferOutPtr)
//	{
//    RS485ReceiveDataParase(&RS485RecvDataCacheBufferOutPtr[1],RS485RecvDataCacheBufferOutPtr[0]);		
//		memset(RS485RecvDataCacheBufferOutPtr,0,10);
//		RS485RecvDataCacheBufferOutPtr += 10;
//		
//		if(RS485RecvDataCacheBufferOutPtr >= RS485RecvDataCacheBufferEndPtr)
//		{
//		  RS485RecvDataCacheBufferOutPtr = RS485RecvDataCacheBuffer[0];
//		}
//	}
//}

//void DataIn_RS485SendDataCacheBuffer(uint8_t *PushData,uint8_t PushCount)
//{
//  RS485SendDataCacheBufferInPtr[0] = PushCount;
//	memcpy(&RS485SendDataCacheBufferInPtr[1],PushData,PushCount);
//	RS485SendDataCacheBufferInPtr += 10;
//	
//	if(RS485SendDataCacheBufferInPtr >= RS485SendDataCacheBufferEndPtr)
//	{
//	  RS485SendDataCacheBufferInPtr = RS485SendDataCacheBuffer[0];
//	}
//}

//uint8_t DataOut_RS485SendDataCacheBuffer(void)
//{
//  if(RS485SendDataCacheBufferOutPtr != RS485SendDataCacheBufferInPtr)
//	{
//		TempCont = RS485SendDataCacheBufferOutPtr[0];
//		memcpy(TempBuffer,&RS485SendDataCacheBufferOutPtr[1],TempCont);
//		RS485SendDataCacheBufferOutPtr += 10;
//		
//		if(RS485SendDataCacheBufferOutPtr >= RS485SendDataCacheBufferEndPtr)
//		{
//		  RS485SendDataCacheBufferOutPtr =  RS485SendDataCacheBuffer[0];
//		}
//	  return 1;
//	}
//	return 0;
//}	 
	 