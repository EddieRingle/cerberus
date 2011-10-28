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

#ifndef __included_hashtable_h
#define __included_hashtable_h

#include <stdlib.h>
#include <stdbool.h>

#include "universal_include.h"
#include "murmurhash.h"

struct table_entry {
    const char         *key;
    void               *value;
    uint32_t            hash;
    struct table_entry *next;
};

struct hash_table {
    uint32_t       capacity;
    uint32_t       size;
    struct table_entry **table;
};

typedef struct hash_table HashTable;

HashTable *crb_hashtable_create(uint32_t _size);
void       crb_hashtable_destroy(HashTable *_tbl);

void       crb_hashtable_grow(HashTable *_tbl);
void       crb_hashtable_hash(const char *_key, uint32_t *hash);
bool       crb_hashtable_insert(HashTable *_tbl, const char *_key, void *_data);
void      *crb_hashtable_find(HashTable *_tbl, const char *_key);
void      *crb_hashtable_remove(HashTable *_tbl, const char *_key);

#endif /* __included_hashtable_h */
