#include "printer.h"
#include "./escpos/init_parser.h"
#include <QDebug>

CPrinter *CPrinter::instance = NULL;

extern TASK_HANDLE TASK_HANDLE_MAIN ;

#define CONVHEX(c) (c >= 'a' ? c-'a'+10 : c >='A' ? c-'A'+10 : c -'0')


#ifdef __cplusplus
extern "C"
{
#endif

#define msleep(n) usleep(n*1000)

int do_wait(char *arg)
{
    msleep(atoi(arg));
    return 0;
}
int do_key(char *arg)
{
    CPrinter::printer_send(arg,1);
    return 0;
}
int do_text(char *arg)
{
    int length = strlen(arg);

    unsigned char data = 0;

    switch(arg[length - 1])
    {
    case 'h':
    case 'H':
        {
            int idx;
            for (idx = 0; idx < length - 1; idx++)
            {
                data = data * 16 + CONVHEX(arg[idx]);
            }
        }
        break;
    default :
        {
            int idx;
            
            if ( (length > 2)
                && (   'x' == arg[1]
                    || 'X' == arg[1]))
            {
                for (idx = 2; idx < length ; idx++)
                {
                    data = data * 16 + CONVHEX(arg[idx]);
                }
            }
            else
            {
                for (idx = 0; idx < length ; idx++)
                {
                    data = data * 10 + arg[idx] - '0';
                }
            }
        }
        break;
        
    }

    CPrinter::printer_send((char *)&data,1);

    return 0;
}
int do_null(char *arg)
{
    return 0;
}
int do_string(char *arg)
{
    CPrinter::printer_send(arg,strlen(arg));

    return 0;
}

#ifdef __cplusplus
}
#endif


CPrinter::CPrinter(QObject *parent):
    m_iFd (-1)
{
}

CPrinter::~CPrinter()
{
   if (m_iFd != -1)
   {
       ::close(m_iFd);
       m_iFd = -1;
       if (m_taskHdl)
       {
           Task_RelaseTask(m_taskHdl);
           m_taskHdl = NULL;
       }
   }
}

void CPrinter::printerMsgProc(void *para,SAT_MSG_HEAD *pMsg)
{
    CPrinter *printer = (CPrinter *)para;

    switch(pMsg->event)
    {
    case INIT_ALL_THREAD_EVENT:
        {
            qDebug() << "printer started" << endl; 
        }
        break;
    case MAIN_PRINTER_MSG:
        printer->print(pMsg);
        break;
    default:
        break;
   }
}

CPrinter * CPrinter::getInstance()
{
    if (NULL == instance)
    {
        instance = new CPrinter();
        
        instance->m_taskHdl   = Task_CreateMessageTask(CPrinter::printerMsgProc ,NULL,NULL,instance);

        if (!instance->m_taskHdl)
        {
            qDebug() << "getInstance Task_CreateMessageTask Failed" << endl; 
        }
        else
        {
            VOS_SndMsg2(instance->m_taskHdl ,INIT_ALL_THREAD_EVENT,0,NULL);
        }
    }

    return instance;
}

void CPrinter::deleteInst()
{
    if (instance)
    {
        delete instance;

        instance = NULL;
    }
}

int CPrinter::printer_send(char *data, int length)
{
   return  ::write(getInstance()->m_iFd,data,length);
}


void CPrinter::print(SAT_MSG_HEAD *pMsg)
{
    /* creater printer */

    SAT_MSG_STRU *pSatMsg = (SAT_MSG_STRU *)pMsg;

    
    m_iFd = ::open("/dev/usb/lp0", O_RDWR);
    if (m_iFd != -1)
    {

        init_parse_data(pSatMsg->data);
        
        ::close(m_iFd);

        m_iFd = -1;
    }
}

int CPrinter::snd2Printer(SAT_MSG_HEAD *pMsg)
{
    int ret = VOS_SndMsg(m_taskHdl,(SAT_MSG_HEAD *)pMsg);

    if (0 != ret)
    {
        
    }

    return ret;
}

int CPrinter::snd2Printer(char *cont,int iLength)
{
    int ret = -1;

    SAT_MSG_STRU *pMsg = NULL;

	pMsg = (SAT_MSG_STRU *)SatAllocMsg(0 + iLength);
	if (NULL == pMsg )
	{
		return ret;
	}

    pMsg->msgHead.event = MAIN_PRINTER_MSG;

    memcpy(pMsg->data,cont,iLength);

    ret = VOS_SndMsg(m_taskHdl,(SAT_MSG_HEAD *)pMsg);

    if (0 != ret)
    {
    }
    return ret;

}

