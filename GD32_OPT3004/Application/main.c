#include "gd32e23x.h"
#include "systick.h"
#include <stdio.h>

#include <led.h>
#include <delay.h>
#include <usart.h>
#include <wifi.h>

#include "myiic.h"
#include "opt3004.h"


uint32_t USART0_RECEIVE_Buf[100]; 
uint32_t USART1_RECEIVE_Buf[100]; 
extern void Connect_Wifi(void);

int main(void)
{
    systick_config();   
		LED_Init();  
	  wifi_protocol_init();//wifi协议初始化
		USART0_Init();
		USART1_Init();
	  IIC_Init();		
    OPT3004_Init();

	
 while(1)
		{
			Connect_Wifi(); //配网
			wifi_uart_service();//wifi串口数据处理服务
			Get_Data_Deal();
			//printf("X:%d | Y:%d \r\n",OPT3004_Init(),OPT3004_get_register_value(OPT3004_RESULT_REGIDSTER_ADDR));	
    }
}
