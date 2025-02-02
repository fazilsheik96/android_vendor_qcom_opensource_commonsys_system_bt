/*
 * Copyright (C) 2017 The Linux Foundation. All rights reserved
 * Not a Contribution.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the
 * disclaimer below) provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *
 * * Neither the name of The Linux Foundation nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
 * GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
 * HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/******************************************************************************
 *
 *  Copyright (C) 1999-2012 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/
#ifndef SDP_API_H
#define SDP_API_H

#include "bt_target.h"
#include "sdpdefs.h"

/*****************************************************************************
 *  Constants
 ****************************************************************************/

/* Success code and error codes */
#define SDP_SUCCESS 0x0000
#define SDP_INVALID_VERSION 0x0001
#define SDP_INVALID_SERV_REC_HDL 0x0002
#define SDP_INVALID_REQ_SYNTAX 0x0003
#define SDP_INVALID_PDU_SIZE 0x0004
#define SDP_INVALID_CONT_STATE 0x0005
#define SDP_NO_RESOURCES 0x0006
#define SDP_DI_REG_FAILED 0x0007
#define SDP_DI_DISC_FAILED 0x0008
#define SDP_NO_DI_RECORD_FOUND 0x0009
#define SDP_ERR_ATTR_NOT_PRESENT 0x000A
#define SDP_ILLEGAL_PARAMETER 0x000B

#define SDP_NO_RECS_MATCH 0xFFF0
#define SDP_CONN_FAILED 0xFFF1
#define SDP_CFG_FAILED 0xFFF2
#define SDP_GENERIC_ERROR 0xFFF3
#define SDP_DB_FULL 0xFFF4
#define SDP_INVALID_PDU 0xFFF5
#define SDP_SECURITY_ERR 0xFFF6
#define SDP_CONN_REJECTED 0xFFF7
#define SDP_CANCEL 0xFFF8
#define SDP_CONN_BUSY 0xFFF9

/* Define the PSM that SDP uses */
#define SDP_PSM 0x0001

/* Masks for attr_value field of tSDP_DISC_ATTR */
#define SDP_DISC_ATTR_LEN_MASK 0x0FFF
#define SDP_DISC_ATTR_TYPE(len_type) ((len_type) >> 12)
#define SDP_DISC_ATTR_LEN(len_type) ((len_type)&SDP_DISC_ATTR_LEN_MASK)
#define SDP_DISC_ATTR_MAX_LEN 16

/* Maximum number of protocol list items (list_elem in tSDP_PROTOCOL_ELEM) */
#define SDP_MAX_LIST_ELEMS 3

/*****************************************************************************
 *  Type Definitions
 ****************************************************************************/

/* Define a callback function for when discovery is complete. */
typedef void(tSDP_DISC_CMPL_CB)(uint16_t result);
typedef void(tSDP_DISC_CMPL_CB2)(uint16_t result, void* user_data);

typedef struct {
  RawAddress peer_addr;
  uint16_t peer_mtu;
} tSDP_DR_OPEN;

typedef struct {
  uint8_t* p_data;
  uint16_t data_len;
} tSDP_DR_DATA;

typedef union {
  tSDP_DR_OPEN open;
  tSDP_DR_DATA data;
} tSDP_DATA;

/* Define a callback function for when discovery result is received. */
typedef void(tSDP_DISC_RES_CB)(uint16_t event, tSDP_DATA* p_data);

/* Define a structure to hold the discovered service information. */
typedef struct {
  union {
    uint8_t u8;                           /* 8-bit integer            */
    uint16_t u16;                         /* 16-bit integer           */
    uint32_t u32;                         /* 32-bit integer           */
    uint8_t array[SDP_DISC_ATTR_MAX_LEN]; /* Variable length field    */
    struct t_sdp_disc_attr* p_sub_attr;   /* Addr of first sub-attr (list)*/
  } v;

} tSDP_DISC_ATVAL;

typedef struct t_sdp_disc_attr {
  struct t_sdp_disc_attr* p_next_attr; /* Addr of next linked attr     */
  uint16_t attr_id;                    /* Attribute ID                 */
  uint16_t attr_len_type;              /* Length and type fields       */
  tSDP_DISC_ATVAL attr_value;          /* Variable length entry data   */
} tSDP_DISC_ATTR;

typedef struct t_sdp_disc_rec {
  tSDP_DISC_ATTR* p_first_attr;      /* First attribute of record    */
  struct t_sdp_disc_rec* p_next_rec; /* Addr of next linked record   */
  uint32_t time_read;                /* The time the record was read */
  RawAddress remote_bd_addr;         /* Remote BD address            */
} tSDP_DISC_REC;

typedef struct {
  uint32_t mem_size;          /* Memory size of the DB        */
  uint32_t mem_free;          /* Memory still available       */
  tSDP_DISC_REC* p_first_rec; /* Addr of first record in DB   */
  uint16_t num_uuid_filters;  /* Number of UUIds to filter    */
  bluetooth::Uuid uuid_filters[SDP_MAX_UUID_FILTERS]; /* UUIDs to filter */
  uint16_t num_attr_filters; /* Number of attribute filters  */
  uint16_t attr_filters[SDP_MAX_ATTR_FILTERS]; /* Attributes to filter */
  uint8_t* p_free_mem; /* Pointer to free memory       */
#if (SDP_RAW_DATA_INCLUDED == TRUE)
  uint8_t*
      raw_data; /* Received record from server. allocated/released by client  */
  uint32_t raw_size; /* size of raw_data */
  uint32_t raw_used; /* length of raw_data used */
#endif
} tSDP_DISCOVERY_DB;

/* This structure is used to add protocol lists and find protocol elements */
typedef struct {
  uint16_t protocol_uuid;
  uint16_t num_params;
  uint16_t params[SDP_MAX_PROTOCOL_PARAMS];
} tSDP_PROTOCOL_ELEM;

typedef struct {
  uint16_t num_elems;
  tSDP_PROTOCOL_ELEM list_elem[SDP_MAX_LIST_ELEMS];
} tSDP_PROTO_LIST_ELEM;

/* Device Identification (DI) data structure
*/
/* Used to set the DI record */
typedef struct t_sdp_di_record {
  uint16_t vendor;
  uint16_t vendor_id_source;
  uint16_t product;
  uint16_t version;
  bool primary_record;
  char client_executable_url[SDP_MAX_ATTR_LEN]; /* optional */
  char service_description[SDP_MAX_ATTR_LEN];   /* optional */
  char documentation_url[SDP_MAX_ATTR_LEN];     /* optional */
} tSDP_DI_RECORD;

/* Used to get the DI record */
typedef struct t_sdp_di_get_record {
  uint16_t spec_id;
  tSDP_DI_RECORD rec;
} tSDP_DI_GET_RECORD;

/* API into the SDP layer for service discovery. */

/*******************************************************************************
 *
 * Function         SDP_InitDiscoveryDb
 *
 * Description      This function is called to initialize a discovery database.
 *
 * Returns          true if successful, false if one or more parameters are bad
 *
 ******************************************************************************/
bool SDP_InitDiscoveryDb(tSDP_DISCOVERY_DB* p_db, uint32_t len,
                         uint16_t num_uuid, const bluetooth::Uuid* p_uuid_list,
                         uint16_t num_attr, uint16_t* p_attr_list);

/*******************************************************************************
 *
 * Function         SDP_CancelServiceSearch
 *
 * Description      This function cancels an active query to an SDP server.
 *
 * Returns          true if discovery cancelled, false if a matching activity is
 *                  not found.
 *
 ******************************************************************************/
bool SDP_CancelServiceSearch(tSDP_DISCOVERY_DB* p_db);

/*******************************************************************************
 *
 * Function         SDP_ServiceSearchRequest
 *
 * Description      This function queries an SDP server for information.
 *
 * Returns          true if discovery started, false if failed.
 *
 ******************************************************************************/
bool SDP_ServiceSearchRequest(const RawAddress& p_bd_addr,
                              tSDP_DISCOVERY_DB* p_db, tSDP_DISC_CMPL_CB* p_cb);

/*******************************************************************************
 *
 * Function         SDP_ServiceSearchAttributeRequest
 *
 * Description      This function queries an SDP server for information.
 *
 *                  The difference between this API function and the function
 *                  SDP_ServiceSearchRequest is that this one does a
 *                  combined ServiceSearchAttributeRequest SDP function.
 *
 * Returns          true if discovery started, false if failed.
 *
 ******************************************************************************/
bool SDP_ServiceSearchAttributeRequest(const RawAddress& p_bd_addr,
                                       tSDP_DISCOVERY_DB* p_db,
                                       tSDP_DISC_CMPL_CB* p_cb);

/*******************************************************************************
 *
 * Function         SDP_ServiceSearchAttributeRequest2
 *
 * Description      This function queries an SDP server for information.
 *
 *                  The difference between this API function and the function
 *                  SDP_ServiceSearchRequest is that this one does a
 *                  combined ServiceSearchAttributeRequest SDP function with the
 *                  user data piggyback
 *
 * Returns          true if discovery started, false if failed.
 *
 ******************************************************************************/
bool SDP_ServiceSearchAttributeRequest2(const RawAddress& p_bd_addr,
                                        tSDP_DISCOVERY_DB* p_db,
                                        tSDP_DISC_CMPL_CB2* p_cb,
                                        void* user_data);

/* API of utilities to find data in the local discovery database */

/*******************************************************************************
 *
 * Function         SDP_FindAttributeInDb
 *
 * Description      This function queries an SDP database for a specific
 *                  attribute. If the p_start_rec pointer is NULL, it looks from
 *                  the beginning of the database, else it continues from the
 *                  next record after p_start_rec.
 *
 * Returns          Pointer to matching record, or NULL
 *
 ******************************************************************************/
tSDP_DISC_REC* SDP_FindAttributeInDb(tSDP_DISCOVERY_DB* p_db, uint16_t attr_id,
                                     tSDP_DISC_REC* p_start_rec);

/*******************************************************************************
 *
 * Function         SDP_FindAttributeInRec
 *
 * Description      This function searches an SDP discovery record for a
 *                  specific attribute.
 *
 * Returns          Pointer to matching attribute entry, or NULL
 *
 ******************************************************************************/
tSDP_DISC_ATTR* SDP_FindAttributeInRec(tSDP_DISC_REC* p_rec, uint16_t attr_id);

/*******************************************************************************
 *
 * Function         SDP_FindServiceInDb
 *
 * Description      This function queries an SDP database for a specific
 *                  service. If the p_start_rec pointer is NULL, it looks from
 *                  the beginning of the database, else it continues from the
 *                  next record after p_start_rec.
 *
 * Returns          Pointer to record containing service class, or NULL
 *
 ******************************************************************************/
tSDP_DISC_REC* SDP_FindServiceInDb(tSDP_DISCOVERY_DB* p_db,
                                   uint16_t service_uuid,
                                   tSDP_DISC_REC* p_start_rec);

/*******************************************************************************
 *
 * Function         SDP_FindServiceUUIDInDb
 *
 * Description      This function queries an SDP database for a specific
 *                  service. If the p_start_rec pointer is NULL, it looks from
 *                  the beginning of the database, else it continues from the
 *                  next record after p_start_rec.
 *
 * NOTE             the only difference between this function and the previous
 *                  function "SDP_FindServiceInDb()" is that this function takes
 *                  a Uuid input.
 *
 * Returns          Pointer to record containing service class, or NULL
 *
 ******************************************************************************/
tSDP_DISC_REC* SDP_FindServiceUUIDInDb(tSDP_DISCOVERY_DB* p_db,
                                       const bluetooth::Uuid& uuid,
                                       tSDP_DISC_REC* p_start_rec);

/*******************************************************************************
 *
 * Function         SDP_FindServiceUUIDInRec_128bit
 *
 * Description      Read the 128-bit service UUID within a record,
 *                  if there is any.
 *
 * Parameters:      p_rec      - pointer to a SDP record.
 *                  p_uuid     - output parameter to save the UUID found.
 *
 * Returns          true if found, otherwise false.
 *
 ******************************************************************************/
bool SDP_FindServiceUUIDInRec_128bit(tSDP_DISC_REC* p_rec,
                                     bluetooth::Uuid* p_uuid);

/*******************************************************************************
 *
 * Function         SDP_FindServiceInDb_128bit
 *
 * Description      Query an SDP database for a specific service.
 *                  If the p_start_rec pointer is NULL, look from the beginning
 *                  of the database, else continue from the next record after
 *                  p_start_rec.
 *
 * Returns          Pointer to record containing service class, or NULL
 *
 ******************************************************************************/
tSDP_DISC_REC* SDP_FindServiceInDb_128bit(tSDP_DISCOVERY_DB* p_db,
                                          tSDP_DISC_REC* p_start_rec);

/*******************************************************************************
 *
 * Function         SDP_FindProtocolListElemInRec
 *
 * Description      This function looks at a specific discovery record for a
 *                  protocol list element.
 *
 * Returns          true if found, false if not
 *                  If found, the passed protocol list element is filled in.
 *
 ******************************************************************************/
bool SDP_FindProtocolListElemInRec(tSDP_DISC_REC* p_rec, uint16_t layer_uuid,
                                   tSDP_PROTOCOL_ELEM* p_elem);

/*******************************************************************************
 *
 * Function         SDP_FindAvrcpCoverArtPSM
 *
 * Description      This function pulls the cover art protocol service mask from the additional
 *                  protocol descriptor list attribute
 *
 * Returns          TRUE if found, FALSE if not
 *                  If found, the psm will be filled in p_psm.
 *
*******************************************************************************/
bool SDP_FindAvrcpCoverArtPSM (tSDP_DISC_ATTR *p_attr, uint16_t *p_psm);

/*******************************************************************************
 *
 * Function         SDP_FindAddProtoListsElemInRec
 *
 * Description      This function looks at a specific discovery record for a
 *                  protocol list element.
 *
 * Returns          true if found, false if not
 *                  If found, the passed protocol list element is filled in.
 *
 ******************************************************************************/
bool SDP_FindAddProtoListsElemInRec(tSDP_DISC_REC* p_rec, uint16_t layer_uuid,
                                    tSDP_PROTOCOL_ELEM* p_elem);

/*******************************************************************************
 *
 * Function         SDP_FindProfileVersionInRec
 *
 * Description      This function looks at a specific discovery record for the
 *                  Profile list descriptor, and pulls out the version number.
 *                  The version number consists of an 8-bit major version and
 *                  an 8-bit minor version.
 *
 * Returns          true if found, false if not
 *                  If found, the major and minor version numbers that were
 *                  passed in are filled in.
 *
 ******************************************************************************/
bool SDP_FindProfileVersionInRec(tSDP_DISC_REC* p_rec, uint16_t profile_uuid,
                                 uint16_t* p_version);

/* API into SDP for local service database updates */

/*******************************************************************************
 *
 * Function         SDP_CreateRecord
 *
 * Description      This function is called to create a record in the database.
 *                  This would be through the SDP database maintenance API. The
 *                  record is created empty, teh application should then call
 *                  "add_attribute" to add the record's attributes.
 *
 * Returns          Record handle if OK, else 0.
 *
 ******************************************************************************/
uint32_t SDP_CreateRecord(void);

/*******************************************************************************
 *
 * Function         SDP_DeleteRecord
 *
 * Description      This function is called to add a record (or all records)
 *                  from the database. This would be through the SDP database
 *                  maintenance API.
 *
 *                  If a record handle of 0 is passed, all records are deleted.
 *
 * Returns          true if succeeded, else false
 *
 ******************************************************************************/
bool SDP_DeleteRecord(uint32_t handle);

/*******************************************************************************
 *
 * Function         SDP_ReadRecord
 *
 * Description      This function is called to get the raw data of the record
 *                  with the given handle from the database.
 *
 * Returns          -1, if the record is not found.
 *                  Otherwise, the offset (0 or 1) to start of data in p_data.
 *
 *                  The size of data copied into p_data is in *p_data_len.
 *
 ******************************************************************************/
int32_t SDP_ReadRecord(uint32_t handle, uint8_t* p_data, int32_t* p_data_len);

/*******************************************************************************
 *
 * Function         SDP_AddAttribute
 *
 * Description      This function is called to add an attribute to a record.
 *                  This would be through the SDP database maintenance API.
 *                  If the attribute already exists in the record, it is
 *                  replaced with the new value.
 *
 * NOTE             Attribute values must be passed as a Big Endian stream.
 *
 * Returns          true if added OK, else false
 *
 ******************************************************************************/
bool SDP_AddAttribute(uint32_t handle, uint16_t attr_id, uint8_t attr_type,
                      uint32_t attr_len, uint8_t* p_val);

/*******************************************************************************
 *
 * Function         SDP_AddSequence
 *
 * Description      This function is called to add a sequence to a record.
 *                  This would be through the SDP database maintenance API.
 *                  If the sequence already exists in the record, it is replaced
 *                  with the new sequence.
 *
 * NOTE             Element values must be passed as a Big Endian stream.
 *
 * Returns          true if added OK, else false
 *
 ******************************************************************************/
bool SDP_AddSequence(uint32_t handle, uint16_t attr_id, uint16_t num_elem,
                     uint8_t type[], uint8_t len[], uint8_t* p_val[]);

/*******************************************************************************
 *
 * Function         SDP_AddUuidSequence
 *
 * Description      This function is called to add a UUID sequence to a record.
 *                  This would be through the SDP database maintenance API.
 *                  If the sequence already exists in the record, it is replaced
 *                  with the new sequence.
 *
 * Returns          true if added OK, else false
 *
 ******************************************************************************/
bool SDP_AddUuidSequence(uint32_t handle, uint16_t attr_id, uint16_t num_uuids,
                         uint16_t* p_uuids);

/*******************************************************************************
 *
 * Function         SDP_AddProtocolList
 *
 * Description      This function is called to add a protocol descriptor list to
 *                  a record. This would be through the SDP database
 *                  maintenance API. If the protocol list already exists in the
 *                  record, it is replaced with the new list.
 *
 * Returns          true if added OK, else false
 *
 ******************************************************************************/
bool SDP_AddProtocolList(uint32_t handle, uint16_t num_elem,
                         tSDP_PROTOCOL_ELEM* p_elem_list);

/*******************************************************************************
 *
 * Function         SDP_AddAdditionProtoLists
 *
 * Description      This function is called to add a protocol descriptor list to
 *                  a record. This would be through the SDP database maintenance
 *                  API. If the protocol list already exists in the record, it
 *                  is replaced with the new list.
 *
 * Returns          true if added OK, else false
 *
 ******************************************************************************/
bool SDP_AddAdditionProtoLists(uint32_t handle, uint16_t num_elem,
                               tSDP_PROTO_LIST_ELEM* p_proto_list);

/*******************************************************************************
 *
 * Function         SDP_AddProfileDescriptorList
 *
 * Description      This function is called to add a profile descriptor list to
 *                  a record. This would be through the SDP database maintenance
 *                  API. If the version already exists in the record, it is
 *                  replaced with the new one.
 *
 * Returns          true if added OK, else false
 *
 ******************************************************************************/
bool SDP_AddProfileDescriptorList(uint32_t handle, uint16_t profile_uuid,
                                  uint16_t version);

/*******************************************************************************
 *
 * Function         SDP_AddLanguageBaseAttrIDList
 *
 * Description      This function is called to add a language base attr list to
 *                  a record. This would be through the SDP database maintenance
 *                  API. If the version already exists in the record, it is
 *                  replaced with the new one.
 *
 * Returns          true if added OK, else false
 *
 ******************************************************************************/
bool SDP_AddLanguageBaseAttrIDList(uint32_t handle, uint16_t lang,
                                   uint16_t char_enc, uint16_t base_id);

/*******************************************************************************
 *
 * Function         SDP_AddServiceClassIdList
 *
 * Description      This function is called to add a service list to a record.
 *                  This would be through the SDP database maintenance API.
 *                  If the service list already exists in the record, it is
 *                  replaced with the new list.
 *
 * Returns          true if added OK, else false
 *
 ******************************************************************************/
bool SDP_AddServiceClassIdList(uint32_t handle, uint16_t num_services,
                               uint16_t* p_service_uuids);

/*******************************************************************************
 *
 * Function         SDP_DeleteAttribute
 *
 * Description      Delete an attribute from a record.
 *                  This would be through the SDP database maintenance API.
 *
 * Returns          true if deleted OK, else false if not found
 *
 ******************************************************************************/
bool SDP_DeleteAttribute(uint32_t handle, uint16_t attr_id);

/* Device Identification APIs */

/*******************************************************************************
 *
 * Function         SDP_SetLocalDiRecord
 *
 * Description      This function adds a DI record to the local SDP database.
 *
 * Returns          Returns SDP_SUCCESS if record added successfully, else error
 *
 ******************************************************************************/
uint16_t SDP_SetLocalDiRecord(tSDP_DI_RECORD* device_info, uint32_t* p_handle);

/*******************************************************************************
 *
 * Function         SDP_DiDiscover
 *
 * Description      This function queries a remote device for DI information.
 *
 * Returns          SDP_SUCCESS if query started successfully, else error
 *
 ******************************************************************************/
uint16_t SDP_DiDiscover(const RawAddress& remote_device,
                        tSDP_DISCOVERY_DB* p_db, uint32_t len,
                        tSDP_DISC_CMPL_CB* p_cb);

/*******************************************************************************
 *
 * Function         SDP_GetNumDiRecords
 *
 * Description      Searches specified database for DI records
 *
 * Returns          number of DI records found
 *
 ******************************************************************************/
uint8_t SDP_GetNumDiRecords(tSDP_DISCOVERY_DB* p_db);

/*******************************************************************************
 *
 * Function         SDP_GetDiRecord
 *
 * Description      This function retrieves a remote device's DI record from
 *                  the specified database.
 *
 * Returns          SDP_SUCCESS if record retrieved, else error
 *
 ******************************************************************************/
uint16_t SDP_GetDiRecord(uint8_t getRecordIndex,
                         tSDP_DI_GET_RECORD* device_info,
                         tSDP_DISCOVERY_DB* p_db);

/*******************************************************************************
 *
 * Function         SDP_SetTraceLevel
 *
 * Description      This function sets the trace level for SDP. If called with
 *                  a value of 0xFF, it simply reads the current trace level.
 *
 * Returns          the new (current) trace level
 *
 ******************************************************************************/
uint8_t SDP_SetTraceLevel(uint8_t new_level);

/*******************************************************************************
 *
 * Function         SDP_FindServiceUUIDInRec
 *
 * Description      Read the service UUID within a record,
 *                  if there is any.
 *
 * Parameters:      p_rec      - pointer to a SDP record.
 *
 * Returns          true if found, otherwise false.
 *
 ******************************************************************************/
bool SDP_FindServiceUUIDInRec(tSDP_DISC_REC* p_rec, bluetooth::Uuid* p_uuid);

/*********************************************************************************
**
** Function         SDP_Dev_Blacklisted_For_Avrcp15
**
** Description      This function is called to know is local Avrcp Version
**                  1.3 as local Avrcp version is send as 1.3 for black listed
**                  devices
**
** Returns          TRUE if AVRCP local Avrcp Version 1.3 else FALSE
**
********************************************************************************/
bool SDP_Dev_Blacklisted_For_Avrcp15 (RawAddress addr);

bool SDP_AddProfileDescriptorListUuid128(uint32_t handle, uint8_t *profile_uuid,
                                  uint16_t version);

bool SDP_AddServiceClassIdListUuid128(uint32_t handle, uint8_t* p_service_uuids);

#endif /* SDP_API_H */
