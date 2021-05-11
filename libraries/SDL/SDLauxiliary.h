#pragma once

#include <algorithm>
#include <iostream>
#include <stdint.h>

#include "glm/vec3.hpp"
#include "SDL2/SDL.h"

namespace bv {
typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    int height;
    int width;
    uint32_t *buffer;
} screen;

screen *InitializeSDL(int width, int height, bool fullscreen = false);

void PutPixelSDL(screen *s, int x, int y, glm::vec3 color);

void SDL_Renderframe(screen *s);

void KillSDL(screen *s);

void SDL_SaveImage(screen *s, const char *filename);

}