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
#include "jdksavb_world.h"

#ifdef __cplusplus
extern "C" {
#endif

struct jdksavb_net_slots
{
    void ( *terminate )( struct jdksavb_acmp_controller_slots *self );

    void ( *connect_signals )( struct jdksavb_acmp_controller_slots *self,
                               struct jdksavb_acmp_controller_signals *destination_signals );

    void ( *disconnect_signals )( struct jdksavb_acmp_controller_slots *self,
                                  struct jdksavb_acmp_controller_signals *destination_signals );

    /**
     * External Networking Request: The state machine wants to wake up when the socket is writable
     */
    void ( *wake_on_writable )( struct jdksavb_net_slots *self, struct jdksavb_net_signals *netEventHandler, bool enable );

    /**
     * External Networking Request: The state machine wants to connect to a destination
     */
    void ( *connect )( struct jdksavb_net_slots *self,
                       struct jdksavb_net_signals *netEventHandler,
                       struct sockaddr const *addr,
                       socklen_t addr_len );

    /**
     * External Networking Request: The state machine wants to close the socket
     */
    void ( *close )( struct jdksavb_net_slots *self, struct jdksavb_net_signals *netEventHandler );

    /**
     * External Networking Request: The state machine wants to send some data
     */
    ssize_t ( *send )( struct jdksavb_net_slots *self,
                       struct jdksavb_net_signals *netEventHandler,
                       void const *data,
                       size_t data_len );

    /**
     * External Networking Request: The state machine wants to be woken up in the future
     */
    void ( *wake_up )( struct jdksavb_net_slots *self,
                       struct jdksavb_net_signals *netEventHandler,
                       uint32_t delta_time_in_milliseconds );
};

#ifdef __cplusplus
}
#endif
