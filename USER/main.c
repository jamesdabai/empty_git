#include  <includes.h>
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "stm32f4xx.h"

#include "dev_uart.h"

#include "os_app_hooks.h"
#include "sdio_sdcard.h"
#include "dma.h"
#include "malloc.h" 
#include "exfuns.h"    
#include "audio_common.h"






#define MAIN_DEBUG
//#undef MAIN_DEBUG

#ifdef MAIN_DEBUG
    #define main_printf uart_printf
#else
    #define main_printf //
#endif

static  OS_TCB   AppTaskStartTCB;
static  CPU_STK  AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];
static  void  AppTaskStart          (void     *p_arg);


static  OS_TCB   SystemTaskStartTCB;//��ϵͳ���������񣬱������
static  CPU_STK  SystemTaskStartStk[SYS_CFG_TASK_START_STK_SIZE];
static  void  SystemTaskStart (void *p_arg);//��������

static  OS_TCB   MusicTaskStartTCB;
static  CPU_STK  MusicTaskStartStk[APP_CFG_TASK_START_STK_SIZE];
static  void  MusicTaskStart          (void     *p_arg);

static  void  AppTaskCreate         (void);
static  void  AppObjCreate          (void);



//�����ʵ�� -�⺯���汾
//STM32F4����ģ��-�⺯���汾
//�Ա����̣�http://mcudev.taobao.com
void show_sdcard_info(void)
{
	switch(SDCardInfo.CardType)
	{
		case SDIO_STD_CAPACITY_SD_CARD_V1_1:printf("Card Type:SDSC V1.1\r\n");break;
		case SDIO_STD_CAPACITY_SD_CARD_V2_0:printf("Card Type:SDSC V2.0\r\n");break;
		case SDIO_HIGH_CAPACITY_SD_CARD:printf("Card Type:SDHC V2.0\r\n");break;
		case SDIO_MULTIMEDIA_CARD:printf("Card Type:MMC Card\r\n");break;
	}	
  	printf("Card ManufacturerID:%d\r\n",SDCardInfo.SD_cid.ManufacturerID);	//������ID
 	printf("Card RCA:%d\r\n",SDCardInfo.RCA);								//����Ե�ַ
	printf("Card Capacity:%d MB\r\n",(u32)(SDCardInfo.CardCapacity>>20));	//��ʾ����
 	printf("Card BlockSize:%d\r\n\r\n",SDCardInfo.CardBlockSize);			//��ʾ���С
}
//ͼƬ��ʾ ʵ��-�⺯���汾 
//�õ�path·����,Ŀ���ļ����ܸ���
//path:·��		    
//����ֵ:����Ч�ļ���
u16 pic_get_tnum(u8 *path)
{	  
	u8 res;
	u16 rval=0;
 	DIR tdir;	 		//��ʱĿ¼
	FILINFO tfileinfo;	//��ʱ�ļ���Ϣ	
	u8 *fn;	 			 			   			     
    res=f_opendir(&tdir,(const TCHAR*)path); 	//��Ŀ¼
  	tfileinfo.lfsize=_MAX_LFN*2+1;				//���ļ�����󳤶�
	//tfileinfo.lfname=mymalloc(SRAMIN,tfileinfo.lfsize);//Ϊ���ļ������������ڴ�
	if(res==FR_OK&&tfileinfo.lfname!=NULL)
	{
		while(1)//��ѯ�ܵ���Ч�ļ���
		{
	        res=f_readdir(&tdir,&tfileinfo);       		//��ȡĿ¼�µ�һ���ļ�
	        if(res!=FR_OK||tfileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�		  
     		fn=(u8*)(*tfileinfo.lfname?tfileinfo.lfname:tfileinfo.fname);			 
			res=f_typetell(fn);	
			if((res&0XF0)==0X50)//ȡ����λ,�����ǲ���ͼƬ�ļ�	
			{
				rval++;//��Ч�ļ�������1
			}	    
		}  
	} 
	return rval;
}


int main(void)
{
    OS_ERR  err;
    u32 total,free;
    s32 ret;

    BSP_Init();////ʱ�����ã�APB����1:42,2:84Mhz��PLL���á��л���PLL168Mhz,/* Initialize BSP functions                             */
    CPU_Init();                /* Initialize the uC/CPU services                       */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
    uart_init(115200);//��ʼ����ӡ����
	
	uart_printf("enter core\n");
    LED_Init();		        //��ʼ��LED�˿�
    KEY_Init();//��ʼ����������̣�֧����ϰ���
	//dev_key_init();         //xqy
	//ddi_key_open();
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��  
    while(SD_Init())//��ⲻ��SD��
	{
		main_printf("SD Card Error!\n");
		delayms(500);					
		main_printf("Please Check! \n");
		delayms(500);	
		LED0=!LED0;//DS0��˸
	}
	show_sdcard_info();	//��ӡSD�������Ϣ
	exfuns_init();							//Ϊfatfs��ر��������ڴ�				 
  	f_mount(fs[0],"0:",1);                  //����sd����fatfsϵͳ��
    
  	while(exf_getfree("0",&total,&free))	//�õ�SD������������ʣ������
	{
		main_printf("SD Card Fatfs Error!");
		delayms(500);
		LED0=!LED0;//DS0��˸
	}	
    main_printf("SD Total Size:%d MB\n",(total>>10));
    main_printf("SD  Free Size: %d    MB\n",(free>>10));
    if(font_init() == 0)
    {
        uart_printf("open font succcse\n");
    }
    else
    {
        uart_printf("open font fail\n");
    }
    LCD_Init(1);//��ʼ��5.0�����
    OSInit(&err);    /* Init uC/OS-III.xqy�Ὣhook����ָ������                                      */
    main_printf("��ʼ��hook����\n");
    //App_OS_SetAllHooks();
    OSTaskCreate((OS_TCB       *)&AppTaskStartTCB,              /* Create the start task                                */
                 (CPU_CHAR     *)"App Task Start",
                 (OS_TASK_PTR   )AppTaskStart,
                 (void         *)0u,
                 (OS_PRIO       )APP_CFG_TASK_START_PRIO,
                 (CPU_STK      *)&AppTaskStartStk[0u],
                 (CPU_STK_SIZE  )AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE / 10u],
                 (CPU_STK_SIZE  )APP_CFG_TASK_START_STK_SIZE,
                 (OS_MSG_QTY    )0u,
                 (OS_TICK       )0u,
                 (void         *)0u,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
    OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */
    main_printf("��ʼ����\n");
    (void)&err;

    return (0u);
}



static  void  AppTaskStart (void *p_arg)
{
    
    OS_ERR      err;
    u32 key;
    u32 ret;
	(void)p_arg;//�����뾯��
	delay_init(168);		  //��ʼ����ʱ����//ϵͳsystick���������ʼ��	
#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif
    printf("\r\n���ڴ���ϵͳ����\r\n");
    OSTaskCreate((OS_TCB       *)&SystemTaskStartTCB,              /* Create the start task                                */
                 (CPU_CHAR     *)"System Task Start",
                 (OS_TASK_PTR   )SystemTaskStart,
                 (void         *)0u,
                 (OS_PRIO       )APP_CFG_TASK_START_PRIO,
                 (CPU_STK      *)&SystemTaskStartStk[0u],
                 (CPU_STK_SIZE  )SystemTaskStartStk[SYS_CFG_TASK_START_STK_SIZE / 10u],
                 (CPU_STK_SIZE  )SYS_CFG_TASK_START_STK_SIZE,
                 (OS_MSG_QTY    )0u,
                 (OS_TICK       )0u,
                 (void         *)0u,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&err);
    printf("\r\n����ϵͳ�������\r\n");
    
    printf("\r\n���ڴ������ֲ�������\r\n");
    OSTaskCreate((OS_TCB       *)&MusicTaskStartTCB,              /* Create the start task                                */
                (CPU_CHAR     *)"mp3 Task Start",
                (OS_TASK_PTR   )MusicTaskStart,
                (void         *)0u,
                (OS_PRIO       )MP3_CFG_TASK_START_PRIO,
                (CPU_STK      *)&MusicTaskStartStk[0u],
                (CPU_STK_SIZE  )MusicTaskStartStk[SYS_CFG_TASK_START_STK_SIZE / 10u],
                (CPU_STK_SIZE  )SYS_CFG_TASK_START_STK_SIZE,
                (OS_MSG_QTY    )0u,
                (OS_TICK       )0u,
                (void         *)0u,
                (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                (OS_ERR       *)&err);
    printf("\r\n�������ֲ����������\r\n");
    
    while(1)
	{
    	GPIO_ResetBits(GPIOA,GPIO_Pin_6);  //LED0��Ӧ����GPIOA.6���ͣ���  ��ͬLED0=0;
    	GPIO_SetBits(GPIOA,GPIO_Pin_7);   //LED1��Ӧ����GPIOA.7���ߣ��� ��ͬLED1=1;
    	OSTimeDlyHMSM(0u, 0u, 0u, 1000u,
                          OS_OPT_TIME_HMSM_STRICT,
                          &err);
    	GPIO_SetBits(GPIOA,GPIO_Pin_6);	   //LED0��Ӧ����GPIOA.6���ߣ���  ��ͬLED0=1;
    	GPIO_ResetBits(GPIOA,GPIO_Pin_7); //LED1��Ӧ����GPIOA.7���ͣ��� ��ͬLED1=0;
    	OSTimeDlyHMSM(0u, 0u, 0u, 1000u,
                          OS_OPT_TIME_HMSM_STRICT,
                          &err);
        ret = key_read(&key);
        if(key>0)
        {   
            disp_keyvalue(key);
        }
	}

}

static  void  SystemTaskStart (void *p_arg)
{
    OS_ERR      err;

    (void)p_arg;
    main_printf("����ϵͳ����ɨ������\n");
    while(1)
    {
        //drv_key_scan();//������IO�ں�����PE�г�ͻ�ˣ�������ʱ���ð���
       // dev_key_task();
        KEY_Scan(0); 
        OSTimeDlyHMSM(0u, 0u, 0u, 50u,
                          OS_OPT_TIME_HMSM_STRICT,
                          &err);
    }
}
extern u8 FileList[50][50];
extern AudioPlay_Info AudioPlayInfo;
extern u8* const father;

static  void  MusicTaskStart(void *p_arg)
{
    
    OS_ERR      err;
    u16 num,cnt = 0;
    s32 ret;
    char dir_x[50];
    u8 i;
    (void)p_arg;
    num = ReadDir("music");
    main_printf("�������ֲ�������\n");
    for(i=0;i<num;i++)
    {
        //main_printf("%d.%s\n",i,(u8 *)FileList[i]);
        lcd_show(i,4,(u8 *)FileList[i],24,1);
        delayms(20);
    }
    music_play_init(num);//��ʼ���������ſ��ƽṹ��
    AudioPlay_Init();
	while(1)
	{
	    uart_printf("�ڴ� used= %d\n",my_mem_perused(0));
		sprintf(dir_x,"%s/%s","music",FileList[cnt]);
		AudioPlayFile(dir_x);
		ret = AudioPlayInfo.PlayRes;//һ�׸������������
		if(ret == AudioPlay_Exit)
		{
		    break;
		}
		else
		{
		    cnt = music_ctl.music_num;
		}
		#if 0 /*xqy 2018-6-18*/
		if(music_ctl.music_num == num-1)
		{
		    Lcd_ColorBox(0,0,480,854,0X07FF);
		    lcd_show_text(0,50,father,24,1);
		}
		if(AudioPlayInfo.PlayRes == AudioPlay_Next)
		{
			cnt ++;
			if(cnt >= num)
				cnt = 0;
		}
		else if(AudioPlay_Prev == AudioPlayInfo.PlayRes)
		{
			if(cnt != 0)
				cnt --;
			else
				cnt = num-1;
		}
		#endif
	}
    OSTaskDel(&MusicTaskStartTCB, &err);
    main_printf("ɾ������������ƿ����err==%d\n",err);

}

 



