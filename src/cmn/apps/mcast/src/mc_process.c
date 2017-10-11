#include "mc_core.h"
#include "mc_parser.h"
#include "mc_adapter.h"
#include "mc_mgmt.h"

#if (GW_IGMP_TVM == MODULE_YES)
#include "../../gwd/gw_igmp_tvm.h"
#include "mc_api.h"
extern void pkt_print(char *buf, cs_uint16 len);
#endif

void mc_pkt_proc(cs_pkt_t *pkt, mc_node_t *node)
{    
    cs_status ret = CS_E_OK;
    cs_uint8   pkt_chg = 0;

	#if (GW_IGMP_TVM == MODULE_YES)
	GW_TVM_ENABLE_T enable;
	igmp_tvm_enable_status_get(&enable);
	if(GW_TVM_ENABLE == enable)
	{
		
		//gw��ʽ�Ŀ�vlan �鲥�������鲥snooping ģʽ��
		mc_mode_t mc_mode;
		mc_mode_get(0, &mc_mode);
		if(MC_SNOOPING == mc_mode)
		{
			char *eth_pkt = NULL;
			int len = 0;
			int port_id;
			int vlan_oper = 0;
			int vlan_id_cur =0;
			eth_pkt = (char *)(pkt->data + pkt->offset);
			len = pkt->len;
			port_id = (int)(node->port[pkt->port]->portid);
			gw_igmp_tvm_pkt_proc(eth_pkt, &len, port_id, &vlan_oper, &vlan_id_cur);
			pkt->len = len;
			pkt->tag_num += vlan_oper;
			pkt->svlan = vlan_id_cur;			
		}
		else
		{
			cs_printf("gw igmp tvm must wort at igmp snooping mode!please set igmp snooping mode!\n");
		}
	}
	#endif

    ret = mc_pkt_object_build(node, pkt, &node->message);
    if(ret) {
        return ;
    	}
    if(node->ing_filter) {
        mc_fwd_action_t fwd_action = node->ing_filter(pkt, node->message.pdu_type, &pkt_chg);
        if(MC_PKT_DROP == fwd_action) {
            return;
        }

        if(pkt_chg) {
            mc_pkt_obj_update(pkt, &node->message);
        }

         if(MC_PKT_BYPASS == fwd_action) {
            mc_port_t *port = node->message.port;
            
            if(mc_is_uplink_port(node, port->portid)) {
                mc_flood_msg(node, &node->message);
            }
            else {
                mc_fwd_msg(node->up_port, node->message.pdu_type, 
                    node->message.frame, node->message.len);
            }
            
            return;
        }
    }
    
    switch(node->message.pdu_type) { 
        case IGMP_V1_REPORT:
        case IGMP_V2_REPORT:
        case IGMP_V3_REPORT:
        case IGMP_V2_LEAVE:
        case MLD_V1_REPORT:
        case MLD_V2_REPORT:
        case MLD_V1_DONE: 
            mc_report_proc(node, &node->message);
            break;

        case IGMP_QUERY:
        case MLD_QUERY:
            mc_query_proc(node, &node->message);
            break;

        default:
            break;
    }

    return ;

}

cs_status mc_fwd_msg(mc_port_t *port, cs_uint8 pdu_type, cs_uint8 *frame, cs_uint16 len)
{
    mc_node_t *node = port->mc_node;
    if(node->eg_filter) {
        if(node->eg_filter(port->portid, pdu_type, frame, &len)) {
			//cs_printf("mc mes error\n");
            return CS_E_ERROR;
        }
    }
    
    /* tx port stats */
    switch (pdu_type) { 
        case    IGMP_V1_REPORT:
            port->out_igmpv1_report ++;
            break;
            
        case    IGMP_V2_REPORT:
            port->out_igmpv2_report ++;
            break;
            
        case    IGMP_V3_REPORT:
            port->out_igmpv3_report ++;
            break;
            
        
        case    IGMP_V2_LEAVE:
            port->out_igmpv2_leave ++;
            break;
            
        case    MLD_V1_REPORT:
            port->out_mldv1_report ++;
            break;

        case    MLD_V2_REPORT:
            port->out_mldv2_report++;
            break;
            
        case    MLD_V1_DONE: 
            port->out_mldv1_done ++;
            break;
            
        case    IGMP_QUERY:
            port->out_igmp_query++;
            break;
            
        case    MLD_QUERY:
            port->out_mld_query++;
            break;

        default:
            break;
    }
   
    return mc_frame_send(node->device, port->portid, frame, len);
}

cs_status mc_flood_msg(mc_node_t *mc_node, mc_object_t *pObj)
{
    cs_status   ret = CS_E_OK;
    cs_port_id_t port_id;
    mc_port_t *port = NULL;

    cs_uint8 *frame = NULL;

    if(mc_node->port_num > 2) {
        frame = mc_malloc(pObj->len);
    }
    else {
        frame = pObj->frame;
    }
#if (1)
    for(port_id = 0; port_id < mc_node->port_num; port_id++) {            
#else
    	port_id = 0;
#endif
        if(!mc_is_uplink_port(mc_node, port_id)) {
            /* not host port */
            port = mc_port_find(mc_node, port_id);
            if (NULL == port) {
                return CS_E_NOT_FOUND;
            }
            else{
                if(frame != pObj->frame) {
                    memcpy(frame, pObj->frame, pObj->len);
                }
                
                ret = mc_fwd_msg(port, pObj->pdu_type, frame, pObj->len);
                MC_LOG(IROS_LOG_LEVEL_DBG3, "%s,forward query to port %d,ret %d\n",__func__,port_id,ret);
            }
        }
#if (1)
    }
#endif

    if(frame != pObj->frame) {
        mc_free(frame);
    }

    return ret;
}
#if 1
extern void pkt_print(char *buf, cs_uint16 len)
{
    int i;
	cs_printf("pkt len :%d\n", len);
    cs_printf("---------------------------------------------------------");
    for(i = 0; i<len; ++i)
    {
        if(0 == (i&0xf)) cs_printf("\n%04x: ", i);
        if(0 == (i&0x7)) cs_printf(" ");
        cs_printf("%02x ", buf[i]);
    }
    cs_printf(".\n---------------------------------------------------------\n");

}
#endif


