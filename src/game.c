//
// Created by Caio Tavares on 10.01.22.
//

#include "game.h"
#include "image.h"
#include "vendor/cute_headers/cute_tiled.h"

void initScene(Game*);
void prepareScene(Game* game);
void doInput(Game*);
void presentScene(Game*);
void fixPlayerPosition(Game* game);

SDL_Texture *lightTexture;
SDL_Texture *backgroundLayer;
SDL_Texture *lightLayer;
SDL_Texture *resultLayer;

const int FPS = 60;
const int DELAY_TIME = 1000.0f / FPS;

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

    //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    rendererFlags = SDL_RENDERER_PRESENTVSYNC;

    game->renderer = SDL_CreateRenderer(game->window, -1, rendererFlags);

    if (!game->renderer)
    {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_SetRenderDrawBlendMode(game->renderer, SDL_BLENDMODE_BLEND);

    Player player = {
            .y = 50,
            .x = 50,
            .Width = 14*3,
            .Height = 20*3,
            .CurrentFrame = 0,
            .NumberOfFrames = 4,
            .Flip = SDL_FLIP_NONE
    };

    player.Texture = LoadTexture("player.png", game->renderer);
    game->Player = player;

    return game;
}

void RunGame(Game* game) {
    initScene(game);

    Uint32 frameStart, frameTime;

    while (1)
    {
        frameStart = SDL_GetTicks();
        prepareScene(game);

        doInput(game);

        presentScene(game);

        frameTime = SDL_GetTicks() - frameStart;

        if (frameTime < DELAY_TIME) {
            SDL_Delay(DELAY_TIME - frameTime);
        };
    }
}

void initScene(Game* game) {
    cute_tiled_map_t* map = cute_tiled_load_map_from_file("assets/maps/map-01.json", 0);
    cute_tiled_tileset_t* tileset = map->tilesets;

    game->Scene = malloc(sizeof(Scene));
    game->Scene->Tileset = LoadTexture(tileset->image, game->renderer);

    int mapWidth = map->width;
    int mapHeight = map->height;

    game->Scene->Sprites = (Sprite*)malloc(sizeof(Sprite) * mapWidth * mapHeight);

    cute_tiled_layer_t* layer = map->layers;
    int* tileIds = layer->data;

    game->Scene->TileCount = layer->data_count;

    for (int i = 0; i < game->Scene->TileCount; ++i)
    {
        int hFlip, vFlip, dFlip;
        int globalTileID = tileIds[i];
        cute_tiled_get_flags(globalTileID, &hFlip, &vFlip, &dFlip);
        globalTileID = cute_tiled_unset_flags(globalTileID);

        // assume the map file only has one tileset
        // resolve the tile id based on the matching tileset's first gid
        // the final index can be used on the `images` global array
        int id = globalTileID - tileset->firstgid;

        Sprite *sprite = malloc(sizeof(Sprite));
        sprite->ImageId = id;
        sprite->X = (float)(i % mapWidth) * 16 * 2;
        sprite->Y = (float)mapHeight * (i / mapWidth) * 2;
        sprite->Sx = (float)(id % mapWidth) * 16;
        sprite->Sy = (float)mapHeight * (id / mapWidth);
        sprite->CollisionHeight = 0;
        sprite->CollisionWidth = 0;

        game->Scene->Sprites[i] = sprite;
    }

    for (int i = 0; i < game->Scene->TileCount; ++i) {
        Sprite *sprite = game->Scene->Sprites[i];
        cute_tiled_tile_descriptor_t* tile = tileset->tiles;

        while (tile) {
            if (tile->tile_index != sprite->ImageId) {
                tile = tile->next;
                continue;
            }

            sprite->CollisionHeight = tile->objectgroup->objects->height*2;
            sprite->CollisionWidth = tile->objectgroup->objects->width*2;

            tile = tile->next;
        }
    }

    cute_tiled_free_map(map);

    lightTexture = LoadTexture("spot.png", game->renderer);

    backgroundLayer = SDL_CreateTexture(game->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 512, 512);
    lightLayer = SDL_CreateTexture(game->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 512, 512);
    resultLayer = SDL_CreateTexture(game->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 512, 512);
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

            case SDL_KEYUP:
                switch( event.key.keysym.sym )
                {
                    case SDLK_UP:
                    case SDLK_DOWN:
                        game->Player.VelocityY = 0;;
                        break;

                    case SDLK_LEFT:
                    case SDLK_RIGHT:
                        game->Player.VelocityX = 0;
                        break;
                }

                game->Player.CurrentFrame = 0;
                break;

            default:
                break;
        }
    }

    const Uint8* keyState =  SDL_GetKeyboardState(NULL);

    if (keyState[SDL_SCANCODE_LEFT]) {
        game->Player.VelocityX = -1;
    }
    if (keyState[SDL_SCANCODE_RIGHT]) {
        game->Player.VelocityX = 1;
    }
    if(keyState[SDL_SCANCODE_UP]) {
        game->Player.VelocityY = -1;
    }
    if(keyState[SDL_SCANCODE_DOWN]) {
        game->Player.VelocityY = 1;
    }

    if (game->Player.VelocityX != 0 || game->Player.VelocityY != 0) {
        game->Player.CurrentFrame = SDL_GetTicks() / 100 % game->Player.NumberOfFrames;
        //game->Player.CurrentFrame++;
    }

    if (game->Player.CurrentFrame >= game->Player.NumberOfFrames) {
        game->Player.CurrentFrame = 0;
    }

    if (game->Player.VelocityX != 0) {
        game->Player.Flip = SDL_FLIP_HORIZONTAL & (game->Player.VelocityX > 0);
    }
}

void prepareScene(Game* game)
{
    SDL_SetRenderDrawColor( game->renderer, 0x00, 0x00, 0x00, 0x00 );
    SDL_RenderClear( game->renderer );

    SDL_Rect gameRect = {0, 0, 512, 512};
    SDL_SetRenderTarget( game->renderer, backgroundLayer );
    SDL_SetRenderDrawColor( game->renderer, 0x00, 0x00, 0x00, 0x00 );
    SDL_RenderClear( game->renderer );

    for (int i = 0; i < game->Scene->TileCount; ++i)
    {
        Sprite *sprite = game->Scene->Sprites[i];
        SDL_Rect dest;
        SDL_Rect source;

        dest.x = sprite->X;
        dest.y = sprite->Y;
        dest.w, source.w = 16;
        dest.h, source.h = 16;

        source.x = sprite->Sx;
        source.y = sprite->Sy;
        dest.h = 16 * 2;
        dest.w = 16 * 2;

        SDL_RenderCopyEx(game->renderer, game->Scene->Tileset, &source, &dest, 0, 0, SDL_FLIP_NONE);
    }

    game->Player.previousX = game->Player.x;
    game->Player.previousY = game->Player.y;
    game->Player.x += (game->Player.VelocityX * 5);
    game->Player.y += (game->Player.VelocityY * 5);

    fixPlayerPosition(game);

    SDL_Rect playerRect = {game->Player.x, game->Player.y, game->Player.Width, game->Player.Height};
    SDL_Rect srcRect = {
            game->Player.CurrentFrame * (13 + 7) +3,
            28,
            14,
            20
    };

    SDL_RenderCopyEx(game->renderer, game->Player.Texture, &srcRect, &playerRect, 0, 0, game->Player.Flip);

    SDL_SetRenderTarget(game->renderer, NULL);

    // draw light points
    SDL_SetRenderTarget(game->renderer, lightLayer);
    SDL_SetTextureBlendMode(lightLayer, SDL_BLENDMODE_MOD);
    SDL_SetRenderDrawColor(game->renderer, 0x00, 0x00, 0x00, 0x00);
    // change the black color to a more transparent one
    //SDL_SetRenderDrawColor(game->renderer, 0x36, 0x45, 0x9b, 0xff);
    // ----
    SDL_RenderClear(game->renderer);

    SDL_Rect spot1 = {
            game->Player.x -(200/2) + 30,
            game->Player.y -(200/2) + 37,
            200,
            200
    };

    SDL_SetTextureAlphaMod(lightTexture, 255);

    SDL_RenderCopy(game->renderer, lightTexture, NULL, &spot1);

    SDL_SetRenderTarget(game->renderer, NULL);

    // merge all layers
    SDL_SetRenderTarget(game->renderer, resultLayer);

    SDL_SetRenderDrawColor(game->renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(game->renderer);

    SDL_SetTextureBlendMode(resultLayer, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(game->renderer, backgroundLayer, NULL, &gameRect);

    SDL_RenderCopy(game->renderer, lightLayer, NULL, &gameRect);

    SDL_SetRenderTarget(game->renderer, NULL);
    SDL_RenderCopy(game->renderer, resultLayer, NULL, &gameRect);
}

void fixPlayerPosition(Game* game) {
    Player player = game->Player;

    for (int i = 0; i < game->Scene->TileCount; ++i) {
        Sprite *sprite = game->Scene->Sprites[i];

        if (sprite->CollisionWidth == 0 || sprite->CollisionHeight == 0) {
            continue;
        }

        int leftA, leftB;
        int rightA, rightB;
        int topA, topB;
        int bottomA, bottomB;

        leftA = player.x;
        rightA = player.x + player.Width;
        topA = player.y;
        bottomA = player.y + player.Height;

        leftB = sprite->X;
        rightB = sprite->X + sprite->CollisionWidth;
        topB = sprite->Y;
        bottomB = sprite->Y + sprite->CollisionHeight;

        if (bottomA <= topB) { continue; }
        if (topA >= bottomB) { continue; }
        if (rightA <= leftB) { continue; }
        if (leftA >= rightB) { continue; }

        if (game->Player.previousY >= topB && game->Player.previousY <= bottomB) {
            if (leftA > leftB && leftA < rightB)
                game->Player.x = rightB + 2;
            if (rightA > leftB && rightA < rightB)
                game->Player.x = leftB - player.Width - 2;
        }

        if (game->Player.previousX >= leftB && game->Player.previousX <= rightB) {
            if (topA < bottomB && topA > topB)
                game->Player.y = bottomB +1;
            if (bottomA > topB && bottomA < bottomB)
                game->Player.y = topB - player.Height - 2;
        }
    }
}

void presentScene(Game* game)
{
    SDL_RenderPresent(game->renderer);
}
