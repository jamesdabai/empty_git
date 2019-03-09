/********************************Copyright ( c ) ***************************
**                  Shenzhen Xinguodu Technology Co., LTD.
**                        http://www.xinguodu.com
**
**
**Created By:         xqy
**Created Date:       2017-12-13
**Version:            <version >
**Description:        
**
****************************************************************************/

#ifndef _DRV_KEY_H_
#define _DRV_KEY_H_

#include "sys.h"

#define KEY_CHG_PRESS  1//����
#define KEY_CHG_RELEASE  2//�ɿ�

#define LINE_TO_LINE_SCAN 1



#define KEY_ROW_MASK 0X0F//4 ROW
#define KEY_ROW_NUM 4//4 ROW
#define KEY_ROW_STA_INIT 0xffff0000//״̬Ŀǰ16����xqy


#ifndef LINE_TO_LINE_SCAN

struct _key_scan_sta
{
    u8 type;//�仯���� ����1,�ɿ�2
    u16 col;//�仯�����״̬
    u16 row;//�仯�����״̬
};
#else
struct _key_scan_sta
{
    u32 chg;//�ı�İ���
    u32 sta;//��ǰ����״̬
};
#endif

struct _key_fun_tab
{
    u8 name[20];
};

struct _key_value
{
    u8 name[10];
};

//extern void drv_key_sleep(void);
//extern void drv_key_wakeup(void);
#endif


/********************************Copyright ( c ) ***************************
**                  Shenzhen Xinguodu Technology Co., LTD.
**                        http://www.xinguodu.com
**
**
**Created By:     hxj
**Created Date:       2014-3-13
**Version:            <version >
**Description:    
**
****************************************************************************/
#ifndef _DEV_KEY_H_
#define _DEV_KEY_H_

#define KEYNULL     0x00    //��Ч��

#define DIGITAL0   '0'
#define DIGITAL1   '1'
#define DIGITAL2   '2'
#define DIGITAL3   '3'
#define DIGITAL4   '4'
#define DIGITAL5   '5'
#define DIGITAL6   '6'
#define DIGITAL7   '7'
#define DIGITAL8   '8'
#define DIGITAL9   '9'
#define UP         '*'
#define DOWN       '#'


#define F1          0x11
#define F2          0x12
#define F3          0x13     
#define FUNCTION    0x14    // ����
#define ESC         0x1B
#define ALPHA       0x16    // ���뷨/��ĸ�л�
#define CLR         0x17
#define ENTER       0X18
#define PWRONOFF    0x19

#define KEY_DEL     0xff//��֧��

#define KEY_ONOFF_HOLD  0xEF//���ػ�����2S���ϱ�һ��0XEF��APP

#define DDI_KEY_CTL_LONGPRESS   0//���ó�����
#define DDI_KEY_CTL_BKLIGHT     1//���ư�������
#define DDI_KEY_CTL_BKLIGHTTIME 2//���ñ���ʱ��
#define DDI_KEY_CTL_BKLIGHT_CTRL 3//ֱ�ӿ��Ʊ��� 
//xiaqiyun
#define cancel 10 
#define clr    11 
#define up     12 
#define down   13 
#define enter  14 
#define pwroff 15 


extern s32 ddi_key_open (void);
extern s32 ddi_key_close (void);
extern s32 ddi_key_clear (void);
extern s32 ddi_key_read (u32 *lpKey);
extern s32 ddi_key_ioctl(u32 nCmd, u32 lParam, u32 wParam);

extern s32 dev_key_check_onoff(void);
extern s32 dev_key_clear(void);

extern void dev_key_task(void);

extern s32 dev_key_fun_read(u32 *fun_key);
extern void disp_keyvalue(u32 key);


#if 1 /*xqy 2018-6-7*/
//////////////////////////////��ʱ�ÿ������ϵ�3������--����//////////////////////
#include "sys.h" 
/*����ķ�ʽ��ͨ��ֱ�Ӳ����⺯����ʽ��ȡIO*/
#define key_right   1 
#define key_down	2
#define key_ok      3
#define key_up	    4
#define key_exit  	5
#define key_left    6
void KEY_Init(void);	//IO��ʼ��
u8 KEY_Scan(u8);  		//����ɨ�躯��	
#endif




#endif


