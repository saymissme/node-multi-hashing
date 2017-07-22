#include "skunk.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "sha3/sph_skein.h"
#include "sha3/sph_cubehash.h"
#include "sha3/sph_fugue.h"
#include "sha3/sph_gost.h"


void skunk_hash(const char* input, char* output, uint32_t len)
{

	sph_skein512_context    ctx_skein;
	sph_cubehash512_context ctx_cube;
	sph_fugue512_context    ctx_fugue;
	sph_gost512_context     ctx_gost;

	uint32_t hash[16];

	sph_skein512_init(&ctx_skein);
	sph_skein512(&ctx_skein, input, len);
	sph_skein512_close(&ctx_skein, hash);

	sph_cubehash512_init(&ctx_cube);
	sph_cubehash512(&ctx_cube, hash, 64);
	sph_cubehash512_close(&ctx_cube, hash);

	sph_fugue512_init (&ctx_fugue);
	sph_fugue512(&ctx_fugue, hash, 64);
	sph_fugue512_close(&ctx_fugue, hash);

	sph_gost512_init(&ctx_gost);
	sph_gost512(&ctx_gost, hash, 64);
	sph_gost512_close(&ctx_gost, hash);

	memcpy(output, hash, 32);
}

