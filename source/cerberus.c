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

#include "cerberus.h"

static int initialized;

bool crb_initialize(int _flags)
{
    if (_flags & CRB_INIT_ALL) {
        initialized |= CRB_INIT_ALL;
    } else {
        if (_flags & CRB_INIT_GRAPHICS) {
            initialized |= CRB_INIT_GRAPHICS;
        }
        if (_flags & CRB_INIT_SOUND) {
            initialized |= CRB_INIT_SOUND;
        }
        if (_flags & CRB_INIT_INPUT) {
            initialized |= CRB_INIT_INPUT;
        }
        if (_flags & CRB_INIT_NETWORK) {
            initialized |= CRB_INIT_NETWORK;
        }
    }

    if (initialized & CRB_INIT_GRAPHICS) {
        crb_gfx_initialize();
    }
    return true;
}

bool crb_enable(int _flag)
{
    return true;
}

bool crb_disable(int _flag)
{
    return true;
}

bool crb_quit()
{
    if (initialized & CRB_INIT_GRAPHICS) {
        crb_gfx_finish();
    }

    SDL_Quit();

    return true;
}

void crb_debug_out(const char *_msg)
{
    printf("%s\n", _msg);
}
