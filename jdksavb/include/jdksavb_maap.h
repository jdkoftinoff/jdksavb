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
#include "jdksavb_maap_signals.h"
#include "jdksavb_maap_slots.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup maap MAAP - Multicast Address Allocation Protocol
  * See IEEE Std 1722-2011 Annex B.2
  */
/*@{*/

/** \addtogroup maap MAAP State Machines */
/*@{*/

struct jdksavb_maap
{
    /// Inherit from slots
    struct jdksavb_maap_slots slots;
    struct jdksavb_maap_signals *signals;

    /// The system time in microseconds that the last message was sent
    jdksavdecc_timestamp_in_microseconds last_time_in_ms;
};

/// Initialize an jdksavdecc_adp_manager with the specified context and frame_send function and
/// received_entity_available_or_departing function
bool jdksavb_maap_init( struct jdksavb_maap *self, struct jdksavb_frame_sender *frame_sender, uint32_t tag, void *additional );

/// Destroy any resources that the jdksavdecc_adp_manager uses
void jdksavb_maap_terminate( struct jdksavb_maap *self );

/// Receive a MAAPDU and process it
bool jdksavb_maap_receive( struct jdksavb_maap *self,
                           jdksavdecc_timestamp_in_microseconds time_in_microseconds,
                           void const *source_address,
                           int source_address_len,
                           uint8_t const *buf,
                           uint16_t len );

/// Notify the state machine that time has passed. Call asap if early_tick is true.
void jdksavb_maap_tick( void *self, jdksavdecc_timestamp_in_microseconds cur_time_in_micros );

/*@}*/

/*@}*/

#ifdef __cplusplus
}
#endif
