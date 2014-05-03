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

/** \addtogroup util Utilities */
/*@{*/

/** \addtogroup frame raw ethernet frame */
/*@{*/

#define JDKSAVB_FRAME_HEADER_DA_OFFSET (0)
#define JDKSAVB_FRAME_HEADER_SA_OFFSET (6)
#define JDKSAVB_FRAME_HEADER_ETHERTYPE_OFFSET (12)
#define JDKSAVB_FRAME_HEADER_LEN (14)

#ifndef JDKSAVB_FRAME_MAX_PAYLOAD_SIZE
#define JDKSAVB_FRAME_MAX_PAYLOAD_SIZE (1500)
#endif

struct jdksavb_frame {
    jdksavdecc_timestamp_in_microseconds time;
    struct jdksavdecc_eui48 dest_address;
    struct jdksavdecc_eui48 src_address;
    uint16_t ethertype;
    uint16_t tpid;
    uint16_t pcp : 3;
    uint16_t dei : 1;
    uint16_t vid : 12;
    uint16_t length;
    uint8_t payload[JDKSAVB_FRAME_MAX_PAYLOAD_SIZE];
};

static inline void jdksavb_frame_init(struct jdksavb_frame *p) {
    p->time = 0;
    jdksavdecc_eui48_init(&p->dest_address);
    jdksavdecc_eui48_init(&p->src_address);
    p->ethertype = 0;
    p->length = 0;
    p->tpid = 0;
    p->pcp = 0;
    p->dei = 0;
    p->vid = 0;
    memset(p->payload, 0, sizeof(p->payload));
}

ssize_t jdksavb_frame_read(struct jdksavb_frame *p, void const *base, ssize_t pos, size_t len);
ssize_t jdksavb_frame_write(struct jdksavb_frame const *p, void *base, ssize_t pos, size_t len);
void jdksavb_frame_print(struct jdksavdecc_printer *self, struct jdksavb_frame const *p, int dump_payload);

/*@}*/

/** frame sender class */
struct jdksavb_frame_sender {
    void (*destroy)(struct jdksavb_frame_sender *);
    void (*send)(struct jdksavb_frame_sender *, struct jdksavb_frame const *frame);
};


/*@}*/

#ifdef __cplusplus
}
#endif
