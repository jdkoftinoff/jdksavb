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
#include "jdksavb_entity_signals.h"
#include "jdksavb_entity_slots.h"

#include "jdksavb_frame.h"
#include "jdksavb_inflight.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup entity_manager  */
/*@{*/

struct jdksavb_symbol_dispatch;
struct jdksavb_entity;

struct jdksavb_symbol_dispatch {
    uint16_t configuration;
    uint32_t symbol;

    /// Notify the item that time has passed.
    void (*tick)(struct jdksavb_entity *self, uint32_t symbol, jdksavdecc_timestamp_in_milliseconds cur_time_in_ms);

    /// The pdu contains a valid Read Descriptor Command
    /// Fill in the response in place in the pdu and return an AECP AEM status code
    uint8_t (*receive_read_descriptor_command)(struct jdksavb_entity *entity_manager,
                                               uint32_t symbol,
                                               struct jdksavdecc_aecpdu_aem const *aem,
                                               jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                               uint8_t *buf,
                                               uint16_t len);

    /// The pdu contains a valid Command to set or change something
    /// SET_NAME, SET_CONTROL, INCREMENT_CONTROL, DECREMENT_CONTROL, SET_MIXER,
    /// SET_MATRIX, SET_SIGNAL_SELECTOR, etc
    /// Fill in the response in place in the pdu and return an AECP AEM status code
    uint8_t (*receive_set_command)(struct jdksavb_entity *entity_manager,
                                   uint32_t symbol,
                                   struct jdksavdecc_aecpdu_aem const *aem,
                                   jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                   uint8_t *buf,
                                   uint16_t len);

    /// The pdu contains a valid Command to get something
    /// GET_NAME, GET_CONTROL, GET_MIXER, GET_MATRIX, GET_SIGNAL_SELECTOR, etc
    /// Fill in the response in place in the pdu and return an AECP AEM status code
    uint8_t (*receive_get_command)(struct jdksavb_entity *entity_manager,
                                   uint32_t symbol,
                                   struct jdksavdecc_aecpdu_aem const *aem,
                                   jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                   uint8_t *buf,
                                   uint16_t len);
};

#ifndef JDKSAVB_ENTITY_MAX_CONFIGURATIONS
#define JDKSAVB_ENTITY_MAX_CONFIGURATIONS (1)
#endif

#ifndef JDKSAVB_ENTITY_MAX_SYMBOLS_PER_CONFIGURATION
#define JDKSAVB_ENTITY_MAX_SYMBOLS_PER_CONFIGURATION (32)
#endif

#ifndef JDKSAVB_ENTITY_MAX_REGISTERED_CONTROLLERS
#define JDKSAVB_ENTITY_MAX_REGISTERED_CONTROLLERS (8)
#endif

/// jdksavb_entity is a base class for an AEM entity
struct jdksavb_entity {

    /// Inherit from slots
    struct jdksavb_entity_slots slots;

    /// The signals
    struct jdksavb_entity_signals signals;

    /// The entity model
    struct jdksavdecc_entity_model *entity_model;

    /// The symbol dispatch table size
    int symbol_dispatch_table_num_entries;

    /// The symbol dispatch table
    struct jdksavb_symbol_dispatch *symbol_dispatch_table;

    /// The talker

    /// The current ADP manager that advertises for us
    struct jdksavb_adp_advertiser *adp_manager;

    /// The context that the entity is used in.
    void *context;

    /// The seqeunce_id of the last send outgoing command
    uint16_t outgoing_sequence_id;

    /// If we are acquired by a controller, then this is set to the controller's entity id.
    /// If we are not acquired, then this will be FF:FF:FF:FF:FF:FF:FF:FF
    struct jdksavdecc_eui64 acquired_by_controller_entity_id;

    /// If we are acquired by a controller, then this is the MAC address of the controller
    struct jdksavdecc_eui48 acquired_by_controller_mac_address;

    /// If we are acquired by one controller and another controller is trying to acquire
    /// this entity, then this contains the new controller's entity_id during the
    /// controller available negotiation mechanism
    struct jdksavdecc_eui64 acquire_in_progress_by_controller_entity_id;

    /// If we are currently interrogating a controller with a controller available, then
    /// this is the timestamp of when we started the controller available negotiation mechanism
    jdksavdecc_timestamp_in_milliseconds acquire_in_progress_time;

    /// If we are locked by a controller, then this contains the controller's entity id which
    /// locked us
    struct jdksavdecc_eui64 locked_by_controller_entity_id;

    /// If we are locked by a controller, then this contains the timestamp of when the lock first
    /// ocurred
    jdksavdecc_timestamp_in_milliseconds locked_time;

    /// This contains the unordered list of controller entity id's that are currently registered
    /// via the register for unsolicited notifications command. The entity id is FF:FF:FF:FF:FF:FF:FF:FF
    /// If the the slot is not in use
    struct jdksavdecc_eui64 registered_controllers_entity_id[JDKSAVB_ENTITY_MAX_REGISTERED_CONTROLLERS];

    /// This contains the associated MAC address of the controller's that are currently registered
    struct jdksavdecc_eui48 registered_controllers_mac_address[JDKSAVB_ENTITY_MAX_REGISTERED_CONTROLLERS];

    /// The map of inflight commands
    struct jdksavb_inflight_commands inflight_commands;

    /// Notification that a command to a target entity timed out
    void (*command_timed_out)(struct jdksavb_entity *self,
                              struct jdksavdecc_eui64 const *target_entity_id,
                              uint16_t command_type,
                              uint16_t sequence_id);

    /// Check to make sure the command is allowed or disallowed due to acquire or locking
    uint8_t (*validate_permissions)(struct jdksavb_entity *self, struct jdksavdecc_aecpdu_aem const *aem);

    /// The received pdu contains a valid ACMP message
    uint8_t (*received_acmpdu)(struct jdksavb_entity *self,
                               struct jdksavdecc_acmpdu const *acmp,
                               jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                               uint8_t *buf,
                               uint16_t len);

    /// The received pdu contains a valid AEM command for me.
    /// Fill in the response in place in the pdu and return an AECP AEM status code
    uint8_t (*received_aem_command)(struct jdksavb_entity *self,
                                    struct jdksavdecc_aecpdu_aem const *aem,
                                    jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                    uint8_t *buf,
                                    uint16_t len);

    /// The received pdu contains a valid AA command for me.
    /// Fill in the response in place in the pdu and return an AECP AA status code
    uint8_t (*received_aa_command)(struct jdksavb_entity *self,
                                   struct jdksavdecc_aecp_aa const *aa,
                                   jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                   uint8_t *buf,
                                   uint16_t len);

    /// Can we send a command now? i.e. are there no in-flight commands waiting to be acknowledged?
    bool (*can_send_command)(struct jdksavb_entity *self);

    /// Form and send a command now
    void (*send_command)(struct jdksavb_entity *self,
                         struct jdksavdecc_eui64 const *target_entity_id,
                         struct jdksavdecc_eui48 const *target_mac_address,
                         uint16_t aem_command_type,
                         bool track_for_ack,
                         uint8_t const *additional_data1,
                         uint16_t additional_data_length1,
                         uint8_t const *additional_data2,
                         uint16_t additional_data_length2);

    /// Send a direct response to the target entity id, and unsolicited
    /// responses to all other subscribed controllers
    void (*send_responses)(struct jdksavb_entity *self,
                           bool internally_generated,
                           bool send_to_registered_controllers,
                           uint8_t *buf,
                           uint16_t len,
                           uint8_t const *additional_data1,
                           uint16_t additional_data_length1,
                           uint8_t const *additional_data2,
                           uint16_t additional_data_length2);

    /// Formulate an AEM Unsolicited response of specified command_type with the
    /// specified additional data as payload, and send it to all interested controllers
    void (*send_unsolicited_responses)(struct jdksavb_entity *self,
                                       uint16_t aem_command_type,
                                       uint8_t const *additional_data1,
                                       uint16_t additional_data_length1,
                                       uint8_t const *additional_data2,
                                       uint16_t additional_data_length2);

    /// The pdu contains a valid Acquire Entity command.
    /// Fill in the response in place in the pdu and return an AECP AEM status code
    uint8_t (*receive_acquire_entity_command)(struct jdksavb_entity *self,
                                              struct jdksavdecc_aecpdu_aem const *aem,
                                              jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                              uint8_t *buf,
                                              uint16_t len);

    /// The pdu contains a valid Lock Entity command.
    /// Fill in the response in place in the pdu and return an AECP AEM status code
    uint8_t (*receive_lock_entity_command)(struct jdksavb_entity *self,
                                           struct jdksavdecc_aecpdu_aem const *aem,
                                           jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                           uint8_t *buf,
                                           uint16_t len);

    /// The pdu contains a valid Entity Available command.
    /// Fill in the response in place in the pdu and return an AECP AEM status code
    uint8_t (*receive_entity_available_command)(struct jdksavb_entity *self,
                                                struct jdksavdecc_aecpdu_aem const *aem,
                                                jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                                uint8_t *buf,
                                                uint16_t len);

    /// Formulate and send a CONTROLLER_AVAILABLE command to a target controller
    void (*send_controller_available)(struct jdksavb_entity *self,
                                      struct jdksavdecc_eui64 const *target_controller_entity_id,
                                      struct jdksavdecc_eui48 const *target_mac_address);

    /// The pdu contains a valid Controller Available command.
    /// Fill in the response in place in the pdu and return an AECP AEM status code
    uint8_t (*receive_controller_available_command)(struct jdksavb_entity *self,
                                                    struct jdksavdecc_aecpdu_aem const *aem,
                                                    jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                                    uint8_t *buf,
                                                    uint16_t len);

    /// The pdu contains a valid Controller Available response.
    /// return true if the response is handled
    bool (*receive_controller_available_response)(struct jdksavb_entity *self,
                                                  struct jdksavdecc_aecpdu_aem const *aem,
                                                  jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                                  uint8_t *buf,
                                                  uint16_t len);

    /// The pdu contains a valid Read Descriptor Command
    /// Fill in the response in place in the pdu and return an AECP AEM status code
    uint8_t (*receive_read_descriptor_command)(struct jdksavb_entity *self,
                                               struct jdksavdecc_aecpdu_aem const *aem,
                                               jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                               uint8_t *buf,
                                               uint16_t len);

    /// The pdu contains a valid Set Configuration Command
    /// Fill in the response in place in the pdu and return an AECP AEM status code
    uint8_t (*receive_set_configuration_command)(struct jdksavb_entity *self,
                                                 struct jdksavdecc_aecpdu_aem const *aem,
                                                 jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                                 uint8_t *buf,
                                                 uint16_t len);

    /// The pdu contains a valid Get Configuration Command
    /// Fill in the response in place in the pdu and return an AECP AEM status code
    uint8_t (*receive_get_configuration_command)(struct jdksavb_entity *self,
                                                 struct jdksavdecc_aecpdu_aem const *aem,
                                                 jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                                 uint8_t *buf,
                                                 uint16_t len);

    /// The pdu contains a valid Set Name Command
    /// Fill in the response in place in the pdu and return an AECP AEM status code
    uint8_t (*receive_set_name_command)(struct jdksavb_entity *self,
                                        struct jdksavdecc_aecpdu_aem const *aem,
                                        jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                        uint8_t *buf,
                                        uint16_t len);

    /// The pdu contains a valid Get Name Command
    /// Fill in the response in place in the pdu and return an AECP AEM status code
    uint8_t (*receive_get_name_command)(struct jdksavb_entity *self,
                                        struct jdksavdecc_aecpdu_aem const *aem,
                                        jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                        uint8_t *buf,
                                        uint16_t len);

    /// Formulate and send a SET_CONTROL unsolicited response to all subscribed controllers
    void (*send_set_control_unsolicited_response)(struct jdksavb_entity *self,
                                                  uint16_t target_descriptor_index,
                                                  uint8_t *control_value,
                                                  uint16_t control_value_len);

    /// The pdu contains a valid Set Control Command
    /// Fill in the response in place in the pdu and return an AECP AEM status code
    uint8_t (*receive_set_control_command)(struct jdksavb_entity *self,
                                           struct jdksavdecc_aecpdu_aem const *aem,
                                           jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                           uint8_t *buf,
                                           uint16_t len);

    /// The pdu contains a valid Get Control Command
    /// Fill in the response in place in the pdu and return an AECP AEM status code
    uint8_t (*receive_get_control_command)(struct jdksavb_entity *self,
                                           struct jdksavdecc_aecpdu_aem const *aem,
                                           jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                           uint8_t *buf,
                                           uint16_t len);

    /// The pdu contains a valid Increment Control Command
    /// Fill in the response in place in the pdu and return an AECP AEM status code
    uint8_t (*receive_increment_control_command)(struct jdksavb_entity *self,
                                                 struct jdksavdecc_aecpdu_aem const *aem,
                                                 jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                                 uint8_t *buf,
                                                 uint16_t len);

    /// The pdu contains a valid Decrement Control Command
    /// Fill in the response in place in the pdu and return an AECP AEM status code
    uint8_t (*receive_decrement_control_command)(struct jdksavb_entity *self,
                                                 struct jdksavdecc_aecpdu_aem const *aem,
                                                 jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                                 uint8_t *buf,
                                                 uint16_t len);

    /// The pdu contains a valid Set Mixer Command
    /// Fill in the response in place in the pdu and return an AECP AEM status code
    uint8_t (*receive_set_mixer_command)(struct jdksavb_entity *self,
                                         struct jdksavdecc_aecpdu_aem const *aem,
                                         jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                         uint8_t *buf,
                                         uint16_t len);

    /// The pdu contains a valid Get Mixer Command
    /// Fill in the response in place in the pdu and return an AECP AEM status code
    uint8_t (*receive_get_mixer_command)(struct jdksavb_entity *self,
                                         struct jdksavdecc_aecpdu_aem const *aem,
                                         jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                         uint8_t *buf,
                                         uint16_t len);

    /// The pdu contains a valid Set Signal Selector Command
    /// Fill in the response in place in the pdu and return an AECP AEM status code
    uint8_t (*receive_set_signal_selector_command)(struct jdksavb_entity *self,
                                                   struct jdksavdecc_aecpdu_aem const *aem,
                                                   jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                                   uint8_t *buf,
                                                   uint16_t len);

    /// The pdu contains a valid Get Signal Selector Command
    /// Fill in the response in place in the pdu and return an AECP AEM status code
    uint8_t (*receive_get_signal_selector_command)(struct jdksavb_entity *self,
                                                   struct jdksavdecc_aecpdu_aem const *aem,
                                                   jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                                   uint8_t *buf,
                                                   uint16_t len);

    /// The pdu contains a valid Register for Unsolicited Notifications Command
    /// Fill in the response in place in the pdu and return an AECP AEM status code
    uint8_t (*receive_register_unsolicited_notification_command)(struct jdksavb_entity *self,
                                                                 struct jdksavdecc_aecpdu_aem const *aem,
                                                                 jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                                                 uint8_t *buf,
                                                                 uint16_t len);

    /// The pdu contains a valid De-Register for Unsolicited Notifications Command
    /// Fill in the response in place in the pdu and return an AECP AEM status code
    uint8_t (*receive_deregister_unsolicited_notification_command)(struct jdksavb_entity *self,
                                                                   struct jdksavdecc_aecpdu_aem const *aem,
                                                                   jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                                                   uint8_t *buf,
                                                                   uint16_t len);

    /// The pdu contains a valid Read Entity Descriptor command
    /// Fill in the response in place in the pdu and return an AECP AEM status code
    uint8_t (*read_descriptor_entity)(struct jdksavb_entity *self,
                                      struct jdksavdecc_aecpdu_aem const *aem,
                                      jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                      uint8_t *buf,
                                      uint16_t len);

    /// The pdu contains a valid Read Configuration Descriptor command
    /// Fill in the response in place in the pdu and return an AECP AEM status code
    uint8_t (*read_descriptor_configuration)(struct jdksavb_entity *self,
                                             struct jdksavdecc_aecpdu_aem const *aem,
                                             jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                             uint8_t *buf,
                                             uint16_t len);

    /// The pdu contains a valid Read Control Descriptor command
    /// Fill in the response in place in the pdu and return an AECP AEM status code
    uint8_t (*read_descriptor_control)(struct jdksavb_entity *self,
                                       struct jdksavdecc_aecpdu_aem const *aem,
                                       jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                       uint8_t *buf,
                                       uint16_t len);

    /// The pdu contains a valid Read Locale Descriptor command
    /// Fill in the response in place in the pdu and return an AECP AEM status code
    uint8_t (*read_descriptor_locale)(struct jdksavb_entity *self,
                                      struct jdksavdecc_aecpdu_aem const *aem,
                                      jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                      uint8_t *buf,
                                      uint16_t len);

    /// The pdu contains a valid Read Strings Descriptor command
    /// Fill in the response in place in the pdu and return an AECP AEM status code
    uint8_t (*read_descriptor_strings)(struct jdksavb_entity *self,
                                       struct jdksavdecc_aecpdu_aem const *aem,
                                       jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                       uint8_t *buf,
                                       uint16_t len);

    /// The pdu contains a valid Read Memory Descriptor command
    /// Fill in the response in place in the pdu and return an AECP AEM status code
    uint8_t (*read_descriptor_memory)(struct jdksavb_entity *self,
                                      struct jdksavdecc_aecpdu_aem const *aem,
                                      jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                      uint8_t *buf,
                                      uint16_t len);

    /// The pdu contains a valid Read Address Access TLV command
    /// Fill in the response in place in the pdu and return an AECP AA status code
    uint8_t (*receive_aa_read)(struct jdksavb_entity *self,
                               struct jdksavdecc_aecp_aa const *aa,
                               uint32_t virtual_base_address,
                               uint16_t length,
                               uint8_t *response);

    /// The pdu contains a valid Write Address Access TLV command
    /// Fill in the response in place in the pdu and return an AECP AA status code
    uint8_t (*receive_aa_write)(struct jdksavb_entity *self,
                                struct jdksavdecc_aecp_aa const *aa,
                                uint32_t virtual_base_address,
                                uint16_t length,
                                uint8_t const *request);

    /// The pdu contains a valid Execute Address Access TLV command
    /// Fill in the response in place in the pdu and return an AECP AA status code
    uint8_t (*receive_aa_execute)(struct jdksavb_entity *self,
                                  struct jdksavdecc_aecp_aa const *aa,
                                  uint32_t virtual_base_address,
                                  uint16_t length,
                                  uint8_t const *request);
};

/// Initialize an entity manager
bool jdksavb_entity_init(struct jdksavb_entity *self,
                         struct jdksavdecc_entity_model *entity_model,
                         int symbol_dispatch_table_num_entries,
                         struct jdksavb_symbol_dispatch *symbol_dispatch_table);

/// Destroy any resources that the jdksavdecc_adp_manager uses
void jdksavb_entity_destroy(struct jdksavb_entity *self);

struct jdksavb_symbol_dispatch *
    jdksavb_entity_find_symbol(struct jdksavb_entity *self, uint16_t configuration_number, uint32_t symbol);

/// The pdu contains a valid Read Descriptor Command
/// Fill in the response in place in the pdu and return an AECP AEM status code
uint8_t jdksavb_entity_dispatch_symbol_receive_read_descriptor_command(struct jdksavb_entity *self,
                                                                       uint32_t symbol,
                                                                       struct jdksavdecc_aecpdu_aem const *aem,
                                                                       jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                                                       uint8_t *buf,
                                                                       uint16_t len);

/// The pdu contains a valid Command to set or change something
/// SET_NAME, SET_CONTROL, INCREMENT_CONTROL, DECREMENT_CONTROL, SET_MIXER,
/// SET_MATRIX, SET_SIGNAL_SELECTOR, etc
/// Fill in the response in place in the pdu and return an AECP AEM status code
uint8_t jdksavb_entity_dispatch_symbol_set_command(struct jdksavb_entity *self,
                                                   uint32_t symbol,
                                                   struct jdksavdecc_aecpdu_aem const *aem,
                                                   jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                                   uint8_t *buf,
                                                   uint16_t len);

/// The pdu contains a valid Command to get something
/// GET_NAME, GET_CONTROL, GET_MIXER, GET_MATRIX, GET_SIGNAL_SELECTOR, etc
/// Fill in the response in place in the pdu and return an AECP AEM status code
uint8_t jdksavb_entity_dispatch_symbol_receive_get_command(struct jdksavb_entity *self,
                                                           uint32_t symbol,
                                                           struct jdksavdecc_aecpdu_aem const *aem,
                                                           jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                                           uint8_t *buf,
                                                           uint16_t len);

/// Receive an ADPU and process it
bool jdksavb_entity_receive(struct jdksavb_entity *self,
                            void *context,
                            jdksavdecc_timestamp_in_milliseconds time_in_milliseconds,
                            uint8_t *buf,
                            uint16_t len);

/// Notify the state machine that time has passed. Call asap if early_tick is true.
void jdksavb_entity_tick(struct jdksavb_entity *self, jdksavdecc_timestamp_in_milliseconds cur_time_in_ms);

/// Notification that a command to a target entity timed out
void jdksavb_entity_command_timed_out(struct jdksavb_entity *self,
                                      struct jdksavdecc_eui64 const *target_entity_id,
                                      uint16_t command_type,
                                      uint16_t sequence_id);

/// Check to make sure the command is allowed or disallowed due to acquire or locking
uint8_t jdksavb_entity_validate_permissions(struct jdksavb_entity *self, struct jdksavdecc_aecpdu_aem const *aem);

/// The received pdu contains a valid AEM command for me.
/// Fill in the response in place in the pdu and return an AECP AEM status code
uint8_t jdksavb_entity_received_aem_command(struct jdksavb_entity *self,
                                            struct jdksavdecc_aecpdu_aem const *aem,
                                            jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                            uint8_t *buf,
                                            uint16_t len);

/// The received pdu contains a valid AA command for me.
/// Fill in the response in place in the pdu and return an AECP AA status code
uint8_t jdksavb_entity_received_aa_command(struct jdksavb_entity *self,
                                           struct jdksavdecc_aecp_aa const *aa,
                                           jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                           uint8_t *buf,
                                           uint16_t len);

/// Can we send a command now? i.e. are there no in-flight commands waiting to be acknowledged?
bool jdksavb_entity_can_send_command(struct jdksavb_entity *self);

void jdksavb_entity_send_command(struct jdksavb_entity *self,
                                 struct jdksavdecc_eui64 const *target_entity_id,
                                 struct jdksavdecc_eui48 const *target_mac_address,
                                 uint16_t aem_command_type,
                                 bool track_for_ack,
                                 uint8_t const *additional_data1,
                                 uint16_t additional_data_length1,
                                 uint8_t const *additional_data2,
                                 uint16_t additional_data_length2);

/// Send a direct response to the target entity id, and unsolicited
/// responses to all other subscribed controllers
void jdksavb_entity_send_responses(struct jdksavb_entity *self,
                                   bool internally_generated,
                                   bool send_to_registered_controllers,
                                   uint8_t *buf,
                                   uint16_t len,
                                   uint8_t const *additional_data1,
                                   uint16_t additional_data_length1,
                                   uint8_t const *additional_data2,
                                   uint16_t additional_data_length2);

/// Formulate an AEM Unsolicited response of specified command_type with the
/// specified additional data as payload, and send it to all interested controllers
void jdksavb_entity_send_unsolicited_responses(struct jdksavb_entity *self,
                                               uint16_t aem_command_type,
                                               uint8_t const *additional_data1,
                                               uint16_t additional_data_length1,
                                               uint8_t const *additional_data2,
                                               uint16_t additional_data_length2);

/// The pdu contains a valid Acquire Entity command.
/// Fill in the response in place in the pdu and return an AECP AEM status code
uint8_t jdksavb_entity_receive_acquire_entity_command(struct jdksavb_entity *self,
                                                      struct jdksavdecc_aecpdu_aem const *aem,
                                                      jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                                      uint8_t *buf,
                                                      uint16_t len);

/// The pdu contains a valid Lock Entity command.
/// Fill in the response in place in the pdu and return an AECP AEM status code
uint8_t jdksavb_entity_receive_lock_entity_command(struct jdksavb_entity *self,
                                                   struct jdksavdecc_aecpdu_aem const *aem,
                                                   jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                                   uint8_t *buf,
                                                   uint16_t len);

/// The pdu contains a valid Entity Available command.
/// Fill in the response in place in the pdu and return an AECP AEM status code
uint8_t jdksavb_entity_receive_entity_available_command(struct jdksavb_entity *self,
                                                        struct jdksavdecc_aecpdu_aem const *aem,
                                                        jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                                        uint8_t *buf,
                                                        uint16_t len);

// Formulate and send a CONTROLLER_AVAILABLE command to a target controller
void jdksavb_entity_send_controller_available(struct jdksavb_entity *self,
                                              struct jdksavdecc_eui64 const *target_controller_entity_id,
                                              struct jdksavdecc_eui48 const *target_mac_address);

/// The pdu contains a valid Controller Available command.
/// Fill in the response in place in the pdu and return an AECP AEM status code
uint8_t jdksavb_entity_receive_controller_available_command(struct jdksavb_entity *self,
                                                            struct jdksavdecc_aecpdu_aem const *aem,
                                                            jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                                            uint8_t *buf,
                                                            uint16_t len);

/// The pdu contains a valid Controller Available response.
/// return true if the response is handled
bool jdksavb_entity_receive_controller_available_response(struct jdksavb_entity *self,
                                                          struct jdksavdecc_aecpdu_aem const *aem,
                                                          jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                                          uint8_t *buf,
                                                          uint16_t len);

/// The pdu contains a valid Read Descriptor Command
/// Fill in the response in place in the pdu and return an AECP AEM status code
uint8_t jdksavb_entity_receive_read_descriptor_command(struct jdksavb_entity *self,
                                                       struct jdksavdecc_aecpdu_aem const *aem,
                                                       jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                                       uint8_t *buf,
                                                       uint16_t len);

/// The pdu contains a valid Set Configuration Command
/// Fill in the response in place in the pdu and return an AECP AEM status code
uint8_t jdksavb_entity_receive_set_configuration_command(struct jdksavb_entity *self,
                                                         struct jdksavdecc_aecpdu_aem const *aem,
                                                         jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                                         uint8_t *buf,
                                                         uint16_t len);

/// The pdu contains a valid Get Configuration Command
/// Fill in the response in place in the pdu and return an AECP AEM status code
uint8_t jdksavb_entity_receive_get_configuration_command(struct jdksavb_entity *self,
                                                         struct jdksavdecc_aecpdu_aem const *aem,
                                                         jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                                         uint8_t *buf,
                                                         uint16_t len);

/// The pdu contains a valid Set Name Command
/// Fill in the response in place in the pdu and return an AECP AEM status code
uint8_t jdksavb_entity_receive_set_name_command(struct jdksavb_entity *self,
                                                struct jdksavdecc_aecpdu_aem const *aem,
                                                jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                                uint8_t *buf,
                                                uint16_t len);

/// The pdu contains a valid Get Name Command
/// Fill in the response in place in the pdu and return an AECP AEM status code
uint8_t jdksavb_entity_receive_get_name_command(struct jdksavb_entity *self,
                                                struct jdksavdecc_aecpdu_aem const *aem,
                                                jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                                uint8_t *buf,
                                                uint16_t len);

/// Formulate and send a SET_CONTROL unsolicited response to all subscribed controllers
void jdksavb_entity_send_set_control_unsolicited_response(struct jdksavb_entity *self,
                                                          uint16_t target_descriptor_index,
                                                          uint8_t *control_value,
                                                          uint16_t control_value_len);

/// The pdu contains a valid Set Control Command
/// Fill in the response in place in the pdu and return an AECP AEM status code
uint8_t jdksavb_entity_receive_set_control_command(struct jdksavb_entity *self,
                                                   struct jdksavdecc_aecpdu_aem const *aem,
                                                   jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                                   uint8_t *buf,
                                                   uint16_t len);

/// The pdu contains a valid Get Control Command
/// Fill in the response in place in the pdu and return an AECP AEM status code
uint8_t jdksavb_entity_receive_get_control_command(struct jdksavb_entity *self,
                                                   struct jdksavdecc_aecpdu_aem const *aem,
                                                   jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                                   uint8_t *buf,
                                                   uint16_t len);

/// The pdu contains a valid Register for Unsolicited Notifications Command
/// Fill in the response in place in the pdu and return an AECP AEM status code
uint8_t jdksavb_entity_receive_register_unsolicited_notification_command(struct jdksavb_entity *self,
                                                                         struct jdksavdecc_aecpdu_aem const *aem,
                                                                         jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                                                         uint8_t *buf,
                                                                         uint16_t len);

/// The pdu contains a valid De-Register for Unsolicited Notifications Command
/// Fill in the response in place in the pdu and return an AECP AEM status code
uint8_t jdksavb_entity_receive_deregister_unsolicited_notification_command(struct jdksavb_entity *self,
                                                                           struct jdksavdecc_aecpdu_aem const *aem,
                                                                           jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                                                           uint8_t *buf,
                                                                           uint16_t len);

/// The pdu contains a valid Read Entity Descriptor command
/// Fill in the response in place in the pdu and return an AECP AEM status code
uint8_t jdksavb_entity_read_descriptor_entity(struct jdksavb_entity *self,
                                              struct jdksavdecc_aecpdu_aem const *aem,
                                              jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                              uint8_t *buf,
                                              uint16_t len);

/// The pdu contains a valid Read Configuration Descriptor command
/// Fill in the response in place in the pdu and return an AECP AEM status code
uint8_t jdksavb_entity_read_descriptor_configuration(struct jdksavb_entity *self,
                                                     struct jdksavdecc_aecpdu_aem const *aem,
                                                     jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                                     uint8_t *buf,
                                                     uint16_t len);

/// The pdu contains a valid Read Control Descriptor command
/// Fill in the response in place in the pdu and return an AECP AEM status code
uint8_t jdksavb_entity_read_descriptor_control(struct jdksavb_entity *self,
                                               struct jdksavdecc_aecpdu_aem const *aem,
                                               jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                               uint8_t *buf,
                                               uint16_t len);

/// The pdu contains a valid Read Locale Descriptor command
/// Fill in the response in place in the pdu and return an AECP AEM status code
uint8_t jdksavb_entity_read_descriptor_locale(struct jdksavb_entity *self,
                                              struct jdksavdecc_aecpdu_aem const *aem,
                                              jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                              uint8_t *buf,
                                              uint16_t len);

/// The pdu contains a valid Read Strings Descriptor command
/// Fill in the response in place in the pdu and return an AECP AEM status code
uint8_t jdksavb_entity_read_descriptor_strings(struct jdksavb_entity *self,
                                               struct jdksavdecc_aecpdu_aem const *aem,
                                               jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                               uint8_t *buf,
                                               uint16_t len);

/// The pdu contains a valid Read Memory Descriptor command
/// Fill in the response in place in the pdu and return an AECP AEM status code
uint8_t jdksavb_entity_read_descriptor_memory(struct jdksavb_entity *self,
                                              struct jdksavdecc_aecpdu_aem const *aem,
                                              jdksavdecc_timestamp_in_milliseconds cur_time_in_ms,
                                              uint8_t *buf,
                                              uint16_t len);

/// The pdu contains a valid Read Address Access TLV command
/// Fill in the response in place in the pdu and return an AECP AA status code
uint8_t jdksavb_entity_receive_aa_read(struct jdksavb_entity *self,
                                       struct jdksavdecc_aecp_aa const *aa,
                                       uint32_t virtual_base_address,
                                       uint16_t length,
                                       uint8_t *response);

/// The pdu contains a valid Write Address Access TLV command
/// Fill in the response in place in the pdu and return an AECP AA status code
uint8_t jdksavb_entity_receive_aa_write(struct jdksavb_entity *self,
                                        struct jdksavdecc_aecp_aa const *aa,
                                        uint32_t virtual_base_address,
                                        uint16_t length,
                                        uint8_t const *request);

/// The pdu contains a valid Execute Address Access TLV command
/// Fill in the response in place in the pdu and return an AECP AA status code
uint8_t jdksavb_entity_receive_aa_execute(struct jdksavb_entity *self,
                                          struct jdksavdecc_aecp_aa const *aa,
                                          uint32_t virtual_base_address,
                                          uint16_t length,
                                          uint8_t const *request);

/*@}*/

#ifdef __cplusplus
}
#endif
