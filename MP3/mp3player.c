#include "mp3player.h" 

#define DIR_MAX_ITEM 50  //128
#define MUSIC_DEFAULT_DIR "music"
    
//FATFS fatfs;
char FileList[DIR_MAX_ITEM][50];
const char* MUSIC_EXTNAME_FILTER[] = {"MP3","WAV",""};
    
extern uint8_t buf_fin;
u8 change_flag = 0;//ÿ����һ��buff���������ʱ�Ϳ�ʼ�����ݲ�ת��

static const  ID3V2 id3v2[] =
{
    "TEXT", "�������",
    "TENC", "����",
    "WXXX", "URL����(URL)",
    "TCOP", "��Ȩ(Copyright)",
    "TOPE", "ԭ������",
    "TCOM", " ������",
    "TDAT", "����",
    "TPE3", "ָ����",
    "TPE2", "�ֶ�",
    "TPE1", "�������൱��ID3v1��Artist",
    "TPE4", "���루��¼Ա���޸�Ա��",
    "TYER", "����൱��ID3v1��Year",
    "USLT", "���",
    "TALB", "ר���൱��ID3v1��Album",
    "TIT1", "����������",
    "TIT2", "�����൱��ID3v1��Title",
    "TIT3", "������",
    "TCON", "���ɣ����",
    "TBPM", "ÿ���ӽ�����",
    "COMM", "ע���൱��ID3v1��Comment",
    "TDLY", "�����б�¼",
    "TRCK", "���죨���ţ�",
    "TFLT", "����",
    "TENC", "�ļ�����",
    "TIME", "ʱ�䡡",
    "TKEY", "����ؼ���",
    "TLAN", "����",
    "TLEN", "����",
    "TMED", "ý������",
    "TOAL", "ԭ��Ƭ��",
    "TOFN", "ԭ�ļ���",
    "TOLY", "ԭ�������",
    "TORY", "�ļ������ߣ����֤�ߣ�",
    "TPOS", "��Ʒ������",
    "TPUB", "������",
    "TRDA", "¼������",
    "TRSN", "Intenet��̨����",
    "TRSO", "Intenet��̨������",
    "TSIZ", "��С",
    "TSRC", "ISRC�����ʵı�׼��¼���룩",
    "TSSE", "����ʹ�õ������Ӳ�����ã�",
    "UFID", "Ψһ���ļ���ʶ��",
    "TENC", "����",
    "AENC", "��Ƶ���ܼ���",
    "TXXX", "�����ı����ݵ�",
    "APIC", "����СͼƬ�ı�ǩ"
};

u8 CheckExtName(const char* path,const char** extname)
{
    uint8_t i = 0;
    char* ch = strrchr(path,'.');
    
    if(ch == NULL)
        return 0;
    
    if(*extname[0] == '\0')
        return 1;//δָ����չ�� ��������
    
    while(*extname[i] != '\0')
    {
        if(!strncasecmp(ch+1,extname[i],strlen(extname[i])))
            return 1;
        i ++;
    }
    
    return 0;
}

//struct ID3V2_HEAD ID3V2_head;
unsigned char ReadDir(char* dir_str)
{
	uint16_t cnt = 0;
	DIR dir;
	FILINFO fileinfo;
	FRESULT res;
	
	#if _USE_LFN
    fileinfo.lfsize = _MAX_LFN * 2 + 1;
    fileinfo.lfname = (u8 *)mymalloc(fileinfo.lfsize);
    #endif 
	
	f_opendir(&dir,dir_str);
	
	res = f_readdir(&dir,&fileinfo);

	while((fileinfo.fname[0] && res == FR_OK))
	{
		if(fileinfo.fattrib & AM_DIR)
			goto NEXT;
		
		if(!CheckExtName(fileinfo.fname,MUSIC_EXTNAME_FILTER))
			goto NEXT;
		//uart_printf("%s,len=%d\n",fileinfo.lfname,fileinfo.lfsize);
		Str_Copy(&FileList[cnt][0],fileinfo.lfname);
		
		cnt ++;
		if (cnt >= DIR_MAX_ITEM)
			break;
		NEXT:
		res = f_readdir(&dir,&fileinfo);
	}
	myfree(fileinfo.lfname);
	f_closedir(&dir);
	
	return cnt;
}



/**
  * @brief  mp3_player ����mp3�ļ����롢����
  * @param  filename:Ҫ���ŵ��ļ�·��
  * @retval key
  */
u8 mp3_play_song(u8 *pname)
{
    #define read_buf_size 4096
    #define pcm_buf_each_size 4608	
//	/* MP3����ʱ�õ��ı��� */	
	int err, i, current_sample_rate = 0;	

    int				read_offset = 0;				/* ��ƫ��ָ��			*/
    int				bytes_left = 0;					/* ʣ���ֽ���			*/	
	unsigned long	Frames = 0;						/* mP3֡����			*/
	unsigned char	*read_ptr;						/* ������ָ��			*/
	HMP3Decoder		Mp3Decoder;						/* mp3������ָ��	  	*/	

	uint8_t bufflag=0;										//�����л�buffer�ı�־
	static MP3FrameInfo		Mp3FrameInfo;		//mP3֡��Ϣ      

	FIL* fmp3;
	u8 res,rval=0;
	u16 br;
	u8 key;
	u8 *data_read_buf;// read�ļ�������
	short *data_pcm_buf;
	short *data_pcm_buf2;// PCM�����壬ʹ������������ 
	char  *head_size;

	int frame_len;
	
	fmp3=(FIL*)mymalloc(sizeof(FIL));//�����ڴ�
	data_read_buf=(u8*)mymalloc(read_buf_size);		//�����ڴ�����
	data_pcm_buf=(short*)mymalloc(pcm_buf_each_size);
	data_pcm_buf2=(short*)mymalloc(pcm_buf_each_size);
	
	if(data_read_buf==NULL||data_pcm_buf==NULL||data_pcm_buf2==NULL||fmp3==NULL)
	{
	    return 0XFF;//�ڴ�����ʧ��.
	}
	    	
	res=f_open(fmp3,(const TCHAR*)pname,FA_READ);//���ļ�	
	printf("��%s�ļ�==%d \r\n",pname,res);
	if (res!=FR_OK)
	{
		printf("open file %s error \r\n",pname);	
		myfree(fmp3);			
		myfree(data_read_buf);	
		myfree(data_pcm_buf);	
		myfree(data_pcm_buf2);
		return 1;										//�ļ��޷��򿪣���ֹ���롣������һ��ѭ������ȡ��һ���ļ�
	}

	res = f_read(fmp3,data_read_buf,read_buf_size,(UINT*)&br);//����4096���ֽ�
	printf("��һ�ζ��� %d �ֽ� \r\n",br);
	
	//printf_format(data_read_buf,160);//��ӡ����
	if (res!=FR_OK)
	{
		printf("read file %s error \r\n",pname);	
		myfree(fmp3);			
		myfree(data_read_buf);	
		myfree(data_pcm_buf);	
		myfree(data_pcm_buf2);
		return 1;			//�ļ��޷��򿪣���ֹ���롣������һ��ѭ������ȡ��һ���ļ�
	}
	
	//�򿪳ɹ�
	//��ʼ��MP3������
	Mp3Decoder = MP3InitDecoder();//һ��������֯�Ľṹ�壬��̬�����ڴ�xqy
	if (Mp3Decoder==0)
	{
		printf("ERR_MP3_Allocate Buffers\r\n");
		myfree(fmp3);			
		myfree(data_read_buf);	
		myfree(data_pcm_buf);	
		myfree(data_pcm_buf2);
		return 1;										
	}	
	printf("memory_used_mp3_play:%d%%\r\n",my_mem_perused(0));
	read_ptr = data_read_buf;									//ָ��mp3������
	head_size = data_read_buf;//֡ͷ�����㳤�ȵ�ָ��
	//bytes_left = br;								//ʵ�ʶ�������������С��С

	frame_len = (head_size[0+6]&0x7f)*0x200000+(head_size[1+6]&0x7f)*0x4000+(head_size[2+6]&0x7f)*0x80+(head_size[3+6]&0x7f);
    uart_printf("֡ͷ����Ϊ%d\n",frame_len+10);
    
	MP3_ID3V2_disp(read_ptr);//��ӡID3V2��ǩ֡����Ϣ
    uart_printf("ͨ����ǩ֡��õ��ĳ���Ϊ%d\n",frame_len+10);

    f_lseek(fmp3, frame_len+10);//xqy��λ�����������ݴ�
    uart_printf("��ʱ���ļ�ָ��Ϊ%d\n",f_tell(fmp3));
    
	res = f_read(fmp3,data_read_buf,read_buf_size,(UINT*)&br);//����4096���ֽ�//�ٴζ������ݹ�����
	read_ptr = data_read_buf;
	bytes_left = br;	
	
	while(rval==0)
	{
	   // uart_printf("%d\n",change_flag);
	    //if(change_flag>=2)
	       // continue;//�����ݻ�û��������ʱ����ʱ���������������
		read_offset = MP3FindSyncWord(read_ptr, bytes_left);	//Ѱ��֡ͬ�������ص�һ��ͬ���ֵ�λ��//xqyFF FE��ÿ������֡�Ŀ�ͷ
		//printf("Ѱ��ͬ��֡\r\n");
		//printf("read_offset===%d\r\n",read_offset);
		if(read_offset < 0)																		//û���ҵ�ͬ����
		{
		    //printf("û���ҵ�FF FE��ͬ��֡\r\n");
			rval=1;
			break;																							//����ѭ��2���ص�ѭ��1	
		}
		read_ptr += read_offset;					//ƫ����ͬ���ֵ�λ��
		bytes_left -= read_offset;				//ͬ����֮������ݴ�С	
		if(bytes_left < 1024)							//��������
		{
		    //�������ݣ�ȷ��buff�������Ƶ���������ݴ���1024�ֽ�xqy
			/* ע������ط���Ϊ���õ���DMA��ȡ������һ��Ҫ4�ֽڶ���  */ //�ļ�ϵͳ��DMA��ȡ����������ֽڶ���
			i=(uint32_t)(bytes_left)&3;					
			//�ж϶�����ֽ�
			if(i) i=4-i;														//��Ҫ������ֽ�
			memcpy(data_read_buf+i, read_ptr, bytes_left);	//�Ӷ���λ�ÿ�ʼ����
			read_ptr = data_read_buf+i;										//ָ�����ݶ���λ��
			//���ֽڶ�����Ϊ���ļ�ϵͳ����ʱ�򣬶���������4�ֽڶ���
			res = f_read(fmp3, data_read_buf+bytes_left+i, read_buf_size-(bytes_left+i), (UINT*)&br);//��������
			//printf("��������\r\n");
			bytes_left += br;										//��Ч��������С
		}	

		//uart_printf("�ļ�ָ��=%d\n",f_tell(fmp3));
		if(bufflag)
		{
		    PA1_out_high();
			err = MP3Decode(Mp3Decoder, &read_ptr, &bytes_left, data_pcm_buf, 0);					//��ʼ���� ������mp3����ṹ�塢������ָ�롢��������С�������ָ�롢���ݸ�ʽ
            PA1_out_low();
			//printf("��һ������������\r\n");
			//printf_format(data_pcm_buf,4608);//��ӡ����
			//printf("ʣ���ֽ���=%d\r\n",bytes_left);
			change_flag++;
			
		}
		else
		{
		    PA1_out_high();
			err = MP3Decode(Mp3Decoder, &read_ptr, &bytes_left, data_pcm_buf2, 0);	
			PA1_out_low();
			//printf("�ڶ�������������\r\n");
			//printf_format(data_pcm_buf2,4608);//��ӡ����
			//printf("ʣ���ֽ���=%d\r\n",bytes_left);
			change_flag++;
		}
		if(Frames==0)
		{
		    //printf("��һ֡�����ڳ�ʼ��DMA\r\n");
		    set_buff(data_pcm_buf,data_pcm_buf2);//������PCM���ݵ�ָ���ַ��ֵ��DAC���Բ�����ȫ�ֱ���
		    Dac1_Init();
		    Dac2_Init();//��ʼ��DAC
		    TIM3_Int_Init(1,7619-1);//1906/2-1);//7619-1);//��ʼ����ʱ��44.1Khz//����22.05khz
		    
			//DMA_Double_Buf_Init((uint32_t)data_pcm_buf,(uint32_t)data_pcm_buf2,pcm_buf_each_size/2);//�ڵ�һ��λ�ã���ʼ��DMA
		}
		Frames++;
		if (err != ERR_MP3_NONE)									//������
		{
		    printf("������\r\n");
		    printf("return==%d\r\n",err);
			switch (err)
			{
				case ERR_MP3_INDATA_UNDERFLOW:
					printf("ERR_MP3_INDATA_UNDERFLOW\r\n");
					read_ptr = data_read_buf;
					res = f_read(fmp3, data_read_buf, read_buf_size, (UINT*)&br);
					bytes_left = br;
					break;
		
				case ERR_MP3_MAINDATA_UNDERFLOW:
					rval=1;	
					/* do nothing - next call to decode will provide more mainData */
					/*ʲô������ - ���������ý�����ṩ�����mainData */
					printf("ERR_MP3_MAINDATA_UNDERFLOW\r\n");
					break;
		
				default:
					rval=1;	
					printf("UNKNOWN ERROR:%d\r\n", err);
		
					// ������֡
					if (bytes_left > 0)
					{
						bytes_left --;
						read_ptr ++;
					}	
					break;
			}
		}
		else
		{
				MP3GetLastFrameInfo(Mp3Decoder, &Mp3FrameInfo);		//��ȡ������Ϣ		
				//printf("MP3�����ɹ�\r\n", err);
				//printf("��ȡ������Ϣ\r\n", err);

		    /* ���ݽ�����Ϣ���ò����� */
				if (Mp3FrameInfo.samprate != current_sample_rate)	//������ 
				{
					current_sample_rate = Mp3FrameInfo.samprate;

					printf(" \r\n Bitrate       %dKbps", Mp3FrameInfo.bitrate/1000);
				    printf(" \r\n Samprate      %dHz", current_sample_rate);
					printf(" \r\n BitsPerSample %db", Mp3FrameInfo.bitsPerSample);
					printf(" \r\n nChans        %d", Mp3FrameInfo.nChans);
					printf(" \r\n Layer         %d", Mp3FrameInfo.layer);
					printf(" \r\n Version       %d", Mp3FrameInfo.version);
					printf(" \r\n OutputSamps   %d\r\n", Mp3FrameInfo.outputSamps);
					if((Mp3FrameInfo.bitsPerSample==16)&&(Mp3FrameInfo.nChans==2)&&(current_sample_rate>44000)&&(current_sample_rate<48100)&&(Mp3FrameInfo.outputSamps==2304))
					{
					    //DMA_set_size(Mp3FrameInfo.outputSamps);
					    //outputSamps==2304��������ÿ֡���ݹ̶��ǲ���1152�Σ��涨�õģ���˫�����Ļ�ÿ�β����൱�ڲ�������
					    //������������������������1152�β����൱��2304�β�����ÿ�������Ĳ���λ������16λ����λ��ǰ����λ�ں�
					    printf("DMA ���óߴ�\r\n", err);
					}
					else
					{
					    rval=1;
					    printf("�������ԣ�����ѭ��\r\n", err);
					}
				}		
				
				#if 0 /*xqy 2018-1-27*/
				while(buf_fin==0||play_state==stop)
				{
					key=KEY_Scan(0);
					switch(key)
					{
						case KEY1_PRES:
							rval=1;		//��һ��
							break;
						case KEY0_PRES:
							if(play_state==play)
							{
								play_state=stop;
								printf("stop\r\n");
								stop_buf=Disable_DMA();
							}
							else
							{
								play_state=play;
								printf("play\r\n");
								if(stop_buf==bufflag)
								{
									DMA_Double_Buf_Init((uint32_t)data_pcm_buf,(uint32_t)data_pcm_buf2,pcm_buf_each_size/2);
								}
								else
								{
									DMA_Double_Buf_Init((uint32_t)data_pcm_buf2,(uint32_t)data_pcm_buf,pcm_buf_each_size/2);
								}
							}
							break;						
						case WKUP_PRES:
							rval=2;		//��һ��
							break;
					}
				}	
				#endif
			//buf_fin=0;
			bufflag = 1 -bufflag;																			//�л�buffer
			//printf("�л�buffer\r\n", err);
		}
	}
	printf("MP3���Ž���\r\n", err);
	f_close(fmp3);
	//Disable_DMA();
	TIM3_Deinit();
	MP3FreeDecoder(Mp3Decoder);	
	myfree(fmp3);			
	myfree(data_read_buf);	
	myfree(data_pcm_buf);	
	myfree(data_pcm_buf2);	
	return rval;
}







//�õ�path·����,Ŀ���ļ����ܸ��� wav
////path:·��		    
////����ֵ:����Ч�ļ���
//u16 wav_get_tnum(u8 *path)
//{	  
//	u8 res;
//	u16 rval=0;
// 	DIR tdir;	 		//��ʱĿ¼
//	FILINFO tfileinfo;	//��ʱ�ļ���Ϣ		
//	u8 *fn; 			 			   			     
//    res=f_opendir(&tdir,(const TCHAR*)path); //��Ŀ¼
//  	tfileinfo.lfsize=_MAX_LFN*2+1;						//���ļ�����󳤶�
//	tfileinfo.lfname=mymalloc(tfileinfo.lfsize);	//Ϊ���ļ������������ڴ�
//	if(res==FR_OK&&tfileinfo.lfname!=NULL)
//	{
//		while(1)//��ѯ�ܵ���Ч�ļ���
//		{
//	        res=f_readdir(&tdir,&tfileinfo);       		//��ȡĿ¼�µ�һ���ļ�
//	        if(res!=FR_OK||tfileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�		  
//     		fn=(u8*)(*tfileinfo.lfname?tfileinfo.lfname:tfileinfo.fname);			 
//			res=f_typetell(fn);	
//			if(res==0X4A)//ȡ����λ,�����ǲ��������ļ�	
//			{
//				rval++;//��Ч�ļ�������1
//			}	    
//		}  
//	} 
//	myfree(tfileinfo.lfname);
//	return rval;
//}



//��������  wav
//void mp3_play(void)
//{
//	u8 res;
// 	DIR mp3dir;	 		//Ŀ¼
//	FILINFO mp3fileinfo;//�ļ���Ϣ
//	u8 *fn;   			//���ļ���
//	u8 *pname;			//��·�����ļ���
//	u16 totmp3num; 		//�����ļ�����
//	u16 curindex;		//ͼƬ��ǰ����
//	u8 key;				//��ֵ		  
// 	u16 temp;
//	
//	u16 *mp3indextbl;	//���������� 
//	
//	u16 *wavindextbl;	//wav������ 
//	u16 wav_num;
//	u16 totwavnum;//wav tol
//	
// 	while(f_opendir(&mp3dir,"0:/����"))//��ͼƬ�ļ���
// 	{	    
//		printf("MUSIC_fl_err");			  
//	} 									  
//	totmp3num=mp3_get_tnum("0:/����"); //�õ�����Ч�ļ���
//  	while(totmp3num==NULL)//�����ļ�����Ϊ0		
// 	{	    
//		printf("0 music");						  
//	}	
//	totwavnum=wav_get_tnum("0:/����");
//	
//  mp3fileinfo.lfsize=_MAX_LFN*2+1;						//���ļ�����󳤶�
//	mp3fileinfo.lfname=mymalloc(mp3fileinfo.lfsize);	//Ϊ���ļ������������ڴ�
// 	pname=mymalloc(mp3fileinfo.lfsize);				//Ϊ��·�����ļ��������ڴ�
// 	mp3indextbl=mymalloc(2*totmp3num);				//����2*totmp3num���ֽڵ��ڴ�,���ڴ�������ļ�����
//	wavindextbl=mymalloc(2*totwavnum);

// 	while(mp3fileinfo.lfname==NULL||pname==NULL||mp3indextbl==NULL)//�ڴ�������
// 	{	    
//			printf("mem_err");	
//	}  
//	
//	
//	res=f_opendir(&mp3dir,"0:/����"); //��Ŀ¼
//	if(res==FR_OK)
//	{
//		curindex=0;//��ǰ����Ϊ0
//		while(1)//ȫ����ѯһ��
//		{
//			temp=mp3dir.index;								//��¼��ǰindex
//	        res=f_readdir(&mp3dir,&mp3fileinfo);       		//��ȡĿ¼�µ�һ���ļ�
//	        if(res!=FR_OK||mp3fileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�		  
//     		fn=(u8*)(*mp3fileinfo.lfname?mp3fileinfo.lfname:mp3fileinfo.fname);			 
//			res=f_typetell(fn);	
//			if((res&0XF0)==0X40)//ȡ����λ,�����ǲ��������ļ�	
//			{
//				mp3indextbl[curindex]=temp;//��¼����
//				curindex++;
//				mp3_index_show(curindex,totmp3num);
//				puts((const char*)fn);
//				printf("\r\n");
//				if(res==0X4A)
//				{
//					wavindextbl[wav_num]=temp;
//					printf("wav");
//					wav_num++;
//					mp3_index_show(wav_num,totwavnum);					
//					puts((const char*)fn);					
//					printf("\r\n");
//				}										
//			}					
//		} 
//	}   
//	printf("memory_used:%d%%\r\n",mem_perused());
//	
//	wav_num=0;
//	res=f_opendir(&mp3dir,(const TCHAR*)"0:/����"); 	//��Ŀ¼
//	while(res==FR_OK)//�򿪳ɹ�
//	{	
//		dir_sdi(&mp3dir,wavindextbl[wav_num]);			//�ı䵱ǰĿ¼����	   
//		res=f_readdir(&mp3dir,&mp3fileinfo);       		//��ȡĿ¼�µ�һ���ļ�
//		if(res!=FR_OK||mp3fileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�
//		fn=(u8*)(*mp3fileinfo.lfname?mp3fileinfo.lfname:mp3fileinfo.fname);	
//		mp3_index_show(wav_num+1,totwavnum);		 
//		printf("now playing->");
//		puts((const char*)fn);					//��ʾ�������� 
//		printf("\r\n");				 		
//		
//		strcpy((char*)pname,"0:/����/");				//����·��(Ŀ¼)
//		strcat((char*)pname,(const char*)fn);  			//���ļ������ں���
//		
//		if(wav_decode_init(pname,&wavctrl)==0)//�õ��ļ�����Ϣ,��Ϊwav
//		{
//			if((wavctrl.bps==16)&&(wavctrl.nchannels==2)&&(wavctrl.samplerate>44000)&&(wavctrl.samplerate<48100))
//			{
//				key=wav_play_song(pname); 				 		//�������MP3  
//			}
//			else
//			{
//				key=1;
//				printf("wav_file_err 1\r\n");				
//			}
//		}
//		else
//		{
//			key=1;
//			printf("wav_file_err 2\r\n");
//		}			
//		if(key==2)		//��һ��
//		{
//			if(wav_num)wav_num--;
//			else wav_num=totwavnum-1;
// 		}else if(key==1)//��һ��
//		{
//			wav_num++;		   	
//			if(wav_num>=totwavnum)wav_num=0;//��ĩβ��ʱ��,�Զ���ͷ��ʼ
// 		}else break;	//�����˴��� 	 
//	} 						
//	
//	myfree(mp3fileinfo.lfname);	//�ͷ��ڴ�			    
//	myfree(pname);				//�ͷ��ڴ�			    
//	myfree(mp3indextbl);			//�ͷ��ڴ�	 
//}






//
//u16 f_kbps=0;//�����ļ�λ��	
////��ʾ����ʱ��,������ ��Ϣ 
////lenth:�����ܳ���
//void mp3_msg_show(u32 lenth)
//{	
//	static u16 playtime=0;//����ʱ����	     
// 	u16 time=0;// ʱ�����
//	u16 temp=0;	  
//	if(f_kbps==0xffff)//δ���¹�
//	{
//		playtime=0;
//		f_kbps=VS_Get_HeadInfo();	   //��ñ�����
//	}	 	 
//	time=VS_Get_DecodeTime(); //�õ�����ʱ��
//	if(playtime==0)playtime=time;
//	else if((time!=playtime)&&(time!=0))//1sʱ�䵽,������ʾ����
//	{
//		playtime=time;//����ʱ�� 	 				    
//		temp=VS_Get_HeadInfo(); //��ñ�����	   				 
//		if(temp!=f_kbps)
//		{
//			f_kbps=temp;//����KBPS	  				     
//		}			 
//		//��ʾ����ʱ��			 
//		LCD_ShowxNum(30,210,time/60,2,16,0X80);		//����
//		LCD_ShowChar(30+16,210,':',16,0);
//		LCD_ShowxNum(30+24,210,time%60,2,16,0X80);	//����		
// 		LCD_ShowChar(30+40,210,'/',16,0); 	    	 
//		//��ʾ��ʱ��
//		if(f_kbps)time=(lenth/f_kbps)/125;//�õ�������   (�ļ�����(�ֽ�)/(1000/8)/������=����������    	  
//		else time=0;//�Ƿ�λ��	  
// 		LCD_ShowxNum(30+48,210,time/60,2,16,0X80);	//����
//		LCD_ShowChar(30+64,210,':',16,0);
//		LCD_ShowxNum(30+72,210,time%60,2,16,0X80);	//����	  		    
//		//��ʾλ��			   
//   		LCD_ShowxNum(30+110,210,f_kbps,3,16,0X80); 	//��ʾλ��	 
//		LCD_ShowString(30+134,210,200,16,16,"Kbps");	  	  
//		LED0=!LED0;		//DS0��ת
//	}   		 
//}			  		 
////�õ�path·����,Ŀ���ļ����ܸ���
////path:·��		    
////����ֵ:����Ч�ļ���
//u16 mp3_get_tnum(u8 *path)
//{	  
//	u8 res;
//	u16 rval=0;
// 	DIR tdir;	 		//��ʱĿ¼
//	FILINFO tfileinfo;	//��ʱ�ļ���Ϣ		
//	u8 *fn; 			 			   			     
//    res=f_opendir(&tdir,(const TCHAR*)path); //��Ŀ¼
//  	tfileinfo.lfsize=_MAX_LFN*2+1;						//���ļ�����󳤶�
//	tfileinfo.lfname=mymalloc(SRAMIN,tfileinfo.lfsize);	//Ϊ���ļ������������ڴ�
//	if(res==FR_OK&&tfileinfo.lfname!=NULL)
//	{
//		while(1)//��ѯ�ܵ���Ч�ļ���
//		{
//	        res=f_readdir(&tdir,&tfileinfo);       		//��ȡĿ¼�µ�һ���ļ�
//	        if(res!=FR_OK||tfileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�		  
//     		fn=(u8*)(*tfileinfo.lfname?tfileinfo.lfname:tfileinfo.fname);			 
//			res=f_typetell(fn);	
//			if((res&0XF0)==0X40)//ȡ����λ,�����ǲ��������ļ�	
//			{
//				rval++;//��Ч�ļ�������1
//			}	    
//		}  
//	} 
//	myfree(SRAMIN,tfileinfo.lfname);
//	return rval;
//}
////��������
//void mp3_play(void)
//{
//	u8 res;
// 	DIR mp3dir;	 		//Ŀ¼
//	FILINFO mp3fileinfo;//�ļ���Ϣ
//	u8 *fn;   			//���ļ���
//	u8 *pname;			//��·�����ļ���
//	u16 totmp3num; 		//�����ļ�����
//	u16 curindex;		//ͼƬ��ǰ����
//	u8 key;				//��ֵ		  
// 	u16 temp;
//	u16 *mp3indextbl;	//���������� 
// 	while(f_opendir(&mp3dir,"0:/MUSIC"))//��ͼƬ�ļ���
// 	{	    
//		Show_Str(30,190,240,16,"MUSIC�ļ��д���!",16,0);
//		delay_ms(200);				  
//		LCD_Fill(30,190,240,206,WHITE);//�����ʾ	     
//		delay_ms(200);				  
//	} 									  
//	totmp3num=mp3_get_tnum("0:/MUSIC"); //�õ�����Ч�ļ���
//  	while(totmp3num==NULL)//�����ļ�����Ϊ0		
// 	{	    
//		Show_Str(30,190,240,16,"û�������ļ�!",16,0);
//		delay_ms(200);				  
//		LCD_Fill(30,190,240,146,WHITE);//�����ʾ	     
//		delay_ms(200);				  
//	}										   
//  	mp3fileinfo.lfsize=_MAX_LFN*2+1;						//���ļ�����󳤶�
//	mp3fileinfo.lfname=mymalloc(SRAMIN,mp3fileinfo.lfsize);	//Ϊ���ļ������������ڴ�
// 	pname=mymalloc(SRAMIN,mp3fileinfo.lfsize);				//Ϊ��·�����ļ��������ڴ�
// 	mp3indextbl=mymalloc(SRAMIN,2*totmp3num);				//����2*totmp3num���ֽڵ��ڴ�,���ڴ�������ļ�����
// 	while(mp3fileinfo.lfname==NULL||pname==NULL||mp3indextbl==NULL)//�ڴ�������
// 	{	    
//		Show_Str(30,190,240,16,"�ڴ����ʧ��!",16,0);
//		delay_ms(200);				  
//		LCD_Fill(30,190,240,146,WHITE);//�����ʾ	     
//		delay_ms(200);				  
//	}  	
//	VS_HD_Reset();
//	VS_Soft_Reset();
//	vsset.mvol=220;						//Ĭ����������Ϊ220.
//	mp3_vol_show((vsset.mvol-100)/5);	//����������:100~250,��ʾ��ʱ��,���չ�ʽ(vol-100)/5,��ʾ,Ҳ����0~30   
//	//��¼����
//    res=f_opendir(&mp3dir,"0:/MUSIC"); //��Ŀ¼
//	if(res==FR_OK)
//	{
//		curindex=0;//��ǰ����Ϊ0
//		while(1)//ȫ����ѯһ��
//		{
//			temp=mp3dir.index;								//��¼��ǰindex
//	        res=f_readdir(&mp3dir,&mp3fileinfo);       		//��ȡĿ¼�µ�һ���ļ�
//	        if(res!=FR_OK||mp3fileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�		  
//     		fn=(u8*)(*mp3fileinfo.lfname?mp3fileinfo.lfname:mp3fileinfo.fname);			 
//			res=f_typetell(fn);	
//			if((res&0XF0)==0X40)//ȡ����λ,�����ǲ��������ļ�	
//			{
//				mp3indextbl[curindex]=temp;//��¼����
//				curindex++;
//			}	    
//		} 
//	}   
//   	curindex=0;											//��0��ʼ��ʾ
//   	res=f_opendir(&mp3dir,(const TCHAR*)"0:/MUSIC"); 	//��Ŀ¼
//	while(res==FR_OK)//�򿪳ɹ�
//	{	
//		dir_sdi(&mp3dir,mp3indextbl[curindex]);			//�ı䵱ǰĿ¼����	   
//        res=f_readdir(&mp3dir,&mp3fileinfo);       		//��ȡĿ¼�µ�һ���ļ�
//        if(res!=FR_OK||mp3fileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�
//     	fn=(u8*)(*mp3fileinfo.lfname?mp3fileinfo.lfname:mp3fileinfo.fname);			 
//		strcpy((char*)pname,"0:/MUSIC/");				//����·��(Ŀ¼)
//		strcat((char*)pname,(const char*)fn);  			//���ļ������ں���
// 		LCD_Fill(30,190,240,190+16,WHITE);				//���֮ǰ����ʾ
//		Show_Str(30,190,240-30,16,fn,16,0);				//��ʾ�������� 
//		mp3_index_show(curindex+1,totmp3num);
//		key=mp3_play_song(pname); 				 		//�������MP3    
//		if(key==2)		//��һ��
//		{
//			if(curindex)curindex--;
//			else curindex=totmp3num-1;
// 		}else if(key<=1)//��һ��
//		{
//			curindex++;		   	
//			if(curindex>=totmp3num)curindex=0;//��ĩβ��ʱ��,�Զ���ͷ��ʼ
// 		}else break;	//�����˴��� 	 
//	} 											  
//	myfree(SRAMIN,mp3fileinfo.lfname);	//�ͷ��ڴ�			    
//	myfree(SRAMIN,pname);				//�ͷ��ڴ�			    
//	myfree(SRAMIN,mp3indextbl);			//�ͷ��ڴ�	 
//}

////����һ��ָ���ĸ���				     	   									    	 
////����ֵ:0,�����������
////		 1,��һ��
////       2,��һ��
////       0XFF,���ִ�����
//u8 mp3_play_song(u8 *pname)
//{	 
// 	FIL* fmp3;
//    u16 br;
//	u8 res,rval;	  
//	u8 *databuf;	   		   
//	u16 i=0; 
//	u8 key;  	    
//			   
//	rval=0;	    
//	fmp3=(FIL*)mymalloc(SRAMIN,sizeof(FIL));//�����ڴ�
//	databuf=(u8*)mymalloc(SRAMIN,4096);		//����4096�ֽڵ��ڴ�����
//	if(databuf==NULL||fmp3==NULL)rval=0XFF ;//�ڴ�����ʧ��.
//	if(rval==0)
//	{	  
//	  	VS_Restart_Play();  					//�������� 
//		VS_Set_All();        					//������������Ϣ 			 
//		VS_Reset_DecodeTime();					//��λ����ʱ�� 	  
//		res=f_typetell(pname);	 	 			//�õ��ļ���׺	 			  	 						 
//		if(res==0x4c)//�����flac,����patch
//		{	
//			VS_Load_Patch((u16*)vs1053b_patch,VS1053B_PATCHLEN);
//		}  				 		   		 						  
//		res=f_open(fmp3,(const TCHAR*)pname,FA_READ);//���ļ�	 
// 		if(res==0)//�򿪳ɹ�.
//		{ 
//			VS_SPI_SpeedHigh();	//����						   
//			while(rval==0)
//			{
//				res=f_read(fmp3,databuf,4096,(UINT*)&br);//����4096���ֽ�  
//				i=0;
//				do//������ѭ��
//			    {  	
//					if(VS_Send_MusicData(databuf+i)==0)//��VS10XX������Ƶ����
//					{
//						i+=32;
//					}else   
//					{
//						key=KEY_Scan(0);
//						switch(key)
//						{
//							case KEY0_PRES:
//								rval=1;		//��һ��
//								break;
//							case KEY2_PRES:
//								rval=2;		//��һ��
//								break;
//							case WKUP_PRES:	//��������
//								if(vsset.mvol<250)
//								{
//									vsset.mvol+=5;
//						 			VS_Set_Vol(vsset.mvol);	
//								}else vsset.mvol=250;
//								mp3_vol_show((vsset.mvol-100)/5);	//����������:100~250,��ʾ��ʱ��,���չ�ʽ(vol-100)/5,��ʾ,Ҳ����0~30   
//								break;
//							case KEY1_PRES:	//������
//								if(vsset.mvol>100)
//								{
//									vsset.mvol-=5;
//						 			VS_Set_Vol(vsset.mvol);	
//								}else vsset.mvol=100;
//								mp3_vol_show((vsset.mvol-100)/5);	//����������:100~250,��ʾ��ʱ��,���չ�ʽ(vol-100)/5,��ʾ,Ҳ����0~30   
//								break;
//						}
//						mp3_msg_show(fmp3->fsize);//��ʾ��Ϣ	    
//					}	    	    
//				}while(i<4096);//ѭ������4096���ֽ� 
//				if(br!=4096||res!=0)
//				{
//					rval=0;
//					break;//������.		  
//				} 							 
//			}
//			f_close(fmp3);
//		}else rval=0XFF;//���ִ���	   	  
//	}						     	 
//	myfree(SRAMIN,databuf);	  	 		  	    
//	myfree(SRAMIN,fmp3);
//	return rval;	  	 		  	    
//}
int  exist_not(u8 *ptr)//�жϸñ�־�Ƿ���������������
{
    u8 i;
    uart_printf("����Ϊ%d\n",sizeof(id3v2)/sizeof(ID3V2));
    for(i=0;i<sizeof(id3v2)/sizeof(ID3V2);i++)
    {
        if(memcmp_x(ptr,(u8 *)id3v2[i].FrameID,4)==0)
            return i;
    }
    return -1;
}
void memcpy_x(u8 *d,u8 *s,int len)
{
    int i;
    for(i=0;i<len;i++)
    {
        *(d+i)=*(s+i);
    }
}

s32  memcmp_x(u8 *d,u8 *s,int len)
{
    //u16 d_strlen;
    //u16 s_strlen;
    u8 i;
    #if 0 /*xqy 2018-3-3*/
    d_strlen = strlen(d);
    s_strlen = strlen(s);
    if(d_strlen!=s_strlen)
    {
        return -1;
    }

    if(d_strlen > len)
    {
        return -1;
    }
    #endif
    
    for(i=0;i<len;i++)
    {
        if(*(d+i) != *(s+i))
            return -1;
    }
    return 0;
}


int MP3_ID3V2_disp(char *read_ptr)
{
    u8 i,j;
    u8 *ptr;
    u8 frame[5];
    int size=0;//������ǩ���ֽڴ�С
    u8  frame_num = 0;//��ǩ֡������
    s8  exist_flag;
    int size_x;
    u8  size_xqy[4];
    u8  frame_data[50];
    ptr = read_ptr+10;//ƫ������ͷ֡10�ֽ�

    memcpy_x((u8 *)frame,(u8 *)ptr,4);//���Ʊ�ǩ֡��־
    frame[4] = '\0';//�ַ�����������־��ô�ӡ��ʱ������
    for(i=0;i<4;i++)
    {
        size_xqy[i] = *(ptr+4+i);
    }
    uart_printf("��ǩ��Ϊ%s\n",frame);
    while(1)
    {
        exist_flag = exist_not((u8 *)frame);
        if(exist_flag==(-1))//����û�б�ǩ֡�˾�����ȥ�����ϵ�main_data��
            break;

        frame_num++;//����һ����ǩ֡
        
        ptr = ptr+10;//ƫ��������ǩ֡ͷ֡10�ֽ�
        size_x=(size_xqy[0])*0x100000+(size_xqy[1])*0x10000+(size_xqy[2])*0x100+(size_xqy[3]);//֡���ȣ���ȥͷ��10�ֽڣ�
        //�鵰�������ѵ�һƪ�������£���ȻҪ���Ӳ��ˣ���֡ͷ�ĳ��Ⱥ����ǩ֡ͷ�ĳ��Ȳ�һ����xqy
        if(size_x>50)
        {
            uart_printf("��ǩ֡���ݴ���50������ӡ����\n");
        }
        else
        {
            memset((u8 *)frame_data,0x00,50);//��ʽ������
            memcpy_x((u8 *)frame_data,(u8 *)ptr,size_x);//���Ʊ�ǩ֡����
            uart_printf("%s:%s(%s)\n",(u8 *)frame,(u8 *)(frame_data+1),(u8 *)id3v2[exist_flag].str);
            //ID3V2��ǩ����ʵ���ݵĵ�һ�ֽ����ı���־��Ϊ1ʱ�����������������Ϊ�ı����ݣ������������ģ���ʱ����⣩
            
        }
        
        
        ptr = ptr+size_x;//ƫ��������ǩ֡����x�ֽ�
        
        size += 10;//ͷ��Ϣ10�ֽ�
        size += size_x;//��¼��ǩ֡�����ܳ�
        //�ظ�ȡ��Ϣ
        if(ptr - read_ptr>4096)
        {
            uart_printf("��һ֡���ݳ���4096������\n");
            break;
        }
        memcpy_x((u8 *)frame,(u8 *)ptr,4);//���Ʊ�ǩ֡��־
        for(i=0;i<4;i++)
        {
            size_xqy[i] = *(ptr+4+i);
        }
    }
    uart_printf("��ǩ֡�����ܳ���Ϊ%d\n",size);
    return size;
}
int MP3_ID3V1_disp(char *read_ptr)
{
    
}

u8 wav_play_song(u8 *pname)
{
    
}









