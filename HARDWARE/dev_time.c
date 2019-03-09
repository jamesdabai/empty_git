#include "sys.h"
#include "ddi.h"
#include "dev_time.h"

extern u32 OSTickCtr;
u32 dev_get_timeID(void)
{
    u32 timer_tmp;
    
    timer_tmp = OSTickCtr;//��ucosiii�е�ʱ��ֵ
    
    return timer_tmp;
}
/****************************************************************************
**Description:    ��ѯ��Timeʱ�䵽��ǰϵͳʱ���Ƿ񾭹���Delayʱ��
**Input parameters:  
**Output parameters: 
**                   
**Returned value:        
**                   
**Created by:            ��ѧ��(2013-4-12)
**--------------------------------------------------------------------------
**Modified by:      
**Modified by:      
****************************************************************************/
s32 dev_querry_time(u32 Time, u32 Delay)
{
    u32 curtime;//��ǰϵͳʱ��
    u32 passtime;
    
    curtime = dev_get_timeID();

    //uart_printf("id:%d, ti:%d, d:%d\r\n", curtime, Time, Delay);
    if(curtime >= Time)//ϵͳʱ�������
    {
        passtime = curtime - Time;
    }
    else
    {
        passtime = (u32)(~0) - (Time - curtime);
    }
    
    if(passtime >= Delay)
    {
        return FUN_TIME_OVER;//�Ѿ���ȥָ��ʱ��
    }
    else
    {
        return FUN_TIME_NOOVER;//������ʱ��û��Delay��ô��   
    }
}

s32 ddi_sys_get_tick(u32 *nTick)  //ϵͳ�δ�
{
    
    *nTick = dev_get_timeID();
    return DDI_OK;
}

void delay_ms(int ms)//��ʱ����
{
	int us;

    s32 curtime;
	curtime = dev_get_timeID();
	
	while(1)
	{
		if(dev_querry_time(curtime, ms)==FUN_TIME_OVER)
		{
			break;
		}
	}
}



