/*
 * Copyright (c) 2011 Eddie Ringle <eddie@eringle.net>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "universal_include.h"
#include "hashtable.h"
#include "murmurhash.h"

HashTable *crb_hashtable_create(uint32_t _size) {
    if (_size < 1) return NULL;

    HashTable *tbl;
    if ((tbl = malloc(sizeof(HashTable))) == NULL) {
        return NULL;
    }

    if ((tbl->table = malloc(sizeof(struct table_entry*) * _size)) == NULL) {
        return NULL;
    }

    int i = 0;
    for (; i < _size; i++) {
        tbl->table[i] = NULL;
    }

    tbl->size = _size;
    tbl->capacity = _size;

    return tbl;
}

void crb_hashtable_destroy(HashTable *_tbl) {
    if (_tbl == NULL) return;

    if (_tbl->table != NULL) {
        free(_tbl->table);
        _tbl->table = NULL;
    }

    free(_tbl);

    _tbl = NULL;
}

void crb_hashtable_grow(HashTable *_tbl) {
    if (_tbl == NULL) return;

    unsigned int oldSize = _tbl->capacity;
    _tbl->capacity *= 2;
    struct table_entry **oldTable = _tbl->table;
    memset(_tbl->table, 0, sizeof(struct table_entry *) * _tbl->capacity);
    unsigned int i = 0;
    for (; i < oldSize; i++) {
        if (oldTable[i] != NULL) {
            crb_hashtable_insert(_tbl, oldTable[i]->key, oldTable[i]->value);
            free(oldTable[i]);
        }
    }
    free(oldTable);
}

void crb_hashtable_hash(const char *_key, uint32_t *hash) {
    murmurhash3_x86_32(_key, strlen(_key), 0, hash);
}

bool crb_hashtable_insert(HashTable *_tbl, const char *_key, void *_data) {
    if (_tbl == NULL || _key == NULL || _data == NULL) return false;

    if (_tbl->size == _tbl->capacity) {
        crb_hashtable_grow(_tbl);
    }
    struct table_entry *entry = malloc(sizeof(struct table_entry));
    if (entry == NULL) return false;
    crb_hashtable_hash(_key, &entry->hash);
    entry->key = cc_strdup(_key);
    entry->value = _data;
    entry->next = NULL;

    if (_tbl->table[entry->hash] == NULL) {
        _tbl->table[entry->hash] = entry;
    } else {
        struct table_entry *next_entry = _tbl->table[entry->hash]->next;
        while (next_entry != NULL) {
            next_entry = next_entry->next;
        }
        next_entry->next = entry;
    }
    _tbl->size++;
    return true;
}

void *crb_hashtable_find(HashTable *_tbl, const char *_key) {
    if (_tbl == NULL || _key == NULL) return NULL;

    uint32_t hash;
    crb_hashtable_hash(_key, &hash);

    struct table_entry *entry;
    for (entry = _tbl->table[hash]; entry != NULL; entry = entry->next) {
        if (strcmp(_key, entry->key) == 0) return entry;
    }

    return NULL;
}

void *crb_hashtable_remove(HashTable *_tbl, const char *_key) {
    if (_tbl == NULL || _key == NULL) return NULL;

    uint32_t hash;
    crb_hashtable_hash(_key, &hash);

    struct table_entry *entry, *sibling;
    bool top = true;
    for (entry = _tbl->table[hash]; entry != NULL; sibling = entry, entry = entry->next) {
        if (strcmp(_key, entry->key)) {
            break;
        } else {
            top = false;
        }
    }
    if (entry != NULL) {
        void *data = entry->value;
        if (top) {
            _tbl->table[hash] = NULL;
        } else {
            if (entry->next != NULL) {
                sibling->next = entry->next;
            } else {
                sibling->next = NULL;
            }
        }
        free(entry);
        entry = NULL;
        _tbl->size--;
        return data;
    }
}
