
/*
** $Id: misc.h,v 1.22 2005/03/08 02:14:03 snig Exp $
**
** misc.h: the head file for Miscellous module.
**
** Copyright (C) 2003 Feynman Software.
** Copyright (C) 1999 ~ 2002 Wei Yongming.
**
** Create date: 1999/01/03
*/

#ifndef GUI_MISC_H
    #define GUI_MISC_H

/* Function definitions */

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


#define ETC_MAXLINE             1024
#define ETC_FILENOTFOUND        -1
#define ETC_SECTIONNOTFOUND     -2
#define ETC_KEYNOTFOUND         -3
#define ETC_TMPFILEFAILED       -4
#define ETC_FILEIOFAILED        -5
#define ETC_INTCONV             -6
#define ETC_OK                  0



typedef unsigned int GHANDLE;


typedef struct _ETCSECTION
{
    int key_nr;               /* key number in the section */
    char *name;               /* name of the section */
    char **keys;              /* key string arrays */
    char **values;            /* value string arrays */
} ETCSECTION;
typedef ETCSECTION* PETCSECTION;

typedef struct _ETC_S
{
    int section_nr;           /* number of sections */
    PETCSECTION sections;     /* pointer to section arrays */
} ETC_S;


extern GHANDLE hMgEtc;



int GetValueFromEtcFile(const char* pEtcFile, const char* pSection,
                               const char* pKey, char* pValue, int iLen);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* GUI_MISC_H */

