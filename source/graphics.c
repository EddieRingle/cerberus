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

#include "graphics.h"

struct gfx_state {
    bool           windowed;
    Uint8          color_depth;
    Uint16         screen_width;
    Uint16         screen_height;

    HashTable     *texture_sources;
    GLuint         textures[64];
    SDL_Window    *window;
    SDL_GLContext  ctx;
};

static struct gfx_state *state;

bool crb_gfx_initialize()
{
    state = malloc(sizeof(struct gfx_state));

    state->texture_sources = crb_hashtable_create(-1);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialized SDL: %s\n", SDL_GetError());
        return false;
    }

    glGenTextures(64, state->textures);
    if (state->texture_sources)
        return true;
    else
        return false;
}

bool crb_gfx_setup_window(int _width, int _height, int _bpp, bool _windowed)
{
    const SDL_VideoInfo *info = NULL;
    Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
    GLenum err;

    if (_bpp < 16 || _bpp > 32) _bpp = 16;
    state->color_depth = _bpp;

    info = SDL_GetVideoInfo();

    if (info == NULL) {
        return false;
    }

    state->windowed = _windowed;

    if (!_windowed) {
        state->screen_width = _width == 0 ? info->current_w : _width;
        state->screen_height = _height == 0 ? info->current_h : _height;
    } else {
        state->screen_width = _width == 0 ? 1024 : _width;
        state->screen_height = _height == 0 ? 768 : _height;
    }

    printf("Setting up variables for OpenGL\n");
    printf("The requested color depth is %d; we're using %d.\n", _bpp,
           state->color_depth);
    printf("Setting display mode of %dx%dx%d...\n", state->screen_width,
           state->screen_height, state->color_depth);

    if (!state->windowed) flags |= SDL_WINDOW_FULLSCREEN;

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, state->color_depth / 4);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, state->color_depth / 4);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, state->color_depth / 4);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, state->color_depth / 4);

    state->window = SDL_CreateWindow("Cerberus",
                           SDL_WINDOWPOS_CENTERED,
                           SDL_WINDOWPOS_CENTERED,
                           state->screen_width, state->screen_height, flags);
    if (!state->window) {
        printf("Unable to create render window: %s\n", SDL_GetError());
        return false;
    }

    state->ctx = SDL_GL_CreateContext(state->window);
    SDL_GL_SetSwapInterval(1);

    info = SDL_GetVideoInfo();

    printf("Display mode set successfully (%dx%dx%d).\n", info->current_w,
           info->current_h, info->vfmt->BitsPerPixel);

    err = glewInit();

    if (err != GLEW_OK) {
        printf("GLEW Error: %s\n", glewGetErrorString(err));
    } else {
        printf("GLEW initialized successfully.\n");
    }

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_DITHER);
    glDisable(GL_FOG);
    glDisable(GL_LIGHTING);
    glDisable(GL_LOGIC_OP);
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_TEXTURE_1D);
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glDisable(GL_ALPHA_TEST);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, (double)state->screen_width, (double)state->screen_height, 0.0,
            -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    return true;
}

bool crb_gfx_flip()
{
    SDL_GL_SwapWindow(state->window);
    return true;
}

bool crb_gfx_destroy_window()
{
    SDL_GL_DeleteContext(state->ctx);
    SDL_DestroyWindow(state->window);
    return true;
}

bool crb_gfx_finish()
{
    if (state->window) {
        crb_gfx_destroy_window();
    }
    if (state->texture_sources) {
        crb_hashtable_destroy(&(state->texture_sources));
    }
    if (state) {
        free(state);
        state = NULL;
    }
    return true;
}
