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
#include "jdksavb_acmp_controller.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup acmp_controller ACMP Controller State Machine */
/*@{*/

struct jdksavb_acmp_controller
{
    /** The list of messages that the controller can receive */
    struct jdksavb_acmp_controller_slots slots;

    /** The list of signals that the controller can send */
    struct jdksavb_acmp_controller_signals *signals;

    enum
    {
        JDKSAVB_ACMP_CONTROLLER_STOPPED,
        JDKSAVB_ACMP_CONTROLLER_STARTED
    } state;

    /** The maximum number of stream sources that the stream_sources parameter can hold */
    int max_stream_sources;

    /** The current number of stream sources that the stream_sources parameter is holding */
    int num_stream_sources;

    /** The array of stream sources */
    struct jdksavb_acmp_controller_stream_source *stream_sources;

    /** The maximum number of stream sinks that the stream_sinks parameter can hold */
    int max_stream_sinks;

    /** The current number of stream sources that the stream_sinks parameter is holding */
    int num_stream_sinks;

    /** The array of stream sinks */
    struct jdksavb_acmp_controller_stream_sink *stream_sinks;

    /** The maximum number of stream connections that the stream_connections parameter can hold */
    int max_connections;

    /** The current number of stream connections that the stream_connections parameter is holding */
    int num_connections;

    /** The array of stream connections */
    struct jdksavb_acmp_controller_connection *stream_connections;
};

/** Initialize an ACMP Controller */
void jdksavb_acmp_controller_init( struct jdksavb_acmp_controller *self,
                                   int max_stream_sources,
                                   struct jdksavb_acmp_controller_stream_source *stream_sources,
                                   int max_stream_sinks,
                                   struct jdksavb_acmp_controller_stream_sink *stream_sinks,
                                   int max_connections,
                                   struct jdksavb_acmp_controller_connection *stream_connections );

/** Ask the object to to terminate */
void jdksavb_acmp_controller_terminate( struct jdksavb_acmp_controller_slots *self );

/** Ask the object to send its signals to destination_signals */
void jdksavb_acmp_controller_connect_signals( struct jdksavb_acmp_controller_slots *self,
                                              struct jdksavb_acmp_controller_signals *destination_signals );

/** Ask the object to disconnect the destination_signals */
void jdksavb_acmp_controller_disconnect_signals( struct jdksavb_acmp_controller_slots *self,
                                                 struct jdksavb_acmp_controller_signals *destination_signals );

/** Ask the object to start processing */
void jdksavb_acmp_controller_start( struct jdksavb_acmp_controller_slots *self );

/** Ask the object to stop processing */
void jdksavb_acmp_controller_stop( struct jdksavb_acmp_controller_slots *self );

/** Ask the object to handle the specified ethernet frame */
void jdksavb_acmp_controller_handle_pdu( struct jdksavb_acmp_controller_slots *self, struct jdksavb_frame *frame );

/** Ask the object to process any periodic timers */
void jdksavb_acmp_controller_tick( struct jdksavb_acmp_controller_slots *self,
                                   jdksavdecc_timestamp_in_microseconds current_time );

/** Ask the object to track a specific stream sink */
void jdksavb_acmp_controller_track_stream_sink( struct jdksavb_acmp_controller_slots *self,
                                                struct jdksavdecc_eui64 listener_entity_id,
                                                uint16_t listener_unique_id,
                                                bool enable );

/** Ask the object to track a specific stream source */
void jdksavb_acmp_controller_track_stream_source( struct jdksavb_acmp_controller_slots *self,
                                                  struct jdksavdecc_eui64 talker_entity_id,
                                                  uint16_t talker_unique_id,
                                                  bool enable );

/** Ask the object to connect a stream source to a stream sink */
void jdksavb_acmp_controller_connect_stream( struct jdksavb_acmp_controller_slots *self,
                                             struct jdksavdecc_eui64 talker_entity_id,
                                             uint16_t talker_unique_id,
                                             struct jdksavdecc_eui64 listener_entity_id,
                                             uint16_t listener_unique_id,
                                             bool enable );

/*@}*/

/*@}*/

#ifdef __cplusplus
}
#endif
