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
#include <string.h>

#include "darray.h"

DArray *crb_darray_create() {
    DArray *array = malloc(sizeof(DArray));
    array->size = 0;
    array->capacity = 0;
    array->highestIndex = 0;
    array->data = NULL;
    return array;
}

void crb_darray_destroy(DArray **_array) {
    if (_array != NULL && *_array != NULL) {
        if ((*_array)->data != NULL) {
            free((*_array)->data);
        }
        free(*_array);
        *_array = NULL;
    }
}

void crb_darray_grow(DArray *_array) {
    void **oldData;
    int oldCapacity = _array->capacity;

    if (_array->capacity < 1) {
        _array->capacity = 32;
    } else {
        _array->capacity *= 2;
    }

    oldData = _array->data;
    _array->data = calloc(_array->capacity, 1);
    if (oldCapacity > 0) {
        memcpy(_array->data, oldData, oldCapacity);
    }
    free(oldData);
}

int crb_darray_insert(DArray *_array, void *_data) {
    if (_array != NULL && _data != NULL) {
        if (_array->size == _array->capacity) {
            crb_darray_grow(_array);
        }
        _array->highestIndex = _array->size;
        _array->data[_array->size] = _data;
        return _array->size++;
    }
    return -1;
}

void *crb_darray_get(DArray *_array, int _index) {
    if (_array != NULL && _array->data != NULL) {
        if (_index >= 0 && _index < _array->capacity) {
            return _array->data[_index];
        }
    }
    return NULL;
}

void *crb_darray_remove(DArray *_array, int _index) {
    if (_array != NULL) {
        void *data;
        if (_index > _array->highestIndex) {
            return NULL;
        }
        data = _array->data[_index];
        if (data == NULL) {
            return NULL;
        }
        _array->data[_index] = NULL;
        _array->size--;
        return data;
    }
    return NULL;
}
