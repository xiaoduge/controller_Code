#ifndef _PRINTER_H_
#define _PRINTER_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <libgen.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <sys/time.h>

#include <QObject>

#include "datatype.h"
#include "cminterface.h"
#include "notify.h"


#include "SatIpc.h"
#include "msgdef.h"
#include "msg.h"
#include "Interface.h"
#include "memory.h"
#include "sapp.h"
#include "rpc.h"
#include "Errorcode.h"

#include "messageitf.h"
#include "cminterface.h"

#include "helper.h"
#include "vtask.h"
#include "Display.h"

class CPrinter : public QObject
{
    Q_OBJECT
private:
    explicit CPrinter(QObject *parent = 0);
    ~CPrinter();

public:   
signals:
    void feedback(bool result);
    
public slots:

public:
    void print(SAT_MSG_HEAD *pMsg);

    int snd2Printer(SAT_MSG_HEAD *pMsg);

    int snd2Printer(char *cont,int iLength);

private :
    
    int m_iFd;
    
    TASK_HANDLE m_taskHdl;

public:
    static CPrinter *instance;
    static void printerMsgProc(void *para,SAT_MSG_HEAD *pMsg);
    static CPrinter *getInstance();

    static int printer_send(char *data, int length);
    static void deleteInst();
    
};


#endif

