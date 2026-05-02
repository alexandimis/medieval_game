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

/* STRUCTS */
typedef struct{
    uint8_t id;

    SDL_FRect rect;
    SDL_Texture *texture;
}Tile_t;


typedef struct{
    bool loaded;
    Position pos;

    // SDL Rendering
    SDL_Texture *texture;
    SDL_FRect rect;

    // Hash handle
    UT_hash_handle hh;
}Chunk_t;

typedef struct{
    Chunk_t *chunks;
}ChunkMap_t;


/* FUNCTIONS */
void chunk_map_destroy(ChunkMap_t *map);
Chunk_t *chunk_get(ChunkMap_t *map, Position pos);



#endif