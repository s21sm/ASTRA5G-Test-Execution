/*
 * Copyright (C) 2019-2022 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "sbi-path.h"
#include "nnrf-handler.h"

void pcf_nnrf_handle_nf_discover(
        ogs_sbi_xact_t *xact, ogs_sbi_message_t *recvmsg)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_object_t *sbi_object = NULL;
    ogs_sbi_service_type_e service_type = OGS_SBI_SERVICE_TYPE_NULL;
    ogs_sbi_discovery_option_t *discovery_option = NULL;
    ogs_sbi_stream_t *stream = NULL;

    pcf_ue_t *pcf_ue = NULL;
    pcf_sess_t *sess = NULL;

    OpenAPI_nf_type_e target_nf_type = OpenAPI_nf_type_NULL;
    OpenAPI_nf_type_e requester_nf_type = OpenAPI_nf_type_NULL;
    OpenAPI_search_result_t *SearchResult = NULL;

    ogs_assert(recvmsg);
    ogs_assert(xact);
    sbi_object = xact->sbi_object;
    ogs_assert(sbi_object);
    service_type = xact->service_type;
    ogs_assert(service_type);
    target_nf_type = ogs_sbi_service_type_to_nf_type(service_type);
    ogs_assert(target_nf_type);
    requester_nf_type = xact->requester_nf_type;
    ogs_assert(requester_nf_type);

    discovery_option = xact->discovery_option;
    stream = xact->assoc_stream;

    SearchResult = recvmsg->SearchResult;
    if (!SearchResult) {
        ogs_error("No SearchResult");
        return;
    }

    if (sbi_object->type == OGS_SBI_OBJ_UE_TYPE) {
        pcf_ue = (pcf_ue_t *)sbi_object;
        ogs_assert(pcf_ue);
    } else if (sbi_object->type == OGS_SBI_OBJ_SESS_TYPE) {
        sess = (pcf_sess_t *)sbi_object;
        ogs_assert(sess);
        pcf_ue = sess->pcf_ue;
        ogs_assert(pcf_ue);
    } else {
        ogs_fatal("(NF discover) Not implemented [%s:%d]",
            ogs_sbi_service_type_to_name(service_type), sbi_object->type);
        ogs_assert_if_reached();
    }

    ogs_nnrf_disc_handle_nf_discover_search_result(SearchResult);

    nf_instance = ogs_sbi_nf_instance_find_by_discovery_param(
                    target_nf_type, requester_nf_type, discovery_option);
    if (!nf_instance) {
        ogs_error("[%s:%d] (NF discover) No [%s:%s]",
                    pcf_ue ? pcf_ue->supi : "Unknown",
                    sess ? sess->psi : 0,
                    ogs_sbi_service_type_to_name(service_type),
                    OpenAPI_nf_type_ToString(requester_nf_type));
        return;
    }

    OGS_SBI_SETUP_NF_INSTANCE(
            sbi_object->service_type_array[service_type], nf_instance);

    switch (service_type) {
    case OGS_SBI_SERVICE_TYPE_NPCF_POLICYAUTHORIZATION:
        ogs_sbi_xact_remove(xact);

        ogs_assert(sess);
        ogs_assert(stream);
        ogs_assert(true ==
                pcf_sess_sbi_discover_and_send(
                    OGS_SBI_SERVICE_TYPE_NBSF_MANAGEMENT, NULL,
                    pcf_nbsf_management_build_register,
                    sess, stream, nf_instance));
        break;
    default:
        ogs_assert(xact->request);
        ogs_expect(true == pcf_sbi_send_request(nf_instance, xact));
        break;
    }
}
