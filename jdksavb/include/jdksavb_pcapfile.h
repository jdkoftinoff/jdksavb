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
#include "jdksavb_statemachine.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup pcapfile pcap file */
/*@{*/

struct jdksavb_pcapfile_reader {
    FILE *f;
    int swapped;
    int nano;

    jdksavdecc_timestamp_in_microseconds minimum_time_to_synthesize;
    jdksavdecc_timestamp_in_microseconds time_step_in_microseconds;

    void (*destroy)(struct jdksavb_pcapfile_reader *);

    int (*open)(struct jdksavb_pcapfile_reader *self, char const *fname);
    void (*close)(struct jdksavb_pcapfile_reader *self);
    int (*read_frame)(struct jdksavb_pcapfile_reader *self, struct jdksavb_frame *frame);
    int (*dispatch_frames)(struct jdksavb_pcapfile_reader *self, struct jdksavb_state_machine *dispatcher);
    int (*tick)(struct jdksavb_pcapfile_reader *self, jdksavdecc_timestamp_in_microseconds time);
};

void jdksavb_pcapfile_reader_init(struct jdksavb_pcapfile_reader *self,
                                     jdksavdecc_timestamp_in_microseconds minimum_time_to_synthesize,
                                     jdksavdecc_timestamp_in_microseconds time_step_in_microseconds);

void jdksavb_pcapfile_reader_destroy(struct jdksavb_pcapfile_reader *self);
int jdksavb_pcapfile_reader_open(struct jdksavb_pcapfile_reader *self, char const *fname);
void jdksavb_pcapfile_reader_close(struct jdksavb_pcapfile_reader *self);
int jdksavb_pcapfile_reader_read_frame(struct jdksavb_pcapfile_reader *self, struct jdksavb_frame *frame);
int jdksavb_pcapfile_reader_dispatch_frames(struct jdksavb_pcapfile_reader *self,
                                               struct jdksavb_state_machine *dispatcher);

struct jdksavb_pcapfile_writer {
    struct jdksavb_frame_sender inherited;

    FILE *f;
    int swapped;

    void (*destroy)(struct jdksavb_pcapfile_writer *);
    int (*open)(struct jdksavb_pcapfile_writer *self, char const *fname);
    void (*close)(struct jdksavb_pcapfile_writer *self);
};

void jdksavb_pcapfile_writer_init(struct jdksavb_pcapfile_writer *self);
void jdksavb_pcapfile_writer_destroy(struct jdksavb_pcapfile_writer *self);
int jdksavb_pcapfile_writer_open(struct jdksavb_pcapfile_writer *self, char const *fname);
void jdksavb_pcapfile_writer_close(struct jdksavb_pcapfile_writer *self);

void jdksavb_pcapfile_writer_send(struct jdksavb_frame_sender *self, struct jdksavb_frame const *frame);

/*@}*/

#ifdef __cplusplus
}
#endif
