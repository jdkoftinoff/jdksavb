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

#ifdef __cplusplus
extern "C" {
#endif

struct jdksavb_acmp_controller_stream_source;
struct jdksavb_acmp_controller_stream_sink;
struct jdksavb_acmp_controller_connection;
struct jdksavb_acmp_controller_signals;
struct jdksavb_acmp_controller_slots;

struct jdksavb_acmpdu_list_item
{

    struct jdksavdecc_acmpdu acmpdu;
};

struct jdksavb_acmpdu_list
{
    int max_items;
    int num_items;
    struct jdksavdecc_acmpdu *items;
};

void jdksavb_acmpdu_list_init( struct jdksavb_acmpdu_list *self, struct jdksavdecc_acmpdu *items, int max_items );

struct jdksavb_acmp_controller_stream_source
{
    jdksavdecc_timestamp_in_microseconds last_talker_refresh_time;
    struct jdksavdecc_acmpdu acmpdu;
};

struct jdksavb_acmp_controller_stream_sink
{
    jdksavdecc_timestamp_in_microseconds last_listener_refresh_time;
    struct jdksavdecc_acmpdu acmpdu;
};

struct jdksavb_acmp_controller_connection
{
    enum
    {
        JDKSAVB_ACMP_CONTROLLER_CONNECTION_DISABLED = 0,
        JDKSAVB_ACMP_CONTROLLER_CONNECTION_DISCONNECTED,
        JDKSAVB_ACMP_CONTROLLER_CONNECTION_CONNECTING,
        JDKSAVB_ACMP_CONTROLLER_CONNECTION_CONNECTED,
        JDKSAVB_ACMP_CONTROLLER_CONNECTION_DISCONNECTING,
        JDKSAVB_ACMP_CONTROLLER_CONNECTION_ERROR
    } state;
    int retry_count;
    struct jdksavdecc_eui64 talker_entity_id;
    uint16_t talker_unique_id;
    struct jdksavdecc_eui64 listener_entity_id;
    uint16_t listener_unique_id;
    struct jdksavb_acmp_controller_stream_source last_talker_connection_info;
    struct jdksavb_acmp_controller_stream_sink last_listener_info;
};

void jdksavb_acmp_controller_connection_init( struct jdksavb_acmp_controller_connection *self );

/** The actions that an ACMP Controller can be asked to perform */
struct jdksavb_acmp_controller_slots
{
    /** Ask the object to to terminate */
    void ( *terminate )( struct jdksavb_acmp_controller_slots *self );

    /** Ask the object to send its signals to destination_signals */
    void ( *connect_signals )( struct jdksavb_acmp_controller_slots *self,
                               struct jdksavb_acmp_controller_signals *destination_signals );

    /** Ask the object to disconnect the destination_signals */
    void ( *disconnect_signals )( struct jdksavb_acmp_controller_slots *self,
                                  struct jdksavb_acmp_controller_signals *destination_signals );

    /** Ask the object to start processing */
    void ( *start )( struct jdksavb_acmp_controller_slots *self );

    /** Ask the object to stop processing */
    void ( *stop )( struct jdksavb_acmp_controller_slots *self );

    /** Ask the object to handle the specified ethernet frame */
    void ( *handle_pdu )( struct jdksavb_acmp_controller_slots *self, struct jdksavb_frame *frame );

    /** Ask the object to process any periodic timers */
    void ( *tick )( struct jdksavb_acmp_controller_slots *self, jdksavdecc_timestamp_in_microseconds current_time );

    /** Ask the object to track a specific stream sink */
    void ( *track_stream_sink )( struct jdksavb_acmp_controller_slots *self,
                                 struct jdksavdecc_eui64 listener_entity_id,
                                 uint16_t listener_unique_id,
                                 bool enable );

    /** Ask the object to track a specific stream source */
    void ( *track_stream_source )( struct jdksavb_acmp_controller_slots *self,
                                   struct jdksavdecc_eui64 talker_entity_id,
                                   uint16_t talker_unique_id,
                                   bool enable );

    /** Ask the object to connect a stream source to a stream sink */
    void ( *connect_stream )( struct jdksavb_acmp_controller_slots *self,
                              struct jdksavdecc_eui64 talker_entity_id,
                              uint16_t talker_unique_id,
                              struct jdksavdecc_eui64 listener_entity_id,
                              uint16_t listener_unique_id,
                              bool enable );
};

/** The signals that an ACMP Controller can send to another object */
struct jdksavb_acmp_controller_signals
{
    /** The signals were connected */
    void ( *acmp_controller_connected )( struct jdksavb_acmp_controller_signals *self,
                                         struct jdksavb_acmp_controller_slots *acmp_controller_slots );

    /** The signals were disconnected */
    void ( *acmp_controller_disconnected )( struct jdksavb_acmp_controller_signals *self );

    /** The object was started */
    void ( *acmp_controller_started )( struct jdksavb_acmp_controller_signals *self );

    /** The object was stopped */
    void ( *acmp_controller_stopped )( struct jdksavb_acmp_controller_signals *self );

    /** The object is asking to send an ethernet frame */
    void ( *acmp_controller_send_pdu )( struct jdksavb_acmp_controller_signals *self, struct jdksavb_frame *acmpdu );

    /** The object is notifying that a stream source has been discovered or updated */
    void ( *acmp_controller_stream_source_found )( struct jdksavb_acmp_controller_signals *self,
                                                   struct jdksavb_acmp_controller_stream_source *info );

    /** The object is notifying that a stream sink has been discovered or updated */
    void ( *acmp_controller_stream_sink_found )( struct jdksavb_acmp_controller_signals *self,
                                                 struct jdksavb_acmp_controller_stream_sink *info );

    /** The object is notifying that a stream connection state changed */
    void ( *acmp_controller_connection_changed )( struct jdksavb_acmp_controller_signals *self,
                                                  struct jdksavb_acmp_controller_connection *info );
};

#ifdef __cplusplus
}
#endif
