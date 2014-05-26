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
#include "jdksavb_frame.h"
#include "jdksavb_acmp_controller_signals.h"
#include "jdksavb_acmp_controller_slots.h"

#ifdef __cplusplus
extern "C" {
#endif

struct jdksavb_acmp_controller_stream_source;

/** \addtogroup acmp_controller ACMP Controller State Machine */
/*@{*/

struct jdksavb_acmp_controller_stream_source {
    struct jdksavdecc_eui64 talker_entity_id;
    uint16_t talker_unique_id;
    struct jdksavdecc_eui48 destination_mac_address;
    struct jdksavdecc_eui64 stream_id;
    uint16_t flags;
    uint8_t status;
    uint16_t connection_count;
    uint16_t stream_vlan_id;
};

struct jdksavb_acmp_controller_stream_sink {
    struct jdksavdecc_eui64 listener_entity_id;
    uint16_t listener_unique_id;
    uint16_t flags;
    uint8_t status;
};

struct jdksavb_acmp_controller_connection {
    enum {
        JDKSAVB_ACMP_CONTROLLER_STREAM_SINK_STATE_DISABLED = 0,
        JDKSAVB_ACMP_CONTROLLER_STREAM_SINK_STATE_DISCONNECTED,
        JDKSAVB_ACMP_CONTROLLER_STREAM_SINK_STATE_CONNECTING,
        JDKSAVB_ACMP_CONTROLLER_STREAM_SINK_STATE_CONNECTED,
        JDKSAVB_ACMP_CONTROLLER_STREAM_SINK_STATE_DISCONNECTING,
        JDKSAVB_ACMP_CONTROLLER_STREAM_SINK_STATE_ERROR
    } state;
    struct jdksavdecc_eui64 talker_entity_id;
    uint16_t talker_unique_id;
    struct jdksavdecc_eui64 listener_entity_id;
    uint16_t listener_unique_id;
};

struct jdksavb_acmp_controller {
    struct jdksavb_acmp_controller_slots incoming_slots;
    struct jdksavb_acmp_controller_signals outgoing_signals;

    int max_stream_sources;
    struct jdksavb_acmp_controller_stream_source *stream_sources;
    int max_stream_sinks;
    struct jdksavb_acmp_controller_stream_source *stream_sinks;
    int max_connections;
    struct jdksavb_acmp_controller_connection *stream_connections;
};

bool jdksavb_acmp_controller_init(struct jdksavb_acmp_controller *self,
                                  struct jdksavb_frame_sender *frame_sender,
                                  uint32_t tag,
                                  void *additional);

/// Destroy any resources that the jdksavb_acmp_controller uses
void jdksavb_acmp_controller_destroy(void *self);

/// Receive an ACMPDU and process it
bool jdksavb_acmp_controller_rx_frame(void *self, struct jdksavb_frame *rx_frame, size_t pos);

/// Notify the state machine that time has passed. Call asap if early_tick is true.
void jdksavb_acmp_controller_tick(void *self, jdksavdecc_timestamp_in_microseconds timestamp);

/*@}*/

/*@}*/

#ifdef __cplusplus
}
#endif
