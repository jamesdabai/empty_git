#include "dac.h"
//////////////////////////////////////////////////////////////////////////////////	 

//DAC ��������	   
//STM32F4����ģ��-�⺯���汾
//�Ա����̣�http://mcudev.taobao.com
////////////////////////////////////////////////////////////////////////////////// 	
 

//DACͨ��1�����ʼ��
void Dac1_Init(void)
{  
    GPIO_InitTypeDef  GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//ʹ��DACʱ��
	   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��

	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//��ʹ�ô������� TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�ò��η���
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//���Ρ���ֵ����
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1�������ر� BOFF1=1
    DAC_Init(DAC_Channel_1,&DAC_InitType);	 //��ʼ��DACͨ��1

	DAC_Cmd(DAC_Channel_1, ENABLE);  //ʹ��DACͨ��1
  
    DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ
}
//����ͨ��1�����ѹ
//vol:0~3300,����0~3.3V
void Dac1_Set_Vol(u16 vol)//ֱ����16λ�Ĳ���ֵ
{
	u8 temp[2];
	u16 data;
	temp[0] = vol;
	temp[1] = vol>>8;
	data = ((temp[1]-0x80)<<4)|(temp[0]>>4);//���ϳ���16λPCMת����12λ��PCMֵ�ô��룬�Ҳ�֪��ԭ��
	//temp = temp>>4;//����16λ���൱�ڳ���65536����16λ����ת��ΪDAC12λ���ȵ�
	
	//temp/=1000;
	//temp=temp*4096/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,data);//12λ�Ҷ������ݸ�ʽ����DACֵ
}


//DACͨ��1�����ʼ��
void Dac2_Init(void)
{  
    GPIO_InitTypeDef  GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//ʹ��DACʱ��
	   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��

	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//��ʹ�ô������� TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�ò��η���
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//���Ρ���ֵ����
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1�������ر� BOFF1=1
    DAC_Init(DAC_Channel_2,&DAC_InitType);	 //��ʼ��DACͨ��1

	DAC_Cmd(DAC_Channel_2, ENABLE);  //ʹ��DACͨ��2
  
    DAC_SetChannel2Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ
}
//����ͨ��2�����ѹ
//vol:0~3300,����0~3.3V
void Dac2_Set_Vol(u16 vol)
{
	u8 temp[2];
	u16 data;
	temp[0] = vol;
	temp[1] = vol>>8;
	//data = (s16)vol;
	//data = data>>4;
	//data = data + 2048;
	data = ((temp[1]-0x80)<<4)|(temp[0]>>4);//���ϳ���16λPCMת����12λ��PCMֵ�ô��룬�Ҳ�֪��ԭ��
	
	DAC_SetChannel2Data(DAC_Align_12b_R,data);//12λ�Ҷ������ݸ�ʽ����DACֵ
}



















































