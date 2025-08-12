#ifndef __HASH_H
#define __HASH_H
#include <stdint.h>
#include <stddef.h>

uint64_t Jenkins_one_at_a_time_hash(void *data, size_t size);
uint64_t crc32b(void *data, size_t size);
uint64_t KR_v2_hash(void *data, size_t size);
uint64_t MurmurOAAT_32(void *data, size_t size);
uint64_t FNV(void *data, size_t size);
uint64_t DJB2_hash(void *data, size_t size);

#ifndef hash_f

#define hash_f(ptr__, size__) Jenkins_one_at_a_time_hash(ptr__, size__)

#endif

#endif