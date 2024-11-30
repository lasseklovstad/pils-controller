#ifndef MOCK_MBEDTLS_MD_H
#define MOCK_MBEDTLS_MD_H

#include <cstring>

typedef enum {
    MBEDTLS_MD_NONE=0,    /**< None. */
    MBEDTLS_MD_MD2,       /**< The MD2 message digest. */
    MBEDTLS_MD_MD4,       /**< The MD4 message digest. */
    MBEDTLS_MD_MD5,       /**< The MD5 message digest. */
    MBEDTLS_MD_SHA1,      /**< The SHA-1 message digest. */
    MBEDTLS_MD_SHA224,    /**< The SHA-224 message digest. */
    MBEDTLS_MD_SHA256,    /**< The SHA-256 message digest. */
    MBEDTLS_MD_SHA384,    /**< The SHA-384 message digest. */
    MBEDTLS_MD_SHA512,    /**< The SHA-512 message digest. */
    MBEDTLS_MD_RIPEMD160, /**< The RIPEMD-160 message digest. */
} mbedtls_md_type_t;

typedef struct {
    int dummy;
} mbedtls_md_context_t;

typedef struct {
    int dummy;
} mbedtls_md_info_t;

void mbedtls_md_init(mbedtls_md_context_t *ctx) {}
void mbedtls_md_free(mbedtls_md_context_t *ctx) {}
const mbedtls_md_info_t *mbedtls_md_info_from_type(int md_type) { return nullptr; }
void mbedtls_md_setup(mbedtls_md_context_t *ctx, const mbedtls_md_info_t *md_info, int hmac) {}
void mbedtls_md_hmac_starts(mbedtls_md_context_t *ctx, const unsigned char *key, size_t keylen) {}
void mbedtls_md_hmac_update(mbedtls_md_context_t *ctx, const unsigned char *input, size_t ilen) {}
void mbedtls_md_hmac_finish(mbedtls_md_context_t *ctx, unsigned char *output) {
    std::memset(output, 0, 32); // Mock HMAC result
}

#endif // MOCK_MBEDTLS_MD_H