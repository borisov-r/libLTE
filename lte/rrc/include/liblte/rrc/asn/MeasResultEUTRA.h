/*
 * Generated by asn1c-0.9.22 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "./asn1c/ASN1_files/EUTRA-RRC-Definitions.asn"
 * 	`asn1c -gen-PER -fcompound-names -fnative-types`
 */

#ifndef	_MeasResultEUTRA_H_
#define	_MeasResultEUTRA_H_


#include <liblte/rrc/asn/asn_application.h>

/* Including external dependencies */
#include "liblte/rrc/asn/PhysCellId.h"
#include "liblte/rrc/asn/CellGlobalIdEUTRA.h"
#include "liblte/rrc/asn/TrackingAreaCode.h"
#include <liblte/rrc/asn/constr_SEQUENCE.h>
#include "liblte/rrc/asn/RSRP-Range.h"
#include "liblte/rrc/asn/RSRQ-Range.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct PLMN_IdentityList2;
struct AdditionalSI_Info_r9;

/* MeasResultEUTRA */
typedef struct MeasResultEUTRA {
	PhysCellId_t	 physCellId;
	struct MeasResultEUTRA__cgi_Info {
		CellGlobalIdEUTRA_t	 cellGlobalId;
		TrackingAreaCode_t	 trackingAreaCode;
		struct PLMN_IdentityList2	*plmn_IdentityList	/* OPTIONAL */;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *cgi_Info;
	struct MeasResultEUTRA__measResult {
		RSRP_Range_t	*rsrpResult	/* OPTIONAL */;
		RSRQ_Range_t	*rsrqResult	/* OPTIONAL */;
		struct AdditionalSI_Info_r9	*additionalSI_Info_r9	/* OPTIONAL */;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} measResult;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MeasResultEUTRA_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_MeasResultEUTRA;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "liblte/rrc/asn/PLMN-IdentityList2.h"
#include "liblte/rrc/asn/AdditionalSI-Info-r9.h"

#endif	/* _MeasResultEUTRA_H_ */
#include <liblte/rrc/asn/asn_internal.h>
