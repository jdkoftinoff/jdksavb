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
#include "jdksavb_state_machine.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup state_machines jdksavb_state_machines - A collection of
 * jdksavb_state_machine objects */
/*@{*/

/// Container for multiple state machines
struct jdksavb_state_machines {
    /// IsA state machine
    struct jdksavb_state_machine base;

    /// The number of state machines in the list
    int num_state_machines;

    /// The maximum number of state machines allowed
    int max_state_machines;

    /// The list of state machines
    struct jdksavb_state_machine **state_machines;

    /// Add a state machine to the list.
    /// Returns 0 on success
    int (*add_state_machine)(struct jdksavb_state_machines *self, struct jdksavb_state_machine *sm);
};

/// Initialize the state machine list.
int jdksavb_state_machines_init(struct jdksavb_state_machines *self,
                                int max_state_machines,
                                struct jdksavb_frame_sender *frame_sender,
                                uint32_t tag,
                                void *additional);

/// Destroy the state machine list and deallocate the list
void jdksavb_state_machines_destroy(struct jdksavb_state_machine *self);

/// Ask all the state machines to terminate
void jdksavb_state_machines_terminate(struct jdksavb_state_machine *self);

/// Dispatch a tick to all state machines.
/// Returns -1 when all state machines finish terminating
int jdksavb_state_machines_tick(struct jdksavb_state_machine *self, jdksavdecc_timestamp_in_microseconds timestamp);

/// Dispatch the rx_frame to all state machines.
/// Returns the largest parsed octet count that the state machines returned
ssize_t jdksavb_state_machines_rx_frame(struct jdksavb_state_machine *self, struct jdksavb_frame *rx_frame, size_t pos);

/// Add a state machine to the list.
/// Returns 0 on success, <0 if there is no room
int jdksavb_state_machines_add_state_machine(struct jdksavb_state_machines *self, struct jdksavb_state_machine *s);

/*@}*/

#ifdef __cplusplus
}
#endif
