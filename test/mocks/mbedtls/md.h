#ifndef MOCK_MBEDTLS_MD_H
#define MOCK_MBEDTLS_MD_H

#include <cstring>

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