#ifndef CAMERA_H
#define CAMERA_T

#include "player.h"

/* STRUCTS */
typedef struct{
    Position pos;
    Velocity velocity;
}Camera_t;

void camera_init(void);

#endif