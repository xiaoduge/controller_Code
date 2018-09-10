#ifndef BUZZER_CTL_H_
#define BUZZER_CTL_H_

#define	BC_IOCTL_BASE	'B'

struct buzzer_ctl_io_info {
	unsigned int iostate;
};

struct buzzer_ctl_poll_info {
	unsigned int poll_interval;     /* msec */
	unsigned int poll_interval_max; /* msec */
	unsigned int poll_interval_min; /* msec */
};


typedef enum
{
   BUZZER_CTL_BUZZ = 0,
   BUZZER_CTL_OUTPUT_NUM,
}BUZZER_CTL_OUTPUT_ENUM;

typedef enum
{
   BUZZER_CTL_INPUT_DUMMY = 0,
   BUZZER_CTL_INPUT_NUM,
}BUZZER_CTL_INPUT_ENUM;

#define	BCIOC_GETOUTPUT	    _IOR(BC_IOCTL_BASE,  0, struct buzzer_ctl_io_info)
#define	BCIOC_GETINPUT		_IOR(BC_IOCTL_BASE,  1, struct buzzer_ctl_io_info)
#define	BCIOC_GETPOLLINFO	_IOR(BC_IOCTL_BASE,  2, struct buzzer_ctl_poll_info)
#define	BCIOC_SETPOLLINFO   _IOWR(BC_IOCTL_BASE, 3, struct buzzer_ctl_poll_info)

#endif

