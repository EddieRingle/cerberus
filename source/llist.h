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

#ifndef __included_llist_h
#define __included_llist_h

#include "universal_include.h"

struct l_node {
    void          *data;
    struct l_node *next;
    struct l_node *prev;
};

typedef struct {
    struct l_node *first;
    struct l_node *last;

    struct l_node *lastAccessedNode;
    int            lastAccessedIndex;

    int            size;
    bool           managed;
} l_list;

typedef l_list LList;

/**
 * Creates a new LList instance
 * Also specify the data of the first node (_data) and whether or not
 * the LList will manage memory itself (_managed)
 */
LList *crb_llist_create(void *_data, bool _managed);
/**
 * Destroys a LList, also freeing memory of its nodes' data if the managed
 * flag is set
 */
void   crb_llist_destroy(LList **_list);

/**
 * Find index by data
 */
int    crb_llist_find(LList *_list, void *_data);

/**
 * Get data from an index in the specified LList
 */
void  *crb_llist_get(LList *_list, int _index);
/**
 * Insert data at the end of the specified LList
 *
 * Returns the index of the newly inserted data
 */
int  crb_llist_insert(LList *_list, void *_data);
/**
 * Removes data from the LList at the specified index
 *
 * Returns the removed data
 */
void  *crb_llist_remove(LList *_list, int _index);
/**
 * Removes data from the LList at the specified index, also deallocating
 * the memory used by that data (data will be set to NULL)
 */
void   crb_llist_delete(LList *_list, int _index);

#endif /* __included_llist_h */
