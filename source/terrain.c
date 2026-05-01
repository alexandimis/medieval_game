#include "terrain.h"

#include <uthash.h>
#include <SDL3_image/SDL_image.h>

static inline Tile_t get_tile(SDL_Renderer *renderer, Position pos) {

    Tile_t target;
    target.rect = (SDL_FRect){
        pos.x*TILE_SIZE, 
        pos.y*TILE_SIZE, 
        TILE_SIZE, 
        TILE_SIZE
    };
    // perlin noise function here:
    target.texture = (pos.x >= pos.y) ? IMG_LoadTexture(renderer, "../../assets/images/heart.png")
            : IMG_LoadTexture(renderer, "../../assets/images/frog.png");

    return target;
}

//for efficiency
Chunk_t *chunk_load(SDL_Renderer *renderer, Position cpos) {
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
            Position pos = {x,y};

            // Get tile
            Tile_t tile = get_tile(renderer, pos);

            // Add it to the chunk texture
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
Chunk_t *chunk_get(SDL_Renderer *renderer, ChunkMap_t *map, Position pos){
    Chunk_t *chunk = chunk_map_get(map, pos);
    if(!chunk){
        chunk = chunk_load(renderer, pos);
        chunk_map_add(map, chunk);
    }
    return chunk;
}