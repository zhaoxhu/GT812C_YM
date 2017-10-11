#include "app_timer.h"
#include "cs_cmn.h"
#include "sdl_pktctrl.h"
#include "sdl_ma.h"
#include "sdl_mc.h"
#include "sdl_vlan.h"
#include "startup_cfg.h"
#include "mc_api.h"
#include "app_vlan.h"
#include "packet.h"
#ifdef HAVE_MC_CTRL
app_timer_msg_t  g_mc_static_tmr;
mc_tmr_handler_t g_mc_static_tmr_handler = NULL;

cs_status gmp_init()
{
    mc_up_port_cfg_t up_cfg;
    
    if(mc_init()) {
        return CS_E_ERROR;
    }
    
    up_cfg.up_port = CS_PON_PORT_ID;
    up_cfg.up_port_lrn = 0;
    mc_up_port_set(0, &up_cfg);

    mc_mode_set(0, MC_DISABLE);

    return CS_E_OK;
}

void gmp_timer_handle()
{
    if(g_mc_static_tmr_handler) {
        g_mc_static_tmr_handler();
    }
}

static cs_status igmp_mld_parser(cs_pkt_t *pPkt)
{   
    cs_uint8 *payload;

    payload = pPkt->data + pPkt->offset + 
        sizeof(cs_ether_header_t) + pPkt->tag_num * sizeof(cs_vlan_hdr_t );
    
    if( (pPkt->eth_type == EPON_ETHERTYPE_IP)
        && (payload[9] == IP_PROTOCOL_IGMP) ) { /* protocol */
        pPkt->pkt_type = CS_PKT_GMP; 
        return CS_E_OK;
    }

    if( (pPkt->eth_type == EPON_ETHERTYPE_IPV6)
        && (payload[6] == IP_PROTOCOL_MLD) ) {    /* next_header */
        pPkt->pkt_type = CS_PKT_GMP;
        return CS_E_OK;
    }
    
    return CS_E_NOT_SUPPORT;
        
}

/* 
 * util 
 */
 
void * mc_malloc(cs_uint32 size)
{
    return iros_malloc(IROS_MID_IGMP, size);
}

void mc_free(void * ptr)
{
    iros_free(ptr);
}

cs_uint8  mc_device_port_num_get(cs_dev_id_t dev)
{
    cs_uint8 port_num;
    
    startup_config_read(CFG_ID_SWITCH_PORT_NUM, 1, &port_num);
    port_num++; /* add pon port */
    
    return port_num;
}

cs_uint16 mc_device_grp_cap(cs_dev_id_t dev)
{
    cs_uint8 port_num;
    
    startup_config_read(CFG_ID_SWITCH_PORT_NUM, 1, &port_num);

    if(port_num == 4) {
        return 256;
    }
    else {  /* 1-port */
        return 64;
    }
}

cs_uint16 mc_device_port_grp_cap(cs_dev_id_t dev)
{
    cs_uint8 port_num;
    
    startup_config_read(CFG_ID_SWITCH_PORT_NUM, 1, &port_num);

    if(port_num == 4) {
        return 255;
    }
    else {  /* 1-port */
        return 64;
    }
}

cs_status mc_frame_send(cs_dev_id_t dev, cs_port_id_t port, cs_uint8 *frame, cs_uint16 len)
{
    return app_pkt_tx(port, CS_PKT_GMP, frame, len);
}

cs_status mc_timer_init(mc_tmr_handler_t hdl)
{
    cs_int32 tmr = 0;

    g_mc_static_tmr.msg_type = IROS_MSG_TIME_OUT;
    g_mc_static_tmr.timer_type = APP_TIMER_STATIC_IGMP;
    
    tmr = cs_msg_circle_timer_add(app_msg_q, 1000, (void *)&g_mc_static_tmr);
    g_mc_static_tmr_handler = hdl;

    return CS_E_OK;
}   

#if (PRODUCT_CLASS == PRODUCTS_GT812C)
unsigned char mc_ctl_enable_flag = 0;
#endif
cs_status mc_ctrl_enable(cs_dev_id_t dev, cs_boolean enable)
{
#if 0
	cs_printf("in mc_ctrl_enable, enable :0x%x\n", enable);
#endif

    cs_callback_context_t context;
    cs_sdl_pkt_dst_t      state;
    cs_port_id_t port_id;
    cs_uint32 port_num;

    cs_pkt_parser_t parser = NULL;
    cs_pkt_handler_t handler = NULL;

    if(enable) {
        parser = igmp_mld_parser;
        handler = mc_recv_frame;
    }
    
    app_pkt_reg_parser(CS_PKT_GMP, parser);
    app_pkt_reg_handler(CS_PKT_GMP, handler);

    state = enable ? DST_CPU : DST_FE;    

	#if (PRODUCT_CLASS == PRODUCTS_GT812C)
	mc_ctl_enable_flag = enable;
	#endif

    epon_request_onu_spec_pkt_dst_set(context, 0, 0, CS_UP_STREAM, CS_PKT_GMP, state);
    epon_request_onu_spec_pkt_dst_set(context, 0, 0, CS_DOWN_STREAM, CS_PKT_GMP, state);

	#if 0
	cs_printf("in mc_ctrl_enable, state :0x%x\n", state);
	#endif
	


    port_num = mc_device_port_num_get(0);

#if 0
    cs_sdl_mc_l2_entry_t entry;
    cs_port_id_t port;
    entry.mac.addr[0] = 0x01;
    entry.mac.addr[1] = 0x00;
    entry.mac.addr[2] = 0x5e;
    entry.mac.addr[3] = 0x00;
    entry.mac.addr[4] = 0x00;
    entry.mac.addr[5] = 0x01;
    cs_uint16 def_vlan = 0;
	def_vlan = port_def_vlan_id_get(port_id);
    entry.vlan = def_vlan;
#endif

    for(port_id = 1; port_id < port_num; port_id++) {
#if 0
    	port = port_id;
    	FOR_UNIT_START(GT_U32, unit)
    	InternalVlanInit(QD_DEV_PTR, __c_vlan_table[i].vlan.vid);
    	FOR_UNIT_END
		/* Broadcast atu entry */
	    atuEntryBc.DBNum = vid;
	    atuEntryBc.exPrio.macFPri = 0;
	    atuEntryBc.exPrio.macQPri = 0;
	    atuEntryBc.exPrio.useMacFPri = GT_FALSE;
	    atuEntryBc.prio = 0;
	    atuEntryBc.trunkMember = GT_FALSE;
	    for (i=0; i<GT_ETHERNET_HEADER_SIZE; i++)
	        atuEntryBc.macAddr.arEther[i] = 0xFF;
	    atuEntryBc.portVec = 0;
	    atuEntryBc.portVec |= (1<<(dev->cpuPortNum));
#if (FOR_ONU_PON)
	    atuEntryBc.portVec |= (1<<(dev_wanPort[0]));	/* Need change */
#endif

	    atuEntryBc.entryState.mcEntryState = GT_MC_STATIC;
	    if ((result = gfdbAddMacEntry(dev, &atuEntryBc)) != GT_OK)
		{
			MSG_OUT(( "gfdbAddMacEntry return Failed(%d)\r\n", result));
			l_ret_val = result;
		}
#endif

    	/*if enable block unknow multicast pkts(data or rule),disable wiil be igmp transparent mode*/
        epon_request_onu_unknown_mc_forward_set(context, 0, 0, port_id, enable);

        /*no matter igmp is snooping or transparent sw snoop should always being disabled,
         if it is snooping, igmp report pkt will be removed vlan tag zhangjj 2015-1-21*/
        epon_request_sw_snoop_set(context,0,0,port_id,enable);
    }
                
    return CS_E_OK;
}

static void mc_fdb_map_to_mac(mc_fdb_entry_t *fdb, cs_uint8 *mac)
{
    if(fdb->mc_grp.type == MC_IPV4) {
        mac[0] = 0x01;
        mac[1] = 0x00;
        mac[2] = 0x5e;
        
        mac[3] = (fdb->mc_grp.addr.ipv4 >> 16) & 0x7f;
        mac[4] = (fdb->mc_grp.addr.ipv4 >> 8) & 0xff;
        mac[5] = fdb->mc_grp.addr.ipv4 & 0xff;            
    }
    else {
        mac[0] = 0x33;
        mac[1] = 0x33;

        mac[2] = fdb->mc_grp.addr.ipv6.mc_addr8[15];
        mac[3] = fdb->mc_grp.addr.ipv6.mc_addr8[14];
        mac[4] = fdb->mc_grp.addr.ipv6.mc_addr8[13] ;
        mac[5] = fdb->mc_grp.addr.ipv6.mc_addr8[12];
    }
}

static void mc_fdb_to_sdl_entry(mc_fdb_entry_t *fdb, cs_sdl_mc_ip_entry_t *entry)
{
    cs_uint8 src_id;
    
    entry->vlan = fdb->vlanid;        

    /* grp */
    if(MC_IPV4 == fdb->mc_grp.type) {
        entry->grp.ipver = CS_IPV4;
        entry->grp.addr.ipv4 = fdb->mc_grp.addr.ipv4;
    }
    else {
        cs_uint8 i;
        entry->grp.ipver = CS_IPV6;
        for(i=0; i<8; i++) {            
            entry->grp.addr.ipv6[i] = fdb->mc_grp.addr.ipv6.mc_addr16[i%2 ? (i-1) : (i+1)];
        }
    }

    /* filter mode */
    if(SRC_INCLUDE == fdb->filter_mode) {
        entry->type = SDL_MC_IP_ENTRY_TYPE_SRC_INCLUDE;
    }
    else {
        if(fdb->src_num) {
            entry->type = SDL_MC_IP_ENTRY_TYPE_SRC_EXCLUDE;
        }
        else {
            entry->type = SDL_MC_IP_ENTRY_TYPE_GRP_EXCLUDE;
        }
    }

    /* src list */
    if(fdb->src_num > MC_IP_SRC_NUM_MAX_PER_ENTRY) {
        fdb->src_num = MC_IP_SRC_NUM_MAX_PER_ENTRY;
    }
    entry->src_num = fdb->src_num;
    for(src_id = 0; src_id < entry->src_num; src_id++) {
        if(MC_IPV4 == fdb->mc_grp.type) {
            entry->src[src_id].ipver = CS_IPV4;
            entry->src[src_id].addr.ipv4 = fdb->src_ip_tbl[src_id].addr.ipv4;
        }
        else {
            cs_uint8 i;            
            for(i=0; i<8; i++) {     
                entry->src[src_id].ipver = CS_IPV6;
                entry->src[src_id].addr.ipv6[i] = fdb->src_ip_tbl[src_id].addr.ipv6.mc_addr16[i%2 ? (i-1) : (i+1)];
            }
        }
    }
}
 

cs_status  mc_port_fdb_add(mc_fdb_entry_t *fdb)
{                     
    cs_status ret = CS_E_NOT_SUPPORT;
    cs_callback_context_t context;

    mc_fwd_engine_t engine; 

    mc_fwd_engine_get(0, &engine);

    if(MC_ENGINE_L2 == engine) {        
        cs_sdl_mc_l2_entry_t entry;
        cs_sdl_portmask_t  portmap;

        mc_fdb_map_to_mac(fdb, entry.mac.addr);
        entry.vlan = fdb->vlanid;

        ret = epon_request_onu_mc_l2_port_get(context, 0, 0, &entry, &portmap);
        if(ret != CS_E_NOT_FOUND) {
            if( ret || (portmap.bits[0] & (1<<(fdb->portid-1)))) {
                return CS_E_ERROR;
            }
        }
#if 0

        cs_printf("portmap is %x\n",portmap);
        cs_printf("portid is %x\n",fdb->portid);
        cs_sdl_mc_l2_entry_t entry_normal;
        entry_normal.vlan = fdb->vlanid;
        entry_normal.mac.addr[0] = 0x01;
        entry_normal.mac.addr[1] = 0x00;
        entry_normal.mac.addr[2] = 0x5e;
        entry_normal.mac.addr[3] = 0x00;
        entry_normal.mac.addr[4] = 0x00;
        entry_normal.mac.addr[5] = 0x01;

        portmap = 0;
        ret = epon_request_onu_mc_l2_port_get(context, 0, 0, &entry_normal, &portmap);
        if(ret != CS_E_NOT_FOUND) {
            if( ret || (portmap.bits[0] & (1<<(fdb->portid-1)))) {
                return CS_E_ERROR;
            }
        }

        epon_request_onu_mc_l2_entry_add(context, 0, 0, fdb->portid, &entry_normal);
#else
        extern void __get_port_by_vlan(unsigned int vid);
        __get_port_by_vlan(fdb->vlanid);
#endif
        ret = epon_request_onu_mc_l2_entry_add(context, 0, 0, fdb->portid, &entry);

    }
    else if(MC_ENGINE_IP == engine) {
        cs_sdl_mc_ip_entry_t entry;

        mc_fdb_to_sdl_entry(fdb, &entry);
        
        ret = epon_request_onu_mc_ip_entry_add(context, 0, 0, fdb->portid, &entry);
    }

    return ret;
    
}

cs_status  mc_port_fdb_del(mc_fdb_entry_t *fdb)
{          
    cs_callback_context_t context;
    cs_status ret = CS_E_NOT_SUPPORT;
    
    mc_fwd_engine_t engine;        

    mc_fwd_engine_get(0, &engine);

    if(MC_ENGINE_L2 == engine) {        
        cs_sdl_mc_l2_entry_t entry;

        mc_fdb_map_to_mac(fdb, entry.mac.addr);
        entry.vlan = fdb->vlanid;
        
        ret = epon_request_onu_mc_l2_entry_del(context, 0, 0,fdb->portid, &entry);
    }
    else if(MC_ENGINE_IP == engine) {
        cs_ip_t grp;
        
        if(MC_IPV4 == fdb->mc_grp.type) {
            grp.ipver = CS_IPV4;
            grp.addr.ipv4 = fdb->mc_grp.addr.ipv4;
        }
        else {
            cs_uint8 i;
            
            grp.ipver = CS_IPV6;
            
            for(i=0; i<8; i++) {            
                grp.addr.ipv6[i] = fdb->mc_grp.addr.ipv6.mc_addr16[i%2 ? (i-1) : (i+1)];
            }
        }
        
        ret = epon_request_onu_mc_ip_entry_del(context, 0, 0, fdb->portid, grp, fdb->vlanid);
    }

    return ret;
    
}

cs_status  mc_port_fdb_update(mc_fdb_entry_t *old_entry, mc_fdb_entry_t *new_entry)
{             
    cs_callback_context_t context;
    mc_fwd_engine_t engine;

    cs_status ret = CS_E_NOT_SUPPORT;

    mc_fwd_engine_get(0, &engine);

    if(MC_ENGINE_L2 == engine) {        
        cs_sdl_mc_l2_entry_t entry;

        mc_fdb_map_to_mac(new_entry, entry.mac.addr);
        entry.vlan = new_entry->vlanid;
        
#if 1
        cs_sdl_mc_l2_entry_t entry_normal;
        entry_normal.vlan = new_entry->vlanid;
        entry_normal.mac.addr[0] = 0x01;
        entry_normal.mac.addr[1] = 0x00;
        entry_normal.mac.addr[2] = 0x5e;
        entry_normal.mac.addr[3] = 0x00;
        entry_normal.mac.addr[4] = 0x00;
        entry_normal.mac.addr[5] = 0x01;

        epon_request_onu_mc_l2_entry_add(context, 0, 0, new_entry->portid, &entry_normal);
#endif

        return epon_request_onu_mc_l2_entry_add(context, 0, 0, new_entry->portid, &entry);
    }
    else if(MC_ENGINE_IP == engine) {
        cs_sdl_mc_ip_entry_t entry;

        mc_fdb_to_sdl_entry(new_entry, &entry);
        
        ret = epon_request_onu_mc_ip_entry_add(context, 0, 0, new_entry->portid, &entry);
    }

    return ret;
    
}

#endif /* #ifdef HAVE_MC_CTRL */

