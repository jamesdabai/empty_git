#include <string.h>
#include "audio_mp3_rom.h"
#include "mp3dec.h"
//#include "common.h"

#define BigEndianTab2LittleEndianInt(tab) (((unsigned int)(tab)[0]<<24) + ((unsigned int)(tab)[1]<<16) +((unsigned int)(tab)[2]<<8) + ((tab)[3]))

//��ȡMP3������Ϣ
//pname:MP3�ļ�·��
//pctrl:MP3������Ϣ�ṹ�� 
//����ֵ:0,�ɹ�
//    ����,ʧ��
AudioPlayRes MP3_GetInfo_ROM(uint32_t addr,MP3_Info* pMP3info)
{
	MP3FrameInfo frame_info;
	HMP3Decoder decoder;
	MP3_FrameXing* fxing;
	MP3_FrameVBRI* fvbri;
	AudioPlayRes res = AudioPlay_OK;
	
	int offset = 0;//֡ͬ�������������ͷ��ƫ��
	unsigned int br;
	short samples_per_frame;//һ֡�Ĳ�������
	unsigned int p;//����ʶ��VBR���ݿ��ָ��
	unsigned int filesize;//�ļ���С
	unsigned int totframes;//��֡��
	uint8_t* readaddr = (uint8_t *)addr;
	
	decoder = MP3InitDecoder();//MP3���������ڴ�
	offset = MP3FindSyncWord(readaddr,br);//����֡ͬ����Ϣ
	if(offset >= 0 && MP3GetNextFrameInfo(decoder,&frame_info,&readaddr[offset])==0)//�ҵ�֡ͬ����Ϣ����һ֡��Ϣ��ȡ����	
	{
		p = offset+4+32;//����֡ͷ(4bytes)��ͨ����Ϣ(32bytes) ƫ�Ƶ�������
		fvbri = (MP3_FrameVBRI*)(readaddr+p);//VBR�ļ�ͷλ�ڵ�һ����Ч֡��������
		
		if(!strncmp("VBRI",(char*)fvbri->id,4))//����VBRI֡
		{
			if(frame_info.version == MPEG1)
			{
				samples_per_frame = 1152;//MPEG1,layer3ÿ֡����������1152
			}
			else
			{
				samples_per_frame = 576;//MPEG2/MPEG2.5,layer3ÿ֡����������576
			}
			
			totframes = BigEndianTab2LittleEndianInt(fvbri->frames);//�õ���֡��
			pMP3info->TotalSec = totframes * samples_per_frame / frame_info.samprate;//�õ��ļ��ܳ��� ��֡������ÿ֡�Ĳ��������Բ�����
		}
		else//����VBRI֡,�����ǲ���Xing֡����Info֡
		{
			if(frame_info.version==MPEG1)//MPEG1 
			{
				p = (frame_info.nChans==2) ? 32:17;//����������ȷ��ƫ����
				samples_per_frame = 1152;//MPEG1,layer3ÿ֡����������1152
			}
			else
			{
				p = (frame_info.nChans==2) ? 17:9;
				samples_per_frame = 576;//MPEG2/MPEG2.5,layer3ÿ֡����������576
			}
			
			p += (offset+4);
			fxing = (MP3_FrameXing*)(readaddr + p);
			
			if(!strncmp("Xing",(char*)fxing->id,4) || !strncmp("Info",(char*)fxing->id,4))//��Xing֡����Info֡
			{
				if(fxing->flags[3] & 0x01)//������֡���ֶ�
				{
					totframes = BigEndianTab2LittleEndianInt(fxing->frames);//�õ���֡��
					pMP3info->TotalSec = totframes * samples_per_frame / frame_info.samprate;//�õ��ļ��ܳ��� ��֡������ÿ֡�Ĳ��������Բ�����
				}
				else//��������frames�ֶ�
				{
					pMP3info->TotalSec = filesize/(frame_info.bitrate/8);//ʹ���ļ���С���м���
				}
			}
			else//CBR��ʽ,ֱ�Ӽ����ܲ���ʱ��
			{
				pMP3info->TotalSec=filesize/(frame_info.bitrate/8);
			}
		}
		
		pMP3info->Bitrate = frame_info.bitrate;//�õ���ǰ֡������
		pMP3info->Samplerate = frame_info.samprate;//�õ�������
		pMP3info->Channels = frame_info.nChans;//������
		
		if(frame_info.nChans==2)
		{
			pMP3info->SampleSize = frame_info.outputSamps;//���PCM��������С
		}
		else
		{
			pMP3info->SampleSize = frame_info.outputSamps*2;//���PCM��������С,���ڵ�����MP3,ֱ��*2,����Ϊ˫�������
		}
	}
	else
	{
		res = AudioPlay_UnsupportedFormat;
	}
	
	MP3FreeDecoder(decoder);//�ͷ��ڴ�	
	return res;
}

AudioPlayRes MP3_Play_ROM(uint32_t addr)
{
	AudioPlayRes res = AudioPlay_OK;
	HMP3Decoder mp3decoder = NULL;
	uint8_t* readptr = (uint8_t *)addr;//MP3�����ָ��
	uint16_t* pOutputBuffer;//ָ�������������ָ��
	int32_t offset,bytesleft;//buffer��ʣ�����Ч����
	MP3_Info MP3Info;
	
	memset(&MP3Info,0,sizeof(MP3_Info));
	
	res = MP3_GetInfo_ROM(addr,&MP3Info);//��ȡ�ļ���Ϣ
		
	if(!res)
	{
		AudioPlayInfo.BufferSize = MP3Info.SampleSize*2;
		
		//if(AudioPlay_I2SConfig(16,MP3Info.Samplerate,MP3Info.SampleSize))//����I2S
		{
			res = AudioPlay_UnsupportedParameter;
		}
	}
	
	if(res == AudioPlay_OK)
	{
		mp3decoder = MP3InitDecoder();//MP3��������ʼ��
		
		Play_Start();
		while(res == AudioPlay_OK)//û�г��������쳣(���ɷ��ҵ�֡ͬ���ַ�)
		{
			offset = MP3FindSyncWord(readptr,MP3_INPUT_BUFFER_SIZE);//��readptrλ��,��ʼ����ͬ���ַ�
			
			if(offset < 0)//û���ҵ�ͬ���ַ� ���Ž���Ҳ�����˴�����ѭ��
			{
				res = AudioPlay_PlayEnd;
				break;//����֡����ѭ��
			}
			
			else//�ҵ�ͬ���ַ���
			{
				readptr += offset;//MP3��ָ��ƫ�Ƶ�ͬ���ַ���
				bytesleft = MP3_INPUT_BUFFER_SIZE;
				
				AudioPlay_PendSem();
				pOutputBuffer = AudioPlay_GetCurrentBuff();//����õ�ǰ���л������ĵ�ַ
				
				if(MP3Decode(mp3decoder,&readptr,&bytesleft,(short*)pOutputBuffer,0))//����һ֡MP3���� ֱ�ӽ��뵽I2S�Ļ������� ��Լ�ڴ�
				{
					res = AudioPlay_DamagedFile;//���������� ����֡����ѭ��
				}
				AudioPlay_DataProc(pOutputBuffer,MP3Info.SampleSize);
				
				if(MP3Info.Channels == 1)//����ǵ�����
				{
					MonoChannelFill(pOutputBuffer,MP3Info.SampleSize/2);//���������Ϊ������
				}
			}
		}
	}
	
	Play_Stop();
	
	return res;
}
