/*					 Copyright(c) 2008-2015 GWTT, Inc.					  */
/*	
**	pbs_oam_api.c -  c file 
**
**	This file was written by zhaoxh, 22/03/2017
**	
**	Changes:
**
**	Version 	  |  Date		   |	Change		  |    Author	  
**	----------|-----------|-------------|------------
**	1.00		  | 22/03/2017 |	creation		  |    zhaoxh
*/

#include "pbs_oam.h"
#include "oam_common.h"

extern cs_status oam_get_ctc_port_index_info(
        cs_uint8  * data,
        ctc_oam_mgmt_obj_idx_t *mgmtIdx
        );

extern cs_uint32 ctc_oam_build_header(
        cs_uint8 * oui,
        cs_uint32  port,
        cs_uint8 * buf,
        cs_uint8  opcode);
extern cs_uint32  ctc_oam_build_get_not_support(
        ctc_oam_mgmt_obj_idx_t index,
        cs_uint8   * pRecv,
        cs_uint32 *  procRecvLen,
        cs_uint8   * pOut);
cs_uint32  ctc_oam_build_set_not_support(
        ctc_oam_mgmt_obj_idx_t index,
        cs_uint8   * pRecv,
        cs_uint32    RecvLen,
        cs_uint8   * pOut);

extern cs_status oam_vendor_handler_register(
        oam_oui_t   oui,
        oam_vendor_handlers_t   handlers);
void pbs_oam_init(void)
{
   oam_oui_t  oam_oui_pbs = {0x0c, 0x7c, 0x7d};
   static oam_vendor_handlers_t pbs_oam_handlers = {
		NULL, NULL, NULL, NULL, pbs_oam_handle
   };
   oam_vendor_handler_register(oam_oui_pbs,pbs_oam_handlers);
		
}

void
pbs_oam_handle(
        cs_uint32 port, 
        cs_uint8 *frame, 
        cs_uint32 len)
/*                                                                           */
/* INPUTS  : o  port - OAM port ID                                           */
/*           o  frame - Container buffer to store the composed OAM frame     */
/*           o  len - Size of OAM frame                                      */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* API to handler PBS OAM frame                                              */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    oam_pdu_ctc_t *pCTCOamFrame = (oam_pdu_ctc_t *)frame;

    OAM_ASSERT(frame != NULL || len != 0 );
     cs_printf("pbs_oam_handle\n");
    switch(pCTCOamFrame->opcode) {
        case OAM_PDU_PBS_VAR_REQ:
            OAM_ORG_DEBUG("Recv OAM_PDU_CTC_VAR_REQ \n");
            pbs_oam_build_var_resp(port, frame, len);
            break;
        case OAM_PDU_PBS_SET_REQ:
            OAM_ORG_DEBUG("Recv OAM_PDU_CTC_SET_REQ \n");
            pbs_oam_build_set_resp(port, frame, len);
            break; 
        default:
            OAM_ORG_LOG("Recv Unknow Opcode = %d \n",pCTCOamFrame->opcode);
            break;
    }
}
extern void ctc_oam_send(
        cs_uint32 port,
        cs_uint8 *buf,
        cs_int32 len
        );
extern cs_boolean ctc_oam_is_odd_set_oam(
        cs_uint8 branch, 
        cs_uint16 leaf);
extern cs_uint8 extern_flag;

cs_int32 pbs_oam_build_var_resp(
        cs_uint32 port, 
        cs_uint8 *frame, 
        cs_uint32 length)
/*                                                                           */
/* INPUTS  : o  port - OAM port ID                                           */
/*           o  frame - Container buffer to store the composed OAM frame     */
/*           o  lenght - OAM frame lengtjh                                   */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* API to handler CTC OAM var request                                        */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
   cs_printf("pbs_oam_build_var_resp\n");
    cs_uint8    pdu[1518];
    cs_uint8    outPdu[1518];
    oam_pdu_ctc_t   *pOutPkt = (oam_pdu_ctc_t *)pdu;
    oam_pdu_ctc_t   *pRecvPkt = NULL;
    oam_var_desc_t   *pRecvVarCont = NULL;
    cs_uint8    *pOutVarCont = NULL;
    cs_int32   totalContentLen = 0, outPktLen = 0;
    cs_boolean  isFirstGetTlv = TRUE,  idxBuild = FALSE;
    ctc_oam_mgmt_obj_idx_t  mgmtIndex;

    OAM_ASSERT_RET(frame != NULL,0);
    OAM_ASSERT_RET(length >= 
            sizeof(oam_pdu_ctc_t) + sizeof(oam_var_desc_t) - 1, 0);

    memset(pdu, 0, 1518);
    
    pRecvPkt = (oam_pdu_ctc_t *)frame;
    outPktLen = ctc_oam_build_header(pRecvPkt->oui, port,pdu,OAM_PDU_PBS_VAR_RESP);
    pOutVarCont = pOutPkt->data;

    /*shift the header of received packet*/
    totalContentLen = length - sizeof(oam_pdu_ctc_t) + 1;
    pRecvVarCont = (oam_var_desc_t *)pRecvPkt->data;

    memset(&mgmtIndex, 0, sizeof(ctc_oam_mgmt_obj_idx_t));
    mgmtIndex.valid = FALSE;

    while(totalContentLen > 0) {
        cs_uint32 len = 0;
        cs_uint32 procLen = 0;
        OAM_ORG_DEBUG("VAR REQ branch 0x%x, leaf 0x%x\n",
                pRecvVarCont->branch, ntohs(pRecvVarCont->leaf));

        if(pRecvVarCont->branch == 0){   
            if(isFirstGetTlv){
                OAM_ORG_LOG("Bad packet \n");
                return 0;
            }else{
                /*End TLV, break and send*/
                break;
            }
        }

        if(CS_E_OK == oam_get_ctc_port_index_info(
                    (cs_uint8 *)pRecvVarCont,&mgmtIndex)){
            /*shift the index TLV*/
            totalContentLen -= mgmtIndex.idxLen;
            pRecvVarCont = (oam_var_desc_t *)
                ((cs_uint8 *)pRecvVarCont + mgmtIndex.idxLen);
            idxBuild = FALSE;
            OAM_ORG_DEBUG("Index branch :%#x , leaf:%#x value:%#x \n",
                    mgmtIndex.idxBranch,mgmtIndex.idxLeaf,mgmtIndex.idxValue);
            continue;
        }

        memset(outPdu, 0, 1518);
       if(ntohs(pRecvVarCont->leaf) == 49155)
       {
	        while( !extern_flag)
	        	{
	        		 len = pbs_oam_build_get_tlv(pRecvPkt->oui, outPdu,pRecvVarCont,
	                &procLen,mgmtIndex,&idxBuild);

		        if(outPktLen + len > (1518 - 2)){
		            OAM_ORG_DEBUG("build length %d great than max len %d \n",
		            outPktLen + len, 1518 - 2);
		            break;
		        }

		        if(len > 0){
		            memcpy(pOutVarCont,outPdu,len);
		        }
		        outPktLen += len;
		        pOutVarCont += len;

		        totalContentLen -= procLen;

		        /* the remain pkt is less than branch and leaf's length*/
		        if(totalContentLen 
		                < sizeof(oam_var_desc_t)){
		            break;
		        }

		        pRecvVarCont = (oam_var_desc_t*)
		            ((cs_uint8 *)pRecvVarCont +  procLen);
		        isFirstGetTlv = FALSE;
				
			/* packet 2  0's to mark the end - 57.6.3 a)*/
			   *pOutVarCont++ = 0;
			   *pOutVarCont++ = 0;
			   outPktLen += 2;
			
			   if(outPktLen > (sizeof(oam_pdu_ctc_t) - 1 + 2)){
				   ctc_oam_send(port, pdu, outPktLen);
			   }
		        		
	         }
		extern_flag = 0;
       	}
	 else{
	 	 len = pbs_oam_build_get_tlv(pRecvPkt->oui, outPdu,pRecvVarCont,
	                &procLen,mgmtIndex,&idxBuild);

	        if(outPktLen + len > (1518 - 2)){
	            OAM_ORG_DEBUG("build length %d great than max len %d \n",
	            outPktLen + len, 1518 - 2);
	            break;
	        }

	        if(len > 0){
	            memcpy(pOutVarCont,outPdu,len);
	        }
	        outPktLen += len;
	        pOutVarCont += len;

	        totalContentLen -= procLen;

	        /* the remain pkt is less than branch and leaf's length*/
	        if(totalContentLen 
	                < sizeof(oam_var_desc_t)){
	            break;
	        }

	        pRecvVarCont = (oam_var_desc_t*)
	            ((cs_uint8 *)pRecvVarCont +  procLen);
	        isFirstGetTlv = FALSE;

				    /* packet 2  0's to mark the end - 57.6.3 a)*/
		*pOutVarCont++ = 0;
		*pOutVarCont++ = 0;
		outPktLen += 2;

		if(outPktLen > (sizeof(oam_pdu_ctc_t) - 1 + 2)){
		    ctc_oam_send(port, pdu, outPktLen);
		}
	 	}			
    }



    return outPktLen;
}


/*****************************************************************************/
/* $rtn_hdr_start  ctc_oam_build_set_resp                                    */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
cs_int32 pbs_oam_build_set_resp(
        cs_uint32 port, 
        cs_uint8 *frame, 
        cs_uint32 length)
/*                                                                           */
/* INPUTS  : o  port - OAM port ID                                           */
/*           o  frame - Container buffer to store the composed OAM frame     */
/*           o  lenght - OAM frame lengtjh                                   */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* API to handler CTC OAM set request                                        */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
#if 0
	cs_printf("in ctc_oam_build_set_resp, frame[17] :0x%x, frame[21]:0x%x\n", frame[17], frame[21]);
#endif
    cs_uint8    pdu[1518];
    cs_uint8    outPdu[1518];
    oam_pdu_ctc_t *pOutPkt = (oam_pdu_ctc_t *)pdu;
    oam_pdu_ctc_t *pRecvPkt = NULL;
    oam_ctc_onu_var_set_hdr_t *pRecvVarCont = NULL;
    cs_uint8 *pOutVarCont = NULL;
    cs_uint8 *pInVarCont = NULL;
    cs_int32 totalContentLen = 0;
    cs_uint32 outPktLen = 0;
    cs_int32  len = 0;
    cs_printf("pbs_oam_build_set_resp\n");
    cs_boolean  isFirstSetTLV = TRUE,  idxBuild = FALSE;
    ctc_oam_mgmt_obj_idx_t  mgmtIndex;
    cs_uint8 lastBranch = 0;
    cs_uint16 lastLeaf = 0;
    cs_uint8 tlvLen = 0;
    cs_uint8 * pLastRecvVarCont = NULL;

    OAM_ASSERT_RET(frame != NULL,0);
    OAM_ASSERT_RET(length >= 
            sizeof(oam_pdu_ctc_t) + sizeof(oam_var_desc_t) -1, 0);

    memset(pdu, 0, 1518);
    pRecvPkt = (oam_pdu_ctc_t *)frame;
    outPktLen = ctc_oam_build_header(pRecvPkt->oui, port,pdu,OAM_PDU_PBS_SET_RESP);
    pOutVarCont = pOutPkt->data;

    /*shift the header of received packet*/
    totalContentLen = length - sizeof(oam_pdu_ctc_t) + 1;
    pRecvVarCont = (oam_ctc_onu_var_set_hdr_t *)pRecvPkt->data;

    pInVarCont = (cs_uint8 *)pRecvVarCont;

    mgmtIndex.valid = FALSE;
    while(totalContentLen > 0) {
        OAM_ORG_DEBUG("SET REQ branch 0x%x, leaf 0x%x\n",
                pRecvVarCont->branch, ntohs(pRecvVarCont->leaf));
        if(pRecvVarCont->branch == 0){   
            if(isFirstSetTLV){
                OAM_ORG_LOG("Bad packet \n");
                return 0;
            }else{
                /*End TLV, break and send*/
                break;
            }
        }

        if(CS_E_OK == oam_get_ctc_port_index_info((cs_uint8 *)pRecvVarCont,&mgmtIndex)){
            /*shift the index TLV*/
            totalContentLen -= mgmtIndex.idxLen;
            pInVarCont += mgmtIndex.idxLen;
            pRecvVarCont = (oam_ctc_onu_var_set_hdr_t *)pInVarCont;

            idxBuild = FALSE;
            OAM_ORG_DEBUG("Index branch :%#x , leaf:%#x value:%#x \n",
                    mgmtIndex.idxBranch,mgmtIndex.idxLeaf,mgmtIndex.idxValue);
            continue;
        }

        /* check the duplicate TLV */
        if(lastBranch == pRecvVarCont->branch 
                && lastLeaf == ntohs(pRecvVarCont->leaf)){
            cs_uint32 tmpLen; 
            OAM_ORG_DEBUG("duplicate branch:%#x, leaf:%#x , len = %d\n",
                    pRecvVarCont->branch ,ntohs(pRecvVarCont->leaf),
                    pRecvVarCont->width);

            /* special set oam only has branch and leaf, no length*/
            if(ctc_oam_is_odd_set_oam(
				
                        pRecvVarCont->branch,ntohs(pRecvVarCont->leaf))){
                /* Only remove the duplicate tlv from the pdu*/
                tmpLen = sizeof(oam_var_cont_t) - 1;
                memmove(pInVarCont,
                        pInVarCont + tmpLen,
                        totalContentLen - tmpLen);
                totalContentLen -= tmpLen;
            }else{
                tmpLen = pRecvVarCont->width;
                tlvLen += tmpLen;
                memmove(pInVarCont,
                        pInVarCont + sizeof(oam_var_cont_t),
                        totalContentLen - sizeof(oam_var_cont_t));
                totalContentLen -= (tmpLen + sizeof(oam_var_cont_t));
                pInVarCont += tmpLen;
            }

        }else{
            /*is not first tlv*/
            if(lastBranch != 0 
                    && lastLeaf != 0){
                    
                OAM_ORG_DEBUG("process the previous TLV, branch:%#x, leaf:%#x \n",
                        lastBranch, lastLeaf);
               /* process the pLastRecvVarCont */
                memset(outPdu, 0, 1518);
                len = pbs_oam_build_set_tlv(pRecvPkt->oui, outPdu,pLastRecvVarCont,
           									 tlvLen,mgmtIndex,&idxBuild);
                
                if(outPktLen + len > (1518)){
                    OAM_ORG_LOG("build length %d great than max len %d \n",
                            outPktLen + len, 1518 );
                    break;
                }else{
                    memcpy(pOutVarCont,outPdu,len);
                    outPktLen += len;
                    pOutVarCont += len;
                }
            }
            
            lastBranch = pRecvVarCont->branch;
            lastLeaf = ntohs(pRecvVarCont->leaf);
            pLastRecvVarCont = pInVarCont;
            /* special set oam only has branch and leaf, no length*/
            if(ctc_oam_is_odd_set_oam(
				
                        pRecvVarCont->branch,ntohs(pRecvVarCont->leaf))){
                tlvLen = 0;
                pInVarCont += (sizeof(oam_var_cont_t) - 1);
                totalContentLen -=(sizeof(oam_var_cont_t) - 1);
            }else{
			
                tlvLen = pRecvVarCont->width;
				
                pInVarCont += tlvLen + sizeof(oam_var_cont_t);
				
                totalContentLen -= (tlvLen + sizeof(oam_var_cont_t));
            }

        }

        if(totalContentLen 
                < sizeof(oam_var_desc_t)){
            break;
        }

        isFirstSetTLV = FALSE;
        pRecvVarCont = (oam_ctc_onu_var_set_hdr_t *)pInVarCont;
    }
    
    /* process the Last TLV */
    if(pLastRecvVarCont != NULL){
        OAM_ORG_DEBUG("process the final one TLV \n");
        memset(outPdu, 0, 1518);
        len = pbs_oam_build_set_tlv(pRecvPkt->oui, outPdu,
                pLastRecvVarCont,tlvLen,mgmtIndex,&idxBuild);
        
        if(outPktLen + len > (1518)){
            OAM_ORG_LOG("build length %d great than max len %d \n",
                    outPktLen + len, 1518 );
        }else{
            memcpy(pOutVarCont,outPdu,len);
            outPktLen += len;
            pOutVarCont += len;
        }
    }
    
    /* packet 2  0's to mark the end - 57.6.3 a)*/
    *pOutVarCont++ = 0;
    *pOutVarCont++ = 0;
    outPktLen += 2;

    if(outPktLen > (sizeof(oam_pdu_ctc_t) - 1 + 2)){
        ctc_oam_send(port, pdu, outPktLen);
    }
    
    return outPktLen;
}
extern cs_uint8 CTC_MAX_ONU_UNI_PORTS ;
extern cs_uint32 ctc_oam_build_mgmt_index(
        cs_uint8 * pOut,
        cs_uint8 branch, 
        cs_uint16 leaf,
        cs_uint32 index,
        cs_uint8  type);

cs_uint32 pbs_oam_build_get_tlv(
        oam_oui_t oui,
        cs_uint8 * pOutVarCont,
        oam_var_desc_t * pRecvVarCont,
        cs_uint32 * procLen,
        ctc_oam_mgmt_obj_idx_t mgmtIndex,
        cs_boolean * idxBuild)
/*                                                                           */
/* INPUTS  : o  pOutVarCont - the send oam packet                            */
/*           o  pRecevVarCont - the recevied oam packet                      */
/*           o  procLen - the length of process the received packet          */
/*           o  mgmtIndex - the ctc obj mgmt index                           */
/*           o  idxBuild - the index tlv build flag                          */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* build the get branch leaf packet                                          */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    oam_var_cont_t *pOut 
        = (oam_var_cont_t*) pOutVarCont;
    cs_uint32 len = 0, outPktLen = 0; 
    cs_uint32 length = 0;
    ctc_oam_get_handler_t get_handler = NULL;
    cs_boolean buildFlag = FALSE;
    cs_printf("pbs_oam_build_get_tlv\n");
    OAM_ASSERT_RET(idxBuild != NULL 
            && pRecvVarCont != NULL 
            && pOutVarCont != NULL
            && procLen != NULL,0);

    /* default get only process the branch and leaf,
     * but the alarm config need process more than it */
    *procLen = sizeof(oam_var_desc_t);

    get_handler = pbs_oam_find_get_handler(oui, pRecvVarCont->branch,
            ntohs(pRecvVarCont->leaf));
    
    if(get_handler == NULL){
        get_handler = ctc_oam_build_get_not_support;
		cs_printf("no get_handler\n");
    }

    /*no index include*/
    if(!mgmtIndex.valid){
        OAM_ORG_DEBUG(" No index branch %#x, leaf %#x \n",
                pRecvVarCont->branch,ntohs(pRecvVarCont->leaf));
        pOut->branch = pRecvVarCont->branch;
        pOut->leaf = pRecvVarCont->leaf;
        outPktLen = get_handler(mgmtIndex,(cs_uint8 *)pRecvVarCont,
                procLen,pOutVarCont);
    }else{
        /* single index */
        if(mgmtIndex.idxValue != 0XFF
                && mgmtIndex.idxValue != 0XFFFF){
            OAM_ORG_DEBUG(" single index branch %#x, leaf %#x \n",
                    pRecvVarCont->branch,ntohs(pRecvVarCont->leaf));
            if(!*idxBuild){
                OAM_ORG_DEBUG("build index branch = %#x,leaf = %#x,value = %d \n",
                        mgmtIndex.idxBranch,
                        mgmtIndex.idxLeaf,
                        mgmtIndex.idxValue);

                outPktLen = ctc_oam_build_mgmt_index(pOutVarCont,
                        mgmtIndex.idxBranch,
                        mgmtIndex.idxLeaf,
                        mgmtIndex.idxValue,
                        mgmtIndex.idxType);

                OAM_ORG_DEBUG("build index len = %d \n",outPktLen);
                *idxBuild = TRUE;
                buildFlag = TRUE;
                pOutVarCont += outPktLen;
            }
            
            pOut = (oam_var_cont_t*) pOutVarCont;
            pOut->branch = pRecvVarCont->branch;
            pOut->leaf = pRecvVarCont->leaf;

            if((mgmtIndex.idxType == 1) && 
                (mgmtIndex.idxValue > 8) &&
                (get_handler != ctc_oam_build_get_not_support)){
                OAM_ORG_LOG("index = %d  is  greater than max %d \n",
                        mgmtIndex.idxValue,8);
                pOut->width = 0x86;
				cs_printf("error**\n");
                outPktLen += sizeof(oam_var_cont_t);
            }else{
                len = get_handler(mgmtIndex,(cs_uint8 *)pRecvVarCont,
                        procLen,pOutVarCont);

                if((len == 0) && buildFlag){
                    pOutVarCont -= outPktLen;
                    *idxBuild = FALSE;
                    outPktLen = 0;
                }
                else{                    
                    outPktLen += len;
                }
            }
        }else{ /* All index*/
            cs_uint32 i = 0;
            OAM_ORG_DEBUG("all indexs branch %#x, leaf %#x \n",
                    pRecvVarCont->branch,ntohs(pRecvVarCont->leaf));

            for(i = CS_UNI_PORT_ID1; i<= 8; i++){
                mgmtIndex.idxValue = i;
                len = ctc_oam_build_mgmt_index(pOutVarCont,
                        mgmtIndex.idxBranch,
                        mgmtIndex.idxLeaf,
                        mgmtIndex.idxValue,
                        mgmtIndex.idxType);
                *idxBuild = TRUE;
                /*shift the index TLV*/
                pOutVarCont += len;
                outPktLen += len;
                pOut = (oam_var_cont_t *)pOutVarCont;
                pOut->branch = pRecvVarCont->branch;
                pOut->leaf = pRecvVarCont->leaf;

                OAM_ORG_DEBUG("build index branch = %#x,leaf = %#x,value = %d \n",
                        mgmtIndex.idxBranch,
                        mgmtIndex.idxLeaf,
                        mgmtIndex.idxValue);

                length = get_handler(mgmtIndex,(cs_uint8 *)pRecvVarCont,
                        procLen,pOutVarCont);
                
                if(length == 0){
                    pOutVarCont -= len;
                    outPktLen -= len;
                    *idxBuild = FALSE;
                    break;
                }
                
                /*shift the real TLV*/
                pOutVarCont += length;
                outPktLen += length;
            }

        }
    }
    return outPktLen;
}


/*****************************************************************************/
/* $rtn_hdr_start  pbs_oam_build_set_tlv                                     */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
cs_uint32 pbs_oam_build_set_tlv(
        oam_oui_t oui,
        cs_uint8 * pOutVarCont,
        cs_uint8 * pRecv,
        cs_uint32  tlvLen,
        ctc_oam_mgmt_obj_idx_t mgmtIndex,
        cs_boolean * idxBuild)
/*                                                                           */
/* INPUTS  : o  pOutVarCont - the send oam packet                            */
/*           o  pRecv - the recevied oam packet                              */
/*           o  tlvLen - the received packet  length                         */
/*           o  mgmtIndex - the ctc obj mgmt index                           */
/*           o  idxBuild - the index tlv build flag                          */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* build the set branch leaf packet                                          */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    oam_var_cont_t *pOut 
        = (oam_var_cont_t * ) pOutVarCont;
    oam_var_cont_t* pRecvVarCont 
        = (oam_var_cont_t * ) pRecv;
    cs_uint32 len = 0, outPktLen = 0; 
    ctc_oam_set_handler_t set_handler = NULL;
    cs_boolean buildFlag = FALSE;
    
    OAM_ASSERT_RET(idxBuild != NULL 
            && pRecvVarCont != NULL 
            && pOutVarCont != NULL,0);
    
    /*cs_buf_print(pRecv, tlvLen + sizeof(oam_var_cont_t));*/

    set_handler = pbs_oam_find_set_handler(oui, pRecvVarCont->branch,
            ntohs(pRecvVarCont->leaf));
    
    if(set_handler == NULL){
        set_handler = ctc_oam_build_set_not_support;
    }

    /*no index include*/
    if(!mgmtIndex.valid){
        OAM_ORG_DEBUG("No index branch %#x, leaf %#x \n",
                pRecvVarCont->branch,ntohs(pRecvVarCont->leaf));
        pOut->branch = pRecvVarCont->branch;
        pOut->leaf = pRecvVarCont->leaf;
        outPktLen = set_handler(mgmtIndex,pRecv,tlvLen,pOutVarCont);

    }else{

        /* single index */
        if(mgmtIndex.idxValue != 0xff
                && mgmtIndex.idxValue != 0xffff){
            OAM_ORG_DEBUG(" single index branch %#x, leaf %#x \n",
                    pRecvVarCont->branch,ntohs(pRecvVarCont->leaf));
            if(!*idxBuild){
                OAM_ORG_DEBUG("build index branch = %#x,leaf = %#x, value = %d \n",
                        mgmtIndex.idxBranch,
                        mgmtIndex.idxLeaf,
                        mgmtIndex.idxValue);

                outPktLen = ctc_oam_build_mgmt_index(pOutVarCont,
                        mgmtIndex.idxBranch,
                        mgmtIndex.idxLeaf,
                        mgmtIndex.idxValue,
                        mgmtIndex.idxType);

                OAM_ORG_DEBUG("build index len = %d \n",outPktLen);
                *idxBuild = TRUE;
                buildFlag = TRUE;
                pOutVarCont += outPktLen;
            }

            pOut = (oam_var_cont_t*) pOutVarCont;
            pOut->branch = pRecvVarCont->branch;
            pOut->leaf = pRecvVarCont->leaf;

            
            if((mgmtIndex.idxValue > 8) 
                && (set_handler != ctc_oam_build_set_not_support)){
                OAM_ORG_LOG("index = %d  is  greater than max %d \n",
                        mgmtIndex.idxValue,8);
                pOut->width = 0x86;
				cs_printf("error**\n");
                outPktLen += sizeof(oam_var_cont_t);
            }else{
                len = set_handler(mgmtIndex,pRecv,tlvLen,pOutVarCont);

                if((len == 0) && buildFlag){
                    pOutVarCont -= outPktLen;
                    *idxBuild = FALSE;
                    outPktLen = 0;
                }
                else{                    
                    outPktLen += len;
                }
            }
        }else{ /* All index*/
            cs_uint32 i = 0;
            OAM_ORG_DEBUG("all indexs branch %#x, leaf %#x \n",
                    pRecvVarCont->branch,ntohs(pRecvVarCont->leaf));

            if(!*idxBuild){
                outPktLen = ctc_oam_build_mgmt_index(pOutVarCont,
                        mgmtIndex.idxBranch,
                        mgmtIndex.idxLeaf,
                        mgmtIndex.idxValue,
                        mgmtIndex.idxType);
                *idxBuild = TRUE;
                buildFlag = TRUE;
                /*shift the index TLV*/
                pOutVarCont += outPktLen;
            }
            
            /* we only need build one response with port index 0xffff*/
            pOut = (oam_var_cont_t *)pOutVarCont;
            pOut->branch = pRecvVarCont->branch;
            pOut->leaf = pRecvVarCont->leaf;
            for(i = 1; i<= 8; i++){
                mgmtIndex.idxValue = i;

                OAM_ORG_DEBUG("build index branch = %#x,leaf = %#x,value = %d \n",
                        mgmtIndex.idxBranch,
                        mgmtIndex.idxLeaf,
                        mgmtIndex.idxValue);
                len = set_handler(mgmtIndex,pRecv,tlvLen,pOutVarCont);

                if((len == 0) && buildFlag){
                    pOutVarCont -= outPktLen;
                    outPktLen = 0;
                    *idxBuild = FALSE;
                    break;
                }
                
                /*check the set return right or not */
                if(pOut->width != 0x80){
                    break;
                }
            }
            /*shift the real TLV*/
            pOutVarCont += len;
            outPktLen += len;

        }
    }
    return outPktLen;
}
ctc_oam_handle_t g_pbs_oam_xlat_table[] =    
{                                                                        
#undef xx                                                             
#define xx(SEQ,BRANCH,LEAF,LEAF_VAL,GET_FUNC,SET_FUNC)   { BRANCH, LEAF, GET_FUNC, SET_FUNC },  
    PBS_OAM_BUILD_RESP
};       

ctc_oam_get_handler_t pbs_oam_find_get_handler(
        oam_oui_t oui,
        cs_uint8 branch, 
        cs_uint16 leaf)
/*                                                                           */
/* INPUTS  : o  branch - the ctc oam branch id                               */
/*           o  leaf - the ctc oam leaf id                                   */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* find the get handler for special branch and leaf                          */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    ctc_oam_get_handler_t  get_handler = NULL;
    cs_uint32 i = 0;
	cs_printf("pbs_oam_find_get_handler");
    if(get_handler == NULL ){
        for(i = 0 ;i<sizeof(g_pbs_oam_xlat_table)/sizeof(g_pbs_oam_xlat_table[0]) ; i++){

            if(g_pbs_oam_xlat_table[i].branch == branch
                    && g_pbs_oam_xlat_table[i].leaf == leaf){

                OAM_ORG_DEBUG("pbs oam handler found: branch %#x leaf %#x\n",
                        branch, leaf);
                get_handler =  g_pbs_oam_xlat_table[i].get_func;
                break;
            }
        }
    }
    return get_handler;
}

/*****************************************************************************/
/* $rtn_hdr_start  ctc_oam_find_set_handler                                  */
/* CATEGORY   : Device                                                       */
/* ACCESS     : public                                                       */
/* BLOCK      : General                                                      */
/* CHIP       : CS8030                                                       */
/*                                                                           */
ctc_oam_set_handler_t pbs_oam_find_set_handler(
        oam_oui_t oui,
        cs_uint8 branch, 
        cs_uint16 leaf)
/*                                                                           */
/* INPUTS  : o  branch - the ctc oam branch id                               */
/*           o  leaf - the ctc oam leaf id                                   */
/* OUTPUTS : N/A                                                             */
/* DESCRIPTION:                                                              */
/* find the set handler for special branch and leaf                          */
/* $rtn_hdr_end                                                              */
/*****************************************************************************/
{
    ctc_oam_set_handler_t  set_handler = NULL;
    cs_uint32 i = 0;

    if(set_handler == NULL ){
        for(i = 0 ;i<sizeof(g_pbs_oam_xlat_table)/sizeof(g_pbs_oam_xlat_table[0]) ; i++){

            if(g_pbs_oam_xlat_table[i].branch == branch
                    && g_pbs_oam_xlat_table[i].leaf == leaf){

                OAM_ORG_DEBUG("pbs oam handler found: branch %#x leaf %#x\n",
                        branch, leaf);
                set_handler =  g_pbs_oam_xlat_table[i].set_func;
                break;
            }
        }
    }


    return set_handler;
}

