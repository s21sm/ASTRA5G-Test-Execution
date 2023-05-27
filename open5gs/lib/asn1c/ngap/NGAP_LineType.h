/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NGAP-IEs"
 * 	found in "../support/ngap-r16.7.0/38413-g70.asn"
 * 	`asn1c -pdu=all -fcompound-names -findirect-choice -fno-include-deps -no-gen-BER -no-gen-XER -no-gen-OER -no-gen-UPER`
 */

#ifndef	_NGAP_LineType_H_
#define	_NGAP_LineType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum NGAP_LineType {
	NGAP_LineType_dsl	= 0,
	NGAP_LineType_pon	= 1
	/*
	 * Enumeration is extensible
	 */
} e_NGAP_LineType;

/* NGAP_LineType */
typedef long	 NGAP_LineType_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_NGAP_LineType_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_NGAP_LineType;
extern const asn_INTEGER_specifics_t asn_SPC_LineType_specs_1;
asn_struct_free_f LineType_free;
asn_struct_print_f LineType_print;
asn_constr_check_f LineType_constraint;
jer_type_encoder_f LineType_encode_jer;
per_type_decoder_f LineType_decode_aper;
per_type_encoder_f LineType_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _NGAP_LineType_H_ */
#include <asn_internal.h>
