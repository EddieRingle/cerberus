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

#ifndef __included_cerberus_h
#define __included_cerberus_h

#include "universal_include.h"

static unsigned int CRB_INIT_ALL = 0xFFFFFFFF;
static unsigned int CRB_INIT_GRAPHICS = 0x000000FF;
static unsigned int CRB_INIT_SOUND = 0x0000FF00;
static unsigned int CRB_INIT_INPUT = 0x00FF0000;
static unsigned int CRB_INIT_NETWORK = 0xFF000000;

struct app_info {
    char *app_path;
    char *support_path;
};

/* Core engine functions (initialize, enable/disable, and quit) */
bool    crb_initialize(int _flags);
bool    crb_enable(int _flag);
bool    crb_disable(int _flag);
bool    crb_quit();

const char *crb_get_application_path();
const char *crb_get_support_path();

/* Logging functions */
void    crb_debug_out(const char *_msg);

/* Default scene getters */
struct scene;

struct scene *crb_game_get_main_scene();
struct scene *crb_game_get_hud_scene();

/* Generic game loop */
bool    crb_loop();
void    crb_loop_end();

/* Input functions */
int     crb_handle_input();

#include "darray.h"
#include "llist.h"
#include "hashtable.h"
#include "murmurhash.h"

#include "entity.h"
#include "scene.h"
#include "graphics.h"
#include "scripting.h"

#endif /* __included_cerberus_h */
