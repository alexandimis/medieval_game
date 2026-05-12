#ifndef GAME_H
#define GAME_H

/* INCLUDES */
#include <stdbool.h>

/* STRUCTS */

typedef enum{
    MOVE_UP     = 1<<0,
    MOVE_DOWN   = 1<<1,
    MOVE_LEFT   = 1<<2,
    MOVE_RIGHT  = 1<<3,
    RUN         = 1<<4,
    DASH        = 1<<5,
    QUIT        = 1<<6,
}InputState_t;

/* FUNCTIONS */
void game_input(InputState_t *input);
void game_update(InputState_t *input, uint64_t delta_time, uint64_t FPS);

/* STATIC FUNCTIONS */
static void update_player_velocity(InputState_t input, uint64_t delta_time);
static void update_cooldowns(InputState_t *input, uint64_t delta_time);
static void update_camera_position(void);
#endif