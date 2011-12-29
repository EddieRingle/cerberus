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

#ifndef TARGET_OS_WINDOWS
#       include <sys/stat.h>
#else
#       include <shlobj.h>
#endif

#ifdef TARGET_OS_MACOSX
extern char **NXArgv;
#endif

static int initialized;
static bool looping;
static struct scene *main_scene;
static struct scene *hud_scene;
static struct app_info appinfo;

void crb_behavior_render(Entity *_entity, Uint32 _delta)
{
    if (_entity != NULL) {
        float width = 16, height = 16;

        crb_entity_get_number_prop(_entity, "width", &width);
        crb_entity_get_number_prop(_entity, "height", &height);

        glPushMatrix();

        /* Draw entity */
        glBegin(GL_QUADS);
            glColor3f(1.0, 0.0, 0.0);
            glVertex2f(0.0, 0.0);
            glColor3f(1.0, 0.0, 1.0);
            glVertex2f(width, 0.0);
            glColor3f(0.0, 1.0, 0.0);
            glVertex2f(width, height);
            glColor3f(0.0, 0.0, 1.0);
            glVertex2f(0.0, height);
        glEnd();

        glPopMatrix();
    }
}

void _setup_app_info()
{
    char tempPath[2048];
    const char *homePath;
    char *ptr;

    memset(tempPath, 0, sizeof(tempPath));

#if defined ( TARGET_OS_WINDOWS )
    GetModuleFileName(NULL, tempPath, sizeof(tempPath)) != 0);
    if (strlen(tempPath)) {
        ptr = &tempPath[strlen(tempPath)];
        while (*(--ptr) != '\\');
        ptr[1] = '\x0';
    } else {
        strcpy(tempPath, ".\\");
    }
#elif defined ( TARGET_OS_LINUX )
    readlink("/proc/self/exe", tempPath, sizeof(tempPath) - 1);
    if (strlen(tempPath)) {
        ptr = &tempPath[strlen(tempPath)];
        while (*(--ptr) != '/');
        ptr[1] = '\x0';
    } else {
        strcpy(tempPath, "./");
    }
#elif defined ( TARGET_OS_MACOSX )
    strncpy(tempPath, NXArgv[0], sizeof(tempPath));
    if (strlen(tempPath)) {
        ptr = &tempPath[strlen(tempPath)];
        while (*(--ptr) != '/');
        ptr[1] = '\x0';
    } else {
        strcpy(tempPath, "./");
    }
#endif

    appinfo.app_path = crb_strdup(tempPath);

#if defined ( TARGET_OS_WINDOWS )
    memset(tempPath, 0, sizeof(tempPath));
    SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA | CSIDL_FLAG_CREATE, NULL, 0,
                    tempPath);
    strcat(tempPath, "\\Cerberus\\");
#elif defined ( TARGET_OS_LINUX )
    homePath = getenv("HOME");
    sprintf(tempPath, "%s/.cerberus/", homePath);
#elif defined ( TARGET_OS_MACOSX )
    homePath = getenv("HOME");
    sprintf(tempPath, "%s/Library/Application Support/Cerberus", homePath);
#endif

    appinfo.support_path = crb_strdup(tempPath);
}

void _cleanup_app_info()
{
    free(appinfo.app_path);
    appinfo.app_path = NULL;
    free(appinfo.support_path);
    appinfo.support_path = NULL;
}

bool crb_initialize(int _flags)
{
    _setup_app_info();

    printf("App dir: %s\n", appinfo.app_path);
    printf("Support dir: %s\n", appinfo.support_path);

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

    crb_scripting_init("data/scripts/lua/");
    crb_scripting_run_script("sethello");
    crb_scripting_run_script("printhello");

    /* Register default entity behaviors */
    crb_behavior_register("render", crb_behavior_render);

    main_scene = crb_scene_create();
    hud_scene = crb_scene_create();

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

    crb_scripting_finish();

    /* Unregister default entity behaviors */
    crb_behavior_unregister("render");

    crb_scene_destroy(&hud_scene);
    hud_scene = NULL;
    crb_scene_destroy(&main_scene);
    main_scene = NULL;

    _cleanup_app_info();

    return true;
}

const char *crb_get_application_path()
{
    return appinfo.app_path;
}

const char *crb_get_support_path()
{
    return appinfo.support_path;
}

void crb_debug_out(const char *_msg)
{
    printf("%s\n", _msg);
}

struct scene *crb_game_get_main_scene()
{
    return main_scene;
}

struct scene *crb_game_get_hud_scene()
{
    return hud_scene;
}

bool crb_loop()
{
    SDL_Event event;

    bool do_tick;
    bool do_frame;

    Uint32 c_ticks = SDL_GetTicks();
    Uint32 ps_ticks = c_ticks;
    Uint32 last_tick = c_ticks;
    Uint32 last_frame = c_ticks;
    Uint32 ticks_since;
    Uint32 frames = 0;
    Uint32 ticks = 0;

    looping = true;


    while(looping) {

        c_ticks = SDL_GetTicks();
        /* Control logic tick rate */
        do_tick = (c_ticks - last_tick) > (1000.0 / 60.0);

        if (c_ticks - ps_ticks > 1000.0) {
            ps_ticks = c_ticks;
            printf("FPS: %i; TPS: %i\n", frames, ticks);
            frames = 0;
            ticks = 0;
        }

        /* Control frame rate (Vsync)
           We should only do this if the user specifies, though */
        if (false) {
            do_frame = do_tick;
        } else {
            do_frame = true;
        }

        if (do_tick) {
            /* Handle input */
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_KEYUP) {
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        crb_loop_end();
                } else if (event.type == SDL_QUIT) {
                    crb_loop_end();
                }
            }

            /* Update scenes */
            crb_scene_update(main_scene, c_ticks - last_tick);
            crb_scene_update(hud_scene, c_ticks - last_tick);

            ticks++;
        }

        if (do_frame) {
            ticks_since = SDL_GetTicks() - last_frame;

            /* Render scenes */
            crb_scene_render(main_scene, ticks_since);
            crb_scene_render(hud_scene, ticks_since);

            crb_gfx_flip();

            glClear(GL_COLOR_BUFFER_BIT);

            last_frame = ticks_since + last_frame;
            frames++;
        }

        if (do_tick) {
            last_tick = c_ticks;
        }
    }

    return true;
}

void crb_loop_end()
{
    looping = false;
}
