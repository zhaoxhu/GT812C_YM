#ifndef _SYSCFG_H_
#define _SYSCFG_H_

#ifdef	__cplusplus
extern "C"{
#endif
/* Please use -D_RELEASE_VERSION_ opition in Tornado compiler. *\
 * For changing this defination will cause lots of files have to be recompile.
 * You can create two Projects in tornado for debug and release version separately.
 * And in such way, we can save much compiling time for debug and release at the same time */
/*#define _RELEASE_VERSION_*/	
/*#undef  _RELEASE_VERSION_*/

#define _VOS_22_CHANGE_
/*----------------------------------------------------------------------*/
/*
 * �����ĸ��궨���˰汾��Ϣ���ɸ�����Ҫ�ڱ���ǰ�޸ģ��޸ĺ�������±��롣
 */
#define RPU_VERSION           "1.00"
#define RPU_VERSION_PRODUCT   "PSR"
#define RPU_COPY_TIME         "2001-2002"
#define RPU_CMO_NAME          "Ipsion Networks"
/*----------------------------------------------------------------------*/
#define _VOS_NEW_STACK_

/*----------------------------------------------------------------------*/
/*
 * ���������궨���ˡ������͡��ء�����״̬���������걻�����
 * �����á����磺
 *
 *	#define	RPU_MODULE_RTPRO_OSPF	RPU_NO
 *	#define	RPU_MODULE_RTPRO_RIP	RPU_YES
 *
 * ��ʾ��̬·��Э��OSPF���رգ�����̬·��Э��RIP����������
 * ����ϵͳ�ڱ���ʱ���Ϳ��Բ��OSPFģ�������RIPģ�顣
 * ��Ҫע�������Щ�����ϻ����һ�Ժ꣬���ܱ�ͬʱ��Ϊ
 * ��RPU_YES����RPU_NO������һ�㽫�����ں���������С�
 * ע�⣬�벻Ҫ�޸����������ֵ��
 */
#define	RPU_YES		1
#define	RPU_NO		0
/*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*/
/*
 * ������������������İ汾��������ROM���ϣ�����RAM���ϣ�
 * ���������ǻ���ģ����ǲ��ܱ���Ϊ��ͬ��ֵ����һ�Ժ����ʹ
 * RPU����ϳ�Ӳ��ƽ̨�Ƿ�֧�ֵ��ԣ��Ծ�����Բ�ͬ���͵ĵ�
 * �壨ָRAM���ROM�壩����ͬ�Ķ�����RPU���������ʹ��
 * �������꣺
 *
 *	#if	( RPU_BOARD_RAM == RPU_YES )
 *	{
 *		���ǵ��԰壬����Ƕ���ض��ĵ��Դ���
 *	}
 *	#else
 *	{
 *		�ǵ��԰壬���������⴦��
 *	}
 *	#endif
 *
 * ע�⣬����ĵ��Դ�������Բ�ͬӲ������ġ�
 * RPU����������ٵ�ʹ�øúꡣ
 */
#define	RPU_BOARD_RAM			RPU_YES
#define	RPU_BOARD_ROM			RPU_NO
/*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*/
/*
 * ����ĺ�����RPU����������е�Ӳ��ƽ̨�Ƿ�֧��ʵʱ�ӡ�
 * ������Щ��֧��ʵʱ�ӵ�Ӳ��ƽ̨��RPU��VOS��Ҫʹ�ò���ϵͳ�ṩ�ĵ���
 * ������ϵͳ����ʱ�������պ�ʱ�䣻�����������֧��ʵʱ�ӵ�Ӳ��ƽ̨��
 * ��������ǲ�Ӧ�������ġ����磬RPU��VOS������ʹ������꣺
 *
 *	#if( RPU_BOARD_REAL_TIMER == RPU_NO )
 *	{
 *		����ϵͳ������ʱ��
 *	}
 *	else
 *	{
 *		��������ϵͳ������ʱ��
 *	}
 *	#endif
 *
 * ����ݲ��õ�Ӳ��ƽ̨�ľ�����������øú��ֵ��
 */
#define	RPU_BOARD_REAL_TIMER	RPU_YES
/*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*/
/*
 * ����������Ŀ���ؿ��Ծ����������RPU�汾�ǡ����а汾��
 * ���ǡ����԰汾�����������������Ҳ�ǻ���ġ��ڵ��԰汾
 * �У�Ӧ�ó������Ƕ������ĵ�����Ϣ�����ڷ��а汾�У�����
 * ������ġ���ע�⣬ͨ������������Ƶĵ�����Ϣ������Ӳ��
 * ƽ̨�޹أ�Ҳ����˵�������ĵ�����Ϣ��ROM���Ϻ�RAM������
 * һ���ģ�����Ҫ����Ӳ��ƽ̨�ĵ�����Ϣ�ɺ�
 * ��RPU_BOARD_RAM���͡�RPU_BOARD_ROM����ȷ�����μ�
 * ǰ��Ľ��ܣ���RPU���������ʹ���������꣺
 *
 *	#if	( RPU_VERSION_RELEASE == RPU_NO )
 *	{
 *		���԰汾������Ƕ����Դ���
 *	}
 *	#else
 *	{
 *		���а汾���Ͻ�Ƕ����Դ���
 *	}
 *	#endif
 *
 * ��
 *
 *	#if	( RPU_VERSION_DEBUG == RPU_YES )
 *	{
 *		���԰汾������Ƕ����Դ���
 *	}
 *	#else
 *	{
 *		���а汾���Ͻ�Ƕ����Դ���
 *	}
 *	#endif
 */
#define	RPU_VERSION_RELEASE		RPU_NO
#define	RPU_VERSION_DEBUG		RPU_YES
/*----------------------------------------------------------------------*/
    /* �����Ʒ��� */
#define broadcomppc824x      		0x1
#define broadcomppc75x       		0x2
#define galileoppc824x       		0x3
#define broadcommips4131     	0x4
#define broadcomxscale2400   	0x5
#define presterappc824x      		0x6
#define isurf						0x7
#define GT831					0x8
#define EPON3					0x9
#define GT810					0xa
#define GT812                   0xb
#define GT811                   0xc
#define GT861_ONU				0xd
#define GT861_IAD				0xe
#define GT815                   0xf
#define GT812PB                 0x10
#define GT866_ONU               0x11
#define GT866_IAD				0x12
#define GT831B					0x13

#define GT863_ONU   			0x14
#define GT813                   0x15
/* Define PAS Firmware version */
#define _FW_V1_4_B0_3_			0x1
#define _FW_V1_4_B7_4_			0x7

#define BCM53242	0
#define BCM53212	1
#define BCM5324     2

#define FOR_ONU_812_AB ((PRODUCT_CLASS == GT812) || (PRODUCT_CLASS == GT812PB))
#define FOR_812_SERIES (FOR_ONU_812_AB || (PRODUCT_CLASS == GT811))

#define FOR_BCM_ONU_PON (PRODUCT_CLASS == GT866_ONU || PRODUCT_CLASS == GT863_ONU || PRODUCT_CLASS == GT813)

#define FOR_BCM_ONU_PON_VOICE (FOR_GT863_ONU_SERIS)

#define FOR_GT863_ONU_SERIS (PRODUCT_CLASS == GT863_ONU || PRODUCT_CLASS == GT866_ONU)

#define FOR_MRV_BOX_PON	((PRODUCT_CLASS == GT815) || FOR_812_SERIES || (PRODUCT_CLASS == GT831B))
#define FOR_MRV_ONU_PON_NO_811	((PRODUCT_CLASS == GT861_ONU) || (PRODUCT_CLASS == GT815) || FOR_ONU_812_AB || (PRODUCT_CLASS == GT831B))
#define FOR_MRV_ONU_PON	(FOR_MRV_ONU_PON_NO_811 || (PRODUCT_CLASS == GT811))

#define FOR_MULITY_PORT_BOX_ONU_PON	((PRODUCT_CLASS == GT831) || FOR_MRV_BOX_PON || FOR_BCM_ONU_PON)
#define FOR_SINGLE_PORT_BOX_ONU_PON	((PRODUCT_CLASS == GT810))
#define FOR_BOX_ONU_PON	(FOR_MULITY_PORT_BOX_ONU_PON || FOR_SINGLE_PORT_BOX_ONU_PON)
#define FOR_CHASSIS_ONU_PON	(PRODUCT_CLASS == GT861_ONU)

#define FOR_MULITY_PORT_ONU_PON	(FOR_CHASSIS_ONU_PON || FOR_MULITY_PORT_BOX_ONU_PON)
#define FOR_ONU_IAD 	((PRODUCT_CLASS == GT861_IAD) || (PRODUCT_CLASS == isurf) || (PRODUCT_CLASS == GT866_IAD))
#define FOR_ONU_PON_NO_CTRL_CHANNEL 	(FOR_MRV_BOX_PON || FOR_SINGLE_PORT_BOX_ONU_PON)
#define FOR_ONU_PON_NO_831 	(FOR_MRV_ONU_PON || FOR_SINGLE_PORT_BOX_ONU_PON || FOR_BCM_ONU_PON)

#define INCLUDE_IN_ONU 	(FOR_ONU_PON || FOR_ONU_IAD)
#define EXCLUDED_IN_ONU (!INCLUDE_IN_ONU)
#define INCLUDE_IN_ONU_NO_ISURF	 (FOR_ONU_PON || (PRODUCT_CLASS == GT861_IAD) || (PRODUCT_CLASS == GT866_IAD))
#define INCLUDE_IN_BOX_ONU 	(FOR_BOX_ONU_PON || FOR_ONU_IAD)

/*----------------------------------------------------------------------*/
/*
 * ����������ΪRPU������������ֽ�˳��Ҳ��
 * ��������ĺꡣ������ƷӦ�ø���������CPU�����ǡ��������
 * �������ꡣRPU���������ʹ���������꣺
 *
 *	#if	( RPU_LITTLE_ENDIAN == RPU_NO )
 *	{
 *		����BIG_ENDIAN�ֽ�˳�������ݽṹ
 *	}
 *	#else
 *	{
 *		����LITTLE_ENDIAN�ֽ�˳�������ݽṹ
 *	}
 *	#endif
 *
 * ��
 *
 *	#if	( RPU_BIG_ENDIAN == RPU_YES )
 *	{
 *		����BIG_ENDIAN�ֽ�˳�������ݽṹ
 *	}
 *	#else
 *	{
 *		����LITTLE_ENDIAN�ֽ�˳�������ݽṹ
 *	}
 *	#endif
 */
#if (PRODUCT_CLASS == isurf)
#define	LITTLE_ENDIAN		RPU_NO
#define	BIG_ENDIAN		    RPU_YES
#ifdef _DISTRIBUTE_PLATFORM_
#define 	VOS_BIG_ENDIAN			VOS_YES
#endif
#endif

#if (FOR_ONU_PON || (PRODUCT_CLASS == GT861_IAD) || (PRODUCT_CLASS == GT866_IAD))
#define	LITTLE_ENDIAN		RPU_YES
#define	BIG_ENDIAN		    RPU_NO
#ifdef _DISTRIBUTE_PLATFORM_
#define 	VOS_BIG_ENDIAN			VOS_NO
#define   VOS_LITTLE_ENDIAN			VOS_YES
#endif
#endif

/*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*/
/*
 * ���漸����������RPU����������е�Ӳ��ƽ̨��CPU�����͡���
 * �ǣ�����VOSģ�飬RPU������Э�����ģ�齫������ʹ���⼸
 * ���ꡣ�⼸����Ķ���Ҳ�ǻ���ģ�Ҳ����˵����һ�α���ʱ��
 * ������ֻ����һ���������RPU_YES������Ķ���RPU_NO��
 */
#define	RPU_CPU_mPPC_8240		RPU_NO
#define	RPU_CPU_mPPC_750		RPU_YES
#define	RPU_CPU_x86			    RPU_NO
/*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*/
/*
 * ���漸����������RPU����������е�OSƽ̨�����͡������ʵ�������
 * ���е�һ����ΪRPU_YES�������ȫ����ΪRPU_NO�����ǣ�����VOSģ�飬
 * RPU������Э�����ģ�齫������ʹ���⼸���ꡣ
 */
/*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*/
/*
 * �⼸����������RPU����������е�RTOSƽ̨��ʹ�õı�������
 * ���͡����ǣ�����VOSģ�飬��������Э�����ģ�龡����Ҫ
 * ʹ���⼸���ꡣ
 * ������Ҫ˵�����ǣ�pSOS����ϵͳ��Windows3.11�µĻ�������
 * ��Green Hill Software��˾��һ������C-68000�ı�������ʹ��
 * ��RPU_COMPILER_GHC_68000����ʶ����pSOS����ϵͳ��
 * Windows95��Windows98�µĻ����²�����CC386��������ʹ�ú�
 * RPU_COMPILER_CC386����ʶ�������������ֱ���������ʶ���
 * �ֽڶ����Ԥ������ǲ�ͬ�ģ�C-68000ʶ��pack����CC386ʶ��
 * align���������£����Ӧ�ó�����ϣ��ָ����������1�ֽڶ�����
 * �ݽṹTEST_S��Ӧ��������д����
 *
 *	#if( RPU_COMPILER_GHC_68000 == RPU_YES || RPU_COMPILER_GCC == RPU_YES )
 *	#pragma  pack( 1 )
 *	#elif( RPU_COMPILER_CC386 == RPU_YES )
 *	#pragma  align( tagTest = 1 )
 *	#endif
 *
 *	typedef  struct  tagTest
 *	{
 *		......
 *	} TEST_S;
 *
 *	#if( RPU_COMPILER_GHC_68000 == RPU_YES  || RPU_COMPILER_GCC == RPU_YES )
 *	#pragma  pack
 *	#elif( RPU_COMPILER_CC386 == RPU_YES )
 *	#pragma  align
 *	#endif
 *
 * ����ע�⣬��������Ӳ���������ֻ������˵��������ѣ���
 * ����pack��align����ϸ˵������ο�������Я���İ����ļ���
 */
#define	RPU_COMPILER_CC386		RPU_NO
#define	RPU_COMPILER_GHC_68000	RPU_NO
#define	RPU_COMPILER_GCC		RPU_YES
#define	RPU_COMPILER_DCC		RPU_NO
#define	RPU_COMPILER_MSVC		RPU_NO
/*----------------------------------------------------------------------*/

/************************************************************************
 * RPU�����Ʒ��
 ***********************************************************************/
/* #define _L3_CHASIS_SWITCH_ 			RPU_YES */
/*#define _ROUTER_			RPU_NO*/
/************************************************************************/

/************************************************************************
 * �ֲ�ʽ·������궨��
 ***********************************************************************/
/*#define VOS_V2			RPU_YES
***********************************************************************


 *                                                                      *
 * ���µĺ�������RPU����и�Э��ģ���͹���ģ�飬ͨ��������Щ��        *
 * �Ŀ���״̬�������ڱ���ʱ���벻��Ҫ��Э����ܡ����ǣ�����RPU        *
 * ��һЩ����ģ�飬��������룬Ҳ����˵��������Щģ��ĺ���뱻��       *
 * ΪRPU_YES������״̬��                                                *
 *                                                                      *
 * ����Ҫע���һ���ǣ�����Э��͹��ܵ��ص㣬RPU��һЩģ��֮�����      *
 * һ����������ϵ��������������ϵ�����ɸ�ģ�����ͬʱ��������رա�     *
 * Ҳ����˵�����Aģ��������Bģ�飬��ô�����Ҫ����Aģ��ʱ��Bģ��       *
 * Ҳ���뱻������                                                       *
 *                                                                      *
 * ����RPU�ĺ���ģ����뱻���������Է��������ģ���������ϵ����������  *
 *                                                                      *
 ************************************************************************/
/*********************
 * RPU������������� *
 *********************/
/*----------------------------------------------------------------------*/
/*
 * RPU��������豸����ģ�顣
 * ʹ��RPU������������뱻������
 */
#define	RPU_MODULE_DEVICE			RPU_YES
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* FIBͬ��ģ�� */
/*----------------------------------------------------------------------*/
#define	RPU_MODULE_FSYN   			RPU_YES
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/


/*************************
 * RPU����ļ��ز��� *
 *************************/
/*----------------------------------------------------------------------*/
/*
 * ʹ��RPU�����������������뱻������
 */
#define	RPU_MODULE_LOAD			RPU_NO
#ifdef _DISTRIBUTE_PLATFORM_
#define   LMU_MODULE_LOAD                 RPU_NO
#endif

#define RPU_MODULE_IEM          RPU_NO

#define RPU_MODULE_CDP          RPU_NO
/*
 �ȱ���ģ�顣
 */
#define	RPU_MODULE_BACKUP		RPU_NO


/*
 * �ӿڹ���
 */

#define	RPU_MODULE_IFM			RPU_YES


/*
 * RIPģ��
 */
#define RPU_MODULE_RIP          RPU_NO
#define RPU_MODULE_OSPF         RPU_NO
#define RPU_MODULE_ISIS          RPU_NO
#define RPU_MODULE_BGP          RPU_NO
#define RPU_MODULE_EIGRP        RPU_NO

/*
* IP MULTICAST ROUTING
*/
#define RPU_MODULE_MIP			RPU_NO

#define RPU_MODULE_MROUTE       RPU_MODULE_MIP
#define RPU_MODULE_IGMP         RPU_MODULE_MIP
#define RPU_MODULE_PIM          RPU_MODULE_MIP

/*
 *Rtmģ��
 */

#define RPU_MODULE_RTM          RPU_YES


/*
 *LSP����ģ��
 */

#define RPU_MODULE_LSPM         RPU_NO


/*
 *LDPģ��
 */

#define RPU_MODULE_LDP          RPU_NO


/*
 *RSVPģ��
 */

#define RPU_MODULE_RSVP         RPU_NO

/*
 *NATģ��
 */

#define RPU_MODULE_NAT        RPU_NO

/*
 *STPģ��
 */
#define RPU_MODULE_STP	        RPU_NO

#ifdef _DISTRIBUTE_PLATFORM_
/*
* RPU_MODULE_SUPERVISOR
*/
#define RPU_MODULE_SUPERVISOR    RPU_YES

/*
* _MAIL_SEND_
*/
#define _MAIL_SEND_    RPU_NO

/*
 *_HOOK_
 */
#define _HOOK_		RPU_NO

/*
 *ERPPģ��
 */
#define RPU_MODULE_ERRP	        RPU_NO

/*
 *LDTģ��
 */
#define RPU_MODULE_LDT          RPU_NO

/*
 *IGMP_SNOOPģ��
 */
#define RPU_MODULE_IGMP_SNOOP    RPU_YES

/*
 * NP ֧�ּ��ú�
 */
#define RPU_MODULE_NP            RPU_NO

/*
 *CHDLC ֧�ֲü���             
 */
#define LMU_MODULE_CHDLC        RPU_NO

/*
 *ARP ֧�ֲü���             
 */
#define LMU_MODULE_ARP           RPU_YES

/*
 *POS ֧�ֲü���             
 */
#define RPU_MODULE_POS          RPU_NO

/*
 *ATM ֧�ֲü���             
 */
#define RPU_MODULE_ATM          RPU_NO
/*
 *Port Filter�ü�
 */
#define RPU_MODULE_FILTER       RPU_NO
#endif

/*
 *CLIģ��
 */
#define RPU_MODULE_CLI	        RPU_YES

/*
 *SNMP ģ��
 */
#define RPU_MODULE_SNMP         RPU_YES

/*
 *Web ģ��
 */
#define RPU_MODULE_MANA_WEB         RPU_NO

/*
 *GRE����ģ��
 */

#define RPU_MODULE_GRE          RPU_NO

/*
 *ARPģ��
 */
#define RPU_MODULE_ARP	        RPU_YES
/*
 *Manageģ��
 */
#define RPU_MODULE_MANAGE       RPU_YES

/*
 *Manageģ��
 */
#define RPU_MODULE_XMODEM       RPU_NO

/*
 *Ftp clientģ��
 */
#define RPU_MODULE_FTP_CLIENT	RPU_NO

/*
 *Ftp serverģ��
 */
#define RPU_MODULE_FTP_SERVER	RPU_NO

/*
 *SysLogģ��
 */
#define RPU_MODULE_SYSLOG       RPU_YES


/*
* VFS ģ��
*/
#define RPU_MODULE_VFS			RPU_NO

/*
 *FDB ģ��
 */
 #if(FOR_BCM_ONU_PON)
#define RPU_MODULE_FDB          RPU_YES
#else
#define RPU_MODULE_FDB          RPU_NO
#endif

/*
 *Qos ģ��
*/
#define RPU_MODULE_QOS			RPU_NO

/*
 *VRRP ģ��
 */
#define RPU_MODULE_VRRP         RPU_NO

/*
 *HSRP ģ��
 */
#ifndef _DISTRIBUTE_PLATFORM_
#define RPU_MODULE_HSRP         RPU_NO
#else
#define RPU_MODULE_HSRP         RPU_NO
#endif

/*
 *SNTP ģ��
 */
#define RPU_MODULE_SNTP         RPU_YES
/*
 * HOTģ��
 */
#define RPU_MODULE_HOT          RPU_NO

/*
 * L2VPNģ��
 */
#define RPU_MODULE_L2VPN        RPU_NO


/*
 * VPLSģ��
 */
#define RPU_MODULE_VPLS        RPU_NO

/*
 * L3VPNģ��
 */
#define RPU_MODULE_L3VPN        RPU_NO


/*
 * DHCP ģ��
 */
#define RPU_MODULE_DHCP         RPU_YES

/*
 * UDP FORWARDING ģ��
 */
#define RPU_MODULE_UDP_FORWARD	RPU_NO

/*
 * GVRP ģ��
 */
#define RPU_MODULE_GVRP	        RPU_NO

/*
 * RADIUSģ��
 */
#define RPU_MODULE_RADIUS       RPU_NO

/*
 *NASģ��
 */
#define RPU_MODULE_NAS          RPU_NO

/*
 *IEEE 802.1Xģ��
 */
#define RPU_MODULE_DOT1X        RPU_NO

/*
 *HLINKģ��
 */
#define RPU_MODULE_HLK       RPU_NO

/*
 *PPPģ��
 */
#define RPU_MODULE_PPP          RPU_NO

/*
 *PPPOEģ��
 */
#define RPU_MODULE_PPPOE        RPU_NO

/*
*PPPOE����
*/
#define RPU_MODULE_PPPOE_SNOOPING	RPU_NO

/*
*PPPOE�м�
*/
#define RPU_MODULE_PPPOE_RELAY  RPU_NO 

/*
*��ʱ���ͱ���
*/
#define RPU_MODULE_TIMING_PKT  RPU_NO

/*
��vlan�鲥
*/
#define RPU_MODULE_IGMP_TVM     RPU_NO

/*
 *DHCPCģ��
 */
#define RPU_MODULE_DHCPC        RPU_YES

/*
 *DHCP Serverģ��
 */
#define RPU_MODULE_DHCPS        RPU_YES

/*
 *TACACS+ģ��
 */
#define RPU_MODULE_TACC        RPU_NO

/*
 *COPS(PEP)ģ��
 */
#define RPU_MODULE_COPS		RPU_NO

/*
 *HTTP Redirectģ��
 */
#define RPU_MODULE_RED		RPU_NO

/*
 *SSH ģ��
 */
#define RPU_MODULE_SSH 	RPU_NO

/*
  * CPUʹ����ͳ�ƺ���ѭ��ͳ��
  */
#define RPU_MODULE_CPU          RPU_YES

/*
* TRACEROUTE
*/
#define _CONFIG_TRACERT_  
/*
* TRACEROUTE
*/
#define _CONFIG_DNS_            RPU_YES

/*
  * TARGET SHELL������������shell
  */
#define RPU_MODULE_TARGET_SHELL   RPU_YES

/*
  * VOS ACTIVE SHELL������������ʱ����shell
  */
#ifndef  _DISTRIBUTE_PLATFORM_
#define RPU_MODULE_ACTIVE_SHELL   RPU_YES
#else
#define RPU_MODULE_ACTIVE_SHELL   RPU_YES
#endif

#define RPU_MODULE_CTC_OAM 	RPU_NO

/*
  * ���ذ��Ͽ��Բ�����·��������shell,�ӿ������ذ���ʾ
  */
#define RPU_MODULE_RPU_TO_LIC_SHELL   RPU_YES
/*
  * ���ذ��Ͽ�����������������·���������ִ��,���ذ�������
  */
#define RPU_MODULE_CLI_TO_LIC   RPU_YES

#define RPU_HAVE_FPGA			RPU_NO

#define RPU_MODULE_VOICE		RPU_NO

#define RPU_MODULE_E1		RPU_NO

#define RPU_MODULE_VOIP		RPU_NO

#define RPU_LOAD_CFG_FILE_FROM_MASTER RPU_NO

#define RPU_MODULE_EPON_ALL RPU_NO

#define RPU_MODULE_XCVR_MON RPU_NO

#define RPU_MODULE_CPLD		RPU_NO

#define RPU_MODULE_CTRL_CHANNEL		RPU_NO

#ifdef _DISTRIBUTE_PLATFORM_
/* ������Ʒ�Լ���ģ����������ļ� */
/* ��makefile/superset/superset.h����ͬ��Ʒ���Թرղ���Ҫ��ģ�� */
#include "superset.h"
#endif
/************************************************************************
 *                                                                      *
 * ���µĺ�������RPU�����һЩ��ϵͳ��صĳ�����                        *
 * ͨ��������Щ������ֵ������ʹRPU��Ӧ��ͬ��ϵͳ��Ҫ��                *
 * ����Щ�������޸ģ�Ӧ���ɾ���ḻ��ϵͳ��Ա�����ģ�                   *
 * ������ȷ�е����ɣ������벻Ҫ���׸Ķ���Щ������                     *
 *                                                                      *
 ************************************************************************/
/*----------------------------------------------------------------------*/
/* #define */
/*----------------------------------------------------------------------*/


/************************************************************************
 *                                                                      *
 * ������ΪRPU����ĸ������ֶ���ĵ��Ժꡣ                              *
 *                                                                      *
 * ���ǵ���ģ����Եķ����ԣ�ΪRPU�ĸ����ֶ������ϵĵ���                *
 * �ꡣ������Щ����������ڣ����Կ���RPU�����հ汾ֻ����ĳ              *
 * һ���ֵĵ��Դ��룬�Ӷ����Ʊ�����汾�Ĵ�С�����ڵ��ԣ���             *
 * Щ����RPU�ĸ���������һһ��Ӧ�ģ�����VOS�������ԣ���                 *
 * VOS�ĵ��Ժ갴��VOS����ģ���һ��ϸ�����������ĺ�����               *
 * RPU�İ汾�꣨��RPU_VERSION_DEBUG��                                   *
 * RPU_VERSION_RELEASE�����ʹ�ã�Ҳ����˵�������                      *
 * RPU_DEBUG_IP�Ǳ��򿪵ģ����Ǻ�RPU_VERSION_DEBUG��                    *
 * �رղ��Һ�RPU_VERSION_RELEASE���򿪣���ô������IP����                *
 * �ĵ��Դ���ͬ�����ܱ�������汾�С�                                   *
 * Ϊ�˱��ڱ�̣���ģ�����ʹ�ú�RPU_DEBUG_IS_ON�򻯴�                  *
 * �루��RPU_DEBUG_IS_ON�Ķ�������������                              *
 * ���磺                                                               *
 *                                                                      *
 *	#if( RPU_DEBUG_IS_ON( RPU_DEBUG_MPLS ) == RPU_YES )                   *
 *	{                                                                   *
 *		���Դ���                                                        *
 *	}                                                                   *
 *	#endif                                                              *
 *                                                                      *
 ************************************************************************/
#define	RPU_DEBUG_DEVICE		RPU_NO
#define	RPU_DEBUG_LOAD			RPU_NO
#define	RPU_DEBUG_LICA			RPU_NO
#define	RPU_DEBUG_IPC			RPU_NO
#define	RPU_DEBUG_RS			RPU_NO
#define	RPU_DEBUG_VRRP			RPU_NO

#define	RPU_DEBUG_IS_ON( RPU_DEBUG ) \
		( \
		    ( RPU_VERSION_DEBUG == RPU_YES ) \
		  ? ( RPU_YES ) \
		  : ( \
				( RPU_DEBUG == RPU_YES ) \
			  ? ( RPU_YES ) \
			  : ( RPU_NO ) \
			) \
		)

/* LSP��LDP��RSVP��������ϵ */
#if (RPU_MODULE_LSPM == RPU_NO)
#undef  RPU_MODULE_LDP
#undef  RPU_MODULE_RSVP
#define RPU_MODULE_LDP         RPU_NO
#define RPU_MODULE_RSVP        RPU_NO
#elif((RPU_MODULE_LSPM == RPU_YES) && (RPU_MODULE_LDP == RPU_NO) && (RPU_MODULE_RSVP == RPU_NO))
#error if RPU_MODULE_LSPM is on, either RPU_MODULE_LDP or RPU_MODULE_RSVP is on !
#endif

/* L3VPN��LSPM��BGP��������ϵ */
#if (RPU_MODULE_LSPM == RPU_NO || RPU_MODULE_BGP == RPU_NO)
#undef RPU_MODULE_L3VPN
#define RPU_MODULE_L3VPN       RPU_NO
#endif /* (RPU_MODULE_LSPM == RPU_NO || RPU_MODULE_BGP == RPU_NO) */


/* LDP��� */
#if (RPU_MODULE_LDP == RPU_YES)
#define VOS_MPLS_LDP        1                   /* MPLS LDPģ�� */
#define VOS_MPLS_L2VPN      1                   /* MPLS L2VPNģ�� */
#define VOS_MPLS_L2VPN_EX   1 
#define VOS_MPLS_VPLS       1 
#define VOS_MPLS_MIB        1                   /*MPLS_MIB֧��*/
#else /* RPU_MODULE_LDP == RPU_NO */
#define VOS_MPLS_LDP        0                   /* MPLS LDPģ�� */
#define VOS_MPLS_L2VPN      0                   /* MPLS L2VPNģ�� */
#endif /* RPU_MODULE_LDP == RPU_YES */
/* L2VPN��� */
#if (RPU_MODULE_L2VPN == RPU_NO)
#undef  VOS_MPLS_L2VPN
#undef  VOS_MPLS_L2VPN_EX
#define VOS_MPLS_L2VPN      0                   /* MPLS L2VPNģ�� */
#define VOS_MPLS_L2VPN_EX   0 
#endif
/* VPLS */
#if(RPU_MODULE_VPLS == RPU_NO)
#undef  VOS_MPLS_VPLS
#define VOS_MPLS_VPLS       0
#endif

/* RSVP��� */
#if (RPU_MODULE_RSVP == RPU_YES)
#define VOS_MPLS_RSVPTE     1                   
#else /* RPU_MODULE_RSVP == RPU_NO */
#define VOS_MPLS_RSVPTE     0                   /* MPLS RSVP-TEģ�� */
#endif /* RPU_MODULE_RSVP == RPU_YES */

/* MPLS��ת����� */
#if (RPU_MODULE_LSPM == RPU_YES)
#define VOS_MPLS_LSPM       1                   /* MPLS LSP����ģ�� */
#define VOS_MPLS_SOFTFW     1                   /* MPLS ��ת�� */
#else
#define VOS_MPLS_LSPM       0
#define VOS_MPLS_SOFTFW     0                   /* MPLS ��ת�� */
#endif /* (RPU_MODULE_LSPM == RPU_YES) */

/* GRE��� */
#if (RPU_MODULE_GRE == RPU_YES)            
#define VOS_GRE	            1                   /*GRE ģ��*/
#else
#define VOS_GRE	            0    
#endif /* (RPU_MODULE_LSPM == RPU_YES) */

#if (RPU_MODULE_UDP_FORWARD == RPU_YES)
#define VOS_UDPF_SOFTFW       1   /*���ذ�*/
#define VOS_UDPF_LICAFW       0   /*�ӿڰ�*/
#else
#define VOS_UDPF_SOFTFW       0
#endif

/* BGP��� */
#if (RPU_MODULE_BGP == RPU_YES)
#define VOS_ROUTE_BGP        1       	/* BGPЭ��ģ�� */
#define VOS_ROUTE_BGP_MD5    1       	/* BGP MD5���� */
#if (RPU_MODULE_SNMP == RPU_YES)
#define VOS_ROUTE_BGP_MIB     1           /*BGP ��������֧��*/
#endif
#endif /* (VOS_ROUTE_BGP == RPU_YES) */

/* L3VPN��� */
#if (RPU_MODULE_L3VPN == RPU_YES)
#define VOS_VRF_MPLS_VPN	1
#if (RPU_MODULE_SNMP == RPU_YES)
#define VOS_ROUTE_3LVPN_MIB     1           /*3LVPN ��������֧��*/
#endif
#endif /* (RPU_MODULE_L3VPN == RPU_YES) */


/*NAT ���*/
#if (RPU_MODULE_NAT == RPU_YES)
#define VOS_IP_NAT                 1                    /*NAT*/
#else
#define VOS_IP_NAT                 0
#endif
#if ( VOS_IP_NAT && VOS_VRF_MPLS_VPN )
#define VOS_IP_NAT_VPN 		       1 
#endif


/* DHCP Relay��� */
#if (RPU_MODULE_DHCP == RPU_YES && RPU_MODULE_L3VPN == RPU_YES)
#define VOS_VRF_MPLS_VPN_DHCPR   1
#endif

/* �鲥��� */
#if (RPU_MODULE_MROUTE == RPU_YES && RPU_MODULE_IGMP == RPU_YES && RPU_MODULE_PIM == RPU_YES)
#define VOS_MULTICAST_ROUTING 	 1    		  /* ֧���鲥·��ת�� */
#endif

#if VOS_MULTICAST_ROUTING
#define VOS_ROUTE_MROUTE_SOFTWEAR_FORWARD 1 /*���ת��*/
#define VOS_ROUTE_IGMP    1       	/* IGMPЭ��ģ�� */
#ifndef _DISTRIBUTE_PLATFORM_
#define VOS_ROUTE_IGMP_BROADING 1 /* �Ƿ�֧����valn�ڹ㲥igmp����(ת����ʱ��) */
#else
#define VOS_ROUTE_IGMP_BROADING 0 /* �Ƿ�֧����valn�ڹ㲥igmp����(ת����ʱ��) */
#endif
#define VOS_ROUTE_PIM   	1		/* PIMЭ��ģ�� */
#define VOS_ROUTE_PIM_SM   1      /* PIM-SM */
#if VOS_ROUTE_PIM_SM
#define VOS_ROUTE_PIM_DM   1      /* PIM-DM */
#define VOS_ROUTE_PIM_ACCURACY_PORT_FORWARD       1/* pim ���ڶ˿ڵľ�ȷת�� */
#endif
#define VOS_ROUTE_PIM_MULTI	1		/* PIM ��ʵ��֧�� */


#define VOS_ROUTE_MROUTE_NM 0		/* �鲥���ܹ��ܵ�֧�� */
#endif

#ifdef _DISTRIBUTE_PLATFORM_
#define VOS_ROUTE_MVPN		0
#else
#if VOS_MULTICAST_ROUTING && VOS_VRF_MPLS_VPN && VOS_ROUTE_PIM_MULTI
#define VOS_ROUTE_MVPN		1		/*�鲥VPN֧��*/
#endif
#endif

#if VOS_ROUTE_PIM_ACCURACY_PORT_FORWARD
#define VOS_ROUTE_STATIC_MROUTE    1/*֧�־�̬�鲥*/
#endif


/* ���ʿ������ */
#undef _MN_HAVE_NMS_ACCESS_CONTROL_

/*����ģ���VFS������*/
#if (RPU_MODULE_VFS == RPU_NO)
#define RPU_MODULE_FTP_CLIENT  RPU_NO
#define RPU_MODULE_FTP_SERVER  RPU_NO
#endif

#ifdef _DISTRIBUTE_PLATFORM_
#define VOS_PRODUCT_FAKE  1               /* ���ڶಥģ��Ĳ�Ʒ֧�� */
#endif

#ifdef _DISTRIBUTE_PLATFORM_
#define VOS_SSHD  1 			  /*֧��Secure Shell */
#define VOS_LIB_CRYPTO 1
#endif

#ifdef _DISTRIBUTE_PLATFORM_
#define _SWITCH_PLATFOMR2_L3PORT_SUPPORT_        /*3��˿�֧��,per l3 port per vlan */
#define _DISTRIBUTE_PLATFORM_SYSLOG2NVRAM_SUPPORT_  /*֧��syslog��nvram�� bootlog��navram */
#if (INCLUDE_IN_ONU)
#undef _DISTRIBUTE_PLATFORM_SYSLOG2NVRAM_SUPPORT_  /*֧��syslog��nvram�� bootlog��navram */
#endif

#define _DISTRIBUTE_PLATFORM_PACKET_PROCESS_		 /*֧�ֽӿڰ���а�����*/
#define _SWITCH_PLATFOMR2_MIRROR_SUPPORT_        /*֧�ֶ˿ھ���*/
#define _DISTRIBUTE_PLATFORM_ETH0_SUPPORT_          /* ֧�� */
#endif

#ifdef _DISTRIBUTE_PLATFORM_
/* ��Ʒ���Ԥ�� */
#include "sys/main/sys_specs.h"
#endif

#ifdef	__cplusplus
}
#endif	/* end of __cplusplus */

#endif	/* end of _SYSCFG_H_ */

