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

#include "entity.h"

struct entity *crb_entity_create(unsigned int _id, const char *_name)
{
    struct entity *e = (struct entity*)malloc(sizeof(struct entity*));

    if (e != NULL) {
        e->id = _id;
        e->name = crb_strdup(_name);
        e->behaviors = crb_darray_create();
        e->properties = crb_hashtable_create(32);
        e->parent = NULL;
        e->children = crb_llist_create(NULL, false);
    }

    return e;
}

struct property *crb_entity_get_prop(struct entity *_entity, const char *_name)
{
    if (_entity != NULL && _name != NULL) {
        return crb_hashtable_find(_entity->properties, _name);
    }
    return NULL;
}

bool crb_entity_set_prop(struct entity *_entity, const char *_name,
                         int _type, void *_value)
{
    if (_entity != NULL && _name != NULL) {
        struct property *prop = crb_hashtable_find(_entity->properties, _name);
        bool overwritten = (prop != NULL);
        bool sametype = overwritten;
        if (!overwritten) {
            prop = (struct property*)malloc(sizeof(struct property));
        } else if (prop->type != _type) {
            sametype = false;
            free(prop->value);
            prop->value = NULL;
        }
        prop->type = _type;
        switch (_type) {
        case TYPE_STRING: {
            if (!(overwritten && sametype)) {
                prop->value = crb_strdup((const char *)_value);
            } else {
                memmove(prop->value, _value, sizeof((const char *)_value));
            }
            break;
        }
        case TYPE_NUMBER: {
            if (!(overwritten && sametype)) {
                prop->value = malloc(sizeof(float));
            }
            memmove(prop->value, _value, sizeof(float));
            break;
        }
        case TYPE_BOOLEAN: {
            if (!(overwritten && sametype)) {
                prop->value = malloc(sizeof(bool));
            }
            memmove(prop->value, _value, sizeof(bool));
            break;
        }
        case TYPE_NONE: {
            break;
        }
        }
        if (overwritten) {
            return false;
        } else {
            return crb_hashtable_insert(_entity->properties, _name, prop);
        }
    }
    return false;
}

bool crb_entity_remove_prop(struct entity *_entity, const char *_name)
{
    if (_entity != NULL && _name != NULL) {
        struct property *prop = crb_hashtable_remove(_entity->properties,
                                                     _name);
        if (prop == NULL) {
            return false;
        } else {
            free(prop->value);
            prop->value = NULL;
            free(prop);
            prop = NULL;
            return true;
        }
    }
    return false;
}

int crb_entity_prop_type(struct entity *_entity, const char *_name)
{
    struct property *prop = crb_entity_get_prop(_entity, _name);
    if (prop != NULL) {
        return prop->type;
    } else {
        return -1;
    }
}

int crb_entity_get_string_prop(struct entity *_entity, const char *_name,
                               const char **_val)
{
    if (_entity != NULL && _name != NULL && _val != NULL) {
        struct property *prop = crb_hashtable_find(_entity->properties, _name);
        if (prop != NULL && prop->type == TYPE_STRING) {
            *_val = (const char *)prop->value;
            return 1;
        }
    }
    return 0;
}

int crb_entity_get_number_prop(struct entity *_entity, const char *_name,
                               float *_val)
{
    if (_entity != NULL && _name != NULL && _val != NULL) {
        struct property *prop = crb_hashtable_find(_entity->properties, _name);
        if (prop != NULL && prop->type == TYPE_NUMBER) {
            memmove(_val, prop->value, sizeof(float));
            return 1;
        }
    }
    return 0;
}

int crb_entity_get_boolean_prop(struct entity *_entity, const char *_name,
                                bool *_val)
{
    if (_entity != NULL && _name != NULL && _val != NULL) {
        struct property *prop = crb_hashtable_find(_entity->properties, _name);
        if (prop != NULL && prop->type == TYPE_BOOLEAN) {
            memmove(_val, prop->value, sizeof(bool));
            return 1;
        }
    }
    return 0;
}

bool crb_entity_has_behavior(struct entity *_entity, const char *_name)
{
    if (_entity != NULL && _name != NULL) {
        int i;
        struct behavior *b = crb_darray_get(_entity->behaviors, 0);
        for (i = 1; b != NULL; b = crb_darray_get(_entity->behaviors, i++)) {
            if (!strcmp(b->name, _name)) {
                return true;
            }
        }
    }
    return false;
}

struct behavior *crb_entity_get_behavior(struct entity *_entity,
                                         const char *_name)
{
    if (_entity != NULL && _name != NULL) {
        int i;
        struct behavior *b = crb_darray_get(_entity->behaviors, 0);
        for (i = 1; b != NULL; b = crb_darray_get(_entity->behaviors, i++)) {
            if (!strcmp(b->name, _name)) {
                return b;
            }
        }
    }
    return NULL;
}

bool crb_entity_add_behavior(struct entity *_entity, const char *_name,
                             behaviorFunc _func)
{
    if (_entity != NULL && _name != NULL) {
        behaviorFunc f = (_func == NULL) ? crb_behavior_get(_name)->func
                                         : _func;
        struct behavior *b;
        if (f != NULL) {
            b = malloc(sizeof(struct behavior));
            b->name = crb_strdup(_name);
            b->func = _func;
            return crb_entity_has_behavior(_entity, _name)
                    ^ (crb_darray_insert(_entity->behaviors, b) > -1);
        }
    }
    return false;
}

bool crb_entity_remove_behavior(struct entity *_entity, const char *_name)
{
    if (_entity != NULL && _name != NULL) {
        int i;
        struct behavior *b = crb_darray_get(_entity->behaviors, 0);
        for (i = 1; b != NULL; b = crb_darray_get(_entity->behaviors, i++)) {
            if (!strcmp(b->name, _name)) {
                free((void*)b->name);
                b->name = NULL;
                b->func = NULL;
                crb_darray_remove(_entity->behaviors, i - 1);
                return true;
            }
        }
    }
    return false;
}

bool crb_entity_destroy(struct entity **_entity)
{
    struct behavior *b;
    struct property *p;
    struct table_entry *r;
    int i;
    int s;

    free((void*)(*_entity)->name);
    (*_entity)->name = NULL;
    for (i = 0; i < (*_entity)->behaviors->capacity; i++) {
        b = crb_darray_remove((*_entity)->behaviors, i);
        if (b != NULL) {
            free((void*)b->name);
            free(b);
            b = NULL;
        }
    }
    free((*_entity)->behaviors);
    s = (*_entity)->properties->capacity;
    for (i = 0; i < s; i++) {
        while ((r = (*_entity)->properties->table[i]) != NULL) {
            p = crb_hashtable_remove((*_entity)->properties, r->key);
            if (p->type != TYPE_NONE) {
                free(p->value);
            }
            free(p);
            p = NULL;
        }
    }
    free((*_entity)->properties);
    for (i = 0; i < (*_entity)->children->size; i++) {
        crb_llist_remove((*_entity)->children, i);
    }
    free((*_entity)->children);
    free(*_entity);

    *_entity = NULL;

    return true;
}
