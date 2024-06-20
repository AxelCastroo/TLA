#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include "hashMap.h"

typedef enum VarType {
    RBT_VAR,
    BST_VAR,
    AVL_VAR,
	EXP_VAR,
    INT_VAR,
    BOOL_VAR,
	CONST_VAR,
} VarType;

struct key {
    char * varname;
};

struct metadata {
    bool hasValue;
};

struct value {
    VarType type;
    struct metadata metadata;
};

void symbolTableInit();

bool symbolTableFind(struct key * key, struct value * value);

void symbolTableInsert(struct key * key, struct value * value);

void symbolTableDestroy();

#endif