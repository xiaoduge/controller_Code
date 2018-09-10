#ifndef _TIMER_H_1
#define _TIMER_H_1

#define TIMER_UNIT (1000) // 100MS

#define DECREASE_TIME(time) do {(time) = ((time) >=TIMER_UNIT) ? (time)-TIMER_UNIT : 0;}while(0)

enum
{
  TIMER_ONE_SHOT = 0,
  TIMER_PERIOD,
  TIMER_NUM,
};

typedef void (* sys_timeout_handler)(void *arg);

typedef struct  sys_timeo{
  struct sys_timeo *next;
  unsigned int time;
  unsigned int type;
  unsigned int period;
  unsigned int id;
  sys_timeout_handler h;
  void *arg;
}SYS_TIMEO;

typedef struct
{
    int dummy;
    
}TIMER_PARAM_STRU;

void TimerProc(void *lParam);
void TimerInit(void);
SYS_TIMEO * timer_add(unsigned int msecs, int type,sys_timeout_handler h, void *arg);
SYS_TIMEO * timer_start(SYS_TIMEO *to);
void timer_stop(SYS_TIMEO *to);
void timer_reset(SYS_TIMEO *timeout,unsigned int msecs, int type,sys_timeout_handler h, void *arg);

SYS_TIMEO * timer_new(unsigned int msecs, int type,sys_timeout_handler h, void *arg);
void timer_del(SYS_TIMEO *to);

#endif
