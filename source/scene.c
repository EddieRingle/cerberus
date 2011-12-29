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

#include "scene.h"

Scene *crb_scene_create()
{
    Scene *ret = (Scene *)malloc(sizeof(Scene));
    ret->showing = true;
    ret->entities = crb_llist_create(NULL, false);
    ret->overlay = NULL;

    return ret;
}

void crb_scene_add_entity(Scene *_scene, Entity *_entity)
{
    if (_scene != NULL && _entity != NULL)
        crb_llist_insert(_scene->entities, _entity);
}

void crb_scene_remove_entity(Scene *_scene, Entity *_entity)
{
    if (_scene != NULL && _entity != NULL)
        crb_llist_remove(_scene->entities,
                         crb_llist_find(_scene->entities, _entity));
}

void crb_scene_render(Scene *_scene, Uint32 _delta)
{
    int i;
    Behavior *b;
    Entity *e;

    if (_scene != NULL && _scene->showing) {
        for (i = 0; i < _scene->entities->size; i++) {
            e = crb_llist_get(_scene->entities, i);
            if (e != NULL) {
                b = crb_entity_get_behavior(e, "render");
                if (b != NULL) {
                    b->func(e, _delta);
                }
            }
        }
    }
}

void crb_scene_update(Scene *_scene, Uint32 _delta)
{
    int i;
    int j;
    Behavior *b;
    Entity *e;

    if (_scene != NULL && _scene->showing) {
        for (i = 0; i < _scene->entities->size; i++) {
            e = crb_llist_get(_scene->entities, i);
            if (e != NULL) {
                for (j = 0; j < e->behaviors->size; j++) {
                    b = crb_darray_get(e->behaviors, j);
                    if (b != NULL) {
                        b->func(e, _delta);
                    }
                }
            }
        }
    }
}

void crb_scene_destroy(Scene **_scene)
{
    if (_scene != NULL && *_scene != NULL) {
        crb_llist_destroy(&((*_scene)->entities));
        free(*_scene);
        *_scene = NULL;
    }
}
