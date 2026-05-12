#include "SDL3/SDL_events.h"
#include "error.h"
#include "player.h"
#include "game.h"

#include "macros.h"
#include "camera.h"

#include <stdio.h>

/* GLOBAL VARIABLES */
uint64_t dash_cooldown = 0;
extern Camera_t camera;

/* FUNCTIONS */
void game_input(InputState_t *input) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            *input ^= QUIT;
        }
    }

    const bool *keyboard = SDL_GetKeyboardState(NULL);

    *input = keyboard[SDL_SCANCODE_ESCAPE] ? *input|QUIT  : *input;
    *input = keyboard[SDL_SCANCODE_W] ? *input|MOVE_UP    : *input&~MOVE_UP;
    *input = keyboard[SDL_SCANCODE_S] ? *input|MOVE_DOWN  : *input&~MOVE_DOWN;
    *input = keyboard[SDL_SCANCODE_A] ? *input|MOVE_LEFT  : *input&~MOVE_LEFT;
    *input = keyboard[SDL_SCANCODE_D] ? *input|MOVE_RIGHT : *input&~MOVE_RIGHT;
    *input = keyboard[SDL_SCANCODE_LSHIFT] ? *input|RUN   : *input&~RUN;

    if ((dash_cooldown == DASH_COOLDOWN) && (!keyboard[SDL_SCANCODE_SPACE])) {
        dash_cooldown = 0;
    } else if (dash_cooldown == 0) {
        *input = keyboard[SDL_SCANCODE_SPACE] ? *input|DASH : *input&~DASH;
    }
}

void game_update(InputState_t *input, const uint64_t delta_time, const uint64_t FPS) {
    // Update cooldowns
    update_cooldowns(input, delta_time);

    // Update player velocity
    update_player_velocity(*input, delta_time);
    // Update camera
    update_camera_position();

    // Update player position
    player.pos.x += player.velocity.dx * ((float)delta_time / 1000000000.0f);
    player.pos.y += player.velocity.dy * ((float)delta_time / 1000000000.0f);

    camera.pos.x += camera.velocity.dx * ((float)delta_time / 1000000000.0f);
    camera.pos.y += camera.velocity.dy * ((float)delta_time / 1000000000.0f);

    // Update player rectangle based on the camera (to maintain the player in the center)
    player.rect.x = player.pos.x - camera.pos.x + DEFAULT_WINDOW_WIDTH/2;   // Change to be the center of the screen
    player.rect.y = player.pos.y - camera.pos.y + DEFAULT_WINDOW_HEIGHT/2;  // Change to be the center of the screen
}

/* STATIC FUNCTIONS */

// Still trying to figure out a good implementation/rules
static void update_camera_position(void){
    float dx = player.pos.x - camera.pos.x;
    float dy = player.pos.y - camera.pos.y;
    float d = SDL_sqrt(dx*dx+dy*dy);
    float step;


    // snap to player position if they are very close
    if (d < 2) {
        camera.pos = (Position)player.pos;
        return;
    } else if (d < 2*PLAYER_WALKING_SPEED) {
        step = d/(10*PLAYER_WALKING_SPEED);
    } else {
        step = d/PLAYER_WALKING_SPEED;
    }
    // Currently there is no safety check for humongus 'd'

    camera.pos.x += step * dx;
    camera.pos.y += step * dy;
}

/*
Class, today we are going to learn why working with enums as input sucks
*/
static void update_player_velocity(const InputState_t input, const uint64_t delta_time)
{

    float diagonal_coefficient = 1.0f;
    float step = PLAYER_WALKING_SPEED;

    // Check for dash/run before updating dx/dy
    if (input & DASH) {
        step = PLAYER_DASH_SPEED;
        player.status = DASHING;
    }
    else if (input & RUN) {
        step = PLAYER_RUNNING_SPEED;
        player.status = RUNNING;
    }
    else if ((player.velocity.dx != 0) || (player.velocity.dy != 0)) {
        player.status = WALKING;
    }
    else {
        player.status = IDLE;
    }

    // Update dx
    if ((bool)(input & MOVE_LEFT) == (bool)(input & MOVE_RIGHT)) {
        if (player.velocity.dx == 0) {

        } else if (player.velocity.dx > 0) {
            player.velocity.dx -= PLAYER_DECELERATION * ((float)delta_time / 1000000000.0f);
            if (player.velocity.dx < 0) { player.velocity.dx = 0;}
        } else if (player.velocity.dx < 0) {
            player.velocity.dx += PLAYER_DECELERATION * ((float)delta_time / 1000000000.0f);
            if (player.velocity.dx > 0) { player.velocity.dx = 0;}
        }
    } else if (input & MOVE_LEFT) { // Only the A key is pressed
        // If the player is moving diagonally reduce speed by sqrt(2)
        if ((bool)(input & MOVE_UP) ^ (bool)(input & MOVE_DOWN)) {
            diagonal_coefficient = 1.414f;
        }

        player.velocity.dx -= PLAYER_ACCELERATION * ((float)delta_time / 1000000000.0f) / diagonal_coefficient;
        if (player.velocity.dx < (-step) / diagonal_coefficient) { player.velocity.dx = (-step) / diagonal_coefficient; }
    } else if (input & MOVE_RIGHT) { // Only the D key is pressed
        // If the player is moving diagonally reduce speed by sqrt(2)
        if ((bool)(input & MOVE_UP) ^ (bool)(input & MOVE_DOWN)) {
            diagonal_coefficient = 1.414f;
        }

        player.velocity.dx += PLAYER_ACCELERATION * ((float)delta_time / 1000000000.0f) / diagonal_coefficient;
        if (player.velocity.dx > step / diagonal_coefficient) { player.velocity.dx = step / diagonal_coefficient; }
    }

    // Update dy
    if ((bool)(input & MOVE_UP) == (bool)(input & MOVE_DOWN)) {
        if (player.velocity.dy == 0) {

        } else if (player.velocity.dy > 0) {
            player.velocity.dy -= PLAYER_DECELERATION * ((float)delta_time / 1000000000.0f);
            if (player.velocity.dy < 0) { player.velocity.dy = 0; }
        } else if (player.velocity.dy < 0) {
            player.velocity.dy += PLAYER_DECELERATION * ((float)delta_time / 1000000000.0f);
            if (player.velocity.dy > 0) { player.velocity.dy = 0; }
        }
    } else if (input & MOVE_UP) { // Only the W key is pressed
        // If the player is moving diagonally reduce speed by sqrt(2)
        if ((bool)(input & MOVE_LEFT) ^ (bool)(input & MOVE_RIGHT)) {
            diagonal_coefficient = 1.414f;
        }

        player.velocity.dy -= PLAYER_ACCELERATION * ((float)delta_time / 1000000000.0f) / diagonal_coefficient;
        if (player.velocity.dy < (-step) / diagonal_coefficient) { player.velocity.dy = (-step) / diagonal_coefficient; }
    } else if (input & MOVE_DOWN) { // Only the S key is pressed
        // If the player is moving diagonally reduce speed by sqrt(2)
        if ((bool)(input & MOVE_LEFT) ^ (bool)(input & MOVE_RIGHT)) {
            diagonal_coefficient = 1.414f;
        }

        player.velocity.dy += PLAYER_ACCELERATION * ((float)delta_time / 1000000000.0f) / diagonal_coefficient;
        if (player.velocity.dy > step / diagonal_coefficient) { player.velocity.dy = step / diagonal_coefficient; }
    }
}

static void update_cooldowns(InputState_t *input, const uint64_t delta_time) {
    // Handle player dashing
    if (*input & DASH) {
        // If dash was inactive, activate the cooldown
        if (dash_cooldown == 0) { dash_cooldown = 200000000; }
        dash_cooldown -= delta_time;

        // If dash duration expires, deactivate the dash and set the cooldown to a ghost state
        //
        // I set dash_cooldown = 200000000, because it can never have that value except when the
        // dash ends (and i set it manually). So when the user keeps space pressed, the player dashes
        // only once and waits for the release of thet space bar. After the release the dash_cooldown
        // is set to 0, and on press the cooldown starts again.
        if ((int64_t)dash_cooldown <= 0) {
            *input &= ~DASH;
            dash_cooldown = 200000000;
        }
    }
}