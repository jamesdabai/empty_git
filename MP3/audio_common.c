#include <stdio.h>
//#include "audio_common.h"
#include "audio_mp3.h"
#include "audio_wav.h"
#include "ff.h"
#include <string.h>
#include "dev_key.h"

FIL AudioFile;
AudioPlay_Info AudioPlayInfo;
MUSIC_TRL music_ctl;

uint32_t DualSine12bit[DAC_Buffer_Size];//�൱������BUFF��ǰ��һ��������һ����һ��buff2304*2���ֽ�
__IO uint8_t DataRequestFlag = 0;

void AudioPlay_DataProc(uint16_t* buff,uint16_t num)
{
	uint16_t i;
	//16λ��PWM����ת��Ϊ12λ�����ݣ�Ȼ����X�����ƶ�12λ��һ�����ֵ���ﵽȫ����ѹ������ֵ+2048
	//xqy
	for(i = 0;i < num;i ++)//������ֻ��1152�������㣬ÿ��16λ
	{
		buff[i] = (((int16_t*)buff)[i] / 16 + 2048);//ֻȡһ������������
	}
}
void Audio_first_play(FIL* Wav_File)//xqy
{
    u32 br;
    f_read(Wav_File,DualSine12bit,DAC_Buffer_Size*4/2,&br);//��仺����
	AudioPlay_DataProc((void*)DualSine12bit,DAC_Buffer_Size);
	Play_Start();
	f_read(Wav_File,(void*)((uint32_t)DualSine12bit + AudioPlayInfo.BufferSize),DAC_Buffer_Size*4/2,&br);//��仺����
	AudioPlay_DataProc((void*)((uint32_t)DualSine12bit + AudioPlayInfo.BufferSize),DAC_Buffer_Size);
}
void* AudioPlay_GetCurrentBuff(void)
{
	if(DataRequestFlag == 1)//DMA�������ж������ݶ����ڶ���buff�У���Ϊ����Ҫ�����һ��buff��
	{
		return (void*)((uint32_t)DualSine12bit + AudioPlayInfo.BufferSize);
	}
	else if(DataRequestFlag == 2)//����һ���жϣ������ݶ����õ��ʼ
	{
		return DualSine12bit;
	}
	else
	{
		return NULL;
	}
}

void AudioPlay_PendSem(void)
{
	DataRequestFlag = 0;//������ɺ����������ɱ�־λ
	while(DataRequestFlag == 0)//�ȴ��������
	{
		__wfi();
	}
}

void AudioPlay_ClearBuf(void)
{
	uint32_t i;
	for(i = 0;i < DAC_Buffer_Size;i ++)
	{
		DualSine12bit[i] = 0x08000800;
	}
}

void AudioPlay_ClearSem(void)
{
	DataRequestFlag = 0;
}

void curse_disp(u16 row,u16 line,u8 size,u16 color)
{
    u16 x,y,r;
    u8 i;
    x = size*music_ctl.curse_line_bak;
    y = size*music_ctl.curse_row_bak;
    Lcd_ColorBox(x,y,size+2,size+2,0X07FF);
    x = size*(line)+size/2;
    y = size*(row)+size/2;
    r = size/2;
    for(i=0;i<4;i++)
    {
        LCD_Draw_Circle(x,y,r);
        r-=2;
    }
}
void music_play_init(u16 music_totle)
{
     //��ʼ��
    music_ctl.music_totle = music_totle;//��ʼ���ļ����¶��ٸ����ļ�
    music_ctl.music_num   = 0;
    music_ctl.curse_line_bak   = 2;
    music_ctl.curse_row_bak   = 0;
    music_ctl.music_play_sta = AudioPlay_Sequential_loop;//ѭ������
    curse_disp(0,2,24,0x0);
}
s32 music_end_process(void)
{
    s32 ret = AudioPlay_OK;
    switch (music_ctl.music_play_sta)
    {
        case AudioPlay_Sigle_loop:
            ret = AudioPlay_OK;
            break;
            
        case AudioPlay_Sequential_loop:
            if(music_ctl.music_num>=(music_ctl.music_totle-1))
            {
                music_ctl.music_num = 0;
            }
            else
            {
                music_ctl.music_num++;
            }
            curse_disp(music_ctl.music_num,2,24,0x0);
            music_ctl.curse_row_bak =  music_ctl.music_num;
            uart_printf("��һ��\n");
            break;
            
        case AudioPlay_Sigle:
            ret = AudioPlay_Exit;
            break;
            
        case AudioPlay_Sequential:
            if(music_ctl.music_num>=(music_ctl.music_totle-1))
            {
                ret = AudioPlay_Exit;
            }
            else
            {
                music_ctl.music_num++;
            }
            curse_disp(music_ctl.music_num,2,24,0x0);
            music_ctl.curse_row_bak =  music_ctl.music_num;
            uart_printf("��һ��\n");
            break;
            
        default :
            ret = AudioPlay_Exit;
            break;
    }
    
}
AudioPlayRes MusicPlayingProcess(void)
{
    u32 key;
    key_read(&key);
    if(key!=0)
    {
        if(key==key_down)
        {
            if(music_ctl.music_num>=(music_ctl.music_totle-1))
            {
                music_ctl.music_num = 0;
            }
            else
            {
                music_ctl.music_num++;
            }
            curse_disp(music_ctl.music_num,2,24,0x0);
            music_ctl.curse_row_bak =  music_ctl.music_num;
            uart_printf("��һ��\n");
        }
        else if(key==key_up)
        {
            if(music_ctl.music_num<=0)
            {
                music_ctl.music_num = music_ctl.music_totle-1;
            }
            else
            {
                music_ctl.music_num--;
            }
            curse_disp(music_ctl.music_num,2,24,0x0);
            music_ctl.curse_row_bak =  music_ctl.music_num;
            uart_printf("��һ��\n");
        }

        if(key==key_ok)//ȷ�ϼ�
        {
            uart_printf("ȷ��\n");
            return AudioPlay_Next;
        }
        return AudioPlay_OK;
    }
	return AudioPlay_OK;
}

uint8_t AudioPlay_Config(uint8_t Bits,uint32_t SampleRate,uint16_t BufSize)
{
	DMA_InitTypeDef            DMA_InitStructure;
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	NVIC_InitTypeDef				NVIC_InitStructure;
	
	//TIM_DeInit(TIM2);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 84000000/SampleRate-1;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
	TIM_Cmd(TIM2, ENABLE);//���ö�ʱ����ΪDAC�Ĵ���
	//TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //֮ǰһֱû�����x    qy
	#if 0 /*xqy 2018-6-3*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//����DMA�ж�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
	#endif

	DMA_DeInit(DMA1_Stream5);//���֮ǰ��stream4�ϵ������жϱ�־
	//����DMA��ÿ��8����������ÿ����������8��ͨ��
    while (DMA_GetCmdStatus(DMA1_Stream5) != DISABLE){}//�ȴ�DMA������ 
    
	DMA_InitStructure.DMA_Channel = DMA_Channel_7;  //ͨ��ѡ��
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(DAC->DHR12RD);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&DualSine12bit;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//�ڴ浽����
	DMA_InitStructure.DMA_BufferSize = BufSize;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�Զ���������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�ڴ��Զ����Ӵ�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;//���ִ���2�ֽ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//Ӧ��������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
  	
	DMA_Init(DMA1_Stream5, &DMA_InitStructure);
	
	DMA_ClearITPendingBit(DMA1_Stream5,DMA_IT_TCIF5); 
	DMA_ITConfig(DMA1_Stream5,DMA_IT_TC,ENABLE);
	DMA_ITConfig(DMA1_Stream5,DMA_IT_HT,ENABLE);
	//DMA_ITConfig(DMA1_Stream5,DMA_IT_TC|DMA_IT_HT,ENABLE);//DMA_IT_TC|DMA_IT_HT,ENABLE);//����DMA�жϣ���������ж�����/����һ���ж�����
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream5_IRQn;//����DMA�ж�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	DMA_Cmd(DMA1_Stream5, ENABLE);
	uart_printf("���ý���\n");
	return 0;
}

AudioFileType Audio_CheckFileExtname(char* path)
{
	char* temp;
	
	temp = strrchr(path,'.');
	
	temp ++;
	
	if(!strcasecmp(temp,"MP3"))
		return AudioFileType_MP3;
	else if(!strcasecmp(temp,"WAV"))
		return AudioFileType_WAV;
	
	return AudioFileType_ERROR;
}

s32 AudioPlayFile(char* path)
{
    s32 ret;
	memset(&AudioPlayInfo,0,sizeof(AudioPlay_Info));
	AudioPlay_ClearSem();
	
	switch(Audio_CheckFileExtname(path))//�жϺ�׺����ȷ�����ŵ���Ƶ�ļ�����
	{
		case AudioFileType_MP3:
		    uart_printf("MP3����\n");
			AudioPlayInfo.PlayRes = MP3_Play(path);
			uart_printf("MP3����==%d\n",AudioPlayInfo.PlayRes);
			break;
		case AudioFileType_WAV:
		    uart_printf("WAV����\n");
			AudioPlayInfo.PlayRes = WAV_Play(path);
			break;
		default:
			break;
	}
}

void AudioPlay_Init(void)
{
	NVIC_InitTypeDef				NVIC_InitStructure;
	DAC_InitTypeDef                 DAC_InitStructure;
	GPIO_InitTypeDef				GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);//������ʱ��DMA
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_DAC| RCC_APB1Periph_TIM2, ENABLE);//��ʱ����DACʱ��
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ��
    //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	
	DAC_StructInit(&DAC_InitStructure);
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;//����Դ//��ʱ������
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
	//DAC_Init(DAC_Channel_2, &DAC_InitStructure);
	DAC_SoftwareTriggerCmd(DAC_Channel_1,DISABLE);
	//DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);
	DAC_Cmd(DAC_Channel_1, ENABLE);
	//DAC_Cmd(DAC_Channel_2, ENABLE);
	//DAC_SetChannel1Data(DAC_Align_12b_R,0);
	//DAC_SetChannel2Data(DAC_Align_12b_R,0);
}

void Play_Start(void)
{
	DAC_DMACmd(DAC_Channel_1, ENABLE);
	uart_printf("��DACʹ��\n");
}

void Play_Stop(void)
{
	DAC_DMACmd(DAC_Channel_1, DISABLE);
	DAC->DHR12RD = 0x08000800;
	AudioPlay_ClearBuf();
}
//void TIM_Config()

void DMA1_Stream5_IRQHandler(void)
{
    //uart_printf("�ж�һ��\n");
    DataRequestFlag = 1;
	if(DMA_GetITStatus(DMA1_Stream5,DMA_IT_TCIF5))//��������жϱ�־
    {
        DataRequestFlag = 1;
       // uart_printf("ok\n");
    }
	else if(DMA_GetITStatus(DMA1_Stream5,DMA_IT_HTIF5))//����һ������жϱ�־
	{
        DataRequestFlag = 2;
       //uart_printf("half\n");
    }
	//DMA_ClearITPendingBit(DMA1_Stream5,DMA_IT_TCIF7);
	DMA_ClearITPendingBit(DMA1_Stream5,DMA_IT_TCIF5 | DMA_IT_HTIF5);
}
#if 0 /*xqy 2018-6-18*/
u32 time_xqy = 0;
void TIM2_IRQHandler(void)
{
    
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //����ж�
	{
		//LED1=!LED1;//DS1��ת
		time_xqy++;
        uart_printf("time2==%d\n",time_xqy);
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //����жϱ�־λ
}
#endif

