/****************************************************************************
            Software License for Customer Use of Cortina Software
                          Grant Terms and Conditions

IMPORTANT NOTICE - READ CAREFULLY: This Software License for Customer Use
of Cortina Software ("LICENSE") is the agreement which governs use of
software of Cortina Systems, Inc. and its subsidiaries ("CORTINA"),
including computer software (source code and object code) and associated
printed materials ("SOFTWARE").  The SOFTWARE is protected by copyright laws
and international copyright treaties, as well as other intellectual property
laws and treaties.  The SOFTWARE is not sold, and instead is only licensed
for use, strictly in accordance with this document.  Any hardware sold by
CORTINA is protected by various patents, and is sold but this LICENSE does
not cover that sale, since it may not necessarily be sold as a package with
the SOFTWARE.  This LICENSE sets forth the terms and conditions of the
SOFTWARE LICENSE only.  By downloading, installing, copying, or otherwise
using the SOFTWARE, you agree to be bound by the terms of this LICENSE.
If you do not agree to the terms of this LICENSE, then do not download the
SOFTWARE.

DEFINITIONS:  "DEVICE" means the Cortina Systems?Daytona SDK product.
"You" or "CUSTOMER" means the entity or individual that uses the SOFTWARE.
"SOFTWARE" means the Cortina Systems?SDK software.

GRANT OF LICENSE:  Subject to the restrictions below, CORTINA hereby grants
CUSTOMER a non-exclusive, non-assignable, non-transferable, royalty-free,
perpetual copyright license to (1) install and use the SOFTWARE for
reference only with the DEVICE; and (2) copy the SOFTWARE for your internal
use only for use with the DEVICE.

RESTRICTIONS:  The SOFTWARE must be used solely in conjunction with the
DEVICE and solely with Your own products that incorporate the DEVICE.  You
may not distribute the SOFTWARE to any third party.  You may not modify
the SOFTWARE or make derivatives of the SOFTWARE without assigning any and
all rights in such modifications and derivatives to CORTINA.  You shall not
through incorporation, modification or distribution of the SOFTWARE cause
it to become subject to any open source licenses.  You may not
reverse-assemble, reverse-compile, or otherwise reverse-engineer any
SOFTWARE provided in binary or machine readable form.  You may not
distribute the SOFTWARE to your customers without written permission
from CORTINA.

OWNERSHIP OF SOFTWARE AND COPYRIGHTS. All title and copyrights in and the
SOFTWARE and any accompanying printed materials, and copies of the SOFTWARE,
are owned by CORTINA. The SOFTWARE protected by the copyright laws of the
United States and other countries, and international treaty provisions.
You may not remove any copyright notices from the SOFTWARE.  Except as
otherwise expressly provided, CORTINA grants no express or implied right
under CORTINA patents, copyrights, trademarks, or other intellectual
property rights.

DISCLAIMER OF WARRANTIES. THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY
EXPRESS OR IMPLIED WARRANTY OF ANY KIND, INCLUDING ANY IMPLIED WARRANTIES
OF MERCHANTABILITY, NONINFRINGEMENT, OR FITNESS FOR A PARTICULAR PURPOSE,
TITLE, AND NON-INFRINGEMENT.  CORTINA does not warrant or assume
responsibility for the accuracy or completeness of any information, text,
graphics, links or other items contained within the SOFTWARE.  Without
limiting the foregoing, you are solely responsible for determining and
verifying that the SOFTWARE that you obtain and install is the appropriate
version for your purpose.

LIMITATION OF LIABILITY. IN NO EVENT SHALL CORTINA OR ITS SUPPLIERS BE
LIABLE FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT LIMITATION, LOST
PROFITS, BUSINESS INTERRUPTION, OR LOST INFORMATION) OR ANY LOSS ARISING OUT
OF THE USE OF OR INABILITY TO USE OF OR INABILITY TO USE THE SOFTWARE, EVEN
IF CORTINA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
TERMINATION OF THIS LICENSE. This LICENSE will automatically terminate if
You fail to comply with any of the terms and conditions hereof. Upon
termination, You will immediately cease use of the SOFTWARE and destroy all
copies of the SOFTWARE or return all copies of the SOFTWARE in your control
to CORTINA.  IF you commence or participate in any legal proceeding against
CORTINA, then CORTINA may, in its sole discretion, suspend or terminate all
license grants and any other rights provided under this LICENSE during the
pendency of such legal proceedings.
APPLICABLE LAWS. Claims arising under this LICENSE shall be governed by the
laws of the State of California, excluding its principles of conflict of
laws.  The United Nations Convention on Contracts for the International Sale
of Goods is specifically disclaimed.  You shall not export the SOFTWARE
without first obtaining any required export license or other approval from
the applicable governmental entity, if required.  This is the entire
agreement and understanding between You and CORTINA relating to this subject
matter.
GOVERNMENT RESTRICTED RIGHTS. The SOFTWARE is provided with "RESTRICTED
RIGHTS." Use, duplication, or disclosure by the Government is subject to
restrictions as set forth in FAR52.227-14 and DFAR252.227-7013 et seq. or
its successor. Use of the SOFTWARE by the Government constitutes
acknowledgment of CORTINA's proprietary rights therein. Contractor or
Manufacturer is CORTINA.

Copyright (c) 2009 by Cortina Systems Incorporated
****************************************************************************/
/*
 * $Id: sdl_vlan.c,v 1.1.2.59.2.2 2012/05/17 05:58:12 ytang Exp $
 */

#include "plat_common.h" 
#include "sdl_vlan_util.h"
#include "sdl_vlan.h"
#include "sdl_fdb.h"
#include "sdl_util.h"
#include "sdl.h"

#include "log.h"

#include "msApiTypes.h"
#include "msApi.h"

#include "gtDrvSwRegs.h"

#include "MARVELL_BSP_expo.h"
#include "switch_expo.h"
#include "switch_drv.h"
#include "sdl_mc.h"

#define __MC_VLAN_PER_PORT_MAX          8

#define __MC_VLAN_SWAP_PER_PORT_MAX     8



#define __VLAN_MAX                      (8*UNI_PORT_MAX)

#define __SVID_RSVD_TRANSPARENT         0

#define __INVALID_INDEX                 0xffff

#define __DEFAULT_FID                   0

#define __DS_TRANSPARENT_SVID_INDEX     0
#define __US_TRANSPARENT_EVID_INDEX     31
#define __US_TRANSPARENT_EVID           4096
#define __US_TRANSPARENT_ACL_ID         RTK_ACL_VLAN_START
int FALG_DORB = 1;
#define __NULL_CHECK(pointer)                                       \
    do{                                                             \
    if((pointer) == NULL)                                           \
        return CS_E_PARAM;                                          \
    }while(0)

#define __VLAN_MODE_CHECK(mode)                                     \
    do{                                                             \
    if((mode) >= SDL_VLAN_MODE_END)                                 \
        return CS_E_PARAM;                                          \
    }while(0)


typedef struct
{
    cs_boolean          valid;      /* rule entry valid indication */
    cs_sdl_vlan_cfg_t   rule;       /* rule entry info */
}__vlan_rule_t;

typedef struct
{
    cs_boolean  valid;      /* MC VLAN entry valid indication */
    cs_uint16   vid;        /* MC VLAN ID */
}__mc_vlan_t;


#if 0
typedef struct
{
    cs_boolean      valid;      /* VLAN entry valid indication */
    cs_sdl_vlan_t   vlan;       /* VLAN info */
}__vlan_t;
#endif
 __vlan_t __s_vlan_table[__VLAN_MAX];
 __vlan_t __c_vlan_table[__VLAN_MAX];
typedef struct
{
    cs_sdl_vlan_tag_t       def_vlan;                           /* port vlan id */
    cs_sdl_vlan_mode_t      vlan_mode;                          /* port vlan mode */
    __vlan_rule_t           vlan_rule[__VLAN_RULE_PER_PORT_MAX];         /* vlan rule table */
    cs_boolean              mc_vlan_en;                         /* mc vlan configured */
    __mc_vlan_t             mc_vlan[__MC_VLAN_PER_PORT_MAX];    /* mc vlan table */
    cs_sdl_mc_vlan_act_t    mc_act;                             /* mc tag action */
    __vlan_rule_t           mc_swap[__MC_VLAN_SWAP_PER_PORT_MAX];        /* mc vlan swap table */
}__port_vlan_info_t;

typedef enum
{
    __RULE_LOOKUP_ALL   = 0,
    __RULE_LOOKUP_CVLAN = 1,
    __RULE_LOOKUP_SVLAN = 2
}__rule_mode_t;

typedef enum
{
ACCEPT_FRAME_TYPE_ALL = 0,             /* untagged, priority-tagged and tagged */
ACCEPT_FRAME_TYPE_TAG_ONLY,         /* tagged */
ACCEPT_FRAME_TYPE_UNTAG_ONLY,     /* untagged and priority-tagged */
ACCEPT_FRAME_TYPE_END
}__rule_tag_filter_t;


static __port_vlan_info_t __port_vlan_table[UNI_PORT_MAX + 1];


static cs_boolean __vlan_table_lookup
(
    __vlan_t            *vlan_table, 
    cs_uint16           vid, 
    cs_uint16           *index          /* if false, index==first empty entry; if true, index==vid entry */
)
{
    int i;
    cs_boolean empty_found = EPON_FALSE;

    for(i = 0; i < __VLAN_MAX; ++i)
    {
        if( (!vlan_table[i].valid) && (!empty_found) )
        {
            *index = i;
            empty_found = EPON_TRUE;
            continue;
        }
    
        if( (vlan_table[i].valid) && (vid == vlan_table[i].vlan.vid) )
        {   
            *index = i;
            return EPON_TRUE;
        }
    }

    if(!empty_found) /* table is full */
    {
        *index = __INVALID_INDEX;
    }

    return EPON_FALSE;
}


static cs_status __vlan_table_add_port(__vlan_t *vlan_table, cs_uint16 vid, cs_port_id_t port, cs_boolean utg_en)
{
    cs_uint16 index;

    __vlan_table_lookup(vlan_table, vid, &index);

    if(__INVALID_INDEX == index)
    {
        return CS_E_RESOURCE;
    }

    vlan_table[index].vlan.vid = vid;
    vlan_table[index].vlan.mbr |= (1<<L2P_PORT(port));

    if(utg_en)
    {
        vlan_table[index].vlan.utg |= (1<<L2P_PORT(port));
    }
    else
    {
        vlan_table[index].vlan.utg &= ~(1<<L2P_PORT(port));
    }
    
    vlan_table[index].valid = EPON_TRUE;

    return CS_E_OK;
}

static void __vlan_table_del_port(__vlan_t *vlan_table, cs_port_id_t port)
{
    int i;
    GT_LPORT lport = L2P_PORT(port);

    for(i = 0; i < __VLAN_MAX; ++i)
    {
        if(vlan_table[i].valid)
        {
            vlan_table[i].vlan.mbr &= ~(1<<lport);
            vlan_table[i].vlan.utg &= ~(1<<lport);
            if(0 == (vlan_table[i].vlan.mbr & ((1<<UNI_PORT_MAX)-1)))  /* no UNI port in this vlan */
            {
            	InternalVlanDestroy(vlan_table[i].vlan.vid);
                vlan_table[i].valid = EPON_FALSE;
            }
        }
    }
}

static cs_boolean __rule_table_lookup(__vlan_rule_t *rule_table, __rule_mode_t mode, cs_sdl_vlan_cfg_t *rule, cs_uint16 *index)
{
    int i = 0;
    cs_boolean empty_found = EPON_FALSE;

    for(i = 0; i < __VLAN_RULE_PER_PORT_MAX; ++i)
    {
        /* search the first empty entry */
        if( (!rule_table[i].valid) && (!empty_found) )
        {
            *index = i;
            empty_found = EPON_TRUE;
            continue;
        }
    
        if(rule_table[i].valid)
        {
            if( (__RULE_LOOKUP_ALL == mode) &&
                (rule_table[i].rule.c_vlan.vid == rule->c_vlan.vid) &&
                (rule_table[i].rule.s_vlan.vid == rule->s_vlan.vid) ) 
            {
                *index = i;
                return EPON_TRUE;
            }
            
            if( (__RULE_LOOKUP_CVLAN == mode) &&
                (rule_table[i].rule.c_vlan.vid == rule->c_vlan.vid) )
            {
                *index = i;
                return EPON_TRUE;
            }

            if( (__RULE_LOOKUP_SVLAN == mode) &&
                (rule_table[i].rule.s_vlan.vid == rule->s_vlan.vid) )
            {
                *index = i;
                return EPON_TRUE;
            }
        }
    }

    if(!empty_found) /* table is full */
    {
        *index = __INVALID_INDEX;
    }

    return EPON_FALSE;
}

static cs_status __rule_table_add(__vlan_rule_t *rule_table, __rule_mode_t mode, cs_sdl_vlan_cfg_t *rule)
{
    cs_uint16 index = 0;
    cs_boolean rule_found;

    rule_found = __rule_table_lookup(rule_table, mode, rule, &index);

    if(__INVALID_INDEX == index)
    {
        return CS_E_RESOURCE;
    }

    if(!rule_found)
    {
        rule_table[index].rule.s_vlan.vid = rule->s_vlan.vid;
        rule_table[index].rule.c_vlan.vid = rule->c_vlan.vid;
        rule_table[index].valid = EPON_TRUE;
    }
    
    return CS_E_OK;
}


static void __rule_table_clr(__vlan_rule_t *rule_table)
{
    int i;
	memset(rule_table,0,sizeof(__vlan_rule_t));
    for(i = 0; i < __VLAN_RULE_PER_PORT_MAX; ++i)    
    {
        rule_table[i].valid = EPON_FALSE;
    }
}


static cs_boolean __mc_table_lookup(__mc_vlan_t *mc_vlan_table, cs_uint16 vid, cs_uint16 *index)
{
    int i;
    cs_boolean empty_found = EPON_FALSE;

    for(i = 0; i < __MC_VLAN_PER_PORT_MAX; ++i)
    {
        if( (!mc_vlan_table[i].valid) && (!empty_found) )
        {
            *index = i;
            empty_found = EPON_TRUE;
            continue;
        }
    
        if( (mc_vlan_table[i].valid) && (vid == mc_vlan_table[i].vid) )
        {   
            *index = i;
            return EPON_TRUE;
        }
    }

    if(!empty_found) /* table is full */
    {
        *index = __INVALID_INDEX;
    }

    return EPON_FALSE;
}


static cs_status __mc_table_add(__mc_vlan_t *mc_vlan_table, cs_uint16 vid)
{
    cs_uint16 index = 0;

    if(!__mc_table_lookup(mc_vlan_table, vid, &index))
    {
        if(__INVALID_INDEX == index)
        {
            return CS_E_RESOURCE;
        }

        mc_vlan_table[index].vid = vid; 
        mc_vlan_table[index].valid = EPON_TRUE; 
    }

    return CS_E_OK;
}

static cs_status __mc_table_del(__mc_vlan_t *mc_vlan_table, cs_uint16 vid)
{
    cs_uint16 index = 0;

    if(__mc_table_lookup(mc_vlan_table, vid, &index))
    {
        mc_vlan_table[index].valid = EPON_FALSE; 
    }

    return CS_E_OK;
}

#if 0
/* Transparent port should add all others port's vlan */
static cs_uint16 __get_transparent_mbp(void)
{
    int i;
    cs_uint16 mbp = 0;

    for(i = CS_UNI_PORT_ID1; i <= CS_UNI_PORT_ID4; ++i)
    {
        if(SDL_VLAN_MODE_TRANSPARENT == __port_vlan_table[i].vlan_mode)
        {
            mbp |= (1<<(i-1));
        }
    }

    return mbp;
}


/* get the bitmap of ports that still use current MC VLAN */
static cs_uint16 __get_mc_vlan_port_mbp(cs_uint16 s_vid)
{
    cs_port_id_t port_id;
    cs_sdl_vlan_cfg_t vlan_rule;
    cs_uint16 index;
    cs_uint16 mbp = 0;
    
    for(port_id = CS_UNI_PORT_ID1; port_id <= CS_UNI_PORT_ID4; port_id++)
    {
        memset(&vlan_rule, 0, sizeof(cs_sdl_vlan_cfg_t));
        vlan_rule.s_vlan.vid = s_vid;
        
        if(__mc_table_lookup(__port_vlan_table[port_id].mc_vlan, s_vid, &index) || 
          __rule_table_lookup(__port_vlan_table[port_id].mc_swap, __RULE_LOOKUP_SVLAN, &vlan_rule, &index))
        {
            mbp |= (1 << L2P_PORT(port_id));
        }
    }
    
    return mbp;
}
#endif

static void __transparent_us_set(GT_LPORT lport, cs_boolean en)
{
	GT_U32 unit, port;

	gt_getswitchunitbylport(lport, &unit, &port);
	gvlnSetPortVlanDot1qMode(QD_DEV_PTR, port, en?GT_DISABLE:GT_SECURE);
}

#if 0
static void __transparent_hw_set(GT_LPORT lport, cs_boolean tran_en, rtk_vlan_acceptFrameType_t accept_frame_type)
{
    rtl8367b_svlan_memconf_t svlan_memconf;
	//GT_LPORT new_port = P2L_PORT(lport);
  //  __port_vlan_info_t *port_vlan = &__port_vlan_table[new_port];
    memset(&svlan_memconf, 0, sizeof(rtl8367b_svlan_memconf_t));
    if(tran_en)
    {
        rtk_vlan_portIgrFilterEnable_set(lport, EPON_FALSE);
        rtk_vlan_portAcceptFrameType_set(lport, ACCEPT_FRAME_TYPE_ALL);
        __transparent_us_set(lport, EPON_TRUE);

        /* Reserve entry 0 for downstream transparent */
        rtl8367b_getAsicSvlanMemberConfiguration(__DS_TRANSPARENT_SVID_INDEX, &svlan_memconf);
        svlan_memconf.vs_svid = __SVID_RSVD_TRANSPARENT;
        svlan_memconf.vs_member |= (1<<lport);
        svlan_memconf.vs_member |= (1<<SWITCH_UPLINK_PORT);
        svlan_memconf.vs_untag  &= ~(1<<lport);
        svlan_memconf.vs_untag  |= (1<<SWITCH_UPLINK_PORT);
        rtl8367b_setAsicSvlanMemberConfiguration(0, &svlan_memconf);
        
        rtk_svlan_untag_action_set(UNTAG_ASSIGN, __SVID_RSVD_TRANSPARENT);
        rtk_svlan_unmatch_action_set(UNMATCH_ASSIGN, __SVID_RSVD_TRANSPARENT);
    }
    else
    {
        /* non-transparent configuration */
       rtk_vlan_portIgrFilterEnable_set(lport, EPON_TRUE);
        rtk_vlan_portAcceptFrameType_set(lport, accept_frame_type);

        __transparent_us_set(lport, EPON_FALSE);

        rtl8367b_getAsicSvlanMemberConfiguration(__DS_TRANSPARENT_SVID_INDEX, &svlan_memconf);
        svlan_memconf.vs_svid = __SVID_RSVD_TRANSPARENT;
        svlan_memconf.vs_member &= ~(1<<lport);
        if(0 == (svlan_memconf.vs_member & 0xf)) /* remove uplink port when no UNI port in the map*/
        {
        	svlan_memconf.vs_member = 0;
            svlan_memconf.vs_untag  = 0;
            rtk_svlan_untag_action_set(UNTAG_DROP, __SVID_RSVD_TRANSPARENT);
            rtk_svlan_unmatch_action_set(UNMATCH_DROP, __SVID_RSVD_TRANSPARENT);
        }
        rtl8367b_setAsicSvlanMemberConfiguration(0, &svlan_memconf);
    }
}

#else

#define __SVID_RSVD_UNTAG	1

static void __transparent_hw_set(GT_LPORT lport, cs_boolean tran_en, __rule_tag_filter_t accept_frame_type)
{
	GT_U32 unit, port;

	gt_getswitchunitbylport(lport, &unit, &port);

	if(tran_en)		//͸���˿ڵ�����
	{
		
//		rtk_vlan_portIgrFilterEnable_set(lport, EPON_FALSE);				//�رս���˿ڵİ�����vlan ���Ĺ���
//		rtk_vlan_portAcceptFrameType_set(lport, ACCEPT_FRAME_TYPE_ALL);	//�˿ڽ������еİ�
		__transparent_us_set(lport, EPON_TRUE);							//�����ǽ�ָ���Ķ˿ڼ��뵽͸���˿��б�

		/*vlan 0 ������*/
//		unmatch_svlan_memconf.vs_member |= (1<<lport);
	
		/*vlan 1 ������*/
//		untag_svlan_memconf.vs_member |= (1<<lport);
	}
	else			//��͸���˿ڵ�����
	{
//		rtk_vlan_portIgrFilterEnable_set(lport, EPON_TRUE);			//�򿪽���˿ڵİ�����vlan ���Ĺ���
//		rtk_vlan_portAcceptFrameType_set(lport, accept_frame_type);	//�˿ڽ���ָ���İ�
		__transparent_us_set(lport, EPON_FALSE);						//�����ǽ�ָ���Ķ˿ڴ�͸���˿��б����Ƴ�

		/*vlan 0 ������*/	
//		unmatch_svlan_memconf.vs_member &= ~(1<<lport);

		/*vlan 1 ������*/
//		GT_LPORT new_port = P2L_PORT(lport);
//		__port_vlan_info_t *port_vlan = &__port_vlan_table[new_port];
//		if(1 == port_vlan->def_vlan.vid)
//		{
//			untag_svlan_memconf.vs_member |= (1<<lport);
//		}
//		else
//		{
//			untag_svlan_memconf.vs_member &= ~(1<<lport);
//		}
	}	

	if(accept_frame_type == ACCEPT_FRAME_TYPE_ALL)
	{
		gprtSetDiscardTagged(QD_DEV_PTR, port, GT_FALSE);
		gprtSetDiscardUntagged(QD_DEV_PTR, port, GT_FALSE);
	}

	if(accept_frame_type == ACCEPT_FRAME_TYPE_TAG_ONLY)
		gprtSetDiscardUntagged(QD_DEV_PTR, port, GT_TRUE);

	if(accept_frame_type == ACCEPT_FRAME_TYPE_UNTAG_ONLY)
		gprtSetDiscardTagged(QD_DEV_PTR, port, GT_TRUE);

//	rtl8367b_setAsicSvlanMemberConfiguration(__DS_TRANSPARENT_SVID_INDEX, &unmatch_svlan_memconf);
//	rtl8367b_setAsicSvlanMemberConfiguration(__SVID_RSVD_UNTAG, &untag_svlan_memconf);
}

#endif

static void __remove_port_from_svlan(cs_uint16 s_vid, GT_LPORT lport)
{
	return;
}

static void __remove_port_from_cvlan(cs_uint16 c_vid, GT_LPORT lport)
{    

	GT_U32 unit, port;

	if(gt_getswitchunitbylport(lport, &unit, &port) != GT_OK)
		return;

	InternalVlanLeave(QD_DEV_PTR, c_vid, port);

}

static void __clear_hw_mc_swap(cs_port_id_t port_id)
{
    int i;
    GT_LPORT lport = L2P_PORT(port_id);

    __port_vlan_info_t *port_vlan = &__port_vlan_table[port_id];

    for(i = 0; i < __MC_VLAN_SWAP_PER_PORT_MAX; ++i)
    {
        if(port_vlan->mc_swap[i].valid)
        {
            //rtk_svlan_c2s_del(port_vlan->mc_swap[i].rule.c_vlan.vid, lport);
//            rtk_svlan_sp2c_del(port_vlan->mc_swap[i].rule.s_vlan.vid, lport);
            __remove_port_from_svlan(port_vlan->mc_swap[i].rule.s_vlan.vid, lport);
            __remove_port_from_cvlan(port_vlan->mc_swap[i].rule.c_vlan.vid, lport);
        }
    }
}

static void __clear_hw_mc_vlan(cs_port_id_t port_id)
{
    int i;
    GT_LPORT lport = L2P_PORT(port_id);

    __port_vlan_info_t *port_vlan = &__port_vlan_table[port_id];

    for(i = 0; i < __MC_VLAN_PER_PORT_MAX; ++i)
    {
        if(port_vlan->mc_vlan[i].valid)
        {
            __remove_port_from_svlan(port_vlan->mc_vlan[i].vid, lport);
        }
    }
}
void __rule_table_clr_w(__mc_vlan_t*mc_vlan)
{
	int i =0;
	for(i = 0; i < __VLAN_RULE_PER_PORT_MAX; ++i)    
    {
        mc_vlan[i].valid = EPON_FALSE;
    }
}
static void __clear_uc(cs_port_id_t port_id)
{
    int i;
    GT_LPORT lport = L2P_PORT(port_id);

    __port_vlan_info_t *port_vlan = &__port_vlan_table[port_id];

    /* clear HW rule table */
    /* rule entry MUST be delete BEFORE vlan entry */
    for(i = 0; i < __VLAN_RULE_PER_PORT_MAX; ++i)
    {
        if(port_vlan->vlan_rule[i].valid)
        {        	
//            rtk_svlan_c2s_del(port_vlan->vlan_rule[i].rule.c_vlan.vid, lport);

//            rtk_svlan_sp2c_del(port_vlan->vlan_rule[i].rule.s_vlan.vid, lport);
        }
    }
//	rtk_svlan_c2s_del(0, lport);
	#if 0
	if(rtk_svlan_c2s_del(0, lport) == RT_ERR_OK)
		cs_printf("delete vlan 0 flag ok!\r\n");
	else
		cs_printf("delete vlan 0 flag fail!\r\n");
	#endif

    /* clear vlan table for this port */
    for(i = 0; i < __VLAN_MAX; ++i)
    {
        if(__s_vlan_table[i].valid)
        {
            __remove_port_from_svlan(__s_vlan_table[i].vlan.vid, lport);
        }

        if(__c_vlan_table[i].valid)
        {
        	cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_VLAN, "%s vlanid=%d lport=%d\n", __func__, __c_vlan_table[i].vlan.vid, port_id);
            __remove_port_from_cvlan(__c_vlan_table[i].vlan.vid, lport);
        }
    }

    /* clear vlan table and rule table for this port */
    __vlan_table_del_port(__s_vlan_table, port_id);
    __vlan_table_del_port(__c_vlan_table, port_id);
    __rule_table_clr(__port_vlan_table[port_id].vlan_rule);
	__rule_table_clr_w(__port_vlan_table[port_id].mc_vlan);

}
unsigned int traverse_vlan_current_vlan_num(unsigned int i)
{
	unsigned int ret = -1;
	if(i >= __VLAN_MAX)
		return ret;
	if(__s_vlan_table[i].valid)
	{
		ret = __s_vlan_table[i].vlan.vid;
	}
	return ret;
}
void __get_port_by_vlan(unsigned int vid)
{
	cs_sdl_mc_l2_entry_t entry_normal;
	unsigned char lport = 0;
	cs_callback_context_t     context;
    int i;

    entry_normal.mac.addr[0] = 0x01;
    entry_normal.mac.addr[1] = 0x00;
    entry_normal.mac.addr[2] = 0x5e;
    entry_normal.mac.addr[3] = 0x00;
    entry_normal.mac.addr[4] = 0x00;
    entry_normal.mac.addr[5] = 0x01;
//    cs_printf("add mc vlan port =================\n");
	for(i = 0; i < __VLAN_MAX; ++i)
	{
		if(__s_vlan_table[i].valid)
		{
			for(lport = 0;lport<__MC_VLAN_SWAP_PER_PORT_MAX; lport++)
			{
				if(__s_vlan_table[i].vlan.mbr & (1<<lport))
				{
					entry_normal.vlan = __s_vlan_table[i].vlan.vid;
//					        ret = epon_request_onu_mc_l2_port_get(context, 0, 0, &entry_normal, &portmap);
//					        if(ret != CS_E_NOT_FOUND) {
//					            if( ret || (portmap.bits[0] & (1<<(fdb->portid-1)))) {
//					                return CS_E_ERROR;
//					            }
//					        }
					epon_request_onu_mc_l2_entry_add(context, 0, 0, lport+1, &entry_normal);
				}
			}
		}
	}
}

static void __update_hw_mc(cs_port_id_t port_id)
{
    int i;
    __port_vlan_info_t *port_vlan = &__port_vlan_table[port_id];
    cs_sdl_vlan_cfg_t vlan_rule;
    cs_uint16 index;

    /* delete overlap vlan rule */
    if(port_vlan->mc_vlan_en)
    {
        if(SDL_VLAN_MODE_TRANSLATION == port_vlan->vlan_mode)
        {
            /* delete overlaped vlan from vlan rule table*/
            for(i = 0; i < __MC_VLAN_PER_PORT_MAX; ++i)
            {
                if(port_vlan->mc_vlan[i].valid)
                {
                    vlan_rule.s_vlan.vid = port_vlan->mc_vlan[i].vid; /* key */
                    if(__rule_table_lookup(port_vlan->vlan_rule, __RULE_LOOKUP_SVLAN, &vlan_rule, &index))
                    {
//                        rtk_svlan_sp2c_del(port_vlan->mc_vlan[i].vid, L2P_PORT(port_id));
//                    	mtodo: ���vlanת���������
                    }
                }
            }
        }

        if(SDL_VLAN_MODE_AGGREGATION == port_vlan->vlan_mode)
        {
            for(i = 0; i < __MC_VLAN_PER_PORT_MAX; ++i)
            {
                if(port_vlan->mc_vlan[i].valid)
                {
                    vlan_rule.s_vlan.vid = port_vlan->mc_vlan[i].vid; /* key */
                    if(__rule_table_lookup(port_vlan->vlan_rule, __RULE_LOOKUP_SVLAN, &vlan_rule, &index))
                    {
//                        rtk_svlan_dmac_vidsel_set(lport, EPON_FALSE);  /* disable d/s 1:N */
//                    	mtodo: vlan aggegation�ݲ�֧��
                        break;
                    }
                }
            }

            for(i = 0; i < __MC_VLAN_SWAP_PER_PORT_MAX; ++i)
            {
                if(port_vlan->mc_swap[i].valid)
                {
                    vlan_rule.s_vlan.vid = port_vlan->mc_swap[i].rule.s_vlan.vid; /* key */
                    if(__rule_table_lookup(port_vlan->vlan_rule, __RULE_LOOKUP_SVLAN, &vlan_rule, &index))
                    {
//                        rtk_svlan_dmac_vidsel_set(lport, EPON_FALSE);  /* disable d/s 1:N */
//                    	mtodo: vlan aggegation�ݲ�֧��
                        break;
                    }
                }
            }
        }
#if 0
        if(SDL_VLAN_MODE_TRUNK == port_vlan->vlan_mode)
        {
        	cs_sdl_mc_l2_entry_t entry_normal;
        	unsigned char lport = 0;
        	cs_callback_context_t     context;

            entry_normal.mac.addr[0] = 0x01;
            entry_normal.mac.addr[1] = 0x00;
            entry_normal.mac.addr[2] = 0x5e;
            entry_normal.mac.addr[3] = 0x00;
            entry_normal.mac.addr[4] = 0x00;
            entry_normal.mac.addr[5] = 0x01;
            cs_printf("add mc vlan port =================\n");
			for(i = 0; i < __VLAN_MAX; ++i)
			{
				if(__s_vlan_table[i].valid)
				{
					for(lport = 0;lport<__MC_VLAN_SWAP_PER_PORT_MAX; lport++)
					{
						if(__s_vlan_table[i].vlan.mbr & (1<<lport))
						{
							entry_normal.vlan = __s_vlan_table[i].vlan.vid;
//					        ret = epon_request_onu_mc_l2_port_get(context, 0, 0, &entry_normal, &portmap);
//					        if(ret != CS_E_NOT_FOUND) {
//					            if( ret || (portmap.bits[0] & (1<<(fdb->portid-1)))) {
//					                return CS_E_ERROR;
//					            }
//					        }
					        cs_printf("portmap is %d\n",lport+1);
					        cs_printf("vlan is %x\n",entry_normal.vlan);
					        /*down query pkt should be sent in port default vlan and add query pkt with pvid*/
							epon_request_onu_mc_l2_entry_add(context, 0, 0, lport+1, &entry_normal);
						}
					}
				}
			}
        }
#endif
    }
    /* END: delete overlap vlan rule */

    if(SDL_MC_VLAN_TAG_KEEP == port_vlan->mc_act)           
    {
        for(i = 0; i < __MC_VLAN_PER_PORT_MAX; ++i)
        {
            if(port_vlan->mc_vlan[i].valid)
            {
                /* svlan member */
//            	mtodo: �ಥ����vlan��ǩ

            }
        }
    }
    else if(SDL_MC_VLAN_TAG_STRIP == port_vlan->mc_act)     
    {
        for(i = 0; i < __MC_VLAN_PER_PORT_MAX; ++i)
        {
            if(port_vlan->mc_vlan[i].valid)
            {
                /* svlan member */
//				mtodo:�ಥ����vlan��ǩ
            }
        }
    }
    else if(SDL_MC_VLAN_TAG_SWAP == port_vlan->mc_act)      
    {
        /* change previous mc vlan to KEEP mode */
        for(i = 0; i < __MC_VLAN_PER_PORT_MAX; ++i)
        {
            if(port_vlan->mc_vlan[i].valid)
            {
                /* svlan member */
//            	mtodo: ��vlan�鲥
            }
        }

        for(i = 0; i < __MC_VLAN_SWAP_PER_PORT_MAX; ++i)
        {
            if(port_vlan->mc_swap[i].valid)
            {
                /* svlan member */
            	//            	mtodo: ��vlan�鲥
            }
        }
    }
    else
    {
        SDL_MIN_LOG("\nNot supported MC VLAN ACTION: %d\n", port_vlan->mc_act);
    }
}
/***************************************************************************************
 * Function Name:
 *      __update_hw_uc
 * Description:
 * 		�����Ĵ���
 * Input:
 *      port_id of onu
 * Output:
 *      None
 * Return:
 *      void
 * Note:
 *      ��ONU ��ĳ��������ĳһ�����ж˿�ģʽ������(ģʽ: trunk , �ۺ�ģʽ
 *	  ͸��ģʽ��ת��ģʽ�����ģʽ)�������ģʽ����Ӧ�ô������
 *	  �汾�Ĭ�ϳ�ʼ��ģʽΪtrunkģʽ��
 *
****************************************************************************************/
static void __update_hw_uc(cs_port_id_t port_id)
{
    int i;
    GT_LPORT lport = L2P_PORT(port_id);
    __port_vlan_info_t *port_vlan = &__port_vlan_table[port_id];
    cs_uint16 index;
    cs_callback_context_t   context;

    GT_U32 unit, port;

    gt_getswitchunitbylport(lport, &unit, &port);


    /* delete mc-swap rule if it overlap with uc-svlan */
    if(!port_vlan->mc_vlan_en)
    {
        for(i = 0; i < __MC_VLAN_SWAP_PER_PORT_MAX; ++i)
        {
            if(port_vlan->mc_swap[i].valid)
            {
                if(__vlan_table_lookup(__s_vlan_table, port_vlan->mc_swap[i].rule.s_vlan.vid, &index))
                {
                    if(__s_vlan_table[index].vlan.mbr & (1<<lport))
                    {
//                        rtk_svlan_sp2c_del(port_vlan->mc_swap[i].rule.s_vlan.vid, lport);
//                    	mtodo:ɾ���ಥת���뵥��vlan�غϵ�����
                    }
                }
            }
        }
    }
    /* END: delete mc-swap rule if it overlap with uc-svlan */

    /* sync sw S-vlan table to HW (including default vlan) */
    for(i = 0; i < __VLAN_MAX; ++i)
    {
        if(__s_vlan_table[i].valid)
        {
//        	mtodo: ͬ��s-vlan���switch
        }
    }

    if(SDL_VLAN_MODE_TRANSPARENT != port_vlan->vlan_mode)
    {
        /* sync sw C-vlan table to HW (including default vlan) */
        for(i = 0; i < __VLAN_MAX; ++i)
        {
            if( __c_vlan_table[i].valid )
            {
            	GT_U32 j = 0;

    			GT_VTU_ENTRY entry;
    			memset(&entry, 0, sizeof(entry));

    			cs_log_msg(IROS_LOG_LEVEL_DBG3, IROS_MID_VLAN, "%s port(%d) vlanid=%d mbr= 0x%x utg=0x%x\n", __func__, port_id, __c_vlan_table[i].vlan.vid,
    					__c_vlan_table[i].vlan.mbr, __c_vlan_table[i].vlan.utg);


            	FOR_UNIT_START(GT_U32, unit)
            	InternalVlanInit(QD_DEV_PTR, __c_vlan_table[i].vlan.vid);
            	FOR_UNIT_END

            	for(j=0; j<sizeof(__c_vlan_table[i].vlan.mbr)*8;j++)
            	{
            		GT_U32 unit, port;
            		gt_getswitchunitbylport(j, &unit, &port);

            		if(__c_vlan_table[i].vlan.mbr&(1<<j))
            		{
            			if(__c_vlan_table[i].vlan.utg&(1<<j))
            				InternalVlanJoin(QD_DEV_PTR, __c_vlan_table[i].vlan.vid, port, GT_FALSE);
            			else
            				InternalVlanJoin(QD_DEV_PTR, __c_vlan_table[i].vlan.vid, port, GT_TRUE);
            		}
            	}
            }
        }
    }

    switch(port_vlan->vlan_mode)
    {
        case SDL_VLAN_MODE_TRANSPARENT:
            __transparent_hw_set(lport, EPON_TRUE, ACCEPT_FRAME_TYPE_ALL);
            break;

        case SDL_VLAN_MODE_TAG:
            __transparent_hw_set(lport, EPON_FALSE, ACCEPT_FRAME_TYPE_UNTAG_ONLY);   /* ingress accept type */
            break;

        case SDL_VLAN_MODE_TRANSLATION:
            __transparent_hw_set(lport, EPON_FALSE, ACCEPT_FRAME_TYPE_ALL);
            for(i = 0; i < __VLAN_RULE_PER_PORT_MAX; ++i)
            {
                if(port_vlan->vlan_rule[i].valid)
                {
                    /* s2c */
//                    rtk_svlan_sp2c_add(port_vlan->vlan_rule[i].rule.s_vlan.vid, lport, port_vlan->vlan_rule[i].rule.c_vlan.vid);
                    /* c2s */
//                    rtk_svlan_c2s_add(port_vlan->vlan_rule[i].rule.c_vlan.vid, lport, port_vlan->vlan_rule[i].rule.s_vlan.vid);
//                	mtodo: vlanת���ĸ���
                }
            }
            break;

        case SDL_VLAN_MODE_AGGREGATION:
            __transparent_hw_set(lport, EPON_FALSE, ACCEPT_FRAME_TYPE_ALL);
//            rtk_svlan_dmac_vidsel_set(lport, EPON_TRUE); /* enable d/s 1:N */
            
            for(i = 0; i < __VLAN_RULE_PER_PORT_MAX; ++i)
            {
                if(port_vlan->vlan_rule[i].valid)
                {
                    /* c2s */
//                    rtk_svlan_c2s_add(port_vlan->vlan_rule[i].rule.c_vlan.vid, lport, port_vlan->vlan_rule[i].rule.s_vlan.vid);
//                	mtodo:vlan��۵ĸ���
                }
            }
            /* clear dynamic entry from FDB */
            epon_request_onu_fdb_entry_clr_per_port(context,0,0,port_id, SDL_FDB_CLR_DYNAMIC);
            break;

        case SDL_VLAN_MODE_TRUNK:
            __transparent_hw_set(lport, EPON_FALSE, ACCEPT_FRAME_TYPE_ALL);

            for(i = 0; i < __VLAN_RULE_PER_PORT_MAX; ++i)
            {
                if(port_vlan->vlan_rule[i].valid)
                {
                    /* c2s */
//                    rtk_svlan_c2s_add(port_vlan->vlan_rule[i].rule.c_vlan.vid, lport, port_vlan->vlan_rule[i].rule.c_vlan.vid);
					
					#if 0
//					rtk_svlan_c2s_add(0, lport, 0);
					#else
//					rtk_svlan_c2s_add(1, lport, 0);
					#endif
                }
            }

            break;

        case SDL_VLAN_MODE_STACKING:
            __transparent_hw_set(lport, EPON_FALSE, ACCEPT_FRAME_TYPE_ALL);   /* ingress accept type */
            /* no vlan check and always add s-tag for upstream */
//            rtk_vlan_portIgrFilterEnable_set(lport, EPON_FALSE);
//            mtodo:vlan stack����
            __transparent_us_set(lport, EPON_TRUE);
            break;

        default: break;
    }
//    rtk_svlan_defaultSvlan_set(lport, port_vlan->def_vlan.vid); /* default vlan should be set after member set */
    

	gvlnSetPortVid(QD_DEV_PTR, port, port_vlan->def_vlan.vid);
	gvlnSetPortVlanDBNum(QD_DEV_PTR, port, port_vlan->def_vlan.vid);

}
//extern unsigned char mc_ctl_enable_flag;
static void __update_sw
(
    CS_IN cs_port_id_t                  port_id,
    CS_IN cs_sdl_vlan_tag_t             def_vlan,   /* ignored for TRANSPARENT mode */
    CS_IN cs_sdl_vlan_mode_t            vlan_mode,
    CS_IN cs_sdl_vlan_cfg_t             *vlan_cfg,  /* ignored for TRANSPARENT mode */
    CS_IN cs_uint16                     cfg_nums    /* can be zero for modes */
)
{
    int i;
    /* common for all modes */
    __port_vlan_table[port_id].vlan_mode = vlan_mode;
    __port_vlan_table[port_id].def_vlan.vid = def_vlan.vid;
    __port_vlan_table[port_id].def_vlan.pri = def_vlan.pri;
#if 0
    __port_vlan_table[port_id].mc_vlan_en = EPON_FALSE;
#else
    __port_vlan_table[port_id].mc_vlan_en = EPON_TRUE;
#endif
	__port_vlan_table[port_id].mc_act = SDL_MC_VLAN_TAG_KEEP;
    if(SDL_VLAN_MODE_TRANSPARENT != vlan_mode)
    {
        __vlan_table_add_port(__s_vlan_table, def_vlan.vid, port_id, EPON_TRUE);    /* default vlan */
        __vlan_table_add_port(__s_vlan_table, def_vlan.vid, CS_UPLINK_PORT, EPON_FALSE);
		__port_vlan_table[port_id].mc_vlan[0].valid = EPON_TRUE;
		__port_vlan_table[port_id].mc_vlan[0].vid =def_vlan.vid;
    }

    if( (SDL_VLAN_MODE_TRANSLATION == vlan_mode) || (SDL_VLAN_MODE_AGGREGATION == vlan_mode) ) 
    {
        for(i=0; i<cfg_nums; ++i)
        {
        	
            __rule_table_add(__port_vlan_table[port_id].vlan_rule, __RULE_LOOKUP_ALL, &vlan_cfg[i]);
            __vlan_table_add_port(__s_vlan_table, vlan_cfg[i].s_vlan.vid, port_id, EPON_TRUE);
            __vlan_table_add_port(__s_vlan_table, vlan_cfg[i].s_vlan.vid, CS_UPLINK_PORT, EPON_FALSE);
            __vlan_table_add_port(__c_vlan_table, vlan_cfg[i].c_vlan.vid, port_id, EPON_FALSE);
            __vlan_table_add_port(__c_vlan_table, vlan_cfg[i].c_vlan.vid, CS_UPLINK_PORT, EPON_TRUE);
        }
    }
    else if( SDL_VLAN_MODE_TRUNK == vlan_mode )
    {  	
#if 0	/*modified by zhuxh*/
    	/*for gwd vlan mode, vlan 1 strip vlan tag*/
        __vlan_table_add_port(__c_vlan_table, def_vlan.vid, port_id, EPON_TRUE);
        __vlan_table_add_port(__c_vlan_table, def_vlan.vid, CS_UPLINK_PORT,  (def_vlan.vid == 1)?EPON_TRUE:EPON_FALSE);
#else
        /*for ctc vlan mode*/
        __vlan_table_add_port(__c_vlan_table, def_vlan.vid, port_id, EPON_TRUE);
        __vlan_table_add_port(__c_vlan_table, def_vlan.vid, CS_UPLINK_PORT, EPON_FALSE);
#endif

        for(i=0; i<cfg_nums; ++i)
        {
            __rule_table_add(__port_vlan_table[port_id].vlan_rule, __RULE_LOOKUP_CVLAN, &vlan_cfg[i]);
			__port_vlan_table[port_id].mc_vlan[i].valid = EPON_TRUE;
			__port_vlan_table[port_id].mc_vlan[i].vid = vlan_cfg[i].s_vlan.vid;
            __vlan_table_add_port(__s_vlan_table, vlan_cfg[i].c_vlan.vid, port_id, EPON_FALSE);
            __vlan_table_add_port(__s_vlan_table, vlan_cfg[i].c_vlan.vid, CS_UPLINK_PORT, EPON_FALSE);
			#if 1
            __vlan_table_add_port(__c_vlan_table, vlan_cfg[i].c_vlan.vid, port_id, EPON_FALSE);
            __vlan_table_add_port(__c_vlan_table, vlan_cfg[i].c_vlan.vid, CS_UPLINK_PORT, (vlan_cfg[i].c_vlan.vid == 1)?EPON_TRUE:EPON_FALSE);
			#endif
        }
    }
    else if( SDL_VLAN_MODE_TAG == vlan_mode )
    {
    	__vlan_table_add_port(__c_vlan_table, def_vlan.vid, port_id, EPON_TRUE);
    	__vlan_table_add_port(__c_vlan_table, def_vlan.vid, CS_UPLINK_PORT, EPON_FALSE);
    }
    else{/* no need update sw table in transparent/stacking/tag mode */}
}

static void __update_hw(cs_port_id_t port_id)
{
    __port_vlan_info_t *port_vlan = &__port_vlan_table[port_id];

    if(port_vlan->mc_vlan_en)
    {
        __update_hw_uc(port_id);
        __update_hw_mc(port_id);
    }
    else
    {
        __update_hw_mc(port_id);
        __update_hw_uc(port_id);
    }
}

void sdl_sw_vlan_dump(void)
{
}

cs_status epon_request_onu_vlan_set
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_int32                      device_id,
    CS_IN cs_int32                      llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_IN cs_sdl_vlan_tag_t             def_vlan,   /* ignored for TRANSPARENT mode*/
    CS_IN cs_sdl_vlan_mode_t            vlan_mode,
    CS_IN cs_sdl_vlan_cfg_t             *vlan_cfg,  /* ignored for TRANSPARENT mode */
    CS_IN cs_uint16                     cfg_nums    /* can be zero for modes */
)
{
    int i;
    UNI_PORT_CHECK(port_id);
    __VLAN_MODE_CHECK(vlan_mode);

    if( (SDL_VLAN_MODE_TRANSLATION == vlan_mode)
    || (SDL_VLAN_MODE_AGGREGATION == vlan_mode) 
    || (SDL_VLAN_MODE_TRUNK == vlan_mode) )
    {
    	#if 0
    	__port_vlan_table[port_id].vlan_mode = vlan_mode;		//ԭ���Ĵ���
        __NULL_CHECK(vlan_cfg);

        if(cfg_nums > __VLAN_RULE_PER_PORT_MAX)
        {
            return CS_E_PARAM;
        }
		#else
		if(cfg_nums > __VLAN_RULE_PER_PORT_MAX)
        {
            return CS_E_PARAM;
        }
		__NULL_CHECK(vlan_cfg);
		__port_vlan_table[port_id].vlan_mode = vlan_mode;
		#endif

        for(i = 0; i < cfg_nums; ++i)
        {
            VID_CHECK(vlan_cfg[i].s_vlan.vid);
            VID_CHECK(vlan_cfg[i].c_vlan.vid);
        }
    }

    __clear_uc(port_id);
	__update_sw(port_id, def_vlan, vlan_mode, vlan_cfg, cfg_nums);
	__update_hw(port_id);

    return CS_E_OK;
}


cs_status epon_request_onu_vlan_get
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_int32                   device_id,
    CS_IN cs_int32                     llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_OUT cs_sdl_vlan_tag_t            *def_vlan,  /* ignored for TRANSPARENT mode */
    CS_OUT cs_sdl_vlan_mode_t           *vlan_mode,
    CS_OUT cs_sdl_vlan_cfg_t            *vlan_cfg,   /* ignored for TRANSPARENT mode */
    CS_OUT cs_uint16                    *cfg_nums    /* zero for TRANSPARENT/TAGGED mode */
)
{
    int i;
    cs_uint16 vlan_cnt = 0;

    UNI_PORT_CHECK(port_id);
    __NULL_CHECK(def_vlan);
    __NULL_CHECK(vlan_mode);
    __NULL_CHECK(vlan_cfg);
    __NULL_CHECK(cfg_nums);
    
    *cfg_nums = 0;

    if( (SDL_VLAN_MODE_TRANSLATION == __port_vlan_table[port_id].vlan_mode) 
        || (SDL_VLAN_MODE_AGGREGATION == __port_vlan_table[port_id].vlan_mode) ) 
    {
        for(i = 0; i < __VLAN_RULE_PER_PORT_MAX; ++i)
        {
            if(__port_vlan_table[port_id].vlan_rule[i].valid)
            {
                vlan_cfg[vlan_cnt].s_vlan.vid = __port_vlan_table[port_id].vlan_rule[i].rule.s_vlan.vid;
                vlan_cfg[vlan_cnt].c_vlan.vid = __port_vlan_table[port_id].vlan_rule[i].rule.c_vlan.vid;
                ++vlan_cnt;
            }
        }
        *cfg_nums = vlan_cnt;
    }
    else if(SDL_VLAN_MODE_TRUNK == __port_vlan_table[port_id].vlan_mode)
    {
        for(i = 0; i < __VLAN_RULE_PER_PORT_MAX; ++i)
        {
            if(__port_vlan_table[port_id].vlan_rule[i].valid)
            {
                vlan_cfg[vlan_cnt++].c_vlan.vid = __port_vlan_table[port_id].vlan_rule[i].rule.c_vlan.vid;
            }
        }
        *cfg_nums = vlan_cnt;
    }
    else{ /* do nothing */ }
    
    *def_vlan = __port_vlan_table[port_id].def_vlan;
    *vlan_mode = __port_vlan_table[port_id].vlan_mode;

    return CS_E_OK;
}

cs_status epon_onu_sw_get_port_pvid(
	    CS_IN cs_callback_context_t         context,
	    CS_IN cs_int32                      device_id,
	    CS_IN cs_int32                      llidport,
	    CS_IN cs_port_id_t                  port_id,
	    CS_OUT cs_uint16            *vid
	    )
{
     cs_sdl_vlan_tag_t            def_vlan;  /* ignored for TRANSPARENT mode */
     cs_sdl_vlan_mode_t         vlan_mode;
     cs_sdl_vlan_cfg_t            vlan_cfg[__VLAN_RULE_PER_PORT_MAX];   /* ignored for TRANSPARENT mode */
     cs_uint16                       cfg_nums;    /* zero for TRANSPARENT/TAGGED mode */

     cs_status ret = CS_E_ERROR;

     if(epon_request_onu_vlan_get(context, device_id, llidport, port_id, &def_vlan, &vlan_mode, vlan_cfg, &cfg_nums) == CS_OK)
     {
    	 *vid = def_vlan.vid;
    	 ret = CS_E_OK;
     }

     return ret;
}
cs_status epon_request_onu_mc_vlan_add
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_int32                   device_id,
    CS_IN cs_int32                     llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_IN cs_uint16                     *vlan_id,
    CS_IN cs_uint16                     cfg_nums
)
{
    int i;
    cs_status rt = CS_E_OK;    
    cs_uint16 index;
    cs_uint16 used_num = 0;
    cs_uint16 new_num = 0;

    UNI_PORT_CHECK(port_id);

    if( (cfg_nums > __MC_VLAN_PER_PORT_MAX) || (0 == cfg_nums) )
    {
        return CS_E_PARAM;
    }

    __NULL_CHECK(vlan_id);

    for(i = 0; i < cfg_nums; ++i)
    {
        VID_CHECK(vlan_id[i]);
    }

    for(i = 0; i < __MC_VLAN_PER_PORT_MAX; ++i)
    {
        if(__port_vlan_table[port_id].mc_vlan[i].valid)
        {
            ++used_num;
        }
    }

    for(i = 0; i < cfg_nums; ++i)
    {
        if(!__mc_table_lookup(__port_vlan_table[port_id].mc_vlan, vlan_id[i], &index))
        {
            ++new_num;
        }
    }

    if( (used_num + new_num) > __MC_VLAN_PER_PORT_MAX )
    {
        return CS_E_RESOURCE;
    }

    for(i = 0; i < cfg_nums; ++i)
    {
        __mc_table_add(__port_vlan_table[port_id].mc_vlan, vlan_id[i]);
    }

    __port_vlan_table[port_id].mc_vlan_en = EPON_TRUE;

    __update_hw(port_id);

    return rt;
}

cs_status epon_request_onu_mc_vlan_del
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_int32                   device_id,
    CS_IN cs_int32                     llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_IN cs_uint16                     *vlan_id,
    CS_IN cs_uint16                     cfg_nums
)
{
    int i;
    cs_status rt = CS_E_OK;    
    cs_uint16 index;

    UNI_PORT_CHECK(port_id);

    if( (cfg_nums > __MC_VLAN_PER_PORT_MAX) || (0==cfg_nums) )
    {
        return CS_E_PARAM;
    }

    __NULL_CHECK(vlan_id);

    for(i = 0; i < cfg_nums; ++i)
    {
        VID_CHECK(vlan_id[i]);
    }

    for(i = 0; i < cfg_nums; ++i)
    {
        __mc_table_del(__port_vlan_table[port_id].mc_vlan, vlan_id[i]);

        if(!__vlan_table_lookup(__s_vlan_table, vlan_id[i], &index))
        {
            __remove_port_from_svlan(vlan_id[i], L2P_PORT(port_id));
        }
    }

    __update_hw(port_id);

    return rt;
}

cs_status epon_request_onu_mc_vlan_get
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_int32                   device_id,
    CS_IN cs_int32                     llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_OUT cs_uint16                    *vlan_id,
    CS_OUT cs_uint16                    *cfg_nums
)
{
    int i;
    cs_uint16 vlan_cnt = 0;

    UNI_PORT_CHECK(port_id);
    __NULL_CHECK(vlan_id);
    __NULL_CHECK(cfg_nums);

    for(i = 0; i < __MC_VLAN_PER_PORT_MAX; ++i)
    {
        if(__port_vlan_table[port_id].mc_vlan[i].valid)
        {
            vlan_id[vlan_cnt++] = __port_vlan_table[port_id].mc_vlan[i].vid;
        }
    }

    *cfg_nums = vlan_cnt;

    return CS_E_OK;
}

cs_status epon_request_onu_mc_vlan_clr
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_int32                   device_id,
    CS_IN cs_int32                     llidport,
    CS_IN cs_port_id_t                  port_id
)
{
    int i; 

    UNI_PORT_CHECK(port_id);

    __clear_hw_mc_vlan(port_id);

    for(i = 0; i < __MC_VLAN_PER_PORT_MAX; ++i)
    {
        __port_vlan_table[port_id].mc_vlan[i].valid = 0;
    }

    __update_hw(port_id);

    return CS_E_OK;
}

cs_status epon_request_onu_mc_vlan_action_set
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_int32                   device_id,
    CS_IN cs_int32                     llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_IN cs_sdl_mc_vlan_act_t          vlan_act,
    CS_IN cs_sdl_vlan_cfg_t             *swap_rule,
    CS_IN cs_uint16                     rule_nums
)
{
    int i;
	//diag_printf("mc port_id:%d   vlan_act :%d\n",port_id,vlan_act);
    UNI_PORT_CHECK(port_id);

    if(vlan_act >= SDL_MC_VLAN_TAG_END)
    {
        return CS_E_PARAM;
    }

    if(SDL_MC_VLAN_TAG_SWAP == __port_vlan_table[port_id].mc_act) /* old mode is swap, need to clear old swap rule */
    {
        __clear_hw_mc_swap(port_id);
        __rule_table_clr(__port_vlan_table[port_id].mc_swap);
    }

    if(SDL_MC_VLAN_TAG_SWAP == vlan_act)
    {
        __NULL_CHECK(swap_rule);
        if(rule_nums > __MC_VLAN_SWAP_PER_PORT_MAX)
        {
            return CS_E_PARAM;
        }

        for(i = 0; i < rule_nums; ++i)
        {
            VID_CHECK(swap_rule[i].s_vlan.vid);
            VID_CHECK(swap_rule[i].c_vlan.vid);
        }

        for(i = 0; i < rule_nums; ++i)
        {
            __rule_table_add(__port_vlan_table[port_id].mc_swap, __RULE_LOOKUP_ALL, &swap_rule[i]);
        }
    }

    __port_vlan_table[port_id].mc_act = vlan_act;
    __port_vlan_table[port_id].mc_vlan_en = EPON_TRUE;

    __update_hw(port_id);

    return CS_E_OK;
}

cs_status epon_request_onu_mc_vlan_action_get
(
    CS_IN cs_callback_context_t         context,
    CS_IN cs_int32                   device_id,
    CS_IN cs_int32                     llidport,
    CS_IN cs_port_id_t                  port_id,
    CS_OUT cs_sdl_mc_vlan_act_t          *vlan_act,
    CS_OUT cs_sdl_vlan_cfg_t             *swap_rule,
    CS_OUT cs_uint16                     *rule_nums
)
{
    int i;
    cs_uint16 rule_cnt = 0;

    UNI_PORT_CHECK(port_id);
    __NULL_CHECK(vlan_act);
    
    if(SDL_MC_VLAN_TAG_SWAP == __port_vlan_table[port_id].mc_act)
    {
        __NULL_CHECK(swap_rule);
        __NULL_CHECK(rule_nums);
        
        for(i = 0; i < __MC_VLAN_SWAP_PER_PORT_MAX; ++i)
        {
            if(__port_vlan_table[port_id].mc_swap[i].valid)
            {
                swap_rule[rule_cnt].s_vlan.vid = __port_vlan_table[port_id].mc_swap[i].rule.s_vlan.vid;
                swap_rule[rule_cnt].c_vlan.vid = __port_vlan_table[port_id].mc_swap[i].rule.c_vlan.vid;
                ++rule_cnt;
            }
        }

        *rule_nums = rule_cnt;
    }

    *vlan_act = __port_vlan_table[port_id].mc_act;

    return CS_E_OK;
}



cs_status sdl_vlan_init(void)
{
    cs_port_id_t                  port;
    cs_callback_context_t         context;
    cs_sdl_vlan_tag_t             def_vlan;
    cs_sdl_vlan_cfg_t				cfg;

    /* transparent for all ports */
    def_vlan.vid = 1;
	bzero(&cfg, sizeof(cfg));
    for(port = CS_UNI_PORT_ID1; port <=UNI_PORT_MAX; ++port)
    {
        epon_request_onu_vlan_set(context,0,0, port, def_vlan,SDL_VLAN_MODE_TRUNK,&cfg, 0);
    }

    //set uplink port FALLBACK mode

    gvlnSetPortVlanDot1qMode(QD_MASTER_DEV_PTR, CS_UPLINK_PHY_PORT, GT_FALLBACK);

    SDL_MAJ_LOG("sdl vlan initialized\n");
    
    return CS_E_OK;

}

