#include "timer.h"
#include "led.h"
#include "dac.h"
#include "stm32f4xx_gpio.h"


//////////////////////////////////////////////////////////////////////////////////	 
//STM32F4����ģ��-�⺯���汾
//�Ա����̣�http://mcudev.taobao.com							  
////////////////////////////////////////////////////////////////////////////////// 	 


//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!

short *pcm_buf1;
short *pcm_buf2;// PCM�����壬ʹ������������ 
short *pcm_buff_temp;
u8  buff_flag=0;
u16 count=0;
extern u8  change_flag;//�����ⲿ����
u8 PA0_flag = 0;

u8 buff_change = 0;

void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
    TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; //��RCCʱ�ӷ�Ƶ
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�ж����ȼ�����//�����Լ���ӵ�

    PA0_init();//���ڲ���ʱ�䣬ʾ����
}

void TIM3_Deinit()
{
    TIM_Cmd(TIM3,DISABLE); //�رն�ʱ��3
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,DISABLE);  ///�ر�TIM3ʱ��
    return;
}

//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)//ͨ��ʾ�������Եó�������22us���ң���Ӧ44.1KHZ��
{
	#if 0 /*xqy 2018-3-4*/
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
		LED1=!LED1;//DS1��ת
	}
	#endif
	u16 data;

	
	#if 0 /*xqy 2018-3-13*/
	if(PA0_flag)
	{
	    PA0_flag = 0;
	    PA0_out_high();
	}
	else
	{
	    PA0_flag = 1;
	    PA0_out_low();
	}
	#endif
	
	if(buff_flag)
	{
	    data = *(pcm_buf1+count);//���ֽ���ǰ--���ֽ��ں�
	    //data = data<<8;
	    //data += *(pcm_buf1+count);//������
	    Dac1_Set_Vol(data);//�Ƴ�PCM����

	    data = *(pcm_buf1+count+1);
	    //data = data<<8;
	    //data += *(pcm_buf1+count+2);//������
	    Dac2_Set_Vol(data);//�Ƴ�PCM����
	    
	    PA0_out_high();
	}
	else
	{
	    data = *(pcm_buf2+count);
	    //data = data<<8;
	    //data += *(pcm_buf2+count);//������
	    Dac1_Set_Vol(data);//�Ƴ�PCM����

	    data = *(pcm_buf2+count+1);
	    //data = data<<8;
	    //data += *(pcm_buf2+count);//������
	    Dac2_Set_Vol(data);//�Ƴ�PCM����

	    PA0_out_low();
	}
	count +=2;//����ƶ��ĸ��ֽ�
	if(count>=2304)//һ��PCMbuff��ɣ��л�buff//ÿ֡���ݹ̶�1152�β�����ÿ�β���������������������2034��16λ����
	{
	    if(buff_flag)
	    {
	        buff_flag = 0;
	    }
	    else
	    {
            buff_flag = 1;
	    }
	    count = 0;
	    //change_flag--;
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}

void set_buff(short *buff1,short *buff2)
{
    pcm_buf1 = buff1;
    pcm_buf2 = buff2;
    uart_printf("���ú���DAC��buffָ��\n");
}

void PA0_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOEʱ��

    //GPIOE7~E10��ʼ�����������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;//
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
}
void PA0_out_high(void)
{
    GPIO_SetBits(GPIOA,GPIO_Pin_0);
}
void PA0_out_low(void)
{
    GPIO_ResetBits(GPIOA,GPIO_Pin_0);
}

void PA1_out_high(void)
{
    GPIO_SetBits(GPIOA,GPIO_Pin_1);
}
void PA1_out_low(void)
{
    GPIO_ResetBits(GPIOA,GPIO_Pin_1);
}

