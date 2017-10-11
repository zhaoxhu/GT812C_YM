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

DEFINITIONS:  "DEVICE" means the Cortina Systems? Daytona SDK product.
"You" or "CUSTOMER" means the entity or individual that uses the SOFTWARE.
"SOFTWARE" means the Cortina Systems? SDK software.

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

Copyright (c) 2010 by Cortina Systems Incorporated
****************************************************************************/

#include "cs_cmd.h"
#ifdef HAVE_SDL_CMD
#include "sdl_fdb.h"
#include "aal_l2.h"
#include "sdl_init.h"
#include "plat_common.h"
#include "sdl.h"

/* This table saves the fdb SW entry list. 
 * It is the same as OAM.
 * Thru CLI, "fdb add/del" to update SW one by one.
 * Then thru 'fdb static add/del' cmd to update HW.
 * __fdb_new_entry_num tells you how many entries.
 * Make sure the table is continuous !!!
 */
cs_uint16 __fdb_static_entry_num = 0;
cs_sdl_fdb_entry_t __fdb_static_entry[64];

extern sdl_init_cfg_t sdl_int_cfg;
extern sdl_fdb_port_cfg_t g_fdb_port_cfg[UNI_PORT_MAX];

#if (defined HAVE_RTK_CMD && defined HAVE_SDL_CMD_ZTE)
  cs_status fdb_entry_dump_rtk (cs_port_id_t port);
#endif

cs_status fdb_entry_dump (void);


#ifdef HAVE_SDL_CMD_HELP
static const char *fdb_cmd_usage_full[] = 
{
    "fdb full <mode> <port>",
    "mode: ",
    "   0: pass and do not learn",
    "   1: drop and do not learn",
    "   2: Least recent usage algorithm"
    "port num:",
    "   0: Pon port",
    "   1: UNI port 0 <if only 1 UNI port, use this value>",
    "   2: UNI port 1",
    "   ...",
};

static const char *fdb_cmd_usage_limit[] = 
{
    "fdb limit <value> <port>",
    "value:",
    "0 - 16",
    "port num:",
    "   0: Pon port",
    "   1: UNI port 0 <if only 1 UNI port, use this value>",
    "   2: UNI port 1",
    "   ...",
    
};
#if 0
static const char *fdb_cmd_usage_aging[] = 
{
    "fdb aging <time>",
    "time: ",
#ifndef HAVE_RTK_CMD
    " <0 - 0xfff>  0: disable aging"
#else
    " valid: <0s - 458s>   0: disable aging"
#endif
};

static const char *fdb_cmd_usage_learn[] = 
{
    "fdb learn <mode> <port>",
    "mode:",
    "   0: disable fdb learn",
    "   1: enable fdb learn"
    "port num:",
    "   0: Pon port",
    "   1: UNI port 0 <if only 1 UNI port, use this value>",
    "   2: UNI port 1",
    "   ...",
};
#endif
static const char *fdb_cmd_usage_vid[] = 
{
    "fdb vid <mode> <port>",
    "mode:"
    "   0: Don't learn VLAN ID",
    "   1: Learn VLAN ID"
    "port num:",
    "   0: Pon port",
    "   1: UNI port 0 <if only 1 UNI port, use this value>",
    "   2: UNI port 1",
    "   ...",
};

static const char *fdb_cmd_usage_clr[] = 
{
    "fdb clr <mode> <port>",
    "mode:"
    "   0: clear fdb table for all entries",
    "   1: clear fdb table for dynamic entries",
    "   2: clear fdb table for static entries",
    "port num:",
    "   0: Pon port",
    "   1: UNI port 0 <if only 1 UNI port, use this value>",
    "   2: UNI port 1",
    "   ...",
};

static const char *fdb_cmd_usage_show[] = 
{
    "fdb show <'cfg', 'entry', ' '>",
    "   cfg: show fdb configuration",
    "   entry: show fdb entries",
    "   NONE: show all;"
};

static const char *fdb_cmd_usage_add[] = 
{
    "fdb add mac0 mac1 mac2 mac3 mac4 mac5 vid portid type",
    "Attention: Only expert can use this cmd",
    "Add one entry into SW table",
    "   mac0: macaddr byte 0, 0x0 - 0xff",
    "   mac1: macaddr byte 1, 0x0 - 0xff",
    "   mac2: macaddr byte 2, 0x0 - 0xff",
    "   mac3: macaddr byte 3, 0x0 - 0xff",
    "   mac4: macaddr byte 4, 0x0 - 0xff",
    "   mac5: macaddr byte 5, 0x0 - 0xff",
    "   vid:  VLAN ID",
    "   portid:  PORT ID",
    "   type  :  Static(1)/Dynamic(0)"
};

static const char *fdb_cmd_usage_del[] = 
{
    "fdb del <0-63>",
    "Attention: Only expert can use this cmd",
    "Del one entry in SW table"
};
#if 0
static const char *fdb_cmd_usage_static[] = 
{
    "fdb static <'add', 'del', 'get'>",
    "   add: Add the SW table entries into HW",
    "   del: Remove the SW table entries from HW",
    "   get: dump the static entries in HW",
};
#endif
#endif

void __show_fdb_sw_table(void)
{
    cs_uint8 index = 0;
    diag_printf("====== FDB SW table is shown:======\n");
    diag_printf("index   mac_address        vid   port type \n");
    
    for (index = 0; index < __fdb_static_entry_num; index++) {
        diag_printf(" %2d   %02x%02x.%02x%02x.%02x%02x %8d %6d %6d  \n", index,
            __fdb_static_entry[index].mac.addr[0], 
            __fdb_static_entry[index].mac.addr[1], 
            __fdb_static_entry[index].mac.addr[2], 
            __fdb_static_entry[index].mac.addr[3], 
            __fdb_static_entry[index].mac.addr[4], 
            __fdb_static_entry[index].mac.addr[5], 
            __fdb_static_entry[index].vlan_id,
            __fdb_static_entry[index].port,
            __fdb_static_entry[index].type);
    }  
    diag_printf("====== Totally %2d SW entries====\n", __fdb_static_entry_num);
}

void __clr_fdb_sw_table(void)
{
    cs_uint8 index = 0;
    for (index = 0; index < __fdb_static_entry_num; index++) {
        memset(&__fdb_static_entry[index], 0, sizeof(cs_sdl_fdb_entry_t));
    }
    
    __fdb_static_entry_num = 0;
}


sal_cmd_result_t fdb_cmd_full(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_sdl_fdb_full_behavior_t full = 0;
    cs_status  status = 0;
    cs_port_id_t  portid = CS_UNI_PORT_ID1;

    if(argc > 4) return SAL_CMD_INVALID_PARAM;
    
    full = iros_strtol(argv[2]);
    if(full > 2) return SAL_CMD_INVALID_PARAM;
    
    portid = iros_strtol(argv[3]);

    status = epon_request_onu_fdb_full_behavior_set(context, 0, 0, portid, full);
    if(status != CS_E_OK){
        cs_printf("epon_request_onu_fdb_full_behavior_set failed \n");    
        return cs_status_2_cmd_rc_map(status);       
    }

    return cs_status_2_cmd_rc_map(status);
}

sal_cmd_result_t fdb_cmd_limit(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_uint32 limit = 0;
    cs_status  status = 0;
    cs_port_id_t  portid = CS_UNI_PORT_ID1;

    if(argc > 4) return SAL_CMD_INVALID_PARAM;
    limit = iros_strtol(argv[2]);
    portid = iros_strtol(argv[3]);
    status = epon_request_onu_fdb_mac_limit_set(context, 0, 0, portid, limit);

    if(status != CS_E_OK){
        cs_printf("epon_request_onu_fdb_mac_limit_set failed \n");    
        return cs_status_2_cmd_rc_map(status);       
    }

    return cs_status_2_cmd_rc_map(status);
}

void fdb_cmd_aging_get(int argc,char**argv)
{
	cs_callback_context_t context;
    cs_uint32 timer = 0;
	cs_status  status = 0;
	status = epon_request_onu_fdb_age_get(context, 0, 0, &timer);
	if(status != CS_E_OK)
		{
			cs_printf("epon request onu fdb aging get failed\n");
			return ;
		}
	cs_printf("get fdb aging timer:%d\n",timer);
	return ;
}
void fdb_cmd_aging(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_uint32 timer = 0;
    cs_status  status = 0;
    if(argc < 1) 
		{
			return ;
    	}
    timer = iros_strtol(argv[0]);
    status = epon_request_onu_fdb_age_set(context, 0, 0, timer);
    if(status != CS_E_OK){
        cs_printf("epon_request_onu_fdb_age_set failed \n");    
        cs_status_2_cmd_rc_map(status);  
		return;
    }
	else
	{
		cs_printf("set aging timer:%d success\n",timer);
	}

    return ;
}
void fdb_cmd_learn(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  status = 0;
    cs_port_id_t  portid = CS_UNI_PORT_ID1;
    cs_boolean en = 0;

    if(argc != 2)
    {
		return;
    }
    
    en = iros_strtol(argv[0]);
    portid = iros_strtol(argv[1]);  
	cs_printf("en:%d  portid:%d\n",en,portid);
    if (en == 1) {
        status = epon_request_onu_mac_learn_set(context, 0, 0, portid, 1);
    } else if (en == 0) {
        status = epon_request_onu_mac_learn_set(context, 0, 0, portid, 0);
    } else {
        return ;
    }
	if(status == CS_E_OK)
	{
		if(en == 1)
			{
				cs_printf("set port %d fdb learn enable success\n",portid);
			}
		else
			{
				cs_printf("set port %d fdb learn disable success\n",portid);
			}
	}
    if(status != CS_E_OK){
        cs_printf("epon_request_onu_mac_learn_set failed \n");    
        cs_status_2_cmd_rc_map(status);
		return;
    }

    cs_status_2_cmd_rc_map(status);
	return;
}

sal_cmd_result_t fdb_cmd_vid(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  status = 0;
    cs_port_id_t  portid = CS_UNI_PORT_ID1;
    cs_sdl_fdb_learn_mode_t vid = 0;

    if(argc > 4) return SAL_CMD_INVALID_PARAM;

    vid = iros_strtol(argv[2]);      
    if(vid > 1) return SAL_CMD_INVALID_PARAM;
    portid = (cs_port_id_t)iros_strtol(argv[3]);
    
    status = epon_request_onu_fdb_learn_mode_set(context, 0, 0, portid, vid);
    if(status != CS_E_OK){
        cs_printf("epon_request_onu_fdb_learn_mode_set failed \n");    
        return cs_status_2_cmd_rc_map(status);       
    }

    return cs_status_2_cmd_rc_map(status);
}

sal_cmd_result_t fdb_cmd_clr(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  status = 0;
    cs_sdl_fdb_clr_mode_t     fdb_block = 0;
    cs_uint32 port = 0;

    if(argc > 4) {
        return SAL_CMD_INVALID_PARAM;
    }

    port = iros_strtol(argv[3]);
    
    fdb_block = (cs_sdl_fdb_clr_mode_t)iros_strtol(argv[2]);
    if (fdb_block > SDL_FDB_CLR_STATIC) {
        return SAL_CMD_INVALID_PARAM;
    }
    
    if(port == 0xff)
    {
    	__clr_fdb_sw_table();
    	status = epon_request_onu_fdb_entry_clr(context, 0, 0, fdb_block);
    	if(status != CS_E_OK){
    		cs_printf("epon_request_onu_fdb_fdb_clear failed \n");
    		return cs_status_2_cmd_rc_map(status);
    	}
    }
    else
    {
    	status = epon_request_onu_fdb_entry_clr_per_port(context, 0, 0, port, fdb_block);
    	if(status != CS_E_OK)
    	{
    		cs_printf("epon_request_onu_fdb_entry_clr_per_port failed \n");
    		return cs_status_2_cmd_rc_map(status);
    	}
    }
    
    return cs_status_2_cmd_rc_map(status);
}

#if (defined HAVE_RTK_CMD && defined HAVE_SDL_CMD_ZTE)
sal_cmd_result_t fdb_cmd_clr_perport(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  status = 0;
    cs_sdl_fdb_clr_mode_t     fdb_block = 0;
    cs_port_id_t port;

    if(argc > 4) {
        return SAL_CMD_INVALID_PARAM;
    }

    if (iros_strtol(argv[2]) == 0xff) {
        __clr_fdb_sw_table();
        return CS_E_OK;
    }
    
    fdb_block = (cs_sdl_fdb_clr_mode_t)iros_strtol(argv[2]);
    if (fdb_block > SDL_FDB_CLR_STATIC) {
        return SAL_CMD_INVALID_PARAM;
    }
    
    port = (cs_port_id_t)iros_strtol(argv[3]);
    if(!port){
        status = epon_request_onu_fdb_entry_clr(context, 0, 0, fdb_block);
        if(status != CS_E_OK){
            cs_printf("epon_request_onu_fdb_fdb_clear failed \n");    
            return cs_status_2_cmd_rc_map(status);       
        }
    }
    else{
        status = epon_request_onu_fdb_entry_clr_per_port(context, 0, 0, port, fdb_block);
        if(status != CS_E_OK){
            cs_printf("epon_request_onu_fdb_entry_clr_per_port failed \n");    
            return cs_status_2_cmd_rc_map(status);       
        }
        
    }
    
    return cs_status_2_cmd_rc_map(status);
}
#endif

void __show_fdb_cfg(void)
{
    cs_callback_context_t       context;
    cs_status                   status = 0;
    cs_uint32                   aging_time;

    status = epon_request_onu_fdb_age_get(context, 0, 0, &aging_time);
    if(status != CS_E_OK){
        cs_printf("epon_request_onu_fdb_age_get failed \n");    
        return;       
    }
#ifndef HAVE_SDL_CMD_KT 
    cs_uint8 port;
    for(port=0; port<UNI_PORT_MAX;port++) {
        cs_printf("Port ID              : %d  \n",  port);
        cs_printf("MAC Limit            : %d  \n",  g_fdb_port_cfg[port].mac_limit);
        cs_printf("Static Mac Num       : %d  \n",  g_fdb_port_cfg[port].static_mac_num);

#ifndef HAVE_RTK_CMD 
        cs_printf("MAC Learn Enable     : %d  \n",  g_fdb_port_cfg[port].lrn_en);
        cs_printf("MAC Learn Mode       : %d  \n",  g_fdb_port_cfg[port].lrn_mod);
        cs_printf("Full Behavior        : %d  \n\n",g_fdb_port_cfg[port].full_mod);
#else
        cs_printf("\n");
#endif

    } 
    
    cs_printf("aging_time           : %d  \n", aging_time);
#endif

}


void __show_fdb_entry(void)
{
    fdb_entry_dump();
}

#if (defined HAVE_RTK_CMD && defined HAVE_SDL_CMD_ZTE)
void __show_fdb_entry_rtk(cs_port_id_t port)
{
    fdb_entry_dump_rtk(port);
}
#endif

sal_cmd_result_t fdb_cmd_show(int argc, char **argv)
{
    if((argc<2)||(argc>4)) return SAL_CMD_INVALID_PARAM;

    if(2==argc)
    {
        __show_fdb_cfg();
#if (defined HAVE_RTK_CMD && defined HAVE_SDL_CMD_ZTE)
        extern cs_status fdb_static_entry_show(void);
        fdb_static_entry_show();
#else
        __show_fdb_entry();
#endif
        return SAL_CMD_OK; 
    }
    if(cmd_cmp(argv[2], "cfg"))
    {
        __show_fdb_cfg();
        
        return SAL_CMD_OK; 
    }
    
#if (defined HAVE_RTK_CMD && defined HAVE_SDL_CMD_ZTE)
    if(cmd_cmp(argv[2], "entry_rtk"))
    {
        cs_port_id_t port;
        port = (cs_port_id_t)iros_strtol(argv[3]);
        __show_fdb_entry_rtk(port);
        
        return SAL_CMD_OK; 
    }
#endif

    else if(cmd_cmp(argv[2], "entry"))
    {
#if (defined HAVE_RTK_CMD && defined HAVE_SDL_CMD_ZTE)

        extern cs_status fdb_static_entry_show(void);
        fdb_static_entry_show();
#else
        __show_fdb_entry();
#endif
        return SAL_CMD_OK; 
    }
    
    else if(cmd_cmp(argv[2], "sw"))
    {
        __show_fdb_sw_table();
    } else {
        return SAL_CMD_INVALID_PARAM;
    }

    return SAL_CMD_OK;       
}

sal_cmd_result_t fdb_cmd_del(int argc, char **argv)
{
    cs_uint8 index = 0, ii = 0;
    if(argc!=3) return SAL_CMD_INVALID_PARAM;

    index = (cs_uint8)iros_strtol(argv[2]);

    if (index >= __fdb_static_entry_num) {
        diag_printf("ERROR: there are only %d entries\n", __fdb_static_entry_num);
        return SAL_CMD_INVALID_PARAM;
    }

    if(index >= 64) return SAL_CMD_INVALID_PARAM;

    if(index == 63){
        memcpy(&__fdb_static_entry[index], 0, sizeof(cs_sdl_fdb_entry_t));
    }
    else{
        for (ii = index; ii < (__fdb_static_entry_num - 1); ii++) {
            memcpy(&__fdb_static_entry[ii], &__fdb_static_entry[ii+1], sizeof(cs_sdl_fdb_entry_t));
        }
    }
    
    memset(&__fdb_static_entry[__fdb_static_entry_num - 1], 0, sizeof(cs_sdl_fdb_entry_t));

    __fdb_static_entry_num--;

    return 0;       
}


int convertHex(char num, cs_uint8 *hex)
{
    typedef struct 
    {
        char low;
        char high;
        cs_uint8 base;
    } edge_t;

    static edge_t hex_edge[] = 
    {
        { '0', '9', 0 },
        { 'a', 'f', 10 },
        { 'A', 'F', 10 }
    };

    int i;

    for(i=0; i<sizeof(hex_edge)/sizeof(edge_t); i++)
    {
        if(num >= hex_edge[i].low && num <= hex_edge[i].high)
        {
            *hex = num - hex_edge[i].low + hex_edge[i].base;
            return 0;
        }
    }

    return -1;
}

int get_mac_addr(char *num_array, cs_uint8 *mac)
{
    int i;
    cs_uint8 one, two;

    if(strlen(num_array) != 12)
    {
        diag_printf("\nInvalid mac address!\n");
        return -1;
    }

    for(i=0; i<6; i++)
    {
        if( convertHex(num_array[2*i], &two) || convertHex(num_array[2*i+1], &one) )
            return -1;

        mac[i] = two*16 + one;
    }

    return 0;
}
void fdb_cmd_list_del(int argc, char **argv)
{
    cs_uint16  ii = 0;
	cs_uint32 vlanid;
	cs_status  rc = 0;
	cs_uint8       addr[CS_MACADDR_LEN];
	cs_callback_context_t    context;
	//cs_uint32 cout;
    if(argc!=2) 
    	{
    		cs_printf("input error \n");
			return ;
    	}
	vlanid = iros_strtol(argv[0]);
    if(get_mac_addr(argv[1], addr))
    {
    	cs_printf("input mac address error\n");
        return;
    }
	for(ii = 0; ii <(__fdb_static_entry_num ) ;ii++)
		{
			#if 0
			for(cout= 0;cout < 6; cout++)
				{
					cs_printf("addr:%02x:",addr[cout]);
					if(cout == 5)
						{
						cs_printf("\n");
						cs_printf("vlan1:%d\n",vlanid);
						}
				}
			for(cout= 0;cout < 6; cout++)
				{
					cs_printf("addr:%02x:",__fdb_static_entry[ii].mac.addr[cout]);
					if(cout == 5)
						{
						cs_printf("\n");
						cs_printf("vlan2:%d\n",__fdb_static_entry[ii].vlan_id);
						}
				}
			#endif
			if((!memcmp(addr,__fdb_static_entry[ii].mac.addr,6)) && (__fdb_static_entry[ii].vlan_id == vlanid))
				{
					 rc = epon_request_onu_fdb_entry_del(context, 0, 0,  
				        &__fdb_static_entry[ii].mac, __fdb_static_entry[ii].vlan_id);
				    if (rc) {
				        diag_printf("epon_request_onu_fdb_static_entry_del failed\n");
				    	}
					else
						{
							__fdb_static_entry_num--;
							cs_printf("delete %02x:%02x:%02x:%02x:%02x:%02x static fdb entry success\n",__fdb_static_entry[ii].mac.addr[0],
																							__fdb_static_entry[ii].mac.addr[1],
																							__fdb_static_entry[ii].mac.addr[2],
																							__fdb_static_entry[ii].mac.addr[3],
																							__fdb_static_entry[ii].mac.addr[4],
																							__fdb_static_entry[ii].mac.addr[5]);
							return;
						}
				}
		}

		return ;   
}
void fdb_cmd_list_add(int argc, char **argv)
{
	cs_status  rc = 0;
	cs_sdl_fdb_entry_type_t type;
	cs_callback_context_t    context;
    if(argc!=4) 
		return ;

    if (__fdb_static_entry_num >= sdl_int_cfg.max_static_mac_entry) {
        diag_printf("ERROR: entry table is full now\n");
        return;
    }
	type = atoi(argv[3]);
	if(type != 1)
		{
			cs_printf("intput error    please input '1' for static\n");
			return;
		}
    if(get_mac_addr(argv[2], __fdb_static_entry[__fdb_static_entry_num].mac.addr))
    {
    	cs_printf("input mac address error\n");
        return;
    }
    else
    {
	    __fdb_static_entry[__fdb_static_entry_num].vlan_id = iros_strtol(argv[1]);
	    __fdb_static_entry[__fdb_static_entry_num].port = iros_strtol(argv[0]);
	    __fdb_static_entry[__fdb_static_entry_num].type = type;
		rc = epon_request_onu_fdb_entry_add(context, 0, 0, &__fdb_static_entry[__fdb_static_entry_num]);
        if (rc) {
            diag_printf("epon_request_onu_fdb_static_entry_add failed! rc :%d\n",rc);
        }
	    __fdb_static_entry_num++;
    }

    
    return ;       
}

int fdb_static_list_add(char *mac, int port, int vlan_id)
{
	int ret = 0;
	cs_status  rc = 0;
	cs_callback_context_t    context;

    if (__fdb_static_entry_num >= sdl_int_cfg.max_static_mac_entry)
	{
        diag_printf("ERROR: entry table is full now\n");
        ret = CS_E_ERROR;
		goto end;
    }

    if(get_mac_addr(mac, __fdb_static_entry[__fdb_static_entry_num].mac.addr))
    {
    	cs_printf("input mac address error\n");
        ret = CS_E_ERROR;
		goto end;
    }
    else
    {
	    __fdb_static_entry[__fdb_static_entry_num].vlan_id = vlan_id;
	    __fdb_static_entry[__fdb_static_entry_num].port = port;
	    __fdb_static_entry[__fdb_static_entry_num].type = SDL_FDB_ENTRY_STATIC;
		rc = epon_request_onu_fdb_entry_add(context, 0, 0, &__fdb_static_entry[__fdb_static_entry_num]);
        if (rc) {
            diag_printf("epon_request_onu_fdb_static_entry_add failed! rc :%d\n",rc);
        }
	    __fdb_static_entry_num++;
    }
	ret = CS_E_OK;
	
end: 
    return ret;       
}

#if 1
//ɾ����һ��Ԫ�غ󣬴�Ԫ�غ��������Ԫ����ǰ�ƶ�һ����λ
// ��Ϊ��list ��ɾ�������ǳ��ȼ�1����ʵ��ɾ���������һ��Ԫ��
static int fdb_static_list_del_update(cs_sdl_fdb_entry_t *pFdb_entry, int entry_len)
{
	//��ڹ�����
	if(NULL == pFdb_entry)
	{
		cs_printf("in %s, line:%d, arg check err!\n");
		return CS_E_ERROR;
	}

	if(entry_len <= 0)
	{
		cs_printf("in %s, line:%d, arg check err!\n");
		return CS_E_ERROR;
	}
	//ֻ��һ��Ԫ��ʱ�������Ԫ�ز��˳�
	if(1 == entry_len)
	{
		memset(&pFdb_entry[0], 0, sizeof(cs_sdl_fdb_entry_t));
		
		return CS_E_OK;
	}
	
	
	//��һ��Ԫ���Ժ������Ԫ�ؾ���ǰ�ƶ�һ����λ
	cs_sdl_fdb_entry_t *fdb_entry_buf = NULL;
	int buf_len = 0;
	buf_len = sizeof(cs_sdl_fdb_entry_t)*(entry_len-1);
	fdb_entry_buf = (cs_sdl_fdb_entry_t *)malloc(buf_len);
	if(NULL == fdb_entry_buf)
	{
		cs_printf("in %s, line:%d, malloc err!\n");
		return CS_E_ERROR;
	}
	
	memcpy(fdb_entry_buf, &pFdb_entry[1], buf_len);
	memcpy(&pFdb_entry[0], fdb_entry_buf, buf_len);
	free(fdb_entry_buf);
	fdb_entry_buf = NULL;
	//���һ��Ԫ������
	memset(&pFdb_entry[entry_len-1], 0, sizeof(cs_sdl_fdb_entry_t));
	
	//�����˳�
	return CS_E_OK;
}
#endif
int fdb_static_list_del(char *mac, int vlan_id)
{
	int ret = CS_E_OK;
    cs_uint16  ii = 0;
	cs_status  rc = 0;
	cs_uint8       addr[CS_MACADDR_LEN];
	cs_callback_context_t    context;

    if(get_mac_addr(mac, addr))
    {
    	cs_printf("input mac address error\n");
        ret = CS_E_ERROR;
		goto end;
    }
	for(ii = 0; ii <(__fdb_static_entry_num ) ;ii++)
	{
		if((!memcmp(addr,__fdb_static_entry[ii].mac.addr,6)) && (__fdb_static_entry[ii].vlan_id == vlan_id))
		{
			 rc = epon_request_onu_fdb_entry_del(context, 0, 0,  
		        &__fdb_static_entry[ii].mac, __fdb_static_entry[ii].vlan_id);
		    if (rc) 
			{
		        diag_printf("epon_request_onu_fdb_static_entry_del failed error is %d\n",rc);
		    }
			else
			{
				cs_printf("delete %02x:%02x:%02x:%02x:%02x:%02x static fdb entry success\n",__fdb_static_entry[ii].mac.addr[0],
																				__fdb_static_entry[ii].mac.addr[1],
																				__fdb_static_entry[ii].mac.addr[2],
																				__fdb_static_entry[ii].mac.addr[3],
																				__fdb_static_entry[ii].mac.addr[4],
																				__fdb_static_entry[ii].mac.addr[5]);
				#if 1
				if(CS_E_OK != fdb_static_list_del_update(&__fdb_static_entry[ii], __fdb_static_entry_num - ii))
				{
					ret = CS_E_ERROR;
					goto end;
				}
				#endif
				__fdb_static_entry_num--;
				
				ret = CS_E_OK;
				goto end;
			}
		}
	}
	ret = CS_E_ERROR;
	cs_printf("mac not found!\n");
	
end:
		return ret;   
}
extern int fdb_static_list_tlv_info_get(int *len, char **value, int *free_need)
{
	int ret = CS_E_OK;
//    cs_uint16  ii = 0;

	//��ڹ�����
	if(NULL == len)
	{
		cs_printf("arg check err!\n");
		cs_printf("in %s, line :%d\n", __func__, __LINE__);
		ret=-1;
	}
	else
	{
		*len = sizeof(cs_sdl_fdb_entry_t)*__fdb_static_entry_num;
	}
	if(NULL == value)
	{
		cs_printf("arg check err!\n");
		cs_printf("in %s, line :%d\n", __func__, __LINE__);
		ret=-1;
	}
	else
	{
//		char *static_mac_entry = NULL,*p = NULL;
//		static_mac_entry = (char *)iros_malloc(IROS_MID_APP, sizeof(cs_sdl_fdb_entry_t)*__fdb_static_entry_num);
//		p = static_mac_entry;
//		for(ii = 0; ii <(__fdb_static_entry_num ) ;ii++)
//		{
//
//			memcpy(p, __fdb_static_entry[ii].port, sizeof(cs_sdl_fdb_entry_t));
//			p += sizeof(cs_sdl_fdb_entry_t);
//		}
		*value = (char *)&__fdb_static_entry[0];
	}
	if(NULL == free_need)
	{
		cs_printf("arg check err!\n");
		cs_printf("in %s, line :%d\n", __func__, __LINE__);
		ret=-1;
	}
	else
	{
		*free_need = 0;
	}
	return ret;

}
extern int fdb_static_list_tlv_info_handle(int length,char *value,int opcode)
{
    cs_uint16  ii = 0;
	int ret = CS_E_OK;
	cs_callback_context_t     context;

	if(NULL == value)
	{
		return -1;
	}
	if(length > sizeof(cs_sdl_fdb_entry_t)*64)
	{
		return -1;
	}
	if(DATA_SHOW != opcode)
	{
		memcpy(&__fdb_static_entry[0],value,length);
		__fdb_static_entry_num = length/sizeof(cs_sdl_fdb_entry_t);
		for(ii = 0; ii <(__fdb_static_entry_num ) ;ii++)
		{
			ret = epon_request_onu_fdb_entry_add(context, 0, 0, &__fdb_static_entry[ii]);
	        if (ret) {
	            diag_printf("epon_request_onu_fdb_static_entry_add failed! rc :%d\n",ret);
	        }
	        cyg_thread_delay(10);
		}
	}
	if(DATA_SHOW == opcode)
	{
		__show_fdb_sw_table();
	}
	return 0;
}

#if 1
sal_cmd_result_t fdb_cmd_add(int argc, char **argv)
{
    if(argc!=11) return SAL_CMD_INVALID_PARAM;

    if (__fdb_static_entry_num >= sdl_int_cfg.max_static_mac_entry) {
        diag_printf("ERROR: entry table is full now\n");
        return SAL_CMD_FAIL;
    }

    __fdb_static_entry[__fdb_static_entry_num].mac.addr[0] = iros_strtol(argv[2]);
    __fdb_static_entry[__fdb_static_entry_num].mac.addr[1] = iros_strtol(argv[3]);
    __fdb_static_entry[__fdb_static_entry_num].mac.addr[2] = iros_strtol(argv[4]);
    __fdb_static_entry[__fdb_static_entry_num].mac.addr[3] = iros_strtol(argv[5]);
    __fdb_static_entry[__fdb_static_entry_num].mac.addr[4] = iros_strtol(argv[6]);
    __fdb_static_entry[__fdb_static_entry_num].mac.addr[5] = iros_strtol(argv[7]);
    __fdb_static_entry[__fdb_static_entry_num].vlan_id = iros_strtol(argv[8]);
    __fdb_static_entry[__fdb_static_entry_num].port = iros_strtol(argv[9]);
    __fdb_static_entry[__fdb_static_entry_num].type = iros_strtol(argv[10]);

    

    __fdb_static_entry_num++;
    
    return 0;       
}
#endif
cs_status fdb_static_entry_get(cs_uint32 port,cs_uint32 vlanid);
void fdb_cmd_list_get(int argc,char **argv)
{
	 cs_uint32 port;
	 cs_uint32 vlanid;
	 port = atoi(argv[0]);
	 vlanid = atoi(argv[1]);
	 if(port < 1 || port > 4)
	 	{
	 		cs_printf("input error !!!\n");
			cs_printf(" for example:sw fdb-list 1 4008\n");
	 		return ;
	 	}
	 fdb_static_entry_get(port,vlanid);
	 return ;
}
#if 0
void fdb_cmd_list_get(int argc, char **argv)
{
    cs_callback_context_t context;
    cs_status  rc = 0;
    cs_uint32 index;
#if (defined HAVE_RTK_CMD && defined HAVE_SDL_CMD_ZTE)
    cs_port_id_t  port;
#endif

    if(argc < 4) return ;

    if(cmd_cmp(argv[3], "add")) {
        index = (cs_uint8)iros_strtol(argv[4]);
        if (__fdb_static_entry_num <= index) {
            diag_printf("This entry %d is not existing\n", index);
            return ;
        }
        if(index >= 64) return ;
        rc = epon_request_onu_fdb_entry_add(context, 0, 0, &__fdb_static_entry[index]);
        if (rc) {
            diag_printf("epon_request_onu_fdb_static_entry_add failed! rc :%d\n",rc);
        }
    
    } else if(cmd_cmp(argv[3], "del")) {
        index = (cs_uint8)iros_strtol(argv[4]);
        if (__fdb_static_entry_num <= index) {
            diag_printf("This entry %d is not existing\n", index);
            return ;
        }

        if(index >= 64) return ;
		
        rc = epon_request_onu_fdb_entry_del(context, 0, 0,  
            &__fdb_static_entry[index].mac, __fdb_static_entry[index].vlan_id);
        if (rc) {
            diag_printf("epon_request_onu_fdb_static_entry_del failed\n");
        }
    
    } 
    else if (cmd_cmp(argv[3], "get")) {
		 cs_uint32 port;
		 cs_uint32 vlanid;
		 port = atoi(argv[0]);
		 vlanid = atoi(argv[1]);
		 fdb_static_entry_show(port,vlanid);
{
    cs_uint8 i, j;

    cs_printf("\n\n-----------SW Static FDB Table---------------\n\n");
    for(i=0; i<UNI_PORT_MAX; i++){
        cs_uint8 index = 0;
        cs_printf("PortID: %d---\n", i+1);
        for(j=0; j<__FDB_STATIC_ENTRY_MAC_MAX; j++)
        {
            if(__fdb_static_entry_table[i].valid_map & (1<<j)){
                cs_printf("index : %d\n", ++index);
                cs_printf("vlan  : %d\n", __fdb_static_entry_table[i].entry[j].vlan_id);
                cs_printf("mac   : %02x:%02x:%02x:%02x:%02x:%02x\n",
                        __fdb_static_entry_table[i].entry[j].mac.addr[0],
                        __fdb_static_entry_table[i].entry[j].mac.addr[1],
                        __fdb_static_entry_table[i].entry[j].mac.addr[2],
                        __fdb_static_entry_table[i].entry[j].mac.addr[3],
                        __fdb_static_entry_table[i].entry[j].mac.addr[4],
                        __fdb_static_entry_table[i].entry[j].mac.addr[5]);
            }
        }
        cs_printf("\n");
    }
    
    return CS_E_OK;
}
        
    }
     else if (cmd_cmp(argv[3], "get_next")) {
        cs_sdl_fdb_entry_t fdb_entry;
        cs_sdl_fdb_entry_get_mode_t mode;
        cs_uint16 next;
        memset(&fdb_entry, 0, sizeof(cs_sdl_fdb_entry_t));
        mode  = (cs_sdl_fdb_entry_get_mode_t)iros_strtol(argv[4]);
        index = (cs_uint32)iros_strtol(argv[5]);
        rc = epon_request_onu_fdb_entry_get_byindex(context, 0, 0, mode, index, &fdb_entry, &next);
        if (rc) {
            diag_printf("epon_request_onu_fdb_entry_get_byindex failed\n");
        }        
        cs_printf("Entry index : %d  \n",  next);
        cs_printf("MAC address : %02x:%02x:%02x:%02x:%02x:%02x\n", 
            fdb_entry.mac.addr[0], 
            fdb_entry.mac.addr[1], 
            fdb_entry.mac.addr[2], 
            fdb_entry.mac.addr[3], 
            fdb_entry.mac.addr[4], 
            fdb_entry.mac.addr[5]);
        cs_printf("Entry VLAN  : %d   \n", fdb_entry.vlan_id);
        cs_printf("Entry port  : %d   \n", fdb_entry.port); 
        cs_printf("Entry type  : %d   \n", fdb_entry.type);
        
    }
#if (defined HAVE_RTK_CMD && defined HAVE_SDL_CMD_ZTE)
    else if (cmd_cmp(argv[3], "get_next_p")) {
        cs_sdl_fdb_entry_t fdb_entry;
        cs_sdl_fdb_entry_get_mode_t mode;
        cs_uint16 next;
        memset(&fdb_entry, 0, sizeof(cs_sdl_fdb_entry_t));
        port = (cs_port_id_t)iros_strtol(argv[4]);
        mode  = (cs_sdl_fdb_entry_get_mode_t)iros_strtol(argv[5]);
        index = (cs_uint32)iros_strtol(argv[6]);
        rc = epon_request_onu_fdb_entry_get_byindex_per_port(context, 0, 0, port, mode, index, &fdb_entry, &next);
        if (rc) {
            diag_printf("epon_request_onu_fdb_entry_get_byindex_per_port failed\n");
        }        
        cs_printf("Entry index : %d  \n",  next);
        cs_printf("MAC address : %02x:%02x:%02x:%02x:%02x:%02x\n", 
            fdb_entry.mac.addr[0], 
            fdb_entry.mac.addr[1], 
            fdb_entry.mac.addr[2], 
            fdb_entry.mac.addr[3], 
            fdb_entry.mac.addr[4], 
            fdb_entry.mac.addr[5]);
        cs_printf("Entry VLAN  : %d   \n", fdb_entry.vlan_id);
        cs_printf("Entry port  : %d   \n", fdb_entry.port); 
        cs_printf("Entry type  : %d   \n", fdb_entry.type);
    }
#endif
    
    sal_api_2_cmd_rc_map(rc); 
	return;
}

#endif

static sal_cmd_reg_struct_t fdb_cmd_list[] = 
{
#ifdef HAVE_SDL_CMD_HELP
    {fdb_cmd_show, "show", "show fdb configuration or all entires", fdb_cmd_usage_show, sizeof(fdb_cmd_usage_show)/4},
    {fdb_cmd_full, "full", "config fdb full_behavior", fdb_cmd_usage_full, sizeof(fdb_cmd_usage_full)/4},
    {fdb_cmd_limit, "limit", "config max mac number to learn", fdb_cmd_usage_limit, sizeof(fdb_cmd_usage_limit)/4},
   // {fdb_cmd_aging, "aging", "aging timer. 0 - disable aging", fdb_cmd_usage_aging, sizeof(fdb_cmd_usage_aging)/4},
    //{fdb_cmd_learn, "learn", "enable/disable mac learning", fdb_cmd_usage_learn, sizeof(fdb_cmd_usage_learn)/4},
    {fdb_cmd_vid, "vid", "learing VLAN id or not", fdb_cmd_usage_vid , sizeof(fdb_cmd_usage_vid)/4},
#if (defined HAVE_RTK_CMD && defined HAVE_SDL_CMD_ZTE)
    {fdb_cmd_clr_perport, "clr", "clear fdb table for static/dynamic entries", fdb_cmd_usage_clr, sizeof(fdb_cmd_usage_clr)/4},
#else
    {fdb_cmd_clr, "clr", "clear fdb table for static/dynamic entries", fdb_cmd_usage_clr, sizeof(fdb_cmd_usage_clr)/4},
#endif
    {fdb_cmd_add, "add", "Add one entry into SW table", fdb_cmd_usage_add, sizeof(fdb_cmd_usage_add)/4},
    {fdb_cmd_del, "del", "Del one entry in SW table", fdb_cmd_usage_del , sizeof(fdb_cmd_usage_del)/4},
//    {fdb_cmd_static, "static", "Config SW table into ASIC", fdb_cmd_usage_static, sizeof(fdb_cmd_usage_static)/4},

#else
    {fdb_cmd_show, "show"},// "show fdb configuration or all entires"},// fdb_cmd_usage_show, sizeof(fdb_cmd_usage_show)/4},
    {fdb_cmd_full, "full"},// "config fdb full_behavior"},// fdb_cmd_usage_full, sizeof(fdb_cmd_usage_full)/4},
    {fdb_cmd_limit, "limit"},// "config max mac number to learn"},// fdb_cmd_usage_limit, sizeof(fdb_cmd_usage_limit)/4},
    {fdb_cmd_aging, "aging"},// "aging timer. 0 - disable aging"},// fdb_cmd_usage_aging, sizeof(fdb_cmd_usage_aging)/4},
    {fdb_cmd_learn, "learn"},// "enable/disable mac learning"},// fdb_cmd_usage_learn, sizeof(fdb_cmd_usage_learn)/4},
    {fdb_cmd_vid, "vid"},// "learing VLAN id or not"},// fdb_cmd_usage_vid , sizeof(fdb_cmd_usage_vid)/4},
#if (defined HAVE_RTK_CMD && defined HAVE_SDL_CMD_ZTE)
    {fdb_cmd_clr_perport, "clr"},// "clear fdb table for static/dynamic entries", fdb_cmd_usage_clr, sizeof(fdb_cmd_usage_clr)/4},
#else
    {fdb_cmd_clr, "clr"},// "clear fdb table for static/dynamic entries"},// fdb_cmd_usage_clr, sizeof(fdb_cmd_usage_clr)/4},
#endif
    {fdb_cmd_add, "add"},// "Add one entry into SW table", fdb_cmd_usage_add, sizeof(fdb_cmd_usage_add)/4},
    {fdb_cmd_del, "del"},// "Del one entry in SW table", fdb_cmd_usage_del , sizeof(fdb_cmd_usage_del)/4},
    {fdb_cmd_static, "static"},// "Config SW table into ASIC", fdb_cmd_usage_static, sizeof(fdb_cmd_usage_static)/4},
#endif
};

sal_cmd_cfg_t fdb_cmd_cfg = {"fdb", "FDB configuration", fdb_cmd_list, sizeof(fdb_cmd_list)/sizeof(sal_cmd_reg_struct_t)};

 //#ifdef HAVE_SDL_CMD

cs_status fdb_cmd_self_reg(void)
{
        cs_status ret = CS_E_OK;
#ifdef HAVE_SDL_CMD
    
        ret = sal_cmd_reg(&fdb_cmd_cfg);
#endif
        return ret;
}

cs_status fdb_entry_dump (void)
{
    cs_uint8 index = 0, num = 0;
    cs_aal_fdb_entry_t fdb;
    cs_printf("================Valid FDB entries is shown:================\n");
    cs_printf("index static     mac_address     port aging   vid  dot1p\n");
    
    for (index = 0; index < sdl_int_cfg.max_fdb_entry; index++) {
        if (aal_fdb_entry_get(index, &fdb)) {
            return CS_E_ERROR;
        }
        if (fdb.vld == 0) {
            continue;
        }
        /* learning entry aging out */
        if (fdb.aging_status == 0 && fdb.static_flag == 0) {
            continue;
        }
        
        if ((fdb.mac.addr[0] == 0) &&
            (fdb.mac.addr[1] == 0) &&
            (fdb.mac.addr[2] == 0) &&
            (fdb.mac.addr[3] == 0) &&
            (fdb.mac.addr[4] == 0) &&
            (fdb.mac.addr[5] == 0)) {
            continue;
        }
        cs_printf(" %2d     %1d     %02x:%02x:%02x:%02x:%02x:%02x  %2d  %4d", index,
            fdb.static_flag, fdb.mac.addr[0], fdb.mac.addr[1], fdb.mac.addr[2], 
            fdb.mac.addr[3], fdb.mac.addr[4], fdb.mac.addr[5], fdb.port_id, fdb.aging_status);
        if (fdb.vid_vld == 0) {
            cs_printf("     NULL");
        } else {
            cs_printf("     %3d ", fdb.vid);
        }
        if (fdb.dot1p_vld == 0) {
            cs_printf(" NULL");
        } else {
            cs_printf(" %3d ", fdb.dot1p);
        }
        cs_printf("\n");
        num++;
        
    }
    cs_printf("================Totally %2d valid entries===================\n", num);
    return CS_E_OK;
}

#if (defined HAVE_RTK_CMD && defined HAVE_SDL_CMD_ZTE)
cs_status fdb_entry_dump_rtk (cs_port_id_t port)
{
    cs_callback_context_t  context;
    cs_sdl_fdb_entry_get_mode_t mode = SDL_FDB_ENTRY_GET_MODE_ALL;
    cs_sdl_fdb_entry_t fdb;
    cs_uint16 offset=0;
    cs_uint16 index = 0;
    cs_uint32 num = 0;
    cs_status rt = CS_E_OK;
    
    UNI_PORT_CHECK(port);  
    
    cs_printf("================Valid FDB entries is shown:================\n");

    while(1){    
        memset(&fdb, 0, sizeof(fdb));
        rt = epon_request_onu_fdb_entry_get_byindex_per_port(context, 0,  0,  port, mode, offset, &fdb, &index);
        if(rt)
            break;
        
        cs_printf("Entry index : %d  \n", index);
        cs_printf("Entry type  : %d  \n", fdb.type);
        cs_printf("Entry port  : %d  \n", fdb.port); 
        cs_printf("Entry VLAN  : %d  \n", fdb.vlan_id);
        cs_printf("MAC address : %02x:%02x:%02x:%02x:%02x:%02x\n\n", 
                                 fdb.mac.addr[0], 
                                 fdb.mac.addr[1], 
                                 fdb.mac.addr[2], 
                                 fdb.mac.addr[3], 
                                 fdb.mac.addr[4], 
                                 fdb.mac.addr[5]);
        
        offset = index;
        num++;        
        if(num % 10 == 0) {
            cs_thread_delay(10);
        }        
    }
    cs_printf("================Totally %2d valid entries===================\n", num);

    return CS_E_OK;
}
#endif

#endif


