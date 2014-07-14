#include "jdksavb_network_port_slots.h"
#include "jdksavb_net_udp_slots.h"
#include "jdksavb_net_tcp_slots.h"
#include "jdksavb_net_raw_slots.h"
#include "jdksavb_acmp_controller_slots.h"
#include "jdksavb_acmp_listener_slots.h"
#include "jdksavb_acmp_talker_slots.h"
#include "jdksavb_adp_advertiser_slots.h"
#include "jdksavb_adp_discover_slots.h"
#include "jdksavb_aps_slots.h"
#include "jdksavb_apc_slots.h"
#include "jdksavb_controller_slots.h"
#include "jdksavb_depacketizer_slots.h"
#include "jdksavb_packetizer_slots.h"
#include "jdksavb_maap_slots.h"
#include "jdksavb_msrp_slots.h"
#include "jdksavb_mvrp_slots.h"
#include "jdksavb_gptp_slots.h"
#include "jdksavb_entity_slots.h"
#include "jdksavb_hardware_control_slots.h"
#include "jdksavb_apc_slots.h"
#include "jdksavb_aps_slots.h"
#include "jdksavb_gptp_slots.h"
#include "jdksavb_network_port_slots.h"

#ifdef __cplusplus
extern "C" {
#endif

struct jdksavdecc_manager_signals
{
    struct jdksavb_entity_slots entity;
    struct jdksavb_network_port_slots network_port;
    struct jdksavb_net_raw_slots net_raw;
    struct jdksavb_net_udp_slots net_udp;
    struct jdksavb_net_tcp_slots net_tcp;
    struct jdksavb_apc_slots apc;
    struct jdksavb_aps_slots aps;

    struct jdksavb_adp_discover_slots adp_discover;
    struct jdksavb_adp_advertiser_slots adp_advertiser;

    struct jdksavb_gptp_slots gptp;
    struct jdksavb_maap_slots maap;
    struct jdksavb_mvrp_slots mvrp;
    struct jdksavb_msrp_slots msrp;
    struct jdksavb_acmp_listener_slots acmp_listener;
    struct jdksavb_acmp_talker_slots acmp_talker;
    struct jdksavb_acmp_controller_slots acmp_controller;

};

#ifdef __cplusplus
}
#endif
