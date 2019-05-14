#ifndef _SAT_IPC_H_
#define _SAT_IPC_H_



#ifndef WIN32

/// pthread

#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef void * sp_thread_result_t;
typedef pthread_mutex_t sp_thread_mutex_t;
typedef pthread_cond_t  sp_thread_cond_t;
typedef pthread_t       sp_thread_t;
typedef pthread_attr_t  sp_thread_attr_t;

#define sp_thread_mutex_init(m,a)   pthread_mutex_init(m,a)
#define sp_thread_mutex_destroy(m)  pthread_mutex_destroy(m)
#define sp_thread_mutex_lock(m)     pthread_mutex_lock(m)
#define sp_thread_mutex_unlock(m)   pthread_mutex_unlock(m)

#define sp_thread_cond_init(c,a)    pthread_cond_init(c,a)
#define sp_thread_cond_destroy(c)   pthread_cond_destroy(c)
#define sp_thread_cond_wait(c,m)    pthread_cond_wait(c,m)
#define sp_thread_cond_signal(c)    pthread_cond_signal(c)
#define sp_thread_cond_timedwait(c,m,t) pthread_cond_timedwait(c,m,t)

#define sp_thread_attr_init(a)        pthread_attr_init(a)
#define sp_thread_attr_setdetachstate pthread_attr_setdetachstate
#define SP_THREAD_CREATE_DETACHED     PTHREAD_CREATE_DETACHED

#define sp_thread_self    pthread_self
#define sp_thread_create  pthread_create

#define SP_THREAD_CALL
typedef sp_thread_result_t ( * sp_thread_func_t )( void * args );

#define sp_sleep(x) sleep(x)

typedef pthread_key_t	 tls_key_t;

typedef sem_t sp_sem_t;

#define sp_thread_sem_init(sem,init,cnt)  sem_init(sem,0,init)
#define sp_thread_sem_post(sem)           sem_post(sem)
#define sp_thread_sem_pend(sem)           sem_wait(sem)
#define sp_thread_sem_destroy(sem)        sem_destroy(sem)
#define sp_thread_sem_getvalue(sem,value) sem_getvalue(sem,value);

int  tls_init(tls_key_t *pkey);

int  tls_setkey(tls_key_t *pkey,void* value);
void *  tls_getkey(tls_key_t *pkey);

#ifdef __cplusplus
}
#endif

#else ///////////////////////////////////////////////////////////////////////

// win32 thread

#include <winsock2.h>
#include <process.h>
#include <windows.h> 

#ifdef __cplusplus
extern "C"
{
#endif

typedef unsigned sp_thread_t;

typedef unsigned sp_thread_result_t;
#define SP_THREAD_CALL __stdcall
typedef sp_thread_result_t ( __stdcall * sp_thread_func_t )( void * args );

typedef HANDLE  sp_thread_mutex_t;
typedef HANDLE  sp_thread_cond_t;
typedef DWORD   sp_thread_attr_t;

#define SP_THREAD_CREATE_DETACHED 1
#define sp_sleep(x) Sleep(1000*x)

typedef HANDLE  sp_sem_t;

typedef DWORD 	 tls_key_t;

int sp_thread_mutex_init( sp_thread_mutex_t * mutex, void * attr );
int  sp_thread_mutex_destroy( sp_thread_mutex_t * mutex );
int  sp_thread_mutex_lock( sp_thread_mutex_t * mutex );
int  sp_thread_mutex_unlock( sp_thread_mutex_t * mutex );
int  sp_thread_cond_init( sp_thread_cond_t * cond, void * attr );
int  sp_thread_cond_destroy( sp_thread_cond_t * cond );
int  sp_thread_cond_wait( sp_thread_cond_t * cond, sp_thread_mutex_t * mutex );
int  sp_thread_cond_timedwait( sp_thread_cond_t * cond, sp_thread_mutex_t * mutex );

int  sp_thread_cond_signal( sp_thread_cond_t * cond );
sp_thread_t  sp_thread_self();
int  sp_thread_attr_init( sp_thread_attr_t * attr );
int  sp_thread_attr_setdetachstate( sp_thread_attr_t * attr, int detachstate );
int  sp_thread_create( sp_thread_t * thread, sp_thread_attr_t * attr,
		sp_thread_func_t myfunc, void * args );
int  tls_init(tls_key_t *pkey);
int  tls_setkey(tls_key_t *pkey,void* value);
void*  tls_getkey(tls_key_t *pkey);
int  sp_thread_sem_init(sp_sem_t *psem,int init,int cnt);
int sp_thread_sem_post(sp_sem_t *sem);
int sp_thread_sem_pend(sp_sem_t *sem);
int sp_thread_sem_destroy( sp_sem_t *sem );
#ifdef __cplusplus
}
#endif

#endif


#endif
