#pragma once

#include <iostream>

#include <SDL2/SDL.h>
#ifdef __SWITCH__
#include <SDL2/SDL2_gfxPrimitives.h>
#endif
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "main.h"
#include "console.h"

static SDL_Window *_window;
static SDL_Renderer *_renderer;

extern Console console;

namespace sdl_helper {
    void init() {
        SDL_Init(SDL_INIT_EVERYTHING);

        #ifdef __SWITCH__
        SDL_CreateWindowAndRenderer(1280, 720, 0, &_window, &_renderer);
        #else
        _window = SDL_CreateWindow(
            "NXTBrowser",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            1280,
            720,
            SDL_WINDOW_ALLOW_HIGHDPI
        );
        SDL_SetWindowResizable(_window, SDL_TRUE);
        _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
        //SDL_RenderSetLogicalSize(_renderer, 1920, 1080);
        //SDL_RenderSetIntegerScale(_renderer, SDL_TRUE);
        //SDL_RenderSetScale(_renderer, 2, 2); //TODO: res based
        #endif
        SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
        SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
        IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_WEBP | IMG_INIT_TIF);
        TTF_Init();
    }

    void exit() {
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(_window);
        
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }

    void drawTexture(SDL_Surface *_surface, std::string path, int x, int y) {
        SDL_Surface *surface = IMG_Load(path.c_str());
        SDL_Rect position;

        SDL_Rect src = {0, 0, surface->w, surface->h};
        SDL_Rect dst = {x, y, surface->w, surface->h};

        SDL_BlitSurface(surface, NULL, _surface, NULL);
        SDL_FreeSurface(surface);
    }

    SDL_Rect renderText (std::string text, SDL_Surface *_surface, SDL_Rect pos, int width,
                        TTF_Font *font, SDL_Color color) {
        if(text.empty() || font == NULL) {
            console.printf("SDL->Failed to render text");
            return {0, 0, 0, 0};
        }

        SDL_Surface *surface = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), color, width);

        SDL_Rect src = {0, 0, surface->w, surface->h};
        SDL_Rect dst = {pos.x, pos.y, surface->w, surface->h};
        
        SDL_BlitSurface(surface, &src, _surface, &dst);
        SDL_FreeSurface(surface);
        return src;
    }

    void renderBackground (SDL_Surface *_surface, SDL_Rect pos, SDL_Color color) {
        SDL_FillRect(_surface, &pos, SDL_MapRGBA(_surface->format, color.r, color.g, color.b, color.a));
    }

    // deprecated, remove asap
    void drawRect(SDL_Surface *_surface, int x, int y, int w, int h, int r, int b, int g, int a) {
        SDL_Rect pos;
        pos.x = x;
        pos.y = y;
        pos.h = h;
        pos.w = w;
        SDL_FillRect(_surface, &pos, SDL_MapRGBA(_surface->format, r, g, b, a));
    }
}