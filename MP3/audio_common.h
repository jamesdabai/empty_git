#ifndef __AUDIOPLAY_H__
#define __AUDIOPLAY_H__

//#include "common.h"
#include "ff.h"
#include "stm32f4xx.h"


#define AUDIO_FLAG_INFO_READY (1<<1)//������Ϣ�ṹ��������
#define AUDIO_FLAG_PAUSED (1<<2)//���ű���ͣ
#define AUDIO_FLAG_ABORT (1<<3)//���������߳�

#define DAC_Buffer_Size 2304
//2304*32bit ������˫������2304*16bit

typedef enum
{
	AudioPlay_OK = 0,
	AudioPlay_OpenFileError,//�޷����ļ�
	AudioPlay_ReadFileError,//�޷���ȡ�ļ�
	AudioPlay_UnsupportedFormat,//������Ƶ�ļ��ļ�
	AudioPlay_UnsupportedParameter,//�ļ���������ȷ
	AudioPlay_DamagedFile,//�ļ���
	
	AudioPlay_MP3_UnsupportedID3v2Version,
	AudioPlay_MP3_NoID3v1Tag,
	AudioPlay_MP3_NoID3v2Tag,
	
	AudioPlay_Next,
	AudioPlay_Prev,
	
	AudioPlay_Exit,
	AudioPlay_PlayEnd,
	AudioPlay_Sigle_loop,//����ѭ����һֱ��ͬһ�׸�
	AudioPlay_Sequential_loop,//˳��ѭ����
	AudioPlay_Sigle,//�������ţ�ֻ����һ�׸��ͣ����
	AudioPlay_Sequential,//˳�򲥷ţ�������ֹͣ������
	
}AudioPlayRes;

typedef enum
{
	AudioFileType_ERROR,
	AudioFileType_MP3,
	AudioFileType_WAV,
}AudioFileType;

typedef struct
{
	AudioFileType FileType;
	volatile AudioPlayRes PlayRes;
	
	uint16_t TotalSec;
	uint16_t CurrentSec;
	
	uint32_t Bitrate;
	uint32_t Samplerate;
	uint32_t BufferSize;
	uint8_t Channels;
	volatile uint8_t Flags;
}AudioPlay_Info;


typedef struct
{
    u16 TotalSec;//������ʱ��
	u16 CurrentSec;//��ǰʱ��
    u16  music_totle;//���ļ������ж��ٸ���
    u16  music_num;//��ѡ�еĸ����ı��
    u16  curse_line_bak;//��걸��
    u16  curse_row_bak;//��걸��
    u8   music_play_sta;//����״̬
}MUSIC_TRL;

extern MUSIC_TRL music_ctl;//xqy
extern AudioPlay_Info AudioPlayInfo;
extern FIL AudioFile;
extern uint32_t DualSine12bit[DAC_Buffer_Size];

void AudioPlay_ClearBuf(void);
void AudioPlay_ClearSem(void);
void AudioPlay_PendSem(void);

void AudioPlay_DataProc(uint16_t* buff,uint16_t num);
AudioPlayRes MusicPlayingProcess(void);
void* AudioPlay_GetCurrentBuff(void);
uint8_t AudioPlay_Config(uint8_t Bits,uint32_t SampleRate,uint16_t BufSize);

void AudioPlay_Init(void);
void Play_Start(void);
void Play_Stop(void);
void Audio_StopPlay(void);

s32 AudioPlayFile(char* path);

#endif
