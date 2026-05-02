#ifndef RENDERER_H
#define RENDERER_H

/* INCLUDES */
#include "SDL3/SDL_render.h"
#include "terrain.h"

/* FUNCTIONS */
int renderer_init(SDL_Window **window);
void renderer_render(SDL_Texture *texture, SDL_FRect *rect);
void refresh(ChunkMap_t *map);

#endif