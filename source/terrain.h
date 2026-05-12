#ifndef TERRAIN_H
#define TERRAIN_H

/* INCLUDES */
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "player.h"
#include <uthash.h>


/* DEFINES */
#define CHUNK_SIZE 16
#define TILE_SIZE 16
#define CHUNK_PIXELS (CHUNK_SIZE * TILE_SIZE)

/* STRUCTS */
typedef struct{
    uint8_t id;

    SDL_FRect rect;
    SDL_Texture *texture;
}Tile_t;

// Every chunk loads one texture instead of CHUNK_SIZE-by-CHUNK_SIZE tiles
typedef struct{
    bool loaded;

    // SDL Rendering
    SDL_Texture *texture;
    SDL_FRect rect;

    // Hash handle
    UT_hash_handle hh;
    Position pos;       // It's also used as the hash key
}Chunk_t;

// All loaded chunks are stored in a hash (ChunkMap_t)
typedef struct{
    Chunk_t *chunks;
}ChunkMap_t;


/* FUNCTIONS */
void chunk_map_destroy(ChunkMap_t *map);
Chunk_t *chunk_get(ChunkMap_t *map, Position pos);



#endif