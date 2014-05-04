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

/** \addtogroup acmp_talker ACMP Listener State Machine */
/*@{*/

#ifndef JDKSAVB_ACMP_LISTENER_MAX_STREAMS
#define JDKSAVB_ACMP_LISTENER_MAX_STREAMS (8)
#endif

struct jdksavb_acmp_listener_context {
    enum {
        JDKSAVB_ACMP_LISTENER_STATE_DISABLED = 0,
        JDKSAVB_ACMP_LISTENER_STATE_DISCONNECTED,
        JDKSAVB_ACMP_LISTENER_STATE_FAST_CONNECTION_IN_PROGRESS,
        JDKSAVB_ACMP_LISTENER_STATE_CONNECTION_IN_PROGRESS,
        JDKSAVB_ACMP_LISTENER_STATE_CONNECTED,
        JDKSAVB_ACMP_LISTENER_STATE_DISCONNECTION_IN_PROGRESS,
    } state;

    struct jdksavdecc_eui48 destination_mac_address;
    struct jdksavdecc_eui64 stream_id;
    struct jdksavdecc_eui64 talker_entity_id;
    uint16_t talker_unique_id;
    uint16_t flags;
    uint16_t stream_vlan_id;
    void *context;
};

struct jdksavb_acmp_listener {
    struct jdksavdecc_state_machine *base;
    struct jdksavdecc_entity *entity;
    uint16_t listener_stream_sinks;
    struct jdksavb_acmp_listener_context listener_sink[JDKSAVB_ACMP_LISTENER_MAX_STREAMS];
};

bool jdksavb_acmp_listener_init(struct jdksavb_acmp_listener *self,
                                struct jdksavdecc_entity *entity,
                                struct jdksavb_frame_sender *frame_sender,
                                uint32_t tag,
                                void *additional);

/// Destroy any resources that the jdksavb_acmp_listener uses
void jdksavb_acmp_listener_destroy(struct jdksavdecc_state_machine *self);

/// Receive an ACMPDU and process it
bool jdksavb_acmp_listener_rx_frame(struct jdksavdecc_state_machine *self, struct jdksavb_frame *rx_frame, size_t pos);

/// Notify the state machine that time has passed. Call asap if early_tick is true.
void jdksavb_acmp_listener_tick(struct jdksavdecc_state_machine *self, jdksavdecc_timestamp_in_microseconds timestamp);

/*@}*/

#ifdef __cplusplus
}
#endif
