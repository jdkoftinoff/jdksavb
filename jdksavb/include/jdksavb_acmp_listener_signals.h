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

#include "jdksavb_srp_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct jdksavb_acmp_listener_slots;

/** These are the signals that can come from an acmp_listener state machine */
struct jdksavb_acmp_listener_signals
{

    void ( *acmp_listener_started )( struct jdksavb_acmp_listener_signals *self, struct jdksavb_acmp_listener_slots *source );

    void ( *acmp_listener_stopped )( struct jdksavb_acmp_listener_signals *self, struct jdksavb_acmp_listener_slots *source );

    void ( *acmp_listener_srp_info_request )( struct jdksavb_acmp_listener_signals *self,
                                              struct jdksavb_acmp_listener_slots *source,
                                              struct jdksavdecc_eui64 talker_stream_id );

    void ( *acmp_listener_talker_added )( struct jdksavb_acmp_listener_signals *self,
                                          struct jdksavb_acmp_listener_slots *source,
                                          struct jdksavdecc_eui64 talker_stream_id,
                                          struct jdksavdecc_eui48 talker_mac,
                                          uint16_t talker_vid );

    void ( *acmp_listener_talker_removed )( struct jdksavb_acmp_listener_signals *self,
                                            struct jdksavb_acmp_listener_slots *source );

    void ( *acmp_listener_send_pdu )( void *self, struct jdksavb_frame *acmpdu );
};

#ifdef __cplusplus
}
#endif
