#include <string.h>
#include<ctype.h>
#include <stdio.h>

#include "helper.h"

#ifdef __cplusplus
extern "C"
{
#endif



//设置时钟
//把输入的时钟转换为秒钟
//以1970年1月1日为基准
//1970~2099年为合法年份
//返回值:0,成功;其他:错误代码.
//月份数据表                                                                       
unsigned char const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表  
//平年的月份日期表
const unsigned char mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};


unsigned char TIME_Get_Week(unsigned short year,unsigned char month,unsigned char day)
{    
   unsigned short temp2;
   unsigned char yearH,yearL;
  
   yearH = year/100;     
   yearL = year%100;
   // 如果为21世纪,年份数加100 
   if (yearH > 19) yearL+=100;
   // 所过闰年数只算1900年之后的 
   temp2 = yearL+yearL/4;
   temp2 = temp2%7;
   temp2 = temp2+day+table_week[month-1];
   if ((yearL%4) == 0
        && month < 3)
    {
       temp2--;
    }
   return(temp2%7);
}

//判断是否是闰年函数
//月份   1  2  3  4  5  6  7  8  9  10 11 12
//闰年   31 29 31 30 31 30 31 31 30 31 30 31
//非闰年 31 28 31 30 31 30 31 31 30 31 30 31
//输入:年份
//输出:该年份是不是闰年.1,是.0,不是
unsigned char Is_Leap_Year(unsigned short year)
{                    
   if(year%4==0) //必须能被4整除
   {
      if(year%100==0)
      {
          if(year%400==0)
              return 1;//如果以00结尾,还要能被400整除          
          else
              return 0;  
      }
      else 
          return 1;  
   }
   else 
   {
       return 0;
   }
}                           

unsigned int RTC_Set(unsigned short syear,unsigned char smon,unsigned char sday,unsigned char hour,unsigned char min,unsigned char sec)
{
   unsigned short t;
   unsigned int seccount=0;

   
   if(syear < 2000 || syear > 2099)
        return 0;                    //syear范围1970-2099，此处设置范围为2000-2099       
   for(t = 1970; t < syear; t++)     //把所有年份的秒钟相加
   {
      if(Is_Leap_Year(t))
        seccount += 31622400;         //闰年的秒钟数
      else 
        seccount += 31536000;         //平年的秒钟数
   }
   smon -= 1;
   for(t = 0; t < smon; t++)         //把前面月份的秒钟数相加
   {
      seccount += (unsigned int)mon_table[t]*86400;//月份秒钟数相加
      if(Is_Leap_Year(syear)&&t==1)
        seccount += 86400;           //闰年2月份增加一天的秒钟数        
   }
   seccount += (unsigned int)(sday-1)*86400;//把前面日期的秒钟数相加
   seccount += (unsigned int)hour*3600;     //小时秒钟数
   seccount += (unsigned int)min*60;        //分钟秒钟数
   seccount += sec;                //最后的秒钟加上去
 
   return seccount;     
}


//得到当前的时间
//返回值:0,成功;其他:错误代码.
unsigned char TIME_Get(unsigned int time,TM_STRU *tm)
{
   static unsigned short daycnt = 0;
          unsigned int  timecount = 0;
          unsigned int  temp = 0;
          unsigned short temp1 = 0;

   timecount = time;

   temp = timecount/86400;   //得到天数(秒钟数对应的)
   
   //if(daycnt != temp)//超过一天了
   {      
      daycnt = temp;
      temp1 = 1970;  //从1970年开始
      while(temp >= 365)
      {                         
         if(Is_Leap_Year(temp1))//是闰年
         {
            if(temp >= 366)
            {
                temp -= 366; //闰年的秒钟数
            }
            else 
            {
                temp1++; // ylf: why ++ ??
                break;
            } 
         }
         else
         {
            temp -= 365;       //平年
         }
         temp1++; // increase year count
      }  
      tm->w_year = temp1;//得到年份
      temp1 = 0;
      while( temp >=28 )//超过了一个月
      {
         if(Is_Leap_Year(tm->w_year) && temp1 == 1)//当年是不是闰年/2月份
         {
            if(temp >= 29)
            {
                temp -= 29;//闰年的秒钟数
            }
            else 
            {
                break;
            }
         }
         else
         {
            if(temp >= mon_table[temp1])
            {
                temp -= mon_table[temp1];//平年
            }
            else 
            {
                break;
            }
         }
         temp1++; 
      }
      tm->w_month = temp1 + 1;//得到月份
      tm->w_date = temp + 1;  //得到日期
   }
   temp = timecount%86400;     //得到秒钟数      
   tm->hour = temp/3600;     //小时
   tm->min = (temp%3600)/60; //分钟     
   tm->sec = (temp%3600)%60; //秒钟
   tm->week = TIME_Get_Week(tm->w_year,tm->w_month,tm->w_date);//获取星期  
   return 0;
}


void att_helper_trim(char **pstr)
{
    char *p,*q;

    p = *pstr;

    if (!p || strlen(p) == 0)
    {
        return;
    }

    while(isspace(*p)) p++;

    for ( q = p + strlen(p)-1 ; q >= p && isspace(*q) ; q-- ) 
    {
        *q = 0;
    }

    *pstr = p;

}

int Att_value_parse(char *parg,char sep,char **pptag,char **ppvalue)
{
    *pptag = parg;

    *ppvalue = strchr(*pptag,sep);
    
    if (!*ppvalue)
    {
        return 0;
    }
        
    **ppvalue = 0;
    
    (*ppvalue)++;

    att_helper_trim(pptag);   

    att_helper_trim(ppvalue);   

    return 1;
}

int Att_value_split(char *parg,char sep)
{
    int cnt = 0;
    
    char *pvalue = strchr(parg,sep);

    while (pvalue)
    {
        *pvalue = 0;

         pvalue++;

         pvalue = strchr(pvalue,sep);

         cnt++;
    }

    return cnt;
}


char *att_int2percent(int value ,int mult)
{
    static char outbuf[32];
    
    float ov = (value *1.0) / mult;

    //ov *= 100;

    sprintf(outbuf,"%.2f%%",ov);

    return outbuf;
    
}

int Ringbuff_GetOccuSlots(RING_BUFFER_STRU *rb)
{

    return (rb->front + rb->size - rb->rear) % rb->size;
}

int Ringbuff_GetFreeSlots(RING_BUFFER_STRU *rb)
{
    return (rb->size - Ringbuff_GetOccuSlots(rb));
}

int Ringbuff_Empty(RING_BUFFER_STRU *rb)
{
    return (rb->front == rb->rear);
}

int Ringbuff_Full(RING_BUFFER_STRU *rb)
{
    return (((rb->front + 1)% rb->size) == rb->rear);
}

int Ringbuff_Fill(RING_BUFFER_STRU *rb,char *data,int length)
{
    int items = length / rb->unit;

    if (length > rb->unit * rb->size)
    {
        return -1;
    }

    if (Ringbuff_GetFreeSlots(rb) >= items)
    {
        if (rb->front >= rb->rear)
        {
           if (items < rb->size - rb->front) 
           {
                memcpy(&rb->buffer[rb->front*rb->unit],data,length);
                rb->front += items;
           }
           else
           {
               memcpy(&rb->buffer[rb->front*rb->unit],data,(rb->size - rb->front)*rb->unit);
               data += (rb->size - rb->front)*rb->unit;
               length -= (rb->size - rb->front);
               memcpy(&rb->buffer[0],data,length);
               rb->front = length/rb->unit;
           }
        }
        else
        {
            memcpy(&rb->buffer[rb->front*rb->unit],data,length);
            rb->front += items;
        }
        
    }
    else
    {
       memcpy(rb->buffer,data,length);
       rb->rear = 0;
       rb->front = items;
    }
    return 0;
}

void RingBuff_Increse( int value, int size)
{
    value = (value +1)%size;
}
void Write_sys_int(const char *sysfilename,int value)
{
    FILE *fp;
    char buf[20];

    memset(buf,0,sizeof(buf));
    
    fp = fopen(sysfilename,"w+");

    sprintf(buf,"%d",value);
    
    if (fp)
    {
        fwrite(buf,sizeof(buf),1,fp);
        fclose(fp);
    }

}
void Get_sys_int(const char *sysfilename, int* value)
{
    FILE *fp;
    char buf[20];

    memset(buf,0,sizeof(buf));
    
    fp = fopen(sysfilename,"r+");
    
    if (fp)
    {
        fread(buf,sizeof(buf),1,fp);

        *value = atoi(buf);
        
        fclose(fp);
    }
}

void Write_sys_string(const char *sysfilename,const  char* value)
{
    FILE *fp;
    char buf[250];

    memset(buf,0,sizeof(buf));
    
    fp = fopen(sysfilename,"w+");

    sprintf(buf,"%s",value);
    
    if (fp)
    {
        fwrite(buf,strlen(buf),1,fp);
        fclose(fp);
    }

}
void Get_sys_string(const char *sysfilename, char* value)
{
    FILE *fp;
    char buf[250];

    memset(buf,0,sizeof(buf));
    
    fp = fopen(sysfilename,"r+");
    
    if (fp)
    {
        fread(buf,sizeof(buf),1,fp);

        strcpy(value, buf);
        
        fclose(fp);
    }
}


unsigned char helper_hex2bin(unsigned char ucData1,unsigned char ucData2)
{
    unsigned char ucBin;

    if (ucData1 >= 'a' && ucData1 <= 'f')
    {
        ucData1 = ucData1 - 'a' + 10;
    }
    else if(ucData1 >= 'A' && ucData1 <= 'F')
    {
        ucData1 = ucData1 - 'A' + 10;
    }
    else
    {
        ucData1 = ucData1 - '0' ;
    }

    if (ucData2 >= 'a' && ucData2 <= 'f')
    {
        ucData2 = ucData2 - 'a' + 10;
    }
    else if(ucData2 >= 'A' && ucData2 <= 'F')
    {
        ucData2 = ucData2 - 'A' + 10;
    }
    else
    {
        ucData2 = ucData2 - '0' ;
    }
    
    ucBin =  (ucData1 << 4) | ucData2;

    return ucBin;
    
}

#ifdef __cplusplus
}
#endif

