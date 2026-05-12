#include "macros.h"
#include "camera.h"

/* GLOBAL VARIABLES*/
Camera_t camera = {0,0,0,0};


void camera_init(void){
    camera.pos.x = DEFAULT_WINDOW_WIDTH/2;
    camera.pos.y = DEFAULT_WINDOW_HEIGHT/2;
    camera.velocity = (Velocity){0.0f, 0.0f};
}