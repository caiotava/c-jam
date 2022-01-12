//
// Created by Caio Tavares on 10.01.22.
//

#include "game.h"

void prepareScene(Game*);
void doInput(Game*);
void presentScene(Game*);

Game* NewGame(GameOpt opt)
{
    int rendererFlags;
    Game *game = malloc(sizeof(Game));

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "SDL_Init Error: %s\n",  SDL_GetError());
        return NULL;
    }

    game->window = SDL_CreateWindow(
            opt.title,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            opt.screenWidth,
            opt.screenHeight,
            opt.windowFlags
            );

    if (!game->window) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    rendererFlags = SDL_RENDERER_ACCELERATED;

    game->renderer = SDL_CreateRenderer(game->window, -1, rendererFlags);

    if (!game->renderer)
    {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        return NULL;
    }

    return game;
}

void RunGame(Game* game) {
    while (1)
    {
        prepareScene(game);

        doInput(game);

        presentScene(game);

        SDL_Delay(16);
    }
}

void doInput(Game* game)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                exit(0);
                break;

            default:
                break;
        }
    }
}

void prepareScene(Game* game)
{
    SDL_SetRenderDrawColor(game->renderer, 96, 128, 255, 255);
    SDL_RenderClear(game->renderer);
}

void presentScene(Game* game)
{
    SDL_RenderPresent(game->renderer);
}
