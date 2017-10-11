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

DEFINITIONS:  "DEVICE" means the Cortina Systems?LynxD SDK product.
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
 *
 * plat_config.h
 *
 * $Id: plat_config.h,v 1.1.2.11 2011/11/08 07:58:28 lhuang Exp $
 */

#ifndef _PLAT_CONFIG_H_
#define _PLAT_CONFIG_H_


/*
** Thread Parameters for application threads
*/
#define PCM_THREAD_STACKSIZE                (2 * 4096)
#define SHELL_THREAD_STACKSIZE              (2 * 4096)
#define AAL_THREAD_STACKSIZE                (2 * 4096)
#define TIMER_THREAD_STACK_SIZE             (2 * 4096)
#define VOIP_THREAD_STACKSIZE               (4 * 4096)
#define APPL_THREAD_STACKSIZE               (4 * 4096)
#define DAEMON_THREAD_STACKSIZE             (1 * 4096)
#define LOG_THREAD_STACKSIZE                (1 * 4096)
#define TELNETD_THREAD_STACKSIZE            (2 * 4096)
#define TERM_SERV_THREAD_STACKSIZE          (1 * 4096)
#define POLLING_THREAD_STACKSIZE            (1 * 4096)
#define PPP_THREAD_STACKSIZE            (1 * 4096)
#define TEST_UART_STACKSIZE				(1 * 4096)	

#define PCM_THREAD_PRIORITY         5
#define SHELL_THREAD_PRIORITY       6
#define AAL_THREAD_PRIORITY         7
#define TIMER_THREAD_PRIORITY       8
#define VOIP_THREAD_PRIORITY        9
#define APPL_THREAD_PRIORITY        10
#define ECOS_NET_FAST_THREAD_PRI    14
#define ECOS_NET_THREAD_PRI         15
#define ECOS_ATHTTPD_THREAD_PRI     16
#define DAEMON_THREAD_PRIORITY          17
#define LOG_THREAD_PRIORITY         18
#define TELNETD_THREAD_PRIORITY     19

#define PORT_STATS_THREAD_PRIORITY 24
#define UART1_THREAD_PRIORITY        20
#define UART2_THREAD_PRIORITY        21
#define UART3_THREAD_PRIORITY        22
#define UART4_THREAD_PRIORITY        23
#define TERM_SERV_UART2NET_THREAD_PRIORITY 9
#define TERM_SERV_IP_RX_THREAD_PRIORITY 12
#define TERM_SERV_NET2UART_THREAD_PRIORITY 13
#define POLLING_THREAD_PRIORITY     30
#define PPP_THREAD_PRIORITY     13


#define PCM_THREAD_NAME             "PCM Thread"
#define SHELL_THREAD_NAME           "Shell Thread"
#define AAL_THREAD_NAME             "AAL Thread"
#define TIMER_THREAD_NAME           "Timer Thread"
#define VOIP_THREAD_NAME            "VOIP Thread"
#define APPL_THREAD_NAME            "APP Thread"
#define DAEMON_THREAD_NAME         "Daemon Thread"
#define LOG_THREAD_NAME             "LOG Thread"
#define TELENTD_THREAD_NAME         "TELNETD Thread"
#define POLLING_THREAD_NAME         "POLLING Thread"
#define STAT_THREAD_NAME        "Stat Thread"
#define PPP_THREAD_NAME         "PPP Thread"
#define UART_THREAD_ONE			"uart1 Thread"
#define UART_THREAD_TWO			"uart2 Thread"
#define UART_THREAD_THR			"uart3 Thread"
#define UART_THREAD_FOR			"uart4 Thread"

#if 1
//�߳�����ӦС��20 ���ֽ�
#define STORM_BC_THREAD_NAME	"storm bc Thread"
#endif

/*
** Init sequence for APP
*/
#define INIT_STEP_GLB       0
#define INIT_STEP_SDL       1
#define INIT_STEP_APP       2
#define INIT_STEP_SERVICE   3


/*
** Priority for application queue
*/
#define APP_QUEUE_PRI_NUM  4
#define APP_QUEUE_PRI_0 0
#define APP_QUEUE_PRI_1 1
#define APP_QUEUE_PRI_2 2
#define APP_QUEUE_PRI_3 3


#endif
