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

#include "jdksavb_statemachines.h"

int jdksavb_state_machines_init(struct jdksavb_state_machines *self,
                                int max_state_machines,
                                struct jdksavb_frame_sender *frame_sender,
                                uint32_t tag,
                                void *additional) {
    jdksavb_state_machine_init(&self->base, frame_sender, tag, additional);
    self->base.destroy = jdksavb_state_machines_destroy;
    self->base.tick = jdksavb_state_machines_tick;
    self->base.rx_frame = jdksavb_state_machines_rx_frame;
    self->add_state_machine = jdksavb_state_machines_add_state_machine;
    self->max_state_machines = max_state_machines;
    self->num_state_machines = 0;
    self->state_machines = calloc(sizeof(struct jdksavb_state_machine *), max_state_machines);
    if (self->state_machines) {
        return 0;
    } else {
        return 1;
    }
}

void jdksavb_state_machines_destroy(struct jdksavb_state_machine *self_) {
    struct jdksavb_state_machines *self = (struct jdksavb_state_machines *)self_;
    int i;

    // Destroy all sub-state machines
    for (i = 0; i < self->num_state_machines; ++i) {
        struct jdksavb_state_machine *sm = self->state_machines[i];
        sm->destroy(sm);
    }
    // free the memory for our list
    free(self->state_machines);
}

void jdksavb_state_machines_terminate(struct jdksavb_state_machine *self_) {
    struct jdksavb_state_machines *self = (struct jdksavb_state_machines *)self_;
    int i;

    // Tell all sub state machines to terminate. Don't terminate self now, that
    // happens after all sub state machines actually
    // terminate
    for (i = 0; i < self->num_state_machines; ++i) {
        struct jdksavb_state_machine *sm = self->state_machines[i];
        sm->terminate(sm);
        self->state_machines[i] = 0;
    }
    self->num_state_machines = 0;
}

int jdksavb_state_machines_tick(struct jdksavb_state_machine *self_, jdksavdecc_timestamp_in_microseconds timestamp) {
    int r = 0;
    struct jdksavb_state_machines *self = (struct jdksavb_state_machines *)self_;
    int i;

    // Try run the base class tick. Continue with our tick if it is not
    // terminated
    if (jdksavb_state_machine_tick(&self->base, timestamp) == 0) {
        // Run tick on all sub state machines. Keep track of how many sub state
        // machines are terminated
        int num_state_machines_ended = 0;
        for (i = 0; i < self->num_state_machines; ++i) {
            struct jdksavb_state_machine *sm = self->state_machines[i];

            // Run the sub state machine tick
            if (sm->tick(sm, timestamp) != 0) {
                // It was terminated, so count it
                num_state_machines_ended++;
            }
        }
        // Did all of our sub state machines terminate?
        if (num_state_machines_ended == self->num_state_machines) {
            // Yes, then we really are terminated
            self->base.terminated = 1;
        }
        r = 0;
    } else {
        r = -1;
    }

    return r;
}

ssize_t jdksavb_state_machines_rx_frame(struct jdksavb_state_machine *self_, struct jdksavb_frame *rx_frame, size_t pos) {
    struct jdksavb_state_machines *self = (struct jdksavb_state_machines *)self_;
    int i;
    ssize_t max_r = -1;

    // Pass the frame to all sub state machines
    for (i = 0; i < self->num_state_machines; ++i) {
        ssize_t r = -1;
        struct jdksavb_state_machine *sm = self->state_machines[i];

        // Call rx_frame for the state machine
        r = sm->rx_frame(sm, rx_frame, pos);

        // if the sub state machine needs an immediate tick
        // because of this event, do the tick now for this sub state machine
        // only,
        // using the timestamp of the frame

        if (sm->do_early_tick) {
            sm->tick(sm, rx_frame->time);
        }

        // Was it the frame handled?
        if (r > max_r) {
            max_r = r;
        }
    }

    return max_r;
}

int jdksavb_state_machines_add_state_machine(struct jdksavb_state_machines *self, struct jdksavb_state_machine *s) {
    int r = -1;

    if (self->state_machines && (self->num_state_machines < self->max_state_machines)) {
        r = 0;
        self->state_machines[self->num_state_machines++] = s;
    }

    return r;
}
