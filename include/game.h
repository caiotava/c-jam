//
// Created by Caio Tavares on 10.01.22.
//

#ifndef C_JAM_GAME_H
#define C_JAM_GAME_H

#include <SDL.h>
#include "scene.h"
#include "player.h"

#define GAME_WIDTH   512
#define GAME_HEIGHT  512

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    Scene *Scene;
    Player Player;
    int Running;
    int FinishGame;
} Game;

typedef struct {
    const char* title;
    int screenHeight;
    int screenWidth;
    int windowFlags;
} GameOpt;

Game* NewGame(GameOpt);
void RunGame(Game*);

#endif //C_JAM_GAME_H
