/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <fcc_status.h>
#include <toolchain/common.h>
#include "pelion_fcc_err.h"

#define FCC_STATUS_TO_STRING(_s) case _s: error = STRINGIFY(_s); break

const char *fcc_status_to_string(int fcc_status)
{
	const char *error;

	switch (fcc_status) {
	FCC_STATUS_TO_STRING(FCC_STATUS_SUCCESS);
	FCC_STATUS_TO_STRING(FCC_STATUS_ERROR);
	FCC_STATUS_TO_STRING(FCC_STATUS_MEMORY_OUT);
	FCC_STATUS_TO_STRING(FCC_STATUS_INVALID_PARAMETER);
	FCC_STATUS_TO_STRING(FCC_STATUS_STORE_ERROR);
	FCC_STATUS_TO_STRING(FCC_STATUS_INTERNAL_ITEM_ALREADY_EXIST);
	FCC_STATUS_TO_STRING(FCC_STATUS_CA_ERROR);
	FCC_STATUS_TO_STRING(FCC_STATUS_ROT_ERROR);
	FCC_STATUS_TO_STRING(FCC_STATUS_ENTROPY_ERROR);
	FCC_STATUS_TO_STRING(FCC_STATUS_FACTORY_DISABLED_ERROR);
	FCC_STATUS_TO_STRING(FCC_STATUS_INVALID_CERTIFICATE);
	FCC_STATUS_TO_STRING(FCC_STATUS_INVALID_CERT_ATTRIBUTE);
	FCC_STATUS_TO_STRING(FCC_STATUS_INVALID_CA_CERT_SIGNATURE);
	FCC_STATUS_TO_STRING(FCC_STATUS_EXPIRED_CERTIFICATE);
	FCC_STATUS_TO_STRING(FCC_STATUS_INVALID_LWM2M_CN_ATTR);
	FCC_STATUS_TO_STRING(FCC_STATUS_KCM_ERROR);
	FCC_STATUS_TO_STRING(FCC_STATUS_KCM_STORAGE_ERROR);
	FCC_STATUS_TO_STRING(FCC_STATUS_KCM_FILE_EXIST_ERROR);
	FCC_STATUS_TO_STRING(FCC_STATUS_KCM_CRYPTO_ERROR);
	FCC_STATUS_TO_STRING(FCC_STATUS_NOT_INITIALIZED);
	FCC_STATUS_TO_STRING(FCC_STATUS_BUNDLE_ERROR);
	FCC_STATUS_TO_STRING(FCC_STATUS_BUNDLE_RESPONSE_ERROR);
	FCC_STATUS_TO_STRING(FCC_STATUS_BUNDLE_UNSUPPORTED_GROUP);
	FCC_STATUS_TO_STRING(FCC_STATUS_BUNDLE_INVALID_GROUP);
	FCC_STATUS_TO_STRING(FCC_STATUS_BUNDLE_INVALID_SCHEME);
	FCC_STATUS_TO_STRING(FCC_STATUS_ITEM_NOT_EXIST);
	FCC_STATUS_TO_STRING(FCC_STATUS_EMPTY_ITEM);
	FCC_STATUS_TO_STRING(FCC_STATUS_WRONG_ITEM_DATA_SIZE);
	FCC_STATUS_TO_STRING(FCC_STATUS_URI_WRONG_FORMAT);
	FCC_STATUS_TO_STRING(FCC_STATUS_FIRST_TO_CLAIM_NOT_ALLOWED);
	FCC_STATUS_TO_STRING(FCC_STATUS_BOOTSTRAP_MODE_ERROR);
	FCC_STATUS_TO_STRING(FCC_STATUS_OUTPUT_INFO_ERROR);
	FCC_STATUS_TO_STRING(FCC_STATUS_WARNING_CREATE_ERROR);
	FCC_STATUS_TO_STRING(FCC_STATUS_UTC_OFFSET_WRONG_FORMAT);
	FCC_STATUS_TO_STRING(FCC_STATUS_CERTIFICATE_PUBLIC_KEY_CORRELATION_ERROR);
	FCC_STATUS_TO_STRING(FCC_STATUS_BUNDLE_INVALID_KEEP_ALIVE_SESSION_STATUS);
	FCC_STATUS_TO_STRING(FCC_STATUS_TOO_MANY_CSR_REQUESTS);
	FCC_STATUS_TO_STRING(FCC_STATUS_NOT_SUPPORTED);
	default:
		error = "UNKNOWN";
	}

	return error;
}