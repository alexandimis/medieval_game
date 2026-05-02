#include "renderer.h"
#include "terrain.h"

#define STB_PERLIN_IMPLEMENTATION
#include <stb_perlin.h>
#include <uthash.h>
#include <SDL3_image/SDL_image.h>

// Extarnel variables
extern SDL_Renderer *renderer;

static inline Tile_t get_tile(Position pos, Position cpos) {

    Tile_t target;
    target.rect = (SDL_FRect){
        pos.x*TILE_SIZE, 
        pos.y*TILE_SIZE, 
        TILE_SIZE, 
        TILE_SIZE
    };
    target.texture = SDL_CreateTexture(
        renderer, 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_STATIC, 
        TILE_SIZE, 
        TILE_SIZE
    );

    float scale = 0.08f;
    float noise = stb_perlin_noise3(
        (cpos.x * CHUNK_SIZE + pos.x) * scale, 
        (cpos.y * CHUNK_SIZE + pos.y) * scale, 
        0.0f, 
        0, 
        0, 
        0
    );
    float normalized = noise * 0.5f + 0.5f;
    if(normalized < 0) normalized = 0;
    if(normalized > 1) normalized = 1;
    uint8_t gray = normalized * 255;

    uint32_t color =
        ((uint32_t)gray << 24) |
        ((uint32_t)gray << 16) |
        ((uint32_t)gray << 8)  |
        255;

    SDL_SetRenderDrawColor(renderer, gray, gray, gray, 255);
    SDL_RenderFillRect(renderer, &target.rect);
    // SDL_SetTextureScaleMode(target.texture, SDL_SCALEMODE_NEAREST);

    return target;
}

Chunk_t *chunk_load(Position cpos) {
    SDL_Texture *texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        CHUNK_SIZE*TILE_SIZE,
        CHUNK_SIZE*TILE_SIZE
    );

    SDL_SetRenderTarget(renderer, texture);

    // For every tile in the chunk
    for(int y = 0; y < CHUNK_SIZE; y++){
        for(int x = 0; x < CHUNK_SIZE; x++){ 
            Position pos = {x, y};
            Tile_t tile = get_tile(pos, cpos);

            // Add tile to the chunk texture
            SDL_RenderTexture(renderer, tile.texture, NULL, &tile.rect);
        }
    }

    SDL_SetRenderTarget(renderer, NULL);

    // Build chunk
    Chunk_t *chunk = SDL_malloc(sizeof(Chunk_t));
    chunk->loaded = true;
    chunk->pos = (Position)cpos;
    chunk->texture = texture;
    chunk->rect = (SDL_FRect){
        cpos.x * CHUNK_SIZE * TILE_SIZE,
        cpos.y * CHUNK_SIZE * TILE_SIZE,
        CHUNK_SIZE * TILE_SIZE,
        CHUNK_SIZE * TILE_SIZE
    };

    return chunk;
}

Chunk_t *chunk_map_get(ChunkMap_t *map, Position pos){
    Chunk_t *target = NULL;
    HASH_FIND(hh, map->chunks, &pos, sizeof(pos), target);
    return target;
}

void chunk_map_add(ChunkMap_t *map, Chunk_t *chunk){
    // If the chunk already exists don't do anything
    if(chunk_map_get(map, chunk->pos)){
        return;
    }

    HASH_ADD(hh, map->chunks, pos, sizeof(Position), chunk);
}

void chunk_map_destroy(ChunkMap_t *map){
    Chunk_t *chunk;
    Chunk_t *tmp;

    HASH_ITER(hh, map->chunks, chunk, tmp) {
        HASH_DEL(map->chunks, chunk);

        if (chunk->texture) {
            SDL_DestroyTexture(chunk->texture);
        }

        free(chunk);
    }

    map->chunks = NULL;
}

// General purpose function
// Returns a chunk with given coordinates
// If the chunk is not found it generates/loads it, adds it in the map and returns it
Chunk_t *chunk_get(ChunkMap_t *map, Position pos){
    Chunk_t *chunk = chunk_map_get(map, pos);
    if(!chunk){
        chunk = chunk_load(pos);
        chunk_map_add(map, chunk);
    }
    return chunk;
}