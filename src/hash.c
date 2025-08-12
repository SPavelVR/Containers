#include "..\include\hash.h"

// все взято с этого поста: https://stackoverflow.com/questions/7666509/hash-function-for-string

uint64_t DJB2_hash(void *data, size_t size)
{
    uint8_t* str = (uint8_t*) data;
    uint64_t hash = 5381;
    for (size_t i = 0; i < size; i++)
        hash = ((hash << 5) + hash) + str[i]; /* hash * 33 + c */
    return hash;
}

uint64_t FNV(void *data, size_t size)
{
    // See: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp
    uint64_t h = 2166136261UL;
    uint8_t* key = (uint8_t*) data;
    for(size_t i = 0; i < size; i++)
    {
        h ^= key[i];
        h *= 16777619;
    }
    return h;
}

uint64_t MurmurOAAT_32(void *data, size_t size)
{
    // One-byte-at-a-time hash based on Murmur's mix
    // Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp
    uint64_t h = 2166136261UL;
    uint8_t* str = (uint8_t*) data;
    for (size_t i = 0; i < size; i++) {
        h ^= str[i];
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}

uint64_t KR_v2_hash(void *data, size_t size)
{
    // Source: https://stackoverflow.com/a/45641002/5407270
    // a.k.a. Java String hashCode()
    uint64_t hashval = 0;
    uint8_t* str = (uint8_t*) data;
    for (size_t i = 0; i < size; i++)
        hashval = str[i] + 31*hashval;
    return hashval;
}

uint64_t Jenkins_one_at_a_time_hash(void *data, size_t size)
{
    uint64_t hash = 0xCBF29CE484222325;
    uint8_t* str = (uint8_t*) data;
    for (size_t i = 0; i < size; ++i)
    {
        hash += str[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

uint64_t crc32b(void *data, size_t size) {
    // Source: https://stackoverflow.com/a/21001712
    uint8_t* str = (uint8_t*) data;
    uint64_t byte, crc, mask;
    size_t j = 0;
    crc = 0xFFFFFFFF;
    for (size_t i = 0; i < size; i++) {
        byte = str[i];
        crc = crc ^ byte;
        for (j = 7; j >= 0; j--) {
            mask = ~(crc & 1);
            crc = (crc >> 1) ^ (0xEDB88320 & mask);
        }
    }
    return ~crc;
}
