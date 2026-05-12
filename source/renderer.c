#include "SDL3/SDL_init.h"
#include "SDL3/SDL_log.h"
#include "macros.h"
#include "player.h"
#include "terrain.h"
#include "renderer.h"

#include "camera.h"

/* GLABAL VARIABLES */
SDL_Renderer *renderer;
extern Camera_t camera;

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
    
    int base_x = (int)SDL_floor(camera.pos.x / CHUNK_PIXELS);
    int base_y = (int)SDL_floor(camera.pos.y / CHUNK_PIXELS);

    float cam_off_x = camera.pos.x - base_x * CHUNK_PIXELS;
    float cam_off_y = camera.pos.y - base_y * CHUNK_PIXELS;

    // Add background/terrain chunks
    Position pos;
    // Add chunk padding 
    for(int y = 0; y < SDL_ceil(DEFAULT_WINDOW_HEIGHT/CHUNK_PIXELS)+3; y++){
        for(int x = 0; x < SDL_ceil(DEFAULT_WINDOW_WIDTH/CHUNK_PIXELS)+1; x++){
            pos.x = base_x + x;
            pos.y = base_y + y;

            Chunk_t *chunk = chunk_get(map, pos);

            chunk->rect.x = x*CHUNK_PIXELS - cam_off_x;
            chunk->rect.y = y*CHUNK_PIXELS - cam_off_y;

            SDL_RenderTexture(renderer, chunk->texture, NULL, &chunk->rect);
        }
    }

    // Add player
    SDL_RenderTexture(renderer, player.texture, NULL, &player.rect);

    // Render
    SDL_RenderPresent(renderer);
}