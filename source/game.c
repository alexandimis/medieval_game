#include "SDL3/SDL_events.h"
#include "error.h"
#include "player.h"
#include "game.h"

#include <stdio.h>

static const float gravity = 9.81;

void game_input(InputState *input) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            input->quit = true;
        }
    }

    const bool *keyboard = SDL_GetKeyboardState(NULL);

    input->move_up = keyboard[SDL_SCANCODE_W];
    input->move_down = keyboard[SDL_SCANCODE_S];
    input->move_left = keyboard[SDL_SCANCODE_A];
    input->move_right = keyboard[SDL_SCANCODE_D];
}

void game_update(const InputState input, const float delta_time) {
    // Update player velocity
    update_player_velocity(input);    

    // Update player position
    player.pos.x += player.velocity.dx * delta_time;
    player.pos.y += player.velocity.dy * delta_time;

    // Update player rectangle
    player.rect.x = player.pos.x;
    player.rect.y = player.pos.y;
}

/* STATIC FUNCTIONS */
static void update_player_velocity(const InputState input)
{
    // Set inactivity speed to 0
    player.speed = 0;

    if ((input.move_up == true) && (input.move_right == true)) {
        player.speed += 1.0f / 1.414f;
    } else if ((input.move_up == true) && (input.move_left == true)) {
        player.speed += 1.0f / 1.414f;
    } else if ((input.move_down == true) && (input.move_right == true)) {
        player.speed += 1.0f / 1.414f;
    } else if ((input.move_down == true) && (input.move_left == true)) {
        player.speed += 1.0f / 1.414f;
    }
    else {
        player.speed += 1.0f;
    }

    // Update player dy velocity
    if ((input.move_up == true) && (input.move_down == true)) {
        player.velocity.dy = 0.0f;
    } else {
        if (input.move_up == true) {
            player.velocity.dy = -200.0f * player.speed; // Y position coordinates are inverted
        } else if (input.move_down == true) {
            player.velocity.dy = 200.0f * player.speed; // Y position coordinates are inverted
        } else {
            player.velocity.dy = 0.0f;
        }
    }

    player.velocity.dy -= gravity;

    // Update player dx velocity
    if ((input.move_left == true) && (input.move_right == true)) {
        player.velocity.dx = 0.0f;
    } else {
        if (input.move_left == true) {
            player.velocity.dx = -200.0f * player.speed;
        } else if (input.move_right == true) {
            player.velocity.dx = 200.0f * player.speed;
        } else {
            player.velocity.dx = 0.0f;
        }
    }
}