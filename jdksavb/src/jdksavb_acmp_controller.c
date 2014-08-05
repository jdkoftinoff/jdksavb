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
#include "jdksavb_acmp_controller.h"

static struct jdksavb_acmp_controller_slots jdksavb_acmp_controller_slots_table
    = {jdksavb_acmp_controller_terminate,           jdksavb_acmp_controller_connect_signals,
       jdksavb_acmp_controller_disconnect_signals,  jdksavb_acmp_controller_start,
       jdksavb_acmp_controller_stop,                jdksavb_acmp_controller_handle_pdu,
       jdksavb_acmp_controller_tick,                jdksavb_acmp_controller_track_stream_sink,
       jdksavb_acmp_controller_track_stream_source, jdksavb_acmp_controller_connect_stream};

void jdksavb_acmp_controller_init( struct jdksavb_acmp_controller *self,
                                   int max_stream_sources,
                                   struct jdksavb_acmp_controller_stream_source *stream_sources,
                                   int max_stream_sinks,
                                   struct jdksavb_acmp_controller_stream_sink *stream_sinks,
                                   int max_connections,
                                   struct jdksavb_acmp_controller_connection *stream_connections )
{
    self->slots = jdksavb_acmp_controller_slots_table;
    self->signals = 0;
    self->state = JDKSAVB_ACMP_CONTROLLER_STOPPED;
    self->max_stream_sources = max_stream_sources;
    self->num_stream_sources = 0;
    self->stream_sources = stream_sources;
    self->max_stream_sinks = max_stream_sinks;
    self->num_stream_sinks = 0;
    self->stream_sinks = stream_sinks;
    self->max_connections = max_connections;
    self->num_connections = 0;
    self->stream_connections = stream_connections;
}

/** Ask the object to to terminate */
void jdksavb_acmp_controller_terminate( struct jdksavb_acmp_controller_slots *self_ )
{
    struct jdksavb_acmp_controller *self = (struct jdksavb_acmp_controller *)self_;
    if ( self->signals )
    {
        self_->disconnect_signals( self_, self->signals );
    }
}

/** Ask the object to send its signals to destination_signals */
void jdksavb_acmp_controller_connect_signals( struct jdksavb_acmp_controller_slots *self_,
                                              struct jdksavb_acmp_controller_signals *destination_signals )
{
    struct jdksavb_acmp_controller *self = (struct jdksavb_acmp_controller *)self_;
    self->signals = destination_signals;
    destination_signals->acmp_controller_connected( destination_signals, self_ );
}

/** Ask the object to disconnect the destination_signals */
void jdksavb_acmp_controller_disconnect_signals( struct jdksavb_acmp_controller_slots *self_,
                                                 struct jdksavb_acmp_controller_signals *destination_signals )
{
    struct jdksavb_acmp_controller *self = (struct jdksavb_acmp_controller *)self_;
    destination_signals->acmp_controller_disconnected( destination_signals );
    self->signals = 0;
}

/** Ask the object to start processing */
void jdksavb_acmp_controller_start( struct jdksavb_acmp_controller_slots *self_ )
{
    struct jdksavb_acmp_controller *self = (struct jdksavb_acmp_controller *)self_;
    if ( JDKSAVB_ACMP_CONTROLLER_STOPPED == self->state )
    {
        /* TODO: Initialize state machines */
        self->state = JDKSAVB_ACMP_CONTROLLER_STARTED;
        if ( self->signals )
        {
            self->signals->acmp_controller_started( self->signals );
        }
    }
}

/** Ask the object to stop processing */
void jdksavb_acmp_controller_stop( struct jdksavb_acmp_controller_slots *self_ )
{
    struct jdksavb_acmp_controller *self = (struct jdksavb_acmp_controller *)self_;

    if ( JDKSAVB_ACMP_CONTROLLER_STARTED == self->state )
    {
        /* TODO: Halt state machines */
        self->state = JDKSAVB_ACMP_CONTROLLER_STOPPED;
        if ( self->signals )
        {
            self->signals->acmp_controller_stopped( self->signals );
        }
    }
}

/** Ask the object to handle the specified ethernet frame */
void jdksavb_acmp_controller_handle_pdu( struct jdksavb_acmp_controller_slots *self_, struct jdksavb_frame *frame )
{
    struct jdksavb_acmp_controller *self = (struct jdksavb_acmp_controller *)self_;
}

/** Ask the object to process any periodic timers */
void jdksavb_acmp_controller_tick( struct jdksavb_acmp_controller_slots *self_,
                                   jdksavdecc_timestamp_in_microseconds current_time )
{
    struct jdksavb_acmp_controller *self = (struct jdksavb_acmp_controller *)self_;
}

/** Ask the object to track a specific stream sink */
void jdksavb_acmp_controller_track_stream_sink( struct jdksavb_acmp_controller_slots *self_,
                                                struct jdksavdecc_eui64 listener_entity_id,
                                                uint16_t listener_unique_id,
                                                bool enable )
{
    struct jdksavb_acmp_controller *self = (struct jdksavb_acmp_controller *)self_;
}

/** Ask the object to track a specific stream source */
void jdksavb_acmp_controller_track_stream_source( struct jdksavb_acmp_controller_slots *self_,
                                                  struct jdksavdecc_eui64 talker_entity_id,
                                                  uint16_t talker_unique_id,
                                                  bool enable )
{
    struct jdksavb_acmp_controller *self = (struct jdksavb_acmp_controller *)self_;
}

/** Ask the object to connect a stream source to a stream sink */
void jdksavb_acmp_controller_connect_stream( struct jdksavb_acmp_controller_slots *self_,
                                             struct jdksavdecc_eui64 talker_entity_id,
                                             uint16_t talker_unique_id,
                                             struct jdksavdecc_eui64 listener_entity_id,
                                             uint16_t listener_unique_id,
                                             bool enable )
{
    struct jdksavb_acmp_controller *self = (struct jdksavb_acmp_controller *)self_;
}
