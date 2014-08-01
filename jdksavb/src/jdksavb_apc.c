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

#include "jdksavb_world.h"
#include "jdksavb_apc.h"

void jdksavb_apc_init( struct jdksavb_apc *self, struct jdksavb_netrequest *net )
{
    self->m_net = net;
    self->m_base.netEventSocketReadable = jdksavb_apc_NetEventSocketReadable;
    self->m_base.netEventSocketConnected = jdksavb_apc_NetEventSocketConnected;
    self->m_base.netEventSocketWritable = jdksavb_apc_NetEventSocketWritable;
    self->m_base.netEventSocketClosed = jdksavb_apc_NetEventSocketClosed;
    self->m_base.netEventSocketTimePassed = jdksavb_apc_NetEventSocketTimePassed;
    self->closeTcpConnection = jdksavb_apc_CloseTcpConnection;
    self->connectToProxy = jdksavb_apc_ConnectToProxy;
    self->getHttpResponse = jdksavb_apc_GetHttpResponse;
    self->initialize = jdksavb_apc_Initialize;
    self->notifyLinkStatus = jdksavb_apc_NotifyLinkStatus;
    self->processMsg = jdksavb_apc_ProcessMsg;
    self->sendHttpRequest = jdksavb_apc_SendHttpRequest;
    self->sendIdRequest = jdksavb_apc_SendIdRequest;
    self->sendMsgToAps = jdksavb_apc_SendMsgToAps;
    self->sendNopToAps = jdksavb_apc_SendNopToAps;
    self->notifyNewEntityId = jdksavb_apc_NotifyNewEntityId;
}

void jdksavb_apc_terminate( struct jdksavb_apc *self )
{
    /* Nothing to do here */
}

void jdksavb_apc_start( struct jdksavb_apc *self,
                        struct jdksavb_apc_Addr const *addr,
                        struct jdksavdecc_eui48 primaryMac,
                        struct jdksavdecc_eui64 preferred_entity_id )
{
    self->m_addr = *addr;
    self->m_primaryMac = primaryMac;
    self->m_entityId = preferred_entity_id;
    jdksavb_apc_goto_state_BEGIN( self );
}

void jdksavb_apc_finish( struct jdksavb_apc *self )
{
    self->m_finished = true;
}

void jdksavb_apc_NetEventSocketReadable( struct jdksavb_netevent *self, void const *data, ssize_t data_len )
{
    /* TODO */
}

void jdksavb_apc_NetEventSocketConnected( struct jdksavb_netevent *self )
{
    /* TODO */
}

void jdksavb_apc_NetEventSocketWritable( struct jdksavb_netevent *self )
{
    /* TODO */
}

void jdksavb_apc_NetEventSocketClosed( struct jdksavb_netevent *self )
{
    /* TODO */
}

void jdksavb_apc_NetEventSocketTimePassed( struct jdksavb_netevent *self, jdksavdecc_timestamp_in_milliseconds timestamp )
{
    /* TODO */
}

void jdksavb_apc_CloseTcpConnection( struct jdksavb_apc *self )
{
    /* TODO */
}

void jdksavb_apc_ConnectToProxy( struct jdksavb_apc *self, struct jdksavb_apc_Addr const *addr )
{
    /* TODO */
}

bool jdksavb_apc_GetHttpResponse( struct jdksavb_apc *self )
{
    /* TODO */
    return false;
}

void jdksavb_apc_Initialize( struct jdksavb_apc *self )
{
    /* See IEEE Std 1722.1 Annex C.5.3.2.4 */
    self->m_apcMsgOut = false;
    self->m_apsMsgIn = false;
    self->m_finished = false;
    self->m_idAssigned = false;
    self->m_incomingTcpClosed = false;
    self->m_linkStatusMsg = false;
    self->m_responseValid = false;
    self->m_tcpConnected = false;
}

void jdksavb_apc_NotifyLinkStatus( struct jdksavb_apc *self, struct jdksavdecc_appdu const *linkMsg )
{
    /* TODO This is normally overridden by application */
}

void jdksavb_apc_ProcessMsg( struct jdksavb_apc *self, struct jdksavdecc_appdu const *apsMsg )
{
    /* TODO This is normally overridden by application */
}

void jdksavb_apc_NotifyNewEntityId( struct jdksavb_apc *self, struct jdksavdecc_eui64 entity_id )
{
    /* TODO This is normally overriden by application */
}

void
    jdksavb_apc_SendIdRequest( struct jdksavb_apc *self, struct jdksavdecc_eui48 primaryMac, struct jdksavdecc_eui64 entity_id )
{
    /* TODO Form and Send ID Request */
}

void jdksavb_apc_SendHttpRequest( struct jdksavb_apc *self, struct jdksavb_apc_Addr const *addr )
{
    /* TODO Form and send HTTP request */
}

void jdksavb_apc_SendMsgToAps( struct jdksavb_apc *self, struct jdksavdecc_appdu const *apcMsg )
{
    /* TODO form and send apcMsg */
}

void jdksavb_apc_SendNopToAps( struct jdksavb_apc *self )
{
    /* TODO form and semd a Nop msg */
}

void jdksavb_apc_execute_state( struct jdksavb_apc *self )
{
    switch ( self->m_state )
    {
    case jdksavb_apc_State_BEGIN:
        jdksavb_apc_execute_state_BEGIN( self );
        break;
    case jdksavb_apc_State_INITIALIZE:
        jdksavb_apc_execute_state_INITIALIZE( self );
        break;
    case jdksavb_apc_State_WAIT_FOR_CONNECT:
        jdksavb_apc_execute_state_WAIT_FOR_CONNECT( self );
        break;
    case jdksavb_apc_State_ACCEPT:
        jdksavb_apc_execute_state_ACCEPT( self );
        break;
    case jdksavb_apc_State_START_TRANSFER:
        jdksavb_apc_execute_state_START_TRANSFER( self );
        break;
    case jdksavb_apc_State_WAITING:
        jdksavb_apc_execute_state_WAITING( self );
        break;
    case jdksavb_apc_State_CLOSED:
        jdksavb_apc_execute_state_CLOSED( self );
        break;
    case jdksavb_apc_State_LINK_STATUS:
        jdksavb_apc_execute_state_LINK_STATUS( self );
        break;
    case jdksavb_apc_State_RECEIVE_MSG:
        jdksavb_apc_execute_state_RECEIVE_MSG( self );
        break;
    case jdksavb_apc_State_SEND_MSG:
        jdksavb_apc_execute_state_SEND_MSG( self );
        break;
    case jdksavb_apc_State_ENTITY_ID_ASSIGNED:
        jdksavb_apc_execute_state_ENTITY_ID_ASSIGNED( self );
        break;
    case jdksavb_apc_State_SEND_NOP:
        jdksavb_apc_execute_state_SEND_NOP( self );
        break;
    case jdksavb_apc_State_FINISH:
        jdksavb_apc_execute_state_FINISH( self );
        break;
    case jdksavb_apc_State_END:
        jdksavb_apc_execute_state_END( self );
        break;
    default:
        self->m_state = jdksavb_apc_State_END;
        break;
    }
}

void jdksavb_apc_goto_state_BEGIN( struct jdksavb_apc *self )
{
    self->m_state = jdksavb_apc_State_BEGIN;
}

void jdksavb_apc_execute_state_BEGIN( struct jdksavb_apc *self )
{
    jdksavb_apc_goto_state_INITIALIZE( self );
}

void jdksavb_apc_goto_state_INITIALIZE( struct jdksavb_apc *self )
{
    self->m_state = jdksavb_apc_State_INITIALIZE;
    self->initialize( self );
    self->connectToProxy( self, &self->m_addr );
}

void jdksavb_apc_execute_state_INITIALIZE( struct jdksavb_apc *self )
{
    jdksavb_apc_goto_state_WAIT_FOR_CONNECT( self );
}

void jdksavb_apc_goto_state_WAIT_FOR_CONNECT( struct jdksavb_apc *self )
{
    self->m_state = jdksavb_apc_State_WAIT_FOR_CONNECT;
}

void jdksavb_apc_execute_state_WAIT_FOR_CONNECT( struct jdksavb_apc *self )
{
    if ( self->m_tcpConnected )
    {
        jdksavb_apc_goto_state_ACCEPT( self );
    }
    else if ( self->m_finished )
    {
        jdksavb_apc_goto_state_FINISH( self );
    }
}

void jdksavb_apc_goto_state_ACCEPT( struct jdksavb_apc *self )
{
    self->m_state = jdksavb_apc_State_ACCEPT;
    self->sendHttpRequest( self, &self->m_addr );
}

void jdksavb_apc_execute_state_ACCEPT( struct jdksavb_apc *self )
{
    if ( self->getHttpResponse( self ) )
    {
        jdksavb_apc_goto_state_START_TRANSFER( self );
    }
    else
    {
        jdksavb_apc_goto_state_CLOSED( self );
    }
}

void jdksavb_apc_goto_state_START_TRANSFER( struct jdksavb_apc *self )
{
    self->m_state = jdksavb_apc_State_START_TRANSFER;
    self->sendIdRequest( self, self->m_primaryMac, self->m_entityId );
    self->m_nopTimeout = self->m_currentTime + 10000;
}

void jdksavb_apc_execute_state_START_TRANSFER( struct jdksavb_apc *self )
{
    jdksavb_apc_goto_state_WAITING( self );
}

void jdksavb_apc_goto_state_WAITING( struct jdksavb_apc *self )
{
    self->m_state = jdksavb_apc_State_WAITING;
}

void jdksavb_apc_execute_state_WAITING( struct jdksavb_apc *self )
{
    if ( self->m_incomingTcpClosed )
    {
        jdksavb_apc_goto_state_CLOSED( self );
    }
    else if ( self->m_finished )
    {
        jdksavb_apc_goto_state_CLOSED( self );
    }
    else if ( self->m_linkStatusMsg )
    {
        jdksavb_apc_goto_state_LINK_STATUS( self );
    }
    else if ( self->m_apsMsgIn )
    {
        jdksavb_apc_goto_state_RECEIVE_MSG( self );
    }
    else if ( self->m_apcMsgOut )
    {
        jdksavb_apc_goto_state_SEND_MSG( self );
    }
    else if ( self->m_idAssigned )
    {
        jdksavb_apc_goto_state_ENTITY_ID_ASSIGNED( self );
    }
    else if ( self->m_currentTime > self->m_nopTimeout )
    {
        jdksavb_apc_goto_state_SEND_NOP( self );
    }
}

void jdksavb_apc_goto_state_CLOSED( struct jdksavb_apc *self )
{
    self->m_state = jdksavb_apc_State_CLOSED;
    self->closeTcpConnection( self );
}

void jdksavb_apc_execute_state_CLOSED( struct jdksavb_apc *self )
{
    jdksavb_apc_goto_state_FINISH( self );
}

void jdksavb_apc_goto_state_LINK_STATUS( struct jdksavb_apc *self )
{
    self->m_state = jdksavb_apc_State_LINK_STATUS;
    self->notifyLinkStatus( self, &self->m_linkMsg );
    self->m_linkStatusMsg = false;
}

void jdksavb_apc_execute_state_LINK_STATUS( struct jdksavb_apc *self )
{
    jdksavb_apc_goto_state_WAITING( self );
}

void jdksavb_apc_goto_state_RECEIVE_MSG( struct jdksavb_apc *self )
{
    self->m_state = jdksavb_apc_State_RECEIVE_MSG;
    self->processMsg( self, &self->m_apsMsg );
    self->m_apsMsgIn = false;
}

void jdksavb_apc_execute_state_RECEIVE_MSG( struct jdksavb_apc *self )
{
    jdksavb_apc_goto_state_WAITING( self );
}

void jdksavb_apc_goto_state_SEND_MSG( struct jdksavb_apc *self )
{
    self->m_state = jdksavb_apc_State_SEND_MSG;
    self->sendMsgToAps( self, &self->m_apcMsg );
    self->m_apcMsgOut = false;
    self->m_nopTimeout = self->m_currentTime + 10000;
}

void jdksavb_apc_execute_state_SEND_MSG( struct jdksavb_apc *self )
{
    jdksavb_apc_goto_state_WAITING( self );
}

void jdksavb_apc_goto_state_ENTITY_ID_ASSIGNED( struct jdksavb_apc *self )
{
    self->m_state = jdksavb_apc_State_ENTITY_ID_ASSIGNED;
    self->m_entityId = self->m_newId;
    self->notifyNewEntityId( self, self->m_entityId );
    self->m_nopTimeout = self->m_currentTime + 10000;
}

void jdksavb_apc_execute_state_ENTITY_ID_ASSIGNED( struct jdksavb_apc *self )
{
    jdksavb_apc_goto_state_WAITING( self );
}

void jdksavb_apc_goto_state_SEND_NOP( struct jdksavb_apc *self )
{
    self->m_state = jdksavb_apc_State_SEND_NOP;
    self->sendNopToAps( self );
    self->m_nopTimeout = self->m_currentTime + 10000;
}

void jdksavb_apc_execute_state_SEND_NOP( struct jdksavb_apc *self )
{
    jdksavb_apc_goto_state_WAITING( self );
}

void jdksavb_apc_goto_state_FINISH( struct jdksavb_apc *self )
{
    self->m_state = jdksavb_apc_State_FINISH;
}

void jdksavb_apc_execute_state_FINISH( struct jdksavb_apc *self )
{
    jdksavb_apc_goto_state_END( self );
}

void jdksavb_apc_goto_state_END( struct jdksavb_apc *self )
{
    self->m_state = jdksavb_apc_State_END;
}

void jdksavb_apc_execute_state_END( struct jdksavb_apc *self )
{
    /* Do Nothing */
}

bool jdksavb_apc_send( struct jdksavb_apc *self, struct jdksavdecc_frame const *avdecc_frame )
{
    bool r = false;

    if ( !self->m_apcMsgOut )
    {
        jdksavdecc_appdu_set_avdecc_from_apc(
            &self->m_apcMsg, avdecc_frame->dest_address, avdecc_frame->length, avdecc_frame->payload );
        self->m_apcMsgOut = true;
        r = true;
    }
    return r;
}
