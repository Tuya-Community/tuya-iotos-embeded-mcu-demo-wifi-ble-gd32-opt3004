#ifndef  __OPT3004_H__
#define  __OPT3004_H__

#include "gd32e23x.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"

/*1.结果寄存器（地址：0x00）：OPT3004采集完之后，光照数据就是存在这里。
结果寄存器有16位，前4位是指数（用E[3:0]表示），后12位是尾数（用R[11:0]表示）。
计算光照强度的公式为：光照强度=0.01x2 E[3:0] x R[11:0] （单位：lx 勒克斯）
如：寄存器的数值为0x2356，2是指数，356是尾数，换算成十进制为854。那么光照强度为：0.01x22x854=34.16 lx

2.配置寄存器（地址：0x01）：配置OPT3004采集的方式
16位配置寄存器具体如下所示：
15	14	13	12	11	10	09	08	07	06	05	04	03	02	01	00
RN[3]	RN[2]	RN[1]	RN[0]	CT	M[1]	M[0]	OVF	CRF	FH	FL	L	POL	ME	FC[1]	FC[0]
5~12 RN - 配置测量光照的范围，当配置位1100传感器测量范围自动选择
11 CT - 测量时间配置 0- 100Ms 1-800Ms
10:9 M[1:0] - 转换模式：00：关闭模式 01：单次转换 10、11：连续转换
8 OVF - 测量光照超出设定的范围或最大测量值 溢出标志
7 CRF - 转换就绪字段 1-转换完成
6 FH - 转换的光照值 大于上限值 置位
5 FL - 转换的光照值 小于下限值 置位
4 L - 中断输出的两种模式：1-窗口模式：这种模式下高限置位和低限置位INT输出，0-滞后模式：高限置位INT输出 具体看手册
3 POL - INT 中断被触发输出极性 0：拉低 1：拉高
2 ME - 掩码字段
0:1 FC - 超出上限范围故障计数，如果超出次数，大于等于计数设定次，INT输出中断

注：若要用窗口锁存模式，OPT3004触发中断后需要读取配置寄存器才能清除标志位。

3、 下限寄存器（地址：0x02）：设置触发下限，如果要用中断方式触发，需要用到
4、 上限寄存器（地址：0x03）：设置触发上限，如果要用中断方式触发，需要用到
5、 产商ID寄存器（地址：0x7E）：只读寄存器，固定为0x5449
6、 设备ID寄存器（地址：0x7F）：只读寄存器，固定为0x3001
*/
#define OPT3004_I2C_ADDR_W                          0x88
#define OPT3004_I2C_ADDR_R                          0x89
#define DEVICE_ID                                   0x3001
#define MANUFACTURER_ID                             0x5449
#define OPT3004_RESULT_REGIDSTER_ADDR               0x00
#define OPT3004_CONFIG_REGIDSTER_ADDR               0x01
#define OPT3004_LOW_LIMIT_REGIDSTER_ADDR            0x02
#define OPT3004_HIGH_LIMIT_REGIDSTER_ADDR           0x03
#define OPT3004_MANU_ID_REGIDSTER_ADDR              0x7E
#define OPT3004_DEVICE_ID_REGIDSTER_ADDR            0x7F

//#define CONFIG_MAX_LUX_R                            0x0FFF//4095

unsigned int GetOPT3004ManufacturerID(void);
unsigned int GetOPT3004DeviceID(void);
short OPT3004_get_register_value(unsigned char in_register);
void OPT3004_set_register_value(unsigned char RegisterName,unsigned int value);
void OPT3004_config(void);
unsigned char OPT3004_Init(void);
unsigned char OPT3004_get_lux(void);
extern void Get_Data_Deal(void);
#endif /*__OPT3004_H__*/


