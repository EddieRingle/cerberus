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

#include "llist.h"

LList *crb_llist_create(void *_data, bool _managed) {
    LList *list = malloc(sizeof(LList));
    list->size = 0;
    if (list == NULL) {
        return NULL;
    } else {
        if (_data != NULL) {
            crb_llist_insert(list, _data);
        }
        list->managed = _managed;
        return list;
    }
}

void crb_llist_destroy(LList **_list) {
    if (_list != NULL && *_list != NULL) {
        if ((*_list)->first != NULL) {
            bool managed = (*_list)->managed;
            struct l_node *node = (*_list)->first;
            while (node != NULL) {
                if (managed) {
                    free(node->data);
                }
                if (node->next != NULL) {
                    node = node->next;
                    free(node->prev);
                    node->prev = NULL;
                } else {
                    free(node);
                    node = NULL;
                }
            }
        }
        free(*_list);
        *_list = NULL;
        _list = NULL;
    }
}

struct l_node *crb_llist_getNode(LList *_list, int _index) {
    struct l_node *node = NULL, *tracking;
    int i;

    if (_list == NULL || _list->size == 0) {
        return NULL;
    }
    if (_index == _list->lastAccessedIndex) {
        node = _list->lastAccessedNode;
    } else {
        if ((_list->size - 1 - _index) <= (_list->size - 1) / 2) {
            tracking = _list->last;
            i = _list->size - 1;
            for (; i >= 0; i--) {
                if (i == _index && tracking != NULL) {
                    node = tracking;
                } else {
                    tracking = tracking->prev;
                }
            }
        } else {
            tracking = _list->first;
            i = 0;
            for (; i < _list->size; i++) {
                if (i == _index && tracking != NULL) {
                    node = tracking;
                } else if (tracking != NULL) {
                    tracking = tracking->next;
                }
            }
        }
    }
    _list->lastAccessedIndex = _index;
    _list->lastAccessedNode = node;
    return node;
}

void *crb_llist_get(LList *_list, int _index) {
    struct l_node *node = crb_llist_getNode(_list, _index);
    if (node != NULL) {
        return node->data;
    }
    return NULL;
}

int crb_llist_insert(LList *_list, void *_data) {
    struct l_node *node;

    if (_list == NULL || _data == NULL) {
        return -1;
    }
    /* Prepare a new node for insertion */
    node = malloc(sizeof(struct l_node));
    node->data = _data;
    node->prev = NULL;
    node->next = NULL;

    if (_list->size == 0) {
        /* We're inserting the very first node */
        _list->first = node;
        _list->last = node;
    } else {
        node->prev = _list->last;
        _list->last->next = node;
        _list->last = node;
    }
    _list->lastAccessedNode = node;
    _list->lastAccessedIndex = _list->size++;
    return _list->lastAccessedIndex;
}

void *crb_llist_remove(LList *_list, int _index) {
    struct l_node *node;
    void *ret;

    if (_index < 0 || _index >= _list->size) {
        return NULL;
    }

    node = crb_llist_getNode(_list, _index);
    if (node != NULL) {
        /* Quietly slip out of town */
        if (_index == 0) {
            _list->first = node->next;
        }
        if (_index == _list->size - 1) {
            _list->last = node->prev;
        }
        if (node->prev != NULL) {
            node->prev->next = node->next;
        }
        if (node->next != NULL) {
            node->next->prev = node->prev;
        }
        /* ... then get mugged */
        ret = node->data;
        free(node);
        node = NULL;
        _list->size--;
        _list->lastAccessedIndex = -1;
        _list->lastAccessedNode = NULL;
        return ret;
    }
    return NULL;
}

void crb_llist_delete(LList *_list, int _index) {
    void *data = crb_llist_remove(_list, _index);
    if (data != NULL) {
        free(data);
    }
}
