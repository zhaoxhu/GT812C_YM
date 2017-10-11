#ifndef __SYS_SPECS_H__
#define __SYS_SPECS_H__



#ifdef __cplusplus
extern "C"
{
#endif

/*
* ע��
*
* 1. ���ļ��Ķ������±����������̣���ǲ�Ʒ�������벻Ҫ���ڴ˴���
*
* 2. Ŀǰ��ƽ̨�ܶ�ط���slot��port��vid�ȶ��� base 0 ����ģ�
*    ������ base 1 ����ģ�����Ҫ��ֵ�� 1 Ϊ�ף�������ȡ��ƽ̨
*    ���������ٿ���������ѡ���� base 0 ���� base 1 ����
*/

/*
* Ŀǰ�йز�Ʒ���Ķ���ɢ���ܹ㣬�����ڸ��ĺ�ά����
* ���ļ����岻ͬ��Ʒ���ԭ��ɢ�������Ķ���һ���Ƴ���
* 
* ����������̬�����ú� XXX ���⣬�������ñ��� g_ulXXX ������
* ��Ǳ��룬������Ҫ���¶���������ñ��ļ��ĺ�������
* 
* MAXTrunkNum
*   ϵͳ�ڿ��Դ��������trunk��Ŀ��
* 
* MAXPortOnTrunk
*   trunk ����Ա�˿���Ŀ��
* 
* MAXVlanNum
*   ϵͳ�ڿ��Դ��������vlan��Ŀ��
* 
* MAXVlanNumWacky
*   ��������£�ϵͳ�ڿ��Դ��������vlan��Ŀ��
* 
* MAXSuperVlanNum
*   ϵͳ�ڿ��Դ��������super vlan��Ŀ��
*
* MAXL3InterfaceNum
*   ��������ip��vlan������
*  
* MAXSlotNum
*   ϵͳ������λ�š�
* 
* MAXPortOnSlotNum
*   ÿ��λ���˿�����
*
* EthPortOnCPCNum
*   ???
*
* MAXPortNum
*   ϵͳ�����˿���
*
*
*
*/
	
    #define MAXTrunkNum                     1
    #define MAXPortOnTrunk                  1
    
    /* vlan */
    #define MAXVlanNum                      4095 /* include invalid vid 0, 4095 */
    #define MAXVlanNumWacky                 128
    #define MAXSuperVlanNum                 1
    #define MAXSubVlanOnSuperVlan       	2
    #define MAXL3InterfaceNum               4 /* vlan �� ip */
    
    /* gvrp dynamic vlan */
    #define MAX_DyNamicVlan_NUM             5
    #define MAX_DyNamicVlan_Wacky_NUM       0     /* if have 5616 */
    #define MAXSlotNum                      2	/* Must >= (real Slot Num + 1) */
    #define MAXPortOnSlotNum                5	/* Must >= (real Port Num + 1) */
    #define EthPortOnCPCNum                 6	/* Must >= (real Port Num + 1) */
    #define MAXPortNum                      (MAXPortOnSlotNum*MAXSlotNum)

    /* fdb */
    #define MAXFdbDropNum                   1
    #define MAXFdbNormalNum                 1

    #define VOS_TASKPRI_GVRP                150
    #define VOS_TASKPRI_FDB                 200

#ifndef DEF_CTRL_CHAN_VLANID
	#define DEF_CTRL_CHAN_VLANID 			2
#endif
	//#define MAX_ROTER_PORT (pRcpDev->numOfPorts)
	#define SysBeginOfPortOnSlot   1
	#define SysEndOfPortOnSlot     g_ulMAXPortOnSlotNum
	#define SysIsValidPortOnSlot( _port ) \
    	((_port)>=SysBeginOfPortOnSlot && (_port)<=SysEndOfPortOnSlot)
#endif




