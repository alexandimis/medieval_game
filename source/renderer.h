#ifndef RENDERER_H
#define RENDERER_H

/* INCLUDES */
#include "SDL3/SDL_render.h"
#include "terrain.h"

/* FUNCTIONS */
int renderer_init(SDL_Window **window, SDL_Renderer **renderer);
void renderer_render(SDL_Renderer *renderer, SDL_Texture *texture, SDL_FRect *rect);
void refresh(SDL_Renderer *renderer, ChunkMap_t *map);

#endif