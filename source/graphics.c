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

#if defined USE_OPENGL
#    include "graphics_opengl.h"

bool (*crb_gfx_initialize)(void) = crb_ogl_initialize;
bool (*crb_gfx_setup_window)(int, int, int, bool) = crb_ogl_setup_window;
bool (*crb_gfx_flip)(void) = crb_ogl_flip;
bool (*crb_gfx_bind_textures)(Texture*) = crb_ogl_bind_textures;
bool (*crb_gfx_bind_texture_by_id)(unsigned int) = crb_ogl_bind_texture_by_id;
int  (*crb_gfx_tex_from_image)(const char*) = crb_ogl_tex_from_image;
bool (*crb_gfx_destroy_window)(void) = crb_ogl_destroy_window;
bool (*crb_gfx_finish)(void) = crb_ogl_finish;

#elif defined USE_SDL

/* Not yet implemented
#    include "graphics_sdl.h"
bool (*crb_gfx_initialize)(void) = crb_sdl_initialize;
bool (*crb_gfx_setup_window)(int, int, int, bool) = crb_sdl_setup_window;
bool (*crb_gfx_flip)(void) = crb_sdl_flip;
bool (*crb_gfx_bind_textures)(Texture) = crb_sdl_bind_textures;
bool (*crb_gfx_bind_texture_by_id)(unsigned int) = crb_sdl_bind_texture_by_id;
int  (*crb_gfx_tex_from_image)(const char*) = crb_sdl_tex_from_image;
bool (*crb_gfx_destroy_window)(void) = crb_sdl_destroy_window;
bool (*crb_gfx_finish)(void) = crb_sdl_finish;
*/

#endif
