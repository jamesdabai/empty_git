#ifndef __DAC_H
#define __DAC_H	 
#include "sys.h"	     			    
//////////////////////////////////////////////////////////////////////////////////	 

//DAC ��������	   
//STM32F4����ģ��-�⺯���汾
//�Ա����̣�http://mcudev.taobao.com
////////////////////////////////////////////////////////////////////////////////// 	

void Dac1_Init(void);		//DACͨ��1��ʼ��	 	 
void Dac1_Set_Vol(u16 vol);	//����ͨ��1�����ѹ

void Dac2_Init(void);       //DACͨ��2��ʼ��        
void Dac2_Set_Vol(u16 vol);   //����ͨ��2�����ѹ

#endif

















