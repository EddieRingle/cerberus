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

#ifndef __included_entity_h
#define __included_entity_h

#include "universal_include.h"

#include "llist.h"
#include "darray.h"
#include "hashtable.h"

enum {
    TYPE_NONE,
    TYPE_STRING,
    TYPE_NUMBER,
    TYPE_BOOLEAN
};

struct property {
    int   type;
    void *value;
};

struct entity;

typedef void(*behaviorFunc)(struct entity*,Uint32);

struct behavior {
    behaviorFunc  func;
    const char   *name;
};

struct entity { // tolua_export
    int            id; // tolua_export
    const char    *name; // tolua_export

    DArray        *behaviors;
    HashTable     *properties;

    struct entity *parent;
    LList         *children;
}; // tolua_export

struct entity   *crb_entity_create(unsigned int _id, const char *_name);
struct property *crb_entity_get_prop(struct entity *_entity, const char *_name);
bool             crb_entity_set_prop(struct entity *_entity, const char *_name,
                                     int _type, void *_value);
bool             crb_entity_remove_prop(struct entity *_entity,
                                        const char *_name);
int              crb_entity_prop_type(struct entity *_entity,
                                      const char *_name);
int              crb_entity_get_string_prop(struct entity *_entity,
                                            const char *_name,
                                            const char **_val);
int              crb_entity_get_number_prop(struct entity *_entity,
                                            const char *_name, float *_val);
int              crb_entity_get_boolean_prop(struct entity *_entity,
                                             const char *_name, bool *_val);
bool             crb_entity_has_behavior(struct entity *_entity,
                                         const char *_name);
struct behavior *crb_entity_get_behavior(struct entity *_entity,
                                         const char *_name);
bool             crb_entity_add_behavior(struct entity *_entity,
                                         const char *_name, behaviorFunc _func);
bool             crb_entity_remove_behavior(struct entity *_entity,
                                            const char *_name);
bool             crb_entity_destroy(struct entity **_entity);

#include "behavior.h"

#endif /* __included_entity_h */
