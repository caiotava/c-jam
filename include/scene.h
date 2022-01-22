//
// Created by Caio Tavares on 16.01.22.
//

#ifndef C_JAM_SCENE_H
#define C_JAM_SCENE_H

#include <SDL.h>

typedef struct {
    int ImageId;
    float X, Y;
    float Sx, Sy;
    float CollisionWidth;
    float CollisionHeight;
} Sprite;

typedef struct {
    SDL_Texture* Tileset;
    Sprite** Sprites;
    int TileCount;
} Scene;

#endif //C_JAM_SCENE_H
