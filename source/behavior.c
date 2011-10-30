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

#include "behavior.h"

static DArray *behaviors;

int crb_behavior_register(const char *_name, behaviorFunc _func)
{
    struct behavior *b;

    if (behaviors == NULL) {
        behaviors = crb_darray_create();
    }

    if (crb_behavior_get(_name) != NULL) {
        return 0;
    } else {
        b = (struct behavior*)malloc(sizeof(struct behavior));
        return crb_darray_insert(behaviors, b) > -1;
    }
}

struct behavior *crb_behavior_get(const char *_name)
{
    struct behavior *b;
    int i;

    b = crb_darray_get(behaviors, 0);
    for (i = 1; b != NULL; b = crb_darray_get(behaviors, i++)) {
        if (!strcmp(b->name, _name)) {
            return b;
        }
    }

    return NULL;
}

int crb_behavior_unregister(const char *_name)
{
    struct behavior *b;
    int i;

    b = crb_darray_get(behaviors, 0);
    for (i = 1; b != NULL; b = crb_darray_get(behaviors, i++)) {
        if (!strcmp(b->name, _name)) {
            crb_darray_remove(behaviors, i - 1);
            free(b);
            return 1;
        }
    }

    return 0;
}
