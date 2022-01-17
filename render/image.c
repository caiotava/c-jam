//
// Created by Caio Tavares on 12.01.22.
//

#include "image.h"
#include <SDL_image.h>

SDL_Texture* LoadTexture(const char* path, SDL_Renderer* renderer) {
    char str[80] = "assets/";
    strcat(str, path);

    SDL_Surface *surface = IMG_Load(str);

    if (!surface) {
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}