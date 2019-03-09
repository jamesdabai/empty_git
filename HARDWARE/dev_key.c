/********************************Copyright ( c ) ***************************
**                  Shenzhen Xinguodu Technology Co., LTD.
**                        http://www.xinguodu.com
**
**
**Created By:         xqy
**Created Date:       2017-12-13
**Version:            <version >
**Description:        ��ֲ����������stm32��
**
****************************************************************************/



#include <stdarg.h>
#include <stdio.h>

#include "dev_key.h"
#include "stm32f4xx_gpio.h"
#include "dev_time.h"
#include "ddi.h"
#include "core_cm4.h"

#define Test_DRV_KEY
#ifdef Test_DRV_KEY
#define key_debug uart_printf
#else
#define key_debug(a,...)
#endif

struct _key_scan_ctr
{
    u32 old_sta;//������һ���ȶ�״̬
    u32 new_sta;
    u8 col;//��¼ɨ���COL
    u8 deb;//����
};

static struct _key_scan_ctr KeyScnCtr;

//����ɨ���״̬���壬
#define KEY_SCAN_STA_BUF_MAX 10
struct _key_scan_sta_buf
{
    struct _key_scan_sta chg[KEY_SCAN_STA_BUF_MAX];  
    u8 head;
    u8 end;
};

static struct _key_scan_sta_buf KeyChgBuf;

enum KEY_COL
{
    KEY_COL_0 = 0,
    KEY_COL_1,
    KEY_COL_2,
    KEY_COL_3,
    //KEY_COL_PWR,//ר������ɨ��PWR��
    KEY_COL_MAX,
};

struct _key_value key_value[6] = 
{
    "ȡ��",
    "���",
    "�Ϸ�",
    "�·�",
    "ȷ��",
    "���ػ�",
};


void key_init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOEʱ��

  //GPIOE7~E10��ʼ�����������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;//�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIO

  //GPIOE11~E14��ʼ������������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;//������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIO
	
  GPIO_SetBits(GPIOE,GPIO_Pin_7 | GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10);//

}



/****************************************************************************
**Description:      ��ָ�� col���0���������1     
**Input parameters:  
**Output parameters: 
**                   
**Returned value:        
**                   
**Created by:            hxj(2014-3-20)
**--------------------------------------------------------------------------
**Modified by:      
**Modified by:      
****************************************************************************/
static s32 drv_key_col_output(u8 col)
{
    u16 pin_data;
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;//�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    
	//set col output 0
	pin_data = GPIO_Pin_7 |GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
	GPIO_SetBits(GPIOE,pin_data);//�����1�ߵ�ƽ
	
	
    #if 0 /*xqy 2017-12-13*/
    GPIO_WritePinOutput(KEY_COL_GPIO, PORTB0, HIGH_LEVEL);
	GPIO_WritePinOutput(KEY_COL_GPIO, PORTB1, HIGH_LEVEL);
	GPIO_WritePinOutput(KEY_COL_GPIO, PORTB2, HIGH_LEVEL);
	GPIO_WritePinOutput(KEY_COL_GPIO, PORTB3, HIGH_LEVEL);
    #endif


    if(col != KEY_COL_0)
    {
        #if 0 /*xqy 2017-12-13*/
        //DrvKeyDebug("COL 0 input\r\n");
		PORT_SetPinMux(KEY_COL_PORT, PORTB0, kPORT_MuxAsGpio);    // set as gpio
		GPIO_PinInit(KEY_COL_GPIO, PORTB0, &input_config);        // input
		pin_pull_Config.pullSelect = kPORT_PullUp;	
        pin_pull_Config.mux = kPORT_MuxAsGpio;
        PORT_SetPinConfig(PORTB, PORTB0, &pin_pull_Config);       // enable pull up
        #endif
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
        GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIO
		
    }

    if(col != KEY_COL_1)
    {
        #if 0 /*xqy 2017-12-13*/
        //DrvKeyDebug("COL 1 input\r\n");
		PORT_SetPinMux(KEY_COL_PORT, PORTB1, kPORT_MuxAsGpio);    // set as gpio
		GPIO_PinInit(KEY_COL_GPIO, PORTB1, &input_config);        // input
		pin_pull_Config.pullSelect = kPORT_PullUp;	
        pin_pull_Config.mux = kPORT_MuxAsGpio;
        PORT_SetPinConfig(PORTB, PORTB1, &pin_pull_Config);       // enable pull up	
        #endif
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
        GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIO
	
    }

    if(col != KEY_COL_2)
    {
        #if 0 /*xqy 2017-12-13*/
        //DrvKeyDebug("COL 2 input\r\n");
		PORT_SetPinMux(KEY_COL_PORT, PORTB2, kPORT_MuxAsGpio);    // set as gpio
		GPIO_PinInit(KEY_COL_GPIO, PORTB2, &input_config);        // input
		pin_pull_Config.pullSelect = kPORT_PullUp;	
        pin_pull_Config.mux = kPORT_MuxAsGpio;
        PORT_SetPinConfig(PORTB, PORTB2, &pin_pull_Config);       // enable pull up	
        #endif
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
        GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIO

    }

    if(col != KEY_COL_3)
    {
        #if 0 /*xqy 2017-12-13*/
        //DrvKeyDebug("COL 3 input\r\n");
		PORT_SetPinMux(KEY_COL_PORT, PORTB3, kPORT_MuxAsGpio);    // set as gpio
		GPIO_PinInit(KEY_COL_GPIO, PORTB3, &input_config);        //set as  input
		pin_pull_Config.pullSelect = kPORT_PullUp;	
        pin_pull_Config.mux = kPORT_MuxAsGpio;
        PORT_SetPinConfig(PORTB, PORTB3, &pin_pull_Config);       // enable pull up	
        #endif
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIO

    }

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    //DrvKeyDebug("key scan, next col output low:%d \r\n", col);
    switch(col)
    {
        case KEY_COL_0:
			
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
            GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIO
		    GPIO_ResetBits(GPIOE,GPIO_Pin_7);
            break;

        case KEY_COL_1:
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
            GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIO
		    GPIO_ResetBits(GPIOE,GPIO_Pin_8);
            break;

        case KEY_COL_2:
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
            GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIO
		    GPIO_ResetBits(GPIOE,GPIO_Pin_9);
            break;

        case KEY_COL_3:
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
            GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIO
		    GPIO_ResetBits(GPIOE,GPIO_Pin_10);
            break;
        
        default:
            key_debug("key scan err col\r\n");
            break;
    }
    return 0;
}

/****************************************************************************
**Description:        ��ROW��ֵ������Ҷ���     
**Input parameters:  
**Output parameters: 
**                   
**Returned value:        
**                   
**Created by:            hxj(2014-3-20)
**--------------------------------------------------------------------------
**Modified by:      
**Modified by:      
****************************************************************************/
static s32 drv_key_row_read(u32 *status)
{
    u16 row_sta;

	row_sta = GPIO_ReadInputData(GPIOE);

	*status = (row_sta >> 11) & 0x0F;//ȡ11��14λ������
    return 0;
}
/****************************************************************************
**Description:       ��ʼ������ɨ��Ӳ��     
**Input parameters:  
**Output parameters: 
**                   
**Returned value:        
**                   
**Created by:            hxj(2014-3-20)
**--------------------------------------------------------------------------
**Modified by:      
**Modified by:      
****************************************************************************/
s32 drv_key_init(void)
{
    key_init();//��ʼ���к���
    drv_key_col_output(KEY_COL_0);
    KeyScnCtr.col = KEY_COL_0;
    KeyScnCtr.old_sta = 0XFFFFFFFF;//��ʼ��Ϊû�а�������
    KeyScnCtr.new_sta = 0XFFFFFFFF;
    KeyScnCtr.deb = 0;
    KeyChgBuf.end = 0;
    KeyChgBuf.head = 0;
    return 0;
}

/****************************************************************************
**Description:      ����ɨ�谴��״̬  �����ϱ�״̬�����ڰ��������ɰ���������   
**Input parameters:  
**Output parameters: 
**                   
**Returned value:        
**                   
**Created by:            hxj(2014-3-20)
**--------------------------------------------------------------------------
**Modified by:      
**Modified by:      
****************************************************************************/
void drv_key_scan(void)
{
    u32 row_sta_tmp;
    
    static u32 row_sta = KEY_ROW_STA_INIT;//��ౣ��32��
    
    //��ROW
    drv_key_row_read(&row_sta_tmp);
    //key_debug("row sta:%d, %01x\r\n", KeyScnCtr.col, row_sta_tmp);
    row_sta += (row_sta_tmp & KEY_ROW_MASK)<<(KEY_ROW_NUM * KeyScnCtr.col);
    //��һCOL���0
    KeyScnCtr.col++;
    if(KeyScnCtr.col >= KEY_COL_MAX)
    {   
        row_sta_tmp = row_sta&0xFFFF;
        //key_debug("%s,%04x\r\n",(u8 *)"һ��" ,row_sta_tmp);
        KeyScnCtr.col = KEY_COL_0;
    }

    drv_key_col_output(KeyScnCtr.col);//����COL���
    
    if(KeyScnCtr.col == KEY_COL_0)//��������COL��ɨ����һ��
    {
        if(row_sta != KeyScnCtr.new_sta)
        {
            //DrvKeyDebug("����\r\n");
            KeyScnCtr.new_sta = row_sta;  
            KeyScnCtr.deb = 0;
        }
        else if(row_sta != KeyScnCtr.old_sta)
        {
            KeyScnCtr.deb++;
            if(KeyScnCtr.deb>=2)
            {
                //���仯�ļ�����ǰ����״̬�ϱ�
                KeyChgBuf.chg[KeyChgBuf.head].chg = KeyScnCtr.old_sta^row_sta;//����仯����
                key_debug("chg:%04x, sta:%04x\r\n", KeyChgBuf.chg[KeyChgBuf.head].chg, row_sta);
                KeyChgBuf.chg[KeyChgBuf.head].sta = row_sta;
                KeyChgBuf.head++;
                if(KeyChgBuf.head >= KEY_SCAN_STA_BUF_MAX)
                    KeyChgBuf.head = 0;
                
                KeyScnCtr.old_sta = row_sta;
                KeyScnCtr.deb = 0;
            }
        }
        else
        {
            KeyScnCtr.deb = 0;    
        }

        row_sta = KEY_ROW_STA_INIT;
    }
}


/****************************************************************************
**Description:      �������ı��¼�     
**Input parameters:  
**Output parameters: 
**                   
**Returned value:        
**                   
**Created by:            hxj(2014-3-11)
**--------------------------------------------------------------------------
**Modified by:      
**Modified by:      
****************************************************************************/
s32 drv_key_chgsta_read(struct _key_scan_sta* chg)
{
    if(KeyChgBuf.head != KeyChgBuf.end)
    {
        chg->chg = KeyChgBuf.chg[KeyChgBuf.end].chg;
        chg->sta = KeyChgBuf.chg[KeyChgBuf.end].sta;
        KeyChgBuf.end++;
        if(KeyChgBuf.end >= KEY_SCAN_STA_BUF_MAX)
            KeyChgBuf.end = 0;
        return 1;
    }
    return 0;
}

/****************************************************************************
**Description:      ��ӡ���µļ�     
**Input parameters:      
**Output parameters:     
**                       
**Returned value:        
**                       
**Created by:            xqy(2017-12-14)
**--------------------------------------------------------------------------
**Modified by:          
**Modified by:          
****************************************************************************/
void disp_keyvalue(u32 key)
{
    switch (key)
    {   
       case 0:
       case 1:
       case 2:
       case 3:
       case 4:
       case 5:
       case 6:
       case 7:
       case 8:
       case 9:
           //uart_printf("����:%d\n",key);
           break;
       case cancel:
       case clr   :
       case up    :
       case down  :
       case enter :
       case pwroff:
           uart_printf("����:%s\n",(u8 *)key_value[key-10].name);
           break;
        default:
            break;
    }
}
#if 0 /*xqy 2017-12-13*/
/****************************************************************************
**Description:      ����͹���ǰ��Ҫ���ð������Ա㰴������     
**Input parameters:  
**Output parameters: 
**                   
**Returned value:        
**                   
**Created by:            hxj(2014-4-28)
**--------------------------------------------------------------------------
**Modified by:      
**Modified by:      
****************************************************************************/
void drv_key_sleep(void)
{

    gpio_pin_config_t input_config = 
	{
        kGPIO_DigitalInput, 0,
    };

    gpio_pin_config_t output_config = 
	{
        kGPIO_DigitalOutput, LOW_LEVEL,
    };

    port_pin_config_t pin_pull_Config = {0};  //PULL UP
	pin_pull_Config.pullSelect = kPORT_PullUp;	
	pin_pull_Config.mux = kPORT_MuxAsGpio; 

    // row output 0
	PORT_SetPinMux(KEY_ROW_PORT, PORTB10, kPORT_MuxAsGpio);	  // set as gpio
	GPIO_PinInit(KEY_ROW_GPIO, PORTB10, &output_config);	  // set as  output
	//GPIO_WritePinOutput(KEY_ROW_GPIO, PORTB10, LOW_LEVEL);    // output 0	
	
	PORT_SetPinMux(KEY_ROW_PORT, PORTB11, kPORT_MuxAsGpio);	  // set as gpio
	GPIO_PinInit(KEY_ROW_GPIO, PORTB11, &output_config);	  // set as  output
	//GPIO_WritePinOutput(KEY_ROW_GPIO, PORTB11, LOW_LEVEL);    // output 0	

	PORT_SetPinMux(KEY_ROW_PORT, PORTB16, kPORT_MuxAsGpio);	  // set as gpio
	GPIO_PinInit(KEY_ROW_GPIO, PORTB16, &output_config);	  // set as  output
	//GPIO_WritePinOutput(KEY_ROW_GPIO, PORTB16, LOW_LEVEL);    // output 0	

	PORT_SetPinMux(KEY_ROW_PORT, PORTB17, kPORT_MuxAsGpio);	  // set as gpio
	GPIO_PinInit(KEY_ROW_GPIO, PORTB17, &output_config);	  // set as  output
	//GPIO_WritePinOutput(KEY_ROW_GPIO, PORTB17, LOW_LEVEL);    // output 0	

	PORT_SetPinMux(KEY_ROW_PORT, PORTB18, kPORT_MuxAsGpio);	  // set as gpio
	GPIO_PinInit(KEY_ROW_GPIO, PORTB18, &output_config);	  // set as  output
	//GPIO_WritePinOutput(KEY_ROW_GPIO, PORTB18, LOW_LEVEL);    // output 0	

    //col input
    PORT_SetPinInterruptConfig(KEY_COL_PORT, PORTB0, kPORT_InterruptFallingEdge);
	PORT_SetPinMux(KEY_COL_PORT, PORTB0, kPORT_MuxAsGpio);	  // set as gpio
	GPIO_PinInit(KEY_COL_GPIO, PORTB0, &input_config);		  // input
	PORT_SetPinConfig(KEY_COL_PORT, PORTB0, &pin_pull_Config); 	  // enable pull up

    PORT_SetPinInterruptConfig(KEY_COL_PORT, PORTB1, kPORT_InterruptFallingEdge);
	PORT_SetPinMux(KEY_COL_PORT, PORTB1, kPORT_MuxAsGpio);	  // set as gpio
	GPIO_PinInit(KEY_COL_GPIO, PORTB1, &input_config);		  // input
	PORT_SetPinConfig(KEY_COL_PORT, PORTB1, &pin_pull_Config); 	  // enable pull up	

    PORT_SetPinInterruptConfig(KEY_COL_PORT, PORTB2, kPORT_InterruptFallingEdge);
	PORT_SetPinMux(KEY_COL_PORT, PORTB2, kPORT_MuxAsGpio);	  // set as gpio
	GPIO_PinInit(KEY_COL_GPIO, PORTB2, &input_config);		  // input
	PORT_SetPinConfig(KEY_COL_PORT, PORTB2, &pin_pull_Config); 	  // enable pull up

    PORT_SetPinInterruptConfig(KEY_COL_PORT, PORTB3, kPORT_InterruptFallingEdge);
	PORT_SetPinMux(KEY_COL_PORT, PORTB3, kPORT_MuxAsGpio);	  // set as gpio
	GPIO_PinInit(KEY_COL_GPIO, PORTB3, &input_config);		  // input
	PORT_SetPinConfig(KEY_COL_PORT, PORTB3, &pin_pull_Config); 	  // enable pull up	

   

	EnableIRQ(PORTB_IRQn);
}

/****************************************************************************
**Description:      �رհ����жϹ���     
**Input parameters:  
**Output parameters: 
**                   
**Returned value:        
**                   
**Created by:            hxj(2014-4-28)
**--------------------------------------------------------------------------
**Modified by:      
**Modified by:      
****************************************************************************/
void drv_key_wakeup(void)
{
    gpio_pin_config_t output_config = {
        kGPIO_DigitalOutput, HIGH_LEVEL,
    };
    DisableIRQ(PORTB_IRQn);
    // col output 1
	PORT_SetPinMux(KEY_COL_PORT, PORTB0, kPORT_MuxAsGpio);	  // set as gpio
	GPIO_PinInit(KEY_COL_GPIO, PORTB0, &output_config);	      // set as  output
	//GPIO_WritePinOutput(KEY_COL_GPIO, PORTB0, HIGH_LEVEL);    // output 1	
	
	PORT_SetPinMux(KEY_COL_PORT, PORTB1, kPORT_MuxAsGpio);	  // set as gpio
	GPIO_PinInit(KEY_COL_GPIO, PORTB1, &output_config);	      // set as  output
	//GPIO_WritePinOutput(KEY_COL_GPIO, PORTB1, HIGH_LEVEL);    // output 1	

	PORT_SetPinMux(KEY_COL_PORT, PORTB2, kPORT_MuxAsGpio);	  // set as gpio
	GPIO_PinInit(KEY_COL_GPIO, PORTB2, &output_config);	      // set as  output
	//GPIO_WritePinOutput(KEY_COL_GPIO, PORTB2, HIGH_LEVEL);    // output 1	

	PORT_SetPinMux(KEY_COL_PORT, PORTB3, kPORT_MuxAsGpio);	  // set as gpio
	GPIO_PinInit(KEY_COL_GPIO, PORTB3, &output_config);	      // set as  output
	//GPIO_WritePinOutput(KEY_COL_GPIO, PORTB3, HIGH_LEVEL);    // output 1	

	

	drv_key_init();
}
#endif


/********************************Copyright ( c ) ***************************
**                  Shenzhen Xinguodu Technology Co., LTD.
**                        http://www.xinguodu.com
**
**
**Created By:     hxj
**Created Date:       2014-3-13
**Version:            <version >
**Description:    �����ӿ�
**
****************************************************************************/


//����ɨ��ʱ���ṹ������ɨ��õ��İ���״̬��λ��

struct _key_pra
{
    u8 jiegou;//�ṹ����ֵ(������ֵ)�����������������£���1��ʼ
    u8 sch; //��·ͼ����ֵ ���������������£���1��ʼ
    u32 line_mask;//����ɨ���״̬����
};


//�����K200P��
const struct _key_pra KeyPra[16]=
{
    18, 14, 10,//0
    5,  1,  12,//1
    6,  2,  8,//2
    7,  3,  4,//3
    9,  5,  0,//4
    10, 6,  13,//5
    11, 7,  9,//6
    13, 9,  5,//7
    14, 10, 1,//8
    15, 11, 14,//9
    12, 13, 2,//ȡ��
    16, 17, 6,//���
    17, 18, 15,//��
    19, 12, 7,//��
    20, 15, 3,//ȷ��
    20, 15, 11,//�ػ�
};

const struct _key_fun_tab key_fun_tab[21]=
{
    "null\0",
    "on/off\0",
    "f\0",
    "f1\0",
    "f2\0",
    "ch1\0",
    "ch2\0",
    "ch3\0",
    "ch\0",
    "ch4\0",
    "ch5\0",
    "ch6\0",
    "cancel\0",
    "ch7\0",
    "ch8\0",
    "ch9\0",
    "clr\0",
    "up\0",
    "ch0\0",
    "down\0",
    "enter\0"
};//20������

#define key_buf_max 8//64  //sxlneicun

struct _key_buf
{
    u32 value[key_buf_max];
    u8 head;
    u8 end;
};

static struct _key_buf key_buf;

struct _key_ctrl
{

    u32 lastkey;//��һ���ϱ��ļ�ֵ
    u32 timeid;//�ϴ��ϱ�������ʱ��
    u32 holdtime;//��������ʱ��㣬���ڱ�����
    u32 delay;//���೤ʱ���ٱ�һ��
    u32 last_key_sta;
    u8 keysta;//1 �м����£�0�޼�����
    u8 holdflag;//������ʶ
    u8 longpress;

};

static struct _key_ctrl KeyCtrl;
static u8 KeyGd = 0;

#define KEY_LONGPRESS_OTHER_DELAY (200)//MS �������ϱ����

#define KEY_LONGPRESS_FRIST_DELAY (2000)//MS//������ʱ��
#define KEY_PWRONOFF_DELAY      (3000)//ms//���ػ���Ӧʱ��
#define KEY_HOLD_ERR_TIME       (10*1000)//10s�㿨��
/****************************************************************************
**Description:
**Input parameters:
**Output parameters:
**
**Returned value:
**
**Created by:            hxj(2014-3-11)
**--------------------------------------------------------------------------
**Modified by:
**Modified by:
****************************************************************************/
s32 dev_key_init()
{
    key_buf.end = 0;
    key_buf.head = 0;

    KeyCtrl.longpress = 0;//Ĭ�ϲ�����������
    KeyCtrl.keysta = 0;
    KeyCtrl.holdflag = 0;
    (KeyCtrl.last_key_sta) = 0xffffffff;
    drv_key_init();
    return 0;
}
/****************************************************************************
**Description:      �򿪰���
**Input parameters:
**Output parameters:
**
**Returned value:
**
**Created by:            hxj(2014-3-11)
**--------------------------------------------------------------------------
**Modified by:
**Modified by:
****************************************************************************/
s32 dev_key_open(void)
{
    KeyGd = 1;
    return 0;
}

/****************************************************************************
**Description:      �رհ���
**Input parameters:
**Output parameters:
**
**Returned value:
**
**Created by:            hxj(2014-3-11)
**--------------------------------------------------------------------------
**Modified by:
**Modified by:
****************************************************************************/
s32 dev_key_close(void)
{
    KeyGd = 0;
    return 0;
}

/****************************************************************************
**Description:      �������ֵ����
**Input parameters:
**Output parameters:
**
**Returned value:
**
**Created by:            hxj(2014-3-11)
**--------------------------------------------------------------------------
**Modified by:
**Modified by:
****************************************************************************/
s32 dev_key_clear(void)
{
    key_buf.end = key_buf.head;
    return 0;
}

/****************************************************************************
**Description:      �������ֵ
**Input parameters:
**Output parameters:
**
**Returned value:
**
**Created by:            hxj(2014-3-11)
**--------------------------------------------------------------------------
**Modified by:
**Modified by:
****************************************************************************/
s32 dev_key_read(u32 *lpKey)
{
    if(key_buf.head != key_buf.end)
    {
        *lpKey = key_buf.value[key_buf.end++];
        if(key_buf.end >= key_buf_max)
            key_buf.end = 0;
    }
    else
    {
        *lpKey = 0;
    }

    return 0;
}

/****************************************************************************
**Description:      ͵��һ�£����Ƿ��а���     
**Input parameters:  
**Output parameters: 
**                   
**Returned value:        
**                   
**Created by:            hxj(2014-7-17)
**--------------------------------------------------------------------------
**Modified by:      
**Modified by:      
****************************************************************************/
s32 dev_key_peek(u32 *lpKey)
{

    if(key_buf.head != key_buf.end)
    {
        *lpKey = key_buf.value[key_buf.end];
    }
    else
    {
        *lpKey = 0;
    }

    return 0;
    
}

/****************************************************************************
**Description:    д��ֵ������       
**Input parameters:  
**Output parameters: 
**                   
**Returned value:        
**                   
**Created by:          
**--------------------------------------------------------------------------
**Modified by: 2014.07.28 ���ӵ���������LCD���⹦�ܡ�
**Modified by:      
****************************************************************************/
static s32 dev_key_write(u32 key)
{
    //dev_key_bl_on();
    //dev_lcd_bl_on();
        
    key_buf.value[key_buf.head++] = key;
    if(key_buf.head >= key_buf_max)
        key_buf.head = 0;
    return 0;
}


/****************************************************************************
**Description:       дһ�����ػ�������ֵ������    
**Input parameters:  
**Output parameters: 
**                   
**Returned value:        
**                   
**Created by:      
**--------------------------------------------------------------------------
**Modified by:      
**Modified by:      
****************************************************************************/
s32 dev_key_wirte_power(void)
{
    dev_key_write(KEY_ONOFF_HOLD);   
    return 0;
}
/****************************************************************************
**Description:      ���ư���
**Input parameters:
**Output parameters:
**
**Returned value:
**
**Created by:            hxj(2014-3-11)
**--------------------------------------------------------------------------
**Modified by:
**Modified by:
****************************************************************************/
s32 dev_key_ioctl(u32 nCmd, u32 lParam, u32 wParam)
{
    s32 ret = DDI_OK;
#if 0
    switch(nCmd)
    {
    case DDI_KEY_CTL_LONGPRESS://���ó�����
        if(lParam == 0)//��ֹ������
        {
            KeyCtrl.longpress = 0;
        }
        else if(lParam == 1)//��������
        {
            KeyCtrl.longpress = 1;
        }
        else
        {
            //��������
            ret = DDI_EINVAL;
        }
        break;

    case DDI_KEY_CTL_BKLIGHT://���ư�������
        if((SysPwr.machine == MACHINE_G2)||(SysPwr.machine == MACHINE_G2_PT))
        {
            if(lParam == 0)//��
            {
                DevKeyDebug("dis ��������\r\n");
                dev_key_bl_dis();
            }
            else if(lParam == 1)//��
            {
                DevKeyDebug("en ��������\r\n");
                dev_key_bl_en();
            }
            else
            {
                //��������
                ret = DDI_EINVAL;
            }
        }
        break;

    case DDI_KEY_CTL_BKLIGHTTIME: //���ñ���ʱ��
        if((SysPwr.machine == MACHINE_G2)||(SysPwr.machine == MACHINE_G2_PT))
        {
            if(lParam > 255)
            {
                DevKeyDebug("���ñ���ʱ���������\r\n");
                ret = DDI_EINVAL;
            }
            else
            {
                dev_key_bl_settime(lParam);
            }
        }
        break;

    case DDI_KEY_CTL_BKLIGHT_CTRL:
        if((SysPwr.machine == MACHINE_G2)||(SysPwr.machine == MACHINE_G2_PT))
        {
            if(lParam == 0)//Ϩ��
            {
                dev_key_bl_off(); 
                ret = DDI_OK;
            }
            else if(lParam == 1)//����
            {
                dev_key_bl_core_on();
                ret = DDI_OK;
            }
            else
            {
                ret = DDI_EINVAL;   
            }
        }
        break;
        
    default:
        ret = DDI_EINVAL;
        break;
    }
#endif
    return ret;
}


/****************************************************************************
**Description:      ����ɨ��ʱ������״̬���ֵ
**Input parameters:  sta ����״̬
**Output parameters:
**
**Returned value: ��ֵ
**
**Created by:            hxj(2014-3-20)
**--------------------------------------------------------------------------
**Modified by:
**Modified by:
****************************************************************************/
static u32 dev_key_get_keyvalue(u32 sta)
{
    u32 key_value = 0;
    u32 key_sta;

    s8 i;
    u8 cnt;

    i = 15;//�������
    cnt = 0;
    while(i >= 0)
    {
        //key_debug("i=%d\n",i);
        key_sta = sta & (0x01<<(KeyPra[i].line_mask));
        if( key_sta == 0)
        {
            key_value = (key_value << 8)+i;
            cnt++;
            if(cnt >4)//���4����
            {
                key_value = 0;
                //key_debug("��ֵ���࣬����\n");
                break;
            }
        }
        i--;
    }
    //key_debug("����ֵ%08x\n",key_value);
    return key_value;
}
/****************************************************************************
**Description:      ��������������������������
                    ��CORE�����е���
**Input parameters:
**Output parameters:
**
**Returned value:
**
**Created by:            hxj(2014-3-11)
**--------------------------------------------------------------------------
**Modified by:
**Modified by:
****************************************************************************/
void dev_key_task(void)
{
    u32 key_value;
    s32 res;
    u8 i;
    u8 cnt;
    u32 lParam;
    struct _key_scan_sta key_chg_sta;

    static u8 onoff_sta = 0;//��ִֻ֤��һ��
    static s32 timeid = 0;

    if(KeyGd == 0)//�����ر�״̬����ɨ��
        return;

    //�������仯
    res = drv_key_chgsta_read(&key_chg_sta);
    if(1 == res)
    {
        key_debug("chg: %04x, %04x\r\n", key_chg_sta.chg, key_chg_sta.sta);
    
        //�仯
        //�󰴼�
        if(((KeyCtrl.last_key_sta) | key_chg_sta.sta) == 0xffffffff)
        {

            //���ڿ������жϣ�Ҫϸ�� ����һ������һֱ��ס������Ϊ�ǿ���
            //ֻ�и��ϴεİ���״̬��ȫ��һ��ʱ���Ÿ��¿���ʱ��
            KeyCtrl.holdtime = dev_get_timeID();//ÿ�ΰ���״̬�ı䶼����
            key_debug("��ǰʱ��Ϊ%d\r\n", KeyCtrl.holdtime);
        }
        //�����ֵ
        (KeyCtrl.last_key_sta) = key_chg_sta.sta;
        key_value = dev_key_get_keyvalue(key_chg_sta.sta);
        KeyCtrl.lastkey = key_value;
        key_debug("���� %d\r\n", key_value);
        if((key_chg_sta.chg & key_chg_sta.sta) == 0)
        {
            //����
            key_debug("���� %08x\r\n", key_value);
            if(KeyCtrl.holdflag == 0)//û�п���
            {
                key_debug("дһ����ֵ%d\r\n", key_value);
                dev_key_write(key_value);
            }
            KeyCtrl.keysta = 1;

        }
        else
        {
            //�����仯�������ɿ�
            key_debug("�����仯\r\n");
            if(key_chg_sta.sta == 0XFFFFFFFF)
            {
                //��ǰû������
                key_debug("�޼�\r\n");
                KeyCtrl.keysta = 0;
            }
        }

        KeyCtrl.delay = KEY_LONGPRESS_FRIST_DELAY;//2S  ���淶��2S
        KeyCtrl.timeid = dev_get_timeID();//���°�������ʱ��

    }

    //---������---  //������������Ӧ��������������ɨ�������Ҫ�����޸� FIX
    if(KeyCtrl.keysta == 1)//�м�����
    {
        if(FUN_TIME_OVER == dev_querry_time(KeyCtrl.holdtime, KEY_HOLD_ERR_TIME))
        {
            if(KeyCtrl.holdflag==0)
            //dev_lcd_keywarm();
            KeyCtrl.holdflag = 1;//�м�����
        }
    }
    else if(KeyCtrl.keysta == 0
            && KeyCtrl.holdflag == 1)
    {
        KeyCtrl.holdflag = 0;
    }

    //----����������
    if(KeyCtrl.longpress == 1//��������
            &&KeyCtrl.holdflag == 0//û�п���
      )
    {
        if(KeyCtrl.keysta == 1)//�м�����
        {
            //ʱ�䵽
            if(FUN_TIME_OVER == dev_querry_time(KeyCtrl.timeid, KeyCtrl.delay))
            {
                if(KeyCtrl.lastkey != 0x01)//���ֻ�йػ������£���������
                {
                    dev_key_write(KeyCtrl.lastkey);
                }

                KeyCtrl.timeid = dev_get_timeID();
                KeyCtrl.delay = KEY_LONGPRESS_OTHER_DELAY;
            }
        }
    }
    #if 0 /*xqy 2017-12-13*/
    //�����ػ���
    #ifndef COER_DEAL_ONOFF
    //�����ػ����
    if(1 == dev_key_check_onoff())//on off ����,���뱣��һֱ���²Ż�ִ�п��ػ�
    {
        if(onoff_sta == 0)//˵���ǵ�һ�α仯
        {
            key_debug("���ػ���������\r\n");    
            onoff_sta = 1;
            timeid = dev_get_timeID();
        }
        else if(onoff_sta == 1)
        {
            //�жϿ��һ�������סʱ��
            if(FUN_TIME_OVER == dev_querry_time(timeid, SYS_POWER_HOLD))//�ݶ�2S���ػ�
            {
                key_debug("д��һ���ػ���\r\n");
                dev_key_wirte_power();
                onoff_sta = 3;
            }
        }
        else
        {
        
        }
    }
    else
    {
        onoff_sta = 0;    
    }
    #endif
    #endif


}

/****************************************************************************
**Description:      ��������͹���
**Input parameters:
**Output parameters:
**
**Returned value:
**
**Created by:            hxj(2014-4-28)
**--------------------------------------------------------------------------
**Modified by:
**Modified by:
****************************************************************************/
void dev_key_sleep(void)
{
    //ֹͣ����ɨ��
    dev_key_close();
    //���ð����жϻ���
    //drv_key_sleep();
    
}

void dev_key_wakeup(void)
{
    dev_key_init();     //add by hecaiwen,2014.11.07,�������͹��ĺ��������ѳ���"����"�е�ȱ��
   // drv_key_wakeup();
    dev_key_open();
}


/****************************************************************************
**Description:      ��⿪�ؼ��Ƿ���//ֱ�Ӽ�⣬����ͨ������ɨ��?
**Input parameters:
**Output parameters:
**
**Returned value: 1 ����
**
**Created by:            hxj(2014-3-14)
**--------------------------------------------------------------------------
**Modified by:
**Modified by:
****************************************************************************/
s32 dev_key_check_onoff(void)
{

    if((KeyCtrl.last_key_sta & (1 << (KeyPra[1].line_mask))) == 0)
    {
        return 1;
    }
    else
        return 0;

}

/****************************************************************************
**Description:      ���԰���
**Input parameters:
**Output parameters:
**
**Returned value:
**
**Created by:            hxj(2014-3-10)
**--------------------------------------------------------------------------
**Modified by:
**Modified by:
****************************************************************************/
s32 dev_key_test(void)
{
    u32 key;

    while(1)
    {
        //uart_printf("read\r\n");
        drv_key_scan();     //����ɨ��
        dev_key_task();     //�������仯��

        
        dev_key_read(&key);
        //uart_printf("key:%08x\r\n", key);
        if(key !=0)
        {

            key_debug("key: %s ", &key_fun_tab[key&0x000000ff]);

            key = key>>8;
            if(key !=0)
            {
                key_debug("%s ", &key_fun_tab[key&0x000000ff]);
            }
            key = key>>8;
            if(key !=0)
            {
                key_debug("%s ", &key_fun_tab[key&0x000000ff]);
            }
            key = key>>8;
            if(key !=0)
            {
                key_debug("%s ", &key_fun_tab[key&0x000000ff]);
            }

            key_debug("--\r\n");
        }
    }
}

//----------------------DDI--------------------
#include "ddi.h"


s32 DdiKeyGd = -1;


s32 ddi_key_open (void)
{

    if(DdiKeyGd == -1)
    {
        dev_key_open();
        DdiKeyGd = 0;
    }
    return DDI_OK;
}

s32 ddi_key_close (void)
{
    ;

    if(DdiKeyGd == 0)
    {
        dev_key_close();
        DdiKeyGd = -1;
    }
    return DDI_OK;
}

s32 ddi_key_clear (void)
{
    ;

    if(DdiKeyGd == 0)
    {
        dev_key_clear();
        return DDI_OK;
    }
    else
    {
        return DDI_EIO;
    }

}

/****************************************************************************
**Description:
**Input parameters:
**Output parameters:
**
**Returned value:
**
**Created by:            hxj(2014-5-7)
**--------------------------------------------------------------------------
**Modified by:     2014.05.07 ��������OK
**Modified by:
****************************************************************************/
s32 ddi_key_read (u32 *lpKey)
{
    u32 key, key_tmp;
    s32 ret = 0;

    //DEBUG_SHOW_FUN_NAME;

    *lpKey = 0;

    dev_key_read(&key);

    key_tmp = key;

    if(DdiKeyGd == -1)//�豸δ��
    {
        //DevKeyDebug("�豸δ��\r\n");
        return DDI_EIO;
    }
    if(key == 0)//�ް���
    {
        return 0;
    }
    //ͳ�ư�������
    if((key & 0xff) != 0)
    {
        ret++;
    }
    key = (key >> 8);

    if((key & 0xff) != 0)
    {
        ret++;
    }
    key = (key >> 8);

    if((key & 0xff) != 0)
    {
        ret++;
    }
    key = (key >> 8);

    if((key & 0xff) != 0)
    {
        ret++;
    }

    *lpKey = key_tmp;

    return ret;

}

/****************************************************************************
**Description:
**Input parameters:
**Output parameters:
**
**Returned value:
**
**Created by:            hxj(2014-5-7)
**--------------------------------------------------------------------------
**Modified by:      2014.05.07 ����OK
**Modified by:
****************************************************************************/
s32 ddi_key_ioctl(u32 nCmd, u32 lParam, u32 wParam)
{
    ;

    if(DdiKeyGd == -1)//�豸Ϊ��
    {
        return DDI_EIO;
    }

    return dev_key_ioctl(nCmd, lParam, wParam);
}




static const u32 keyfun[21]=
{
    0,
    0,    FUNCTION,    F1,    F2,
    '1', '2',  '3',   ALPHA,
    '4', '5',  '6',   ESC,
    '7', '8',  '9',   CLR,
    '*', '0',  '#',   ENTER,
};


/*
    ��Ӧ�������е��ã�����Ҫɨ�谴��
    �ң�������hxj 2015.06.13
*/
s32 dev_key_fun_read(u32 *fun_key)
{
    u32 key;

    dev_key_read(&key);

    if(
        ((key & 0x000000ff) != 0)
        && ((key & 0xffffff00) == 0)
    )//���Գ��򲻴�����
    {
        if(key == KEY_ONOFF_HOLD)
        {
            *fun_key = key;
            return 0;
        }

        //dev_buzzer_open(100);

        *fun_key = keyfun[key&0x000000ff];

    }
    else
        *fun_key = 0;

    return 0;
}


#if 1 /*xqy 2018-6-7*/
//////////////////////////////��ʱ�ÿ������ϵ�3������--����//////////////////////
static struct _key_buf KEY_BUFF;

//������ʼ������
void KEY_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOA,GPIOEʱ��
 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5; //KEY2 KEY3��Ӧ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
    KEY_BUFF.end = 0;
    KEY_BUFF.head = 0;
 
} 

static s32 key_write(u32 key)
{
    uart_printf("д��һ����ֵ=%d\n",key);
    if(key==key_exit*key_ok)//���ͬʱ����
    {
        NVIC_SystemReset();
    }
    KEY_BUFF.value[KEY_BUFF.head++] = key;//��󴢴�8������ֵ
    if(KEY_BUFF.head >= key_buf_max)
        KEY_BUFF.head = 0;
    return 0;
}
s32 key_read(u32 *lpKey)
{
    if(KEY_BUFF.head != KEY_BUFF.end)
    {
        *lpKey = KEY_BUFF.value[KEY_BUFF.end++];
        if(KEY_BUFF.end >= key_buf_max)
            KEY_BUFF.end = 0;
    }
    else
    {
        *lpKey = 0;
    }

    return 0;
}
s32 key_clear(void)
{
    KEY_BUFF.end = KEY_BUFF.head;
    return 0;
}

//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//4��WKUP���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>WK_UP!!
u8 KEY_Scan(u8 mode)
{	 
    u16 key_PE;
    static u16 key_PE_bak=0;
    u32 key_value_x = 1;
    u8 i;
	static u8 key_up_flag=1;//�������ɿ���־
	
	if(mode)
	    key_up_flag=1;  //֧������		
	key_PE = GPIO_ReadInputData(GPIOE);
	key_PE = ~(key_PE);//ȡ��
	key_PE &= 0x3f;//ֻȡ��6λ����
	if(key_up_flag && key_PE)//
	{
	    key_PE_bak = key_PE;
		delay_os(50);//delayms(10);//ȥ���� 
		key_up_flag=0;
        for(i=0;i<6;i++)
        {
            if(key_PE&0x01)
            {
                key_value_x *= (i+1);//��ϰ������ǰ�����˵�ֵ���ܺò�����ȡֵʱ��˱Ƚ��Ƿ��Ǹ���ϼ�����
            }
			key_PE>>=1;
        }
        key_write(key_value_x);
	}
	else if(key_PE==0)//�ް�������
	{
	    key_up_flag=1; 	
	}
	else if(key_PE!=key_PE_bak)//�ް�������
	{
	    key_PE_bak = key_PE;
	    key_up_flag=1; 	
	}
 	return 0;// �ް�������
}
#endif






