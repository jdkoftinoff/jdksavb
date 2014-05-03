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

/** \addtogroup inflight Inflight message handling */
/*@{*/

#ifndef JDKSAVB_INFLIGHT_MAX_COMMANDS
#define JDKSAVB_INFLIGHT_MAX_COMMANDS (8)
#endif


struct jdksavb_inflight_command_info {
    /// This is the timestamp of the last command that we sent to another entity
    jdksavdecc_timestamp_in_milliseconds sent_time;

    /// This is the number of milliseconds before the message times out
    uint16_t time_out_in_ms;

    /// This is the entity id that was the target of the last command that we sent
    struct jdksavdecc_eui64 target_entity_id;

    /// This is the command_type of the last command that we sent to another entity
    uint16_t command_type;

    /// This is the sequence id of the command that was sent
    uint16_t sequence_id;

    /// This is the pointer to the function to call when the command timed out
    void (*timed_out)(
            struct jdksavb_inflight_command_info *self,
            void *context );
};

void jdksavb_inflight_command_info_init(
        struct jdksavb_inflight_command_info *self);

int jdksavb_inflight_command_info_compare(
        void const *lhs,
        void const *rhs );

struct jdksavb_inflight_commands {
    int num_inflight_commands;
    struct jdksavb_inflight_command_info inflight_commands[ JDKSAVB_INFLIGHT_MAX_COMMANDS ];
};

void jdksavb_inflight_commands_init( struct jdksavb_inflight_commands *self );

bool jdksacdecc_entity_manager_inflight_commands_full( struct jdksavb_inflight_commands *self );

void jdksavb_inflight_commands_sort(
        struct jdksavb_inflight_commands *self );

bool jdksavb_inflight_commands_add(
        struct jdksavb_inflight_commands *self,
        struct jdksavb_inflight_command_info const *info );

int jdksavb_inflight_commands_find(
        struct jdksavb_inflight_commands *self,
        struct jdksavdecc_eui64 const *target_entity_id,
        uint16_t sequence_id );

void jdksavb_inflight_commands_tick(
        struct jdksavb_inflight_commands *self,
        jdksavdecc_timestamp_in_milliseconds cur_time,
        void *context );

void jdksavb_inflight_commands_remove(
        struct jdksavb_inflight_commands *self,
        int num );

void jdksavb_inflight_commands_remove_target(
        struct jdksavb_inflight_commands *self,
        struct jdksavdecc_eui64 const *target_entity_id );



/*@}*/


#ifdef __cplusplus
}
#endif
