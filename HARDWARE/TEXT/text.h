#ifndef __TEXT_H__
#define __TEXT_H__	 
#include <stm32f4xx.h>
#include "fontupd.h"
//////////////////////////////////////////////////////////////////////////////////	 
 
//������ʾ ��������	   
//STM32F4����ģ��-�⺯���汾
//�Ա����̣�http://mcudev.taobao.com									  
////////////////////////////////////////////////////////////////////////////////// 	 
//�ֿ�����SD�������·��
u8*const GBK24_PATH= "font/GBK24.FON";		//GBK24�Ĵ��λ��
u8*const GBK16_PATH= "font/GBK16.FON";		//GBK16�Ĵ��λ��
u8*const GBK12_PATH= "font/GBK12.FON";		//GBK12�Ĵ��λ��
u8*const UNIGBK_PATH="font/UNIGBK.BIN";		//UNIGBK.BIN�Ĵ��λ��

s32 font_init(void);										//��ʼ���ֿ� 					     
void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size);			//�õ����ֵĵ�����
void Show_Font(u16 x,u16 y,u8 *font,u8 size,u8 mode);					//��ָ��λ����ʾһ������
void Show_Str(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size,u8 mode);	//��ָ��λ����ʾһ���ַ��� 
void Show_Str_Mid(u16 x,u16 y,u8*str,u8 size,u8 len);
#endif
