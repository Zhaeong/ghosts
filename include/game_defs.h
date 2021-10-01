#ifndef GAMEDEFS_H
#define GAMEDEFS_H

#include <string>
#include <SDL.h>
#include <SDL_image.h>


using namespace std;



#define GAMEWIDTH  700 
#define GAMEHEIGHT 500


#define PI 3.14159265
#define AUDIO_FORMAT AUDIO_S16LSB

/*
A man wakes up for work


*/
const Uint32 TEXTUREFORMAT = SDL_PIXELFORMAT_RGBA8888;
const int NUM_AUDIO_CLIPS = 4;


struct AudioClip
{
    bool mActive;
    string wavPath;
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;
};

struct GameState
{
    string SceneCurrent;
    string SceneNext;

    bool darken;
    SDL_Color screenColor;
    SDL_Window *window;
    SDL_Renderer *renderer;


    //Audio
    SDL_AudioDeviceID audioDevice;

    //Used for when two sounds are played at once
    AudioClip curSound;


    AudioClip acArray[NUM_AUDIO_CLIPS];
    AudioClip chordDm;
  
    //Time controls
    Uint32 startTime;
    Uint32 curTime;

    //SDL Textures
    SDL_Texture *fontTexture;
    SDL_Texture *mainBoxTexture;

  
};


#endif // GAMEDEFS_H

