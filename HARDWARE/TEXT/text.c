#include "sys.h" 
#include "fontupd.h"
#include "lcd.h"
#include "text.h"	
#include "string.h"												    
#include "usart.h"
#include "exfuns.h" 

//////////////////////////////////////////////////////////////////////////////////	 
 
//������ʾ ��������	   
//STM32F4����ģ��-�⺯���汾
//�Ա����̣�http://mcudev.taobao.com									  
////////////////////////////////////////////////////////////////////////////////// 	 

//xqy��Щ�ֿⶼ����ɨ�ģ����Ǻ�ɨ
FIL* GBK_12 = NULL;
FIL* GBK_16 = NULL;
FIL* GBK_24 = NULL;

//code �ַ�ָ�뿪ʼ
//���ֿ��в��ҳ���ģ
//code �ַ����Ŀ�ʼ��ַ,GBK��
//mat  ���ݴ�ŵ�ַ (size/8+((size%8)?1:0))*(size) bytes��С	
//size:�����С
void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size)
{		    
	unsigned char qh,ql;
	unsigned char i;					  
	unsigned long foffset; 
	u32 br_xqy;
	u8 csize=(size/8+((size%8)?1:0))*(size);//�õ�����һ���ַ���Ӧ������ռ���ֽ���	 
	qh=*code;
	ql=*(++code);
	if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//�� ���ú���
	{   		   
	    uart_printf("�ǳ��ú���\n");
	    for(i=0;i<csize;i++)
	        *mat++=0x00;//�������
	    return; //��������
	}          
	if(ql<0x7f)
	    ql-=0x40;//ע��!
	else
	    ql-=0x41;
	qh-=0x81;   
	foffset=((unsigned long)190*qh+ql)*csize;	//�õ��ֿ��е��ֽ�ƫ����  		  
	switch(size)
	{
		case 12:
		    f_lseek(GBK_12,foffset);
		    f_read(GBK_12,mat,csize,&br_xqy);
			//W25QXX_Read(mat,foffset+ftinfo.f12addr,csize);
			break;
		case 16:
		    f_lseek(GBK_16,foffset);
		    f_read(GBK_16,mat,csize,&br_xqy);
			//W25QXX_Read(mat,foffset+ftinfo.f16addr,csize);
			break;
		case 24:
		    f_lseek(GBK_24,foffset);
		    f_read(GBK_24,mat,csize,&br_xqy);
			//W25QXX_Read(mat,foffset+ftinfo.f24addr,csize);
			break;
			
	}
	//uart_printf("\n--%d------%d-----%d-\n",size,csize,foffset);
	//printf_format(mat,csize);
	//printf_format_bit(mat,size,csize);
}  
//��ʾһ��ָ����С�ĺ���
//x,y :���ֵ�����
//font:����GBK��
//size:�����С
//mode:0,������ʾ,1,������ʾ	 

void Show_Font(u16 x,u16 y,u8 *font,u8 size,u8 mode)
{
	u8 temp,t,t1;
	u16 y0=y;
	u8 dzk[72];   
	u8 csize=(size/8+((size%8)?1:0))*(size);//�õ�����һ���ַ���Ӧ������ռ���ֽ���	 
	if(size!=12&&size!=16&&size!=24)
	    return;	//��֧�ֵ�size
	Get_HzMat(font,dzk,size);	//�õ���Ӧ��С�ĵ������� 
	for(t=0;t<csize;t++)
	{   												   
		temp=dzk[t];			//�õ���������                          
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)
			    DrawPixel_1(x,y,POINT_COLOR);
			else if(mode==0)
			    DrawPixel_1(x,y,BACK_COLOR); 
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}  
}
//��ָ��λ�ÿ�ʼ��ʾһ���ַ���	    
//֧���Զ�����
//(x,y):��ʼ����
//width,height:����
//str  :�ַ���
//size :�����С
//mode:0,�ǵ��ӷ�ʽ;1,���ӷ�ʽ    	   		   
void Show_Str(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size,u8 mode)
{					
	u16 x0=x;
	u16 y0=y;							  	  
    u8 bHz=0;     //�ַ���������  	    				    				  	  
    while(*str!=0)//����δ����
    { 
        if(!bHz)
        {
	        if(*str>0x80)
	        {
	            bHz=1;//���� 
	        }
	        else              //�ַ�
	        {      
                if(x>(x0+width-size/2))//����
				{				   
					y+=size;
					x=x0;	   
				}							    
		        if(y>(y0+height-size))
		            break;//Խ�緵��      
		        if(*str==13)//���з���
		        {         
		            y+=size;
					x=x0;
		            str++; 
		        }  
		        else 
		        {
		            LCD_ShowChar(x,y,*str,size,mode);//��Ч����д�� 
		        }
				str++; 
		        x+=size/2; //�ַ�,Ϊȫ�ֵ�һ�� 
	        }
        }
        else//���� 
        {     
            bHz=0;//�к��ֿ�    
            if(x>(x0+width-size))//����
			{	    
				y+=size;
				x=x0;		  
			}
	        if(y>(y0+height-size))
	            break;//Խ�緵��  						     
	        Show_Font(x,y,str,size,mode); //��ʾ�������,������ʾ 
	        str+=2; 
	        x+=size;//��һ������ƫ��	    
        }						 
    }   
}  			 		 
//��ָ����ȵ��м���ʾ�ַ���
//����ַ����ȳ�����len,����Show_Str��ʾ
//len:ָ��Ҫ��ʾ�Ŀ��			  
void Show_Str_Mid(u16 x,u16 y,u8*str,u8 size,u8 len)
{
	u16 strlenth=0;
   	strlenth=strlen((const char*)str);
	strlenth*=size/2;
	if(strlenth>len)
	{
	    Show_Str(x,y,lcddev.width,lcddev.height,str,size,1);
	}
	else
	{
		strlenth=(len-strlenth)/2;
	    Show_Str(strlenth+x,y,lcddev.width,lcddev.height,str,size,1);
	}
}   
//��ʼ������
//����ֵ:0,�ֿ����.
//		 ����,�ֿⶪʧ
s32 font_init(void)
{	
    s32 ret = 0;
    GBK_12 = (FIL*)mymalloc(sizeof(FIL));//�����ڴ�
    GBK_16 = (FIL*)mymalloc(sizeof(FIL));//�����ڴ�
    GBK_24 = (FIL*)mymalloc(sizeof(FIL));//�����ڴ�
    if(!GBK_12 || !GBK_16 || !GBK_24)
    {
        uart_printf("font int malloc fail!\n");
        return -1;
    }
    
    if(f_open(GBK_12,GBK12_PATH,FA_READ))//���ļ�
	{
	    ret = -1;
		uart_printf("��%s�ļ�����\n",GBK12_PATH);
	}
	if(f_open(GBK_16,GBK16_PATH,FA_READ))//���ļ�
	{
	    ret = -1;
		uart_printf("��%s�ļ�����\n",GBK16_PATH);
	}
	if(f_open(GBK_24,GBK24_PATH,FA_READ))//���ļ�
	{
	    ret = -1;
		uart_printf("��%s�ļ�����\n",GBK24_PATH);
	}
	return ret;
}

void lcd_show_text(u16 x,u16 y,u8*str,u8 size,u8 mode)
{
    Show_Str(x,y,lcddev.width,lcddev.height,str,size,mode);
}

void lcd_show(u16 row,u16 line,u8*str,u8 size,u8 mode)
{
    u16 x,y;
    y = size*(row);
    x = size*(line);
    #if 0 /*xqy 2018-6-17*/
    if(x>(LCD_WIDTH-size) || y>(LCD_HIGH-size))
        return;
    #endif
    Show_Str(x,y,lcddev.width,lcddev.height,str,size,mode);
}
























		  






