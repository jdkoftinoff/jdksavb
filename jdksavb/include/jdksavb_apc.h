#pragma once

/*
Copyright (c) 2014, Jeff Koftinoff
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the {organization} nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "jdksavdecc.h"
#include "jdksavb_net.h"
#include "jdksavb_apc_signals.h"
#include "jdksavb_apc_slots.h"

#ifdef __cplusplus
extern "C" {
#endif

struct jdksavb_apc_Addr
{
    struct sockaddr *m_addr;
    socklen_t m_addr_len;
    char m_path[4096];
    char m_host[1024];
    char m_port[128];
    char *m_additional_request_headers;
};

/**
 * @brief The jdksavb_apc_State enum
 *
 * See IEEE Std 1722.1 Annex C.5.3.3, Figure C.3
 *
 */
enum jdksavb_apc_State
{
    jdksavb_apc_State_BEGIN = 0,
    jdksavb_apc_State_INITIALIZE,
    jdksavb_apc_State_WAIT_FOR_CONNECT,
    jdksavb_apc_State_ACCEPT,
    jdksavb_apc_State_START_TRANSFER,
    jdksavb_apc_State_WAITING,
    jdksavb_apc_State_CLOSED,
    jdksavb_apc_State_LINK_STATUS,
    jdksavb_apc_State_RECEIVE_MSG,
    jdksavb_apc_State_SEND_MSG,
    jdksavb_apc_State_ENTITY_ID_ASSIGNED,
    jdksavb_apc_State_SEND_NOP,
    jdksavb_apc_State_FINISH,
    jdksavb_apc_State_END
};

struct jdksavb_apc
{
    /**
     * @brief m_base dispatch table for network events
     *
     * The effective "Base Class" for receiving network events from a TCP socket
     */
    struct jdksavb_netevent m_base;

    /**
     * @brief m_net TCP network socket
     *
     * The object which manages socket requests
     */
    struct jdksavb_netrequest *m_net;

    /**
     * The Current State
     */
    enum jdksavb_apc_State m_state;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.1
     */
    struct jdksavb_apc_Addr m_addr;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.2
     */
    struct jdksavdecc_appdu m_apcMsg;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.3
     */
    bool m_apcMsgOut;
    /**
     * See IEEE 1722.1 Annex C.5.3.1.4
     */
    struct jdksavdecc_appdu m_apsMsg;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.5
     */
    bool m_apsMsgIn;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.6
     */
    jdksavdecc_timestamp_in_milliseconds m_currentTime;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.7
     */
    bool m_finished;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.8
     */
    struct jdksavdecc_eui64 m_entityId;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.9
     */
    bool m_idAssigned;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.10
     */
    bool m_incomingTcpClosed;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.11
     */
    struct jdksavdecc_appdu m_linkMsg;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.12
     */
    bool m_linkStatusMsg;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.13
     */
    struct jdksavdecc_eui64 m_newId;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.14
     */
    bool m_nopTimeout;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.15
     */
    struct jdksavdecc_eui48 m_primaryMac;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.16
     */
    bool m_responseValid;

    /**
     * See IEEE 1722.1 Annex C.5.3.1.17
     */
    bool m_tcpConnected;

    /**
     * See IEEE Std 1722.1 Annex C.5.3.2.1
     */
    void ( *closeTcpConnection )( struct jdksavb_apc *self );

    /**
     * See IEEE Std 1722.1 Annex C.5.3.2.2
     */
    void ( *connectToProxy )( struct jdksavb_apc *self, struct jdksavb_apc_Addr const *addr );

    /**
     * See IEEE Std 1722.1 Annex C.5.3.2.3
     */
    bool ( *getHttpResponse )( struct jdksavb_apc *self );

    /**
     * See IEEE Std 1722.1 Annex C.5.3.2.4
     */
    void ( *initialize )( struct jdksavb_apc *self );

    /**
     * See IEEE Std 1722.1 Annex C.5.3.2.5
     */
    void ( *notifyLinkStatus )( struct jdksavb_apc *self, struct jdksavdecc_appdu const *linkMsg );

    /**
     * See IEEE Std 1722.1 Annex C.5.3.2.6
     */
    void ( *processMsg )( struct jdksavb_apc *self, struct jdksavdecc_appdu const *apsMsg );

    /**
     * See IEEE Std 1722.1 Annex C.5.3.2.7
     */
    void ( *sendIdRequest )( struct jdksavb_apc *self, struct jdksavdecc_eui48 primaryMac, struct jdksavdecc_eui64 entity_id );

    /**
     * See IEEE Std 1722.1 Annex C.5.3.2.8
     */
    void ( *sendHttpRequest )( struct jdksavb_apc *self, struct jdksavb_apc_Addr const *addr );

    /**
     * See IEEE Std 1722.1 Annex C.5.3.2.9
     */
    void ( *sendMsgToAps )( struct jdksavb_apc *self, struct jdksavdecc_appdu const *apcMsg );

    /**
     * See IEEE Std 1722.1 Annex C.5.3.2.10
     */
    void ( *sendNopToAps )( struct jdksavb_apc *self );

    /**
     * See IEEE Std 1722.1 Annex C.5.3.2.X
     */
    void ( *notifyNewEntityId )( struct jdksavb_apc *self, struct jdksavdecc_eui64 entity_id );
};

void jdksavb_apc_init( struct jdksavb_apc *self, struct jdksavb_netrequest *net );

void jdksavb_apc_terminate( struct jdksavb_apc *self );

void jdksavb_apc_start( struct jdksavb_apc *self,
                        struct jdksavb_apc_Addr const *addr,
                        struct jdksavdecc_eui48 primaryMac,
                        struct jdksavdecc_eui64 preferred_entity_id );

void jdksavb_apc_execute_state( struct jdksavb_apc *self );

void jdksavb_apc_finish( struct jdksavb_apc *self );

void jdksavb_apc_NetEventSocketReadable( struct jdksavb_netevent *self, void const *data, ssize_t data_len );

void jdksavb_apc_NetEventSocketConnected( struct jdksavb_netevent *self );

void jdksavb_apc_NetEventSocketWritable( struct jdksavb_netevent *self );

void jdksavb_apc_NetEventSocketClosed( struct jdksavb_netevent *self );

void jdksavb_apc_NetEventSocketTimePassed( struct jdksavb_netevent *self, jdksavdecc_timestamp_in_milliseconds timestamp );

void jdksavb_apc_goto_state_BEGIN( struct jdksavb_apc *self );

void jdksavb_apc_execute_state_BEGIN( struct jdksavb_apc *self );

void jdksavb_apc_goto_state_INITIALIZE( struct jdksavb_apc *self );

void jdksavb_apc_execute_state_INITIALIZE( struct jdksavb_apc *self );

void jdksavb_apc_goto_state_WAIT_FOR_CONNECT( struct jdksavb_apc *self );

void jdksavb_apc_execute_state_WAIT_FOR_CONNECT( struct jdksavb_apc *self );

void jdksavb_apc_goto_state_ACCEPT( struct jdksavb_apc *self );

void jdksavb_apc_execute_state_ACCEPT( struct jdksavb_apc *self );

void jdksavb_apc_goto_state_START_TRANSFER( struct jdksavb_apc *self );

void jdksavb_apc_execute_state_START_TRANSFER( struct jdksavb_apc *self );

void jdksavb_apc_goto_state_WAITING( struct jdksavb_apc *self );

void jdksavb_apc_execute_state_WAITING( struct jdksavb_apc *self );

void jdksavb_apc_goto_state_CLOSED( struct jdksavb_apc *self );

void jdksavb_apc_execute_state_CLOSED( struct jdksavb_apc *self );

void jdksavb_apc_goto_state_LINK_STATUS( struct jdksavb_apc *self );

void jdksavb_apc_execute_state_LINK_STATUS( struct jdksavb_apc *self );

void jdksavb_apc_goto_state_RECEIVE_MSG( struct jdksavb_apc *self );

void jdksavb_apc_execute_state_RECEIVE_MSG( struct jdksavb_apc *self );

void jdksavb_apc_goto_state_SEND_MSG( struct jdksavb_apc *self );

void jdksavb_apc_execute_state_SEND_MSG( struct jdksavb_apc *self );

void jdksavb_apc_goto_state_ENTITY_ID_ASSIGNED( struct jdksavb_apc *self );

void jdksavb_apc_execute_state_ENTITY_ID_ASSIGNED( struct jdksavb_apc *self );

void jdksavb_apc_goto_state_SEND_NOP( struct jdksavb_apc *self );

void jdksavb_apc_execute_state_SEND_NOP( struct jdksavb_apc *self );

void jdksavb_apc_goto_state_FINISH( struct jdksavb_apc *self );

void jdksavb_apc_execute_state_FINISH( struct jdksavb_apc *self );

void jdksavb_apc_goto_state_END( struct jdksavb_apc *self );

void jdksavb_apc_execute_state_END( struct jdksavb_apc *self );

bool jdksavb_apc_send( struct jdksavb_apc *self, struct jdksavdecc_frame const *avdecc_frame );

void jdksavb_apc_CloseTcpConnection( struct jdksavb_apc *self );

void jdksavb_apc_ConnectToProxy( struct jdksavb_apc *self, struct jdksavb_apc_Addr const *addr );

bool jdksavb_apc_GetHttpResponse( struct jdksavb_apc *self );

void jdksavb_apc_Initialize( struct jdksavb_apc *self );

void jdksavb_apc_NotifyLinkStatus( struct jdksavb_apc *self, struct jdksavdecc_appdu const *linkMsg );

void jdksavb_apc_ProcessMsg( struct jdksavb_apc *self, struct jdksavdecc_appdu const *apsMsg );

void jdksavb_apc_SendIdRequest( struct jdksavb_apc *self,
                                struct jdksavdecc_eui48 primaryMac,
                                struct jdksavdecc_eui64 entity_id );

void jdksavb_apc_SendHttpRequest( struct jdksavb_apc *self, struct jdksavb_apc_Addr const *addr );

void jdksavb_apc_SendMsgToAps( struct jdksavb_apc *self, struct jdksavdecc_appdu const *apcMsg );

void jdksavb_apc_SendNopToAps( struct jdksavb_apc *self );

void jdksavb_apc_NotifyNewEntityId( struct jdksavb_apc *self, struct jdksavdecc_eui64 entity_id );

#ifdef __cplusplus
}
#endif
