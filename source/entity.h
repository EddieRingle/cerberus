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

#include "vertex.h"

struct entity {
    Uint32         id;
    char          *name;

    DArray        *behaviors;
    HashTable     *properties;

    Vertex        *vertices;
    Uint32         vertexCount;

    struct entity *parent;
    LList         *children;
};

typedef struct entity Entity;

enum {
    TYPE_NONE,
    TYPE_STRING,
    TYPE_NUMBER,
    TYPE_BOOLEAN
};

struct property_t {
    int   type;
    void *value;
};

typedef struct property_t Property;

typedef void(*behaviorFunc)(Entity*,Uint32);

struct behavior_t {
    behaviorFunc  func;
    char         *name;
};

typedef struct behavior_t Behavior;

Entity   *crb_entity_create(Uint32 _id, const char *_name);
Property *crb_entity_get_prop(Entity *_entity, const char *_name);
bool      crb_entity_set_prop(Entity *_entity, const char *_name, int _type,
                              void *_value);
bool      crb_entity_remove_prop(Entity *_entity, const char *_name);
int       crb_entity_prop_type(Entity *_entity, const char *_name);
int       crb_entity_get_string_prop(Entity *_entity, const char *_name,
                                     const char **_val);
int       crb_entity_get_number_prop(Entity *_entity, const char *_name,
                                     float *_val);
int       crb_entity_get_boolean_prop(Entity *_entity, const char *_name,
                                      bool *_val);
bool      crb_entity_has_behavior(Entity *_entity, const char *_name);
Behavior *crb_entity_get_behavior(Entity *_entity, const char *_name);
bool      crb_entity_add_behavior(Entity *_entity, const char *_name,
                                  behaviorFunc _func);
bool      crb_entity_remove_behavior(Entity *_entity, const char *_name);
bool      crb_entity_destroy(Entity **_entity);

#include "behavior.h"

#endif /* __included_entity_h */
