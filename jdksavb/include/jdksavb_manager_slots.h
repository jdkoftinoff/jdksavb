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

#include "jdksavb_network_port_signals.h"
#include "jdksavb_net_udp_signals.h"
#include "jdksavb_net_tcp_signals.h"
#include "jdksavb_net_raw_signals.h"
#include "jdksavb_acmp_controller_signals.h"
#include "jdksavb_acmp_listener_signals.h"
#include "jdksavb_acmp_talker_signals.h"
#include "jdksavb_adp_advertiser_signals.h"
#include "jdksavb_adp_discover_signals.h"
#include "jdksavb_aps_signals.h"
#include "jdksavb_apc_signals.h"
#include "jdksavb_controller_signals.h"
#include "jdksavb_depacketizer_signals.h"
#include "jdksavb_packetizer_signals.h"
#include "jdksavb_maap_signals.h"
#include "jdksavb_msrp_signals.h"
#include "jdksavb_mvrp_signals.h"
#include "jdksavb_gptp_signals.h"
#include "jdksavb_entity_signals.h"
#include "jdksavb_hardware_control_signals.h"
#include "jdksavb_apc_signals.h"
#include "jdksavb_aps_signals.h"
#include "jdksavb_gptp_signals.h"
#include "jdksavb_network_port_signals.h"

#ifdef __cplusplus
extern "C" {
#endif

struct jdksavdecc_manager_slots
{
    struct jdksavb_entity_signals entity;
    struct jdksavb_network_port_signals network_port;
    struct jdksavb_net_raw_signals net_raw;
    struct jdksavb_net_udp_signals net_udp;
    struct jdksavb_net_tcp_signals net_tcp;
    struct jdksavb_apc_signals apc;
    struct jdksavb_aps_signals aps;

    struct jdksavb_adp_discover_signals adp_discover;
    struct jdksavb_adp_advertiser_signals adp_advertiser;

    struct jdksavb_gptp_signals gptp;
    struct jdksavb_maap_signals maap;
    struct jdksavb_mvrp_signals mvrp;
    struct jdksavb_msrp_signals msrp;
    struct jdksavb_acmp_listener_signals acmp_listener;
    struct jdksavb_acmp_talker_signals acmp_talker;
    struct jdksavb_acmp_controller_signals acmp_controller;

};

#ifdef __cplusplus
}
#endif
