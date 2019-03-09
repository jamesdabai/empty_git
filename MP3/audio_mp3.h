#ifndef __AUDIO_MP3_H__ 
#define __AUDIO_MP3_H__

#include "ff.h"
#include "audio_common.h"

#define MP3_INPUT_BUFFER_SIZE 4096//MP3����ʱ,�ļ�buf��С

typedef __packed struct//ID3V1 ��ǩ
{
	unsigned char id[3];			//ID,TAG������ĸ
	unsigned char title[30];	//��������
	unsigned char artist[30];	//����������
	unsigned char album[30];	//ר��������
	unsigned char year[4];		//���
	unsigned char comment[30];	//��ע
	unsigned char genre;			//���� 
}ID3V1_Tag;

typedef __packed struct//ID3V2 ��ǩͷ
{
    unsigned char id[3];		//ID
    unsigned char mversion;	//���汾��
    unsigned char sversion;	//�Ӱ汾��
    unsigned char flags;		//��ǩͷ��־
    unsigned char size[4];		//��ǩ��Ϣ��С(��������ǩͷ10�ֽ�) ��ǩ��С=size+10.
}ID3V2_TagHead;

//ID3V2.3 �汾֡ͷ
typedef __packed struct 
{
	unsigned char id[4];			//֡ID
	unsigned char size[4];		//֡��С
	unsigned char flags[2];		//֡��־
}ID3V23_TagFrameHead;

typedef __packed struct//MP3 Xing֡��Ϣ(û��ȫ���г���,���г����õĲ���)
{
	unsigned char id[4];			//֡ID,ΪXing/Info
	unsigned char flags[4];		//��ű�־
	unsigned char frames[4];	//��֡��
	unsigned char fsize[4];		//�ļ��ܴ�С(������ID3)
}MP3_FrameXing;
 
typedef __packed struct//MP3 VBRI֡��Ϣ(û��ȫ���г���,���г����õĲ���)
{
    unsigned char id[4];		//֡ID,ΪXing/Info
	unsigned char version[2];	//�汾��
	unsigned char delay[2];		//�ӳ�
	unsigned char quality[2];	//��Ƶ����,0~100,Խ������Խ��
	unsigned char fsize[4];		//�ļ��ܴ�С
	unsigned char frames[4];	//�ļ���֡�� 
}MP3_FrameVBRI;

typedef __packed struct//MP3���ƽṹ��
{
	unsigned char Channels;				//������
	unsigned int TotalSec;				//���׸�ʱ��,��λ:��
	unsigned int CurrentSec;			//��ǰ����ʱ��
	unsigned int Bitrate;	   		//������
	unsigned int Samplerate;			//������
	unsigned short SampleSize;			//PCM�����������С(��16λΪ��λ),������MP3,�����ʵ�����*2(����DAC���)
	unsigned int DataStartOffset;		//����֡��ʼ��λ��(���ļ������ƫ��)
	unsigned int DataSize;				//MP3��ȥ������Ϣ�Ժ��������
}MP3_Info;

void MonoChannelFill(uint16_t* buf,uint16_t size);
AudioPlayRes MP3_Play(char* path);

#endif
