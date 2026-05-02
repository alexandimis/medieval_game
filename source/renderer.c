#include "SDL3/SDL_init.h"
#include "SDL3/SDL_log.h"
#include "macros.h"
#include "player.h"
#include "terrain.h"
#include "renderer.h"

/* GLABAL VARIABLES */
SDL_Renderer *renderer;

#include <SDL3_image/SDL_image.h>

inline int renderer_init(SDL_Window **window) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    // Initialize SDL's variables for the renderer
    *window = SDL_CreateWindow(WINDOW_TITLE, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(*window, NULL);

    // After renderer is initialized, set the player texture
    player.texture = IMG_LoadTexture(renderer, "../../assets/images/rick_and_morty.png");

    return 0;
}

inline void renderer_render(SDL_Texture *texture, SDL_FRect *rect) {
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, NULL, rect);
    SDL_RenderPresent(renderer);
}

// Renders everything with the correct order
// Currently only the player and chunks
void refresh(ChunkMap_t *map){
    SDL_RenderClear(renderer);

    // Add background/terrain chunks
    for(int y = 0; y < 6; y++){
        for(int x = 0; x < 6; x++){
            Position pos = {x,y};
            Chunk_t *chunk = chunk_get(map, pos);

            SDL_RenderTexture(renderer, chunk->texture, NULL, &chunk->rect);
        }
    }

    // Add player
    SDL_RenderTexture(renderer, player.texture, NULL, &player.rect);

    // Render
    SDL_RenderPresent(renderer);
}