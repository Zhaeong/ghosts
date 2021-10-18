#include "main.h"


GameState GS;


//top level varable declarations for extern variables
//defined in game_defs.h
int NUM_UI_ENTITIES = 0;
Position UI_POS_Array[10];

#ifndef EMSCRIPTEN
const int FPS = 60;
//How many miliseconds per frame
const int FrameDelay = 1000 / FPS;
#endif 

//bool ShowDebug = true;
bool ShowDebug = false;

void gameloop() 
{
    Uint32 frameStart;

    frameStart = SDL_GetTicks();  

    GS.curTime = frameStart - GS.startTime;

    //The color at which the screen will be if alpha = 0 on all textures
    SDL_SetRenderDrawColor(GS.renderer, 255, 255, 255, GS.screenColor.a);

    SDL_RenderClear(GS.renderer);


    ////////////////////////////////////////////////////////////////////////
    //Main Game Code
    ////////////////////////////////////////////////////////////////////////




    ////////////////////////////////////////////////////////////////////////
    //End of main game code
    ////////////////////////////////////////////////////////////////////////

#ifndef EMSCRIPTEN
    Uint32 frameTime;
    frameTime = SDL_GetTicks() - frameStart;

    if (FrameDelay > frameTime)
    {
        SDL_Delay(FrameDelay - frameTime);
    }
#endif 
    //emscripten_cancel_main_loop();
}

//Need to use this main signature (int argv, char** args)
//Because SDL alters default program entry point resolving
//SDL calls your entry point expecting a specific signature,
//which is no longer subject to compiler interpretation
int main(int argv, char **args)
{
    //cout << "Starting Game\n";

    GS.startTime = SDL_GetTicks();
    GS.curTime = SDL_GetTicks();
    //Initiate SDL
    StartSDL(&(GS.window), &(GS.renderer));

    //Initial state variables

    GS.fontTexture = GetSDLTexture(GS.renderer, GS.window, "./res/png/mainText.png");
    RemoveTextureWhiteSpace(GS.fontTexture);

    GS.mainBoxTexture = GetSDLTexture(GS.renderer, GS.window, "./res/png/textBox.png");
    RemoveTextureWhiteSpace(GS.mainBoxTexture);

    //GS.blackTex.mAlpha = 0;


    GS.audioDevice = SDL_OpenAudioDevice(NULL, 0, &GS.chordDm.wavSpec, NULL, 0);
    if (GS.audioDevice == 0) 
    {
        printf("Failed to open audio: %s", SDL_GetError());
    } 

    SDL_PauseAudioDevice(GS.audioDevice, 0);

    //InitUIEntity();
    AddUIEntity(12,34);
    AddUIEntity(12,34);
    AddUIEntity(12,34);




#ifdef EMSCRIPTEN
    emscripten_set_main_loop(gameloop, 0, 0);
#else
    while(1)
    {
        gameloop();
    }
#endif 
    return 0;

}

