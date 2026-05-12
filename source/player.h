#ifndef PLAYER_H
#define PLAYER_H

/* INCLUDES */
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"

/* DEFINES */
#define MAX_NAME_LENGTH 32
#define PLAYER_ACCELERATION 3000.0f
#define PLAYER_DECELERATION 2000.0f
#define PLAYER_WALKING_SPEED 150.0f
#define PLAYER_RUNNING_SPEED 300.0f
#define PLAYER_DASH_SPEED 2000.0f
#define DASH_COOLDOWN 200000000

/* STRUCTS */
typedef struct {
    float x;
    float y;
} Position;

typedef struct {
    float dx;
    float dy;
} Velocity;

typedef struct {
    float width;
    float height;
} Dimensions;

typedef enum {
    IDLE,
    WALKING,
    RUNNING,
    DASHING,
    ATTACKING,
    BLOCKING,
    JUMPING
} Status;

typedef struct {
    // Player info
    char name[MAX_NAME_LENGTH];

    // Player stats
    int health;
    float damage;

    // Player movement
    Position pos;
    Velocity velocity;
    Dimensions dimensions;

    // Player module
    SDL_FRect rect;
    SDL_Texture *texture;
    Status status;
} Player;

extern Player player;

/* FUNCTIONS */
void player_init(const char name[MAX_NAME_LENGTH]);

#endif