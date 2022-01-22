//
// Created by Caio Tavares on 16.01.22.
//

#ifndef C_JAM_PLAYER_H
#define C_JAM_PLAYER_H

#include <SDL.h>

const int PLAYER_TILE_SIZE;

typedef struct {
    int x, previousX;
    int y, previousY;
    int Width;
    int Height;
    int CurrentFrame;
    int NumberOfFrames;
    int VelocityX, VelocityY;
    SDL_RendererFlip Flip;
    SDL_Texture *Texture;
} Player;

#endif //C_JAM_PLAYER_H
