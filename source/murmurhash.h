//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.

#ifndef __included_murmurhash_h
#define __included_murmurhash_h

#if defined(_MSC_VER)

typedef unsigned char uint8_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;

#else

#include <stdint.h>

#endif

void murmurhash3_x86_32(const void * key, int len, uint32_t seed, void * out);
void murmurhash3_x86_128(const void * key, int len, uint32_t seed, void * out);
void murmurhash3_x64_128(const void * key, int len, uint32_t seed, void * out);

#endif // __included_murmurhash_h
