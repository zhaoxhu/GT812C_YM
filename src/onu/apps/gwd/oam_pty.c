#include "oam_pty.h"
#include <network.h>
#include "plat_common.h"
#include <network.h>
#include "plat_common.h"
#include "oam.h"



//�����
#define OAM_MESSAGE_INCLUDE	1
#define gw_printf	cs_printf
#define PTY_TIMER_INTERVAL
/* Defines for Semaphore, Queue Timeout parameters */
#define GW_OSAL_NO_WAIT          (0)
#define GW_OSAL_WAIT_FOREVER     (-1)

#define PTY_PACKET       	0x06
#define PTY_NOTI_MSG        0x07
#define PTY_TIMER_MSG       0x08
#define PTY_ONU_LOSE        0x09

#define CLI_PTY_TRANSMIT	0x14

#define NAME_LEN 			8
#define PTY_NUM_MAX			8
#define OAM_PTY_PORT_BASE	7000
#define OAM_PTY_FD_BASE		7000


#define PTY_CONNECT_REQ_RESP_ACCEPT 3
#define PTY_CONNECT_REQ_RESP_REJECT 2


#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif


typedef cyg_bool_t gw_bool_t;
#define PTY_SHELL_CLOSE_FLAG	0x5192

#define GW_OSAL_MAX_THREAD		32

#define TIME_OUT_MAX		30		//���ʱʱ��(��λ:��)

#define MASTER_WRITE	0
#define SLAVE_WRITE		1
#define MASTER_READ		0
#define SLAVE_READ		1

#define CON_CTL_CODE_CONRES_LEN		2

#define CONSOLE_READ_INTERVAL_TIME	25		//��λ:����

#define SOCKET_PTY			0
#define PIPE_PTY			0
#define QUEUE_PTY			0
#define BUF_PTY_SUPPORT		1

#define PTY_BUF_SIZE		1024


//�������ͱ���
typedef signed char gw_int8;
typedef unsigned char gw_uint8;
typedef int gw_int32;
typedef unsigned int gw_uint32;

//����ö��
typedef enum
{
	PTY_ERR_OK = 0,
	PTY_ERR_CREATE_PTY,
	PTY_ERR_OPEN_MASTER,
	PTY_ERR_OPEN_SLAVE,
	PTY_ERR_CREAT_QUEUE,
	PTY_ERR_CREATE_MAIN_THREAD,
	PTY_ERR_CREATE_SUB_THREAD,
	PTY_ERR_ADD_TIMER,
}PTY_ERR_CODE;

typedef enum
{
	GW_E_OK = 		0,
	GW_E_ERROR = 	-1,
}gw_status;

enum
{
	GW_LOG_LEVEL_DEBUG,
	GW_LOG_LEVEL_INFO,
	GW_LOG_LEVEL_MINOR,
	GW_LOG_LEVEL_MAJOR,
	GW_LOG_LEVEL_CRI,
};
#ifndef OAM_MESSAGE_INCLUDE
typedef struct gwtt_oam_session_info
{
	unsigned int SendSerNo;
	unsigned char SessionID[8];
} GWTT_OAM_SESSION_INFO;
#else
#endif

typedef struct _CLI_PTY_CTRL_
{
	long lConnect;
	long lFd;
	char bSessionId[8];
	long lTimeOut;
	long lSendKpl;
	unsigned long lSerNo;
}CLI_PTY_CTRL;

#if (SOCKET_PTY)
typedef struct
{
	struct sockaddr_in sa_master;
	struct sockaddr_in sa_slave;
}socket_pty_t;
#endif

#if (PIPE_PTY)
typedef struct
{
	int master_read;
	int master_write;
	int slave_read;
	int slave_write;
}pipe_pty_t;
#endif

#if (QUEUE_PTY)
typedef struct
{
	unsigned int master_queue_id;
	unsigned int slave_queue_id;
}queue_pty_t;
#endif

typedef cyg_mutex_t	gw_mutex_t;

#if (BUF_PTY_SUPPORT)


#define DATA_SIZE	1024
typedef struct
{
	int data_head;		//��Ч���ݵĿ�ʼ(����) ����λ��
	int data_tail;		//��Ч���ݵĽ�β(������) д��λ��
	int data_len;		//��Ч���ݵĳ���(���Ϊsize-1)
	int size;		//�������Ĵ�С
	char *buf;		//����������ʼ��ַ
	gw_mutex_t mutex;		//������ֹͬʱ����������
	gw_mutex_t mutex_read;		//����û�����ݵ�ʱ�������
}pty_buf_t;

typedef struct
{
	pty_buf_t master_buf;
	pty_buf_t slave_buf;
}buf_pty_t;
#endif


typedef union
{
	#if (SOCKET_PTY)
	socket_pty_t socket_pty;
	#endif
	#if (PIPE_PTY)
	pipe_pty_t pipe_pty;
	#endif
	#if (QUEUE_PTY)
	queue_pty_t queue_pty;
	#endif
	#if (BUF_PTY_SUPPORT)
	buf_pty_t buf_pty;
	#endif
}pty_connect_t;

typedef struct pty
{
	char name[NAME_LEN];
	int pty_num;
	int fd_master;
	int fd_slave;
	pty_connect_t pty_connect;
	struct pty *next;
}Pty;

typedef enum
{
   CON_CTL_CODE_KPL = 1,
   CON_CTL_CODE_CONREQ,
   CON_CTL_CODE_CONRES,
   CON_CTL_CODE_FREEREQ,
   CON_CTL_CODE_FREERES,
   CON_CTL_CODE_DATA
} CONSOLE_CTRL_CODE;

#ifndef OAM_MESSAGE_INCLUDE
struct gw_epon_timer_t;

typedef void (* gw_epon_timer_func_t) (struct gw_epon_timer_t *timer);

struct gw_epon_timer_t {
        /* the tick to expire */
        gw_uint32                   tick;
        /* timer double link list */
        struct gw_epon_timer_t             *prev;
        struct gw_epon_timer_t             *next;
        /* timer function */
        gw_epon_timer_func_t               tmfunc;
        void                            * opaque;
};



typedef struct gw_oam_message_list_node
{
	struct gw_oam_message_list_node *next;
	cs_uint8 GwOpcode;
	cs_uint32 SendSerNo;
	cs_uint16 WholePktLen;
	cs_uint16 RevPktLen;
	cs_uint8 SessionID[8];
	struct gw_epon_timer_t TimerID;
	cs_uint8 *pPayLoad;
} GW_OAM_MESSAGE_NODE;
#else
typedef GWTT_OAM_MESSAGE_NODE GW_OAM_MESSAGE_NODE;
#endif






//��������
static int CreatePty(char *name);
static void DeletePty(int pty_num);
static int OpenMasterDev(int pty_num);
static void CloseMasterDev(int fd);
static int OpenSlaveDev(int pty_num);
static void CloseSlaveDev(int fd);
static int pty_write(int fd,char *str,int length);
static int pty_read(int fd,char *str,int length);

static void gw_oam_pty_main_thread_entry(gw_uint32 *para);
static void gw_oam_pty_sub_thread_entry(gw_uint32 *para);
static void OamPtyShellTimerNoti(void *para);
static void OamPtyPacketProcess(GWTT_OAM_SESSION_INFO *pSeInf, char *pPayLoad, long lPayLen);
static void OamPtyNotiMsgProcess(long int flag, long int fd);
static void OamPtyTimerMsgProcess(void);
static int pty_master_fd_get(void);
static int pty_slave_fd_get(void);
static void start_oamPtyCliThread();
static void OamPtyShellCloseNoti(long lFd);
extern int read_from_console(char *str,int length);
extern int write_to_console(char *str,int length);
static void end_oamPtyCliThread();



//��Ҫģ����ʵ�ֵĽӿ�
gw_status gw_pri_queue_create (gw_uint32 *queue_id, const gw_int8 *queue_name, gw_uint32 queue_depth, gw_uint32 data_size, gw_int32 pri_num);
int gw_queue_delete(unsigned int queue_id);
gw_status gw_pri_queue_get (gw_uint32 queue_id, void *data, gw_uint32 size, gw_uint32 *size_copied, gw_int32 timeout);
gw_status gw_pri_queue_put (gw_uint32 queue_id, void *data, gw_uint32 size, gw_int32 timeout, gw_int32 priority);

gw_status gw_thread_create(gw_uint32 *thread_id,  const gw_int8 *thread_name,
                              const void *function_pointer, void *param , gw_uint32 stack_size,
                              gw_uint32 priority, gw_uint32 flags);
gw_status gw_thread_delete(gw_uint32 thread_id);
gw_status gw_thread_delay(gw_uint32 milli_second);
gw_int32 gw_circle_timer_add(gw_uint32 timeout, void (*callback)(void *), void *data);
int gw_comm_onu_msg_send(unsigned char GwOpcode, unsigned int SendSerNo, unsigned char *pSentData, 
									const unsigned short SendDataSize, unsigned char  *pSessionIdfield);
void gw_oam_pty_cli_thread_entry(gw_uint32 * para);
void gw_mutex_init(gw_mutex_t *mutex);
void gw_mutex_destroy(gw_mutex_t *mutex);
gw_bool_t gw_mutex_lock(gw_mutex_t *mutex);
gw_bool_t gw_mutex_trylock(gw_mutex_t *mutex);
void gw_mutex_unlock(gw_mutex_t *mutex);



//ģ�����ṩ�Ľӿ�
extern cs_int32 cs_pri_queue_create (cs_uint32 *queue_id, const cs_int8 *queue_name, cs_uint32 queue_depth, cs_uint32 data_size, cs_int32 pri_num);
extern int cs_queue_delete(unsigned int queue_id);
extern cs_int32 cs_pri_queue_get (cs_uint32 queue_id, void *data, cs_uint32 size, cs_uint32 *size_copied, cs_int32 timeout);
extern cs_int32 cs_pri_queue_put (cs_uint32 queue_id, void *data, cs_uint32 size, cs_int32 timeout, cs_int32 priority);
extern cs_int32 cs_thread_create(cs_uint32 *thread_id, const cs_int8 *thread_name, 
							const void *function_pointer, void *param, cs_uint32 stack_size,
                              cs_uint32 priority, cs_uint32 flags);
extern cs_int32 cs_thread_delay(cs_uint32 milli_second);
cs_int32 cs_circle_timer_add(cs_uint32 timeout, void (*callback)(void *), void *data);
extern int CommOnuMsgSend(unsigned char GwOpcode, unsigned int SendSerNo, unsigned char *pSentData,
						const unsigned short SendDataSize, unsigned char  *pSessionIdfield);
extern void oam_pty_cli_start(void);




//����ȫ�ֱ���

//���ӵ�ȫ�ֱ���
static gw_uint8		g_pty_dev_name[]="oampty";
static gw_int32		g_pty_id;
static gw_int32 	g_pty_master;
static gw_int32		g_pty_slave;

//���е�ȫ�ֱ���
static gw_uint32 g_oam_pty_queue_id = 0;
static gw_uint8 g_oam_pty_queue_name[]="ptyqueue";
static gw_uint32 g_oam_pty_queue_deepth = 100;
static gw_uint32 g_oam_pty_data_szie = 128;
static gw_uint32 g_oam_pty_queue_pri = 3;

//���̵߳�ȫ�ֱ���
static gw_uint32 	g_oam_pty_main_thread_id;
static gw_uint8 	g_oam_pty_main_thread_name[]="ptymthread";
static gw_uint32 	g_oam_pty_main_thread_stack_size = 4*1024;
static gw_uint32 	g_oam_pty_main_thread_pri = 14;

//���̵߳�ȫ�ֱ���
static gw_uint32 	g_oam_pty_sub_thread_id;
static gw_uint8		g_oam_pty_sub_thread_name[]="ptysthread";
static gw_uint32 	g_oam_pty_sub_thread_stack_size = 4*1024;
static gw_uint32 	g_oam_pty_sub_thread_pri = 14;

//��������̵߳�ȫ�ֱ���
static gw_uint32	g_oam_pty_cli_thread_id = GW_OSAL_MAX_THREAD;
static gw_uint8		g_oam_pty_cli_thread_name[]="ptycli";
static gw_uint32 	g_oam_pty_cli_thread_stack_size = 6*1024;
static gw_uint32 	g_oam_pty_cli_thread_pri = 20;


static CLI_PTY_CTRL gmCliPtyCtrl = {0,0,{0},0, 0, 0};

static Pty *head_pty = NULL;
static int pty_maxnum = 0;

//�������ڱ�����Դ
gw_mutex_t master_mutex;
gw_mutex_t slave_mutex;





//��ʼ�� oam pty
extern void init_oam_pty(void)
{
	PTY_ERR_CODE err_code = PTY_ERR_OK;
	//��������
	if(-1 == (g_pty_id = CreatePty(g_pty_dev_name)))
	{
		gw_printf("CreatePty failed\n");
		err_code = PTY_ERR_CREATE_PTY;
		goto err;
	}
	
	//��pty ���������������
	if(-1 == (g_pty_master = OpenMasterDev(g_pty_id)))
	{
		gw_printf("OpenMasterDev failed\n");
		err_code = PTY_ERR_OPEN_MASTER;
		goto err;
	}
	
	//�����������pty ������
	if(-1 == (g_pty_slave = OpenSlaveDev(g_pty_id)))
	{
		gw_printf("OpenSlaveDev failed\n");
		err_code = PTY_ERR_OPEN_SLAVE;
		goto err;
	}
	
	//�������У�ʧ����ر����Ӳ��˳�
	if(GW_E_OK != gw_pri_queue_create(&g_oam_pty_queue_id, g_oam_pty_queue_name,
		g_oam_pty_queue_deepth, g_oam_pty_data_szie, g_oam_pty_queue_pri))
	{
		gw_printf("create %s fail!\r\n", g_oam_pty_queue_name);
		err_code = PTY_ERR_CREAT_QUEUE;
		goto err;
	}

	//����pty ���̣߳�ʧ����ɾ�����У��ر����Ӳ��˳�
	if(GW_E_OK != gw_thread_create(&g_oam_pty_main_thread_id, g_oam_pty_main_thread_name,
		gw_oam_pty_main_thread_entry, NULL, g_oam_pty_main_thread_stack_size, g_oam_pty_main_thread_pri, 0))
	{
		gw_printf("create %s fail !\r\n", g_oam_pty_main_thread_name);
		err_code = PTY_ERR_CREATE_MAIN_THREAD;
		goto err;
	}
	
	//����pty ���̣߳�ʧ����ɾ�����̣߳�ɾ�����У��ر����Ӳ��˳�
	if(GW_E_OK != gw_thread_create(&g_oam_pty_sub_thread_id, g_oam_pty_sub_thread_name, 
		gw_oam_pty_sub_thread_entry, NULL, g_oam_pty_sub_thread_stack_size, g_oam_pty_sub_thread_pri, 0))
	{
		gw_printf("create %s fail !\r\n", g_oam_pty_sub_thread_name);
		err_code = PTY_ERR_CREATE_SUB_THREAD;
		goto err;
	}
	
	//��ʱ����
	if(0 == gw_circle_timer_add(1000, OamPtyShellTimerNoti, NULL))
	{
		gw_printf("gw_circle_timer_add failed\n");
		err_code = PTY_ERR_ADD_TIMER;
		goto err;
	}
err:
	switch(err_code)
	{
		//����������break;	
		case PTY_ERR_ADD_TIMER:
			gw_thread_delete(g_oam_pty_sub_thread_id);
		case PTY_ERR_CREATE_SUB_THREAD:	
			gw_thread_delete(g_oam_pty_main_thread_id);
		case PTY_ERR_CREATE_MAIN_THREAD:
			gw_queue_delete(g_oam_pty_queue_id);
		case PTY_ERR_CREAT_QUEUE:
			CloseSlaveDev(g_pty_slave);
		case PTY_ERR_OPEN_SLAVE:
			CloseMasterDev(g_pty_master);
		case PTY_ERR_OPEN_MASTER:
			DeletePty(g_pty_id);
		case PTY_ERR_CREATE_PTY:
		case PTY_ERR_OK :
			gw_printf("%s success!\n", __func__);
			break;
		default:
			gw_printf("unkonwn err_code :%d\n", err_code);
			break;
	}
}


//ʵ�ִ������нӿ�
gw_status gw_pri_queue_create (gw_uint32 *queue_id, const gw_int8 *queue_name, gw_uint32 queue_depth, gw_uint32 data_size, gw_int32 pri_num)
{
	gw_status ret = GW_E_OK;
	if(CS_E_OSAL_OK == cs_pri_queue_create(queue_id, queue_name, queue_depth, data_size, pri_num))
	{
		ret = GW_E_OK;
	}
	else
	{
		ret = GW_E_ERROR;
	}
	return ret;
}
int gw_queue_delete(unsigned int queue_id)
{
	int ret = 0;
	if(CS_E_OSAL_OK == cs_queue_delete(queue_id))
	{
		ret = 0;
	}
	else
	{
		ret = -1;
	}
	return ret;
}

gw_status gw_pri_queue_get (gw_uint32 queue_id, void *data, gw_uint32 size, gw_uint32 *size_copied, gw_int32 timeout)
{
	gw_status ret = GW_E_OK;
	if(CS_E_OSAL_OK == cs_pri_queue_get(queue_id, data, size, size_copied, timeout))
	{
		ret = GW_E_OK;
	}
	else
	{
		ret = GW_E_ERROR;
	}
	return ret;
}

gw_status gw_pri_queue_put (gw_uint32 queue_id, void *data, gw_uint32 size, gw_int32 timeout, gw_int32 priority)
{
	gw_status ret = GW_E_OK;
	if(CS_E_OSAL_OK == cs_pri_queue_put(queue_id, data, size, timeout, priority))
	{
		ret = GW_E_OK;
	}
	else
	{
		ret = GW_E_ERROR;
	}
	return ret;
}


//ʵ�ִ����̵߳Ľӿ�
gw_status gw_thread_create(gw_uint32 *thread_id,  const gw_int8 *thread_name,
                              const void *function_pointer, void *param , gw_uint32 stack_size,
                              gw_uint32 priority, gw_uint32 flags)
{
	gw_status ret = GW_E_OK;
	if(CS_E_OSAL_OK == cs_thread_create(thread_id, thread_name, function_pointer, param, stack_size, priority, flags))
	{
		ret = GW_E_OK;
	}
	else
	{
		ret = GW_E_ERROR;
	}
	return ret;
}
gw_status gw_thread_delete(gw_uint32 thread_id)
{
	gw_status ret = GW_E_OK;
	if(CS_E_OSAL_OK == cs_thread_delete(thread_id))
	{
		ret = GW_E_OK;
	}
	else
	{
		ret = GW_E_ERROR;
	}
	return ret;
}



gw_int32 gw_circle_timer_add(gw_uint32 timeout, void (*callback)(void *), void *data)
{
	return cs_circle_timer_add(timeout, callback, data);
}

//pty �ӿ�

/*create a new pty added to the existing link */
/*failed return -1                            */
/*max number of pty is 8*/
static int CreatePty(char *name)
{
	//��ڹ�����
	if(NULL == name)
	{
		gw_printf("arg check err!\n");
		return -1;
	}

	//����pty �ڵ�
	Pty *pPty = NULL;
	if(NULL == (pPty = (Pty *)malloc(sizeof(Pty))))
	{
		gw_printf("create pty device %s fail!\r\n", name);
		return -1;
	}
	else
	{
	}
	memset(pPty, 0, sizeof(Pty));


	//����pty �ڵ���Ϣ
		//ȷ�����ֳ���
	int name_length = 0;
	int length = 0;
	length = strlen(name);
	if(NAME_LEN - 1 < length)
	{
		name_length = NAME_LEN-1;
	}
	else
	{
		name_length = length;
	}
	strncpy(pPty->name,name,name_length);
	pPty->name[name_length]='\0';
	pPty->pty_num = pty_maxnum++;;
	pPty->next = NULL;
	
	//���ڵ��������
	if(NULL == head_pty)
	{
		head_pty = pPty;
	}
	else
	{
		if(pty_maxnum < PTY_NUM_MAX)
		{
			Pty *p = NULL;
			p = head_pty;
			while(NULL != p->next)
			{
				p = p->next;
			}

			p->next = pPty;
			
		}
		else
		{
			gw_printf("you cannot create pty more than 8\n");
			return -1;
		}
	}

	return pPty->pty_num;
}


static void DeletePty(int pty_num)
{
	//��ڹ�����
	if((pty_num < 0)||(pty_num > PTY_NUM_MAX))
	{
		gw_printf("arg check err!, in %s\n", __func__);
		return;
	}
	
	//���Ұ���������Ϣ�Ľڵ�
	Pty *p_free = NULL;

	//����Ϊ�գ�ֱ�ӷ���
	if(NULL == head_pty)
	{
		gw_printf("NULL == head_pty!, in %s\n", __func__);
		return;
	}
	
	if(pty_num == head_pty->pty_num)
	{
		//���ҽڵ�Ϊͷ�ڵ�ʱ
		p_free = head_pty;
		head_pty = head_pty->next;	
	}
	else
	{
		//���ҽڵ㲻Ϊͷ�ڵ�ʱ
		Pty *p = NULL;
		Pty *p_previous = NULL;
		while(NULL != p)
		{
			p_previous = p;
			p = p->next;
			if(pty_num == p->pty_num)
			{
				p_free = p;
			}
			else
			{
				continue;
			}
			p_previous->next = p->next;
		}
	}

	if(NULL != p_free)
	{
		//�ҵ����ͷŽڵ�
		free(p_free);
		p_free = NULL;
	}
	else
	{
		//δ�ҵ��ڵ����˳�
		gw_printf("pty_num not found!, in %s\n", __func__);
	}
	
	return;	
}


#if (SOCKET_PTY)

static int OpenMasterDev(int pty_num)
{
	//��ڹ�����
	if((pty_num < 0)||(pty_num > PTY_NUM_MAX))
	{
		gw_printf("arg check err!, in %s\n", __func__);
		return -1;
	}
	//���Ұ���������Ϣ�ڵ㣬û���ҵ�����-1
	Pty *pty_dev = head_pty;
	int found = 0;
	while(NULL != pty_dev)
	{
		if(pty_num == pty_dev->pty_num)
		{
			found = 1;
			break;
		}
		else
		{
			continue;
		}
	}
	if(1 != found)
	{
		gw_printf("not found!, in %s\n", __func__);
		return -1;
	}
	else
	{
	}
	
	//�ҵ��󴴽��׽��֣�ʧ�ܷ���-1
	if(-1 == (pty_dev->fd_master = socket(AF_INET, SOCK_DGRAM, PF_UNSPEC)))
	{
		gw_printf("%s socket fail!\r\n", __func__);
		return -1;
	}
	else
	{
	}
	
	//���׽��֣�ʧ�ܷ���-1
	memset(&(pty_dev->pty_connect.socket_pty.sa_master), 0, sizeof(pty_dev->pty_connect.socket_pty.sa_master));
	pty_dev->pty_connect.socket_pty.sa_master.sin_family = AF_INET;
	pty_dev->pty_connect.socket_pty.sa_master.sin_port = htons(OAM_PTY_PORT_BASE+2*pty_dev->pty_num);
	pty_dev->pty_connect.socket_pty.sa_master.sin_addr.s_addr = htonl(INADDR_ANY);
	if(-1 == bind(pty_dev->fd_master, (struct sockaddr *)&pty_dev->pty_connect.socket_pty.sa_master, sizeof(struct sockaddr)))
	{
		gw_printf("%s bind sock fail!\r\n", __func__);
		close(pty_dev->fd_master);
		return -1;
	}
	else
	{
	}

	return pty_dev->fd_master;
}

static void CloseMasterDev(int fd)
{
	//��ڹ�����
	if(fd < 0)
	{
		return;
	}
	
	//���ҽڵ�
	Pty *pty_dev = NULL;
	pty_dev = head_pty;
	while(NULL != pty_dev)
	{
		//�ҵ��ڵ���ر�
		if(fd == pty_dev->fd_master)
		{
			close(pty_dev->fd_master);
			pty_dev->fd_master = 0;
			break;
		}
		else
		{
			pty_dev = pty_dev->next;
		}
	}

	return;
}

static int OpenSlaveDev(int pty_num)
{
	//��ڹ�����
	if((pty_num < 0)||(pty_num > PTY_NUM_MAX))
	{
		gw_printf("arg check err!, in %s\n", __func__);
		return -1;
	}
	//���Ұ���������Ϣ�ڵ㣬û���ҵ�����-1
	Pty *pty_dev = head_pty;
	int found = 0;
	while(NULL != pty_dev)
	{
		if(pty_num == pty_dev->pty_num)
		{
			found = 1;
			break;
		}
		else
		{
			continue;
		}
	}
	if(1 != found)
	{
		gw_printf("not found!, in %s\n", __func__);
		return -1;
	}
	else
	{
	}
	
	//�ҵ��󴴽��׽��֣�ʧ�ܷ���-1
	if(-1 == (pty_dev->fd_slave = socket(AF_INET, SOCK_DGRAM, PF_UNSPEC)))
	{
		gw_printf("%s socket fail!\r\n", __func__);
		return -1;
	}
	else
	{
	}
	
	//���׽��֣�ʧ�ܷ���-1
	memset(&(pty_dev->pty_connect.socket_pty.sa_slave), 0, sizeof(pty_dev->pty_connect.socket_pty.sa_slave));
	pty_dev->pty_connect.socket_pty.sa_slave.sin_family = AF_INET;
	pty_dev->pty_connect.socket_pty.sa_slave.sin_port = htons(OAM_PTY_PORT_BASE+2*pty_dev->pty_num+1);
	pty_dev->pty_connect.socket_pty.sa_slave.sin_addr.s_addr = htonl(INADDR_ANY);
	if(-1 == bind(pty_dev->fd_slave, (struct sockaddr *)&pty_dev->pty_connect.socket_pty.sa_slave, sizeof(struct sockaddr)))
	{
		gw_printf("%s bind sock fail!\r\n", __func__);
		close(pty_dev->fd_slave);
		return -1;
	}
	else
	{
	}

	return pty_dev->fd_slave;
}
static void CloseSlaveDev(int fd)
{
	//��ڹ�����
	if(fd < 0)
	{
		gw_printf("arg check err!, in %s\n", __func__);
		return;
	}
	
	//���ҽڵ�
	Pty *pty_dev = NULL;
	pty_dev = head_pty;
	while(NULL != pty_dev)
	{
		//�ҵ��ڵ���ر�
		if(fd == pty_dev->fd_slave)
		{
			close(pty_dev->fd_slave);
			pty_dev->fd_slave = 0;
			break;
		}
		else
		{
			pty_dev = pty_dev->next;
		}
	}

	return;
}

/*return the number real writed  */
/*0 is cannot write, -1 is wrong  */
static int pty_write(int fd,char *str,int length)
{
	Pty *pty_dev = head_pty;
	int write_num = 0;
	int sel = 0;
	//��ڹ�����
	if(fd < 0)
	{
		return -1;
	}
	if(NULL == str)
	{
		return -1;
	}
	if(length < 0)
	{
		return -1;
	}

	//����pty �ڵ�
	while(NULL != pty_dev)
	{	
		if(pty_dev->fd_master == fd )
		{
			sel = SLAVE_WRITE;
			break;
		}
		if(pty_dev->fd_slave == fd)
		{
			sel = MASTER_WRITE;
			break;
		}
		pty_dev = pty_dev->next;
	}

	//δ�ҵ�
	if(NULL == pty_dev)
	{
		gw_printf("in %s, pty_dev not found\n", __func__);
		return 0;
	}
	else
	{
		//do nothing
	}

	//������Ϣ
	write_num=sendto(fd,str,length, 0, (struct sockaddr *)((sel == MASTER_WRITE)?&pty_dev->pty_connect.socket_pty.sa_master:&pty_dev->pty_connect.socket_pty.sa_slave), sizeof(struct sockaddr));
	return write_num;
}

static int pty_read(int fd,char *str,int length)
{
	Pty *pty_dev = head_pty;
	int sel = 0;
	int read_num = 0;
	socklen_t socklen = sizeof(struct sockaddr);
	//��ڹ�����
	if(fd < 0)
	{
		return -1;
	}
	if(NULL == str)
	{
		return -1;
	}
	if(length < 0)
	{
		return -1;
	}
	
	//����pty �ڵ�
	while(NULL != pty_dev)
	{
		if(pty_dev->fd_master == fd )
		{
			sel = SLAVE_READ;
			break;
		}
		if(pty_dev->fd_slave == fd)
		{
			sel = MASTER_READ;
			break;
		}
		pty_dev = pty_dev->next;
	}

	//δ�ҵ�
	if(NULL == pty_dev)
	{
		gw_printf("in %s, pty_dev not found\n", __func__);
		return 0;
	}
	else
	{
		//do nothing
	}

	//������Ϣ
	read_num = recvfrom(fd,str,length, MSG_DONTWAIT, (struct sockaddr*)((sel==MASTER_READ)?&pty_dev->pty_connect.socket_pty.sa_master:&pty_dev->pty_connect.socket_pty.sa_slave), &socklen);
	return read_num;
}

#endif

#if (PIPE_PTY)
static int OpenMasterDev(int pty_num)
{
	//��ڹ�����
	if((pty_num < 0)||(pty_num > PTY_NUM_MAX))
	{
		gw_printf("arg check err!, in %s\n", __func__);
		return -1;
	}
	//���Ұ���������Ϣ�ڵ㣬û���ҵ�����-1
	Pty *pty_dev = head_pty;
	int found = 0;
	while(NULL != pty_dev)
	{
		if(pty_num == pty_dev->pty_num)
		{
			found = 1;
			break;
		}
		else
		{
			continue;
		}
	}
	if(1 != found)
	{
		gw_printf("not found!, in %s\n", __func__);
		return -1;
	}
	else
	{
	}

	pty_dev->fd_master = OAM_PTY_FD_BASE + 2 * pty_num;

	//����pipe
	int pipe_two[2];
	//ʧ�ܷ���-1
	if(pipe(pipe_two) < 0)
	{
		return -1;
	}
	//����pipe ��Ϣ
	pty_dev->pty_connect.pipe_pty.master_read = pipe_two[0];
	pty_dev->pty_connect.pipe_pty.master_write = pipe_two[1];
	
	return pty_dev->fd_master;
}


static void CloseMasterDev(int fd)
{
	//��ڹ�����
	if(fd < 0)
	{
		return;
	}
	
	//���ҽڵ�
	Pty *pty_dev = NULL;
	pty_dev = head_pty;
	while(NULL != pty_dev)
	{
		//�ҵ��ڵ���ر�
		if(fd == pty_dev->fd_master)
		{
			close(pty_dev->pty_connect.pipe_pty.master_read);
			close(pty_dev->pty_connect.pipe_pty.master_write);
			pty_dev->fd_master = 0;
			break;
		}
		else
		{
			pty_dev = pty_dev->next;
		}
	}

	return;
}


static int OpenSlaveDev(int pty_num)
{
	//��ڹ�����
	if((pty_num < 0)||(pty_num > PTY_NUM_MAX))
	{
		gw_printf("arg check err!, in %s\n", __func__);
		return -1;
	}
	//���Ұ���������Ϣ�ڵ㣬û���ҵ�����-1
	Pty *pty_dev = head_pty;
	int found = 0;
	while(NULL != pty_dev)
	{
		if(pty_num == pty_dev->pty_num)
		{
			found = 1;
			break;
		}
		else
		{
			continue;
		}
	}
	if(1 != found)
	{
		gw_printf("not found!, in %s\n", __func__);
		return -1;
	}
	else
	{
	}

	
	//����pipe
	int pipe_two[2];
	//ʧ�ܷ���-1
	if(pipe(pipe_two) < 0)
	{
		return -1;
	}
	//����pipe ��Ϣ
	pty_dev->pty_connect.pipe_pty.slave_read = pipe_two[0];
	pty_dev->pty_connect.pipe_pty.slave_write = pipe_two[1];
	pty_dev->fd_slave = OAM_PTY_FD_BASE + 2 * pty_num + 1;
	
	return pty_dev->fd_slave;
}

static void CloseSlaveDev(int fd)
{
	//��ڹ�����
	if(fd < 0)
	{
		gw_printf("arg check err!, in %s\n", __func__);
		return;
	}
	
	//���ҽڵ�
	Pty *pty_dev = NULL;
	pty_dev = head_pty;
	while(NULL != pty_dev)
	{
		//�ҵ��ڵ���ر�
		if(fd == pty_dev->fd_slave)
		{
			close(pty_dev->pty_connect.pipe_pty.slave_read);
			close(pty_dev->pty_connect.pipe_pty.slave_write);
			pty_dev->fd_slave = 0;
			break;
		}
		else
		{
			pty_dev = pty_dev->next;
		}
	}

	return;
}


/*return the number real writed  */
/*0 is cannot write, -1 is wrong  */
static int pty_write(int fd,char *str,int length)
{
	Pty *pty_dev = head_pty;
	int write_num = 0;
	int fd_write = 0;
	//��ڹ�����
	if(fd < 0)
	{
		return -1;
	}
	if(NULL == str)
	{
		return -1;
	}
	if(length < 0)
	{
		return -1;
	}

	//����pty �ڵ�
	while(NULL != pty_dev)
	{	
		if(pty_dev->fd_master == fd )
		{
			fd_write = pty_dev->pty_connect.pipe_pty.slave_write;
			break;
		}
		if(pty_dev->fd_slave == fd)
		{
			fd_write = pty_dev->pty_connect.pipe_pty.master_write;
			break;
		}
		pty_dev = pty_dev->next;
	}

	//δ�ҵ�
	if(NULL == pty_dev)
	{
		gw_printf("in %s, pty_dev not found\n", __func__);
		return 0;
	}
	else
	{
		//do nothing
	}

	//������Ϣ
	write_num=write(fd_write,str,length);
	return write_num;
}


static int pty_read(int fd,char *str,int length)
{
	Pty *pty_dev = head_pty;
	int read_num = 0;
	int fd_read = 0;
	//��ڹ�����
	if(fd < 0)
	{
		return -1;
	}
	if(NULL == str)
	{
		return -1;
	}
	if(length < 0)
	{
		return -1;
	}
	
	//����pty �ڵ�
	while(NULL != pty_dev)
	{
		if(pty_dev->fd_master == fd)
		{
			fd_read = pty_dev->pty_connect.pipe_pty.slave_read;
			break;
		}
		if(pty_dev->fd_slave == fd)
		{
			fd_read = pty_dev->pty_connect.pipe_pty.master_read;
			break;
		}
		pty_dev = pty_dev->next;
	}

	//δ�ҵ�
	if(NULL == pty_dev)
	{
		gw_printf("in %s, pty_dev not found\n", __func__);
		return 0;
	}
	else
	{
		//do nothing
	}

	//������Ϣ
	read_num = read(fd_read, str, length);
	return read_num;
}

#endif

#if (QUEUE_PTY)
typedef struct
{
	int len;
	char *data;
}pty_infor_t;
static int OpenMasterDev(int pty_num)
{
	//��ڹ�����
	if((pty_num < 0)||(pty_num > PTY_NUM_MAX))
	{
		gw_printf("arg check err!, in %s\n", __func__);
		return -1;
	}
	//���Ұ���������Ϣ�ڵ㣬û���ҵ�����-1
	Pty *pty_dev = head_pty;
	int found = 0;
	while(NULL != pty_dev)
	{
		if(pty_num == pty_dev->pty_num)
		{
			found = 1;
			break;
		}
		else
		{
			continue;
		}
	}
	if(1 != found)
	{
		gw_printf("not found!, in %s\n", __func__);
		return -1;
	}
	else
	{
	}

	//��������
	gw_uint32 queue_id = 0;
	gw_int8 queue_name[] = "queue_pty";
	gw_uint32 queue_depth = 200;
	gw_uint32 data_size = 12;
	gw_int32 pri_num = 3;
	if(GW_E_OK != gw_pri_queue_create(&queue_id, queue_name, queue_depth, data_size, pri_num))
	{
		gw_printf("gw_pri_queue_create :%s failed\n", queue_name);
		return -1;
	}
	//���������Ϣ
	pty_dev->pty_connect.queue_pty.master_queue_id = queue_id;
	pty_dev->fd_master = OAM_PTY_FD_BASE + 2 * pty_num;
	
	return pty_dev->fd_master;
}



static void CloseMasterDev(int fd)
{
	//��ڹ�����
	if(fd < 0)
	{
		return;
	}
	
	//���ҽڵ�
	Pty *pty_dev = NULL;
	pty_dev = head_pty;
	while(NULL != pty_dev)
	{
		//�ҵ��ڵ���ر�
		if(fd == pty_dev->fd_master)
		{
			gw_queue_delete(pty_dev->pty_connect.queue_pty.master_queue_id);
			pty_dev->pty_connect.queue_pty.master_queue_id = 0;
			pty_dev->fd_master = 0;
			break;
		}
		else
		{
			pty_dev = pty_dev->next;
		}
	}

	return;
}


static int OpenSlaveDev(int pty_num)
{
	//��ڹ�����
	if((pty_num < 0)||(pty_num > PTY_NUM_MAX))
	{
		gw_printf("arg check err!, in %s\n", __func__);
		return -1;
	}
	//���Ұ���������Ϣ�ڵ㣬û���ҵ�����-1
	Pty *pty_dev = head_pty;
	int found = 0;
	while(NULL != pty_dev)
	{
		if(pty_num == pty_dev->pty_num)
		{
			found = 1;
			break;
		}
		else
		{
			continue;
		}
	}
	if(1 != found)
	{
		gw_printf("not found!, in %s\n", __func__);
		return -1;
	}
	else
	{
	}

	//��������
	gw_uint32 queue_id = 0;
	gw_int8 queue_name[] = "queue_pty";
	gw_uint32 queue_depth = 200;
	gw_uint32 data_size = 12;
	gw_int32 pri_num = 3;
	if(GW_E_OK != gw_pri_queue_create(&queue_id, queue_name, queue_depth, data_size, pri_num))
	{
		gw_printf("gw_pri_queue_create :%s failed\n", queue_name);
		return -1;
	}

	//���������Ϣ
	pty_dev->pty_connect.queue_pty.slave_queue_id = queue_id;
	pty_dev->fd_slave = OAM_PTY_FD_BASE + 2 * pty_num + 1;
	
	return pty_dev->fd_slave;
}

static void CloseSlaveDev(int fd)
{
	//��ڹ�����
	if(fd < 0)
	{
		return;
	}
	
	//���ҽڵ�
	Pty *pty_dev = NULL;
	pty_dev = head_pty;
	while(NULL != pty_dev)
	{
		//�ҵ��ڵ���ر�
		if(fd == pty_dev->fd_master)
		{
			gw_queue_delete(pty_dev->pty_connect.queue_pty.slave_queue_id);
			pty_dev->pty_connect.queue_pty.slave_queue_id = 0;
			pty_dev->fd_slave = 0;
			break;
		}
		else
		{
			pty_dev = pty_dev->next;
		}
	}

	return;
}



/*return the number real writed  */
/*0 is cannot write, -1 is wrong  */
static int pty_write(int fd,char *str,int length)
{
	Pty *pty_dev = head_pty;
	int write_num = 0;
	gw_uint32 queue_id = 0;
	//��ڹ�����
	if(fd < 0)
	{
		return -1;
	}
	if(NULL == str)
	{
		return -1;
	}
	if(length < 0)
	{
		return -1;
	}

	//����pty �ڵ�
	while(NULL != pty_dev)
	{	
		if(pty_dev->fd_master == fd )
		{
			queue_id = pty_dev->pty_connect.queue_pty.master_queue_id;
			break;
		}
		if(pty_dev->fd_slave == fd)
		{
			queue_id = pty_dev->pty_connect.queue_pty.slave_queue_id;
			break;
		}
		pty_dev = pty_dev->next;
	}

	//δ�ҵ�
	if(NULL == pty_dev)
	{
		gw_printf("in %s, pty_dev not found\n", __func__);
		return 0;
	}
	else
	{
		//do nothing
	}

	//��֯��Ϣ
	pty_infor_t pty_infor;
	char *buf = NULL;
	if(NULL == (buf = (char *)malloc(length)))
	{
		gw_printf("in %s, line :%d, malloc failed\n", __func__, __LINE__);
		return 0;
	}
	else
	{
		memcpy(buf, str, length);
		pty_infor.data = buf;
		pty_infor.len = length;
	}
	
	//������Ϣ
	if(GW_E_OK != gw_pri_queue_put(queue_id, &pty_infor, sizeof(pty_infor), GW_OSAL_WAIT_FOREVER, 0))
	{
		gw_printf("in %s, line :%d, gw_pri_queue_put failed\n", __func__, __LINE__);
		return 0;
	}
	
	write_num = length;
	return write_num;
}



static int pty_read(int fd,char *str,int length)
{
	Pty *pty_dev = head_pty;
	int read_num = 0;
	gw_uint32 queue_id = 0;
	//��ڹ�����
	if(fd < 0)
	{
		gw_printf("in %s, line :%d, arg check err!\n", __func__, __LINE__);
		return -1;
	}
	if(NULL == str)
	{
		gw_printf("in %s, line :%d, arg check err!\n", __func__, __LINE__);
		return -1;
	}
	if(length < 0)
	{
		gw_printf("in %s, line :%d, arg check err!\n", __func__, __LINE__);
		return -1;
	}
	
	//����pty �ڵ�
	while(NULL != pty_dev)
	{
		if(pty_dev->fd_master == fd)
		{
			queue_id = pty_dev->pty_connect.queue_pty.slave_queue_id;
			break;
		}
		if(pty_dev->fd_slave == fd)
		{
			queue_id = pty_dev->pty_connect.queue_pty.master_queue_id;
			break;
		}
		pty_dev = pty_dev->next;
	}

	//δ�ҵ�
	if(NULL == pty_dev)
	{
		gw_printf("in %s, pty_dev not found\n", __func__);
		return 0;
	}
	else
	{
		//do nothing
	}

	//������Ϣ
	gw_uint32 size_copied = 0;
	pty_infor_t pty_infor;
	if(GW_E_OK != gw_pri_queue_get(queue_id, &pty_infor, sizeof(pty_infor), &size_copied, GW_OSAL_WAIT_FOREVER))
	{
		gw_printf("in %s, line :%d, gw_pri_queue_put failed\n", __func__, __LINE__);
		return 0;
	}
	//������Ϣ
	if(pty_infor.len < length)
	{
		read_num = pty_infor.len;
	}
	else
	{
		read_num = length;
	}
	memcpy(str, pty_infor.data, read_num);
	free(pty_infor.data);
	return read_num;
}


#endif

#if (BUF_PTY_SUPPORT)
#define MASTER_BUF_SIZE		4096
#define SLAVE_BUF_SIZE		1024

int pty_buf_init(pty_buf_t *pty_buf, int buf_size)
{
	pty_buf->data_head = 0;
	pty_buf->data_tail = 0;
	pty_buf->data_len = 0;
	pty_buf->buf = (char *)malloc(buf_size);
	if(NULL == pty_buf->buf)
	{
		gw_printf("in %s, line :%d, malloc failed\n", __func__, __LINE__);
	}
	pty_buf->size = buf_size;
	memset(pty_buf->buf, 0, sizeof(pty_buf->buf));
	gw_mutex_init(&(pty_buf->mutex));
	gw_mutex_init(&(pty_buf->mutex_read));
	return 0;
}

int pty_buf_destroy(pty_buf_t *pty_buf)
{
	pty_buf->data_head = 0;
	pty_buf->data_tail = DATA_SIZE;
	pty_buf->data_len = 0;
	free(pty_buf->buf);
	pty_buf->buf = NULL;
	gw_mutex_destroy(&(pty_buf->mutex));
	return 0;
}


int pty_buf_write(pty_buf_t *pty_buf, char *str,int length)
{
	//��ڹ�����
	if(NULL == pty_buf)
	{
		return -1;
	}
	
	if(NULL == str)
	{
		return -1;
	}

	if(length <= 0)
	{
		return -1;
	}

	//�ж��Ƿ����д��
	if(pty_buf->data_len + length > pty_buf->size - 1)
	{
		gw_printf("in %s, buf full\n", __func__);
		return -1;
	}

	int write_num = 0;
	int str_offset = 0;
	int write_pos = 0;
	int size = 0;
	char *buf = NULL;
	
	//������Դ
	gw_mutex_lock(&(pty_buf->mutex));

	size = pty_buf->size;
	buf  = pty_buf->buf;
	str_offset = 0;
	write_pos = pty_buf->data_tail;
	while(write_num < length)
	{
		buf[write_pos] = str[str_offset];

		str_offset++;
		write_pos = (write_pos + 1) % size;
			
		write_num++;
	}

	pty_buf->data_tail = write_pos;
	
	pty_buf->data_len = (pty_buf->data_tail - pty_buf->data_head + pty_buf->size) % pty_buf->size;
	
	//��д�룬���Զ�
	gw_mutex_unlock(&(pty_buf->mutex_read));
	
	//�ͷ���Դ
	gw_mutex_unlock(&(pty_buf->mutex));

	#if 0
	cs_printf("in %s, write_num :%d\n", __func__, write_num);
	#endif
	//����д�����ݵĳ���
	return write_num;
}


int pty_buf_read(pty_buf_t *pty_buf, char *str,int length)
{
	//��ڹ�����
	if(NULL == pty_buf)
	{
		return 0;
	}
	
	if(NULL == str)
	{
		return 0;
	}

	if(length < 0)
	{
		return 0;
	}

	int read_num = 0;
	int str_offset = 0;
	int read_pos = 0;
	int size = 0;
	int data_len = 0;
	char *buf = NULL;

	//û�пɶ���Դ������
	if(pty_buf->data_len <= 0)
	{
		gw_mutex_lock(&(pty_buf->mutex_read));
	}
	
	//������Դ
	gw_mutex_lock(&(pty_buf->mutex));
	
	data_len = pty_buf->data_len;
	size = pty_buf->size;
	buf  = pty_buf->buf;
	str_offset = 0;
	read_pos = pty_buf->data_head;
	while((read_num < length)&&(data_len > 0))
	{
		str[str_offset] = buf[read_pos];

		str_offset++;
		read_pos = (read_pos + 1) % size;
			
		read_num++;
		data_len--;
	}

	pty_buf->data_head = read_pos;
	
	pty_buf->data_len = (pty_buf->data_tail - pty_buf->data_head + pty_buf->size) % pty_buf->size;
	
	//�ͷ���Դ
	gw_mutex_unlock(&(pty_buf->mutex));

	//���ض������ݵĳ���
	#if 0
	cs_printf("in %s, read_num :%d\n", __func__, read_num);
	#endif
	return read_num;
}

static int OpenMasterDev(int pty_num)
{
	//��ڹ�����
	if((pty_num < 0)||(pty_num > PTY_NUM_MAX))
	{
		gw_printf("arg check err!, in %s\n", __func__);
		return -1;
	}
	//���Ұ���������Ϣ�ڵ㣬û���ҵ�����-1
	Pty *pty_dev = head_pty;
	int found = 0;
	while(NULL != pty_dev)
	{
		if(pty_num == pty_dev->pty_num)
		{
			found = 1;
			break;
		}
		else
		{
			continue;
		}
	}
	if(1 != found)
	{
		gw_printf("not found!, in %s\n", __func__);
		return -1;
	}
	else
	{
	}

	pty_buf_init(&(pty_dev->pty_connect.buf_pty.master_buf), MASTER_BUF_SIZE);
	pty_dev->fd_master = OAM_PTY_FD_BASE + 2 * pty_num;
	return pty_dev->fd_master;
}
static void CloseMasterDev(int fd)
{
	//��ڹ�����
	if(fd < 0)
	{
		return;
	}
	
	//���ҽڵ�
	Pty *pty_dev = NULL;
	pty_dev = head_pty;
	while(NULL != pty_dev)
	{
		//�ҵ��ڵ���ر�
		if(fd == pty_dev->fd_master)
		{
			pty_buf_destroy(&(pty_dev->pty_connect.buf_pty.master_buf));
			pty_dev->fd_master = 0;
			break;
		}
		else
		{
			pty_dev = pty_dev->next;
		}
	}
}
static int OpenSlaveDev(int pty_num)
{
	//��ڹ�����
	if((pty_num < 0)||(pty_num > PTY_NUM_MAX))
	{
		gw_printf("arg check err!, in %s\n", __func__);
		return -1;
	}
	//���Ұ���������Ϣ�ڵ㣬û���ҵ�����-1
	Pty *pty_dev = head_pty;
	int found = 0;
	while(NULL != pty_dev)
	{
		if(pty_num == pty_dev->pty_num)
		{
			found = 1;
			break;
		}
		else
		{
			continue;
		}
	}
	if(1 != found)
	{
		gw_printf("not found!, in %s\n", __func__);
		return -1;
	}
	else
	{
	}

	pty_buf_init(&(pty_dev->pty_connect.buf_pty.slave_buf), SLAVE_BUF_SIZE);
	
	pty_dev->fd_slave = OAM_PTY_FD_BASE + 2 * pty_num + 1;
	return pty_dev->fd_slave;

}
static void CloseSlaveDev(int fd)
{
	//��ڹ�����
	if(fd < 0)
	{
		return;
	}
	
	//���ҽڵ�
	Pty *pty_dev = NULL;
	pty_dev = head_pty;
	while(NULL != pty_dev)
	{
		//�ҵ��ڵ���ر�
		if(fd == pty_dev->fd_slave)
		{
			pty_buf_destroy(&(pty_dev->pty_connect.buf_pty.slave_buf));
			pty_dev->fd_slave = 0;
			break;
		}
		else
		{
			pty_dev = pty_dev->next;
		}
	}
}
/*return the number real writed  */
/*0 is cannot write, -1 is wrong  */
static int pty_write(int fd,char *str,int length)
{
	Pty *pty_dev = head_pty;
	int write_num = 0;
	pty_buf_t *pPty_buf;
	//��ڹ�����
	if(fd < 0)
	{
		return -1;
	}
	if(NULL == str)
	{
		return -1;
	}
	if(length < 0)
	{
		return -1;
	}

	//����pty �ڵ�
	while(NULL != pty_dev)
	{	
		if(pty_dev->fd_master == fd )
		{
			pPty_buf = &(pty_dev->pty_connect.buf_pty.slave_buf);
			break;
		}
		if(pty_dev->fd_slave == fd)
		{
			pPty_buf = &(pty_dev->pty_connect.buf_pty.master_buf);
			break;
		}
		pty_dev = pty_dev->next;
	}

	//δ�ҵ�
	if(NULL == pty_dev)
	{
		gw_printf("in %s, pty_dev not found\n", __func__);
		return 0;
	}
	else
	{
		//do nothing
	}

	#if 0
	write_num = 0;
	do
	{
		write_num = write_num + pty_buf_write(pPty_buf, str, length);
	}while(write_num != length);
	#else
	write_num = pty_buf_write(pPty_buf, str, length);
	#endif

	return length;
}
static int pty_read(int fd,char *str,int length)
{
	Pty *pty_dev = head_pty;
	int read_num = 0;
	pty_buf_t *pPty_buf;
	//��ڹ�����
	if(fd < 0)
	{
		return -1;
	}
	if(NULL == str)
	{
		return -1;
	}
	if(length < 0)
	{
		return -1;
	}

	//����pty �ڵ�
	while(NULL != pty_dev)
	{	
		if(pty_dev->fd_master == fd )
		{
			pPty_buf = &(pty_dev->pty_connect.buf_pty.master_buf);
			break;
		}
		if(pty_dev->fd_slave == fd)
		{
			pPty_buf = &(pty_dev->pty_connect.buf_pty.slave_buf);
			break;
		}
		pty_dev = pty_dev->next;
	}

	//δ�ҵ�
	if(NULL == pty_dev)
	{
		gw_printf("in %s, pty_dev not found\n", __func__);
		return -1;
	}
	else
	{
		//do nothing
	}

	read_num = pty_buf_read(pPty_buf, str, length);
	return read_num;

}

#endif


static void gw_oam_pty_main_thread_entry(gw_uint32 *para)
{
	gw_uint32 aumsg[4], len = 0;
	while(1)
	{
		//��ȡ��Ϣ
		if(GW_E_OK == gw_pri_queue_get(g_oam_pty_queue_id, aumsg, sizeof(aumsg), &len, GW_OSAL_WAIT_FOREVER))
		{
			if(len > 0)
			{
				//������Ϣ
				switch(aumsg[2])
				{
					//oam pty ���Ĵ���
					case PTY_PACKET:
						OamPtyPacketProcess((GWTT_OAM_SESSION_INFO*)aumsg[1], (gw_uint8 *)aumsg[3], aumsg[0]);
						break;
					//�Ͽ����Ӵ���
					case PTY_NOTI_MSG:
						OamPtyNotiMsgProcess(aumsg[1], aumsg[3]);
						break;
					//��ʱ��Ϣ����
					case PTY_TIMER_MSG:
						OamPtyTimerMsgProcess();
						break;
					case PTY_ONU_LOSE:
						break;
				}
			}
			else
			{
				
			}
		}
		else
		{
			continue;
		}
	}
}


static void gw_oam_pty_sub_thread_entry(gw_uint32 *para)
{
	int length = 0;
	gw_uint8 *buf = malloc(PTY_BUF_SIZE);
	gw_uint8 *buf_read = NULL;

	if(buf == NULL)
	{
		gw_printf("in %s, malloc failed\n", __func__);
		return;
	}
	
	while(1)
	{
		memset(buf, 0, PTY_BUF_SIZE);
		buf[0] = CON_CTL_CODE_DATA;
		buf_read = &buf[1];
		length = read_from_console(buf_read, PTY_BUF_SIZE -2);	//buf �ĵ�һλ���ڴ�ű������ͣ����һλ���ڴ�Ž�����'\0'

		if(length > 0)
		{
			buf_read[length] = '\0';
			gw_comm_onu_msg_send(CLI_PTY_TRANSMIT, gmCliPtyCtrl.lSerNo++, buf, length+2, gmCliPtyCtrl.bSessionId);
		}
		else
		{
			//do nothing
		}
		
		//��ʱ
		gw_thread_delay(CONSOLE_READ_INTERVAL_TIME);
	}
}

static void OamPtyShellTimerNoti(void *para)
{
	unsigned long   lMsg[4] = {0};

	lMsg[0] = 0;
    lMsg[1] = 0;
    lMsg[2] = PTY_TIMER_MSG;
    lMsg[3] = 0;

	if(GW_E_OK != gw_pri_queue_put(g_oam_pty_queue_id, lMsg, sizeof(lMsg), GW_OSAL_WAIT_FOREVER, 0))
	{
		gw_printf("OamPtyShellTimerNoti send msg fail!\r\n");
	}
}



static gw_status oam_pty_pkt_conreq_proc(GWTT_OAM_SESSION_INFO *pSeInf, char *pPayLoad, long lPayLen)
{
	gw_status ret = GW_E_OK;
	//��ڹ�����
	if(NULL == pSeInf)
	{
		return GW_E_ERROR;
	}
	if(NULL == pPayLoad)
	{
		return GW_E_ERROR;
	}
	int connect_status = 0;
	char bResBuf[8] = {0};

	#if 0
	cs_printf("in %s, line :%d\n", __func__, __LINE__);
	#endif
	//��ȡ����״̬
	connect_status = gmCliPtyCtrl.lConnect;
	if(0 != connect_status)
	{
		//�����Ӵ���
		

		//������֤
		if(memcmp(gmCliPtyCtrl.bSessionId, pSeInf->SessionID, sizeof(gmCliPtyCtrl.bSessionId)) == 0)
	    {
	    	//��֤ͨ�������ܣ���֯Ӧ����
         	bResBuf[0] = CON_CTL_CODE_CONRES;
			bResBuf[1] = PTY_CONNECT_REQ_RESP_ACCEPT;
	    }
		else
	    {
	    	//��֤δͨ�����ܾ�����֯Ӧ����
       	    bResBuf[0] = CON_CTL_CODE_CONRES;
			bResBuf[1] = PTY_CONNECT_REQ_RESP_REJECT;
	    }
	}
	else
	{
		//δ���Ӵ���

		//��ȡ����id
		gmCliPtyCtrl.lFd = pty_master_fd_get();

		
		if(gmCliPtyCtrl.lFd > 0)
		{
			//���ӳɹ�
			//����������Ϣ
			gmCliPtyCtrl.lConnect = TRUE;
			gmCliPtyCtrl.lSerNo   = 0;
			gmCliPtyCtrl.lTimeOut = 0;
			gmCliPtyCtrl.lSendKpl = 0;
			memcpy(gmCliPtyCtrl.bSessionId, pSeInf->SessionID, sizeof(gmCliPtyCtrl.bSessionId));

			//������������߳�
			start_oamPtyCliThread();
			
			//��֯Ӧ����:����
			bResBuf[0] = CON_CTL_CODE_CONRES;
			bResBuf[1] = PTY_CONNECT_REQ_RESP_ACCEPT;
		}
		else
		{
			//����ʧ��
			//����������Ϣ
			gmCliPtyCtrl.lConnect = FALSE;
			gmCliPtyCtrl.lSerNo   = 0;
			gmCliPtyCtrl.lTimeOut = 0;
			gmCliPtyCtrl.lSendKpl = 0;
			memset(gmCliPtyCtrl.bSessionId, 0, sizeof(gmCliPtyCtrl.bSessionId));

			//��֯Ӧ����:�ܾ�
			bResBuf[0] = CON_CTL_CODE_CONRES;
			bResBuf[1] = PTY_CONNECT_REQ_RESP_REJECT;
		}	
	}
	
	//���ͱ���	
	gw_comm_onu_msg_send(CLI_PTY_TRANSMIT, gmCliPtyCtrl.lSerNo++, bResBuf, CON_CTL_CODE_CONRES_LEN, pSeInf->SessionID);	
		
	return ret;
}

static gw_status oam_pty_pkt_conres_proc(GWTT_OAM_SESSION_INFO *pSeInf, char *pPayLoad, long lPayLen)
{
	gw_status ret = GW_E_OK;
	//��������
	return ret;
}

static gw_status oam_pty_pkt_freereq_proc(GWTT_OAM_SESSION_INFO *pSeInf, char *pPayLoad, long lPayLen)
{
	gw_status ret = GW_E_OK;
	//��ڹ�����
	if(NULL == pSeInf)
	{
		return GW_E_ERROR;
	}
	if(NULL == pPayLoad)
	{
		return GW_E_ERROR;
	}
	int connect_status;
	connect_status = gmCliPtyCtrl.lConnect;
	if(0 != connect_status)
	{
		//�Ѵ�������
		if(0 == memcmp(gmCliPtyCtrl.bSessionId, pSeInf->SessionID, 8))
		{
			//��֤ͨ��
			char  bResBuf[8] = {0};
            char  bQuitCmd[8] = {0};
			int i = 0;
			//֪ͨolt �ѽ��ܶϿ���������
			bResBuf[0] = CON_CTL_CODE_FREERES;
			bResBuf[1] = PTY_CONNECT_REQ_RESP_ACCEPT;

			gw_comm_onu_msg_send(CLI_PTY_TRANSMIT, gmCliPtyCtrl.lSerNo++, bResBuf, 2, pSeInf->SessionID);

			//��ʱ
			gw_thread_delay(100);

			//֪ͨ�������ģ�飬Ҫ�Ͽ�
			strcpy(bQuitCmd, "quit\r");
			for(i=0; i<strlen(bQuitCmd); i++)
			{
#if 0
				write_to_console(bQuitCmd+i, 1);
#else
				//delete cli_loop thread first,redirect cli->channel to serial zhangjj 2015-1-13
				//�ر�oam pty ģʽ
				oam_pty_shell_close();

#endif
			}
	            
		}
		else
		{
			//��֤δͨ��
		}
	}
	else
	{
		//δ��������
	}
	return ret;
}

static gw_status oam_pty_pkt_freeres_proc(GWTT_OAM_SESSION_INFO *pSeInf, char *pPayLoad, long lPayLen)
{
	gw_status ret = GW_E_OK;
	//��������
	return ret;
}

static gw_status oam_pty_pkt_kpl_proc(GWTT_OAM_SESSION_INFO *pSeInf, char *pPayLoad, long lPayLen)
{
	gw_status ret = GW_E_OK;
	//��ڹ�����
	if(NULL == pSeInf)
	{
		return GW_E_ERROR;
	}
	if(NULL == pPayLoad)
	{
		return GW_E_ERROR;
	}

	//����״̬�ж��Լ�������֤
	if((gmCliPtyCtrl.lConnect) && (memcmp(gmCliPtyCtrl.bSessionId, pSeInf->SessionID, 8) == 0))
	{	
		//�������¼�ʱ
		gmCliPtyCtrl.lTimeOut = 0;
		ret = GW_E_OK;
	}
	else
	{
		ret = GW_E_ERROR;
	}
	
	return ret;
}

static gw_status oam_pty_pkt_data_proc(GWTT_OAM_SESSION_INFO *pSeInf, char *pPayLoad, long lPayLen)
{
	gw_status ret = GW_E_OK;
	//��ڹ�����
	if(NULL == pSeInf)
	{
		return GW_E_ERROR;
	}
	if(NULL == pPayLoad)
	{
		return GW_E_ERROR;
	}

	//����״̬�ж��Լ�������֤
	if((0 != gmCliPtyCtrl.lConnect) &&
		(0 == memcmp(gmCliPtyCtrl.bSessionId, pSeInf->SessionID, 8)))
	{
		//��ȡ����(��ʼ�ͳ���)
		char *cmd_str = NULL;
		int cmd_len = 0;
		
		cmd_str = &pPayLoad[1];	//pPayLoad �ĵ�0 λ�Ǳ�������
		cmd_len = lPayLen -1;
		
		//����Ϸ��Ժ����������ģ�����Ӵ����Լ��
		if((0 < lPayLen) && (0 < gmCliPtyCtrl.lFd) )
		{
			//����Ϸ��Ժ����������ģ�����Ӵ����Լ��ͨ��
			//��������������ģ��
			if(0xff != cmd_str[0])	//forbidden telnet option string 
			{
				#if 0
				int i = 0;
				for(i=0; i<cmd_len; i++)
				{
					//һ�η���һ���ַ�
					//pty_write(gmCliPtyCtrl.lFd, cmd_str+i, 1);
					write_to_console(cmd_str+i, 1);
				}
				#else
				write_to_console(cmd_str, cmd_len);
				#endif
			}
			else
			{
				//��telnet ���������������
			}
		}
		else
		{
			//����Ϸ��Ժ����������ģ�����Ӵ����Լ�鲻ͨ������������
		}
	}
	else
	{
		//�������������ģ���������ӣ���������			
	}
	
	return ret;
}


static void OamPtyPacketProcess(GWTT_OAM_SESSION_INFO *pSeInf, char *pPayLoad, long lPayLen)
{
	//��ڹ�����
	if(NULL == pSeInf)
	{
		return;
	}
	if(NULL == pPayLoad)
	{
		return;
	}
	
	//��ȡ��������
	CONSOLE_CTRL_CODE lCtlCode;
	lCtlCode = pPayLoad[0];
	
	//���ݱ������ͽ��д���
	switch(lCtlCode)
	{
		//�������Ӵ���
		case CON_CTL_CODE_CONREQ:
			oam_pty_pkt_conreq_proc(pSeInf, pPayLoad, lPayLen);
			break;
			
		//Ӧ�����Ӵ���
		case CON_CTL_CODE_CONRES:
			oam_pty_pkt_conres_proc(pSeInf, pPayLoad, lPayLen);
			break;

		//����Ͽ����Ӵ���
		case CON_CTL_CODE_FREEREQ:
			oam_pty_pkt_freereq_proc(pSeInf, pPayLoad, lPayLen);
			break;

		//Ӧ��Ͽ����Ӵ���
		case CON_CTL_CODE_FREERES:
			oam_pty_pkt_freeres_proc(pSeInf, pPayLoad, lPayLen);
			break;

		//�������Ӵ���
		case CON_CTL_CODE_KPL:
			oam_pty_pkt_kpl_proc(pSeInf, pPayLoad, lPayLen);
			break;

		//���ݴ���
		case CON_CTL_CODE_DATA:
			oam_pty_pkt_data_proc(pSeInf, pPayLoad, lPayLen);
			break;
		default:
			break;
	}
		

	//��������	
	if(NULL != pSeInf)
	{
		free(pSeInf);
	}
	
    if(NULL != pPayLoad)
	{
		free(pPayLoad);
	}
	return;
}

static void OamPtyNotiMsgProcess(long int flag, long int fd)
{
	//��ڹ�����
	if(PTY_SHELL_CLOSE_FLAG != flag)
	{
		return;
	}
	if(fd != gmCliPtyCtrl.lFd)
	{
		return;
	}

	//���Ӵ����Լ��
	if(0 != gmCliPtyCtrl.lConnect)
	{
		//�����Ѵ���
		//��֯Ӧ��Ͽ����ӱ���
		char bRes[8];
		bRes[0] = CON_CTL_CODE_FREEREQ;
		
		//���ͱ���
		gw_comm_onu_msg_send(CLI_PTY_TRANSMIT, gmCliPtyCtrl.lSerNo++, bRes, 1, gmCliPtyCtrl.bSessionId);
		gw_thread_delay(100);
		gw_comm_onu_msg_send(CLI_PTY_TRANSMIT, gmCliPtyCtrl.lSerNo++, bRes, 1, gmCliPtyCtrl.bSessionId);

		//��ʼ��������Ϣ
		gmCliPtyCtrl.lConnect = 0;
		gmCliPtyCtrl.lFd = 0;
		memset(gmCliPtyCtrl.bSessionId, 0, sizeof(gmCliPtyCtrl.bSessionId));
		gmCliPtyCtrl.lTimeOut = 0;
		gmCliPtyCtrl.lSendKpl = 0;
		gmCliPtyCtrl.lSerNo = 0;
		

		//�ر���������߳�
		end_oamPtyCliThread();
		//����shell ģʽ
		extern void cli_serial_set(void);
		cli_serial_set();
	}
	else
	{
		//���Ӳ����ڣ���������
	}

	//�˳�
	return;
}

static void OamPtyTimerMsgProcess(void)
{
	//��ȡ����״̬
	if(0 != gmCliPtyCtrl.lConnect)
	{
		//�Ѵ�������
		
		//��ʱ
		gmCliPtyCtrl.lTimeOut++;

		if(TIME_OUT_MAX < gmCliPtyCtrl.lTimeOut)
		{
			OamPtyShellCloseNoti(g_pty_master);
			gw_printf("pty time out, closed!\r\n");
		}
	}
	else
	{
		//δ��������
	}
}

gw_status gw_thread_delay(gw_uint32 milli_second)
{
	gw_status ret = GW_E_OK;
	if(CS_E_OSAL_OK == cs_thread_delay(milli_second))
	{
		ret = GW_E_OK;
	}
	else
	{
		ret = GW_E_ERROR;
	}
	return 0;
}

int gw_comm_onu_msg_send(unsigned char GwOpcode, unsigned int SendSerNo, unsigned char *pSentData,const unsigned short SendDataSize, unsigned char  *pSessionIdfield)
{
	int ret = 0;
	if(0 == CommOnuMsgSend(GwOpcode, SendSerNo, pSentData, SendDataSize, pSessionIdfield))
	{
		ret = 0;
	}
	else
	{
		cs_printf("CommOnuMsgSend err!\n");
		ret = -1;
	}
	return ret;
}

void gw_mutex_init(gw_mutex_t *mutex)
{
	cyg_mutex_init(mutex);
}

void gw_mutex_destroy(gw_mutex_t *mutex)
{
	cyg_mutex_destroy(mutex);
}

gw_bool_t gw_mutex_lock(gw_mutex_t *mutex)
{
	return cyg_mutex_lock(mutex);
}

gw_bool_t gw_mutex_trylock(gw_mutex_t *mutex)
{
	return cyg_mutex_trylock(mutex);
}

void gw_mutex_unlock(gw_mutex_t *mutex)
{
	cyg_mutex_unlock(mutex);
}



static int pty_master_fd_get(void)
{
	return g_pty_master;
}

static int pty_slave_fd_get(void)
{
	return g_pty_slave;
}

void gw_oam_pty_cli_thread_entry(gw_uint32 * para)
{
	oam_pty_cli_start();
}

static void start_oamPtyCliThread(void)
{
	if(GW_OSAL_MAX_THREAD == g_oam_pty_cli_thread_id)
	{
        if(GW_E_OK != gw_thread_create(&g_oam_pty_cli_thread_id, g_oam_pty_cli_thread_name, gw_oam_pty_cli_thread_entry, NULL, g_oam_pty_cli_thread_stack_size,
                g_oam_pty_cli_thread_pri, 0))
    	{
    		 gw_printf("create %s fail !\r\n", "cli thread");
    	}   
	}	
}

static void end_oamPtyCliThread(void)
{
	if(GW_OSAL_MAX_THREAD != g_oam_pty_cli_thread_id)
	{
		if(GW_E_OK != gw_thread_delete(g_oam_pty_cli_thread_id))
		{
			gw_printf("in %s, delete thread failed\n", __func__);
		}
		else
		{
			g_oam_pty_cli_thread_id = GW_OSAL_MAX_THREAD;
		}
		
	}
}

extern void oam_pty_shell_close(void)
{
	OamPtyShellCloseNoti(g_pty_master);
}
static void OamPtyShellCloseNoti(long lFd)
{
	unsigned long   lMsg[4] = {0};

	lMsg[0] = 0;
    lMsg[1] = PTY_SHELL_CLOSE_FLAG;
    lMsg[2] = PTY_NOTI_MSG;
    lMsg[3] = (unsigned long)lFd;

	if(GW_E_OK != gw_pri_queue_put(g_oam_pty_queue_id, lMsg, sizeof(lMsg), GW_OSAL_WAIT_FOREVER, 0))
	{
		gw_printf("OamPtyShellCloseNoti send msg fail!\r\n");
	}
}


extern int gwd_oam_pty_trans(void *pMsg)
{
	GW_OAM_MESSAGE_NODE *msg = (GW_OAM_MESSAGE_NODE *)pMsg;
	gw_uint32 data[4];
	GWTT_OAM_SESSION_INFO * pSessionInfo = NULL;
	gw_uint8 * payload = NULL;

	payload = malloc(msg->RevPktLen+8);
	pSessionInfo = malloc(sizeof(GWTT_OAM_SESSION_INFO));

	if(payload == NULL || pSessionInfo == NULL)
	{
		if(payload)
			free(payload);
		if(pSessionInfo)
			free(pSessionInfo);
		return GW_E_ERROR;
	}

	memset(payload, 0, msg->RevPktLen+8);
	memcpy(payload, msg->pPayLoad, msg->RevPktLen);

	pSessionInfo->SendSerNo = msg->SendSerNo;
	memcpy(pSessionInfo->SessionID, msg->SessionID, 8);

	data[0] = msg->RevPktLen;
	data[1] = (gw_uint32)pSessionInfo;
	data[2] = PTY_PACKET;
	data[3] = (gw_uint32)payload;

	if(GW_E_OK != gw_pri_queue_put(g_oam_pty_queue_id, data, sizeof(data), GW_OSAL_WAIT_FOREVER, 0))
	{
		free(payload);
		free(pSessionInfo);
		gw_printf("pty queue put fail!\r\n");
		return GW_E_ERROR;
	}

	return GW_E_OK;
}

extern int read_from_oam_pty(char *str,int length)
{
	int slave_fd = 0;
	int num_read = 0;
	slave_fd = pty_slave_fd_get();
	num_read = pty_read(slave_fd, str, length);
	return num_read;
}

extern int write_to_oam_pty(char *str,int length)
{
	int slave_fd = 0;
	int num_write = 0;
	slave_fd = pty_slave_fd_get();
	num_write = pty_write(slave_fd, str, length);
	return num_write;
}

extern int read_from_console(char *str,int length)
{
	int master_fd = 0;
	int num_read = 0;
	master_fd = pty_master_fd_get();
	num_read = pty_read(master_fd, str, length);
	return num_read;
}

extern int write_to_console(char *str,int length)
{
	int master_fd = 0;
	int num_write = 0;
	master_fd = pty_master_fd_get();
	num_write = pty_write(master_fd, str, length);
	return num_write;
}



