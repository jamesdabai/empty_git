#ifndef __DAC_H
#define __DAC_H	 
#include "sys.h"	     			    
//////////////////////////////////////////////////////////////////////////////////	 

//DAC 驱动代码	   
//STM32F4工程模板-库函数版本
//淘宝店铺：http://mcudev.taobao.com
////////////////////////////////////////////////////////////////////////////////// 	

void Dac1_Init(void);		//DAC通道1初始化	 	 
void Dac1_Set_Vol(u16 vol);	//设置通道1输出电压

void Dac2_Init(void);       //DAC通道2初始化        
void Dac2_Set_Vol(u16 vol);   //设置通道2输出电压

#endif

















