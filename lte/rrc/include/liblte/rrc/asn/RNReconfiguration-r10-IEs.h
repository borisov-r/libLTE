/*
 * Generated by asn1c-0.9.22 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "./asn1c/ASN1_files/EUTRA-RRC-Definitions.asn"
 * 	`asn1c -gen-PER -fcompound-names -fnative-types`
 */

#ifndef	_RNReconfiguration_r10_IEs_H_
#define	_RNReconfiguration_r10_IEs_H_


#include <liblte/rrc/asn/asn_application.h>

/* Including external dependencies */
#include <liblte/rrc/asn/OCTET_STRING.h>
#include <liblte/rrc/asn/constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct RN_SystemInfo_r10;
struct RN_SubframeConfig_r10;

/* RNReconfiguration-r10-IEs */
typedef struct RNReconfiguration_r10_IEs {
	struct RN_SystemInfo_r10	*rn_SystemInfo_r10	/* OPTIONAL */;
	struct RN_SubframeConfig_r10	*rn_SubframeConfig_r10	/* OPTIONAL */;
	OCTET_STRING_t	*lateNonCriticalExtension	/* OPTIONAL */;
	struct RNReconfiguration_r10_IEs__nonCriticalExtension {
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *nonCriticalExtension;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RNReconfiguration_r10_IEs_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RNReconfiguration_r10_IEs;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "liblte/rrc/asn/RN-SystemInfo-r10.h"
#include "liblte/rrc/asn/RN-SubframeConfig-r10.h"

#endif	/* _RNReconfiguration_r10_IEs_H_ */
#include <liblte/rrc/asn/asn_internal.h>
