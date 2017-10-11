/*                   Copyright(c) 2008-2015 GWTT, Inc.                    */
/*  
**  pbs_oam_api.h -  Header file 
**
**  This file was written by zhaoxh, 20/04/2017
**  
**  Changes:
**
**  Version       |  Date          |    Change        |    Author	  
**  ----------|-----------|-------------|------------
**	1.00          | 20/04/2017 |	creation	      |    zhaoxh
*/
#ifndef _PBSOAM_
#define _PBSOAM_
#include"pbs_oam_api.h"
/* PBS ext opcode definitions */
#define OAM_PDU_PBS_VAR_REQ                 0x01
#define OAM_PDU_PBS_VAR_RESP                0x02
#define OAM_PDU_PBS_SET_REQ                 0x03
#define OAM_PDU_PBS_SET_RESP                0x04
#define OAM_PBS_ATTRIB_BRANCH            0xc7
#define  PBS_OAM_BUILD_RESP \
    xx(01, OAM_PBS_ATTRIB_BRANCH,OAM_PBS_LEAF_CODE_ONU_MAC, 0xC001,\
          pbs_attr_oam_get_locate_mac, NULL) \
    xx(02, OAM_PBS_ATTRIB_BRANCH,OAM_PBS_LEAF_CODE_ONU_MAC_NUMBER, 0xC002, \
          pbs_attr_oam_get_mac_number, NULL) \
    xx(03, OAM_PBS_ATTRIB_BRANCH,OAM_PBS_LEAF_CODE_ONU_MAC_ENTRY, 0xC003,\
          pbs_attr_oam_get_mac_entry, NULL) \
          \
    xx(04, OAM_PBS_ATTRIB_BRANCH, OAM_PBS_LEAF_CODE_ONU_DEVICE_NAME,0xC004,\
          pbs_attr_oam_get_device_name, pbs_attr_oam_set_device_name) \
    xx(05, OAM_PBS_ATTRIB_BRANCH,OAM_PBS_LEAF_CODE_ONU_DEVICE_DESPRIPTION, 0xC005,\
          pbs_attr_oam_get_device_description, pbs_attr_oam_set_device_description) \
    xx(06, OAM_PBS_ATTRIB_BRANCH,OAM_PBS_LEAF_CODE_ONU_DEVICE_LOCATION, 0xC006,\
	pbs_attr_oam_get_device_locaction, pbs_attr_oam_set_device_locaction) \
    xx(07, OAM_PBS_ATTRIB_BRANCH,OAM_PBS_LEAF_CODE_ONU_IOSLATION_STATUS, 0xC007,\
	pbs_attr_oam_get_ioslation_status, pbs_attr_oam_set_ioslation_status) \
    xx(08, OAM_PBS_ATTRIB_BRANCH,OAM_PBS_LEAF_CODE_ONU_STORM, 0xC008,\
	pbs_attr_oam_get_onu_storm, pbs_attr_oam_set_onu_storm) \
    xx(09, OAM_PBS_ATTRIB_BRANCH,OAM_PBS_LEAF_CODE_ONU_PORT_MODE, 0xC009,\
	pbs_attr_oam_get_port_mode, pbs_attr_oam_set_port_mode) \
    xx(10, OAM_PBS_ATTRIB_BRANCH,OAM_PBS_LEAF_CODE_ONU_LOOPBACK, 0xC00A,\
	pbs_attr_oam_get_uni_loopback, pbs_attr_oam_set_uni_loopback) \
	\
    xx(11, OAM_PBS_ATTRIB_BRANCH,OAM_PBS_LEAF_CODE_ONU_CONFIGURATION, 0xC00B,\
	NULL, pbs_attr_oam_set_onu_configuration) \
	\
    xx(12, OAM_PBS_ATTRIB_BRANCH,OAM_PBS_LEAF_CODE_ONU_CAPABILITY, 0xC101,\
	pbs_attr_oam_get_function_capability, NULL) 


typedef enum{
    #undef xx
    #define xx(SEQ,BRANCH,LEAF,LEAF_VAL,GET_FUNC,SET_FUNC) LEAF=LEAF_VAL,
    PBS_OAM_BUILD_RESP
}pbs_oam_leaf_e;

typedef cs_uint8               oam_oui_t[3];
typedef struct {
        cs_uint8           dst_mac[6];
        cs_uint8           src_mac[6];
        cs_uint16          eth_type;
        cs_uint8           sub_type;
        cs_uint16          flags;
        cs_uint8           code;
} __attribute__((packed)) oam_pdu_hdr_t;
typedef struct {
        oam_pdu_hdr_t      hdr;
        cs_uint8           oui[3];
        cs_uint8           opcode;
        cs_uint8           data[1];
} __attribute__((packed)) oam_pdu_ctc_t;
typedef struct {
        cs_uint8           branch;
        cs_uint16          leaf;
} __attribute__((packed)) oam_var_desc_t;
typedef cs_uint32 (*ctc_oam_get_handler_t)(
        ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32 * procRecvLen,
        cs_uint8 * pOut);

typedef cs_uint32 (* ctc_oam_set_handler_t)(
        ctc_oam_mgmt_obj_idx_t index,
        cs_uint8   * pRecv,
        cs_uint32    Recvlen,
        cs_uint8   * pOut);


typedef void (* oam_vendor_handler_t)(
        cs_port_id_t port,
        cs_uint8 *frame,
        cs_uint32 length);

typedef cs_uint32 (* oam_vendor_build_handler_t)(
        cs_port_id_t port,
        cs_uint8 *frame,
        cs_uint32 length);

typedef struct {
        cs_uint8   branch;
        cs_uint16  leaf;
        cs_uint8   width;
        cs_uint8   data[1];
} __attribute__((packed)) oam_ctc_onu_var_set_hdr_t;
typedef struct {
    cs_uint8 branch;
    cs_uint16 leaf;
    ctc_oam_get_handler_t get_func;
    ctc_oam_set_handler_t set_func;
}ctc_oam_handle_t;
typedef struct{
    oam_vendor_handler_t            ext_info_proc_handler;
    oam_vendor_build_handler_t      ext_info_build_handler;
    oam_vendor_handler_t            info_proc_handler;
    oam_vendor_build_handler_t      info_build_handler;
    oam_vendor_handler_t            org_handler;
}oam_vendor_handlers_t;
cs_int32 pbs_oam_build_var_resp(
        cs_uint32 port, 
        cs_uint8 *frame, 
        cs_uint32 length);
cs_int32 pbs_oam_build_set_resp(
        cs_uint32 port, 
        cs_uint8 *frame, 
        cs_uint32 length);
cs_uint32 pbs_oam_build_get_tlv(
        oam_oui_t oui,
        cs_uint8 * pOutVarCont,
        oam_var_desc_t * pRecvVarCont,
        cs_uint32 * procLen,
        ctc_oam_mgmt_obj_idx_t mgmtIndex,
        cs_boolean * idxBuild);
cs_uint32 pbs_oam_build_set_tlv(
        oam_oui_t oui,
        cs_uint8 * pOutVarCont,
        cs_uint8 * pRecv,
        cs_uint32  tlvLen,
        ctc_oam_mgmt_obj_idx_t mgmtIndex,
        cs_boolean * idxBuild);
ctc_oam_get_handler_t pbs_oam_find_get_handler(
        oam_oui_t oui,
        cs_uint8 branch, 
        cs_uint16 leaf);
ctc_oam_set_handler_t pbs_oam_find_set_handler(
        oam_oui_t oui,
        cs_uint8 branch, 
        cs_uint16 leaf);
void
pbs_oam_handle(
        cs_uint32 port, 
        cs_uint8 *frame, 
        cs_uint32 len);

#endif

