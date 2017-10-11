 /* 				  Copyright(c) 2008-2015 GWTT, Inc. 				   */
 /*  
 **  pbs_oam_api.c -  c file 
 **
 **  This file was written by zhaoxh, 22/03/2017
 **  
 **  Changes:
 **
 **  Version	   |  Date			|	 Change 	   |	Author	   
 **  ----------|-----------|-------------|------------
 **  1.00		   | 22/03/2017 |	 creation		   |	zhaoxh
 */
#include"pbs_oam_api.h"
#include "sdl_port.h"
#include "oam.h"
#include "sdl_fdb.h"

cs_uint8 extern_flag = 0;
 #define GW_OK				CS_E_OK
#define GW_RETURN_FAIL		CS_E_ERROR
 extern  cs_status epon_request_onu_fdb_entry_get_byindex( 
	  CS_IN  cs_callback_context_t		   context,
	  CS_IN  cs_int32					   device_id,
	  CS_IN  cs_int32					   llidport,
	  CS_IN  cs_sdl_fdb_entry_get_mode_t   mode,
	  CS_IN  cs_uint16					   offset,
	  CS_OUT cs_sdl_fdb_entry_t 		   *entry,
	  CS_OUT cs_uint16					   *next
  );

  unsigned int pbs_attr_oam_get_locate_mac( ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32 * procRecvLen,
        cs_uint8 * pOut)
  {
	  unsigned char flag ;
	  cs_callback_context_t context;
	  cs_uint16 idx = 0, next = 0;
	  cs_sdl_fdb_entry_t entry;
	   memset(&entry, 0, sizeof(cs_sdl_fdb_entry_t));
	  cs_printf("pbs_attr_oam_get_locate_mac\n");
	  macLocatePort_r *inData = ( macLocatePort_r*)pRecV;
	  macLocatePort_s *outData = (macLocatePort_s*)pOut;
	  while(epon_request_onu_fdb_entry_get_byindex(context, 0, 0, SDL_FDB_ENTRY_GET_MODE_ALL, idx, &entry, &next) == CS_OK)
          {
    	 	cs_uint16 vid = entry.vlan_id?entry.vlan_id:1;
		idx = next;
		 if(memcmp(entry.mac.addr,inData->mac,6) && inData->vlan_id == vid)
			 {
				 memcpy(outData->mac,entry.mac.addr,6);
				 outData->vlan_id = vid;
				 outData->port_id = entry.port;
				 outData->result = 0x01;
				 outData->mac_type = entry.type+1;
				 flag = 1;//\ufffd\ufffd\ufffd\ufffd\u04b5\ufffd\ufffd\ufffd\u05be\u03bb\u03aa1
				 break;
			 }
			 else{
				 flag = 0;
			 }
    	
          }
	  outData->hdr.width = 0x0b;
	  if(1 == flag)
	  {
		  return sizeof(oam_var_cont_t)+outData->hdr.width;
	  }
	  else{
		  outData->result = 0x02;
		  return sizeof(oam_var_cont_t)+outData->hdr.width;
	  }
}

 unsigned int pbs_attr_oam_get_mac_number( ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32 * procRecvLen,
        cs_uint8 * pOut)
  {
	  macNumber_t *outData = (macNumber_t*)pOut;
	  unsigned short macNum = 0;
	  cs_callback_context_t context;
	  cs_uint16 idx = 0, next = 0;
	  cs_sdl_fdb_entry_t entry;
	  memset(&entry, 0, sizeof(cs_sdl_fdb_entry_t));
	   while(epon_request_onu_fdb_entry_get_byindex(context, 0, 0, SDL_FDB_ENTRY_GET_MODE_ALL, idx, &entry, &next) == CS_OK)
           {
		idx = next;
		macNum++;
           }
	   cs_printf("macNum = %d\n",macNum);
	outData->mac_number = htons(macNum);
	  outData->hdr.width = 0x02;
	  return sizeof(oam_var_cont_t)+outData->hdr.width;
           

  }
 
  cs_uint32 AllMacCount = 0;
  cs_uint32 AllMacEntryCount = 0;
  unsigned char flag = 0;
  allMac_t *macEntry = NULL;
  unsigned int pbs_attr_oam_get_mac_entry( ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32 * procRecvLen,
        cs_uint8 * pOut)
  {
        	  cs_callback_context_t context;
	  cs_uint16 idx = 0, next = 0,vid;
	  cs_sdl_fdb_entry_t entry;
	  memset(&entry, 0, sizeof(cs_sdl_fdb_entry_t));
	  macType_t *inData = (macType_t *)pRecV;
	  oam_var_cont_t hdr;
	  allMac_t tempMac;
	   allMac_t *temp  = NULL;
	  cs_printf("pbs_attr_oam_get_mac_entry\n");
	  /*get all mac */
	  if(0 == flag)
	  {
	  	 while( epon_request_onu_fdb_entry_get_byindex(context, 0, 0, inData->mac_type-1, idx, &entry, &next) == CS_OK)
		{
			flag = 1;
			idx = next;
			AllMacEntryCount++;	
		}
		AllMacCount = AllMacEntryCount;	  
		if(NULL == (temp  = (allMac_t*)malloc(sizeof(allMac_t)*AllMacEntryCount)))
		{
			cs_printf("pbs malloc error\n");
		         return -1;
		}
	         macEntry  = temp;
		memset(&entry, 0, sizeof(cs_sdl_fdb_entry_t));
		idx = 0;
		next = 0;
		  while(epon_request_onu_fdb_entry_get_byindex(context, 0, 0,  inData->mac_type-1, idx, &entry, &next) == CS_OK)
		  {
		 	vid = entry.vlan_id?entry.vlan_id:1;
			idx = next;
			memcpy(tempMac.mac,entry.mac.addr,6);
			tempMac.mac_type = entry.type+1;
			tempMac.port_id = entry.port;
			tempMac.vlan_id = htons(vid);
		          memcpy(temp,&tempMac,sizeof(allMac_t));
		          temp++;
				  
		  }
	  }	
         /*get all mac over*/
      
	 cs_uint16 mac_number =0 ;

 
	 if(AllMacEntryCount < 20)
	 {
		 mac_number = AllMacEntryCount;
		 extern_flag = 1;
	 }
	 else
	 {
		 mac_number = 20;
		 AllMacEntryCount -=20;
	 }
          hdr.width = 6+10*mac_number;
          pOut += (sizeof(hdr)-1);
	 memcpy(pOut,&(hdr.width),1);
	 pOut += 1;
	 cs_uint32 tempAllMacCount = htonl(AllMacCount);
	 memcpy(pOut,&tempAllMacCount,sizeof(AllMacCount));
	 pOut += 4;
	 cs_uint16 tempmac_number = htons(mac_number);
	 memcpy(pOut,&tempmac_number,sizeof(mac_number));
	 pOut += 2;
	 memcpy(pOut,macEntry,mac_number*sizeof(allMac_t));
	 macEntry += mac_number;
          return sizeof(oam_var_cont_t)+hdr.width;
	 }
  
  extern ONU_SYS_INFO_TOTAL onu_system_info_total;
  extern void   Onu_Sysinfo_Get(void);
  extern int   Onu_Sysinfo_Save(void);
  unsigned int pbs_attr_oam_set_device_name( ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32 procRecvLen,
        cs_uint8 * pOut)
  {
            oam_var_cont_t *hdr = (oam_var_cont_t *)pOut;
	   onuDeviceName_t *deviceName = (onuDeviceName_t *)pRecV;
	  if(128 < strlen(deviceName->device_name))
	  {
		 hdr->width = 0x86;
	  }
	  Onu_Sysinfo_Get();
	  sprintf((char *)onu_system_info_total.device_name, "%s", deviceName->device_name);
	  if (GWD_RETURN_OK == Onu_Sysinfo_Save())
	 {
			 hdr->width  = 0x80;
	 }
	 else{
		hdr->width  = 0x87;
	 }
	 return sizeof(oam_var_cont_t);
  }
  
  unsigned int pbs_attr_oam_get_device_name( ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32 * procRecvLen,
        cs_uint8 * pOut)
  {
           onuDeviceName_t *deviceName = (onuDeviceName_t *)pOut;
	  unsigned char size_name = strlen((char *)onu_system_info_total.device_name);
	  Onu_Sysinfo_Get();
	  memcpy(deviceName->device_name,onu_system_info_total.device_name,size_name);
	 deviceName->hdr.width = size_name;
	 
	  return sizeof(oam_var_cont_t)+size_name;
  }
  
 unsigned int pbs_attr_oam_set_device_description( ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32  procRecvLen,
        cs_uint8 * pOut)
  {
	  unsigned short size_name = 0;
	  oam_var_cont_t *hdr = (oam_var_cont_t *)pOut;
	  onuDeviceDescription_t *deviceDescription = (onuDeviceDescription_t *)pRecV;
	  size_name = strlen((char *)deviceDescription->device_description);
	   if(128 < size_name)
	   {
		 hdr->width= 0x86;
	   }
	  Onu_Sysinfo_Get();
	  
	  sprintf((char *)onu_system_info_total.device_description, "%s", deviceDescription->device_description);
	  if (GWD_RETURN_OK == Onu_Sysinfo_Save())
	 {
	 
			 hdr->width = 0x80;
	 }
	 else{
		 
		 hdr->width = 0x87;
	 }
	 return sizeof(oam_var_cont_t);
  }
  
  unsigned int pbs_attr_oam_get_device_description(  ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32 * procRecvLen,
        cs_uint8 * pOut)
  {
  	 onuDeviceDescription_t *deviceDescription = (onuDeviceDescription_t *)pOut;
	  unsigned char size_name = strlen((char *)onu_system_info_total.device_description);
	  Onu_Sysinfo_Get();
	  memcpy(deviceDescription->device_description,onu_system_info_total.device_description,size_name);
	  deviceDescription->hdr.width= size_name;
	  return sizeof(oam_var_cont_t)+size_name;
  }
  
  unsigned int pbs_attr_oam_set_device_locaction( ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32  procRecvLen,
        cs_uint8 * pOut)
  {
	  unsigned short size_name = 0;
	   oam_var_cont_t *hdr = (oam_var_cont_t *)pOut;
	  onuDeviceLocation_t *deveceLocation = (onuDeviceLocation_t *)pRecV;
	  size_name = strlen((char *)deveceLocation->device_location);
	   if(128 < size_name)
	   {
		 hdr->width= 0x86;
	   }
	  Onu_Sysinfo_Get();
	  sprintf((char *)onu_system_info_total.device_place, "%s", deveceLocation->device_location);
	  if (GWD_RETURN_OK == Onu_Sysinfo_Save())
	 {
	 
	 	 hdr->width = 0x80;

	 }
	 else{
		 
		 hdr->width = 0x87;
	 }
	 return sizeof(hdr);;
  }
  
  unsigned int pbs_attr_oam_get_device_locaction( ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32 * procRecvLen,
        cs_uint8 * pOut)
  {
	  unsigned char size_name = strlen((char *)onu_system_info_total.device_place);
	  onuDeviceLocation_t *deveceLocation = (onuDeviceLocation_t *)pOut;
	  Onu_Sysinfo_Get();
	  memcpy(deveceLocation->device_location,onu_system_info_total.device_place,size_name);
	  deveceLocation->hdr.width= size_name;
	   return sizeof(oam_var_cont_t)+size_name;
  }
  extern cs_status epon_request_onu_port_isolation_set(
CS_IN cs_callback_context_t     context,
CS_IN cs_int32                  device_id,
CS_IN cs_int32                  llidport,
CS_IN cs_boolean                enable
);
extern cs_status epon_request_onu_port_isolation_get(
	    CS_IN cs_callback_context_t     context,
	    CS_IN cs_int32                  device_id,
	    CS_IN cs_int32                  llidport,
	    CS_OUT cs_boolean               *enable
	);
  unsigned int pbs_attr_oam_set_ioslation_status( ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32  procRecvLen,
        cs_uint8 * pOut)
  {
        
	 CS_IN cs_callback_context_t     context = {0};
	 oam_var_cont_t *hdr = (oam_var_cont_t *)pOut;
	 onuIsolation_t *Isolation = (onuIsolation_t*)pRecV;
	 if(Isolation->isolation_stat>1)
	 {
	 	 hdr->width  = 0x86;
	 }
	 if(epon_request_onu_port_isolation_set(context, 0,0,Isolation->isolation_stat) != GW_OK)
	 {
	 
		 hdr->width  = 0x87;
	 }
          else{
		  	  hdr->width  = 0x80;
	}
   
	 return sizeof(oam_var_cont_t);
  }
  
  unsigned int pbs_attr_oam_get_ioslation_status( ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32 * procRecvLen,
        cs_uint8 * pOut)
  {
        	 CS_IN cs_callback_context_t     context = {0};
	 onuIsolation_t *Isolation = (onuIsolation_t*)pOut;
	 if(epon_request_onu_port_isolation_get(context, 0,0,&Isolation->isolation_stat) != GW_OK)
	 {
			 return GW_RETURN_FAIL;
	 }
	 Isolation->hdr.width = 1;
	 
	 return sizeof(oam_var_cont_t)+Isolation->hdr.width;
  }
 static onuStorm_t *temp = NULL;
  unsigned int pbs_attr_oam_set_onu_storm(  ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32  procRecvLen,
        cs_uint8 * pOut)
  {
    oam_var_cont_t *hdr = (oam_var_cont_t *)pOut;
	if(NULL == (temp  = (onuStorm_t*)malloc(sizeof(onuStorm_t))))
		{
			cs_printf("pbs malloc error\n");
		         return -1;
		}
	temp = (onuStorm_t *)pRecV;
	hdr->width =0x80;
	return sizeof(oam_var_cont_t);
  }
  
 unsigned int pbs_attr_oam_get_onu_storm( ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32 * procRecvLen,
        cs_uint8 * pOut)
  {
  		onuStorm_t *p = (onuStorm_t*)pOut;
		p = temp;
		return sizeof(oam_var_cont_t)+temp->hdr.width;
  }
 extern int port_mode_set(int port, int mode);
 extern int port_mode_get(int port, int *mode);
  unsigned int pbs_attr_oam_set_port_mode( ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32  procRecvLen,
        cs_uint8 * pOut)
  {
           onuPortMode_t* inData = (onuPortMode_t*)pRecV;
           oam_var_cont_t *hdr = (oam_var_cont_t *)pOut;
	  unsigned char port;
	  unsigned char flag = 0;
	  unsigned char mode;
	  
	 switch(inData->port_mode)
		 {
			 case 0x01:
				mode = 11;
				 break;
			 case 0x02:
				mode = 10;
				 break;
			 case 0x03:
				 mode = 9;
				 break;
			 case 0x04:
				mode = 8;
				 break;
			 case 0x05:
				mode = 12;
				 break;
			 default:
				hdr->width = 0x86;
				 break;
		 }
		 for( port =1; port <= 8; port++)
		 {
		 
			 if(GW_OK != port_mode_set(port,mode))
			 {
				 flag = 1;
			 }
		 }
		 if(0 == flag)
		 {

			 hdr->width = 0x80;
		 }
		 else{
			 hdr->width = 0x87;
		 }
	  return sizeof(oam_var_cont_t); 
  }
  
  unsigned int pbs_attr_oam_get_port_mode( ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32 * procRecvLen,
        cs_uint8 * pOut)
  {
           onuPortMode_t* outData = (onuPortMode_t*)pOut;
	  unsigned char port = 1;
	 int mode;
	  if(GW_OK != port_mode_get(port,&mode))
	  {
		  return GW_RETURN_FAIL;
	  }
	  switch(mode)
		 {
			case 11:
				outData->port_mode= 0x01;
				 break;
			 case 10:
				outData->port_mode = 0x02;
				 break;
			 case 9:
				 outData->port_mode = 0x03;
				 break;
			 case 8:
				outData->port_mode = 0x04;
				 break;
			 case 12:
				outData->port_mode = 0x05;
				 break;
			 default:
				 return GW_RETURN_FAIL;
		 }
	outData->hdr.width= 1;
	  return sizeof(oam_var_cont_t)+outData->hdr.width; 
  }
  static unsigned short downtime_t = 0;
  static unsigned short restarttimes_t = 0;
 
  unsigned int pbs_attr_oam_set_uni_loopback(  ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32  procRecvLen,
        cs_uint8 * pOut)
  {
	  onuLoopbackCloseTime_t *inData = (onuLoopbackCloseTime_t *)pRecV;
           oam_var_cont_t *hdr = (oam_var_cont_t *)pOut;
	  
	  downtime_t = inData->port_downtime;
	  restarttimes_t = inData->port_restarttimes;
	 hdr->width = 0x80;
	  return sizeof(oam_var_cont_t);
	  
  }
  unsigned int pbs_attr_oam_get_uni_loopback(  ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32*  procRecvLen,
        cs_uint8 * pOut)
  {
  
	  onuLoopbackCloseTime_t *outData =   (onuLoopbackCloseTime_t *)pOut;
	  outData->port_downtime = downtime_t;
	  outData->port_restarttimes = restarttimes_t;
	  outData->hdr.width = 0x04;
	  return outData->hdr.width;
	  
  }
  unsigned int pbs_attr_oam_set_onu_configuration( ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32  procRecvLen,
        cs_uint8 * pOut)
  {
          oam_var_cont_t *hdr = (oam_var_cont_t *)pOut;
          hdr->width = 0x80;
	 return sizeof(oam_var_cont_t)+sizeof(oam_var_cont_t);
	 
  }
  unsigned int pbs_attr_oam_get_function_capability(  ctc_oam_mgmt_obj_idx_t index,
        cs_uint8 * pRecV,
        cs_uint32 * procRecvLen,
        cs_uint8 * pOut)
  {
           cs_printf("pbs_attr_oam_get_function_capability");
	  onuCapability_t *outData = (onuCapability_t *)pOut ;
	  int i = 0;
	  outData->result = 0x1;
	  for(i = 0;i<16;i++)
	  {
	  	outData->capability[i] = 0xff;
	  }
	  
	  outData->hdr.width= 0x11;
	  return sizeof(oam_var_cont_t)+outData->hdr.width;
	  
  }
  
  
  

 
