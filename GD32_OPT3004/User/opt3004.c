#include "opt3004.h"
#include "delay.h" 
#include "myiic.h"
#include "protocol.h"
#include "mcu_api.h"

#define ARRY_SIZE 10
uint32_t opt3004_data[ARRY_SIZE];
uint8_t arry_wpr=0;
static float   vFlux 		= 0;

//往寄存器里面写的数据
//ADD引脚连接到GND	
 unsigned char msb=0;
 unsigned char lsb=0;
 unsigned char ret_suc=0;
 short data;
 
/*********************************************************************************
* Function:
* Description:    写传感器寄存器的接口
* Input:
* Outpot:
* Return:
* History:
*******************************************************************************/
void OPT3004_set_register_value(unsigned char RegisterName,unsigned int value)
{
	
	  IIC_Start();//起始信号
	  IIC_Send_Byte(OPT3004_I2C_ADDR_W);//发送设备地址+写信号
	  IIC_Wait_Ack();
	  IIC_Send_Byte(RegisterName);//发送寄存器地址
	  IIC_Wait_Ack();
	  IIC_Send_Byte((unsigned char)(value>>8));
	  IIC_Wait_Ack();
	  IIC_Send_Byte((unsigned char)(value&0x00FF));
	  IIC_Wait_Ack();
	  IIC_Stop();
}
/*********************************************************************************
* Function:
* Description:    读传感器寄存器的接口
* Input:
* Outpot:
* Return: short类型 寄存器内部数据
* History:
*******************************************************************************/
short OPT3004_get_register_value(uint8_t in_register)
{
    IIC_Start();
    IIC_Send_Byte(OPT3004_I2C_ADDR_W);//发送设备地址+写信号
	  ret_suc = IIC_Wait_Ack();
  	if(ret_suc == 1)	{IIC_Stop();return -1;}
    IIC_Send_Byte(in_register);//发送存储单元地址
    ret_suc =IIC_Wait_Ack();
		if(ret_suc == 1)	{IIC_Stop();return -1;}
	  IIC_Stop();  //停止信号
	
	  IIC_Start();   //起始信号
    IIC_Send_Byte(OPT3004_I2C_ADDR_R);//发送设备地址+读信号
	  ret_suc =IIC_Wait_Ack();
		if(ret_suc == 1)	{IIC_Stop();return -1;}
    msb=IIC_Read_Byte(1);
	  lsb=IIC_Read_Byte(0);
	  IIC_Stop();
	  Delay_ms(5);
	  data=msb<<8;
	  data|=lsb;
	  return data;
}
/*********************************************************************************
* Function:
* Description:  读取厂商ID 
* Input:
* Outpot:
* Return:
* History:
*******************************************************************************/
//获取OPT3004的出厂编号默认值读出来应该是0x5449
unsigned int GetOPT3004ManufacturerID()
{
	unsigned int IDNum=0;
	IDNum =OPT3004_get_register_value(OPT3004_MANU_ID_REGIDSTER_ADDR);
	return IDNum;
}
/*********************************************************************************
* Function:
* Description:  读取设备ID
* Input:
* Outpot:
* Return:
* History:
*******************************************************************************/
//获取OPT3004的设备ID初始值为0x3001
unsigned int GetOPT3004DeviceID()
{
  unsigned int IDNum=0;
	IDNum =OPT3004_get_register_value(OPT3004_DEVICE_ID_REGIDSTER_ADDR);
	return IDNum;         
}
/*********************************************************************************
* Function:
* Description:    OPT3004初始化函数
* Input:
* Outpot:
* Return: 0为初始化成功，1为初始化失败
* History:
*******************************************************************************/
unsigned char OPT3004_Init()
{
  unsigned int ManufacturerIDNum=0;
	unsigned int DeviceIDNum=0;
	OPT3004_config();//配置OPT3004
	Delay_ms(5);
	ManufacturerIDNum=GetOPT3004ManufacturerID();
	Delay_us(100);
  DeviceIDNum=GetOPT3004DeviceID();
	if((0x5449 == ManufacturerIDNum)&&(0x3001 == DeviceIDNum))
		//如果读出来的ID都正确，表示初始化成功
	{
	 return 0;
	}
	else //否则初始化不成功
	{
	 return 1;
	}
}

/*********************************************************************************
* Function:
* Description:   配置寄存器
* Input: 无
* Outpot:无
* Return: 
* History:
*******************************************************************************/
void OPT3004_config(void)
{
	OPT3004_set_register_value(0x01,0xcc10);
}

/*********************************************************************************
* Function:
* Description:   读取传感器数据
* Input: 无
* Outpot:
* Return: unsigned char
* History:
*******************************************************************************/
 
unsigned char OPT3004_get_lux(void)
{
	uint16_t  Result = 0;
	uint8_t		vRval = 0;
	uint16_t  vCfg 	= 0;
	
	uint16_t  vDatE = 0;
	uint16_t  vDatR = 0;
	
	float   vFval 		= 0.0;
	float   vLsbSize 	= 0.0;
	
	
	
vCfg=OPT3004_get_register_value(OPT3004_CONFIG_REGIDSTER_ADDR);
if((vCfg&(0x01<<7)))
 {
	Result = OPT3004_get_register_value(OPT3004_RESULT_REGIDSTER_ADDR);
	//Result =(1<<((Result&0xF000)>>12))*(Result&0x0FFF);
	//Result1 = (0.01f * vFval);
	vDatE = ((Result&0xF000)>>12);
	vDatR = (Result&0x0FFF);
	vFval = (0x01<<vDatE);
	vLsbSize = (0.01f * vFval);
	vFlux  = (vLsbSize * (float)vDatR);
	opt3004_data[arry_wpr] = ((vFlux)*100.0f);//透明外壳不需要矫正 ，乳白色外壳推荐*1.8矫正 *vp_Lux = ((vFlux*1.8)*100.0)
	arry_wpr++;	
 if(arry_wpr >= ARRY_SIZE)		
	{
		arry_wpr = 0;
	}
 }
	else
	{
		vRval = 0x01;//光照采集失败
	}
	
	return vRval;
}
//数据处理
void Get_Data_Deal(void)
  {
		if(0 == OPT3004_get_lux())
		{
			mcu_dp_value_update(DPID_BRIGHT_VALUE,vFlux); //当前亮度值上报;	
		}
	}