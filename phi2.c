
#include "phi2.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

// Hashing modules

#include "sha3/sph_skein.h"
#include "sha3/sph_jh.h"
#include "sha3/sph_cubehash.h"
#include "sha3/sph_fugue.h"
#include "sha3/sph_gost.h"
#include "sha3/sph_echo.h"
#include "Lyra2RE/Lyra2.h"

#include "common.h"

void phi2_hash(const char* input, char* output, uint32_t len)
{
	sph_cubehash512_context     ctx_cubehash;
	sph_jh512_context           ctx_jh;
	sph_gost512_context         ctx_gost;
	sph_echo512_context         ctx_echo;
	sph_skein512_context        ctx_skein;
		
	uint8_t _ALIGN(128) hash[128], hashA[64], hashB[64] ;

	sph_cubehash512_init(&ctx_cubehash);
	sph_cubehash512(&ctx_cubehash, input, len);
	sph_cubehash512_close(&ctx_cubehash, (void*) hashB);

    LYRA2_phi2(&hashA[ 0], 32, &hashB[ 0], 32, &hashB[ 0], 32, 1, 8, 8);
    LYRA2_phi2(&hashA[32], 32, &hashB[32], 32, &hashB[32], 32, 1, 8, 8);

	sph_jh512_init(&ctx_jh);
    sph_jh512(&ctx_jh, (const void*)hashA, 64);
    sph_jh512_close(&ctx_jh, (void*)hash);

    if (hash[0] & 1) {
        sph_gost512_init(&ctx_gost);
        sph_gost512(&ctx_gost, (const void*)hash, 64);
        sph_gost512_close(&ctx_gost, (void*)hash);
    } else {
        sph_echo512_init(&ctx_echo);
        sph_echo512(&ctx_echo, (const void*)hash, 64);
        sph_echo512_close(&ctx_echo, (void*)hash);

        sph_echo512_init(&ctx_echo);
        sph_echo512(&ctx_echo, (const void*)hash, 64);
        sph_echo512_close(&ctx_echo, (void*)hash);
    }
    sph_skein512_init(&ctx_skein);
    sph_skein512(&ctx_skein, (const void*)hash, 64);
    sph_skein512_close(&ctx_skein, (void*)hash);

	for (int i=0; i<32; i++)
		hash[i] ^= hash[i+32];

	memcpy(output, hash, 32);
}
