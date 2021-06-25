#ifndef __MYIIC_H
#define __MYIIC_H

#include "gd32e23x.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

//IO操作函数         
#define SET_IIC_SCL() gpio_bit_set(GPIOB, GPIO_PIN_0)//输出1
#define CLR_IIC_SCL() gpio_bit_reset(GPIOB, GPIO_PIN_0)//输出0

#define SET_IIC_SDA() gpio_bit_set(GPIOB, GPIO_PIN_1)//输出1
#define CLR_IIC_SDA() gpio_bit_reset(GPIOB, GPIO_PIN_1)//输出0

#define READ_SDA   gpio_input_bit_get(GPIOB, GPIO_PIN_1)  //输入SDA

//IO方向设置
#define SDA_IN()      gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_1)//配置为输入
#define SDA_OUT()     { gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_1);\
           gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);} //配置为输出
   
//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口                                 
void IIC_Start(void);                                //发送IIC开始信号
void IIC_Stop(void);                                  //发送IIC停止信号
void IIC_Send_Byte(unsigned char txd);                        //IIC发送一个字节
unsigned char IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
unsigned char IIC_Wait_Ack(void);                                 //IIC等待ACK信号
void IIC_Ack(void);                                        //IIC发送ACK信号
void IIC_NAck(void);                                //IIC不发送ACK信号

void IIC_Write_One_Byte(unsigned char daddr,unsigned char addr,unsigned char data);
unsigned char IIC_Read_One_Byte(unsigned char daddr,unsigned char addr);          

#endif


