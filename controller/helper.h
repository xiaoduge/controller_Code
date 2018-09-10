#ifndef _HELPER_H_
#define _HELPER_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
       unsigned char hour;
       unsigned char min;
       unsigned char sec;                 

       unsigned short w_year;
       unsigned char  w_month;
       unsigned char  w_date;
       unsigned char  week;            
}TM_STRU;

typedef struct
{
    int size; // buffer size
    int unit; // item size
    int front;
	int rear;
	char *buffer;
}RING_BUFFER_STRU;


void att_helper_trim(char **pstr);
int Att_value_parse(char *parg,char sep,char **pptag,char **ppvalue);
int Att_value_split(char *parg,char sep);
char *att_int2percent(int value ,int mult);

unsigned char TIME_Get(unsigned int time,TM_STRU *tm);
unsigned int RTC_Set(unsigned short syear,unsigned char smon,unsigned char sday,unsigned char hour,unsigned char min,unsigned char sec);
void Write_sys_int(const char *sysfilename,int value);
void Get_sys_int(const char *sysfilename, int* value);
void Write_sys_string(const char *sysfilename,const char* value);
void Get_sys_string(const char *sysfilename, char* value);
unsigned char helper_hex2bin(unsigned char ucData1,unsigned char ucData2);

#ifdef __cplusplus
}
#endif

#endif
