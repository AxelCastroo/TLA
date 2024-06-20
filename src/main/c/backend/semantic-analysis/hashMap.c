#include "hashMap.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct entry{
    tAny key;
    tAny value;
    uint8_t status;
} entry;

typedef struct hashMapCDT {
    uint64_t totalSize;
    uint64_t usedSize;
    double threshold;
    hashFp prehash;
    keyEqualsFp keyEquals;
    entry * lookup;
    uint64_t lookupSize;
    uint64_t keySize;
    uint64_t valueSize;
} hashMapCDT;

#define INITIAL_SIZE 20
typedef enum {FREE = 0, USED, BRIDGE} status;

hashMapADT hashMapInit(uint64_t keySize, uint64_t valueSize, hashFp prehash, keyEqualsFp keyEquals) {
    hashMapADT new = malloc(sizeof(hashMapCDT));
    if(new == NULL || (new->lookup = calloc(INITIAL_SIZE, sizeof(entry))) == NULL) {
        free(new);
        return NULL;
    }
    new->totalSize = 0;
    new->usedSize = 0;
    new->threshold = 0.75;
    new->prehash = prehash;
    new->keyEquals = keyEquals;
    new->lookupSize = INITIAL_SIZE;
    new->keySize = keySize;
    new->valueSize = valueSize;
    return new;
}

static uint64_t hash(hashMapADT hm, tAny key) {
    return hm->prehash(key) % hm->lookupSize;
}

static void resize(hashMapADT hm) {
    entry * oldTable = hm->lookup;
    hm->lookup = calloc(hm->lookupSize*2, sizeof(entry));
    if(hm->lookup == NULL){
        hm->lookup = oldTable;
        return;
    }
    uint64_t oldSize = hm->lookupSize;
    hm->lookupSize *= 2;
    hm->totalSize = 0;
    hm->usedSize = 0;
    for(uint64_t i = 0; i < oldSize; i++) {
        if(oldTable[i].status == USED) {
            hashMapInsertOrUpdate(hm, oldTable[i].key, oldTable[i].value);
            free(oldTable[i].key);
            free(oldTable[i].value);
        }
    }
    free(oldTable);
}

void hashMapInsertOrUpdate(hashMapADT hm, tAny key, tAny value) {
    if(hm == NULL || key == NULL || value == NULL) {
        return;
    }

    bool updated = hashMapRemove(hm, key);

    // ===== CREATE NEW ENTRY =====
    entry newEntry;

    newEntry.key = malloc(hm->keySize);
    memcpy(newEntry.key, key, hm->keySize);

    newEntry.value = malloc(hm->valueSize);
    memcpy(newEntry.value, value, hm->valueSize);

    newEntry.status = USED;
    // =====

    uint64_t pos = hash(hm, key);
    bool found = false;
    if(hm->lookup[pos].status == USED) {
        for(uint64_t i = 1; i < hm->lookupSize && !found; i++) {
            uint64_t index = (pos+i) % hm->lookupSize;
            if(hm->lookup[index].status != USED) {
                pos = index;
                found = true;
            }
        }
    }
    hm->usedSize++;
    if(!updated) {
        hm->totalSize++;
    }
    hm->lookup[pos] = newEntry;
    bool mustResize = ((double)hm->usedSize/hm->lookupSize) > hm->threshold;
    if(mustResize) {
        resize(hm);
    }
}

static bool keyEquals(hashMapADT hm, tAny key1, tAny key2) {
    if(hm->keyEquals != NULL) {
        return hm->keyEquals(key1, key2);
    }
    return memcmp(key1, key2, hm->keySize) == 0;
}

bool hashMapRemove(hashMapADT hm, tAny key) {
    if(key == NULL) return false;

    uint64_t pos = hash(hm, key);
    for(uint64_t i = 0; i < hm->lookupSize; i++) {
        uint64_t index = (pos + i) % hm->lookupSize;
        entry * aux = &hm->lookup[index];
        if(aux->status == FREE) {
            return false;
        }
        if(aux->status == USED && keyEquals(hm, aux->key, key)) {
            if(hm->lookup[(index+1) % hm->lookupSize].status == FREE) {
                aux->status = FREE;
                hm->totalSize--;
            } else {
                aux->status = BRIDGE;
            }
            free(aux->key);
            free(aux->value);
            hm->usedSize--;
            return true;
        }
    }
    return false;
}

bool hashMapFind(hashMapADT hm, tAny key, tAny value) {
    if(key == NULL) return false;

    uint64_t pos = hash(hm, key);
    for(uint64_t i = 0; i < hm->lookupSize; i++) {
        uint64_t index = (pos + i) % hm->lookupSize;
        entry * aux = &hm->lookup[index];
        if(aux->status == FREE) return false;
        if(aux->status == USED && keyEquals(hm, aux->key, key)) {
            if(value != NULL) {
                memcpy(value, aux->value, hm->valueSize);
            }
            return true;
        }
    }
    return false;
}

uint64_t hashMapSize(hashMapADT hm) {
    return hm->usedSize;
}

void hashMapDestroy(hashMapADT hm) {
    for(uint64_t i = 0; i < hm->lookupSize; i++) {
        entry * aux = &hm->lookup[i];
        if(aux->status == USED) {
            free(aux->key);
            free(aux->value);
        }
    }
    free(hm->lookup);
    free(hm);
}

