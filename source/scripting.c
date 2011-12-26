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

#include "scripting.h"

struct scripting_state {
    const char *script_dir;
    lua_State  *luas;
};

static struct scripting_state *state;

static int push_table_instance(lua_State *L, int lo)
{
    if (lua_istable(L, lo)) {
        lua_pushstring(L, ".c_instance");
        lua_gettable(L, lo);
        if (lua_isuserdata(L, -1)) {
            lua_replace(L, lo);
            return 1;
        } else {
            lua_pop(L, 1);
            return 0;
        }
    }
    return 0;
}

void crb_scripting_register_entity(lua_State *L)
{
    int methods, metatable;

    lua_newtable(L);
    methods = lua_gettop(L);

    luaL_newmetatable(L, "Entity");
    metatable = lua_gettop(L);
}

bool crb_scripting_init(const char *_dir)
{
    state = malloc(sizeof(struct scripting_state));
    state->script_dir = crb_strdup(_dir);
    state->luas = lua_open();

    luaL_openlibs(state->luas);


    crb_scripting_register_entity(state->luas);

    return true;
}

bool crb_scripting_run_script(const char *_name)
{
    char *file;
    int lerr;
    int s;
    size_t len;
\
    len = strlen(crb_get_application_path()) + strlen(state->script_dir)
            + strlen(_name) + 5;
    file = malloc(len);

    sprintf(file, "%s%s%s%s", crb_get_application_path(), state->script_dir,
            _name, ".lua");

    lerr = luaL_loadfile(state->luas, file);
    switch (lerr)
    {
        case 0:
        s = luaL_dofile(state->luas, file);
        if (s != 0) {
            printf("\nError occurred while trying to run Lua script.\n");
            printf("\tFile: %s\n", file);
            printf("\tError: %s\n\n", lua_tostring(state->luas, -1));
            return false;
        } else {
            return true;
        }
    case LUA_ERRSYNTAX:
        printf("Lua script failed because of a syntax error.\n");
        return false;
    case LUA_ERRMEM:
        printf("Lua script failed because of a memory error.\n");
        return false;
    }

    free(file);

    return false;
}

bool crb_scripting_finish()
{
    lua_close(state->luas);
    state->luas = NULL;

    free(state->script_dir);
    state->script_dir = NULL;

    free(state);
    state = NULL;

    return true;
}
