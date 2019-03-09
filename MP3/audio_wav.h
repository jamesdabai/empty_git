#ifndef __AUDIO_WAV_H__
#define __AUDIO_WAV_H__

#include "ff.h"
#include "audio_common.h"

typedef __packed struct//RIFF��
{
	unsigned int ChunkID;//chunk id;����̶�Ϊ"RIFF",��0x46464952
	unsigned int ChunkSize ;//���ϴ�С;�ļ��ܴ�С-8
	unsigned int Format;//��ʽ;WAVE,��0x45564157
}ChunkRIFF;

typedef __packed struct//FMT��
{
	unsigned int ChunkID;//chunk id;����̶�Ϊ"fmt ",��0x20746D66
	unsigned int ChunkSize ;//�Ӽ��ϴ�С(������ID��Size)
	unsigned short AudioFormat;//��Ƶ��ʽ;0X01,��ʾ����PCM;0X11��ʾIMA ADPCM
	unsigned short NumOfChannels;//ͨ������ 1��ʾ������ 2��ʾ˫����
	unsigned int SampleRate;//������
	unsigned int ByteRate;//�ֽ�����
	unsigned short BlockAlign;//�����(�ֽ�)
	unsigned short BitsPerSample;//�����������ݴ�С;4λADPCM,����Ϊ4
}ChunkFMT;

typedef __packed struct//DATA��
{
	unsigned int ChunkID;//chunk id;����̶�Ϊ"data",��0x61746164
	unsigned int ChunkSize;//�Ӽ��ϴ�С(������ID��Size)
}ChunkHDR;

typedef __packed struct
{ 
	unsigned short AudioFormat;//��Ƶ��ʽ;0X01,��ʾ����PCM;0X11��ʾIMA ADPCM
	unsigned short nChannels;//ͨ������;1,��ʾ������;2,��ʾ˫����
	unsigned short BlockAlign;//�����(�ֽ�)

	unsigned int Bitrate;//������(λ��)
	unsigned int Samplerate;//������
	unsigned short Bps;//λ��,����16bit,24bit,32bit

	unsigned int DataStartOffset;//����֡��ʼ��λ��(���ļ������ƫ��)
}Wav_Info;

AudioPlayRes WAV_Play(char* path);

#endif
