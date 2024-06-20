#include "symbolTable.h"
#include "hashMap.h"
#include <stdint.h>
#include <string.h>

#define SEED 0x13572468

static hashMapADT table = NULL;

// MurmurOAAT_32_Modified
static uint64_t symbolHashFunction(tAny key) {

    uint64_t h = SEED;
    struct key aux = *(struct key*) key;

    const char * str = aux.varname;
    // One-byte-at-a-time hash based on Murmur's mix
    // Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp
    for (; *str; ++str) {
        h ^= *str;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}

static bool symbolKeyEquals(tAny key1, tAny key2) {
    struct key aux1 = *(struct key *) key1;
    struct key aux2 = *(struct key *) key2;
    return strcmp(aux1.varname, aux2.varname) == 0;
}

void symbolTableInit() {
    if(table != NULL) {
        symbolTableDestroy();
    }
    table = hashMapInit(sizeof(struct key), sizeof(struct value), symbolHashFunction, symbolKeyEquals);
}

bool symbolTableFind(struct key * key, struct value * value) {
    return hashMapFind(table, key, value);
}

void symbolTableInsert(struct key * key, struct value * value) {
    hashMapInsertOrUpdate(table, key, value);
}

void symbolTableDestroy() {
    hashMapDestroy(table);
    table = NULL;
}