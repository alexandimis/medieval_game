#include "macros.h"
#include "error.h"
#include "player.h"
#include "renderer.h"
#include "game.h"
#include "main.h"

#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

// Global player variable
Player player;

int main(int argc, char* argv[]) {
    /* INITIALIZATIONS */
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    if (renderer_init(&window, &renderer) != 0) { return 1; }

    // Player
    const char name[MAX_NAME_LENGTH] = "Bob"; // TODO: implement a method of getting the player name
    player_init(name);

    // Event handling variables
    SDL_Event event;
    InputState input = {false};

    // Time variables
    uint64_t current_time = 0;
    uint64_t delta_time = 1;
    uint64_t last_time = SDL_GetTicksNS();
    uint64_t last_second = SDL_GetTicksNS();
    uint64_t FPS = 0;


    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    /* MAIN LOOP */
    while (!input.quit) {
        // Calculate FPS
        FPS = 1000000000 / delta_time;
        printf("FPS: %lu\n", FPS);
        SDL_DelayPrecise(1000000000 / MAX_REFRESH_RATE);

        // Calculate delta time
        current_time = SDL_GetTicksNS();
        delta_time = current_time - last_time;
        last_time = current_time;

        // Get input information
        game_input(&input);

        // Update the game
        game_update(input, delta_time, FPS);

        // RENDER
        renderer_render(renderer, player.texture);
    }

    return 0;
}