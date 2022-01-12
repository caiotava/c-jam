#include <stdio.h>
#include "game/game.h"
#include "cute_headers/cute_tiled.h"

int main() {
    GameOpt opt = {
            .title = "Game",
            .screenHeight = GAME_HEIGHT,
            .screenWidth = GAME_WIDTH,
            .windowFlags = SDL_WINDOW_SHOWN
    };

    Game* game = NewGame(opt);

    if (!game) {
        printf("error to create a new game\n");
        exit(1);
    }

    RunGame(game);

    return 0;
}
