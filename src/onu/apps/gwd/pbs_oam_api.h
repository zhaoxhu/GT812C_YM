/*                   Copyright(c) 2008-2015 GWTT, Inc.                    */
/*  
**  pbs_oam_api.h -  Header file 
**
**  This file was written by zhaoxh, 19/04/2017
**  
**  Changes:
**
**  Version       |  Date          |    Change        |    Author	  
**  ----------|-----------|-------------|------------
**	1.00          | 19/04/2017 |	creation	      |    zhaoxh
*/
#ifndef _PBOSHI_
#define _PBOSHI_
#include "cs_types.h"
#if defined(__cplusplus)
extern "C" {
#endif

/**************************************************define struct************************************************/
//#include "oam_std_pdu.h"
//#include "app_oam_ctc.h"
/*mac\u5730\u5740\u5b9a\u4f4donu uni\u7aef\u53e3,macLocatePort_r\u6536\u5230olt\u7684\u7ed3\u6784\u4f53\uff0cmacLocatePort_s onu\u56de\u590dolt\u7ed3\u6784\u4f53*/
typedef struct{
    cs_boolean valid;
    cs_uint8  idxBranch;
    cs_uint16 idxLeaf;
    cs_uint32 idxLen;
    cs_uint32 idxValue;
    cs_uint8  idxType; /* eth, e1, voip */
}ctc_oam_mgmt_obj_idx_t; 
typedef struct {
        cs_uint8           branch;
        cs_uint16          leaf;
        cs_uint8           width;
} __attribute__((packed)) oam_var_cont_t;
typedef struct macLocatePort_r{
         oam_var_cont_t hdr;
	unsigned char mac[6];
	unsigned short vlan_id;
}__attribute__((packed)) macLocatePort_r;

typedef struct macLocatePort_s{
	oam_var_cont_t hdr;
	unsigned char result;
	unsigned char mac[6];
	unsigned short vlan_id;
	unsigned  char port_id;
	unsigned char mac_type;
	
}__attribute__((packed)) macLocatePort_s;

/*onu\u7aef\u53e3mac\u5730\u5740\u6570\u76ee\u67e5\u8be2*/
typedef struct macNumber_t{
         oam_var_cont_t hdr;
	unsigned short mac_number;
}__attribute__((packed)) macNumber_t;

/*onu mac\u5730\u5740\u8868*/
typedef struct  allMac{
	unsigned char mac[6];
	unsigned short vlan_id;
	unsigned char port_id;
	unsigned char mac_type;
}__attribute__((packed))  allMac_t;
typedef struct  macType{
         oam_var_cont_t hdr;
	unsigned char mac_type;
}__attribute__((packed)) macType_t;

/*onu device name */
typedef struct onuDeviceName{
         oam_var_cont_t hdr;
	unsigned char device_name[128];
}__attribute__((packed)) onuDeviceName_t;

/*onu Device Description*/
typedef struct onuDeviceDescription{
          oam_var_cont_t hdr;
	unsigned char device_description[127];
}__attribute__((packed)) onuDeviceDescription_t;

/*onu device location*/
typedef struct onuDeviceLocation{
         oam_var_cont_t hdr;
	unsigned char device_location[128];
}__attribute__((packed)) onuDeviceLocation_t;

/*onu \u7aef\u53e3\u9694\u79bb\u72b6\u6001*/
typedef struct onuIsolation{
          oam_var_cont_t hdr;
	unsigned char isolation_stat;
}__attribute__((packed)) onuIsolation_t;

/*onu storm */
typedef struct onuStorm{
          oam_var_cont_t hdr;
	unsigned char storm_type;
	unsigned char storm_mode;
	unsigned int storm_rate;
}__attribute__((packed)) onuStorm_t;

/*onu \u7aef\u53e3\u53cc\u5de5\u901f\u7387\u6a21\u5f0f*/
typedef struct onuPortMode{
          oam_var_cont_t hdr;
	unsigned char port_mode;
}__attribute__((packed)) onuPortMode_t;

/*onu \u73af\u8def\u68c0\u67e5\u5173\u95ed\u65f6\u95f4*/
typedef struct onuLoopbackCloseTime{
          oam_var_cont_t hdr;
	unsigned short port_downtime;
	unsigned short port_restarttimes;
}__attribute__((packed)) onuLoopbackCloseTime_t;

/*onu \u672c\u5730\u914d\u7f6e\u4fdd\u5b58\u548c\u6062\u590d*/
typedef struct onuAction{
         oam_var_cont_t hdr;
	unsigned char action;
}__attribute__((packed)) onuAction_t;

/*onu\u6269\u5c55\u5c5e\u6027\u652f\u6301\u4fe1\u606f\u67e5\u8be2*/
typedef struct onuCapability{
          oam_var_cont_t hdr;
	unsigned char result;
	unsigned char  capability[16];
}__attribute__((packed)) onuCapability_t;
/*********************************************function declaration********************************************************/
unsigned int pbs_attr_oam_get_locate_mac( ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32 * procRecvLen,
        cs_uint8 * pOut);
unsigned int pbs_attr_oam_get_mac_number( ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32 * procRecvLen,
        cs_uint8 * pOut);
unsigned int pbs_attr_oam_get_mac_entry( ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32 * procRecvLen,
        cs_uint8 * pOut);
unsigned int pbs_attr_oam_set_device_name( ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32  procRecvLen,
        cs_uint8 * pOut);
unsigned int pbs_attr_oam_get_device_name( ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32 * procRecvLen,
        cs_uint8 * pOut);
unsigned int pbs_attr_oam_set_device_description( ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32  procRecvLen,
        cs_uint8 * pOut);
unsigned int pbs_attr_oam_get_device_description( ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32 * procRecvLen,
        cs_uint8 * pOut);
unsigned int pbs_attr_oam_set_device_locaction( ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32  procRecvLen,
        cs_uint8 * pOut);
unsigned int pbs_attr_oam_get_device_locaction( ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32 * procRecvLen,
        cs_uint8 * pOut);
unsigned int pbs_attr_oam_set_ioslation_status( ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32  procRecvLen,
        cs_uint8 * pOut);
unsigned int pbs_attr_oam_get_ioslation_status( ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32 * procRecvLen,
        cs_uint8 * pOut);
unsigned int pbs_attr_oam_set_onu_storm(  ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32  procRecvLen,
        cs_uint8 * pOut);
unsigned int pbs_attr_oam_get_onu_storm( ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32 * procRecvLen,
        cs_uint8 * pOut);
unsigned int pbs_attr_oam_set_port_mode( ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32 procRecvLen,
        cs_uint8 * pOut);
 unsigned int pbs_attr_oam_get_port_mode( ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32 * procRecvLen,
        cs_uint8 * pOut);
 unsigned int pbs_attr_oam_set_uni_loopback(  ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32  procRecvLen,
        cs_uint8 * pOut);
 unsigned int pbs_attr_oam_get_uni_loopback(  ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32 * procRecvLen,
        cs_uint8 * pOut);
 unsigned int pbs_attr_oam_set_onu_configuration(  ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32  procRecvLen,
        cs_uint8 * pOut);
 unsigned int pbs_attr_oam_get_function_capability( ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32 * procRecvLen,
        cs_uint8 * pOut);
 
#endif
