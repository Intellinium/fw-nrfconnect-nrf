/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr.h>
#include <sys/printk.h>
#include <stdio.h>
#include <stdlib.h>
#include <psa/crypto.h>
#include <psa/crypto_extra.h>
#include <logging/log.h>
#include <crypto_common.h>

LOG_MODULE_REGISTER(chachapoly, LOG_LEVEL_DBG);

/* ====================================================================== */
/*		Global variables/defines for the Chacha20-Poly1305 example		  */

#define NRF_CRYPTO_EXAMPLE_CHACHAPOLY_TEXT_SIZE (100)
#define NRF_CRYPTO_EXAMPLE_CHACHAPOLY_ADDITIONAL_SIZE (35)
#define NRF_CRYPTO_EXAMPLE_CHACHAPOLY_NONCE_SIZE (12)
#define NRF_CRYPTO_EXAMPLE_CHACHAPOLY_TAG_SIZE (16)

/* Chacha20-Poly1305 sample Nonce, DO NOT USE IN PRODUCTION */
static uint8_t m_nonce[NRF_CRYPTO_EXAMPLE_CHACHAPOLY_NONCE_SIZE];

/* Below text is used as plaintext for encryption/decryption */
static uint8_t m_plain_text[NRF_CRYPTO_EXAMPLE_CHACHAPOLY_TEXT_SIZE] = {
	"Example string to demonstrate basic usage of Chacha20-Poly1305."
};

/* Below text is used as additional data for authentication */
static uint8_t
	m_additional_data[NRF_CRYPTO_EXAMPLE_CHACHAPOLY_ADDITIONAL_SIZE] = {
		"Example string of additional data"
	};

static uint8_t m_encrypted_text[NRF_CRYPTO_EXAMPLE_CHACHAPOLY_TEXT_SIZE +
				NRF_CRYPTO_EXAMPLE_CHACHAPOLY_TAG_SIZE];

static uint8_t m_decrypted_text[NRF_CRYPTO_EXAMPLE_CHACHAPOLY_TEXT_SIZE];

psa_key_handle_t key_handle;
/* ====================================================================== */

int crypto_finish(void)
{
	psa_status_t status;

	/* Destroy the key handle */
	status = psa_destroy_key(key_handle);
	if (status != PSA_SUCCESS) {
		PRINT_ERROR("psa_destroy_key", status);
		return APP_ERROR;
	}

	return APP_SUCCESS;
}

int generate_key(void)
{
	psa_status_t status;
	psa_key_attributes_t key_attributes = PSA_KEY_ATTRIBUTES_INIT;

	/* Crypto settings for Chacha20-Poly1305 */
	psa_set_key_usage_flags(&key_attributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
	psa_set_key_lifetime(&key_attributes, PSA_KEY_LIFETIME_VOLATILE);
	psa_set_key_algorithm(&key_attributes, PSA_ALG_CHACHA20_POLY1305);
	psa_set_key_type(&key_attributes, PSA_KEY_TYPE_CHACHA20);
	psa_set_key_bits(&key_attributes, 256);

	/* Generate a random key. The key is not exposed to the application,
	 * we can use it to encrypt/decrypt using the key handle
	 */
	status = psa_generate_key(&key_attributes, &key_handle);
	if (status != PSA_SUCCESS) {
		PRINT_ERROR("psa_generate_key", status);
		return APP_ERROR;
	}

	/* After the key handle is acquired the attributes are not needed */
	psa_reset_key_attributes(&key_attributes);

	PRINT_MESSAGE("ChachaPoly key generated successfully!");

	return APP_SUCCESS;
}

int encrypt_chachapoly(void)
{
	uint32_t output_len;
	psa_status_t status;

	PRINT_MESSAGE("Encrypting using Chacha20-Poly1305...");

	/* Generate a random nonce */
	status = psa_generate_random(m_nonce, NRF_CRYPTO_EXAMPLE_CHACHAPOLY_NONCE_SIZE);
	if (status != PSA_SUCCESS) {
		PRINT_ERROR("psa_generate_random", status);
		return APP_ERROR;
	}

	/* Perform the authenticated encryption */
	status = psa_aead_encrypt(key_handle,
				  PSA_ALG_CHACHA20_POLY1305,
				  m_nonce,
				  sizeof(m_nonce),
				  m_additional_data,
				  sizeof(m_additional_data),
				  m_plain_text,
				  sizeof(m_plain_text),
				  m_encrypted_text,
				  sizeof(m_encrypted_text),
				  &output_len);
	if (status != PSA_SUCCESS) {
		PRINT_ERROR("psa_aead_encrypt", status);
		return APP_ERROR;
	}

	PRINT_MESSAGE("Encryption successful!");
	PRINT_HEX("Nonce", m_nonce, sizeof(m_nonce));
	PRINT_HEX("Plaintext", m_plain_text, sizeof(m_plain_text));
	PRINT_HEX("Additional data", m_additional_data, sizeof(m_additional_data));
	PRINT_HEX("Encrypted text", m_encrypted_text, sizeof(m_encrypted_text));

	return APP_SUCCESS;
}

int decrypt_chachapoly(void)
{
	uint32_t output_len;
	psa_status_t status;

	PRINT_MESSAGE("Decrypting using Chacha20-Poly1305 ...");

	/* Decrypt and authenticate the encrypted data */
	status = psa_aead_decrypt(key_handle,
				  PSA_ALG_CHACHA20_POLY1305,
				  m_nonce,
				  sizeof(m_nonce),
				  m_additional_data,
				  sizeof(m_additional_data),
				  m_encrypted_text,
				  sizeof(m_encrypted_text),
				  m_decrypted_text,
				  sizeof(m_decrypted_text),
				  &output_len);
	if (status != PSA_SUCCESS) {
		PRINT_ERROR("psa_aead_decrypt", status);
		return APP_ERROR;
	}

	PRINT_HEX("Decrypted text", m_decrypted_text, sizeof(m_decrypted_text));

	/* Check the validity of the decryption */
	if (memcmp(m_decrypted_text, m_plain_text, NRF_CRYPTO_EXAMPLE_CHACHAPOLY_TEXT_SIZE) != 0) {
		PRINT_MESSAGE("Error: Decrypted text doesn't match the plaintext");
		return APP_ERROR;
	}

	PRINT_MESSAGE("Decryption and authentication was successful!");

	return APP_SUCCESS;
}

int main(void)
{
	int status;

	PRINT_MESSAGE("Starting ChachaPoly example...");
	status = crypto_init();
	if (status != APP_SUCCESS) {
		PRINT_MESSAGE(APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	status = generate_key();
	if (status != APP_SUCCESS) {
		PRINT_MESSAGE(APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	status = encrypt_chachapoly();
	if (status != APP_SUCCESS) {
		PRINT_MESSAGE(APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	status = decrypt_chachapoly();
	if (status != APP_SUCCESS) {
		PRINT_MESSAGE(APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	status = crypto_finish();
	if (status != APP_SUCCESS) {
		PRINT_MESSAGE(APP_ERROR_MESSAGE);
		return APP_ERROR;
	}

	PRINT_MESSAGE(APP_SUCCESS_MESSAGE);
	return APP_SUCCESS;
}
