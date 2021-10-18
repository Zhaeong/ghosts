#ifndef GAME_H
#define GAME_H
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <cstring>
#include <iostream>

#ifdef EMSCRIPTEN
    #include <emscripten.h>
#endif 

#include "game_defs.h"

#include "ui.h"



using namespace std;


int StartSDL(SDL_Window **window, SDL_Renderer **renderer);

SDL_Texture* GetSDLTexture(SDL_Renderer *renderer, SDL_Window *window, string textureLocation);


void RemoveTextureWhiteSpace(SDL_Texture *texture);

AudioClip InitAudio(string filepath);

void PlayAudio(GameState *GS, AudioClip clip);

void PlayAudioFromPath(GameState *GS, string clipPath);


#endif // GAME_H
 
