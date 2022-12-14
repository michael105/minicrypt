//  BLAKE2 - size-optimized implementation
//
//  Copyright 2012, Samuel Neves <sneves@dei.uc.pt> (original work)
//  Copyright 2018, Ayke van Laethem
//  Copyright 2020, Cameron Martens
//
//  You may use this under the terms of the CC0, the OpenSSL Licence, or
//  the Apache Public License 2.0, at your option. The terms of these
//  licenses can be found at:
//
//  - CC0 1.0 Universal : http://creativecommons.org/publicdomain/zero/1.0
//  - OpenSSL license   : https://www.openssl.org/source/license.html
//  - Apache 2.0        : http://www.apache.org/licenses/LICENSE-2.0
//
//  More information about the BLAKE2 hash function can be found at
//  https://blake2.net

#pragma once

//#include <stddef.h>
//#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

//Fixed constant that cannot be changed.
#define BLAKE2S_BLOCKBYTES (64)

//Options that can be set depending on application needs.
//Except for BLAKE2S_OUTLEN, disabling features will improve code size.
#define BLAKE2S_64BIT     (0)   //Input length may be 4GB or larger
#define BLAKE2S_KEYED     (0)   //Support for keyed hashing

typedef struct {
    uint32_t h[8];
    union {
        uint32_t t[2];
        uint64_t T;
    };
    uint32_t f0;
    uint32_t buflen;
    uint8_t  buf[BLAKE2S_BLOCKBYTES];
} blake2s_state;

//API. See the options above for the applicable restrictions.
#if BLAKE2S_OUTLEN == 0
void blake2s_init(blake2s_state *S, size_t outlen);
#if BLAKE2S_KEYED
void blake2s_init_key(blake2s_state *S, size_t outlen, const void *key, size_t keylen);
#endif
void blake2s_final(blake2s_state *S, void *out, size_t outlen);
#else
void blake2s_init(blake2s_state *S);
#if BLAKE2S_KEYED
void blake2s_init_key(blake2s_state *S, const void *key, size_t keylen);
#endif
void blake2s_final(blake2s_state *S, void *out);
#endif
void blake2s_update(blake2s_state *S, const void *in, size_t inlen);

#if defined(__cplusplus)
}
#endif
