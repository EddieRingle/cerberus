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

#include "hashtable.h"

HashTable *crb_hashtable_create(uint32_t _size) {
    HashTable *tbl = malloc(sizeof(HashTable));
    tbl->size = 0;
    tbl->capacity = 0;
    tbl->table = NULL;
    return tbl;
}

void crb_hashtable_destroy(HashTable **_tbl) {
    if (_tbl == NULL || *_tbl == NULL) return;

    if ((*_tbl)->table != NULL) {
        free((*_tbl)->table);
        (*_tbl)->table = NULL;
    }

    free(*_tbl);

    *_tbl = NULL;
}

void crb_hashtable_grow(HashTable *_tbl) {
    unsigned int oldCapacity;
    struct table_entry **oldTable;
    unsigned int i;

    if (_tbl == NULL) return;

    oldCapacity = _tbl->capacity;
    if (_tbl->capacity > 0) {
        _tbl->capacity *= 2;
    } else {
        _tbl->capacity = 32;
    }
    oldTable = _tbl->table;
    _tbl->table = calloc(_tbl->capacity, sizeof(struct table_entry*));
    for (i = 0; i < oldCapacity; i++) {
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
    struct table_entry *entry, *next_entry;
    uint32_t index;

    if (_tbl == NULL || _key == NULL || _data == NULL) return false;

    if (_tbl->size == _tbl->capacity) {
        crb_hashtable_grow(_tbl);
    }

    entry = malloc(sizeof(struct table_entry));
    if (entry == NULL) return false;
    crb_hashtable_hash(_key, &entry->hash);
    index = entry->hash % _tbl->capacity;
    entry->key = crb_strdup(_key);
    entry->value = _data;
    entry->next = NULL;

    if (_tbl->table[index] == NULL) {
        _tbl->table[index] = entry;
    } else {
        next_entry = _tbl->table[index];
        while (next_entry != NULL) {
            if (next_entry->next == NULL) {
                next_entry->next = entry;
                break;
            }
            next_entry = next_entry->next;
        }
    }
    _tbl->size++;
    return true;
}

void *crb_hashtable_find(HashTable *_tbl, const char *_key) {
    uint32_t hash;
    struct table_entry *entry;

    if (_tbl == NULL || _key == NULL) return NULL;

    crb_hashtable_hash(_key, &hash);

    for (entry = _tbl->table[hash % _tbl->capacity]; entry != NULL; entry = entry->next) {
        if (strcmp(_key, entry->key) == 0) return entry->value;
    }

    return NULL;
}

void *crb_hashtable_remove(HashTable *_tbl, const char *_key) {
    uint32_t hash;
    struct table_entry *entry, *sibling;
    bool top = true;
    void *data;

    if (_tbl == NULL || _key == NULL) return NULL;

    crb_hashtable_hash(_key, &hash);

    for (entry = _tbl->table[hash % _tbl->capacity]; entry != NULL; sibling = entry, entry = entry->next) {
        if (strcmp(_key, entry->key) == 0) {
            break;
        } else {
            top = false;
        }
    }
    if (entry != NULL) {
        data = entry->value;
        if (top) {
            if (entry->next != NULL) {
                _tbl->table[hash % _tbl->capacity] = entry->next;
            } else {
                _tbl->table[hash % _tbl->capacity] = NULL;
            }
        } else {
            if (entry->next != NULL) {
                sibling->next = entry->next;
            } else {
                sibling->next = NULL;
            }
        }
        free(entry->key);
        free(entry);
        entry = NULL;
        _tbl->size--;
        return data;
    }

    return NULL;
}
