/*
 * Generated by asn1c-0.9.22 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "./asn1c/ASN1_files/EUTRA-RRC-Definitions.asn"
 * 	`asn1c -gen-PER -fcompound-names -fnative-types`
 */

#ifndef	_RF_Parameters_H_
#define	_RF_Parameters_H_


#include <liblte/rrc/asn/asn_application.h>

/* Including external dependencies */
#include "liblte/rrc/asn/SupportedBandListEUTRA.h"
#include <liblte/rrc/asn/constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* RF-Parameters */
typedef struct RF_Parameters {
	SupportedBandListEUTRA_t	 supportedBandListEUTRA;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RF_Parameters_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RF_Parameters;

#ifdef __cplusplus
}
#endif

#endif	/* _RF_Parameters_H_ */
#include <liblte/rrc/asn/asn_internal.h>
