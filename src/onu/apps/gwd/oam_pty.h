#ifndef __OAM_PTY_H__
#define __OAM_PTY_H__


//�����ṩ�Ľӿ�
extern void init_oam_pty(void);
extern int gwd_oam_pty_trans(void *pMsg);
extern int read_from_oam_pty(char *str,int length);
extern int write_to_oam_pty(char *str,int length);
extern void oam_pty_shell_close(void);


/*end of file*/
#endif
