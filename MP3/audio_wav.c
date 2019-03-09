//#include "common.h"
#include "audio_wav.h"
#include <string.h>

AudioPlayRes WAV_GetInfo(FIL* file,Wav_Info* info)
{
	uint8_t buf[64];
	uint32_t br;
	uint32_t fptr = 0;
	
	ChunkFMT *fmt;
	ChunkHDR *header;
	
	f_lseek(file,0);
	f_read(file,buf,sizeof(buf),&br);//��ȡ32�ֽ�
	
	fptr = 12;//�ʼ��RIFF�� 12bytes
	
	if(((ChunkRIFF*)buf)->ChunkID == 0x46464952 && ((ChunkRIFF*)buf)->Format == 0x45564157)//�����IDΪRIFF ��ʽ��WAVE
	{
		fmt = (ChunkFMT*)(buf + 12);//��ȡFMT��
		if(fmt->ChunkID == 0x20746D66)//�����FMT��
		{
			info->AudioFormat=fmt->AudioFormat;//��Ƶ��ʽ
			info->nChannels=fmt->NumOfChannels;//ͨ����
			info->Samplerate=fmt->SampleRate;//������
			info->Bitrate=fmt->ByteRate*8;//�õ�λ��
			info->BlockAlign=fmt->BlockAlign;//�����
			info->Bps=fmt->BitsPerSample;//λ�� 16/24/32λ
			uart_printf("��Ƶ��ʽ==%04x\n",info->AudioFormat);
			uart_printf("ͨ����==%d\n",info->nChannels);
			uart_printf("������==%d\n",info->Samplerate);
			uart_printf("�õ�λ��==%d\n",info->Bitrate);
			uart_printf("�����==%d\n",info->BlockAlign);
			uart_printf("λ��==%d\n",info->Bps);
		}
		else
		{
			return AudioPlay_UnsupportedFormat;//�ļ���ʽ����WAVE
		}
		
		fptr += fmt->ChunkSize + 8;//�����ļ�ͷ
		
		while(1)
		{
			f_lseek(file,fptr);
			f_read(file,buf,sizeof(buf),&br);//��ȡ32�ֽ�
			header = (ChunkHDR*) buf;
			
			if(header->ChunkID == 0x61746164)
			{
				info->DataStartOffset = fptr + 8;//����ͷ
				break;//�ҵ�������
			}
			
			fptr += header->ChunkSize + 8;//����Ѱ����һ�� �������ļ�ͷ�Ĵ�С
			
			if(fptr > 4096)//����4k��û���ҵ�
				return AudioPlay_UnsupportedFormat;//�ļ���ʽ����WAVE
		}
	}
	else
	{
		return AudioPlay_UnsupportedFormat;//����RIFF�ļ�
	}
	uart_printf("����ƫ��==%d\n",info->DataStartOffset);
	return AudioPlay_OK;
}

AudioPlayRes WAV_Play(char* path)
{
	AudioPlayRes res = AudioPlay_OK;
	Wav_Info __info;
	Wav_Info* WavInfo = NULL;
	FIL* Wav_File = NULL;
	UINT br = 0xFFFF;
	u8 exit_flag=0;
	
	Wav_File = &AudioFile;
	WavInfo = &__info;
	
	if(f_open(Wav_File,path,FA_READ))//���ļ�
	{
		res =  AudioPlay_OpenFileError;//���ļ�����
		uart_printf("���ļ�����\n");
	}
	else//�򿪳ɹ�
	{
		res = WAV_GetInfo(Wav_File,WavInfo);//��ȡ�ļ���Ϣ
		
		if(res == AudioPlay_OK)
		{			
			AudioPlayInfo.FileType = AudioFileType_WAV;
			AudioPlayInfo.Channels = WavInfo->nChannels;
			
			AudioPlayInfo.Samplerate = WavInfo->Samplerate;
			AudioPlayInfo.Bitrate = WavInfo->Bitrate/1000;
			AudioPlayInfo.TotalSec = (f_size(Wav_File) - WavInfo->DataStartOffset) / (WavInfo->Bitrate / WavInfo->Bps * WavInfo->nChannels);
			AudioPlayInfo.BufferSize = DAC_Buffer_Size * 2;

			AudioPlayInfo.Flags |= AUDIO_FLAG_INFO_READY;
			//AudioPlay_Init();//xqy��ӣ�ADCѡ��ʱ��ΪADCת������Դ
			if(AudioPlay_Config(WavInfo->Bps,WavInfo->Samplerate,DAC_Buffer_Size))//����DMA������
			{
				res = AudioPlay_UnsupportedParameter;//���ݸ�ʽ��֧��
			}
		}
	}
	
	if(res == AudioPlay_OK)
	{
		f_lseek(Wav_File,WavInfo->DataStartOffset);//��λ��PWM���ݵĿ�ʼ�ط�
		//Play_Start();
		Audio_first_play(Wav_File);
	}
	
	while(res == AudioPlay_OK)
	{
		if(br < DAC_Buffer_Size * 2)//�����ļ���
		{
			//res = music_end_process();//һ�׸������������
			uart_printf("���Ž���\n");
			break;
		}
		
		AudioPlayInfo.CurrentSec = (Wav_File->fptr - WavInfo->DataStartOffset) / (WavInfo->Bitrate / WavInfo->Bps * WavInfo->nChannels);//���㲥��ʱ��
		
		/*�û������� ��ʼ*/
		res = MusicPlayingProcess();
		if(res)
		{
		    exit_flag = 1;
		    break;
		}
			
		/*�û������� ����*/
		
		//uart_printf("time = %d\n",AudioPlayInfo.CurrentSec);
		AudioPlay_PendSem();
		//uart_printf("read file\n");
		f_read(Wav_File,(unsigned char*)AudioPlay_GetCurrentBuff(),DAC_Buffer_Size*4/2,&br);//��仺����
		AudioPlay_DataProc(AudioPlay_GetCurrentBuff(),DAC_Buffer_Size);

	}
	if(!exit_flag)
	{
	    res = music_end_process();//һ�׸������������;
	}
	uart_printf("�ڴ� used= %d\n",my_mem_perused(0));
	Play_Stop();
	f_close(Wav_File);//�ر��ļ�
	return res;
}
